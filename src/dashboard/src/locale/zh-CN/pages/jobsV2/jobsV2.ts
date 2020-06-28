import MyJobs from "../../../../pages/JobsV2/MyJobs"

const index = {
    chooseCluster:'选择集群',
    myJobs:'我的任务',
    allJobs:'所有任务'
}
const myJob = {
    id:'Id',
    name:'名称',
    status:'状态',
    deviceNumber:'设备数量',
    preemptible:'是否可抢占',
    priority:'优先级',
    submitted:'提交时间',
    started:'开始时间',
    finished:'结束时间',
    actions:'操作',
    noRecordstoDisplay:'没有记录',
    failed:'已失败',
    running:'运行中',
    killed:'被停止',
    queued:'队列中',
    error:'错误'
}
const allJob = {
    id:'Id',
    name:'名称',
    status:'状态',
    deviceNumber:'设备数量',
    preemptible:'是否可抢占',
    priority:'优先级',
    submitted:'提交时间',
    started:'开始时间',
    finished:'结束时间',
    actions:'操作',
    noRecordstoDisplay:'没有记录',
    failed:'已失败',
    running:'运行中',
    killed:'被停止',
    queued:'队列中',
    error:'错误',
    runningJob:'运行中任务',
    queuingJobs:'队列中任务',
    unapprovedJobs:'无效任务',
    pauseesJobs:'暂停任务'
}
const materialTable = {
    firstPage:'第一页',
    previousPage:'前一页',
    nextPage:'后一页',
    lastPage:'末尾页',
    search:'搜索',
    actions:'操作',
    noRecordsToDisplay:'没有数据'
}
const brief = {
    brief:'简介',
    endpoints:'终端',
    metrics:'指标',
    console:'控制台',
    jobId:'任务 Id',
    jobName:'任务名称',
    vcName:'虚拟集群名称',
    dockerImage:'Docker 镜像',
    command:'指令',
    dataPath:'数据路径',
    workPath:'工作路径',
    jobPath:'任务路径',
    deviceType:'设备类型',
    jobStatus:'任务状态',
    jobSubmissionTime:'任务提交时间'
}
const endpoints = {
    newInteractivePort:'新建交互端口',
    sendEmailForSupport:'发送邮件获取支持'
}
export default{
    ...index,
    ...myJob,
    ...allJob,
    ...materialTable,
    ...brief,
    ...endpoints
}