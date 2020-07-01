ccflags-y += -Wall -Werror
ifeq ($(DAVINCI_HIAI_DKMS),y)


	EXTRA_CFLAGS += -I$(HIAI_DKMS_DIR)/dev_inc
	EXTRA_CFLAGS += -I$(HIAI_DKMS_DIR)/inc/driver
	EXTRA_CFLAGS += -I$(HIAI_DKMS_DIR)/devmm
	EXTRA_CFLAGS += -I$(HIAI_DKMS_DIR)/devmmcommon
	EXTRA_CFLAGS += -I$(HIAI_DKMS_DIR)/libc_sec/include


	ifeq ($(TARGET_PRODUCT),cloud)
		EXTRA_CFLAGS += -DCFG_SOC_PLATFORM_CLOUD_HOST
	endif

	ifeq ($(TARGET_PRODUCT),cloud)
		EXTRA_CFLAGS += -I$(HIAI_DKMS_DIR)/devmm/devmm/ascd910
	else ifeq ($(TARGET_PRODUCT),mini)
		ifeq ($(TARGET_CHIP_ID), hi1951)
			EXTRA_CFLAGS += -I$(HIAI_DKMS_DIR)/devmm/devmm/ascd610
		else
			EXTRA_CFLAGS += -I$(HIAI_DKMS_DIR)/devmm/devmm/ascd310
		endif
	else
		ifeq ($(TARGET_CHIP_ID), hi1951)
			EXTRA_CFLAGS += -I$(HIAI_DKMS_DIR)/devmm/devmm/ascd610
		else
			EXTRA_CFLAGS += -I$(HIAI_DKMS_DIR)/devmm/devmm/ascd310
		endif
	endif

else

	CUR_MAKEFILE_PATH := $(strip \
                $(eval LOCAL_MODULE_MAKEFILE := $$(lastword $$(MAKEFILE_LIST))) \
                $(patsubst %/,%, $(dir $(LOCAL_MODULE_MAKEFILE))) \
                )

	EXTRA_CFLAGS += -I$(CUR_MAKEFILE_PATH)/../../../../drivers/dev_inc
	EXTRA_CFLAGS += -I$(CUR_MAKEFILE_PATH)/../../../../inc/driver
	EXTRA_CFLAGS += -I$(CUR_MAKEFILE_PATH)/../../../../drivers/dev_plat/devmm/devmm
	EXTRA_CFLAGS += -I$(CUR_MAKEFILE_PATH)/../../../../drivers/dev_plat/devmm/devmmcommon
	EXTRA_CFLAGS += -I$(CUR_MAKEFILE_PATH)/../../../../libc_sec/include

	ifeq ($(TARGET_PRODUCT),cloud)
		EXTRA_CFLAGS += -DCFG_SOC_PLATFORM_CLOUD_HOST
	endif

	ifeq ($(TARGET_PRODUCT),cloud)
		EXTRA_CFLAGS += -I$(CUR_MAKEFILE_PATH)/../../devmm/devmm/ascd910
	else ifeq ($(TARGET_PRODUCT),mini)
		ifeq ($(TARGET_CHIP_ID), hi1951)
			EXTRA_CFLAGS += -I$(CUR_MAKEFILE_PATH)/../../devmm/devmm/ascd610
		else
			EXTRA_CFLAGS += -I$(CUR_MAKEFILE_PATH)/../../devmm/devmm/ascd310
		endif
	else
		ifeq ($(TARGET_CHIP_ID), hi1951)
			EXTRA_CFLAGS += -I$(CUR_MAKEFILE_PATH)/../../devmm/devmm/ascd610
		else
			EXTRA_CFLAGS += -I$(CUR_MAKEFILE_PATH)/../../devmm/devmm/ascd310
		endif
	endif

endif

drv_devmm_host-objs := devmm_dev.o devmm_msg_init.o devmm_proc_info.o devmm_channel.o devmm_nsm.o devmm_proc_rdma.o devmm_common.o devmm_proc_ipc.o

ifeq ($(TARGET_PRODUCT),cloud)
	drv_devmm_host-objs += ascd910/devmm_adapt.o
else ifeq ($(TARGET_PRODUCT),mini)
	ifeq ($(TARGET_CHIP_ID), hi1951)
		drv_devmm_host-objs += ascd610/devmm_adapt.o
	else
		drv_devmm_host-objs += ascd310/devmm_adapt.o
	endif
else
	ifeq ($(TARGET_CHIP_ID), hi1951)
		drv_devmm_host-objs += ascd610/devmm_adapt.o
	else
		drv_devmm_host-objs += ascd310/devmm_adapt.o
	endif
endif

obj-m := drv_devmm_host.o
