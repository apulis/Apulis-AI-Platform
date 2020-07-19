// ****************************************************************************** 
// Copyright     :  Copyright (C) 2018, Hisilicon Technologies Co. Ltd.
// File name     :  cpu_cluster_cfg_reg_offset.h
// Project line  :  ICT
// Department    :  ICT Processor Chipset Development Dep
// Author        :  xxx
// Version       :  1.0
// Date          :  2018/6/1
// Description   :  The description of xxx project
// Others        :  Generated automatically by nManager V4.2 
// History       :  xxx 2018/08/16 12:18:43 Create file
// ******************************************************************************

#ifndef __CPU_CLUSTER_CFG_REG_OFFSET_H__
#define __CPU_CLUSTER_CFG_REG_OFFSET_H__

/* CPU_CLUSTER_CFG Base address of Module's Register */
#define CLOUD_CPU_CLUSTER_CFG_BASE                       (0x0)

/******************************************************************************/
/*                      CLOUD CPU_CLUSTER_CFG Registers' Definitions                            */
/******************************************************************************/

#define CLOUD_CPU_CLUSTER_CFG_SC_CLUS_CRG_CLK_EN_REG          (CLOUD_CPU_CLUSTER_CFG_BASE + 0x310)  /* SC_CLUS_CRG_CLK_EN表示对当前CLUSTER内CRG的CLKEN信号的配置。 */
#define CLOUD_CPU_CLUSTER_CFG_SC_CLUS_CRG_CLK_DIS_REG         (CLOUD_CPU_CLUSTER_CFG_BASE + 0x314)  /* SC_CLUS_CRG_CLK_DIS表示对当前CLUSTER内CRG的时钟禁止使能配置。 */
#define CLOUD_CPU_CLUSTER_CFG_SC_CLUS_MBIST_CLK_EN_REG        (CLOUD_CPU_CLUSTER_CFG_BASE + 0x320)  /* SC_CLUS_MBIST_CLK_EN表示对当前CLUSTER内MBIST的CLKEN信号的配置。 */
#define CLOUD_CPU_CLUSTER_CFG_SC_CLUS_MBIST_CLK_DIS_REG       (CLOUD_CPU_CLUSTER_CFG_BASE + 0x324)  /* SC_CLUS_MBIST_CLK_DIS表示对当前CLUSTER内MBIST的CLKEN信号的关闭。 */
#define CLOUD_CPU_CLUSTER_CFG_SC_L3T_CLK_EN_REG               (CLOUD_CPU_CLUSTER_CFG_BASE + 0x330)  /* L3T时钟使能寄存器 */
#define CLOUD_CPU_CLUSTER_CFG_SC_L3T_CLK_DIS_REG              (CLOUD_CPU_CLUSTER_CFG_BASE + 0x334)  /* L3T时钟禁止寄存器 */
#define CLOUD_CPU_CLUSTER_CFG_SC_CLUS_CRG_RESET_REQ_REG       (CLOUD_CPU_CLUSTER_CFG_BASE + 0xA00)  /* SC_CLUS_CRG_RESET_REQ表示对当前CLUSTER内CRG的RST_REQ信号的配置。 */
#define CLOUD_CPU_CLUSTER_CFG_SC_CLUS_CRG_RESET_DREQ_REG      (CLOUD_CPU_CLUSTER_CFG_BASE + 0xA04)  /* SC_CLUS_CRG_RESET_DEREQ表示对当前CLUSTER内CRG的RST_REQ信号的去复位。 */
#define CLOUD_CPU_CLUSTER_CFG_SC_CLUS_MBIST_RESET_REQ_REG     (CLOUD_CPU_CLUSTER_CFG_BASE + 0xA10)  /* SC_CLUS_MBIST_RESET_REQ表示对当前CLUSTER内MBIST的RST_REQ信号的配置。 */
#define CLOUD_CPU_CLUSTER_CFG_SC_CLUS_MBIST_RESET_DREQ_REG    (CLOUD_CPU_CLUSTER_CFG_BASE + 0xA14)  /* SC_CLUS_MBIST_RESET_DEREQ表示对当前CLUSTER内MBIST的RST_REQ信号的去复位。 */
#define CLOUD_CPU_CLUSTER_CFG_SC_CLUS_RAS_RESET_REQ_REG       (CLOUD_CPU_CLUSTER_CFG_BASE + 0xA20)  /* SC_CLUS_RAS_RESET_REQ表示对当前CLUSTER内MBIST的RST_REQ信号的配置。 */
#define CLOUD_CPU_CLUSTER_CFG_SC_CLUS_RAS_RESET_DREQ_REG      (CLOUD_CPU_CLUSTER_CFG_BASE + 0xA24)  /* SC_CLUS_RAS_RESET_DEREQ表示对当前CLUSTER内MBIST的RST_REQ信号的去复位。 */
#define CLOUD_CPU_CLUSTER_CFG_SC_L3T_RESET_REQ_REG            (CLOUD_CPU_CLUSTER_CFG_BASE + 0xA30)  /* L3T软复位请求信号。 */
#define CLOUD_CPU_CLUSTER_CFG_SC_L3T_RESET_DREQ_REG           (CLOUD_CPU_CLUSTER_CFG_BASE + 0xA34)  /* L3T软复位去请求信号。 */
#define CLOUD_CPU_CLUSTER_CFG_SC_BIU_RESET_REQ_REG            (CLOUD_CPU_CLUSTER_CFG_BASE + 0xA38)  /* BIU软复位请求信号。 */
#define CLOUD_CPU_CLUSTER_CFG_SC_BIU_RESET_DREQ_REG           (CLOUD_CPU_CLUSTER_CFG_BASE + 0xA3C)  /* BIU软复位去请求信号。 */
#define CLOUD_CPU_CLUSTER_CFG_SC_RING_CLOSED_PORT_REG         (CLOUD_CPU_CLUSTER_CFG_BASE + 0x2020) /* RING CLOSE PORT控制寄存器 */
#define CLOUD_CPU_CLUSTER_CFG_SC_DEBUG_ERRRSP_REG             (CLOUD_CPU_CLUSTER_CFG_BASE + 0x2064) /* TAISHAN DEBUG的ERR响应类型控制寄存器 */
#define CLOUD_CPU_CLUSTER_CFG_SC_CPU_EDBGRQ_CTRL_REG          (CLOUD_CPU_CLUSTER_CFG_BASE + 0x2100) /* 对应cluster的CPU core debug请求。 */
#define CLOUD_CPU_CLUSTER_CFG_SC_CPU_PMU_CTRL_REG             (CLOUD_CPU_CLUSTER_CFG_BASE + 0x2108) /* 对应cluster的PMU snapshot trigger request. */
#define CLOUD_CPU_CLUSTER_CFG_SC_CLUS_BIU_CLKEN_REG           (CLOUD_CPU_CLUSTER_CFG_BASE + 0x210C) /* 对应cluster的TAISHAN中BIU部分时钟使能 */
#define CLOUD_CPU_CLUSTER_CFG_SC_CPU_GICDIS_CTRL_REG          (CLOUD_CPU_CLUSTER_CFG_BASE + 0x2110) /* 关闭对应cluster的CPU core的GIC接口逻辑，同时把nIRQ，nFIQ，nVIRQ，nVFIQ信号直接路由至处理器。 */
#define CLOUD_CPU_CLUSTER_CFG_SC_CPU_CLKEN_REG                (CLOUD_CPU_CLUSTER_CFG_BASE + 0x2114) /* 对应cluster的CPU core的全局时钟使能。 */
#define CLOUD_CPU_CLUSTER_CFG_SC_CPU_L2FSHREQ_CTRL_REG        (CLOUD_CPU_CLUSTER_CFG_BASE + 0x2118) /* 对应cluster的L2 hardware flush request。 */
#define CLOUD_CPU_CLUSTER_CFG_SC_CPU_CLREXMONREQ_CTRL_REG     (CLOUD_CPU_CLUSTER_CFG_BASE + 0x211C) /* 对应cluster的clearing of the external global exclusive monitor request. */
#define CLOUD_CPU_CLUSTER_CFG_SC_CPU_SINACT_CTRL_REG          (CLOUD_CPU_CLUSTER_CFG_BASE + 0x2124) /* SC_CPU_SINACT_CTRL为对应cluster的skyros接口使能 */
#define CLOUD_CPU_CLUSTER_CFG_SC_CLUS0_CTRL_RVBA0_REG         (CLOUD_CPU_CLUSTER_CFG_BASE + 0x2128) /* SC_CLUS0_CTRL_RVBA表示AArch64下CPU0复位基地址的设置。此寄存器为安全寄存器，只有安全CPU可配 */
#define CLOUD_CPU_CLUSTER_CFG_SC_CLUS0_CTRL_RVBA1_REG         (CLOUD_CPU_CLUSTER_CFG_BASE + 0x212C) /* SC_CLUS0_CTRL_RVBA表示AArch64下CPU1复位基地址的设置。此寄存器为安全寄存器，只有安全CPU可配 */
#define CLOUD_CPU_CLUSTER_CFG_SC_CLUS0_CTRL_RVBA2_REG         (CLOUD_CPU_CLUSTER_CFG_BASE + 0x2130) /* SC_CLUS0_CTRL_RVBA表示AArch64下CPU2复位基地址的设置。此寄存器为安全寄存器，只有安全CPU可配 */
#define CLOUD_CPU_CLUSTER_CFG_SC_CLUS0_CTRL_RVBA3_REG         (CLOUD_CPU_CLUSTER_CFG_BASE + 0x2134) /* SC_CLUS0_CTRL_RVBA表示AArch64下CPU3复位基地址的设置。此寄存器为安全寄存器 */
#define CLOUD_CPU_CLUSTER_CFG_SC_PERIPHBASE_REG               (CLOUD_CPU_CLUSTER_CFG_BASE + 0x2168) /* SC_PERIPHBASE表示CPU内部GICC起始地址 */
#define CLOUD_CPU_CLUSTER_CFG_SC_CPU_DBGL1RSTDISABLE_CTRL_REG (CLOUD_CPU_CLUSTER_CFG_BASE + 0x2174) /* 对应cluster的CPU core debug请求。 */
#define CLOUD_CPU_CLUSTER_CFG_SC_DBG_AUTH_CTRL_REG            (CLOUD_CPU_CLUSTER_CFG_BASE + 0x2178)
#define CLOUD_CPU_CLUSTER_CFG_SC_CPU_L2RSTDISABLE_CTRL_REG    (CLOUD_CPU_CLUSTER_CFG_BASE + 0x2180) /* 对应cluster的CPU core 请求。 */
#define CLOUD_CPU_CLUSTER_CFG_SC_CFG_END_REG                  (CLOUD_CPU_CLUSTER_CFG_BASE + 0x2188) /* CFGEND配置寄存器 */
#define CLOUD_CPU_CLUSTER_CFG_SC_POWER_DOWN_L3T_REG           (CLOUD_CPU_CLUSTER_CFG_BASE + 0x2500) /* L3T POWER DOWN 控制寄存器 */
#define CLOUD_CPU_CLUSTER_CFG_SC_TAISHAN_CFGIPC_REG           (CLOUD_CPU_CLUSTER_CFG_BASE + 0x2510) /* TAISHAN 低功耗控制使能端口 */
#define CLOUD_CPU_CLUSTER_CFG_SC_TAISHAN0_MEM_CTRL_REG        (CLOUD_CPU_CLUSTER_CFG_BASE + 0x3000) /* TAISHAN0 MEM调速信号控制寄存器 */
#define CLOUD_CPU_CLUSTER_CFG_SC_TAISHAN1_MEM_CTRL_REG        (CLOUD_CPU_CLUSTER_CFG_BASE + 0x3010) /* TAISHAN1 MEM调速信号控制寄存器 */
#define CLOUD_CPU_CLUSTER_CFG_SC_MBIST_CPUI_FUNC_RESET_REG    (CLOUD_CPU_CLUSTER_CFG_BASE + 0x3510) /* CPU启动MBIST请求信号 */
#define CLOUD_CPU_CLUSTER_CFG_SC_RF_CUSTOMTSER_REG            (CLOUD_CPU_CLUSTER_CFG_BASE + 0x3620) /* RF_CUSTOMTSER配置寄存器 */
#define CLOUD_CPU_CLUSTER_CFG_SC_LA_CUSTOMTSER_REG            (CLOUD_CPU_CLUSTER_CFG_BASE + 0x3630) /* LA_CUSTOMTSER配置寄存器 */
#define CLOUD_CPU_CLUSTER_CFG_SC_CPM_CFG0_REG                 (CLOUD_CPU_CLUSTER_CFG_BASE + 0x3700) /* CPM的配置寄存器 */
#define CLOUD_CPU_CLUSTER_CFG_SC_CPM_CFG1_REG                 (CLOUD_CPU_CLUSTER_CFG_BASE + 0x3704) /* CPM的配置寄存器 */
#define CLOUD_CPU_CLUSTER_CFG_SC_CPM_ST0_REG                  (CLOUD_CPU_CLUSTER_CFG_BASE + 0x3708) /* CPM的状态寄存器 */
#define CLOUD_CPU_CLUSTER_CFG_SC_CPM_CFG2_REG                 (CLOUD_CPU_CLUSTER_CFG_BASE + 0x3710) /* CPM的配置寄存器 */
#define CLOUD_CPU_CLUSTER_CFG_SC_CPM_CFG3_REG                 (CLOUD_CPU_CLUSTER_CFG_BASE + 0x3714) /* CPM的配置寄存器 */
#define CLOUD_CPU_CLUSTER_CFG_SC_CPM_ST1_REG                  (CLOUD_CPU_CLUSTER_CFG_BASE + 0x3718) /* CPM的状态寄存器 */
#define CLOUD_CPU_CLUSTER_CFG_SC_CLUS_CRG_CLK_ST_REG          (CLOUD_CPU_CLUSTER_CFG_BASE + 0x5310) /* SC_CLUS_CRG_CLK_ST表示当前CLUSTER内CRG的时钟状态。 */
#define CLOUD_CPU_CLUSTER_CFG_SC_CLUS_MBIST_CLK_ST_REG        (CLOUD_CPU_CLUSTER_CFG_BASE + 0x5320) /* SC_CLUS_MBIST_CLK_ST表示对当前CLUSTER内MBIST的CLKEN信号的状态。 */
#define CLOUD_CPU_CLUSTER_CFG_SC_L3T_CLK_ST_REG               (CLOUD_CPU_CLUSTER_CFG_BASE + 0x5330) /* L3T时钟状态寄存器 */
#define CLOUD_CPU_CLUSTER_CFG_SC_CLUS_CRG_RESET_ST_REG        (CLOUD_CPU_CLUSTER_CFG_BASE + 0x5A00) /* SC_CLUS_CRG_RESET_ST表示当前CLUSTER内RESET的状态。 */
#define CLOUD_CPU_CLUSTER_CFG_SC_CLUS_MBIST_RESET_ST_REG      (CLOUD_CPU_CLUSTER_CFG_BASE + 0x5A10) /* SC_CLUS_MBIST_RESET_ST表示对当前CLUSTER内MBIST的RST_REQ信号的状态。 */
#define CLOUD_CPU_CLUSTER_CFG_SC_CLUS_RAS_RESET_ST_REG        (CLOUD_CPU_CLUSTER_CFG_BASE + 0x5A20) /* SC_CLUS_RAS_RESET_ST表示对当前CLUSTER内RAS的RST_REQ信号的状态。 */
#define CLOUD_CPU_CLUSTER_CFG_SC_L3_TRESET_ST_REG             (CLOUD_CPU_CLUSTER_CFG_BASE + 0x5A30) /* L3T的RST_REQ信号的状态。 */
#define CLOUD_CPU_CLUSTER_CFG_SC_BIU_TRESET_ST_REG            (CLOUD_CPU_CLUSTER_CFG_BASE + 0x5A38) /* BIU的RST_REQ信号的状态。 */
#define CLOUD_CPU_CLUSTER_CFG_SC_MBIST_CTRL0_TAISHAN0_REG     (CLOUD_CPU_CLUSTER_CFG_BASE + 0x5C00) /* TAISHAN0 MBSIT控制寄存器0 */
#define CLOUD_CPU_CLUSTER_CFG_SC_MBIST_CTRL1_TAISHAN0_REG     (CLOUD_CPU_CLUSTER_CFG_BASE + 0x5C30) /* TAISHAN0 MBSIT控制寄存器1 */
#define CLOUD_CPU_CLUSTER_CFG_SC_MBIST_CTRL0_TAISHAN1_REG     (CLOUD_CPU_CLUSTER_CFG_BASE + 0x5C50) /* TAISHAN1 MBSIT控制寄存器0 */
#define CLOUD_CPU_CLUSTER_CFG_SC_MBIST_CTRL1_TAISHAN1_REG     (CLOUD_CPU_CLUSTER_CFG_BASE + 0x5C80) /* TAISHAN1 MBSIT控制寄存器1 */
#define CLOUD_CPU_CLUSTER_CFG_SC_CPU_PMUSNAPSHOTACK_ST_REG    (CLOUD_CPU_CLUSTER_CFG_BASE + 0x6100) /* 对应cluster的PMU snapshot trigger acknowledge */
#define CLOUD_CPU_CLUSTER_CFG_SC_CPU_STANDBYWFE_ST_REG        (CLOUD_CPU_CLUSTER_CFG_BASE + 0x6108) /* 对应cluster的CPU core处于WFE待命模式指示。 */
#define CLOUD_CPU_CLUSTER_CFG_SC_CPU_STANDBYWFI_ST_REG        (CLOUD_CPU_CLUSTER_CFG_BASE + 0x610C) /* 对应cluster的CPU core处于WFI待命模式指示。 */
#define CLOUD_CPU_CLUSTER_CFG_SC_CPU_DBGACK_ST_REG            (CLOUD_CPU_CLUSTER_CFG_BASE + 0x6110) /* 对应cluster的CPU core debug请求响应指示。 */
#define CLOUD_CPU_CLUSTER_CFG_SC_CPU_CLREXMONACK_ST_REG       (CLOUD_CPU_CLUSTER_CFG_BASE + 0x6114) /* 对应cluster的clearing of the external global exclusive monitor acknowledge. */
#define CLOUD_CPU_CLUSTER_CFG_SC_CPU_STANDBYWFIMP_ST_REG      (CLOUD_CPU_CLUSTER_CFG_BASE + 0x6118) /* 对应cluster的MP处于WFI待命模式指示。 */
#define CLOUD_CPU_CLUSTER_CFG_SC_CPU_L2FSHDONE_ST_REG         (CLOUD_CPU_CLUSTER_CFG_BASE + 0x611C) /* 对应cluster的L2 hardware flush request指示。 */
#define CLOUD_CPU_CLUSTER_CFG_MODE_EN_PPU0_REG                (CLOUD_CPU_CLUSTER_CFG_BASE + 0x8000) /* Power Management Mode选择使能 */
#define CLOUD_CPU_CLUSTER_CFG_PWR_UP_REQ_PPU0_REG             (CLOUD_CPU_CLUSTER_CFG_BASE + 0x8004) /* 上电请求 */
#define CLOUD_CPU_CLUSTER_CFG_PWR_DN_REQ_PPU0_REG             (CLOUD_CPU_CLUSTER_CFG_BASE + 0x8008) /* 下电请求 */
#define CLOUD_CPU_CLUSTER_CFG_MTCMOS_ACK_PPU0_REG             (CLOUD_CPU_CLUSTER_CFG_BASE + 0x800C) /* MTCMOS ACK 寄存器 */
#define CLOUD_CPU_CLUSTER_CFG_ROUGH_ISO_EN_PPU0_REG           (CLOUD_CPU_CLUSTER_CFG_BASE + 0x8010) /* ISO强制配置使能寄存器 */
#define CLOUD_CPU_CLUSTER_CFG_ROUGH_ISO_DIS_PPU0_REG          (CLOUD_CPU_CLUSTER_CFG_BASE + 0x8014) /* ISO强制配置清除寄存器 */
#define CLOUD_CPU_CLUSTER_CFG_ROUGH_ISO_ST_PPU0_REG           (CLOUD_CPU_CLUSTER_CFG_BASE + 0x8018) /* ISO强制配置状态寄存器 */
#define CLOUD_CPU_CLUSTER_CFG_ROUGH_MTCMOS_EN_PPU0_REG        (CLOUD_CPU_CLUSTER_CFG_BASE + 0x8020) /* MTCMOS强制配置使能寄存器 */
#define CLOUD_CPU_CLUSTER_CFG_ROUGH_MTCMOS_DIS_PPU0_REG       (CLOUD_CPU_CLUSTER_CFG_BASE + 0x8024) /* MTCMOS强制配置清除寄存器 */
#define CLOUD_CPU_CLUSTER_CFG_ROUGH_MTCMOS_ST_PPU0_REG        (CLOUD_CPU_CLUSTER_CFG_BASE + 0x8028) /* MTCMOS强制配置状态寄存器 */
#define CLOUD_CPU_CLUSTER_CFG_ROUGH_SRST_REQ_PPU0_REG         (CLOUD_CPU_CLUSTER_CFG_BASE + 0x8030) /* 复位请求强制配置使能寄存器 */
#define CLOUD_CPU_CLUSTER_CFG_ROUGH_SRST_DREQ_PPU0_REG        (CLOUD_CPU_CLUSTER_CFG_BASE + 0x8034) /* 复位请求强制配置清除寄存器 */
#define CLOUD_CPU_CLUSTER_CFG_ROUGH_SRST_ST_PPU0_REG          (CLOUD_CPU_CLUSTER_CFG_BASE + 0x8038) /* 复位请求强制配置状态寄存器 */
#define CLOUD_CPU_CLUSTER_CFG_ROUGH_CLKEN_PPU0_REG            (CLOUD_CPU_CLUSTER_CFG_BASE + 0x8040) /* 复位请求强制配置使能寄存器 */
#define CLOUD_CPU_CLUSTER_CFG_ROUGH_CLKEN_DIS_PPU0_REG        (CLOUD_CPU_CLUSTER_CFG_BASE + 0x8044) /* 复位请求强制配置清除寄存器 */
#define CLOUD_CPU_CLUSTER_CFG_ROUGH_CLKEN_ST_PPU0_REG         (CLOUD_CPU_CLUSTER_CFG_BASE + 0x804C) /* 复位请求强制配置状态寄存器 */
#define CLOUD_CPU_CLUSTER_CFG_MTCMOS_TIME_TH_PPU0_REG         (CLOUD_CPU_CLUSTER_CFG_BASE + 0x8050) /* MTCMOS开关时间阈值寄存器 */
#define CLOUD_CPU_CLUSTER_CFG_ISO_TIME_TH_PPU0_REG            (CLOUD_CPU_CLUSTER_CFG_BASE + 0x8054) /* ISO开关时间阈值寄存器 */
#define CLOUD_CPU_CLUSTER_CFG_RST_TIME_TH_PPU0_REG            (CLOUD_CPU_CLUSTER_CFG_BASE + 0x8058) /* 复位时间阈值寄存器 */
#define CLOUD_CPU_CLUSTER_CFG_SC_PWR_PACCEPT_PPU0_REG         (CLOUD_CPU_CLUSTER_CFG_BASE + 0x8060) /* 下电反馈配置寄存器 */
#define CLOUD_CPU_CLUSTER_CFG_PWR_ST_PPU0_REG                 (CLOUD_CPU_CLUSTER_CFG_BASE + 0x8100) /* Debug寄存器 */
#define CLOUD_CPU_CLUSTER_CFG_ACT_CNT_PPU0_REG                (CLOUD_CPU_CLUSTER_CFG_BASE + 0x8104) /* Debug寄存器 */
#define CLOUD_CPU_CLUSTER_CFG_PWR_INT_PPU0_REG                (CLOUD_CPU_CLUSTER_CFG_BASE + 0x8200) /* PowerManagement中断寄存器 */
#define CLOUD_CPU_CLUSTER_CFG_PWR_INT_MASK_PPU0_REG           (CLOUD_CPU_CLUSTER_CFG_BASE + 0x8204) /* PowerManagement中断屏蔽寄存器 */
#define CLOUD_CPU_CLUSTER_CFG_PWR_INT_STATUS_PPU0_REG         (CLOUD_CPU_CLUSTER_CFG_BASE + 0x8208) /* PowerManagement中断状态寄存器 */
#define CLOUD_CPU_CLUSTER_CFG_MODE_EN_PPU1_REG                (CLOUD_CPU_CLUSTER_CFG_BASE + 0x9000) /* Power Management Mode选择使能 */
#define CLOUD_CPU_CLUSTER_CFG_PWR_UP_REQ_PPU1_REG             (CLOUD_CPU_CLUSTER_CFG_BASE + 0x9004) /* 上电请求 */
#define CLOUD_CPU_CLUSTER_CFG_PWR_DN_REQ_PPU1_REG             (CLOUD_CPU_CLUSTER_CFG_BASE + 0x9008) /* 下电请求 */
#define CLOUD_CPU_CLUSTER_CFG_MTCMOS_ACK_PPU1_REG             (CLOUD_CPU_CLUSTER_CFG_BASE + 0x900C) /* MTCMOS ACK寄存器 */
#define CLOUD_CPU_CLUSTER_CFG_ROUGH_ISO_EN_PPU1_REG           (CLOUD_CPU_CLUSTER_CFG_BASE + 0x9010) /* ISO强制配置使能寄存器 */
#define CLOUD_CPU_CLUSTER_CFG_ROUGH_ISO_DIS_PPU1_REG          (CLOUD_CPU_CLUSTER_CFG_BASE + 0x9014) /* ISO强制配置清除寄存器 */
#define CLOUD_CPU_CLUSTER_CFG_ROUGH_ISO_ST_PPU1_REG           (CLOUD_CPU_CLUSTER_CFG_BASE + 0x9018) /* ISO强制配置状态寄存器 */
#define CLOUD_CPU_CLUSTER_CFG_ROUGH_MTCMOS_EN_PPU1_REG        (CLOUD_CPU_CLUSTER_CFG_BASE + 0x9020) /* MTCMOS强制配置使能寄存器 */
#define CLOUD_CPU_CLUSTER_CFG_ROUGH_MTCMOS_DIS_PPU1_REG       (CLOUD_CPU_CLUSTER_CFG_BASE + 0x9024) /* MTCMOS强制配置清除寄存器 */
#define CLOUD_CPU_CLUSTER_CFG_ROUGH_MTCMOS_ST_PPU1_REG        (CLOUD_CPU_CLUSTER_CFG_BASE + 0x9028) /* MTCMOS强制配置状态寄存器 */
#define CLOUD_CPU_CLUSTER_CFG_ROUGH_SRST_REQ_PPU1_REG         (CLOUD_CPU_CLUSTER_CFG_BASE + 0x9030) /* 复位请求强制配置使能寄存器 */
#define CLOUD_CPU_CLUSTER_CFG_ROUGH_SRST_DREQ_PPU1_REG        (CLOUD_CPU_CLUSTER_CFG_BASE + 0x9034) /* 复位请求强制配置清除寄存器 */
#define CLOUD_CPU_CLUSTER_CFG_ROUGH_SRST_ST_PPU1_REG          (CLOUD_CPU_CLUSTER_CFG_BASE + 0x9038) /* 复位请求强制配置状态寄存器 */
#define CLOUD_CPU_CLUSTER_CFG_ROUGH_CLKEN_PPU1_REG            (CLOUD_CPU_CLUSTER_CFG_BASE + 0x9040) /* 复位请求强制配置使能寄存器 */
#define CLOUD_CPU_CLUSTER_CFG_ROUGH_CLKEN_DIS_PPU1_REG        (CLOUD_CPU_CLUSTER_CFG_BASE + 0x9044) /* 复位请求强制配置清除寄存器 */
#define CLOUD_CPU_CLUSTER_CFG_ROUGH_CLKEN_ST_PPU1_REG         (CLOUD_CPU_CLUSTER_CFG_BASE + 0x904C) /* 复位请求强制配置状态寄存器 */
#define CLOUD_CPU_CLUSTER_CFG_MTCMOS_TIME_TH_PPU1_REG         (CLOUD_CPU_CLUSTER_CFG_BASE + 0x9050) /* MTCMOS开关时间阈值寄存器 */
#define CLOUD_CPU_CLUSTER_CFG_ISO_TIME_TH_PPU1_REG            (CLOUD_CPU_CLUSTER_CFG_BASE + 0x9054) /* ISO开关时间阈值寄存器 */
#define CLOUD_CPU_CLUSTER_CFG_RST_TIME_TH_PPU1_REG            (CLOUD_CPU_CLUSTER_CFG_BASE + 0x9058) /* 复位时间阈值寄存器 */
#define CLOUD_CPU_CLUSTER_CFG_SC_PWR_PACCEPT_PPU1_REG         (CLOUD_CPU_CLUSTER_CFG_BASE + 0x9060) /* 下电反馈配置寄存器 */
#define CLOUD_CPU_CLUSTER_CFG_PWR_ST_PPU1_REG                 (CLOUD_CPU_CLUSTER_CFG_BASE + 0x9100) /* Debug寄存器 */
#define CLOUD_CPU_CLUSTER_CFG_ACT_CNT_PPU1_REG                (CLOUD_CPU_CLUSTER_CFG_BASE + 0x9104) /* Debug寄存器 */
#define CLOUD_CPU_CLUSTER_CFG_PWR_INT_PPU1_REG                (CLOUD_CPU_CLUSTER_CFG_BASE + 0x9200) /* PowerManagement中断寄存器 */
#define CLOUD_CPU_CLUSTER_CFG_PWR_INT_MASK_PPU1_REG           (CLOUD_CPU_CLUSTER_CFG_BASE + 0x9204) /* PowerManagement中断屏蔽寄存器 */
#define CLOUD_CPU_CLUSTER_CFG_PWR_INT_STATUS_PPU1_REG         (CLOUD_CPU_CLUSTER_CFG_BASE + 0x9208) /* PowerManagement中断状态寄存器 */
#define CLOUD_CPU_CLUSTER_CFG_CPU_CFG_VERSION0_REG            (CLOUD_CPU_CLUSTER_CFG_BASE + 0xE0A0) /* VERSION寄存器0 */
#define CLOUD_CPU_CLUSTER_CFG_CPU_CFG_MAGIC_WORD_REG          (CLOUD_CPU_CLUSTER_CFG_BASE + 0xE0A4) /* CPU_CFG 版本号锁定寄存器 */
#define CLOUD_CPU_CLUSTER_CFG_CPU_CFG_ECO_CFG0_REG            (CLOUD_CPU_CLUSTER_CFG_BASE + 0xE0A8) /* 用于ECO的冗余配置寄存器0 */
#define CLOUD_CPU_CLUSTER_CFG_CPU_CFG_ECO_CFG1_REG            (CLOUD_CPU_CLUSTER_CFG_BASE + 0xE0AC) /* 用于ECO的冗余配置寄存器1 */
#define CLOUD_CPU_CLUSTER_CFG_CPU_CFG_ECO_CFG2_REG            (CLOUD_CPU_CLUSTER_CFG_BASE + 0xE0B0) /* 用于ECO的冗余配置寄存器2 */
#define CLOUD_CPU_CLUSTER_CFG_CPU_CFG_ECO_CFG3_REG            (CLOUD_CPU_CLUSTER_CFG_BASE + 0xE0B4) /* 用于ECO的冗余配置寄存器3 */
#define CLOUD_CPU_CLUSTER_CFG_SC_SYSCTRL_LOCK_REG             (CLOUD_CPU_CLUSTER_CFG_BASE + 0xF100) /* 系统控制器锁定寄存器。（注意：1，此寄存器空间必须是安全访问才能成功） */
#define CLOUD_CPU_CLUSTER_CFG_SC_SYSCTRL_UNLOCK_REG           (CLOUD_CPU_CLUSTER_CFG_BASE + 0xF110) /* 系统控制器解锁寄存器（注意：1，此寄存器空间必须是安全访问才能成功）。 */
#define CLOUD_CPU_CLUSTER_CFG_SC_ECO_RSV0_REG                 (CLOUD_CPU_CLUSTER_CFG_BASE + 0xFF00) /* ECO 寄存器0 */
#define CLOUD_CPU_CLUSTER_CFG_SC_ECO_RSV1_REG                 (CLOUD_CPU_CLUSTER_CFG_BASE + 0xFF04) /* ECO 寄存器1 */
#define CLOUD_CPU_CLUSTER_CFG_SC_ECO_RSV2_REG                 (CLOUD_CPU_CLUSTER_CFG_BASE + 0xFF08) /* ECO 寄存器2 */
#define CLOUD_CPU_CLUSTER_CFG_SC_ECO_RSV3_REG                 (CLOUD_CPU_CLUSTER_CFG_BASE + 0xFF0C) /* ECO 寄存器3 */
#define CLOUD_CPU_CLUSTER_CFG_SC_ECO_RSV4_REG                 (CLOUD_CPU_CLUSTER_CFG_BASE + 0xFF10) /* ECO 寄存器4 */
#define CLOUD_CPU_CLUSTER_CFG_SC_ECO_RSV5_REG                 (CLOUD_CPU_CLUSTER_CFG_BASE + 0xFF14) /* ECO 寄存器5 */
#define CLOUD_CPU_CLUSTER_CFG_FPGA_VER_REG                    (CLOUD_CPU_CLUSTER_CFG_BASE + 0xFFFC) /* FPGA版本寄存器 */

#endif // __CPU_CLUSTER_CFG_REG_OFFSET_H__
