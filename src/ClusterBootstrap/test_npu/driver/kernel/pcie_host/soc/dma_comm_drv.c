/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2012-2019. All rights reserved.
 * Description:
 * Author: huawei
 * Create: 2017/8/18
 */

#include <linux/delay.h>
#include <linux/errno.h>

#include "dma_drv.h"

void devdrv_dma_reg_wr(void __iomem *io_base, u32 offset, u32 val)
{
    writel(val, io_base + offset);
}

void devdrv_dma_reg_rd(const void __iomem *io_base, u32 offset, u32 *val)
{
    *val = readl(io_base + offset);
}

void devdrv_set_dma_sq_base(void __iomem *io_base, u64 val)
{
    devdrv_dma_reg_wr(io_base, DEVDRV_DMA_QUEUE_SQ_BASE_H, (u32)(val >> 32));
    devdrv_dma_reg_wr(io_base, DEVDRV_DMA_QUEUE_SQ_BASE_L, (u32)val);
}

void devdrv_set_dma_sq_depth(void __iomem *io_base, u32 val)
{
    /* the controller will +1 at the set depth by software */
    devdrv_dma_reg_wr(io_base, DEVDRV_DMA_QUEUE_SQ_DEPTH, val - 1);
}

void devdrv_set_dma_sq_tail(void __iomem *io_base, u32 val)
{
    devdrv_dma_reg_wr(io_base, DEVDRV_DMA_QUEUE_SQ_TAIL, val);
}

void devdrv_set_dma_cq_base(void __iomem *io_base, u64 val)
{
    devdrv_dma_reg_wr(io_base, DEVDRV_DMA_QUEUE_CQ_BASE_H, (u32)(val >> 32));
    devdrv_dma_reg_wr(io_base, DEVDRV_DMA_QUEUE_CQ_BASE_L, (u32)val);
}

void devdrv_set_dma_cq_depth(void __iomem *io_base, u32 val)
{
    /* the controller will +1 at the set depth by software */
    devdrv_dma_reg_wr(io_base, DEVDRV_DMA_QUEUE_CQ_DEPTH, val - 1);
}

void devdrv_set_dma_cq_head(void __iomem *io_base, u32 val)
{
    devdrv_dma_reg_wr(io_base, DEVDRV_DMA_QUEUE_CQ_HEAD, val);
}

void devdrv_set_dma_queue_pause(void __iomem *io_base, u32 val)
{
    u32 regval;

    devdrv_dma_reg_rd(io_base, DEVDRV_DMA_QUEUE_CTRL0, &regval);
    regval &= ~DMA_CTRL0_PAUSE_MASK;
    regval |= (val << DMA_CTRL0_PAUSE_OFFSET) & DMA_CTRL0_PAUSE_MASK;

    devdrv_dma_reg_wr(io_base, DEVDRV_DMA_QUEUE_CTRL0, regval);
}

void devdrv_set_dma_arb_weight(void __iomem *io_base, u32 val)
{
    u32 regval;

    devdrv_dma_reg_rd(io_base, DEVDRV_DMA_QUEUE_CTRL0, &regval);
    regval &= ~DMA_CTRL0_ARB_WAIGHT_MASK;
    regval |= (val << DMA_CTRL0_ARB_WAIGHT_OFFSET) & DMA_CTRL0_ARB_WAIGHT_MASK;

    devdrv_dma_reg_wr(io_base, DEVDRV_DMA_QUEUE_CTRL0, regval);
}

void devdrv_set_dma_err_abort_en(void __iomem *io_base, u32 val)
{
    u32 regval;

    devdrv_dma_reg_rd(io_base, DEVDRV_DMA_QUEUE_CTRL0, &regval);
    regval &= ~DMA_CTRL0_ERR_ABORT_EN_MASK;
    regval |= (val << DMA_CTRL0_ERR_ABORT_EN_OFFSET) & DMA_CTRL0_ERR_ABORT_EN_MASK;

    devdrv_dma_reg_wr(io_base, DEVDRV_DMA_QUEUE_CTRL0, regval);
}

