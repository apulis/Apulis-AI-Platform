# -*- coding: UTF-8 -*-
from flask_restplus import fields

class SubmitJob:
    params = {
        "jobName":{"description":'The job name', "_in":'query',"type":"string","required":True},
        "resourcegpu":{"description":'job需要的gpu number', "_in":'query',"type":"int","required":True},
        "gpuType":{"description":'机器的gpu type,如nvidia，amd等', "_in":'query',"type":"string","required":True},
        "workPath":{"description":'job work dirpath', "_in":'query',"type":"string"},
        "dataPath":{"description":'job data dirpath', "_in":'query',"type":"string","required":True},
        "jobPath":{"description":'job相关元数据保存的路径', "_in":'query',"type":"string"},
        "image":{"description":'job base docker image', "_in":'query',"type":"string","required":True},
        "cmd":{"description":'job base docker command', "_in":'query',"type":"string"},
        "logDir":{"description":'job log dir', "_in":'query',"type":"string"},
        "interactivePort":{"description":'需要开放的端口', "_in":'query',"type":"int"},
        "userName":{"description":'who submit the job?', "_in":'query',"type":"string"},
        "vcName":{"description":'which vc does the job submit under?', "_in":'query',"type":"string","required":True},
        "preemptionAllowed":{"description":'job是否可以被抢占资源?', "_in":'query',"type":"string"},
        "userId":{"description":'uid of the one who submit the job', "_in":'query',"type":"int"},
        "runningasroot":{"description":'Whether does the job run as root?', "_in":'query',"type":"string"},
        "containerUserId":{"description":'docker run user id，一般为0', "_in":'query',"type":"int"},
        "familyToken":{"description":'familyToken', "_in":'query',"type":"string"},
        "isParent":{"description":'isParent', "_in":'query',"type":"int"},
        "jobType":{"description":'training or data之一', "_in":'query',"type":"string","required":True},
        "nodeSelector":{"description":'nodeSelector', "_in":'query',"type":"string"},
        "jobtrainingtype":{"description":'regular,distribute or inference之一', "_in":'query',"type":"string"},
        "numpsworker":{"description":'jobs ps worker num', "_in":'query',"type":"int"},
        "nummpiworker":{"description":'jobs pi worker num', "_in":'query',"type":"int"},
        "jobPriority":{"description":'jobs Priority', "_in":'query',"type":"int"},
    }
    model = {
        'jobId': fields.String
    }

class RandomNumber(fields.Raw):
    __schema_type__ = "dict"
    __schema_example__ = """{"blobfuse": [{"accountKey": "", "containerName": "", "mountPath": "", "mountOptions": "", "accountName": ""}],
"imagePull": [{"username": "", "password": "", "registry": ""}]}"""
    def format(self, value):
        return 111
    def output(self, key, obj, **kwargs):
        return 111

class PostJob:
    def __init__(self, api):
        self.params = {
            "image":fields.String,
            "hostNetwork":fields.Boolean,
            "userId":fields.Integer(example="12345678"),
            "dataPath":fields.String,
            "isParent":fields.String,
            # "plugins":RandomNumber,
            "plugins":fields.Nested(
                api.model("plugins",
                          {
                              "blobfuse":fields.List(fields.Nested(api.model("blobfuse",{
                                  "accountKey":fields.String,
                                  "containerName":fields.String,
                                  "mountPath":fields.String,
                                  "mountOptions":fields.String,
                                  "accountName":fields.String,
                              }))),
                              "imagePull":fields.List(fields.Nested(api.model("imagePull",{
                                  "username":fields.String,
                                  "password":fields.String,
                                  "registry":fields.String,
                              })))
                          })
            ),
            "jobType":fields.String,
            "jobPath":fields.String,
            "containerUserId":fields.Integer,
            "resourcegpu":fields.Integer,
            "env":fields.String,
            "enabledatapath":fields.Boolean,
            "preemptionAllowed":fields.String,
            "jobtrainingtype":fields.String,
            "familyToken":fields.String,
            "enableworkpath":fields.Boolean,
            "vcName":fields.String,
            "userName":fields.String,
            "workPath":fields.String,
            "isPrivileged":fields.Boolean,
            "cmd":fields.String,
            "jobName":fields.String,
            "enablejobpath":fields.Boolean,
            "team":fields.String,
            "gpuType":fields.String,
            "model_base_path": fields.String
        }
        self.model = {
            'jobId': fields.String
        }

