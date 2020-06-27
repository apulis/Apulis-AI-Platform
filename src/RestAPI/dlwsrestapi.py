import sys
import json
import os
import signal
import base64
import yaml
import uuid
import requests
import logging
import timeit
import thread
import time
import sys
import traceback
import threading
from logging.config import dictConfig
from flask import Flask, Response,redirect,render_template
from flask_restful import reqparse, abort,url_for
from flask_restplus import Resource, Api, apidoc
from flask import request, jsonify
from flask_jwt_extended import (
    JWTManager, jwt_required, create_access_token,
    get_jwt_identity
)
import prometheus_client

sys.path.append(os.path.join(os.path.dirname(os.path.abspath(__file__)),"../utils"))
#from JobRestAPIUtils import SubmitDistJob, GetJobList, GetJobStatus, DeleteJob, GetTensorboard, GetServiceAddress, GetLog, GetJob
from config import config
import JobRestAPIUtils
import ModelConvertPushUtils
from authorization import ResourceType, Permission, AuthorizationManager, ACLManager
import model
import authorization
from config import global_vars
from DataHandler import DataHandler
from mysql_conn_pool import MysqlConn
from jwt_authorization import create_jwt_token_with_message

CONTENT_TYPE_LATEST = str("text/plain; version=0.0.4; charset=utf-8")

dir_path = os.path.dirname(os.path.realpath(__file__))
with open(os.path.join(dir_path, 'logging.yaml'), 'r') as f:
    logging_config = yaml.load(f)
    dictConfig(logging_config)
logger = logging.getLogger('restfulapi')

app = Flask(__name__)
# jwt configure
app.config['JWT_SECRET_KEY'] = config["jwt"]["secret_key"]
app.config['PROPAGATE_EXCEPTIONS'] = True
jwt = JWTManager(app)
# swagger docs configure
api = Api(app, version='1.0', title='restful API',
    description='A simple API',doc="/apis/docs/",prefix="/apis"
)
custom_apidoc = apidoc.Apidoc('restplus_custom_doc', __name__,
                              template_folder='templates',
                              static_folder=os.path.dirname(apidoc.__file__) + '/static',
                              static_url_path='/swaggerui')

@custom_apidoc.add_app_template_global
def swagger_static(filename):
    return url_for('restplus_custom_doc.static',
                   filename='{0}'.format(filename))

app.register_blueprint(custom_apidoc, url_prefix='/apis/docs')
@api.documentation
def custom_ui():
    return render_template("swagger-ui.html", title=api.title, specs_url="/apis/swagger.json")
# end

verbose = True
logger.info( "------------------- Restful API started ------------------------------------- ")
logger.info("%s", config)


if "initAdminAccess" not in global_vars or not global_vars["initAdminAccess"]:
    logger.info("===========Init Admin Access===============")
    global_vars["initAdminAccess"] = True
    logger.info('setting admin access!')
    ACLManager.UpdateAce("Administrator", AuthorizationManager.GetResourceAclPath("", ResourceType.Cluster), Permission.Admin, 0)
    if config.get("administrators") and config["administrators"]:
        for one in config["administrators"]:
            if len(one.split("@"))==2:
                dataHandler = DataHandler()
                dataHandler.UpdateAccountInfo(one,"Microsoft", one.split("@")[0], one.split("@")[0], "tryme2020", 1, 1)
                ACLManager.UpdateAce(one.split("@")[0], AuthorizationManager.GetResourceAclPath("", ResourceType.Cluster),
                                     Permission.Admin, 0)
    logger.info('admin access given!')

def _stacktraces():
   code = []

   for threadId, stack in sys._current_frames().items():
       code.append("\n# ThreadID: %s" % threadId)
       for filename, lineno, name, line in traceback.extract_stack(stack):
           code.append('File: "%s", line %d, in %s' % (filename, lineno, name))
           if line:
               code.append("  %s" % (line.strip()))

   for line in code:
       print("_stacktrace: " + line)


def _WorkerThreadFunc():
   while True:
       _stacktraces()
       time.sleep(60)

#workerThread = threading.Thread(target=_WorkerThreadFunc, args=())
#workerThread.daemon = True
#workerThread.start()


def istrue(value):
    if isinstance(value, bool):
        return value
    elif isinstance(value, basestring):
        return value.lower()[0]=='y'
    else:
        return bool(value)

def tolist(value):
    if isinstance( value, basestring):
        if len(value)>0:
            return [value]
        else:
            return []
    else:
        return value


def remove_creds(job):
    job_params = job.get("jobParams", None)
    if job_params is None:
        return

    plugins = job_params.get("plugins", None)
    if plugins is None or not isinstance(plugins, dict):
        return

    blobfuse = plugins.get("blobfuse", None)
    if blobfuse is not None and isinstance(blobfuse, list):
        for bf in blobfuse:
            bf.pop("accountName", None)
            bf.pop("accountKey", None)

    image_pull = plugins.get("imagePull", None)
    if image_pull is not None and isinstance(image_pull, list):
        for i_p in image_pull:
            i_p.pop("username", None)
            i_p.pop("password", None)

def generate_response(result):
    resp = jsonify(result)
    resp.headers["Access-Control-Allow-Origin"] = "*"
    resp.headers["dataType"] = "json"
    return resp

def get_query_params(*args,**kwargs):
    parser = reqparse.RequestParser()
    for paramName in args:
        parser.add_argument(paramName)
    args = parser.parse_args()
    return [args[name] for name in args]

