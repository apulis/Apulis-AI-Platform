## 环境预备

- 节点OS版本：ubuntu 18.04

- 节点root密码一致且允许SSH访问

- 节点配置短域名解析

  /etc/hosts例子    

  ```
  127.0.0.1      localhost
  
  your_master_local_ip   master
  your_master_local_ip   master.sigsus.cn
  
  your_worker_local_ip   worker01
  your_worker_local_ip   worker01.sigsus.cn
  
  your_second_worker_local_ip   worker02
  your_second_worker_local_ip   worker02.sigsus.cn
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