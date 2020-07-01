/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2012-2019. All rights reserved.
 * Description:
 * Author: huawei
 * Create: 2017/8/18
 */

#ifndef _DMA_COMM_DRV_H_
#define _DMA_COMM_DRV_H_

#ifdef CONFIG_DEBUG_BUGVERBOSE
#undef CONFIG_DEBUG_BUGVERBOSE
#endif

#include <asm/io.h>
#include "drv_log.h"

#define module_devdrv "drv_pcie"
#define devdrv_err(fmt...) drv_err(module_devdrv, fmt)
#define devdrv_event(fmt...) drv_event(module_devdrv, fmt)
#define devdrv_info(fmt...) drv_info(module_devdrv, fmt)
#define devdrv_debug(fmt...)

#define DEVDRV_DMA_CHAN_OFFSET 0x100

#define DEVDRV_DMA_TIMEOUT 1000000 /* 1s */
#define DEVDRV_DMA_QUEUE_NOT_WORK 0x1

#define DEVDRV_DMA_SML_PKT_DATA_SIZE 32
#define DEVDRV_DMA_SML_PKT_SQ_DESC_NUM 2

/* RO/SO attribute in SQ */
#define DEVDRV_DMA_SQ_ATTR_SO (1 << 0)
#define DEVDRV_DMA_SQ_ATTR_RO (1 << 1)

/* the opcode of SQ descriptor */
enum devdrv_dma_opcode {
    DEVDRV_DMA_SMALL_PACKET = 0x1,
    DEVDRV_DMA_READ = 0x2,
    DEVDRV_DMA_WRITE = 0x3
};

#define DMA_CTRL0_CHAN_EN_OFFSET 0
#define DMA_CTRL0_CHAN_EN_MASK (0x1 << DMA_CTRL0_CHAN_EN_OFFSET)
#define DMA_CTRL0_ERR_ABORT_EN_OFFSET 2
#define DMA_CTRL0_ERR_ABORT_EN_MASK (0x1 << DMA_CTRL0_ERR_ABORT_EN_OFFSET)
#define DMA_CTRL0_PAUSE_OFFSET 4
#define DMA_CTRL0_PAUSE_MASK (0x1 << DMA_CTRL0_PAUSE_OFFSET)
#define DMA_CTRL0_ARB_WAIGHT_OFFSET 8
#define DMA_CTRL0_ARB_WAIGHT_MASK (0xff << DMA_CTRL0_ARB_WAIGHT_OFFSET)
#define DMA_CTRL0_CQSQ_DIR_OFFSET 24
#define DMA_CTRL0_CQSQ_DIR_MASK (0x1 << DMA_CTRL0_CQSQ_DIR_OFFSET)

#define DMA_CTRL1_QUE_RESET_OFFSET 0
#define DMA_CTRL1_QUE_RESET_MASK (0x1 << DMA_CTRL1_QUE_RESET_OFFSET)

#define DMA_CTRL2_PF_NUM_OFFSET 0
#define DMA_CTRL2_PF_NUM_MASK (0x7 << DMA_CTRL2_PF_NUM_OFFSET)

#define DMA_FSM_QUE_STS_OFFSET 0
#define DMA_FSM_QUE_STS_MASK (0xf << DMA_FSM_QUE_STS_OFFSET)
#define DMA_FSM_QUE_WORK_OFFSET 4
#define DMA_FSM_QUE_WORK_MASK (0x1 << DMA_FSM_QUE_WORK_OFFSET)

#define DMA_SQ_HEAD_OFFSET 0
#define DMA_SQ_HEAD_MASK (0xffff << DMA_SQ_HEAD_OFFSET)

#define DMA_CQ_TAIL_OFFSET 0
#define DMA_CQ_TAIL_MASK (0xffff << DMA_CQ_TAIL_OFFSET)

