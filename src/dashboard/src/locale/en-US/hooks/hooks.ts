const userActions = {
    sendEmailForSupport: 'Send email for support',
    approve: 'Approve',
    pause: 'Pause',
    resume:'Resume',
    kill:'Kill',
    isBeingApproved:'is being approved.',
    approveRequestIsAccepted:'\'s approve request is accepted.',
    isFailedToApprove:' is failed to approve.',
    isBeingResumed:' is being resumed.',
    resumeRequestIsAccepted:'\'s resume request is accepted.',
    isFailedToResume:' is failed to resume.',
    isBeingKilled:' is being killed.',
    killRequestIsAccepted:'\'s kill request is accepted.',
    isFailedToKill:' is failed to kill.',
}
const useConfirm = {
    no:'NO',
    yes:'YES',
}
export default {
    ...userActions,
    ...useConfirm
}