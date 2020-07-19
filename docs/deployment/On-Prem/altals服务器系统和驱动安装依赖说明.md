Atlas 服务器系统和驱动安装说明
================================================================================

整体更新主要参考《Ascend 910 驱动和开发环境安装指南.chm》

Apulis Platform 安装清单：
--------------------------------------------------------------------------------

1. [ubuntu 18.04.4](http://cdimage.ubuntu.com/ubuntu/releases/18.04/release/)


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


