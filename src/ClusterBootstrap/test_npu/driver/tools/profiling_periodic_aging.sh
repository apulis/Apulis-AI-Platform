#!/bin/bash

DEBUG=0
SCRIPT_PATH=$(dirname "$(readlink -f "$0")")
SCRIPT_PATH=${SCRIPT_PATH}"/profConf.sh"
PROFILING_CFG_DIR="/var/log/npu/conf/profiling/"
AGING_CONF_FILE=${PROFILING_CFG_DIR}"profiling_aging.conf"
AGING_SERVICE_LOG=${PROFILING_CFG_DIR}"profiling_aging.log"
AGING_SERVICE_LOG_OLD=${PROFILING_CFG_DIR}"profiling_aging.old"
PROFILING_DATA_DIR="/var/log/npu/profiling/"
MAX_AGING_SERVICE_LOG_LENGTH=$((50*1024*1024))
DEFAULT_AGING_THRESHOLD=90
DEFAULT_AGING_AMOUNT=10
FOR_PERCENTAGE=100
PROFILING_AGING_CAPACITY=0
PROFILING_DISK_CAPACITY=0
CURRENT_USED_CAPACITY=0
PROFILING_AGING_THRESHOLD=0
PROFILING_AGING_AMOUNT=0
counter=0
G_FIND_FLAG=0
declare -a G_MAP=()
MNT_INFO_FILE=${PROFILING_CFG_DIR}"mnt_info_file.log"

function PrintLog(){
    if [ ! -f $AGING_SERVICE_LOG ]; then
        touch $AGING_SERVICE_LOG
        chmod -f 600 $AGING_SERVICE_LOG
    fi
    fileMode=$(stat -c %a $AGING_SERVICE_LOG)
    if [ "${fileMode}" -ne "600" ]; then
        chmod -f 600 $AGING_SERVICE_LOG
    fi
    echo "$(date +%Y-%m-%d-%H:%M:%S) $1" >> $AGING_SERVICE_LOG
}

function DebugPrint(){
    if [[ "$DEBUG" -eq 1 ]]
    then
        echo "$1"
    fi
}

function GetProfilingDataPath(){
    PROFILING_DATA_DIR=$(/bin/bash ${SCRIPT_PATH} -q --hostDir)
    if [ $? -ne 0 ]; then
        PrintLog "[ERROR] get profiling data path error"
        exit 1
    fi
    if [ -z "$PROFILING_DATA_DIR" ]; then
        PrintLog "[ERROR] profiling data path is empty"
        exit 1
    fi
}

function RemoveFile(){
    local needRemoveFile=$1
    if [ -f ${needRemoveFile} ]; then
        local fileSize=($(du -s "${needRemoveFile}" | awk '{ print $1 }'))
        if [ ! -z ${fileSize} ]; then
            ((fileSize=fileSize*1024))
            ((totalRemoveSize=totalRemoveSize+fileSize))
            DebugPrint "Aging files:${needRemoveFile}, fileSize: ${fileSize}, totalRemoveSize:${totalRemoveSize}"
            rm -rf "${needRemoveFile:?}"
        else
            PrintLog "[WARN] NeedRemoveFile:${needRemoveFile} invalid"
        fi
    fi
}

