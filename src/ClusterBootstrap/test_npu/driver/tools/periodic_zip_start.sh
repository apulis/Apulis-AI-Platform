#!/bin/bash
DEFAULT_CONF_FILE=/var/log/npu/conf/slog/ziplog.conf
CONF_FILE=$DEFAULT_CONF_FILE
DEFAULT_ZIP_SERVICE_SWITCH=1
USER_SLOG_PATH=/usr/slog
LOG_LOCK_FILE=${USER_SLOG_PATH}"/log_zip_service.lock"
BBOX_LOCK_FILE=${USER_SLOG_PATH}"/bbox_zip_service.lock"
PROFILING_LOCK_FILE=${USER_SLOG_PATH}"/profiling_zip_service.lock"
ADX_LOCK_FILE=${USER_SLOG_PATH}"/adx_zip_service.lock"
CRON_FILE=${USER_SLOG_PATH}"/.cron"
START_WORD="# compress process start"
END_WORD="# compress process end"

cd "$(dirname $0)"
DIR=`pwd`
ZIP_SERVICE_SWITCH=`grep "^ZIP_SERVICE_SWITCH\s*=" $CONF_FILE |awk -F'=' '{ print $2 }' | sed s/[[:space:]]//g`
if [ ! -n "$ZIP_SERVICE_SWITCH" ]
then
    echo "ZIP_SERVICE_SWITCH has no value, use default value $DEFAULT_ZIP_SERVICE_SWITCH"
    ZIP_SERVICE_SWITCH=$DEFAULT_ZIP_SERVICE_SWITCH
elif [[ ! $ZIP_SERVICE_SWITCH =~ ^[0-1]{1}$ ]]
then
        echo "ERROR: ZIP_SERVICE_SWITCH is invalid!"
        exit 1
fi
if [ $ZIP_SERVICE_SWITCH -eq 0 ]
then
    crontab -l > $CRON_FILE
    START_LINE=$(sed -n "/$START_WORD/=" "$CRON_FILE")
    END_LINE=$(sed -n "/$END_WORD/=" "$CRON_FILE")
    if [ -z $START_LINE ] || [ -z $END_LINE ]; then
        rm -f $CRON_FILE
        echo "ZIP_SERVICE_SWITCH is 0, the compress service is not started"
        exit 0
    fi
    sed -i "$START_LINE,${END_LINE}d" "$CRON_FILE"
    if [ -f $LOG_LOCK_FILE ]; then
        rm -f $LOG_LOCK_FILE
    fi
    if [ -f $BBOX_LOCK_FILE ]; then
        rm -f $BBOX_LOCK_FILE
    fi
    if [ -f $PROFILING_LOCK_FILE ]; then
        rm -f $PROFILING_LOCK_FILE
    fi
    if [ -f $ADX_LOCK_FILE ]; then
        rm -f $ADX_LOCK_FILE
    fi
    crontab $CRON_FILE
    rm -f $CRON_FILE
    echo "ZIP_SERVICE_SWITCH is 0, the compress service will be stoped!"
    exit 0
fi

CRONTAB_MODE=`grep "^CRONTAB_MODE\s*=" $CONF_FILE |awk -F'=' '{ print $2 }' | sed s/[[:space:]]//g`
CRONTAB_TIME=`grep "^CRONTAB_TIME\S*=" $CONF_FILE |awk -F'=' '{ print $2 }' | sed s/[[:space:]]//g | sed ':a;N;$!ba;s/\n//g'`
CRON_INFO="* * * * * flock -xn $LOG_LOCK_FILE -c 'nice -n 10 /bin/bash $DIR/periodic_zip.sh LOG';\
flock -xn $BBOX_LOCK_FILE -c 'nice -n 10 /bin/bash $DIR/periodic_zip.sh BBOX';\
flock -xn $PROFILING_LOCK_FILE -c 'nice -n 10 /bin/bash $DIR/periodic_zip.sh PROFILING';\
flock -xn $ADX_LOCK_FILE -c 'nice -n 10 /bin/bash $DIR/periodic_zip.sh ADX'"

if [ -z "$CRONTAB_MODE" ]; then
    echo "parameter CRONTAB_MODE is null"
