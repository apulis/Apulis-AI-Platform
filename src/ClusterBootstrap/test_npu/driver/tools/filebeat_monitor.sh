#!/bin/bash
# This script restart filebeat process by appmond.
# Copyright (c) Huawei Technologies Co., Ltd. 2018-2019. All rights reserved.
#set -e
LOG_FILE="/usr/slog/filebeat_monitor_log.txt"
HIAI_DIR=$(cut -d '=' -f 2 /lib/davinci.conf)
PROCESS="${HIAI_DIR}/driver/tools/filebeat"
DATE_FORMATE="+%F %T.%3N"
CONFIG_FILE="/var/log/npu/conf/slog/filebeat_lab.yml"

date > "${LOG_FILE}"
if grep '^[[:digit:]]*$' <<< "$1";then
    echo "Process ID of the input parameter: $1" >> "${LOG_FILE}"
else
    echo "Invalid input parameter: $1, effective value should be Process Number" >> "${LOG_FILE}"
    exit 1
fi

# Determine if the filebeat process exists or not
# if  process exists, kill it
if [ $(ps -ef | grep -v grep | grep -c "$PROCESS") -ne 0 ];then
    echo "kill $PROCESS: \"$1\"" >> "${LOG_FILE}"
    kill -31 "$1"
    int=0
    while(( "${int}"<10 ))
    do
        if [ $(ps -ef | grep -v grep | grep -c "${PROCESS}"$) -ne 0 ];then
            echo "wait to kill ${PROCESS}: \"$1\"" >> "${LOG_FILE}"
            sleep 1
        else
            break
        fi
        let int++
    done
fi

# restart filbeat process
echo "raise ${PROCESS} " >> "${LOG_FILE}"
if [ -f "${PROCESS}" ]; then
    is_lab=0
    # check whether lab env
    startline=$[$(sed -n '/output.elasticsearch:/=' "$CONFIG_FILE")+1]
    endline=$[$(sed -n '/pipeline:/=' "$CONFIG_FILE")-1]
    if [ "$startline" -eq "$endline" ];then
        if [ "$startline" -eq "$endline" ];then
            is_lab=$(sed -n "$endline"p "$CONFIG_FILE"  | sed -n '/^[^#]/p' | wc -l)
        fi
    fi

    if [ "$is_lab" -eq 1 ];then
        nohup ${HIAI_DIR}/driver/tools/filebeat -c /var/log/npu/conf/slog/filebeat_lab.yml >/dev/null 2>&1 &
    else
        nohup ${HIAI_DIR}/driver/tools/filebeat -c /var/log/npu/conf/slog/filebeat.yml >/dev/null 2>&1 &
    fi
else
    echo "${PROCESS} is not existed, filebeat restart failed!" >> "${LOG_FILE}"
    exit 1
fi

# wait for 1s
sleep 1
date "${DATE_FORMATE}" >> "${LOG_FILE}"
ps -ef | grep "${PROCESS}" | grep -v grep >> "${LOG_FILE}"

COUNT=$(ps -ef | grep ${HIAI_DIR}/driver/tools/filebeat | grep -v grep | wc -l)
if [ "$COUNT" -eq 0 ]; then
    echo "filebeat restart failed!" >> "${LOG_FILE}"
else
    echo "filebeat restart successfully!" >> "${LOG_FILE}"
fi
