#!/bin/sh
ASCEND_SECLOG="/var/log/ascend_seclog"
targetdir=/usr/local/Ascend
sourcedir=$PWD/usr/local/Ascend
installInfo="/etc/ascend_install.info"
. $installInfo
username=$UserName
usergroup=$UserGroup
if [ "$username" = "" ]; then
    username=HwHiAiUser
    usergroup=HwHiAiUser
fi

filelist="/etc/ascend_filelist.info"
logFile="${ASCEND_SECLOG}/ascend_install.log"

log() {
    cur_date=`date +"%Y-%m-%d %H:%M:%S"`
    echo "[driver] [$cur_date] "$1 >> $logFile
}
chattrDriver() {
    . $installInfo
    chattr +i $Driver_Install_Path_Param/*.sh > /dev/null 2>&1
    chattr +i $Driver_Install_Path_Param/add-ons/*.so* > /dev/null 2>&1
    chattr -R +i $Driver_Install_Path_Param/driver/* > /dev/null 2>&1
    if [ $Driver_Install_Type = "docker" ]; then
        if [ -d $Driver_Install_Path_Param/driver/script ];then
            chattr -R +i $Driver_Install_Path_Param/driver/script/* > /dev/null 2>&1
        fi
        if [ -d $Driver_Install_Path_Param/driver/tools ];then
            chattr -R +i $Driver_Install_Path_Param/driver/tools/* > /dev/null 2>&1
        fi
        if [ -d $Driver_Install_Path_Param/driver/lib64 ];then
            chattr -R +i $Driver_Install_Path_Param/driver/lib64/* > /dev/null 2>&1
        fi
        log "[INFO]add chattr for driver docker mode"
    fi
    log "[INFO]add chattr for driver package"
}
unchattrDriver() {
    . $installInfo
    chattr -i $Driver_Install_Path_Param/*.sh > /dev/null 2>&1
    chattr -i $Driver_Install_Path_Param/add-ons/*.so*> /dev/null 2>&1
    chattr -R -i $Driver_Install_Path_Param/driver/* > /dev/null 2>&1
    if [ $Driver_Install_Type = "docker" ]; then
        if [ -d $Driver_Install_Path_Param/driver/script ];then
            chattr -R -i $Driver_Install_Path_Param/driver/script/* > /dev/null 2>&1
        fi
        if [ -d $Driver_Install_Path_Param/driver/tools ];then
            chattr -R -i $Driver_Install_Path_Param/driver/tools/* > /dev/null 2>&1
        fi
        if [ -d $Driver_Install_Path_Param/driver/lib64 ];then
            chattr -R -i $Driver_Install_Path_Param/driver/lib64/* > /dev/null 2>&1
        fi
        log "[INFO]unchattr for driver docker mode"
    fi
    log "[INFO]unchattr  for driver package"
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

show_path() {
    log "[INFO]target path : $targetdir"
}

update_targetdir() {
    if [ ! -z $1 ] && [ -d $1 ];then
        targetdir=$1
        targetdir=${targetdir%*/}
    else
        log "[ERROR]target path($1) is wrong, uninstall failed"
        return 1
    fi

    show_path
    return 0
}

drv_ddr_remove() {
    if [ -e ${targetdir}/driver/device/ascend_910.crl ]; then
        rm -f ${targetdir}/driver/device/ascend_910.crl
    fi
    remove_result=n
    if [ -f ${targetdir}/driver/script/install_common_parser.sh ] && [ -f ${targetdir}/driver/script/filelist.csv ]; then
        ${targetdir}/driver/script/install_common_parser.sh --remove $Driver_Install_Type $1 ${targetdir}/driver/script/filelist.csv
        if [ $? -eq 0 ]; then
            remove_result=y
        fi
    fi
    if [ $remove_result = n ]; then
        ./install_common_parser.sh --remove $Driver_Install_Type $1 ./filelist.csv
        if [ $? -eq 0 ];then
            remove_result=y
        fi
    fi
    if [ $remove_result = y ]; then
        log "[INFO]rm -rf ${targetdir}/driver success"
        return 0
    else
        log "[ERROR]rm -rf ${targetdir}/driver failed"
        return 1
    fi
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
            log "[INFO]rm ${tmp_file}"
        fi
        weak_updates_ko=$(find /lib/modules/$(uname -r)/ -name "${tmp_file}*")
        if [ ! -z "${weak_updates_ko}" ]; then
            rm -f "${weak_updates_ko}" > /dev/null 2>&1
            log "[INFO]rm -f [${weak_updates_ko}] success"
        fi
        # remove from filelist
        sed -i 's#'$file'#DAVINCI_FILELIST_DEL#;/DAVINCI_FILELIST_DEL/d' $filelist
    done < $filelist

    return 0
}

