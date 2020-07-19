#!/bin/sh
ASCEND_SECLOG="/var/log/ascend_seclog"
logFile="${ASCEND_SECLOG}/ascend_install.log"
filelist="/etc/ascend_filelist.info"
sourcedir=$PWD/driver
do_dkms=n
hotreset_status_file="/var/log/hotreset_status.log"

log() {
    cur_date=`date +"%Y-%m-%d %H:%M:%S"`
    echo "[driver] [$cur_date] "$1 >> $logFile
}

show_path() {
    log "target path : $targetdir"
}

update_targetdir() {
    if [ ! -z $1 ] && [ -d $1 ];then
        targetdir=$1
        targetdir=${targetdir%*/}
    else
        log "target path($1) is wrong, upgrade failed"
        return 1
    fi

    show_path
    return 0
}

# 此函数来自/usr/sbin/dkms中distro_version_rpm
distro_version_rpm()
{
    which rpm > /dev/null 2>&1 || { echo unknown; return; }
    local r wp ver dist

    for r in redhat-release sles-release suse-release ovs-release; do
        wp=$(rpm -q --whatprovides "$r") || continue
        ver=$(rpm -q --qf "%{version}\n" ${wp})
        case $r in
            sles*)
                echo sles${ver}
                ;;
            suse*)
                echo suse${ver}
                ;;
            ovs*)
                echo ovm${ver}
                ;;
            redhat*)
                case $wp in
                    redhat*|sl*)
                        ver=$(echo $ver | \
                        sed -e 's/^\([[:digit:]]*\).*/\1/g')
                        echo el${ver}
                        ;;
                    centos*|enterprise*)
                        echo el${ver}
                        ;;
                    fedora*)
                        echo fc${ver}
                        ;;
                    *)
                        echo unknown
                        ;;
                esac
                ;;
            *)
                echo unknown
                ;;
        esac
        return
    done
    echo unknown
}

# 此函数来自/usr/sbin/dkms中distro_version
distro_version()
{
    # What distribution are we running?
    local LSB_DESCRIPTION DISTRIB_ID DISTRIB_RELEASE ver

    # Try the LSB-provided strings first
    if [ -r /etc/lsb-release ]; then
        . /etc/lsb-release
    elif type lsb_release >/dev/null 2>&1; then
        DISTRIB_ID=$(lsb_release -i -s)
        DISTRIB_RELEASE=$(lsb_release -r -s)
    fi

    case ${DISTRIB_ID} in
        Fedora)
            echo fc${DISTRIB_RELEASE}
            ;;
        RedHatEnterprise*|CentOS|ScientificSL)
            # OEL also reports as such; format is 4.7, 5.3
            ver=$(echo "${DISTRIB_RELEASE}" | \
            sed -e 's/^\([[:digit:]]*\).*/\1/g')
            echo el${ver}
            ;;
        SUSE*)
            if [[ $(lsb_release -d -s) =~ Enterprise ]]; then
                echo sles${DISTRIB_RELEASE}
            else
                echo suse${DISTRIB_RELEASE}
            fi
            ;;
        *)
            if [ ${DISTRIB_ID} ] && [ ${DISTRIB_RELEASE} ]; then
                echo "${DISTRIB_ID}${DISTRIB_RELEASE}"
            else
                distro_version_rpm
            fi
            ;;
    esac
}

# 此函数来自/usr/sbin/dkms中override_dest_module_location
override_dest_module_location()
{
    local orig_location="$1"
    case "$running_distribution" in
    sles[123456789])
        ;;
    suse[123456789])
        ;;
    suse10\.[01])
        ;;
    fc*)
        echo "/extra" && return
        ;;
    el*)
        echo "/extra" && return
        ;;
    ovm*)
        echo "/extra" && return
        ;;
    sles*)
        echo "/updates" && return
        ;;
    suse*)
        echo "/updates" && return
        ;;
    Ubuntu*)
        echo "/updates/dkms" && return
        ;;
    Debian*)
        echo "/updates/dkms" && return
        ;;
    *)
        ;;
    esac
    echo "$orig_location"
}

# 获取dkms输出ko路径
ko_output_absolute_path(){
    running_distribution="`distro_version`"
    local ko_output_path=`override_dest_module_location "/updates"`
    local absolute_path="/lib/modules/`uname -r`${ko_output_path}"
    echo $absolute_path
}

