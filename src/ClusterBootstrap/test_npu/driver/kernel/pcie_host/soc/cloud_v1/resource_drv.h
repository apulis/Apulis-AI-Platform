/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2012-2019. All rights reserved.
 * Description:
 * Author: Huawei Technologies Co., Ltd.
 * Create: 2018/8/18
 */

#ifndef __RESOURCE_DRV_H__
#define __RESOURCE_DRV_H__

#include <linux/pci.h>

#include "resource_comm_drv.h"

/* DMA chan distribution table. */
#define DMA_CHAN_LOCAL_USED_NUM             11
#define DMA_CHAN_LOCAL_USED_START_INDEX     0
#define DMA_CHAN_REMOTE_USED_NUM            11
#define DMA_CHAN_REMOTE_USED_START_INDEX    11
#define DMA_CHAN_TS_USED_NUM                8
#define DMA_CHAN_TS_USED_START_INDEX        22
#define DEVDRV_RSV_MEM_BASE 0

#endif /* __RESOURCE_DRV_H__ */
