import re
import base64
import yaml
import logging
from config import config

def getNodename():
    domain = config["domain"]
    if "endpoint_use_short_domian" in config:
        return config["endpoint_use_short_domian"],domain
    elif "master_private_ip" in config:
        return config["master_private_ip"].split(".", 1)
    else:
        return config["webportal_node"].split("." + domain)[0],domain

def parse_endpoint(endpoint,job=None):
    epItem = {
        "id": endpoint["id"],
        "name": endpoint["name"],
        "username": endpoint["username"],
        "status": endpoint["status"],
        "hostNetwork": endpoint["hostNetwork"],
        "podName": endpoint["podName"],
        "domain": config["domain"],
    }
    if "podPort" in endpoint:
        epItem["podPort"] = endpoint["podPort"]
    if endpoint["status"] == "running":
        port = int(endpoint["endpointDescription"]["spec"]["ports"][0]["nodePort"])
        epItem["port"] = port
        if "nodeName" in endpoint:
            epItem["nodeName"],epItem["domain"] = getNodename()
        if epItem["name"] == "ssh":
            try:
                if job:
                    desc = list(yaml.full_load_all(base64.b64decode(job["jobDescription"])))
                    if epItem["id"].find("worker") != -1:
                        desc = desc[int(re.match(".*worker(\d+)-ssh", epItem["id"]).groups()[0]) + 1]
                    elif epItem["id"].find("ps0") != -1:
                        desc = desc[0]
                    else:
                        desc = desc[0]
                    for i in desc["spec"]["containers"][0]["env"]:
                        if i["name"] == "DLTS_JOB_TOKEN":
                            epItem["password"] = i["value"]
            except Exception as e:
                logging.error(e)
        elif epItem["name"] == "inference-url":
            epItem["modelname"] = endpoint["modelname"]
            epItem["port"] = base64.b64encode(str(epItem["port"]).encode("utf-8"))
        elif epItem["name"] == "ipython" or epItem["name"] == "tensorboard":
            epItem["port"] = base64.b64encode(str(epItem["port"]).encode("utf-8"))
    if epItem["name"] in ["ipython","tensorboard","inference-url"]:
        if "extranet_port" in config and config["extranet_port"]:
            epItem["domain"] = epItem["domain"] + ":" + str(config["extranet_port"])
    return epItem