#define DEVDRV_DMA_ERR_MASK 0x2034
#define DEVDRV_DMA_QUEUE_SQ_BASE_L HiPCIECTRL40V200_HIPCIEC_AP_DMA_REG_DMA_QUEUE_SQ_BASE_L_0_REG
#define DEVDRV_DMA_QUEUE_SQ_BASE_H HiPCIECTRL40V200_HIPCIEC_AP_DMA_REG_DMA_QUEUE_SQ_BASE_H_0_REG
#define DEVDRV_DMA_QUEUE_SQ_DEPTH HiPCIECTRL40V200_HIPCIEC_AP_DMA_REG_DMA_QUEUE_SQ_DEPTH_0_REG
#define DEVDRV_DMA_QUEUE_SQ_TAIL HiPCIECTRL40V200_HIPCIEC_AP_DMA_REG_DMA_QUEUE_SQ_TAIL_PTR_0_REG
#define DEVDRV_DMA_QUEUE_CQ_BASE_L HiPCIECTRL40V200_HIPCIEC_AP_DMA_REG_DMA_QUEUE_CQ_BASE_L_0_REG
#define DEVDRV_DMA_QUEUE_CQ_BASE_H HiPCIECTRL40V200_HIPCIEC_AP_DMA_REG_DMA_QUEUE_CQ_BASE_H_0_REG
#define DEVDRV_DMA_QUEUE_CQ_DEPTH HiPCIECTRL40V200_HIPCIEC_AP_DMA_REG_DMA_QUEUE_CQ_DEPTH_0_REG
#define DEVDRV_DMA_QUEUE_CQ_HEAD HiPCIECTRL40V200_HIPCIEC_AP_DMA_REG_DMA_QUEUE_CQ_HEAD_PTR_0_REG
#define DEVDRV_DMA_QUEUE_CTRL0 HiPCIECTRL40V200_HIPCIEC_AP_DMA_REG_DMA_QUEUE_CTRL0_0_REG
#define DEVDRV_DMA_QUEUE_CTRL1 HiPCIECTRL40V200_HIPCIEC_AP_DMA_REG_DMA_QUEUE_CTRL1_0_REG
#define DEVDRV_DMA_QUEUE_FSM_STS HiPCIECTRL40V200_HIPCIEC_AP_DMA_REG_DMA_QUEUE_FSM_STS_0_REG
#define DEVDRV_DMA_QUEUE_SQ_STS HiPCIECTRL40V200_HIPCIEC_AP_DMA_REG_DMA_QUEUE_SQ_STS_0_REG
#define DEVDRV_DMA_QUEUE_CQ_TAIL HiPCIECTRL40V200_HIPCIEC_AP_DMA_REG_DMA_QUEUE_CQ_TAIL_PTR_0_REG
#define DEVDRV_DMA_QUEUE_INT_STS HiPCIECTRL40V200_HIPCIEC_AP_DMA_REG_DMA_QUEUE_INT_STS_0_REG
#define DEVDRV_DMA_QUEUE_INT_MSK HiPCIECTRL40V200_HIPCIEC_AP_DMA_REG_DMA_QUEUE_INT_MSK_0_REG
#define DEVDRV_DMA_QUEUE_DESP0 HiPCIECTRL40V200_HIPCIEC_AP_DMA_REG_DMA_QUEUE_DESP0_0_REG
#define DEVDRV_DMA_QUEUE_DESP1 HiPCIECTRL40V200_HIPCIEC_AP_DMA_REG_DMA_QUEUE_DESP1_0_REG
#define DEVDRV_DMA_QUEUE_DESP2 HiPCIECTRL40V200_HIPCIEC_AP_DMA_REG_DMA_QUEUE_DESP2_0_REG
#define DEVDRV_DMA_QUEUE_DESP3 HiPCIECTRL40V200_HIPCIEC_AP_DMA_REG_DMA_QUEUE_DESP3_0_REG
#define DEVDRV_DMA_QUEUE_ERR_ADDR_L HiPCIECTRL40V200_HIPCIEC_AP_DMA_REG_DMA_QUEUE_ERR_ADDR_L_0_REG
#define DEVDRV_DMA_QUEUE_ERR_ADDR_H HiPCIECTRL40V200_HIPCIEC_AP_DMA_REG_DMA_QUEUE_ERR_ADDR_H_0_REG
#define DEVDRV_DMA_QUEUE_SQ_READ_ERR_PTR HiPCIECTRL40V200_HIPCIEC_AP_DMA_REG_DMA_QUEUE_SQ_READ_ERR_PTR_0_REG
#define DEVDRV_DMA_QUEUE_INT_RO HiPCIECTRL40V200_HIPCIEC_AP_DMA_REG_DMA_QUEUE_INT_RO_0_REG
#define DEVDRV_DMA_INIT_SET HiPCIECTRL40V200_HIPCIEC_AP_DMA_REG_DMA_QUEUE_INT_SET_0_REG
#define DEVDRV_DMA_QUEUE_DESP4 HiPCIECTRL40V200_HIPCIEC_AP_DMA_REG_DMA_QUEUE_DESP4_0_REG
#define DEVDRV_DMA_QUEUE_DESP5 HiPCIECTRL40V200_HIPCIEC_AP_DMA_REG_DMA_QUEUE_DESP5_0_REG
#define DEVDRV_DMA_QUEUE_DESP6 HiPCIECTRL40V200_HIPCIEC_AP_DMA_REG_DMA_QUEUE_DESP6_0_REG
#define DEVDRV_DMA_QUEUE_DESP7 HiPCIECTRL40V200_HIPCIEC_AP_DMA_REG_DMA_QUEUE_DESP7_0_REG
#define DEVDRV_DMA_QUEUE_CTRL2 HiPCIECTRL40V200_HIPCIEC_AP_DMA_REG_DMA_QUEUE_CTRL2_0_REG

