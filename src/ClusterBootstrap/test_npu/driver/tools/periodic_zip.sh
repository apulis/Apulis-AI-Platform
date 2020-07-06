#!/bin/bash

#### GLOBAL COMMON VARIABLES ####
DEBUG=0
INSTALL_CONFIG_PATH="/lib/davinci.conf"
SCRIPT_PATH_END_STRING="/driver/tools"
NEW_SCRIPT_PATH_END_STRING="/toolkit/bin"
DEFAULT_SCRIPT_PATH="/usr/local/HiAI"
MAX_LOG_PATH_LENGTH=$((10*4097))
MAX_LOG_PATH_NUM=10
DEFAULT_MAX_ZIP_FILE_NUMBER=180
DEFAULT_MAX_DIR_DEPTH=3
ZIP_FILE_NUMBER_LIMIT=1000
DEFAULT_COMPRESS_SWITCH=1

DEFAULT_CONF_FILE="/var/log/npu/conf/slog/ziplog.conf"
CONF_FILE=$DEFAULT_CONF_FILE
ZIP_SERVICE_LOG="/var/log/npu/slog/slogd/zipservicelog"
ZIP_SERVICE_LOG_OLD="/var/log/npu/slog/slogd/zipservicelog.old"
ZIP_SERVICE_LOG_LOCK="/var/log/npu/slog/slogd/.zipservicelog.lock"
MAX_ZIP_SERVICE_LOG_LENGTH=$((5*1024*1024))
DIR_PERMISSION=640
DIR_PERMISSION_READ=440
IS_TO_ZIP_TRUE=1
IS_TO_ZIP_FALSE=0

#minimum lifetime of the readonly log file
LOG_MIN_LIFETIME=60

# Module Index
MODULE_LOG=0
MODULE_BBOX=1
MODULE_PROFILING=2
MODULE_ADX=3

# Name and configuration scripts for each module
MODULE_NAME=("LOG" "BBOX" "PROFILING" "ADX")
MODULE_CONF=("logConf.sh" "bboxConf.sh" "profConf.sh" "adxConf.sh")

#### GLOBAL MODULES VARIABLES ####
#### GLOBAL MODULES VARIABLES FOR LOG ####

#### GLOBAL MODULES VARIABLES FOR BBOX ####
DEFAULT_MAX_ZIP_FILE_NUMBER_BBOX=10
ZIP_FILE_NUMBER_LIMIT_BBOX=80
DEFAULT_MAX_DIR_DEPTH_BBOX=0
DIR_DEPTH_LIMIT_BBOX=0

#### GLOBAL MODULES VARIABLES FOR PROFILING BEGIN####
DEFAULT_MAX_ZIP_FILE_NUMBER_PROFILING=10000
DEFAULT_MAX_DIR_DEPTH_PROFILING=5
MAX_ZIP_FILE_NUMBER_PROFILING=10000000
MAX_DIR_DEPTH_PROFILING=9
#### GLOBAL MODULES VARIABLES FOR PROFILING END####

#### GLOBAL MODULES VARIABLES FOR ADX BEGIN####
DEFAULT_MAX_ZIP_FILE_NUMBER_ADX=10000
DEFAULT_MAX_DIR_DEPTH_ADX=25
MAX_ZIP_FILE_NUMBER_ADX=10000000
MAX_DIR_DEPTH_ADX=30
#### GLOBAL MODULES VARIABLES FOR ADX END####

function PrintLog(){
    local cmd
    cmd="echo $1 >> $ZIP_SERVICE_LOG"
    flock -x $ZIP_SERVICE_LOG_LOCK -c "$cmd"
}

function ErrorLog(){
    PrintLog "[ERROR] [$moduleName] $(date +%Y-%m-%d-%H:%M:%S) $1"
}

function WarningLog(){
    PrintLog "[WARNING] [$moduleName] $(date +%Y-%m-%d-%H:%M:%S) $1"
}

function InfoLog(){
    PrintLog "[INFO] [$moduleName] $(date +%Y-%m-%d-%H:%M:%S) $1"
}

function DebugPrint(){
    if [[ "$DEBUG" -eq 1 ]]
    then
        echo "$1"
    fi
}

