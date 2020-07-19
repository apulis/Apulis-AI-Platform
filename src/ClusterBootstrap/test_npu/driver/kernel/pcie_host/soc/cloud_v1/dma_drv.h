/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2012-2019. All rights reserved.
 * Description:
 * Author: huawei
 * Create:2019
 */

#ifndef _DMA_DRV_H_
#define _DMA_DRV_H_

#include "hipciec_ap_dma_reg_reg_offset.h"

#define HISI_IEP_DMA_DEVICE_ID 0xa122

#define DEVDRV_DMA_MSI_MAX_VECTORS 32
#define DMA_CONFIG_REGVAL_OFFSET (0x1 << 11)

#define DEVDRV_DMA_CHAN_NUM 30
#define DMA_DONE_IRQ_NUM 30
#define DMA_DONE_IRQ_BASE 0
#define DMA_ERR_IRQ_NUM 1
#define DMA_ERR_IRQ_BASE 31

#define DMA_ERR_MASK 0x1ffe

#define DEVDRV_DMA_CHANNEL_IDLE_STATE 0x0
#define DEVDRV_DMA_CHANNEL_RUN_STATE 0x1
#define DEVDRV_DMA_CHANNEL_CPL_STATE 0x2
#define DEVDRV_DMA_CHANNEL_PAUSE_STATE 0x3
#define DEVDRV_DMA_CHANNEL_HALT_STATE 0x4
#define DEVDRV_DMA_CHANNEL_ABORT_STATE 0x5
#define DEVDRV_DMA_CHANNEL_WAIT_STATE 0x6
#define DEVDRV_DMA_CHANNEL_BUFFCLR_STATE 0x7

#define DEVDRV_ADDR_MOVE 32
#define DEVDRV_DMA_CQ_HEAD(cq_depth) 0

struct devdrv_dma_sq_node {
    u32 opcode : 4;
    u32 drop : 1;
    u32 nw : 1;
    u32 wd_barrier : 1;
    u32 rd_barrier : 1;
    u32 ldie : 1;
    u32 rdie : 1;
    u32 reserved2 : 2;
    u32 attr : 3;
    u32 reserved3 : 1;
    u32 addrt : 2;
    u32 reserved4 : 2;
    u32 pf : 3;
    u32 vfen : 1;
    u32 vf : 8;
    u32 pasid : 20;
    u32 er : 1;
    u32 pmr : 1;
    u32 prfen : 1;
    u32 reserved5 : 1;
    u32 msi_l : 8;
    u32 flow_id : 8;
    u32 msi_h : 3;
    u32 reserved6 : 5;
    u32 th : 1;
    u32 ph : 2;
    u32 reserved7 : 5;
    u32 attr_d : 3;
    u32 addrt_d : 2;
    u32 th_d : 1;
    u32 ph_d : 2;
    u32 length;
    u32 src_addr_l;
    u32 src_addr_h;
    u32 dst_addr_l;
    u32 dst_addr_h;
};

#define DMA_MSI_L_MASK 0xff
#define DMA_MSI_H_BIT_OFFSET 8
#define DMA_MSI_H_MASK 0x7

struct devdrv_dma_cq_node {
    u32 reserved1;
    u32 reserved2;
    u32 sqhd : 16;
    u32 reserved3 : 16;
    u32 reserved4 : 16;
    u32 vld : 1;
    u32 status : 15;
};

#include "dma_comm_drv.h"

#define DEVDRV_DMA_COMMON_AND_CH_ERR_STS HiPCIECTRL40V200_HIPCIEC_AP_DMA_REG_COMMON_AND_CH_ERR_STS_REG

#define DEVDRV_SDI_AXIM_AWUSER_MODE_CTRL 0x230
#define DEVDRV_SDI_AXIM_AWUSER_SET_1 0x238

#endif