class SubmitJob(Resource):
    @api.doc(params=model.SubmitJob.params)
    @api.response(200, "succeed", api.model("SubmitJob", model.SubmitJob.model))
    def get(self):
        parser = reqparse.RequestParser()
        parser.add_argument('jobName')
        parser.add_argument('resourcegpu')
        parser.add_argument('gpuType')
        parser.add_argument('workPath')
        parser.add_argument('dataPath')
        parser.add_argument('jobPath')
        parser.add_argument('image')
        parser.add_argument('cmd')
        parser.add_argument('logDir')
        parser.add_argument('interactivePort')
        parser.add_argument('userName')
        parser.add_argument('vcName')
        parser.add_argument('preemptionAllowed')
        parser.add_argument('userId')
        parser.add_argument('runningasroot')
        parser.add_argument('containerUserId')

        parser.add_argument('familyToken')
        parser.add_argument('isParent')
        parser.add_argument('jobType')
        parser.add_argument('nodeSelector')


        parser.add_argument('jobtrainingtype')
        parser.add_argument('numpsworker')
        parser.add_argument('nummpiworker')

        parser.add_argument('jobPriority')

        args = parser.parse_args()

        params = {}
        ret = {}

        for key, value in args.iteritems():
            if value is not None:
                params[key] = value

        if args["jobName"] is None or len(args["jobName"].strip()) == 0:
            ret["error"] = "job name cannot be empty"
        elif args["vcName"] is None or len(args["vcName"].strip()) == 0:
            ret["error"] = "vc name cannot be empty"
        elif args["resourcegpu"] is None or len(args["resourcegpu"].strip()) == 0:
            ret["error"] = "Number of GPU cannot be empty"
        elif args["gpuType"] is None or len(args["gpuType"].strip()) == 0:
            ret["error"] = "GPU Type cannot be empty"
        elif args["dataPath"] is None or len(args["dataPath"].strip()) == 0:
            ret["error"] = "datapath cannot be empty"
        elif args["image"] is None or len(args["image"].strip()) == 0:
            ret["error"] = "docker image cannot be empty"
        elif args["jobType"] is None or len(args["jobType"].strip()) == 0:
            ret["error"] = "jobType cannot be empty"
        else:
            params["jobName"] = args["jobName"]
            params["vcName"] = args["vcName"]
            params["resourcegpu"] = args["resourcegpu"]
            params["gpuType"] = args["gpuType"]
            params["workPath"] = args["workPath"]
            params["dataPath"] = args["dataPath"]
            params["image"] = args["image"]
            params["cmd"] = args["cmd"]
            params["jobType"] = args["jobType"]
            params["preemptionAllowed"] = args["preemptionAllowed"]

            params["jobtrainingtype"] = args["jobtrainingtype"]

            if args["jobtrainingtype"] == "PSDistJob":
                params["numps"] = 1
                params["numpsworker"] = args["numpsworker"]

            if args["jobtrainingtype"] == "MPIDistJob":
                params["nummpiworker"] = args["nummpiworker"]

            if args["jobPath"] is not None and len(args["jobPath"].strip()) > 0:
                params["jobPath"] = args["jobPath"]

            if args["logDir"] is not None and len(args["logDir"].strip()) > 0:
                params["logDir"] = args["logDir"]

            if args["userId"] is not None and len(args["userId"].strip()) > 0:
                params["userId"] = args["userId"]
            else:
                # !! note: if userId is not provided, the container will be running as root. There shouldn't be any security concern since all the resources in docker container should be user's own property. Also, we plan to allow user to choose "run as root".
                params["userId"] = "0"

            if args["nodeSelector"] is not None and len(args["nodeSelector"].strip()) > 0:
                params["nodeSelector"] = {args["nodeSelector"]:"active"}


            if args["interactivePort"] is not None and len(args["interactivePort"].strip()) > 0:
                params["interactivePort"] = args["interactivePort"]

            if args["containerUserId"] is not None and len(args["containerUserId"].strip()) > 0:
                params["containerUserId"] = args["containerUserId"]
            else:
                params["containerUserId"] = params["userId"]

            if args["userName"] is not None and len(args["userName"].strip()) > 0:
                params["userName"] = args["userName"]
            else:
                params["userName"] = "default"
            if args["familyToken"] is not None and len(args["familyToken"].strip()) > 0:
                params["familyToken"] = args["familyToken"]
            else:
                params["familyToken"] = str(uuid.uuid4())
            if args["isParent"] is not None and len(args["isParent"].strip()) > 0:
                params["isParent"] = args["isParent"]
            else:
                params["isParent"] = "1"

            if args["jobPriority"] is not None and len(args["jobPriority"].strip()) > 0:
                params["jobPriority"] = args["jobPriority"]

            params["mountpoints"] = []
            addcmd = ""
            if "mounthomefolder" in config and istrue(config["mounthomefolder"]) and "storage-mount-path" in config:
                alias = JobRestAPIUtils.getAlias(params["userName"])
                params["mountpoints"].append({"name":"homeholder","containerPath":os.path.join("/home", alias),"hostPath":os.path.join(config["storage-mount-path"], "work", alias)})
            if "mountpoints" in config and "storage-mount-path" in config:
                # see link_fileshares in deploy.py
                for k, v in config["mountpoints"].iteritems():
                    if "mountpoints" in v:
                        for basename in tolist(v["mountpoints"]):
                            if basename!="" and basename not in config["default-storage-folders"] and basename in config["deploymounts"]:
                                hostBase = os.path.join(config["storage-mount-path"], basename[1:]) if os.path.isabs(basename) else os.path.join(config["storage-mount-path"], basename)
                                basealias = basename[1:] if os.path.isabs(basename) else basename
                                containerBase = os.path.join("/", basename)
                                alias = JobRestAPIUtils.getAlias(params["userName"])
                                shares = [alias]
                                if "publicshare" in v:
                                    if "all" in v["publicshare"]:
                                        shares += (tolist(v["publicshare"]["all"]))
                                    if basename in v["publicshare"]:
                                        shares += (tolist(v["publicshare"][basename]))
                                for oneshare in shares:
                                    hostPath = os.path.join(hostBase, oneshare)
                                    containerPath = os.path.join(containerBase, oneshare)
                                    if v["type"]=="emptyDir":
                                        params["mountpoints"].append({"name":basealias+"-"+oneshare,
                                                                        "containerPath": containerPath,
                                                                        "hostPath": "/emptydir",
                                                                        "emptydir": "yes" })
                                    else:
                                        params["mountpoints"].append({"name":basealias+"-"+oneshare,
                                                                        "containerPath": containerPath,
                                                                        "hostPath": hostPath })
                                    if False and "type" in v and v["type"]!="local" and v["type"]!="localHDD":
                                        # This part is disabled, see if False above
                                        # This is a shared drive, we can try to create it, and enable the write permission
                                        if not os.path.exists(hostPath):
                                            cmd = "sudo mkdir -m 0777 -p %s; " % hostPath
                                            os.system( cmd )
                                            logger.info( cmd )
                                            if oneshare==alias:
                                                cmd = "sudo chown %s:%s %s; " % (params["containerUserId"], "500000513", hostPath )
                                                os.system(cmd )
                                                logger.info( cmd )
                                    addcmd += "chmod 0777 %s ; " % containerPath
                                    if oneshare==alias:
                                        addcmd += "chown %s:%s %s ; " % ( params["userId"], "500000513", containerPath )
            if verbose and len(params["mountpoints"]) > 0:
                logger.info("Mount path for job %s", params )
                for mounts in params["mountpoints"]:
                    logger.info( "Share %s, mount %s at %s", mounts["name"], mounts["hostPath"], mounts["containerPath"])
            if len(addcmd) > 0:
                params["cmd"] = addcmd + params["cmd"]
            output = JobRestAPIUtils.SubmitJob(json.dumps(params))

            if "jobId" in output:
                ret["jobId"] = output["jobId"]
            else:
                if "error" in output:
                    ret["error"] = "Cannot create job!" + output["error"]
                else:
                    ret["error"] = "Cannot create job!"

        resp = jsonify(ret)
        resp.headers["Access-Control-Allow-Origin"] = "*"
        resp.headers["dataType"] = "json"
        return resp
##
## Actually setup the Api resource routing here
##
api.add_resource(SubmitJob, '/SubmitJob')



class PostJob(Resource):
    @api.expect(api.model("PostJobModel", model.PostJob(api).params))
    @api.response(200, "succeed", api.model("PostJob", model.PostJob(api).model))
    def post(self):
        params = request.get_json(force=True)
        logger.info("Post Job")
        logger.info(params)

        ret = {}
        output = JobRestAPIUtils.SubmitJob(json.dumps(params))

        if "jobId" in output:
            ret["jobId"] = output["jobId"]
        else:
            if "error" in output:
                ret["error"] = "Cannot create job!" + output["error"]
            else:
                ret["error"] = "Cannot create job!"
        logger.info("Submit job through restapi, output is %s, ret is %s", output, ret)
        resp = jsonify(ret)
        resp.headers["Access-Control-Allow-Origin"] = "*"
        resp.headers["dataType"] = "json"
        return resp

api.add_resource(PostJob, '/PostJob')

class PostInferenceJob(Resource):
    @api.expect(api.model("PostInferenceJob", model.PostInferenceJob(api).params))
    def post(self):
        params = request.get_json(force=True)
        logger.info("Post PostInferenceJob Job")
        logger.info(params)

        ret = {}
        output = JobRestAPIUtils.PostInferenceJob(json.dumps(params))

        if "jobId" in output:
            ret["jobId"] = output["jobId"]
        else:
            if "error" in output:
                ret["error"] = "Cannot create job!" + output["error"]
            else:
                ret["error"] = "Cannot create job!"
        logger.info("Submit job through restapi, output is %s, ret is %s", output, ret)
        resp = jsonify(ret)
        resp.headers["Access-Control-Allow-Origin"] = "*"
        resp.headers["dataType"] = "json"
        return resp

api.add_resource(PostInferenceJob, '/PostInferenceJob')

### Model Convert && FD Push

class PostModelConversionJob(Resource):
    @api.expect(api.model("PostModelConversionJob", model.PostModelConversionJob(api).params))
    def post(self):
        params = request.get_json(force=True)
        logger.info("Post PostInferenceJob Job")
        logger.info(params)

        ret = {}
        output = JobRestAPIUtils.PostModelConversionJob(json.dumps(params))

        if "jobId" in output:
            ret["jobId"] = output["jobId"]
        else:
            if "error" in output:
                ret["error"] = "Cannot create job!" + output["error"]
            else:
                ret["error"] = "Cannot create job!"
        logger.info("Submit job through restapi, output is %s, ret is %s", output, ret)
        resp = jsonify(ret)
        resp.headers["Access-Control-Allow-Origin"] = "*"
        resp.headers["dataType"] = "json"
        return resp

api.add_resource(PostModelConversionJob, '/PostModelConversionJob')

