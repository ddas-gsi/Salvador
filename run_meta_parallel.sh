#!/bin/bash

### Usage:
# Default suffix:
#   ./run_meta_parallel.sh 6095 6066
# Custom suffix:
#   ./run_meta_parallel.sh 6095 6066 --suffix "_53Au_final_test.root"

# Check for at least one RUN number
if [ "$#" -lt 1 ]; then
    echo "Usage: $0 RUN_Nbr1 [RUN_Nbr2 ...] [--suffix SFX]"
    exit 1
fi

# Default file prefixes and suffix
IFILE_PREFIX="/u/ddas/software/Salvador/ridf/Gamma25_"
OFILE_PREFIX="/u/ddas/software/Salvador/rootfiles/ddas/salva/meta/meta_"
OFILE_SUFFIX=".root"
# OFILE_SUFFIX="_53Au_final_tof5fix.root"

MAX_PARALLEL=10  # Maximum number of parallel jobs

# Parse optional suffix argument
ARGS=()
while [[ $# -gt 0 ]]; do
    case "$1" in
        --suffix)
            OFILE_SUFFIX="$2"
            shift 2
            ;;
        *)
            ARGS+=("$1")
            shift
            ;;
    esac
done

# Replace original arguments with RUN numbers only
set -- "${ARGS[@]}"

# Create output directory if it doesn't exist
mkdir -p "$OFILE_PREFIX"

# Function to run scripts for a single RUN
run_single() {
    local RUN_Nbr=$1

    echo "--------------------------------------------"
    echo "Starting full analysis for RUN Number: $RUN_Nbr"
    echo "--------------------------------------------"

    echo "Running Metamorphosis..."
    Metamorphosis \
        -i "${IFILE_PREFIX}${RUN_Nbr}.ridf.gz" \
        -o "${OFILE_PREFIX}${RUN_Nbr}${OFILE_SUFFIX}" \
        -s ./settings/set_50Ca_Au.dat
    sleep 2

    echo "✅ Finished processing RUN Number: $RUN_Nbr"
    echo ""
}

# Counter for running jobs
job_count=0

for RUN_Nbr in "$@"; do
    run_single "$RUN_Nbr" &   # Run in background
    ((job_count++))
    echo "Running $job_count jobs in parallel..." 

    # Wait if max parallel jobs reached
    if [[ $job_count -ge $MAX_PARALLEL ]]; then
        wait -n              # Wait for any job to finish
        ((job_count--))      # Decrease job count
    fi
done

# Wait for all remaining background jobs
wait

echo "✅✅ All RUN numbers processed."

