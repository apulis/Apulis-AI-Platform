ifeq ($(DAVINCI_HIAI_DKMS),y)

        EXTRA_CFLAGS += -I$(HIAI_DKMS_DIR)/inc/driver
        EXTRA_CFLAGS += -I$(HIAI_DKMS_DIR)/dev_inc
        EXTRA_CFLAGS += -I$(HIAI_DKMS_DIR)/npu_inc
        EXTRA_CFLAGS += -I$(HIAI_DKMS_DIR)/devdrv_driver_host
        EXTRA_CFLAGS += -I$(HIAI_DKMS_DIR)/device_manager_inc

        ifeq ($(TARGET_PRODUCT),cloud)
                EXTRA_CFLAGS += -DCFG_SOC_PLATFORM_CLOUD
                EXTRA_CFLAGS += -I$(HIAI_DKMS_DIR)/inc/soc/cloud_v1
                EXTRA_CFLAGS += -I$(HIAI_DKMS_DIR)/devdrv_platform_host/cloud
                EXTRA_CFLAGS += -DCFG_SOC_PLATFORM_CLOUD_HOST
        else  ifeq ($(TARGET_PRODUCT),mini)
                EXTRA_CFLAGS += -DCFG_SOC_PLATFORM_MINI
                EXTRA_CFLAGS += -I$(HIAI_DKMS_DIR)/inc/soc/mini_v1
                ifeq ($(TARGET_CHIP_ID), hi1951)
                EXTRA_CFLAGS += -DCFG_SOC_PLATFORM_MINIV2
                EXTRA_CFLAGS += -I$(HIAI_DKMS_DIR)/devdrv_platform_host/miniv2
                else
                EXTRA_CFLAGS += -I$(HIAI_DKMS_DIR)/devdrv_platform_host/mini
                endif
        else
                EXTRA_CFLAGS += -DCFG_SOC_PLATFORM_MINI
                EXTRA_CFLAGS += -I$(HIAI_DKMS_DIR)/inc/soc/mini_v1
                ifeq ($(TARGET_CHIP_ID), hi1951)
                EXTRA_CFLAGS += -DCFG_SOC_PLATFORM_MINIV2
                EXTRA_CFLAGS += -I$(HIAI_DKMS_DIR)/devdrv_platform_host/miniv2
                else
                EXTRA_CFLAGS += -I$(HIAI_DKMS_DIR)/devdrv_platform_host/mini
                endif
        endif
else

	CUR_MAKEFILE_PATH := $(strip \
                $(eval LOCAL_MODULE_MAKEFILE := $$(lastword $$(MAKEFILE_LIST))) \
                $(patsubst %/,%, $(dir $(LOCAL_MODULE_MAKEFILE))) \
                )

        EXTRA_CFLAGS += -I$(CUR_MAKEFILE_PATH)/../../../../../inc/driver
        EXTRA_CFLAGS += -I$(CUR_MAKEFILE_PATH)/../../../../../drivers/dev_inc
        EXTRA_CFLAGS += -I$(CUR_MAKEFILE_PATH)/../../../../../drivers/npu_inc
        EXTRA_CFLAGS += -I$(CUR_MAKEFILE_PATH)/../devdrv_driver_host/
        EXTRA_CFLAGS += -I$(CUR_MAKEFILE_PATH)/../../device_manager_inc/

	ifeq ($(TARGET_PRODUCT),cloud)
		EXTRA_CFLAGS += -DCFG_SOC_PLATFORM_CLOUD
		EXTRA_CFLAGS += -I$(CUR_MAKEFILE_PATH)/../../../../../inc/soc/cloud_v1
		EXTRA_CFLAGS += -I$(CUR_MAKEFILE_PATH)/../../device_manager_slave/devdrv_platform_slave/cloud
                EXTRA_CFLAGS += -DCFG_SOC_PLATFORM_CLOUD_HOST
	else
		EXTRA_CFLAGS += -DCFG_SOC_PLATFORM_MINI
		EXTRA_CFLAGS += -I$(CUR_MAKEFILE_PATH)/../../../../../inc/soc/mini_v1
		ifeq ($(TARGET_CHIP_ID), hi1951)
		EXTRA_CFLAGS += -DCFG_SOC_PLATFORM_MINIV2
		EXTRA_CFLAGS += -I$(CUR_MAKEFILE_PATH)/../devdrv_platform_host/miniv2
		else
		EXTRA_CFLAGS += -I$(CUR_MAKEFILE_PATH)/../devdrv_platform_host/mini
		endif
	endif

endif

obj-m += drv_devmng_host.o
drv_devmng_host-objs := devdrv_manager.o devdrv_host_drvops.o devdrv_manager_msg.o
drv_devmng_host-objs += devdrv_pm.o devdrv_ipc_msg.o devdrv_driver_pm.o
drv_devmng_host-objs += devdrv_manager_dfx.o devdrv_manager_rand.o
drv_devmng_host-objs += devdrv_black_box.o devdrv_nsm.o
drv_devmng_host-objs += devdrv_manager_container.o devdrv_ipc_notify.o
drv_devmng_host-objs += devdrv_info.o devdrv_pcie.o
drv_devmng_host-objs += devdrv_device_online.o

ifeq ($(TARGET_PRODUCT),cloud)
	ccflags-y += -Wall -Werror
endif