class SetFDInfo(Resource):
    @api.expect(api.model("SetFDInfo", model.SetFDInfo(api).params))
    def post(self):
        params = request.get_json(force=True)
        logger.info("Set FD server info")
        logger.info(params)

        ret = ModelConvertPushUtils.SetFDInfo(params)
        logger.info("Set fd server info through restapi, result is %s", ret)
        resp = jsonify(ret)
        resp.headers["Access-Control-Allow-Origin"] = "*"
        resp.headers["dataType"] = "json"
        return resp

api.add_resource(SetFDInfo, '/SetFDInfo')

class GetFDInfo(Resource):
    @api.expect(api.model("GetFDInfo", model.GetFDInfo(api).params))
    def get(self):
        logger.info("Request to get fd server info")
        ret = ModelConvertPushUtils.GetFDInfo()
        logger.info("Get fd server info through restapi, result is %s", ret)
        resp = jsonify(ret)
        resp.headers["Access-Control-Allow-Origin"] = "*"
        resp.headers["dataType"] = "json"
        return resp

api.add_resource(GetFDInfo, '/GetFDInfo')

class PushModelToFD(Resource):
    @api.expect(api.model("PushModelToFD", model.PushModelToFD(api).params))
    def post(self):
        params = request.get_json(force=True)
        logger.info("Push model to fd")
        logger.info(params)

        ret = ModelConvertPushUtils.PushModelToFD(params)
        logger.info("Push model file to fd through restapi, result is %s", ret)
        resp = jsonify(ret)
        resp.headers["Access-Control-Allow-Origin"] = "*"
        resp.headers["dataType"] = "json"
        return resp

api.add_resource(PushModelToFD, '/PushModelToFD')

# shows a list of all todos, and lets you POST to add new tasks
class ListJobs(Resource):
    @api.doc(params=model.ListJobs.params)
    @api.response(200, "succeed", api.model("ListJobs", model.ListJobs.model))
    def get(self):
        parser = reqparse.RequestParser()
        parser.add_argument('userName')
        parser.add_argument('num')
        parser.add_argument('vcName')
        parser.add_argument('jobOwner')
        args = parser.parse_args()
        num = None
        if args["num"] is not None:
            try:
                num = int(args["num"])
            except:
                pass
        jobs = JobRestAPIUtils.GetJobList(args["userName"], args["vcName"], args["jobOwner"], num)

        jobList = []
        queuedJobs = []
        runningJobs = []
        finishedJobs = []
        visualizationJobs = []
        for job in jobs:
            job.pop("jobDescriptionPath",None)
            job.pop("jobDescription",None)

            job["jobParams"] = json.loads(base64.b64decode(job["jobParams"]))

            if "endpoints" in job and job["endpoints"] is not None and len(job["endpoints"].strip()) > 0:
                job["endpoints"] = json.loads(job["endpoints"])

            if "jobStatusDetail" in job and job["jobStatusDetail"] is not None and len(job["jobStatusDetail"].strip()) > 0:
                try:
                    s = job["jobStatusDetail"]
                    s = base64.b64decode(s)
                    s = json.loads(s)
                    job["jobStatusDetail"] = s
                except Exception as e:
                    job["jobStatusDetail"] = s
                    pass

            # Remove credentials
            remove_creds(job)

            if job["jobStatus"] == "running":
                if job["jobType"] == "training":
                    runningJobs.append(job)
                elif job["jobType"] == "visualization":
                    visualizationJobs.append(job)
            elif job["jobStatus"] == "queued" or job["jobStatus"] == "scheduling" or job["jobStatus"] == "unapproved":
                queuedJobs.append(job)
            else:
                finishedJobs.append(job)

        ret = {}
        ret["queuedJobs"] = queuedJobs
        ret["runningJobs"] = runningJobs
        ret["finishedJobs"] = finishedJobs
        ret["visualizationJobs"] = visualizationJobs
        ret["meta"] = {"queuedJobs": len(queuedJobs),"runningJobs": len(runningJobs),"finishedJobs": len(finishedJobs),"visualizationJobs": len(visualizationJobs)}
        resp = jsonify(ret)
        resp.headers["Access-Control-Allow-Origin"] = "*"
        resp.headers["dataType"] = "json"
        return resp
##
## Actually setup the Api resource routing here
##
api.add_resource(ListJobs, '/ListJobs')

# shows a list of all jobs, and lets you POST to add new tasks
class ListJobsV2(Resource):
    @api.doc(params=model.ListJobsV2.params)
    def get(self):
        parser = reqparse.RequestParser()
        parser.add_argument('userName')
        parser.add_argument('num')
        parser.add_argument('vcName')
        parser.add_argument('jobOwner')
        args = parser.parse_args()
        num = None
        if args["num"] is not None:
            try:
                num = int(args["num"])
            except:
                pass

        jobs = JobRestAPIUtils.GetJobListV2(args["userName"], args["vcName"], args["jobOwner"], num)
        for _, joblist in jobs.items():
            if isinstance(joblist, list):
                for job in joblist:
                    remove_creds(job)

        resp = generate_response(jobs)
        return resp
##
## Actually setup the Api resource routing here
##
api.add_resource(ListJobsV2, '/ListJobsV2')

class GetAllDevice(Resource):
    @api.doc(params=model.GetAllDevice.params)
    @api.response(200, "succeed", api.model("GetAllDevice", model.GetAllDevice.model))
    def get(self):
        parser = reqparse.RequestParser()
        parser.add_argument('userName')
        args = parser.parse_args()
        userName = args["userName"]
        result = JobRestAPIUtils.GetAllDevice(userName)
        resp = jsonify(result)
        resp.headers["Access-Control-Allow-Origin"] = "*"
        resp.headers["dataType"] = "json"
        return resp
api.add_resource(GetAllDevice, '/GetAllDevice')

class GetAllSupportInference(Resource):
    def get(self):
        result = JobRestAPIUtils.GetAllSupportInference()
        resp = jsonify(result)
        resp.headers["Access-Control-Allow-Origin"] = "*"
        resp.headers["dataType"] = "json"
        return resp
api.add_resource(GetAllSupportInference, '/GetAllSupportInference')

class ListInferenceJob(Resource):
    @api.doc(params=model.ListInferenceJob.params)
    def get(self):
        parser = reqparse.RequestParser()
        parser.add_argument('num')
        parser.add_argument('vcName')
        parser.add_argument('jobOwner')
        args = parser.parse_args()
        jobs = JobRestAPIUtils.ListInferenceJob(args["jobOwner"],args["vcName"],args["num"])
        for _, joblist in jobs.items():
            if isinstance(joblist, list):
                for job in joblist:
                    remove_creds(job)
        resp = generate_response(jobs)
        return resp
api.add_resource(ListInferenceJob, '/ListInferenceJob')

class ListModelConversionJob(Resource):
    @api.doc(params=model.ListModelConversionJob.params)
    def get(self):
        parser = reqparse.RequestParser()
        parser.add_argument('num')
        parser.add_argument('vcName')
        parser.add_argument('jobOwner')
        args = parser.parse_args()
        jobs = JobRestAPIUtils.ListModelConversionJob(args["jobOwner"], args["vcName"], args["num"])
        for _, joblist in jobs.items():
            if isinstance(joblist, list):
                for job in joblist:
                    remove_creds(job)
        resp = generate_response(jobs)
        return resp
api.add_resource(ListModelConversionJob, '/ListModelConversionJob')

class CountJobByStatus(Resource):
    @api.doc(params=model.CountJobByStatus.params)
    def get(self):
        parser = reqparse.RequestParser()
        parser.add_argument('userName')
        parser.add_argument('targetStatus')
        parser.add_argument('vcName')
        args = parser.parse_args()
        userName = args["userName"]
        targetStatus = args["targetStatus"]
        vcName = args["vcName"]
        result = JobRestAPIUtils.CountJobByStatus(userName,vcName,targetStatus)
        resp = jsonify(result)
        resp.headers["Access-Control-Allow-Origin"] = "*"
        resp.headers["dataType"] = "json"
        return resp
api.add_resource(CountJobByStatus, '/CountJobByStatus')

