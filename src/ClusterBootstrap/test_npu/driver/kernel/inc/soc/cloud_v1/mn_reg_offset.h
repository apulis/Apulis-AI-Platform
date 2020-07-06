// ****************************************************************************** 
// Copyright     :  Copyright (C) 2018, Hisilicon Technologies Co. Ltd.
// File name     :  mn_reg_offset.h
// Project line  :  
// Department    :  
// Author        :  信恒超
// Version       :  1.0
// Date          :  2012/9/20
// Description   :  项目描述信息
// Others        :  Generated automatically by nManager V4.2 
// History       :  信恒超 2018/03/23 15:54:59 Create file
// ******************************************************************************

#ifndef __MN_REG_OFFSET_H__
#define __MN_REG_OFFSET_H__

/* MN Base address of Module's Register */
#define SOC_MN_BASE                       (0x33000)

/******************************************************************************/
/*                      SOC MN Registers' Definitions                            */
/******************************************************************************/

#define SOC_MN_MN_EOBAR_CFG_TARGET_H_REG   (SOC_MN_BASE + 0x0)  /* eobarrier target id 配置寄存器高位部分 */
#define SOC_MN_MN_EOBAR_CFG_TARGET_L_REG   (SOC_MN_BASE + 0x4)  /* eobarrier target id 配置寄存器低位部分 */
#define SOC_MN_MN_ECBAR_CFG_TARGET_H_REG   (SOC_MN_BASE + 0x8)  /* ecbarrier target id 配置寄存器高位部分 */
#define SOC_MN_MN_ECBAR_CFG_TARGET_L_REG   (SOC_MN_BASE + 0xC)  /* ecbarrier target id 配置寄存器低位部分 */
#define SOC_MN_MN_DVMSNP_CFG_TARGET_H_REG  (SOC_MN_BASE + 0x10) /* dvmop 发出snp的target id 配置寄存器高位部分 */
#define SOC_MN_MN_DVMSNP_CFG_TARGET_L_REG  (SOC_MN_BASE + 0x14) /* dvmop 发出snp的target id 配置寄存器低位部分 */
#define SOC_MN_MN_DVMREQ_CFG_TARGET_H_REG  (SOC_MN_BASE + 0x18) /* dvmop 发出req的target id 配置寄存器高位部分 */
#define SOC_MN_MN_DVMREQ_CFG_TARGET_L_REG  (SOC_MN_BASE + 0x1C) /* dvmop 发出req的target id 配置寄存器低位部分 */
#define SOC_MN_MN_SPECIAL_ID0_REG          (SOC_MN_BASE + 0x20) /* MN ID配置寄存器 */
#define SOC_MN_MN_SPECIAL_ID1_REG          (SOC_MN_BASE + 0x24) /* PA ID配置寄存器 */
#define SOC_MN_MN_DYNAMIC_CTRL_REG         (SOC_MN_BASE + 0x28) /* MN_DYNAMIC_CTRL为MN的动态配置控制寄存器。 */
#define SOC_MN_MN_EVENT_CNT0_REG           (SOC_MN_BASE + 0x30) /* MN_EVENT_CNT0为事件统计寄存器0。 */
#define SOC_MN_MN_EVENT_CNT1_REG           (SOC_MN_BASE + 0x34) /* MN_EVENT_CNT1为事件统计寄存器1。 */
#define SOC_MN_MN_EVENT_CNT2_REG           (SOC_MN_BASE + 0x38) /* MN_EVENT_CNT2为事件统计寄存器2。 */
#define SOC_MN_MN_EVENT_CNT3_REG           (SOC_MN_BASE + 0x3C) /* MN_EVENT_CNT3为事件统计寄存器3。 */
#define SOC_MN_MN_EVENT_CTRL_REG           (SOC_MN_BASE + 0x40) /* MN_EVENT_CTRL为MN的事件统计控制寄存器。 */
#define SOC_MN_MN_EVENT_TYPE_REG           (SOC_MN_BASE + 0x48) /* MN_EVENT_TYPE为MN的事件统计类型寄存器。 */
#define SOC_MN_MN_INTM_REG                 (SOC_MN_BASE + 0x60) /* MN_RINT为MN的中断屏蔽寄存器。 */
#define SOC_MN_MN_RINT_REG                 (SOC_MN_BASE + 0x64) /* MN_RINT为MN的原始中断状态寄存器。 */
#define SOC_MN_MN_INTS_REG                 (SOC_MN_BASE + 0x68) /* MN_INTS为MN的屏蔽后的中断状态寄存器。 */
#define SOC_MN_MN_INTC_REG                 (SOC_MN_BASE + 0x6C) /* MN_INTC为MN的中断清除寄存器。 */
#define SOC_MN_MN_RESPERR_CFG_TARGET_H_REG (SOC_MN_BASE + 0x80) /* respeer 屏蔽 id 配置寄存器高位部分 */
#define SOC_MN_MN_RESPERR_CFG_TARGET_L_REG (SOC_MN_BASE + 0x84) /* resperr 屏蔽 id 配置寄存器低位部分 */
#define SOC_MN_MN_LEFT_REQ_CTRL_REG        (SOC_MN_BASE + 0xA0) /* MN_LEFT_REQ_CTRL为MN的读buffer信息的配置使能。 */
#define SOC_MN_MN_LEFT_REQ_STAT_REG        (SOC_MN_BASE + 0xA4) /* MN_LEFT_REQ_STAT为MN的所选buffer的状态寄存器。 */
#define SOC_MN_MN_LEFT_REQ_INFO_REG        (SOC_MN_BASE + 0xA8) /* MN_LEFT_REQ_INFO为MN所选buffer的信息记录。 */
#define SOC_MN_MN_LEFT_REQ_TARGET_H_REG    (SOC_MN_BASE + 0xB0) /* 没回请求响应的id 的高位部分 */
#define SOC_MN_MN_LEFT_REQ_TARGET_L_REG    (SOC_MN_BASE + 0xB4) /* 没回请求响应的id 的低位部分 */
#define SOC_MN_MN_LEFT_SNP_TARGET_H_REG    (SOC_MN_BASE + 0xB8) /* 没回snoop响应的id 的高位部分 */
#define SOC_MN_MN_LEFT_SNP_TARGET_L_REG    (SOC_MN_BASE + 0xBC) /* 没回snoop响应的id 的低位部分 */
#define SOC_MN_MN_DEFAULT_FIRST_INFO0_REG  (SOC_MN_BASE + 0xC0) /* MN_DEFAULT_FIRST_INFO0为MN的第一个default请求的信息记录。 */
#define SOC_MN_MN_DEFAULT_FIRST_INFO1_REG  (SOC_MN_BASE + 0xC4) /* MN_DEFAULT_FIRST_INFO1为MN的第一个default请求的信息记录。 */
#define SOC_MN_MN_DEFAULT_FIRST_INFO2_REG  (SOC_MN_BASE + 0xC8) /* MN_DEFAULT_FIRST_INFO2为MN的第一个default请求的信息记录。 */
#define SOC_MN_MN_DEFAULT_LAST_INFO0_REG   (SOC_MN_BASE + 0xD0) /* MN_DEFAULT_LAST_INFO0为MN的最后一个default请求的信息记录。 */
#define SOC_MN_MN_DEFAULT_LAST_INFO1_REG   (SOC_MN_BASE + 0xD4) /* MN_DEFAULT_LAST_INFO1为MN的最后一个default请求的信息记录。 */
#define SOC_MN_MN_DEFAULT_LAST_INFO2_REG   (SOC_MN_BASE + 0xD8) /* MN_DEFAULT_LAST_INFO2为MN的最后一个default请求的信息记录。 */
#define SOC_MN_MN_STATUS_REG               (SOC_MN_BASE + 0xE0) /* MN_STATUS为MN的状态寄存器。 */

#endif // __MN_REG_OFFSET_H__
