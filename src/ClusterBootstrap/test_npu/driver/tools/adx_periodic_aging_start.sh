#!/bin/bash
DEFAULT_CONF_PATH=/var/log/npu/conf/adx
CONF_FILE=${DEFAULT_CONF_PATH}"/adx_aging.conf"
LOCK_FILE=${DEFAULT_CONF_PATH}"/adx_aging.lock"
CRON_FILE=${DEFAULT_CONF_PATH}"/adx_aging.cron"
SCRIPT_PATH=$(dirname "$(readlink -f "$0")")
ADX_PARAM=$1
CRONTAB_MODE=0
CRONTAB_TIME=1
if [ -z "${ADX_PARAM}" ]; then
    echo -e "Parameters failed!\n\texample: bash adx_periodic_aging_start.sh --start"
    exit 1
fi

which crontab > /dev/null 2>&1
if [ $? -ne 0 ];then
    echo "[ERROR] Please install crontab first."
    exit 1
fi

if [ -f "${CONF_FILE}" ]; then
    CRONTAB_MODE=$(grep "^CRONTAB_MODE\s*=" $CONF_FILE |awk -F'=' '{ print $2 }' | sed s/[[:space:]]//g)
    CRONTAB_TIME=$(grep "^CRONTAB_TIME\S*=" $CONF_FILE |awk -F'=' '{ print $2 }' | sed s/[[:space:]]//g | sed ':a;N;$!ba;s/\n//g')
fi

cron_file=$CRON_FILE
cron_info="* * * * * umask 077;flock -xn $LOCK_FILE -c 'nice -n 10 /bin/bash $SCRIPT_PATH/adx_periodic_aging.sh'"

if [ -z "$CRONTAB_MODE" ] || [ -z "$CRONTAB_TIME" ]; then
    echo "[ERROR] parameter CRONTAB_MODE or CRONTAB_TIME is null"
    exit 1
elif [ "$CRONTAB_MODE" = 0 ]; then
    if [[ ! $CRONTAB_TIME =~ ^([1-9]|[1-5][0-9])$ ]]; then
        echo "[ERROR] CRONTAB_TIME:${CRONTAB_TIME} is invalid!"
        exit 1
    fi
    cron_info="*/$CRONTAB_TIME * * * * umask 077;flock -xn $LOCK_FILE -c 'nice -n 10 /bin/bash $SCRIPT_PATH/adx_periodic_aging.sh'"
elif [ "$CRONTAB_MODE" = 1 ]; then
    if [[ ! $CRONTAB_TIME =~ ^([1-9]|[1][0-9]|[2][0-3])$ ]]; then
        echo "[ERROR] CRONTAB_TIME:${CRONTAB_TIME} is invalid!"
        exit 1
    fi
    cron_info="0 */$CRONTAB_TIME * * * umask 077;flock -xn $LOCK_FILE -c 'nice -n 10 /bin/bash $SCRIPT_PATH/adx_periodic_aging.sh'"
elif [ "$CRONTAB_MODE" = 2 ]; then
    if [[ ! $CRONTAB_TIME =~ ^([1-9]|[1-2][0-9]|[3][0-1])$ ]]; then
        echo "[ERROR] CRONTAB_TIME:${CRONTAB_TIME} is invalid!"
        exit 1
    fi
    cron_info="0 0 */$CRONTAB_TIME * * umask 077;flock -xn $LOCK_FILE -c 'nice -n 10 /bin/bash $SCRIPT_PATH/adx_periodic_aging.sh'"
elif [ "$CRONTAB_MODE" = 3 ]; then
    if [[ ! $CRONTAB_TIME =~ ^([1-9]|[1][0-2])$ ]]; then
        echo "[ERROR] CRONTAB_TIME:${CRONTAB_TIME} is invalid!"
        exit 1
    fi
    cron_info="0 0 1 */$CRONTAB_TIME * umask 077;flock -xn $LOCK_FILE -c 'nice -n 10 /bin/bash $SCRIPT_PATH/adx_periodic_aging.sh'"
elif [ "$CRONTAB_MODE" = 4 ]; then
    if [[ ! $CRONTAB_TIME =~ ^([1-7])$ ]]; then
        echo "[ERROR] CRONTAB_TIME:${CRONTAB_TIME} is invalid!"
        exit 1
    fi
    cron_info="0 0 * * */$CRONTAB_TIME umask 077;flock -xn $LOCK_FILE -c 'nice -n 10 /bin/bash $SCRIPT_PATH/adx_periodic_aging.sh'"
else
    echo "[ERROR] wrong parameter:CRONTAB_MODE"
    exit 1
fi

#generate/update crontab file
start_keyword="# adx aging process start"
end_keyword="# adx aging process end"
if [ ! -f $cron_file ]; then
    touch $cron_file
    chmod -f 600 $cron_file
fi
crontab -l > $cron_file
startline=$(sed -n "/$start_keyword/=" $cron_file)
commandline=$(sed -n "/adx_periodic_aging.sh/=" $cron_file)
endline=$(sed -n "/$end_keyword/=" $cron_file)

if [ "${ADX_PARAM}" == "--start" ]; then
    if [ -z "$startline" ]; then
        echo "$start_keyword" >> $cron_file
        echo "$end_keyword" >> $cron_file
        sed -i "/$start_keyword/a\\$cron_info" $cron_file
    else
        echo "adx aging server is running"
        exit 0
    fi
elif [ "${ADX_PARAM}" == "--stop" ]; then
    if [ -z "$startline" ]; then
        echo "adx aging server is not running"
        exit 0
    else
        sed -i "${startline},${endline}"d $cron_file
    fi
elif [ "${ADX_PARAM}" == "--restart" ]; then
    if [ -z "$startline" ]; then
        echo "$start_keyword" >> $cron_file
        echo "$end_keyword" >> $cron_file
        sed -i "/$start_keyword/a\\$cron_info" $cron_file
    else
        sed -i "${commandline}c $cron_info" $cron_file
    fi
else
    echo -e "Parameters failed!\n\texample: bash adx_periodic_aging_start.sh --start"
    exit 1
fi

#start crontab
crontab $cron_file
if [ $? -eq 1 ]; then
    echo "[ERROR] adx aging server \"${ADX_PARAM}\" failed"
    exit 1
else
    echo "adx aging server \"${ADX_PARAM}\" success"
    exit 0
fi
