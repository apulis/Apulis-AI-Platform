/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2012-2019. All rights reserved.
 * Description: DMA Base Function
 * Author: Huawei Technologies Co., Ltd.
 * Create: 2018/8/18
 */

#ifndef _DMA_COMMON_H_
#define _DMA_COMMON_H_

#include <linux/interrupt.h>
#include <linux/device.h>
#include <linux/spinlock.h>
#include <linux/semaphore.h>

#include "dma_drv.h"
#include "nvme_drv.h"

#include "devdrv_interface.h"

#define DMA_DONE_BUDGET 64

/* sq is 16bit in cq desc, sq size 32byte, guaranteed total size is less than 4M */
#define DEVDRV_MAX_DMA_CH_SQ_DEPTH 0x10000
#define DEVDRV_MAX_DMA_CH_CQ_DEPTH 0x10000
#define DEVDRV_DMA_CH_SQ_DESC_RSV 16
#define DEVDRV_MAX_DMA_SQ_SPACE ((DEVDRV_MAX_DMA_CH_SQ_DEPTH) - (DEVDRV_DMA_CH_SQ_DESC_RSV))

#define DEVDRV_DMA_MAX_REMOTE_IRQ 128

#ifdef CFG_SOC_PLATFORM_ESL_FPGA /* for fpga */
#define DEVDRV_DMA_COPY_TIMEOUT (HZ * 100)
#define DEVDRV_DMA_WAIT_CHAN_AVAIL_TIMEOUT 20000
#else
/* Due to the impact of the previously unfinished dma task, the time-out period cannot
  be given based on the amount of data moved by the dma. Consider the influence of
  the PCIE bus bandwidth and the multi-channel of the DMA, giving a larger waiting time  */
#define DEVDRV_DMA_COPY_TIMEOUT (HZ * 10) /* 10s */
/* wait for dma chan SQ queue when full */
#define DEVDRV_DMA_WAIT_CHAN_AVAIL_TIMEOUT 10000
#endif

/* 10s*/
#define DEVDRV_DMA_QUERY_MAX_WAIT_TIME 10000000

#define DEVDRV_DMA_SQCQ_SIDE_BIT 0
#define DEVDRV_DMA_SML_PKT_BIT 1

#define DEVDRV_DMA_SYNC 1
#define DEVDRV_DMA_ASYNC 2

#define DEVDRV_INVALID_INSTANCE (-1)

#define DEVDRV_DMA_RO_RELEX_ORDER 0x2

#define DEVDRV_SOFTBD_STATUS_INVALID (-2)
#define DEVDRV_DELAY_US 1000

/* device is alive or dead */
#define DEVDRV_DMA_ALIVE 0
#define DEVDRV_DMA_DEAD 1

/* DMA completion status */
enum {
    DEVDRV_DMA_SUCCESS = 0x0,
    DEVDRV_DMA_FAILED = 0x1
};

/* the side of the SQ and CQ of a DMA channel */
enum devdrv_dma_sqcq_side {
    DEVDRV_DMA_LOCAL_SIDE = 0x0,
    DEVDRV_DMA_REMOTE_SIDE = 0x1
};

#define DEVDRV_DMA_SQ_DESC_SIZE sizeof(struct devdrv_dma_sq_node)
#define DEVDRV_DMA_CQ_DESC_SIZE sizeof(struct devdrv_dma_cq_node)

struct devdrv_dma_soft_bd {
    int valid;
    int copy_type;
    int wait_type;
    int owner_bd; /* The number of the last bd sent by the chain  */
    int status;
    u32 trans_id;
    struct semaphore sync_sem;
    void *priv;
    void (*callback_func)(void *, u32, u32);
};

struct devdrv_sync_dma_stat {
        u64 dma_chan_copy_cnt;
        u64 sync_submit_cnt;
        u64 async_submit_cnt;
        u64 sml_submit_cnt;
        u64 trigger_remot_int_cnt;
        u64 trigger_local_128;
        u64 done_int_cnt;
        u64 done_int_in_time;
        u64 re_schedule_cnt;
        u64 done_tasklet_in_cnt;
        u64 done_tasklet_in_time;
        u64 done_tasklet_out_time;
        u64 err_int_cnt;
        u64 err_work_cnt;
        u64 sync_sem_up_cnt;
        u64 async_proc_cnt;
        u64 max_task_op_time;
        u64 last_soft_bd_proced;
        u64 sq_idle_bd_cnt;
};

