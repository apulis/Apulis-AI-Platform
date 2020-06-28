const jobStatus = {
    unapproved:'Unapproved',
    'queued' :'Queued',
    'scheduling' :'Scheduling',
    'running'  :'Running',
    'finished' :'Finished',
    'failed':'Failed',
    'pausing':'Pausing',
    'paused':'Paused',
    'killing' :'Killing',
    'killed':'Killed',
    'error':'Error'

}
export default{
    ...jobStatus
}