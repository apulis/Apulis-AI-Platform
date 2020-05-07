# DLWS集群安装步骤

### 配置说明 & 示例
| 名称       | 配置    | GPU  | 操作系统     | 公网IP       | 子网IP       | 描述                        |
| ---------- | ------- | ---- | ------------ | ------------ | ------------ | --------------------------- |
| dev master | 6C64G   | N/A  | ubuntu 18.04 | 121.46.18.83 | 192.168.1.3  | 部署发起节点 k8s master节点 |
| worker-01  | 36C512G | 8    | ubuntu 18.04 | 121.46.18.83 | 192.168.1.8  | k8s worker节点              |
| worker-02  | 36C512G | 8    | ubuntu 18.04 | 121.46.18.83 | 192.168.1.9  | k8s worker节点              |
| freenas    | 16C187G | N/A  | freenas      | 121.46.18.83 | 192.168.1.10 | k8s worker节点              |

其中：

1. master和worker需在同一个子网或VPC，dev与master、worker可以不在同一个子网或VPC
2. worker节点需携带GPU，GPU类别为NVidia，安装驱动（driver）版本>= 430  


### 安装准备
#### DEV节点创建非ROOT用户

**（可选；如果有root用户，或者sudo权限用户，可略过此步骤）**

**./create_user.sh  用户名  密码**

```
#! /bin/bash

#设置变量name接收第一个参数（要创建的用户名），$n表示第n个参数，且=两边不能有空格
name=$1
#设置变量pass接收第二个参数（要为其设置的密码）
pass=$2

#echo语句会输出到控制台，${变量}或者 $变量 表示变量代表的字符串
echo "you are setting username : ${name}"
echo "you are setting password : $pass for ${name}"

#添加用户$name，此处sudo需要设置为无密码，后面将会作出说明
sudo useradd $name

#如果上一个命令正常运行，则输出成功，否则提示失败并以非正常状态退出程序
# $?表示上一个命令的执行状态，-eq表示等于，[ 也是一个命令
# if fi 是成对使用的，后面是前面的倒置，很多这样的用法。
if [ $? -eq 0 ];then
   echo "user ${name} is created success!"
else
   echo "user ${name} is created failed!!!"
   exit 1
fi
#sudo passwd $name会要求填入密码，下面将$pass作为密码传入
#echo $pass | sudo passwd $name --stdin  &> /dev/null

#ubuntu系统不支持passwd的stdin参数，所以要使用chpasswd命令
sudo echo "$name:$pass" | chpasswd
if [ $? -eq 0 ];then
   echo "${name}'s password is set success!"
else
   echo "${name}'s password is set failed!!!"
fi

if [ -d /home/$name ]
then
   echo "/home/$name is already exist!"
   exit 1
else
    sudo mkdir /home/$name
    sudo chown -R $name /home/$name
fi

#添加用户组
sudo usermod -aG $name,sudo,docker $name
if [ $? -eq 0 ];then
   echo "${name}'s group is set success!"
else
   echo "${name}'s group is set failed!!!"
fi

#设置shell
sudo usermod -s /bin/bash $name
if [ $? -eq 0 ];then
   echo "${name}'s shell is set success!"
else
   echo "${name}'s shell is set failed!!!"
fi

#设置shell配置文件
sudo cp .bashrc /home/$name/
if [ $? -eq 0 ];then
   echo "${name}'s bashrc is set success!"
else
   echo "${name}'s bashrc is set failed!!!"
fi
```



#### 免密码配置

**针对非root用户，需在dev、master、worker三个机器中配置**

- 执行sudo visudo

- 检查文件内容，确认是否存在以下配置，没有则新增

     ```
     %sudo ALL=(ALL:ALL) ALL
     %sudo ALL=(ALL) NOPASSWD:ALL
     ```



#### 配置节点Hostname

china-gpu02-master

china-gpu02-worker1

china-gpu02-worker2



配置方法（依china-gpu02-master为例子）：

- 更新/etc/hostname内容为china-gpu02-master

