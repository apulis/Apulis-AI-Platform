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
    TherewontbeenoughworkersmatchyourrequestnProceed:'There won\'t be enough workers match your request.\nProceed?',
    Mustbeapositiveintegerfrom0to:'必须是一个从0到',
    andcanonlybeoneof01248:'并且只能是0、1、2、4、8中的一个',
}
export default{
    ...allJobTips,
    ...myJobsTips,
    ...priorityField,
    ...training,
    NameErrorText:'名称必须由字母、数字、下划线或水平线组成!',
    SameNameErrorText:'已经有相同的名字了!',
    NoChineseErrorText:'不能包含汉字!',
    OneInteractivePortsMsg:'必须是40000到49999之间的正整数!',
    InteractivePortsMsg:'必须是40000到49999之间的正整数!倍数可以用逗号分隔。',
    NoNumberText:'不能包含数字',
}