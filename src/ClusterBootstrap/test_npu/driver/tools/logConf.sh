#!/bin/bash
##############################################################
# This script get setting from slog.conf and filebeat.yml and set.
# Copyright (c) Huawei Technologies Co., Ltd. 2018-2019. All rights reserved.
filebeat_path=/var/log/npu/conf/slog/filebeat.yml
slog_path=/var/log/npu/conf/slog/slog.conf
logFile="/var/log/npu/toolchain/run/logConf.log"
logPath=/var/log/npu/toolchain/run
logFileBak="/var/log/npu/toolchain/run/logConf.log.bak"
MaxLogLength=$((10*1024*1024)) #10MB
CALLER=$(basename "${0}")
canWriteLog=1

#check if the log is larger than 10M, backup it.
function CheckLogFile(){
    if [ -f $logFile ] && [ "$(ls -l $logFile | awk '{ print $5 }')" -gt $MaxLogLength ]
    then
        mv -f $logFile $logFileBak
        chmod 440 $logFileBak
    fi
}

#print log to /var/log/npu/toolchain/run/logConf.log
function PrintLog() {
    CheckLogFile
    cur_date=`date +"%Y-%m-%d %H:%M:%S"`
    if [ -f $logFile ]; then
        if [ -w $logFile ]; then
            echo $1" $USER $cur_date 127.0.0.1 "$2 >> $logFile
            chmod 640 $logFile
        else
            if [ $canWriteLog -ne 0 ]; then
                canWriteLog=0
                echo "[error] logfile: $logFile has no permission to write"
            fi
        fi
    else
        if [ ! -d $logPath ];then
            if [ $canWriteLog -ne 0 ]; then
                canWriteLog=0
                echo "[error] logpath:$logPath is not exits, please create it."
            fi
        else
            if [ -w $logPath ];then
                echo $1" $USER $cur_date 127.0.0.1 "$2 >> $logFile
                chmod 640 $logFile
            else
                if [ $canWriteLog -ne 0 ];then
                    echo "[error] logpath:$logPath can't create file, please check permission"
                    canWriteLog=0
                fi
            fi
        fi
    fi
}

