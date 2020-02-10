# DLWS集群安装步骤

### 目标配置  

操作系统：ubuntu 16.04 LTS、Ubuntu 18.04 LTS  

机器构成：Dev机器1台，Master节点机器1台，携带GPU的Worker节点机器1台   

GPU类别：NVidia，driver版本 >= 430  



### 安装环境预备

1. 免密码配置（针对非root用户）

   - 执行sudo visudo

   - 检查文件内容，确认是否存在以下配置，没有则新增

     ```
     %sudo ALL=(ALL:ALL) ALL
     %sudo ALL=(ALL) NOPASSWD:ALL
     ```

2. GIT配置

   - 检查GIT是否安装

     ```
     执行指令：git --version
     输出如下：git version 2.7.4
     ```

     

   - 如未安装GIT，请执行

     ```
     sudo apt update
     sudo apt install git
     ```

     

   - 配置账号

     ```
     git config user.name "your_github_account_name"
     git config user.email "your_github_account_email"
     ```

   - 配置GitHub访问授权

3. DNS配置

   - 假设环境配置如下

     > master节点主机名：apulis-sz-dev-infra01
     >
     > worker节点主机名：apulis-sz-dev-worker01
     >
     > dev节点主机名：apulis-chinaeast-infra01
     >
     > 域名解析供应商：https://dns.console.aliyun.com
     >
     > 主域名：sigsus.cn

     

   - 配置节点IP到对应域名的映射关系

     apulis-sz-dev-infra01.sigsus.cn

     apulis-sz-dev-worker01.sigsus.cn

     apulis-chinaeast-infra01.sigsus.cn

     

   - 配置子域名快捷搜索（dev、master、worker三个机器）

     修改文件：vim /etc/resolvconf/resolv.conf.d/base  
     增加数据：search sigsus.cn  
     执行指令：sudo resolvconf -u   	

     

   - 检查DNS配置

     指令执行是否成功

     ```
     ping apulis-sz-dev-infra01
     ping apulis-sz-dev-worker01
     ```

     

4. 配置镜像文件

     获取系统codebase

     ```
     lsb_release -a
     ```

     设置apt-get镜像源（假设codebase=bionic，采用aliyun镜像），编辑/etc/apt/sources.list

     ```
     deb http://mirrors.aliyun.com/ubuntu/ bionic main restricted universe multiverse
     deb-src http://mirrors.aliyun.com/ubuntu/ bionic main restricted universe multiverse
     
     deb http://mirrors.aliyun.com/ubuntu/ bionic-security main restricted universe multiverse
     deb-src http://mirrors.aliyun.com/ubuntu/ bionic-security main restricted universe multiverse
     
     deb http://mirrors.aliyun.com/ubuntu/ bionic-updates main restricted universe multiverse
     deb-src http://mirrors.aliyun.com/ubuntu/ bionic-updates main restricted universe multiverse
     
     deb http://mirrors.aliyun.com/ubuntu/ bionic-backports main restricted universe multiverse
     deb-src http://mirrors.aliyun.com/ubuntu/ bionic-backports main restricted universe multiverse
     
     deb http://mirrors.aliyun.com/ubuntu/ bionic-proposed main restricted universe multiverse
     deb-src http://mirrors.aliyun.com/ubuntu/ bionic-proposed main restricted universe multiverse
     
     deb https://mirrors.aliyun.com/docker-ce/linux/ubuntu bionic stable
     ```

     

### 执行安装

