altals服务器系统和驱动安装指导
==========================================================================

系统安装
--------------------------------------------------------------------------

* 通过ibmc安装ubuntu步骤

- 官方说明

  https://support.huawei.com/enterprise/en/doc/EDOC1100048786/916c5c0a/mounting-a-file-to-the-virtual-cd-rom-drive-vmm--d-connect

- ibmc登录
    ```
    # 密码和用户名，与ibmc同
    ssh Administrator@192.168.1.21
    # 查询ibmc地址：
    ipmctool lan print 1
    ```
- 挂载iso镜像
    ```
    # 查询挂载情况：
    ipmcget -t vmm -d info
    # 执行挂载：
    ipmcset -t vmm -d connect -v nfs://192.168.1.3/data/nfsshare/isountu-18.04.1-server-arm64.iso
    # 执行卸载：
    ipmcset -t vmm -d disconnect
    ```
- 配置为光驱启动

    + 路径：ibmc主页  -->系统管理 --> BIOS配置

    + 配置：光驱启动，保存配置

- IBMC配置

    1. 安装JAVA RUNTIME Environment
    2. 将目标机器的IBMC 2198和8208端口转发打开（ibmc只开放同样端口），把其它机器的IBMC端口转发规则禁用
  
- ubuntu系统

    1. ibmc主页  -->虚拟控制台 --> JAVA集成远程控制台
    2. 登陆OS，执行reboot后重装ubuntu
  
- 重置系统加载顺序
  
  *将启动顺序 重新配置为 硬盘加载*
  

安装NPU驱动
--------------------------------------------------------------------------

- 拷贝共享存储中的配置文件

    1. `mkdir -p /etc/ceph`
    2. `scp root@192.168.3.2:/home/bifeng.peng/ceph-config/atlas01.tar .` 
    3. 将atlas01.tar文件解压至/etc/ceph
    4. 挂载ceph `ceph-fuse -m 192.168.2.16 /mnt/fuse`

- 安装配置

    *安装此脚本内的所有安装函数*

    `/mnt/fuse/atlas01-pbf/install-atlas/install.sh `

- 配置NPU设备IP，以atlas01为例

    ```
    hccn_tool -i 0 -ip -s address 192.168.10.1 netmask 255.255.255.0
    hccn_tool -i 1 -ip -s address 192.168.20.1 netmask 255.255.255.0
    hccn_tool -i 2 -ip -s address 192.168.30.1 netmask 255.255.255.0
    hccn_tool -i 3 -ip -s address 192.168.40.1 netmask 255.255.255.0
    hccn_tool -i 4 -ip -s address 192.168.10.2 netmask 255.255.255.0
    hccn_tool -i 5 -ip -s address 192.168.20.2 netmask 255.255.255.0
    hccn_tool -i 6 -ip -s address 192.168.30.2 netmask 255.255.255.0
    hccn_tool -i 7 -ip -s address 192.168.40.2 netmask 255.255.255.0
    ```
- 检查网口配置状态是否正确 (状态UP为正确)

  `for i in {0..7}; do hccn_tool -i ${i} -link -g; done`

- 导入共享存储的镜像

    ```
    cd /mnt/fuse/atlas01-image/
    ./image.sh load
    ```
- 检查NPU驱动状态

    `npu-smi info`




安装网卡驱动
--------------------------------------------------------------------------------

- 下载以下说明文档

  [《华为 TM210 灵活IO卡 用户指南 01.PDF》](./static/华为TM210灵活IO卡用户指南03.pdf)

- 创建开机启动脚本

    ```shell
    # vim /etc/rc.local
    #!/bin/bash
    dhclient
    ceph-fuse -m 192.168.2.16 /mnt/fuse
    exit 0
    ```

- 设置开机启动

    ```
    sudo chmod 755 /etc/rc.local
    sudo systemctl enable rc-local
    sudo systemctl restart rc-local
    sudo systemctl status rc-local
    ```


系统和驱动安装的依赖包说明
================================================================================

整体更新主要参考《Ascend 910 驱动和开发环境安装指南.chm》

Apulis Platform 安装清单：
--------------------------------------------------------------------------------

1. [ubuntu 18.04.1](http://cdimage.ubuntu.com/ubuntu/releases/18.04/release/)


2. NPU驱动（华为提供）

    ```
    ./Ascend910-driver-1.72.t2.100.b020-ubuntu18.04.aarch64.run --full
    ./Ascend910-firmware-1.72.t2.100.b020.run --full
    ./Ascend910-aicpu_kernels-1.72.t2.0.b020.run --full  --dev_reset=n
    ./Ascend-fwkacllib-1.72.t2.0.b020-ubuntu18.04.aarch64.run --full
    ./Ascend-tfplugin-1.72.t2.0.b020-ubuntu18.04.aarch64.run --full
    ./Ascend-atc-1.72.t2.0.b020-ubuntu18.04.aarch64.run --full
    ./Ascend-opp-1.72.t2.0.b020-ubuntu18.04.aarch64.run --full 
    ./Ascend-toolkit-1.72.t2.0.b020-ubuntu18.04.aarch64.run --full 
    ```

3. 网卡驱动（UbuntuOS下需要装板载网卡驱动，指导书中暂未提及）

4. tensorflow/mindspore 组件（华为指定）

    ``` 
    pip3 install /usr/local/Ascend/fwkacllib/lib64/topi-0.1.0-py3-none-any.whl
    pip3 install /usr/local/Ascend/fwkacllib/lib64/te-0.4.0-py3-none-any.whl
    pip3 install /usr/local/Ascend/fwkacllib/lib64/hccl-0.4.0-py3-none-any.whl

    # 安装mindspore
    pip3 install https://ms-release.obs.cn-north-4.myhuaweicloud.com/0.3.0-alpha/MindSpore/ascend/ubuntu_aarch64/mindspore_ascend-0.3.0-cp37-cp37m-linux_aarch64.whl

    # 安装tensorflow
    pip3 install  npu_bridge-1.15.0-py3-none-any.whl
    pip3 install  tensorflow-1.15.0-cp37-cp37m-linux_aarch64.whl
    ```

4. 其它各类依赖（解决依赖错误所做的安装）

    * ubuntu 系统工具包

        ```
        apt-get install python3.4-dev -y
        sudo apt-get install gfortran -y 
        sudo apt-get install libopenblas-dev -y
        sudo apt-get install liblapack-dev -y
        sudo apt-get install libatlas-dev -y
        sudo apt-get install libblas-dev -y
        sudo apt install python3-h5py
        sudo apt install cmake
        sudo apt install gcc
        sudo apt install nodejs
        sudo apt-get install -y libffi-dev python-dev libevent-dev
        sudo apt-get install gfortran
        sudo apt-get install libopenblas-dev
        sudo apt-get install liblapack-dev
        sudo apt-get install libatlas-dev
        sudo apt-get install libblas-dev
        sudo apt-get install haveged（亚文建议）
        ```
    * python 工具包
        ```
        numpy  ≥ 1.13.3
        decorator ≥ 4.4.0
        sympy==1.4
        cffi==1.12.3
        pyyaml
        pathlib2
        grpcio
        grpcio-tools
        protobuf
        scipy
        requests
        pybind11>=2.4.3
        wheel>=0.32.0
        ```