elif [ "$CRONTAB_MODE" = 0 ]; then
    CRON_INFO="*/$CRONTAB_TIME * * * * flock -xn $LOG_LOCK_FILE -c 'nice -n 10 /bin/bash $DIR/periodic_zip.sh LOG';\
flock -xn $BBOX_LOCK_FILE -c 'nice -n 10 /bin/bash $DIR/periodic_zip.sh BBOX';\
flock -xn $PROFILING_LOCK_FILE -c 'nice -n 10 /bin/bash $DIR/periodic_zip.sh PROFILING';\
flock -xn $ADX_LOCK_FILE -c 'nice -n 10 /bin/bash $DIR/periodic_zip.sh ADX'"
elif [ "$CRONTAB_MODE" = 1 ]; then
    CRON_INFO="0 */$CRONTAB_TIME * * * flock -xn $LOG_LOCK_FILE -c 'nice -n 10 /bin/bash $DIR/periodic_zip.sh LOG';\
flock -xn $BBOX_LOCK_FILE -c 'nice -n 10 /bin/bash $DIR/periodic_zip.sh BBOX';\
flock -xn $PROFILING_LOCK_FILE -c 'nice -n 10 /bin/bash $DIR/periodic_zip.sh PROFILING';\
flock -xn $ADX_LOCK_FILE -c 'nice -n 10 /bin/bash $DIR/periodic_zip.sh ADX'"
elif [ "$CRONTAB_MODE" = 2 ]; then
    CRON_INFO="0 0 */$CRONTAB_TIME * * flock -xn $LOG_LOCK_FILE -c 'nice -n 10 /bin/bash $DIR/periodic_zip.sh LOG';\
flock -xn $BBOX_LOCK_FILE -c 'nice -n 10 /bin/bash $DIR/periodic_zip.sh BBOX';\
flock -xn $PROFILING_LOCK_FILE -c 'nice -n 10 /bin/bash $DIR/periodic_zip.sh PROFILING';\
flock -xn $ADX_LOCK_FILE -c 'nice -n 10 /bin/bash $DIR/periodic_zip.sh ADX'"
elif [ "$CRONTAB_MODE" = 3 ]; then
    CRON_INFO="0 0 1 */$CRONTAB_TIME * flock -xn $LOG_LOCK_FILE -c 'nice -n 10 /bin/bash $DIR/periodic_zip.sh LOG';\
flock -xn $BBOX_LOCK_FILE -c 'nice -n 10 /bin/bash $DIR/periodic_zip.sh BBOX';\
flock -xn $PROFILING_LOCK_FILE -c 'nice -n 10 /bin/bash $DIR/periodic_zip.sh PROFILING';\
flock -xn $ADX_LOCK_FILE -c 'nice -n 10 /bin/bash $DIR/periodic_zip.sh ADX'"
elif [ "$CRONTAB_MODE" = 4 ]; then
    CRON_INFO="0 0 * * */$CRONTAB_TIME flock -xn $LOG_LOCK_FILE -c 'nice -n 10 /bin/bash $DIR/periodic_zip.sh LOG';\
flock -xn $BBOX_LOCK_FILE -c 'nice -n 10 /bin/bash $DIR/periodic_zip.sh BBOX';\
flock -xn $PROFILING_LOCK_FILE -c 'nice -n 10 /bin/bash $DIR/periodic_zip.sh PROFILING';\
flock -xn $ADX_LOCK_FILE -c 'nice -n 10 /bin/bash $DIR/periodic_zip.sh ADX'"
elif [ "$CRONTAB_MODE" = 5 ]; then
    CRONTAB2=`sed -e '/^#/d' $CONF_FILE | grep CRONTAB_TIME |awk -F'=' '{ print $2 }' | sed s/[[:space:]]/?/g | sed ':a;N;$!ba;s/\n//g'`
    CRON_INFO1="$CRONTAB2"" flock -xn $LOG_LOCK_FILE -c 'nice -n 10 /bin/bash $DIR/periodic_zip.sh LOG';\
flock -xn $BBOX_LOCK_FILE -c 'nice -n 10 /bin/bash $DIR/periodic_zip.sh BBOX';\
flock -xn $PROFILING_LOCK_FILE -c 'nice -n 10 /bin/bash $DIR/periodic_zip.sh PROFILING';\
flock -xn $ADX_LOCK_FILE -c 'nice -n 10 /bin/bash $DIR/periodic_zip.sh ADX'"
    CRON_INFO=${CRON_INFO1//\?/ }
else
    echo "fail:wrong parameter:CRONTAB_MODE"
    exit 1
fi

#generate/update crontab file
if [ ! -f $CRON_FILE ]; then
    touch $CRON_FILE
fi
crontab -l > $CRON_FILE
ZIP_SVC_LINE=`sed -n "/$START_WORD/=" $CRON_FILE`
if [ -z "$ZIP_SVC_LINE" ];then
    echo "$START_WORD" >> $CRON_FILE
    echo "$END_WORD" >> $CRON_FILE
fi
START_LINE=$[`sed -n "/$START_WORD/=" $CRON_FILE`+1]
END_LINE=$[`sed -n "/$END_WORD/=" $CRON_FILE`-1]
if [ $START_LINE -le $END_LINE ];then
    sed -i "$START_LINE,$END_LINE"d $CRON_FILE
fi
sed -i "/$START_WORD/a\\$CRON_INFO" $CRON_FILE

#start crontab
if [ -f $LOG_LOCK_FILE ]; then
    rm -f $LOG_LOCK_FILE
fi

if [ -f $BBOX_LOCK_FILE ]; then
    rm -f $BBOX_LOCK_FILE
fi

if [ -f $PROFILING_LOCK_FILE ]; then
    rm -f $PROFILING_LOCK_FILE
fi

if [ -f $ADX_LOCK_FILE ]; then
    rm -f $ADX_LOCK_FILE
fi

crontab $CRON_FILE
if [ $? -eq 1 ]
then
    echo "crontab service starts failed"
    exit 1
else
    echo "compress service is configured"
    echo "crontab settings for compress service:"
    echo "$(crontab -l)"
    exit 0
fi