struct devdrv_dma_channel {
    struct device *dev;
    void __iomem *io_base; /* the base address of DMA channel */
    u32 local_id;
    u32 func_id;
    u32 chan_id; /* the actual index of DMA channel in DMA controller */
    u32 flag;    /* bit0: SQ and CQ side, remote or local;
                 bit1: DMA small packet is supported or not; */
    struct devdrv_dma_sq_node *sq_desc_base;
    struct devdrv_dma_cq_node *cq_desc_base;
    dma_addr_t sq_desc_dma;
    dma_addr_t cq_desc_dma;
    u32 sq_depth;
    u32 cq_depth;
    u32 sq_tail;
    u32 cq_head;
    u32 sq_head;

    struct devdrv_dma_soft_bd *dma_soft_bd;
    struct tasklet_struct dma_done_task;
    struct work_struct err_work;
    int done_irq;
    int err_irq;
    spinlock_t lock;
    spinlock_t cq_lock;
    u32 rounds;
    u32 remote_irq_cnt; /* the count of remote interrupt */
    struct devdrv_sync_dma_stat status;
    struct devdrv_dma_dev *dma_dev;
};

struct devdrv_dma_chan_irq_info {
    u32 done_irq;
    u32 err_irq;
    int err_irq_flag;
};

struct data_type_chan {
    int chan_start_id;
    int chan_num;
    int last_use_chan;
};

struct devdrv_dma_dev {
    u32 en_flg; /* disable during power sleep */
    u32 dev_id;
    u32 func_id;
    u32 dma_irq_base;
    void __iomem *io_base;
    struct device *dev;
    struct data_type_chan data_chan[DEVDRV_DMA_DATA_TYPE_MAX];
    u32 chan_count;
    u32 sq_cq_side;  // DEVDRV_DMA_*_SIDE
    void *drvdata;
    u32 dev_status;
    struct devdrv_dma_channel dma_chan[0];
};

struct devdrv_dma_func_para {
    u32 dev_id;
    u32 chip_id;
    u32 func_id;
    struct device *dev;
    void __iomem *io_base;
    void *drvdata;
    u32 chan_begin;
    u32 chan_num;
    u32 done_irq_base;
    u32 err_irq_base;
    u32 err_flag;
};

void devdrv_dma_config_axim_aruser_mode(void __iomem *io_base);
struct devdrv_dma_dev *devdrv_dma_init(struct devdrv_dma_func_para *para_in, u32 sq_cq_side, u32 func_id);
void devdrv_dma_exit(struct devdrv_dma_dev *dma_dev);

/* these functions is both used in host and device */
struct devdrv_dma_dev *devdrv_get_dma_dev(u32 dev_id);
void devdrv_dfx_dma_report_to_bbox(struct devdrv_dma_channel *dma_chan, u32 queue_init_sts);
void devdrv_dma_check_sram_init_status(const void __iomem *io_base, unsigned long timeout);
int devdrv_register_irq_func(void *drvdata, int vector_index, irqreturn_t (*callback_func)(int, void *), void *para,
                             const char *name);
int devdrv_unregister_irq_func(void *drvdata, int vector_index, void *para);
int devdrv_notify_dma_err_irq(void *drvdata, u32 dma_chan_id, u32 err_irq);
int devdrv_check_dl_dlcmsm_state(void *drvdata);
int devdrv_dma_copy(struct devdrv_dma_dev *dma_dev, enum devdrv_dma_data_type type, int instance,
                    struct devdrv_dma_node *dma_node, u32 node_cnt, int wait_type,int copy_type,
                    struct devdrv_asyn_dma_para_info *para_info);
int devdrv_dma_copy_sml_pkt(struct devdrv_dma_dev *dma_dev, enum devdrv_dma_data_type type, dma_addr_t dst,
                            const void *data, u32 size);
void devdrv_dma_err_proc(struct devdrv_dma_channel *dma_chan);
int devdrv_dma_para_check(u32 dev_id, enum devdrv_dma_data_type type, struct devdrv_dma_node *dma_node, u32 node_cnt,
    int copy_type, struct devdrv_asyn_dma_para_info *para_info);
void devdrv_set_dma_status(struct devdrv_dma_dev *dma_dev, u32 status);

#endif