class KillJob(Resource):
    @api.doc(params=model.KillJob.params)
    @api.response(200, "succeed", api.model("KillJob", model.KillJob.model))
    def get(self):
        parser = reqparse.RequestParser()
        parser.add_argument('jobId')
        parser.add_argument('userName')
        args = parser.parse_args()
        jobId = args["jobId"]
        userName = args["userName"]
        result = JobRestAPIUtils.KillJob(userName, jobId)
        ret = {}
        if result:
            # NOTE "Success" prefix is used in reaper, please also update reaper code
            # if need to change it.
            ret["result"] = "Success, the job is scheduled to be terminated."
        else:
            ret["result"] = "Cannot Kill the job. Job ID:" + jobId

        resp = jsonify(ret)
        resp.headers["Access-Control-Allow-Origin"] = "*"
        resp.headers["dataType"] = "json"

        return resp
##
## Actually setup the Api resource routing here
##
api.add_resource(KillJob, '/KillJob')



class PauseJob(Resource):
    @api.doc(params=model.PauseJob.params)
    @api.response(200, "succeed", api.model("PauseJob", model.PauseJob.model))
    def get(self):
        parser = reqparse.RequestParser()
        parser.add_argument('jobId')
        parser.add_argument('userName')
        args = parser.parse_args()
        jobId = args["jobId"]
        userName = args["userName"]
        result = JobRestAPIUtils.PauseJob(userName, jobId)
        ret = {}
        if result:
            ret["result"] = "Success, the job is scheduled to be paused."
        else:
            ret["result"] = "Cannot pause the job. Job ID:" + jobId

        resp = jsonify(ret)
        resp.headers["Access-Control-Allow-Origin"] = "*"
        resp.headers["dataType"] = "json"

        return resp
##
## Actually setup the Api resource routing here
##
api.add_resource(PauseJob, '/PauseJob')



class ResumeJob(Resource):
    @api.doc(params=model.ResumeJob.params)
    @api.response(200, "succeed", api.model("ResumeJob", model.ResumeJob.model))
    def get(self):
        parser = reqparse.RequestParser()
        parser.add_argument('jobId')
        parser.add_argument('userName')
        args = parser.parse_args()
        jobId = args["jobId"]
        userName = args["userName"]
        result = JobRestAPIUtils.ResumeJob(userName, jobId)
        ret = {}
        if result:
            ret["result"] = "Success, the job is scheduled to be resumed."
        else:
            ret["result"] = "Cannot resume the job. Job ID:" + jobId

        resp = jsonify(ret)
        resp.headers["Access-Control-Allow-Origin"] = "*"
        resp.headers["dataType"] = "json"

        return resp
##
## Actually setup the Api resource routing here
##
api.add_resource(ResumeJob, '/ResumeJob')



class CloneJob(Resource):
    @api.doc(params=model.CloneJob.params)
    @api.response(200, "succeed", api.model("CloneJob", model.CloneJob.model))
    def get(self):
        parser = reqparse.RequestParser()
        parser.add_argument('jobId')
        parser.add_argument('userName')
        args = parser.parse_args()
        jobId = args["jobId"]
        userName = args["userName"]
        result = JobRestAPIUtils.CloneJob(userName, jobId)
        ret = {}
        if result:
            ret["result"] = "Success, the job is scheduled to be cloned."
        else:
            ret["result"] = "Cannot clone the job. Job ID:" + jobId

        resp = jsonify(ret)
        resp.headers["Access-Control-Allow-Origin"] = "*"
        resp.headers["dataType"] = "json"

        return resp
##
## Actually setup the Api resource routing here
##
api.add_resource(CloneJob, '/CloneJob')



class ApproveJob(Resource):
    @api.doc(params=model.ApproveJob.params)
    @api.response(200, "succeed", api.model("ApproveJob", model.ApproveJob.model))
    def get(self):
        parser = reqparse.RequestParser()
        parser.add_argument('jobId')
        parser.add_argument('userName')
        args = parser.parse_args()
        jobId = args["jobId"]
        userName = args["userName"]
        result = JobRestAPIUtils.ApproveJob(userName, jobId)
        ret = {}
        if result:
            ret["result"] = "Success, the job has been approved."
        else:
            ret["result"] = "Cannot approve the job. Job ID:" + jobId

        resp = jsonify(ret)
        resp.headers["Access-Control-Allow-Origin"] = "*"
        resp.headers["dataType"] = "json"

        return resp
##
## Actually setup the Api resource routing here
##
api.add_resource(ApproveJob, '/ApproveJob')



class GetCommands(Resource):
    @api.doc(params=model.GetCommands.params)
    def get(self):
        parser = reqparse.RequestParser()
        parser.add_argument('jobId')
        parser.add_argument('userName')
        args = parser.parse_args()
        jobId = args["jobId"]
        userName = args["userName"]
        commands = JobRestAPIUtils.GetCommands(userName, jobId)
        resp = jsonify(commands)
        resp.headers["Access-Control-Allow-Origin"] = "*"
        resp.headers["dataType"] = "json"

        return resp
##
## Actually setup the Api resource routing here
##
api.add_resource(GetCommands, '/GetCommands')



class GetJobDetail(Resource):
    @api.doc(params=model.GetJobDetail.params)
    @api.response(200, "succeed", api.model("GetJobDetail", model.GetJobDetail.model))
    def get(self):
        parser = reqparse.RequestParser()
        parser.add_argument('jobId')
        parser.add_argument('userName')
        args = parser.parse_args()
        jobId = args["jobId"]
        userName = args["userName"]
        job = JobRestAPIUtils.GetJobDetail(userName, jobId)
        job["jobParams"] = json.loads(base64.b64decode(job["jobParams"]))
        if "endpoints" in job and job["endpoints"] is not None and len(job["endpoints"].strip()) > 0:
            job["endpoints"] = json.loads(job["endpoints"])
        if "jobStatusDetail" in job and job["jobStatusDetail"] is not None and len(job["jobStatusDetail"].strip()) > 0:
            try:
                job["jobStatusDetail"] = json.loads(base64.b64decode(job["jobStatusDetail"]))
            except Exception as e:
                pass
        if "jobMeta" in job:
            job.pop("jobMeta",None)

        # Remove credentials
        remove_creds(job)

        resp = jsonify(job)
        resp.headers["Access-Control-Allow-Origin"] = "*"
        resp.headers["dataType"] = "json"
        return resp
##
## Actually setup the Api resource routing here
##
api.add_resource(GetJobDetail, '/GetJobDetail')


class GetJobDetailV2(Resource):
    @api.doc(params=model.GetJobDetailV2.params)
    @api.response(200, "succeed", api.model("GetJobDetailV2", model.GetJobDetailV2.model))
    def get(self):
        parser = reqparse.RequestParser()
        parser.add_argument('jobId')
        parser.add_argument('userName')
        args = parser.parse_args()
        jobId = args["jobId"]
        userName = args["userName"]
        job = JobRestAPIUtils.GetJobDetailV2(userName, jobId)
        remove_creds(job)
        resp = generate_response(job)
        return resp
##
## Actually setup the Api resource routing here
##
api.add_resource(GetJobDetailV2, '/GetJobDetailV2')


class GetJobLog(Resource):
    @api.doc(params=model.GetJobLog.params)
    def get(self):
        parser = reqparse.RequestParser()
        parser.add_argument('jobId', required=True)
        parser.add_argument('userName', required=True)
        args = parser.parse_args()
        jobId = args["jobId"]
        userName = args["userName"]
        return JobRestAPIUtils.GetJobLog(userName, jobId)
##
## Actually setup the Api resource routing here
##
api.add_resource(GetJobLog, '/GetJobLog')


class GetJobStatus(Resource):
    @api.doc(params=model.GetJobStatus.params)
    def get(self):
        parser = reqparse.RequestParser()
        parser.add_argument('jobId')
        args = parser.parse_args()
        jobId = args["jobId"]
        job = JobRestAPIUtils.GetJobStatus(jobId)
        resp = jsonify(job)
        resp.headers["Access-Control-Allow-Origin"] = "*"
        resp.headers["dataType"] = "json"

        return resp
##
## Actually setup the Api resource routing here
##
api.add_resource(GetJobStatus, '/GetJobStatus')


class GetClusterStatus(Resource):
    @api.doc(params=model.GetClusterStatus.params)
    def get(self):
        parser = reqparse.RequestParser()
        parser.add_argument('userName')
        args = parser.parse_args()
        userName = args["userName"]
        cluster_status, last_updated_time = JobRestAPIUtils.GetClusterStatus()
        cluster_status["last_updated_time"] = last_updated_time
        resp = jsonify(cluster_status)
        resp.headers["Access-Control-Allow-Origin"] = "*"
        resp.headers["dataType"] = "json"

        return resp
