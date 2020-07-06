// ****************************************************************************** 
// Copyright     :  Copyright (C) 2018, Hisilicon Technologies Co. Ltd.
// File name     :  misc_up_cfg_reg_offset.h
// Project line  :  ICT
// Department    :  ICT Processor Chipset Development Dep
// Author        :huawei
// Version       :  1.0
// Date          :  2018/7/2
// Description   :  The description of CLOUD project
// Others        :  Generated automatically by nManager V4.2 
// History       :huawei 2018/11/09 16:35:42 Create file
// ******************************************************************************

#ifndef __MISC_UP_CFG_REG_OFFSET_H__
#define __MISC_UP_CFG_REG_OFFSET_H__

/* MISC_UP_CFG Base address of Module's Register */
#define SOC_MISC_UP_CFG_BASE                       (0x160000)

/******************************************************************************/
/*                      SOC MISC_UP_CFG Registers' Definitions                            */
/******************************************************************************/

#define SOC_MISC_UP_CFG_SC_PLL7_SEL_REG            (SOC_MISC_UP_CFG_BASE + 0x8)    /* PLL7选择 */
#define SOC_MISC_UP_CFG_SC_PLL7_FCTRL_REG          (SOC_MISC_UP_CFG_BASE + 0x10)   /* SC_PLL7_FCTRL为CDIE_CRG的PLL7整数分频 */
#define SOC_MISC_UP_CFG_SC_PLL7_FCTRL_FRAC_REG     (SOC_MISC_UP_CFG_BASE + 0x14)   /* SC_PLL7_FCTRL_FRAC为CDIE_CRG的PLL7小数分频 */
#define SOC_MISC_UP_CFG_SC_PLL7_HI_FNPLL_CFG0_REG  (SOC_MISC_UP_CFG_BASE + 0x20)   /* PLL输入配置寄存器;静态信号 */
#define SOC_MISC_UP_CFG_SC_PLL7_HI_FNPLL_CFG1_REG  (SOC_MISC_UP_CFG_BASE + 0x24)   /* PLL输入配置寄存器;静态信号 */
#define SOC_MISC_UP_CFG_SC_PLL7_HI_FNPLL_CFG2_REG  (SOC_MISC_UP_CFG_BASE + 0x28)   /* PLL输入配置寄存器;静态信号 */
#define SOC_MISC_UP_CFG_SC_PLL7_HI_FNPLL_CFG3_REG  (SOC_MISC_UP_CFG_BASE + 0x2C)   /* PLL输入配置寄存器;静态信号 */
#define SOC_MISC_UP_CFG_SC_PLL7_HI_FNPLL_CFG4_REG  (SOC_MISC_UP_CFG_BASE + 0x30)   /* PLL输入配置寄存器;静态信号 */
#define SOC_MISC_UP_CFG_SC_PLL7_HI_FNPLL_CFG5_REG  (SOC_MISC_UP_CFG_BASE + 0x34)   /* PLL输入配置寄存器;静态信号 */
#define SOC_MISC_UP_CFG_SC_PLL7_HI_FNPLL_CFG6_REG  (SOC_MISC_UP_CFG_BASE + 0x38)   /* PLL输入配置寄存器;静态信号 */
#define SOC_MISC_UP_CFG_SC_PLL7_HI_FNPLL_CFG7_REG  (SOC_MISC_UP_CFG_BASE + 0x3C)   /* PLL输入配置寄存器;静态信号 */
#define SOC_MISC_UP_CFG_SC_PLL7_HI_FNPLL_CFG8_REG  (SOC_MISC_UP_CFG_BASE + 0x40)   /* PLL输入配置寄存器;静态信号 */
#define SOC_MISC_UP_CFG_SC_PLL7_HI_FNPLL_CFG9_REG  (SOC_MISC_UP_CFG_BASE + 0x44)   /* PLL输入配置寄存器;静态信号 */
#define SOC_MISC_UP_CFG_SC_PLL7_HI_FNPLL_CFG10_REG (SOC_MISC_UP_CFG_BASE + 0x48)   /* PLL输入配置寄存器;静态信号 */
#define SOC_MISC_UP_CFG_SC_PLL7_HI_FCTRL_REG       (SOC_MISC_UP_CFG_BASE + 0x80)   /* SC_PLL7_HI_FCTRL为CDIE_CRG的PLL7_HI整数分频 */
#define SOC_MISC_UP_CFG_SC_PLL7_HI_FCTRL_FRAC_REG  (SOC_MISC_UP_CFG_BASE + 0x84)   /* SC_PLL7_HI_FCTRL_FRAC为CDIE_CRG的PLL7_HI小数分频 */
#define SOC_MISC_UP_CFG_SC_PLL_CLK_BYPASS0_REG     (SOC_MISC_UP_CFG_BASE + 0xA0)   /* SC_PLL_CLK_BYPASS0为CDIE_CRG的PLL相关工作时钟选择。 */
#define SOC_MISC_UP_CFG_SC_PLL_CLK_BYPASS1_REG     (SOC_MISC_UP_CFG_BASE + 0xA4)   /* SC_PLL_CLK_BYPASS1为CDIE_CRG的PLL相关工作时钟选择。 */
#define SOC_MISC_UP_CFG_SC_HW_AIC_RESET_REQ_REG    (SOC_MISC_UP_CFG_BASE + 0xA00)  /* AIC软复位请求控制寄存器 */
#define SOC_MISC_UP_CFG_SC_HW_AIC_RESET_DREQ_REG   (SOC_MISC_UP_CFG_BASE + 0xA04)  /* AIC软复位去请求控制寄存器 */
#define SOC_MISC_UP_CFG_SC_PLL_SRC_INT_REG         (SOC_MISC_UP_CFG_BASE + 0x4000) /* PLL中断源寄存器 */
#define SOC_MISC_UP_CFG_SC_PLL_INT_MASK_REG        (SOC_MISC_UP_CFG_BASE + 0x4004) /* PLL中断屏蔽寄存器 */
#define SOC_MISC_UP_CFG_SC_PLL_LOCK_STATUS_REG     (SOC_MISC_UP_CFG_BASE + 0x5000) /* PLL锁定状态寄存器。 */
#define SOC_MISC_UP_CFG_PLL7_HI_FNPLL_STATE0_REG   (SOC_MISC_UP_CFG_BASE + 0x5004) /* PLL输出状态寄存器;静态信号 */
#define SOC_MISC_UP_CFG_PLL7_HI_FNPLL_STATE1_REG   (SOC_MISC_UP_CFG_BASE + 0x5008) /* PLL输出状态寄存器;静态信号 */
#define SOC_MISC_UP_CFG_SC_HW_AIC_RESET_ST_REG     (SOC_MISC_UP_CFG_BASE + 0x5A00) /* HW_AIC的当前软复位状态 */
#define SOC_MISC_UP_CFG_SC_TS_EN_REG               (SOC_MISC_UP_CFG_BASE + 0x6000) /* 时间戳使能信号 */
#define SOC_MISC_UP_CFG_SC_PLL_INT_STATUS_REG      (SOC_MISC_UP_CFG_BASE + 0x8000) /* PLL中断状态寄存器 */
#define SOC_MISC_UP_CFG_SC_SYSCTRL_LOCK_REG        (SOC_MISC_UP_CFG_BASE + 0xF100) /* 系统控制器锁定寄存器。（注意：1，此寄存器空间必须是安全访问才能成功） */
#define SOC_MISC_UP_CFG_SC_SYSCTRL_UNLOCK_REG      (SOC_MISC_UP_CFG_BASE + 0xF110) /* 系统控制器解锁寄存器。（注意：1，此寄存器空间必须是安全访问才能成功） */
#define SOC_MISC_UP_CFG_SC_ECO_RSV0_REG            (SOC_MISC_UP_CFG_BASE + 0xFF00) /* ECO 寄存器 */
#define SOC_MISC_UP_CFG_SC_ECO_RSV1_REG            (SOC_MISC_UP_CFG_BASE + 0xFF04) /* ECO 寄存器1 */
#define SOC_MISC_UP_CFG_SC_ECO_RSV2_REG            (SOC_MISC_UP_CFG_BASE + 0xFF08) /* ECO 寄存器2 */
#define SOC_MISC_UP_CFG_SC_ECO_RSV3_REG            (SOC_MISC_UP_CFG_BASE + 0xFF0C) /* ECO 寄存器3 */
#define SOC_MISC_UP_CFG_SC_ECO_RSV4_REG            (SOC_MISC_UP_CFG_BASE + 0xFF10) /* ECO 寄存器4 */
#define SOC_MISC_UP_CFG_SC_ECO_RSV5_REG            (SOC_MISC_UP_CFG_BASE + 0xFF14) /* ECO 寄存器5 */
#define SOC_MISC_UP_CFG_FPGA_VER_REG               (SOC_MISC_UP_CFG_BASE + 0xFFFC) /* FPGA版本寄存器 */

#endif // __MISC_UP_CFG_REG_OFFSET_H__
