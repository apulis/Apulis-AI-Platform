#!/usr/bin/python3

import string

STORAGE_TYPE_APP_DATA = 1
STORAGE_TYPE_COMPONENT_DATA = 2
STORAGE_TYPE_MODEL_DATA = 3
STORAGE_TYPE_LABEL_DATA = 4

class StorageConfig(object):

    storage_info = {
        STORAGE_TYPE_APP_DATA : {
            "pv":"aiplatform-app-data-pv",
            "pvc":"aiplatform-app-data-pvc",
        },
        STORAGE_TYPE_COMPONENT_DATA : {
            "pv":"aiplatform-component-data-pv",
            "pvc":"aiplatform-component-data-pvc",
        },
        STORAGE_TYPE_MODEL_DATA : {
            "pv":"aiplatform-model-data-pv",
            "pvc":"aiplatform-model-data-pvc",
        },
        STORAGE_TYPE_LABEL_DATA : {
            "pv":"aiplatform-label-data-pv",
            "pvc":"aiplatform-label-data-pvc",
        }
    }

    def __init__(self):
        super(StorageConfig, self).__init__()
        return

    @staticmethod
    def get_pvc_name(storage_type):
        
        if StorageConfig._storage_supported(storage_type):
            return StorageConfig._get_pvc(storage_type)
        else:
            return None

    @staticmethod
    def get_pv_name(storage_type):
        
        if StorageConfig._storage_supported(storage_type):
            return StorageConfig._get_storage_name(storage_type)
        else:
            return None

    @staticmethod
    def _get_pvc(storage_type):
        return StorageConfig.storage_info[storage_type]["pvc"]

    @staticmethod
    def _get_storage_name(storage_type):
        return StorageConfig.storage_info[storage_type]["pv"]

    @staticmethod
    def _storage_supported(storage_type):
        if storage_type in StorageConfig.storage_info:
            return True
        else:
            return False