##
## Actually setup the Api resource routing here
##
api.add_resource(GetClusterStatus, '/GetClusterStatus')


class AddCommand(Resource):
    @api.doc(params=model.AddCommand.params)
    @api.response(200, "succeed", api.model("AddCommand", model.AddCommand.model))
    def get(self):
        parser = reqparse.RequestParser()
        parser.add_argument('jobId')
        parser.add_argument('command')
        parser.add_argument('userName')
        args = parser.parse_args()
        userName = args["userName"]
        jobId = args["jobId"]
        command = args["command"]
        ret = {}
        if command is None or len(command) == 0:
            ret["result"] = "Cannot Run empty Command. Job ID:" + jobId
        else:
            result = JobRestAPIUtils.AddCommand(userName, jobId, command)
            if result:
                ret["result"] = "Success, the command is scheduled to be run."
            else:
                ret["result"] = "Cannot Run the Command. Job ID:" + jobId

        resp = jsonify(ret)
        resp.headers["Access-Control-Allow-Origin"] = "*"
        resp.headers["dataType"] = "json"

        return resp
##
## Actually setup the Api resource routing here
##
api.add_resource(AddCommand, '/AddCommand')

class AddUser(Resource):
    @api.doc(params=model.AddUser.params)
    def get(self):
        parser = reqparse.RequestParser()
        parser.add_argument('userName')
        parser.add_argument('uid')
        parser.add_argument('gid')
        parser.add_argument('groups')
        args = parser.parse_args()

        ret = {}
        userName = args["userName"]
        if args["uid"] is None or len(args["uid"].strip()) == 0:
            uid = authorization.INVALID_ID
        else:
            uid = args["uid"]

        if args["gid"] is None or len(args["gid"].strip()) == 0:
            gid = authorization.INVALID_ID
        else:
            gid = args["gid"]

        if args["groups"] is None or len(args["groups"].strip()) == 0:
            groups = []
        else:
            groups = args["groups"]

        ret["status"] = JobRestAPIUtils.AddUser(userName, uid, gid, groups)
        resp = jsonify(ret)
        resp.headers["Access-Control-Allow-Origin"] = "*"
        resp.headers["dataType"] = "json"

        return resp
##
## Actually setup the Api resource routing here
##
api.add_resource(AddUser, '/AddUser')

class ListUser(Resource):
    def get(self):
        ret = {}
        ret = JobRestAPIUtils.ListUser()
        resp = jsonify(ret)
        resp.headers["Access-Control-Allow-Origin"] = "*"
        resp.headers["dataType"] = "json"

        return resp
api.add_resource(ListUser, '/ListUser')

class HasCurrentActiveJob(Resource):
    @api.doc(params=model.HasCurrentActiveJob.params)
    def get(self):
        parser = reqparse.RequestParser()
        parser.add_argument('userName')
        args = parser.parse_args()
        userName = args["userName"]
        ret = DataHandler().HasCurrentActiveJob(userName)
        resp = jsonify(ret)
        resp.headers["Access-Control-Allow-Origin"] = "*"
        resp.headers["dataType"] = "json"
        return resp
api.add_resource(HasCurrentActiveJob, '/HasCurrentActiveJob')

class UpdateUserPerm(Resource):
    @api.doc(params=model.UpdateUserPerm.params)
    def get(self):
        parser = reqparse.RequestParser()
        parser.add_argument('isAdmin')
        parser.add_argument('isAuthorized')
        parser.add_argument('identityName')
        args = parser.parse_args()
        identityName = args["identityName"]
        isAdmin = args["isAdmin"]
        isAuthorized = args["isAuthorized"]
        ret = {}
        ret["result"] = JobRestAPIUtils.updateUserPerm(identityName,isAdmin,isAuthorized)
        resp = jsonify(ret)
        resp.headers["Access-Control-Allow-Origin"] = "*"
        resp.headers["dataType"] = "json"

        return resp
api.add_resource(UpdateUserPerm, '/UpdateUserPerm')

class SignUp(Resource):
    @api.doc(params=model.SignUp.params)
    def get(self):
        parser = reqparse.RequestParser()
        parser.add_argument('openId')
        parser.add_argument('group')
        parser.add_argument('nickName')
        parser.add_argument('userName')
        parser.add_argument('password')
        parser.add_argument('isAdmin')
        parser.add_argument('isAuthorized')

        args = parser.parse_args()
        openId = args["openId"]
        group = args["group"]
        nickName = args["nickName"]
        userName = args["userName"]
        password = args["password"]

        ret = {}

        if args["isAdmin"] is None or args["isAdmin"].strip() != 'true':
            isAdmin = False
        else:
            isAdmin = True

        if args["isAuthorized"] is None or args["isAuthorized"].strip() != 'true':
            isAuthorized = False
        else:
            isAuthorized = True

        output = JobRestAPIUtils.SignUp(openId, group, nickName, userName, password, isAdmin, isAuthorized)
        if "error" in output:
            ret["result"] = False
            ret["error"] = output["error"]
        else:
            ret["result"] = True

        resp = jsonify(ret)
        resp.headers["Access-Control-Allow-Origin"] = "*"
        resp.headers["dataType"] = "json"

        return resp
##
## Actually setup the Api resource routing here
##
api.add_resource(SignUp, '/SignUp')


class GetAccountByOpenId(Resource):
    @api.doc(params=model.GetAccountByOpenId.params)
    def get(self):
        parser = reqparse.RequestParser()
        parser.add_argument('openId')
        parser.add_argument('group')
        args = parser.parse_args()

        openId = args["openId"]
        group = args["group"]

        ret = JobRestAPIUtils.GetAccountByOpenId(openId, group)
        resp = jsonify(ret)
        resp.headers["Access-Control-Allow-Origin"] = "*"
        resp.headers["dataType"] = "json"

        return resp
##
## Actually setup the Api resource routing here
##
api.add_resource(GetAccountByOpenId, '/getAccountInfo')

class GetAccountByuserName(Resource):
    @api.doc(params=model.GetAccountByuserName.params)
    def get(self):
        parser = reqparse.RequestParser()
        parser.add_argument('userName')
        args = parser.parse_args()
        userName = args["userName"]

        ret = JobRestAPIUtils.GetAccountByUserName(userName)
        resp = jsonify(ret)
        resp.headers["Access-Control-Allow-Origin"] = "*"
        resp.headers["dataType"] = "json"

        return resp
##
## Actually setup the Api resource routing here
##
api.add_resource(GetAccountByuserName, '/getAccountInfoByUserName')

class OpenSignInRedirect(Resource):
    @api.doc(params=model.OpenSignIn.params)
    def get(self,signinType):
        if signinType == "wechat":
            return redirect("https://open.weixin.qq.com/connect/qrconnect?appid={}&redirect_uri=https%3A%2F%2F{}%2Fapi/login/wechat&response_type=code&scope=snsapi_userinfo,snsapi_login&state=".format(config["Authentication"]["WeChat"]["AppId"],config["webportal_node"]))
        elif signinType == "microsoft":
            return redirect("https://login.microsoftonline.com/common/oauth2/v2.0/authorize?client_id={}&scope=https%3A%2F%2Fgraph.microsoft.com%2Fuser.read&response_type=code&redirect_uri=https%3A%2F%2F{}%2Fapi%2Flogin%2Fmicrosoft&state=".format(config["Authentication"]["Microsoft"]["ClientId"],config["webportal_node"]))
        else:
            return redirect("/")
api.add_resource(OpenSignInRedirect, '/openLogin/<signinType>')

