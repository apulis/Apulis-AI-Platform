#!/usr/bin/python3

import string

# default namespace 
PVC_TYPE_APP_DATA = 1
PVC_TYPE_COMPONENT_DATA = 2
PVC_TYPE_MODEL_DATA = 3
PVC_TYPE_LABEL_DATA = 4

# kfserving-pod namespace
PVC_TYPE_MODEL_DATA_KFSERVING_POD = 5

# 
class StorageConfig(object):

    pvc_info = {
        PVC_TYPE_APP_DATA : {
            "pv":"aiplatform-app-data-pv",
            "pvc":"aiplatform-app-data-pvc",
        },
        PVC_TYPE_COMPONENT_DATA : {
            "pv":"aiplatform-component-data-pv",
            "pvc":"aiplatform-component-data-pvc",
        },
        PVC_TYPE_MODEL_DATA : {
            "pv":"aiplatform-model-data-pv",
            "pvc":"aiplatform-model-data-pvc",
        },
        PVC_TYPE_LABEL_DATA : {
            "pv":"aiplatform-label-data-pv",
            "pvc":"aiplatform-label-data-pvc",
        },
        PVC_TYPE_MODEL_DATA_KFSERVING_POD : {
            "pv":"aiplatform-model-data-pv-kfserving-pod",
            "pvc":"aiplatform-model-data-pvc-kfserving-pod",
        }
    }

    def __init__(self):
        super(StorageConfig, self).__init__()
        return

    @staticmethod
    def get_pvc_name(pvc_type):
        
        if StorageConfig._pvc_supported(pvc_type):
            return StorageConfig._get_pvc(pvc_type)
        else:
            return None

    @staticmethod
    def get_pv_name(pvc_type):
        
        if StorageConfig._pvc_supported(pvc_type):
            return StorageConfig._get_pv_name(pvc_type)
        else:
            return None

    @staticmethod
    def _get_pvc(pvc_type):
        return StorageConfig.pvc_info[pvc_type]["pvc"]

    @staticmethod
    def _get_pv_name(pvc_type):
        return StorageConfig.pvc_info[pvc_type]["pv"]

    @staticmethod
    def _pvc_supported(pvc_type):
        if pvc_type in StorageConfig.pvc_info:
            return True
        else:
            return False