const jobStatus = {
    unapproved:'未批准',
    'queued' :'队列中',
    'scheduling' :'日程中',
    'running'  :'运行中',
    'finished' :'已完成',
    'failed':'已失败',
    'pausing':'暂停中',
    'paused':'已暂停',
    'killing' :'关闭中',
    'killed':'已关闭',
    'error':'错误',
    'started at ':'开始于： ',
    'error at ':'发生错误于： ',
    'paused at ':'停止于：',
    'failed at ':'失败于： ',
    'finished at ':'完成于： ',
    'killed at ':'终止于： '
}
export default{
    ...jobStatus
}