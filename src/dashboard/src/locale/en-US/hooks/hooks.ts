const userActions = {
    sendEmailForSupport: 'Send email for support',
    approve: 'Approve',
    pause: 'Pause',
    resume:'Resume',
    kill:'Kill'
}
const useConfirm = {
    no:'NO',
    yes:'YES',
}
export default {
    ...userActions,
    ...useConfirm
}