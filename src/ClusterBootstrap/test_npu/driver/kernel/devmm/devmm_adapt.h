/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2012-2019. All rights reserved.
 * Description: This file contains api declaration for memory manager driver.
 * Author: huawei
 * Create: 2019-06-11
 */

#ifndef __DEVMM_ADAPT_H
#define __DEVMM_ADAPT_H

#include <securec.h>
#include "devmm_dev.h"

#define DEVMM_END_TYPE DEVMM_END_HOST
#define DEVMM_DEV_DEVNODE_MODE (0660)

int devmm_set_dma_addr_to_device(struct devmm_ioctl_arg *arg);
int devmm_get_dma_addr_from_device(struct devmm_ioctl_arg *arg);
#endif /* __DEVMM_ADAPT_H */
