Known Issues List
============================================================================

* Bug：[217](https://apulis.zentaopm.com/bug-view-217.html) 【arm】tf：直接启动docker容器和通过平台提交job运行，都会概率性出现“hdc device id error”
* Bug：[317](https://apulis.zentaopm.com/bug-view-317.html) 【atlas01】【高频偶现】停止运行中的npu分布式job后，再新提交分布式job，容易出现unavailable internal errors
* Bug：[330](https://apulis.zentaopm.com/bug-view-217.html) 【atlas01】提交分布式的tf训练任务，训练结束后暂停job再恢复job，提示“job config file does not exist”


---

|Bug编号 |所属产品|所属模块|所属项目|Bug标题|重现步骤|Bug状态|截止日期|由谁创建|创建日期|影响版本|指派给|指派日期|最后修改者|修改日期|附件|
|:------|:-------|:------|:------|:------|:-----|:--------|:-----|:------|:------|:------|:------|:------|:------|:--------|:------|:---|
330 | DL平台 | /View and Manage Jobs | Davinci项目 |【atlas01】提交分布式的tf训练任务，训练结束后暂停job再恢复job，提示“job config file does not exist”|[是否必现]<br>共测试3次，必现<br>[步骤]<br>1、提交tf的分布式训练任务job<br>2、训练任务结束后，暂停job，再恢复job <br>[结果]<br>1、脚本运行报错，提示“job config file does not exist”；<br>报错日志见附件<br><img src=""{253.png}"" alt="""" />|激活|2020-06-19|褚云霞|2020-06-12|release-0.0.5-beta|刘娟|2020-06-15|彭碧峰|2020-06-15|tf-error.txt|
317|DL平台(#3)|/View and Manage Jobs(#8)|Davinci项目(#5)|【atlas01】【高频偶现】停止运行中的npu分布式job后，再新提交分布式job，容易出现unavailable internal errors|[是否必现]<br>高频偶现<br>[步骤]<br>1、当前有tf-分布式job在运行<br>2、停止该job<br>3、短时间内（小于1分半）重新提交tf-分布式job<br>[结果]<br>1、新提交的job，高概率出现运行时报错；附图如下，日志文件见附件<br>2、gpu job相隔时间较短提交job，无该问题<br><img src=""{237.png}"" alt="""" /><br>[期望]<br>1、训练任务均可正常运行|激活|2020-06-19|褚云霞|2020-06-12|DLWorkspace/tree/poc_distributed_job(#19)|刘娟|2020-06-12|彭碧峰|2020-06-12|tf-error.txt|
217 |DL平台 |/View and Manage Jobs(#8)|Davinci项目(#5)|【arm】tf：直接启动docker容器和通过平台提交job运行，都会概率性出现“hdc device id error”|[步骤]<br>1、直接在服务器上启动docker容器（非特权模式），进容器内执行训练任务的脚本，共测试10次<br>2、通过平台提交job，设置占用的NPU数量为4，共测试10次<br>3、通过平台提交job，设置占用的NPU数量为8，共测试10次<br>4、通过平台提交job，设置为两个4npu job的并发，共测试10次，即共有20个job<br>[结果]<br>1、10次中有2次出现hdc device id error，概率为20%<br>2、10次中有1次出现hdc device id error，概率为10%<br>3、10次中有1次出现hdc device id error，概率为10%<br>4、20次中有3次出现hdc device id error，概率为15%<br>直接启动docker容器时的报错为<br><img src=""{148.png}"" alt="""" /><br>通过平台提交job运行时的报错为<br><img src=""{149.png}"" alt="""" /><br>[期望]<br>1、无该报错，tf的训练任务可正常运行直至结束"|激活|2020-06-11|褚云霞|2020-06-04|DLWorkspace/tree/release-0.0.3-beta(#13)|刘娟|2020-06-12|刘娟|2020-06-16||
