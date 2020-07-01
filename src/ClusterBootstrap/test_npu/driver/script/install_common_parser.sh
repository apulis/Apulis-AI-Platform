#!/bin/bash

file_list=""

# 修改各文件及目录的权限
change_mod() 
{
    if [ $2 != "NA" ]; then
        chmod -R $2 $1
        if [ $? -ne 0 ]; then
            echo "Error: $1 chmod failed!"
            exit -1
        fi
    fi
}

# 修改各文件及目录的属性
change_own() 
{
    if [ $2 != "NA" ]; then
        if [ $2 = "\$username:\$usergroup" ]; then
            chown -R $username:$usergroup $1
        else
            chown -R $2 $1
        fi
        if [ $? -ne 0 ]; then
            echo "Error: $1 chown failed!"
            exit -1
        fi
    fi
}

#创建软连接
create_softlink()
{
    if [ $2 != "NA" ];then
        if [ ! -d $(dirname $2) ];then
            mkdir -p $(dirname $2)
        fi
        link_target=$(cd `dirname $1`;pwd)/$(basename $1)
        ln -s $link_target $2
        if [ $? -ne 0 ]; then
            echo "Error: $2 softlink to $link_target failed!"
            exit -1
        fi
    fi
}

# 创建文件夹
create_folder() 
{
    if [ ! -d $1 ]; then
        mkdir -p $1
        if [ $? -ne 0 ]; then
            echo "Error: $1 mkdir failed!"
            exit -1
        fi
    fi
    if [ $# -ge 4 ];then
        change_mod $1 $2
        change_own $1 $3
        create_softlink $1 $4
    fi
}

# 拷贝文件
copy_file()
{
    if [ -e $1 ];then
        cp -r $1 $2
        if [ $? -ne 0 ]; then
            echo "Error: $1 copy failed!"
            exit -1
        fi
        if [ $# -ge 5 ];then
            change_mod $2 $3
            change_own $2 $4
            create_softlink $2 $5
        fi
    else
        echo "Error: $1 is not existed!"
        exit -1
    fi
}

#移除文件
remove_file()
{
    if [ -e $1 ] || [ -L $1 ];then
        rm -r $1
        if [ $? -ne 0 ]; then
            echo "Error: $1 remove failed!"
            exit -1
        fi
    fi
}

#解析filelist.csv脚本,入参为$1: install_type $2:operate_type $3:filelist_path
parse_filelist()
{
    local install_type=$1
    local operate_type=$2
    local filelist_path=$3
    if [ ! -f $filelist_path ]; then
        echo "Error: $filelist_path is not existed!"
        exit -1
    fi

    if [ "$operate_type" = copy ] || [ "$operate_type" = mkdir ]  ; then
        if [ "$install_type" = full ]; then
            file_list=($(awk -F ',' '$2=="'$operate_type'" && ($8~"run"||$8~"docker"||$8~"devel"||$8~"all") {print $3,$4,$6,$7,$9}' $filelist_path))
        else
            file_list=($(awk -F ',' '$2=="'$operate_type'" && ($8~"'$install_type'"||$8~"all") {print $3,$4,$6,$7,$9}' $filelist_path))
        fi
    else
        if [ "$install_type" = full ]; then
            file_list=($(awk -F ',' '($8~"run"||$8~"docker"||$8~"devel"||$8~"all") {print $4,$9}' $filelist_path))
        else
            file_list=($(awk -F ',' '($8~"'$install_type'"||$8~"all") {print $4,$9}' $filelist_path))
        fi
    fi
}

#执行拷贝动作，入参为$1: install_type, $2: install_path $3:filelist_path
do_copy_files()
{
    parse_filelist $1 "copy" $3
    local install_path=$2
    n=0
    while [ $n -lt ${#file_list[@]} ]
    do
        target=${file_list[(($n+1))]}
        softlink=${file_list[(($n+4))]}
        if [[ $target != /* ]];then
            target=$install_path/$target
        fi
        tmpdir=$(dirname $target)
        if [ ! -d $tmpdir ]; then
            mkdir -p $tmpdir
        fi
        if [ $softlink != "NA" ] && [[ $softlink != /* ]];then
            softlink=$install_path/$softlink
        fi
        copy_file ${file_list[n]} $target ${file_list[(($n+2))]} ${file_list[(($n+3))]} $softlink
        n=$(($n+5))
    done
    echo "copy filelist successfully!"
}

#执行创建目录动作，入参为$1: install_type, $2: install_path $3:filelist_path
do_create_dirs()
{
    parse_filelist $1 "mkdir" $3
    local install_path=$2
    n=1
    while [ $n -lt ${#file_list[@]} ]
    do
        target=${file_list[n]}
        if [[ $target != /* ]];then
            target=$install_path/$target
        fi
        create_folder $target ${file_list[(($n+1))]} ${file_list[(($n+2))]} ${file_list[(($n+3))]}
        n=$(($n+5))
    done
    echo "create install folders successfully!"
}

#删除安装文件，入参为$1: install_type, $2: install_path $3:filelist_path
remove_install_files()
{
    parse_filelist $1 "" $3
    local install_path=$2
    n=0
    while [ $n -lt ${#file_list[@]} ]
    do
        target=${file_list[$n]}
        if [ $target != "NA" ];then
            if [[ $target != /* ]];then
                target=$install_path/$target
            fi
            remove_file $target
        fi
        n=$(($n+1))
    done
    echo "remove install files and folders successfully!"
}

#打印安装信息
print_install_content(){
    install_type=$1
    filelist_path=$2
    echo "安装文件信息："
    if [ "$install_type" = full ]; then
        awk -F ',' '$2=="copy" && ( $8~"run"||$8~"docker"||$8~"devel"||$8~"all") {print $4}' $filelist_path
    else
        awk -F ',' '$2=="copy" && ( $8~"'${install_type}'" ||$8~"all") {print $4}' $filelist_path
    fi
    echo "目录创建信息："
    if [ "$install_type" = full ]; then
        awk -F ',' '$2=="mkdir" && ( $8~"run"||$8~"docker"||$8~"devel"||$8~"all") {print $4}' $filelist_path
    else
        awk -F ',' '$2=="mkdir" && ( $8~"'${install_type}'" ||$8~"all") {print $4}' $filelist_path
    fi
    exit 0
}

help_info()
{
    echo "Usage: $0 [param][ --username=<user> --usergroup=<group> ] install_tpye install_path filelist_path"
    echo "param can be one of the following :"
    echo "    --help | -h          : Print out this help message"
    echo "    --copy | -c          : Copy the install content"
    echo "    --mkdir | -m         : Create the install folder "
    echo "    --remove | -r        : Remove the install content"
    echo "    --print | -p         : print the install content and folder info"
    echo "    --username=<user>    : specify user"
    echo "    --usergroup=<group>  : specify group"
    exit 0
}

OPERATE_TYPE=""

while true
do
    case "$1" in
    --copy | -c)
    OPERATE_TYPE="copy"
    shift
    ;;
    --mkdir | -m)
    OPERATE_TYPE="mkdir"
    shift
    ;;
    --remove | -r)
    OPERATE_TYPE="remove"
    shift
    ;;
    --print | -p)
    OPERATE_TYPE="print"
    shift
    ;;
    --username=*)
    username=`echo $1 | cut -d"=" -f2 `
    shift
    ;;
    --usergroup=*)
    usergroup=`echo $1 | cut -d"=" -f2 `
    shift
    ;;
    -h | --help)
    help_info
    ;;
    -*)
    echo Unrecognized input options : "$1"
    help_info
    ;;
    *)
    break
    ;;
    esac
done

if [ $# -lt 3 ]; then
    echo "It's too few input params: $@"
    exit -1
else
    case $OPERATE_TYPE in
    "copy")
        do_copy_files $1 $2 $3
        ;;
    "remove")
        remove_install_files $1 $2 $3
        ;;
    "mkdir")
        do_create_dirs $1 $2 $3
        ;;
    "print")
        print_install_content $1 $3
        ;;
    esac
fi

exit 0