driver_delete_temp() {
    local sys_dir=/lib/modules/$(uname -r)/kernel/drivers/davinci
    if [ -d $sys_dir ]; then
        rm -rf $sys_dir
        depmod > /dev/null 2>&1
        log "[INFO]rm -rf $sys_dir success"
    fi
}

driver_auto_insmod_unload() {
    sys_dir=$(ko_output_absolute_path)
    if [ -d $sys_dir ]; then
        filelist_remove driver_ko
        # upgrade the relation
        depmod > /dev/null 2>&1

        log "[INFO]driver auto insmod unload success"
    fi

    driver_delete_temp

    return 0
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
            log "[INFO]dkms status : "
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

pcie_load_path_unset() {
    if [ -f "/lib/davinci.conf" ]; then
        rm /lib/davinci.conf
        if [ $? -ne 0 ];then
            log "[INFO]rm /lib/davinci.conf failed"
            return 1
        fi

        log "[INFO]remove /lib/davinci.conf success"
    fi

    return 0
}

pcivnic_mac_unset()
{
    conf_file=/etc/d-pcivnic.conf
    udev_rule_file=/etc/udev/rules.d/70-persistent-net.rules
    max_dev_num=0

    if [ -f $conf_file ];then
        rm $conf_file
    fi

    if [ -f $udev_rule_file ];then
        for dev_id in $(seq 0 $max_dev_num)
        do
            ifname="endvnic"
            # del
            sed -i 's#'$ifname'#PCIVNIC_TMP_VARIABLE_WILL_DEL#;/PCIVNIC_TMP_VARIABLE_WILL_DEL/d' $udev_rule_file
        done
    fi

    log "[INFO]pcivnic mac unset success"
    return 0
}

uninstall_tool() {
    if [ -f $Driver_Install_Path_Param/driver/script/run_driver_tool_uninstall.sh ]; then
        $Driver_Install_Path_Param/driver/script/run_driver_tool_uninstall.sh --uninstall
        if [ $? -ne 0 ]; then
            log "[ERROR]tool uninstall failed."
            return 1
        fi
    else
        log "[WARNING]tool uninstall script not found."
    fi
    return 0
}

delTargetdir() {
    if [ ${targetdir} = "/usr/local/Ascend" ] && [ `ls ${targetdir} | wc -l` -eq 0 ];then
        rm -rf ${targetdir}
    fi
}

drv_remove_security() {
    if [ -f /etc/sudoers ];then
        sed -i '/'${username}' ALL=(root) NOPASSWD:\/usr\/bin\/device_boot_init.sh \*/d' /etc/sudoers
    fi
}

# start!

while true
do
    case "$1" in
    --uninstall)
        update_targetdir $2
        if [ $? -ne 0 ];then
            exit 1
        fi
        unchattrDriver
        if [ $Driver_Install_Type = "docker" ]; then
            uninstall_tool
            drv_ddr_remove $targetdir
            if [ $? -ne 0 ];then
                exit 1
            else
                delTargetdir
                log "[INFO]docker mode,not rm ko "
                log "[INFO]docker mode,Driver package uninstall success."
                exit 0
            fi
        fi
        dkms_uninstall_all
        driver_auto_insmod_unload

        drv_remove_security
        uninstall_tool
        # hotreset device, must be before intall file removed
        if [ ! -f ${targetdir}/driver/script/device_hot_reset.sh ]; then
            log "[INFO]no hotreset file: "${targetdir}/driver/script/device_hot_reset.sh
            if [ ! -f ${sourcedir}/device_hot_reset.sh ]; then
                log "[INFO]no hotreset file: "${sourcedir}/device_hot_reset.sh
                echo "no hot reset file"
                exit 0
            else
                ${sourcedir}/device_hot_reset.sh
            fi
        else
            ${targetdir}/driver/script/device_hot_reset.sh
        fi
        drv_ddr_remove $targetdir
        if [ $? -ne 0 ];then
            chattrDriver
            exit 1
        fi

        pcie_load_path_unset
        if [ $? -ne 0 ];then
            chattrDriver
            exit 1
        fi

        pcivnic_mac_unset
        delTargetdir

        if [ -f $filelist ]; then
            rm -f $filelist
        fi
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


