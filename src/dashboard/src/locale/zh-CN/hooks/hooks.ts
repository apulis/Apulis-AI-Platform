const userActions = {
    sendEmailForSupport: '发送邮件寻求支持',
    approve: '批准',
    pause: '暂停',
    resume:'继续',
    kill:'终止'
}
const useConfirm = {
    no:'取消',
    yes:'确认',
}
export default {
    ...userActions,
    ...useConfirm,
}