void devdrv_set_dma_chan_en(void __iomem *io_base, u32 val)
{
    u32 regval;

    devdrv_dma_reg_rd(io_base, DEVDRV_DMA_QUEUE_CTRL0, &regval);
    regval &= ~DMA_CTRL0_CHAN_EN_MASK;
    regval |= (val << DMA_CTRL0_CHAN_EN_OFFSET) & DMA_CTRL0_CHAN_EN_MASK;

    devdrv_dma_reg_wr(io_base, DEVDRV_DMA_QUEUE_CTRL0, regval);
}

void devdrv_get_dma_chan_en(const void __iomem *io_base, u32 *val)
{
    u32 regval;

    devdrv_dma_reg_rd(io_base, DEVDRV_DMA_QUEUE_CTRL0, &regval);
    *val = (regval & DMA_CTRL0_CHAN_EN_MASK) >> DMA_CTRL0_CHAN_EN_OFFSET;
}

void devdrv_set_dma_sqcq_dir(void __iomem *io_base, u32 val)
{
    u32 regval;

    devdrv_dma_reg_rd(io_base, DEVDRV_DMA_QUEUE_CTRL0, &regval);
    regval &= ~DMA_CTRL0_CQSQ_DIR_MASK;
    regval |= (val << DMA_CTRL0_CQSQ_DIR_OFFSET) & DMA_CTRL0_CQSQ_DIR_MASK;

    devdrv_dma_reg_wr(io_base, DEVDRV_DMA_QUEUE_CTRL0, regval);
}

void devdrv_set_dma_queue_reset(void __iomem *io_base, u32 val)
{
    u32 regval;

    devdrv_dma_reg_rd(io_base, DEVDRV_DMA_QUEUE_CTRL1, &regval);
    regval &= ~DMA_CTRL1_QUE_RESET_MASK;
    regval |= (val << DMA_CTRL1_QUE_RESET_OFFSET) & DMA_CTRL1_QUE_RESET_MASK;

    devdrv_dma_reg_wr(io_base, DEVDRV_DMA_QUEUE_CTRL1, regval);
}

void devdrv_get_dma_queue_sts(const void __iomem *io_base, u32 *val)
{
    u32 regval;

    devdrv_dma_reg_rd(io_base, DEVDRV_DMA_QUEUE_FSM_STS, &regval);
    *val = (regval & DMA_FSM_QUE_STS_MASK) >> DMA_FSM_QUE_STS_OFFSET;
}

void devdrv_get_dma_queue_not_work(const void __iomem *io_base, u32 *val)
{
    u32 regval;

    devdrv_dma_reg_rd(io_base, DEVDRV_DMA_QUEUE_FSM_STS, &regval);
    *val = (regval & DMA_FSM_QUE_WORK_MASK) >> DMA_FSM_QUE_WORK_OFFSET;
}

void devdrv_dma_interrupt_mask(void __iomem *io_base, u32 val)
{
    devdrv_dma_reg_wr(io_base, DEVDRV_DMA_QUEUE_INT_MSK, val);
}

void devdrv_set_dma_pf_num(void __iomem *io_base, u32 val)
{
    u32 regval;

    devdrv_dma_reg_rd(io_base, DEVDRV_DMA_QUEUE_CTRL2, &regval);
    regval &= ~DMA_CTRL2_PF_NUM_MASK;
    regval |= (val << DMA_CTRL2_PF_NUM_OFFSET) & DMA_CTRL2_PF_NUM_MASK;

    devdrv_dma_reg_wr(io_base, DEVDRV_DMA_QUEUE_CTRL2, regval);
}

int devdrv_dma_is_axi_write_error(const void __iomem *io_base)
{
    u32 val;
    const int NO_SPECIAL_ERROR = 0;
    const int SPECIAL_ERROR = 1;

    devdrv_dma_reg_rd(io_base, DEVDRV_DMA_QUEUE_INT_STS, &val);
    if (val == DEVDRV_DMA_AXI_WRITE) {
        return SPECIAL_ERROR;
    }
    return NO_SPECIAL_ERROR;
}

void devdrv_record_sq_err_ptr(const void __iomem *io_base)
{
    u32 regval;

    devdrv_dma_reg_rd(io_base, DEVDRV_DMA_QUEUE_SQ_READ_ERR_PTR, &regval);
    devdrv_err("the sq read err ptr is: %d\n", regval);
}

