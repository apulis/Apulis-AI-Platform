/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2012-2019. All rights reserved.
 * Description: APB Base Functions
 * Author: Huawei Technologies Co., Ltd.
 * Create: 2018/8/18
 */

#include <linux/delay.h>
#include <linux/errno.h>

#include "apb_drv.h"
#include "devdrv_atu.h"

void devdrv_apb_reg_wr(void __iomem *io_base, u32 offset, u32 val)
{
    writel(val, (io_base + offset));
}

void devdrv_apb_reg_rd(const void __iomem *io_base, u32 offset, u32 *val)
{
    *val = readl(io_base + offset);
}

int devdrv_check_dlcmsm(const void __iomem *io_base)
{
#ifndef CFG_SOC_PLATFORM_CLOUD
    /* timeout 400ms, given by mini PCIe FS */
    int timeout = DEVDRV_DL_DLCMSM_STATE_TIMEOUT;
    u32 dl_dlcmsm_state;
    u32 reg_val;

    devdrv_apb_reg_rd(io_base, DEVDRV_DL_DFX_FSM_STATE, &reg_val);
    dl_dlcmsm_state = reg_val & DEVDRV_DL_DLCMSM_STATE_BIT;
    while (dl_dlcmsm_state != DEVDRV_DL_DLCMSM_STATE_OK) {
        if (timeout == 0) {
            devdrv_err("devdrv_check_dlcmsm timeout\n");
            return -ETIMEDOUT;
        }

        devdrv_apb_reg_rd(io_base, DEVDRV_DL_DFX_FSM_STATE, &reg_val);
        dl_dlcmsm_state = reg_val & DEVDRV_DL_DLCMSM_STATE_BIT;

        udelay(100);
        timeout -= 100;
    }
#endif
    return 0;
}

void devdrv_rx_atu_init(const void __iomem *io_base, u32 pf_num, u32 bar_num, struct devdrv_iob_atu atu[], int num)
{
    int i, atu_id = 0;
    u32 var;
    u32 offset;

    for (i = 0; i < DEVDRV_RX_ATU_NUM; i++) {
        if (atu_id >= num)
            break;

        offset = i * DEVDRV_RX_ATU_REG_SIZE;
        devdrv_apb_reg_rd(io_base, offset + DEVDRV_RX_ATU_CONTROL0_REG, &var);

        /* disable */
        if ((var & RX_ATU_ENABLEE_MASK) == 0)
            continue;

        /* not pf mode */
        if ((var & RX_ATU_FUNC_MODE_MASK) != (RX_ATU_FUNC_MODE_PF << RX_ATU_FUNC_MODE_OFFSET))
            continue;

        devdrv_apb_reg_rd(io_base, offset + DEVDRV_RX_ATU_CONTROL1_REG, &var);

        /* diff pf num */
        if ((var & RX_ATU_PF_NUM_MASK) != (pf_num << RX_ATU_PF_NUM_OFFSET))
            continue;

        /* diff bar num */
        if ((var & RX_ATU_BAR_NUM_MASK) != (bar_num << RX_ATU_BAR_NUM_OFFSET))
            continue;

        /* size */
        devdrv_apb_reg_rd(io_base, offset + DEVDRV_RX_ATU_CONTROL2_REG, &var);
        atu[atu_id].size = (u64)var << 32;
        devdrv_apb_reg_rd(io_base, offset + DEVDRV_RX_ATU_REGION_SIZE_REG, &var);
        atu[atu_id].size |= var;

        /* base addr */
        devdrv_apb_reg_rd(io_base, offset + DEVDRV_RX_ATU_BASE_H_REG, &var);
        atu[atu_id].base_addr = (u64)var << 32;
        devdrv_apb_reg_rd(io_base, offset + DEVDRV_RX_ATU_BASE_L_REG, &var);
        atu[atu_id].base_addr |= var;

        /* target addr */
        devdrv_apb_reg_rd(io_base, offset + DEVDRV_RX_ATU_TAR_H_REG, &var);
        atu[atu_id].target_addr = (u64)var << 32;
        devdrv_apb_reg_rd(io_base, offset + DEVDRV_RX_ATU_TAR_L_REG, &var);
        atu[atu_id].target_addr |= var;

        atu[atu_id].valid = ATU_VALID;
        atu[atu_id].atu_id = atu_id;

        devdrv_info("rx atu index %d, atu id %d, size 0x%llx\r\n", i, atu_id, atu[atu_id].size);

        atu_id++;
    }

    devdrv_info("pf_num %d, bar_num %d, total rx atu num %d.\r\n.", pf_num, bar_num, atu_id);
}

