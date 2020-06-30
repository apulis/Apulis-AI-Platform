const copyableTextListItem = {
    'copyiedToClipboard':'复制文本成功',
    'failedToCopyText':'复制文本失败',
}
const jobStatus = {
    unapproved:'未批准',
    'queued' :'队列中',
    'scheduling' :'调度中',
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
    'killed at ':'终止于： ',
    'toUse':'可用',
}
export default{
    ...jobStatus,
    ...copyableTextListItem
} 