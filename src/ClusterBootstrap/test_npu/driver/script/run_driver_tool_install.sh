#!/bin/bash
ASCEND_SECLOG="/var/log/ascend_seclog"
installInfo="/etc/ascend_install.info"
logFile="${ASCEND_SECLOG}/ascend_install.log"

. $installInfo
installType=$Driver_Install_Type
username=$UserName
usergroup=$UserGroup
if [ x$username = "x" ]; then
    username=HwHiAiUser
    usergroup=HwHiAiUser
fi

targetdir=${Driver_Install_Path_Param}/driver
install_tools_dir=${Driver_Install_Path_Param}/driver/tools
ide_daemon_path=`eval echo "~${username}"`/ide_daemon
slog_path="/var/log/npu/slog/slogd"

log() {
    cur_date=`date +"%Y-%m-%d %H:%M:%S"`
    echo "[driver] [tool] [$cur_date] "$1 >> $logFile
}

# ada和hdcd进程拉起
cp_cfg_files(){
    if [ -d $install_tools_dir ];then
        mkdir -p $ide_daemon_path
        chmod -f 700 $ide_daemon_path
        chown -f $username:$usergroup $ide_daemon_path

        cp -f $install_tools_dir/ide_daemon.cfg $ide_daemon_path
        cp -f $install_tools_dir/ide_daemon_*.pem $ide_daemon_path
        chmod -f 640 $ide_daemon_path/ide_daemon.cfg
        chmod -f 600 $ide_daemon_path/ide_daemon_*.pem
        chown -f $username:$usergroup $ide_daemon_path/ide_daemon.cfg
        chown -f $username:$usergroup $ide_daemon_path/ide_daemon_*.pem
        chmod -f 550 ${Driver_Install_Path_Param}/driver/tools/ide_cmd.sh
        chown -f root:root ${Driver_Install_Path_Param}/driver/tools/ide_cmd.sh
        mkdir -p /var/log/npu
        chmod -f 750 /var/log/npu
        chown -f $username:$usergroup /var/log/npu

        # profiling
        mkdir -p /var/log/npu/profiling
        chmod -f 750 /var/log/npu/profiling
        chown -Rf $username:$usergroup /var/log/npu/profiling

        # slog
        mkdir -p /var/log/npu/slog/slogd
        mkdir -p /var/log/npu/slog/container
        chmod -f 750 /var/log/npu/slog
        chmod -f 750 /var/log/npu/slog/slogd
        chmod -f 750 /var/log/npu/slog/container
        chown -Rf $username:$usergroup /var/log/npu/slog

        mkdir -p /var/log/npu/oplog
        chmod -f 750 /var/log/npu/oplog
        chown -Rf $username:$usergroup /var/log/npu/oplog

        mkdir -p $slog_path
        chmod -f 750 $slog_path
        chown -Rf $username:$usergroup $slog_path

        mkdir -p /usr/slog
        chmod -f 750 /usr/slog
        chown -Rf $username:$usergroup /usr/slog

        mkdir -p /var/log/npu/toolchain/run
        chmod 750 /var/log/npu/toolchain/
        chmod 750 /var/log/npu/toolchain/run
        chown -Rf $username:$usergroup /var/log/npu/toolchain

        # bbox
        mkdir -p /var/log/npu/bbox
        chmod -f 750 /var/log/npu/bbox
        chown -Rf $username:$usergroup /var/log/npu/bbox

        mkdir -p /var/log/npu/hisi_logs
        chmod -f 750 /var/log/npu/hisi_logs
        chown -Rf $username:$usergroup /var/log/npu/hisi_logs
        find /var/log/npu/hisi_logs -name history.log | xargs chmod u+w >> /dev/null 2>&1

        # adx
        mkdir -p /var/log/npu/ide_daemon/run
        mkdir -p /var/log/npu/ide_daemon/run/dump
        chmod -f 750 /var/log/npu/ide_daemon
        chmod -f 750 /var/log/npu/ide_daemon/run
        chmod -f 750 /var/log/npu/ide_daemon/dump
        chown -Rf $username:$usergroup /var/log/npu/ide_daemon

        # coredump
        mkdir -p /var/log/npu/coredump
        chmod -f 750 /var/log/npu/coredump
        chown -Rf $username:$usergroup /var/log/npu/coredump

        # conf
        mkdir -p /var/log/npu/conf/adx
        mkdir -p /var/log/npu/conf/slog
        mkdir -p /var/log/npu/conf/bbox
        mkdir -p /var/log/npu/conf/profiling
        chmod -f 750 /var/log/npu/conf
        chmod -f 750 /var/log/npu/conf/adx
        chmod -f 750 /var/log/npu/conf/slog
        chmod -f 750 /var/log/npu/conf/bbox
        chmod -f 750 /var/log/npu/conf/profiling

        if [ -f $install_tools_dir/adx_aging.conf ]; then
            cp -f $install_tools_dir/adx_aging.conf /var/log/npu/conf/adx
            chmod -f 640 /var/log/npu/conf/adx/adx_aging.conf
        fi
        if [ -f $install_tools_dir/slog.conf ]; then
            cp -f $install_tools_dir/slog.conf /var/log/npu/conf/slog
            chmod -f 640 /var/log/npu/conf/slog/slog.conf
        fi
        if [ -f $install_tools_dir/profile.cfg ]; then
            cp -f $install_tools_dir/profile.cfg /var/log/npu/conf/profiling
            chmod -f 640 /var/log/npu/conf/profiling/profile.cfg
        fi
        if [ -f $install_tools_dir/bbox.conf ]; then
            cp -f $install_tools_dir/bbox.conf /var/log/npu/conf/bbox
            chmod -f 640 /var/log/npu/conf/bbox/bbox.conf
        fi

        chown -Rf $username:$usergroup /var/log/npu/conf

        log "set config files succeed."
    fi
}