void devdrv_record_err_addr(const void __iomem *io_base)
{
    u32 regval;
    u64 val;

    devdrv_dma_reg_rd(io_base, DEVDRV_DMA_QUEUE_ERR_ADDR_H, &regval);
    val = ((u64)regval << 32);

    devdrv_dma_reg_rd(io_base, DEVDRV_DMA_QUEUE_ERR_ADDR_L, &regval);
    val |= regval;

    devdrv_err("the err addr is\n");
}

void devdrv_record_dma_queue_desp(const void __iomem *io_base)
{
    u32 reg_val;

    devdrv_dma_reg_rd(io_base, DEVDRV_DMA_QUEUE_DESP0, &reg_val);
    devdrv_err("the 1st DW of current descriptor is reg_val: 0x%x\n", reg_val);
    devdrv_dma_reg_rd(io_base, DEVDRV_DMA_QUEUE_DESP1, &reg_val);
    devdrv_err("the 2st DW of current descriptor is reg_val: 0x%x\n", reg_val);
    devdrv_dma_reg_rd(io_base, DEVDRV_DMA_QUEUE_DESP2, &reg_val);
    devdrv_err("the 3st DW of current descriptor is reg_val: 0x%x\n", reg_val);
    devdrv_dma_reg_rd(io_base, DEVDRV_DMA_QUEUE_DESP3, &reg_val);
    devdrv_err("the 4st DW of current descriptor is reg_val: 0x%x\n", reg_val);
    devdrv_dma_reg_rd(io_base, DEVDRV_DMA_QUEUE_DESP4, &reg_val);
    devdrv_err("the 5st DW of current descriptor is reg_val: 0x%x\n", reg_val);
    devdrv_dma_reg_rd(io_base, DEVDRV_DMA_QUEUE_DESP5, &reg_val);
    devdrv_err("the 6st DW of current descriptor is reg_val: 0x%x\n", reg_val);
    devdrv_dma_reg_rd(io_base, DEVDRV_DMA_QUEUE_DESP6, &reg_val);
    devdrv_err("the 7st DW of current descriptor is reg_val: 0x%x\n", reg_val);
    devdrv_dma_reg_rd(io_base, DEVDRV_DMA_QUEUE_DESP7, &reg_val);
    devdrv_err("the 8st DW of current descriptor is reg_val: 0x%x\n", reg_val);
}

/* printk the dma channel dxf register */
void devdrv_record_dma_dxf_info(void __iomem *io_base, u32 *queue_init_sts)
{
    u32 val = 0;
    devdrv_record_dma_queue_desp(io_base);
    devdrv_record_err_addr(io_base);
    devdrv_record_sq_err_ptr(io_base);
    devdrv_dma_reg_rd(io_base, DEVDRV_DMA_QUEUE_SQ_STS, &val);
    devdrv_err("the hardware sq head is %d\n", val & 0xFFFF);
    devdrv_dma_reg_rd(io_base, DEVDRV_DMA_QUEUE_SQ_TAIL, &val);
    devdrv_err("the hardware sq tail is %d\n", val);
    devdrv_dma_reg_rd(io_base, DEVDRV_DMA_QUEUE_CQ_HEAD, &val);
    devdrv_err("the hardware cq head is %d\n", val);
    devdrv_dma_reg_rd(io_base, DEVDRV_DMA_QUEUE_CQ_TAIL, &val);
    devdrv_err("the hardware cq tail is %d\n", val);
    devdrv_dma_reg_rd(io_base, DEVDRV_DMA_QUEUE_INT_STS, &val);
    devdrv_err("the err type is: 0x%x\n", val);
    *queue_init_sts = val;
    devdrv_dma_reg_wr(io_base, DEVDRV_DMA_QUEUE_INT_STS, val & 0x1FFE);
}

int devdrv_dma_check_queue_sts(const void __iomem *io_base, unsigned long timeout)
{
    u32 dma_queue_sts;
    int ret = 0;

    devdrv_get_dma_queue_sts(io_base, &dma_queue_sts);
    while (dma_queue_sts == DEVDRV_DMA_CHANNEL_RUN_STATE) {
        if (timeout-- == 0) {
            ret = -ETIMEDOUT;
            break;
        }

        devdrv_get_dma_queue_sts(io_base, &dma_queue_sts);
        devdrv_debug("dma_queue_sts %x\n", dma_queue_sts);
        udelay(1);
    }

    return ret;
}

