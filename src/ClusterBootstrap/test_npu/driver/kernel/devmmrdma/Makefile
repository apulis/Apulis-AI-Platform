ccflags-y += -Wall -Werror -fstack-protector-all
ifeq ($(DAVINCI_HIAI_DKMS),y)
EXTRA_CFLAGS += -I$(HIAI_DKMS_DIR)/dev_inc
EXTRA_CFLAGS += -I$(HIAI_DKMS_DIR)/inc/driver
EXTRA_CFLAGS += -I$(HIAI_DKMS_DIR)/devmmcommon

ifeq ($(TARGET_PRODUCT),cloud)
EXTRA_CFLAGS += -DCFG_SOC_PLATFORM_CLOUD_HOST
endif

else

KERNEL_DIR := $(srctree)

EXTRA_CFLAGS += -I$(KERNEL_DIR)/../../drivers/dev_inc
EXTRA_CFLAGS += -I$(KERNEL_DIR)/../../inc/driver
EXTRA_CFLAGS += -I$(KERNEL_DIR)/../../drivers/dev_plat/devmm/devmmcommon

endif

ifeq ($(TARGET_PRODUCT), mini)
    ifeq ($(TARGET_CHIP_ID), hi1951)
        drv_devmm_rdma_host-objs := devmm_rdma.o
        obj-m := drv_devmm_rdma_host.o
    endif
else
    devmm_rdma-objs :=
    obj-m := devmm_rdma.o
endif