class PostInferenceJob:
    def __init__(self,api):
        self.params = {
            "image":fields.String,
            "vcName":fields.String,
            "userId": fields.Integer(example="12345678"),
            "userName":fields.String,
            "jobName":fields.String,
            "model_base_path": fields.String,
            "framework": fields.String,
            "device": fields.String,
        }

class PostModelConversionJob:
    def __init__(self, api):
        self.params = {
            "image":fields.String,
            "vcName":fields.String,
            "userId": fields.Integer(example="12345678"),
            "userName":fields.String,
            "jobName":fields.String,
            "inputPath": fields.String,
            "outputPath": fields.String,
            "conversionType": fields.String(example="tensorflow-A310"),
            "device": fields.String,
        }

class PushModelToFD:
    def __init__(self, api):
        self.params = {
            "jobId": fields.String
        }

class SetFDInfo:
    def __init__(self, api):
        self.params = {
            "username": fields.String,
            "password": fields.String,
            "url": fields.String
        }

class GetFDInfo:
    def __init__(self, api):
        self.params = {}

class ListJobResponse(fields.Raw):
    __schema_type__ = "List"
    __schema_example__ = """[{"jobId":xxx,"jobStatus":"xxx"},....]"""

class ListJobResponseMeta(fields.Raw):
    __schema_type__ = "dict"
    __schema_example__ = """{"queuedJobs": len(queuedJobs),"runningJobs": len(runningJobs),"finishedJobs": len(finishedJobs),"visualizationJobs": len(visualizationJobs)}"""

class ListJobs:
    params = {
        "userName":{"description":'user name', "_in":'query',"type":"string","required":True},
        "num":{"description":'list job name', "_in":'query',"type":"int","required":True},
        "vcName":{"description":'current vc name', "_in":'query',"type":"string","required":True},
        "jobOwner":{"description":'Who"s job do you want to see?', "_in":'query',"type":"string","required":True},
    }
    model = {
        "queuedJobs":ListJobResponse,
        "runningJobs":ListJobResponse,
        "finishedJobs":ListJobResponse,
        "visualizationJobs":ListJobResponse,
        "meta":ListJobResponseMeta,
    }

class ListJobsV2:
    params = {
        "userName": {"description": 'user name', "_in": 'query', "type": "string","required":True},
        "num": {"description": 'list job name', "_in": 'query', "type": "int","required":True},
        "vcName": {"description": 'current vc name', "_in": 'query', "type": "string","required":True},
        "jobOwner": {"description": 'Who"s job do you want to see?', "_in": 'query', "type": "string","required":True},
    }

class KillJob:
    params = {
        "jobId":{"description": 'job id', "_in": 'query', "type": "string","required":True},
        "userName":{"description": 'current user name', "_in": 'query', "type": "string","required":True},
    }
    model = {
        "result":fields.String(example="Success, the job is scheduled to be terminated.")
    }

class PauseJob:
    params = {
        "jobId":{"description": 'job id', "_in": 'query', "type": "string","required":True},
        "userName":{"description": 'current user name', "_in": 'query', "type": "string","required":True},
    }
    model = {
        "result": fields.String(example="Success, the job is scheduled to be paused.")
    }

class ResumeJob:
    params = {
        "jobId": {"description": 'job id', "_in": 'query', "type": "string","required":True},
        "userName": {"description": 'current user name', "_in": 'query', "type": "string","required":True},
    }
    model = {
        "result": fields.String(example="Success, the job is scheduled to be resumed.")
    }

class CloneJob:
    params = {
        "jobId": {"description": 'job id', "_in": 'query', "type": "string","required":True},
        "userName": {"description": 'current user name', "_in": 'query', "type": "string","required":True},
    }
    model = {
        "result": fields.String(example="Success, the job is scheduled to be cloned.")
    }

class ApproveJob:
    params = {
        "jobId": {"description": 'job id', "_in": 'query', "type": "string","required":True},
        "userName": {"description": 'current user name', "_in": 'query', "type": "string","required":True},
    }
    model = {
        "result": fields.String(example="Success, the job has been approved.")
    }

