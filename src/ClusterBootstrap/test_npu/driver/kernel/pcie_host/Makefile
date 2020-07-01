ifeq ($(DAVINCI_HIAI_DKMS),y)
    EXTRA_CFLAGS += -I$(HIAI_DKMS_DIR)/pcie_host/soc
    EXTRA_CFLAGS += -I$(HIAI_DKMS_DIR)/common_h
    EXTRA_CFLAGS += -I$(HIAI_DKMS_DIR)/dev_inc
    EXTRA_CFLAGS += -I$(HIAI_DKMS_DIR)/pcie_host/interfaces
    EXTRA_CFLAGS += -I$(HIAI_DKMS_DIR)/libc_sec/include

    ifeq ($(TARGET_PRODUCT),cloud)
        EXTRA_CFLAGS += -I$(HIAI_DKMS_DIR)/inc/soc/cloud_v1/
        EXTRA_CFLAGS += -I$(HIAI_DKMS_DIR)/pcie_host/soc/cloud_v1
        EXTRA_CFLAGS += -DCFG_SOC_PLATFORM_CLOUD
        EXTRA_CFLAGS += -DCFG_SOC_PLATFORM_CLOUD_HOST
    else
        ifeq ($(TARGET_CHIP_ID), $(ASCEND_FLORENCE_DC_V10))
            EXTRA_CFLAGS += -I$(HIAI_DKMS_DIR)/pcie_host/soc/mdc_v1
            EXTRA_CFLAGS += -I$(HIAI_DKMS_DIR)/inc/soc/cloud_v1
        else
            EXTRA_CFLAGS += -I$(HIAI_DKMS_DIR)/inc/soc/mini_v1/
            EXTRA_CFLAGS += -I$(HIAI_DKMS_DIR)/pcie_host/soc/mini_v1
        endif
    endif

else

	CUR_MAKEFILE_PATH := $(strip \
                $(eval LOCAL_MODULE_MAKEFILE := $$(lastword $$(MAKEFILE_LIST))) \
                $(patsubst %/,%, $(dir $(LOCAL_MODULE_MAKEFILE))) \
                )

	EXTRA_CFLAGS += -I$(CUR_MAKEFILE_PATH)/../../../../drivers/dev_inc
	EXTRA_CFLAGS += -I$(CUR_MAKEFILE_PATH)/soc/
	EXTRA_CFLAGS += -I$(CUR_MAKEFILE_PATH)/../../devdrv/common_h
	EXTRA_CFLAGS += -I$(CUR_MAKEFILE_PATH)/../../../../libc_sec/include

    ifeq ($(TARGET_PRODUCT),cloud)
        EXTRA_CFLAGS += -I$(CUR_MAKEFILE_PATH)/../../../../inc/soc/cloud_v1/
        EXTRA_CFLAGS += -I$(CUR_MAKEFILE_PATH)/soc/cloud_v1
        EXTRA_CFLAGS += -DCFG_SOC_PLATFORM_CLOUD
        EXTRA_CFLAGS += -DCFG_SOC_PLATFORM_CLOUD_HOST
    else
        ifeq ($(TARGET_CHIP_ID), $(ASCEND_FLORENCE_DC_V10))
            EXTRA_CFLAGS += -I$(CUR_MAKEFILE_PATH)/../../../../inc/soc/cloud_v1/
            EXTRA_CFLAGS += -I$(CUR_MAKEFILE_PATH)/soc/mdc_v1
            EXTRA_CFLAGS += -DCFG_SOC_PLATFORM_MDC_V51
            EXTRA_CFLAGS += -DCFG_SOC_PLATFORM_CLOUD
        else
            EXTRA_CFLAGS += -I$(CUR_MAKEFILE_PATH)/../../../../inc/soc/mini_v1/
            EXTRA_CFLAGS += -I$(CUR_MAKEFILE_PATH)/soc/mini_v1
        endif
    endif
endif

ccflags-y += -Wall -Werror
obj-m   += drv_pcie_host.o
drv_pcie_host-objs := devdrv_dma.o devdrv_msg.o devdrv_ctrl.o devdrv_sysfs.o devdrv_pci.o devdrv_common_msg.o devdrv_device_load.o
drv_pcie_host-objs += devdrv_atu_interface.o
drv_pcie_host-objs += interfaces/devdrv_dma_interface.o
drv_pcie_host-objs += soc/dma_comm_drv.o soc/nvme_comm_drv.o soc/apb_comm_drv.o soc/resource_comm_drv.o

ifeq ($(TARGET_PRODUCT),cloud)
drv_pcie_host-objs += soc/cloud_v1/dma_drv.o soc/cloud_v1/nvme_drv.o soc/cloud_v1/apb_drv.o soc/cloud_v1/resource_drv.o
else
    ifeq ($(TARGET_CHIP_ID), $(ASCEND_FLORENCE_DC_V10))
        drv_pcie_host-objs += soc/mdc_v1/dma_drv.o soc/mdc_v1/nvme_drv.o soc/mdc_v1/apb_drv.o soc/mdc_v1/resource_drv.o
    else
        drv_pcie_host-objs += soc/mini_v1/dma_drv.o soc/mini_v1/nvme_drv.o soc/mini_v1/apb_drv.o soc/mini_v1/resource_drv.o
    endif
endif

