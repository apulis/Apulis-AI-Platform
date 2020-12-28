// ****************************************************************************** 
// Copyright     :  Copyright (C) 2018, Hisilicon Technologies Co. Ltd.
// File name     :  cfg_disp_sub0_reg_offset.h
// Department    :  CAD Development Department
// Author        :  xxx
// Version       :  1.0
// Date          :  2013/3/10
// Description   :  The description of xxx project
// Others        :  Generated automatically by nManager V4.2 
// History       :  xxx 2018/08/18 16:31:36 Create file
// ******************************************************************************

#ifndef __CFG_DISP_SUB0_REG_OFFSET_H__
#define __CFG_DISP_SUB0_REG_OFFSET_H__

/* cfg_disp_sub0 Base address of Module's Register */
#define xxx_cfg_disp_sub0_BASE                       (0x1000)

/******************************************************************************/
/*                      xxx cfg_disp_sub0 Registers' Definitions                            */
/******************************************************************************/

#define xxx_cfg_disp_sub0_DISP_PCIE_BYPASS_REG             (xxx_cfg_disp_sub0_BASE + 0x0)    /* PCIE固定DID信息配置寄存器 */
#define xxx_cfg_disp_sub0_DISP_DEF_EP_RC0_REG              (xxx_cfg_disp_sub0_BASE + 0x4)    /* 非PBU(直连EP)Default口DID信息配置寄存器 */
#define xxx_cfg_disp_sub0_DISP_DEF_EP_RC1_REG              (xxx_cfg_disp_sub0_BASE + 0x8)    /* 非PBU(直连EP)Default口DID信息配置寄存器 */
#define xxx_cfg_disp_sub0_PA_ID_CFG_REG                    (xxx_cfg_disp_sub0_BASE + 0x10)   /* PA ID静态配置 */
#define xxx_cfg_disp_sub0_DISP_RSP_CTRL_REG                (xxx_cfg_disp_sub0_BASE + 0x14)   /* dispatch的ERR响应类型控制寄存器 */
#define xxx_cfg_disp_sub0_EWA_CTRL_REG                     (xxx_cfg_disp_sub0_BASE + 0x18)   /* EWA屏蔽控制寄存器 */
#define xxx_cfg_disp_sub0_DISP_BAR_PATH_SEL_REG            (xxx_cfg_disp_sub0_BASE + 0x1C)   /* 静态功能配置寄存器 */
#define xxx_cfg_disp_sub0_DISP_AXI0_ORDER_REG              (xxx_cfg_disp_sub0_BASE + 0x20)   /* dispatch的AXI接口0读写保序地址范围配置 */
#define xxx_cfg_disp_sub0_DISP_AXI1_ORDER_REG              (xxx_cfg_disp_sub0_BASE + 0x24)   /* dispatch的AXI接口1读写保序地址范围配置 */
#define xxx_cfg_disp_sub0_DISP_AXI2_ORDER_REG              (xxx_cfg_disp_sub0_BASE + 0x28)   /* dispatch的AXI接口2读写保序地址范围配置 */
#define xxx_cfg_disp_sub0_DISP_AXI3_ORDER_REG              (xxx_cfg_disp_sub0_BASE + 0x2C)   /* dispatch的AXI接口3读写保序地址范围配置 */
#define xxx_cfg_disp_sub0_DISP_AXI4_ORDER_REG              (xxx_cfg_disp_sub0_BASE + 0x30)   /* dispatch的AXI接口4读写保序地址范围配置 */
#define xxx_cfg_disp_sub0_DISP_AXI5_ORDER_REG              (xxx_cfg_disp_sub0_BASE + 0x34)   /* dispatch的AXI接口5读写保序地址范围配置 */
#define xxx_cfg_disp_sub0_DISP_AXI6_ORDER_REG              (xxx_cfg_disp_sub0_BASE + 0x38)   /* dispatch的AXI接口6读写保序地址范围配置 */
#define xxx_cfg_disp_sub0_DISP_AXI7_ORDER_REG              (xxx_cfg_disp_sub0_BASE + 0x3C)   /* dispatch的AXI接口7读写保序地址范围配置 */
#define xxx_cfg_disp_sub0_DISP_DAW_VF_CTRL_REG             (xxx_cfg_disp_sub0_BASE + 0xC0)   /* DAW及VF配置控制寄存器 */
#define xxx_cfg_disp_sub0_DISP_DAW_EN_REG                  (xxx_cfg_disp_sub0_BASE + 0xD0)   /* dispatch 平台设备DAW窗口使能配置 */
#define xxx_cfg_disp_sub0_DISP_RC0_BAR_DAW_EN_REG          (xxx_cfg_disp_sub0_BASE + 0xD4)   /* dispatch PCIE化设备的BAR窗口使能配置 */
#define xxx_cfg_disp_sub0_DISP_RC1_BAR_DAW_EN_REG          (xxx_cfg_disp_sub0_BASE + 0xD8)   /* dispatch PCIE化设备的BAR窗口使能配置 */
#define xxx_cfg_disp_sub0_DISP_ECAM_DAW_EN_REG             (xxx_cfg_disp_sub0_BASE + 0xDC)   /* dispatch ECAM DAW窗口使能配置 */
#define xxx_cfg_disp_sub0_DISP_VF_DAW_EN_REG               (xxx_cfg_disp_sub0_BASE + 0xE0)   /* dispatch vf控制寄存器 */
#define xxx_cfg_disp_sub0_DISP_VF_BASEADDR_REG             (xxx_cfg_disp_sub0_BASE + 0xF0)   /* VF空间基地址配置。 */
#define xxx_cfg_disp_sub0_DISP_ECAM_BASEADDR_REG           (xxx_cfg_disp_sub0_BASE + 0xF4)   /* ECAM空间基地址配置。 */
#define xxx_cfg_disp_sub0_DISP_BLOCK_CYCLE_REG             (xxx_cfg_disp_sub0_BASE + 0xF8)   /* DISP调度控制 */
#define xxx_cfg_disp_sub0_DISP_DAW0_ADDR_REG               (xxx_cfg_disp_sub0_BASE + 0x100)  /* 二级DAW0起始地址配置 */
#define xxx_cfg_disp_sub0_DISP_DAW0_SIZE_ID_REG            (xxx_cfg_disp_sub0_BASE + 0x104)  /* 二级DAW0窗口大小和目的配置 */
#define xxx_cfg_disp_sub0_DISP_DAW1_ADDR_REG               (xxx_cfg_disp_sub0_BASE + 0x110)  /* 二级DAW1起始地址配置 */
#define xxx_cfg_disp_sub0_DISP_DAW1_SIZE_ID_REG            (xxx_cfg_disp_sub0_BASE + 0x114)  /* 二级DAW1窗口大小和目的配置 */
#define xxx_cfg_disp_sub0_DISP_DAW2_ADDR_REG               (xxx_cfg_disp_sub0_BASE + 0x120)  /* 二级DAW2起始地址配置 */
#define xxx_cfg_disp_sub0_DISP_DAW2_SIZE_ID_REG            (xxx_cfg_disp_sub0_BASE + 0x124)  /* 二级DAW2窗口大小和目的配置 */
#define xxx_cfg_disp_sub0_DISP_DAW3_ADDR_REG               (xxx_cfg_disp_sub0_BASE + 0x130)  /* 二级DAW3起始地址配置 */
#define xxx_cfg_disp_sub0_DISP_DAW3_SIZE_ID_REG            (xxx_cfg_disp_sub0_BASE + 0x134)  /* 二级DAW3窗口大小和目的配置 */
#define xxx_cfg_disp_sub0_DISP_DAW4_ADDR_REG               (xxx_cfg_disp_sub0_BASE + 0x140)  /* 二级DAW4起始地址配置 */
#define xxx_cfg_disp_sub0_DISP_DAW4_SIZE_ID_REG            (xxx_cfg_disp_sub0_BASE + 0x144)  /* 二级DAW4窗口大小和目的配置 */
#define xxx_cfg_disp_sub0_DISP_DAW5_ADDR_REG               (xxx_cfg_disp_sub0_BASE + 0x150)  /* 二级DAW5起始地址配置 */
#define xxx_cfg_disp_sub0_DISP_DAW5_SIZE_ID_REG            (xxx_cfg_disp_sub0_BASE + 0x154)  /* 二级DAW5窗口大小和目的配置 */
#define xxx_cfg_disp_sub0_DISP_DAW6_ADDR_REG               (xxx_cfg_disp_sub0_BASE + 0x160)  /* 二级DAW6起始地址配置 */
#define xxx_cfg_disp_sub0_DISP_DAW6_SIZE_ID_REG            (xxx_cfg_disp_sub0_BASE + 0x164)  /* 二级DAW6窗口大小和目的配置 */
#define xxx_cfg_disp_sub0_DISP_DAW7_ADDR_REG               (xxx_cfg_disp_sub0_BASE + 0x170)  /* 二级DAW7起始地址配置 */
#define xxx_cfg_disp_sub0_DISP_DAW7_SIZE_ID_REG            (xxx_cfg_disp_sub0_BASE + 0x174)  /* 二级DAW7窗口大小和目的配置 */
#define xxx_cfg_disp_sub0_DISP_DAW8_ADDR_REG               (xxx_cfg_disp_sub0_BASE + 0x180)  /* 二级DAW8起始地址配置 */
#define xxx_cfg_disp_sub0_DISP_DAW8_SIZE_ID_REG            (xxx_cfg_disp_sub0_BASE + 0x184)  /* 二级DAW8窗口大小和目的配置 */
#define xxx_cfg_disp_sub0_DISP_DAW9_ADDR_REG               (xxx_cfg_disp_sub0_BASE + 0x190)  /* 二级DAW9起始地址配置 */
#define xxx_cfg_disp_sub0_DISP_DAW9_SIZE_ID_REG            (xxx_cfg_disp_sub0_BASE + 0x194)  /* 二级DAW9窗口大小和目的配置 */
#define xxx_cfg_disp_sub0_DISP_DAW10_ADDR_REG              (xxx_cfg_disp_sub0_BASE + 0x1A0)  /* 二级DAW10起始地址配置 */
#define xxx_cfg_disp_sub0_DISP_DAW10_SIZE_ID_REG           (xxx_cfg_disp_sub0_BASE + 0x1A4)  /* 二级DAW10窗口大小和目的配置 */
#define xxx_cfg_disp_sub0_DISP_DAW11_ADDR_REG              (xxx_cfg_disp_sub0_BASE + 0x1B0)  /* 二级DAW11起始地址配置 */
#define xxx_cfg_disp_sub0_DISP_DAW11_SIZE_ID_REG           (xxx_cfg_disp_sub0_BASE + 0x1B4)  /* 二级DAW11窗口大小和目的配置 */
#define xxx_cfg_disp_sub0_DISP_VF00_ID_REG                 (xxx_cfg_disp_sub0_BASE + 0x300)  /* VF00所属子系统的ID配置。 */
#define xxx_cfg_disp_sub0_DISP_VF01_ID_REG                 (xxx_cfg_disp_sub0_BASE + 0x304)  /* VF01所属子系统的ID配置。 */
#define xxx_cfg_disp_sub0_DISP_VF02_ID_REG                 (xxx_cfg_disp_sub0_BASE + 0x308)  /* VF02所属子系统的ID配置。 */
#define xxx_cfg_disp_sub0_DISP_VF03_ID_REG                 (xxx_cfg_disp_sub0_BASE + 0x30C)  /* VF03所属子系统的ID配置。 */
#define xxx_cfg_disp_sub0_DISP_VF04_ID_REG                 (xxx_cfg_disp_sub0_BASE + 0x310)  /* VF04所属子系统的ID配置。 */
#define xxx_cfg_disp_sub0_DISP_VF05_ID_REG                 (xxx_cfg_disp_sub0_BASE + 0x314)  /* VF05所属子系统的ID配置。 */
#define xxx_cfg_disp_sub0_DISP_VF06_ID_REG                 (xxx_cfg_disp_sub0_BASE + 0x318)  /* VF06所属子系统的ID配置。 */
#define xxx_cfg_disp_sub0_DISP_VF07_ID_REG                 (xxx_cfg_disp_sub0_BASE + 0x31C)  /* VF07所属子系统的ID配置。 */
#define xxx_cfg_disp_sub0_DISP_VF08_ID_REG                 (xxx_cfg_disp_sub0_BASE + 0x320)  /* VF08所属子系统的ID配置。 */
#define xxx_cfg_disp_sub0_DISP_VF09_ID_REG                 (xxx_cfg_disp_sub0_BASE + 0x324)  /* VF09所属子系统的ID配置。 */
#define xxx_cfg_disp_sub0_DISP_VF10_ID_REG                 (xxx_cfg_disp_sub0_BASE + 0x328)  /* VF10所属子系统的ID配置。 */
#define xxx_cfg_disp_sub0_DISP_VF11_ID_REG                 (xxx_cfg_disp_sub0_BASE + 0x32C)  /* VF11所属子系统的ID配置。 */
#define xxx_cfg_disp_sub0_DISP_VF12_ID_REG                 (xxx_cfg_disp_sub0_BASE + 0x330)  /* VF12所属子系统的ID配置。 */
#define xxx_cfg_disp_sub0_DISP_VF13_ID_REG                 (xxx_cfg_disp_sub0_BASE + 0x334)  /* VF13所属子系统的ID配置。 */
#define xxx_cfg_disp_sub0_DISP_VF14_ID_REG                 (xxx_cfg_disp_sub0_BASE + 0x338)  /* VF14所属子系统的ID配置。 */
#define xxx_cfg_disp_sub0_DISP_VF15_ID_REG                 (xxx_cfg_disp_sub0_BASE + 0x33C)  /* VF15所属子系统的ID配置。 */
#define xxx_cfg_disp_sub0_DISP_VF16_ID_REG                 (xxx_cfg_disp_sub0_BASE + 0x340)  /* VF16所属子系统的ID配置。 */
#define xxx_cfg_disp_sub0_DISP_VF17_ID_REG                 (xxx_cfg_disp_sub0_BASE + 0x344)  /* VF17所属子系统的ID配置。 */
#define xxx_cfg_disp_sub0_DISP_VF18_ID_REG                 (xxx_cfg_disp_sub0_BASE + 0x348)  /* VF18所属子系统的ID配置。 */
#define xxx_cfg_disp_sub0_DISP_VF19_ID_REG                 (xxx_cfg_disp_sub0_BASE + 0x34C)  /* VF19所属子系统的ID配置。 */
#define xxx_cfg_disp_sub0_DISP_VF20_ID_REG                 (xxx_cfg_disp_sub0_BASE + 0x350)  /* VF20所属子系统的ID配置。 */
#define xxx_cfg_disp_sub0_DISP_VF21_ID_REG                 (xxx_cfg_disp_sub0_BASE + 0x354)  /* VF21所属子系统的ID配置。 */
#define xxx_cfg_disp_sub0_DISP_VF22_ID_REG                 (xxx_cfg_disp_sub0_BASE + 0x358)  /* VF22所属子系统的ID配置。 */
#define xxx_cfg_disp_sub0_DISP_VF23_ID_REG                 (xxx_cfg_disp_sub0_BASE + 0x35C)  /* VF23所属子系统的ID配置。 */
#define xxx_cfg_disp_sub0_DISP_VF24_ID_REG                 (xxx_cfg_disp_sub0_BASE + 0x360)  /* VF24所属子系统的ID配置。 */
#define xxx_cfg_disp_sub0_DISP_VF25_ID_REG                 (xxx_cfg_disp_sub0_BASE + 0x364)  /* VF25所属子系统的ID配置。 */
#define xxx_cfg_disp_sub0_DISP_VF26_ID_REG                 (xxx_cfg_disp_sub0_BASE + 0x368)  /* VF26所属子系统的ID配置。 */
#define xxx_cfg_disp_sub0_DISP_VF27_ID_REG                 (xxx_cfg_disp_sub0_BASE + 0x36C)  /* VF27所属子系统的ID配置。 */
#define xxx_cfg_disp_sub0_DISP_VF28_ID_REG                 (xxx_cfg_disp_sub0_BASE + 0x370)  /* VF28所属子系统的ID配置。 */
#define xxx_cfg_disp_sub0_DISP_VF29_ID_REG                 (xxx_cfg_disp_sub0_BASE + 0x374)  /* VF29所属子系统的ID配置。 */
#define xxx_cfg_disp_sub0_DISP_VF30_ID_REG                 (xxx_cfg_disp_sub0_BASE + 0x378)  /* VF30所属子系统的ID配置。 */
#define xxx_cfg_disp_sub0_DISP_VF31_ID_REG                 (xxx_cfg_disp_sub0_BASE + 0x37C)  /* VF31所属子系统的ID配置。 */
#define xxx_cfg_disp_sub0_DISP_RC0_BASE_ADDR_REG           (xxx_cfg_disp_sub0_BASE + 0x400)  /* PCIE RC0 DAW起始地址配置 */
#define xxx_cfg_disp_sub0_DISP_RC0_LIMIT_ADDR_REG          (xxx_cfg_disp_sub0_BASE + 0x404)  /* PCIE RC0 DAW 结束地址配置 */
#define xxx_cfg_disp_sub0_DISP_RC1_BASE_ADDR_REG           (xxx_cfg_disp_sub0_BASE + 0x408)  /* PCIE RC1 DAW起始地址配置 */
#define xxx_cfg_disp_sub0_DISP_RC1_LIMIT_ADDR_REG          (xxx_cfg_disp_sub0_BASE + 0x40C)  /* PCIE RC1 DAW 结束地址配置 */
#define xxx_cfg_disp_sub0_DISP_RC0_BAR_DAW0_BASE_ADDR_REG  (xxx_cfg_disp_sub0_BASE + 0x410)  /* 二级PCIE化设备BAR空间DAW0窗口起始地址配置 */
#define xxx_cfg_disp_sub0_DISP_RC0_BAR_DAW0_LIMIT_ADDR_REG (xxx_cfg_disp_sub0_BASE + 0x414)  /* 二级PCIE化设备BAR空间DAW0窗口结束地址配置 */
#define xxx_cfg_disp_sub0_DISP_RC0_BAR_DAW0_DID_REG        (xxx_cfg_disp_sub0_BASE + 0x418)  /* 二级PCIE化设备BAR空间DAW0窗口ID路由配置 */
#define xxx_cfg_disp_sub0_DISP_RC0_BAR_DAW1_BASE_ADDR_REG  (xxx_cfg_disp_sub0_BASE + 0x420)  /* 二级PCIE化设备BAR空间DAW1窗口起始地址配置 */
#define xxx_cfg_disp_sub0_DISP_RC0_BAR_DAW1_LIMIT_ADDR_REG (xxx_cfg_disp_sub0_BASE + 0x424)  /* 二级PCIE化设备BAR空间DAW1窗口结束地址配置 */
#define xxx_cfg_disp_sub0_DISP_RC0_BAR_DAW1_DID_REG        (xxx_cfg_disp_sub0_BASE + 0x428)  /* 二级PCIE化设备BAR空间DAW1窗口ID路由配置 */
#define xxx_cfg_disp_sub0_DISP_RC0_BAR_DAW2_BASE_ADDR_REG  (xxx_cfg_disp_sub0_BASE + 0x430)  /* 二级PCIE化设备BAR空间DAW2窗口起始地址配置 */
#define xxx_cfg_disp_sub0_DISP_RC0_BAR_DAW2_LIMIT_ADDR_REG (xxx_cfg_disp_sub0_BASE + 0x434)  /* 二级PCIE化设备BAR空间DAW2窗口结束地址配置 */
#define xxx_cfg_disp_sub0_DISP_RC0_BAR_DAW2_DID_REG        (xxx_cfg_disp_sub0_BASE + 0x438)  /* 二级PCIE化设备BAR空间DAW2窗口ID路由配置 */
#define xxx_cfg_disp_sub0_DISP_RC0_BAR_DAW3_BASE_ADDR_REG  (xxx_cfg_disp_sub0_BASE + 0x440)  /* 二级PCIE化设备BAR空间DAW3窗口起始地址配置 */
#define xxx_cfg_disp_sub0_DISP_RC0_BAR_DAW3_LIMIT_ADDR_REG (xxx_cfg_disp_sub0_BASE + 0x444)  /* 二级PCIE化设备BAR空间DAW3窗口结束地址配置 */
#define xxx_cfg_disp_sub0_DISP_RC0_BAR_DAW3_DID_REG        (xxx_cfg_disp_sub0_BASE + 0x448)  /* 二级PCIE化设备BAR空间DAW3窗口ID路由配置 */
#define xxx_cfg_disp_sub0_DISP_RC1_BAR_DAW0_BASE_ADDR_REG  (xxx_cfg_disp_sub0_BASE + 0x480)  /* 二级PCIE化设备BAR空间DAW0窗口起始地址配置 */
#define xxx_cfg_disp_sub0_DISP_RC1_BAR_DAW0_LIMIT_ADDR_REG (xxx_cfg_disp_sub0_BASE + 0x484)  /* 二级PCIE化设备BAR空间DAW0窗口结束地址配置 */
#define xxx_cfg_disp_sub0_DISP_RC1_BAR_DAW0_DID_REG        (xxx_cfg_disp_sub0_BASE + 0x488)  /* 二级PCIE化设备BAR空间DAW0窗口ID路由配置 */
#define xxx_cfg_disp_sub0_DISP_RC1_BAR_DAW1_BASE_ADDR_REG  (xxx_cfg_disp_sub0_BASE + 0x490)  /* 二级PCIE化设备BAR空间DAW1窗口起始地址配置 */
#define xxx_cfg_disp_sub0_DISP_RC1_BAR_DAW1_LIMIT_ADDR_REG (xxx_cfg_disp_sub0_BASE + 0x494)  /* 二级PCIE化设备BAR空间DAW1窗口结束地址配置 */
#define xxx_cfg_disp_sub0_DISP_RC1_BAR_DAW1_DID_REG        (xxx_cfg_disp_sub0_BASE + 0x498)  /* 二级PCIE化设备BAR空间DAW1窗口ID路由配置 */
#define xxx_cfg_disp_sub0_DISP_RC1_BAR_DAW2_BASE_ADDR_REG  (xxx_cfg_disp_sub0_BASE + 0x4A0)  /* 二级PCIE化设备BAR空间DAW2窗口起始地址配置 */
#define xxx_cfg_disp_sub0_DISP_RC1_BAR_DAW2_LIMIT_ADDR_REG (xxx_cfg_disp_sub0_BASE + 0x4A4)  /* 二级PCIE化设备BAR空间DAW2窗口结束地址配置 */
#define xxx_cfg_disp_sub0_DISP_RC1_BAR_DAW2_DID_REG        (xxx_cfg_disp_sub0_BASE + 0x4A8)  /* 二级PCIE化设备BAR空间DAW2窗口ID路由配置 */
#define xxx_cfg_disp_sub0_DISP_RC1_BAR_DAW3_BASE_ADDR_REG  (xxx_cfg_disp_sub0_BASE + 0x4B0)  /* 二级PCIE化设备BAR空间DAW3窗口起始地址配置 */
#define xxx_cfg_disp_sub0_DISP_RC1_BAR_DAW3_LIMIT_ADDR_REG (xxx_cfg_disp_sub0_BASE + 0x4B4)  /* 二级PCIE化设备BAR空间DAW3窗口结束地址配置 */
#define xxx_cfg_disp_sub0_DISP_RC1_BAR_DAW3_DID_REG        (xxx_cfg_disp_sub0_BASE + 0x4B8)  /* 二级PCIE化设备BAR空间DAW3窗口ID路由配置 */
#define xxx_cfg_disp_sub0_DISP_ECAM_DAW0_ADDR_REG          (xxx_cfg_disp_sub0_BASE + 0x500)  /* 二级PCIE化设备ECAM空间DAW0窗口地址 */
#define xxx_cfg_disp_sub0_DISP_ECAM_DAW0_DID_REG           (xxx_cfg_disp_sub0_BASE + 0x504)  /* 二级PCIE化设备ECAM空间DAW0窗口DID配置 */
#define xxx_cfg_disp_sub0_DISP_ECAM_DAW1_ADDR_REG          (xxx_cfg_disp_sub0_BASE + 0x510)  /* 二级PCIE化设备ECAM空间DAW1窗口地址 */
#define xxx_cfg_disp_sub0_DISP_ECAM_DAW1_DID_REG           (xxx_cfg_disp_sub0_BASE + 0x514)  /* 二级PCIE化设备ECAM空间DAW1窗口DID配置 */
#define xxx_cfg_disp_sub0_DISP_ECAM_DAW2_ADDR_REG          (xxx_cfg_disp_sub0_BASE + 0x520)  /* 二级PCIE化设备ECAM空间DAW2窗口地址 */
#define xxx_cfg_disp_sub0_DISP_ECAM_DAW2_DID_REG           (xxx_cfg_disp_sub0_BASE + 0x524)  /* 二级PCIE化设备ECAM空间DAW2窗口DID配置 */
#define xxx_cfg_disp_sub0_DISP_ECAM_DAW3_ADDR_REG          (xxx_cfg_disp_sub0_BASE + 0x530)  /* 二级PCIE化设备ECAM空间DAW3窗口地址 */
#define xxx_cfg_disp_sub0_DISP_ECAM_DAW3_DID_REG           (xxx_cfg_disp_sub0_BASE + 0x534)  /* 二级PCIE化设备ECAM空间DAW3窗口DID配置 */
#define xxx_cfg_disp_sub0_DISP_ECAM_DAW4_ADDR_REG          (xxx_cfg_disp_sub0_BASE + 0x540)  /* 二级PCIE化设备ECAM空间DAW4窗口地址 */
#define xxx_cfg_disp_sub0_DISP_ECAM_DAW4_DID_REG           (xxx_cfg_disp_sub0_BASE + 0x544)  /* 二级PCIE化设备ECAM空间DAW4窗口DID配置 */
#define xxx_cfg_disp_sub0_DISP_ECAM_DAW5_ADDR_REG          (xxx_cfg_disp_sub0_BASE + 0x550)  /* 二级PCIE化设备ECAM空间DAW5窗口地址 */
#define xxx_cfg_disp_sub0_DISP_ECAM_DAW5_DID_REG           (xxx_cfg_disp_sub0_BASE + 0x554)  /* 二级PCIE化设备ECAM空间DAW5窗口DID配置 */
#define xxx_cfg_disp_sub0_DISP_ECAM_DAW6_ADDR_REG          (xxx_cfg_disp_sub0_BASE + 0x560)  /* 二级PCIE化设备ECAM空间DAW6窗口地址 */
#define xxx_cfg_disp_sub0_DISP_ECAM_DAW6_DID_REG           (xxx_cfg_disp_sub0_BASE + 0x564)  /* 二级PCIE化设备ECAM空间DAW6窗口DID配置 */
#define xxx_cfg_disp_sub0_DISP_ECAM_DAW7_ADDR_REG          (xxx_cfg_disp_sub0_BASE + 0x570)  /* 二级PCIE化设备ECAM空间DAW7窗口地址 */
#define xxx_cfg_disp_sub0_DISP_ECAM_DAW7_DID_REG           (xxx_cfg_disp_sub0_BASE + 0x574)  /* 二级PCIE化设备ECAM空间DAW7窗口DID配置 */
#define xxx_cfg_disp_sub0_DISP_OFFSET_L_ADDR0_REG          (xxx_cfg_disp_sub0_BASE + 0x600)  /* 偏置地址低20bit的配置寄存器 */
#define xxx_cfg_disp_sub0_DISP_OFFSET_L_ADDR1_REG          (xxx_cfg_disp_sub0_BASE + 0x604)  /* 偏置地址低20bit的配置寄存器 */
#define xxx_cfg_disp_sub0_DISP_OFFSET_L_ADDR2_REG          (xxx_cfg_disp_sub0_BASE + 0x608)  /* 偏置地址低20bit的配置寄存器 */
#define xxx_cfg_disp_sub0_DISP_OFFSET_L_ADDR3_REG          (xxx_cfg_disp_sub0_BASE + 0x60C)  /* 偏置地址低20bit的配置寄存器 */
#define xxx_cfg_disp_sub0_DISP_OFFSET_L_ADDR4_REG          (xxx_cfg_disp_sub0_BASE + 0x610)  /* 偏置地址低20bit的配置寄存器 */
#define xxx_cfg_disp_sub0_DISP_OFFSET_L_ADDR5_REG          (xxx_cfg_disp_sub0_BASE + 0x614)  /* 偏置地址低20bit的配置寄存器 */
#define xxx_cfg_disp_sub0_DISP_OFFSET_L_ADDR6_REG          (xxx_cfg_disp_sub0_BASE + 0x618)  /* 偏置地址低20bit的配置寄存器 */
#define xxx_cfg_disp_sub0_DISP_OFFSET_L_ADDR7_REG          (xxx_cfg_disp_sub0_BASE + 0x61C)  /* 偏置地址低20bit的配置寄存器 */
#define xxx_cfg_disp_sub0_DISP_OFFSET_H_ADDR0_REG          (xxx_cfg_disp_sub0_BASE + 0x620)  /* 偏置地址高28bit的配置寄存器 */
#define xxx_cfg_disp_sub0_DISP_OFFSET_H_ADDR1_REG          (xxx_cfg_disp_sub0_BASE + 0x624)  /* 偏置地址高28bit的配置寄存器 */
#define xxx_cfg_disp_sub0_DISP_OFFSET_H_ADDR2_REG          (xxx_cfg_disp_sub0_BASE + 0x628)  /* 偏置地址高28bit的配置寄存器 */
#define xxx_cfg_disp_sub0_DISP_OFFSET_H_ADDR3_REG          (xxx_cfg_disp_sub0_BASE + 0x62C)  /* 偏置地址高28bit的配置寄存器 */
#define xxx_cfg_disp_sub0_DISP_OFFSET_H_ADDR4_REG          (xxx_cfg_disp_sub0_BASE + 0x630)  /* 偏置地址高28bit的配置寄存器 */
#define xxx_cfg_disp_sub0_DISP_OFFSET_H_ADDR5_REG          (xxx_cfg_disp_sub0_BASE + 0x634)  /* 偏置地址高28bit的配置寄存器 */
#define xxx_cfg_disp_sub0_DISP_OFFSET_H_ADDR6_REG          (xxx_cfg_disp_sub0_BASE + 0x638)  /* 偏置地址高28bit的配置寄存器 */
#define xxx_cfg_disp_sub0_DISP_OFFSET_H_ADDR7_REG          (xxx_cfg_disp_sub0_BASE + 0x63C)  /* 偏置地址高28bit的配置寄存器 */
#define xxx_cfg_disp_sub0_DISP_MAGIC_WORD_REG              (xxx_cfg_disp_sub0_BASE + 0x7F0)  /* DISP 版本号锁定寄存器 */
#define xxx_cfg_disp_sub0_DISP_INTRAW0_REG                 (xxx_cfg_disp_sub0_BASE + 0x800)  /* dispatch的原始中断状态寄存器 */
#define xxx_cfg_disp_sub0_DISP_INTMASK0_REG                (xxx_cfg_disp_sub0_BASE + 0x804)  /* dispatch的中断屏蔽寄存器 */
#define xxx_cfg_disp_sub0_DISP_INTSTAT0_REG                (xxx_cfg_disp_sub0_BASE + 0x808)  /* dispatch的屏蔽后的中断状态寄存器 */
#define xxx_cfg_disp_sub0_DISP_INTCLR0_REG                 (xxx_cfg_disp_sub0_BASE + 0x80C)  /* dispatch的中断清除寄存器 */
#define xxx_cfg_disp_sub0_DISP_INTRAW1_REG                 (xxx_cfg_disp_sub0_BASE + 0x810)  /* dispatch的原始中断状态寄存器 */
#define xxx_cfg_disp_sub0_DISP_INTMASK1_REG                (xxx_cfg_disp_sub0_BASE + 0x814)  /* dispatch的中断屏蔽寄存器 */
#define xxx_cfg_disp_sub0_DISP_INTSTAT1_REG                (xxx_cfg_disp_sub0_BASE + 0x818)  /* dispatch的屏蔽后的中断状态寄存器 */
#define xxx_cfg_disp_sub0_DISP_INTCLR1_REG                 (xxx_cfg_disp_sub0_BASE + 0x81C)  /* dispatch的中断清除寄存器 */
#define xxx_cfg_disp_sub0_DISP_ERR_ADDR_L_REG              (xxx_cfg_disp_sub0_BASE + 0x840)  /* dispatch的错误地址状态寄存器 */
#define xxx_cfg_disp_sub0_DISP_ERR_ADDR_H_REG              (xxx_cfg_disp_sub0_BASE + 0x844)  /* dispatch的错误地址状态寄存器 */
#define xxx_cfg_disp_sub0_DISP_ERR_INFO0_REG               (xxx_cfg_disp_sub0_BASE + 0x848)  /* dispatch的错误状态寄存器0 */
#define xxx_cfg_disp_sub0_DISP_ERR_INFO1_REG               (xxx_cfg_disp_sub0_BASE + 0x84C)  /* dispatch的错误状态寄存器1 */
#define xxx_cfg_disp_sub0_DISP_ERR_ACCESS_RST_PORT_REG     (xxx_cfg_disp_sub0_BASE + 0x850)  /* dispatch访问复位的AXI端口状态寄存器 */
#define xxx_cfg_disp_sub0_DISP_PORT0_STATE_REG             (xxx_cfg_disp_sub0_BASE + 0x900)  /* dispatch端口0状态寄存器 */
#define xxx_cfg_disp_sub0_DISP_PORT1_STATE_REG             (xxx_cfg_disp_sub0_BASE + 0x904)  /* dispatch端口1状态寄存器 */
#define xxx_cfg_disp_sub0_DISP_PORT2_STATE_REG             (xxx_cfg_disp_sub0_BASE + 0x908)  /* dispatch端口2状态寄存器 */
#define xxx_cfg_disp_sub0_DISP_PORT3_STATE_REG             (xxx_cfg_disp_sub0_BASE + 0x90C)  /* dispatch端口3状态寄存器 */
#define xxx_cfg_disp_sub0_DISP_PORT4_STATE_REG             (xxx_cfg_disp_sub0_BASE + 0x910)  /* dispatch端口4状态寄存器 */
#define xxx_cfg_disp_sub0_DISP_PORT5_STATE_REG             (xxx_cfg_disp_sub0_BASE + 0x914)  /* dispatch端口5状态寄存器 */
#define xxx_cfg_disp_sub0_DISP_PORT6_STATE_REG             (xxx_cfg_disp_sub0_BASE + 0x918)  /* dispatch端口6状态寄存器 */
#define xxx_cfg_disp_sub0_DISP_PORT7_STATE_REG             (xxx_cfg_disp_sub0_BASE + 0x91C)  /* dispatch端口7状态寄存器 */
#define xxx_cfg_disp_sub0_DISP_DEF_PORT_STATE_REG          (xxx_cfg_disp_sub0_BASE + 0x920)  /* dispatch Default Slave端口状态寄存器 */
#define xxx_cfg_disp_sub0_DISP_BARRIER_ST_REG              (xxx_cfg_disp_sub0_BASE + 0xA00)  /* dispatch Barrier统计状态寄存器 */
#define xxx_cfg_disp_sub0_DISP_ECO_CFG0_REG                (xxx_cfg_disp_sub0_BASE + 0xC00)  /* 用于ECO的冗余配置寄存器0 */
#define xxx_cfg_disp_sub0_DISP_ECO_CFG1_REG                (xxx_cfg_disp_sub0_BASE + 0xC04)  /* 用于ECO的冗余配置寄存器1 */
#define xxx_cfg_disp_sub0_DISP_ECO_CFG2_REG                (xxx_cfg_disp_sub0_BASE + 0xC08)  /* 用于ECO的冗余配置寄存器2 */
#define xxx_cfg_disp_sub0_DISP_ECO_CFG3_REG                (xxx_cfg_disp_sub0_BASE + 0xC0C)  /* 用于ECO的冗余配置寄存器3 */
#define xxx_cfg_disp_sub0_DISP_VERSION0_REG                (xxx_cfg_disp_sub0_BASE + 0xC10)  /* VERSION寄存器0 */
#define xxx_cfg_disp_sub0_DISP_VERSION1_REG                (xxx_cfg_disp_sub0_BASE + 0xC14)  /* VERSION寄存器1 */
#define xxx_cfg_disp_sub0_DISP_VERSION2_REG                (xxx_cfg_disp_sub0_BASE + 0xC18)  /* VERSION寄存器2 */
#define xxx_cfg_disp_sub0_DISP_VERSION3_REG                (xxx_cfg_disp_sub0_BASE + 0xC1C)  /* VERSION寄存器3 */
#define xxx_cfg_disp_sub0_DISP_BUSY_REG                    (xxx_cfg_disp_sub0_BASE + 0x1000) /* 设备忙状态寄存器 */
#define xxx_cfg_disp_sub0_DISP_DFX_CTRL_REG                (xxx_cfg_disp_sub0_BASE + 0x1040) /* DFX时钟门控控制寄存器 */
#define xxx_cfg_disp_sub0_DISP_TRAP_SEL_REG                (xxx_cfg_disp_sub0_BASE + 0x1048) /* dispatch Slave接口TRAP选择寄存器 */
#define xxx_cfg_disp_sub0_DISP_TRAP_CTRL_REG               (xxx_cfg_disp_sub0_BASE + 0x1050) /* Trap功能控制寄存器 */
#define xxx_cfg_disp_sub0_DISP_TRAP_REVERSE_REG            (xxx_cfg_disp_sub0_BASE + 0x1054) /* Trap条件取反寄存器 */
#define xxx_cfg_disp_sub0_DISP_TRAP_ORDER_REG              (xxx_cfg_disp_sub0_BASE + 0x105C) /* Trap匹配顺序寄存器 */
#define xxx_cfg_disp_sub0_DISP_TRAP_CMD_COMP0_REG          (xxx_cfg_disp_sub0_BASE + 0x1100) /* Slave CMD通道TRAP信息配置寄存器0 */
#define xxx_cfg_disp_sub0_DISP_TRAP_CMD_COMP1_REG          (xxx_cfg_disp_sub0_BASE + 0x1104) /* Slave CMD通道TRAP信息配置寄存器1 */
#define xxx_cfg_disp_sub0_DISP_TRAP_CMD_MASK0_REG          (xxx_cfg_disp_sub0_BASE + 0x1108) /* Slave CMD通道TRAP信息屏蔽寄存器0 */
#define xxx_cfg_disp_sub0_DISP_TRAP_CMD_MASK1_REG          (xxx_cfg_disp_sub0_BASE + 0x110C) /* Slave CMD通道TRAP信息屏蔽寄存器1 */
#define xxx_cfg_disp_sub0_DISP_TRAP_CMD_COMP2_REG          (xxx_cfg_disp_sub0_BASE + 0x1110) /* Slave CMD通道TRAP信息配置寄存器2 */
#define xxx_cfg_disp_sub0_DISP_TRAP_CMD_MASK2_REG          (xxx_cfg_disp_sub0_BASE + 0x1114) /* Slave CMD通道TRAP信息屏蔽寄存器2 */
#define xxx_cfg_disp_sub0_DISP_TRAP_RSP_COMP0_REG          (xxx_cfg_disp_sub0_BASE + 0x1210) /* Slave RSP通道TRAP信息配置寄存器0 */
#define xxx_cfg_disp_sub0_DISP_TRAP_RSP_MASK0_REG          (xxx_cfg_disp_sub0_BASE + 0x1214) /* Slave RSP通道TRAP信息屏蔽寄存器0 */
#define xxx_cfg_disp_sub0_DISP_TRAP_CMD_INFO0_REG          (xxx_cfg_disp_sub0_BASE + 0x1600) /* Slave CMD通道TRAP信息记录寄存器0 */
#define xxx_cfg_disp_sub0_DISP_TRAP_CMD_INFO1_REG          (xxx_cfg_disp_sub0_BASE + 0x1604) /* Slave CMD通道TRAP信息记录寄存器1 */
#define xxx_cfg_disp_sub0_DISP_TRAP_CMD_INFO2_REG          (xxx_cfg_disp_sub0_BASE + 0x1608) /* Slave CMD通道TRAP信息记录寄存器2 */
#define xxx_cfg_disp_sub0_DISP_TRAP_RSP_INFO0_REG          (xxx_cfg_disp_sub0_BASE + 0x1708) /* Slave RSP通道TRAP信息记录寄存器0 */
#define xxx_cfg_disp_sub0_DISP_PORT0_CUR_ADDR_L_REG        (xxx_cfg_disp_sub0_BASE + 0x2000) /* DISP出口FIFO当前命令信息 */
#define xxx_cfg_disp_sub0_DISP_PORT0_CUR_ADDR_H_REG        (xxx_cfg_disp_sub0_BASE + 0x2004) /* DISP出口FIFO当前命令信息 */
#define xxx_cfg_disp_sub0_DISP_PORT0_CUR_ID_REG            (xxx_cfg_disp_sub0_BASE + 0x2008) /* DISP出口FIFO当前命令信息 */
#define xxx_cfg_disp_sub0_DISP_PORT1_CUR_ADDR_L_REG        (xxx_cfg_disp_sub0_BASE + 0x2010) /* DISP出口FIFO当前命令信息 */
#define xxx_cfg_disp_sub0_DISP_PORT1_CUR_ADDR_H_REG        (xxx_cfg_disp_sub0_BASE + 0x2014) /* DISP出口FIFO当前命令信息 */
#define xxx_cfg_disp_sub0_DISP_PORT1_CUR_ID_REG            (xxx_cfg_disp_sub0_BASE + 0x2018) /* DISP出口FIFO当前命令信息 */
#define xxx_cfg_disp_sub0_DISP_PORT2_CUR_ADDR_L_REG        (xxx_cfg_disp_sub0_BASE + 0x2020) /* DISP出口FIFO当前命令信息 */
#define xxx_cfg_disp_sub0_DISP_PORT2_CUR_ADDR_H_REG        (xxx_cfg_disp_sub0_BASE + 0x2024) /* DISP出口FIFO当前命令信息 */
#define xxx_cfg_disp_sub0_DISP_PORT2_CUR_ID_REG            (xxx_cfg_disp_sub0_BASE + 0x2028) /* DISP出口FIFO当前命令信息 */
#define xxx_cfg_disp_sub0_DISP_PORT3_CUR_ADDR_L_REG        (xxx_cfg_disp_sub0_BASE + 0x2030) /* DISP出口FIFO当前命令信息 */
#define xxx_cfg_disp_sub0_DISP_PORT3_CUR_ADDR_H_REG        (xxx_cfg_disp_sub0_BASE + 0x2034) /* DISP出口FIFO当前命令信息 */
#define xxx_cfg_disp_sub0_DISP_PORT3_CUR_ID_REG            (xxx_cfg_disp_sub0_BASE + 0x2038) /* DISP出口FIFO当前命令信息 */
#define xxx_cfg_disp_sub0_DISP_PORT4_CUR_ADDR_L_REG        (xxx_cfg_disp_sub0_BASE + 0x2040) /* DISP出口FIFO当前命令信息 */
#define xxx_cfg_disp_sub0_DISP_PORT4_CUR_ADDR_H_REG        (xxx_cfg_disp_sub0_BASE + 0x2044) /* DISP出口FIFO当前命令信息 */
#define xxx_cfg_disp_sub0_DISP_PORT4_CUR_ID_REG            (xxx_cfg_disp_sub0_BASE + 0x2048) /* DISP出口FIFO当前命令信息 */
#define xxx_cfg_disp_sub0_DISP_PORT5_CUR_ADDR_L_REG        (xxx_cfg_disp_sub0_BASE + 0x2050) /* DISP出口FIFO当前命令信息 */
#define xxx_cfg_disp_sub0_DISP_PORT5_CUR_ADDR_H_REG        (xxx_cfg_disp_sub0_BASE + 0x2054) /* DISP出口FIFO当前命令信息 */
#define xxx_cfg_disp_sub0_DISP_PORT5_CUR_ID_REG            (xxx_cfg_disp_sub0_BASE + 0x2058) /* DISP出口FIFO当前命令信息 */
#define xxx_cfg_disp_sub0_DISP_PORT6_CUR_ADDR_L_REG        (xxx_cfg_disp_sub0_BASE + 0x2060) /* DISP出口FIFO当前命令信息 */
#define xxx_cfg_disp_sub0_DISP_PORT6_CUR_ADDR_H_REG        (xxx_cfg_disp_sub0_BASE + 0x2064) /* DISP出口FIFO当前命令信息 */
#define xxx_cfg_disp_sub0_DISP_PORT6_CUR_ID_REG            (xxx_cfg_disp_sub0_BASE + 0x2068) /* DISP出口FIFO当前命令信息 */
#define xxx_cfg_disp_sub0_DISP_PORT7_CUR_ADDR_L_REG        (xxx_cfg_disp_sub0_BASE + 0x2070) /* DISP出口FIFO当前命令信息 */
#define xxx_cfg_disp_sub0_DISP_PORT7_CUR_ADDR_H_REG        (xxx_cfg_disp_sub0_BASE + 0x2074) /* DISP出口FIFO当前命令信息 */
#define xxx_cfg_disp_sub0_DISP_PORT7_CUR_ID_REG            (xxx_cfg_disp_sub0_BASE + 0x2078) /* DISP出口FIFO当前命令信息 */

#endif // __CFG_DISP_SUB0_REG_OFFSET_H__
