/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2012-2019. All rights reserved.
 * Description:
 * Author: huawei
 * Create: 2019-10-15
 */
#ifndef __DEVDRV_MANAGER_MSG_H
#define __DEVDRV_MANAGER_MSG_H

#include <linux/of_address.h>
#include <linux/platform_device.h>
#include <linux/slab.h>
#include <asm/uaccess.h>
#include <linux/idr.h>
#include <linux/errno.h>
#include <linux/fs.h>

#include "devdrv_common.h"
#include "devdrv_manager_common.h"
#include "devdrv_pm.h"
#include "devdrv_id.h"
#include "devdrv_manager.h"
#include "devdrv_interface.h"

int devdrv_manager_d2h_sync_get_stream(void *msg, u32 *ack_len);
int devdrv_manager_d2h_sync_get_event(void *msg, u32 *ack_len);
int devdrv_manager_d2h_sync_get_model(void *msg, u32 *ack_len);
int devdrv_manager_d2h_sync_get_notify(void *msg, u32 *ack_len);
int devdrv_manager_d2h_sync_get_task(void *msg, u32 *ack_len);

int devdrv_manager_d2h_sync_get_sq(void *msg, u32 *ack_len);
int devdrv_manager_d2h_sync_get_cq(void *msg, u32 *ack_len);
int devdrv_manager_msg_h2d_alloc_sync_stream(struct devdrv_cce_ctrl *cce_ctrl);
int devdrv_manager_msg_h2d_alloc_sync_event(struct devdrv_cce_ctrl *cce_ctrl);
int devdrv_manager_msg_h2d_alloc_sync_sq(struct devdrv_info *dev_info);
int devdrv_manager_msg_h2d_alloc_sync_cq(struct devdrv_info *dev_info);
int devdrv_manager_h2d_sync_get_devinfo(struct devdrv_info *dev_info);
int devdrv_manager_msg_h2d_alloc_sync_model(struct devdrv_cce_ctrl *cce_ctrl);
int devdrv_manager_msg_h2d_alloc_sync_notify(struct devdrv_cce_ctrl *cce_ctrl);
int devdrv_manager_msg_h2d_alloc_sync_task(struct devdrv_cce_ctrl *cce_ctrl);

#endif /* __DEVDRV_MANAGER_MSG_H */
