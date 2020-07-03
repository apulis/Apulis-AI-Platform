鏊立人工智能开放平台部署指导
=================================================================================

### 版本更新记录

  | 版本号 |	更新时间 |	主要内容或重大修改 | 修改人 |
  | :------:| :------: | :------: |:------: |
  0.0.8	| 2020年05月27日	| 初稿 | bifeng
  0.1.0	| 2020年07月03日	| 终稿 | haiyuan

### 商标和版权 

  "Apulis Platform",“鏊立人工智能开放平台”及图标是依瞳科技有限公司的商标或注册商标。

  在本用户手册中描述的随机软件，是基于最终用户许可协议的条款和条件提供的，只能按照该最终用户许可协议的规定使用和复制。 
  本手册受到著作权法律法规保护，未经依瞳科技有限公司事先书面授权，任何人士不得以任何方式对本手册的全部或任何部分进行复制、抄录、删减或将其编译为机读格式，以任何形式在可检索系统中存储，在有线或无线网络中传输，或以任何形式翻译为任何文字。 

### 文档说明  

  本文档支持用户半自动化完成网络配置，系统设置，平台的集群环境和基础应用的安装，以及平台版本更新和配置备份和恢复。其中涉及linux系统配置，GPU,NPU驱动安装，docker, k8s集群安装和平台集群部署。要求参与部署的运维人员熟悉linux,docker和k8s等。
  详细操作步骤和结果可以参看安装部署指导视频：(https://pan.baidu.com/s/1wIhf8cRqkB0Ruel__xq-QQ 提取码: 65ic)。


***

安装环境说明
-------------------------------------------------------------------------------

**安装要求**
* 普通节点：ubuntu 18.04 server
* NPU节点：ubuntu 18.04.1 server （小版本号必须为1）
* 管理员账号：root或具备sudo权限的用户，所有节点允许root登录，保持密码一致
* worker节点：需禁用SecureBoot（影响GPU驱动无法升级）
* 所有节点：允许SSH连接
* GPU节点：预装NVIDIA GPU驱动，请参考[安装手册](https://docs.nvidia.com/datacenter/tesla/sla-installation-notes/index.html#ubuntu-lts)
* NPU节点：预装华为NPU驱动，请参考 A910驱动安装手册
* **编译节点：CPU架构需与master节点架构一致，譬如同为AMD64或同为Arm64**
* 支持的平台版本：Apulis Platfom release-0.1.0

其中：
* worker节点配置需求
   - 若设备为NVIDIA GPU，则驱动版本不低于 430
   - 若设备为Huawei NPU，则驱动版本不低于 1.72.T2.100.B020

节点组网规划示例
--------------------------------------------------------------------------------
**master和worker需在同一个子网或VPC**
| 主机名       | 配置    | 计算设备  | 操作系统       | 公网IP        | 子网IP      | 描述                          |
| ----------- | ------- | -------- | -------------- | ------------ | ----------- | ----------------------------- |
| atlas02     | 6C64G   | 8 NPU    | ubuntu 18.04.1 | 121.46.18.84 | 192.168.3.6 | k8s master节点/NPU worker 节点 |
| atlas01     | 36C512G | 8 NPU    | ubuntu 18.04.1 | 121.46.18.84 | 192.168.3.2 | NPU Woker节点                  |
| atlas-gpu01 | 6C64G   | 8 GPU    | ubuntu 18.04.4 | 121.46.18.84 | 192.168.3.4 | GPU Woker节点                  |
| atlas-gpu02 | 6C64G   | 8 GPU    | ubuntu 18.04.4 | 121.46.18.84 | 192.168.3.3 | GPU Woker节点                  |

节点基础配置
-------------------------------------------------------------------------------

1. 配置节点Hostname

    * 按照部署规划配置各个节点的主机名
        ```
        vim /etc/hostname
        # 更新内容为atlas01
        # 设置hostname立即生效：
        sudo hostnamectl set-hostname atlas02
        ``` 

2. 配置域名DNS（如不提供公网访问，请跳过此章节）

    ​*需要在DNS提供商如[阿里云](https://dns.console.aliyun.com)控制台进行配置*

    * 主域名：**sigsus.cn**
    * 配置示例：

    | 主机记录 | 记录类型 | 记录值 |
    | ---- | ---- | ---- |
    | atlas.sigsus.cn | A | 121.46.18.84 |

3. 配置Worker节点DNS

* 配置短域名搜索（所有节点）

  - 安装resolvconf
    ```
    apt update
    apt install resolvconf
    ```
  - 增加短域名   
    ```
    ​mkdir -p /etc/resolvconf/resolv.conf.d/
    ​echo "search sigsus.cn" > /etc/resolvconf/resolv.conf.d/base
    sudo resolvconf -u
    ​​# 此处**sigsus.cn**即为config.yaml中的domain
    ```

* 同步代码库

    ```
      mkdir -p /home/apulis
      cd /home/apulis
      git clone https://gitee.com/apulis/apulis_platform.git
      cd apulis_platform/src/ClusterBootstrap/
    ```

* 选项一：节点有独立公网IP

   通过DNS提供商为每个节点配置，配置示例：

  | 主机记录              | 记录类型 | 记录值        |
  | --------------------- | -------- | ------------- |
  | atlas01.sigsus.cn     | A        | 121.46.18.xxx |
  | atlas-gpu01.sigsus.cn | A        | 121.46.18.xxx |
  | atlas-gpu02.sigsus.cn | A        | 121.46.18.xxx |

* 选项二：节点没有公网IP

    *此种情况需为每个节点配置短域名解析：*
    ```
    # mkdir -p deploy/etc

    # 配置hosts
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

    chmod 666 deploy/etc/hosts
    # 在HOST src/ClusterBootstrap/目录下执行
    cp ./deploy/etc/hosts  /etc/hosts
    ```

执行安装
-------------------------------------------------------------------------------

1. 安装脚本路径说明

    * ​执行目录 ：**src/ClusterBootstrap/**  
    * ​集群配置文件：**src/ClusterBootstrap/config.yaml**   
    * ​安装脚本：**src/ClusterBootstrap/deploy.py**

2. 设置集群配置文件

​	以下字段做相应修改

| cluster_name      | 集群名称，必须保证唯一                                       |
| ----------------- | ------------------------------------------------------------ |
| DLWSAdmins        | 程序部署集群所采用的用户                                     |
| mysql_password    | 集群mysql存储的登录密码                                      |
| machines          | 配置项：<br>    machine_hostname: <br>             role: infrastructure<br/>             private-ip: 192.168.3.6<br/>             archtype: arm64<br/>             type: npu<br/>             vendor: huawei<br />    machine_hostname:<br/>              archtype: amd64<br/>              role: worker<br/>              type: gpu <br/>              vendor: nvidia<br/>              os: ubuntu<br>说明：<BR> archType表示架构，arm64或amd64<br> type表示计算设备类型，gpu或npu<BR> os当前支持ubuntu |
| dockerregistry    | 镜像仓库                                                     |
| onpremise_cluster | 集群基础信息<BR>worker_node_num表示总worker数<br>gpu_count_per_node表示每个节点的总GPU/NPU数 |
| mountpoints       | 存储挂载点配置<BR>字段说明：<BR>        server表示存储点的主机名<BR>        filesharename表示存储Server的数据存储路径<BR>        curphysicalmountpoint表示存储设备在集群各个节点的挂载路径 |
| k8s-gitbranch     | k8s版本号                                                    |
| Authentications   | 登录方式：<br>       1. 用户名密码（默认登录方式）<BR>       2. 微软登录 <BR>TenantId、ClientId、ClientSecret的获取请参考 [微软官方说明](https://portal.azure.com/#blade/Microsoft_AAD_IAM/ActiveDirectoryMenuBlade/RegisteredApps) <br> 3. 微信登录：<br> TenantId、ClientId、ClientSecret的获取请参考 [微信官方说明](https://open.weixin.qq.com/cgi-bin/frame?t=home/web_tmpl&lang=zh_CN) <br> 域名说明：<br> 假设cluster_name=atlas，domain=**sigsus.cn** <BR> 那么域名即为：**atlas.sigsus.cn** <BR> 微软登录与微信登录 必须基于此域名申请 |

* 平台配置示例

```yaml
# Configuration Path: src/ClusterBootstrap/config.yaml
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
    - jeck@hotmail.com
    - stef.jobs@gmail.com
    - tony@apulis.com
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
mysql_password: ********
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
    TenantId: ********
    ClientId: ********
    ClientSecret: ********  

  Wechat:
    AppId: ********
    AppSecret: ********

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
    smtp_url: smtp.qq.com
    login: admin@admin.com
    password: *********
    sender: dmin@admin.com
    receiver: ["dmin@admin.com"]

enable_custom_registry_secrets: True
```

3. 设置配置环境

* 在master host节点执行部署
  ```
  # 执行环境配置脚本
  ./scripts/prepare_ubuntu_dev.sh
  ```

4. 创建集群ID

    `./deploy.py --verbose -y build`

5. 配置节点用户密码

    **为集群每个节点配置一致的ROOT密码，执行指令：**

    ```bash
    cd deploy/sshkey
    echo "root" > "rootuser"
    echo "<your_root_password>" > "rootpasswd"
    ```

6. 安装SSH Key到所有节点

    `./deploy.py --verbose sshkey install`

7. 配置/检测内网DNS解析 

* 节点不存在公网IP

    `./deploy.py --verbose copytoall ./deploy/etc/hosts  /etc/hosts`

* 检查所有机器DNS配置是否成功（atlas02为master节点主机名）：

    `./deploy.py execonall ping atlas02 -c 2`

* 检查集群节点是否可正常访问

  `./deploy.py --verbose execonall sudo ls -al`

8. 设置集群节点的安装环境

    ```bash
    ./deploy.py --verbose runscriptonall ./scripts/prepare_ubuntu.sh

    # 上一个语句会重启安装有GPU的worker节点。需等待所有服务器启动完毕，再执行以下步骤！！
    ./deploy.py --verbose runscriptonall ./scripts/prepare_ubuntu.sh continue
    ./deploy.py --verbose execonall sudo usermod -aG docker dlwsadmin
    ```


    **注意：**

    * dlwsadmin为操作集群机器所采用的用户名，配置于config.yaml
    * 如果nvidia驱动安装不成功，可能与具体的设备配置有关，譬如secure boot问题等等，请联系开发人员定位和完善

9. GPU Worker机器状态确认

    * 确认nvidia驱动与nvidia-docker均已正常安装
      ```
      nvidia-docker run --rm dlws/cuda nvidia-smi
      docker run --rm -ti dlws/cuda nvidia-smi
      ```
    * 注意问题：
    如docker指令执行正常，但nvidia-docker指令执行错误，则修改`/etc/docker/daemon.json`，
    将"nvidia-docker"设置为`default runtime`
      ```
      vim /etc/docker/daemon.json
      # 增加
      "default-runtime": "nvidia",

      # 完整例子：
      {
          "default-runtime": "nvidia",
          "runtimes": {
              "nvidia": {
                  "path": "nvidia-container-runtime",
                  "runtimeArgs": []
              }
          }
      }

      # 然后重启docker：
      systemctl daemon-reload
      systemctl restart docker
      ```

10. 安装kubeadm客户端

    `./deploy.py runscriptonroles infra worker ./scripts/install_kubeadm.sh`

11. 安装K8S集群平台

* 关闭各节点swap

  `./deploy.py --verbose execonall sudo swapoff -a`


- master节点永久关闭swap

  `sudo sed -i.bak '/ swap / s/^\(.*\)$/#\1/g' /etc/fstab`
    
- 安装集群基础软件

    * 初始化 k8s

      ```
      ./deploy.py --verbose kubeadm init

      # 如果 coredns 一直pending，请执行：
      # weave.yaml 可以在 https://cloud.weave.works/k8s/net?k8s-version=v1.18.0 下载
      kubectl apply -f  /home/apulis/weave.yaml
      ```

      *如果出现 x509: certificate signed by unknown authority，在同步证书之前执行以下步骤：*

      ```
      cp  /etc/kubernetes/admin.conf ./deploy/sshkey/admin.conf
      chown $(id -u):$(id -g) ./deploy/sshkey/admin.conf
      ```

    * 同步证书

      `./deploy.py --verbose copytoall ./deploy/sshkey/admin.conf /root/.kube/config`

- （可选）设置master充当worker

    `./deploy.py --verbose kubernetes uncordon`

- 设置集群节点标签
  
    ```
    ./deploy.py --verbose kubeadm join
    ./deploy.py --verbose -y kubernetes labelservice
    ./deploy.py --verbose -y labelworker
    ```

12. 挂载存储节点

* 安装NFS服务

    `./deploy.py runscriptonroles infra worker ./scripts/install_nfs.sh`


* 配置挂载目录（在存储节点操作）

  - 创建挂载目录：

    `mkdir -p /mnt/local`

    *挂载目录需与`config.yaml`中所配置一致*
    ```
    mountpoints:
      nfsshare1:
        type: nfs
        server: atlas02
        filesharename: /mnt/local
        curphysicalmountpoint: /mntdlws
        mountpoints: ""
    ```
  * 设置访问白名单

    *编辑文件 `vim /etc/exports`，并增加挂载目录的IP白名单*

    `/mnt/local   *(rw,fsid=0,insecure,no_subtree_check,async,no_root_squash)`

* 更新共享目录信息

    sudo exportfs -a
    
* 执行挂载

  `./deploy.py --verbose mount`

* 挂载结果确认

    `./deploy.py --verbose execonall df -h`

13. 部署GPU/A910 NPU插件

    ```
    ./deploy.py --verbose kubernetes start nvidia-device-plugin
    ./deploy.py --verbose kubernetes start a910-device-plugin
    ```

14. 重置APIServer NodePort端口段

    *编辑文件`vim /etc/kubernetes/manifests/kube-apiserver.yaml`*

    `- --service-node-port-range=30000-49999`

    *保存文件后，大约1分钟后API-SERVER自动重启并生效*

15. 设置webui服务配置文件

    * 配置：../dashboard/config/local.yaml

    *根据据集群实际的域名，修改domain、casUrl以及clusters*

    ```
    sign: "Sign key for JWT"
    winbind: "Will call /domaininfo/GetUserId with userName query to get the user's id info"
    masterToken: "Access token of all users"

    activeDirectory:
    tenant: "********************"
    clientId: "********************"
    clientSecret: "********************"

    wechat:
    appId: "********************"
    appSecret: "********************"

    domain: "https://atlas.sigsus.cn"
    administrators:
    - tom@hotmail.com
    - jeck@apulis.com
    - tony@apulis.com
    - tomas@apulis.com

    clusters:
    atlas02:
        restfulapi: "http://altas02.sigsus.cn/apis"
        title: Grafana-endpoint-of-the-cluster
        workStorage: work
        dataStorage: data
        grafana: "http://atlas02.sigsus.cn/grafana/"
        prometheus: http://atlas02.sigsus.cn:9091
        
    userGroup:
    type: custom
    domain: http://atlas02.sigsus.cn
    backEndPath: /custom-user-dashboard-backend
    frontEndPath: /custom-user-dashboard
    ```


部署集群应用
-------------------------------------------------------------------------------

1. 本地登录docker hub （账号为 docker hub 注册账号）
    > docker login

2. 生成dashboard, jobmanager等服务的配置文件

    `./deploy.py --verbose webui`

3. 编译restfulapi和webui3服务
    * master为AMD64架构
        ```
        ./deploy.py --verbose docker push restfulapi2
        ./deploy.py --verbose docker push webui3
        ./deploy.py --nocache docker push custom-user-dashboard-frontend
        ./deploy.py --nocache docker push custom-user-dashboard-backend
        ```
    * master为ARM64架构
        ```
        ./deploy.py --verbose --archtype arm64 docker push restfulapi2
        ./deploy.py --verbose --archtype arm64 docker push webui3
        ./deploy.py --nocache --archtype arm64 docker push custom-user-dashboard-frontend
        ./deploy.py --nocache --archtype arm64 docker push custom-user-dashboard-backend
        ```

4. 编译对请求加密组件openresty
    * master为AMD64架构
        ```bash script
        ./deploy.py --verbose docker push openresty
        ```
    * master为ARM64架构
        ```bash script
        ./deploy.py --verbose --archtype arm64 docker push openresty
        ```
    
5. 编译依赖容器（请参考src/ClusterBootstrap/step_by_step.sh）：
    * master为AMD64架构
        ```
        ./deploy.py --verbose docker push init-container
        ```
        *注：如果集群有arm64架构的worker机器，在其中一台arm64的worker节点上编译并上传镜像*
        ```
        ./deploy.py --verbose --archtype arm64 docker push init-container
        ```
   * master为ARM64架构
        ```
        ./deploy.py --verbose --archtype arm64 docker push init-container
        ```
        *注：如果集群有amd64架构的worker机器，在其中一台amd64的worker节点上编译并上传镜像*
        ```
        ./deploy.py --verbose docker push init-container
        ```
  
6. 编译监控相关的镜像
    * master为AMD64架构
        ```bash script
        ./deploy.py docker push watchdog
        ./deploy.py docker push gpu-reporter
        ./deploy.py docker push job-exporter
        ./deploy.py docker push repairmanager2
        ```
        *注：如果集群有arm64架构的worker机器，在其中一台arm64节点上编译并上传镜像*
        ```bash script
        ./deploy.py --archtype arm64 docker push watchdog
        ./deploy.py --archtype arm64 docker push gpu-reporter
        ./deploy.py --archtype arm64 docker push job-exporter
        ```
    * master为ARM64架构
        ```bash script
        ./deploy.py --archtype arm64 docker push watchdog
        ./deploy.py --archtype arm64 docker push gpu-reporter
        ./deploy.py --archtype arm64 docker push job-exporter
        ./deploy.py --archtype arm64 docker push repairmanager2
        ```
        *注：如果集群有amd64架构的worker机器，在其中一台amd64节点上编译并上传镜像*
        ```bash script
        ./deploy.py docker push watchdog
        ./deploy.py docker push gpu-reporter
        ./deploy.py docker push job-exporter
        ```

7. 配置Nginx
    ```
    ./deploy.py --verbose nginx fqdn
    ./deploy.py --verbose nginx config
    ```

8. 启动mysql并设置

    `./deploy.py --verbose kubernetes start mysql`
    * 安装Mysql客户端 `apt install mysql-client-core-5.7 -y`
    * 配置Mysql账号和权限
    ```bash
      mysql -h 127.0.0.1 -uroot -p
      use mysql;
      grant all privileges on *.* to 'root'@'%' with grant option;
      flush privileges;
      alter user 'root'@'%' identified with mysql_native_password by '******';
    ```

9. 启动集群应用

    ***注意npu的配置必须在启用monitor之前配置***
    ```
    ./deploy.py --verbose kubernetes start jobmanager2 restfulapi2 nginx custommetrics repairmanager2 openresty
    ./deploy.py --background --sudo runscriptonall scripts/npu/npu_info_gen.py
    ./deploy.py --verbose kubernetes start monitor
    ```

10. 启动dashboard
    * master为AMD64架构
        ```
        ./deploy.py --verbose nginx webui3
        ./deploy.py --verbose kubernetes start webui3
        ./deploy.py kubernetes start custom-user-dashboard
        ```
    * master为ARM64架构
        ```
        ./deploy.py --verbose --archtype arm64 nginx webui3
        ./deploy.py --verbose kubernetes start webui3
        ./deploy.py kubernetes start custom-user-dashboard
        ```


平台升级
-------------------------------------------------------------------------------

1. 同步版本或更新源码分支

2. 更新dashboard, jobmanager等服务的配置文件
    ```
    ./deploy.py --verbose webui         
    ```

3. 编译restfulapi和webui3服务
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

4. 编译监控相关的镜像
    - master为AMD64架构
        ```bash script
        ./deploy.py docker push watchdog
        ./deploy.py docker push gpu-reporter
        ./deploy.py docker push job-exporter
        ./deploy.py docker push repairmanager2
        ```
    - master为ARM64架构
        ```bash script
        ./deploy.py --archtype arm64 docker push watchdog
        ./deploy.py --archtype arm64 docker push gpu-reporter
        ./deploy.py --archtype arm64 docker push job-exporter
        ./deploy.py --archtype arm64 docker push repairmanager2
        ```

5. 更新Nginx配置
    ```
    ./deploy.py --verbose nginx fqdn
    ./deploy.py --verbose nginx config
    ```

6.  重启应用后端
    ```
    ./deploy.py --verbose kubernetes stop/start jobmanager2 restfulapi2 nginx custommetrics repairmanager2 openresty
    ./deploy.py --background --sudo runscriptonall scripts/npu/npu_info_gen.py
    ./deploy.py --verbose kubernetes stop/start monitor
    ```

7. 重启dashboard
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

备份和恢复配置
-------------------------------------------------------------------------

* 备份配置

    ```
    cd src/ClusterBootstrap
    ./deploy.py backup [backup_file_prefix] [password]
    ```

* 恢复配置

    `./deploy.py restore [backup_file] [password]`



清除集群信息(***高危操作***)
-------------------------------------------------------------------------

* 清除所有的worker信息

  `./deploy.py kubeadm reset`

* 清除master集群信息

  ```
  kubeadm reset
  rm -rf /etc/cni/net.d/&&rm -rf $HOME/.kube
  ```


FAQ
-------------------------------------------------------------------------------
1.  `deploy.py 4063 line：AttributeError: 'module' object has no attribute 'FullLoader'`
    ```
    # 在部署环境中重新安装 PyYAML
    pip install --ignore-installed PyYAML
    ```

2. 强制 Kill Job方法：
    ```
    Kubectl get pods                     # 查看僵死pod
    Kubectl delete po --force <pod id>   # 强制 kill pod 
    Docker ps |grep mind* <tf>           # 查看响应的容器
    Docker top <docker id>               # 查看容器内的进程
    Kill -9 <process id>                 # 逐个kill容器中所有进程
    ```

3. 使用私有镜像库

    1. 在提交 job 的 Advanced ——> Custom Docker Registry 中配置私有镜像

    2. 在每个节点的host系统的 docker 配置文件 `/etc/docker/daemon.json` 中指定私有镜像库

    3. 在返回 job 提交页面的 docker image 配置项中指定私有镜像库的镜像，前缀必须加私有镜像库地址


4. `image pull backoff`

    1. 当更新或安装平台时，出现该错误可以在（配置/私有）docker hub 查看镜像是否编译并 push 成功
    2. `kubectl describe po -n <namespace> <podname> `查看错误详情，如果时相关依赖镜像拉取失败则会重试的


5. 登录页返回未认证或空白，需要修正前端配置的链接, 注意https或端口配置
    ```
    # vim /etc/WebUI/local.yaml

    clusters:
      sandbox03-master:
          restfulapi: "http://altas02.sigsus.cn/apis"
          title: Grafana-endpoint-of-the-cluster
          workStorage: work
          dataStorage: data
          grafana: "http://altas02.sigsus.cn/endpoints/grafana/"
          prometheus: http://altas02.sigsus.cn:9091
    userGroup:
      type: custom
      domain: "http://altas02.sigsus.cn"
      backEndPath: /custom-user-dashboard-backend
      frontEndPath: /custom-user-dashboard
    ```

6. NPU 节点重启之后，需要重新配置

    `./deploy.py --background --sudo runscriptonall scripts/npu/npu_info_gen.py`

7. 如果master节点上有gpu资源`./deploy.py --verbose runscriptonall ./scripts/prepare_ubuntu.sh`执行上述语句的时候，master可能会重启，导致worker节点装不上gpu驱动，需要手动在worker节点进行安装
    ```
    sudo add-apt-repository -y ppa:graphics-drivers/ppa
    sudo apt-get purge -y nvidia*
    sudo apt-get update
    yes | sudo apt-get install -y nvidia-driver-430
    sudo shutdown -r
    ```