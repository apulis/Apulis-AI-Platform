// ****************************************************************************** 
// Copyright     :  Copyright (C) 2018, Hisilicon Technologies Co. Ltd.
// File name     :  its_cfg_reg_offset.h
// Department    :  CAD Development Department
// Author        :  xxx
// Version       :  1
// Date          :  2013/3/10
// Description   :  The description of xxx project
// Others        :  Generated automatically by nManager V4.2 
// History       :  xxx 2018/07/18 20:25:42 Create file
// ******************************************************************************

#ifndef __ITS_CFG_REG_OFFSET_H__
#define __ITS_CFG_REG_OFFSET_H__

/* ITS_CFG Base address of Module's Register */
//#define xxx_ITS_CFG_BASE                       (0x10000000)
#define xxx_ITS_CFG_BASE                       (0xA8100000)

/******************************************************************************/
/*                      xxx ITS_CFG Registers' Definitions                            */
/******************************************************************************/

#define xxx_ITS_CFG_GITS_CTLR_REG               (xxx_ITS_CFG_BASE + 0x0)     /* GITS控制寄存器 */
#define xxx_ITS_CFG_GITS_IIDR_REG               (xxx_ITS_CFG_BASE + 0x4)     /* GITS版本寄存器 */
#define xxx_ITS_CFG_GITS_TYPER_REG              (xxx_ITS_CFG_BASE + 0x8)     /* GITS配置信息寄存器 */
#define xxx_ITS_CFG_GITS_CBASER_L_REG           (xxx_ITS_CFG_BASE + 0x80)    /* GITS命令队列基地址寄存器[31:0] */
#define xxx_ITS_CFG_GITS_CBASER_H_REG           (xxx_ITS_CFG_BASE + 0x84)    /* GITS命令队列基地址寄存器[63:32] */
#define xxx_ITS_CFG_GITS_CWRITER_L_REG          (xxx_ITS_CFG_BASE + 0x88)    /* 软件写下一个命令的偏移地址寄存器[31:0] */
#define xxx_ITS_CFG_GITS_CWRITER_H_REG          (xxx_ITS_CFG_BASE + 0x8C)    /* 软件写下一个命令的偏移地址寄存器[63:32] */
#define xxx_ITS_CFG_GITS_CREADR_L_REG           (xxx_ITS_CFG_BASE + 0x90)    /* GITS读下一个命令的偏移地址寄存器[31:0] */
#define xxx_ITS_CFG_GITS_CREADR_H_REG           (xxx_ITS_CFG_BASE + 0x94)    /* GITS读下一个命令的偏移地址寄存器[63:32] */
#define xxx_ITS_CFG_GITS_BASER0_L_REG           (xxx_ITS_CFG_BASE + 0x100)   /* 外设/VCPU表基地址寄存器[31:0]。n{7,0} */
#define xxx_ITS_CFG_GITS_BASER0_H_REG           (xxx_ITS_CFG_BASE + 0x104)   /* 外设/VCPU表基地址寄存器[63:32]。n{7,0} */
#define xxx_ITS_CFG_GITS_BASER1_L_REG           (xxx_ITS_CFG_BASE + 0x108)   /* 外设/VCPU表基地址寄存器[31:0]。n{7,0} */
#define xxx_ITS_CFG_GITS_BASER1_H_REG           (xxx_ITS_CFG_BASE + 0x10C)   /* 外设/VCPU表基地址寄存器[63:32]。n{7,0} */
#define xxx_ITS_CFG_GITS_BASER2_L_REG           (xxx_ITS_CFG_BASE + 0x110)   /* 外设/VCPU表基地址寄存器[31:0]。n{7,0} */
#define xxx_ITS_CFG_GITS_BASER2_H_REG           (xxx_ITS_CFG_BASE + 0x114)   /* 外设/VCPU表基地址寄存器[63:32]。n{7,0} */
#define xxx_ITS_CFG_GITS_CIDR0_REG              (xxx_ITS_CFG_BASE + 0xFFF0)  /* GITS外设ID寄存器0 */
#define xxx_ITS_CFG_GITS_CIDR1_REG              (xxx_ITS_CFG_BASE + 0xFFF4)  /* GITS外设ID寄存器1 */
#define xxx_ITS_CFG_GITS_CIDR2_REG              (xxx_ITS_CFG_BASE + 0xFFF8)  /* GITS外设ID寄存器2 */
#define xxx_ITS_CFG_GITS_CIDR3_REG              (xxx_ITS_CFG_BASE + 0xFFFC)  /* GITS外设ID寄存器3 */
#define xxx_ITS_CFG_GITS_PIDR0_REG              (xxx_ITS_CFG_BASE + 0xFFE0)  /* GITS架构版本寄存器 */
#define xxx_ITS_CFG_GITS_PIDR1_REG              (xxx_ITS_CFG_BASE + 0xFFE4)  /* GITS架构版本寄存器 */
#define xxx_ITS_CFG_GITS_PIDR2_REG              (xxx_ITS_CFG_BASE + 0xFFE8)  /* GITS架构版本寄存器 */
#define xxx_ITS_CFG_GITS_PIDR3_REG              (xxx_ITS_CFG_BASE + 0xFFEC)  /* GITS架构版本寄存器 */
#define xxx_ITS_CFG_GITS_PIDR4_REG              (xxx_ITS_CFG_BASE + 0xFFD0)  /* GITS架构版本寄存器 */
#define xxx_ITS_CFG_GITS_PIDR5_REG              (xxx_ITS_CFG_BASE + 0xFFD4)  /* GITS架构版本寄存器 */
#define xxx_ITS_CFG_GITS_PIDR6_REG              (xxx_ITS_CFG_BASE + 0xFFD8)  /* GITS架构版本寄存器 */
#define xxx_ITS_CFG_GITS_PIDR7_REG              (xxx_ITS_CFG_BASE + 0xFFDC)  /* GITS架构版本寄存器 */
#define xxx_ITS_CFG_GITS_TRANSLATER_L_REG       (xxx_ITS_CFG_BASE + 0x10040) /* GITS中断接收寄存器 */
#define xxx_ITS_CFG_GITS_TRANSLATER_H_REG       (xxx_ITS_CFG_BASE + 0x10044) /* GITS中断接收寄存器 */
#define xxx_ITS_CFG_GITS_DFX_AXIERR_REG         (xxx_ITS_CFG_BASE + 0x20000) /* 访问总线错误指示 */
#define xxx_ITS_CFG_GITS_CFG_CNT_CLR_CE_REG     (xxx_ITS_CFG_BASE + 0x20004) /* CNT_CYC类型寄存器读清控制信号 */
#define xxx_ITS_CFG_GITS_DFX_AXIWR_CNT_REG      (xxx_ITS_CFG_BASE + 0x2000C) /* GITS总线操作计数器 */
#define xxx_ITS_CFG_GITS_DFX_SYS_ERR0_REG       (xxx_ITS_CFG_BASE + 0x20010) /* GITS系统错误状态寄存器0 */
#define xxx_ITS_CFG_GITS_DFX_SYS_ERR1_REG       (xxx_ITS_CFG_BASE + 0x20014) /* GITS系统错误状态寄存器1 */
#define xxx_ITS_CFG_GITS_DFX_SYS_ERR2_REG       (xxx_ITS_CFG_BASE + 0x20018) /* GITS系统错误状态寄存器2 */
#define xxx_ITS_CFG_GITS_DFX_ST_REG             (xxx_ITS_CFG_BASE + 0x2001C) /* DFX GITS状态机 */
#define xxx_ITS_CFG_GITS_DFX_CFIFO_ST_REG       (xxx_ITS_CFG_BASE + 0x20020) /* DFX CMDQ FIFO状态寄存器 */
#define xxx_ITS_CFG_GITS_AXIM_USER_REG          (xxx_ITS_CFG_BASE + 0x20040) /* AXIM USER控制寄存器 */
#define xxx_ITS_CFG_GITS_DFX_LAST_ID_REG        (xxx_ITS_CFG_BASE + 0x20044) /* DFX接收的中断号记录寄存器 */
#define xxx_ITS_CFG_GITS_DFX_LAST_DID_REG       (xxx_ITS_CFG_BASE + 0x20048) /* DFX GITS接收的最后的命令中的设备号 */
#define xxx_ITS_CFG_GITS_DFX_CACHE_CNT_CS_REG   (xxx_ITS_CFG_BASE + 0x2004C) /* CACHE统计类型选择寄存器 */
#define xxx_ITS_CFG_GITS_DFX_CACHE_MISS_CNT_REG (xxx_ITS_CFG_BASE + 0x20050) /* CACHE不命中统计计数器 */
#define xxx_ITS_CFG_GITS_DFX_LPI_CNT_REG        (xxx_ITS_CFG_BASE + 0x20060) /* 接收到的LPI中断统计计数器 */
#define xxx_ITS_CFG_GITS_DFX_DISCARD_CNT_REG    (xxx_ITS_CFG_BASE + 0x20064) /* 丢失的LPI中断统计计数器 */
#define xxx_ITS_CFG_GITS_DFX_LPITX_CNT_REG      (xxx_ITS_CFG_BASE + 0x20068) /* 发送LPI中断统计计数器 */
#define xxx_ITS_CFG_GITS_FUNC_EN_REG            (xxx_ITS_CFG_BASE + 0x20080) /* GITS功能使能开关 */
#define xxx_ITS_CFG_GITS_DFX_VECTOR_CS_REG      (xxx_ITS_CFG_BASE + 0x20084) /* 监控中断向量配置信号 */
#define xxx_ITS_CFG_GITS_DFX_INF0_REG           (xxx_ITS_CFG_BASE + 0x20088) /* 监控中断路由信息 */
#define xxx_ITS_CFG_GITS_DFX_INF1_REG           (xxx_ITS_CFG_BASE + 0x2008C) /* 监控中断路由信息 */
#define xxx_ITS_CFG_GITS_DFX_INF2_REG           (xxx_ITS_CFG_BASE + 0x20090) /* 监控中断路由信息 */
#define xxx_ITS_CFG_GITS_DFX_INF3_REG           (xxx_ITS_CFG_BASE + 0x20094) /* 监控中断路由信息 */
#define xxx_ITS_CFG_GITS_DFX_INF4_REG           (xxx_ITS_CFG_BASE + 0x20098) /* 监控中断路由信息 */
#define xxx_ITS_CFG_GITS_DFX_INF5_REG           (xxx_ITS_CFG_BASE + 0x2009C) /* 监控中断路由信息 */

#endif // __ITS_CFG_REG_OFFSET_H__
