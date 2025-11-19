#!/usr/bin/env python3
import argparse
import subprocess
from concurrent.futures import ThreadPoolExecutor, as_completed
import os
def process_run(run, log_dir):
    
    os.makedirs(log_dir, exist_ok=True)
    log_path = os.path.join(log_dir, f"hist_{run}.log")
    
    commands = [
        ## 52,53,54Ca
        ## Au
        f"Persistence -i ~/rootfiles/kw/disi_{run}.root -o ~/rootfiles/kw/hist/pers_in56Tiout56Ti_{run}.root -s set_53Ca_Be.dat -tn tr_in56Tiout56Ti -wt 0",
        f"Persistence -i ~/rootfiles/kw/disi_{run}.root -o ~/rootfiles/kw/hist/pers_in57Tiout57Ti_{run}.root -s set_53Ca_Be.dat -tn tr_in57Tiout57Ti -wt 0",
        f"Persistence -i ~/rootfiles/kw/disi_{run}.root -o ~/rootfiles/kw/hist/pers_in58Tiout58Ti_{run}.root -s set_53Ca_Be.dat -tn tr_in58Tiout58Ti -wt 0",
        f"Persistence -i ~/rootfiles/kw/disi_{run}.root -o ~/rootfiles/kw/hist/pers_in54Scout54Sc_{run}.root -s set_53Ca_Be.dat -tn tr_in54Scout54Sc -wt 0",
        f"Persistence -i ~/rootfiles/kw/disi_{run}.root -o ~/rootfiles/kw/hist/pers_in55Scout55Sc_{run}.root -s set_53Ca_Be.dat -tn tr_in55Scout55Sc -wt 0",
        f"Persistence -i ~/rootfiles/kw/disi_{run}.root -o ~/rootfiles/kw/hist/pers_in56Scout56Sc_{run}.root -s set_53Ca_Be.dat -tn tr_in56Scout56Sc -wt 0",
        f"Persistence -i ~/rootfiles/kw/disi_{run}.root -o ~/rootfiles/kw/hist/pers_in52Caout52Ca_{run}.root -s set_53Ca_Be.dat -tn tr_in52Caout52Ca -wt 0",
        f"Persistence -i ~/rootfiles/kw/disi_{run}.root -o ~/rootfiles/kw/hist/pers_in53Caout53Ca_{run}.root -s set_53Ca_Be.dat -tn tr_in53Caout53Ca -wt 0",
        f"Persistence -i ~/rootfiles/kw/disi_{run}.root -o ~/rootfiles/kw/hist/pers_in54Caout54Ca_{run}.root -s set_53Ca_Be.dat -tn tr_in54Caout54Ca -wt 0",
        f"Persistence -i ~/rootfiles/kw/disi_{run}.root -o ~/rootfiles/kw/hist/pers_in51Kout51K_{run}.root   -s set_53Ca_Be.dat -tn tr_in51Kout51K   -wt 0",       
        
        ## Be target
        #f"Persistence -i ~/rootfiles/kw/disi_{run}.root -o ~/rootfiles/kw/hist/pers_in56Tiout56Ti_{run}.root -s set_53Ca_Be.dat -tn tr_in56Tiout56Ti -wt 0",
        #f"Persistence -i ~/rootfiles/kw/disi_{run}.root -o ~/rootfiles/kw/hist/pers_in56Tiout55Ti_{run}.root -s set_53Ca_Be.dat -tn tr_in56Tiout55Ti -wt 0",
        #f"Persistence -i ~/rootfiles/kw/disi_{run}.root -o ~/rootfiles/kw/hist/pers_in56Tiout55Sc_{run}.root -s set_53Ca_Be.dat -tn tr_in56Tiout55Sc -wt 0",
        #f"Persistence -i ~/rootfiles/kw/disi_{run}.root -o ~/rootfiles/kw/hist/pers_in56Tiout54Sc_{run}.root -s set_53Ca_Be.dat -tn tr_in56Tiout54Sc -wt 0",
        #f"Persistence -i ~/rootfiles/kw/disi_{run}.root -o ~/rootfiles/kw/hist/pers_in57Tiout57Ti_{run}.root -s set_53Ca_Be.dat -tn tr_in57Tiout57Ti -wt 0",
        #f"Persistence -i ~/rootfiles/kw/disi_{run}.root -o ~/rootfiles/kw/hist/pers_in57Tiout56Ti_{run}.root -s set_53Ca_Be.dat -tn tr_in57Tiout56Ti -wt 0",
        #f"Persistence -i ~/rootfiles/kw/disi_{run}.root -o ~/rootfiles/kw/hist/pers_in57Tiout56Sc_{run}.root -s set_53Ca_Be.dat -tn tr_in57Tiout56Sc -wt 0",
        #f"Persistence -i ~/rootfiles/kw/disi_{run}.root -o ~/rootfiles/kw/hist/pers_in57Tiout55Sc_{run}.root -s set_53Ca_Be.dat -tn tr_in57Tiout55Sc -wt 0",
        #f"Persistence -i ~/rootfiles/kw/disi_{run}.root -o ~/rootfiles/kw/hist/pers_in58Tiout58Ti_{run}.root -s set_53Ca_Be.dat -tn tr_in58Tiout58Ti -wt 0",
        #f"Persistence -i ~/rootfiles/kw/disi_{run}.root -o ~/rootfiles/kw/hist/pers_in58Tiout57Ti_{run}.root -s set_53Ca_Be.dat -tn tr_in58Tiout57Ti -wt 0",       
        #f"Persistence -i ~/rootfiles/kw/disi_{run}.root -o ~/rootfiles/kw/hist/pers_in58Tiout57Sc_{run}.root -s set_53Ca_Be.dat -tn tr_in58Tiout57Sc -wt 0",
        #f"Persistence -i ~/rootfiles/kw/disi_{run}.root -o ~/rootfiles/kw/hist/pers_in58Tiout56Sc_{run}.root -s set_53Ca_Be.dat -tn tr_in58Tiout56Sc -wt 0",
        #f"Persistence -i ~/rootfiles/kw/disi_{run}.root -o ~/rootfiles/kw/hist/pers_in54Scout54Sc_{run}.root -s set_53Ca_Be.dat -tn tr_in54Scout54Sc -wt 0",
        #f"Persistence -i ~/rootfiles/kw/disi_{run}.root -o ~/rootfiles/kw/hist/pers_in54Scout53Sc_{run}.root -s set_53Ca_Be.dat -tn tr_in54Scout53Sc -wt 0",
        #f"Persistence -i ~/rootfiles/kw/disi_{run}.root -o ~/rootfiles/kw/hist/pers_in55Scout55Sc_{run}.root -s set_53Ca_Be.dat -tn tr_in55Scout55Sc -wt 0",
        #f"Persistence -i ~/rootfiles/kw/disi_{run}.root -o ~/rootfiles/kw/hist/pers_in55Scout54Sc_{run}.root -s set_53Ca_Be.dat -tn tr_in55Scout54Sc -wt 0",
        #f"Persistence -i ~/rootfiles/kw/disi_{run}.root -o ~/rootfiles/kw/hist/pers_in56Scout56Sc_{run}.root -s set_53Ca_Be.dat -tn tr_in56Scout56Sc -wt 0",
        #f"Persistence -i ~/rootfiles/kw/disi_{run}.root -o ~/rootfiles/kw/hist/pers_in52Caout51Ca_{run}.root -s set_53Ca_Be.dat -tn tr_in52Caout51Ca -wt 0",
        #f"Persistence -i ~/rootfiles/kw/disi_{run}.root -o ~/rootfiles/kw/hist/pers_in52Caout52Ca_{run}.root -s set_53Ca_Be.dat -tn tr_in52Caout52Ca -wt 0",
        #f"Persistence -i ~/rootfiles/kw/disi_{run}.root -o ~/rootfiles/kw/hist/pers_in53Caout53Ca_{run}.root -s set_53Ca_Be.dat -tn tr_in53Caout53Ca -wt 0",
        #f"Persistence -i ~/rootfiles/kw/disi_{run}.root -o ~/rootfiles/kw/hist/pers_in53Caout52Ca_{run}.root -s set_53Ca_Be.dat -tn tr_in53Caout52Ca -wt 0",
        #f"Persistence -i ~/rootfiles/kw/disi_{run}.root -o ~/rootfiles/kw/hist/pers_in54Caout54Ca_{run}.root -s set_53Ca_Be.dat -tn tr_in54Caout54Ca -wt 0",       
        
        ## 50,51Ca
        ## Be target
        #f"Persistence -i ~/rootfiles/kw/disi_{run}.root -o ~/rootfiles/kw/hist/pers_in52Scout52Sc_{run}.root -s set_50Ca_C.dat -tn tr_in52Scout52Sc -wt 0",
        #f"Persistence -i ~/rootfiles/kw/disi_{run}.root -o ~/rootfiles/kw/hist/pers_in52Scout51Sc_{run}.root -s set_50Ca_C.dat -tn tr_in52Scout51Sc -wt 0",
        #f"Persistence -i ~/rootfiles/kw/disi_{run}.root -o ~/rootfiles/kw/hist/pers_in52Scout50Ca_{run}.root -s set_50Ca_C.dat -tn tr_in52Scout50Ca -wt 0",
        #f"Persistence -i ~/rootfiles/kw/disi_{run}.root -o ~/rootfiles/kw/hist/pers_in50Caout50Ca_{run}.root -s set_50Ca_C.dat -tn tr_in50Caout50Ca -wt 0",
        #f"Persistence -i ~/rootfiles/kw/disi_{run}.root -o ~/rootfiles/kw/hist/pers_in50Caout49Ca_{run}.root -s set_50Ca_C.dat -tn tr_in50Caout49Ca -wt 0",
        #f"Persistence -i ~/rootfiles/kw/disi_{run}.root -o ~/rootfiles/kw/hist/pers_in50Caout48Ca_{run}.root -s set_50Ca_C.dat -tn tr_in50Caout48Ca -wt 0",
        #f"Persistence -i ~/rootfiles/kw/disi_{run}.root -o ~/rootfiles/kw/hist/pers_in51Caout51Ca_{run}.root -s set_50Ca_C.dat -tn tr_in51Caout51Ca -wt 0",
        #f"Persistence -i ~/rootfiles/kw/disi_{run}.root -o ~/rootfiles/kw/hist/pers_in51Caout50Ca_{run}.root -s set_50Ca_C.dat -tn tr_in51Caout50Ca -wt 0",
        #f"Persistence -i ~/rootfiles/kw/disi_{run}.root -o ~/rootfiles/kw/hist/pers_in51Caout49Ca_{run}.root -s set_50Ca_C.dat -tn tr_in51Caout49Ca -wt 0",
        #f"Persistence -i ~/rootfiles/kw/disi_{run}.root -o ~/rootfiles/kw/hist/pers_in50Caout48K_{run}.root  -s set_50Ca_C.dat -tn tr_in50Caout48K  -wt 0",
        #f"Persistence -i ~/rootfiles/kw/disi_{run}.root -o ~/rootfiles/kw/hist/pers_in51Caout49K_{run}.root  -s set_50Ca_C.dat -tn tr_in51Caout49K  -wt 0",
        
        ## Au target
        #f"Persistence -i ~/rootfiles/kw/disi_{run}.root -o ~/rootfiles/kw/hist/pers_in50Caout50Ca_{run}.root -s set_50Ca_Au.dat -tn tr_in50Caout50Ca -wt 0",
        #f"Persistence -i ~/rootfiles/kw/disi_{run}.root -o ~/rootfiles/kw/hist/pers_in51Caout51Ca_{run}.root -s set_50Ca_Au.dat -tn tr_in51Caout51Ca -wt 0",

        ## C target
        #f"Persistence -i ~/rootfiles/kw/disi_{run}.root -o ~/rootfiles/kw/hist/pers_in52Scout51Sc_{run}.root -s set_50Ca_C.dat -tn tr_in52Scout51Sc -wt 0",
        #f"Persistence -i ~/rootfiles/kw/disi_{run}.root -o ~/rootfiles/kw/hist/pers_in52Scout52Sc_{run}.root -s set_50Ca_C.dat -tn tr_in52Scout52Sc -wt 0",
        #f"Persistence -i ~/rootfiles/kw/disi_{run}.root -o ~/rootfiles/kw/hist/pers_in52Scout50Ca_{run}.root -s set_50Ca_C.dat -tn tr_in52Scout50Ca -wt 0",
        #f"Persistence -i ~/rootfiles/kw/disi_{run}.root -o ~/rootfiles/kw/hist/pers_in50Caout48Ca_{run}.root -s set_50Ca_C.dat -tn tr_in50Caout48Ca -wt 0",
        #f"Persistence -i ~/rootfiles/kw/disi_{run}.root -o ~/rootfiles/kw/hist/pers_in50Caout49Ca_{run}.root -s set_50Ca_C.dat -tn tr_in50Caout49Ca -wt 0",
        #f"Persistence -i ~/rootfiles/kw/disi_{run}.root -o ~/rootfiles/kw/hist/pers_in50Caout50Ca_{run}.root -s set_50Ca_C.dat -tn tr_in50Caout50Ca -wt 0",
        #f"Persistence -i ~/rootfiles/kw/disi_{run}.root -o ~/rootfiles/kw/hist/pers_in50Caout48K_{run}.root  -s set_50Ca_C.dat -tn tr_in50Caout48K  -wt 0",
        #f"Persistence -i ~/rootfiles/kw/disi_{run}.root -o ~/rootfiles/kw/hist/pers_in50Caout49K_{run}.root  -s set_50Ca_C.dat -tn tr_in50Caout49K  -wt 0",
        #f"Persistence -i ~/rootfiles/kw/disi_{run}.root -o ~/rootfiles/kw/hist/pers_in50Caout47K_{run}.root  -s set_50Ca_C.dat -tn tr_in50Caout47K  -wt 0",
        #f"Persistence -i ~/rootfiles/kw/disi_{run}.root -o ~/rootfiles/kw/hist/pers_in51Caout50Ca_{run}.root -s set_50Ca_C.dat -tn tr_in51Caout50Ca -wt 0",
        #f"Persistence -i ~/rootfiles/kw/disi_{run}.root -o ~/rootfiles/kw/hist/pers_in51Caout51Ca_{run}.root -s set_50Ca_C.dat -tn tr_in51Caout51Ca -wt 0",
        #f"Persistence -i ~/rootfiles/kw/disi_{run}.root -o ~/rootfiles/kw/hist/pers_in51Caout49K_{run}.root  -s set_50Ca_C.dat -tn tr_in51Caout49K  -wt 0",
        #f"Persistence -i ~/rootfiles/kw/disi_{run}.root -o ~/rootfiles/kw/hist/pers_in51Caout50K_{run}.root  -s set_50Ca_C.dat -tn tr_in51Caout50K  -wt 0",
        #f"Persistence -i ~/rootfiles/kw/disi_{run}.root -o ~/rootfiles/kw/hist/pers_in49Kout47K_{run}.root   -s set_50Ca_C.dat -tn tr_in49Kout47K   -wt 0",
        #f"Persistence -i ~/rootfiles/kw/disi_{run}.root -o ~/rootfiles/kw/hist/pers_in49Kout48K_{run}.root   -s set_50Ca_C.dat -tn tr_in49Kout48K   -wt 0",
        #f"Persistence -i ~/rootfiles/kw/disi_{run}.root -o ~/rootfiles/kw/hist/pers_in49Kout49K_{run}.root   -s set_50Ca_C.dat -tn tr_in49Kout49K   -wt 0",

##        f"Persistence -i ~/rootfiles/kw/disi_{run}.root -o ~/rootfiles/kw/hist/pers_in52Sc_{run}.root -s set.dat -tn tr_in52Sc -wt 0",
##        f"Persistence -i ~/rootfiles/kw/disi_{run}.root -o ~/rootfiles/kw/hist/pers_in50Ca_{run}.root -s set.dat -tn tr_in50Ca -wt 0",
##        f"Persistence -i ~/rootfiles/kw/disi_{run}.root -o ~/rootfiles/kw/hist/pers_in51Ca_{run}.root -s set.dat -tn tr_in51Ca -wt 0",
##        f"Persistence -i ~/rootfiles/kw/disi_{run}.root -o ~/rootfiles/kw/hist/pers_in49K_{run}.root  -s set.dat  -tn tr_in49K -wt 0"
##        f"Persistence -i ~/rootfiles/kw/disi_{run}.root -o ~/rootfiles/kw/hist/pers_in54Sc_{run}.root -s set_53Ca_Be.dat -tn tr_in54Sc -wt 0",
##        f"Persistence -i ~/rootfiles/kw/disi_{run}.root -o ~/rootfiles/kw/hist/pers_in55Sc_{run}.root -s set_53Ca_Be.dat -tn tr_in55Sc -wt 0",
##        f"Persistence -i ~/rootfiles/kw/disi_{run}.root -o ~/rootfiles/kw/hist/pers_in56Sc_{run}.root -s set_53Ca_Be.dat -tn tr_in56Sc -wt 0",
##        f"Persistence -i ~/rootfiles/kw/disi_{run}.root -o ~/rootfiles/kw/hist/pers_in52Ca_{run}.root -s set_53Ca_Be.dat -tn tr_in52Ca -wt 0",
##        f"Persistence -i ~/rootfiles/kw/disi_{run}.root -o ~/rootfiles/kw/hist/pers_in53Ca_{run}.root -s set_53Ca_Be.dat -tn tr_in53Ca -wt 0",
##        f"Persistence -i ~/rootfiles/kw/disi_{run}.root -o ~/rootfiles/kw/hist/pers_in54Ca_{run}.root -s set_53Ca_Be.dat -tn tr_in54Ca -wt 0",
##        f"Persistence -i ~/rootfiles/kw/disi_{run}.root -o ~/rootfiles/kw/hist/pers_in56Ti_{run}.root -s set_53Ca_Be.dat -tn tr_in56Ti -wt 0",
##        f"Persistence -i ~/rootfiles/kw/disi_{run}.root -o ~/rootfiles/kw/hist/pers_in57Ti_{run}.root -s set_53Ca_Be.dat -tn tr_in57Ti -wt 0",
##        f"Persistence -i ~/rootfiles/kw/disi_{run}.root -o ~/rootfiles/kw/hist/pers_in58Ti_{run}.root -s set_53Ca_Be.dat -tn tr_in58Ti -wt 0",
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
        "-j", "--jobs", type=int, default=16,
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
                print(f"[Run {run}] {status}. Log: {os.path.join(args.log_dir, f'hist_{run}.log')}")
            except Exception as e:
                print(f"[Run {run}] Exception: {e}")

if __name__ == "__main__":
    main()
