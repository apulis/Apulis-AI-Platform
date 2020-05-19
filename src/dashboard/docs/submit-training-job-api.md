

## 一、获取之前填写的模板

路径：/api/teams/{teamId}/templates

方法：GET

数据格式实例

```javascript
[{
    "json": "{\"workPath\": \"\", \"name\": \"testJob\", \"workers\": 0, \"jobPath\": \"\", \"dataPath\": \"\", \"environmentVariables\": [], \"enableWorkPath\": true, \"enableJobPath\": true, \"command\": \"run start.sh\", \"ssh\": true, \"tensorboard\": false, \"plugins\": {\"blobfuse\": [{\"accountKey\": \"\", \"containerName\": \"\", \"mountPath\": \"\", \"mountOptions\": \"--file-cache-timeout-in-seconds=120\", \"accountName\": \"\"}]}, \"ipython\": false, \"gpus\": 1, \"type\": \"RegularJob\", \"image\": \"ikubernetes/myapp\", \"enableDataPath\": true}",
    "name": "testTemplate"
}, {
    "json": "{\"workPath\": \"\", \"name\": \"\", \"workers\": 0, \"jobPath\": \"\", \"dataPath\": \"\", \"environmentVariables\": [], \"enableWorkPath\": true, \"enableJobPath\": true, \"command\": \"\", \"ssh\": false, \"tensorboard\": false, \"plugins\": {\"blobfuse\": [{\"accountKey\": \"\", \"containerName\": \"\", \"mountPath\": \"\", \"mountOptions\": \"--file-cache-timeout-in-seconds=120\", \"accountName\": \"\"}]}, \"ipython\": false, \"gpus\": 0, \"type\": \"RegularJob\", \"image\": \"\", \"enableDataPath\": true}",
    "name": "bb"
}]
```

后端接口地址: /templates?userName={userName}&vcName={teamId}

在获取这写数据后，可以快速以之前存储过的表格填充数据

## 二、 提交训练任务

路径：/api/clusters/{clusterId}/jobs 

方法：POST

数据结构示例：

```json
{
    "userName": "xianjie",
    "userId": 20001,
    "jobType": "training",
    "gpuType": "nvidia",
    "vcName": "platform",
    "containerUserId": 0,
    "jobName": "nn",
    "jobtrainingtype": "RegularJob",
    "preemptionAllowed": "True",
    "image": "r",
    "cmd": "echo \"1\"",
    "workPath": "./",
    "enableworkpath": true,
    "dataPath": "./",
    "enabledatapath": true,
    "jobPath": "",
    "enablejobpath": true,
    "env": [],
    "hostNetwork": false,
    "isPrivileged": false,
    "plugins": {
        "blobfuse": [{
            "accountName": "",
            "accountKey": "",
            "containerName": "",
            "mountPath": "",
            "mountOptions": "--file-cache-timeout-in-seconds=120"
        }]
    },
    "resourcegpu": 0
}
```

后端接口： /PostJob

