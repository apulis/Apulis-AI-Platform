const index = {
    brief:'简介',
    endpoints:'终端',
    metrics:'指标',
    console:'控制台'
}
const endpoints = {
    newInteractivePort:'新建交互端口',
    tensorBoardWillListenOnDirectory:'TensorBoard将监听docker容器内部中的目录地址：',
    insideDockerContainer:'',
    enabled:'启动成功',
    enabling:'启动中',
    failedToEnable:'启动失败',
    jobIs:'任务正在',
    now:'',
    interactivePortIsRequired:'请输入交互式端口',
}
const brief = {
    jobId:'任务 Id',
    jobName:'任务名称',
    vcName:'虚拟集群名称',
    dockerImage:'Docker 镜像',
    command:'指令',
    dataPath:'数据路径',
    workPath:'工作路径',
    jobPath:'任务路径',
    preemptible:'可抢占',
    deviceType:'设备类型',
    numberOfDevice:'设备数量',
    jobStatus:'任务状态',
    jobSubmissionTime:'任务提交时间'
}
const console = {
    becauseThisJobHasNotStarted:'任务未开始，没有日志',
}
export default{
    ...index,
    ...endpoints,
    ...brief,
    ...console,
}