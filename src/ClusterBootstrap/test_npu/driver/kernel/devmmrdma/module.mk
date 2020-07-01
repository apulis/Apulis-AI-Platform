
LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

ifeq ($(TARGET_PRODUCT), mini)
    ifeq ($(TARGET_CHIP_ID), hi1951)
        LOCAL_MODULE := drv_devmm_rdma_host
    endif
else
    LOCAL_MODULE := devmm_rdma
endif

LOCAL_KO_SRC_FOLDER := $(LOCAL_PATH)

LOCAL_DEPEND_KO := drv_devmm_host

ifeq ($(TARGET_PRODUCT), mini)
    ifeq ($(TARGET_CHIP_ID), hi1951)
        LOCAL_INSTALLED_KO_FILES := drv_devmm_rdma_host.ko
    endif
else
    LOCAL_INSTALLED_KO_FILES := devmm_rdma.ko
endif

include $(BUILD_HOST_KO)


include $(CLEAR_VARS)

ifeq ($(TARGET_PRODUCT), mini)
    ifeq ($(TARGET_CHIP_ID), hi1951)
        LOCAL_MODULE := drv_devmm_rdma_host
    endif
else
    LOCAL_MODULE := devmm_rdma
endif

LOCAL_KO_SRC_FOLDER := $(LOCAL_PATH)

LOCAL_DEPEND_KO := drv_devmm

ifeq ($(TARGET_PRODUCT), mini)
    ifeq ($(TARGET_CHIP_ID), hi1951)
        LOCAL_INSTALLED_KO_FILES := drv_devmm_rdma_host.ko
    endif
else
    LOCAL_INSTALLED_KO_FILES := devmm_rdma.ko
endif

include $(BUILD_DEVICE_KO)