drv_vermagic_check() {
    pcie_hdc_ko=$sourcedir/host/drv_pcie_hdc_host.ko

    kernel_ver=$(uname -r)
    ko_vermagic=$(modinfo $pcie_hdc_ko | grep vermagic | awk '{print $2}')

    primary_kernel_ver=$(echo $kernel_ver | awk -F [-+] '{print $1}')
    primary_ko_vermagic=$(echo $ko_vermagic | awk -F [-+] '{print $1}')
    if [ "$primary_kernel_ver"x != "$primary_ko_vermagic"x ];then
        log "current kernel primary version($kernel_ver) is different from the driver($ko_vermagic)."
        return 1
    fi

    config_modversions=$(cat /boot/config-$kernel_ver | grep CONFIG_MODVERSIONS)
    log "config_modversions is : $config_modversions"
    if [ "$config_modversions"x != "CONFIG_MODVERSIONS=y"x ];then
        # full check vermagic
        if [ "$kernel_ver"x != "$ko_vermagic"x ];then
            log "driver ko vermagic($ko_vermagic) is different from the os($kernel_ver), need rebuild driver"
            return 1
        fi

        log "current kernel version($kernel_ver) is equal to the driver."
    else
        # don't insmod drv_pcie_host.ko, it will be failed when load boot files
        # test insmod the driver
        log "drv_pcie_hdc_host.ko might not load, try to insmod it"
        insmod $pcie_hdc_ko >drv_vermagic_check_tmp_log 2>&1
        cat drv_vermagic_check_tmp_log | grep -i "Invalid module format" >>/dev/null
        if [ $? -eq 0 ];then
            log "driver in run packet test load failed, need rebuild driver"
            log "test load info :"
            cat drv_vermagic_check_tmp_log >>$logFile
            rm drv_vermagic_check_tmp_log
            return 1
        fi

        rm drv_vermagic_check_tmp_log
        log "driver in run packet could load ok, no need rebuild"
    fi

    return 0
}

drv_dkms_dependent_check() {
    log "dkms environment check..."

    dpkg-query -s dkms 2>/dev/null | grep "Status: install ok installed" >/dev/null 2>&1
    if [ $? -ne 0 ];then
        rpm -qa 2>/dev/null | grep dkms >/dev/null 2>&1
        if [ $? -ne 0 ];then
            log "The program 'dkms' is currently not installed."
            log "dkms environment check failed."
            return 1
        fi
    fi

    dpkg-query -s gcc 2>/dev/null | grep "Status: install ok installed" >/dev/null 2>&1
    if [ $? -ne 0 ];then
        rpm -qa 2>/dev/null | grep gcc >/dev/null 2>&1
        if [ $? -ne 0 ];then
            log "The program 'gcc' is currently not installed."
            log "dkms environment check failed."
            return 1
        fi
    fi

    dpkg-query -s linux-headers-$(uname -r) 2>/dev/null | grep "Status: install ok installed" >/dev/null 2>&1
    if [ $? -ne 0 ];then
        rpm -qa 2>/dev/null | grep kernel-headers-$(uname -r) >/dev/null 2>&1
        if [ $? -ne 0 ];then
            log "linux headers is currently not installed."
            log "dkms environment check failed."
            return 1
        fi
    fi

    do_dkms=y
    log "dkms environment check success"
    return 0
}

drv_dkms_env_check() {
    sources=$sourcedir/kernel

    # to check whether EVB environment.
    if [ ! -d $sources ];then
        drv_vermagic_check
        if [ $? -ne 0 ];then
            log "driver vermagic check failed."
            return 1
        fi
        return 0
    else
        drv_dkms_dependent_check
        if [ $? -ne 0 ];then
            log "driver dkms dependence check failed"

            # to check whether drivers' version is equal to kernel.
            drv_vermagic_check
            if [ $? -ne 0 ];then
                log "driver vermagic check failed."
                return 1
            fi
            return 0
        fi
        return 0
    fi
}

filelist_write_one() {
    echo "[$1] $2" >>$filelist
}