- 设置hostname立即生效：sudo hostnamectl set-hostname china-gpu02-master

  

#### 配置DNS

**需要在DNS提供商控制台进行配置**

DNS提供商：https://dns.console.aliyun.com

主域名：sigsus.cn

 示例：

| 主机记录 | 记录类型 | 记录值 | 对应节点(参考) |
| ---- | ---- | ---- | ---- |
| china-gpu02 | A | 121.46.18.83 | master |
| china-gpu02-worker1 | A | 121.46.18.83 | woker01 |
| china-gpu02-worker2 | A        | 121.46.18.83 | worker02      |




### 执行安装
#### 文件/路径说明  

安装程序所在目录 ：**DLWorkspace/src/ClusterBootstrap/**  
集群配置文件：**DLWorkspace/src/ClusterBootstrap/config.yaml**   
安装文件：**DLWorkspace/src/ClusterBootstrap/deploy.py**



#### 设置集群配置文件

以下字段做相应修改

- cluster_name —— 集群名称

- cloud_influxdb_node —— master节点FQDN

- DeployAuthentications —— 登录方式（如有微信，需相应增加）

- DLWSAdmins —— 增加对应的管理员名称

- mysql_password —— 修改为指定的密码

- machines 

  - china-gpu02-master —— private-id —— 修改为对应master节点的内网IP
  - type和vendor —— 更改为对应的硬件和厂家名

- dockerregistry —— 更改为对应的docker hub

- onpremise_cluster

  - worker_node_num —— 更改为worker节点数
  - gpu_count_per_node —— 更改为节点gpu数量

- mountpoints

  - server —— 更改为数据节点的短域名
  - filesharename —— 挂载点目录名（可不更改）

- nfs_disk_mnt

  - 更改为master节点短域名

  

config.yaml样例

```
cluster_name: china-gpu02

network:
  domain: sigsus.cn
  container-network-iprange: "10.0.0.0/8"

etcd_node_num: 1
mounthomefolder : True

# These may be needed for KubeGPU
# # kube_custom_cri : True
# # kube_custom_scheduler: True
kubepresleep: 1
cloud_influxdb_node: china-gpu02.sigsus.cn

DeployAuthentications:
- Microsoft

UserGroups:
  DLWSAdmins:
    Allowed:
    - jinlmsft@hotmail.com
    - jin.li@apulis.com
    - bifeng.peng@apulis.com
    - hui.yuan@apulis.com
    gid: "20001"
    uid: 20000-20004
  DLWSRegister:
    Allowed:
    - '@apulis.com'
    - '@outlook.com'
    - '@hotmail.com'
    gid: "20001"
    uid: 20005-29999

WebUIadminGroups:
- DLWSAdmins

WebUIauthorizedGroups:
- DLWSAdmins

WebUIregisterGroups:
- DLWSRegister

WinbindServers: []

datasource: MySQL
mysql_password: corona@2020@wuhan
webuiport: 3081
useclusterfile : true
platform-scripts : ubuntu


machines:
  china-gpu02:
    role: infrastructure
    private-ip: 192.168.1.3
  china-gpu02-worker1:
    role: worker
    type: gpu
    vendor: nvidia
  china-gpu02-worker2:
    role: worker
    type: gpu
    vendor: nvidia

admin_username: dlwsadmin

# settings for docker
dockerregistry: apulistech/
dockers:
  hub: apulistech/

cloud_config:
  dev_network:
    source_addresses_prefixes: [ "66.114.136.16/29", "73.140.21.119/32"]
  default_admin_username: dlwsadmin
  dev_network:
    source_addresses_prefixes:
    - 66.114.136.16/29
    - 73.140.21.119/32
    tcp_port_ranges: 22 1443 2379 3306 5000 8086 10250 10255 22222
  inter_connect:
    tcp_port_ranges: 22 1443 2379 3306 5000 8086 10250
  nfs_share:
    source_ips:
    - 116.66.187.0/24
  nfs_ssh:
    port: 22
  tcp_port_for_pods: 30000-49999
  tcp_port_ranges: 80 443 30000-49999 25826 3000 22222 9091 9092
  udp_port_ranges: '25826'
  vnet_range: 116.66.187.0/24

cloud_elasticsearch_node: china-gpu02.sigsus.cn
cloud_elasticsearch_port: '9200'

cloud_influxdb_port: '8086'
cloud_influxdb_tp_port: '25826'

custom_mounts: []
data-disk: /dev/[sh]d[^a]
dataFolderAccessPoint: ''


defalt_virtual_cluster_name: platform
default-storage-folders:
- jobfiles
- storage
- work
- namenodeshare

deploymounts: []

discoverserver: 4.2.2.1
dltsdata-storage-mount-path: /dltsdata
dns_server:
  azure_cluster: 8.8.8.8
  onpremise: 8.8.8.8

Authentications:
  Microsoft:
    TenantId: 19441c6a-f224-41c8-ac36-82464c2d9b13
    ClientId: 487f34da-74af-4c0d-85d9-d678a118d99d
    ClientSecret: "1MZ[7?g0vPv_6cahAvPuohwuQKrrJEh."

  DingTalk:
    AppId: dingoap3bz8cizte9xu62e
    AppSecret: sipRMeNixpgWQOw-sI6TFS5vdvtXozY3y75ik_Zue2KGywfSBBwV7er_8yp-7vaj

  Wechat:
    AppId: wx403e175ad2bf1d2d
    AppSecret: dc8cb2946b1d8fe6256d49d63cd776d0

supported_platform:  ["onpremise"]
onpremise_cluster:
  worker_node_num:    2
  gpu_count_per_node: 8
  gpu_type:           nvidia


mountpoints:
  nfsshare1:
    type: nfs
    server: storage-server
    filesharename: /mypool
    curphysicalmountpoint: /mntdlws
    mountpoints: ""

nfs_disk_mnt:
  storage-server:
    fileshares:
    - mypool
    path: /
    role: nfs

nfs_client_CIDR:
  node_range:
  - 116.66.187.0/24

basic_auth: d368c36acb3343b4,admin,1000
deploydockerETCD: false

k8sAPIport: 6443
k8s-gitbranch: v1.18.0
deploy_method: kubeadm

repair-manager:
  ecc_rule:
    cordon_dry_run: True
  alert:
    smtp_url: smtp.office365.com
    login: dev@apulis.com
    smtp_url: smtp.office365.com
    login: dev@apulis.com
    password: Yitong#123
    sender: dev@apulis.com
    receiver: ["181330729@qq.com"]
    
jwt:
  secret_key: "Sign key for JWT"
  algorithm: HS256
  token_ttl: 86400
```



#### 安装**DEV**执行环境

```
./scripts/prepare_ubuntu_dev.sh
```

  

#### 配置DNS内网解析

**以下机器：dev、master、worker三个机器**  

- 创建目录：mkdir -p /etc/resolvconf/resolv.conf.d/

  修改文件：vim /etc/resolvconf/resolv.conf.d/base  

  增加数据：search sigsus.cn  
  执行指令：sudo resolvconf -u

  

- 配置节点hosts文件（**单个公网IP的集群使用，router不具备短域名或局域网内DNS配置功能**）

  执行：mkdir -p deploy/etc

  路径：DLWorkspace/src/ClusterBootstrap/deploy/etc/hosts

  作用：实现内网DNS解析

  ```
  127.0.0.1       localhost
  127.0.1.1       ubuntu
  
  192.168.1.3     china-gpu02
  192.168.1.8     china-gpu02-worker1
  192.168.1.9     china-gpu02-worker2
  
  
  10.200.0.1      china-gpu02-master-ib
  10.200.0.2      china-gpu02-worker1-ib
  10.200.0.3      china-gpu02-worker2-ib
  10.200.0.4      storage-server
  
  192.168.1.3     china-gpu02.sigsus.cn
  192.168.1.8     china-gpu02-worker1.sigsus.cn
  192.168.1.9     china-gpu02-worker2.sigsus.cn
  
  
  10.200.0.1      china-gpu02-master-ib.sigsus.cn
  10.200.0.2      china-gpu02-worker1-ib.sigsus.cn
  10.200.0.3      china-gpu02-worker2-ib.sigsus.cn
  10.200.0.4      storage-server.sigsus.cn
  
  # The following lines are desirable for IPv6 capable hosts
  ::1     localhost ip6-localhost ip6-loopback
  fe00::0 ip6-localnet
  ff00::0 ip6-mcastprefix
  ff02::1 ip6-allnodes
  ff02::2 ip6-allrouters
  ```
  
- 执行hosts文件配置（deploy.py位于**DLWorkspace/src/ClusterBootstrap/**）

  ```
  sudo cp ./deploy/etc/hosts  /etc/hosts
  ./deploy.py --verbose copytoall ./deploy/etc/hosts  /etc/hosts
  ```

- 检查DNS配置

  在master、proxy上指令执行看是否成功 

  ``` 
    ping china-gpu02
    ping china-gpu02-worker1
    ping china-gpu02-worker2
  ```

  

#### 初始化部署环境

```
./deploy.py --verbose -y build 
```



#### 配置集群节点ROOT用户密码

将集群节点ROOT密码设置一致，然后执行指令：

```
cd deploy/sshkey
echo "root" > "rootuser"
echo "your_root_password" > "rootpasswd"
```



#### 安装SSH Key到所有集群节点

```
 ./deploy.py --verbose sshkey install
```



#### 检查集群节点是否可正常访问

```
./deploy.py --verbose execonall sudo ls -al
```



#### 设置集群节点的安装环境

```
./deploy.py --verbose runscriptonall ./scripts/prepare_ubuntu.sh

上一个语句会重启worker节点。需等待所有服务器 启动完毕，再执行以下步骤！！
./deploy.py --verbose runscriptonall ./scripts/prepare_ubuntu.sh continue
./deploy.py --verbose execonall sudo usermod -aG docker dlwsadmin
```


其中：

- dlwsadmin为操作集群机器所采用的用户名，配置于config.yaml
- 如果nvidia驱动安装不成功，可能与具体的设备配置有关，譬如secure boot问题等等，请联系开发人员定位和完善
- 

#### Worker机器状态确认

```
1、指令确认
   nvidia-docker run --rm dlws/cuda nvidia-smi
   docker run --rm -ti dlws/cuda nvidia-smi
   保证以上两条指令均能够正常输出，才表明nvidia驱动与nvidia-docker均已正常安装

2、问题定位
   如docker指令执行正常，但nvidia-docker指令执行错误，则修改/etc/docker/daemon.json，
   将nvidia-docker设置为default runtime

   vim /etc/docker/daemon.json
   增加
   "default-runtime": "nvidia",

    完整例子：
    {
        "default-runtime": "nvidia",
        "runtimes": {
            "nvidia": {
                "path": "nvidia-container-runtime",
                "runtimeArgs": []
            }
        }
    }
    
    然后重启docker：
    systemctl daemon-reload
    systemctl restart docker
```



#### 安装kubernetes kubeadm客户端

```
./deploy.py runscriptonroles infra worker ./scripts/install_kubeadm.sh
```



#### 安装K8S集群平台

##### 各节点关闭swap

```
./deploy.py --verbose execonall sudo swapoff -a
```

##### master节点关闭swap

```
sudo vim /etc/fstab
注释掉带swap字段行：
#UUID=xxxxxxxx-xxxx-xxxxx-xxx-xxxxxxxxxxx none            swap  sw
```

##### 安装集群基础软件

```
./deploy.py --verbose execonall docker pull dlws/pause-amd64:3.0
./deploy.py --verbose execonall docker tag  dlws/pause-amd64:3.0 gcr.io/google_containers/pause-amd64:3.0

./deploy.py --verbose kubeadm init
./deploy.py --verbose copytoall ./deploy/sshkey/admin.conf /root/.kube/config
```

##### 设置集群节点标签

```
./deploy.py --verbose kubeadm join
./deploy.py --verbose -y kubernetes labelservice
./deploy.py --verbose -y labelworker
```



#### 挂载数据共享文件夹

##### 安装NFS服务（所有节点，包括存储点）

```
apt-get update
apt-get install nfs-kernel-server nfs-common portmap
sudo ln -s /etc/init.d/nfs-kernel-server /etc/init.d/nfs

/etc/init.d/nfs-kernel-server restart
```



##### 配置挂载目录（存储节点）

- 创建目录。假设所要挂载的目录是：/data/nfsshare

  mkdir -p /data/nfsshare 

-  设置白名单

  编辑文件 /etc/exports，并增加挂载目录的IP白名单

   > /data/nfsshare           192.168.1.0/24(rw,fsid=0,insecure,no_subtree_check,async,no_root_squash)



##### 更新共享信息

```
exportfs -a
```

##### 执行挂载

```
./deploy.py --verbose mount
```



#### 部署NVidia插件

```
./deploy.py --verbose kubernetes start nvidia-device-plugin
```

​    

#### 重置API Server服务器的NodePort端口段

编辑文件/etc/kubernetes/manifests/kube-apiserver.yaml

增加指定参数：

```
- --service-node-port-range=30000-49999
```

保存文件后，API-SERVER自动重启并生效



#### 设置Dashboard服务配置文件

文件：**DLWorkspace/src/dashboard/config/local.yaml**

domain、casUrl以及clusters，**三者需依据集群实际的域名，做相应的修改**



内容样例：

```
sign: "Sign key for JWT"
winbind: "Will call /domaininfo/GetUserId with userName query to get the user's id info"
masterToken: "Access token of all users"

authEnabled:
  wechat: 1
  microsoft: 1

activeDirectory:
  tenant: "19441c6a-f224-41c8-ac36-82464c2d9b13"
  clientId: "487f34da-74af-4c0d-85d9-d678a118d99d"
  clientSecret: "1MZ[7?g0vPv_6cahAvPuohwuQKrrJEh."

dingtalk:
  appId: "dingoap3bz8cizte9xu62e"
  appSecret: "sipRMeNixpgWQOw-sI6TFS5vdvtXozY3y75ik_Zue2KGywfSBBwV7er_8yp-7vaj"

wechat:
  appId: "wx403e175ad2bf1d2d"
  appSecret: "dc8cb2946b1d8fe6256d49d63cd776d0"

domain: "https://china-gpu02.sigsus.cn"
casUrl: "http://china-gpu02.sigsus.cn/cas"

administrators:
  - jinlmsft@hotmail.com
  - jin.li@apulis.com
  - bifeng.peng@apulis.com
  - hui.yuan@apulis.com

clusters:
  china-gpu02:
      restfulapi: "http://china-gpu02.sigsus.cn/apis"
      title: Grafana-endpoint-of-the-cluster
      workStorage: work
      dataStorage: data
      grafana: "https://china-gpu02.sigsus.cn/grafana/"
      prometheus: http://china-gpu02.sigsus.cn:9091
```



#### 部署集群应用

##### 登录docker hub （用户名为config.yaml所配置）

> docker login

##### 生成dashboard, jobmanager等服务的配置文件

```
./deploy.py --verbose webui         
```

##### 编译restfulapi和webui3服务

```
./deploy.py --verbose docker push restfulapi2
./deploy.py --verbose docker push webui3
```

##### 编译GPU Reporter

```
./deploy.py --verbose docker push gpu-reporter
```

##### 编译Job容器的依赖容器（请参考DLWorkspace/src/ClusterBootstrap/step_by_step.sh）：

```
./deploy.py --verbose docker push init-container
```

##### 配置Nginx

```
./deploy.py --verbose nginx fqdn
./deploy.py --verbose nginx config
```

##### 启动集群应用

```
./deploy.py --verbose kubernetes start mysql jobmanager restfulapi2 monitor nginx custommetrics
./deploy.py --verbose kubernetes start cloudmonitor
```

##### 启动dashboard

```
./deploy.py --verbose nginx webui3
./deploy.py --verbose kubernetes start webui3
```