class GetCommands:
    params = {
        "jobId": {"description": 'job id', "_in": 'query', "type": "string","required":True},
        "userName": {"description": 'current user name', "_in": 'query', "type": "string","required":True},
    }

class OneJobResponse(fields.Raw):
    __schema_type__ = "dict"
    __schema_example__ = """{"jobId":xxx,"jobStatus":"xxx",....}"""

class GetJobDetail:
    params = {
        "jobId": {"description": 'job id', "_in": 'query', "type": "string","required":True},
        "userName": {"description": 'current user name', "_in": 'query', "type": "string","required":True},
    }
    model = {
        "result": OneJobResponse
    }

class GetJobDetailV2:
    params = {
        "jobId": {"description": 'job id', "_in": 'query', "type": "string","required":True},
        "userName": {"description": 'current user name', "_in": 'query', "type": "string","required":True},
    }
    model = {
        "result": OneJobResponse
    }

class GetJobLog:
    params = {
        "jobId": {"description": 'job id', "_in": 'query', "type": "string","required":True},
        "userName": {"description": 'current user name', "_in": 'query', "type": "string","required":True},
    }

class GetJobStatus:
    params ={
        "jobId": {"description": 'job id', "_in": 'query', "type": "string","required":True},
    }

class GetClusterStatus:
    params = {
        "userName": {"description": 'current user name', "_in": 'query', "type": "string","required":True},
    }

class AddCommand:
    params = {
        "jobId": {"description": 'job id', "_in": 'query', "type": "string","required":True},
        "command": {"description": 'command', "_in": 'query', "type": "string","required":True},
        "userName": {"description": 'current user name', "_in": 'query', "type": "string","required":True},
    }
    model = {
        "result": fields.String(example="Success, the command is scheduled to be run.")
    }

class AddUser:
    params = {
        "userName": {"description": 'userName', "_in": 'query', "type": "string","required":True},
        "uid": {"description": 'uid', "_in": 'query', "type": "int","required":True},
        "gid": {"description": 'gid', "_in": 'query', "type": "int","required":True},
        "groups": {"description": 'gid', "_in": 'query', "type": "list","required":True},
    }

class UpdateUserPerm:
    params = {
        "isAdmin": {"description": 'isAdmin', "_in": 'query', "type": "int","required":True},
        "isAuthorized": {"description": 'isAuthorized', "_in": 'query', "type": "int","required":True},
        "identityName": {"description": 'identityName', "_in": 'query', "type": "string","required":True},
        "groups": {"description": 'gid', "_in": 'query', "type": "list","required":True},
    }

class SignUp:
    params = {
        "openId": {"description": 'openId', "_in": 'query', "type": "int","required":True},
        "group": {"description": 'group', "_in": 'query', "type": "string","required":True},
        "nickName": {"description": 'nickName', "_in": 'query', "type": "string","required":True},
        "userName": {"description": 'userName', "_in": 'query', "type": "string","required":True},
        "password": {"description": 'password', "_in": 'query', "type": "string","required":True},
        "isAdmin": {"description": 'isAdmin', "_in": 'query', "type": "bool","required":True},
        "isAuthorized": {"description": 'isAuthorized', "_in": 'query', "type": "bool","required":True},
    }

class GetAccountByOpenId:
    params = {
        "openId": {"description": 'openId', "_in": 'query', "type": "int","required":True},
        "group": {"description": 'group', "_in": 'query', "type": "string","required":True},
    }

class UpdateAce:
    params = {
        "identityName": {"description": 'identityName', "_in": 'query', "type": "string","required":True},
        "resourceType": {"description": 'resourceType', "_in": 'query', "type": "string","required":True},
        "resourceName": {"description": 'resourceName', "_in": 'query', "type": "string","required":True},
        "userName": {"description": 'userName', "_in": 'query', "type": "string","required":True},
        "permissions": {"description": 'permissions', "_in": 'query', "type": "int","required":True},
    }

