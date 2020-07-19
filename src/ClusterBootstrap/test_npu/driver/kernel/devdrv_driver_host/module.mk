

LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := drv_devdrv_host

LOCAL_KO_SRC_FOLDER := $(LOCAL_PATH)
LOCAL_DEPEND_KO:=drv_devmm_host drv_devmng_host
LOCAL_INSTALLED_KO_FILES := drv_devdrv_host.ko

include $(BUILD_HOST_KO)