1. 设置集群配置文件

   样例

   ```
   cluster_name: apulis-sz-dev
   
   network:
     domain: sigsus.cn
     container-network-iprange: "10.0.0.0/8"
   
   etcd_node_num: 1
   mounthomefolder : True
   
   # These may be needed for KubeGPU
   # kube_custom_cri : True
   # kube_custom_scheduler: True
   kubepresleep: 1
   cloud_influxdb_node: apulis-sz-dev-infra01.sigsus.cn
   
   DeployAuthentications:
   - Microsoft
   - Gmail
   
   UserGroups:
     DLWSAdmins:
       Allowed:
       - jinlmsft@hotmail.com
       - jinli.ccs@gmail.com
       - jin.li@apulis.com
       - bifeng.peng@apulis.com
       gid: "20001"
       uid: "20000"
     DLWSRegister:
       Allowed:
       - '@gmail.com'
       - '@live.com'
       - '@outlook.com'
       - '@hotmail.com'
       gid: "20001"
       uid: 20001-29999
   
   WebUIadminGroups:
   - DLWSAdmins
   
   WebUIauthorizedGroups:
   - DLWSAdmins
   
   WebUIregisterGroups:
   - DLWSRegister
   
   WinbindServers: []
    
   
   
   datasource: MySQL
   mysql_password: apulis#2019#wednesday
   webuiport: 3081
   useclusterfile : true
   platform-scripts : ubuntu
   
   
   machines:
     apulis-sz-dev-infra01:
       role: infrastructure
       private-ip: 115.220.9.243
     apulis-sz-dev-worker01:
       role: worker
       type: gpu
   
   admin_username: dlwsadmin
   
   # settings for docker
   dockerregistry: apulistech/
   dockers:
     hub: apulistech/
    
   cloud_config:
     dev_network:
       source_addresses_prefixes: [ "66.114.136.16/29", "73.140.21.119/32"]
   
   
   cloud_config:
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
       - 192.168.0.0/16
     nfs_ssh:
       port: 22
     tcp_port_for_pods: 30000-49999
     tcp_port_ranges: 80 443 30000-49999 25826 3000 22222 9091 9092
     udp_port_ranges: '25826'
     vnet_range: 192.168.0.0/16
     
   cloud_elasticsearch_node: apulis-sz-dev-infra01.sigsus.cn
   cloud_elasticsearch_port: '9200'
   
   cloud_influxdb_port: '8086'
   cloud_influxdb_tp_port: '25826'
   
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
   dltsdata-storage-mount-path: /dltsdata
   dns_server:
     azure_cluster: 8.8.8.8
     onpremise: 10.50.10.50
     
   Authentications:
     Microsoft:
       TenantId: 19441c6a-f224-41c8-ac36-82464c2d9b13
       ClientId: 487f34da-74af-4c0d-85d9-d678a118d99d
       ClientSecret: "1MZ[7?g0vPv_6cahAvPuohwuQKrrJEh."
   
     DingTalk:
       AppId: dingoap3bz8cizte9xu62e
       AppSecret: sipRMeNixpgWQOw-sI6TFS5vdvtXozY3y75ik_Zue2KGywfSBBwV7er_8yp-7vaj
   
   supported_platform:  ["onpremise"]
   onpremise_cluster:
     worker_node_num:    1
     gpu_count_per_node: 1
     gpu_type:           nvidia
    
   
   mountpoints:
     nfsshare1:
       type: nfs
       server: apulis-sz-dev-worker01
       filesharename: /dev/share
       curphysicalmountpoint: /mntdlws
       mountpoints: ""
   ```

   

2. 初始化部署环境

   ```
   ./deploy.py --verbose -y build 
   ```

   

3. 配置集群节点ROOT用户密码

   将集群节点ROOT密码设置一致，然后执行指令：

   ```
   cd deploy/sshkey
   echo "root" > "rootuser"
   echo "your_root_password" > "rootpasswd"
   ```

   

4. 安装SSH Key到所有集群节点

   ```
   ./deploy.py sshkey install
   ```

   

5. 检查集群节点是否可正常访问

   ```
   ./deploy.py execonall sudo ls -al
   ```

   

6. 设置集群节点的安装环境

   ```
   ./deploy.py --verbose runscriptonall ./scripts/prepare_ubuntu.sh
   ./deploy.py --verbose runscriptonall ./scripts/prepare_ubuntu.sh continue
   ./deploy.py --verbose execonall sudo usermod -aG docker dlwsadmin
   ```

   其中：

   dlwsadmin为操作集群机器所采用的用户名，配置于config.yaml

   

