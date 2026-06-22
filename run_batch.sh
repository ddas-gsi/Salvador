#!/usr/bin/env bash

##### USAGE:
# MAX_PARALLEL=8 ./run_batch.sh --runs runs.txt
# ./run_batch.sh --runs runs.txt --suffix "_v3.root" --force

# ./run_batch.sh --runs runFiles/runs_1000_p1.csv --summary summaryFiles/summary_1000_p1.tsv --force
# ./run_batch.sh --runs runFiles/runs_1000_p2.csv --summary summaryFiles/summary_1000_p2.tsv --force
# ./run_batch.sh --runs runFiles/runs_2000_p1.csv --summary summaryFiles/summary_2000_p1.tsv --force
# ./run_batch.sh --runs runFiles/runs_2000_p2.csv --summary summaryFiles/summary_2000_p2.tsv --force
# ./run_batch.sh --runs runFiles/runs_3000_p1.csv --summary summaryFiles/summary_3000_p1.tsv --force
# ./run_batch.sh --runs runFiles/runs_5000_p1.csv --summary summaryFiles/summary_5000_p1.tsv --force
# ./run_batch.sh --runs runFiles/runs_5000_p2.csv --summary summaryFiles/summary_5000_p2.tsv --force
# ./run_batch.sh --runs runFiles/runs_5000_p3.csv --summary summaryFiles/summary_5000_p3.tsv --force
# ./run_batch.sh --runs runFiles/runs_6000_p1.csv --summary summaryFiles/summary_6000_p1.tsv --force
# ./run_batch.sh --runs runFiles/runs_6000_p2.csv --summary summaryFiles/summary_6000_p2.tsv --force
# ./run_batch.sh --runs runFiles/runs_6000_p3.csv --summary summaryFiles/summary_6000_p3.tsv --force

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
SUMMARY="./summaryFiles/summary.tsv"

FORCE=0

MAX_PARALLEL=${MAX_PARALLEL:-10}      # Default 10 parallel jobs
DRY_RUN=0
RUNFILE=""

RUN_DISI=0
DISI_DRY_RUN=0
DISI_SUMMARY="./summaryFiles/summary_disi.tsv"
CUT_DIR="/u/ddas/software/Salvador/rootfiles/ddas/cuts/disicuts"
DISI_CUT_FILE_SUFFIX=""
DISI_DIR="/u/ddas/software/Salvador/rootfiles/ddas/disi"

mkdir -p "$ODIR" "$LOGDIR"

########################
# ARGUMENT PARSING
########################
usage() {
    echo "Usage:"
    echo "  $0 --runs runs.txt|runs.csv [--suffix SUF] [--dry-run] [--force] [--summary FILE] [--disi] [--dsum DISI_SUMMARY_FILE] [--disi-dry]"
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
        --disi)
            RUN_DISI=1
            shift
            ;;
        --dsum)
            DISI_SUMMARY="$2"
            shift 2
            ;;
        --disi-dry)
            DISI_DRY_RUN=1
            shift
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

resolve_CutFiles(){
    case $(( $1 / 1000)) in
        1) echo "${CUT_DIR}/disi_Cuts_49K_C_1000${DISI_CUT_FILE_SUFFIX}.root" ;;
        2) echo "${CUT_DIR}/disi_Cuts_49K_Au_2000${DISI_CUT_FILE_SUFFIX}.root" ;;
        3) echo "${CUT_DIR}/disi_Cuts_49K_Be_3000${DISI_CUT_FILE_SUFFIX}.root" ;;
        5) echo "${CUT_DIR}/disi_Cuts_51K_Be_5000${DISI_CUT_FILE_SUFFIX}.root" ;;
        6) echo "${CUT_DIR}/disi_Cuts_51K_Au_6000${DISI_CUT_FILE_SUFFIX}.root" ;;
        *) echo "" ;;
    esac
}

########################
# SUMMARY INIT
########################
echo -e "RUN\tSTATUS\tRUNTIME(s)\tOUTPUT\tDATETIME" > "$SUMMARY"

if [[ $RUN_DISI -eq 1 ]]; then   
    echo -e "RUN\tSTATUS\tRUNTIME(s)\tOUTPUT\tDATETIME" > "$DISI_SUMMARY"
fi

