#!/bin/bash

DEBUG=0
SCRIPT_PATH=$(dirname "$(readlink -f "$0")")
SCRIPT_PATH=${SCRIPT_PATH}"/adxConf.sh"
ADX_CFG_DIR="/var/log/npu/conf/adx/"
AGING_CONF_FILE=${ADX_CFG_DIR}"adx_aging.conf"
AGING_SERVICE_LOG=${ADX_CFG_DIR}"adx_aging.log"
AGING_SERVICE_LOG_OLD=${ADX_CFG_DIR}"adx_aging.old"
ADX_DATA_DIR="/var/log/npu/ide_daemon/dump"
MAX_AGING_SERVICE_LOG_LENGTH=$((50*1024*1024))
DEFAULT_AGING_THRESHOLD=90
DEFAULT_AGING_AMOUNT=10
FOR_PERCENTAGE=100
ADX_AGING_CAPACITY=0
ADX_DISK_CAPACITY=0
CURRENT_USED_CAPACITY=0
ADX_AGING_THRESHOLD=0
ADX_AGING_AMOUNT=0
counter=0
G_FIND_FLAG=0
declare -a G_MAP=()
MNT_INFO_FILE="/var/log/npu/ide_daemon/run/mnt_info_file.log"

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

function GetAdxDataPath(){
    ADX_DATA_DIR=$(/bin/bash ${SCRIPT_PATH} -q --hostDir)
    if [ $? -ne 0 ]; then
        PrintLog "[ERROR] get adx data path error"
        exit 1
    fi
    if [ -z "$ADX_DATA_DIR" ]; then
        PrintLog "[ERROR] adx data path is empty"
        exit 1
    fi
}

