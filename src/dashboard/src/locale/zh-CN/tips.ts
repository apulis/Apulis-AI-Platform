const allJobTips = {
    Failedtofetchjobsfromcluster:'从集群中获取任务失败',
    OnlyRunningOrQueuingOrUnapprovedOrPausesjobswillbeshownandwillnotshowFinishedjobs:'只显示正在运行/排队/未批准/暂停的任务，不会显示已完成的任务',
    
}
const myJobsTips = {
    'Failedtofetchjobsfromcluster':'从集群中获取任务失败'
}
const priorityField = {
   'Priorityissetsuccessfully':'优先级设置成功',
   'Failedtosetpriority':'设置优先级失败'
}
const training = {
    Failedtosavethetemplate:'保存模板失败',
    Needselectonetemplate:'需要保存一个模板',
    Failedtodeletethetemplate:'删除模板失败',
    TemplateNameisrequired:'请输入模板名称',
    TherewontbeenoughworkersmatchyourrequestnProceed:'没有足够的工作节点 \n是否继续?',
    Mustbeapositiveintegerfrom0to:'必须是一正整数，可选范围 0 -',
    andcanonlybeoneof01248:'并且只能是0、1、2、4、8中的一个',
}
const userActionTips = {
    pauseJob:'停止任务',
    resumeJob:'继续任务',
    isBeingPaused:'正在停止。',
    pauseRequestAccepted:'的停止请求已经发起',
    isFailedToPause:'停止失败',
}
const warnConstants = {
    SUCCESSFULLYAPPROVED : "批准成功",
    SUCCESSFULLYPAUSED : "暂停成功",
    SUCCESSFULLYRESUMED : "恢复成功",
    SUCCESSFULLYUPDATEDPRIORITY : "更新优先级成功",
    SUCESSFULKILLED : "中断成功",
    SUCCESSFULSUBMITTED : "任务提交成功",
    SUCCESSFULTEMPLATEDELETE : "删除模板成功",
    SUCCESSFULTEMPLATEDSAVE : "保存模板成功",
}
const global = {
    deepLearningPlatform:'Apulis 深度学习平台 ',
}
export default{
    ...allJobTips,
    ...myJobsTips,
    ...priorityField,
    ...training,
    ...global,
    ...userActionTips,
    ...warnConstants,
    NameErrorText:'名称必须由字母、数字、下划线或水平线组成!',
    SameNameErrorText:'已经有相同的名字了!',
    NoChineseErrorText:'不能包含汉字!',
    OneInteractivePortsMsg:'必须是40000到49999之间的正整数!',
    InteractivePortsMsg:'必须是40000到49999之间的正整数!多个端口之间使用逗号分隔。',
    NoNumberText:'不能包含数字',
}