class DeleteAce:
    params = {
        "identityName": {"description": 'identityName', "_in": 'query', "type": "string","required":True},
        "resourceType": {"description": 'resourceType', "_in": 'query', "type": "string","required":True},
        "resourceName": {"description": 'resourceName', "_in": 'query', "type": "string","required":True},
        "userName": {"description": 'userName', "_in": 'query', "type": "string","required":True},
    }

class IsClusterAdmin:
    params = {
        "userName": {"description": 'userName', "_in": 'query', "type": "string","required":True},
    }

class GetACL:
    params = {
        "userName": {"description": 'userName', "_in": 'query', "type": "string","required":True},
    }

class ListVCs:
    params = {
        "userName": {"description": 'userName', "_in": 'query', "type": "string","required":True},
    }

class GetVC:
    params = {
        "userName": {"description": 'userName', "_in": 'query', "type": "string","required":True},
        "vcName": {"description": 'vcName', "_in": 'query', "type": "string","required":True},
    }

class AddVC:
    params = {
        "userName": {"description": 'userName', "_in": 'query', "type": "string","required":True},
        "vcName": {"description": 'vcName', "_in": 'query', "type": "string","required":True},
        "quota": {"description": 'quota', "_in": 'query', "type": "dict","required":True},
        "metadata": {"description": 'metadata', "_in": 'query', "type": "dict","required":True},
    }

class DeleteVC:
    params = {
        "userName": {"description": 'userName', "_in": 'query', "type": "string","required":True},
        "vcName": {"description": 'vcName', "_in": 'query', "type": "string","required":True},
    }

class UpdateVC:
    params = {
        "userName": {"description": 'userName', "_in": 'query', "type": "string","required":True},
        "vcName": {"description": 'vcName', "_in": 'query', "type": "string","required":True},
        "quota": {"description": 'quota', "_in": 'query', "type": "dict","required":True},
        "metadata": {"description": 'metadata', "_in": 'query', "type": "dict","required":True},
    }

class ListStorages:
    params = {
        "userName": {"description": 'userName', "_in": 'query', "type": "string"},
        "vcName": {"description": 'vcName', "_in": 'query', "type": "string"},
    }

class AddStorage:
    params = {
        "userName": {"description": 'userName', "_in": 'query', "type": "string"},
        "vcName": {"description": 'vcName', "_in": 'query', "type": "string"},
        "storageType": {"description": 'storageType', "_in": 'query', "type": "string"},
        "url": {"description": 'url', "_in": 'query', "type": "string"},
        "metadata": {"description": 'metadata', "_in": 'query', "type": "string"},
        "defaultMountPath": {"description": 'defaultMountPath', "_in": 'query', "type": "string"},
    }

class DeleteStorage:
    params = {
        "userName": {"description": 'userName', "_in": 'query', "type": "string"},
        "vcName": {"description": 'vcName', "_in": 'query', "type": "string"},
        "url": {"description": 'url', "_in": 'query', "type": "string"},
    }

class UpdateStorage:
    params = {
        "userName": {"description": 'userName', "_in": 'query', "type": "string"},
        "vcName": {"description": 'vcName', "_in": 'query', "type": "string"},
        "storageType": {"description": 'storageType', "_in": 'query', "type": "string"},
        "url": {"description": 'url', "_in": 'query', "type": "string"},
        "metadata": {"description": 'metadata', "_in": 'query', "type": "string"},
        "defaultMountPath": {"description": 'defaultMountPath', "_in": 'query', "type": "string"},
    }

class Endpoint:
    params = {
        "jobId": {"description": 'job id', "_in": 'query', "type": "string","required":True},
        "userName": {"description": 'current user name', "_in": 'query', "type": "string","required":True},
    }


class EndpointModel(fields.Raw):
    __schema_type__ = "list"
    __schema_example__ = """[{"name":"port-43333","podPort":43333},ssh,ipython,tensorboard]"""

class EndpointPost:
    params = {
        "userName": {"description": 'current user name', "_in": 'query', "type": "string","required":True},
    }
    params2 = {
        "jobId":fields.String,
        "endpoints":EndpointModel
    }

class Templates:
    params = {
        "vcName": {"description": 'vcName', "_in": 'query', "type": "string","required":True},
        "userName": {"description": 'current user name', "_in": 'query', "type": "string","required":True},
    }