# get path of script which can get zipped path
SCRIPT_PATH=`grep "^DAVINCI_HOME_PATH\s*=" $INSTALL_CONFIG_PATH |awk -F'=' '{ print $2 }' | sed s/[[:space:]]//g`
if [ ! -n "$SCRIPT_PATH" ]
then
    PrintLog "[INFO] $(date +%Y-%m-%d-%H:%M:%S) SCRIPT_PATH has no value, use default value $DEFAULT_SCRIPT_PATH"
    SCRIPT_PATH=$DEFAULT_SCRIPT_PATH
fi
# if toolkit/bin/logConf.sh or driver/tools/logConf.sh exit use it, default is /toolkit/bin/logConf.sh
if [ -f "$SCRIPT_PATH$SCRIPT_PATH_END_STRING/logConf.sh" ]; then
    SCRIPT_PATH="$SCRIPT_PATH$SCRIPT_PATH_END_STRING"
elif [ -f "$SCRIPT_PATH$NEW_SCRIPT_PATH_END_STRING/logConf.sh" ]; then
    SCRIPT_PATH="$SCRIPT_PATH$NEW_SCRIPT_PATH_END_STRING"
else
    PrintLog "[ERROR] $(date +%Y-%m-%d-%H:%M:%S) can not find logConf.sh."
    SCRIPT_PATH="$SCRIPT_PATH$NEW_SCRIPT_PATH_END_STRING"
fi

#get the line range of one module
#input: $1: the begin flag e.g. "LOG" for log module
#$2: the end flag e.g. "LOG_END" for log module
#output: begin and end line number of one module
function FindModuleLineNumber(){
    beginFlag=$1
    endFlag=$2
    beginLineNum=$(grep -n "^\[\s*$beginFlag\s*\]" $CONF_FILE | awk -F ":" '{print $1}')
    endLineNum=$(grep -n "^\[\s*$endFlag\s*\]" $CONF_FILE | awk -F ":" '{print $1}')

    if [ ! -n "$beginLineNum" ] || [ ! -n "$endLineNum" ]
    then
        ErrorLog "Lack of module flag fields($beginFlag and $endFlag)!"
        exit 1
    fi
    echo "$beginLineNum"
    echo "$endLineNum"
}

