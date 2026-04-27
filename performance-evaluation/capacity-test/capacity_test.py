import argparse
import asyncio
import csv
import logging
import os
import shutil
import tempfile
from pathlib import Path

import asyncssh
import bs4 as bs
import numpy as np
import requests
from typing_extensions import Tuple

VMSTAT_CMD = "vmstat {} {} > /tmp/vmstat.csv"
CLEAN_CMD = "cat {} | sed 's/^[ \\t]*//' | sed -r 's/\s+/,/g' | awk '((/^ *procs/) || (/^ *r/ && NR!=2)) {{next}}1' > {}"
JMETER_FIELDNAMES_OUT = ["ctt", "throughput", "response_time"]
VMSTAT_FIELDNAMES = (
    "r,b,swpd,free,buff,cache,si,so,bi,bo,in,cs,us,sy,id,wa,st,gu".split(",")
)

parser = argparse.ArgumentParser(
    description="Perform capacity test on a WEB server using JMETER"
)

# Jmeter parameters
parser.add_argument(
    "--jmeter-path",
    help="Path to jmeter executable",
    default="/usr/bin/jmeter",
    type=str,
)
parser.add_argument(
    "--jmeter-test-plan", help="Path to jmx file", default="capacity.jmx", type=str
)
parser.add_argument(
    "--jmeter-log", help="Log file for jmeter", default="jmeter.log", type=str
)
parser.add_argument(
    "--test-request",
    help="Test request to be used to verify if host is up",
    default="VerySmall",
    type=str,
)
parser.add_argument(
    "--users",
    help="Number of users to be placed in the thread group",
    default=50,
    type=int,
)

# Repetition parameters
parser.add_argument(
    "--observations",
    help="Number of observations to perform for a single sample",
    default=3,
    type=int,
)
parser.add_argument(
    "--obs-duration",
    help="Duration (seconds) of a single repetition. Used to set thread group lifetime",
    default=300,
    type=int,
)
parser.add_argument(
    "--vmstat-delay",
    help="Sampling time (seconds) used for vmstat",
    default=5,
    type=int,
)

# File parameters
parser.add_argument(
    "--output-dir",
    help="Directory to use results data",
    default=os.path.join(os.getcwd(), "data"),
)
parser.add_argument(
    "--force", "-f", help="Delete previous $workdir if exists", action="store_true"
)

# Server parameters
parser.add_argument(
    "--server-addr",
    help="Server address and port. specified as <ip>:<port>",
    required=True,
    type=str,
)
parser.add_argument("--http-proto", help="HTTP protocol", default="http", type=str)
parser.add_argument(
    "--ssh-port",
    help="SSH port for remote management of the server",
    default=22,
    type=int,
)
parser.add_argument(
    "--ssh-key",
    help="SSH key path. Priviledged access is required to reboot server machine",
    required=True,
    type=str,
)
parser.add_argument(
    "--ssh-user",
    help="SSH key path. Priviledged access is required to reboot server machine",
    default="root",
    type=str,
)

# CTT parameters
parser.add_argument("--start-ctt", help="Start value for CTT", default=1000, type=int)
parser.add_argument("--stop-ctt", help="Stop value for CTT", default=3000, type=int)
parser.add_argument(
    "--incr-ctt", help="Increment amount for CTT", default=1000, type=int
)

# log parameters
parser.add_argument(
    "--log-level",
    help="Log level. Must be one of DEBUG, INFO, WARNING",
    default="INFO",
    type=str,
)
parser.add_argument(
    "--log-file", help="If specified, saves logs to a file", default=None, type=str
)


def calc_jmeter_stats(files: str) -> Tuple[float]:
    throughput = np.zeros(len(files))
    response_time = np.zeros(len(files))

    for i, file in enumerate(files):
        with open(file, "r") as f:
            r = csv.DictReader(f)

            timestamps = []
            elapsed = []
            for row in r:
                timestamps.append(int(row["timeStamp"]))
                elapsed.append(int(row["elapsed"]))

            start, stop, n = timestamps[0], timestamps[-1], len(timestamps)

        duration = (stop - start) / 1000

        throughput[i] = n / duration
        response_time[i] = np.mean(elapsed)

    return throughput.mean(), response_time.mean()


