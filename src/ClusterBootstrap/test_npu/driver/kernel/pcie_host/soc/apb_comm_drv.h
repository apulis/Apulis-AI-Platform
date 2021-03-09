/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2012-2019. All rights reserved.
 * Description:
 * Author: huawei
 * Create: 2017/8/18
 */

#ifndef _APB_COMM_DRV_H_
#define _APB_COMM_DRV_H_

#include <asm/io.h>
#include "devdrv_util.h"
#include "devdrv_atu.h"

#include "hipciec_dl_reg_reg_offset.h"
#include "hipciec_ap_iob_rx_com_reg_reg_offset.h"
#include "hipciec_ap_iob_tx_reg_reg_offset.h"

#define module_devdrv "drv_pcie"
#define devdrv_err(fmt...) drv_err(module_devdrv, fmt)
#define devdrv_info(fmt...) drv_info(module_devdrv, fmt)

#define RX_ATU_FUNC_MODE_OFFSET 1
#define RX_ATU_FUNC_MODE_MASK (0x1 << RX_ATU_FUNC_MODE_OFFSET)
#define RX_ATU_FUNC_MODE_PF 0
#define RX_ATU_ENABLE_OFFSET 0
#define RX_ATU_ENABLEE_MASK (0x1 << RX_ATU_ENABLE_OFFSET)
#define RX_ATU_PF_NUM_OFFSET 3
#define RX_ATU_PF_NUM_MASK (0x7 << RX_ATU_PF_NUM_OFFSET)
#define RX_ATU_BAR_NUM_OFFSET 0
#define RX_ATU_BAR_NUM_MASK (0x7 << RX_ATU_BAR_NUM_OFFSET)

#define TX_ATU_PF_NUM_OFFSET 5
#define TX_ATU_PF_NUM_MASK (0x7 << TX_ATU_PF_NUM_OFFSET)
#define TX_ATU_EP_NUM_OFFSET 4
#define TX_ATU_EP_NUM_MASK (0x1 << TX_ATU_EP_NUM_OFFSET)
#define TX_ATU_EP_NUM_CORE1_PORT0 0
#define TX_ATU_TYPE_TRANS_MODE_OFFSET 2
#define TX_ATU_TYPE_TRANS_MODE_MASK (0x1 << TX_ATU_TYPE_TRANS_MODE_OFFSET)
#define TX_ATU_TYPE_TRANS_MODE_MEM 0
#define TX_ATU_TYPE_TRANS_MODE_IO 1
#define TX_ATU_WORK_MODE_OFFSET 1
#define TX_ATU_WORK_MODE_MASK (0x1 << TX_ATU_WORK_MODE_OFFSET)
#define TX_ATU_WORK_MODE_EP 1
#define TX_ATU_EN_OFFSET 0
#define TX_ATU_EN_MASK (0x1 << TX_ATU_EN_OFFSET)

#define DEVDRV_DL_DFX_FSM_STATE HiPCIECTRL40V200_HIPCIEC_DL_REG_DFX_FSM_STATE_REG

#define DEVDRV_DL_DLCMSM_STATE_OFFSET 0
#define DEVDRV_DL_DLCMSM_STATE_BIT (0x7 << DEVDRV_DL_DLCMSM_STATE_OFFSET)
#define DEVDRV_DL_DLCMSM_STATE_OK 0x4
#define DEVDRV_DL_DLCMSM_STATE_TIMEOUT 400000 /* 400ms, given in mini FS */

#define DEVDRV_PCIE_RC_MODE 0
#define DEVDRV_PCIE_EP_MODE 1