class OpenSignIn(Resource):
    def get(self,signinType):
        parser = reqparse.RequestParser()
        parser.add_argument('remoteError')
        parser.add_argument('code')
        parser.add_argument('state')
        args = parser.parse_args()
        code = args["code"]
        re = None
        if signinType == "wechat":
            re = requests.get(url="https://api.weixin.qq.com/sns/oauth2/access_token?appid="+config["Authentication"]["WeChat"]["AppId"]+"&secret="+
                             config["Authentication"]["WeChat"]["AppSecret"]+"&code="+code+"&grant_type=authorization_code")

        elif signinType=="microsoft":
            re = requests.post(url="https://login.microsoftonline.com/common/oauth2/token",data={"client_id":config["Authentication"]["Microsoft"]["ClientId"],
                                                                                            "client_secret":config["Authentication"]["Microsoft"]["ClientSecret"],
                                                                                            "code":code,
                                                                                            "grant_type":"authorization_code",
                                                                                            "redirect_uri":"https://"+config["webportal_node"]+"/apis"
                                                                                                           +url_for("opensignin",signinType="microsoft")})
        if not re:
            logging.error(re.content)
            return redirect("/login?error=get-token-failed")
        response = json.loads(re.content.decode("utf-8"))
        access_token =response.get("access_token")
        refresh_token =response.get("refresh_token")
        openid =response.get("openid")
        re = None
        if signinType == "wechat":
            re = requests.get("https://api.weixin.qq.com/sns/userinfo?access_token="+access_token+"&openid="+openid+"&lang=zh_CN")
        elif signinType == "microsoft":
            re = requests.get("https://graph.microsoft.com/v1.0/me",headers={"Authorization":"Bearer "+access_token})
        if not re:
            logging.error(re.content)
            return redirect("/login?error=get-info-failed")
        info_response = json.loads(re.content.decode("utf-8"))
        if signinType == "microsoft":
            nickName = info_response.get("displayName")
            email = info_response.get("mail")
            openId = info_response.get("id")
            group = "Microsoft"
            if not nickName:
                nickName = email
        elif signinType == "wechat":
            nickName = info_response.get("nickname")
            openId = info_response.get("unionid")
            email = info_response.get("email")
            group = "Wechat"
        else:
            return redirect("/login?error=get-detail-failed")
        info = JobRestAPIUtils.GetAccountByOpenId(openId, group)
        if not info:
            return redirect("/signup?openId={}&group={}".format(openId,group))
        token = create_jwt_token_with_message(info)
        return redirect("/?token=" + token)
api.add_resource(OpenSignIn, '/login/<signinType>')

class SignIn(Resource):
    @api.doc(params=model.SignIn.params)
    def get(self):
        parser = reqparse.RequestParser()
        parser.add_argument('openId')
        parser.add_argument('group')
        parser.add_argument('password')
        args = parser.parse_args()

        openId = args["openId"]
        group = args["group"]
        password = args["password"]
        token = None
        if group=="Account":
            ret = DataHandler().GetAccountByOpenIdAndPassword(openId, group, password)
            if ret:
                token = create_jwt_token_with_message(ret[0])
                return token
        resp = jsonify(token)
        resp.headers["Access-Control-Allow-Origin"] = "*"
        resp.headers["dataType"] = "json"
        return "user account not exist!"


api.add_resource(SignIn, '/signIn')


class AddUserV2(Resource):
    @api.expect(api.model("AddUserV2", model.AddUserV2.params))
    def post(self):
        # current_user = get_jwt_identity()
        params = request.get_json(silent=True)
        openId = params["openId"]
        group = params["group"]
        nickName = params["nickName"]
        userName = params["userName"]
        identityName = params["identityName"]
        password = params["password"]
        email = params.get("email","")
        phoneNumber = params.get("phoneNumber","")
        isAdmin = params["isAdmin"]
        isAuthorized = params["isAuthorized"]

        if group!="Account":
            return "wrong group type",400
        ret = {}
        if not AuthorizationManager.HasAccess(userName, ResourceType.Cluster, "", Permission.Admin):
            return 403
        output = JobRestAPIUtils.SignUp(openId, group, nickName, identityName, password, isAdmin, isAuthorized)
        if "error" in output:
            ret["result"] = False
            ret["error"] = output["error"]
        else:
            dataHander = DataHandler()
            dataHander.UpdateEmailAndPhone(openId, group, email, phoneNumber)
            ret["result"] = True

        resp = jsonify(ret)
        resp.headers["Access-Control-Allow-Origin"] = "*"
        resp.headers["dataType"] = "json"

        return resp

api.add_resource(AddUserV2, '/addUser2')

class UpdateUserPermission(Resource):
    @api.expect(api.model("UpdateUserPermission", model.UpdateUserPermission.params))
    def patch(self):
        params = request.get_json(silent=True)
        userName = params["userName"]
        isAdmin = params["isAdmin"]
        isAuthorized = params["isAuthorized"]
        identityName = params["identityName"]
        if not AuthorizationManager.HasAccess(userName, ResourceType.Cluster, "", Permission.Admin):
            return 403
        permission = Permission.Admin if isAdmin else (Permission.User if isAuthorized else Permission.Unauthorized)
        resourceAclPath = AuthorizationManager.GetResourceAclPath("", ResourceType.Cluster)
        ret = ACLManager.UpdateAce(identityName, resourceAclPath, permission, 0)
        if ret:
            DataHandler().UpdateAccountPermission(identityName,isAdmin,isAuthorized)
        resp = jsonify(ret)
        resp.headers["Access-Control-Allow-Origin"] = "*"
        resp.headers["dataType"] = "json"
        return resp
api.add_resource(UpdateUserPermission, '/UpdateUserPermission')

class DeleteUser(Resource):
    @api.expect(api.model("DeleteUser", model.DeleteUser.params))
    def delete(self):
        params = request.get_json(silent=True)
        userName = params["userName"]
        identityName = params["identityName"]
        if not AuthorizationManager.HasAccess(userName, ResourceType.Cluster, "", Permission.Admin):
            return 403
        resourceAclPath = AuthorizationManager.GetResourceAclPath("", ResourceType.Cluster)
        ret =  ACLManager.DeleteAce(identityName, resourceAclPath)
        if ret:
            ret = DataHandler().DeleteUser(identityName)
        resp = jsonify(ret)
        resp.headers["Access-Control-Allow-Origin"] = "*"
        resp.headers["dataType"] = "json"
        return resp

api.add_resource(DeleteUser, '/DeleteUser')

class GetAllUsers(Resource):
    def get(self):
        data_handler = None
        try:
            data_handler = DataHandler()
            ret = data_handler.GetUsers()
            resp = jsonify(ret)
            resp.headers["Access-Control-Allow-Origin"] = "*"
            resp.headers["dataType"] = "json"
            return resp
        except Exception as e:
            return "Internal Server Error. " + str(e), 400
        finally:
            if data_handler is not None:
                data_handler.Close()

##
## Actually setup the Api resource routing here
##
api.add_resource(GetAllUsers, '/GetAllUsers')

class GetAllAccountUser(Resource):
    def get(self):
        data_handler = None
        try:
            data_handler = DataHandler()
            ret = data_handler.GetAllAccountUser()
            resp = jsonify(ret)
            resp.headers["Access-Control-Allow-Origin"] = "*"
            resp.headers["dataType"] = "json"
            return resp
        except Exception as e:
            return "Internal Server Error. " + str(e), 400
        finally:
            if data_handler is not None:
                data_handler.Close()

api.add_resource(GetAllAccountUser, '/GetAllAccountUser')

class UpdateAce(Resource):
    @api.doc(params=model.UpdateAce.params)
    def get(self):
        parser = reqparse.RequestParser()
        parser.add_argument('userName')
        parser.add_argument('identityName')
        parser.add_argument('resourceType')
        parser.add_argument('resourceName')
        parser.add_argument('permissions')
        args = parser.parse_args()
        username = args["userName"]
        identityName = str(args["identityName"])
        resourceType = int(args["resourceType"])
        resourceName = str(args["resourceName"])
        permissions = int(args["permissions"])
        ret = {}
        ret["result"] = JobRestAPIUtils.UpdateAce(username, identityName, resourceType, resourceName, permissions)
        resp = jsonify(ret)
        resp.headers["Access-Control-Allow-Origin"] = "*"
        resp.headers["dataType"] = "json"

        return resp
##
## Actually setup the Api resource routing here
##
api.add_resource(UpdateAce, '/UpdateAce')


class DeleteAce(Resource):
    @api.doc(params=model.DeleteAce.params)
    def get(self):
        parser = reqparse.RequestParser()
        parser.add_argument('userName')
        parser.add_argument('identityName')
        parser.add_argument('resourceType')
        parser.add_argument('resourceName')
        args = parser.parse_args()
        username = args["userName"]
        identityName = str(args["identityName"])
        resourceType = int(args["resourceType"])
        resourceName = str(args["resourceName"])
        ret = {}
        ret["result"] = JobRestAPIUtils.DeleteAce(username, identityName, resourceType, resourceName)
        resp = jsonify(ret)
        resp.headers["Access-Control-Allow-Origin"] = "*"
        resp.headers["dataType"] = "json"

        return resp
