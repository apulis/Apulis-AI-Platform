# 多架构补充说明
* 针对异构集群，我们添加了`archtype`参数，调整service启动方案，以适用于多种场景
* 这里多架构不仅代表cpu架构的不同，也适用于业务中gpu/npu架构等
## archtype参数
* 主要用于对不同架构下的镜像进行区分，架构包括且不限于:
    - amd64
    - arm64
    - amd64-npu
    - arm64-npu
* 需要在`config.yaml`中`machines`处配置，与机器对应
* 该参数在`deploy.py`中docker build/push中使用，用于打包对应环境的docker
* amd64为默认架构，当架构为amd64时，无需设置该参数(目前)
## deploy.py
* deploy.py命令添加了`--archtype`支持
* 仅在`docker build`/`docker push`命令中需要使用
* 添加了该参数，会在`docker-image`文件夹下寻找对应的dockerfile进行打包
* amd64架构，上传到dockerhub的tag与目前情况相同，其他架构tag后会添加"-$archtype"
## Dockerfile
* `docker-images`文件夹下的dockerfile可能有多种
* 命名规则:
    - 必须有一个文件名为`Dockerfile`，默认是amd64架构的dockerfile
    - 其他dockerfile命名格式为`Dockerfile-$archtype`
* 每种archtype无需有完全与自己对应的dockerfile名称，程序会自动选取与`Dockerfile-$archtype` prefix匹配最长的dockerfile 
## services
* 异构集群中，相同名称的service可能会分离为多个service分别启动
* 分离的service不会冲突，会以nodeselector的方式，在对应node启动
* 需要分离的service，会有多个`launch_order`文件
