/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2012-2019. All rights reserved.
 * Description:
 * Author: huawei
 * Create: 2017/8/18
 */

#include <asm/io.h>

#include "nvme_drv.h"

#define NVME_INT_REQ_OFFSET 0
#define NVME_INT_VECTOR_OFFSET 2
#define NVME_INT_PF_NUM_OFFSET 24
#define NVME_INT_VF_NUM_OFFSET 16

/*
reg 0:
bit0: Queue0 sqdb int status
bit1: Queue8 sqdb int status
бн
bit15:Queue 120 sqdb int status
bit16~bit31 rsv

reg 1:
bit0: Queue128 sqdb int status
bit1: Queue136 sqdb int status
бн
bit15:Queue 248 sqdb int status

reg 2:
bit0: Queue1 sqdb int status
bit1: Queue9 sqdb int status
бн
bit15:Queue 121 sqdb int status

reg 3:
bit0: Queue129 sqdb int status
bit1: Queue137 sqdb int status
бн
bit15:Queue 249 sqdb int status

reg 15:
bit0: Queue135 sqdb int status
bit1: Queue143 sqdb int status
бн
bit15:Queue 255 sqdb int status
*/
#define REG_SIZE 4

const u32 g_irq_reg_offset[AGENTDRV_NVME_DB_IRQ_NUM] = {
    DEVDRV_IRQ_OFFSET_0 * REG_SIZE, DEVDRV_IRQ_OFFSET_2 * REG_SIZE, DEVDRV_IRQ_OFFSET_4 * REG_SIZE,
    DEVDRV_IRQ_OFFSET_6 * REG_SIZE,  DEVDRV_IRQ_OFFSET_8 * REG_SIZE, DEVDRV_IRQ_OFFSET_10 * REG_SIZE,
    DEVDRV_IRQ_OFFSET_12 * REG_SIZE, DEVDRV_IRQ_OFFSET_14 * REG_SIZE
};

const u32 g_irq0_db[AGENTDRV_QCNT_EACH_IRQ] = {
    DEVDRV_IRQ0_DB_0,  DEVDRV_IRQ0_DB_8,   DEVDRV_IRQ0_DB_16,  DEVDRV_IRQ0_DB_24, DEVDRV_IRQ0_DB_32, DEVDRV_IRQ0_DB_40,
    DEVDRV_IRQ0_DB_48, DEVDRV_IRQ0_DB_56,  DEVDRV_IRQ0_DB_64,  DEVDRV_IRQ0_DB_72, DEVDRV_IRQ0_DB_80, DEVDRV_IRQ0_DB_88,
    DEVDRV_IRQ0_DB_96, DEVDRV_IRQ0_DB_104, DEVDRV_IRQ0_DB_112, DEVDRV_IRQ0_DB_120
};

int devdrv_get_chip_type(void)
{
    return HISI_CLOUD;
}

int devdrv_get_board_type(void)
{
    int board_type;

    /* read from acpi */
    board_type = BOARD_CLOUD_AI_SERVER;

    return board_type;
}

void devdrv_raise_int_to_h(void __iomem *io_base, u32 pf, u32 vf, u32 irq)
{
    u32 regval;

    regval = ((irq << NVME_INT_VECTOR_OFFSET) | (1 << NVME_INT_REQ_OFFSET) | (pf << NVME_INT_PF_NUM_OFFSET) |
              (vf << NVME_INT_VF_NUM_OFFSET));

    devdrv_nvme_reg_wr(io_base, AGENTDRV_NVME_INT_REQ, regval);
}

void devdrv_get_nvme_irq_db(void __iomem *io_base, u32 reg_offset, u32 irq_num, u32 db_id[], u32 *db_num)
{
    u32 i, num, regval;

    devdrv_nvme_reg_rd(io_base, reg_offset, &regval);
    devdrv_nvme_reg_wr(io_base, reg_offset, regval);

    num = 0;
    for (i = 0; i < AGENTDRV_QCNT_EACH_IRQ; i++) {
        if (regval & 0x1)
            db_id[num++] = g_irq0_db[i] + irq_num;
        regval >>= 1;
    }
    *db_num = num;
}

void devdrv_get_nvme_irq_sq_db(void __iomem *io_base, u32 irq_num, u32 db_id[], int len, u32 *db_num)
{
    u32 reg_offset = AGENTDRV_NVME_SQ_DB_STS_BASE + g_irq_reg_offset[irq_num];

    (void)len;
    devdrv_get_nvme_irq_db(io_base, reg_offset, irq_num, db_id, db_num);
}

void devdrv_get_nvme_irq_cq_db(void __iomem *io_base, u32 irq_num, u32 db_id[], int len, u32 *db_num)
{
    u32 reg_offset = AGENTDRV_NVME_CQ_DB_STS_BASE + g_irq_reg_offset[irq_num];

    (void)len;
    devdrv_get_nvme_irq_db(io_base, reg_offset, irq_num, db_id, db_num);
}

void devdrv_set_nvme_irq_mask(void __iomem *io_base, u32 irq_num)
{
    u32 reg_offset;

    /* vector */
    reg_offset = AGENTDRV_NVME_INT_VECTOR_MASK_BASE + (REG_SIZE * irq_num);
    devdrv_nvme_reg_wr(io_base, reg_offset, 0x1);
}

void devdrv_set_nvme_irq_unmask(void __iomem *io_base, u32 irq_num)
{
    u32 reg_offset;

    /* vector */
    reg_offset = AGENTDRV_NVME_INT_VECTOR_MASK_BASE + (REG_SIZE * irq_num);
    devdrv_nvme_reg_wr(io_base, reg_offset, 0);
}

void devdrv_set_nvme_irq_enbale(void __iomem *io_base, u32 irq_num)
{
    u32 reg_offset;

    /* sq */
    reg_offset = AGENTDRV_NVME_SQ_DB_INT_MASK_BASE + g_irq_reg_offset[irq_num];
    devdrv_nvme_reg_wr(io_base, reg_offset, 0);

    /* cq */
    reg_offset = AGENTDRV_NVME_CQ_DB_INT_MASK_BASE + g_irq_reg_offset[irq_num];
    devdrv_nvme_reg_wr(io_base, reg_offset, 0);

    /* vector */
    devdrv_set_nvme_irq_unmask(io_base, irq_num);
}

int devdrv_nvme_agent_irq_num2vector(u32 irq_num, u32 *irq_vector)
{
    if ((irq_num < AGENTDRV_NVME_DB_IRQ_BASE) || (irq_num >= (AGENTDRV_NVME_DB_IRQ_BASE + AGENTDRV_NVME_DB_IRQ_NUM))) {
        return -1;
    } else {
        *irq_vector = irq_num - AGENTDRV_NVME_DB_IRQ_BASE;
        return 0;
    }
}

u32 devdrv_nvme_agent_irq_vector2num(u32 irq_vector)
{
    return irq_vector + AGENTDRV_NVME_DB_IRQ_BASE;
}

void devdrv_soc_func2vfpf(u32 func_id, u32 rdie, u32 *vf_en, u32 *pf_num, u32 *vf_num)
{
    *vf_en = 0;
    *pf_num = DEVDRV_PF_NUM;
    *vf_num = 0;
}
