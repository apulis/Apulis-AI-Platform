import os
import ntpath
import json
import time
from DataHandler import DataHandler, DataManager
import requests
from requests.auth import HTTPBasicAuth

def SetFDInfo(params):
    dataHandler = DataHandler()
    return dataHandler.SetFDInfo(params)

def GetFDInfo():
    dataHander = DataHandler()
    return dataHander.GetFDInfo()

def GetModelConversionInfo(jobId):
    dataHander = DataHandler()
    return dataHander.GetModelConvertInfo(jobId)

def PushModelToFD(params):
    ret = {}
    jobId = params['jobId']
    fdinfo = GetFDInfo()
    modconvertInfo = GetModelConversionInfo(jobId)
    if modconvertInfo is None:
        ret["err"] = "Job not exists"
        return ret
    fileId = modconvertInfo["fileId"]
    if fileId is None or fileId == '' or fileId == 'None':
        create_file_res = fd_create_file(modconvertInfo, fdinfo)
        if create_file_res is False:
            ret["err"] = "Faild to create file from fd"
            return ret
        modconvertInfo = GetModelConversionInfo(jobId)
    return fd_push_file(modconvertInfo, fdinfo)

def fd_create_file(modconvertInfo, fdinfo):
    dataHandler = DataHandler()
    # if outpath exists, use the same file id.
    existPathInfo = dataHandler.GetModelConvertInfoByOutputpath(modconvertInfo["outputPath"])
    if existPathInfo is not None:
        fileId = existPathInfo["fileId"]
        dataHandler.UpdateModelConversionFileId(modconvertInfo['jobId'], fileId)
        return True
    # create file from fd
    url = fdinfo["url"] + "/redfish/v1/rich/AppDeployService/ResourceFiles"
    auth = HTTPBasicAuth(fdinfo['username'], fdinfo['password'])
    data = {
        'Name': get_filename(modconvertInfo["outputPath"]),
        'Description': modconvertInfo["jobId"] + " model file",
        'Type': 'model_file'
    }
    resp = requests.post(url, auth=auth, verify=False, data=json.dumps(data))
    if resp.status_code == 201:
        fileId = resp.json()['FileID']
        dataHandler.UpdateModelConversionFileId(modconvertInfo['jobId'], fileId)
        return True
    else:
        return False

def fd_push_file(modconvertInfo, fdinfo):
    ret = {}
    dataHandler = DataHandler()
    dataHandler.UpdateModelConversionStatus(modconvertInfo['jobId'], "pushing")
    url = fdinfo["url"] + "/redfish/v1/rich/AppDeployService/ResourceFiles/" + modconvertInfo["fileId"] + '/Versions'
    auth = HTTPBasicAuth(fdinfo['username'], fdinfo['password'])
    headers = {
        "Version": str(int(time.time())),
        "Description": modconvertInfo["jobId"] + " model file",
    }
    files = {
        "tiFile": (get_filename(modconvertInfo["outputPath"]), open(get_om_filepath(modconvertInfo["outputPath"]), 'rb'))
    }
    try:
        resp = requests.post(url, auth=auth, verify=False, headers=headers, files=files)
        if resp.status_code == 201:
            ret["success"] = True
            dataHandler.UpdateModelConversionStatus(modconvertInfo['jobId'], "push success")
        else:
            ret["success"] = False
            ret["msg"] = resp.json()['error']
            dataHandler.UpdateModelConversionStatus(modconvertInfo['jobId'], "push failed")
    except Exception as e:
        ret["success"] = False
        ret["msg"] = str(e)
        dataHandler.UpdateModelConversionStatus(modconvertInfo['jobId'], "push failed")
    return ret

def get_filename(filepath):
    head, tail = ntpath.split(filepath)
    return tail + ".om"

def get_om_filepath(filepath):
    return filepath + ".om"
