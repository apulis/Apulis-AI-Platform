Known Issues List
============================================================================

* [217](#jump)【arm】tf：直接启动docker容器和通过平台提交job运行，都会概率性出现“hdc device id error”
* [317](#jump)【atlas01】【高频偶现】停止运行中的npu分布式job后，再新提交分布式job，容易出现unavailable internal errors
* [330](#jump)【atlas01】提交分布式的tf训练任务，训练结束后暂停job再恢复job，提示“job config file does not exist”


---
* Bug编号: <span id="jump">330 </span>

    + 所属模块: Submit Traing Job
    + Bug标题: 【atlas01】提交分布式的tf训练任务，训练结束后暂停job再恢复job，提示“job config file does not exist”
    + 重现步骤: 
        - [是否必现] 测试共3次，都出现
        - [步骤]
            1. 提交tf的分布式训练任务job
            2. 训练任务结束后，暂停job，再恢复job 
        - [结果]
            1. 脚本运行报错，提示“job config file does not exist”；<br>
                报错日志见[附件01](resource/330_tf_error_01.log),[附件02](resource/330_tf_error_02.log), [附件03](resource/330_tf_error_file-read-253.png)       
    + Bug状态: 激活
    + 由谁创建: 褚云霞
    + 创建日期: 2020-06-12
    + 影响版本: release-0.0.5-beta
    + 指派给: 刘娟
    + 指派日期: 2020-06-12
    + 最后修改者: 彭碧峰
    + 修改日期: 2020-06-15


* Bug编号: <span id="jump">317</span>

    + 所属模块: Submit Traing Job
    + Bug标题: 【atlas01】【高频偶现】停止运行中的npu分布式job后，再新提交分布式job，容易出现unavailable internal errors
    + 重现步骤: 
        - [是否必现] 高频出现
        - [步骤]
            1. 当前有tf-分布式job在运行
            2. 停止该job
            3. 短时间内（小于1分半）重新提交tf-分布式job
        - [结果]
            1. 新提交的job，高概率出现运行时报错；请查看[附图](resource/317_tf_error.png)，日志文件见[附件](resource/317_tf_error.log)
            2. gpu job相隔时间较短提交job，无该问题     
    + Bug状态: 激活
    + 由谁创建: 褚云霞
    + 创建日期: 2020-06-12
    + 影响版本: release-0.0.5-beta
    + 指派给: 刘娟
    + 指派日期: 2020-06-12
    + 最后修改者: 彭碧峰
    + 修改日期: 2020-06-12


* Bug编号: <span id="jump">217</span>

    + 所属模块: Submit Traing Job
    + Bug标题: 【arm】tf：直接启动docker容器和通过平台提交job运行，都会概率性出现“hdc device id error”
    + 重现步骤: 
        - [是否必现] 高频出现
        - [步骤]
            1. 直接在服务器上启动docker容器（非特权模式），进容器内执行训练任务的脚本，共测试10次
            2. 通过平台提交job，设置占用的NPU数量为4，共测试10次
            3. 通过平台提交job，设置占用的NPU数量为8，共测试10次
            4. 通过平台提交job，设置为两个4npu job的并发，共测试10次，即共有20个job
        - [结果]
            1. 10次中有2次出现hdc device id error，概率为20%
            2. 10次中有1次出现hdc device id error，概率为10%
            3. 10次中有1次出现hdc device id error，概率为10%
            4. 20次中有3次出现hdc device id error，概率为15%<br>
                [直接启动docker容器时的报错](resource/217_tf_error01.png)<br>
                [通过平台提交job运行时的报错](resource/217_tf_error02.png)<br>
    + Bug状态: 激活
    + 由谁创建: 褚云霞
    + 创建日期: 2020-06-04
    + 影响版本: release-0.0.5-beta
    + 指派给: 刘娟
    + 指派日期: 2020-06-12
    + 最后修改者: 彭碧峰
    + 修改日期: 2020-06-16