def calc_vmstat_stats(files: str) -> np.ndarray:
    mean = np.zeros(shape=(len(files), len(VMSTAT_FIELDNAMES)), dtype=np.float64)
    for i, file in enumerate(files):
        with open(file, "r") as f:
            lines = f.readlines()

        values = np.zeros(shape=(len(lines) - 1, len(VMSTAT_FIELDNAMES)))
        for k, line in enumerate(lines[1:]):
            values[k] = [int(x) for x in line.split(",")]

        mean[i] = np.mean(values, axis=0)

    return np.mean(mean, axis=0)


async def wait_host_is_up(url: str, retries: int = 100, interval: int = 5) -> None:
    # wait for server to come alive
    for _ in range(retries):
        try:
            r = requests.get(url)
            if r.status_code == 200:
                logging.debug("server %s alive", url)
                break
        except:
            logging.warning("server not responding; sleeping for %d seconds", interval)
            await asyncio.sleep(interval)
    else:
        raise Exception("server %s not responding", url)


# on unreliable connections, the vmstat cmd hangs when executed for a long time
# so we set the timeout to the duration of the observation and download the file afterwards
# assuming that after the timeout the command is completed
async def vmstat(
    addr: str,
    ssh_port: int,
    ssh_key: str,
    ssh_user: str,
    cmd: str,
    outfile: str,
    duration: float,
) -> None:
    try:
        async with asyncssh.connect(
            addr, port=ssh_port, client_keys=[ssh_key], username=ssh_user
        ) as conn:
            with tempfile.NamedTemporaryFile() as f:
                logging.debug("saving vmstat result to tempfile %s", f.name)
                await conn.run(cmd, timeout=duration, check=True)
                await asyncssh.scp((conn, "/tmp/vmstat.csv"), f.name)
                logging.debug("parsing vmstat file and copying to %s", outfile)
                os.system(CLEAN_CMD.format(f.name, outfile))
    except asyncio.CancelledError:
        logging.warning("requested cancellation by user")
    except TimeoutError:
        async with asyncssh.connect(
            addr, port=ssh_port, client_keys=[ssh_key], username=ssh_user
        ) as conn:
            with tempfile.NamedTemporaryFile() as f:
                await asyncssh.scp((conn, "/tmp/vmstat.csv"), f.name)
                logging.debug("parsing vmstat file and copying to %s", outfile)
                os.system(CLEAN_CMD.format(f.name, outfile))
    except Exception as x:
        logging.error("vmtstat error: %s, %s", x, x.args)
    finally:
        logging.info("completed vmstat task")


async def jmeter(bin: str, plan: str, res: str, log: str) -> None:
    logging.info("starting jmeter observation; saving to %s", res)
    try:
        p = await asyncio.create_subprocess_shell(
            " ".join(
                [
                    bin,
                    "-n",
                    "-t",
                    plan,
                    "-l",
                    res,
                    "-j",
                    log,
                ]
            ),
            stdout=asyncio.subprocess.DEVNULL,
            stderr=asyncio.subprocess.DEVNULL,
        )
        await p.communicate()
    except asyncio.CancelledError:
        logging.warning("requested cancellation by user")
    except Exception as x:
        logging.error("jmeter error: %s", x)
    finally:
        logging.info("completed jmeter observation")


