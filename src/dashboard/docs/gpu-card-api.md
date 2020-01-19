## 当前 VC 信息

路径：/api/teams/{teamId}/clusters/{clusterId}

方法：GET

数据格式：

```javascript
{
    "AvaliableJobNum": 0,
    "gpu_avaliable": {
        "nvidia": 0
    },
    "gpu_capacity": {
        "nvidia": 1
    },
    "gpu_preemptable_used": {},
    "gpu_unschedulable": {
        "nvidia": 1
    },
    "gpu_used": {
        "nvidia": 0
    },
    "metadata": "{\"nvidia\":{\"num_gpu_per_node\":1}}",
    "node_status": [{
        "InternalIP": "192.168.0.63",
        "gpuType": "",
        "gpu_allocatable": {},
        "gpu_capacity": {},
        "gpu_preemptable_used": {
            "": 0
        },
        "gpu_used": {
            "": 0
        },
        "labels": {
            "FragmentGPUJob": "active",
            "alert-templates": "active",
            "all": "active",
            "beta.kubernetes.io/arch": "amd64",
            "beta.kubernetes.io/os": "linux",
            "blobfuse-flexvol-installer": "active",
            "cloud-collectd-node-agent": "active",
            "cloud-fluentd-es-config-v0.1.0": "active",
            "cloud-fluentd-es-v2.0.2": "active",
            "datanode": "active",
            "default": "active",
            "detectron": "active",
            "dlws-scripts": "active",
            "elasticsearch-logging": "active",
            "fluentd-es-config-v0.1.0": "active",
            "fluentd-es-v2.0.2": "active",
            "freeflowrouter": "active",
            "glusterfs": "active",
            "google-cadvisor": "active",
            "grafana-configuration": "active",
            "hdfs": "active",
            "hdfsdatanode": "active",
            "hdfsformat": "active",
            "hdfsjournal": "active",
            "hdfsnn1": "active",
            "hdfsnn2": "active",
            "hdfsstandby": "active",
            "job-exporter": "active",
            "kubernetes.io/arch": "amd64",
            "kubernetes.io/hostname": "apulis-sz-dev-worker01",
            "kubernetes.io/os": "linux",
            "nginx": "active",
            "node-exporter": "active",
            "nvidia-device-plugin-daemonset": "active",
            "nvidiaheartbeat": "active",
            "prometheus-alert": "active",
            "recogserver": "active",
            "sparknode": "active",
            "webui2": "active",
            "webui3": "active",
            "worker": "active",
            "yarnnodemanager": "active",
            "yarnrm1": "active",
            "yarnrm2": "active",
            "zk": "active",
            "zk-config": "active",
            "zk-headless": "active"
        },
        "name": "apulis-sz-dev-worker01",
        "pods": ["nginx-pz5m8 (gpu #:0)", "prometheus-metrics-prometheus-0 (gpu #:0) (gpu #:0)", "prometheus-operator-58bc8b9958-jqt2c (gpu #:0)"],
        "scheduled_service": ["nvidiaheartbeat", "cloud-fluentd-es-v2.0.2", "zk", "hdfsjournal", "glusterfs", "hdfsstandby", "yarnnodemanager", "fluentd-es-v2.0.2", "webui2", "webui3", "hdfs", "nvidia-device-plugin-daemonset", "cloud-fluentd-es-config-v0.1.0", "hdfsnn1", "hdfsnn2", "datanode", "recogserver", "google-cadvisor", "fluentd-es-config-v0.1.0", "sparknode", "grafana-configuration", "hdfsformat", "prometheus-alert", "zk-headless", "zk-config", "worker", "detectron", "nginx", "alert-templates", "job-exporter", "blobfuse-flexvol-installer", "FragmentGPUJob", "freeflowrouter", "yarnrm1", "yarnrm2", "hdfsdatanode", "elasticsearch-logging", "node-exporter", "dlws-scripts", "cloud-collectd-node-agent"],
        "unschedulable": false
    }, {
        "InternalIP": "192.168.0.195",
        "gpuType": "",
        "gpu_allocatable": {},
        "gpu_capacity": {},
        "gpu_preemptable_used": {
            "": 0
        },
        "gpu_used": {
            "": 0
        },
        "labels": {
            "FragmentGPUJob": "active",
            "alert-manager": "active",
            "alert-templates": "active",
            "all": "active",
            "beta.kubernetes.io/arch": "amd64",
            "beta.kubernetes.io/os": "linux",
            "blobfuse-flexvol-installer": "active",
            "cloud-collectd-node-agent": "active",
            "cloud-fluentd-es-config-v0.1.0": "active",
            "cloud-fluentd-es-v2.0.2": "active",
            "datanode": "active",
            "default": "active",
            "detectron": "active",
            "dlws-scripts": "active",
            "elasticsearch": "active",
            "elasticsearch-logging": "active",
            "fluentd-es-config-v0.1.0": "active",
            "fluentd-es-v2.0.2": "active",
            "freeflowrouter": "active",
            "google-cadvisor": "active",
            "grafana": "active",
            "grafana-configuration": "active",
            "hdfsdatanode": "active",
            "hdfsformat": "active",
            "hdfsjournal": "active",
            "hdfsnn1": "active",
            "hdfsnn2": "active",
            "hdfsstandby": "active",
            "infrastructure": "active",
            "job-exporter": "active",
            "jobmanager": "active",
            "journalnode": "active",
            "kibana": "active",
            "kubernetes.io/arch": "amd64",
            "kubernetes.io/hostname": "apulis-sz-dev-infra01",
            "kubernetes.io/os": "linux",
            "mysql": "active",
            "namenode1": "active",
            "nginx": "active",
            "node-exporter": "active",
            "nvidia-device-plugin-daemonset": "active",
            "nvidiaheartbeat": "active",
            "prometheus": "active",
            "prometheus-alert": "active",
            "recogserver": "active",
            "repairmanager": "active",
            "restfulapi": "active",
            "sparknode": "active",
            "watchdog": "active",
            "webportal": "active",
            "webui2": "active",
            "webui3": "active",
            "yarnnodemanager": "active",
            "yarnrm1": "active",
            "yarnrm2": "active",
            "zk": "active",
            "zk-config": "active",
            "zk-headless": "active",
            "zookeeper": "active"
        },
        "name": "apulis-sz-dev-infra01",
        "pods": ["jobmanager-fk9tc (gpu #:0) (gpu #:0)", "nginx-728w9 (gpu #:0)", "restfulapi-8h2rb (gpu #:0)"],
        "scheduled_service": ["cloud-fluentd-es-v2.0.2", "nvidiaheartbeat", "webui3", "zk", "hdfsjournal", "mysql", "kibana", "hdfsstandby", "yarnnodemanager", "fluentd-es-v2.0.2", "watchdog", "webportal", "prometheus-alert", "nvidia-device-plugin-daemonset", "webui2", "cloud-fluentd-es-config-v0.1.0", "grafana", "hdfsnn1", "hdfsnn2", "datanode", "recogserver", "elasticsearch", "cloud-collectd-node-agent", "fluentd-es-config-v0.1.0", "sparknode", "grafana-configuration", "hdfsformat", "infrastructure", "journalnode", "zk-headless", "zk-config", "namenode1", "zookeeper", "detectron", "nginx", "prometheus", "alert-templates", "job-exporter", "blobfuse-flexvol-installer", "restfulapi", "FragmentGPUJob", "freeflowrouter", "jobmanager", "yarnrm1", "yarnrm2", "dlws-scripts", "hdfsdatanode", "elasticsearch-logging", "alert-manager", "repairmanager", "node-exporter", "google-cadvisor"],
        "unschedulable": false
    }],
    "quota": "{\"nvidia\":1}",
    "user_status": [],
    "user_status_preemptable": [],
    "vcName": "platform"
}
```

后端接口地址：

/GetVC?userName={userName}&vcName={vcName}



## 相关配置信息

路径: /api/clusters/{clusterId}

方法：GET

数据结构示例

```javascript
{
    "restfulapi": "https://apulis-sz-dev-infra01.sigsus.cn/apis",
    "title": "Grafana-endpoint-of-the-cluster",
    "workStorage": "work",
    "dataStorage": "data",
    "grafana": "https://apulis-sz-dev-infra01.sigsus.cn/grafana/",
    "prometheus": "http://apulis-sz-dev-infra01.sigsus.cn:9091"
}
```

这是直接从 dashboard/config/local.yaml 中读取的配置

## 获取存储相关数据

路径：

1. {prometheus}/prometheus/api/v1/query?query=node_filesystem_avail_bytes%7Bfstype%3D%27nfs4%27%7D

2. {prometheus}/prometheus/api/v1/query?query=node_filesystem_size_bytes%7Bfstype%3D%27nfs4%27%7D

这里是 prometheus 是上面 **相关配置信息** 返回的 prometheus