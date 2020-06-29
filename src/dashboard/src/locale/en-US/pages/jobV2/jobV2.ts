const index = {
    brief:'Brief',
    endpoints:'Endpoints',
    metrics:'Metrics',
    console:'Console'
}
const endpoints = {
    newInteractivePort:'New Interactive Port',
    tensorBoardWillListenOnDirectory:'TensorBoard will listen on directory',
    insideDockerContainer:'inside docker container.'
}
const brief = {
    jobId:'Job Id',
    jobName:'Job Name',
    vcName:'VCName',
    dockerImage:'Docker Image',
    command:'Command',
    dataPath:'Data Path',
    workPath:'Work Path',
    jobPath:'Job Path',
    preemptible:'Preemptible',
    deviceType:'Device Type',
    numberOfDevice:'Number of Device',
    jobStatus:'Job Status',
    jobSubmissionTime:'Job Submission Time'
}
export default{
    ...index,
    ...endpoints,
    ...brief
}