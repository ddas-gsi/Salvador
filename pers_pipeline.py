#!/usr/bin/env python3

# # Pipeline: select trees → run Persistence (parallel) → merge (hadd)

import ROOT
import subprocess
from pathlib import Path
from concurrent.futures import ProcessPoolExecutor, ThreadPoolExecutor, as_completed
from collections import defaultdict
import time
import re
import os
import argparse

# --- Config Default ---

runSeries = 1000

# What isotope you want -----
ISO = "K"
# ISO = "Ca"   

FILE_SUFFIX=""  
DIR = ""  
# FILE_SUFFIX="_new" 
# DIR = "/new"  

max_workers = 30  

RUN_SERIES = {
    1000: [1001, 1002, 1003, 1004, 1005, 1006, 1007, 1008, 1009, 1010, 1011, 1012, 1013],   
    2000: [2004, 2005, 2006, 2007, 2008, 2009, 2010, 2011, 2012, 2013, 2014, 2015 ,2016, 2017, 2018, 2019, 2020, 2021],
    3000: [3001, 3002, 3003, 3004, 3005],
    5000: [5002, 5003, 5004, 5005, 5006, 5007, 5008, 5009, 5010, 5011, 5012, 5013, 5014, 5015, 5016, 5017, 5018, 5019, 5020, 5021, 5022, 5023, 5024, 5025, 5026, 5027, 5028, 5029, 5030, 5031, 5032, 5033, 5034, 5035],
    6000: [
        6003, 6004, 6005, 6006, 6007, 6008, 6009, 6010, 6011, 6012, 6013, 6014, 6015, 6016, 6017, 6018, 6019, 6020,
        6021, 6022, 6023, 6024, 6025, 6026, 6027, 6028, 6029, 6030, 6031, 6032, 6033, 6034, 6035, 6036, 6037, 6038,
        6039, 6040, 6041, 6042, 6043, 6044, 6045, 6046, 6047, 6048, 6049, 6050, 6051, 6052, 6053, 6054, 6055, 6056,
        6057, 6058, 6059, 6060, 6061, 6062, 6063, 6064, 6065, 6066, 6067, 6068, 6069, 6070, 6071, 6072, 6073, 6074,
        6075, 6076, 6077, 6078, 6079, 6080, 6081, 6082, 6083, 6084, 6085, 6086, 6087, 6088, 6089, 6090, 6091, 6092,
        6093, 6094, 6095, 6096, 6097, 6098, 6099, 6100, 6101, 6102, 6103, 6104, 6105, 6106, 6107, 6108
        ]
}

SETTINGS = {
    "K": {
        1000: "set_49K_C.dat",
        2000: "set_49K_Au.dat",
        3000: "set_49K_Be.dat",
        5000: "set_51K_Be.dat",
        6000: "set_51K_Au.dat",
    },
    "Ca": {
        1000: "Ca/set_50Ca_C.dat",
        2000: "Ca/set_50Ca_Au.dat",
        3000: "Ca/set_50Ca_Be.dat",
        5000: "Ca/set_52Ca_Be.dat",
        6000: "Ca/set_52Ca_Au.dat",
    }
}
# SETTINGS_MAP = SETTINGS.get(ISO)
# if SETTINGS_MAP is None:
#     raise ValueError(f"Unsupported ISO: {ISO}")



def parse_arguments():

    parser = argparse.ArgumentParser(
        description="Persistence pipeline"
    )

    parser.add_argument(
        "iso",
        nargs="?",
        default=ISO,
        choices=SETTINGS.keys(),
        help=f"Isotope (default: {ISO})"
    )

    parser.add_argument(
        "-w", "--workers",
        type=int,
        default=max_workers,
        help=f"Number of parallel workers (default: {max_workers})"
    )

    parser.add_argument(
        "-r", "--runSeries",
        type=int,
        default=runSeries,
        choices={1000,2000,3000,5000,6000},
        help=f"Run series to process (default: {runSeries})"
    )

    return parser.parse_args()

args = parse_arguments()
ISO = args.iso
max_workers = args.workers
runSeries = args.runSeries
SETTINGS_MAP = SETTINGS[ISO]
runNrs = RUN_SERIES[runSeries]

print(f"Using isotope: {ISO}")
print(f"Processing run series: {runSeries} with runs: {runNrs}")
print(f"Using workers : {max_workers}")


inputFiles = [
    Path.home() / f"Lustre/gamma/ddas/RIBF249/rootfiles/ddas/disi{DIR}/disi_{rn}{FILE_SUFFIX}.root"
    for rn in runNrs
]

for f in inputFiles:
    if not f.exists():
        raise FileNotFoundError(f"Missing input file: {f}")

outputDir = Path.home() / "Lustre/gamma/ddas/RIBF249/rootfiles/ddas/pers"