filelist_remove() {
    if [ ! -f $filelist ];then
        return 0
    fi

    while read file
    do
        file=$(echo $file | grep "\[$1\]" | awk '{print $2}')
        local tmp_file=${file##*/}
        ls $sys_path | grep $tmp_file > /dev/null 2>&1
        if [ $? -eq 0 ];then
            rm -rf ${sys_path}/${tmp_file}*
        fi

        # remove from filelist
        sed -i 's#'$file'#DAVINCI_FILELIST_DEL#;/DAVINCI_FILELIST_DEL/d' $filelist
    done < $filelist

    return 0
}

driver_auto_insmod() {
    if [ -d $sys_path ];then
        filelist_remove driver_ko
        depmod
    else
        mkdir -p $sys_path
        if [ $? -ne 0 ];then
            log "mkdir -p $sys_path failed"
            return 1
        fi
        log "create path: $sys_path success"
    fi

    ls $targetdir/host/ >hiai_ko_file_list_tmp.log
    while read file
    do
        ln -sf $targetdir/host/$file $sys_path/$file
    done < hiai_ko_file_list_tmp.log
    rm hiai_ko_file_list_tmp.log

    depmod

    log "driver ko auto insmod config success"
    return 0
}

load_file_rename() {
    if [ -f $1 ] && [ ! -f $2 ];then
        mv $1 $2
    fi
}

load_file_check() {
        if [ ! -f $1/davinci_cloud.cpio.gz ] || [ ! -f $1/davinci_cloud.image ] || [ ! -f $1/davinci_cloud_tee.bin ];then
                log "host load file is missing, failed"
                return 1
        fi

        return 0
}

device_pcie_load_file_rename() {
        if [ ! -d $targetdir/device ];then
                return 0
        fi

        cd $targetdir/device
        load_file_rename filesystem-le.cpio.gz davinci_cloud.cpio.gz
        load_file_rename Image davinci_cloud.image
        load_file_rename tee.bin davinci_cloud_tee.bin
        load_file_rename IMU_task.bin davinci_cloud_IMU_task.bin
        load_file_rename imp.bin davinci_cloud_imp.bin
        load_file_rename nve.bin davinci_cloud_nve.bin
        cd - >/dev/null

        load_file_check $targetdir/device
        if [ $? -ne 0 ];then
                log "load_file_check $targetdir/device failed"
                return 1
        fi

        log "device pcie load files rename success"
        return 0
}

driver_ko_install() {
    drv_dkms_env_check
    if [ $? -ne 0 ];then
        log "drv_dkms_env_check failed"
        return 1
    fi

    sys_path=$(ko_output_absolute_path)
    log "ko put path $sys_path"
    if [ ! -d $targetdir/host ];then
        log "ko is lost"
        return 1
    fi

    if [ "$do_dkms" = "n" ];then
        driver_auto_insmod
        if [ $? -ne 0 ];then
            log "driver_auto_insmod failed"
            return 1
        fi
    else
        log "rebuild the hiai kernel driver by dkms..."
        ./driver/script/run_driver_dkms_install.sh
        if [ $? -ne 0 ];then
            log "dkms install failed"
            return 1
        fi
    fi

    ls $targetdir/host/ >hiai_ko_file_list_tmp.log
    while read file
    do
        filelist_write_one driver_ko $sys_path/$file
    done < hiai_ko_file_list_tmp.log
    rm hiai_ko_file_list_tmp.log

    return 0
}

drv_host_ko_copy() {
    local must_dir="host device kernel" dir_item

    for dir_item in $must_dir
    do
        if [ -d ${sourcedir}/${dir_item} ]; then
            rm -rf ${targetdir}/${dir_item}
            if [ $? -ne 0 ];then
                log "rm -rf ${targetdir}/${dir_item} failed"
                return 1
            fi
            log "rm -rf ${targetdir}/${dir_item} success"

            mkdir -p ${targetdir}/${dir_item}
            if [ $? -ne 0 ];then
                log "mkdir -p ${targetdir}/${dir_item} failed"
                return 1
            fi
            log "mkdir -p ${targetdir}/${dir_item} success"

            cp -rf ${sourcedir}/${dir_item} ${targetdir}/${dir_item}/../
            if [ $? -ne 0 ];then
                log "cp ${targetdir}/${dir_item} failed"
                return 1
            fi
            log "cp ${targetdir}/${dir_item} success"
        fi
    done

    return 0
}

filelist_remove() {
    if [ ! -f $filelist ];then
        return 0
    fi

    while read file
    do
        file=$(echo $file | grep "\[$1\]" | awk '{print $2}')
        local tmp_file=${file##*/}
        ls $sys_dir | grep $tmp_file > /dev/null 2>&1
        if [ $? -eq 0 ];then
            rm -rf ${sys_dir}/${tmp_file}*
        fi

        # remove from filelist
        sed -i 's#'$file'#DAVINCI_FILELIST_DEL#;/DAVINCI_FILELIST_DEL/d' $filelist
    done < $filelist

    log "delete file($1) and remove it from filelist success"
    return 0
}

driver_auto_insmod_unload() {
    sys_dir=$(ko_output_absolute_path)
    if [ -d $sys_dir ]; then
        filelist_remove driver_ko
        # upgrade the relation
        depmod

        log "driver auto insmod unload success"
    fi

    return 0
}

progress_bar() {
    local parent_progress=$1 weight=$2 child_progress=$3
    local output_progress
    output_progress=`awk 'BEGIN{printf "%d\n",('$parent_progress'+'$weight'*'$child_progress'/100)}'`
    echo "upgradePercentage:${output_progress}%"
    log "upgradePercentage:${output_progress}%"
}

changeMode() {
    chmod -Rf 550 ${targetdir}/host
    chmod -Rf 550 ${targetdir}/device
}

# start!

while true
do
    case "$1" in
    --upgrade)
        update_targetdir $2
        if [ $? -ne 0 ];then
            exit 1
        fi

        log "set hot reset scan success in delay upgrade"
        echo "scan_success" > "$hotreset_status_file"

        # should be install path run_driver_uninstall.sh
        ./driver/script/run_driver_uninstall.sh --uninstall $2
        if [ $? -ne 0 ];then
            exit 1
        fi

        ./driver/script/run_driver_install.sh "install" $2 $3
        if [ $? -ne 0 ];then
            exit 1
        fi

        rm -f "$hotreset_status_file"

        exit 0
        ;;
    --*)
        shift
        continue
        ;;
    *)
        break
        ;;
    esac
done

exit 1


