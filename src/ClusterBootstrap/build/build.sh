#!/bin/bash

set -x

# 镜像名和代码分支配置
image_name=restfulapi2
tag_name=rc5
branch_name=v1.2.0
image_fullname=${image_name}:${branch_name}-${tag_name}

# 推送到harbor
harbor_path=harbor.sigsus.cn:8443/library/apulistech/
harbor_fullname=${harbor_path}${image_fullname}

# 目标集群上harbor仓库中镜像全名
local_harbor_fullname=harbor.sigsus.cn:8443/sz_gongdianju/apulistech/dlworkspace_restfulapi2:latest

# 拷贝到其它机器
# (ip port)
remote_machine=(219.133.167.42 52080)
#remote_machine=(192.168.1.176 22)
tar_name=`sed 's|:|_|g' <<< ${image_fullname}`.tar


########################################################
function prebuild(){

    # 1. 创建虚拟环境
    if [ ! -d "pythonenv2.7" ]; then
        virtualenv -p python2.7 pythonenv2.7
        . pythonenv2.7/bin/activate

        pip install -r ../scripts/requirements.txt
        pip install -r ../../ClusterManager/requirements.txt
    else
        . pythonenv2.7/bin/activate
    fi


    # 2. 准备源代码
    git pull origin $branch_name

    rm -rf restfulapi2
    cp -r ../../docker-images/restfulapi2 .

    # 3. 渲染配置文件
    cd ..
    ./deploy.py rendertemplate ../docker-images/restfulapi2/ports.conf build/restfulapi2/ports.conf
    ./deploy.py rendertemplate ../docker-images/restfulapi2/000-default.conf build/restfulapi2/000-default.conf
    cd build/restfulapi2

    #rm -rf Jobs_Templete
    #rm -rf utils
    #rm -rf RestAPI
    #rm -rf ClusterManager

    cp -r ../../../Jobs_Templete Jobs_Templete
    cp -r ../../../utils utils
    cp -r ../../../RestAPI RestAPI
    cp -r ../../../ClusterManager ClusterManager
    cp ../../../../version-info version-info

    # 下载kubectl，jobmanger会用到？
    if [ ! -e "../kubectl" ]; then
        curl -LO "https://storage.googleapis.com/kubernetes-release/release/$(curl -s https://storage.googleapis.com/kubernetes-release/release/stable.txt)/bin/linux/amd64/kubectl"
        chmod +x ./kubectl
        cp kubectl ../
    else
        cp ../kubectl ./
        chmod +x ./kubectl
    fi
}

function build(){

    docker build . -t ${image_fullname} -f Dockerfile
    docker tag ${image_fullname} ${harbor_fullname}
}

function postbuild() {

    docker push ${harbor_fullname}
    docker save ${harbor_fullname} > ${tar_name}


    ## 拷贝到远程临时目录
    scp -P ${remote_machine[1]}  ${tar_name} root@${remote_machine[0]}:/tmp

    ## 导入镜像包并重新打印标签、push 本地harbor等等
    cmd="cd /tmp; docker load < ${tar_name};
             docker tag ${harbor_fullname} ${local_harbor_fullname};
             docker push ${local_harbor_fullname};
             cd /home/dlwsadmin/DLWorkspace/YTung/src/ClusterBootstrap;
             ./deploy.py kubernetes stop ${image_name} jobmanager2;
             ./deploy.py kubernetes start jobmanager2;
             ./deploy.py kubernetes start ${image_name};
    "
    echo $cmd
    ssh -p ${remote_machine[1]} root@${remote_machine[0]} $cmd
}


function main(){

    prebuild
    build
    postbuild
}

main $*


