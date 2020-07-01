# DLWS集群安装步骤


### 1. 配置说明 & 示例
| 主机名      | 配置    | 计算设备 | 操作系统       | 公网IP       | 子网IP      | 描述                           |
| ----------- | ------- | -------- | -------------- | ------------ | ----------- | ------------------------------ |
| atlas02     | 6C64G   | 8 NPU    | ubuntu 18.04.1 | 121.46.18.84 | 192.168.3.6 | NPU Woker节点                  |
| atlas01     | 36C512G | 8 NPU    | ubuntu 18.04.1 | 121.46.18.84 | 192.168.3.2 | k8s master节点/NPU worker 节点 |
| atlas-gpu01 | 6C64G   | 8 GPU    | ubuntu 18.04.4 | 121.46.18.84 | 192.168.3.4 | GPU Woker节点                  |
| atlas-gpu02 | 6C64G   | 8 GPU    | ubuntu 18.04.4 | 121.46.18.84 | 192.168.3.3 | GPU Woker节点                  |

其中：

1. master和worker需在同一个子网或VPC，dev与master、worker可以不在同一个子网或VPC
2. worker节点计算设备配置
   - 若设备为NVIDIA GPU，则驱动版本不低于430
   - 若设备为Huawei NPU, 则驱动版本不低于 1.72.T2.100.B020
****

### 2. 安装准备

#### 2.1 安装要求

- **操作系统**

  普通节点：ubuntu 18.04 server

  NPU节点：ubuntu 18.04.1 server （小版本号必须为1）

  

- **用户配置**

  root用户，或具备sudo权限的非ROOT用户

  **所有机器允许root登录，且root密码一致**

  

