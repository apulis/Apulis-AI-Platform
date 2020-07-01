ifeq ($(DAVINCI_HIAI_DKMS),y)
    EXTRA_CFLAGS += -I$(HIAI_DKMS_DIR)/dev_inc
    EXTRA_CFLAGS += -I$(HIAI_DKMS_DIR)/libc_sec/include

    ifeq ($(TARGET_PRODUCT),cloud)
        EXTRA_CFLAGS += -DCFG_SOC_PLATFORM_CLOUD
        EXTRA_CFLAGS += -DCFG_SOC_PLATFORM_CLOUD_HOST
    else
        ifeq ($(TARGET_CHIP_ID),$(ASCEND_FLORENCE_DC_V10))
            EXTRA_CFLAGS += -DCFG_SOC_PLATFORM_ESL_FPGA
            EXTRA_CFLAGS += -DCFG_SOC_PLATFORM_MDC_V51
        endif
    endif

else
    CUR_MAKEFILE_PATH := $(strip \
                $(eval LOCAL_MODULE_MAKEFILE := $$(lastword $$(MAKEFILE_LIST))) \
                $(patsubst %/,%, $(dir $(LOCAL_MODULE_MAKEFILE))) \
                )

    EXTRA_CFLAGS += -I$(CUR_MAKEFILE_PATH)/../../../../drivers/dev_inc
    EXTRA_CFLAGS += -I$(CUR_MAKEFILE_PATH)/../../../../libc_sec/include
    ifeq ($(TARGET_PRODUCT),cloud)
        EXTRA_CFLAGS += -DCFG_SOC_PLATFORM_CLOUD
        EXTRA_CFLAGS += -DCFG_SOC_PLATFORM_CLOUD_HOST
    else
        ifeq ($(TARGET_CHIP_ID),$(ASCEND_FLORENCE_DC_V10))
            EXTRA_CFLAGS += -DCFG_SOC_PLATFORM_ESL_FPGA
            EXTRA_CFLAGS += -DCFG_SOC_PLATFORM_MDC_V51
        endif
    endif

endif

ccflags-y += -Wall -Werror

obj-m += drv_pcie_hdc_host.o
drv_pcie_hdc_host-objs := hdcdrv_core.o hdcdrv_host.o hdcdrv_mem.o hdcdrv_epoll.o
