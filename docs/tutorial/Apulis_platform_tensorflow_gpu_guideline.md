# 依瞳人工智能开放平台训练任务指南

## 英伟达GPU训练任务

### 系统环境要求

* CUDA 10.1
* Horovod 1.19.2
* TensorFlow 2.2+
* 服务器之间需有 InfiniBand 通讯

* 以上环境包含在镜像 `apulistech/horovod:0.19.2-tf2.2.0-torch1.5.0-py3.6.9-gpu`中


### 在平台中创建基于Horovod的多机多GPU分布式训练示例


* 训练代码：基于TensorFlow2/Keras的Horovod分布式训练示例

* 数据集：MNIST数据集

* 硬件环境:两台英伟达GPU服务器，每台上各有8颗NVIDIA GPU。

#### 具体步骤如下：

1. 登录进平台，选择“Submit Training Job”以进入提交训练任务页面（如图1）；

<img src="img/job_submission.png" style="zoom:34%;" />

2. 输入Job Name，例如“`horovod`”；
3. 选择Job Type为“`Distributed Job`”；
4. 选择Preemptible Job为“`NO`“；
5. 选择Device Type为“`Nvidia`”；
6. 选择Number of Nodes为“2”；
7. 在Docker Image中输入“`apulistech/horovod:0.19.2-tf2.2.0-torch1.5.0-py3.6.9-gpu`”；此镜像中已经安装Horovod 1.9.2、TensorFlow 2.2，以及示例代码已经内置于`/examples`目录中。
8. 在`Command`中输入如下指令：

    ```shell
    cd ~ && mkdir -p ~/checkpoints && horovodrun -np 16 -hostfile /home/${DLWS_USER_NAME}/ib_config -p $DLWS_SD_worker0_SSH_PORT --network-interface ib0 python /examples/tensorflow2_keras_mnist.py ~/checkpoints/ ~/tensorboard/${DLWS_JOB_ID}/logs/ && sleep infinity
    ```
其中 `/home/${DLWS_USER_NAME}/ib_config` 这个文件为平台生成，内容包括worker节点的hostname或IP，以及worker节点的GPU数量。

9. 勾选SSH、Jupyter、TensorBoard后，点击“SUBMIT”
10. 在View and Manage Jobs里打开刚刚创建的Job，等待Job的状态改变为“Running”
11. 选择`ENDPOINTS`选项卡；
12. 点击iPython里的链接，可以打开Jupyter Lab查看和编辑代码文件（如图2）。在Jupyter Lab左侧的目录列表中可以定位到模型目录~/checkpoints/，这里保存了多个模型文件，名字的数字代表训练时的epoch数；

<img src="img/jupyter_editor.png" style="zoom:25.5%;" />

13. 点击TensorBoard里的链接，可以查看这个训练任务的训练结果（如图3）；

<img src="img/tensorboard.png" style="zoom:40%;" />

14. 根据SSH中的信息，可以使用SSH Client登录此任务所在的容器；

#### 基于Horovod的单机多GPU分布式训练
单机多GPU和和上面多机多GPU训练的流程基本一致。不同之处如下：

* 选择Number of Nodes为“1”；

* 修改command，调整GPU的个数为8个。

    ```shell
    cd ~ && mkdir -p ~/checkpoints && horovodrun -np 8 -hostfile /home/${DLWS_USER_NAME}/ib_config -p $DLWS_SD_worker0_SSH_PORT --network-interface ib0 python /examples/tensorflow2_keras_mnist.py ~/checkpoints/ ~/tensorboard/${DLWS_JOB_ID}/logs/ && sleep infinity
    ```