int devdrv_dma_check_fsm_sts(const void __iomem *io_base, unsigned long timeout)
{
    u32 dma_queue_sts, dma_queue_not_work;
    int ret = 0;

    devdrv_get_dma_queue_sts(io_base, &dma_queue_sts);
    devdrv_get_dma_queue_not_work(io_base, &dma_queue_not_work);
    while ((dma_queue_sts != DEVDRV_DMA_CHANNEL_IDLE_STATE) || (dma_queue_not_work != DEVDRV_DMA_QUEUE_NOT_WORK)) {
        if (timeout-- == 0) {
            ret = -ETIMEDOUT;
            break;
        }

        devdrv_get_dma_queue_sts(io_base, &dma_queue_sts);
        devdrv_get_dma_queue_not_work(io_base, &dma_queue_not_work);
        devdrv_debug("dma_queue_sts %x, dma_queue_not_work %x\n", dma_queue_sts, dma_queue_not_work);
        udelay(1);
    }

    return ret;
}

/* reset the DMA channel configuration */
int devdrv_dma_ch_cfg_reset(void __iomem *io_base)
{
    u32 dma_queue_sts;
    u32 dma_queue_en;
    int ret;

    /* If the DMA Queue is in IDLE_STATE, the channel reset flow
     * can be finished
     */
    devdrv_get_dma_queue_sts(io_base, &dma_queue_sts);
    devdrv_debug("dma_queue_sts %x\n", dma_queue_sts);
    if (dma_queue_sts == DEVDRV_DMA_CHANNEL_IDLE_STATE)
        return 0;

    if (dma_queue_sts == DEVDRV_DMA_CHANNEL_RUN_STATE) {
        /* set dma_queue_pause to 0x1 */
        devdrv_set_dma_queue_pause(io_base, 1);

        /* disable dma_queue_en bits of DEVDRV_DMA_QUEUE_CTRL0 */
        devdrv_set_dma_chan_en(io_base, 0);

        /* check dma channel queue sts */
        ret = devdrv_dma_check_queue_sts(io_base, DEVDRV_DMA_TIMEOUT);
        if (ret) {
            devdrv_err("devdrv_dma_check_queue_sts failed\n");
            return -ETIMEDOUT;
        }
    }

    devdrv_get_dma_chan_en(io_base, &dma_queue_en);
    if (dma_queue_en == 1)
        devdrv_set_dma_chan_en(io_base, 0);

    /* set dma_queue_reset bits of DEVDRV_DMA_QUEUE_CTRL1 */
    devdrv_set_dma_queue_reset(io_base, 1);

    /* set sq tail and cq head */
    devdrv_set_dma_sq_tail(io_base, 0);
    devdrv_set_dma_cq_head(io_base, DEVDRV_DMA_CQ_HEAD(1024));

    /* set dma_queue_pause to 0x0 */
    devdrv_set_dma_queue_pause(io_base, 0);

    /* check the DMA queue fsm sts */
    ret = devdrv_dma_check_fsm_sts(io_base, DEVDRV_DMA_TIMEOUT);
    if (ret) {
        devdrv_err("dma_ch_check_fsm_sts failed\n");
        return -ETIMEDOUT;
    }

    return 0;
}

void devdrv_dma_ch_cfg_init(void __iomem *io_base, u64 sq_addr, u64 cq_addr, u32 sq_depth, u32 cq_depth, u32 pf_num,
                            u32 sqcq_side)
{
    devdrv_set_dma_sq_base(io_base, sq_addr);

    devdrv_set_dma_cq_base(io_base, cq_addr);

    /* set dma channel sq depth */
    devdrv_set_dma_sq_depth(io_base, sq_depth);
    /* set dma channel cq depth */
    devdrv_set_dma_cq_depth(io_base, cq_depth);
    /* set dma channel sq tail */
    devdrv_set_dma_sq_tail(io_base, 0);
    /* set dma channel cq head */
    devdrv_set_dma_cq_head(io_base, DEVDRV_DMA_CQ_HEAD(cq_depth));
    /* set dma queue arb weight */
    devdrv_set_dma_arb_weight(io_base, 0);

    /* set sq and cq side */
    devdrv_set_dma_sqcq_dir(io_base, sqcq_side);

    devdrv_dma_interrupt_mask(io_base, 0);
    /* set PF num */
    devdrv_set_dma_pf_num(io_base, pf_num);
    devdrv_set_dma_err_abort_en(io_base, 0);
}
