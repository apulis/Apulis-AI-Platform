#!/bin/bash
ASCEND_SECLOG="/var/log/ascend_seclog"
sourcedir=$PWD/usr/local/Ascend

logFile="${ASCEND_SECLOG}/ascend_install.log"
installInfo="/etc/ascend_install.info"
. $installInfo
installType=$Driver_Install_Type
username=$UserName
if [ "$username" = "" ]; then
   username=HwHiAiUser
fi

targetdir=$Driver_Install_Path_Param

log() {
    cur_date=`date +"%Y-%m-%d %H:%M:%S"`
    echo "[driver] [$cur_date] "$1 >> $logFile
}

stop_filebeat(){
    filebeat_pid=`ps -ef|grep filebeat | grep -v grep | awk '{print $2}'`
    if [ ${#filebeat_pid} -gt 0 ];then
        for id in $filebeat_pid
        do
            kill -9 $id
            if [ $? -eq 0 ];then
                log "Stopped filebeat successfully"
            else
                log "Failed to stop filebeat!"
                return 1
            fi
        done
    fi
    return 0
}

stop_zipsh(){
    sed -i '/periodic_zip.sh/d' /var/spool/cron/${username} >>/dev/null 2>&1
    sed -i '/compress log/d' /var/spool/cron/${username} >>/dev/null 2>&1
    sed -i '/profiling_periodic_aging.sh/d' /var/spool/cron/${username} >>/dev/null 2>&1
    sed -i '/profiling aging process/d' /var/spool/cron/${username} >>/dev/null 2>&1
    sed -i '/adx_periodic_aging.sh/d' /var/spool/cron/${username} >>/dev/null 2>&1
    sed -i '/adx aging process/d' /var/spool/cron/${username} >>/dev/null 2>&1
    log "Stopped zip service successfully"
}

remove_link(){
    rm -rf /usr/bin/profConf.sh
    rm -rf /usr/bin/logConf.sh
    rm -rf /usr/bin/adxConf.sh
}

toolkit_remove_security() {
    if [ -f /etc/sudoers ];then
        sed -i '/'${username}' ALL=(root) NOPASSWD:\/bin\/date -s \*,.*ide_cmd.sh \*/d' /etc/sudoers
    fi
}
log "[INFO] step into run_driver_tool_uninstall.sh ......"

if [ $installType = "docker" ]; then
    exit 0
fi

stop_filebeat
if [ $? -ne 0 ]; then
    exit 1
fi

stop_zipsh

remove_link

toolkit_remove_security
exit 0
