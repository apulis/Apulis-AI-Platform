import os
import sys
import json
import yaml
from jinja2 import Template
from job import Job
import copy
import re

sys.path.append(os.path.join(os.path.dirname(os.path.abspath(__file__)), "../utils"))
from osUtils import mkdirsAsUser
from pod_template_utils import enable_cpu_config
import config
from DataHandler import DataHandler

import logging
import logging.config
import storage

logger = logging.getLogger(__name__)


class PodTemplate():
    def __init__(self, template, deployment_template=None, enable_custom_scheduler=False, secret_templates=None):
        self.template = template
        self.deployment_template = deployment_template
        self.enable_custom_scheduler = enable_custom_scheduler
        self.secret_templates = secret_templates

    def generate_deployment(self, pod):
        assert(isinstance(self.template, Template))
        pod_yaml = self.deployment_template.render(job=pod)
        return yaml.full_load(pod_yaml)


    def generate_pod(self, pod, cmd):
        assert(isinstance(self.template, Template))
        if self.enable_custom_scheduler:
            if "useGPUTopology" in pod and pod["useGPUTopology"]:
                gpu_topology_flag = 1
            else:
                # for cases when desired topology is explictly given or not desired
                gpu_topology_flag = 0
            pod_name = pod["podName"]
            request_gpu = int(pod["gpuLimit"])

            podInfo = {
                "podname": pod_name,
                "requests": {
                    "alpha.gpu/gpu-generate-topology": gpu_topology_flag
                },
                "runningcontainer": {
                    pod_name: {
                        "requests": {"alpha.gpu/numgpu": request_gpu}
                    },
                },
            }

            if "annotations" not in pod:
                pod["annotations"] = {}
            pod["annotations"]["pod.alpha/DeviceInformation"] = "'" + json.dumps(podInfo) + "'"
            # gpu requests specified through annotation
            pod["gpuLimit"] = 0

        pod_yaml = self.template.render(job=pod)
        # because user's cmd can be multiple lines, should add after yaml load
        pod_obj = yaml.full_load(pod_yaml)
        pod_obj["spec"]["containers"][0]["env"].append({"name": "DLWS_LAUNCH_CMD", "value": cmd})

        return pod_obj

    def generate_custom_resource(self, resource):
        assert(isinstance(self.template, Template))
        pod_yaml = self.template.render(job=resource)
        # because user's cmd can be multiple lines, should add after yaml load
        resource_obj = yaml.full_load(pod_yaml)
        return resource_obj


    def generate_pods(self, job):
        """
        Return (pods, errors)
        """

        assert(isinstance(job, Job))
        params = job.params
        if any(required_field not in params for required_field in
                [
                    "jobtrainingtype",
                    "jobName",
                    "jobPath",
                    "workPath",
                    "dataPath",
                    "cmd",
                    "userId",
                    "resourcegpu",
                    "userName",
                ]):
            return None, "Missing required parameters!"

        vc_without_shared_storage = job.get_vc_without_shared_storage()

        job.job_path = params["jobPath"]
        job.work_path = params["workPath"]
        job.data_path = params["dataPath"]

        # TODO user's mountpoints first, but should after 'job_path'
        job.add_mountpoints(job.ssh_path_mountpoints())   # added a list of mount points
        job.add_mountpoints(job.job_path_mountpoint())

        # TODO: Refactor special VC dependency
        if params["vcName"] not in vc_without_shared_storage:
            job.add_mountpoints(job.home_path_mountpoint())

        if "mountpoints" in params:
            job.add_mountpoints(params["mountpoints"])

        # TODO: Refactor special VC dependency
        if params["vcName"] not in vc_without_shared_storage:
            logger.info("job-%s to mount work path and data path" % (job.job_id))
            job.add_mountpoints(job.work_path_mountpoint())
            job.add_mountpoints(job.data_path_mountpoint())

        job.add_mountpoints(job.vc_custom_storage_mountpoints())
        job.add_mountpoints(job.vc_storage_mountpoints())
        
        # TODO: remove following log statement
        logger.info("job-%s mount points(%s)" % (job.job_id, str(job.mountpoints)))

        params["mountpoints"] = job.mountpoints
        params["mountpoints_pvc"] = job.get_pvc_mountpoints()  # pvc deduplication
        logger.info("job-%s  pvcs(%s)" % (job.job_id, str(params["mountpoints_pvc"])))

        params["user_email"] = params["userName"]
        params["homeFolderHostpath"] = job.get_homefolder_hostpath()
        params["pod_ip_range"] = job.get_pod_ip_range()
        params["usefreeflow"] = job.is_freeflow_enabled()
        params["jobNameLabel"] = ''.join(e for e in params["jobName"] if e.isalnum())
        params["rest-api"] = job.get_rest_api_url()

        if "nodeSelector" not in params:
            params["nodeSelector"] = {}

        if "gpuType" in params and params["gpuType"]:
            params["nodeSelector"]["gpuType"] = params["gpuType"]

        # Set up VC dedicated node usage
        vc_node_hard_assignment = job.get_vc_node_hard_assignment()
        if isinstance(vc_node_hard_assignment, dict):
            vc = params["vcName"]
            # TODO: Fix the case where CPU worker exists in a GPU pool
            if vc in vc_node_hard_assignment and \
                    vc_node_hard_assignment[vc] is True:
                params["nodeSelector"]["vc"] = vc
            else:
                params["nodeSelector"]["vc"] = "default"

        params = enable_cpu_config(params, job.cluster)

        if "envs" not in params:
            params["envs"] =[]

        job.add_plugins(job.get_plugins())
        params["plugins"] = job.plugins

        # Set NCCL_IB_DISABLE=1 if specified
        nccl_ib_disable = job.get_nccl_ib_disable()
        if nccl_ib_disable is not None and nccl_ib_disable is True:
            params["nccl_ib_disable"] = True

        pods = []
        if all(hyper_parameter in params for hyper_parameter in ["hyperparametername", "hyperparameterstartvalue", "hyperparameterendvalue", "hyperparameterstep"]):
            env_name = params["hyperparametername"]
            start = int(params["hyperparameterstartvalue"])
            end = int(params["hyperparameterendvalue"])
            step = int(params["hyperparameterstep"])

            for idx, val in enumerate(range(start, end, step)):
                pod = copy.deepcopy(params)
                params["envs"].append({"name": "DLWS_ROLE_NAME", "value": "master"})
                params["envs"].append({"name": "DLWS_NUM_GPU_PER_WORKER", "value": params["resourcegpu"]})
                pod["podName"] = "{0}-pod-{1}".format(job.job_id, idx)
                pod["envs"].append({"name": env_name, "value": val})
                pods.append(pod)
        else:
            pod = copy.deepcopy(params)
            pod["envs"].append({"name": "DLWS_ROLE_NAME", "value": "master"})
            pod["envs"].append({"name": "DLWS_NUM_GPU_PER_WORKER", "value": params["resourcegpu"]})
            pod["podName"] = job.job_id
            pods.append(pod)

        # add ai framework type to envs
        if "frameworkType" in params:
            pod["envs"].append({"name": "aiframework", "value": params["frameworkType"]})
        else:
            pass

        k8s_pods = []
        gpuMapping = DataHandler().GetAllDevice()

        for idx,pod in enumerate(pods):

            pod["numps"] = 0
            pod["numworker"] = 1
            pod["fragmentGpuJob"] = True

            if "gpuLimit" not in pod:
                pod["gpuLimit"] = pod["resourcegpu"]

            if "gpuStr" not in pod and "gpuType" in pod and pod["gpuType"]:
                deviceDict = gpuMapping.get(pod["gpuType"])
                if deviceDict is None:
                    return None,"wrong device type"
                else:
                    pod["gpuStr"] = deviceDict.get("deviceStr")

                pod["schedulerName"]=""
                if pod["gpuStr"] == "npu.huawei.com/NPU":
                    pod["schedulerName"]="volcano"
                else:
                    pass

                ## npu pod
                if pod["gpuStr"] == "npu.huawei.com/NPU":
                    pod["mount_npu_log"] = True
                else:
                    pod["mount_npu_log"] = False 

            elif "gpuStr" in pod:

                if pod["gpuStr"] == "npu.huawei.com/NPU":
                    pod["envs"].append({"name":"DLWS_IS_NPU_JOB","value":"true"})
                else:
                    pod["envs"].append({"name":"DLWS_IS_NPU_JOB","value":"false"})

            else:
                pass

            if params["jobtrainingtype"] == "InferenceJob":
                if pod["resourcegpu"]>=1:
                    pod["gpuLimit"] = 1

                if "inference_port" not in pod:
                    pod["inference_port"] = 8080

                # pod["model_name"] = pod["jobName"]
                pod["model_name"] = "ifs-"+pod["jobId"]

                assert "model_base_path" in pod
                pod["model_extra_path"] = pod["model_base_path"]
                pod["model_extra_path"] = re.sub("^/data", "", pod["model_extra_path"])
                pod["model_extra_path"] = re.sub("^/home", "", pod["model_extra_path"])

                pod["model_path_pvc"] = storage.StorageConfig.get_pvc_name(storage.PVC_TYPE_MODEL_DATA_KFSERVING_POD)

                pod["framework"] = params["framework"]
                
                if "version" in params:
                    pod["version"] = params["version"]
                if "-" in params["framework"]:
                    pod["framework"],pod["version"] = params["framework"].rsplit("-")

                if "gpuType" in pod and pod["gpuType"] and pod["gpuType"].endswith("arm64"):
                    # if pod["resourcegpu"]>0:
                    #     pod["version"] += "-npu"
                    # else:
                    pod["archType"] = "arm64"
                else:
                    if pod["resourcegpu"] > 0 and pod["framework"].lower() in ["tensorflow"]:
                        pod["version"] += "-gpu"
                    pod["archType"] = "amd64"

                pod["minReplicas"] = params["minReplicas"] if "minReplicas" in params else 1
                pod["maxReplicas"] = max(params["maxReplicas"] if "maxReplicas" in params else 1,pod["minReplicas"])

            else:

                pass # inference job

            # set cpu limits and memory limits
            device_type = params["gpuType"] 

            # cpu
            device_num = int(pod["gpuLimit"])
            if device_num == 0:
                quota = config.GetResourceLimitByDevice(device_type, config.ResourceLimit.CPU, 1)
            else:
                quota = config.GetResourceLimitByDevice(device_type, config.ResourceLimit.CPU, int(pod["gpuLimit"]))

            if quota is not None:
                pod["cpulimit"]=quota
                logger.info("job-%s cpu quota(%s)" % (job.job_id, str(quota)))
            else:
                logger.info("job-%s cpu quota is none" % (job.job_id))

            # memory
            if device_num == 0:
                quota = config.GetResourceLimitByDevice(device_type, config.ResourceLimit.MEM, 1)
            else:
                quota = config.GetResourceLimitByDevice(device_type, config.ResourceLimit.MEM, int(pod["gpuLimit"]))
            
            if quota is not None:
                pod["memorylimit"]=quota
                logger.info("job-%s mem quota(%s)" % (job.job_id, str(quota)))
            else:
                logger.info("job-%s mem quota is none" % (job.job_id))

            # default: memory request
            pod["memoryrequest"] = "100Mi"

            pod["jobtrainingtype"]=params["jobtrainingtype"]

            # mount /pod
            pod["mountpoints"].append(job.pod_path_mountpoint(os.path.join(job.job_path, "master")))
            pod["mountpoints"].append(job.ssh_config_path_mountpoint(job.job_path))

            if os.environ.get("INIT_CONTAINER_IMAGE"):
                pod["initialize"]=True
                pod["init-container"] =os.environ.get("INIT_CONTAINER_IMAGE")

            if params["jobtrainingtype"] == "InferenceJob":
                k8s_pod = self.generate_custom_resource(pod)
            else:
                k8s_pod = self.generate_pod(pod, params["cmd"])
            k8s_pods.append(k8s_pod)

        return k8s_pods, None

    def generate_secrets(self, job):
        """generate_plugin_secrets must be called after generate_pods"""
        assert (isinstance(job, Job))
        params = job.params

        if params is None:
            return []

        if "plugins" not in params:
            return []

        plugins = params["plugins"]
        if not isinstance(plugins, dict):
            return []

        # Create secret config for each plugins
        k8s_secrets = []
        for plugin, plugin_config in plugins.items():
            if plugin == "blobfuse" and isinstance(plugin_config, list):
                for bf in plugin_config:
                    k8s_secret = self.generate_blobfuse_secret(bf)
                    k8s_secrets.append(k8s_secret)
            elif plugin == "imagePull" and isinstance(plugin_config, list):
                for image_pull in plugin_config:
                    k8s_secret = self.generate_image_pull_secret(image_pull)
                    k8s_secrets.append(k8s_secret)
        return k8s_secrets

    def generate_blobfuse_secret(self, plugin):
        assert self.secret_templates is not None
        assert "blobfuse" in self.secret_templates
        secret_template = self.secret_templates["blobfuse"]
        assert isinstance(secret_template, Template)

        secret_yaml = secret_template.render(plugin=plugin)
        return yaml.full_load(secret_yaml)

    def generate_image_pull_secret(self, plugin):
        assert self.secret_templates is not None
        assert "imagePull" in self.secret_templates
        secret_template = self.secret_templates["imagePull"]
        assert isinstance(secret_template, Template)

        secret_yaml = secret_template.render(plugin=plugin)
        return yaml.full_load(secret_yaml)
