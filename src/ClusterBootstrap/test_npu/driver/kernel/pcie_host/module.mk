
LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := drv_pcie_host

LOCAL_KO_SRC_FOLDER := $(LOCAL_PATH)
LOCAL_DEPEND_KO:=drv_seclib_host
LOCAL_INSTALLED_KO_FILES := drv_pcie_host.ko

include $(BUILD_HOST_KO)
