const allJobTips = {
    Failedtofetchjobsfromcluster:'Failed to fetch jobs from cluster',
    OnlyRunningOrQueuingOrUnapprovedOrPausesjobswillbeshownandwillnotshowFinishedjobs:'Only Running/Queuing/Unapproved/Pauses jobs will be shown and will not show Finished jobs',
    
}
const myJobsTips = {
    'Failedtofetchjobsfromcluster':'Failed to fetch jobs from cluster'
}
const priorityField = {
   'Priorityissetsuccessfully':'Priority is set successfully',
   'Failedtosetpriority':'Failed to set priority',
   
}
const training = {
    Failedtosavethetemplate:'Failed to save the template',
    Needselectonetemplate:'Need select one template',
    Failedtodeletethetemplate:'Failed to delete the template',
    TemplateNameisrequired:'Template Name is required！',
    TherewontbeenoughworkersmatchyourrequestnProceed:'There won\'t be enough workers match your request.\nProceed?',
    Mustbeapositiveintegerfrom0to:'Must be a positive integer from 0 to',
    andcanonlybeoneof01248:'and can only be one of 0, 1, 2, 4, 8',
}
const userActionTips = {
    pauseJob:'Pause job',
    isBeingPaused:'is being paused.',
    pauseRequestAccepted:'\'s pause request is accepted.',
    isFailedToPause:'is failed to pause.',
}
export default{
    ...allJobTips,
    ...myJobsTips,
    ...priorityField,
    ...training,
    ...userActionTips,
    NameErrorText:'The Name Must be composed of letter, numbers, underscore or horizontal line！',
    SameNameErrorText:'Already has the same name!',
    NoChineseErrorText:'Cannot contain Chinese characters!',
    OneInteractivePortsMsg:'Must be a positive integer between 40000 and 49999！',
    InteractivePortsMsg:'Must be a positive integer between 40000 and 49999！Multiple can be separated by comma.',
    NoNumberText:'Cannot contain numbers'
}