#define DEVDRV_RX_ATU_CONTROL0_REG HiPCIECTRL40V200_HIPCIEC_AP_IOB_RX_COM_REG_IOB_RXATU_CONTROL_0_0_REG
#define DEVDRV_RX_ATU_CONTROL1_REG HiPCIECTRL40V200_HIPCIEC_AP_IOB_RX_COM_REG_IOB_RXATU_CONTROL_1_0_REG
#define DEVDRV_RX_ATU_CONTROL2_REG HiPCIECTRL40V200_HIPCIEC_AP_IOB_RX_COM_REG_IOB_RXATU_CONTROL_2_0_REG
#define DEVDRV_RX_ATU_REGION_SIZE_REG HiPCIECTRL40V200_HIPCIEC_AP_IOB_RX_COM_REG_IOB_RXATU_REGION_SIZE_0_REG
#define DEVDRV_RX_ATU_BASE_L_REG HiPCIECTRL40V200_HIPCIEC_AP_IOB_RX_COM_REG_IOB_RXATU_BASE_ADDR_L_0_REG
#define DEVDRV_RX_ATU_BASE_H_REG HiPCIECTRL40V200_HIPCIEC_AP_IOB_RX_COM_REG_IOB_RXATU_BASE_ADDR_H_0_REG
#define DEVDRV_RX_ATU_TAR_L_REG HiPCIECTRL40V200_HIPCIEC_AP_IOB_RX_COM_REG_IOB_RXATU_TAR_ADDR_L_0_REG
#define DEVDRV_RX_ATU_TAR_H_REG HiPCIECTRL40V200_HIPCIEC_AP_IOB_RX_COM_REG_IOB_RXATU_TAR_ADDR_H_0_REG
#define DEVDRV_RX_ATU_REG_SIZE 0x20

#define DEVDRV_TX_ATU_CONTROL0_REG HiPCIECTRL40V200_HIPCIEC_AP_IOB_TX_REG_IOB_TXATU_CONTROL_0_0_REG
#define DEVDRV_TX_ATU_CONTROL2_REG HiPCIECTRL40V200_HIPCIEC_AP_IOB_TX_REG_IOB_TXATU_CONTROL_2_0_REG
#define DEVDRV_TX_ATU_REGION_SIZE_REG HiPCIECTRL40V200_HIPCIEC_AP_IOB_TX_REG_IOB_TXATU_REGION_SIZE_0_REG
#define DEVDRV_TX_ATU_BASE_L_REG HiPCIECTRL40V200_HIPCIEC_AP_IOB_TX_REG_IOB_TXATU_BASE_ADDR_L_0_REG
#define DEVDRV_TX_ATU_BASE_H_REG HiPCIECTRL40V200_HIPCIEC_AP_IOB_TX_REG_IOB_TXATU_BASE_ADDR_H_0_REG
#define DEVDRV_TX_ATU_TAR_L_REG HiPCIECTRL40V200_HIPCIEC_AP_IOB_TX_REG_IOB_TXATU_TAR_ADDR_L_0_REG
#define DEVDRV_TX_ATU_TAR_H_REG HiPCIECTRL40V200_HIPCIEC_AP_IOB_TX_REG_IOB_TXATU_TAR_ADDR_H_0_REG
#define DEVDRV_TX_ATU_REG_SIZE 0x20

int devdrv_check_dlcmsm(const void __iomem *io_base);
int devdrv_get_devid_by_bus(unsigned char bus_number, int *devid);
int agentdrv_get_rc_ep_mode(u32 *mode);
u32 agentdrv_soc_get_func_total(void);

void devdrv_apb_reg_wr(void __iomem *io_base, u32 offset, u32 val);
void devdrv_apb_reg_rd(const void __iomem *io_base, u32 offset, u32 *val);

void devdrv_rx_atu_init(const void __iomem *io_base, u32 pf_num, u32 bar_num, struct devdrv_iob_atu atu[], int num);
void devdrv_add_tx_atu(void __iomem *io_base, u32 atu_id, u32 pf_num, struct devdrv_iob_atu *atu);
void devdrv_del_tx_atu(void __iomem *io_base, u32 atu_id, u32 pf_num, struct devdrv_iob_atu *atu);
int devdrv_get_tx_atu(const void __iomem *io_base, u32 atu_id, u32 pf_num, struct devdrv_iob_atu *atu);

#endif
