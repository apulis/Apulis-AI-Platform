/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2012-2019. All rights reserved.
 * Description:
 * Author: huawei
 * Create: 2019-10-15
 */
#ifndef _DEVDRV_DEVINIT_H
#define _DEVDRV_DEVINIT_H

#include <linux/cdev.h>

#include "devdrv_common.h"
#include "devdrv_manager.h"
#if defined(CFG_SOC_PLATFORM_MINIV2)
#define MAX_NODE_NUM 2
#define MAX_NODE_NUM_OF_FRAME 4
#else
#define MAX_NODE_NUM 4
#define MAX_NODE_NUM_OF_FRAME 8
#endif

#define DEVDRV_ID_MAGIC 'D'
#define DEVDRV_ALLOC_STREAM_ID _IOWR(DEVDRV_ID_MAGIC, 1, struct devdrv_ioctl_arg)
#define DEVDRV_FREE_STREAM_ID _IOW(DEVDRV_ID_MAGIC, 2, struct devdrv_ioctl_arg)
#define DEVDRV_ALLOC_EVENT_ID _IOWR(DEVDRV_ID_MAGIC, 3, struct devdrv_ioctl_arg)
#define DEVDRV_FREE_EVENT_ID _IOW(DEVDRV_ID_MAGIC, 4, struct devdrv_ioctl_arg)
#define DEVDRV_REPORT_WAIT _IOWR(DEVDRV_ID_MAGIC, 5, struct devdrv_ioctl_arg)
#define DEVDRV_ALLOC_MODEL_ID _IOWR(DEVDRV_ID_MAGIC, 6, struct devdrv_ioctl_arg)
#define DEVDRV_FREE_MODEL_ID _IOW(DEVDRV_ID_MAGIC, 7, struct devdrv_ioctl_arg)
#define DEVDRV_ALLOC_NOTIFY_ID _IOWR(DEVDRV_ID_MAGIC, 8, struct devdrv_ioctl_arg)
#define DEVDRV_FREE_NOTIFY_ID _IOW(DEVDRV_ID_MAGIC, 9, struct devdrv_ioctl_arg)
#define DEVDRV_ALLOC_TASK_ID _IOWR(DEVDRV_ID_MAGIC, 11, struct devdrv_ioctl_arg)
#define DEVDRV_FREE_TASK_ID _IOW(DEVDRV_ID_MAGIC, 12, struct devdrv_ioctl_arg)


#define DEVDRV_TSMSG_SEND_ASYNC _IOWR(DEVDRV_ID_MAGIC, 13, struct devdrv_ioctl_arg)
#define DEVDRV_TSMSG_RECV_ASYNC _IOWR(DEVDRV_ID_MAGIC, 14, struct devdrv_ioctl_arg)

#define DEVDRV_GET_OCCUPY_STREAM_ID _IOWR(DEVDRV_ID_MAGIC, 15, struct devdrv_ioctl_arg)

#define DEVDRV_MAX_CMD 40

// useless definition (macro DEVDRV_CUSTOM_IOCTL)
#define DEVDRV_CUSTOM_IOCTL _IOWR(DEVDRV_ID_MAGIC, 64, struct devdrv_custom_para)

int devdrv_mmap_sq_mem(struct devdrv_cce_context *cce_context, struct devdrv_cce_ctrl *cce_ctrl, u32 index);
void devdrv_unmap_sq_mem(struct devdrv_cce_context *cce_context, struct devdrv_cce_ctrl *cce_ctrl, u32 index);

#endif
