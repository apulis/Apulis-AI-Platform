#!/bin/bash
# This script restart log-daemon process by appmond ###
#Copyright (c) Huawei Technologies Co., Ltd. 2018-2019. All rights reserved.

LOG_FILE="/usr/slog/log_monitor_log.txt"
HIAI_DIR=$(cut -d '=' -f 2 /lib/davinci.conf)
PROCESS="${HIAI_DIR}/driver/tools/log-daemon"
DATE_FORMATE="+%F %T.%3N"
int=1

date > $LOG_FILE
if grep '^[[:digit:]]*$' <<< "$1";then
    echo "Process ID of the input parameter: $1" >> $LOG_FILE
else
    echo "Invalid input parameter: $1, effective value should be Process Number" >> $LOG_FILE
    exit 1
fi

## Determine if the log-daemon process exists or not
## if  process exists, kill it
if [ $(ps -ef | grep -v grep | grep -c "${PROCESS}$") -ne 0 ];then
    echo "kill $PROCESS: $1" >> $LOG_FILE
    kill -31 "$1"
    while(( $int<=10 ))
    do
        if [ $(ps -ef | grep -v grep | grep -c "${PROCESS}$") -ne 0 ];then
            echo "wait to kill $PROCESS: $1" >> $LOG_FILE
            sleep 1
        fi
        let "int++"
    done
fi

## restart log-daemon process
echo "raise $PROCESS " >> $LOG_FILE
if [ -f "$PROCESS" ]; then
    BBOX_MONITOR_SH="${HIAI_DIR}/driver/tools/bbox_daemon_monitor.sh"
    if [ -f "${BBOX_MONITOR_SH}" ]; then
        . "${BBOX_MONITOR_SH}"
        recover_bbox_conf "${HIAI_DIR}" $LOG_FILE >> $LOG_FILE 2>&1
    fi
    nohup ${HIAI_DIR}/driver/tools/log-daemon >/dev/null 2>&1 &
    int=1
    while(( $int<=5 ))
    do
        date "$DATE_FORMATE" >> $LOG_FILE
        ps -ef | grep "${PROCESS}$" | grep -v grep >> $LOG_FILE
        sleep 0.5
        let "int++"
    done
else
    echo "$PROCESS is not existed" >> $LOG_FILE
fi
date "$DATE_FORMATE" >> $LOG_FILE
ps -ef | grep "${PROCESS}$" | grep -v grep >> $LOG_FILE
