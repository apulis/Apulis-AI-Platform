#!/bin/sh
set -x

echo "VISIBLE_IDS: ${VISIBLE_IDS}"
device_group=${VISIBLE_IDS}
device_num=$#

export PYTHONPATH=/usr/local/HiAI/runtime/python3.6/site-packages/topi.egg:/usr/local/HiAI/runtime/python3.6/site-packages/te.egg:/usr/local/HiAI/runtime/ops/op_impl/built-in/ai_core/tbe
export LD_LIBRARY_PATH=/usr/local/lib/:/usr/local/python3/lib:/usr/lib/:/usr/local/HiAI/runtime/lib64/:/usr/local/HiAI/driver/lib64/
export DDK_VERSION_PATH=/usr/local/HiAI/runtime/ddk_info
export PATH=/usr/local/HiAI/runtime/ccec_compiler/bin:$PATH
#tdt&fmk
export ACCESS_KEY_ID=HCY8BGCN1YM5ZWYOK1MH
export SECRET_ACCESS_KEY=9zYwf1uabSQY0JTnFqbUqG7vcfqYBaTdXde2GUcq

#tdt
export S3_USE_HTTPS=0
export S3_VERIFY_SSL=0
export S3_ENDPOINT=10.134.170.106:8080
export AWS_CREDENTIAL_PROFILES_FILE="/d_solution/config/obs_profile_file.cfg"

#fmk
export JOB_ID=123456789
export HEARTBEAT=1
export PROFILING_MODE=false
export AICPU_PROFILING_MODE=false
export RANK_TABLE_FILE=/d_solution/config/hccl.json
export RANK_SIZE=1

TYPE=x
PODNAME_BASE=cloud0

touch /d_solution/main.log
device_index=0
for devcice_phy_id in ${device_group}
do
    if [ x"${TYPE}" = x1 ] ;
    then
        echo "[`date +%Y%m%d-%H:%M:%S`] [INFO] 1 docker includes 1 device " >> /d_solution/main.log
        PODNAME=${PODNAME_BASE}_${devcice_phy_id}
    else
        echo "[`date +%Y%m%d-%H:%M:%S`] [INFO] 1 docker includes more device " >> /d_solution/main.log
        PODNAME=${PODNAME_BASE}
    fi

    echo "[`date +%Y%m%d-%H:%M:%S`] [INFO] start: RANK_ID=${PODNAME} DEVICE_ID=${devcice_phy_id} train.sh ${device_index} & " >> /d_solution/main.log
    RANK_ID=${PODNAME} DEVICE_ID=${devcice_phy_id} /d_solution/train.sh ${device_index}  &
    let device_index++
done

wait