function AgingFilesFromFile(){
    totalRemoveSize=0
    local oldRecordFileName="${PROFILING_CFG_DIR}aging/prof_aging_old_record"
    local fileSliceNamePrefix="${PROFILING_CFG_DIR}aging/prof_aging_file_record.slice_"
    local needRemoveSize=$1
    if [ ! -f ${oldRecordFileName} ]; then
        PrintLog "[WARN] prof_aging_old_record file is not exist"
        touch ${oldRecordFileName}
        echo "0" > ${oldRecordFileName}
        chmod -f 600 ${oldRecordFileName}
    fi
    local sliceNum=$(cat ${oldRecordFileName})
    if [ -z ${sliceNum} ] || [[ ! $sliceNum =~ ^([0-9]|[1-9]{1}[0-9]{0,19})$ ]]; then
        PrintLog "[ERR ] Slice num:${sliceNum} invalid"
        exit 1
    fi
    PrintLog "[INFO] Old slice num:${sliceNum}"
    local fileSliceName="${fileSliceNamePrefix}${sliceNum}.DONE"
    if [ ! -f "${fileSliceNamePrefix}${sliceNum}" ] && [ ! -f "${fileSliceName}" ]; then
        PrintLog "[ERR ] The file needed to aging is not exist, file:${fileSliceNamePrefix}${sliceNum}"
        exit 1
    fi
    local totalLineNum=0
    while [ -f "${fileSliceName}" ]
    do
        local lineNum=0
        while read line
        do
            RemoveFile "${line}.done"
            RemoveFile "${line}.zip"
            RemoveFile "${line}"
            let lineNum+=1
            let totalLineNum+=1
            if [ ${totalRemoveSize} -ge "${needRemoveSize}" ]; then
                sed -i "1,${lineNum}"d ${fileSliceName}
                echo "${sliceNum}" > ${oldRecordFileName}
                PrintLog "[INFO] New slice num:${sliceNum}, totalLineNum:${totalLineNum}, old success"
                PrintLog "[INFO] TotalRemoveSize:${totalRemoveSize}, needRemoveSize:${needRemoveSize}, old success"
                exit 0
            fi
        done <<< "$(cat ${fileSliceName})"
        PrintLog "[INFO] totalLineNum:${totalLineNum}, delete ${fileSliceName}"
        rm -rf "${fileSliceName:?}"
        let sliceNum+=1
        fileSliceName="${fileSliceNamePrefix}${sliceNum}.DONE"
    done
    echo "${sliceNum}" > ${oldRecordFileName}
    PrintLog "[INFO] New slice num:${sliceNum}, totalLineNum:${totalLineNum}"
    PrintLog "[INFO] TotalRemoveSize:${totalRemoveSize}, needRemoveSize:${needRemoveSize}"
}

#check if the aging service log is larger than 50M, clear it.
function CheckAgingServiceLogFile(){
    if [ -f $AGING_SERVICE_LOG ] && [ "$(ls -l $AGING_SERVICE_LOG | awk '{ print $5 }')" -gt $MAX_AGING_SERVICE_LOG_LENGTH ]
    then
        mv -f $AGING_SERVICE_LOG $AGING_SERVICE_LOG_OLD
        chmod -f 400 $AGING_SERVICE_LOG_OLD
        PrintLog "[INFO] clear aging service log file"
    fi
}

function GetDiskInfoByKey()
{
    objdir="$1"

    while IFS= read -r line
    do
        mntdir="$(echo "${line}" | awk '{print $6}')"
        if [ "${mntdir}" == "${objdir}" ]; then
            if [ -d "${PROFILING_DATA_DIR}" ]; then
                PROFILING_DISK_CAPACITY="$(echo "${line}" | awk '{print $2}')"
                PROFILING_DISK_CAPACITY=$(echo "${PROFILING_DISK_CAPACITY}" | awk '{print $1*1024}')
                CURRENT_USED_CAPACITY="$(echo "${line}" | awk '{print $3}')"
                CURRENT_USED_CAPACITY=$(echo "${CURRENT_USED_CAPACITY}" | awk '{print $1*1024}')
                G_FIND_FLAG=1
                break
            fi
        fi
    done < "${MNT_INFO_FILE}"
}

