const copyableTextListItem = {
    'copyiedToClipboard':'Copied to clipboard',
    'failedToCopyText':'Failed to copy text',
}
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
    'killed at ':'killed at ',
    'toUse':'to use',
    'waiting for available resource. requested: ':'waiting for available resource. requested: ',
    '. available: ':'. available: ',
}
export default{
    ...jobStatus,
    ...copyableTextListItem,
}