function GetLogConfItem(){
    local beginLine=$1
    local endLine=$2
    MAX_DIR_DEPTH=$(sed -n ''"${beginLine}"','"${endLine}"'p' $CONF_FILE | grep "^MAX_DIR_DEPTH\s*=" |awk -F "=" '{print $2}' |  sed s/[[:space:]]//g)
    if [ ! -n "$MAX_DIR_DEPTH" ]
    then
        WarningLog "the max directory depth is not configured, use default value $DEFAULT_MAX_DIR_DEPTH"
        MAX_DIR_DEPTH=$DEFAULT_MAX_DIR_DEPTH
    elif [[ ! $MAX_DIR_DEPTH =~ ^[0-$DEFAULT_MAX_DIR_DEPTH]{1}$ ]]
    then
        ErrorLog "$MAX_DIR_DEPTH is invalid or exceeds the limit of $DEFAULT_MAX_DIR_DEPTH!"
        exit 1
    fi

    MAX_ZIP_FILE_NUMBER=$(sed -n ''"${beginLine}"','"${endLine}"'p' $CONF_FILE | grep "^MAX_ZIP_FILE_NUMBER\s*=" |awk -F "=" '{print $2}' |  sed s/[[:space:]]//g)
    if [ ! -n "$MAX_ZIP_FILE_NUMBER" ]
    then
        WarningLog "the constrain max zip file number is not configured, use default value $DEFAULT_MAX_ZIP_FILE_NUMBER"
        MAX_ZIP_FILE_NUMBER=$DEFAULT_MAX_ZIP_FILE_NUMBER
    elif [[ ! $MAX_ZIP_FILE_NUMBER =~ ^[1-9]{1}[0-9]{0,3}$ ]] || [ $MAX_ZIP_FILE_NUMBER -gt $ZIP_FILE_NUMBER_LIMIT ]
    then
        ErrorLog "MAX_ZIP_FILE_NUMBER is invalid or exceed the constrain of $ZIP_FILE_NUMBER_LIMIT!"
        exit 1
    fi
}

# get configuration items for BBOX
function GetBboxConfItem(){
    local beginLine=$1
    local endLine=$2
    MAX_DIR_DEPTH=$(sed -n ''"${beginLine}"','"${endLine}"'p' $CONF_FILE | grep "^MAX_DIR_DEPTH\s*=" |awk -F "=" '{print $2}' |  sed s/[[:space:]]//g)
    if [ ! -n "$MAX_DIR_DEPTH" ]
    then
        WarningLog "the max directory depth is not configured, use default value $DEFAULT_MAX_DIR_DEPTH_BBOX"
        MAX_DIR_DEPTH=$DEFAULT_MAX_DIR_DEPTH_BBOX
    elif [[ ! $MAX_DIR_DEPTH =~ ^[0-$DIR_DEPTH_LIMIT_BBOX]{1}$ ]]
    then
        ErrorLog "MAX_DIR_DEPTH is invalid or exceeds the limit of $DIR_DEPTH_LIMIT_BBOX!"
        exit 1
    fi

    MAX_ZIP_FILE_NUMBER=$(sed -n ''"${beginLine}"','"${endLine}"'p' $CONF_FILE | grep "^MAX_ZIP_FILE_NUMBER\s*=" |awk -F "=" '{print $2}' |  sed s/[[:space:]]//g)
    if [ ! -n "$MAX_ZIP_FILE_NUMBER" ]
    then
        WarningLog "the constrain max zip file number is not configured, use default value $DEFAULT_MAX_ZIP_FILE_NUMBER"
        MAX_ZIP_FILE_NUMBER=$DEFAULT_MAX_ZIP_FILE_NUMBER_BBOX
    elif [[ ! $MAX_ZIP_FILE_NUMBER =~ ^[1-9]{1}[0-9]{0,1}$ ]] || [ $MAX_ZIP_FILE_NUMBER -gt $ZIP_FILE_NUMBER_LIMIT_BBOX ]
    then
        ErrorLog "MAX_ZIP_FILE_NUMBER is invalid or exceed the constrain of $ZIP_FILE_NUMBER_LIMIT_BBOX!"
        exit 1
    fi
}

function GetProfilingConfItem(){
    local beginLine=$1
    local endLine=$2
    MAX_DIR_DEPTH=$(sed -n ''"${beginLine}"','"${endLine}"'p' $CONF_FILE | grep "^MAX_DIR_DEPTH\s*=" |awk -F "=" '{print $2}' |  sed s/[[:space:]]//g)
    if [ ! -n "$MAX_DIR_DEPTH" ]
    then
        WarningLog "the max directory depth is not configured, use default value $DEFAULT_MAX_DIR_DEPTH_PROFILING"
        MAX_DIR_DEPTH=$DEFAULT_MAX_DIR_DEPTH_PROFILING
    elif [[ ! $MAX_DIR_DEPTH =~ ^[0-$MAX_DIR_DEPTH_PROFILING]{1}$ ]]
    then
        ErrorLog "MAX_DIR_DEPTH is invalid or exceeds the limit of $DEFAULT_MAX_DIR_DEPTH_PROFILING!"
        exit 1
    fi
    MAX_ZIP_FILE_NUMBER=$(sed -n ''"${beginLine}"','"${endLine}"'p' $CONF_FILE | grep "^MAX_ZIP_FILE_NUMBER\s*=" |awk -F "=" '{print $2}' |  sed s/[[:space:]]//g)
    if [ ! -n "$MAX_ZIP_FILE_NUMBER" ]
    then
        WarningLog "the constrain max zip file number is not configured, use default value $DEFAULT_MAX_ZIP_FILE_NUMBER_PROFILING"
        MAX_ZIP_FILE_NUMBER=$DEFAULT_MAX_ZIP_FILE_NUMBER_PROFILING
    elif [[ ! $MAX_ZIP_FILE_NUMBER =~ ^[1-9]{1}[0-9]{0,7}$ ]] || [ $MAX_ZIP_FILE_NUMBER -gt $MAX_ZIP_FILE_NUMBER_PROFILING ]
    then
        ErrorLog "MAX_ZIP_FILE_NUMBER is invalid or exceed the constrain of $MAX_ZIP_FILE_NUMBER_PROFILING!"
        exit 1
    fi
}

function GetAdxConfItem(){
    local beginLine=$1
    local endLine=$2
    MAX_DIR_DEPTH=$(sed -n ''"${beginLine}"','"${endLine}"'p' $CONF_FILE | grep "^MAX_DIR_DEPTH\s*=" |awk -F "=" '{print $2}' |  sed s/[[:space:]]//g)
    if [ ! -n "$MAX_DIR_DEPTH" ]
    then
        WarningLog "the max directory depth is not configured, use default value $DEFAULT_MAX_DIR_DEPTH_ADX"
        MAX_DIR_DEPTH=$DEFAULT_MAX_DIR_DEPTH_ADX
    elif [[ ! $MAX_DIR_DEPTH =~ ^[1-9]{1}[0-9]{0,1}$ ]] || [ $MAX_DIR_DEPTH -gt $MAX_DIR_DEPTH_ADX ]
    then
        ErrorLog "MAX_DIR_DEPTH is invalid or exceeds the limit of $MAX_DIR_DEPTH_ADX!"
        exit 1
    fi
    MAX_ZIP_FILE_NUMBER=$(sed -n ''"${beginLine}"','"${endLine}"'p' $CONF_FILE | grep "^MAX_ZIP_FILE_NUMBER\s*=" |awk -F "=" '{print $2}' |  sed s/[[:space:]]//g)
    if [ ! -n "$MAX_ZIP_FILE_NUMBER" ]
    then
        WarningLog "the constrain max zip file number is not configured, use default value $DEFAULT_MAX_ZIP_FILE_NUMBER_ADX"
        MAX_ZIP_FILE_NUMBER=$DEFAULT_MAX_ZIP_FILE_NUMBER_ADX
    elif [[ ! $MAX_ZIP_FILE_NUMBER =~ ^[1-9]{1}[0-9]{0,7}$ ]] || [ $MAX_ZIP_FILE_NUMBER -gt $MAX_ZIP_FILE_NUMBER_ADX ]
    then
        ErrorLog "MAX_ZIP_FILE_NUMBER is invalid or exceed the constrain of $MAX_ZIP_FILE_NUMBER_ADX!"
        exit 1
    fi
}

function GetConfItem(){
    local beginLine=$1
    local endLine=$2
    local module=$3

    if [ "$module" -eq $MODULE_PROFILING ]; then
        GetProfilingConfItem "$beginLine" "$endLine"
    elif [ "$module" -eq $MODULE_BBOX ]; then
        GetBboxConfItem "$beginLine" "$endLine"
    elif [ "$module" -eq $MODULE_LOG ]; then
        GetLogConfItem "$beginLine" "$endLine"
    elif [ "$module" -eq $MODULE_ADX ]; then
        GetAdxConfItem "$beginLine" "$endLine"
    else
        ErrorLog "Invalid module index $module"
        exit 1;
    fi
}

function GetLogPathConfItem(){
    local beginLine=$1
    local endLine=$2
    local moduleIdx=$3
    LOG_PATH_LIST=$(sed -n ''"${beginLine}"','"${endLine}"'p' $CONF_FILE | grep "^LOG_PATH_LIST\s*=" |awk -F "=" '{print $2}' |  sed s/[[:space:]]//g)

    if [ ${#LOG_PATH_LIST} -gt $MAX_LOG_PATH_LENGTH ]
    then
        ErrorLog "log input path list is invalid"
        exit 1
    fi

    if [ ! -n "$LOG_PATH_LIST" ]
    then
        if [ "$moduleIdx" -eq $MODULE_PROFILING ] || [ "$moduleIdx" -eq $MODULE_BBOX ] || [ "$moduleIdx" -eq $MODULE_ADX ]; then
            arrLogPathStr=$(/bin/bash "$SCRIPT_PATH/${MODULE_CONF[$moduleIdx]}" -q --hostDir)
        else
            arrLogPathStr=$(/bin/bash "$SCRIPT_PATH/${MODULE_CONF[$moduleIdx]}" getDir)
        fi
        if [ $? -ne 0 ]; then
            ErrorLog "get LOG_PATH_LIST from config file error"
            exit 1
        fi
        if [ -z "$arrLogPathStr" ]; then
            ErrorLog "LOG_PATH_LIST is empty from config file"
            exit 1
        fi
        arrLogPath=($arrLogPathStr)
    else
        arrLogPath=(${LOG_PATH_LIST//;/ })
    fi
    InfoLog "the LOG_PATH_LIST is  ${arrLogPath[*]}"

    if [ ${#arrLogPath[*]} -gt $MAX_LOG_PATH_NUM ]
    then
        ErrorLog "the number configured paths exceeds the limit of $MAX_LOG_PATH_NUM"
        exit 1
    fi
}

#find file that to be zipped later, if the postfix of the file is .log and is not writeable then return 1,else return 0
#this function may be extended to support other criteria to decide if one file is to be compressed or not
function IsToZipLogFile(){
    local file=$1
    if [ ! -d "$file" ] && [ "${file##*.}"x = "log"x ] && [ -z "$(ls -l $file | awk '{print $1}'| sed 's/\.//g' | sed 's/+//g' |sed 's/-//g'|sed 's/r//g')" ]
    then
        curTimeStmp=$(date +%s)
        lastModify=$(stat -c %Y $file)
        let delta=$curTimeStmp-$lastModify
        if [ $delta -gt $LOG_MIN_LIFETIME ]
        then
            return $IS_TO_ZIP_TRUE
        else
            return $IS_TO_ZIP_FALSE
        fi
    else
        return $IS_TO_ZIP_FALSE
    fi
}

# Verify the full path is host-0 or device-xx directories and check every sub-directory in it.
# All the sub-directories recorded with "FILEDONE" or "PROCFAIL" in DONE file will be moved
# into the backup direcotry and then function return 1. Otherwise do nothing and return 0.
# The to be zipped dir-name is stored in global variable bboxToZipDir.
function IsToZipBboxFile(){
    local path=$1
    local parent=${path%/*}
    local current=${path##*/}
    local list=""
    bboxToZipDir=""

    if [ -d "$path" ] && [[ $current =~ ^host-0$|^device-[0-9]{1,2}$ ]]; then
        list=$(ls "$path")
        for child in $list; do
            if [ ! -d "$path/$child" ]; then
                continue
            fi

            if [[ ! "$child" =~ ^[0-9]{14}-[0-9]{9}$ ]]; then
                WarningLog "skip unexpected directory $path/$child"
                continue
            fi

            local doneFile="$path/$child/DONE"
            if [ -f "$doneFile" ] && [ "$(grep -cE "FILEDONE|PROCFAIL" "$doneFile")" -eq 1 ]; then
                if [ -z "$bboxToZipDir" ]; then
                    bboxToZipDir="$parent/backup/$current-$(date +%Y%m%d%H%M%S)"
                fi
                if [ ! -e "$bboxToZipDir" ]; then
                    mkdir -p "$bboxToZipDir"
                fi
                if [ -f "$path/history.log" ]; then
                    cp "$path/history.log" "$bboxToZipDir"
                else
                    WarningLog "can not find history.log in $path"
                fi
                mv -f "$path/$child" "$bboxToZipDir"
            fi
        done
    fi

    if [ -n "$bboxToZipDir" ]; then
        return $IS_TO_ZIP_TRUE
    else
        return $IS_TO_ZIP_FALSE
    fi
}

#find file that to be zipped later, if the postfix of the file is .done then return 1,else return 0
#this function may be extended to support other criteria to decide if one file is to be compressed or not
function IsToZipProfilingFile(){
    local file=$1
    local zipFile=""
    if [ ! -d "$file" ] && [ -f "$file" ] && [ "${file##*.}"x = "done"x ]
    then
        zipFile=$(echo "$file"|sed 's/.done$/.zip/g')
        if [ -f "$zipFile" ]; then
            return $IS_TO_ZIP_FALSE
        fi
        return $IS_TO_ZIP_TRUE
    else
        return $IS_TO_ZIP_FALSE
    fi
}

#find file that to be zipped later, if the postfix of the file is .log and is not writeable then return 1,else return 0
#this function may be extended to support other criteria to decide if one file is to be compressed or not
function IsToZipAdxFile(){
    local file=$1
    local zipFile=""
    if [ ! -d "$file" ] && [ -r "$file" ]
    then
        if [ "${file##*.}"x = "zip"x ] || [ `stat -c %a "$file"` != "440" ]; then
            return $IS_TO_ZIP_FALSE
        elif [[ "${file}" =~ "/record/" ]]; then
            return $IS_TO_ZIP_FALSE
        fi
        return $IS_TO_ZIP_TRUE
    else
        return $IS_TO_ZIP_FALSE
    fi
}

function IsToZipModuleFile(){
    local fullName=$1
    local module=$2

    if [ "$module" -eq $MODULE_LOG ]; then
        IsToZipLogFile "$fullName"
    elif [ "$module" -eq $MODULE_BBOX ]; then
        IsToZipBboxFile "$fullName"
    elif [ "$module" -eq $MODULE_PROFILING ]; then
        IsToZipProfilingFile "$fullName"
    elif [ "$module" -eq $MODULE_ADX ]; then
        IsToZipAdxFile "$fullName"
    else
        return $IS_TO_ZIP_FALSE
    fi

    return $?
}

#traverse the input path recursively until reaches the maximum depth constraint
#meanwhile generate the file list that to be zipped later
#at last, remove the zip file that exceed the limit
#input $1:root path; $2:depth; $3 module idx;
function GetToZipFileList(){
    local dir=$1
    local depth=$2
    local module=$3

    DebugPrint "dir: $dir"
    let depth+=1
    if [ $depth -gt $MAX_DIR_DEPTH ]
        then return
    fi
    local fileList=($(ls -rt "$dir"))
    for file in ${fileList[*]}
    do
        local fullName="$dir/$file"
        if [ "$module" -eq $MODULE_PROFILING ] && [ -d "$fullName" ] && [ "$file" = "container" ]; then
            continue
        fi
        IsToZipModuleFile "$fullName" "$module"
        if [ $? -eq $IS_TO_ZIP_TRUE ]
        then
            DebugPrint "fullname: $fullName"
            CompressModuleFile "$fullName" "$module"
            continue
        else
            if [ -d "$fullName" ]
            then
                GetToZipFileList "$fullName" $depth "$module"
            fi
        fi
    done
    # traverse the input path recursively until reaches the maximum depth constraints
    GetToDeleteOldZipFiles "$dir" "$module"
}

# brief : count the number of zip files and remove oldest ones when exceed the limit
function GetToDeleteOldZipFiles(){
    local dir=$1
    local module=$2
    local zipCnt=0
    local deletZipCnt=0

    if [ "$module" -eq $MODULE_BBOX ]
    then
        dir="$dir/backup"
    fi

    if [ "$module" -eq $MODULE_PROFILING ]
    then
        return
    fi

    if [ "$module" -eq $MODULE_ADX ]
    then
        return
    fi

    zipCnt=$(find "$dir" -maxdepth 1 |grep -cE "\.zip$")
    DebugPrint "$dir total zipcnt: $zipCnt"

    if [ "$zipCnt" -gt $MAX_ZIP_FILE_NUMBER ]
    then
        let deletZipCnt=$zipCnt-$MAX_ZIP_FILE_NUMBER
        DebugPrint "$dir: deletnum: $deletZipCnt"
        InfoLog "current number of zip files $zipCnt exceeds MAX_ZIP_FILE_NUMBER $MAX_ZIP_FILE_NUMBER"
        RemoveOldZipFiles "$dir" $deletZipCnt
    fi
}

#when the number of zipped files is greater then the MAX_ZIP_FILE_NUMBER,
#remove some of the oldest ones according to the timestamps
#input $1:dir name $2:the number of zip files that to be removed
function RemoveOldZipFiles(){
    local dir=$1
    local num=$2
    local zipFileList=($(find "$dir" -maxdepth 1 |grep -E "\.zip$" |xargs ls -rt))

    local i=0
    while [ $i -lt "$num" ]
    do
        DebugPrint "remove zip files: ${zipFileList[$i]}"
        InfoLog "Remove zip files: ${zipFileList[$i]}"
        rm -rf "${zipFileList[$i]}"
        let i+=1
    done
}

function CompressModuleFile(){
    local toZipFile=$1
    local module=$2
    local zipName=""

    if [ "$module" -eq $MODULE_LOG ]
    then
        zipName=$(echo "$toZipFile"| sed 's/.log$/.zip/g')
        CompressFile "$toZipFile" "$zipName"
    elif [ "$module" -eq $MODULE_BBOX ]
    then
        toZipFile="$bboxToZipDir"
        zipName="$bboxToZipDir.zip"
        CompressDir "$toZipFile" "$zipName"
    elif [ "$module" -eq $MODULE_PROFILING ]
    then
        toZipDoneFile=$toZipFile
        toZipSliceFile=$(echo "$toZipFile"|awk -F ".done" '{print $1}')
        zipName="$toZipSliceFile.zip"
        CompressFileForProfiling "$toZipDoneFile" "$toZipSliceFile" "$zipName"
    elif [ "$module" -eq $MODULE_ADX ]
    then
        toZipDumpFile=$toZipFile
        zipName="$toZipDumpFile.zip"
        CompressFileForAdx "$toZipDumpFile" "$zipName"
    fi
}

function CompressFileForAdx(){
    local toZipDumpFile=$1
    local zipName=$2

    if [ ! -f "$toZipDumpFile" ]; then
        ErrorLog "$toZipDumpFile is not exist"
    else
        InfoLog "Compress $toZipDumpFile"
        zip -jm "$zipName" "$toZipDumpFile" >> $ZIP_SERVICE_LOG
        if [ $? -eq 0 ]
        then
            chmod 600 "$zipName"
        else
            ErrorLog "Compress $toZipDumpFile failed"
        fi
    fi
}

function CompressFileForProfiling(){
    local toZipDoneFile=$1
    local toZipSliceFile=$2
    local zipName=$3

    if [ ! -f "$toZipDoneFile" ]; then
        ErrorLog "$toZipDoneFile is not exist"
    elif [ ! -f "$toZipSliceFile" ]; then
        ErrorLog "$toZipSliceFile is not exist"
    else
        InfoLog "Compress $toZipSliceFile"
        zip -jm "$zipName" "$toZipDoneFile" "$toZipSliceFile" >> $ZIP_SERVICE_LOG
        if [ $? -eq 0 ]
        then
            chmod 600 "$zipName"

        else
            ErrorLog "Compress $toZipDoneFile and $toZipSliceFile failed"
        fi
    fi
}

function CompressDir(){
    local toZipDir=$1
    local zipName=$2

    cd "$toZipDir/.."
    InfoLog "Compress $toZipDir"
    zip -mr7 "$zipName" "${toZipDir##*/}" >> $ZIP_SERVICE_LOG
    cd "-" >/dev/null
    if [ -e "$zipName" ]; then
        chmod $DIR_PERMISSION_READ "$zipName"
    else
        ErrorLog "Compress $file failed"
    fi
}

function CompressFile(){
    local toZipFile=$1
    local zipName=$2

    InfoLog "Compress $toZipFile"
    zip -jm "$zipName" "$toZipFile" >> $ZIP_SERVICE_LOG
    if [ $? -eq 0 ]
    then
        chmod $DIR_PERMISSION_READ "$zipName"
    else
        ErrorLog "Compress $file failed"
    fi
}

#if the COMPRESS_SWITCH is 0, then do not compress
function GetLogCompressSwitch(){
    local beginLine=$1
    local endLine=$2
    COMPRESS_SWITCH=$(sed -n ''"${beginLine}"','"${endLine}"'p' $CONF_FILE | grep "^COMPRESS_SWITCH\s*=" |awk -F "=" '{print $2}' |  sed s/[[:space:]]//g)
    if [ ! -n "$COMPRESS_SWITCH" ]
    then
        WarningLog "the COMPRESS_SWITCH is not configured, use default value $DEFAULT_COMPRESS_SWITCH"
        COMPRESS_SWITCH=$DEFAULT_COMPRESS_SWITCH
    elif [[ ! $COMPRESS_SWITCH =~ ^[0-1]{1}$ ]]
    then
        ErrorLog "COMPRESS_SWITCH is invalid！"
        exit 1
    fi
    if [ $COMPRESS_SWITCH -eq 0 ]
    then
        exit 0
    fi
}

#check if the compress service log is larger than 10M, clear it.
function CheckCompressServiceLogFile(){
    if [ -f $ZIP_SERVICE_LOG ] && [ "$(ls -l $ZIP_SERVICE_LOG | awk '{ print $5 }')" -gt $MAX_ZIP_SERVICE_LOG_LENGTH ]
    then
        mv -f $ZIP_SERVICE_LOG $ZIP_SERVICE_LOG_OLD
        chmod $DIR_PERMISSION_READ $ZIP_SERVICE_LOG_OLD
        PrintLog "[INFO] $(date +%Y-%m-%d-%H:%M:%S) clear compress service log file."
    fi
}

function ProcessModule(){
    local moduleIdx=$1
    local moduleName=${MODULE_NAME[$moduleIdx]}
    local lineNumPair=($(FindModuleLineNumber "$moduleName" "$moduleName""_END"))

    InfoLog "ProcessModule $moduleName"
    GetLogCompressSwitch "${lineNumPair[0]}" "${lineNumPair[1]}"
    GetConfItem "${lineNumPair[0]}" "${lineNumPair[1]}" "$moduleIdx"
    GetLogPathConfItem "${lineNumPair[0]}" "${lineNumPair[1]}" "$moduleIdx"
    for path in ${arrLogPath[*]}
    do
        if [ ! -n "$path" ] || [ ! -d "$path" ]
        then
            ErrorLog "Input $path is not a valid file path"
        fi
        if [ ! -r "$path" ] || [ ! -w "$path" ]
        then
            ErrorLog "path $path is not readable or not writeable"
        fi
        GetToZipFileList "$path" -1 "$moduleIdx"
    done
    InfoLog "ProcessModule $moduleName end"
}

LOGNAME="LOG"
BBOXNAME="BBOX"
PROFILINGNAME="PROFILING"
ADXNAME="ADX"
touch $ZIP_SERVICE_LOG && chmod $DIR_PERMISSION $ZIP_SERVICE_LOG
CheckCompressServiceLogFile
if [ "$#" -eq "1" ]; then
# start to process log module
if [ "$1" == "$LOGNAME" ]; then
    PrintLog "[INFO] $(date +%Y-%m-%d-%H:%M:%S) periodic LOG compress service starts."
    ProcessModule $MODULE_LOG
    wait
# start to process bbox module
elif [ "$1" == "$BBOXNAME" ]; then
    PrintLog "[INFO] $(date +%Y-%m-%d-%H:%M:%S) periodic BBOX compress service starts."
    ProcessModule $MODULE_BBOX
    wait
# start to process profiling module
elif [ "$1" == "$PROFILINGNAME" ]; then
    PrintLog "[INFO] $(date +%Y-%m-%d-%H:%M:%S) periodic PROFILING compress service starts."
    ProcessModule $MODULE_PROFILING
    wait
# start to process adx module
elif [ "$1" == "$ADXNAME" ]; then
    PrintLog "[INFO] $(date +%Y-%m-%d-%H:%M:%S) periodic ADX compress service starts."
    ProcessModule $MODULE_ADX
    wait
fi
else
    PrintLog "[INFO] $(date +%Y-%m-%d-%H:%M:%S) periodic compress service starts."
    # start to process log module
    ProcessModule $MODULE_LOG &
    # start to process bbox module
    ProcessModule $MODULE_BBOX &
    # start to process profiling module
    ProcessModule $MODULE_PROFILING &
    #start to process adx module
    ProcessModule $MODULE_ADX
    wait
fi