async def run(
    jmeter_path: str,
    jmeter_logfile: str,
    jmeter_test_plan: str,
    test_request: str,
    observations: int,
    duration: int,
    vmstat_delay: int,
    start_ctt: int,
    stop_ctt: int,
    incr_ctt: int,
    data_dir: str,
    server_addr: str,
    http_proto: str,
    ssh_key: str,
    ssh_port: int,
    ssh_user: str,
) -> None:
    # open jmx file
    with open(jmeter_test_plan, "r") as f:
        tp = bs.BeautifulSoup(f, features="xml")

    # set server address
    tp.find("ConfigTestElement").find(
        "stringProp", {"name": "HTTPSampler.domain"}
    ).string = server_addr

    # set protocol
    tp.find("ConfigTestElement").find(
        "stringProp", {"name": "HTTPSampler.protocol"}
    ).string = http_proto

    p = (
        tp.find("HTTPSamplerProxy", {"testname": test_request})
        .find("stringProp", {"name": "HTTPSampler.path"})
        .text
    )

    test_url = f"{http_proto}://{server_addr}/{p}"

    # set repetition duration
    tp.find("ThreadGroup").find(
        "stringProp", {"name": "ThreadGroup.duration"}
    ).string = str(duration)

    # create output file
    jmeter_out = os.path.join(data_dir, "jmeter.csv")
    vmstat_out = os.path.join(data_dir, "vmstat.csv")

    try:
        with open(jmeter_out, "x") as f:
            f.write(",".join(JMETER_FIELDNAMES_OUT))
            f.write("\n")
        with open(vmstat_out, "x") as f:
            f.write("ctt,")
            f.write(",".join(VMSTAT_FIELDNAMES))
            f.write("\n")
    except FileExistsError:
        pass

    # format vmstat command acccording to input parameters
    vmstat_repetitions = int(duration / vmstat_delay)

    vmstat_cmd = VMSTAT_CMD.format(str(vmstat_delay), str(vmstat_repetitions))
    # ensure output directory is created
    measures_path = os.path.join(data_dir, "measures")
    Path(measures_path).mkdir(exist_ok=True, parents=True)

    for current_ctt in range(start_ctt, stop_ctt + incr_ctt, incr_ctt):
        # update test plan ctt
        tp.find("ConstantThroughputTimer").find("doubleProp").find(
            "value"
        ).string = str(current_ctt)

        # copy test plan file
        plan_path = os.path.join(data_dir, "plan.jmx")

        with open(plan_path, "w") as f:
            f.write(tp.decode())

        # wait for host to come alive
        await wait_host_is_up(test_url)

        jmeter_files = []
        vmstat_files = []
        for i in range(observations):
            # create repetition file out
            obs_path = os.path.join(measures_path, f"jmeter_{current_ctt}_{i+1}.csv")

            # vmstat_file name: vmstat.csv
            vmstat_file = os.path.join(measures_path, f"vmstat_{current_ctt}_{i+1}.csv")

            logging.debug(
                "starting jmeter observation %d with %d ctt (%f rps); saving to %s",
                i + 1,
                current_ctt,
                current_ctt / 60,
                jmeter_out,
            )
            logging.debug("starting vmstat task; saving to %s", vmstat_file)

            try:
                async with asyncio.TaskGroup() as tg:
                    tg.create_task(
                        jmeter(jmeter_path, plan_path, obs_path, jmeter_logfile)
                    )
                    tg.create_task(
                        vmstat(
                            server_addr,
                            ssh_port,
                            ssh_key,
                            ssh_user,
                            vmstat_cmd,
                            vmstat_file,
                            duration,
                        )
                    )
            except Exception as x:
                logging.error(
                    "error during capture: %s; check debug logs for more info", x
                )

            # save file
            vmstat_files.append(vmstat_file)
            jmeter_files.append(obs_path)

        logging.debug("rebooting server %s", server_addr)

        # reboot server
        async with asyncssh.connect(
            server_addr, port=ssh_port, client_keys=[ssh_key], username=ssh_user
        ) as conn:
            await conn.run("reboot")

        avg_througput, avg_response_time = calc_jmeter_stats(jmeter_files)

        with open(jmeter_out, "a") as f:
            f.write(
                ",".join(
                    [str(x) for x in (current_ctt, avg_througput, avg_response_time)]
                )
            )
            f.write("\n")

        stats = calc_vmstat_stats(vmstat_files)

        with open(vmstat_out, "a") as f:
            f.write(str(current_ctt) + ",")
            a = ",".join([str(x) for x in stats.tolist()])
            f.write(a)
            f.write("\n")

        await asyncio.sleep(5)


if __name__ == "__main__":
    args = parser.parse_args()

    numeric_level = getattr(logging, args.log_level.upper(), None)
    if not isinstance(numeric_level, int):
        raise ValueError("Invalid log level: %s" % args.log_level)

    logging.basicConfig(
        filename=args.log_file,
        format="%(levelname)s %(asctime)s: %(message)s",
        level=numeric_level,
    )
    try:
        if args.force:
            logging.warning("removing %s", args.output_dir)
            shutil.rmtree(args.output_dir)
    except FileNotFoundError:
        logging.info("no previous workdir found")
    finally:
        Path(args.output_dir).mkdir(parents=True, exist_ok=True)

    asyncio.run(
        run(
            args.jmeter_path,
            args.jmeter_log,
            args.jmeter_test_plan,
            args.test_request,
            args.observations,
            args.obs_duration,
            args.vmstat_delay,
            args.start_ctt,
            args.stop_ctt,
            args.incr_ctt,
            args.output_dir,
            args.server_addr,
            args.http_proto,
            args.ssh_key,
            args.ssh_port,
            args.ssh_user,
        )
    )