########################
# WORKER FUNCTION
########################
run_single() {
    local RUN=$1
    local START END RUNTIME DT STATUS DISI_START DISI_END DISI_RUNTIME DISI_DT DISI_STATUS
    local SETTINGS OUTFILE LOGFILE CUTFILE DISI_INFILE DISI_OUTFILE DISI_LOGFILE

    SETTINGS=$(resolve_settings "$RUN")
    OUTFILE="${OFILE_PREFIX}${RUN}${OFILE_SUFFIX}"
    LOGFILE="${LOGDIR}/run_${RUN}.log"

    if [[ -z "$SETTINGS" ]]; then
        echo -e "${RUN}\tNO_SETTINGS\t0\t-\t-" >> "$SUMMARY"
        return
    fi

    if [[ -f "$OUTFILE" ]]; then
        if [[ $FORCE -eq 1 ]]; then
            echo "⚠️  FORCING overwrite for RUN $RUN"
            rm -f "$OUTFILE"
        else
            echo -e "${RUN}\tSKIPPED\t0\t${OUTFILE}\t-" >> "$SUMMARY"
            return
        fi
    fi


    if [[ $DRY_RUN -eq 1 ]]; then
        echo "[DRY] Metamorphosis -i ${IFILE_PREFIX}${RUN}.ridf.gz -o ${OUTFILE} -s ${SETTINGS}"
        echo -e "${RUN}\tDRY_RUN\t0\t${OUTFILE}\t-" >> "$SUMMARY"
        STATUS=0
    else
        START=$(date +%s)

        Metamorphosis \
            -i "${IFILE_PREFIX}${RUN}.ridf.gz" \
            -o "${OUTFILE}" \
            -s "${SETTINGS}" \
            > "$LOGFILE" 2>&1

        STATUS=$?
        END=$(date +%s)
        DT=$(date)
        RUNTIME=$((END - START))

        if [[ $STATUS -eq 0 ]]; then
            echo -e "${RUN}\tOK\t${RUNTIME}\t${OUTFILE}\t${DT}" >> "$SUMMARY"
        else
            echo -e "${RUN}\tFAILED\t${RUNTIME}\t${OUTFILE}\t${DT}" >> "$SUMMARY"
        fi
    fi

    ########################
    # Optional DISI Run
    ########################
    if [[ $RUN_DISI -eq 1 && $STATUS -eq 0 ]]; then
        
        sleep 10

        CUTFILE=$(resolve_CutFiles "$RUN")
        DISI_INFILE=$OUTFILE
        DISI_OUTFILE="${DISI_DIR}/disi_${RUN}${DISI_CUT_FILE_SUFFIX}.root"
        DISI_LOGFILE="${LOGDIR}/disi_run_${RUN}.log"

        if [[ -z "$CUTFILE" ]]; then
            echo -e "${RUN}\tNO_CUTFILE\t0\t-\t-" >> "$DISI_SUMMARY"
            return
        fi

        if [[ $DISI_DRY_RUN -eq 1 ]]; then
            echo "[DRY] Disintegration -i ${DISI_INFILE} -o ${DISI_OUTFILE} -c ${CUTFILE}"
            echo -e "${RUN}\tDRY_RUN\t0\t${DISI_OUTFILE}\t-" >> "$DISI_SUMMARY"
            return
        fi

        echo "--------------------------------------------"
        echo "RUNNING Disi for IN FILE: $DISI_INFILE"
        echo "CUT FILE: $CUTFILE"
        echo "--------------------------------------------"

        DISI_START=$(date +%s)

        Disintegration \
            -i "${DISI_INFILE}" \
            -o "${DISI_OUTFILE}" \
            -c "${CUTFILE}" \
            > "$DISI_LOGFILE" 2>&1
        
        DISI_STATUS=$?
        DISI_END=$(date +%s)
        DISI_DT=$(date)
        DISI_RUNTIME=$((DISI_END - DISI_START))

        if [[ $DISI_STATUS -eq 0 ]]; then
            echo -e "${RUN}\tOK\t${DISI_RUNTIME}\t${DISI_OUTFILE}\t${DISI_DT}" >> "$DISI_SUMMARY"
        else
            echo -e "${RUN}\tFAILED\t${DISI_RUNTIME}\t${DISI_OUTFILE}\t${DISI_DT}" >> "$DISI_SUMMARY"
        fi

    fi
}

########################
# CLEANUP HANDLER 
########################
# Handle Ctrl+C 
cleanup() {
    echo ""
    echo "❌ Ctrl+C detected. Stopping all running Disintegration jobs..."

    # Kill all child jobs of this script
    jobs -p | xargs -r kill
    
    wait
    echo "❌ All jobs terminated."
    exit 1
}

trap cleanup SIGINT SIGTERM
# ----------------------

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

wait

SC_END=$(date +%s)
SC_RUNTIME=$((SC_END - SC_START))

########################
# FINAL REPORT
########################
echo ""
echo "================ SUMMARY ================"
column -t "$SUMMARY"
echo "========================================="


if [[ $RUN_DISI -eq 1 ]]; then
    echo ""
    echo "============= DISI SUMMARY =============="
    column -t "$DISI_SUMMARY"
    echo "========================================="
fi


echo ""
echo ""
echo " Total batch runtime: ${SC_RUNTIME} seconds"
