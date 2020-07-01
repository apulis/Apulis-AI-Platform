/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2012-2019. All rights reserved.
 * Description:
 * Author: huawei
 * Create: 2019-10-15
 */
#ifndef _DEVDRV_CQSQ_H
#define _DEVDRV_CQSQ_H

#include "devdrv_common.h"
#include "devdrv_mailbox.h"
#include "devdrv_manager_common.h"

#define DEVDRV_NO_NEED_TO_INFORM 0
#define DEVDRV_HAVE_TO_INFORM 1

void devdrv_cqsq_destroy(struct devdrv_cce_ctrl *cce_ctrl);
int devdrv_cqsq_init(struct devdrv_info *dev_info, u32 tsid, u32 num_sq, u32 num_cq);
int devdrv_cqsq_alloc(struct devdrv_cce_ctrl *cce_ctrl, struct devdrv_cce_context *cce_context,
                      struct devdrv_stream_info *id);
void devdrv_cqsq_free_id(struct devdrv_cce_ctrl *cce_ctrl, struct devdrv_cce_context *cce_context, int sq_index,
                         int cq_index);
struct devdrv_ts_cq_info *devdrv_get_cq_exist(const struct devdrv_cce_ctrl *cce_ctrl,
                                              const struct devdrv_cce_context *cce_context);
struct devdrv_ts_cq_info *devdrv_get_new_cq(struct devdrv_cce_ctrl *cce_ctrl, struct devdrv_cce_context *cce_context);
void devdrv_free_cq(struct devdrv_cce_ctrl *cce_ctrl, struct devdrv_cce_context *cce_context,
                    struct devdrv_ts_cq_info *cq_info);
int devdrv_mailbox_cqsq_inform(struct devdrv_mailbox *mailbox, struct devdrv_cce_context *cce_context, u16 cmd,
                               u16 sq_index, u64 sq_addr, u16 cq_index, u64 cq_addr, u16 stream_id);
int devdrv_init_functional_cqsq(struct devdrv_info *info, u32 tsid);
void devdrv_destroy_functional_cqsq(struct devdrv_info *info, u32 tsid);
int devdrv_alloc_sq_msg_chan(struct devdrv_info *dev_info, struct devdrv_manager_msg_resource_id *dev_msg_resource_id);
int devdrv_alloc_cq_msg_chan(struct devdrv_info *dev_info, struct devdrv_manager_msg_resource_id *dev_msg_resource_id);
int devdrv_add_sq_msg_chan(struct devdrv_info *dev_info, struct devdrv_manager_msg_resource_id *dev_msg_resource_id);
int devdrv_add_cq_msg_chan(struct devdrv_info *dev_info, struct devdrv_manager_msg_resource_id *dev_msg_resource_id);

#endif
