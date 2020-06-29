const index = {
    chooseCluster:'Choose Cluster',
    myJobs:'My Jobs',
    allJobs:'All Jobs'
}
const myJob = {
    id:'Id',
    name:'Name',
    status:'Status',
    deviceNumber:'Number of Device',
    preemptible:'Preemptible',
    priority:'Priority',
    submitted:'Submitted',
    started:'Started',
    finished:'Finished',
    actions:'Actions',
    noRecordstoDisplay:'No records to display',
    failed:'Failed',
    running:'Running',
    killed:'Killed',
    queued:'Queued',
    error:'Error'
}
const allJob = {
    id:'Id',
    name:'Name',
    status:'Status',
    deviceNumber:'Number of Device',
    user:'user',
    preemptible:'Preemptible',
    priority:'Priority',
    submitted:'Submitted',
    started:'Started',
    finished:'Finished',
    actions:'Actions',
    noRecordstoDisplay:'No records to display',
    failed:'Failed',
    running:'Running',
    killed:'Killed',
    queued:'Queued',
    error:'Error',
    runningJob:'Running Jobs',
    queueingJobs:'Queuing Jobs',
    unapprovedJobs:'Unapproved Jobs',
    pauseJobs:'Pausses Jobs',
}
const materialTable = {
    firstPage:'First Page',
    previousPage:'Previous Page',
    nextPage:'Next Page',
    lastPage:'Last Page',
    search:'Search',
    actions:'Actions',
    noRecordsToDisplay:'No records to display',
    labelRowsPerPage: 'rows',
    labelRowsSelect: 'rows'
}

const endpoints = {
    newInteractivePort:'New Interactive Port',
    sendEmailForSupport:'Send email for support'
}
export default{
    ...index,
    ...myJob,
    ...allJob,
    ...materialTable,
    ...endpoints
}