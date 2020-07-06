/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2012-2019. All rights reserved.
 * Description: This program is free software; you can redistribute it and /or modify it
 *                     under the terms of the GNU General Public License as published by the Free
 *                     Software Foundation; either version 2 of the License, or (at your option)
 *                     any later version
 * Author: huawei
 * Create:2019
 */

#include <linux/delay.h>
#include <asm/io.h>

#include "dma_drv.h"
#include "resource_drv.h"
int devdrv_get_dma_err_chan(const void __iomem *io_base, u32 *chan_id)
{
    u32 val;
    u32 i;
    for (i = 0; i < DMA_CHAN_TS_USED_START_INDEX; i++) {
        devdrv_dma_reg_rd(io_base + (u64)i * DEVDRV_DMA_CHAN_OFFSET, DEVDRV_DMA_QUEUE_INT_RO, &val);
        if (val & DMA_ERR_MASK) {
            devdrv_info("err dma chan %d err 0x%x.\n", i, val);
            *chan_id = i;
            return 0;
        }
    }

    devdrv_info("not find err dma chan in err irq.\n");

    return -EINVAL;
}

void devdrv_dma_config_axim_aruser_mode(void __iomem *io_base)
{
    u32 regval = 0;

    /* SDI_AXIM_AWUSER_MODE_CTRL.awuser_stash_mode (11 bit) set 0x1 */
    devdrv_dma_reg_rd(io_base, DEVDRV_SDI_AXIM_AWUSER_MODE_CTRL, &regval);
    regval = regval | DMA_CONFIG_REGVAL_OFFSET;
    devdrv_dma_reg_wr(io_base, DEVDRV_SDI_AXIM_AWUSER_MODE_CTRL, regval);

    /* SDI_AXIM_AWUSER_SET_1.awuser_stash_set (1~10 bit) set 0x0 */
    devdrv_dma_reg_rd(io_base, DEVDRV_SDI_AXIM_AWUSER_SET_1, &regval);
    regval = regval & (~0x7FF);
    devdrv_dma_reg_wr(io_base, DEVDRV_SDI_AXIM_AWUSER_SET_1, regval);
}

void devdrv_dma_check_sram_init_status(const void __iomem *io_base, unsigned long timeout)
{
    u32 regval = 0;

    devdrv_dma_reg_rd(io_base, DEVDRV_DMA_COMMON_AND_CH_ERR_STS, &regval);
    while ((regval & 0x10000) != 0x10000) {
        if (timeout-- == 0) {
            devdrv_err("devdrv_dma_check_sram_init_status check fail\n");
            break;
        }

        devdrv_dma_reg_rd(io_base, DEVDRV_DMA_COMMON_AND_CH_ERR_STS, &regval);
        devdrv_debug("regval 0x%x\n", regval);
        udelay(1);
    }
}

void devdrv_dma_set_sq_addr_info(struct devdrv_dma_sq_node *sq_desc, u64 src_addr, u64 dst_addr, u32 length)
{
    sq_desc->src_addr_l = (u32)src_addr;
    sq_desc->src_addr_h = (u32)(src_addr >> DEVDRV_ADDR_MOVE);

    sq_desc->dst_addr_l = (u32)dst_addr;
    sq_desc->dst_addr_h = (u32)(dst_addr >> DEVDRV_ADDR_MOVE);

    sq_desc->length = length;
}

void devdrv_dma_set_sq_attr(struct devdrv_dma_sq_node *sq_desc, u32 opcode, u32 attr, u32 pf, u32 wd_barrier,
                            u32 rd_barrier)
{
    sq_desc->opcode = opcode;
    sq_desc->attr = attr;
    sq_desc->pf = pf;
    sq_desc->wd_barrier = wd_barrier;
    sq_desc->rd_barrier = rd_barrier;
    devdrv_debug("opcode %x, attr %x, pf %x, wb_barrier %x, rd_barrier %x\n", opcode, attr, pf, wd_barrier, rd_barrier);
}

void devdrv_dma_set_sq_irq(struct devdrv_dma_sq_node *sq_desc, u32 rdie, u32 ldie, u32 msi)
{
    sq_desc->rdie = rdie;
    sq_desc->ldie = ldie;
    sq_desc->msi_l = msi & DMA_MSI_L_MASK;
    sq_desc->msi_h = (msi >> DMA_MSI_H_BIT_OFFSET) & DMA_MSI_H_MASK;
    devdrv_debug("rdie %x, ldie %x, msi %x\n", rdie, ldie, msi);
}

bool devdrv_dma_get_cq_valid(struct devdrv_dma_cq_node *cq_desc, u32 rounds)
{
    /* In order to avoid the software clearing the valid flag,
    the hardware will change from 1 to 0 after the cq is used up,
    and so on. 1 is valid for the first time */
    if (rounds & 0x1)
        return (cq_desc->vld == 0);
    else
        return (cq_desc->vld == 1);
}

void devdrv_dma_set_cq_invalid(struct devdrv_dma_cq_node *cq_desc)
{
    /* no need to set */;
    (void)cq_desc;
}

u32 devdrv_dma_get_cq_sqhd(struct devdrv_dma_cq_node *cq_desc)
{
    return (u32)cq_desc->sqhd;
}

u32 devdrv_dma_get_cq_status(struct devdrv_dma_cq_node *cq_desc)
{
    return (u32)cq_desc->status;
}

void devdrv_dma_err_interrupt_unmask(void __iomem *io_base)
{
    devdrv_dma_reg_wr(io_base, DEVDRV_DMA_ERR_MASK, 0x0);
    return;
}