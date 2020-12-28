import os
import ntpath
import json
import time
from DataHandler import DataHandler, DataManager
from datetime import datetime
import requests
from requests.auth import HTTPBasicAuth
import logging

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
    if fdinfo is None:
        ret["err"] = "FD server not set"
        return ret
    modconvertInfo = GetModelConversionInfo(jobId)
    if modconvertInfo is None:
        ret["err"] = "Job not exists"
        return ret
    fileId = modconvertInfo["fileId"]
    logging.info("modconvertInfo: %s", modconvertInfo)
    if fileId is None or fileId == '' or fileId == 'None' or fileId == "push failed":
        create_file_res = fd_create_file(modconvertInfo, fdinfo)
        if create_file_res is False:
            ret["err"] = "Faild to create file from fd"
            return ret
        modconvertInfo = GetModelConversionInfo(jobId)
    return fd_push_file(modconvertInfo, fdinfo)

def check_FD_image_file_is_created(modconvertInfo,fdinfo):
    headers = {
        "Version": "v" + datetime.now().strftime("%Y%m%d%H%M%S"),
        "Description": modconvertInfo["jobId"] + " model file",
    }
    url = fdinfo["url"] + "/redfish/v1/rich/AppDeployService/ResourceFiles/"+modconvertInfo["fileId"]
    auth = HTTPBasicAuth(fdinfo['username'], fdinfo['password'])
    try:
        resp = requests.get(url, headers=headers,auth=auth, verify=False)
        logging.info("fd_create_file: %d, %s", resp.status_code, resp.json())
        if resp.status_code == 200:
            return True
    except Exception as e:
        pass
    return False

def find_file_id(name,fdinfo):
    url = fdinfo["url"] + "/redfish/v1/rich/AppDeployService/ResourceFiles"
    auth = HTTPBasicAuth(fdinfo['username'], fdinfo['password'])
    try:
        resp = requests.get(url, auth=auth, verify=False)
        if resp.status_code == 200:
            for one in resp.json().get("Members",[]):
                if one["Name"] == name:
                    return one["FileID"]
    except Exception as e:
        pass
    return None

def fd_create_file(modconvertInfo, fdinfo):
    dataHandler = DataHandler()
    # if outpath exists, use the same file id.
    existPathInfo = dataHandler.GetModelConvertInfoByOutputpath(modconvertInfo["outputPath"])
    if existPathInfo is not None and existPathInfo["fileId"] != "push failed":
        fileId = existPathInfo["fileId"]
        dataHandler.UpdateModelConversionFileId(modconvertInfo['jobId'], fileId)
        if check_FD_image_file_is_created(existPathInfo,fdinfo):
            return True

    url = fdinfo["url"] + "/redfish/v1/rich/AppDeployService/ResourceFiles"
    auth = HTTPBasicAuth(fdinfo['username'], fdinfo['password'])
    headers = {
        "Version": "v" + datetime.now().strftime("%Y%m%d%H%M%S"),
        "Description": modconvertInfo["jobId"] + " model file",
    }
    data = {
        'Name': get_filename(modconvertInfo["outputPath"]),
        'Description': modconvertInfo["jobId"] + " model file",
        'Type': 'model_file'
    }
    try:
        resp = requests.post(url, headers=headers,auth=auth, verify=False, data=json.dumps(data))
        logging.info("fd_create_file: %d, %s", resp.status_code, resp.json())
        if resp.status_code == 201:
            fileId = resp.json()['FileID']
            dataHandler.UpdateModelConversionFileId(modconvertInfo['jobId'], fileId)
            return True
        elif resp.status_code == 400:
            if resp.json()['error']["@Message.ExtendedInfo"][0]["MessageId"]=="Base.1.0.ResourceAlreadyExists":
                fileId = find_file_id(get_filename(modconvertInfo["outputPath"]),fdinfo)
                dataHandler.UpdateModelConversionFileId(modconvertInfo['jobId'], fileId)
                return True
        else:
            dataHandler.UpdateModelConversionFileId(modconvertInfo["jobId"], "push failed")
            return False
    except Exception as e:
        dataHandler.UpdateModelConversionFileId(modconvertInfo["jobId"], "push failed")
        return False

def fd_push_file(modconvertInfo, fdinfo):
    ret = {}
    dataHandler = DataHandler()
    dataHandler.UpdateModelConversionStatus(modconvertInfo['jobId'], "pushing")
    current_version = fd_get_version_num(modconvertInfo["fileId"], fdinfo)
    if current_version >= 32:
        ret["success"] = False
        ret["msg"] = "fd only support less than 32 versions"
        return ret

    version = 'v' + str(current_version + 1)
    url = fdinfo["url"] + "/redfish/v1/rich/AppDeployService/ResourceFiles/" + modconvertInfo["fileId"] + '/Versions'
    auth = HTTPBasicAuth(fdinfo['username'], fdinfo['password'])
    headers = {
        "Version": "v" + datetime.now().strftime("%Y%m%d%H%M%S"),
        "Description": modconvertInfo["jobId"] + " model file",
    }
    files = {
        "tiFile": (get_filename(modconvertInfo["outputPath"]), open(get_om_filepath(modconvertInfo["outputPath"]), 'rb'))
    }
    try:
        resp = requests.post(url, auth=auth, verify=False, headers=headers, files=files)
        logging.info("fd_push_file: %d, %s", resp.status_code, resp.json())
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

def fd_get_version_num(fileId, fdinfo):
    url = fdinfo["url"] + "/redfish/v1/rich/AppDeployService/ResourceFiles/" + fileId
    auth = HTTPBasicAuth(fdinfo['username'], fdinfo['password'])
    try:
        resp = requests.get(url, auth=auth, verify=False)
        if resp.status_code == 200:
            return len(resp.json()['Versions'])
        return 0
    except Exception:
        return 0

def get_filename(filepath):
    head, tail = ntpath.split(filepath)
    return tail + ".om"

def get_om_filepath(filepath):
    return filepath + ".om"