class TemplatesPost:
    params = {
        "vcName": {"description": 'vcName', "_in": 'query', "type": "string","required":True},
        "userName": {"description": 'current user name', "_in": 'query', "type": "string","required":True},
        "database": {"description": 'database', "_in": 'query', "type": "string","required":True},
        "templateName": {"description": 'templateName', "_in": 'query', "type": "string","required":True},
    }

class TemplatesDelete:
    params = {
        "vcName": {"description": 'vcName', "_in": 'query', "type": "string","required":True},
        "userName": {"description": 'current user name', "_in": 'query', "type": "string","required":True},
        "database": {"description": 'database', "_in": 'query', "type": "string","required":True},
        "templateName": {"description": 'templateName', "_in": 'query', "type": "string","required":True},
    }

class JobPriority:
    params = {
        "userName": {"description": 'current user name', "_in": 'query', "type": "string","required":True},

    }

class SignIn:
    params = {
        "openId": {"description": '填username', "_in": 'query', "type": "string","required":True},
        "group": {"description": '该接口当前只能为Account', "_in": 'query', "type": "string","required":True},
        "password": {"description": 'password', "_in": 'query', "type": "string","required":True},
    }
class AddUserV2Model(fields.Raw):
    __schema_type__ = "dict"
    __schema_example__ = """{"openId":"zhangsan","group":"Account","nickName":"zhangsan","userName":"zhangsan","password":"123456",
    "email":"zhangsan@apulis.com","phoneNumber":"12345679811","isAdmin": false,"isAuthorized": true}"""

class AddUserV2:
    params = {
        "openId":fields.String,
        "group":fields.String,
        "nickName":fields.String,
        "userName":fields.String,
        "identityName":fields.String,
        "password":fields.String,
        "email":fields.String,
        "phoneNumber":fields.String,
        "isAdmin":fields.Boolean,
        "isAuthorized":fields.Boolean,
    }

class UpdateUserPermission:
    params={
        "isAdmin":fields.Boolean,
        "isAuthorized":fields.Boolean,
        "userName": fields.String,
        "identityName": fields.String,
    }

class DeleteUser:
    params = {
        "userName": fields.String,
        "identityName": fields.String,
    }

class OpenSignIn:
    params = {
        "signinType":{"description": 'wechat or microsoft', "_in": 'path', "type": "string","required":True},
    }

class OpenSignInRedirect:
    params = {
        "signinType": {"description": 'wechat or microsoft', "_in": 'path', "type": "string","required":True},
    }
class GetAccountByuserName:
    params = {
        "userName": {"description": 'userName', "_in": 'path', "type": "string","required":True},
    }

class GetAllDeviceModel(fields.Raw):
    __schema_type__ = "dict"
    __schema_example__ = """{"deviceStr":"npu.huawei.com/NPU","capacity":2}"""

class GetAllDevice:
    params = {
        "userName": {"description": 'userName', "_in": 'query', "type": "string", "required": True},
    }
    model = {
        "device type":GetAllDeviceModel}

class ListInferenceJob:
    params = {
        "jobOwner": {"description": 'jobOwner', "_in": 'query', "type": "string", "required": True},
        "vcName": {"description": 'vcName', "_in": 'query', "type": "string", "required": True},
        "num": {"description": 'num', "_in": 'query', "type": "string", "required": False},
    }
    model = {
        "device type":GetAllDeviceModel}

class ListModelConversionJob:
    params = {
        "jobOwner": {"description": 'jobOwner', "_in": 'query', "type": "string", "required": True},
        "vcName": {"description": 'vcName', "_in": 'query', "type": "string", "required": True},
        "num": {"description": 'num', "_in": 'query', "type": "string", "required": False},
    }
    model = {
        "device type":GetAllDeviceModel}

class CountJobByStatus:
    params = {
        "userName": {"description": 'userName', "_in": 'query', "type": "string", "required": True},
        "vcName": {"description": 'vcName', "_in": 'query', "type": "string", "required": True},
        "targetStatus": {"description": 'targetStatus,Multiple constants are separated by commas', "_in": 'query', "type": "string", "required": True},
    }

class HasCurrentActiveJob:
    params = {
        "userName": {"description": 'userName', "_in": 'query', "type": "string", "required": True},
    }
