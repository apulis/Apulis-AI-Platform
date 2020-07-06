/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2012-2019. All rights reserved.
 * Description: Host Msg
 * Author: Huawei Technologies Co., Ltd.
 * Create: 2018/8/18
 */

#ifndef __DEVDRV_MSG_H_
#define __DEVDRV_MSG_H_

#include <linux/interrupt.h>
#include <linux/pci.h>
#include <linux/mutex.h>
#include <linux/jiffies.h>

#include "devdrv_dma.h"
#include "devdrv_pci.h"
#include "devdrv_common_msg.h"
#include "devdrv_interface.h"
#include "devdrv_msg_def.h"

#define DEVDRV_SUCCESS 0x5a
#define DEVDRV_FAILED 0xa5

/* device is alive or dead */
#define DEVDRV_DEVICE_ALIVE 0
#define DEVDRV_DEVICE_DEAD 1
#define DEVDRV_DEVICE_REMOVE 2


/* sync or async msg type */
#define DEVDRV_MSG_SYNC (0 << 0)
#define DEVDRV_MSG_ASYNC (1 << 0)

#define DEVDRV_MSG_CHAN_IRQ_NUM 2
#define DEVDRV_DB_QUEUE_TYPE 2
#define DEVDRV_MSG_CHAN_CNT_3559 5

struct devdrv_msg_queue_info {
    u32 depth;
    u32 desc_size;
    dma_addr_t dma_handle;
    void *desc_h;
    void *desc_d;
    u32 slave_mem_offset;
    u32 head_h;
    u32 tail_h;
    u32 head_d;
    u32 tail_d;
    s32 irq_vector;
};

struct devdrv_msg_chan_stat {
    u64 tx_total_cnt;
    u64 tx_success_cnt;
    u64 tx_len_check_err;
    u64 tx_reply_len_check_err;
    u64 tx_status_abnormal_err;
    u64 tx_irq_timeout_err;
    u64 tx_timeout_err;
    u64 tx_process_err;
    u64 tx_invalid_para_err;
    u64 rx_total_cnt;
    u64 rx_success_cnt;
    u64 rx_para_err;
    u64 rx_work_max_time;
    u64 rx_work_delay_cnt;
};

struct devdrv_msg_chan {
    struct devdrv_msg_dev *msg_dev;
    u32 chan_id;
    u64 seq_num;
    u32 status; /* if this channel is used */
    void *priv; /* inited by vnic */
    u32 flag;   /* sync or async:bit0 */
    struct devdrv_msg_queue_info sq_info;
    struct devdrv_msg_queue_info cq_info;
    void __iomem *io_base; /* the address of SQ tail db */
    enum devdrv_msg_client_type msg_type;
    int irq_rx_msg_notify;
    int irq_tx_finish_notity;
    void (*rx_msg_notify)(void *msg_chan);
    void (*tx_finish_notify)(void *msg_chan);
    int (*rx_msg_process)(void *msg_chan, void *data, u32 in_data_len, u32 out_data_len, u32 *real_out_len);
    int rx_work_flag;
    struct work_struct rx_work;
    struct mutex mutex;
    u32 stamp;
    struct devdrv_msg_chan_stat chan_stat;
    enum msg_queue_type queue_type;
};

struct devdrv_msg_slave_mem {
    u32 offset;
    u32 len;
};

struct devdrv_msg_slave_mem_node {
    struct devdrv_msg_slave_mem mem;
    struct list_head list;
};

struct devdrv_non_trans_msg_send_data_para {
    void *data;
    u32 in_data_len;
    u32 out_data_len;
    u32 *real_out_len;
};

struct devdrv_msg_dev {
    struct devdrv_pci_ctrl *pci_ctrl;
    struct device *dev;
    void __iomem *db_io_base;   /* the base addr of doorbell */
    void __iomem *ctrl_io_base; /* the base addr of nvme ctrl reg */
    void __iomem *reserve_mem_base;
    u32 chan_cnt;
    u32 func_id;
    struct devdrv_common_msg common_msg;
    struct mutex mutex;
    struct devdrv_msg_chan *admin_msg_chan;

    struct workqueue_struct *work_queue;

    struct devdrv_msg_slave_mem slave_mem;
    struct list_head slave_mem_list; /* for realloc msg chan */

    /* msg_chan must be the last element */
    struct devdrv_msg_chan msg_chan[0];
};

struct devdrv_msg_chan_info {
    enum devdrv_msg_client_type msg_type;
    enum msg_queue_type queue_type;
    u32 level;
    u32 queue_depth;
    /* param for trans */
    u32 sq_desc_size;
    u32 cq_desc_size;
    void (*rx_msg_notify)(void *msg_chan);
    void (*tx_finish_notify)(void *msg_chan);
    /* param for non-trans */
    u32 flag; /* sync or async:bit0 */
    int (*rx_msg_process)(void *msg_chan, void *data, u32 in_data_len, u32 out_data_len, u32 *real_out_len);
};

struct devdrv_msg_chan *devdrv_alloc_trans_queue(void *priv, struct devdrv_trans_msg_chan_info *chan_info);
int devdrv_free_trans_queue(struct devdrv_msg_chan *msg_chan);

struct devdrv_msg_chan *devdrv_alloc_non_trans_queue(void *priv, struct devdrv_non_trans_msg_chan_info *chan_info);
int devdrv_free_non_trans_queue(struct devdrv_msg_chan *msg_chan);

int devdrv_msg_init(struct devdrv_pci_ctrl *pci_ctrl, int irq_base, int irq_num);
void devdrv_msg_exit(struct devdrv_pci_ctrl *pci_ctrl);

int devdrv_sync_non_trans_msg_send(struct devdrv_msg_chan *msg_chan, void *data, u32 in_data_len, u32 out_data_len,
                                   u32 *real_out_len, enum devdrv_common_msg_type msg_type);

int devdrv_get_general_interrupt_db_info(struct devdrv_msg_dev *msg_dev, u32 *db_start, u32 *db_num);
int devdrv_notify_dev_online(struct devdrv_msg_dev *msg_dev, u32 devid, u32 status);
void devdrv_set_device_status(struct devdrv_pci_ctrl *pci_ctrl, u32 status);

int devdrv_admin_msg_chan_send(struct devdrv_msg_dev *msg_dev, enum devdrv_admin_msg_opcode opcode, const void *cmd,
                               int size, void *reply, int reply_size);
int devdrv_get_rx_atu_info(struct devdrv_pci_ctrl *pci_ctrl, unsigned int bar_num);
#endif
