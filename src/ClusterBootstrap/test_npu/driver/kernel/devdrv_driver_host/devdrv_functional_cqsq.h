/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2012-2019. All rights reserved.
 * Description:
 * Author: huawei
 * Create: 2019-10-15
 */
#ifndef __DEVDRV_FUNCTIONAL_CQSQ_H
#define __DEVDRV_FUNCTIONAL_CQSQ_H

#include <linux/spinlock.h>
#include <linux/workqueue.h>

#include "devdrv_functional_cqsq_api.h"

#define DEVDRV_MAX_FUNCTIONAL_SQ_DEPTH 1024
#define DEVDRV_MAX_FUNCTIONAL_CQ_DEPTH 1024

#define DEVDRV_FUNCTIONAL_PHASE_ZERO 0
#define DEVDRV_FUNCTIONAL_PHASE_ONE 1

#define DEVDRV_FUNCTIONAL_BRIEF_CQ_OFFSET 4
#define DEVDRV_FUNCTIONAL_DETAILED_CQ_OFFSET 8

#define DEVDRV_FUNCTIONAL_DETAILED_CQ_LENGTH 128

#define devdrv_functional_sq_doorbell_index(index) ((index) + DEVDRV_FUNCTIONAL_SQ_FIRST_INDEX)
#define devdrv_functional_sq_array_index(index) ((index)-DEVDRV_FUNCTIONAL_SQ_FIRST_INDEX)

#define devdrv_functional_cq_doorbell_index(index) ((index) + DEVDRV_FUNCTIONAL_CQ_FIRST_INDEX)
#define devdrv_functional_cq_array_index(index) ((index)-DEVDRV_FUNCTIONAL_CQ_FIRST_INDEX)

struct devdrv_functional_sq_info {
    u32 devid;
    u32 index;
    u32 depth;
    u32 slot_len;

    u8 *addr;
    phys_addr_t phy_addr;
    dma_addr_t host_dma_addr;
    void *host_dma_buffer; /* online use, alloc 128 bytes as a buffer for dma */
    u32 head;
    u32 tail;
    u32 credit;
    u32 *doorbell;

    enum devdrv_cqsq_func function;
};

struct devdrv_functional_cq_info {
    u32 devid;
    u32 index;
    u32 depth;
    u32 slot_len;
    u32 type;
    struct work_struct work;
    struct devdrv_cce_ctrl *cce_ctrl;

    u8 *addr;
    phys_addr_t phy_addr;

    dma_addr_t host_dma_addr;
    void *host_dma_buffer; /* online use, alloc 128 bytes as a buffer for dma */

    u32 head;
    u32 tail;
    u32 phase;
    u32 *doorbell;
    struct list_head int_list_node;
    void (*callback)(const u8 *cq_slot, u8 *sq_slot);

    enum devdrv_cqsq_func function;
    struct mutex lock;
};

struct devdrv_functional_int_context {
    int irq_num;
    struct workqueue_struct *wq;
    spinlock_t spinlock;
    struct devdrv_cce_ctrl *cce_ctrl;
    struct list_head int_list_header;
    struct tasklet_struct cqsq_tasklet;
};

struct devdrv_functional_cqsq {
    struct devdrv_functional_sq_info *sq_info;
    struct devdrv_functional_cq_info *cq_info;
    int sq_num;
    int cq_num;
    spinlock_t spinlock;
    struct devdrv_functional_int_context int_context;
};

struct devdrv_functional_cq_report {
    u8 phase;
    u8 reserved[3];
    u16 sq_index;
    u16 sq_head;
};

#endif