- **硬件配置**

  - worker节点：需禁用SecureBoot（如不禁用，将导致GPU驱动无法升级）

  - 所有节点：配置并允许SSH连接、关闭防火墙

  - GPU节点：预装好驱动，NVIDIA GPU请参考[安装手册](https://docs.nvidia.com/datacenter/tesla/tesla-installation-notes/index.html#ubuntu-lts)

  - NPU节点：预装好驱动，华为NPU请参考 A910驱动安装手册

    

- **步骤说明**：执行时如提示无权限，则使用sudo权限执行

- **编译节点：CPU架构需与master节点架构一致，譬如同为AMD64或同为Arm64**
  
    
  
#### 2.2 配置节点Hostname

​	将各个节点的主机名，按照**配置说明**配置   
​	

​     配置方法（依atlas02为例子）：

- 编辑/etc/hostname，更新内容为atlas02
- 设置hostname立即生效：sudo hostnamectl set-hostname atlas02

  

#### 2.3 配置域名DNS（如不提供公网访问，请跳过此章节）

​     **需要在DNS提供商控制台进行配置**

​	 以阿里云为例：https://dns.console.aliyun.com

​	 主域名：**sigsus.cn**

 	配置示例：

| 主机记录 | 记录类型 | 记录值 |
| ---- | ---- | ---- |
| atlas.sigsus.cn | A | 121.46.18.84 |

#### 2.4 配置Worker节点DNS

- **配置短域名搜索**（所有机器）

  - 安装resolvconf

    `apt update`

    `apt install resolvconf`

  - 增加短域名   

  ​        `mkdir -p /etc/resolvconf/resolv.conf.d/`   

  ​        `echo "search sigsus.cn" > /etc/resolvconf/resolv.conf.d/base`     

  ​		`sudo resolvconf -u`    

  

  ​        此处**sigsus.cn**即为config.yaml中的domain  

  ​        见**3.2 设置集群配置文件** 章节

  

- **节点存在独立公网IP（选项一）**

   通过DNS提供上为每个节点配置IP路由

  配置示例：

  | 主机记录              | 记录类型 | 记录值        |
  | --------------------- | -------- | ------------- |
  | atlas01.sigsus.cn     | A        | 121.46.18.xxx |
  | atlas-gpu01.sigsus.cn | A        | 121.46.18.xxx |
  | atlas-gpu02.sigsus.cn | A        | 121.46.18.xxx |

- **节点不存在公网IP（选项二）**

     此种情况需为每个节点配置 短域名解析，于SHELL执行以下指令

  - `mkdir -p deploy/etc`

  - 配置hosts

     ```shell
     cat << EOF > deploy/etc/hosts
     127.0.0.1       localhost
     
      192.168.3.2    atlas02
      192.168.3.2    atlas02.sigsus.cn
      192.168.3.2    atlas.sigsus.cn
     
      192.168.3.6    atlas01
      192.168.3.6    atlas01.sigsus.cn
     
     
      192.168.3.3    atlas-gpu02
      192.168.3.3    atlas-gpu02.sigsus.cn
     
      192.168.3.4    atlas-gpu01
      192.168.3.4    atlas-gpu01.sigsus.cn 
     EOF
     ```

  - `chmod 666 deploy/etc/hosts`

  - 于HOST路径（容器外）**DLWorkspace/src/ClusterBootstrap/** 下执行

     `cp ./deploy/etc/hosts  /etc/hosts`

----

### 3. 执行安装

#### 3.1 安装脚本路径说明       

​		执行目录 ：**DLWorkspace/src/ClusterBootstrap/**  
​		集群配置文件：**DLWorkspace/src/ClusterBootstrap/config.yaml**   
​		安装脚本：**DLWorkspace/src/ClusterBootstrap/deploy.py**

#### 3.2 设置集群配置文件

​	以下字段做相应修改

| cluster_name      | 集群名称，必须保证唯一                                       |
| ----------------- | ------------------------------------------------------------ |
| DLWSAdmins        | 程序部署集群所采用的用户                                     |
| mysql_password    | 集群mysql存储的登录密码                                      |
| machines          | 格式：<br>    machine_hostname: <br>             role: infrastructure<br/>             private-ip: 192.168.3.6<br/>             archtype: arm64<br/>             type: npu<br/>             vendor: huawei<br />    machine_hostname:<br/>              archtype: amd64<br/>              role: worker<br/>              type: gpu <br/>              vendor: nvidia<br/>              os: ubuntu<br><BR> 说明：<BR> archType表示架构，arm64或amd64<br> type表示计算设备类型，gpu或npu<BR> os当前支持ubuntu |
| dockerregistry    | 镜像仓库                                                     |
| onpremise_cluster | 集群基础信息<BR>worker_node_num表示总worker数<br>gpu_count_per_node表示每个节点的总GPU/NPU数 |
| mountpoints       | 存储挂载点配置<BR>字段说明：<BR>        server表示存储点的主机名<BR>        filesharename表示存储Server的数据存储路径<BR>        curphysicalmountpoint表示存储设备在集群各个节点的挂载路径 |
| k8s-gitbranch     | k8s版本号                                                    |
| Authentications   | 登录方式：<br>       1. 用户名密码（默认登录方式）<BR>       2. 微软登录 <BR>       3. 微信登录 <br> <br> 微软登录：<br> TenantId、ClientId、ClientSecret的获取请参考 [微软官方说明](https://portal.azure.com/#blade/Microsoft_AAD_IAM/ActiveDirectoryMenuBlade/RegisteredApps) <br> <br> 微信登录：<br> TenantId、ClientId、ClientSecret的获取请参考 [微信官方说明](https://open.weixin.qq.com/cgi-bin/frame?t=home/web_tmpl&lang=zh_CN) <br> <BR> 域名说明：<br> 假设cluster_name=atlas，domain=**sigsus.cn** <BR> 那么域名即为：**atlas.sigsus.cn** <BR> 微软登录与微信登录 必须基于此域名申请 |

config.yaml样例

```yaml
cluster_name: atlas

network:
  domain: sigsus.cn
  container-network-iprange: "10.0.0.0/8"

etcd_node_num: 1
mounthomefolder : True

# These may be needed for KubeGPU
# kube_custom_cri : True
# kube_custom_scheduler: True
kubepresleep: 1

UserGroups:
  DLWSAdmins:
    Allowed:
    - jinlmsft@hotmail.com
    - jinli.ccs@gmail.com
    - jin.li@apulis.com
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

admin_username: dlwsadmin

# settings for docker
dockerregistry: apulistech/
dockers:
  hub: apulistech/
  tag: "1.9"
 

custom_mounts: []
admin_username: dlwsadmin

# settings for docker
dockerregistry: apulistech/
dockers:
  hub: apulistech/
  tag: "1.9"
 

custom_mounts: []
data-disk: /dev/[sh]d[^a]
dataFolderAccessPoint: ''

datasource: MySQL
defalt_virtual_cluster_name: platform
default-storage-folders:
- jobfiles
- storage
- work
- namenodeshare

deploymounts: []

discoverserver: 4.2.2.1
dltsdata-atorage-mount-path: /dltsdata
dns_server:
  azure_cluster: 8.8.8.8
  onpremise: 10.50.10.50
  
Authentications:
  Microsoft:
    TenantId: 19441c6a-f224-41c8-ac36-82464c2d9b13
    ClientId: 487f34da-74af-4c0d-85d9-d678a118d99d
    ClientSecret: "1MZ[7?g0vPv_6cahAvPuohwuQKrrJEh."

  Wechat:
    AppId: "wx403e175ad2bf1d2d"
    AppSecret: "dc8cb2946b1d8fe6256d49d63cd776d0"

supported_platform:  ["onpremise"]
onpremise_cluster:
  worker_node_num:    1
  gpu_count_per_node: 1
  gpu_type:           nvidia

mountpoints:
  nfsshare1:
    type: nfs
    server: atlas02
    filesharename: /mnt/local
    curphysicalmountpoint: /mntdlws
    mountpoints: ""

jwt:
  secret_key: "Sign key for JWT"
  algorithm: HS256
  token_ttl: 86400

k8sAPIport: 6443
k8s-gitbranch: v1.18.0
deploy_method: kubeadm

repair-manager:
  cluster_name: "atlas"
  ecc_rule:
    cordon_dry_run: True
  alert:
          #    smtp_url: smtp.office365.com
          #    login: dev@apulis.com
          #    smtp_url: smtp.office365.com
          #    login: dev@apulis.com
          #    password: Yitong#123
          #    sender: dev@apulis.com
    smtp_url: smtp.qq.com
    login: 1023950387@qq.com
    password: vtguxryxqyrkbfdd
    sender: 1023950387@qq.com
    receiver: ["1023950387@qq.com"]

enable_custom_registry_secrets: True
```

#### 3.3 设置**DEV**执行环境

- 通过容器执行部署（**选项一**）

  AMD64架构：`python3 devenv.py`

  Arm64架构：`python3 devenv_arm64.py`

  架构配置要求，请参考 **“2.1 安装要求”、“编译节点”说明**

- 通过HOST机器执行部署（**选项二**）

  cd到执行目录，执行`./scripts/prepare_ubuntu_dev.sh`


#### 3.4 创建集群ID

```shell
./deploy.py --verbose -y build 
```

#### 3.5 配置节点ROOT用户密码

​       为集群节点配置一致的ROOT密码，执行指令：

​    （此步骤用于为每个节点 创建部署集群的用户）

```shell
cd deploy/sshkey
echo "root" > "rootuser"
echo "your_root_password" > "rootpasswd"
```

#### 3.6 安装SSH Key到所有节点

```
 ./deploy.py --verbose sshkey install
```

#### 3.7 配置/检测内网DNS解析 

- **节点不存在公网IP（参考关联章节： 2.4 配置Worker节点DNS - 节点不存在公网IP）**

```
./deploy.py --verbose copytoall ./deploy/etc/hosts  /etc/hosts
```

-  **检查所有机器DNS配置是否成功**（atlas02为master节点主机名）：


``` shell
./deploy.py execonall ping atlas02 -c 2
```

-  **检查集群节点是否可正常访问**

```
./deploy.py --verbose execonall sudo ls -al
```

#### 3.8 设置集群节点的安装环境

```shell
./deploy.py --verbose runscriptonall ./scripts/prepare_ubuntu.sh

上一个语句会重启worker节点。需等待所有服务器 启动完毕，再执行以下步骤！！
./deploy.py --verbose runscriptonall ./scripts/prepare_ubuntu.sh continue
./deploy.py --verbose execonall sudo usermod -aG docker dlwsadmin
```


其中：

- dlwsadmin为操作集群机器所采用的用户名，配置于config.yaml
- 如果nvidia驱动安装不成功，可能与具体的设备配置有关，譬如secure boot问题等等，请联系开发人员定位和完善

  

#### 3.9 GPU Worker机器状态确认

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



#### 3.10 安装kubeadm客户端
     ./deploy.py runscriptonroles infra worker ./scripts/install_kubeadm.sh

#### 3.11 安装K8S集群平台

- ##### 各节点关闭swap

    ```
    ./deploy.py --verbose execonall sudo swapoff -a
    ```

- ##### master节点永久关闭swap
    ```
    sudo sed -i.bak '/ swap / s/^\(.*\)$/#\1/g' /etc/fstab
    ```
    
- ##### 安装集群基础软件

    ```
    ./deploy.py --verbose kubeadm init
    ./deploy.py --verbose copytoall ./deploy/sshkey/admin.conf /root/.kube/config
    ```
- ##### （可选）设置master充当worker
    ```shell script
    ./deploy.py --verbose kubernetes uncordon 
    ```
- ##### 设置集群节点标签
  
    ```
    ./deploy.py --verbose kubeadm join
    ./deploy.py --verbose -y kubernetes labelservice
    ./deploy.py --verbose -y labelworker
    ```


#### 3.12 挂载存储节点

- ##### 安装NFS服务
    ```
    ./deploy.py runscriptonroles infra worker ./scripts/install_nfs.sh
    ```


- ##### 配置挂载目录（在存储节点操作）

  - 创建目录

     假设所要挂载的目录是：/mnt/local

     挂载目录需与config.yaml中所配置一致

     > mountpoints:
     >   nfsshare1:
     >     type: nfs
     >     server: atlas02
     >     filesharename: /mnt/local
     >     curphysicalmountpoint: /mntdlws
     >     mountpoints: ""

     执行：

     `mkdir -p /mnt/local`

  - 设置白名单

     编辑文件 /etc/exports，并增加挂载目录的IP白名单

     >  /mnt/local               *(rw,fsid=0,insecure,no_subtree_check,async,no_root_squash)

- ##### 更新共享信息

        sudo exportfs -a
    
- ##### 执行挂载

        ./deploy.py --verbose mount

- **挂载结果确认**

  ```
  ./deploy.py --verbose execonall df -h
  ```

#### 3.13 部署NVidia GPU/A910 NPU插件

```
./deploy.py --verbose kubernetes start nvidia-device-plugin
./deploy.py --verbose kubernetes start a910-device-plugin
```

​    

#### 3.14 重置APIServer NodePort端口段

​          编辑文件/etc/kubernetes/manifests/kube-apiserver.yaml

​          增加指定参数：

```
- --service-node-port-range=30000-49999
```

​           保存文件后，大约1分钟后API-SERVER自动重启并生效

#### 3.15 设置webui服务配置文件

​		文件：**DLWorkspace/src/dashboard/config/local.yaml**

​		domain、casUrl以及clusters，**三者需依据集群实际的域名，做相应的修改**

​		内容样例：

```
sign: "Sign key for JWT"
winbind: "Will call /domaininfo/GetUserId with userName query to get the user's id info"
masterToken: "Access token of all users"

activeDirectory:
  tenant: "19441c6a-f224-41c8-ac36-82464c2d9b13"
  clientId: "487f34da-74af-4c0d-85d9-d678a118d99d"
  clientSecret: "1MZ[7?g0vPv_6cahAvPuohwuQKrrJEh."

wechat:
  appId: "wx403e175ad2bf1d2d"
  appSecret: "dc8cb2946b1d8fe6256d49d63cd776d0"

domain: "https://atlas.sigsus.cn"
administrators:
  - jinlmsft@hotmail.com
  - jin.li@apulis.com
  - bifeng.peng@apulis.com
  - hui.yuan@apulis.com

clusters:
  atlas02:
      restfulapi: "http://altas.sigsus.cn/apis"
      title: Grafana-endpoint-of-the-cluster
      workStorage: work
      dataStorage: data
      grafana: "https://atlas.sigsus.cn/grafana/"
      prometheus: http://atlas.sigsus.cn:9091
      
userGroup:
  type: custom
  domain: http://atlas.sigsus.cn
  backEndPath: /custom-user-dashboard-backend
  frontEndPath: /custom-user-dashboard
```



#### 3.16 部署集群应用 

1. ##### 本地登录docker hub （用户名为config.yaml所配置）
    > docker login

2. ##### 生成dashboard, jobmanager等服务的配置文件
    ```
    ./deploy.py --verbose webui         
    ```

3. ##### 编译restfulapi和webui3服务
    - master为AMD64架构
        ```
        ./deploy.py --verbose docker push restfulapi2
        ./deploy.py --verbose docker push webui3
        ./deploy.py --nocache docker push custom-user-dashboard-frontend
        ./deploy.py --nocache docker push custom-user-dashboard-backend
        ```
    - master为ARM64架构
        ```
        ./deploy.py --verbose --archtype arm64 docker push restfulapi2
        ./deploy.py --verbose --archtype arm64 docker push webui3
        ./deploy.py --nocache --archtype arm64 docker push custom-user-dashboard-frontend
        ./deploy.py --nocache --archtype arm64 docker push custom-user-dashboard-backend
        ```

4. ##### 编译对请求加密组件openresty
    - master为AMD64架构
        ```shell script
        ./deploy.py --verbose docker push openresty
        ```
    - master为ARM64架构
        ```shell script
        ./deploy.py --verbose --archtype arm64 docker push openresty
        ```
    
5. ##### 编译Job容器的依赖容器（请参考DLWorkspace/src/ClusterBootstrap/step_by_step.sh）：
    - master为AMD64架构
        ```
        ./deploy.py --verbose docker push init-container
        ```
        如果集群有arm64架构的worker机器，在其中一台arm64的worker机器上执行  
        ```
        ./deploy.py --verbose --archtype arm64 docker push init-container
        ```
   - master为ARM64架构
        ```
        ./deploy.py --verbose --archtype arm64 docker push init-container
        ```
        如果集群有amd64架构的worker机器，在其中一台amd64的worker机器上执行  
        ```
        ./deploy.py --verbose docker push init-container
        ```
  
6. ##### 编译监控相关的镜像
    - master为AMD64架构
        ```shell script
        ./deploy.py docker push watchdog
        ./deploy.py docker push gpu-reporter
        ./deploy.py docker push job-exporter
        ./deploy.py docker push repairmanager2
        ```
        注： 如果集群有arm64架构的worker机器，在其中一台arm64机器上执行
        ```shell script
        ./deploy.py --archtype arm64 docker push watchdog
        ./deploy.py --archtype arm64 docker push gpu-reporter
        ./deploy.py --archtype arm64 docker push job-exporter
        ```
    - master为ARM64架构
        ```shell script
        ./deploy.py --archtype arm64 docker push watchdog
        ./deploy.py --archtype arm64 docker push gpu-reporter
        ./deploy.py --archtype arm64 docker push job-exporter
        ./deploy.py --archtype arm64 docker push repairmanager2
        ```
        注： 如果集群有amd64架构的worker机器，在其中一台amd64机器上执行
        ```shell script
        ./deploy.py docker push watchdog
        ./deploy.py docker push gpu-reporter
        ./deploy.py docker push job-exporter
        ```

7. ##### 配置Nginx
    ```
    ./deploy.py --verbose nginx fqdn
    ./deploy.py --verbose nginx config
    ```

8. ##### 启动mysql并设置
    ```shell script
      ./deploy.py --verbose kubernetes start mysql
    ```
    进入mysql容器
    ```shell script
      mysql -uroot -p
      use mysql;
      create user 'root'@'%' identified by '';
      grant all privileges on *.* to 'root'@'%' with grant option;
      flush privileges;
      alter user 'root'@'%' identified with mysql_native_password by 'apulis#2019#wednesday';
    ```

9. ##### 启动集群应用
    ```
    1. ./deploy.py --verbose kubernetes start jobmanager2 restfulapi2 monitor nginx custommetrics repairmanager2 openresty
    2. ./deploy.py --background --sudo runscriptonall scripts/npu/npu_info_gen.py
3. ./deploy.py --verbose kubernetes start monitor
    ```
    
    注意：
    
    - 指令2必须先于指令3执行
    - node重启后 需重新执行指令2 (华为发布NPU-SMI指令后, 可自动兼容)
    
    
    
10. ##### 启动dashboard

    - master为AMD64架构
        ```
        ./deploy.py --verbose nginx webui3
        ./deploy.py --verbose kubernetes start webui3
        ./deploy.py kubernetes start custom-user-dashboard
        ```
    - master为ARM64架构
        ```
        ./deploy.py --verbose --archtype arm64 nginx webui3
        ./deploy.py --verbose kubernetes start webui3
        ./deploy.py kubernetes start custom-user-dashboard
        ```