#define DEVDRV_DMA_AXI_WRITE 0x100

#define AGENTDRV_AP_SDI_AXIM_REG 0x13000

void devdrv_dma_reg_wr(void __iomem *io_base, u32 offset, u32 val);
void devdrv_dma_reg_rd(const void __iomem *io_base, u32 offset, u32 *val);

int devdrv_get_dma_err_chan(const void __iomem *io_base, u32 *chan_id);

void devdrv_get_dma_queue_sts(const void __iomem *io_base, u32 *val);

void devdrv_set_dma_sq_tail(void __iomem *io_base, u32 val);
void devdrv_set_dma_cq_head(void __iomem *io_base, u32 val);

int devdrv_dma_ch_cfg_reset(void __iomem *io_base);
void devdrv_dma_ch_cfg_init(void __iomem *io_base, u64 sq_addr, u64 cq_addr, u32 sq_depth, u32 cq_depth, u32 pf_num,
                            u32 sqcq_side);
void devdrv_record_dma_dxf_info(void __iomem *io_base, u32 *queue_init_sts);

void devdrv_dma_set_sq_addr_info(struct devdrv_dma_sq_node *sq_desc, u64 src_addr, u64 dst_addr, u32 length);

void devdrv_dma_set_sq_attr(struct devdrv_dma_sq_node *sq_desc, u32 opcode, u32 attr, u32 pf, u32 wd_barrier,
                            u32 rd_barrier);

void devdrv_dma_set_sq_irq(struct devdrv_dma_sq_node *sq_desc, u32 rdie, u32 ldie, u32 msi);

bool devdrv_dma_get_cq_valid(struct devdrv_dma_cq_node *cq_desc, u32 rounds);
void devdrv_dma_set_cq_invalid(struct devdrv_dma_cq_node *cq_desc);
u32 devdrv_dma_get_cq_sqhd(struct devdrv_dma_cq_node *cq_desc);
u32 devdrv_dma_get_cq_status(struct devdrv_dma_cq_node *cq_desc);

int devdrv_dma_is_axi_write_error(const void __iomem *io_base);
void devdrv_set_dma_chan_en(void __iomem *io_base, u32 val);
void devdrv_dma_err_interrupt_unmask(void __iomem *io_base);
#endif
