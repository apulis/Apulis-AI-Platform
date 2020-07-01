# This shell used for user to uninstall the dkms alone
#!/bin/sh
ASCEND_SECLOG="/var/log/ascend_seclog"
logFile="${ASCEND_SECLOG}/ascend_install.log"
installInfo="/etc/ascend_install.info"

. $installInfo
targetdir=${Driver_Install_Path_Param}
targetdir=${targetdir%*/}

log() {
    cur_date=`date +"%Y-%m-%d %H:%M:%S"`
    echo "[driver] [$cur_date] "$1 >> $logFile
}

# $1 : module_name
dkms_remove() {
    dkms status | grep $1 >/dev/null
    if [ $? -eq 0 ];then
        tmp_version=$(dkms status | grep $1 | grep $(uname -r) | awk '{print $2}' | awk -F [,:] '{print $1}')
        if [ "$tmp_version"x = ""x ];then
            tmp_version=$(dkms status | grep $1 | grep "added" | awk '{print $2}' | awk -F [,:] '{print $1}')
            if [ "$tmp_version"x = ""x ];then
                tmp_version=$(dkms status | grep $1 | grep -n 1 | awk '{print $2}' | awk -F [,:] '{print $1}')
            fi
        fi

        if [ "$tmp_version"x = ""x ];then
            log "[WARNING]dkms version get faild"
            log "[WARNING]dkms status : "
            dkms status | grep $1 >>$logFile 2>&1
            return
        fi

        log "[INFO]davinci hiai kernel driver is installed($1-$tmp_version), delete and reinstall"

        dkms uninstall -m $1 -v $tmp_version --no-initrd >>$logFile 2>&1
        dkms remove -m $1 -v $tmp_version --all --no-initrd >>$logFile 2>&1
        log "[INFO]dkms remove success"

        tmp_src_dir=/usr/src/$1-$tmp_version
        if [ -d $tmp_src_dir ];then
            rm -rf $tmp_src_dir
            log "[INFO]rm -rf $tmp_src_dir success"
        fi
    fi
}

dkms_uninstall_all() {
    sources=$targetdir/driver/kernel
    if [ -d $sources ];then
        dpkg-query -s dkms 2>/dev/null | grep "Status: install ok installed" >/dev/null 2>&1
        if [ $? -ne 0 ];then
            rpm -qa 2>/dev/null | grep dkms >/dev/null 2>&1
            if [ $? -ne 0 ];then
                log "[INFO]no dkms, clear success"
                return 0
            fi
        fi

        module_name=$(cat $sources/dkms.conf | grep PACKAGE_NAME | awk -F '"' '{print $2}')
        module_version=$(cat $sources/dkms.conf | grep PACKAGE_VERSION | awk -F '"' '{print $2}')

        dkms_remove $module_name

        src_dir=/usr/src/$module_name-$module_version
        if [ -d $src_dir ];then
            rm -rf $src_dir
            log "[INFO]rm -rf $src_dir success"
        fi

        log "[INFO]clear dkms success"
    fi

    return 0
}

# start

dkms_uninstall_all

exit 0


