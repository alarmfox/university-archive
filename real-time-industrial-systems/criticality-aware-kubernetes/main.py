import subprocess
import argparse
import logging
import time

parser = argparse.ArgumentParser(
    description="Measure kubelet pod startup"
)

parser.add_argument(
    "--image", help="Image to be used for deployment. Use the format <image>:<tag>",
    type=str, required=True
)
parser.add_argument(
    "--prio", help="Priority to give to kubelet", default="jmeter.log", type=str
)

parser.add_argument(
    "--repetitions", "-r",
    help="Number of repetition to perform",
    default=20,
    type=int,
)

parser.add_argument(
    "--log-level",
    help="Log level. Must be one of DEBUG, INFO, WARNING",
    default="INFO",
    type=str
)


def run(
    image: str,
    repetitions: int,
) -> None:

    logging.debug("starting with following params repetitions=%d, image=%s",
                  repetitions, image)

    for pods in [20, 30, 40, 50]:
        print("Running deployment for", pods, "pods")
        print("Starting kubelet")
        input("Press a key to start...")

        for i in range(repetitions):
            # create deployment
            print("repetition", f"{i+1}/{repetitions}")
            subprocess.run(
                ["kubectl", "create", "deployment",
                 "test-deployment",
                 "--image", image,
                 "--replicas", str(pods)
                 ])

            # wait for pods to start
            while True:
                pods_process = subprocess.Popen(
                    ["kubectl", "get", "pods"], stdout=subprocess.PIPE)
                out, err = pods_process.communicate()

                lines = out.split(b"\n")[1:]
                pods_running = list(
                    map(lambda s: 1 if b"Running" in s or b"Completed" in s else 0, lines))
                pods_running = sum(pods_running)
                if pods_running == pods:
                    break
                print("Waiting for", pods - pods_running, "pods")
                time.sleep(3)

            # remove deployment
            subprocess.run(
                ["kubectl", "delete", "deployment", "test-deployment"]
            )

            while True:
                pods_process = subprocess.Popen(
                    ["kubectl", "get", "pods"], stdout=subprocess.PIPE)
                out, err = pods_process.communicate()

                pods_running = len(out.split(b"\n")[1:])
                if pods_running == 0:
                    break
                print(pods_running, "are still running...")
                time.sleep(3)


if __name__ == "__main__":
    args = parser.parse_args()

    numeric_level = getattr(logging, args.log_level.upper(), None)
    if not isinstance(numeric_level, int):
        raise ValueError("Invalid log level: %s" % args.log_level)

    logging.basicConfig(
        format="%(levelname)s %(asctime)s: %(message)s",
        level=numeric_level,
    )

    run(
        args.image,
        args.repetitions,
    )
