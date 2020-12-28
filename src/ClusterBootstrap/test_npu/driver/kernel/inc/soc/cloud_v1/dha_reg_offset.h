// ****************************************************************************** 
// Copyright     :  Copyright (C) 2019, Hisilicon Technologies Co. Ltd.
// File name     :  dha_reg_offset.h
// Department    :  CAD Development Department
// Author        :  xxx
// Version       :  1
// Date          :  2013/3/10
// Description   :  The description of xxx project
// Others        :  Generated automatically by nManager V4.2 
// History       :  xxx 2019/06/18 22:47:19 Create file
// ******************************************************************************

#ifndef __DHA_REG_OFFSET_H__
#define __DHA_REG_OFFSET_H__

/* DHA Base address of Module's Register */
#define SOC_DHA_BASE                       (0x1000)

/******************************************************************************/
/*                      SOC DHA Registers' Definitions                            */
/******************************************************************************/

#define SOC_DHA_DHA_CTRL_REG             (SOC_DHA_BASE + 0x0)    /* DHA控制寄存器(静态配置) */
#define SOC_DHA_DHA_CC_CTRL_REG          (SOC_DHA_BASE + 0x4)    /* DHA一致性控制寄存器(静态配置) */
#define SOC_DHA_DHA_INIT_CTRL_REG        (SOC_DHA_BASE + 0x8)    /* DHA初始化控制寄存器 */
#define SOC_DHA_DHA_DIR_CTRL_REG         (SOC_DHA_BASE + 0xC)    /* DHA Dir控制寄存器(静态配置) */
#define SOC_DHA_DHA_FUNC_DIS_REG         (SOC_DHA_BASE + 0x10)   /* DHA功能关闭寄存器(静态配置) */
#define SOC_DHA_DHA_L2_CTRL_REG          (SOC_DHA_BASE + 0x14)   /* L2控制寄存器（静态配置） */
#define SOC_DHA_DHA_L2_FLUSH_CTRL_REG    (SOC_DHA_BASE + 0x18)   /* L2 FLUSH控制寄存器 */
#define SOC_DHA_DHA_L2_FLUSH_REG         (SOC_DHA_BASE + 0x1C)   /* L2 FLUSH状态寄存器（静态配置） */
#define SOC_DHA_DHA_TOTEM_NUM_REG        (SOC_DHA_BASE + 0x20)   /* 系统Totem结构配置寄存器(静态配置) */
#define SOC_DHA_DHA_CANUML_REG           (SOC_DHA_BASE + 0x24)   /* DHA CA结构配置寄存器低位(静态配置) */
#define SOC_DHA_DHA_CANUMH_REG           (SOC_DHA_BASE + 0x28)   /* DHA CA结构配置寄存器高位(静态配置) */
#define SOC_DHA_DHA_INTLV_REG            (SOC_DHA_BASE + 0x2C)   /* DHA内地址交织配置寄存器（安全属性寄存器） */
#define SOC_DHA_DHA_CBUF_SCH_TH_REG      (SOC_DHA_BASE + 0x30)   /* DHA内CBUF的调度参考水线值配置寄存器 */
#define SOC_DHA_DHA_CBUF_RETRY_TH_REG    (SOC_DHA_BASE + 0x34)   /* DHA内CBUF的retry水线值配置寄存器 */
#define SOC_DHA_DHA_RXREQ_SCHCOEF_REG    (SOC_DHA_BASE + 0x38)   /* DHA内CBUF调度和IQ发Pgnt防饿死系数配置寄存器 */
#define SOC_DHA_DHA_DMA_CTRL_REG         (SOC_DHA_BASE + 0x60)   /* DMA控制寄存器 */
#define SOC_DHA_DHA_DMA_THRES_REG        (SOC_DHA_BASE + 0x64)   /* DHA DMA水线配置寄存器 */
#define SOC_DHA_DHA_DMABW_CTRL0_REG      (SOC_DHA_BASE + 0x68)   /* SDMA第一级带宽限制配置寄存器 */
#define SOC_DHA_DHA_DMABW_CTRL1_REG      (SOC_DHA_BASE + 0x6C)   /* SDMA第二级带宽限制配置寄存器 */
#define SOC_DHA_DHA_RSPCOMB_PORT0_REG    (SOC_DHA_BASE + 0x80)   /* 系统支持的CCIX Port0配置(标记dbidResp和compResp需要合并为compdbidResp返回的请求发起者,主要是DVPP) */
#define SOC_DHA_DHA_RSPCOMB_PORT1_REG    (SOC_DHA_BASE + 0x84)   /* 系统支持的CCIX Port1配置(静态配置) */
#define SOC_DHA_DHA_CACHE_MODE_REG       (SOC_DHA_BASE + 0x88)   /* L3 cache模式配置寄存器(静态配置) */
#define SOC_DHA_DHA_DDRINTLV_ADDR_REG    (SOC_DHA_BASE + 0x8C)   /* 各DDR交织模式选取的地址段配置（安全属性静态配置寄存器） */
#define SOC_DHA_DHA_SRAMINTLV_REG        (SOC_DHA_BASE + 0x98)   /* L3D作为SRAM使用时的交织控制寄存器。（安全属性寄存器） */
#define SOC_DHA_DHA_DAW_CTRL_REG         (SOC_DHA_BASE + 0xC0)   /* DAW配置控制寄存器。（安全属性寄存器） */
#define SOC_DHA_DHA_DAW_EN_REG           (SOC_DHA_BASE + 0xD0)   /* DAW窗口使能寄存器。（安全属性寄存器） */
#define SOC_DHA_DHA_DAW0_ADDR_REG        (SOC_DHA_BASE + 0x100)  /* 一级DAW0起始地址配置。（安全属性寄存器） */
#define SOC_DHA_DHA_DAW0_SIZE_ID_REG     (SOC_DHA_BASE + 0x104)  /* 一级DAW0窗口大小和目的配置。（安全属性寄存器） */
#define SOC_DHA_DHA_DAW1_ADDR_REG        (SOC_DHA_BASE + 0x110)  /* 一级DAW1起始地址配置。（安全属性寄存器） */
#define SOC_DHA_DHA_DAW1_SIZE_ID_REG     (SOC_DHA_BASE + 0x114)  /* 一级DAW1窗口大小和目的配置。（安全属性寄存器） */
#define SOC_DHA_DHA_DAW2_ADDR_REG        (SOC_DHA_BASE + 0x120)  /* 一级DAW2起始地址配置。（安全属性寄存器） */
#define SOC_DHA_DHA_DAW2_SIZE_ID_REG     (SOC_DHA_BASE + 0x124)  /* 一级DAW2窗口大小和目的配置。（安全属性寄存器） */
#define SOC_DHA_DHA_DAW3_ADDR_REG        (SOC_DHA_BASE + 0x130)  /* 一级DAW3起始地址配置。（安全属性寄存器） */
#define SOC_DHA_DHA_DAW3_SIZE_ID_REG     (SOC_DHA_BASE + 0x134)  /* 一级DAW3窗口大小和目的配置寄存器。（安全属性寄存器） */
#define SOC_DHA_DHA_DAW4_ADDR_REG        (SOC_DHA_BASE + 0x140)  /* 一级DAW4起始地址配置寄存器。（安全属性寄存器） */
#define SOC_DHA_DHA_DAW4_SIZE_ID_REG     (SOC_DHA_BASE + 0x144)  /* 一级DAW4窗口大小和目的配置寄存器。（安全属性寄存器） */
#define SOC_DHA_DHA_DAW5_ADDR_REG        (SOC_DHA_BASE + 0x150)  /* 一级DAW5起始地址配置寄存器。（安全属性寄存器） */
#define SOC_DHA_DHA_DAW5_SIZE_ID_REG     (SOC_DHA_BASE + 0x154)  /* 一级DAW5窗口大小和目的配置寄存器。（安全属性寄存器） */
#define SOC_DHA_DHA_DAW6_ADDR_REG        (SOC_DHA_BASE + 0x160)  /* 一级DAW6起始地址配置寄存器。（安全属性寄存器） */
#define SOC_DHA_DHA_DAW6_SIZE_ID_REG     (SOC_DHA_BASE + 0x164)  /* 一级DAW6窗口大小和目的配置寄存器。（安全属性寄存器） */
#define SOC_DHA_DHA_DAW7_ADDR_REG        (SOC_DHA_BASE + 0x170)  /* 一级DAW7起始地址配置寄存器。（安全属性寄存器） */
#define SOC_DHA_DHA_DAW7_SIZE_ID_REG     (SOC_DHA_BASE + 0x174)  /* 一级DAW7窗口大小和目的配置寄存器。（安全属性寄存器） */
#define SOC_DHA_DHA_DAW8_ADDR_REG        (SOC_DHA_BASE + 0x180)  /* 一级DAW8起始地址配置寄存器。（安全属性寄存器） */
#define SOC_DHA_DHA_DAW8_SIZE_ID_REG     (SOC_DHA_BASE + 0x184)  /* 一级DAW8窗口大小和目的配置寄存器。（安全属性寄存器） */
#define SOC_DHA_DHA_DAW9_ADDR_REG        (SOC_DHA_BASE + 0x190)  /* 一级DAW9起始地址配置寄存器。（安全属性寄存器） */
#define SOC_DHA_DHA_DAW9_SIZE_ID_REG     (SOC_DHA_BASE + 0x194)  /* 一级DAW9窗口大小和目的配置寄存器。（安全属性寄存器） */
#define SOC_DHA_DHA_DAW10_ADDR_REG       (SOC_DHA_BASE + 0x1A0)  /* 一级DAW10起始地址配置寄存器。（安全属性寄存器） */
#define SOC_DHA_DHA_DAW10_SIZE_ID_REG    (SOC_DHA_BASE + 0x1A4)  /* 一级DAW10窗口大小和目的配置寄存器。（安全属性寄存器） */
#define SOC_DHA_DHA_DAW11_ADDR_REG       (SOC_DHA_BASE + 0x1B0)  /* 一级DAW11起始地址配置寄存器。（安全属性寄存器） */
#define SOC_DHA_DHA_DAW11_SIZE_ID_REG    (SOC_DHA_BASE + 0x1B4)  /* 一级DAW11窗口大小和目的配置寄存器。（安全属性寄存器） */
#define SOC_DHA_DHA_DAW12_ADDR_REG       (SOC_DHA_BASE + 0x1C0)  /* 一级DAW12起始地址配置寄存器。（安全属性寄存器） */
#define SOC_DHA_DHA_DAW12_SIZE_ID_REG    (SOC_DHA_BASE + 0x1C4)  /* 一级DAW12窗口大小和目的配置寄存器。（安全属性寄存器） */
#define SOC_DHA_DHA_DAW13_ADDR_REG       (SOC_DHA_BASE + 0x1D0)  /* 一级DAW13起始地址配置寄存器。（安全属性寄存器） */
#define SOC_DHA_DHA_DAW13_SIZE_ID_REG    (SOC_DHA_BASE + 0x1D4)  /* 一级DAW13窗口大小和目的配置寄存器。（安全属性寄存器） */
#define SOC_DHA_DHA_DAW14_ADDR_REG       (SOC_DHA_BASE + 0x1E0)  /* 一级DAW14起始地址配置寄存器。（安全属性寄存器） */
#define SOC_DHA_DHA_DAW14_SIZE_ID_REG    (SOC_DHA_BASE + 0x1E4)  /* 一级DAW14窗口大小和目的配置寄存器。（安全属性寄存器） */
#define SOC_DHA_DHA_DAW15_ADDR_REG       (SOC_DHA_BASE + 0x1F0)  /* 一级DAW15起始地址配置寄存器。（安全属性寄存器） */
#define SOC_DHA_DHA_DAW15_SIZE_ID_REG    (SOC_DHA_BASE + 0x1F4)  /* 一级DAW15窗口大小和目的配置寄存器。（安全属性寄存器） */
#define SOC_DHA_DHA_DAW16_ADDR_REG       (SOC_DHA_BASE + 0x200)  /* 一级DAW16起始地址配置寄存器。（安全属性寄存器） */
#define SOC_DHA_DHA_DAW16_SIZE_ID_REG    (SOC_DHA_BASE + 0x204)  /* 一级DAW16窗口大小和目的配置寄存器。（安全属性寄存器） */
#define SOC_DHA_DHA_DAW17_ADDR_REG       (SOC_DHA_BASE + 0x210)  /* 一级DAW17起始地址配置寄存器。（安全属性寄存器） */
#define SOC_DHA_DHA_DAW17_SIZE_ID_REG    (SOC_DHA_BASE + 0x214)  /* 一级DAW17窗口大小和目的配置寄存器。（安全属性寄存器） */
#define SOC_DHA_DHA_DAW18_ADDR_REG       (SOC_DHA_BASE + 0x220)  /* 一级DAW18起始地址配置寄存器。（安全属性寄存器） */
#define SOC_DHA_DHA_DAW18_SIZE_ID_REG    (SOC_DHA_BASE + 0x224)  /* 一级DAW18窗口大小和目的配置寄存器。（安全属性寄存器） */
#define SOC_DHA_DHA_DAW19_ADDR_REG       (SOC_DHA_BASE + 0x230)  /* 一级DAW19起始地址配置寄存器。（安全属性寄存器） */
#define SOC_DHA_DHA_DAW19_SIZE_ID_REG    (SOC_DHA_BASE + 0x234)  /* 一级DAW19窗口大小和目的配置寄存器。（安全属性寄存器） */
#define SOC_DHA_DHA_DAW20_ADDR_REG       (SOC_DHA_BASE + 0x240)  /* 一级DAW20起始地址配置寄存器。（安全属性寄存器） */
#define SOC_DHA_DHA_DAW20_SIZE_ID_REG    (SOC_DHA_BASE + 0x244)  /* 一级DAW20窗口大小和目的配置寄存器。（安全属性寄存器） */
#define SOC_DHA_DHA_DAW21_ADDR_REG       (SOC_DHA_BASE + 0x250)  /* 一级DAW21起始地址配置寄存器。（安全属性寄存器） */
#define SOC_DHA_DHA_DAW21_SIZE_ID_REG    (SOC_DHA_BASE + 0x254)  /* 一级DAW21窗口大小和目的配置寄存器。（安全属性寄存器） */
#define SOC_DHA_DHA_DAW22_ADDR_REG       (SOC_DHA_BASE + 0x260)  /* 一级DAW22起始地址配置寄存器。（安全属性寄存器） */
#define SOC_DHA_DHA_DAW22_SIZE_ID_REG    (SOC_DHA_BASE + 0x264)  /* 一级DAW22窗口大小和目的配置寄存器。（安全属性寄存器） */
#define SOC_DHA_DHA_DAW23_ADDR_REG       (SOC_DHA_BASE + 0x270)  /* 一级DAW23起始地址配置寄存器。（安全属性寄存器） */
#define SOC_DHA_DHA_DAW23_SIZE_ID_REG    (SOC_DHA_BASE + 0x274)  /* 一级DAW23窗口大小和目的配置寄存器。（安全属性寄存器） */
#define SOC_DHA_DHA_DAW24_ADDR_REG       (SOC_DHA_BASE + 0x280)  /* 一级DAW24起始地址配置寄存器。（安全属性寄存器） */
#define SOC_DHA_DHA_DAW24_SIZE_ID_REG    (SOC_DHA_BASE + 0x284)  /* 一级DAW24窗口大小和目的配置寄存器。（安全属性寄存器） */
#define SOC_DHA_DHA_DAW25_ADDR_REG       (SOC_DHA_BASE + 0x290)  /* 一级DAW25起始地址配置寄存器。（安全属性寄存器） */
#define SOC_DHA_DHA_DAW25_SIZE_ID_REG    (SOC_DHA_BASE + 0x294)  /* 一级DAW25窗口大小和目的配置寄存器。（安全属性寄存器） */
#define SOC_DHA_DHA_DAW26_ADDR_REG       (SOC_DHA_BASE + 0x2A0)  /* 一级DAW26起始地址配置寄存器。（安全属性寄存器） */
#define SOC_DHA_DHA_DAW26_SIZE_ID_REG    (SOC_DHA_BASE + 0x2A4)  /* 一级DAW26窗口大小和目的配置寄存器。（安全属性寄存器） */
#define SOC_DHA_DHA_DAW27_ADDR_REG       (SOC_DHA_BASE + 0x2B0)  /* 一级DAW27起始地址配置寄存器。（安全属性寄存器） */
#define SOC_DHA_DHA_DAW27_SIZE_ID_REG    (SOC_DHA_BASE + 0x2B4)  /* 一级DAW27窗口大小和目的配置寄存器。（安全属性寄存器） */
#define SOC_DHA_DHA_DAW28_ADDR_REG       (SOC_DHA_BASE + 0x2C0)  /* 一级DAW28起始地址配置寄存器。（安全属性寄存器） */
#define SOC_DHA_DHA_DAW28_SIZE_ID_REG    (SOC_DHA_BASE + 0x2C4)  /* 一级DAW28窗口大小和目的配置寄存器。（安全属性寄存器） */
#define SOC_DHA_DHA_DAW29_ADDR_REG       (SOC_DHA_BASE + 0x2D0)  /* 一级DAW29起始地址配置寄存器。（安全属性寄存器） */
#define SOC_DHA_DHA_DAW29_SIZE_ID_REG    (SOC_DHA_BASE + 0x2D4)  /* 一级DAW29窗口大小和目的配置寄存器。（安全属性寄存器） */
#define SOC_DHA_DHA_DAW30_ADDR_REG       (SOC_DHA_BASE + 0x2E0)  /* 一级DAW30起始地址配置寄存器。（安全属性寄存器） */
#define SOC_DHA_DHA_DAW30_SIZE_ID_REG    (SOC_DHA_BASE + 0x2E4)  /* 一级DAW30窗口大小和目的配置寄存器。（安全属性寄存器） */
#define SOC_DHA_DHA_DAW31_ADDR_REG       (SOC_DHA_BASE + 0x2F0)  /* 一级DAW31起始地址配置寄存器。（安全属性寄存器） */
#define SOC_DHA_DHA_DAW31_SIZE_ID_REG    (SOC_DHA_BASE + 0x2F4)  /* 一级DAW31窗口大小和目的配置寄存器。（安全属性寄存器） */
#define SOC_DHA_DHA_SEC_CTRL_REG         (SOC_DHA_BASE + 0x300)  /* DHA安全控制寄存器地址交织配置寄存器（安全属性寄存器） */
#define SOC_DHA_DHA_SEC_ADDR0_REG        (SOC_DHA_BASE + 0x310)  /* DHA安全空间地址寄存器地址交织配置寄存器（安全属性寄存器） */
#define SOC_DHA_DHA_SEC_ADDR1_REG        (SOC_DHA_BASE + 0x314)  /* DHA安全空间地址寄存器地址交织配置寄存器（安全属性寄存器） */
#define SOC_DHA_DHA_SEC_ADDR2_REG        (SOC_DHA_BASE + 0x318)  /* DHA安全空间地址寄存器地址交织配置寄存器（安全属性寄存器） */
#define SOC_DHA_DHA_SEC_ADDR3_REG        (SOC_DHA_BASE + 0x31C)  /* DHA安全空间地址寄存器地址交织配置寄存器（安全属性寄存器） */
#define SOC_DHA_DHA_SEC_ADDR4_REG        (SOC_DHA_BASE + 0x320)  /* DHA安全空间地址寄存器地址交织配置寄存器（安全属性寄存器） */
#define SOC_DHA_DHA_SEC_ADDR5_REG        (SOC_DHA_BASE + 0x324)  /* DHA安全空间地址寄存器地址交织配置寄存器（安全属性寄存器） */
#define SOC_DHA_DHA_SEC_ADDR6_REG        (SOC_DHA_BASE + 0x328)  /* DHA安全空间地址寄存器地址交织配置寄存器（安全属性寄存器） */
#define SOC_DHA_DHA_SEC_ADDR7_REG        (SOC_DHA_BASE + 0x32C)  /* DHA安全空间地址寄存器地址交织配置寄存器（安全属性寄存器） */
#define SOC_DHA_DHA_SEC_SIZE0_REG        (SOC_DHA_BASE + 0x330)  /* DHA安全空间范围寄存器地址交织配置寄存器（安全属性寄存器） */
#define SOC_DHA_DHA_SEC_SIZE1_REG        (SOC_DHA_BASE + 0x334)  /* DHA安全空间范围寄存器地址交织配置寄存器（安全属性寄存器） */
#define SOC_DHA_DHA_SEC_SIZE2_REG        (SOC_DHA_BASE + 0x338)  /* DHA安全空间范围寄存器地址交织配置寄存器（安全属性寄存器） */
#define SOC_DHA_DHA_SEC_SIZE3_REG        (SOC_DHA_BASE + 0x33C)  /* DHA安全空间范围寄存器地址交织配置寄存器（安全属性寄存器） */
#define SOC_DHA_DHA_SEC_SIZE4_REG        (SOC_DHA_BASE + 0x340)  /* DHA安全空间范围寄存器地址交织配置寄存器（安全属性寄存器） */
#define SOC_DHA_DHA_SEC_SIZE5_REG        (SOC_DHA_BASE + 0x344)  /* DHA安全空间范围寄存器地址交织配置寄存器（安全属性寄存器） */
#define SOC_DHA_DHA_SEC_SIZE6_REG        (SOC_DHA_BASE + 0x348)  /* DHA安全空间范围寄存器地址交织配置寄存器（安全属性寄存器） */
#define SOC_DHA_DHA_SEC_SIZE7_REG        (SOC_DHA_BASE + 0x34C)  /* DHA安全空间范围寄存器地址交织配置寄存器（安全属性寄存器） */
#define SOC_DHA_DHA_DDR_LEVEL_REG        (SOC_DHA_BASE + 0x350)  /* DHA中DDR相关水线配置寄存器(静态配置) */
#define SOC_DHA_DHA_INTCMD_NUM_REG       (SOC_DHA_BASE + 0x354)  /* DHA CMD buffer接收片内请求数量配置寄存器(静态配置) */
#define SOC_DHA_DHA_EXTSNP_TH_REG        (SOC_DHA_BASE + 0x358)  /* DHA发出的片外snoop outstanding配置寄存器(静态配置) */
#define SOC_DHA_DHA_SPILL_TH_REG         (SOC_DHA_BASE + 0x35C)  /* DHA Spill outstanding配置寄存器(静态配置) */
#define SOC_DHA_DHA_ALLOC_TH_REG         (SOC_DHA_BASE + 0x360)  /* DHA目录ALLOC水线配置寄存器(静态配置) */
#define SOC_DHA_DHA_DDR_BW_REG           (SOC_DHA_BASE + 0x364)  /* DHA DDR带宽控制寄存器 */
#define SOC_DHA_DHA_CMDSCH_TH_REG        (SOC_DHA_BASE + 0x368)  /* DHA命令调度水线配置寄存器 */
#define SOC_DHA_DHA_ATOMIC_TH_REG        (SOC_DHA_BASE + 0x36C)  /* DHA Atomic monitor水线配置寄存器 */
#define SOC_DHA_DHA_COMSPEC_TH_REG       (SOC_DHA_BASE + 0x370)  /* DHA内存数据压缩投机读取DDR水线配置 */
#define SOC_DHA_DHA_DMCLBW_TH_REG        (SOC_DHA_BASE + 0x374)  /* DHA低带宽水线配置 */
#define SOC_DHA_DHA_MSD0_CTRL_REG        (SOC_DHA_BASE + 0x380)  /* MSD0配置寄存器(安全属性静态配置寄存器)。 */
#define SOC_DHA_DHA_MSD1_CTRL_REG        (SOC_DHA_BASE + 0x384)  /* MSD1配置寄存器(安全属性静态配置寄存器)。 */
#define SOC_DHA_DHA_MSD2_CTRL_REG        (SOC_DHA_BASE + 0x388)  /* MSD2配置寄存器(安全属性静态配置寄存器)。 */
#define SOC_DHA_DHA_MSD3_CTRL_REG        (SOC_DHA_BASE + 0x38C)  /* MSD3配置寄存器(安全属性静态配置寄存器)。 */
#define SOC_DHA_DHA_MSD4_CTRL_REG        (SOC_DHA_BASE + 0x390)  /* MSD4配置寄存器(安全属性静态配置寄存器)。 */
#define SOC_DHA_DHA_MSD5_CTRL_REG        (SOC_DHA_BASE + 0x394)  /* MSD5配置寄存器(安全属性静态配置寄存器)。 */
#define SOC_DHA_DHA_MSD6_CTRL_REG        (SOC_DHA_BASE + 0x398)  /* MSD6配置寄存器(安全属性静态配置寄存器)。 */
#define SOC_DHA_DHA_MSD7_CTRL_REG        (SOC_DHA_BASE + 0x39C)  /* MSD7配置寄存器(安全属性静态配置寄存器)。 */
#define SOC_DHA_DHA_MSD0_MAP_REG         (SOC_DHA_BASE + 0x3A0)  /* MSD0地址段重映射寄存器(安全属性静态配置寄存器)。 */
#define SOC_DHA_DHA_MSD1_MAP_REG         (SOC_DHA_BASE + 0x3A4)  /* MSD1地址段重映射寄存器(安全属性静态配置寄存器)。 */
#define SOC_DHA_DHA_MSD2_MAP_REG         (SOC_DHA_BASE + 0x3A8)  /* MSD2地址段重映射寄存器(安全属性静态配置寄存器)。 */
#define SOC_DHA_DHA_MSD3_MAP_REG         (SOC_DHA_BASE + 0x3AC)  /* MSD3地址段重映射寄存器(安全属性静态配置寄存器)。 */
#define SOC_DHA_DHA_MSD4_MAP_REG         (SOC_DHA_BASE + 0x3B0)  /* MSD4地址段重映射寄存器(安全属性静态配置寄存器)。 */
#define SOC_DHA_DHA_MSD5_MAP_REG         (SOC_DHA_BASE + 0x3B4)  /* MSD5地址段重映射寄存器(安全属性静态配置寄存器)。 */
#define SOC_DHA_DHA_MSD6_MAP_REG         (SOC_DHA_BASE + 0x3B8)  /* MSD6地址段重映射寄存器(安全属性静态配置寄存器)。 */
#define SOC_DHA_DHA_MSD7_MAP_REG         (SOC_DHA_BASE + 0x3BC)  /* MSD7地址段重映射寄存器(安全属性静态配置寄存器)。 */
#define SOC_DHA_DHA_DIR_SCAN_CTRL_REG    (SOC_DHA_BASE + 0x3C4)  /* DIR扫描配置寄存器 */
#define SOC_DHA_DHA_AICRC0_REG           (SOC_DHA_BASE + 0x3C8)  /* AICORE对应READCNT配置寄存器0 */
#define SOC_DHA_DHA_AICRC1_REG           (SOC_DHA_BASE + 0x3CC)  /* AICORE对应READCNT配置寄存器1 */
#define SOC_DHA_DHA_AICRC2_REG           (SOC_DHA_BASE + 0x3D0)  /* AICORE对应READCNT配置寄存器2 */
#define SOC_DHA_DHA_AICRC3_REG           (SOC_DHA_BASE + 0x3D4)  /* AICORE对应READCNT配置寄存器3 */
#define SOC_DHA_DHA_FLOW_MODE_REG        (SOC_DHA_BASE + 0x3D8)  /* DHA流控配置寄存器 */
#define SOC_DHA_DHA_CMD_UTL_TH0_REG      (SOC_DHA_BASE + 0x3DC)  /* CMD对应流控水线配置寄存器0 */
#define SOC_DHA_DHA_CMD_UTL_TH1_REG      (SOC_DHA_BASE + 0x3E0)  /* CMD对应流控水线配置寄存器1 */
#define SOC_DHA_DHA_IQ_UTL_TH_REG        (SOC_DHA_BASE + 0x3E4)  /* IQ对应流控水线配置寄存器 */
#define SOC_DHA_DHA_TXDAT_DET_TTIME_REG  (SOC_DHA_BASE + 0x3E8)  /* 流量检查对应时间配置寄存器 */
#define SOC_DHA_DHA_TXDAT_PUSH_TTIME_REG (SOC_DHA_BASE + 0x3EC)  /* 流量PUSH对应时间配置寄存器 */
#define SOC_DHA_DHA_DYN_CTRL_REG         (SOC_DHA_BASE + 0x400)  /* DHA动态控制寄存器 */
#define SOC_DHA_DHA_ECC_INJECT_REG       (SOC_DHA_BASE + 0x500)  /* DHA ECC错误注入寄存器 */
#define SOC_DHA_DHA_MAGIC_WORD_REG       (SOC_DHA_BASE + 0x7F0)  /* DHA版本修改寄存器 */
#define SOC_DHA_DHA_PERF_INTR_REG        (SOC_DHA_BASE + 0x800)  /* 统计事件溢出原始中断状态寄存器 */
#define SOC_DHA_DHA_PERF_INTM_REG        (SOC_DHA_BASE + 0x804)  /* 统计事件溢出中断屏蔽寄存器 */
#define SOC_DHA_DHA_PERF_INTS_REG        (SOC_DHA_BASE + 0x808)  /* 统计事件溢出中断状态寄存器 */
#define SOC_DHA_DHA_PERF_INTC_REG        (SOC_DHA_BASE + 0x80C)  /* 统计事件溢出中断清除寄存器 */
#define SOC_DHA_DHA_RSVD0_REG            (SOC_DHA_BASE + 0xC00)  /* 保留寄存器0 */
#define SOC_DHA_DHA_RSVD1_REG            (SOC_DHA_BASE + 0xC04)  /* 保留寄存器1 */
#define SOC_DHA_DHA_RSVD2_REG            (SOC_DHA_BASE + 0xC08)  /* 保留寄存器2 */
#define SOC_DHA_DHA_RSVD3_REG            (SOC_DHA_BASE + 0xC0C)  /* 保留寄存器3 */
#define SOC_DHA_DHA_VERSION0_REG         (SOC_DHA_BASE + 0xC10)  /* DHA版本寄存器 */
#define SOC_DHA_DHA_VERSION1_REG         (SOC_DHA_BASE + 0xC14)  /* EMU/FPGA版本寄存器 */
#define SOC_DHA_DHA_STATUS0_REG          (SOC_DHA_BASE + 0x1000) /* DHA状态寄存器 */
#define SOC_DHA_DHA_STATUS1_REG          (SOC_DHA_BASE + 0x1004) /* DHA状态寄存器 */
#define SOC_DHA_DHA_OVERFLOW_REG         (SOC_DHA_BASE + 0x1008) /* DHA buffer溢出状态寄存器 */
#define SOC_DHA_DHA_STATUS_IFL_REG       (SOC_DHA_BASE + 0x100C) /* DHA Interface接口状态寄存器 */
#define SOC_DHA_DHA_STATUS_IFH_REG       (SOC_DHA_BASE + 0x1010) /* DHA Interface接口状态寄存器 */
#define SOC_DHA_DHA_PROBE_CTRL_REG       (SOC_DHA_BASE + 0x1014) /* DHA状态查询控制寄存器 */
#define SOC_DHA_DHA_PROBE_INFOL_REG      (SOC_DHA_BASE + 0x1020) /* DHA状态寄存器0 */
#define SOC_DHA_DHA_ERR_MASKL_REG        (SOC_DHA_BASE + 0x1048) /* DHA error mask寄存器 */
#define SOC_DHA_DHA_ERR_MASKH_REG        (SOC_DHA_BASE + 0x104C) /* DHA error mask寄存器 */
#define SOC_DHA_DHA_MEMCTRL_HISI_REG     (SOC_DHA_BASE + 0x1B00) /* DHA memory控制寄存器 */
#define SOC_DHA_DHA_SUBEVENT_CTRL_REG    (SOC_DHA_BASE + 0x1C00) /* DHA latency统计控制寄存器 */
#define SOC_DHA_DHA_EVENT_REGION0_REG    (SOC_DHA_BASE + 0x1C10) /* DDR统计地址段配置寄存器0 */
#define SOC_DHA_DHA_EVENT_REGION1_REG    (SOC_DHA_BASE + 0x1C14) /* DDR统计地址段配置寄存器1 */
#define SOC_DHA_DHA_TOTAL_CMDLATL_REG    (SOC_DHA_BASE + 0x1D00) /* DHA命令总延迟寄存器低位 */
#define SOC_DHA_DHA_TOTAL_CMDLATH_REG    (SOC_DHA_BASE + 0x1D04) /* DHA命令总延迟寄存器高位 */
#define SOC_DHA_DHA_TOTAL_CMDCNTL_REG    (SOC_DHA_BASE + 0x1D08) /* DHA命令总数量寄存器低位 */
#define SOC_DHA_DHA_TOTAL_CMDCNTH_REG    (SOC_DHA_BASE + 0x1D0C) /* DHA命令总数量寄存器高位 */
#define SOC_DHA_DHA_TOTAL_DDRLATL_REG    (SOC_DHA_BASE + 0x1D10) /* DHA读DDR命令总延迟寄存器低位 */
#define SOC_DHA_DHA_TOTAL_DDRLATH_REG    (SOC_DHA_BASE + 0x1D14) /* DHA读DDR命令总延迟寄存器高位 */
#define SOC_DHA_DHA_TOTAL_DDRCNTL_REG    (SOC_DHA_BASE + 0x1D18) /* DHA读DDR命令总数量寄存器低位 */
#define SOC_DHA_DHA_TOTAL_DDRCNTH_REG    (SOC_DHA_BASE + 0x1D1C) /* DHA读DDR命令总数量寄存器高位 */
#define SOC_DHA_DHA_SENTRY_CNT_REG       (SOC_DHA_BASE + 0x1D20) /* DHA single pattern有效目录entry计数器 */
#define SOC_DHA_DHA_DENTRY_CNT_REG       (SOC_DHA_BASE + 0x1D24) /* DHA double pattern有效目录entry计数器 */
#define SOC_DHA_DHA_MCA_CNT_REG          (SOC_DHA_BASE + 0x1D28) /* DHA内部MCA/ECNT模式目录pattern计数器 */
#define SOC_DHA_DHA_EST_CNT_REG          (SOC_DHA_BASE + 0x1D2C) /* DHA内部EST模式目录pattern计数器 */
#define SOC_DHA_DHA_TCA_CNT_REG          (SOC_DHA_BASE + 0x1D30) /* DHA内部TCA/ISCA模式目录pattern计数器 */
#define SOC_DHA_DHA_VCA_CNT_REG          (SOC_DHA_BASE + 0x1D34) /* DHA内部VCA/ITCA/AVEC模式目录pattern计数器 */
#define SOC_DHA_DHA_PERF_ENABLE_REG      (SOC_DHA_BASE + 0x1E00) /* DHA统计事件全局使能寄存器 */
#define SOC_DHA_DHA_EVENT_CTRL_REG       (SOC_DHA_BASE + 0x1E04) /* DHA的事件统计控制寄存器 */
#define SOC_DHA_DHA_EVENT_TYPE0_REG      (SOC_DHA_BASE + 0x1E80) /* DHA_EVENT_TYPE0为DHA的事件统计类型配置寄存器 */
#define SOC_DHA_DHA_EVENT_TYPE1_REG      (SOC_DHA_BASE + 0x1E84) /* DHA_EVENT_TYPE1为DHA的事件统计类型配置寄存器 */
#define SOC_DHA_DHA_EVENT_CNT0L_REG      (SOC_DHA_BASE + 0x1F00) /* DHA_EVENT_CNT0L为事件统计寄存器0的低位部分 */
#define SOC_DHA_DHA_EVENT_CNT0H_REG      (SOC_DHA_BASE + 0x1F04) /* DHA_EVENT_CNT0H为事件统计寄存器0的高位部分 */
#define SOC_DHA_DHA_EVENT_CNT1L_REG      (SOC_DHA_BASE + 0x1F08) /* DHA_EVENT_CNT1L为事件统计寄存器1 */
#define SOC_DHA_DHA_EVENT_CNT1H_REG      (SOC_DHA_BASE + 0x1F0C) /* DHA_EVENT_CNT1H为事件统计寄存器1的高位部分 */
#define SOC_DHA_DHA_EVENT_CNT2L_REG      (SOC_DHA_BASE + 0x1F10) /* DHA_EVENT_CNT2L为事件统计寄存器2的低位部分 */
#define SOC_DHA_DHA_EVENT_CNT2H_REG      (SOC_DHA_BASE + 0x1F14) /* DHA_EVENT_CNT2H为事件统计寄存器2的高位部分 */
#define SOC_DHA_DHA_EVENT_CNT3L_REG      (SOC_DHA_BASE + 0x1F18) /* DHA_EVENT_CNT3L为事件统计寄存器3的低位部分 */
#define SOC_DHA_DHA_EVENT_CNT3H_REG      (SOC_DHA_BASE + 0x1F1C) /* DHA_EVENT_CNT3H为事件统计寄存器3的高位部分 */
#define SOC_DHA_DHA_EVENT_CNT4L_REG      (SOC_DHA_BASE + 0x1F20) /* DHA_EVENT_CNT4为事件统计寄存器4的低位部分 */
#define SOC_DHA_DHA_EVENT_CNT4H_REG      (SOC_DHA_BASE + 0x1F24) /* DHA_EVENT_CNT4H为事件统计寄存器4的高位部分 */
#define SOC_DHA_DHA_EVENT_CNT5L_REG      (SOC_DHA_BASE + 0x1F28) /* DHA_EVENT_CNT5L为事件统计寄存器5的低位部分 */
#define SOC_DHA_DHA_EVENT_CNT5H_REG      (SOC_DHA_BASE + 0x1F2C) /* DHA_EVENT_CNT5H为事件统计寄存器5的高位部分 */
#define SOC_DHA_DHA_EVENT_CNT6L_REG      (SOC_DHA_BASE + 0x1F30) /* DHA_EVENT_CNT6L为事件统计寄存器6的低位部分 */
#define SOC_DHA_DHA_EVENT_CNT6H_REG      (SOC_DHA_BASE + 0x1F34) /* DHA_EVENT_CNT6H为事件统计寄存器6的高位部分 */
#define SOC_DHA_DHA_EVENT_CNT7L_REG      (SOC_DHA_BASE + 0x1F38) /* DHA_EVENT_CNT7为事件统计寄存器7的低位部分 */
#define SOC_DHA_DHA_EVENT_CNT7H_REG      (SOC_DHA_BASE + 0x1F3C) /* DHA_EVENT_CNT7H为事件统计寄存器7的高位部分 */
#define SOC_DHA_DHA_ERR_FRL_REG          (SOC_DHA_BASE + 0x2000) /* 错误记录特性寄存器低位 */
#define SOC_DHA_DHA_ERR_FRH_REG          (SOC_DHA_BASE + 0x2004) /* 错误记录特性寄存器高位 */
#define SOC_DHA_DHA_ERR_CTRLL_REG        (SOC_DHA_BASE + 0x2008) /* 错误记录控制寄存器低位 */
#define SOC_DHA_DHA_ERR_CTRLH_REG        (SOC_DHA_BASE + 0x200C) /* 错误记录控制寄存器高位 */
#define SOC_DHA_DHA_ERR_STATUSL_REG      (SOC_DHA_BASE + 0x2010) /* 错误记录原始状态寄存器低位 */
#define SOC_DHA_DHA_ERR_STATUSH_REG      (SOC_DHA_BASE + 0x2014) /* 错误记录原始状态寄存器高位 */
#define SOC_DHA_DHA_ERR_ADDRL_REG        (SOC_DHA_BASE + 0x2018) /* 错误记录地址寄存器低位 */
#define SOC_DHA_DHA_ERR_ADDRH_REG        (SOC_DHA_BASE + 0x201C) /* 错误记录地址寄存器高位 */
#define SOC_DHA_DHA_ERR_MISC0L_REG       (SOC_DHA_BASE + 0x2020) /* 错误记录信息寄存器低位 */
#define SOC_DHA_DHA_ERR_MISC0H_REG       (SOC_DHA_BASE + 0x2024) /* 错误记录信息寄存器高位 */
#define SOC_DHA_DHA_ERR_MISC1L_REG       (SOC_DHA_BASE + 0x2028) /* 错误记录信息寄存器低位 */
#define SOC_DHA_DHA_ERR_MISC1H_REG       (SOC_DHA_BASE + 0x202C) /* 错误记录信息寄存器高位 */
#define SOC_DHA_SC_DHA_CLK_EN_REG        (SOC_DHA_BASE + 0x410)  /* DHA时钟使能寄存器 */
#define SOC_DHA_SC_DHA_CLK_DIS_REG       (SOC_DHA_BASE + 0x414)  /* DHA时钟禁止寄存器 */
#define SOC_DHA_SC_DHA_RESET_REQ_REG     (SOC_DHA_BASE + 0xA10)  /* DHA软复位请求控制寄存器 */
#define SOC_DHA_SC_DHA_RESET_DREQ_REG    (SOC_DHA_BASE + 0xA14)  /* DHA软复位去请求控制寄存器 */
#define SOC_DHA_SC_DHA_MBIST_REQ_REG     (SOC_DHA_BASE + 0x3510) /* DHA启动MBIST请求信号 */
#define SOC_DHA_SC_DHA_CLK_ST_REG        (SOC_DHA_BASE + 0x5410) /* DHA时钟状态寄存器 */
#define SOC_DHA_SC_DHA_RESET_ST_REG      (SOC_DHA_BASE + 0x5A10) /* DHA的当前软复位状态 */

#endif // __DHA_REG_OFFSET_H__