7. Worker机器状态确认

   ```
   nvidia-docker run --rm dlws/cuda nvidia-smi
   docker run --rm -ti dlws/cuda nvidia-smi
   保证以上两条指令均能够正常输出，才表明nvidia驱动与nvidia-docker均已正常安装
   
   如nvidia-docker指令执行正常，但docker指令执行错误，则修改/etc/docker/daemon.json，
   将nvidia-docker设置为default runtime
   ```

   

8. 安装K8S集群平台

   安装集群基础软件

   ```
   ./deploy.py execonall docker pull dlws/pause-amd64:3.0
   ./deploy.py execonall docker tag  dlws/pause-amd64:3.0 gcr.io/google_containers/pause-amd64:3.0
    
   ./deploy.py --verbose -y deploy
   ```

   设置集群节点标签

   ```
   ./deploy.py --verbose -y updateworker
   ./deploy.py --verbose -y kubernetes labels
   ```

   

9. 挂载数据共享文件夹

   ```
   ./deploy.py --verbose mount
   ```

   

10. 部署NVidia插件

    ```
    ./deploy.py --verbose kubectl create -f https://raw.githubusercontent.com/NVIDIA/k8s-device-plugin/v1.9/nvidia-device-plugin.yml
    ```

    

11. 设置Dashboard服务配置文件

    文件：src/dashboard/config/local.yaml

    内容：

    ```
    sign: "Sign key for JWT"
    winbind: "Will call /domaininfo/GetUserId with userName query to get the user's id info"
    masterToken: "Access token of all users"
    
    AddGroupLink: http://add-group/
    WikiLink: http://wiki/
    
    activeDirectory:
      tenant: "19441c6a-f224-41c8-ac36-82464c2d9b13"
      clientId: "6d93837b-d8ce-48b9-868a-39a9d843dc57"
      clientSecret: "eIHVKiG2TlYa387tssMSj?E?qVGvJi[]"
    
    dingtalk:
      appId: "dingoap3bz8cizte9xu62e"
      appSecret: "sipRMeNixpgWQOw-sI6TFS5vdvtXozY3y75ik_Zue2KGywfSBBwV7er_8yp-7vaj"
    
    domain: "https://apulis-sz-dev-infra01.sigsus.cn"
    casUrl: "https://apulis-sz-dev-infra01.sigsus.cn"
    
    administrators:
      - jinlmsft@hotmail.com
      - jinli.ccs@gmail.com
      - jin.li@apulis.com
      - bifeng.peng@apulis.com
    
    clusters:
      apulis-dev:
        restfulapi: "https://apulis-sz-dev-infra01.sigsus.cn/apis"
        title: Grafana-endpoint-of-the-cluster
        workStorage: work
        dataStorage: data
        grafana: "https://apulis-sz-dev-infra01.sigsus.cn/grafana/"
    ```

    

12. 部署集群应用

    生成dashboard, jobmanager等服务的配置文件

    ```
    ./deploy.py --verbose webui         
    ```

    编译restfulapi和webui3服务

    ```
    ./deploy.py --verbose docker push restfulapi
    ./deploy.py --verbose docker push webui3
    ```

    编译GPU Reporter

    ```
    cd deploy/docker-images/gpu-reporter
    docker build -t apulistech/apulis-sz-dev_gpu-reporter .
    docker push apulistech/apulis-sz-dev_gpu-reporter
    ```

    配置Nginx

    ```
    ./deploy.py --verbose nginx fqdn
    ./deploy.py --verbose nginx config
    ```

    启动集群应用

    ```
    ./deploy.py --verbose kubernetes start mysql jobmanager restfulapi monitor nginx custommetrics
    ./deploy.py --verbose kubernetes start cloudmonitor
    ```

    启动dashboard

    ```
    ./deploy.py --verbose nginx webui3
    ./deploy.py --verbose kubernetes start webui3
    ```

    