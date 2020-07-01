/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2012-2019. All rights reserved.
 * Description:
 * Author: huawei
 * Create:
 * Notes:
 * History:
 *
 */

#ifndef __DEVDRV_PLATFORM_RESOURCE_H
#define __DEVDRV_PLATFORM_RESOURCE_H

#include "devdrv_common.h"

#define DEVDRV_TS_BINARY_PATH "/var/tsch_fw.bin"
#define DEVDRV_AICPU_BINARY_PATH "/var/aicpu_fw.bin"

#define DEVDRV_FUNCTIONAL_CQ_UPDATE_IRQ 31
#define DEVDRV_MAILBOX_ACK_IRQ 32
#define DEVDRV_MAILBOX_DATA_ACK_IRQ 33
#define DEVDRV_PERI_DISP_RAS_CTRL_NFE 34

#define DEVDRV_SCLID 2
#define DEVDRV_CCPU_CLUSTER 0
#define DEVDRV_AICPU_CLUSTER 0
#define DEVDRV_TSCPU_CLUSTER 1

/* all CPU numbers, including control CPU */
#define AICPU_MAX_NUM 16
#define CONFIG_CORE_PER_CLUSTER 0x8
#define CONFIG_CLUSTER_PER_TOTEM 0x1

/* *************** aicpu config *************** */
#define FW_CPU_ID_BASW_OF_FPGA 2
#define FW_CPU_NUM_OF_FPGA 2
#define FW_CPU_ID_BASW 4
#define FW_CPU_NUM 4

#define DEVDRV_MAILBOX_SEND_OFFLINE_IRQ 1
#define DEVDRV_CQ_PER_IRQ 16
#define DEVDRV_CQ_UPDATE_IRQ_SUM 1
#define DEVDRV_CQ_IRQ_NUM 32

#define CPU_TYPE_OF_CCPU 0
#define CPU_TYPE_OF_TS 1
#define CPU_TYPE_OF_AICPU 2
#define CPU_TYPE_OF_DCPU 3
#define DEVDRV_TS_MEMORY_SIZE (184 * 1024 * 1024)
#define DEVDRV_TS_DOORBELL_SIZE             \
    ((DEVDRV_TS_DOORBELL_SQ_NUM + DEVDRV_TS_DOORBELL_CQ_NUM) * \
        DEVDRV_TS_DOORBELL_STRIDE)

enum devdrv_dts_addr_index {
    DEVDRV_DTS_GIC_BASE_INDEX = 0,
    DEVDRV_DTS_DISPATCH_INDEX = 1,
    DEVDRV_DTS_SYSCTL_INDEX = 2,
    DEVDRV_DTS_MN_INDEX = 3,
    DEVDRV_DTS_LPM3_STATUS_INDEX = 4,
    DEVDRV_DTS_FW_CORE_RESET_ENTRY_INDEX = 5,
    DEVDRV_DTS_FW_CLUSTER0_SUBCTRL_INDEX = 6,
    DEVDRV_DTS_TS_FW_CORE_RESET_ENTRY_INDEX = 7,
    DEVDRV_DTS_MAX_RESOURCE_NODE = 8,
};

struct devdrv_id_data {
    /*
     * how many sq num available for current device & host
     * used for resource initialization
     */
    u32 min_sq_id;
    u32 max_sq_id;
    u32 min_cq_id;
    u32 max_cq_id;
    u32 min_stream_id;
    u32 max_stream_id;
    u32 min_event_id;
    u32 max_event_id;
    u32 min_model_id;
    u32 max_model_id;
    u32 min_notify_id;
    u32 max_notify_id;
    u32 min_task_id;
    u32 max_task_id;

    /* resource numbers on this side!! */
    u32 cur_stream_num;
    u32 cur_event_num;
    u32 cur_sq_num;
    u32 cur_cq_num;
    u32 cur_model_num;
    u32 cur_notify_num;
    u32 cur_task_num;
};

struct devdrv_ts_pdata {
    u32 tsid;

    void __iomem *sram_vaddr;
    u8 __iomem *ts_mbox_send_vaddr;
    u8 __iomem *ts_mbox_rcv_vaddr;
    u32 __iomem *ts_sysctl_vaddr;
    void __iomem *doorbell_vaddr;
    void __iomem *tsensor_shm_vaddr;

    u64 sram_paddr;
    u64 ts_mbox_send_paddr;
    u64 ts_mbox_rcv_paddr;
    u64 doorbell_paddr;
    u64 tsensor_shm_paddr;
    u64 ts_sysctl_paddr;

    u32 sram_size;
    u32 ts_mbox_send_size;
    u32 ts_mbox_rcv_size;
    u32 doorbell_size;
    u32 tsensor_shm_size;
    u32 ts_sysctl_size;

    int irq_cq_update[DEVDRV_CQ_IRQ_NUM];
    int irq_mailbox_ack;
    int irq_mailbox_data_ack;
    int irq_functional_cq;
    int disp_nfe_irq;

    u64 ts_sq_static_addr;
    size_t ts_sq_static_size;

    u8 ts_start_fail;
    int ts_load_fail;

    dma_addr_t ts_dma_handle;
    void *ts_load_addr;

    u32 ts_cpu_core_num;

    struct devdrv_id_data id_data;
};

struct devdrv_platform_info {
    u32 board_id;
    u32 slot_id;

    u32 occupy_bitmap;

    void __iomem *gicv3_base;
    void __iomem *sysctl_base;
    void __iomem *mn_vaddr;
    void __iomem *lpm3_status;
    void __iomem *disp_base;

    u32 sclid;
    u32 ts_cluster;
    u32 ccpu_cluster;
    u32 aicpu_cluster;

    u64 devdrv_addr_base[DEVDRV_DTS_MAX_RESOURCE_NODE];
};

struct devdrv_platform_data {
    u32 dev_id;
    u32 env_type;
    u32 ai_core_num;
    union {
        struct devdrv_pci_info pci_info;
        struct devdrv_platform_info platform_info;
    };

    u32 ts_num; /* actual ts number for this platform */
    struct devdrv_ts_pdata ts_pdata[DEVDRV_MAX_TS_NUM];
};

enum dev_chip_id {
    CHIP0_ID,
    CHIP1_ID,
    MAX_CHIP_NUM,
};

static inline struct devdrv_id_data *devdrv_get_id_data(struct devdrv_platform_data *pdata, u32 tsid)
{
    return &pdata->ts_pdata[tsid].id_data;
}

static inline struct devdrv_ts_pdata *devdrv_get_ts_data(struct devdrv_platform_data *pdata, u32 tsid)
{
    return &pdata->ts_pdata[tsid];
}


#endif /* __DEVDRV_PLATFORM_RESOURCE_H */
