#!/bin/bash
PROF_CFG_FILE=/var/log/npu/conf/profiling/profile.cfg
PROF_Q=$1
PROF_HOSTDIR=$2

if [ ! -f ${PROF_CFG_FILE} ]; then
    echo "Can not find profiling config file: ${PROF_CFG_FILE}"
    exit 1
fi
NUM_RESLUT_DIR=$(grep -n "\"result_dir\": \"" ${PROF_CFG_FILE} | head -1 | cut -d ":" -f 1)
if [ -z "${NUM_RESLUT_DIR}" ]; then
    echo "Can not find [\"result_dir\":] in ${PROF_CFG_FILE}"
    exit 1
fi

RESULT_DIR=$(grep "\"result_dir\": \"" ${PROF_CFG_FILE} | awk -F "\"result_dir\": \"" '{print $2}' | awk -F "\", " '{print $1}')

if [ -z "${PROF_Q}" ] || [ -z "${PROF_HOSTDIR}" ]; then
    echo -e "Parameters failed!\n\texample: bash profConf.sh -q --hostDir"
    exit 1
fi
if [ "${PROF_Q}" != "-q" ] || [ "${PROF_HOSTDIR}" != "--hostDir" ]; then
    echo -e "Command not support now! \n\texample: bash profConf.sh -q --hostDir"
    exit 1
else
    echo -e "${RESULT_DIR}"
    exit 0
fi
