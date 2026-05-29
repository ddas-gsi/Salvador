#!/usr/bin/env bash

##### USAGE:
# ./run_batch.sh --runs runs.txt
# ./run_batch.sh --runs runs.txt --dry-run
# MAX_PARALLEL=8 ./run_batch.sh --runs runs.txt
# ./run_batch.sh --runs runs.txt --force
# ./run_batch.sh --runs runs.txt --suffix "_v3.root" --force
# ./run_batch.sh --runs runs_6000.csv --dry-run --force
# ./run_batch.sh --runs runs_2000.csv --dry-run --force
# ./run_batch.sh --runs runs_1000_p2.csv --dry-run --summary summary_1000_p2.tsv --force

# While running check:
# tail -f logs/run_6003.log

# After completion check:
# column -t summary.tsv

# Rerun failed jobs:
# awk '$2=="FAILED"{print $1}' summary.tsv > failed.txt
# ./run_batch.sh --runs failed.txt

set -u

########################
# CONFIGURATION
########################
IFILE_PREFIX="/u/ddas/software/Salvador/ridf/Gamma25_"
ODIR="/u/ddas/software/Salvador/rootfiles/ddas/meta"
OFILE_PREFIX="${ODIR}/meta_"
OFILE_SUFFIX=".root"

LOGDIR="./logs"
SUMMARY="./summary.tsv"

FORCE=0

MAX_PARALLEL=${MAX_PARALLEL:-5}      # Default 10 parallel jobs
DRY_RUN=0
RUNFILE=""

mkdir -p "$ODIR" "$LOGDIR"

########################
# ARGUMENT PARSING
########################
usage() {
    echo "Usage:"
    echo "  $0 --runs runs.txt|runs.csv [--suffix SUF] [--dry-run] [--force] [--summary FILE]"
    exit 1
}

while [[ $# -gt 0 ]]; do
    case "$1" in
        --runs)
            RUNFILE="$2"
            shift 2
            ;;
        --suffix)
            OFILE_SUFFIX="$2"
            shift 2
            ;;
        --dry-run)
            DRY_RUN=1
            shift
            ;;
        --force)
            FORCE=1
            shift
            ;;
        --summary)
            SUMMARY="$2"
            shift 2
            ;;
        *)
            usage
            ;;
    esac
done


[[ -z "$RUNFILE" ]] && usage
[[ ! -f "$RUNFILE" ]] && { echo "❌ Run file not found"; exit 1; }

########################
# LOAD RUNS
########################
mapfile -t RUNS < <(
    awk -F',' '
    NR==1 && $1 ~ /run/i {next}
    {gsub(/[^0-9]/,"",$1); if($1!="") print $1}
    ' "$RUNFILE"
)

[[ ${#RUNS[@]} -eq 0 ]] && { echo "❌ No runs found"; exit 1; }

########################
# SETTINGS RESOLUTION
########################
resolve_settings() {
    case $(( $1 / 1000 )) in
        1) echo "./settings/set_49K_C.dat" ;;
        2) echo "./settings/set_49K_Au.dat" ;;
        3) echo "./settings/set_49K_Be.dat" ;;
        5) echo "./settings/set_51K_Be.dat" ;;
        6) echo "./settings/set_51K_Au.dat" ;;
        *) echo "" ;;
    esac
}

########################
# SUMMARY INIT
########################
echo -e "RUN\tSTATUS\tRUNTIME(s)\tOUTPUT" > "$SUMMARY"

########################
# WORKER FUNCTION
########################
run_single() {
    local RUN=$1
    local START END RUNTIME STATUS
    local SETTINGS OUTFILE LOGFILE

    SETTINGS=$(resolve_settings "$RUN")
    OUTFILE="${OFILE_PREFIX}${RUN}${OFILE_SUFFIX}"
    LOGFILE="${LOGDIR}/run_${RUN}.log"

    if [[ -z "$SETTINGS" ]]; then
        echo -e "${RUN}\tNO_SETTINGS\t0\t-" >> "$SUMMARY"
        return
    fi

    if [[ -f "$OUTFILE" ]]; then
        if [[ $FORCE -eq 1 ]]; then
            echo "⚠️  FORCING overwrite for RUN $RUN"
            rm -f "$OUTFILE"
        else
            echo -e "${RUN}\tSKIPPED\t0\t${OUTFILE}" >> "$SUMMARY"
            return
        fi
    fi


    if [[ $DRY_RUN -eq 1 ]]; then
        echo "[DRY] Metamorphosis -i ${IFILE_PREFIX}${RUN}.ridf.gz -o ${OUTFILE} -s ${SETTINGS}"
        echo -e "${RUN}\tDRY_RUN\t0\t${OUTFILE}" >> "$SUMMARY"
        return
    fi

    START=$(date +%s)

    Metamorphosis \
        -i "${IFILE_PREFIX}${RUN}.ridf.gz" \
        -o "${OUTFILE}" \
        -s "${SETTINGS}" \
        > "$LOGFILE" 2>&1

    STATUS=$?
    END=$(date +%s)
    RUNTIME=$((END - START))

    if [[ $STATUS -eq 0 ]]; then
        echo -e "${RUN}\tOK\t${RUNTIME}\t${OUTFILE}" >> "$SUMMARY"
    else
        echo -e "${RUN}\tFAILED\t${RUNTIME}\t${OUTFILE}" >> "$SUMMARY"
    fi
}

########################
# PARALLEL EXECUTION
########################
SC_START=$(date +%s)

job_count=0

for RUN in "${RUNS[@]}"; do
    run_single "$RUN" &
    ((job_count++))

    if [[ $job_count -ge $MAX_PARALLEL ]]; then
        wait -n
        ((job_count--))
    fi
done

SC_END=$(date +%s)
SC_RUNTIME=$((SC_END - SC_START))

wait

########################
# FINAL REPORT
########################
echo ""
echo "================ SUMMARY ================"
column -t "$SUMMARY"
echo "========================================="

echo ""
echo ""
echo " Total batch runtime: ${SC_RUNTIME} seconds"
