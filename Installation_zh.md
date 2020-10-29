## 环境预备

- 所有节点OS版本：ubuntu 18.04

- 所有节点root密码一致且允许SSH访问

- 所有节点配置短域名解析

  /etc/hosts配置如下 （依单master双worker节点为例）   

  ```
  127.0.0.1      localhost
  
  your_master_local_ip   master
  your_master_local_ip   master.sigsus.cn
  
  your_worker_local_ip   worker01
  your_worker_local_ip   worker01.sigsus.cn
  
  your_second_worker_local_ip   worker02
  your_second_worker_local_ip   worker02.sigsus.cn
  ```


- 配置harbor（master节点）

  1. [安装harbor](https://goharbor.io/docs/1.10/install-config/)

  2. 配置harbor

     - 配置证书

       参考此 [配置步骤](https://goharbor.io/docs/2.1.0/install-config/configure-https/)

       **注意：必须依照域名harbor.sigsus.cn生成证书**

     - 更改端口与证书路径

       https端口：8443

       certificate: /opt/harbor/cert/harbor.sigsus.cn.crt

       private_key: /opt/harbor/cert/harbor.sigsus.cn.key

  3. 启动harbor

     cd /opt/harbor/

     docker-compose up -d

  

## 编译组件

### 组件信息

| 名称                    | REPO地址                                              | 分支            |
| ----------------------- | ----------------------------------------------------- | --------------- |
| DLTS Main Project       | https://github.com/apulis/apulis_platform.git         | V1.0.0          |
| AIArts-Frontend         | https://github.com/apulis/AIArts-Frontend.git         | V1.0.0          |
| AIArts-Backend          | https://github.com/apulis/AIArts-Backend.git          | V1.0.0          |
| user-dashboard-frontend | https://github.com/apulis/user-dashboard-frontend.git | V1.0.0          |
| user-dashboard-backend  | https://github.com/apulis/user-dashboard-backend.git  | V1.0.0          |
| image-label-frontend    | https://github.com/apulis/NewObjectLabel              | master          |
| image-label             | https://github.com/apulis/data-platform-backend.git   | dev             |
| ascend-for-volcano      | https://github.com/apulis/ascend-for-volcano          | huaweidls-0.4.0 |
| ascend-device-plugin    | https://github.com/apulis/ascend-device-plugin        | v1.0.0          |
| kfserving               | https://github.com/apulis/kfserving.git               | 0.2.2           |

### 编译组件

#### 1. 配置编译环境

- 创建虚拟环境

  ```shell
  virtualenv -p python2.7 pythonenv2.7
  . pythonenv2.7/bin/activate
  ```

- 安装python安装包

  ```
  cd DLWorkspace/src/ClusterBootstrap/
  pip install -r scripts/requirements.txt
  ```

- 安装golang（可选；Atlas特定组件编译；仅当集群存在NPU计算设备时使用）

  参考：https://golang.org/doc/install

  

#### 2. 编译DLTS Main PROJECT组件

​     **编译restfulapi2**

```
cd DLWorkspace/src/ClusterBootstrap/
./deploy.py docker build restfulapi2
```

​    **编译init-container** 

```
./deploy.py docker push init-container
```

​	**编译job-exporter**

```
./deploy.py docker push job-exporter
```

​    **编译gpu-reporter** 

```
./deploy.py docker push gpu-reporter
```

​     **编译watchdog**

```
./deploy.py docker push watchdog
```

​    **编译repairmanager2**

```
./deploy.py docker push repairmanager2
```

#### 3.  编译AIArts-Frontend

```shell
cd AIArts-Frontend/
docker build -t dlworkspace_aiarts-frontend:1.0.0 .
```

#### 4.  编译AIArts-Backend

```shell
cd AIArtsBackend/deployment/
bash build.sh
```

#### 5.  编译user-dashboard-frontend
```shell
cd user-dashboard-frontend/
docker build -t dlworkspace_custom-user-dashboard-frontend:latest .
```

#### 6.  编译user-dashboard-backend
```shell
cd user-dashboard-backend/
docker build -t dlworkspace_custom-user-dashboard-backend:latest .
```
#### 7.  编译image-label-frontend
```shell
cd NewObjectLabel/
docker build -t dlworkspace_image-label:latest .
```
#### 8.  编译image-label-backend
```shell
cd DLWorkspace/src/ClusterBootstrap/
./deploy.py docker push data-platform-backend
```
#### 9.  编译ascend-for-volcano

-  创建目录

   ```mkdir -p ${GOPATH}/{src/github.com/google,src/k8s.io,src/volcano.sh}```

- 将软件包中获取的ascend-for-volcano文件夹上传到“${GOPATH}/src/volcano.sh/“目录下，并将文件夹重命名为volcano

- 创建build文件夹

  ```shell
  cd ${GOPATH}/src/volcano.sh/volcano/
  mkdir -p build
  ```

- 创建并编辑build.sh

  执行```cd ${GOPATH}/src/volcano.sh/volcano/build```

  执行```vim build.sh```, 输入：

  ```shell
  #!/bin/sh
  
  cd ${GOPATH}/src/volcano.sh/volcano/
  
  make clean
  export PATH=$GOPATH/bin:$PATH
  export GO111MODULE=off
  export GOMOD=""
  export GIT_SSL_NO_VERIFY=1
  
  make image_bins
  make images
  make generate-yaml
  mkdir _output/DockFile/
  
  docker save -o _output/DockFile/vc-webhook-manager-base.tar.gz volcanosh/vc-webhook-manager-base
  docker save -o _output/DockFile/vc-webhook-manager.tar.gz volcanosh/vc-webhook-manager
  docker save -o _output/DockFile/vc-controller-manager.tar.gz volcanosh/vc-controller-manager
  docker save -o _output/DockFile/vc-vc-scheduler.tar.gz volcanosh/vc-scheduler
  ```

- 编译镜像

  ```shell
  chmod +x build.sh
  ./build.sh
  ```

- 查看镜像

  ```
  docker images | grep volcanosh
  ```

  

#### 10.  编译ascend-device-plugin

​         以下操作位于Atlas服务器

- 登录atlas服务器，安装golang

- 配置golang编译环境

  执行：```vim ~/.bashrc```, 输入以下内容并保存：

  ```
  export GO111MODULE=on
  export GOPROXY=https://gocenter.io
  export GONOSUMDB=*
  ```

  

- 将ascend-device-plugin文件夹上传到任意目录（如“/home”）

- 在ascend-device-plugin目录下创建prepare_build.sh文件

  **cd /home/ascend-device-plugin/build**

  vim prepare_build.sh

  根据实际写入以下内容：

  ```shell
  #!/bin/bash
  ASCNED_TYPE=910 #根据芯片类型选择310或910。
  ASCNED_INSTALL_PATH=/usr/local/Ascend  #驱动安装路径，根据实际修改。
  USE_ASCEND_DOCKER=false  #是否使用昇腾Docker，请修改为false。
  
  CUR_DIR=$(dirname $(readlink -f $0))
  TOP_DIR=$(realpath ${CUR_DIR}/..)
  
  LD_LIBRARY_PATH_PARA1=${ASCNED_INSTALL_PATH}/driver/lib64/driver
  LD_LIBRARY_PATH_PARA2=${ASCNED_INSTALL_PATH}/driver/lib64
  apt-get install -y pkg-config
  apt-get install -y dos2unix
  TYPE=Ascend910
  PKG_PATH=${TOP_DIR}/src/plugin/config/config_910
  PKG_PATH_STRING=\$\{TOP_DIR\}/src/plugin/config/config_910
  LIBDRIVER="driver/lib64/driver"
  
  if [ ${ASCNED_TYPE} == "310"  ]; then
    TYPE=Ascend310
    LD_LIBRARY_PATH_PARA1=${ASCNED_INSTALL_PATH}/driver/lib64
    PKG_PATH=${TOP_DIR}/src/plugin/config/config_310
    PKG_PATH_STRING=\\$\\{TOP_DIR\\}/src/plugin/config/config_310
    LIBDRIVER="/driver/lib64"
  fi
  sed -i "s/Ascend[0-9]\\{3\\}/${TYPE}/g" ${TOP_DIR}/ascendplugin.yaml
  sed -i "s#ath: /usr/local/Ascend/driver#ath: ${ASCNED_INSTALL_PATH}/driver#g" ${TOP_DIR}/ascendplugin.yaml
  sed -i "/^ENV LD_LIBRARY_PATH /c ENV LD_LIBRARY_PATH ${LD_LIBRARY_PATH_PARA1}:${LD_LIBRARY_PATH_PARA2}/common" ${TOP_DIR}/Dockerfile
  sed -i "/^ENV USE_ASCEND_DOCKER /c ENV USE_ASCEND_DOCKER ${USE_ASCEND_DOCKER}" ${TOP_DIR}/Dockerfile
  sed -i "/^libdriver=/c libdriver=$\\{prefix\\}/${LIBDRIVER}" ${PKG_PATH}/ascend_device_plugin.pc
  sed -i "/^prefix=/c prefix=${ASCNED_INSTALL_PATH}" ${PKG_PATH}/ascend_device_plugin.pc
  sed -i "/^CONFIGDIR=/c CONFIGDIR=${PKG_PATH_STRING}" ${CUR_DIR}/build_in_docker.sh
  ```

- 编译镜像

  ```
  chmod +x prepare_build.sh
  ./prepare_build.sh
  
  chmod +x build_910.sh
  dos2unix build_910.sh  
  
  ./build_910.sh dockerimages    
  ```

- 检查镜像

  ```
  docker images | grep deviceplugin
  ```

  

#### 11.  编译kfserving
```shell script
IMAGE_PUSH_HUB_URL=harbor.sigsus.cn/sz_gongdianju/apulistech
或者
IMAGE_PUSH_HUB_URL=apulistech
./scripts/kfserving.sh push istio
./scripts/kfserving.sh push knative
./scripts/kfserving.sh push kfserving
```


## 执行部署

### 1. 设定配置文件

 **cd DLWorkspace/src/ClusterBootstrap/**

**vim config.yaml**

```
cluster_name: atlas

network:
  domain: sigsus.cn
  container-network-iprange: "10.0.0.0/8"

UserGroups:
  DLWSAdmins:
    Allowed:
    - jinlmsft@hotmail.com
    gid: "20001"
    uid: "20000"
  DLWSRegister:
    Allowed:
    - '@gmail.com'
    - '@live.com'
    - '@outlook.com'
    - '@hotmail.com'
    - '@apulis.com'
    gid: "20001"
    uid: 20001-29999

WebUIadminGroups:
- DLWSAdmins

WebUIauthorizedGroups:
- DLWSAdmins

WebUIregisterGroups:
- DLWSRegister

datasource: MySQL
mysql_password: apulis#2019#wednesday
webuiport: 3081
useclusterfile : true

machines:
  atlas02:
    role: infrastructure
    private-ip: 192.168.3.2
    archtype: arm64
    type: npu
    vendor: huawei

scale_up:
  atlas01:
    archtype: arm64
    role: worker
    type: npu 
    vendor: huawei
    os: ubuntu

  atlas-gpu01:
    archtype: amd64
    role: worker
    type: gpu 
    vendor: nvidia
    os: ubuntu

  atlas-gpu02:
    archtype: amd64
    role: worker
    type: gpu 
    vendor: nvidia
    os: ubuntu

# settings for docker
dockerregistry: apulistech/
dockers:
  hub: apulistech/
  tag: "1.9"
 
dataFolderAccessPoint: ''

Authentications:
  Microsoft:
    TenantId: 
    ClientId: 
    ClientSecret: 

  Wechat:
    AppId: 
    AppSecret: 

mountpoints:
  nfsshare1:
    type: nfs
    server: atlas02
    filesharename: /mnt/local
    curphysicalmountpoint: /mntdlws
    mountpoints: ""

repair-manager:
  cluster_name: "atlas"
  ecc_rule:
    cordon_dry_run: True
  alert:
    smtp_url: smtp.qq.com
    login: 
    password: 
    sender: 
    receiver: ["XXX@XXX.com"]

enable_custom_registry_secrets: True
platform_name: Apulis Platform
kube-vip: XXX.XXX.XXX.XXX
```

### 2. 执行部署

-  **切换目录：cd DLWorkspace/src/ClusterBootstrap/**

- **安装部署节点环境**

  ```
  ./scripts/prepare_ubuntu_dev.sh
  ```

- **安装集群节点环境**

  ```shell
  ./deploy.py --verbose sshkey install
  ./deploy.py --verbose runscriptonall ./scripts/prepare_ubuntu.sh
  ./deploy.py --verbose runscriptonall ./scripts/prepare_ubuntu.sh continue
  ./deploy.py --verbose execonall sudo usermod -aG docker dlwsadmin
  ```

- **安装K8S集群**

  ```
  ./deploy.py --verbose execonall sudo swapoff -a
  ./deploy.py runscriptonroles infra worker ./scripts/install_kubeadm.sh
  
  ./deploy.py --verbose kubeadm init
  ./deploy.py --verbose copytoall ./deploy/sshkey/admin.conf /root/.kube/config
  
  ./deploy.py --verbose kubeadm join
  ./deploy.py --verbose -y kubernetes labelservice
  ./deploy.py --verbose -y labelworker
  ```

- **渲染集群配置**

  ```
  ./deploy.py renderservice
  ./deploy.py renderimage
  ./deploy.py webui
  ./deploy.py nginx webui3
  
  ./deploy.py nginx fqdn
  ./deploy.py nginx config
  ```

- **挂载共享存储**

  ```
  ./deploy.py runscriptonroles infra worker ./scripts/install_nfs.sh
  ./deploy.py --force mount
  ./deploy.py execonall "df -h"    
  ```

- **启动集群服务器**

  ```shell
  ./deploy.py kubernetes start nvidia-device-plugin
  ./deploy.py kubernetes start  a910-device-plugin
  
  ./deploy.py kubernetes start mysql
  ./deploy.py kubernetes start jobmanager2 restfulapi2 nginx custommetrics repairmanager2 openresty
  ./deploy.py --sudo --background runscriptonall scripts/npu/npu_info_gen.py
  ./deploy.py kubernetes start monitor
  
  ./deploy.py kubernetes start istio
  ./deploy.py kubernetes start knative kfserving
  ./deploy.py kubernetes start webui3 custom-user-dashboard image-label aiarts-frontend aiarts-backend data-platform
  ```