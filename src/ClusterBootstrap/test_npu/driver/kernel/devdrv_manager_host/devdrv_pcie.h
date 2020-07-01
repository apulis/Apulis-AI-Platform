/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2012-2019. All rights reserved.
 * Description:
 * Author: huawei
 * Create: 2020-1-19
 */
#ifndef __DEVDRV_PCIE_H
#define __DEVDRV_PCIE_H

#include <linux/slab.h>
#include <linux/cred.h>

#include "devdrv_manager_container.h"
#include "devdrv_pm.h"
#include "devdrv_interface.h"
#include "devdrv_common.h"

int devdrv_manager_pcie_pre_reset(struct file *filep, unsigned int cmd, unsigned long arg);
int devdrv_manager_pcie_rescan(struct file *filep, unsigned int cmd, unsigned long arg);
int devdrv_manager_pcie_hot_reset(struct file *filep, unsigned int cmd, unsigned long arg);
int drv_pcie_sram_read(struct file *filep, unsigned int cmd, unsigned long arg);
int drv_pcie_sram_write(struct file *filep, unsigned int cmd, unsigned long arg);
int drv_pcie_bbox_ddr_read(struct file *filep, unsigned int cmd, unsigned long arg);
int drv_pcie_bbox_imu_ddr_read(struct file *filep, unsigned int cmd, unsigned long arg);
int devdrv_manager_p2p_attr_op(struct file *filep, unsigned int cmd, unsigned long arg);
int drv_dma_mmap(struct file *filep, unsigned int cmd, unsigned long arg);
int drv_dma_munmap(struct file *filep, unsigned int cmd, unsigned long arg);
int drv_get_user_config(struct file *filep, unsigned int cmd, unsigned long arg);
int drv_get_device_boot_status(struct file *filep, unsigned int cmd, unsigned long arg);
int devdrv_manager_get_host_phy_mach_flag(struct file *filep, unsigned int cmd, unsigned long arg);
int devdrv_manager_get_master_device_in_the_same_os(struct file *filep, unsigned int cmd, unsigned long arg);

#endif /* __DEVDRV_PCIE_H */