function GetDiskCapAndUsedItem()
{
    # get the current mnt dir info
    [ -f "${MNT_INFO_FILE}" ] && rm -f "${MNT_INFO_FILE}"
    df -k | awk 'NR>1{print}' | while IFS= read -r line; do echo "${line}"; done > "${MNT_INFO_FILE}"

    #get the all subdirectories
    final="${PROFILING_DATA_DIR: -1}"
    if [ "${final}" == "/" ]; then
        strkey="${PROFILING_DATA_DIR%/*}"
    else
        strkey="${PROFILING_DATA_DIR}"
    fi
    G_MAP[${counter}]="${strkey}"
    counter=$((counter+1))
    while :
    do
        strkey="${strkey%/*}"
        if [ "x${strkey}" != "x" ]; then
            G_MAP[${counter}]="${strkey}"
            counter=$((counter+1))
        else
            G_MAP[${counter}]="/"
            break
        fi
    done

    #get disk info by objdir
    length=${#G_MAP[@]}
    for ((i=0; i<length; i++))
    do
        objkey="${G_MAP[i]}"
        if [ "x${objkey}" != "x" ]; then
            GetDiskInfoByKey "${objkey}"
            [ "${G_FIND_FLAG}" == "1" ] && break
        fi
    done

    [ -f "${MNT_INFO_FILE}" ] && rm -f "${MNT_INFO_FILE}"
}

function GetProfilingConfItem()
{
    GetProfilingDataPath
    PrintLog "[INFO] PROFILING_DATA_DIR:${PROFILING_DATA_DIR}"

    GetDiskCapAndUsedItem
    PrintLog "[INFO] PROFILING_DISK_CAPACITY:${PROFILING_DISK_CAPACITY}, CURRENT_USED_CAPACITY:${CURRENT_USED_CAPACITY}"

    PROFILING_AGING_THRESHOLD=$(grep "^AGING_THRESHOLD\s*=" $AGING_CONF_FILE |awk -F'=' '{ print $2 }' | sed s/[[:space:]]//g)
    if [ ! -n "$PROFILING_AGING_THRESHOLD" ]; then
        PrintLog "[WARNING] PROFILING_AGING_THRESHOLD has no value, set default value:${DEFAULT_AGING_THRESHOLD}%"
        PROFILING_AGING_THRESHOLD=$DEFAULT_AGING_THRESHOLD
    elif [[ ! $PROFILING_AGING_THRESHOLD =~ ^[1-9]{1}[0-9]{0,1}$ ]]; then
        PrintLog "[ERROR] PROFILING_AGING_THRESHOLD:${PROFILING_AGING_THRESHOLD} is invalid!"
        exit 1
    fi
    PrintLog "[INFO] PROFILING_AGING_THRESHOLD:${PROFILING_AGING_THRESHOLD}%"
    PROFILING_AGING_CAPACITY=$((PROFILING_DISK_CAPACITY*PROFILING_AGING_THRESHOLD/FOR_PERCENTAGE))
    if [ "$PROFILING_AGING_CAPACITY" -eq 0 ]; then
        PrintLog "[ERROR] PROFILING_AGING_CAPACITY is 0"
        exit 1
    fi
    PrintLog "[INFO] PROFILING_AGING_CAPACITY:${PROFILING_AGING_CAPACITY}"
    PROFILING_AGING_AMOUNT=$(grep "^AGING_AMOUNT\s*=" $AGING_CONF_FILE |awk -F'=' '{ print $2 }' | sed s/[[:space:]]//g)
    if [ ! -n "$PROFILING_AGING_AMOUNT" ]; then
        PrintLog "[WARNING] PROFILING_AGING_AMOUNT has no value, set default value:${DEFAULT_AGING_AMOUNT}%"
        PROFILING_AGING_AMOUNT=$DEFAULT_AGING_AMOUNT
    elif [[ ! $PROFILING_AGING_AMOUNT =~ ^([0-9]|[1-4][0-9]|50)$ ]]; then
        PrintLog "[ERROR] PROFILING_AGING_AMOUNT:${PROFILING_AGING_AMOUNT} is invalid!"
        exit 1
    fi
    PrintLog "[INFO] PROFILING_AGING_AMOUNT:${PROFILING_AGING_AMOUNT}%"
}

function ProcessProfilingAging()
{
    GetProfilingConfItem

    if [ "${CURRENT_USED_CAPACITY}" -le "${PROFILING_AGING_CAPACITY}" ]; then
        PrintLog "[INFO] CURRENT_USED_CAPACITY:${CURRENT_USED_CAPACITY}, PROFILING_AGING_CAPACITY:${PROFILING_AGING_CAPACITY}"
        PrintLog "[INFO] The aging threshold is not reached and no aging is required."
        exit 0
    fi
    totalNeedRemoveSize=$((CURRENT_USED_CAPACITY-PROFILING_AGING_CAPACITY+PROFILING_AGING_CAPACITY*PROFILING_AGING_AMOUNT/FOR_PERCENTAGE))
    PrintLog "[INFO] Need aging file size:${totalNeedRemoveSize}"
    local profDataDirUsedCapacity="$(du -s "$PROFILING_DATA_DIR" | awk '{ print $1 }')"
    AgingFilesFromFile ${totalNeedRemoveSize}
}

function main(){
    PrintLog "[INFO] periodic aging service starts."
    CheckAgingServiceLogFile
    #start to process profiling aging
    ProcessProfilingAging
}

# excute the main function
main