##
## Actually setup the Api resource routing here
##
api.add_resource(DeleteAce, '/DeleteAce')


class IsClusterAdmin(Resource):
    @api.doc(params=model.IsClusterAdmin.params)
    def get(self):
        parser = reqparse.RequestParser()
        parser.add_argument('userName')
        args = parser.parse_args()
        username = args["userName"]
        ret = {}
        ret["result"] = AuthorizationManager.IsClusterAdmin(username)
        resp = jsonify(ret)
        resp.headers["Access-Control-Allow-Origin"] = "*"
        resp.headers["dataType"] = "json"

        return resp
##
## Actually setup the Api resource routing here
##
api.add_resource(IsClusterAdmin, '/IsClusterAdmin')


class GetACL(Resource):
    @api.doc(params=model.GetACL.params)
    def get(self):
        parser = reqparse.RequestParser()
        parser.add_argument('userName')
        args = parser.parse_args()
        username = args["userName"]
        ret = {}
        ret["result"] = AuthorizationManager.GetAcl(username)
        resp = jsonify(ret)
        resp.headers["Access-Control-Allow-Origin"] = "*"
        resp.headers["dataType"] = "json"

        return resp
##
## Actually setup the Api resource routing here
##
api.add_resource(GetACL, '/GetACL')


class GetAllACL(Resource):
    def get(self):
        ret = {}
        ret["result"] = ACLManager.GetAllAcl()
        resp = jsonify(ret)
        resp.headers["Access-Control-Allow-Origin"] = "*"
        resp.headers["dataType"] = "json"

        return resp
##
## Actually setup the Api resource routing here
##
api.add_resource(GetAllACL, '/GetAllACL')


class ListVCs(Resource):
    @api.doc(params=model.ListVCs.params)
    def get(self):
        parser = reqparse.RequestParser()
        parser.add_argument('userName')
        args = parser.parse_args()
        userName = args["userName"]
        ret = {}
        ret["result"] = JobRestAPIUtils.ListVCs(userName)
        resp = jsonify(ret)
        resp.headers["Access-Control-Allow-Origin"] = "*"
        resp.headers["dataType"] = "json"

        return resp

##
## Actually setup the Api resource routing here
##
api.add_resource(ListVCs, '/ListVCs')


class GetVC(Resource):
    @api.doc(params=model.GetVC.params)
    def get(self):
        parser = reqparse.RequestParser()
        parser.add_argument('userName')
        parser.add_argument('vcName')
        args = parser.parse_args()
        userName = args["userName"]
        vcName = args["vcName"]
        ret = JobRestAPIUtils.GetVC(userName, vcName)

        resp = jsonify(ret)
        resp.headers["Access-Control-Allow-Origin"] = "*"
        resp.headers["dataType"] = "json"

        return resp

##
## Actually setup the Api resource routing here
##
api.add_resource(GetVC, '/GetVC')


class AddVC(Resource):
    @api.doc(params=model.AddVC.params)
    def get(self):
        parser = reqparse.RequestParser()
        parser.add_argument('vcName')
        parser.add_argument('quota')
        parser.add_argument('metadata')
        parser.add_argument('userName')
        args = parser.parse_args()
        vcName = args["vcName"]
        quota = args["quota"]
        metadata = args["metadata"]
        userName = args["userName"]
        ret = {}
        ret["result"] = JobRestAPIUtils.AddVC(userName, vcName, quota, metadata)
        resp = jsonify(ret)
        resp.headers["Access-Control-Allow-Origin"] = "*"
        resp.headers["dataType"] = "json"

        return resp
##
## Actually setup the Api resource routing here
##
api.add_resource(AddVC, '/AddVC')


class DeleteVC(Resource):
    @api.doc(params=model.DeleteVC.params)
    def get(self):
        parser = reqparse.RequestParser()
        parser.add_argument('vcName')
        parser.add_argument('userName')
        args = parser.parse_args()
        vcName = args["vcName"]
        userName = args["userName"]
        ret = {}
        ret["result"] = JobRestAPIUtils.DeleteVC(userName, vcName)
        resp = jsonify(ret)
        resp.headers["Access-Control-Allow-Origin"] = "*"
        resp.headers["dataType"] = "json"

        return resp
##
## Actually setup the Api resource routing here
##
api.add_resource(DeleteVC, '/DeleteVC')


class UpdateVC(Resource):
    @api.doc(params=model.UpdateVC.params)
    def get(self):
        parser = reqparse.RequestParser()
        parser.add_argument('vcName')
        parser.add_argument('quota')
        parser.add_argument('metadata')
        parser.add_argument('userName')
        args = parser.parse_args()
        vcName = args["vcName"]
        quota = args["quota"]
        metadata = args["metadata"]
        userName = args["userName"]
        ret = {}
        ret["result"] = JobRestAPIUtils.UpdateVC(userName, vcName, quota, metadata)

        resp = jsonify(ret)
        resp.headers["Access-Control-Allow-Origin"] = "*"
        resp.headers["dataType"] = "json"

        return resp
##
## Actually setup the Api resource routing here
##
api.add_resource(UpdateVC, '/UpdateVC')


class ListStorages(Resource):
    @api.doc(params=model.ListStorages.params)
    def get(self):
        parser = reqparse.RequestParser()
        parser.add_argument('vcName')
        parser.add_argument('userName')
        args = parser.parse_args()
        vcName = args["vcName"]
        userName = args["userName"]
        ret = {}
        ret["result"] = JobRestAPIUtils.ListStorages(userName, vcName)
        resp = jsonify(ret)
        resp.headers["Access-Control-Allow-Origin"] = "*"
        resp.headers["dataType"] = "json"

        return resp
##
## Actually setup the Api resource routing here
##
api.add_resource(ListStorages, '/ListStorages')


class AddStorage(Resource):
    @api.doc(params=model.AddStorage.params)
    def get(self):
        parser = reqparse.RequestParser()
        parser.add_argument('vcName')
        parser.add_argument('storageType')
        parser.add_argument('url')
        parser.add_argument('metadata')

        parser.add_argument('defaultMountPath')
        parser.add_argument('userName')
        args = parser.parse_args()
        vcName = args["vcName"]
        storageType = args["storageType"]
        url = args["url"]

        metadata = args["metadata"]
        defaultMountPath = args["defaultMountPath"]
        userName = args["userName"]
        ret = {}
        ret["result"] = JobRestAPIUtils.AddStorage(userName, vcName, url, storageType, metadata, defaultMountPath)
        resp = jsonify(ret)
        resp.headers["Access-Control-Allow-Origin"] = "*"
        resp.headers["dataType"] = "json"

        return resp
##
## Actually setup the Api resource routing here
##
api.add_resource(AddStorage, '/AddStorage')


class DeleteStorage(Resource):
    @api.doc(params=model.DeleteStorage.params)
    def get(self):
        parser = reqparse.RequestParser()
        parser.add_argument('vcName')
        parser.add_argument('userName')
        parser.add_argument('url')
        args = parser.parse_args()
        vcName = args["vcName"]
        userName = args["userName"]
        url = args["url"]
        ret = {}
        ret["result"] = JobRestAPIUtils.DeleteStorage(userName, vcName, url)
        resp = jsonify(ret)
        resp.headers["Access-Control-Allow-Origin"] = "*"
        resp.headers["dataType"] = "json"

        return resp
##
## Actually setup the Api resource routing here
##
api.add_resource(DeleteStorage, '/DeleteStorage')


class UpdateStorage(Resource):
    @api.doc(params=model.UpdateStorage.params)
    def get(self):
        parser = reqparse.RequestParser()
        parser.add_argument('vcName')
        parser.add_argument('storageType')
        parser.add_argument('url')
        parser.add_argument('metadata')

        parser.add_argument('defaultMountPath')
        parser.add_argument('userName')
        args = parser.parse_args()
        vcName = args["vcName"]
        storageType = args["storageType"]
        url = args["url"]
        metadata = args["metadata"]
        defaultMountPath = args["defaultMountPath"]
        userName = args["userName"]
        ret = {}
        ret["result"] = JobRestAPIUtils.UpdateStorage(userName, vcName, url, storageType, metadata, defaultMountPath)

        resp = jsonify(ret)
        resp.headers["Access-Control-Allow-Origin"] = "*"
        resp.headers["dataType"] = "json"

        return resp
