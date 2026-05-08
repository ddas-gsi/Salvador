#!/bin/bash

### Usage examples:
# 13 files:  ./run_disi_parallel.sh 1001 1002 1003 1004 1005 1006 1007 1008 1009 1010 1011 1012 1013
# 18 files:  ./run_disi_parallel.sh 2004 2005 2006 2007 2008 2009 2010 2011 2012 2013 2014 2015 2016 2017 2018 2019 2020 2021
# 05 files:  ./run_disi_parallel.sh 3001 3002 3003 3004 3005
# 34 files:  ./run_disi_parallel.sh 5002 5003 5004 5005 5006 5007 5008 5009 5010 5011 5012 5013 5014 5015 5016 5017 5018 5019 5020 5021 5022 5023 5024 5025 5026 5027 5028 5029 5030 5031 5032 5033 5034 5035
# 106 files: ./run_disi_parallel.sh 6003 6004 6005 6006 6007 6008 6009 6010 6011 6012 6013 6014 6015 6016 6017 6018 6019 6020 6021 6022 6023 6024 6025 6026 6027 6028 6029 6030 6031 6032 6033 6034 6035 6036 6037 6038 6039 6040 6041 6042 6043 6044 6045 6046 6047 6048 6049 6050 6051 6052 6053 6054 6055 6056 6057 6058 6059 6060 6061 6062 6063 6064 6065 6066 6067 6068 6069 6070 6071 6072 6073 6074 6075 6076 6077 6078 6079 6080 6081 6082 6083 6084 6085 6086 6087 6088 6089 6090 6091 6092 6093 6094 6095 6096 6097 6098 6099 6100 6101 6102 6103 6104 6105 6106 6107 6108


FILE_SUFFIX="_new"   # for cutfile and output file naming
# FILE_SUFFIX=""   # for cutfile and output file naming

# Check for at least one RUN number
if [ "$#" -lt 1 ]; then
    echo "Usage: $0 RUN_Nbr1 [RUN_Nbr2 ...]"
    exit 1
fi

# ----------- PATH CONFIG -----------
META_DIR="./rootfiles/ddas/meta"

if [ $FILE_SUFFIX == "_new" ]; then
    DISI_DIR="./rootfiles/ddas/disi/new"
else
    DISI_DIR="./rootfiles/ddas/disi"
fi

CUT_DIR="./rootfiles/ddas/cuts/disicuts"
# -----------------------------------

MAX_PARALLEL=10   # CPU 

mkdir -p "$DISI_DIR"

# ----------- FUNCTION --------------
run_single() {
    local RUN_Nbr=$1
    local CUT_FILE

    # Choose cut file by run series
    case $((RUN_Nbr / 1000)) in
        1)
            CUT_FILE="${CUT_DIR}/disi_Cuts_49K_C_1000${FILE_SUFFIX}.root"
            ;;
        2)
            CUT_FILE="${CUT_DIR}/disi_Cuts_49K_Au_2000${FILE_SUFFIX}.root"
            ;;
        3)
            CUT_FILE="${CUT_DIR}/disi_Cuts_49K_Be_3000${FILE_SUFFIX}.root"
            ;;
        5)
            CUT_FILE="${CUT_DIR}/disi_Cuts_51K_Be_5000${FILE_SUFFIX}.root"
            ;;
        6)
            CUT_FILE="${CUT_DIR}/disi_Cuts_51K_Au_6000${FILE_SUFFIX}.root"
            ;;
        *)
            echo "❌ No cut file defined for RUN $RUN_Nbr"
            return 1
            ;;
    esac

    echo "--------------------------------------------"
    echo "RUN: $RUN_Nbr"
    echo "CUT FILE: $CUT_FILE"
    echo "--------------------------------------------"

    Disintegration \
        -i "${META_DIR}/meta_${RUN_Nbr}.root" \
        -o "${DISI_DIR}/disi_${RUN_Nbr}${FILE_SUFFIX}.root" \
        -c "${CUT_FILE}"

    echo "✅ Finished RUN $RUN_Nbr"
    echo ""
}
# -----------------------------------

job_count=0

for RUN_Nbr in "$@"; do
    run_single "$RUN_Nbr" &     # background job
    ((job_count++))

    if [[ $job_count -ge $MAX_PARALLEL ]]; then
        wait -n                # wait for any job to finish
        ((job_count--))
    fi
done

wait
echo "✅✅ All Disintegration jobs finished."
