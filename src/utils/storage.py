#!/usr/bin/python3

import string

class StorageConfig(object):

    support_list = ["nfs", "ceph"]

    def __init__(self):
        super(StorageConfig, self).__init__()
        return

    @staticmethod
    def get_pvc_name(storage_type):
        
        storage_type = storage_type.lower()

        if StorageConfig._storage_supported(storage_type):
            return StorageConfig._get_pvc(storage_type)
        else:
            return None

    @staticmethod
    def get_pv_name(storage_type):
        
        storage_type = storage_type.lower()
        if StorageConfig._storage_supported(storage_type):
            return StorageConfig._get_storage_name(storage_type)
        else:
            return None

    @staticmethod
    def _get_pvc(storage_type):
        return storage_type + "-pvc"

    @staticmethod
    def _get_storage_name(storage_type):
        return storage_type + "-storage"

    @staticmethod
    def _storage_supported(storage_type):
        if storage_type in StorageConfig.support_list:
            return True
        else:
            return False