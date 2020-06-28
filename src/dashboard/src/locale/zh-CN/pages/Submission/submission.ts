const index = {
    submitTrainingJob:'提交训练任务',
    cluster:'集群',
    jobName:'任务名',
    jobNameIsRequired:'请输入任务名!',
    jobTemplate:'任务模板',
    jobType:'任务类型',
    preemptibleJob:'抢占式任务',
    deviceType:'设备类型',
    deviceNumber:'设备数量',
    dockerImage:'Docker 镜像',
    command:'指令',
    interactivePorts:'交互端口',
    advanced:'高级',
    template:'模板',
    submit:'提交'
}
const advance = {
    customDockerRegistry:'自定义Docker Registry',
    username:'用户名',
    password:'密码',
    mountDirectories:'挂载目录',
    pathInContainer:'容器路径',
    pathOnHostMachineOrStorageServer:'主机/存储服务器中的路径',
    enable:'启用',
    workPath:'工作路径',
    dataPath:'数据路径',
    jobPath:'任务路径',
    environmentVariables:'环境变量',
    name:'名称',
    value:'变量值',
    environmentVariableName:'环境变量名称',
    environmentVariableValue:'环境变量值',
    jobNameIsRequired:'请输入任务名',
    dockerImageIsRequired:'请输入docker镜像',
    commandIsRequired:'请输入命令',
    tensorboardListenTips:'在docker容器中，tensorboard将监听的目录：~/tensorboard/<JobId>/logs'
}
const template = {
    templateManagements:'模板管理',
    templateName:'模板名称',
    scope:'作用域',
    save:'保存',
    delete:'删除'
}
export default{
   ...index,
    ...advance,
    ...template
}