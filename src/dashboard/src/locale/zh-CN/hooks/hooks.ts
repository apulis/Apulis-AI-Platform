const userActions = {
    sendEmailForSupport: '发送邮件寻求支持',
    approve: '批准',
    pause: '暂停',
    resume:'继续',
    kill:'终止',
    isBeingApproved:'正被批准',
    approveRequestIsAccepted:'的批准请求已接受',
    isFailedToApprove:'批准失败',
    isBeingResumed:'正在恢复',
    resumeRequestIsAccepted:'的恢复请求已接受',
    isFailedToResume:'恢复失败',
    isBeingKilled:'正在中断',
    killRequestIsAccepted:'的中断请求已接受',
    isFailedToKill:'中断失败',
}
const useConfirm = {
    no:'取消',
    yes:'确认',
}
export default {
    ...userActions,
    ...useConfirm,
}