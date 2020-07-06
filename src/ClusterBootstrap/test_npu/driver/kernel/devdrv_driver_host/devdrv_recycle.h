/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2012-2019. All rights reserved.
 * Description:
 * Author: huawei
 * Create: 2019-10-15
 */
#ifndef __DEVDRV_RECYCLE_H
#define __DEVDRV_RECYCLE_H

#include <linux/workqueue.h>

#include "devdrv_common.h"

struct devdrv_source {
    u32 stream_num;
    u32 event_num;
    u32 sq_num;
    u32 cq_num;
};

struct devdrv_transmission {
    u32 send_count;
    u32 receive_count;
};

#define DEVDRV_RECYCLE_MAX_EVENT_NUM 25
#define DEVDRV_RECYCLE_STATUS 1

struct devdrv_event_recycle {
    struct devdrv_mailbox_message_header header;  // 8
    u16 count;                                    // 2
    u16 event[DEVDRV_RECYCLE_MAX_EVENT_NUM];      // 50
    u8 plat_type;
    u8 reserved[3];
};

void devdrv_recycle_work(struct work_struct *work);

#endif
