#!/bin/sh
ASCEND_SECLOG="/var/log/ascend_seclog"
logFile="${ASCEND_SECLOG}/ascend_install.log"
installInfo="/etc/ascend_install.info"

. $installInfo
targetdir=${Driver_Install_Path_Param}
targetdir=${targetdir%*/}

module_name=davinci_ascend
module_version=1.0

log() {
    cur_date=`date +"%Y-%m-%d %H:%M:%S"`
    echo "[driver] [$cur_date] "$1 >> $logFile
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

# $1 : module_name
dkms_remove() {
    dkms status | grep $1 >/dev/null
    if [ $? -eq 0 ];then
        tmp_version=$(dkms status | grep $1 | grep $(uname -r) | awk '{print $2}' | awk -F [,:] '{print $1}')
        if [ "$tmp_version"x = ""x ];then
            tmp_version=$(dkms status | grep $1 | grep "added" | awk '{print $2}' | awk -F [,:] '{print $1}')
            if [ "$tmp_version"x = ""x ];then
                tmp_version=$(dkms status | grep $1 | head -n 1 | awk '{print $2}' | awk -F [,:] '{print $1}')
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

dkms_copy_source() {
    sources=$targetdir/driver/kernel
    if [ ! -d $sources ];then
        log "[ERROR]hiai kernel driver source missing, failed"
        return 1
    fi

    module_name=$(cat $sources/dkms.conf | grep PACKAGE_NAME | awk -F '"' '{print $2}')
    module_version=$(cat $sources/dkms.conf | grep PACKAGE_VERSION | awk -F '"' '{print $2}')
    log "[INFO]dkms module name : $module_name, version : $module_version"

    dkms_remove $module_name

    src_dir=/usr/src/$module_name-$module_version
    if [ -d $src_dir ];then
        rm -rf $src_dir
        log "[INFO]rm -rf $src_dir success"
    fi

    mkdir -p $src_dir
    if [ $? -ne 0 ];then
        log "[ERROR]mkdir -p $src_dir failed"
        return 1
    fi
    log "[INFO]create $src_dir success"

    cp -rf $sources/* $src_dir/
    if [ $? -ne 0 ];then
        log "[ERROR]cp -rf $sources/ $src_dir/ failed"
        chmod -R 500 $src_dir/*
        return 1
    fi
    chmod -R 500 $src_dir/*
    log "[INFO]cp -rf $sources/ $src_dir/ success"

    return 0
}

dkms_add() {
    log "[INFO]run dkms add command..."
    dkms add -m $module_name -v $module_version >>$logFile 2>&1
}

dkms_build() {
    log "[INFO]run dkms build command..."
    dkms build -m $module_name -v $module_version >>$logFile 2>&1
}

dkms_install() {
    log "[INFO]run dkms install command..."
    dkms install -m $module_name -v $module_version --no-initrd >>$logFile 2>&1
}

dkms_output_check() {
    sys_dir=$(ko_output_absolute_path)
    if [ ! -d $sys_dir ]; then
        if [ ! -d $sys_dir ]; then
            log "[ERROR]dkms output is NONE, dkms failed"
            return 1
        fi
    fi

    sources=$targetdir/driver/kernel
    cat $sources/dkms.conf | grep BUILT_MODULE_NAME >hiai_dkms_output_tmp.log
    while read file
    do
        file=$(echo $file | awk -F '"' '{print $2}')
        ls $sys_dir | grep $file.ko > /dev/null 2>&1
        if [ $? -ne 0 ];then
            log "[ERROR]There has no $sys_dir/$file.ko, dkms install failed"
            return 1
        fi
    done < hiai_dkms_output_tmp.log
    rm hiai_dkms_output_tmp.log

    log "[INFO]dkms output files check success"
    return 0
}

add_readme_to_install_path() {
    if [ -d $targetdir/driver/host ];then
        echo "warning:" >$targetdir/driver/host/README.txt
        echo "davinci hiai host kernel driver is managed by dkms, driver ko in this path is no more effective." >>$targetdir/driver/host/README.txt
        echo "driver ko installed by dkms is in : $sys_dir." >>$targetdir/driver/host/README.txt
    fi

    log "[INFO]run install path add $targetdir/driver/host/README.txt success"
}

dkms_copy_source
if [ $? -ne 0 ];then
    exit 1
fi

log "[INFO]driver dkms log >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>"

dkms_add
dkms_build
dkms_install

log "[INFO]driver dkms log <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<"

dkms_output_check
if [ $? -ne 0 ];then
    echo -e "\033[31m dkms install failed, details in : $logFile \033[0m"
    log "[ERROR]dkms install files check failed"
    exit 1
fi

add_readme_to_install_path

exit 0

