/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2012-2019. All rights reserved.
 * Description:
 * Author: huawei
 * Create: 2019-10-15
 */
#ifndef __DEVDRV_ID_H
#define __DEVDRV_ID_H

#include "devdrv_common.h"
#include "devdrv_manager_common.h"

#define DEVDRV_INVALID_INDEX 0xFEFE

enum devdrv_uio_type {
    DEVDRV_INIT_SQ_UIO,
    DEVDRV_INIT_CQ_UIO,
};

#define DEVDRV_NOTIFY_INFORM_TS 0
#define DEVDRV_NOTIFY_NOT_INFORM_TS 1

struct devdrv_ts_notify_msg {
    struct devdrv_mailbox_message_header header;
    u16 notifyId;
    u16 resv[25];
    u8 plat_type;
    u8 reserved[3];
};

int devdrv_id_init(struct devdrv_info *dev_info, u32 tsid, u32 streams, u32 events);
void devdrv_event_id_destroy(struct devdrv_cce_ctrl *cce_ctrl);
void devdrv_stream_id_destroy(struct devdrv_cce_ctrl *cce_ctrl);
int devdrv_model_id_init(struct devdrv_info *dev_info, u32 tsid, u32 model_id_num);
void devdrv_model_id_destroy(struct devdrv_cce_ctrl *cce_ctrl);

int devdrv_task_id_init(struct devdrv_info *dev_info, u32 tsid, u32 task_id_num);
void devdrv_task_id_destroy(struct devdrv_cce_ctrl *cce_ctrl);
struct devdrv_event_info *devdrv_alloc_event_id(struct devdrv_cce_ctrl *cce_ctrl,
                                                struct devdrv_cce_context *cce_context);

int devdrv_free_stream_id(struct devdrv_cce_ctrl *cce_ctrl, struct devdrv_cce_context *cce_context, int id);
int devdrv_free_event_id(struct devdrv_cce_ctrl *cce_ctrl, struct devdrv_cce_context *cce_context, int id);
struct devdrv_stream_info *devdrv_alloc_stream(struct devdrv_cce_ctrl *cce_ctrl, struct devdrv_cce_context *cce_context,
                                               int *no_stream, u32 strategy);
int devdrv_free_stream(struct devdrv_cce_ctrl *cce_ctrl, struct devdrv_cce_context *cce_context, int id, int inform);
int devdrv_free_id(struct devdrv_cce_ctrl *cce_ctrl, struct devdrv_cce_context *cce_context, unsigned int cmd, int id,
                   unsigned long arg);

int devdrv_init_cq_uio(struct devdrv_cce_ctrl *cce_ctrl, struct devdrv_cq_sub_info *cq_sub_info, u32 size);

void devdrv_exit_cq_uio(struct devdrv_cce_ctrl *cce_ctrl, struct devdrv_ts_cq_info *cq_info);
int devdrv_alloc_task_id(struct devdrv_cce_ctrl *cce_ctrl, struct devdrv_cce_context *cce_context);
int devdrv_free_task_id(struct devdrv_cce_ctrl *cce_ctrl, struct devdrv_cce_context *cce_context, int id);
int devdrv_add_task_msg_chan(struct devdrv_info *dev_info, u32 tsid,
                             struct devdrv_manager_msg_resource_id *dev_msg_resource_id);
int devdrv_alloc_task_msg_chan(struct devdrv_info *dev_info, u32 tsid,
                               struct devdrv_manager_msg_resource_id *dev_msg_resource_id);

int devdrv_alloc_model_id(struct devdrv_cce_ctrl *cce_ctrl, struct devdrv_cce_context *cce_context);
int devdrv_free_model_id(struct devdrv_cce_ctrl *cce_ctrl, struct devdrv_cce_context *cce_context, int id);
int devdrv_free_one_notify_id(struct devdrv_cce_ctrl *cce_ctrl, struct devdrv_cce_context *cce_context, int id,
                              int inform_type);
int devdrv_alloc_stream_msg_chan(struct devdrv_info *dev_info, u32 tsid,
                                 struct devdrv_manager_msg_resource_id *dev_msg_resource_id);
int devdrv_alloc_event_msg_chan(struct devdrv_info *dev_info, u32 tsid,
                                struct devdrv_manager_msg_resource_id *dev_msg_resource_id);
int devdrv_alloc_model_msg_chan(struct devdrv_info *dev_info, u32 tsid,
                                struct devdrv_manager_msg_resource_id *dev_msg_resource_id);
int devdrv_alloc_notify_msg_chan(struct devdrv_info *dev_info, u32 tsid,
                                 struct devdrv_manager_msg_resource_id *dev_msg_resource_id);

int devdrv_add_stream_msg_chan(struct devdrv_info *dev_info, u32 tsid,
                               struct devdrv_manager_msg_resource_id *dev_msg_resource_id);
int devdrv_add_event_msg_chan(struct devdrv_info *dev_info, u32 tsid,
                              struct devdrv_manager_msg_resource_id *dev_msg_resource_id);
int devdrv_add_model_msg_chan(struct devdrv_info *dev_info, u32 tsid,
                              struct devdrv_manager_msg_resource_id *dev_msg_resource_id);
int devdrv_add_notify_msg_chan(struct devdrv_info *dev_info, u32 tsid,
                               struct devdrv_manager_msg_resource_id *dev_msg_resource_id);

struct devdrv_event_info *devdrv_get_event_id(struct devdrv_cce_ctrl *cce_ctrl);
int devdrv_alloc_notify_id(struct devdrv_cce_ctrl *cce_ctrl, struct devdrv_cce_context *cce_context,
                           struct devdrv_ioctl_arg *arg);
int devdrv_free_notify_id(struct devdrv_cce_ctrl *cce_ctrl, struct devdrv_cce_context *cce_context,
    struct devdrv_ioctl_arg *arg);
int devdrv_notify_id_init(struct devdrv_info *dev_info, u32 tsid, u32 notify_id_num);
void devdrv_notify_id_destroy(struct devdrv_cce_ctrl *cce_ctrl);
int devdrv_notify_ts_msg(struct devdrv_info *dev_info, u32 tsid, int notifyId);
#endif
