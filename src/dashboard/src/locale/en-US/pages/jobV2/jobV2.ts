const index = {
    brief:'Brief',
    endpoints:'Endpoints',
    metrics:'Metrics',
    console:'Console',
    failedToFetchClusterConfig:'Failed to fetch cluster config:',
    failedToFetchJob:'Failed to fetch job:',
}
const endpoints = {
    newInteractivePort:'New Interactive Port',
    tensorBoardWillListenOnDirectory:'TensorBoard will listen on directory',
    insideDockerContainer:'inside docker container.',
    enabled:'enabled',
    enabling:'Enabling',
    failedToEnable:'Failed to enable',
    jobIs:'Job is',
    now:'now.',
    interactivePortIsRequired:'Interactive Port is required！',
    port:'Port',
    exposed:'exposed',
    failedToExposePort:'Failed to expose port',
    exposingPort:'Exposing port',
    alreadyHasPort:'Already has port',
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
const console = {
    becauseThisJobHasNotStarted:'Because this job has not started, there is no logs'
}
export default{
    ...index,
    ...endpoints,
    ...brief,
    ...console,
}