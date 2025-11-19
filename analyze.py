#!/usr/bin/env python3
import argparse
import subprocess
from concurrent.futures import ThreadPoolExecutor, as_completed
import os
def process_run(run, log_dir):
    
    os.makedirs(log_dir, exist_ok=True)
    log_path = os.path.join(log_dir, f"run_{run}.log")
    
    # Define the pipeline of commands for each run
    commands = [
        ## 52,53,54Ca
        ## Au target
        f"Metamorphosis -i ~/ridf/Gamma25_{run}.ridf -o ~/rootfiles/kw/meta_{run}.root -s set_53Ca_Au.dat",
        f"Disintegration -i ~/rootfiles/kw/meta_{run}.root -o ~/rootfiles/kw/disi_{run}.root -c cuts_53Ca_Au.root"
        ## Be target
        #f"Metamorphosis -i ~/ridf/Gamma25_{run}.ridf -o ~/rootfiles/kw/meta_{run}.root -s set_53Ca_Be.dat",
        #f"Disintegration -i ~/rootfiles/kw/meta_{run}.root -o ~/rootfiles/kw/disi_{run}.root -c cuts_53Ca_Be.root"
        #f"Disintegration -i ~/rootfiles/kw/meta_{run}.root -o ~/rootfiles/kw/disi_{run}.root -c ScCacuts_53Ca_Be.root"
        
        ## 50,51Ca
        ## Be target
        #f"Metamorphosis -i ~/ridf/Gamma25_{run}.ridf -o ~/rootfiles/kw/meta_{run}.root -s set_50Ca_Be.dat",
        #f"Disintegration -i ~/rootfiles/kw/meta_{run}.root -o ~/rootfiles/kw/disi_{run}.root -c cuts_50Ca_Be.root"
        ## Au target
        #f"Metamorphosis -i ~/ridf/Gamma25_{run}.ridf -o ~/rootfiles/kw/meta_{run}.root -s set_50Ca_Au.dat",
        #f"Disintegration -i ~/rootfiles/kw/meta_{run}.root -o ~/rootfiles/kw/disi_{run}.root -c cuts_50Ca_Au.root"
        ## C target
        #f"Metamorphosis -i ~/ridf/Gamma25_{run}.ridf -o ~/rootfiles/kw/meta_{run}.root -s set_50Ca_C.dat",
        #f"Disintegration -i ~/rootfiles/kw/meta_{run}.root -o ~/rootfiles/kw/disi_{run}.root -c cuts_50Ca_C.root"
    ]
    with open(log_path, "w") as log_fh:
        for cmd in commands:
            header = f"[Run {run}] Executing: {cmd}\n"
            log_fh.write(header)
            log_fh.flush()
            print(header,end="")

            result = subprocess.run(cmd, shell=True,
                                    stdout=log_fh, stderr=subprocess.STDOUT)
            if result.returncode != 0:
                error_msg = f"[Run {run}] ERROR: Command failed with exit code {result.returncode}\n"
                log_fh.write(error_msg)
                return False

        log_fh.write(f"[Run {run}] All commands completed successfully.\n")
    return True

def main():
    parser = argparse.ArgumentParser(
        description="Parallel run processor with per-run logs"
    )
    group = parser.add_mutually_exclusive_group(required=True)
    group.add_argument(
        "-r", "--runs", nargs="+",
        help="List of run numbers (e.g. 001 002 003)"
    )
    group.add_argument(
        "-R", "--run-file", type=str,
        help="Path to a file containing run numbers, one per line"
    )
    parser.add_argument(
        "-j", "--jobs", type=int, default=8,
        help="Number of runs to process in parallel"
    )
    parser.add_argument(
        "-l", "--log-dir", default="logs",
        help="Directory to store per-run log files"
    )
    args = parser.parse_args()

    # Build the list of runs
    if args.run_file:
        runs = []
        with open(args.run_file) as f:
            for raw in f:
                line = raw.strip()
                # skip blank lines and full-line comments
                if not line or line.startswith("#"):
                    continue
                # strip inline comments
                line = line.split("#", 1)[0].strip()
                if line:
                    runs.append(line)
    else:
        runs = args.runs

    # Launch parallel processing
    with ThreadPoolExecutor(max_workers=args.jobs) as executor:
        future_to_run = {
            executor.submit(process_run, run, args.log_dir): run
            for run in runs
        }
        for future in as_completed(future_to_run):
            run = future_to_run[future]
            try:
                success = future.result()
                status = "SUCCESS" if success else "FAILURE"
                print(f"[Run {run}] {status}. Log: {os.path.join(args.log_dir, f'run_{run}.log')}")
            except Exception as e:
                print(f"[Run {run}] Exception: {e}")

if __name__ == "__main__":
    main()