##
## Actually setup the Api resource routing here
##
api.add_resource(UpdateStorage, '/UpdateStorage')


class Endpoint(Resource):
    @api.doc(params=model.Endpoint.params)
    def get(self):
        '''return job["endpoints"]: curl -X GET /endpoints?jobId=...&userName=...'''
        parser = reqparse.RequestParser()
        parser.add_argument('jobId')
        parser.add_argument('userName')
        args = parser.parse_args()
        jobId = args["jobId"]
        username = args["userName"]

        ret = JobRestAPIUtils.GetEndpoints(username, jobId)

        # TODO: return 403 error code
        # Return empty list for now to keep backward compatibility with old portal.
        resp = generate_response(ret)
        return resp

    @api.doc(params=model.EndpointPost.params)
    @api.expect(api.model("Endpoint", model.EndpointPost.params2))
    def post(self):
        '''set job["endpoints"]: curl -X POST -H "Content-Type: application/json" /endpoints --data "{'jobId': ..., 'endpoints': ['ssh', 'ipython'] }"'''
        parser = reqparse.RequestParser()
        parser.add_argument('userName')
        args = parser.parse_args()
        username = args["userName"]

        params = request.get_json(silent=True)
        job_id = params["jobId"]
        requested_endpoints = params["endpoints"]

        interactive_ports = []
        # endpoints should be ["ssh", "ipython", "tensorboard", {"name": "port name", "podPort": "port on pod in 40000-49999"}]
        for interactive_port in [ elem for elem in requested_endpoints if elem not in ["ssh", "ipython", "tensorboard"] ]:
            if any(required_field not in interactive_port for required_field in ["name", "podPort"]):
                # if ["name", "port"] not in interactive_port:
                return ("Bad request, interactive port should have \"name\" and \"podPort\"]: %s" % requested_endpoints), 400
            if int(interactive_port["podPort"]) < 40000 or int(interactive_port["podPort"]) > 49999:
                return ("Bad request, interactive podPort should in range 40000-49999: %s" % requested_endpoints), 400
            if len(interactive_port["name"]) > 16:
                return ("Bad request, interactive port name length shoule be less than 16: %s" % requested_endpoints), 400
            interactive_ports.append(interactive_port)

        msg, statusCode = JobRestAPIUtils.UpdateEndpoints(username, job_id, requested_endpoints, interactive_ports)
        if statusCode != 200:
            return msg, statusCode

        resp = generate_response(msg)
        return resp


##
## Actually setup the Endpoint resource routing here
##
api.add_resource(Endpoint, '/endpoints')


class Templates(Resource):
    @api.doc(params=model.Templates.params)
    def get(self):
        parser = reqparse.RequestParser()
        parser.add_argument('vcName', location="args")
        parser.add_argument('userName', location="args")
        args = parser.parse_args()
        vcName = args["vcName"]
        userName = args["userName"]

        dataHandler = DataHandler()
        ret = dataHandler.GetTemplates("master") or []
        ret += dataHandler.GetTemplates("vc:" + vcName) or []
        ret += dataHandler.GetTemplates("user:" + userName) or []
        dataHandler.Close()
        resp = jsonify(ret)
        resp.headers["Access-Control-Allow-Origin"] = "*"
        resp.headers["dataType"] = "json"

        return resp

    @api.doc(params=model.TemplatesPost.params)
    def post(self):
        parser = reqparse.RequestParser()
        parser.add_argument('vcName', location="args")
        parser.add_argument('userName', location="args")
        parser.add_argument('database', location="args")
        parser.add_argument('templateName', location="args")
        args = parser.parse_args()
        vcName = args["vcName"]
        userName = args["userName"]
        database = args["database"]
        templateName = args["templateName"]

        if database == 'master':
            if AuthorizationManager.HasAccess(userName, ResourceType.Cluster, "", Permission.Admin):
                scope = 'master'
            else:
                return 'access denied', 403;
        elif database == 'vc':
            if AuthorizationManager.HasAccess(userName, ResourceType.VC, vcName, Permission.Admin):
                scope = 'vc:' + vcName
            else:
                return 'access denied', 403;
        else:
            scope = 'user:' + userName
        template_json = request.json

        if template_json is None:
            return jsonify(result=False, message="Invalid JSON")

        dataHandler = DataHandler()
        ret = {}
        ret["result"] = dataHandler.UpdateTemplate(templateName, scope, json.dumps(template_json))
        dataHandler.Close()
        resp = jsonify(ret)
        resp.headers["Access-Control-Allow-Origin"] = "*"
        resp.headers["dataType"] = "json"

        return resp

    @api.doc(params=model.TemplatesDelete.params)
    def delete(self):
        parser = reqparse.RequestParser()
        parser.add_argument('vcName', location="args")
        parser.add_argument('userName', location="args")
        parser.add_argument('database', location="args")
        parser.add_argument('templateName', location="args")
        args = parser.parse_args()
        vcName = args["vcName"]
        userName = args["userName"]
        database = args["database"]
        templateName = args["templateName"]

        if database == 'master':
            if AuthorizationManager.HasAccess(userName, ResourceType.Cluster, "", Permission.Admin):
                scope = 'master'
            else:
                return 'access denied', 403;
        elif database == 'vc':
            if AuthorizationManager.HasAccess(userName, ResourceType.VC, vcName, Permission.Admin):
                scope = 'vc:' + vcName
            else:
                return 'access denied', 403;
        else:
            scope = 'user:' + userName

        dataHandler = DataHandler()
        ret = {}
        ret["result"] = dataHandler.DeleteTemplate(templateName, scope)
        dataHandler.Close()
        resp = jsonify(ret)
        resp.headers["Access-Control-Allow-Origin"] = "*"
        resp.headers["dataType"] = "json"

        return resp

api.add_resource(Templates, '/templates')


class JobPriority(Resource):
    def get(self):
        job_priorites = JobRestAPIUtils.get_job_priorities()
        resp = jsonify(job_priorites)
        resp.headers["Access-Control-Allow-Origin"] = "*"
        resp.headers["dataType"] = "json"
        return resp

    @api.doc(params=model.JobPriority.params)
    def post(self):
        parser = reqparse.RequestParser()
        parser.add_argument('userName', location="args")
        args = parser.parse_args()
        username = args["userName"]

        payload = request.get_json(silent=True)
        success, all_job_priorities = JobRestAPIUtils.update_job_priorites(username, payload)
        http_status = 200 if success else 400

        # Only return job_priorities affected in the POST request
        job_priorities = {}
        for job_id, _ in payload.items():
            if job_id in all_job_priorities:
                job_priorities[job_id] = all_job_priorities[job_id]
            else:
                job_priorities[job_id] = JobRestAPIUtils.DEFAULT_JOB_PRIORITY

        resp = jsonify(job_priorities)
        resp.headers["Access-Control-Allow-Origin"] = "*"
        resp.headers["dataType"] = "json"
        resp.status_code = http_status
        return resp

##
## Actually setup the Api resource routing here
##
api.add_resource(JobPriority, '/jobs/priorities')



def dumpstacks(signal, frame):
    code = []
    logging.info("received signum %d", signal)
    logging.info("db pools connections: [%s]", str(MysqlConn.connection_statics()))
    # logging.info("\nfeature_count:\n{}".format(feature_count))
    for threadId, stack in sys._current_frames().items():
        code.append("n# Thread: %d" % (threadId))
        for filename, lineno, name, line in traceback.extract_stack(stack):
            code.append('File:"%s", line %d, in %s' % (filename, lineno, name))
            if line:
                code.append(" %s" % (line.strip()))
    logging.info("\n".join(code))


@app.route("/metrics")
def metrics():
    return Response(prometheus_client.generate_latest(), mimetype=CONTENT_TYPE_LATEST)

if __name__ == '__main__':
    signal.signal(signal.SIGUSR2, dumpstacks)
    app.run(debug=False,host="0.0.0.0",threaded=True)

