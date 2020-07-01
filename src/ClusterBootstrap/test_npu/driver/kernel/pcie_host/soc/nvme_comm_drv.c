/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2012-2019. All rights reserved.
 * Description:
 * Author: huawei
 * Create: 2017/8/18
 */

#include <asm/io.h>

#include "nvme_drv.h"

void devdrv_nvme_reg_wr(void __iomem *io_base, u32 offset, u32 val)
{
    writel(val, io_base + offset);
}

void devdrv_nvme_reg_rd(const void __iomem *io_base, u32 offset, u32 *val)
{
    *val = readl(io_base + offset);
}

void devdrv_set_admin_sq_base(void __iomem *io_base, u64 val, u32 pf_idx)
{
    devdrv_nvme_reg_wr(io_base, DEVDRV_MSG_SQ_BASE_ADDR_H, (u32)(val >> 32));
    devdrv_nvme_reg_wr(io_base, DEVDRV_MSG_SQ_BASE_ADDR_L, (u32)val);
}

void devdrv_get_admin_sq_base(const void __iomem *io_base, u64 *val, u32 pf_idx)
{
    u32 regval;

    devdrv_nvme_reg_rd(io_base, DEVDRV_MSG_SQ_BASE_ADDR_H + (pf_idx * DEVDRV_NVME_CTRL_PF_OFST), &regval);
    *val = (u64)regval << 32;
    devdrv_nvme_reg_rd(io_base, DEVDRV_MSG_SQ_BASE_ADDR_L + (pf_idx * DEVDRV_NVME_CTRL_PF_OFST), &regval);
    *val |= regval;
}

void devdrv_set_sq_doorbell(void __iomem *io_base, u32 val)
{
    devdrv_nvme_reg_wr(io_base, 0, val);
}

void devdrv_set_cq_doorbell(void __iomem *io_base, u32 val)
{
    devdrv_nvme_reg_wr(io_base, DEVDRV_MSG_CHAN_DB_OFFSET, val);
}
