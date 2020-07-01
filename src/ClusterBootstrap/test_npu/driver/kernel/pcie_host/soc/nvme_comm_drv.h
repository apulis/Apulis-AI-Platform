/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2012-2019. All rights reserved.
 * Description:
 * Author: huawei
 * Create: 2017/8/18
 */

#ifndef _NVME_COMM_DRV_H_
#define _NVME_COMM_DRV_H_

#include <asm/io.h>

#define HISI_MINI   0
#define HISI_CLOUD  1
#define HISI_MINIV2 2

#define BOARD_MINI_PCIE_CARD 0x0
#define BOARD_MINI_PCIE_CARD_BOARDID 1
#define BOARD_MINI_MDC 0x1
#define BOARD_MINI_MDC_BOARDID 2
#define BOARD_MINI_EVB 0x2
#define BOARD_MINI_EVB_BOARDID_900 900
#define BOARD_MINI_EVB_BOARDID_901 901
#define BOARD_MINI_EVB_BOARDID_902 902
#define BOARD_MINI_OTHERS 0x3
#define BOARD_CLOUD_PCIE_CARD 0x100
#define BOARD_CLOUD_AI_SERVER 0x101
#define BOARD_CLOUD_EVB 0x102
#define BOARD_CLOUD_OTHERS 0x103

#define BOARD_CLOUD_EVB_DEV_NUM 2
#define BOARD_CLOUD_AI_SERVER_DEV_NUM 4
#define BOARD_CLOUD_MAX_DEV_NUM 8
#define BOARD_MINIV2_DEV_NUM 1

#define NETWORK_PF_0 0
#define NETWORK_PF_1 1

#define PCIE_PF_0 0
#define PCIE_PF_1 1

#define DEVDRV_NVME_CTRL_PF_OFST 0x2000

/* base addr of queue depth */
#define DEVDRV_MSG_CHAN_QUEUE_BASE_ADDR HiPCIECTRL40V200_HIPCIEC_NVME_PF_LOCAL_CTRL_REG_AQA_REG
/* SQ base address low 32 bits */
#define DEVDRV_MSG_SQ_BASE_ADDR_L HiPCIECTRL40V200_HIPCIEC_NVME_PF_LOCAL_CTRL_REG_ASQB_LOW_REG
#define DEVDRV_MSG_SQ_BASE_ADDR_H HiPCIECTRL40V200_HIPCIEC_NVME_PF_LOCAL_CTRL_REG_ASQB_HIGH_REG
#define DEVDRV_MSG_CQ_BASE_ADDR_L HiPCIECTRL40V200_HIPCIEC_NVME_PF_LOCAL_CTRL_REG_ACQB_LOW_REG
#define DEVDRV_MSG_CQ_BASE_ADDR_H HiPCIECTRL40V200_HIPCIEC_NVME_PF_LOCAL_CTRL_REG_ACQB_HIGH_REG

void devdrv_nvme_reg_wr(void __iomem *io_base, u32 offset, u32 val);
void devdrv_nvme_reg_rd(const void __iomem *io_base, u32 offset, u32 *val);

int devdrv_get_chip_type(void);
#ifdef CFG_SOC_PLATFORM_CLOUD
int devdrv_get_board_type(void);
#endif
int devdrv_get_iep_nvme_device_id(void);

void devdrv_get_admin_sq_base(const void __iomem *io_base, u64 *val, u32 pf_idx);
void devdrv_set_admin_sq_base(void __iomem *io_base, u64 val, u32 pf_idx);
void devdrv_raise_int_to_h(void __iomem *io_base, u32 pf, u32 vf, u32 irq);

void devdrv_set_sq_doorbell(void __iomem *io_base, u32 val);
void devdrv_set_cq_doorbell(void __iomem *io_base, u32 val);

void devdrv_get_nvme_irq_sq_db(void __iomem *io_base, u32 irq_num, u32 db_id[], int len, u32 *db_num);
void devdrv_get_nvme_irq_cq_db(void __iomem *io_base, u32 irq_num, u32 db_id[], int len, u32 *db_num);
void devdrv_set_nvme_irq_mask(void __iomem *io_base, u32 irq_num);
void devdrv_set_nvme_irq_unmask(void __iomem *io_base, u32 irq_num);
void devdrv_set_nvme_irq_enbale(void __iomem *io_base, u32 irq_num);

int devdrv_nvme_agent_irq_num2vector(u32 irq_num, u32 *irq_vector);
u32 devdrv_nvme_agent_irq_vector2num(u32 irq_vector);
void devdrv_soc_func2vfpf(u32 func_id, u32 rdie, u32 *vf_en, u32 *pf_num, u32 *vf_num);

#endif
