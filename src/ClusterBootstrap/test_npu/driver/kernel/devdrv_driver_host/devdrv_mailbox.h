/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2012-2019. All rights reserved.
 * Description:
 * Author: huawei
 * Create: 2019-10-15
 */
#ifndef __DEVDRV_MAILBOX_H
#define __DEVDRV_MAILBOX_H

#include <linux/irq.h>
#include <linux/list.h>
#include <linux/semaphore.h>
#include <linux/spinlock.h>
#include <linux/workqueue.h>
#include <linux/mutex.h>

#define DEVDRV_MAILBOX_PAYLOAD_LENGTH 64
#define DEVDRV_MAILBOX_SEND_SRAM (SOC_TS_SRAM_REG - SOC_TS_BASE) /* (0x1F0200000) */
#define DEVDRV_MAILBOX_RECEIVE_SRAM (DEVDRV_MAILBOX_SEND_SRAM + 64)

#define DEVDRV_MAILBOX_SEND_ONLINE_IRQ 0

#define DEVDRV_MAILBOX_READ_DONE_IRQ 327

#define DEVDRV_MAILBOX_SRAM 0
#define DEVDRV_MAILBOX_IPC 1

#define DEVDRV_MAILBOX_SYNC 0
#define DEVDRV_MAILBOX_ASYNC 1

#define DEVDRV_MAILBOX_FREE 0
#define DEVDRV_MAILBOX_BUSY 1

#define DEVDRV_MAILBOX_NO_FEEDBACK 0
#define DEVDRV_MAILBOX_NEED_FEEDBACK 1

#define DEVDRV_MAILBOX_SYNC_MESSAGE 1
#define DEVDRV_MAILBOX_CONTEXT_ASYNC_MESSAGE 2
#define DEVDRV_MAILBOX_KERNEL_ASYNC_MESSAGE 3

#define DEVDRV_MAILBOX_VALID_MESSAGE 0
#define DEVDRV_MAILBOX_RECYCLE_MESSAGE 1

#define DEVDRV_MAILBOX_SEMA_TIMEOUT_SECOND 5
#define DEVDRV_MAILBOX_MESSAGE_VALID 0x5A5A

struct devdrv_mailbox_sending_queue {
    spinlock_t spinlock;
    volatile int status; /* mailbox busy or free */
    int mailbox_type;    /* mailbox communication method: SPI+SRAM or IPC */
    struct workqueue_struct *wq;
    struct list_head list_header;
};

struct devdrv_mailbox_waiting_queue {
    spinlock_t spinlock;
    int mailbox_type; /* mailbox communication method: SPI+SRAM or IPC */
    struct workqueue_struct *wq;
    struct list_head list_header;
};

struct devdrv_mailbox_message {
    u8 *message_payload;
    int message_length;
    struct semaphore wait; /* for synchronizing */
    int feedback;          /* need feedback or no need */
    int feedback_num;      /* number of expect ackdata */
    u8 *feedback_buffer;   /* buffer to save feedback data, must alloc memory when message inited */
    int feedback_count;    /* counter of received feedback */
    int process_result;    /* result of processing mailbox by TS */
    int sync_type;         /* sync or nonsync */
    int cmd_type;          /* related to task */
    int message_index;     /* message index when there is several message within one task */
    int message_pid;       /* who am i(process who send this message) */
    struct work_struct send_work;
    struct work_struct wait_work;
    struct list_head send_list_node;
    struct list_head wait_list_node;
    void (*callback)(void *data);
    struct devdrv_mailbox *mailbox;
    struct devdrv_cce_context *cce_context;
    struct devdrv_info *dev_info;
    struct devdrv_cce_ctrl *cce_ctrl;
    int abandon; /* init with zero value, set 1 mean that this message should be abandoned */

    int message_type; /* use for enter different message process branch, user donot need to set this para */
    /*
     * sync_message
     * context_async_message
     * kernel_async_message
     */
    u8 is_sent;
};

struct devdrv_mailbox {
    struct devdrv_mailbox_sending_queue send_queue;
    struct devdrv_mailbox_waiting_queue wait_queue;
    u8 __iomem *send_sram;
    u8 __iomem *receive_sram;
    int ack_irq;
    int data_ack_irq;
    volatile int working;
    struct mutex wait_mutex;
};

struct devdrv_mailbox_message_header {
    u16 valid;    /* validity judgement, 0x5a5a is valid */
    u16 cmd_type; /* identify command or operation */
    u32 result;   /* TS's process result succ or fail: no error: 0, error: not 0 */
};

#define MAX_RDMA_INFO_LEN 56
struct rdam_info {
    struct devdrv_mailbox_message_header header;
    u8 buf[MAX_RDMA_INFO_LEN];
};

struct share_memory_info {
    struct devdrv_mailbox_message_header header;
    u64 first_addr;
    u32 size;
};

/* inform   pid to ts recycle resource */
#define MAX_INFORM_PID_INFO 12
struct aicpu_service_exit_info {
    u32 app_cnt;
    int pid[MAX_INFORM_PID_INFO];
    u8 plat_type;
    u8 reserved[3];
};

struct recycle_service_info {
    struct devdrv_mailbox_message_header header;
    struct aicpu_service_exit_info pid_info;
};

/* inform stream id to ts recycle resource */
#define MAX_INFORM_STREAM_INFO 6
struct devdrv_stream_ts_info {
    u32 stream_cnt;
    int stream[MAX_INFORM_STREAM_INFO];
    u32 status[MAX_INFORM_STREAM_INFO];
    u8 plat_type;
    u8 reserved[3];
};

struct devdrv_mailbox_stream_info {
    struct devdrv_mailbox_message_header header;
    struct devdrv_stream_ts_info stream_info;
};

struct hwts_addr_info {
    struct devdrv_mailbox_message_header header;
    u8 local_devid;
    u8 host_devid;
};

int devdrv_send_stream_info_to_ts(struct devdrv_mailbox *mailbox, u32 stream_id, u32 status);

void devdrv_mailbox_recycle(struct devdrv_mailbox *mailbox);
int devdrv_mailbox_init(struct devdrv_mailbox *mailbox);
void devdrv_mailbox_exit(struct devdrv_mailbox *mailbox);
int devdrv_mailbox_kernel_sync_no_feedback(struct devdrv_mailbox *mailbox, const u8 *buf, u32 len, int *result);
irqreturn_t devdrv_mailbox_ack_irq(int irq, void *data);
irqreturn_t devdrv_mailbox_feedback_irq(int irq, void *data);
int devdrv_send_rdmainfo_to_ts(u32 dev_id, const u8 *buf, u32 len, int *result);
int devdrv_send_recycle_info_to_ts(u32 dev_id, u32 tsid, int pid, int *result);
int devdrv_get_notify_base_addr(u32 devid, u64 *phy_addr);
int devdrv_send_hwts_addr_to_ts(u32 dev_id, u32 tsid, u32 host_id);
#endif /* __DEVDRV_MAILBOX_H */