#get hostDir setting in slog.conf
get_dir(){
    echo -e $(sed -e '/^#/d' "${slog_path}" | grep logAgentFileDir |awk -F'=' '{ print $2 }' | sed s/[[:space:]]//g)
}
if [ "${1}" = "getDir" ]; then
    if [ -z "$(get_dir)" ];then
        exit 1
    else
        get_dir
        exit 0
    fi
fi

#check filebeat_path and slog_path in this file
if [ ! -f "$filebeat_path" ]; then
    echo "wrong filebeat_path setting in logConf.sh:no such file ${filebeat_path}"
    exit 1
fi
if [ ! -f "$slog_path" ]; then
    echo "wrong slog_path setting in logConf.sh:no such file ${slog_path}"
    exit 1
fi

#deal with paramaters
show_usage(){
    if [ "$1" = "set" ];then
        echo -e "$CALLER:set path failed. \033[32m Use Example\033[0m: [--hostDir /xx/xx] / [--outputDir /xx/xx] / [--hostDir /xx/xx --outputDir /xx/xx]"
    elif [ "$1" = "get" ];then
        echo -e "$CALLER:get path failed. \033[32m Use Example\033[0m: [-q --hostDir]  / [-q --outputDir] / [-q --hostDir -q --outputDir]"
    else
        echo -e "$CALLER:wrong parameters."
        echo -e "\033[32m set path Example\033[0m: [--hostDir /xx/xx] / [--outputDir /xx/xx] / [--hostDir /xx/xx --outputDir /xx/xx]"
        echo -e "\033[32m get path Example\033[0m: [-q --hostDir]  / [-q --outputDir] / [-q --hostDir -q --outputDir]"
    fi
    exit 1
}
get_output_dir(){
    startline=$[$(sed -n "/$1/=" "$filebeat_path")+1]
    endline=$[$(sed -n "/$2/=" "$filebeat_path")-1]
    if [ "${startline}" -le "${endline}" ];then
        sed -n "$startline,$endline"p "${filebeat_path}"
    fi
}

if [[ "${1}" != "-q" ]] && [[ "${1}" != "--hostDir" ]] && [[ "${1}" != "--outputDir" ]] ;then
    if [[ "${1}" =~ "--" ]];then
        show_usage "set"
    else
        show_usage
    fi
fi

#get path
if [ "${1}" = "-q" ];then
    hostdir=$(get_dir)
    outputdir=$(get_output_dir "# -->outputDir pathset start" "# -->outputDir pathset end"|awk -F'"' '{ print $2 }')
    if [ $# = 2 ];then
        if [ "${2}" = "--hostDir" ];then
            echo "Origin Log Stored Path:$hostdir"
            exit 0
        elif [ "${2}" = "--outputDir" ];then
            echo "Sorting Log Stored Path:$outputdir"
            exit 0
        else
            echo "$CALLER: unrecognized option: ${2}"
            show_usage "get"
        fi
    elif [ $# = 4 ];then
        if [[ "$@" != '-q --hostDir -q --outputDir'  && "$@" != '-q --outputDir -q --hostDir' ]];then
            show_usage "get"
        else
            echo "Origin Log Drop Path:$hostdir"
            echo "Sorting Log Path:$outputdir"
            exit 0
        fi
    else
        show_usage "get"
    fi
#set path
else
    if [ $# = 2 ];then
        if [ "${1}" = "--hostDir" ];then
            PrintLog "SetPath MINOR" "update hostDir in slog.conf, new hostDir is ${2}"
            if [ -d "${2}" ]; then
                host_dir="${2}"
            else
                echo "wrong parameter ${2}:no such directory "
                PrintLog "SetPath MINOR" "wrong parameter ${2}:no such directory"
                show_usage "set"
            fi
        elif [ "${1}" = "--outputDir" ];then
            PrintLog "SetPath MINOR" "update outputDir in filebeat.yml, new hostDir is ${2}"
            if [ -d "${2}" ]; then
                output_dir="${2}"
            else
                echo "wrong parameter ${2}:no such directory "
                PrintLog "SetPath MINOR" "wrong parameter ${2}:no such directory"
                show_usage "set"
            fi
        else
            show_usage "set"
        fi
    elif [ $# = 4 ];then
        if [[ "${1}" = "--hostDir" ]] && [[ "${3}" = "--outputDir" ]];then
            PrintLog "SetPath MINOR" "update hostDir in slog.conf, new hostDir is ${2}"
            PrintLog "SetPath MINOR" "update outputDir in filebeat.yml, new hostDir is ${4}"
            if [ ! -d "${2}" ];then
                echo "wrong parameter ${2}:no such directory "
                PrintLog "SetPath MINOR" "wrong parameter ${2}:no such directory"
                show_usage "set"
            elif [ ! -d "${4}" ];then
                echo "wrong parameter ${4}:no such directory "
                PrintLog "SetPath MINOR" "wrong parameter ${4}:no such directory"
                show_usage "set"
            else
                host_dir="${2}"
                output_dir="${4}"
            fi
        elif [[ "${3}" = "--hostDir" ]] && [[ "${1}" = "--outputDir" ]];then
            PrintLog "SetPath MINOR" "update hostDir in slog.conf, new hostDir is ${4}"
            PrintLog "SetPath MINOR" "update outputDir in filebeat.yml, new hostDir is ${2}"
            if [ ! -d "${2}" ];then
                echo "wrong parameter ${2}:no such directory "
                PrintLog "SetPath MINOR" "wrong parameter ${2}:no such directory"
                show_usage "set"
            elif [ ! -d "${4}" ];then
                echo "wrong parameter ${4}:no such directory "
                PrintLog "SetPath MINOR" "wrong parameter ${4}:no such directory"
                show_usage "set"
            else
                host_dir="${4}"
                output_dir="${2}"
            fi
        else
            show_usage "set"
        fi
    else
        show_usage "set"
    fi
fi

#update host_dir in slog.conf and filebeat.yml
if [ ! -z "$host_dir" ];then
    sed -i "s|logAgentFileDir=.*|logAgentFileDir= $host_dir|g" "${slog_path}"
    if [ $? -eq 0 ];then
        PrintLog "SetPath MINOR" "set log_path success"
        echo "set log_path success"
    else
        PrintLog "SetPath MINOR" "set log_path failed"
        echo "set log_path failed"
        exit 1
    fi
    startline=$[$(sed -n '/# -->hostDir pathset start/=' "${filebeat_path}")+1]
    endline=$[$(sed -n '/# -->hostDir pathset end/=' "${filebeat_path}")-1]
    if [ "${startline}" -le "${endline}" ];then
       $(sed -i "$startline,$endline"d "$filebeat_path")
    fi
    sed -i "/# -->hostDir pathset start/a\    - $host_dir/*/*.log \n    - $host_dir/*/*/*/*.log  " "${filebeat_path}"
    if [ $? -eq 0 ];then
        PrintLog "SetPath MINOR" "set filebeat input_path success"
        echo "set filebeat input_path success"
    else
        PrintLog "SetPath MINOR" "set input_path failed"
        echo "set input_path failed"
    fi
fi

#update output_dir in filebeat.yml
if [ ! -z "$output_dir" ];then
    startline=$[$(sed -n '/# -->outputDir pathset start/=' "${filebeat_path}")+1]
    endline=$[$(sed -n '/# -->outputDir pathset end/=' "${filebeat_path}")-1]
    if [ "${startline}" -le "${endline}" ];then
       $(sed -i "$startline,$endline"d "${filebeat_path}")
    fi
    $(sed -i "/# -->outputDir pathset start/a\  path: \"$output_dir\"" "${filebeat_path}")
    if [ $? -eq 0 ];then
        echo "set filebeat output_path success"
        PrintLog "SetPath MINOR" "set filebeat output_path success"
    else
        echo "set filebeat output_path failed"
        PrintLog "SetPath MINOR" "set filebeat output_path failed"
    fi
fi
