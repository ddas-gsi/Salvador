#!/bin/bash

### Usage:
# Default suffix:
#   ./run_meta_parallel.sh 6095 6066
# Custom suffix:
#   ./run_meta_parallel.sh 6095 6066 --suffix "_53Au_final_test.root"
# 13 files:  ./run_meta_parallel.sh 1001 1002 1003 1004 1005 1006 1007 1008 1009 1010 1011 1012 1013
# 18 files:  ./run_meta_parallel.sh 2004 2005 2006 2007 2008 2009 2010 2011 2012 2013 2014 2015 2016 2017 2018 2019 2020 2021
# 05 files:  ./run_meta_parallel.sh 3001 3002 3003 3004 3005
# 34 files:  ./run_meta_parallel.sh 5002 5003 5004 5005 5006 5007 5008 5009 5010 5011 5012 5013 5014 5015 5016 5017 5018 5019 5020 5021 5022 5023 5024 5025 5026 5027 5028 5029 5030 5031 5032 5033 5034 5035
# 106 files: ./run_meta_parallel.sh 6003 6004 6005 6006 6007 6008 6009 6010 6011 6012 6013 6014 6015 6016 6017 6018 6019 6020 6021 6022 6023 6024 6025 6026 6027 6028 6029 6030 6031 6032 6033 6034 6035 6036 6037 6038 6039 6040 6041 6042 6043 6044 6045 6046 6047 6048 6049 6050 6051 6052 6053 6054 6055 6056 6057 6058 6059 6060 6061 6062 6063 6064 6065 6066 6067 6068 6069 6070 6071 6072 6073 6074 6075 6076 6077 6078 6079 6080 6081 6082 6083 6084 6085 6086 6087 6088 6089 6090 6091 6092 6093 6094 6095 6096 6097 6098 6099 6100 6101 6102 6103 6104 6105 6106 6107 6108


# Check for at least one RUN number
if [ "$#" -lt 1 ]; then
    echo "Usage: $0 RUN_Nbr1 [RUN_Nbr2 ...] [--suffix SFX]"
    exit 1
fi

# Default file prefixes and suffix
IFILE_PREFIX="./ridf/Gamma25_"
ODIR="/u/ddas/software/Salvador/rootfiles/ddas/meta/"
OFILE_PREFIX="/u/ddas/software/Salvador/rootfiles/ddas/meta/meta_"
OFILE_SUFFIX=".root"
# OFILE_SUFFIX="_new.root"


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
mkdir -p "$ODIR"

# Function to run scripts for a single RUN
run_single() {
    local RUN_Nbr=$1
    local SETTINGS_FILE

    # Determine run series (1000, 2000, etc.)
    case $((RUN_Nbr / 1000)) in
        1)
            SETTINGS_FILE="./settings/set_49K_C.dat"
            ;;
        2)
            SETTINGS_FILE="./settings/set_49K_Au.dat"
            ;;
        3)
            SETTINGS_FILE="./settings/set_49K_Be.dat"
            ;;
        5)
            SETTINGS_FILE="./settings/set_51K_Be.dat"
            ;;
        6)
            SETTINGS_FILE="./settings/set_51K_Au.dat"
            ;;
        *)
            echo "❌ No settings file defined for RUN $RUN_Nbr"
            return 1
            ;;
    esac

    echo "--------------------------------------------"
    echo "Starting full analysis for RUN Number: $RUN_Nbr"
    echo "Using settings file: $SETTINGS_FILE"
    echo "--------------------------------------------"

    echo "Running Metamorphosis..."
    Metamorphosis \
        -i "${IFILE_PREFIX}${RUN_Nbr}.ridf.gz" \
        -o "${OFILE_PREFIX}${RUN_Nbr}${OFILE_SUFFIX}" \
        -s "${SETTINGS_FILE}" 
        # -n 10000  # Uncomment to limit number of events for testing
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