def choose_settings_file(runNrs):
    series = { (rn // 1000) * 1000 for rn in runNrs }

    if len(series) != 1:
        raise ValueError(f"Mixed run series detected: {sorted(series)}")

    series = series.pop()

    if series not in SETTINGS_MAP:
        raise ValueError(f"No settings file defined for {series} series")

    return Path.home() / "software/Salvador/settings" / SETTINGS_MAP[series]

settingsFile = choose_settings_file(runNrs)
print(f"Using settings file: {settingsFile}")

def select_trees(treeList, ISO):
    pattern = re.compile(rf"tr_in\d+{ISO}out\d+{ISO}")
    return [t for t in treeList if pattern.fullmatch(t)]

def select_trees_inelastic(treeList, ISO):
    pattern = re.compile(rf"tr_in(\d+){ISO}out\1{ISO}")
    return [t for t in treeList if pattern.fullmatch(t)]

# Ensure output directory exists
outputDir.mkdir(parents=True, exist_ok=True)
(outputDir / "singleRuns").mkdir(parents=True, exist_ok=True)

rootFile = ROOT.TFile(str(inputFiles[0]), "READ")
treeList = rootFile.GetListOfKeys()

# Filter non-empty trees 
trees_to_process = []
for tr in treeList:
    tree_name = tr.GetName()
    entries = rootFile.Get(tree_name).GetEntries()
    print(""f"Tree: {tree_name}, \t Entries: {entries}")
    if entries > 0:
        trees_to_process.append(tree_name)


trees_to_process = select_trees_inelastic(trees_to_process, ISO)

print(f"No of Trees to process : {len(trees_to_process)}")
print(f"Trees to process : {trees_to_process}")

def getInputFileName(rn):
    return Path.home() / f"Lustre/gamma/ddas/RIBF249/rootfiles/ddas/disi{DIR}/disi_{rn}{FILE_SUFFIX}.root"

def runPersSingleThread(tree_name, rn):
    try:
        inputFile = getInputFileName(rn)
        run_tag = f"{rn}"
        outputFile = outputDir / f"singleRuns/pers_{tree_name}_{run_tag}.root"

        cmd = [
            "Persistence",
            "-i", str(inputFile),
            "-o", str(outputFile),
            "-s", str(settingsFile),
            "-tn", tree_name,
            "-wt", "0",
        ]

        rootFile = ROOT.TFile(str(inputFile), "READ")
        entries = rootFile.Get(tree_name).GetEntries()
        start_time = time.time()
        print(f"[{tree_name} | {rn}] START: "," ".join(cmd), flush=True)
        subprocess.run(cmd, check=True)
        end_time = time.time()
        elapsed_time = end_time - start_time
        print(f"🌿 [{tree_name} | {rn}] DONE in \t {elapsed_time:.2f} seconds ✅ \t [{entries/elapsed_time:.2f} events/sec]", flush=True)

        return {
            "success": True,
            "tree": tree_name,
            "run": rn,
            "output": outputFile
        }

    except Exception as e:
        print(f"[{tree_name} | {rn}] FAILED: {e}", flush=True)
        return {
            "success": False,
            "tree": tree_name,
            "run": rn,
            "output": None
        }


def wrapper(args):
    tree_name, rn = args
    return runPersSingleThread(tree_name, rn)


def hadd_tree(tree, files):
    if not files:
        print(f"[{tree}] No files to merge, skipping")
        return

    series = { (rn // 1000) * 1000 for rn in runNrs }

    if len(series) != 1:
        raise ValueError(f"Mixed run series detected: {sorted(series)}")
    
    series = series.pop()

    outputFile = outputDir / f"merged/pers_{tree}_{series}_merged.root"
    outputFile.parent.mkdir(parents=True, exist_ok=True)

    cmd = ["hadd", "-f", str(outputFile)] + [str(f) for f in files]

    print(f"[{tree}] Merging {len(files)} files...", flush=True)

    try:
        subprocess.run(cmd, check=True)
        print(f"[{tree}] MERGED → {outputFile}", flush=True)
    except Exception as e:
        print(f"[{tree}] MERGE FAILED: {e}", flush=True)





if __name__ == "__main__":

    start_time = time.time()

    tasks = [(tree, rn) for tree in trees_to_process for rn in runNrs]
    results = []
    with ThreadPoolExecutor(max_workers=max_workers) as executor:
        future_to_task = {
        executor.submit(runPersSingleThread, tree, rn): (tree, rn)
        for tree, rn in tasks
        }

        for future in as_completed(future_to_task):
            res = future.result()
            results.append(res)

    time.sleep(5)  # Small delay to ensure all files are closed before merging

    tree_outputs = defaultdict(list)

    for res in results:
        if res["success"] and res["output"] is not None:
            tree_outputs[res["tree"]].append(res["output"])

    # print(tree_outputs)

    for tree, files in tree_outputs.items():
        hadd_tree(tree, files)

    end_time = time.time()
    total_time = end_time - start_time
    print(f"Total execution time:\t {total_time:.2f} seconds ✅")
  
         