function RemoveAllFiles(){
    local num=${#agingFileList[*]}
    local i=0
    local removeFileNum=0
    removeFilesSize=0
    local agingDir=${ADX_DATA_DIR}
    while [ $i -lt "$num" ]
    do
        if [ "${removeFilesSize}" -ge "${needRemoveSize}" ]; then
            DebugPrint "RemoveAllFiles Aging success, removeFilesSize:${removeFilesSize}, needRemoveSize:${needRemoveSize}, i:${i}, removeFileNum:${removeFileNum}"
            PrintLog "[INFO] RemoveAllFiles Aging success, removeFilesSize:${removeFilesSize}, needRemoveSize:${needRemoveSize}, i:${i}, removeFileNum:${removeFileNum}"
            exit 0
        fi
        if [[ ${agingFileList[$i]} =~ :$ ]]; then
            agingDir=$(echo "${agingFileList[$i]}"|sed 's/:$//g')
            DebugPrint "RemoveAllFiles fileList: ${agingDir} is dir"
            let i+=1
            continue
        fi
        agingFile="${agingDir}/${agingFileList[$i]}"
        if [ -d "${agingFile}" ]; then
            DebugPrint "RemoveAllFiles agingFile: ${agingFile} is dir or is in container dir or not slice file"
            let i+=1
            continue
        fi
        if [ "$(echo "$agingFile" |grep -cE "container")" -eq 1 ] && [ "$(echo "$agingFile" |grep -cE "slice_")" -ne 1 ]; then
            DebugPrint "RemoveAllFiles agingFile: ${agingFile} is in container dir and is not slice file"
            let i+=1
            continue
        fi
        local fileSize=($(du -s "${agingFile}" | awk '{ print $1 }'))
        ((fileSize=fileSize*1024))
        ((removeFilesSize=removeFilesSize+fileSize))
        DebugPrint "RemoveAllFiles aging files: ${agingFile}, fileSize: ${fileSize}, removeFilesSize:${removeFilesSize}"
        rm -rf "${agingFile}"
        let i+=1
        let removeFileNum+=1
    done
    DebugPrint "RemoveAllFiles removeFilesSize:${removeFilesSize}, needRemoveSize:${needRemoveSize}, i:${i}, removeFileNum:${removeFileNum}"
    PrintLog "[INFO] RemoveAllFiles removeFilesSize:${removeFilesSize}, needRemoveSize:${needRemoveSize}, i:${i}, removeFileNum:${removeFileNum}"
}

function ClearRemoveFile(){
    local rmPath=$1
    until [ ${ADX_DATA_DIR} = ${rmPath} ]; do
        if [ -f ${rmPath} ]; then
            rm ${rmPath:?} >> /dev/null 2>&1
        fi

        if [ -d ${rmPath} ]; then
            rmdir ${rmPath:?} >> /dev/null 2>&1
            if [ $? -ne 0 ]; then
                break;
            fi
        fi
        rmPath=${rmPath%/*}
    done
}

function RemoveFile(){
    local needRemoveFile=$1
    if [ -f ${needRemoveFile} ]; then
        local fileSize=($(du -s "${needRemoveFile}" | awk '{ print $1 }'))
        if [ ! -z ${fileSize} ]; then
            ((fileSize=fileSize*1024))
            ((totalRemoveSize=totalRemoveSize+fileSize))
            DebugPrint "Aging files:${needRemoveFile}, fileSize: ${fileSize}, totalRemoveSize:${totalRemoveSize}"
            ClearRemoveFile "${needRemoveFile}"
        else
            PrintLog "[WARN] NeedRemoveFile:${needRemoveFile} invalid"
        fi
    fi
}

function AgingFilesFromFile(){
    totalRemoveSize=0
    local oldRecordFileName="${ADX_DATA_DIR}/record/adx_dump_old_record"
    local fileSliceNamePrefix="${ADX_DATA_DIR}/record/adx_slice_"
    local needRemoveSize=$1
    if [ ! -f ${oldRecordFileName} ]; then
        PrintLog "[WARN] adx_aging_old_record file is not exist"
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
    local fileSliceName="${fileSliceNamePrefix}${sliceNum}.done"
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
        fileSliceName="${fileSliceNamePrefix}${sliceNum}.done"
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
            if [ -d "${ADX_DATA_DIR}" ]; then
                ADX_DISK_CAPACITY="$(echo "${line}" | awk '{print $2}')"
                ADX_DISK_CAPACITY=$(echo "${ADX_DISK_CAPACITY}" | awk '{print $1*1024}')
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
    final="${ADX_DATA_DIR: -1}"
    if [ "${final}" == "/" ]; then
        strkey="${ADX_DATA_DIR%/*}"
    else
        strkey="${ADX_DATA_DIR}"
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

function GetAdxConfItem()
{
    GetAdxDataPath
    PrintLog "[INFO] ADX_DATA_DIR:${ADX_DATA_DIR}"

    GetDiskCapAndUsedItem
    PrintLog "[INFO] ADX_DISK_CAPACITY:${ADX_DISK_CAPACITY}, CURRENT_USED_CAPACITY:${CURRENT_USED_CAPACITY}"

    ADX_AGING_THRESHOLD=$(grep "^AGING_THRESHOLD\s*=" $AGING_CONF_FILE |awk -F'=' '{ print $2 }' | sed s/[[:space:]]//g)
    if [ ! -n "$ADX_AGING_THRESHOLD" ]; then
        PrintLog "[WARNING] ADX_AGING_THRESHOLD has no value, set default value:${DEFAULT_AGING_THRESHOLD}%"
        ADX_AGING_THRESHOLD=$DEFAULT_AGING_THRESHOLD
    elif [[ ! $ADX_AGING_THRESHOLD =~ ^[1-9]{1}[0-9]{0,1}$ ]]; then
        PrintLog "[ERROR] ADX_AGING_THRESHOLD:${ADX_AGING_THRESHOLD} is invalid!"
        exit 1
    fi
    PrintLog "[INFO] ADX_AGING_THRESHOLD:${ADX_AGING_THRESHOLD}%"
    ADX_AGING_CAPACITY=$((ADX_DISK_CAPACITY*ADX_AGING_THRESHOLD/FOR_PERCENTAGE))
    if [ "$ADX_AGING_CAPACITY" -eq 0 ]; then
        PrintLog "[ERROR] ADX_AGING_CAPACITY is 0"
        exit 1
    fi
    PrintLog "[INFO] ADX_AGING_CAPACITY:${ADX_AGING_CAPACITY}"
    ADX_AGING_AMOUNT=$(grep "^AGING_AMOUNT\s*=" $AGING_CONF_FILE |awk -F'=' '{ print $2 }' | sed s/[[:space:]]//g)
    if [ ! -n "$ADX_AGING_AMOUNT" ]; then
        PrintLog "[WARNING] ADX_AGING_AMOUNT has no value, set default value:${DEFAULT_AGING_AMOUNT}%"
        ADX_AGING_AMOUNT=$DEFAULT_AGING_AMOUNT
    elif [[ ! $ADX_AGING_AMOUNT =~ ^([0-9]|[1-4][0-9]|50)$ ]]; then
        PrintLog "[ERROR] ADX_AGING_AMOUNT:${ADX_AGING_AMOUNT} is invalid!"
        exit 1
    fi
    PrintLog "[INFO] ADX_AGING_AMOUNT:${ADX_AGING_AMOUNT}%"
}

function ProcessAdxAging()
{
    GetAdxConfItem

    if [ "${CURRENT_USED_CAPACITY}" -le "${ADX_AGING_CAPACITY}" ]; then
        PrintLog "[INFO] CURRENT_USED_CAPACITY:${CURRENT_USED_CAPACITY}, ADX_AGING_CAPACITY:${ADX_AGING_CAPACITY}"
        PrintLog "[INFO] The aging threshold is not reached and no aging is required."
        exit 0
    fi
    totalNeedRemoveSize=$((CURRENT_USED_CAPACITY-ADX_AGING_CAPACITY+ADX_AGING_CAPACITY*ADX_AGING_AMOUNT/FOR_PERCENTAGE))
    PrintLog "[INFO] Need aging file size:${totalNeedRemoveSize}"
    local profDataDirUsedCapacity="$(du -s "$ADX_DATA_DIR" | awk '{ print $1 }')"
    AgingFilesFromFile ${totalNeedRemoveSize}
}

function main(){
    PrintLog "[INFO] periodic aging service starts."
    CheckAgingServiceLogFile
    #start to process adx aging
    ProcessAdxAging
}

# excute the main function
main
