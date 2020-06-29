const jobStatus = {
    'unapproved':'Unapproved',
    'queued' :'Queued',
    'scheduling' :'Scheduling',
    'running'  :'Running',
    'finished' :'Finished',
    'failed':'Failed',
    'pausing':'Pausing',
    'paused':'Paused',
    'killing' :'Killing',
    'killed':'Killed',
    'error':'Error',
    'started at ':'started at ',
    'error at ':'error at ',
    'paused at ':'paused at',
    'failed at ':'failed at ',
    'finished at ':'finished at ',
    'killed at ':'killed at '
}
export default{
    ...jobStatus
}