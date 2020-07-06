ifeq ($(DAVINCI_HIAI_DKMS),y)
	EXTRA_CFLAGS += -I$(HIAI_DKMS_DIR)/dev_inc
	EXTRA_CFLAGS += -I$(HIAI_DKMS_DIR)/libc_sec/include
	EXTRA_CFLAGS += -I$(HIAI_DKMS_DIR)/pcievnic_common_h
	EXTRA_CFLAGS += -I$(HIAI_DKMS_DIR)/pcievnic_host
        ifeq ($(TARGET_PRODUCT),cloud)
        EXTRA_CFLAGS += -DCFG_SOC_PLATFORM_CLOUD_HOST
        endif

	ifeq ($(TARGET_CHIP_ID),$(ASCEND_FLORENCE_DC_V10))
	EXTRA_CFLAGS += -DCFG_SOC_PLATFORM_ESL_FPGA
	endif	
else
	CUR_MAKEFILE_PATH := $(strip \
                $(eval LOCAL_MODULE_MAKEFILE := $$(lastword $$(MAKEFILE_LIST))) \
                $(patsubst %/,%, $(dir $(LOCAL_MODULE_MAKEFILE))) \
                )

	EXTRA_CFLAGS += -I$(CUR_MAKEFILE_PATH)/../../../../drivers/dev_inc
	EXTRA_CFLAGS += -I$(CUR_MAKEFILE_PATH)/../../../../libc_sec/include
	EXTRA_CFLAGS += -I$(CUR_MAKEFILE_PATH)/../../../../drivers/dev_plat/pcivnic/pcievnic_common_h
	EXTRA_CFLAGS += -I$(CUR_MAKEFILE_PATH)/../../../../drivers/dev_plat/pcivnic/pcievnic_host
        ifeq ($(TARGET_PRODUCT),cloud)
        EXTRA_CFLAGS += -DCFG_SOC_PLATFORM_CLOUD_HOST
        endif

	ifeq ($(TARGET_CHIP_ID),$(ASCEND_FLORENCE_DC_V10))
	EXTRA_CFLAGS += -DCFG_SOC_PLATFORM_ESL_FPGA
	endif
endif

ccflags-y += -Wall -Werror

obj-m += drv_pcie_vnic_host.o
drv_pcie_vnic_host-objs := pcivnic_main.o pcivnic_host.o
