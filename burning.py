#!/usr/bin/env python3
# Parallel processing version of burning.py using ThreadPoolExecutor.

import re
import subprocess
import csv
from itertools import product
from concurrent.futures import ThreadPoolExecutor, as_completed
from threading import Lock

# CPU Usage
use_core = 30  # Number of cores to use

# Lists of parameters
lcx_list = [1, 2, 3, -1]  # -lcx values
fid_list = [3, 7, 8, 11, -1, 37]  # -fid values

# Base command template
base_cmd = (
    "BurningGiraffe -i ./rootfiles/ddas/salva/meta/meta_1010.root "
    "-o ./rootfiles/ddas/salva/burn/burn_1010_clean_cutf{fid}_lc{lcx}.root "
    "-tn tr -v 2 -lcx {lcx} -fid {fid}"
)

# Generate all command combinations
commands = [{"cmd": base_cmd.format(lcx=lcx, fid=fid), "lcx": lcx, "fid": fid} for lcx, fid in product(lcx_list, fid_list)]

# Regex patterns
patterns = {
    "RunNbr": r"Run Number: (\d+)",
    "cutType": r"Loaded (\S+) cut for FocalPlane (\d+)",
    "CutApplied_50Ca20_pid2": r"Cut Applied 50Ca20 in PID 2:\s+(\d+)",
    "CutApplied_49K19_pid2": r"Cut Applied 49K19 in PID 2:\s+(\d+)",
    "Total_50Ca20_pid2": r"Total 50Ca20 in PID 2:\s+(\d+)",
    "Total_49K19_pid2": r"Total 49K19 in PID 2:\s+(\d+)",
    "CutApplied_50Ca20_pid5": r"Cut Applied 50Ca20 in PID 5:\s+(\d+)",
    "CutApplied_49K19_pid5": r"Cut Applied 49K19 in PID 5:\s+(\d+)",
    "Total_50Ca20_pid5": r"Total 50Ca20 in PID 5:\s+(\d+)",
    "Total_49K19_pid5": r"Total 49K19 in PID 5:\s+(\d+)",

    "OutputFile": r"output file: ./rootfiles/ddas/salva/burn/(\S+)",
}

# Thread-safe CSV writing
csv_lock = Lock()

def run_burning(idx, cmd_dict):
    cmd = cmd_dict["cmd"]
    lcx = cmd_dict["lcx"]
    fid = cmd_dict["fid"]

    print(f"Running: {cmd}")
    result = subprocess.run(cmd, shell=True, capture_output=True, text=True)
    output = result.stdout + result.stderr

    # Remove progress lines
    meaningful_output_lines = [line for line in output.splitlines()
                               if not re.match(r"\s*\d+(\.\d+)? % done", line)]
    meaningful_output = "\n".join(meaningful_output_lines)

    # Extract Run Number
    run_number = "NA"
    match = re.search(patterns["RunNbr"], meaningful_output)
    if match:
        run_number = int(match.group(1))

    # Extract cutType and FocalPlane
    cut_type = "NA"
    focal_plane = "NA"
    match = re.search(patterns["cutType"], meaningful_output)
    if match:
        cut_type = match.group(1)
        focal_plane = int(match.group(2))
        if lcx == -1:
            cut_type = "All"
        if fid == -1:
            focal_plane = "All"
        else:
            focal_plane = fid

    # Extract Output File
    output_file = "NA"
    match = re.search(patterns["OutputFile"], meaningful_output)
    if match:
        output_file = match.group(1)

    # Extract numeric fields
    def extract_int(field):
        match = re.search(patterns[field], meaningful_output)
        return int(match.group(1)) if match else "NA"

    cut_applied_50Ca_pid2 = extract_int("CutApplied_50Ca20_pid2")
    total_50Ca_pid2 = extract_int("Total_50Ca20_pid2")
    cut_applied_49K_pid2 = extract_int("CutApplied_49K19_pid2")
    total_49K_pid2 = extract_int("Total_49K19_pid2")
    cut_applied_50Ca_pid5 = extract_int("CutApplied_50Ca20_pid5")
    total_50Ca_pid5 = extract_int("Total_50Ca20_pid5")
    cut_applied_49K_pid5 = extract_int("CutApplied_49K19_pid5")
    total_49K_pid5 = extract_int("Total_49K19_pid5")

    # Calculate percentages
    percent_50Ca_pid2 = (cut_applied_50Ca_pid2 / total_50Ca_pid2 * 100) if total_50Ca_pid2 != "NA" and total_50Ca_pid2 != 0 else "NA"
    percent_49K_pid2 = (cut_applied_49K_pid2 / total_49K_pid2 * 100) if total_49K_pid2 != "NA" and total_49K_pid2 != 0 else "NA"
    percent_50Ca_pid5 = (cut_applied_50Ca_pid5 / total_50Ca_pid5 * 100) if total_50Ca_pid5 != "NA" and total_50Ca_pid5 != 0 else "NA"
    percent_49K_pid5 = (cut_applied_49K_pid5 / total_49K_pid5 * 100) if total_49K_pid5 != "NA" and total_49K_pid5 != 0 else "NA"

    # Build row in proper CSV order

    row = [
        f"Run_{idx}",
        run_number,
        cut_type,
        focal_plane,
        cut_applied_50Ca_pid2,
        total_50Ca_pid2,
        cut_applied_49K_pid2,
        total_49K_pid2,
        cut_applied_50Ca_pid5,
        total_50Ca_pid5,
        cut_applied_49K_pid5,
        total_49K_pid5,
        lcx,
        fid,
        percent_50Ca_pid2,
        percent_49K_pid2,
        percent_50Ca_pid5,
        percent_49K_pid5,
        output_file
    ]

    print(f"Filtered meaningful output for Run_{idx}:")
    print(meaningful_output)

    return row

# Write CSV
with open("burning_giraffe_results.csv", "w", newline="") as f:
    writer = csv.writer(f)
    writer.writerow(["Run", "RunNbr", "cutType", "FocalPlane", 
                     "CutApplied_50Ca20_pid2", "Total_50Ca20_pid2", "CutApplied_49K19_pid2", "Total_49K19_pid2",
                     "CutApplied_50Ca20_pid5", "Total_50Ca20_pid5", "CutApplied_49K19_pid5", "Total_49K19_pid5", 
                     "lcx", "fid", "percent_50Ca_pid2", "percent_49K_pid2", "percent_50Ca_pid5", "percent_49K_pid5", "OutputFile"])

    # Use ThreadPoolExecutor for parallel runs
    with ThreadPoolExecutor(max_workers=use_core) as executor:  
        future_to_idx = {executor.submit(run_burning, idx, cmd): idx for idx, cmd in enumerate(commands, start=1)}
        for future in as_completed(future_to_idx):
            row = future.result()
            with csv_lock:
                writer.writerow(row)

print("Results written to burning_giraffe_results.csv")







