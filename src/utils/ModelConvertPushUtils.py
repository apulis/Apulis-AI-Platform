import json
import time
from pathlib import Path
from DataHandler import DataHandler, DataManager
import requests
from requests.auth import HTTPBasicAuth

def loadParams(paramsJson):
    return json.loads(paramsJson)

def SetFDInfo(paramsJson):
    ret = {}
    params = loadParams(paramsJson)
    dataHandler = DataHandler()
    dataHandler.SetFDInfo(params)
    return ret

def GetFDInfo():
    dataHander = DataHandler()
    return dataHander.GetFDInfo()

def GetModelConversionInfo(jobId):
    dataHander = DataHandler()
    return dataHander.GetModelConvertInfo(jobId)

def PushModelToFD(paramsJson):
    ret = {}
    params = loadParams(paramsJson)
    jobId = params['jobId']
    fdinfo = GetFDInfo()
    modconvertInfo = GetModelConvertInfo(jobId)
    if modconvertInfo is None:
        ret["err"] = "Job not exists"
        return ret
    if modconvertInfo["fileId"] is None or modconvertInfo["fileId"] == '':
        if fd_create_file(modconvertInfo, fdinfo) is not True:
            ret["err"] = "Faild to create file from fd"
            return ret
    return fd_push_file(modconvertInfo, fdinfo)

def fd_create_file(modconvertInfo, fdinfo):
    url = fdinfo["url"] + "/redfish/v1/rich/AppDeployService/ResourceFiles"
    auth = HTTPBasicAuth(fdinfo['username'], fdinfo['password'])
    data = {
        'Name': get_filename(modconvertInfo["outputPath"]),
        'Description': modconvertInfo["jobId"] + " model file",
        'Type': 'model file'
    }
    resp = requests.post(url, auth=auth, verify=False, data=json.dumps(data))
    if resp["status_code"] == 201:
        fileId = resp.json()['FileID']
        dataHander = DataHandler()
        dataHander.UpdateModelConversionFileId(modconvertInfo['jobId'], fileId)
        return True
    else:
        return False

def fd_push_file(modconvertInfo, fdinfo):
    ret = {}
    url = fdinfo["url"] + "/redfish/v1/rich/AppDeployService/ResourceFiles/" + modconvertInfo["fileId"] + '/Versions'
    auth = HTTPBasicAuth(fdinfo['username'], fdinfo['password'])
    headers = {
        "Version": str(int(time.time())),
        "Description": modconvertInfo["jobId"] + " model file",
    }
    files = {
        "tiFile": (get_filename(modconvertInfo["outputPath"], open(modconvertInfo["outputPath"], 'rb')))
    }
    resp = requests.post(url, auth=auth, verify=False, headers=headers, files=files)
    if resp["status_code"] == 201:
        ret["success"] = True
    else:
        ret["success"] = False
        ret["msg"] = resp['error']
    return ret

def get_filename(filepath):
    return Path(filepath).name