cp_zipconf(){
    if [ ! -f $install_tools_dir/ziplog.conf ]; then
        return 0
    fi
    cp $install_tools_dir/ziplog.conf /var/log/npu/conf/slog/
    if [ $? -ne 0 ];then
        log "copy ziplog.conf failed"
        return 1
    fi
    chmod -f 640 /var/log/npu/conf/slog/ziplog.conf
    chown -f $username:$usergroup /var/log/npu/conf/slog/ziplog.conf
    return 0
}

cp_profiling_aging_conf(){
    if [ ! -f $install_tools_dir/profiling_aging.conf ]; then
        return 0
    fi
    cp -f $install_tools_dir/profiling_aging.conf /var/log/npu/conf/profiling/
    if [ $? -ne 0 ];then
        log "copy profiling_aging.conf failed"
        return 1
    fi
    chmod -f 600 /var/log/npu/conf/profiling/profiling_aging.conf
    chown -f $username:$usergroup /var/log/npu/conf/profiling/profiling_aging.conf
    log "set cfg-files about profiling success"
    return 0
}

install_filebeat(){
    # copy filebeat config file
    if [ -f $install_tools_dir/filebeat.yml ]; then
        cp -f $install_tools_dir/filebeat.yml /var/log/npu/conf/slog
        chmod -f 640 /var/log/npu/conf/slog/filebeat.yml
    fi
    if [ -f $install_tools_dir/filebeat_lab.yml ]; then
        cp -f $install_tools_dir/filebeat_lab.yml /var/log/npu/conf/slog
        chmod -f 640 /var/log/npu/conf/slog/filebeat_lab.yml
    fi
    # try to setting filbeat_lab
    su - $username -c "bash $targetdir/tools/settingFilbeat.sh $targetdir"
    if [ $? -ne 0 ];then
      log "skipped setting filbeat_lab"
    fi
    # change owner of new configured filebeat file
    chown -Rf $username:$usergroup /var/log/npu/conf/slog
}

install_link() {
    ln -sf $install_tools_dir/profConf.sh /usr/bin/profConf.sh
    ln -sf $install_tools_dir/logConf.sh /usr/bin/logConf.sh
    ln -sf $install_tools_dir/adxConf.sh /usr/bin/adxConf.sh
}

toolkit_security_configuration() {
    if [ ! -f /etc/sudoers ];then
        echo "${username} ALL=(root) NOPASSWD:/bin/date -s *,${Driver_Install_Path_Param}/driver/tools/ide_cmd.sh *" >> /etc/sudoers
        chmod a-w /etc/sudoers
    else
        local set_write_flag="false"

        if [ ! -w "/etc/sudoers" ];then
            chmod u+w /etc/sudoers
            set_write_flag="true"
        fi

        grep -q "${username} ALL=(root) NOPASSWD:/bin/date -s \*,${Driver_Install_Path_Param}/driver/tools/ide_cmd.sh \*" /etc/sudoers
        if [ $? -ne 0 ];then
            echo "${username} ALL=(root) NOPASSWD:/bin/date -s *,${Driver_Install_Path_Param}/driver/tools/ide_cmd.sh *" >> /etc/sudoers
        fi

        if [ $set_write_flag = "true" ];then
            chmod u-w /etc/sudoers
        fi

        unset set_write_flag
    fi

    return 0
}
log "[INFO] step into run_driver_tool_install.sh ......"

if [ $installType = "docker" ]; then
    exit 0
fi

cp_cfg_files

install_link

cp_zipconf
if [ $? -ne 0 ]; then
    exit 1
fi

cp_profiling_aging_conf
if [ $? -ne 0 ]; then
    exit 1
fi

install_filebeat
toolkit_security_configuration
exit 0
