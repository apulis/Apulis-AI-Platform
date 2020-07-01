/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2012-2019. All rights reserved.
 * Description:
 * Author: huawei
 * Create: Mon May 20 17:22:22 2019
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

#define CPU_TYPE_OF_CCPU 0
#define CPU_TYPE_OF_TS 1
#define CPU_TYPE_OF_AICPU 2
#define CPU_TYPE_OF_DCPU 3

#define DEVDRV_SCLID 2
#define DEVDRV_CCPU_CLUSTER 0
#define DEVDRV_AICPU_CLUSTER 0
#define DEVDRV_TSCPU_CLUSTER 4

/* *************** aicpu config *************** */
#define AICORE_NUM 1
#define AICPU_CLUSTER_MAX_NUM 4
#define AICPU_MAX_NUM 16

#define CONFIG_CORE_PER_CLUSTER 4

#define FW_CPU_ID_BASW_ESL 4
#define FW_CPU_ID_BASW_FPGA 4
#define FIRMWARE_ALIGN_OFFSET_FPGA_ESL 16

#define FW_CPU_ID_BASW (num_online_cpus())
#define FW_CPU_NUM 1
#define FW_CLUSTER_ID_BASW (FW_CPU_ID_BASW / CONFIG_CORE_PER_CLUSTER)

#define CONFIG_CLUSTER_PER_TOTEM ((FW_CPU_NUM + 1) / CONFIG_CORE_PER_CLUSTER)

#define DEVDRV_MAILBOX_SEND_OFFLINE_IRQ     1
#define DEVDRV_CQ_PER_IRQ                   16
#define DEVDRV_CQ_UPDATE_IRQ_SUM            31
#define DEVDRV_CQ_IRQ_NUM                   32

#define DEVDRV_TS_MEMORY_SIZE (160 * 1024 * 1024)
#define DEVDRV_TS_DOORBELL_SIZE ((DEVDRV_TS_DOORBELL_SQ_NUM + DEVDRV_TS_DOORBELL_CQ_NUM) * DEVDRV_TS_DOORBELL_STRIDE)
#define TS_IRQ_NUMBER 4
#define CLOUD_CHIP0_TS_DOORBELL_IRQ 103
#define CLOUD_CHIP1_TS_DOORBELL_IRQ 151
#define CLOUD_CHIP2_TS_DOORBELL_IRQ 199
#define CLOUD_CHIP3_TS_DOORBELL_IRQ 247

#define CHIP0_TS_INTERRUPT_NUM_HWTS_NORMAL 99
#define CHIP1_TS_INTERRUPT_NUM_HWTS_NORMAL 147
#define CHIP2_TS_INTERRUPT_NUM_HWTS_NORMAL 195
#define CHIP3_TS_INTERRUPT_NUM_HWTS_NORMAL 243

#define CHIP0_TS_INTERRUPT_NUM_HWTS_ERROR 100
#define CHIP1_TS_INTERRUPT_NUM_HWTS_ERROR 148
#define CHIP2_TS_INTERRUPT_NUM_HWTS_ERROR 196
#define CHIP3_TS_INTERRUPT_NUM_HWTS_ERROR 244
#define CHIP0_TS_INTERRUPT_NUM_HWTS_DEBUG 101
#define CHIP1_TS_INTERRUPT_NUM_HWTS_DEBUG 149
#define CHIP2_TS_INTERRUPT_NUM_HWTS_DEBUG 197
#define CHIP3_TS_INTERRUPT_NUM_HWTS_DEBUG 245

enum devdrv_dts_addr_index {
    DEVDRV_DTS_GIC_BASE_INDEX = 0,
    DEVDRV_DTS_TS_SUBSYSCTL_INDEX = 1,
    DEVDRV_DTS_TS_DOORBELL_INDEX = 2,
    DEVDRV_DTS_TS_SRAM_INDEX = 3,
    DEVDRV_DTS_DISPATCH_INDEX = 4,
    DEVDRV_DTS_SYSCTL_INDEX = 5,
    DEVDRV_DTS_MN_INDEX = 6,
    DEVDRV_DTS_LPM3_STATUS_INDEX = 7,
    DEVDRV_DTS_FW_CORE_RESET_ENTRY_INDEX = 8,
    DEVDRV_DTS_FW_CLUSTER0_SUBCTRL_INDEX = 9,
    DEVDRV_DTS_TS_FW_CORE_RESET_ENTRY_INDEX = 10,
    DEVDRV_DTS_TSENSOR_SHRAEMEN_INDEX = 11,
    DEVDRV_DTS_MAX_RESOURCE_NODE = 12,
};

#define DEVDRV_MAX_TS_CORE_NUM      4

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

    phys_addr_t sram_paddr;
    phys_addr_t ts_mbox_send_paddr;
    phys_addr_t ts_mbox_rcv_paddr;
    phys_addr_t doorbell_paddr;
    phys_addr_t tsensor_shm_paddr;
    phys_addr_t ts_sysctl_paddr;

    size_t sram_size;
    size_t ts_mbox_send_size;
    size_t ts_mbox_rcv_size;
    size_t doorbell_size;
    size_t tsensor_shm_size;
    size_t ts_sysctl_size;

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
    u32 aicpu_partial_good_enable;
};

struct devdrv_platform_data {
    u32 dev_id;
    u32 env_type;
    u32 ai_core_num;
    union {
        struct devdrv_pci_info pci_info;
        struct devdrv_platform_info platform_info;
    };
    u32 ts_num; /* ts number for this platform */
    struct devdrv_ts_pdata ts_pdata[DEVDRV_MAX_TS_NUM];
};

enum dev_chip_id {
    CHIP0_ID,
    CHIP1_ID,
    CHIP2_ID,
    CHIP3_ID,
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
