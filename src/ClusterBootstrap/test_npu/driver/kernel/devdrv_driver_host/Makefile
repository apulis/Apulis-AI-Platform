
ifeq ($(TARGET_PRODUCT),cloud)
	ccflags-y += -Wall -Werror
endif

EXTRA_CFLAGS += -DCFG_MANAGER_HOST_ENV
ifeq ($(DAVINCI_HIAI_DKMS),y)
	EXTRA_CFLAGS += -I$(HIAI_DKMS_DIR)/devdrv_driver_host
	EXTRA_CFLAGS += -I$(HIAI_DKMS_DIR)/devdrv_manager_host
	EXTRA_CFLAGS += -I$(HIAI_DKMS_DIR)/device_manager_inc
	EXTRA_CFLAGS += -I$(HIAI_DKMS_DIR)/npu_inc
	EXTRA_CFLAGS += -I$(HIAI_DKMS_DIR)/dev_inc

        ifeq ($(TARGET_PRODUCT),cloud)
                EXTRA_CFLAGS += -I$(HIAI_DKMS_DIR)/inc/soc/cloud_v1
                EXTRA_CFLAGS += -I$(HIAI_DKMS_DIR)/devdrv_platform_host/cloud
                EXTRA_CFLAGS += -DCFG_SOC_PLATFORM_CLOUD_HOST
        else ifeq ($(TARGET_PRODUCT),mini)
                EXTRA_CFLAGS += -DCFG_SOC_PLATFORM_MINI
                EXTRA_CFLAGS += -I$(HIAI_DKMS_DIR)/inc/soc/mini_v1
                ifeq ($(TARGET_CHIP_ID), hi1951)
                EXTRA_CFLAGS += -DCFG_SOC_PLATFORM_MINIV2
                EXTRA_CFLAGS += -I$(HIAI_DKMS_DIR)/devdrv_platform_host/miniv2
                else
                EXTRA_CFLAGS += -I$(HIAI_DKMS_DIR)/devdrv_platform_host/mini
                endif
        else
                EXTRA_CFLAGS += -I$(HIAI_DKMS_DIR)/inc/soc/mini_v1
                ifeq ($(TARGET_CHIP_ID), hi1951)
                EXTRA_CFLAGS += -DCFG_SOC_PLATFORM_MINIV2
                EXTRA_CFLAGS += -I$(HIAI_DKMS_DIR)/devdrv_platform_host/miniv2
                else
                EXTRA_CFLAGS += -I$(HIAI_DKMS_DIR)/devdrv_platform_host/mini
                endif
        endif

        ifeq ($(TARGET_PRODUCT),cloud)
                obj-m += drv_devdrv_host.o
        else ifeq ($(TARGET_PRODUCT),mini)
                obj-m += drv_devdrv_host.o
        else
                obj-$(CONFIG_HOST_DEVDRV_DEVDRIVER) += drv_devdrv_host.o
        endif

	drv_devdrv_host-objs := devdrv_cqsq.o devdrv_devinit.o devdrv_svmapi.o devdrv_config_p2p.o
	drv_devdrv_host-objs += devdrv_functional_cqsq.o devdrv_mailbox.o
	drv_devdrv_host-objs += devdrv_id.o devdrv_recycle.o devdrv_shm.o
else

	CUR_MAKEFILE_PATH := $(strip \
                $(eval LOCAL_MODULE_MAKEFILE := $$(lastword $$(MAKEFILE_LIST))) \
                $(patsubst %/,%, $(dir $(LOCAL_MODULE_MAKEFILE))) \
                )

	ifeq ($(CONFIG_HOST_DEVDRV_DEVDRIVER),)
		ifeq ($(CONFIG_NPU_DEVDRV),)
		CONFIG_HOST_DEVDRV_DEVDRIVER=m
		endif
	endif

	EXTRA_CFLAGS += -I$(CUR_MAKEFILE_PATH)/../../../../../drivers/dev_inc
	EXTRA_CFLAGS += -I$(CUR_MAKEFILE_PATH)/../../../../../drivers/npu_inc
	EXTRA_CFLAGS += -I$(CUR_MAKEFILE_PATH)/../../../../../drivers/dev_core/device_manager/device_manager_host/devdrv_driver_host
	EXTRA_CFLAGS += -I$(CUR_MAKEFILE_PATH)/../../../../../drivers/dev_core/device_manager/device_manager_inc
    EXTRA_CFLAGS += -I$(CUR_MAKEFILE_PATH)/../../../../../drivers/dev_core/device_manager/device_manager_host/devdrv_manager_host

	ifeq ($(TARGET_PRODUCT),cloud)
		EXTRA_CFLAGS += -I$(CUR_MAKEFILE_PATH)/../../../../../inc/soc/cloud_v1
		EXTRA_CFLAGS += -I$(CUR_MAKEFILE_PATH)/../../../../../drivers/dev_core/device_manager/device_manager_host/devdrv_platform_host/cloud
		EXTRA_CFLAGS += -DCFG_SOC_PLATFORM_CLOUD_HOST
	else ifeq ($(TARGET_PRODUCT),mini)
		EXTRA_CFLAGS += -DCFG_SOC_PLATFORM_MINI
		EXTRA_CFLAGS += -I$(CUR_MAKEFILE_PATH)/../../../../../inc/soc/mini_v1
		ifeq ($(TARGET_CHIP_ID), hi1951)
			EXTRA_CFLAGS += -DCFG_SOC_PLATFORM_MINIV2
			EXTRA_CFLAGS += -DCFG_SOC_PLATFORM_HOST
			EXTRA_CFLAGS += -I$(CUR_MAKEFILE_PATH)/../../../../../drivers/dev_core/device_manager/device_manager_host/devdrv_platform_host/miniv2
		else
			EXTRA_CFLAGS += -I$(CUR_MAKEFILE_PATH)/../../../../../drivers/dev_core/device_manager/device_manager_host/devdrv_platform_host/mini
		endif
	else
		EXTRA_CFLAGS += -I$(CUR_MAKEFILE_PATH)/../../../../../inc/soc/mini_v1
		ifeq ($(TARGET_CHIP_ID), hi1951)
			EXTRA_CFLAGS += -DCFG_SOC_PLATFORM_MINIV2
			EXTRA_CFLAGS += -I$(CUR_MAKEFILE_PATH)/../../../../../drivers/dev_core/device_manager/device_manager_host/devdrv_platform_host/miniv2
		else
			EXTRA_CFLAGS += -I$(CUR_MAKEFILE_PATH)/../../../../../drivers/dev_core/device_manager/device_manager_host/devdrv_platform_host/mini
		endif
	endif

	ifeq ($(TARGET_PRODUCT),cloud)
		obj-m += drv_devdrv_host.o
	else ifeq ($(TARGET_PRODUCT),mini)
		obj-m += drv_devdrv_host.o
	else
		obj-$(CONFIG_HOST_DEVDRV_DEVDRIVER) += drv_devdrv_host.o
	endif
	drv_devdrv_host-objs := devdrv_cqsq.o devdrv_devinit.o devdrv_svmapi.o devdrv_config_p2p.o
	drv_devdrv_host-objs += devdrv_functional_cqsq.o devdrv_mailbox.o
	drv_devdrv_host-objs += devdrv_id.o devdrv_recycle.o devdrv_shm.o
	obj-$(CONFIG_NPU_DEVDRV_DEVDRIVER) += ../../device_manager_slave/devdrv_driver_slave/

endif
