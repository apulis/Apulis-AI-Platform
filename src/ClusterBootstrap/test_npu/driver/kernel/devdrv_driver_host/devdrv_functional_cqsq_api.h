/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2012-2019. All rights reserved.
 * Description:
 * Author: huawei
 * Create: 2019-10-15
 */
#ifndef __DEVDRV_FUNCTIONAL_CQSQ_API_H
#define __DEVDRV_FUNCTIONAL_CQSQ_API_H

#define DEVDRV_MAILBOX_CREATE_CQSQ_CALC 0x0001
#define DEVDRV_MAILBOX_RELEASE_CQSQ_CALC 0x0002
#define DEVDRV_MAILBOX_CREATE_CQSQ_LOG 0x0003
#define DEVDRV_MAILBOX_RELEASE_CQSQ_LOG 0x0004
#define DEVDRV_MAILBOX_CREATE_CQSQ_DEBUG 0x0005
#define DEVDRV_MAILBOX_RELEASE_CQSQ_DEBUG 0x0006
#define DEVDRV_MAILBOX_CREATE_CQSQ_PROFILE 0x0007
#define DEVDRV_MAILBOX_RELEASE_CQSQ_PROFILE 0x0008
#define DEVDRV_MAILBOX_CREATE_CQSQ_BEAT 0x0009
#define DEVDRV_MAILBOX_RELEASE_CQSQ_BEAT 0x000A
#define DEVDRV_MAILBOX_RECYCLE_EVENT_ID 0x000B
#define DEVDRV_MAILBOX_RECYCLE_STREAM_ID 0x000C
#define DEVDRV_MAILBOX_SEND_CONTAINER_TFLOP 0x000D
#define DEVDRV_MAILBOX_CONFIG_P2P_INFO 0x000E
#define DEVDRV_MAILBOX_SEND_RDMA_INFO 0x000F
#define DEVDRV_MAILBOX_RESET_NOTIFY_ID 0x0010
#define DEVDRV_MAILBOX_RECYCLE_PID_ID 0x0012
#define DEVDRV_MAILBOX_FREE_STREAM_ID 0x0013
#define DEVDRV_MAILBOX_SEND_SHARE_MEMORY_INFO 0x0014
#define  DEVDRV_MAILBOX_SEND_HWTS_INFO 0x0015

#define DEVDRV_FUNCTIONAL_BRIEF_CQ 0
#define DEVDRV_FUNCTIONAL_DETAILED_CQ 1
#define DEVDRV_FUNCTIONAL_CALLBACK_HS_CQ 2
#define DEVDRV_FUNCTIONAL_REPORT_HS_CQ 3
#define DEVDRV_MAILBOX_INVALID_INDEX 0xFFFF

#define DEVDRV_FUNCTIONAl_MAX_SQ_SLOT_LEN 128
#define DEVDRV_FUNCTIONAL_MAX_CQ_SLOT_LEN 128

struct devdrv_mailbox_cqsq {
    u16 valid;    /* validity judgement, 0x5a5a is valid */
    u16 cmd_type; /* command type */
    u32 result;   /* TS's process result succ or fail: no error: 0, error: not 0 */

    u64 sq_addr; /* invalid addr: 0x0 */
    u64 cq0_addr;
    u64 cq1_addr;
    u64 cq2_addr;
    u64 cq3_addr;
    u16 sq_index;  /* invalid index: 0xFFFF */
    u16 cq0_index; /* sq's return */
    u16 cq1_index; /* ts's return */
    u16 cq2_index; /* ai cpu's return */
    u16 cq3_index; /* reserved */

    u16 stream_id;   /* binding stream id for sq and cq (for calculation task) */
    u8 plat_type;    /* inform TS, msg is sent from host or device, device: 0 host: 1 */
    u8 cq_slot_size; /* calculation cq's slot size, default: 12 bytes */
};

enum devdrv_cqsq_func {
    DEVDRV_CQSQ_HEART_BEAT = 0x0,
    DEVDRV_MAX_CQSQ_FUNC,
};

int devdrv_create_functional_sq(u32 devid, u32 tsid, u32 slot_len, u32 *sq_index, u64 *addr);
void devdrv_destroy_functional_sq(u32 devid, u32 tsid, u32 sq_index);
int devdrv_create_functional_cq(u32 devid, u32 tsid, u32 slot_len, u32 cq_type,
                                void (*callback)(const u8 *cq_slot, u8 *sq_slot), u32 *cq_index, u64 *addr);
void devdrv_destroy_functional_cq(u32 devid, u32 tsid, u32 cq_index);
int devdrv_functional_send_sq(u32 devid, u32 tsid, u32 sq_index, const u8 *buffer, u32 buf_len);
int devdrv_mailbox_send_cqsq(u32 devid, u32 tsid, struct devdrv_mailbox_cqsq *cqsq);
int devdrv_functional_set_cq_func(u32 devid, u32 tsid, u32 cq_index, enum devdrv_cqsq_func function);
int devdrv_functional_set_sq_func(u32 devid, u32 tsid, u32 sq_index, enum devdrv_cqsq_func function);

#endif