void devdrv_add_tx_atu(void __iomem *io_base, u32 atu_id, u32 pf_num, struct devdrv_iob_atu *atu)
{
    u32 offset;
    u32 val;

    if (atu_id >= DEVDRV_TX_ATU_NUM) {
        devdrv_info("atu_id %d overflow.\r\n.", atu_id);
        return;
    }

    offset = atu_id * DEVDRV_TX_ATU_REG_SIZE;

    val = (pf_num << TX_ATU_PF_NUM_OFFSET) & TX_ATU_PF_NUM_MASK;
    val |= (TX_ATU_EP_NUM_CORE1_PORT0 << TX_ATU_EP_NUM_OFFSET) & TX_ATU_EP_NUM_MASK;
    val |= (TX_ATU_TYPE_TRANS_MODE_MEM << TX_ATU_TYPE_TRANS_MODE_OFFSET) & TX_ATU_TYPE_TRANS_MODE_MASK;
    val |= (TX_ATU_WORK_MODE_EP << TX_ATU_WORK_MODE_OFFSET) & TX_ATU_WORK_MODE_MASK;
    val |= (0x1 << TX_ATU_EN_OFFSET) & TX_ATU_EN_MASK;
    devdrv_apb_reg_wr(io_base, offset + DEVDRV_TX_ATU_CONTROL0_REG, val);

    /* size */
    devdrv_apb_reg_wr(io_base, offset + DEVDRV_TX_ATU_CONTROL2_REG, (u32)(atu->size >> 32));
    devdrv_apb_reg_wr(io_base, offset + DEVDRV_TX_ATU_REGION_SIZE_REG, (u32)(atu->size));

    /* base */
    devdrv_apb_reg_wr(io_base, offset + DEVDRV_TX_ATU_BASE_H_REG, (u32)(atu->base_addr >> 32));
    devdrv_apb_reg_wr(io_base, offset + DEVDRV_TX_ATU_BASE_L_REG, (u32)(atu->base_addr));

    /* target */
    devdrv_apb_reg_wr(io_base, offset + DEVDRV_TX_ATU_TAR_H_REG, (u32)(atu->target_addr >> 32));
    devdrv_apb_reg_wr(io_base, offset + DEVDRV_TX_ATU_TAR_L_REG, (u32)(atu->target_addr));
}

void devdrv_del_tx_atu(void __iomem *io_base, u32 atu_id, u32 pf_num, struct devdrv_iob_atu *atu)
{
    u32 offset;
    u32 val;
    u32 set_pf_num;

    if (atu_id >= DEVDRV_TX_ATU_NUM) {
        devdrv_info("atu_id %d overflow.\n", atu_id);
        return;
    }

    offset = atu_id * DEVDRV_TX_ATU_REG_SIZE;

    devdrv_apb_reg_rd(io_base, offset + DEVDRV_TX_ATU_CONTROL0_REG, &val);

    /* atu in invalid */
    if ((val & TX_ATU_EN_MASK) == 0) {
        devdrv_info("atu_id %d don't need del.\n", atu_id);
        return;
    }

    /* pf not match */
    set_pf_num = (val & TX_ATU_PF_NUM_MASK) >> TX_ATU_PF_NUM_OFFSET;
    if (set_pf_num != pf_num) {
        devdrv_info("atu_id %d set pf %d, del pf %d not match.\n", atu_id, set_pf_num, pf_num);
        return;
    }

    devdrv_apb_reg_wr(io_base, offset + DEVDRV_TX_ATU_CONTROL0_REG, 0);

    atu->base_addr = 0;
    atu->size = 0;
    atu->target_addr = 0;
    /* size */
    devdrv_apb_reg_wr(io_base, offset + DEVDRV_TX_ATU_CONTROL2_REG, 0);
    devdrv_apb_reg_wr(io_base, offset + DEVDRV_TX_ATU_REGION_SIZE_REG, 0);

    /* base */
    devdrv_apb_reg_wr(io_base, offset + DEVDRV_TX_ATU_BASE_H_REG, 0);
    devdrv_apb_reg_wr(io_base, offset + DEVDRV_TX_ATU_BASE_L_REG, 0);

    /* target */
    devdrv_apb_reg_wr(io_base, offset + DEVDRV_TX_ATU_TAR_H_REG, 0);
    devdrv_apb_reg_wr(io_base, offset + DEVDRV_TX_ATU_TAR_L_REG, 0);
}

int devdrv_get_tx_atu(const void __iomem *io_base, u32 atu_id, u32 pf_num, struct devdrv_iob_atu *atu)
{
    u32 offset;
    u32 val;
    u32 set_pf_num;

    if (atu_id >= DEVDRV_TX_ATU_NUM) {
        devdrv_info("atu_id %d overflow.\r\n.", atu_id);
        return -EINVAL;
    }

    offset = atu_id * DEVDRV_TX_ATU_REG_SIZE;

    devdrv_apb_reg_rd(io_base, offset + DEVDRV_TX_ATU_CONTROL0_REG, &val);

    /* atu in invalid */
    if ((val & TX_ATU_EN_MASK) == 0) {
        return -EINVAL;
    }

    /* pf not match */
    set_pf_num = (val & TX_ATU_PF_NUM_MASK) >> TX_ATU_PF_NUM_OFFSET;
    if (set_pf_num != pf_num) {
        devdrv_info("atu_id %d set pf %d, del pf %d not match.\r\n", atu_id, set_pf_num, pf_num);
        return -EINVAL;
    }

    /* size */
    devdrv_apb_reg_rd(io_base, offset + DEVDRV_TX_ATU_CONTROL2_REG, &val);
    atu->size = (u64)val << 32;
    devdrv_apb_reg_rd(io_base, offset + DEVDRV_TX_ATU_REGION_SIZE_REG, &val);
    atu->size |= val;

    /* base */
    devdrv_apb_reg_rd(io_base, offset + DEVDRV_TX_ATU_BASE_H_REG, &val);
    atu->base_addr = (u64)val << 32;
    devdrv_apb_reg_rd(io_base, offset + DEVDRV_TX_ATU_BASE_L_REG, &val);
    atu->base_addr |= val;

    /* target */
    devdrv_apb_reg_rd(io_base, offset + DEVDRV_TX_ATU_TAR_H_REG, &val);
    atu->target_addr = (u64)val << 32;
    devdrv_apb_reg_rd(io_base, offset + DEVDRV_TX_ATU_TAR_L_REG, &val);
    atu->target_addr |= val;

    return 0;
}
