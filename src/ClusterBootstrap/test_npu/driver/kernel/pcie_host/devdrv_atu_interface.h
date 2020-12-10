/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2012-2019. All rights reserved.
 * Description: atu common interfaces
 * Author: huawei
 * Create: 2017/8/18
 */
#ifndef DEVDRV_ATU_INTERFACE_H
#define DEVDRV_ATU_INTERFACE_H

#include <linux/types.h>

int devdrv_devmem_addr_d2h(u32 devid, phys_addr_t device_phy_addr, phys_addr_t *host_bar_addr);
int devdrv_devmem_addr_h2d(u32 devid, phys_addr_t host_bar_addr, phys_addr_t *device_phy_addr);
#endif
