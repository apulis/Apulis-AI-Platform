#!/bin/bash
##############################################################
# This script get setting from ide_daemon.cfg and set.
# Copyright (c) Huawei Technologies Co., Ltd. 2020. All rights reserved.

installPath=/etc/ascend_install.info
confPath=/home/HwHiAiUser/ide_daemon/ide_daemon.cfg
logFile="/var/log/npu/ide_daemon/run/adxConf.log"
logPath=/var/log/npu/ide_daemon/run
logFileBak="/var/log/npu/ide_daemon/run/adxConf.log.bak"
MaxLogLength=$((10*1024*1024)) #10MB
CALLER=$(basename "${0}")
canWriteLog=1

if [ ! -f ${installPath} ]; then
    installPath=/etc/HiAI_install.info
fi

if [ -f ${installPath} ]; then
    . $installPath
    username=$UserName
    if [ "$username" = "" ]; then
        username=HwHiAiUser
    fi
    user_home_path=`eval echo "~${username}"`
    if [ "$user_home_path" = "" ]; then
        echo "wrong get install user"
        exit 1
    fi
    confPath=${user_home_path}/ide_daemon/ide_daemon.cfg
fi

#check if the log is larger than 10M, backup it.
function CheckLogFile(){
    if [ -f $logFile ] && [ "$(ls -l $logFile | awk '{ print $5 }')" -gt $MaxLogLength ]
    then
        mv -f $logFile $logFileBak
        chmod 440 $logFileBak
    fi
}

#print log to /var/log/npu/toolchain/run/adxConf.log
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

if [ ! -f "$confPath" ]; then
    echo "wrong confPath setting in adxConf.sh:no such file ${confPath}"
    exit 1
fi

#get hostDir setting in ide_daemon.cfg
get_dir(){
    echo -e $(sed -e '/^#/d' "${confPath}" | grep DUMP_PATH |awk -F'=' '{ print $2 }' | sed s/[[:space:]]//g)
}

if [ "${1}" = "getDir" ]; then
    if [ -z "$(get_dir)" ];then
        exit 1
    else
        get_dir
        exit 0
    fi
fi

#deal with paramaters
show_usage(){
    if [ "$1" = "set" ];then
        echo -e "$CALLER:set path failed. \033[32m Use Example\033[0m: [--hostDir /xx/xx]"
    elif [ "$1" = "get" ];then
        echo -e "$CALLER:get path failed. \033[32m Use Example\033[0m: [-q --hostDir]"
    else
        echo -e "$CALLER:wrong parameters."
        echo -e "\033[32m set path Example\033[0m: [--hostDir /xx/xx]"
        echo -e "\033[32m get path Example\033[0m: [-q --hostDir]"
    fi
    exit 1
}

if [[ "${1}" != "-q" ]] && [[ "${1}" != "--hostDir" ]] ;then
    if [[ "${1}" =~ "--" ]];then
        show_usage "set"
    else
        show_usage
    fi
fi

#get path
if [ "${1}" = "-q" ];then
    hostdir=$(get_dir)
    if [ $# = 2 ];then
        if [ "${2}" = "--hostDir" ];then
            echo "$hostdir"
            exit 0
        else
            echo "$CALLER: unrecognized option: ${2}"
            show_usage "get"
        fi
    else
        show_usage "get"
    fi
#set path
else
    if [ $# = 2 ];then
        if [ "${1}" = "--hostDir" ];then
            PrintLog "SetPath MINOR" "update hostDir in ide_daemon.conf, new hostDir is ${2}"
            if [ -d "${2}" ]; then
                host_dir="${2}"
            else
                echo "wrong parameter ${2}:no such directory "
                PrintLog "SetPath MINOR" "wrong parameter ${2}:no such directory"
                show_usage "set"
            fi
        else
            show_usage "set"
        fi
    else
        show_usage "set"
    fi
fi

#update host_dir in ide_daemon.cfg
if [ ! -z "$host_dir" ];then
    sed -i "s|DUMP_PATH=.*|DUMP_PATH= $host_dir|g" "${confPath}"
    if [ $? -eq 0 ];then
        PrintLog "SetPath MINOR" "set dump path success"
        echo "set dump path  success"
    else
        PrintLog "SetPath MINOR" "set dump path failed"
        echo "set dump path  failed"
        exit 1
    fi
fi
