import argparse
import asyncio
import csv
import logging
import os
import shutil
from pathlib import Path

import asyncssh
import bs4 as bs
import numpy as np
import requests
from typing_extensions import Tuple

JMETER_FIELDNAMES_OUT = ["ctt", "page_type", "response_time"]

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
parser.add_argument("--http-proto", help="HTTP protocol",
                    default="http", type=str)
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
parser.add_argument("--ctt", help="Increment amount for CTT",
                    type=int, required=True)

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
    response_time = np.zeros(len(files))

    for i, file in enumerate(files):
        with open(file, "r") as f:
            r = csv.DictReader(f)

            elapsed = []
            for row in r:
                elapsed.append(int(row["elapsed"]))

        response_time[i] = np.mean(elapsed)

    return response_time.mean()


async def wait_host_is_up(url: str, retries: int = 100, interval: int = 5) -> None:
    # wait for server to come alive
    for _ in range(retries):
        try:
            r = requests.get(url)
            if r.status_code == 200:
                logging.debug("server %s alive", url)
                break
        except:
            logging.warning(
                "server not responding; sleeping for %d seconds", interval)
            await asyncio.sleep(interval)
    else:
        raise Exception("server %s not responding", url)


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
    ctt: int,
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

    try:
        with open(jmeter_out, "x") as f:
            f.write(",".join(JMETER_FIELDNAMES_OUT))
            f.write("\n")
    except FileExistsError:
        pass

    # ensure output directory is created
    measures_path = os.path.join(data_dir, "measures")
    Path(measures_path).mkdir(exist_ok=True, parents=True)

    # update test plan ctt
    tp.find("ConstantThroughputTimer").find("doubleProp").find("value").string = str(
        ctt
    )

    # copy test plan file
    plan_path = os.path.join(data_dir, "plan.jmx")

    with open(plan_path, "w") as f:
        f.write(tp.decode())

    jmeter_files = []
    for i in range(observations):
        # wait for host to come alive
        await wait_host_is_up(test_url)

        # create repetition file out
        obs_path = os.path.join(measures_path, f"jmeter_{ctt}_{i+1}.csv")

        logging.debug(
            "starting jmeter observation %d with %d ctt (%f rps); saving to %s",
            i + 1,
            ctt,
            ctt / 60,
            jmeter_out,
        )

        try:
            async with asyncio.TaskGroup() as tg:
                tg.create_task(jmeter(jmeter_path, plan_path,
                               obs_path, jmeter_logfile))
        except Exception as x:
            logging.error(
                "error during capture: %s; check debug logs for more info", x)

        # save file
        jmeter_files.append(obs_path)

        logging.debug("rebooting server %s", server_addr)

        # reboot server
        async with asyncssh.connect(
            server_addr, port=ssh_port, client_keys=[
                ssh_key], username=ssh_user
        ) as conn:
            await conn.run("reboot")

        avg_response_time = calc_jmeter_stats(jmeter_files)

        with open(jmeter_out, "a") as f:
            f.write(",".join([str(x) for x in (ctt, test_request.lower(), avg_response_time)]))
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
            args.ctt,
            args.output_dir,
            args.server_addr,
            args.http_proto,
            args.ssh_key,
            args.ssh_port,
            args.ssh_user,
        )
    )
