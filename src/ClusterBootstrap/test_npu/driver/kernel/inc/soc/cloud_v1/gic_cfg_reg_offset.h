// ****************************************************************************** 
// Copyright     :  Copyright (C) 2018, Hisilicon Technologies Co. Ltd.
// File name     :  gic_cfg_reg_offset.h
// Project line  :  ICT
// Department    :  ICT Processor Chipset Development Dep
// Author        :  xxx
// Version       :  1.0
// Date          :  2018/6/9
// Description   :  The description of AI
// Others        :  Generated automatically by nManager V4.2 
// History       :  xxx 2018/07/13 14:46:58 Create file
// ******************************************************************************

#ifndef __GIC_CFG_REG_OFFSET_H__
#define __GIC_CFG_REG_OFFSET_H__

/* GIC_CFG Base address of Module's Register */
#define CLOUD_GIC_CFG_BASE                       (0x60000000)

/******************************************************************************/
/*                      CLOUD GIC_CFG Registers' Definitions                            */
/******************************************************************************/

#define CLOUD_GIC_CFG_SC_ITS_CLK_EN_REG          (CLOUD_GIC_CFG_BASE + 0x300)  /* ITS时钟使能寄存器 */
#define CLOUD_GIC_CFG_SC_ITS_CLK_DIS_REG         (CLOUD_GIC_CFG_BASE + 0x304)  /* ITS时钟禁止寄存器 */
#define CLOUD_GIC_CFG_SC_FTE_CLK_EN_REG          (CLOUD_GIC_CFG_BASE + 0x308)  /* FTE时钟使能寄存器 */
#define CLOUD_GIC_CFG_SC_FTE_CLK_DIS_REG         (CLOUD_GIC_CFG_BASE + 0x30C)  /* FTE时钟禁止寄存器 */
#define CLOUD_GIC_CFG_SC_SMMU_ICG_EN_REG         (CLOUD_GIC_CFG_BASE + 0x380)  /* SMMU时钟使能寄存器 */
#define CLOUD_GIC_CFG_SC_SMMU_ICG_DIS_REG        (CLOUD_GIC_CFG_BASE + 0x384)  /* SMMU时钟禁止寄存器 */
#define CLOUD_GIC_CFG_SC_KERNEL_SLOW_CLK_EN_REG  (CLOUD_GIC_CFG_BASE + 0x400)  /* KERNEL_SLOW时钟使能寄存器 */
#define CLOUD_GIC_CFG_SC_KERNEL_SLOW_CLK_DIS_REG (CLOUD_GIC_CFG_BASE + 0x404)  /* KERNEL_SLOW时钟禁止寄存器 */
#define CLOUD_GIC_CFG_SC_GIC_CLK_EN_REG          (CLOUD_GIC_CFG_BASE + 0x530)  /* GIC时钟使能寄存器 */
#define CLOUD_GIC_CFG_SC_GIC_CLK_DIS_REG         (CLOUD_GIC_CFG_BASE + 0x534)  /* GIC时钟禁止寄存器 */
#define CLOUD_GIC_CFG_SC_MBIGEN_CLK_EN_REG       (CLOUD_GIC_CFG_BASE + 0x540)  /* MBIGEN时钟使能寄存器 */
#define CLOUD_GIC_CFG_SC_MBIGEN_CLK_DIS_REG      (CLOUD_GIC_CFG_BASE + 0x544)  /* MBIGEN时钟禁止寄存器 */
#define CLOUD_GIC_CFG_SC_ITS_RESET_REQ_REG       (CLOUD_GIC_CFG_BASE + 0xA00)  /* ITS软复位请求控制寄存器 */
#define CLOUD_GIC_CFG_SC_ITS_RESET_DREQ_REG      (CLOUD_GIC_CFG_BASE + 0xA04)  /* ITS软复位去请求控制寄存器 */
#define CLOUD_GIC_CFG_SC_FTE_RESET_REQ_REG       (CLOUD_GIC_CFG_BASE + 0xA08)  /* FTE软复位请求控制寄存器 */
#define CLOUD_GIC_CFG_SC_FTE_RESET_DREQ_REG      (CLOUD_GIC_CFG_BASE + 0xA0C)  /* FTE软复位去请求控制寄存器 */
#define CLOUD_GIC_CFG_SC_GIC_RESET_REQ_REG       (CLOUD_GIC_CFG_BASE + 0xC08)  /* GIC软复位请求控制寄存器 */
#define CLOUD_GIC_CFG_SC_GIC_RESET_DREQ_REG      (CLOUD_GIC_CFG_BASE + 0xC0C)  /* GIC软复位去请求控制寄存器 */
#define CLOUD_GIC_CFG_SC_MBIGEN_RESET_REQ_REG    (CLOUD_GIC_CFG_BASE + 0xC40)  /* MBIGEN软复位请求控制寄存器 */
#define CLOUD_GIC_CFG_SC_MBIGEN_RESET_DREQ_REG   (CLOUD_GIC_CFG_BASE + 0xC44)  /* MBIGEN软复位去请求控制寄存器 */
#define CLOUD_GIC_CFG_SC_EXTINT_CTRL_REG         (CLOUD_GIC_CFG_BASE + 0x2000) /* 外部中断极性配置寄存器 */
#define CLOUD_GIC_CFG_SC_DISPATCH_ERRRSP_REG     (CLOUD_GIC_CFG_BASE + 0x2064) /* dispatch的ERR响应类型控制寄存器 */
#define CLOUD_GIC_CFG_SC_NOCMT_IRQ_REG           (CLOUD_GIC_CFG_BASE + 0x2100) /* 泰山无指令超时中断寄存器 */
#define CLOUD_GIC_CFG_SC_NOCMT_IRQ_INT_MASK_REG  (CLOUD_GIC_CFG_BASE + 0x2104) /* 泰山无指令超时中断屏蔽寄存器 */
#define CLOUD_GIC_CFG_SC_NOCMT_IRQ_STATUS_REG    (CLOUD_GIC_CFG_BASE + 0x2108) /* 泰山无指令超时中断状态寄存器 */
#define CLOUD_GIC_CFG_SC_NOCMT_IRQ_RST_EN_REG    (CLOUD_GIC_CFG_BASE + 0x2120) /* 泰山无指令中断输出复位使能寄存器 */
#define CLOUD_GIC_CFG_SC_NOCMT_IRQ_SEL_REG       (CLOUD_GIC_CFG_BASE + 0x2130) /* 泰山无指令中断输出选择寄存器 */
#define CLOUD_GIC_CFG_SC_RAS_FHI_INT_MASK0_REG   (CLOUD_GIC_CFG_BASE + 0x2200) /* RAS FHI中断屏蔽寄存器 */
#define CLOUD_GIC_CFG_SC_RAS_FHI_INT_MASK1_REG   (CLOUD_GIC_CFG_BASE + 0x2204) /* RAS FHI中断屏蔽寄存器 */
#define CLOUD_GIC_CFG_SC_RAS_ERI_INT_MASK0_REG   (CLOUD_GIC_CFG_BASE + 0x2208) /* RAS ERI中断屏蔽寄存器 */
#define CLOUD_GIC_CFG_SC_RAS_ERI_INT_MASK1_REG   (CLOUD_GIC_CFG_BASE + 0x220C) /* RAS ERI中断屏蔽寄存器 */
#define CLOUD_GIC_CFG_SC_RAS_CE_INT_MASK0_REG    (CLOUD_GIC_CFG_BASE + 0x2210) /* RAS CE中断屏蔽寄存器 */
#define CLOUD_GIC_CFG_SC_RAS_CE_INT_MASK1_REG    (CLOUD_GIC_CFG_BASE + 0x2214) /* RAS CE中断屏蔽寄存器 */
#define CLOUD_GIC_CFG_SC_RAS_FE_INT_MASK0_REG    (CLOUD_GIC_CFG_BASE + 0x2218) /* RAS FE中断屏蔽寄存器 */
#define CLOUD_GIC_CFG_SC_RAS_FE_INT_MASK1_REG    (CLOUD_GIC_CFG_BASE + 0x221C) /* RAS FE中断屏蔽寄存器 */
#define CLOUD_GIC_CFG_SC_RAS_NFE_INT_MASK0_REG   (CLOUD_GIC_CFG_BASE + 0x2220) /* RAS NFE中断屏蔽寄存器 */
#define CLOUD_GIC_CFG_SC_RAS_NFE_INT_MASK1_REG   (CLOUD_GIC_CFG_BASE + 0x2224) /* RAS NFE中断屏蔽寄存器 */
#define CLOUD_GIC_CFG_SC_MEM_CTRL_SMMU_HISI_REG  (CLOUD_GIC_CFG_BASE + 0x3000) /* smmu mem控制寄存器 */
#define CLOUD_GIC_CFG_SC_MEM_CTRL_SMMU_REG       (CLOUD_GIC_CFG_BASE + 0x3004) /* smmu mem控制寄存器 */
#define CLOUD_GIC_CFG_SC_MEM_CTRL_GIC_HISI_REG   (CLOUD_GIC_CFG_BASE + 0x3020) /* gic mem控制寄存器 */
#define CLOUD_GIC_CFG_SC_MEM_CTRL_GIC_REG        (CLOUD_GIC_CFG_BASE + 0x3024) /* gic mem控制寄存器 */
#define CLOUD_GIC_CFG_SC_BUS_NUM_GIC_REG         (CLOUD_GIC_CFG_BASE + 0x3400) /* GIC SMMU BUSNUM配置寄存器 */
#define CLOUD_GIC_CFG_SC_DVE_NUM_GIC_REG         (CLOUD_GIC_CFG_BASE + 0x3404) /* GIC SMMU DVENUM配置寄存器 */
#define CLOUD_GIC_CFG_SC_GICD_INT_MAP_MODE_REG   (CLOUD_GIC_CFG_BASE + 0x4090) /* GICD SPI中断映射方式选择 */
#define CLOUD_GIC_CFG_SC_SPI_INT_EN_REG          (CLOUD_GIC_CFG_BASE + 0x4094) /* SPI中断上报使能信号 */
#define CLOUD_GIC_CFG_SC_TIMER_INT_EN_REG        (CLOUD_GIC_CFG_BASE + 0x4098) /* TIMER 中断上报使能信号 */
#define CLOUD_GIC_CFG_SC_N2N_INT_SEL0_REG        (CLOUD_GIC_CFG_BASE + 0x40A0) /* N-N型中断源选择寄存器0，选择int_sysctrl_nnspi0 */
#define CLOUD_GIC_CFG_SC_N2N_INT_SEL1_REG        (CLOUD_GIC_CFG_BASE + 0x40A4) /* N-N型中断源选择寄存1，选择int_sysctrl_nnspi1 */
#define CLOUD_GIC_CFG_SC_N2N_INT_SEL2_REG        (CLOUD_GIC_CFG_BASE + 0x40A8) /* N-N型中断源选择寄存器2，选择int_sysctrl_nnspi2 */
#define CLOUD_GIC_CFG_SC_N2N_INT_SEL3_REG        (CLOUD_GIC_CFG_BASE + 0x40AC) /* N-N型中断源选择寄存器3，选择int_sysctrl_nnspi3 */
#define CLOUD_GIC_CFG_SC_N2N_INT_SEL4_REG        (CLOUD_GIC_CFG_BASE + 0x40B0) /* N-N型中断源选择寄存器4，选择int_sysctrl_nnspi4 */
#define CLOUD_GIC_CFG_SC_N2N_INT_SEL5_REG        (CLOUD_GIC_CFG_BASE + 0x40B4) /* N-N型中断源选择寄存器5，选择int_sysctrl_nnspi5 */
#define CLOUD_GIC_CFG_SC_N2N_INT_SEL6_REG        (CLOUD_GIC_CFG_BASE + 0x40B8) /* N-N型中断源选择寄存器6，选择int_sysctrl_nnspi6 */
#define CLOUD_GIC_CFG_SC_N2N_INT_SEL7_REG        (CLOUD_GIC_CFG_BASE + 0x40BC) /* N-N型中断源选择寄存器7，选择int_sysctrl_nnspi7 */
#define CLOUD_GIC_CFG_SC_N2N_INT_SEL8_REG        (CLOUD_GIC_CFG_BASE + 0x40C0) /* N-N型中断源选择寄存器8，选择int_sysctrl_nnspi8 */
#define CLOUD_GIC_CFG_SC_N2N_INT_SEL9_REG        (CLOUD_GIC_CFG_BASE + 0x40C4) /* N-N型中断源选择寄存器9，选择int_sysctrl_nnspi9 */
#define CLOUD_GIC_CFG_SC_INT_MODE_REG            (CLOUD_GIC_CFG_BASE + 0x40D0) /* PERF中断模式配置寄存器 */
#define CLOUD_GIC_CFG_SC_ITS_CLK_ST_REG          (CLOUD_GIC_CFG_BASE + 0x5300) /* ITS时钟状态寄存器 */
#define CLOUD_GIC_CFG_SC_FTE_CLK_ST_REG          (CLOUD_GIC_CFG_BASE + 0x5308) /* FTE时钟状态寄存器 */
#define CLOUD_GIC_CFG_SC_SMMU_ICG_ST_REG         (CLOUD_GIC_CFG_BASE + 0x5380) /* SMMU时钟状态寄存器 */
#define CLOUD_GIC_CFG_SC_KERNEL_SLOW_CLK_ST_REG  (CLOUD_GIC_CFG_BASE + 0x5400) /* KERNEL_SLOW时钟状态寄存器 */
#define CLOUD_GIC_CFG_SC_GIC_CLK_ST_REG          (CLOUD_GIC_CFG_BASE + 0x5530) /* GIC时钟状态寄存器 */
#define CLOUD_GIC_CFG_SC_MBIGEN_CLK_ST_REG       (CLOUD_GIC_CFG_BASE + 0x5540) /* MBIGEN时钟状态寄存器 */
#define CLOUD_GIC_CFG_SC_ITS_RESET_ST_REG        (CLOUD_GIC_CFG_BASE + 0x5A00) /* ITS的复位状态寄存器 */
#define CLOUD_GIC_CFG_SC_FTE_RESET_ST_REG        (CLOUD_GIC_CFG_BASE + 0x5A08) /* FTE复位状态寄存器 */
#define CLOUD_GIC_CFG_SC_GIC_RESET_ST_REG        (CLOUD_GIC_CFG_BASE + 0x5C08) /* GIC复位状态寄存器 */
#define CLOUD_GIC_CFG_SC_MBIGEN_RESET_ST_REG     (CLOUD_GIC_CFG_BASE + 0x5C40) /* MBIGEN复位状态寄存器 */
#define CLOUD_GIC_CFG_SC_GIC_NOCMT_ST_REG        (CLOUD_GIC_CFG_BASE + 0x6000) /* 泰山无指令状态寄存器 */
#define CLOUD_GIC_CFG_GIC_CFG_VERSION0_REG       (CLOUD_GIC_CFG_BASE + 0xE0A0) /* VERSION寄存器0 */
#define CLOUD_GIC_CFG_GIC_CFG_MAGIC_WORD_REG     (CLOUD_GIC_CFG_BASE + 0xE0A4) /* GIC_CFG 版本号锁定寄存器 */
#define CLOUD_GIC_CFG_GIC_CFG_ECO_CFG0_REG       (CLOUD_GIC_CFG_BASE + 0xE0A8) /* 用于ECO的冗余配置寄存器1 */
#define CLOUD_GIC_CFG_GIC_CFG_ECO_CFG1_REG       (CLOUD_GIC_CFG_BASE + 0xE0AC) /* 用于ECO的冗余配置寄存器1 */
#define CLOUD_GIC_CFG_GIC_CFG_ECO_CFG2_REG       (CLOUD_GIC_CFG_BASE + 0xE0B0) /* 用于ECO的冗余配置寄存器2 */
#define CLOUD_GIC_CFG_GIC_CFG_ECO_CFG3_REG       (CLOUD_GIC_CFG_BASE + 0xE0B4) /* 用于ECO的冗余配置寄存器3 */
#define CLOUD_GIC_CFG_SC_SYSCTRL_LOCK_REG        (CLOUD_GIC_CFG_BASE + 0xF100) /* 系统控制器锁定寄存器。（注意：1，此寄存器空间必须是安全访问才能成功） */
#define CLOUD_GIC_CFG_SC_SYSCTRL_UNLOCK_REG      (CLOUD_GIC_CFG_BASE + 0xF110) /* 系统控制器解锁寄存器。（注意：1，此寄存器空间必须是安全访问才能成功） */
#define CLOUD_GIC_CFG_SC_N2N_INT_INFO0_REG       (CLOUD_GIC_CFG_BASE + 0xF200) /* N2N中断关键信息查询寄存器0 */
#define CLOUD_GIC_CFG_SC_N2N_INT_INFO1_REG       (CLOUD_GIC_CFG_BASE + 0xF204) /* N2N中断关键信息查询寄存器0 */
#define CLOUD_GIC_CFG_SC_N2N_INT_INFO2_REG       (CLOUD_GIC_CFG_BASE + 0xF208) /* N2N中断关键信息查询寄存器0 */
#define CLOUD_GIC_CFG_SC_ECO_RSV0_REG            (CLOUD_GIC_CFG_BASE + 0xFF00) /* ECO 寄存器 */
#define CLOUD_GIC_CFG_SC_ECO_RSV1_REG            (CLOUD_GIC_CFG_BASE + 0xFF04) /* ECO 寄存器1 */
#define CLOUD_GIC_CFG_SC_ECO_RSV2_REG            (CLOUD_GIC_CFG_BASE + 0xFF08) /* ECO 寄存器2 */
#define CLOUD_GIC_CFG_SC_ECO_RSV3_REG            (CLOUD_GIC_CFG_BASE + 0xFF0C) /* ECO 寄存器3 */
#define CLOUD_GIC_CFG_SC_ECO_RSV4_REG            (CLOUD_GIC_CFG_BASE + 0xFF10) /* ECO 寄存器4 */
#define CLOUD_GIC_CFG_SC_ECO_RSV5_REG            (CLOUD_GIC_CFG_BASE + 0xFF14) /* ECO 寄存器5 */
#define CLOUD_GIC_CFG_FPGA_VER_REG               (CLOUD_GIC_CFG_BASE + 0xFFFC) /* FPGA版本寄存器 */

#endif // __GIC_CFG_REG_OFFSET_H__
