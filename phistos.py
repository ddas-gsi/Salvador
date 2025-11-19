#!/usr/bin/env python3
import argparse
import subprocess
import os
import threading
from concurrent.futures import ThreadPoolExecutor, as_completed
from collections import defaultdict

def run_command(run, cmd, log_dir, lock):
    """
    Execute a single command for a run, logging output into run-specific logfile.
    Logs header and command output under a lock to avoid interleaving.
    """
    os.makedirs(log_dir, exist_ok=True)
    log_path = os.path.join(log_dir, f"hist_{run}.log")

    # Run the command, capturing stdout/stderr
    proc = subprocess.Popen(cmd, shell=True,
                            stdout=subprocess.PIPE,
                            stderr=subprocess.STDOUT,
                            text=True)

    # Read all output
    out, _ = proc.communicate()
    rc = proc.returncode

    header = f"[Run {run}] Executing: {cmd}\n"
    with lock:
        #print(header, end="")  # to console
        with open(log_path, "a") as log_fh:
            log_fh.write(header)
            log_fh.write(out)
            if rc != 0:
                log_fh.write(f"[Run {run}] ERROR: exit code {rc}\n")
            else:
                log_fh.write(f"[Run {run}] Completed successfully.\n")
    return (run, cmd, rc)

def main():
    parser = argparse.ArgumentParser(description="Ultra-parallel batch runner with per-run logs")
    group = parser.add_mutually_exclusive_group(required=True)
    group.add_argument("-r", "--runs", nargs="+",
                       help="List of run numbers (e.g. 001 002 003)")
    group.add_argument("-R", "--run-file", type=str,
                       help="File with run numbers, one per line; '#' comments allowed")
    parser.add_argument("-j", "--jobs", type=int, default=32,
                        help="Total number of parallel commands")
    parser.add_argument("-l", "--log-dir", default="logs",
                        help="Directory to store per-run log files")
    args = parser.parse_args()

    # Build run list
    if args.run_file:
        runs = []
        with open(args.run_file) as f:
            for raw in f:
                line = raw.strip()
                if not line or line.startswith("#"):
                    continue
                line = line.split("#",1)[0].strip()
                if line:
                    runs.append(line)
    else:
        runs = args.runs

    # Define per-run commands
    command_templates = [
        ## 52,53,54Ca on Au
        "-i ~/rootfiles/kw/disi_{run}.root -o ~/rootfiles/kw/hist/pers_in56Tiout56Ti_{run}.root -s set_53Ca_Au.dat -tn tr_in56Tiout56Ti -wt 0",  
        "-i ~/rootfiles/kw/disi_{run}.root -o ~/rootfiles/kw/hist/pers_in57Tiout57Ti_{run}.root -s set_53Ca_Au.dat -tn tr_in57Tiout57Ti -wt 0",  
        "-i ~/rootfiles/kw/disi_{run}.root -o ~/rootfiles/kw/hist/pers_in58Tiout58Ti_{run}.root -s set_53Ca_Au.dat -tn tr_in58Tiout58Ti -wt 0",  
        "-i ~/rootfiles/kw/disi_{run}.root -o ~/rootfiles/kw/hist/pers_in54Scout54Sc_{run}.root -s set_53Ca_Au.dat -tn tr_in54Scout54Sc -wt 0",  
        "-i ~/rootfiles/kw/disi_{run}.root -o ~/rootfiles/kw/hist/pers_in55Scout55Sc_{run}.root -s set_53Ca_Au.dat -tn tr_in55Scout55Sc -wt 0",  
        "-i ~/rootfiles/kw/disi_{run}.root -o ~/rootfiles/kw/hist/pers_in56Scout56Sc_{run}.root -s set_53Ca_Au.dat -tn tr_in56Scout56Sc -wt 0",  
        "-i ~/rootfiles/kw/disi_{run}.root -o ~/rootfiles/kw/hist/pers_in52Caout52Ca_{run}.root -s set_53Ca_Au.dat -tn tr_in52Caout52Ca -wt 0",  
        "-i ~/rootfiles/kw/disi_{run}.root -o ~/rootfiles/kw/hist/pers_in53Caout53Ca_{run}.root -s set_53Ca_Au.dat -tn tr_in53Caout53Ca -wt 0",          
        "-i ~/rootfiles/kw/disi_{run}.root -o ~/rootfiles/kw/hist/pers_in54Caout54Ca_{run}.root -s set_53Ca_Au.dat -tn tr_in54Caout54Ca -wt 0",  
        "-i ~/rootfiles/kw/disi_{run}.root -o ~/rootfiles/kw/hist/pers_in51Kout51K_{run}.root   -s set_53Ca_Au.dat -tn tr_in51Kout51K   -wt 0",  
        
        ## 52,53,54Ca on Be
        #"-i ~/rootfiles/kw/disi_{run}.root -o ~/rootfiles/kw/hist/pers_in56Tiout56Ti_{run}.root -s set_53Ca_Be.dat -tn tr_in56Tiout56Ti -wt 0", 
        #"-i ~/rootfiles/kw/disi_{run}.root -o ~/rootfiles/kw/hist/pers_in56Tiout55Ti_{run}.root -s set_53Ca_Be.dat -tn tr_in56Tiout55Ti -wt 0", 
        #"-i ~/rootfiles/kw/disi_{run}.root -o ~/rootfiles/kw/hist/pers_in56Tiout55Sc_{run}.root -s set_53Ca_Be.dat -tn tr_in56Tiout55Sc -wt 0", 
        #"-i ~/rootfiles/kw/disi_{run}.root -o ~/rootfiles/kw/hist/pers_in56Tiout54Sc_{run}.root -s set_53Ca_Be.dat -tn tr_in56Tiout54Sc -wt 0", 
        #"-i ~/rootfiles/kw/disi_{run}.root -o ~/rootfiles/kw/hist/pers_in57Tiout57Ti_{run}.root -s set_53Ca_Be.dat -tn tr_in57Tiout57Ti -wt 0", 
        #"-i ~/rootfiles/kw/disi_{run}.root -o ~/rootfiles/kw/hist/pers_in57Tiout56Ti_{run}.root -s set_53Ca_Be.dat -tn tr_in57Tiout56Ti -wt 0", 
        #"-i ~/rootfiles/kw/disi_{run}.root -o ~/rootfiles/kw/hist/pers_in57Tiout56Sc_{run}.root -s set_53Ca_Be.dat -tn tr_in57Tiout56Sc -wt 0", 
        #"-i ~/rootfiles/kw/disi_{run}.root -o ~/rootfiles/kw/hist/pers_in57Tiout55Sc_{run}.root -s set_53Ca_Be.dat -tn tr_in57Tiout55Sc -wt 0", 
        #"-i ~/rootfiles/kw/disi_{run}.root -o ~/rootfiles/kw/hist/pers_in58Tiout58Ti_{run}.root -s set_53Ca_Be.dat -tn tr_in58Tiout58Ti -wt 0", 
        #"-i ~/rootfiles/kw/disi_{run}.root -o ~/rootfiles/kw/hist/pers_in58Tiout57Ti_{run}.root -s set_53Ca_Be.dat -tn tr_in58Tiout57Ti -wt 0", 
        #"-i ~/rootfiles/kw/disi_{run}.root -o ~/rootfiles/kw/hist/pers_in58Tiout57Sc_{run}.root -s set_53Ca_Be.dat -tn tr_in58Tiout57Sc -wt 0", 
        #"-i ~/rootfiles/kw/disi_{run}.root -o ~/rootfiles/kw/hist/pers_in58Tiout56Sc_{run}.root -s set_53Ca_Be.dat -tn tr_in58Tiout56Sc -wt 0",
        #
        #"-i ~/rootfiles/kw/disi_{run}.root -o ~/rootfiles/kw/hist/pers_in54Scout54Sc_{run}.root -s set_53Ca_Be.dat -tn tr_in54Scout54Sc -wt 0", 
        #"-i ~/rootfiles/kw/disi_{run}.root -o ~/rootfiles/kw/hist/pers_in54Scout53Sc_{run}.root -s set_53Ca_Be.dat -tn tr_in54Scout53Sc -wt 0", 
        #"-i ~/rootfiles/kw/disi_{run}.root -o ~/rootfiles/kw/hist/pers_in54Scout53Ca_{run}.root -s set_53Ca_Be.dat -tn tr_in54Scout53Ca -wt 0", 
        #"-i ~/rootfiles/kw/disi_{run}.root -o ~/rootfiles/kw/hist/pers_in54Scout52Ca_{run}.root -s set_53Ca_Be.dat -tn tr_in54Scout52Ca -wt 0", 
        #"-i ~/rootfiles/kw/disi_{run}.root -o ~/rootfiles/kw/hist/pers_in55Scout55Sc_{run}.root -s set_53Ca_Be.dat -tn tr_in55Scout55Sc -wt 0", 
        #"-i ~/rootfiles/kw/disi_{run}.root -o ~/rootfiles/kw/hist/pers_in55Scout54Sc_{run}.root -s set_53Ca_Be.dat -tn tr_in55Scout54Sc -wt 0", 
        #"-i ~/rootfiles/kw/disi_{run}.root -o ~/rootfiles/kw/hist/pers_in55Scout54Ca_{run}.root -s set_53Ca_Be.dat -tn tr_in55Scout54Ca -wt 0", 
        #"-i ~/rootfiles/kw/disi_{run}.root -o ~/rootfiles/kw/hist/pers_in55Scout53Ca_{run}.root -s set_53Ca_Be.dat -tn tr_in55Scout53Ca -wt 0", 
        #"-i ~/rootfiles/kw/disi_{run}.root -o ~/rootfiles/kw/hist/pers_in56Scout56Sc_{run}.root -s set_53Ca_Be.dat -tn tr_in56Scout56Sc -wt 0", 
        #"-i ~/rootfiles/kw/disi_{run}.root -o ~/rootfiles/kw/hist/pers_in56Scout55Sc_{run}.root -s set_53Ca_Be.dat -tn tr_in56Scout55Sc -wt 0", 
        #"-i ~/rootfiles/kw/disi_{run}.root -o ~/rootfiles/kw/hist/pers_in56Scout55Ca_{run}.root -s set_53Ca_Be.dat -tn tr_in56Scout55Ca -wt 0", 
        #"-i ~/rootfiles/kw/disi_{run}.root -o ~/rootfiles/kw/hist/pers_in56Scout54Ca_{run}.root -s set_53Ca_Be.dat -tn tr_in56Scout54Ca -wt 0",
        #
        #"-i ~/rootfiles/kw/disi_{run}.root -o ~/rootfiles/kw/hist/pers_in52Caout52Ca_{run}.root -s set_53Ca_Be.dat -tn tr_in52Caout52Ca -wt 0", 
        #"-i ~/rootfiles/kw/disi_{run}.root -o ~/rootfiles/kw/hist/pers_in52Caout51Ca_{run}.root -s set_53Ca_Be.dat -tn tr_in52Caout51Ca -wt 0", 
        #"-i ~/rootfiles/kw/disi_{run}.root -o ~/rootfiles/kw/hist/pers_in52Caout51K_{run}.root  -s set_53Ca_Be.dat -tn tr_in52Caout51K  -wt 0", 
        #"-i ~/rootfiles/kw/disi_{run}.root -o ~/rootfiles/kw/hist/pers_in52Caout50K_{run}.root  -s set_53Ca_Be.dat -tn tr_in52Caout50K  -wt 0",
        #"-i ~/rootfiles/kw/disi_{run}.root -o ~/rootfiles/kw/hist/pers_in53Caout53Ca_{run}.root -s set_53Ca_Be.dat -tn tr_in53Caout53Ca -wt 0", 
        #"-i ~/rootfiles/kw/disi_{run}.root -o ~/rootfiles/kw/hist/pers_in53Caout52Ca_{run}.root -s set_53Ca_Be.dat -tn tr_in53Caout52Ca -wt 0", 
        #"-i ~/rootfiles/kw/disi_{run}.root -o ~/rootfiles/kw/hist/pers_in53Caout52K_{run}.root  -s set_53Ca_Be.dat -tn tr_in53Caout52K  -wt 0", 
        #"-i ~/rootfiles/kw/disi_{run}.root -o ~/rootfiles/kw/hist/pers_in53Caout51K_{run}.root  -s set_53Ca_Be.dat -tn tr_in53Caout51K  -wt 0", 
        #"-i ~/rootfiles/kw/disi_{run}.root -o ~/rootfiles/kw/hist/pers_in54Caout54Ca_{run}.root -s set_53Ca_Be.dat -tn tr_in54Caout54Ca -wt 0",
        #"-i ~/rootfiles/kw/disi_{run}.root -o ~/rootfiles/kw/hist/pers_in54Caout53Ca_{run}.root -s set_53Ca_Be.dat -tn tr_in54Caout53Ca -wt 0",
        #"-i ~/rootfiles/kw/disi_{run}.root -o ~/rootfiles/kw/hist/pers_in54Caout53K_{run}.root  -s set_53Ca_Be.dat -tn tr_in54Caout53K  -wt 0",
        #"-i ~/rootfiles/kw/disi_{run}.root -o ~/rootfiles/kw/hist/pers_in54Caout52K_{run}.root  -s set_53Ca_Be.dat -tn tr_in54Caout52K  -wt 0",

        #"-i ~/rootfiles/kw/disi_{run}.root -o ~/rootfiles/kw/hist/pers_in54Sc_{run}.root -s set_53Ca_Be.dat -tn tr_in54Sc -wt 0",
        #"-i ~/rootfiles/kw/disi_{run}.root -o ~/rootfiles/kw/hist/pers_in55Sc_{run}.root -s set_53Ca_Be.dat -tn tr_in55Sc -wt 0",
        #"-i ~/rootfiles/kw/disi_{run}.root -o ~/rootfiles/kw/hist/pers_in56Sc_{run}.root -s set_53Ca_Be.dat -tn tr_in56Sc -wt 0",
        #"-i ~/rootfiles/kw/disi_{run}.root -o ~/rootfiles/kw/hist/pers_in52Ca_{run}.root -s set_53Ca_Be.dat -tn tr_in52Ca -wt 0",
        #"-i ~/rootfiles/kw/disi_{run}.root -o ~/rootfiles/kw/hist/pers_in53Ca_{run}.root -s set_53Ca_Be.dat -tn tr_in53Ca -wt 0",
        #"-i ~/rootfiles/kw/disi_{run}.root -o ~/rootfiles/kw/hist/pers_in54Ca_{run}.root -s set_53Ca_Be.dat -tn tr_in54Ca -wt 0",
        #"-i ~/rootfiles/kw/disi_{run}.root -o ~/rootfiles/kw/hist/pers_in56Ti_{run}.root -s set_53Ca_Be.dat -tn tr_in56Ti -wt 0",
        #"-i ~/rootfiles/kw/disi_{run}.root -o ~/rootfiles/kw/hist/pers_in57Ti_{run}.root -s set_53Ca_Be.dat -tn tr_in57Ti -wt 0",
        #"-i ~/rootfiles/kw/disi_{run}.root -o ~/rootfiles/kw/hist/pers_in58Ti_{run}.root -s set_53Ca_Be.dat -tn tr_in58Ti -wt 0",


        ## 50,51Ca Be target
        #"-i ~/rootfiles/kw/disi_{run}.root -o ~/rootfiles/kw/hist/pers_in52Scout52Sc_{run}.root -s set_50Ca_C.dat -tn tr_in52Scout52Sc -wt 0",
        #"-i ~/rootfiles/kw/disi_{run}.root -o ~/rootfiles/kw/hist/pers_in52Scout51Sc_{run}.root -s set_50Ca_C.dat -tn tr_in52Scout51Sc -wt 0",
        #"-i ~/rootfiles/kw/disi_{run}.root -o ~/rootfiles/kw/hist/pers_in52Scout50Ca_{run}.root -s set_50Ca_C.dat -tn tr_in52Scout50Ca -wt 0",
        #"-i ~/rootfiles/kw/disi_{run}.root -o ~/rootfiles/kw/hist/pers_in50Caout50Ca_{run}.root -s set_50Ca_C.dat -tn tr_in50Caout50Ca -wt 0",
        #"-i ~/rootfiles/kw/disi_{run}.root -o ~/rootfiles/kw/hist/pers_in50Caout49Ca_{run}.root -s set_50Ca_C.dat -tn tr_in50Caout49Ca -wt 0",
        #"-i ~/rootfiles/kw/disi_{run}.root -o ~/rootfiles/kw/hist/pers_in50Caout48Ca_{run}.root -s set_50Ca_C.dat -tn tr_in50Caout48Ca -wt 0",
        #"-i ~/rootfiles/kw/disi_{run}.root -o ~/rootfiles/kw/hist/pers_in51Caout51Ca_{run}.root -s set_50Ca_C.dat -tn tr_in51Caout51Ca -wt 0",
        #"-i ~/rootfiles/kw/disi_{run}.root -o ~/rootfiles/kw/hist/pers_in51Caout50Ca_{run}.root -s set_50Ca_C.dat -tn tr_in51Caout50Ca -wt 0",
        #"-i ~/rootfiles/kw/disi_{run}.root -o ~/rootfiles/kw/hist/pers_in51Caout49Ca_{run}.root -s set_50Ca_C.dat -tn tr_in51Caout49Ca -wt 0",
        #"-i ~/rootfiles/kw/disi_{run}.root -o ~/rootfiles/kw/hist/pers_in50Caout48K_{run}.root  -s set_50Ca_C.dat -tn tr_in50Caout48K  -wt 0",
        #"-i ~/rootfiles/kw/disi_{run}.root -o ~/rootfiles/kw/hist/pers_in51Caout49K_{run}.root  -s set_50Ca_C.dat -tn tr_in51Caout49K  -wt 0",


        ## 50,51Ca Au target
        #"-i ~/rootfiles/kw/disi_{run}.root -o ~/rootfiles/kw/hist/pers_in50Caout50Ca_{run}.root -s set_50Ca_Au.dat -tn tr_in50Caout50Ca -wt 0",
        #"-i ~/rootfiles/kw/disi_{run}.root -o ~/rootfiles/kw/hist/pers_in51Caout51Ca_{run}.root -s set_50Ca_Au.dat -tn tr_in51Caout51Ca -wt 0",
        
        ## 50,51Ca C target
        #"-i ~/rootfiles/kw/disi_{run}.root -o ~/rootfiles/kw/hist/pers_in52Scout51Sc_{run}.root -s set_50Ca_C.dat -tn tr_in52Scout51Sc -wt 0",
        #"-i ~/rootfiles/kw/disi_{run}.root -o ~/rootfiles/kw/hist/pers_in52Scout52Sc_{run}.root -s set_50Ca_C.dat -tn tr_in52Scout52Sc -wt 0",
        #"-i ~/rootfiles/kw/disi_{run}.root -o ~/rootfiles/kw/hist/pers_in52Scout50Ca_{run}.root -s set_50Ca_C.dat -tn tr_in52Scout50Ca -wt 0",
        #"-i ~/rootfiles/kw/disi_{run}.root -o ~/rootfiles/kw/hist/pers_in50Caout48Ca_{run}.root -s set_50Ca_C.dat -tn tr_in50Caout48Ca -wt 0",
        #"-i ~/rootfiles/kw/disi_{run}.root -o ~/rootfiles/kw/hist/pers_in50Caout49Ca_{run}.root -s set_50Ca_C.dat -tn tr_in50Caout49Ca -wt 0",
        #"-i ~/rootfiles/kw/disi_{run}.root -o ~/rootfiles/kw/hist/pers_in50Caout50Ca_{run}.root -s set_50Ca_C.dat -tn tr_in50Caout50Ca -wt 0",
        #"-i ~/rootfiles/kw/disi_{run}.root -o ~/rootfiles/kw/hist/pers_in50Caout48K_{run}.root  -s set_50Ca_C.dat -tn tr_in50Caout48K  -wt 0",
        #"-i ~/rootfiles/kw/disi_{run}.root -o ~/rootfiles/kw/hist/pers_in50Caout49K_{run}.root  -s set_50Ca_C.dat -tn tr_in50Caout49K  -wt 0",
        #"-i ~/rootfiles/kw/disi_{run}.root -o ~/rootfiles/kw/hist/pers_in50Caout47K_{run}.root  -s set_50Ca_C.dat -tn tr_in50Caout47K  -wt 0",
        #"-i ~/rootfiles/kw/disi_{run}.root -o ~/rootfiles/kw/hist/pers_in51Caout50Ca_{run}.root -s set_50Ca_C.dat -tn tr_in51Caout50Ca -wt 0",
        #"-i ~/rootfiles/kw/disi_{run}.root -o ~/rootfiles/kw/hist/pers_in51Caout51Ca_{run}.root -s set_50Ca_C.dat -tn tr_in51Caout51Ca -wt 0",
        #"-i ~/rootfiles/kw/disi_{run}.root -o ~/rootfiles/kw/hist/pers_in51Caout49K_{run}.root  -s set_50Ca_C.dat -tn tr_in51Caout49K  -wt 0",
        #"-i ~/rootfiles/kw/disi_{run}.root -o ~/rootfiles/kw/hist/pers_in51Caout50K_{run}.root  -s set_50Ca_C.dat -tn tr_in51Caout50K  -wt 0",
        #"-i ~/rootfiles/kw/disi_{run}.root -o ~/rootfiles/kw/hist/pers_in49Kout47K_{run}.root   -s set_50Ca_C.dat -tn tr_in49Kout47K   -wt 0",
        #"-i ~/rootfiles/kw/disi_{run}.root -o ~/rootfiles/kw/hist/pers_in49Kout48K_{run}.root   -s set_50Ca_C.dat -tn tr_in49Kout48K   -wt 0",
        #"-i ~/rootfiles/kw/disi_{run}.root -o ~/rootfiles/kw/hist/pers_in49Kout49K_{run}.root   -s set_50Ca_C.dat -tn tr_in49Kout49K   -wt 0",
        
        ]
    commands = []
    for run in runs:
        for tpl in command_templates:
            cmd = f"Persistence {tpl.format(run=run)}"
            commands.append((run, cmd))

    # Prepare a lock per run to sync logfile writes
    locks = defaultdict(threading.Lock)

    # Submit all commands to a single executor
    with ThreadPoolExecutor(max_workers=args.jobs) as executor:
        future_to_task = {
            executor.submit(run_command, run, cmd, args.log_dir, locks[run]): (run, cmd)
            for run, cmd in commands
        }

        # Collect results
        for future in as_completed(future_to_task):
            run, cmd = future_to_task[future]
            try:
                run, cmd, rc = future.result()
                status = "SUCCESS" if rc == 0 else f"FAILURE({rc})"
                print(f"[Run {run}] {status}: {cmd}")
            except Exception as e:
                print(f"[Run {run}] EXCEPTION: {e}")

if __name__ == "__main__":
    main()
