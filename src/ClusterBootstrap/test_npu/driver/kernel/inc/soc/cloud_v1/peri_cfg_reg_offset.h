// ****************************************************************************** 
// Copyright     :  Copyright (C) 2018, Hisilicon Technologies Co. Ltd.
// File name     :  peri_cfg_reg_offset.h
// Project line  :  ICT
// Department    :  ICT Processor Chipset Development Dep
// Author        :  huawei
// Version       :  1.0
// Date          :  2017/11/13
// Description   :  The description of Hi1910 project
// Others        :  Generated automatically by nManager V4.2 
// History       :  huawei 2018/03/16 18:04:33 Create file
// ******************************************************************************

#ifndef __PERI_CFG_REG_OFFSET_H__
#define __PERI_CFG_REG_OFFSET_H__

/* PERI_CFG Base address of Module's Register */
#define SOC_PERI_CFG_BASE                       (0xc000000)

/******************************************************************************/
/*                      SOC PERI_CFG Registers' Definitions                            */
/******************************************************************************/

#define SOC_PERI_CFG_SC_ITS_CLK_EN_REG                   (SOC_PERI_CFG_BASE + 0x300)  /* ITS时钟使能寄存器 */
#define SOC_PERI_CFG_SC_ITS_CLK_DIS_REG                  (SOC_PERI_CFG_BASE + 0x304)  /* ITS时钟禁止寄存器 */
#define SOC_PERI_CFG_SC_FTE_CLK_EN_REG                   (SOC_PERI_CFG_BASE + 0x308)  /* FTE时钟使能寄存器 */
#define SOC_PERI_CFG_SC_FTE_CLK_DIS_REG                  (SOC_PERI_CFG_BASE + 0x30C)  /* FTE时钟禁止寄存器 */
#define SOC_PERI_CFG_SC_DBG_CLK_EN_REG                   (SOC_PERI_CFG_BASE + 0x310)  /* DBG时钟使能寄存器 */
#define SOC_PERI_CFG_SC_DBG_CLK_DIS_REG                  (SOC_PERI_CFG_BASE + 0x314)  /* DBG时钟禁止寄存器 */
#define SOC_PERI_CFG_SC_M3_CLK_EN_REG                    (SOC_PERI_CFG_BASE + 0x318)  /* M3时钟使能寄存器 */
#define SOC_PERI_CFG_SC_M3_CLK_DIS_REG                   (SOC_PERI_CFG_BASE + 0x31C)  /* M3时钟禁止寄存器 */
#define SOC_PERI_CFG_SC_GPIO_CLK_EN_REG                  (SOC_PERI_CFG_BASE + 0x320)  /* GPIO时钟使能寄存器 */
#define SOC_PERI_CFG_SC_GPIO_CLK_DIS_REG                 (SOC_PERI_CFG_BASE + 0x324)  /* GPIO时钟禁止寄存器 */
#define SOC_PERI_CFG_SC_GIC_CLK_EN_REG                   (SOC_PERI_CFG_BASE + 0x330)  /* GIC时钟使能寄存器 */
#define SOC_PERI_CFG_SC_GIC_CLK_DIS_REG                  (SOC_PERI_CFG_BASE + 0x334)  /* GIC时钟禁止寄存器 */
#define SOC_PERI_CFG_SC_SMMU_ICG_EN_REG                  (SOC_PERI_CFG_BASE + 0x380)  /* SMMU时钟使能寄存器 */
#define SOC_PERI_CFG_SC_SMMU_ICG_DIS_REG                 (SOC_PERI_CFG_BASE + 0x384)  /* SMMU时钟禁止寄存器 */
#define SOC_PERI_CFG_SC_TIMER32_CLK_EN0_REG              (SOC_PERI_CFG_BASE + 0x500)  /* TIMER32时钟使能寄存器 */
#define SOC_PERI_CFG_SC_TIMER32_CLK_DIS0_REG             (SOC_PERI_CFG_BASE + 0x504)  /* TIMER32时钟禁止寄存器 */
#define SOC_PERI_CFG_SC_TIMER32_CLK_EN1_REG              (SOC_PERI_CFG_BASE + 0x508)  /* TIMER32时钟使能寄存器 */
#define SOC_PERI_CFG_SC_TIMER32_CLK_DIS1_REG             (SOC_PERI_CFG_BASE + 0x50C)  /* TIMER32时钟禁止寄存器 */
#define SOC_PERI_CFG_SC_TIMER64_CLK_EN_REG               (SOC_PERI_CFG_BASE + 0x510)  /* TIMER64时钟使能寄存器 */
#define SOC_PERI_CFG_SC_TIMER64_CLK_DIS_REG              (SOC_PERI_CFG_BASE + 0x514)  /* TIMER64时钟禁止寄存器 */
#define SOC_PERI_CFG_SC_WATCHDOG_CLK_EN_REG              (SOC_PERI_CFG_BASE + 0x520)  /* WATCHDOG时钟使能寄存器 */
#define SOC_PERI_CFG_SC_WATCHDOG_CLK_DIS_REG             (SOC_PERI_CFG_BASE + 0x524)  /* WATCHDOG时钟禁止寄存器 */
#define SOC_PERI_CFG_SC_UART_CLK_EN_REG                  (SOC_PERI_CFG_BASE + 0x548)  /* UART时钟使能寄存器 */
#define SOC_PERI_CFG_SC_UART_CLK_DIS_REG                 (SOC_PERI_CFG_BASE + 0x54C)  /* UART时钟禁止寄存器 */
#define SOC_PERI_CFG_SC_MDIO_CLK_EN_REG                  (SOC_PERI_CFG_BASE + 0x560)  /* MDIO时钟使能寄存器 */
#define SOC_PERI_CFG_SC_MDIO_CLK_DIS_REG                 (SOC_PERI_CFG_BASE + 0x564)  /* MDIO时钟禁止寄存器 */
#define SOC_PERI_CFG_SC_SMB_CLK_EN_REG                   (SOC_PERI_CFG_BASE + 0x590)  /* SMB时钟使能寄存器 */
#define SOC_PERI_CFG_SC_SMB_CLK_DIS_REG                  (SOC_PERI_CFG_BASE + 0x594)  /* SMB时钟禁止寄存器 */
#define SOC_PERI_CFG_SC_CER_CLK_EN_REG                   (SOC_PERI_CFG_BASE + 0x598)  /* CER时钟使能寄存器 */
#define SOC_PERI_CFG_SC_CER_CLK_DIS_REG                  (SOC_PERI_CFG_BASE + 0x59C)  /* CER时钟禁止寄存器 */
#define SOC_PERI_CFG_SC_SFC2X_CLK_EN_REG                 (SOC_PERI_CFG_BASE + 0x5A0)  /* SFC2X时钟使能寄存器 */
#define SOC_PERI_CFG_SC_SC2X_CLK_DIS_REG                 (SOC_PERI_CFG_BASE + 0x5A4)  /* SFC2X时钟禁止寄存器 */
#define SOC_PERI_CFG_SC_SFC1X_CLK_EN_REG                 (SOC_PERI_CFG_BASE + 0x5A8)  /* SFC1X时钟使能寄存器 */
#define SOC_PERI_CFG_SC_SFC1X_CLK_DIS_REG                (SOC_PERI_CFG_BASE + 0x5AC)  /* SFC1X时钟禁止寄存器 */
#define SOC_PERI_CFG_SC_SDMA_ICG_EN_REG                  (SOC_PERI_CFG_BASE + 0x5B0)  /* SDMA时钟使能寄存器 */
#define SOC_PERI_CFG_SC_SDMA_ICG_DIS_REG                 (SOC_PERI_CFG_BASE + 0x5B4)  /* SDMA时钟禁止寄存器 */
#define SOC_PERI_CFG_SC_TRNG_ICG_EN_REG                  (SOC_PERI_CFG_BASE + 0x5B8)  /* TRNG时钟使能寄存器 */
#define SOC_PERI_CFG_SC_TRNG_ICG_DIS_REG                 (SOC_PERI_CFG_BASE + 0x5BC)  /* TRNG时钟禁止寄存器 */
#define SOC_PERI_CFG_SC_EMMC_ICG_EN_REG                  (SOC_PERI_CFG_BASE + 0x5C0)  /* EMMC时钟使能寄存器 */
#define SOC_PERI_CFG_SC_EMMC_ICG_DIS_REG                 (SOC_PERI_CFG_BASE + 0x5C4)  /* EMMC时钟禁止寄存器 */
#define SOC_PERI_CFG_SC_ITS_RESET_REQ_REG                (SOC_PERI_CFG_BASE + 0xA00)  /* ITS软复位请求控制寄存器 */
#define SOC_PERI_CFG_SC_ITS_RESET_DREQ_REG               (SOC_PERI_CFG_BASE + 0xA04)  /* ITS软复位去请求控制寄存器 */
#define SOC_PERI_CFG_SC_FTE_RESET_REQ_REG                (SOC_PERI_CFG_BASE + 0xA08)  /* FTE软复位请求控制寄存器 */
#define SOC_PERI_CFG_SC_FTE_RESET_DREQ_REG               (SOC_PERI_CFG_BASE + 0xA0C)  /* FTE软复位去请求控制寄存器 */
#define SOC_PERI_CFG_SC_DBG_RESET_REQ_REG                (SOC_PERI_CFG_BASE + 0xA10)  /* DBG软复位请求控制寄存器 */
#define SOC_PERI_CFG_SC_DBG_RESET_DREQ_REG               (SOC_PERI_CFG_BASE + 0xA14)  /* DBG软复位去请求控制寄存器 */
#define SOC_PERI_CFG_SC_GPIO_RESET_REQ_REG               (SOC_PERI_CFG_BASE + 0xA20)  /* GPIO软复位请求控制寄存器 */
#define SOC_PERI_CFG_SC_GPIO_RESET_DREQ_REG              (SOC_PERI_CFG_BASE + 0xA24)  /* GPIO软复位去请求控制寄存器 */
#define SOC_PERI_CFG_SC_WATCHDOG_RESET_REQ_REG           (SOC_PERI_CFG_BASE + 0xC00)  /* WATCHDOG软复位请求控制寄存器 */
#define SOC_PERI_CFG_SC_WATCHDOG_RESET_DREQ_REG          (SOC_PERI_CFG_BASE + 0xC04)  /* WATCHDOG软复位去请求控制寄存器 */
#define SOC_PERI_CFG_SC_GIC_RESET_REQ_REG                (SOC_PERI_CFG_BASE + 0xC08)  /* GIC软复位请求控制寄存器 */
#define SOC_PERI_CFG_SC_GIC_RESET_DREQ_REG               (SOC_PERI_CFG_BASE + 0xC0C)  /* GIC软复位去请求控制寄存器 */
#define SOC_PERI_CFG_SC_UART_RESET_REQ_REG               (SOC_PERI_CFG_BASE + 0xC28)  /* UART软复位请求控制寄存器 */
#define SOC_PERI_CFG_SC_UART_RESET_DREQ_REG              (SOC_PERI_CFG_BASE + 0xC2C)  /* UART软复位去请求控制寄存器 */
#define SOC_PERI_CFG_SC_MDIO_RESET_REQ_REG               (SOC_PERI_CFG_BASE + 0xC40)  /* MDIO软复位请求控制寄存器 */
#define SOC_PERI_CFG_SC_MDIO_RESET_DREQ_REG              (SOC_PERI_CFG_BASE + 0xC44)  /* MDIO软复位去请求控制寄存器 */
#define SOC_PERI_CFG_SC_M3_RESET_REQ_REG                 (SOC_PERI_CFG_BASE + 0xC68)  /* M3软复位请求控制寄存器 */
#define SOC_PERI_CFG_SC_M3_RESET_DREQ_REG                (SOC_PERI_CFG_BASE + 0xC6C)  /* M3软复位去请求控制寄存器 */
#define SOC_PERI_CFG_SC_SMB_RESET_REQ_REG                (SOC_PERI_CFG_BASE + 0xC70)  /* SMB软复位请求控制寄存器 */
#define SOC_PERI_CFG_SC_SMB_RESET_DREQ_REG               (SOC_PERI_CFG_BASE + 0xC74)  /* SMB软复位去请求控制寄存器 */
#define SOC_PERI_CFG_SC_CER_RESET_REQ_REG                (SOC_PERI_CFG_BASE + 0xC78)  /* CER软复位请求控制寄存器 */
#define SOC_PERI_CFG_SC_CER_RESET_DREQ_REG               (SOC_PERI_CFG_BASE + 0xC7C)  /* CER软复位去请求控制寄存器 */
#define SOC_PERI_CFG_SC_TIMER_RESET_REQ_REG              (SOC_PERI_CFG_BASE + 0xC80)  /* TIMER软复位请求控制寄存器 */
#define SOC_PERI_CFG_SC_TIMER_RESET_DREQ_REG             (SOC_PERI_CFG_BASE + 0xC84)  /* TIMER软复位请求控制寄存器 */
#define SOC_PERI_CFG_SC_SFC_RESET_REQ_REG                (SOC_PERI_CFG_BASE + 0xC90)  /* SFC软复位请求控制寄存器 */
#define SOC_PERI_CFG_SC_SFC_RESET_DREQ_REG               (SOC_PERI_CFG_BASE + 0xC94)  /* SFC软复位去请求控制寄存器 */
#define SOC_PERI_CFG_SC_SDMA_RESET_REQ_REG               (SOC_PERI_CFG_BASE + 0xCA0)  /* SDMA软复位请求控制寄存器 */
#define SOC_PERI_CFG_SC_SDMA_RESET_DREQ_REG              (SOC_PERI_CFG_BASE + 0xCA4)  /* SDMA软复位去请求控制寄存器 */
#define SOC_PERI_CFG_SC_TRNG_RESET_REQ_REG               (SOC_PERI_CFG_BASE + 0xCA8)  /* TRNG软复位请求控制寄存器 */
#define SOC_PERI_CFG_SC_TRNG_RESET_DREQ_REG              (SOC_PERI_CFG_BASE + 0xCAC)  /* TRNG软复位去请求控制寄存器 */
#define SOC_PERI_CFG_SC_EMMC_RESET_REQ_REG               (SOC_PERI_CFG_BASE + 0xCB0)  /* EMMC软复位请求控制寄存器 */
#define SOC_PERI_CFG_SC_EMMC_RESET_DREQ_REG              (SOC_PERI_CFG_BASE + 0xCB4)  /* EMMC软复位去请求控制寄存器 */
#define SOC_PERI_CFG_SC_CHAIN_ERR_CLR_REG                (SOC_PERI_CFG_BASE + 0x1100) /* CFG_BUS中断清除寄存器 */
#define SOC_PERI_CFG_SC_CHAIN_ERR_INTMASK_REG            (SOC_PERI_CFG_BASE + 0x1104) /* CFG_BUS CHAIN ERR的中断屏蔽寄存器 */
#define SOC_PERI_CFG_SC_CHAIN_ERR_INT_STATUS_REG         (SOC_PERI_CFG_BASE + 0x1108) /* CFG_BUS CHAIN ERR的原始中断状态寄存器 */
#define SOC_PERI_CFG_SC_CHAIN_ERR_CPU_CLR_REG            (SOC_PERI_CFG_BASE + 0x1110) /* CFG_BUS_CPU中断清除寄存器 */
#define SOC_PERI_CFG_SC_CHAIN_ERR_CPU_INTMASK_REG        (SOC_PERI_CFG_BASE + 0x1114) /* CFG_BUS_CPU CHAIN ERR的中断屏蔽寄存器 */
#define SOC_PERI_CFG_SC_CHAIN_ERR_CPU_INT_STATUS_REG     (SOC_PERI_CFG_BASE + 0x1118) /* CFG_BUS_CPU CHAIN ERR的原始中断状态寄存器 */
#define SOC_PERI_CFG_SC_DISPATCH_ERRRSP_REG              (SOC_PERI_CFG_BASE + 0x2064) /* dispatch的ERR响应类型控制寄存器 */
#define SOC_PERI_CFG_SC_M3_CTRL0_REG                     (SOC_PERI_CFG_BASE + 0x20B0) /* M3控制0寄存器。 */
#define SOC_PERI_CFG_SC_M3_CTRL1_REG                     (SOC_PERI_CFG_BASE + 0x20B4) /* M3控制寄存器1。 */
#define SOC_PERI_CFG_SC_TSENSOR1_ALARM0_REG              (SOC_PERI_CFG_BASE + 0x20C0) /* 片上温度传感器温度门限配置寄存器 */
#define SOC_PERI_CFG_SC_TSENSOR1_SAMPLE_NUM_REG          (SOC_PERI_CFG_BASE + 0x20C4) /* 硬件连读Tsensor温度值后取平均的配置寄存器（禁止配置为0x0和0x1） */
#define SOC_PERI_CFG_SC_TSENSOR1_CTRL_REG                (SOC_PERI_CFG_BASE + 0x20D0) /* 片上温度传感器Tsensor0工作参数配置。 */
#define SOC_PERI_CFG_SC_ULTRASOC_USER_CFG_REG            (SOC_PERI_CFG_BASE + 0x2100) /* ULTRASOC AXI Master的USER域段配置 */
#define SOC_PERI_CFG_SC_ULTRASOC_PORT_SEL_REG            (SOC_PERI_CFG_BASE + 0x2104) /* ULTRASOC PORT选择配置寄存器 */
#define SOC_PERI_CFG_SC_ULTRASOC_USER_NS_CFG_REG         (SOC_PERI_CFG_BASE + 0x2108) /* ULTRASOC AXI Master的USER域段中NS配置（安全寄存器） */
#define SOC_PERI_CFG_SC_ULTRASOC_CTRL_REG                (SOC_PERI_CFG_BASE + 0x210C) /* ULTRASOC 控制寄存器 */
#define SOC_PERI_CFG_SC_M3_REMAP_ADDR_EN_REG             (SOC_PERI_CFG_BASE + 0x2190) /* m3地址映射使能寄存器。 */
#define SOC_PERI_CFG_SC_M3_REMAP_ADDR_0_REG              (SOC_PERI_CFG_BASE + 0x2194) /* M3 remap到spi flash的基地址配置寄存器。 */
#define SOC_PERI_CFG_SC_M3_REMAP_ADDR_1_REG              (SOC_PERI_CFG_BASE + 0x2198) /* M3 remap到sram的基地址配置寄存器。 */
#define SOC_PERI_CFG_SC_M3_REMAP_ADDR_2_REG              (SOC_PERI_CFG_BASE + 0x219C) /* M3地址重映射寄存器。 */
#define SOC_PERI_CFG_SC_M3_REMAP_ADDR_3_REG              (SOC_PERI_CFG_BASE + 0x21A0) /* M3地址重映射寄存器。 */
#define SOC_PERI_CFG_SC_M3_REMAP_ADDR_4_REG              (SOC_PERI_CFG_BASE + 0x21A4) /* M3地址重映射寄存器。 */
#define SOC_PERI_CFG_SC_M3_REMAP_ADDR_5_REG              (SOC_PERI_CFG_BASE + 0x21A8) /* M3地址重映射寄存器。 */
#define SOC_PERI_CFG_SC_M3_REMAP_ADDR_6_REG              (SOC_PERI_CFG_BASE + 0x21AC) /* M3地址重映射寄存器。 */
#define SOC_PERI_CFG_SC_M3_REMAP_ADDR_7_REG              (SOC_PERI_CFG_BASE + 0x21B0) /* M3地址重映射寄存器。 */
#define SOC_PERI_CFG_SC_M3_REMAP_ADDR_8_REG              (SOC_PERI_CFG_BASE + 0x21B4) /* M3地址重映射寄存器。 */
#define SOC_PERI_CFG_SC_M3_REMAP_ADDR_9_REG              (SOC_PERI_CFG_BASE + 0x21B8) /* M3地址重映射寄存器。 */
#define SOC_PERI_CFG_SC_M3_REMAP_ADDR_10_REG             (SOC_PERI_CFG_BASE + 0x21BC) /* M3地址重映射寄存器。 */
#define SOC_PERI_CFG_SC_M3_REMAP_ADDR_11_REG             (SOC_PERI_CFG_BASE + 0x21C0) /* M3地址重映射寄存器。 */
#define SOC_PERI_CFG_SC_M3_REMAP_ADDR_12_REG             (SOC_PERI_CFG_BASE + 0x21C4) /* M3地址重映射寄存器。 */
#define SOC_PERI_CFG_SC_M3_REMAP_ADDR_13_REG             (SOC_PERI_CFG_BASE + 0x21C8) /* M3地址重映射寄存器。 */
#define SOC_PERI_CFG_SC_M3_REMAP_ADDR_14_REG             (SOC_PERI_CFG_BASE + 0x21CC) /* M3地址重映射寄存器。 */
#define SOC_PERI_CFG_SC_M3_REMAP_ADDR_15_REG             (SOC_PERI_CFG_BASE + 0x21D0) /* M3地址重映射寄存器。 */
#define SOC_PERI_CFG_SC_CFG_AXUSER_L_M3_0_REG            (SOC_PERI_CFG_BASE + 0x2208) /* 协议AXUSER的低32bit配置寄存器 */
#define SOC_PERI_CFG_SC_CFG_AXUSER_M_M3_0_REG            (SOC_PERI_CFG_BASE + 0x220C) /* 协议AXUSER的中间32bit配置寄存器 */
#define SOC_PERI_CFG_SC_CFG_AXUSER_H_M3_0_REG            (SOC_PERI_CFG_BASE + 0x2210) /* 协议AXUSER的高4bit配置寄存器 */
#define SOC_PERI_CFG_SC_CFG_CACHE_CTRL_M3_0_REG          (SOC_PERI_CFG_BASE + 0x2214) /* M3 AXCACHE控制寄存器 */
#define SOC_PERI_CFG_SC_CFG_AXUSER_L_M3_1_REG            (SOC_PERI_CFG_BASE + 0x2220) /* 协议AXUSER的低32bit配置寄存器 */
#define SOC_PERI_CFG_SC_CFG_AXUSER_M_M3_1_REG            (SOC_PERI_CFG_BASE + 0x2224) /* 协议AXUSER的中间32bit配置寄存器 */
#define SOC_PERI_CFG_SC_CFG_AXUSER_H_M3_1_REG            (SOC_PERI_CFG_BASE + 0x2228) /* 协议AXUSER的高4bit配置寄存器 */
#define SOC_PERI_CFG_SC_CFG_CACHE_CTRL_M3_1_REG          (SOC_PERI_CFG_BASE + 0x222C) /* M3 AXCACHE控制寄存器 */
#define SOC_PERI_CFG_SC_CFG_AXUSER_L_M3_2_REG            (SOC_PERI_CFG_BASE + 0x2230) /* 协议AWUSER的低32bit配置寄存器 */
#define SOC_PERI_CFG_SC_CFG_AXUSER_M_M3_2_REG            (SOC_PERI_CFG_BASE + 0x2234) /* 协议AWUSER的中间32bit配置寄存器 */
#define SOC_PERI_CFG_SC_CFG_AXUSER_H_M3_2_REG            (SOC_PERI_CFG_BASE + 0x2238) /* 协议AWUSER的高4bit配置寄存器 */
#define SOC_PERI_CFG_SC_CFG_CACHE_CTRL_M3_2_REG          (SOC_PERI_CFG_BASE + 0x223C) /* M3 AXCACHE控制寄存器 */
#define SOC_PERI_CFG_SC_QOS_CTRL_M3_REG                  (SOC_PERI_CFG_BASE + 0x2240) /* M3 QOS控制寄存器 */
#define SOC_PERI_CFG_SC_QOS_CTRL_GIC_REG                 (SOC_PERI_CFG_BASE + 0x2244) /* GIC QOS控制寄存器 */
#define SOC_PERI_CFG_SC_QOS_CTRL_EMMC_REG                (SOC_PERI_CFG_BASE + 0x2248) /* EMMC QOS控制寄存器 */
#define SOC_PERI_CFG_SC_QOS_CTRL_SDMAM_REG               (SOC_PERI_CFG_BASE + 0x224C) /* SDMAM QOS控制寄存器 */
#define SOC_PERI_CFG_SC_EXT_INT_POLARITY_REG             (SOC_PERI_CFG_BASE + 0x2300) /* 外部中断极性配置寄存器 */
#define SOC_PERI_CFG_SC_CFG_BUS_CTRL0_REG                (SOC_PERI_CFG_BASE + 0x2400) /* CFG BUS命令直接延迟使能寄存器 */
#define SOC_PERI_CFG_SC_CFG_BUS_CTRL1_REG                (SOC_PERI_CFG_BASE + 0x2404) /* CFG BUS命令直接延迟周期数 配置寄存器 */
#define SOC_PERI_CFG_SC_CFG_BUS_DBG_CTRL0_REG            (SOC_PERI_CFG_BASE + 0x2408) /* CFG BUS命令直接延迟使能寄存器 */
#define SOC_PERI_CFG_SC_CFG_BUS_DBG_CTRL1_REG            (SOC_PERI_CFG_BASE + 0x240C) /* CFG BUS命令直接延迟周期数 配置寄存器 */
#define SOC_PERI_CFG_SC_MEM_CTRL_SMMU_REG                (SOC_PERI_CFG_BASE + 0x3000) /* smmu mem控制寄存器0 */
#define SOC_PERI_CFG_SC_MEM_CTRL_GIC_REG                 (SOC_PERI_CFG_BASE + 0x3020) /* gic mem控制寄存器 */
#define SOC_PERI_CFG_SC_MEM_CTRL_ULTRASOC_REG            (SOC_PERI_CFG_BASE + 0x3040) /* ultrasoc mem控制寄存器0 */
#define SOC_PERI_CFG_SC_MEM_CTRL_CER_REG                 (SOC_PERI_CFG_BASE + 0x3050) /* CER mem控制寄存器0 */
#define SOC_PERI_CFG_SC_MEM_CTRL_TRNG_REG                (SOC_PERI_CFG_BASE + 0x3060) /* TRNG mem控制寄存器0 */
#define SOC_PERI_CFG_SC_TIMER64_EN_CTRL_REG              (SOC_PERI_CFG_BASE + 0x3104) /* TIMER64的外部使能控制寄存器 */
#define SOC_PERI_CFG_SC_WDOG_SRST_MASK_REG               (SOC_PERI_CFG_BASE + 0x3200) /* 非安全狗复位请求MASK控制寄存器 */
#define SOC_PERI_CFG_SC_TIMER_CLK_SEL_REG                (SOC_PERI_CFG_BASE + 0x3134) /* TIMER的时钟选择寄存器 */
#define SOC_PERI_CFG_SC_BUS_NUM_PERI_REG                 (SOC_PERI_CFG_BASE + 0x3400) /* PERI SMMU BUSNUM配置寄存器 */
#define SOC_PERI_CFG_SC_DVE_NUM_PERI_REG                 (SOC_PERI_CFG_BASE + 0x3404) /* PERI SMMU DVENUM配置寄存器 */
#define SOC_PERI_CFG_SC_CFG_BUS_WAIT_REG                 (SOC_PERI_CFG_BASE + 0x3410) /* CFG_BUS 超时等待配置寄存器 */
#define SOC_PERI_CFG_SC_EMMC_CLK_SEL_REG                 (SOC_PERI_CFG_BASE + 0x3500) /* EMMC_CLK_SEL配置寄存器 */
#define SOC_PERI_CFG_SC_AXI_USER_L32_EMMC_REG            (SOC_PERI_CFG_BASE + 0x3504) /* EMMC user低32-bit域段配置寄存器 */
#define SOC_PERI_CFG_SC_AXI_USER_H32_EMMC_REG            (SOC_PERI_CFG_BASE + 0x3508) /* EMMC user高32-bit域段配置寄存器 */
#define SOC_PERI_CFG_SC_AXI_USER_67_64_EMMC_REG          (SOC_PERI_CFG_BASE + 0x350C) /* EMMC user[67:64]配置寄存器 */
#define SOC_PERI_CFG_SC_AXI_CACHE_EMMC_REG               (SOC_PERI_CFG_BASE + 0x3510) /* AXI Cache配置寄存器 */
#define SOC_PERI_CFG_SC_AXI_PORT_SEL_REG                 (SOC_PERI_CFG_BASE + 0x3514) /* AXI MUX配置寄存器 */
#define SOC_PERI_CFG_SC_SFC_BYP_CTRL_REG                 (SOC_PERI_CFG_BASE + 0x3518) /* SFC AHB防挂死配置寄存器 */
#define SOC_PERI_CFG_SC_AHB_BYP_CTRL_REG                 (SOC_PERI_CFG_BASE + 0x351C) /* AHB防挂死配置寄存器 */
#define SOC_PERI_CFG_SC_UART_SEL_REG                     (SOC_PERI_CFG_BASE + 0x3520) /* UART选择寄存器 */
#define SOC_PERI_CFG_SC_ULTRASOC_USER_CRL_REG            (SOC_PERI_CFG_BASE + 0x3600) /* ULTRASOC AxUSER配置寄存器 */
#define SOC_PERI_CFG_SC_ULTRASOC_AXPROT_CRL_REG          (SOC_PERI_CFG_BASE + 0x3604) /* ULTRASOC AxPROT配置寄存器 */
#define SOC_PERI_CFG_SC_SFC_DAW_CRL_REG                  (SOC_PERI_CFG_BASE + 0x3650) /* DAW SFC控制寄存器 */
#define SOC_PERI_CFG_SC_EMMC_HADDR39_CRL_REG             (SOC_PERI_CFG_BASE + 0x3680) /* EMMC HADDR控制寄存器 */
#define SOC_PERI_CFG_SC_EXT_INT_MASK_REG                 (SOC_PERI_CFG_BASE + 0x3800) /* 外部管脚屏蔽寄存器 */
#define SOC_PERI_CFG_SC_AO_WAKEUP_INT_MASK_REG           (SOC_PERI_CFG_BASE + 0x3804) /* AO区唤醒中断屏蔽寄存器 */
#define SOC_PERI_CFG_SC_TSENSOR1_INT_REG                 (SOC_PERI_CFG_BASE + 0x4010) /* TSENSOR中断源寄存器 */
#define SOC_PERI_CFG_SC_TSENSOR1_INT_MASK_REG            (SOC_PERI_CFG_BASE + 0x4014) /* TSENSOR中断屏蔽寄存器 */
#define SOC_PERI_CFG_SC_PCIE_PERST_INT_REG               (SOC_PERI_CFG_BASE + 0x4050) /* PCIE PERST_N中断源寄存器 */
#define SOC_PERI_CFG_SC_PCIE_PERST_INT_MASK_REG          (SOC_PERI_CFG_BASE + 0x4054) /* PCIE PERST_N中断屏蔽寄存器 */
#define SOC_PERI_CFG_SC_PWR_CORE_INT_REG                 (SOC_PERI_CFG_BASE + 0x4070) /* CORE上下电中断源寄存器 */
#define SOC_PERI_CFG_SC_PWR_CORE_INT_MASK_REG            (SOC_PERI_CFG_BASE + 0x4074) /* CORE上下电中断屏蔽寄存器 */
#define SOC_PERI_CFG_SC_ITS_CLK_ST_REG                   (SOC_PERI_CFG_BASE + 0x5300) /* ITS时钟状态寄存器 */
#define SOC_PERI_CFG_SC_FTE_CLK_ST_REG                   (SOC_PERI_CFG_BASE + 0x5308) /* FTE时钟状态寄存器 */
#define SOC_PERI_CFG_SC_DBG_CLK_ST_REG                   (SOC_PERI_CFG_BASE + 0x5310) /* DBG时钟状态寄存器 */
#define SOC_PERI_CFG_SC_M3_CLK_ST_REG                    (SOC_PERI_CFG_BASE + 0x5318) /* M3时钟状态寄存器 */
#define SOC_PERI_CFG_SC_GPIO_CLK_ST_REG                  (SOC_PERI_CFG_BASE + 0x5320) /* GPIO时钟状态寄存器 */
#define SOC_PERI_CFG_SC_GIC_CLK_ST_REG                   (SOC_PERI_CFG_BASE + 0x5330) /* GIC时钟状态寄存器 */
#define SOC_PERI_CFG_SC_SMMU_ICG_ST_REG                  (SOC_PERI_CFG_BASE + 0x5380) /* SMMU时钟状态寄存器 */
#define SOC_PERI_CFG_SC_TIMER32_CLK_ST0_REG              (SOC_PERI_CFG_BASE + 0x5500) /* TIMER32时钟状态寄存器 */
#define SOC_PERI_CFG_SC_TIMER32_CLK_ST1_REG              (SOC_PERI_CFG_BASE + 0x5508) /* TIMER32时钟状态寄存器 */
#define SOC_PERI_CFG_SC_TIMER64_CLK_ST_REG               (SOC_PERI_CFG_BASE + 0x5510) /* TIMER64时钟状态寄存器 */
#define SOC_PERI_CFG_SC_WATCHDOG_CLK_ST_REG              (SOC_PERI_CFG_BASE + 0x5520) /* WADTHDOG时钟状态寄存器 */
#define SOC_PERI_CFG_SC_UART_CLK_ST_REG                  (SOC_PERI_CFG_BASE + 0x5548) /* UART时钟状态寄存器 */
#define SOC_PERI_CFG_SC_MDIO_CLK_ST_REG                  (SOC_PERI_CFG_BASE + 0x5560) /* MDIO时钟状态寄存器 */
#define SOC_PERI_CFG_SC_SMB_CLK_ST_REG                   (SOC_PERI_CFG_BASE + 0x5590) /* SMB时钟状态寄存器 */
#define SOC_PERI_CFG_SC_CER_CLK_ST_REG                   (SOC_PERI_CFG_BASE + 0x5598) /* CER时钟状态寄存器 */
#define SOC_PERI_CFG_SC_SFC2X_CLK_ST_REG                 (SOC_PERI_CFG_BASE + 0x55A0) /* SFC2X时钟状态寄存器 */
#define SOC_PERI_CFG_SC_SFC1X_CLK_ST_REG                 (SOC_PERI_CFG_BASE + 0x55A8) /* SFC1X时钟状态寄存器 */
#define SOC_PERI_CFG_SC_SDMA_CLK_ST_REG                  (SOC_PERI_CFG_BASE + 0x55B0) /* SDMA时钟状态寄存器 */
#define SOC_PERI_CFG_SC_TRNG_CLK_ST_REG                  (SOC_PERI_CFG_BASE + 0x55B8) /* TRNG时钟状态寄存器 */
#define SOC_PERI_CFG_SC_EMMC_CLK_ST_REG                  (SOC_PERI_CFG_BASE + 0x55C0) /* EMMC时钟状态寄存器 */
#define SOC_PERI_CFG_SC_ITS_RESET_ST_REG                 (SOC_PERI_CFG_BASE + 0x5A00) /* ITS的复位状态寄存器 */
#define SOC_PERI_CFG_SC_FTE_RESET_ST_REG                 (SOC_PERI_CFG_BASE + 0x5A08) /* FTE复位状态寄存器 */
#define SOC_PERI_CFG_SC_DBG_RESET_ST_REG                 (SOC_PERI_CFG_BASE + 0x5A10) /* DBG复位状态寄存器 */
#define SOC_PERI_CFG_SC_GPIO_RESET_ST_REG                (SOC_PERI_CFG_BASE + 0x5A20) /* GPIO复位状态寄存器 */
#define SOC_PERI_CFG_SC_WATCHDOG_RESET_ST_REG            (SOC_PERI_CFG_BASE + 0x5C00) /* WATCHDOG复位状态寄存器 */
#define SOC_PERI_CFG_SC_GIC_RESET_ST_REG                 (SOC_PERI_CFG_BASE + 0x5C08) /* GIC复位状态寄存器 */
#define SOC_PERI_CFG_SC_UART_RESET_ST_REG                (SOC_PERI_CFG_BASE + 0x5C28) /* UART复位状态寄存器 */
#define SOC_PERI_CFG_SC_MDIO_RESET_ST_REG                (SOC_PERI_CFG_BASE + 0x5C40) /* MDIO复位状态寄存器 */
#define SOC_PERI_CFG_SC_M3_RESET_ST_REG                  (SOC_PERI_CFG_BASE + 0x5C68) /* M3复位状态寄存器 */
#define SOC_PERI_CFG_SC_SMB_RESET_ST_REG                 (SOC_PERI_CFG_BASE + 0x5C70) /* SMB复位状态寄存器 */
#define SOC_PERI_CFG_SC_CER_RESET_ST_REG                 (SOC_PERI_CFG_BASE + 0x5C78) /* CER复位状态寄存器 */
#define SOC_PERI_CFG_SC_TIMER_RESET_ST_REG               (SOC_PERI_CFG_BASE + 0x5C80) /* TIMER复位状态寄存器 */
#define SOC_PERI_CFG_SC_SFC_RESET_ST_REG                 (SOC_PERI_CFG_BASE + 0x5C90) /* SFC复位状态寄存器 */
#define SOC_PERI_CFG_SC_SDMA_RESET_ST_REG                (SOC_PERI_CFG_BASE + 0x5CA0) /* SDMA复位状态寄存器 */
#define SOC_PERI_CFG_SC_TRNG_RESET_ST_REG                (SOC_PERI_CFG_BASE + 0x5CA8) /* TRNG复位状态寄存器 */
#define SOC_PERI_CFG_SC_EMMC_RESET_ST_REG                (SOC_PERI_CFG_BASE + 0x5CB0) /* EMMC复位状态寄存器 */
#define SOC_PERI_CFG_SC_TSENSOR1_ST_REG                  (SOC_PERI_CFG_BASE + 0x60D0) /* tsensor系统状态寄存器。 */
#define SOC_PERI_CFG_SC_TSENSOR1_TEMP_SAMPLE_AVERAGE_REG (SOC_PERI_CFG_BASE + 0x60D4) /* 芯片温度采样值 */
#define SOC_PERI_CFG_SC_ULTRASOC_MEM_ECC_REG             (SOC_PERI_CFG_BASE + 0x6100) /* UltraSoC Memory ECC状态 */
#define SOC_PERI_CFG_SC_M3_STAT_REG                      (SOC_PERI_CFG_BASE + 0x6104) 
#define SOC_PERI_CFG_SC_PCIE_PERESET_ST_REG              (SOC_PERI_CFG_BASE + 0x6200) /* PCIE PERST_N管脚电平状态寄存器。 */
#define SOC_PERI_CFG_SC_EXT_INT_POLARITY_ST_REG          (SOC_PERI_CFG_BASE + 0x6300) /* 外部管脚中断极性配置后的状态寄存器 */
#define SOC_PERI_CFG_SC_EXT_INT_POLARITY_MASK_ST_REG     (SOC_PERI_CFG_BASE + 0x6304) /* 外部管脚中断屏蔽配置后的状态寄存器 */
#define SOC_PERI_CFG_SC_TRNG_FSM_CTRL_REG                (SOC_PERI_CFG_BASE + 0x8000) /* TRNG FSM状态寄存器 */
#define SOC_PERI_CFG_SC_TSENSOR1_INT_STATUS_REG          (SOC_PERI_CFG_BASE + 0x8010) /* TSENSOR中断状态寄存器 */
#define SOC_PERI_CFG_SC_PCIE_PERST_INT_STATUS_REG        (SOC_PERI_CFG_BASE + 0x8050) /* PCIE PERST中断状态寄存器 */
#define SOC_PERI_CFG_SC_PWR_CORE_INT_STATUS_REG          (SOC_PERI_CFG_BASE + 0x8070) /* CORE上下电中断状态寄存器 */
#define SOC_PERI_CFG_SC_KEY_INFO_REG                     (SOC_PERI_CFG_BASE + 0xE000) /* 关键信息寄存器 */
#define SOC_PERI_CFG_PERI_CFG_VERSION0_REG               (SOC_PERI_CFG_BASE + 0xE0A0) /* VERSION寄存器0 */
#define SOC_PERI_CFG_PERI_CFG_MAGIC_WORD_REG             (SOC_PERI_CFG_BASE + 0xE0A4) /* PERI_CFG 版本号锁定寄存器 */
#define SOC_PERI_CFG_PERI_CFG_ECO_CFG0_REG               (SOC_PERI_CFG_BASE + 0xE0A8) /* 用于ECO的冗余配置寄存器1 */
#define SOC_PERI_CFG_PERI_CFG_ECO_CFG1_REG               (SOC_PERI_CFG_BASE + 0xE0AC) /* 用于ECO的冗余配置寄存器1 */
#define SOC_PERI_CFG_PERI_CFG_ECO_CFG2_REG               (SOC_PERI_CFG_BASE + 0xE0B0) /* 用于ECO的冗余配置寄存器2 */
#define SOC_PERI_CFG_PERI_CFG_ECO_CFG3_REG               (SOC_PERI_CFG_BASE + 0xE0B4) /* 用于ECO的冗余配置寄存器3 */
#define SOC_PERI_CFG_SC_SYSCTRL_LOCK_REG                 (SOC_PERI_CFG_BASE + 0xF100) /* 系统控制器锁定寄存器。（注意：1，此寄存器空间必须是安全访问才能成功） */
#define SOC_PERI_CFG_SC_SYSCTRL_UNLOCK_REG               (SOC_PERI_CFG_BASE + 0xF110) /* 系统控制器解锁寄存器。（注意：1，此寄存器空间必须是安全访问才能成功） */
#define SOC_PERI_CFG_SC_ECO_RSV0_REG                     (SOC_PERI_CFG_BASE + 0xFF00) /* ECO 寄存器 */
#define SOC_PERI_CFG_SC_ECO_RSV1_REG                     (SOC_PERI_CFG_BASE + 0xFF04) /* ECO 寄存器1 */
#define SOC_PERI_CFG_SC_ECO_RSV2_REG                     (SOC_PERI_CFG_BASE + 0xFF08) /* ECO 寄存器2 */
#define SOC_PERI_CFG_SC_ECO_RSV3_REG                     (SOC_PERI_CFG_BASE + 0xFF0C) /* ECO 寄存器3 */
#define SOC_PERI_CFG_SC_ECO_RSV4_REG                     (SOC_PERI_CFG_BASE + 0xFF10) /* ECO 寄存器4 */
#define SOC_PERI_CFG_SC_ECO_RSV5_REG                     (SOC_PERI_CFG_BASE + 0xFF14) /* ECO 寄存器5 */
#define SOC_PERI_CFG_FPGA_VER_REG                        (SOC_PERI_CFG_BASE + 0xFFFC) /* FPGA版本寄存器 */

#endif // __PERI_CFG_REG_OFFSET_H__
