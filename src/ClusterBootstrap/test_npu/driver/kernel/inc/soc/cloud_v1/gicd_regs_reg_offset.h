// ****************************************************************************** 
// Copyright     :  Copyright (C) 2018, Hisilicon Technologies Co. Ltd.
// File name     :  gicd_regs_reg_offset.h
// Department    :  CAD Development Department
// Author        :  xxx
// Version       :  1
// Date          :  2013/3/10
// Description   :  The description of xxx project
// Others        :  Generated automatically by nManager V4.2 
// History       :  xxx 2018/07/19 11:13:37 Create file
// ******************************************************************************

#ifndef __GICD_REGS_REG_OFFSET_H__
#define __GICD_REGS_REG_OFFSET_H__

/* GICD_REGS Base address of Module's Register */
#define xxx_GICD_REGS_BASE                       (0xAA000000)

/******************************************************************************/
/*                      xxx GICD_REGS Registers' Definitions                            */
/******************************************************************************/

#define xxx_GICD_REGS_GICD_CTLR_NS_REG                     (xxx_GICD_REGS_BASE + 0x0)    /* Distributor的控制寄存器（非安全操作） */
#define xxx_GICD_REGS_GICD_SETSPI_NSR_REG                  (xxx_GICD_REGS_BASE + 0x40)   /* 产生非安全SPI寄存器 */
#define xxx_GICD_REGS_GICD_CLRSPI_NSR_REG                  (xxx_GICD_REGS_BASE + 0x48)   /* 清除非安全SPI寄存器 */
#define xxx_GICD_REGS_GICD_SETSPI_SR_REG                   (xxx_GICD_REGS_BASE + 0x50)   /* 产生安全SPI寄存器 */
#define xxx_GICD_REGS_GICD_CLRSPI_SR_REG                   (xxx_GICD_REGS_BASE + 0x58)   /* 清除安全SPI寄存器 */
#define xxx_GICD_REGS_GICD_IGROUPR_SPI_0_REG               (xxx_GICD_REGS_BASE + 0x84)   /* 标识spi中断分组至Group0或Group1，n的值由GICD_TYPER.ITLinesNumber决定。m{0,30} */
#define xxx_GICD_REGS_GICD_IGROUPR_SPI_1_REG               (xxx_GICD_REGS_BASE + 0x88)   /* 标识spi中断分组至Group0或Group1，n的值由GICD_TYPER.ITLinesNumber决定。m{0,30} */
#define xxx_GICD_REGS_GICD_IGROUPR_SPI_2_REG               (xxx_GICD_REGS_BASE + 0x8C)   /* 标识spi中断分组至Group0或Group1，n的值由GICD_TYPER.ITLinesNumber决定。m{0,30} */
#define xxx_GICD_REGS_GICD_IGROUPR_SPI_3_REG               (xxx_GICD_REGS_BASE + 0x90)   /* 标识spi中断分组至Group0或Group1，n的值由GICD_TYPER.ITLinesNumber决定。m{0,30} */
#define xxx_GICD_REGS_GICD_IGROUPR_SPI_4_REG               (xxx_GICD_REGS_BASE + 0x94)   /* 标识spi中断分组至Group0或Group1，n的值由GICD_TYPER.ITLinesNumber决定。m{0,30} */
#define xxx_GICD_REGS_GICD_IGROUPR_SPI_5_REG               (xxx_GICD_REGS_BASE + 0x98)   /* 标识spi中断分组至Group0或Group1，n的值由GICD_TYPER.ITLinesNumber决定。m{0,30} */
#define xxx_GICD_REGS_GICD_IGROUPR_SPI_6_REG               (xxx_GICD_REGS_BASE + 0x9C)   /* 标识spi中断分组至Group0或Group1，n的值由GICD_TYPER.ITLinesNumber决定。m{0,30} */
#define xxx_GICD_REGS_GICD_IGROUPR_SPI_7_REG               (xxx_GICD_REGS_BASE + 0xA0)   /* 标识spi中断分组至Group0或Group1，n的值由GICD_TYPER.ITLinesNumber决定。m{0,30} */
#define xxx_GICD_REGS_GICD_IGROUPR_SPI_8_REG               (xxx_GICD_REGS_BASE + 0xA4)   /* 标识spi中断分组至Group0或Group1，n的值由GICD_TYPER.ITLinesNumber决定。m{0,30} */
#define xxx_GICD_REGS_GICD_IGROUPR_SPI_9_REG               (xxx_GICD_REGS_BASE + 0xA8)   /* 标识spi中断分组至Group0或Group1，n的值由GICD_TYPER.ITLinesNumber决定。m{0,30} */
#define xxx_GICD_REGS_GICD_IGROUPR_SPI_10_REG              (xxx_GICD_REGS_BASE + 0xAC)   /* 标识spi中断分组至Group0或Group1，n的值由GICD_TYPER.ITLinesNumber决定。m{0,30} */
#define xxx_GICD_REGS_GICD_IGROUPR_SPI_11_REG              (xxx_GICD_REGS_BASE + 0xB0)   /* 标识spi中断分组至Group0或Group1，n的值由GICD_TYPER.ITLinesNumber决定。m{0,30} */
#define xxx_GICD_REGS_GICD_IGROUPR_SPI_12_REG              (xxx_GICD_REGS_BASE + 0xB4)   /* 标识spi中断分组至Group0或Group1，n的值由GICD_TYPER.ITLinesNumber决定。m{0,30} */
#define xxx_GICD_REGS_GICD_IGROUPR_SPI_13_REG              (xxx_GICD_REGS_BASE + 0xB8)   /* 标识spi中断分组至Group0或Group1，n的值由GICD_TYPER.ITLinesNumber决定。m{0,30} */
#define xxx_GICD_REGS_GICD_IGROUPR_SPI_14_REG              (xxx_GICD_REGS_BASE + 0xBC)   /* 标识spi中断分组至Group0或Group1，n的值由GICD_TYPER.ITLinesNumber决定。m{0,30} */
#define xxx_GICD_REGS_GICD_IGROUPR_SPI_15_REG              (xxx_GICD_REGS_BASE + 0xC0)   /* 标识spi中断分组至Group0或Group1，n的值由GICD_TYPER.ITLinesNumber决定。m{0,30} */
#define xxx_GICD_REGS_GICD_IGROUPR_SPI_16_REG              (xxx_GICD_REGS_BASE + 0xC4)   /* 标识spi中断分组至Group0或Group1，n的值由GICD_TYPER.ITLinesNumber决定。m{0,30} */
#define xxx_GICD_REGS_GICD_IGROUPR_SPI_17_REG              (xxx_GICD_REGS_BASE + 0xC8)   /* 标识spi中断分组至Group0或Group1，n的值由GICD_TYPER.ITLinesNumber决定。m{0,30} */
#define xxx_GICD_REGS_GICD_IGROUPR_SPI_18_REG              (xxx_GICD_REGS_BASE + 0xCC)   /* 标识spi中断分组至Group0或Group1，n的值由GICD_TYPER.ITLinesNumber决定。m{0,30} */
#define xxx_GICD_REGS_GICD_IGROUPR_SPI_19_REG              (xxx_GICD_REGS_BASE + 0xD0)   /* 标识spi中断分组至Group0或Group1，n的值由GICD_TYPER.ITLinesNumber决定。m{0,30} */
#define xxx_GICD_REGS_GICD_IGROUPR_SPI_20_REG              (xxx_GICD_REGS_BASE + 0xD4)   /* 标识spi中断分组至Group0或Group1，n的值由GICD_TYPER.ITLinesNumber决定。m{0,30} */
#define xxx_GICD_REGS_GICD_IGROUPR_SPI_21_REG              (xxx_GICD_REGS_BASE + 0xD8)   /* 标识spi中断分组至Group0或Group1，n的值由GICD_TYPER.ITLinesNumber决定。m{0,30} */
#define xxx_GICD_REGS_GICD_IGROUPR_SPI_22_REG              (xxx_GICD_REGS_BASE + 0xDC)   /* 标识spi中断分组至Group0或Group1，n的值由GICD_TYPER.ITLinesNumber决定。m{0,30} */
#define xxx_GICD_REGS_GICD_IGROUPR_SPI_23_REG              (xxx_GICD_REGS_BASE + 0xE0)   /* 标识spi中断分组至Group0或Group1，n的值由GICD_TYPER.ITLinesNumber决定。m{0,30} */
#define xxx_GICD_REGS_GICD_IGROUPR_SPI_24_REG              (xxx_GICD_REGS_BASE + 0xE4)   /* 标识spi中断分组至Group0或Group1，n的值由GICD_TYPER.ITLinesNumber决定。m{0,30} */
#define xxx_GICD_REGS_GICD_IGROUPR_SPI_25_REG              (xxx_GICD_REGS_BASE + 0xE8)   /* 标识spi中断分组至Group0或Group1，n的值由GICD_TYPER.ITLinesNumber决定。m{0,30} */
#define xxx_GICD_REGS_GICD_IGROUPR_SPI_26_REG              (xxx_GICD_REGS_BASE + 0xEC)   /* 标识spi中断分组至Group0或Group1，n的值由GICD_TYPER.ITLinesNumber决定。m{0,30} */
#define xxx_GICD_REGS_GICD_IGROUPR_SPI_27_REG              (xxx_GICD_REGS_BASE + 0xF0)   /* 标识spi中断分组至Group0或Group1，n的值由GICD_TYPER.ITLinesNumber决定。m{0,30} */
#define xxx_GICD_REGS_GICD_IGROUPR_SPI_28_REG              (xxx_GICD_REGS_BASE + 0xF4)   /* 标识spi中断分组至Group0或Group1，n的值由GICD_TYPER.ITLinesNumber决定。m{0,30} */
#define xxx_GICD_REGS_GICD_IGROUPR_SPI_29_REG              (xxx_GICD_REGS_BASE + 0xF8)   /* 标识spi中断分组至Group0或Group1，n的值由GICD_TYPER.ITLinesNumber决定。m{0,30} */
#define xxx_GICD_REGS_GICD_IGROUPR_SPI_30_REG              (xxx_GICD_REGS_BASE + 0xFC)   /* 标识spi中断分组至Group0或Group1，n的值由GICD_TYPER.ITLinesNumber决定。m{0,30} */
#define xxx_GICD_REGS_GICD_ISENABLER_LOW_REG               (xxx_GICD_REGS_BASE + 0x100)  /* lowid中断中（对应中断ID的范围为{0，31}）单个中断的使能寄存器，决定是否发给Cpu Interface。 */
#define xxx_GICD_REGS_GICD_ISENABLER_SPI_0_REG             (xxx_GICD_REGS_BASE + 0x104)  /* spi单个中断的使能寄存器，决定是否发给Cpu Interface。 */
#define xxx_GICD_REGS_GICD_ISENABLER_SPI_1_REG             (xxx_GICD_REGS_BASE + 0x108)  /* spi单个中断的使能寄存器，决定是否发给Cpu Interface。 */
#define xxx_GICD_REGS_GICD_ISENABLER_SPI_2_REG             (xxx_GICD_REGS_BASE + 0x10C)  /* spi单个中断的使能寄存器，决定是否发给Cpu Interface。 */
#define xxx_GICD_REGS_GICD_ISENABLER_SPI_3_REG             (xxx_GICD_REGS_BASE + 0x110)  /* spi单个中断的使能寄存器，决定是否发给Cpu Interface。 */
#define xxx_GICD_REGS_GICD_ISENABLER_SPI_4_REG             (xxx_GICD_REGS_BASE + 0x114)  /* spi单个中断的使能寄存器，决定是否发给Cpu Interface。 */
#define xxx_GICD_REGS_GICD_ISENABLER_SPI_5_REG             (xxx_GICD_REGS_BASE + 0x118)  /* spi单个中断的使能寄存器，决定是否发给Cpu Interface。 */
#define xxx_GICD_REGS_GICD_ISENABLER_SPI_6_REG             (xxx_GICD_REGS_BASE + 0x11C)  /* spi单个中断的使能寄存器，决定是否发给Cpu Interface。 */
#define xxx_GICD_REGS_GICD_ISENABLER_SPI_7_REG             (xxx_GICD_REGS_BASE + 0x120)  /* spi单个中断的使能寄存器，决定是否发给Cpu Interface。 */
#define xxx_GICD_REGS_GICD_ISENABLER_SPI_8_REG             (xxx_GICD_REGS_BASE + 0x124)  /* spi单个中断的使能寄存器，决定是否发给Cpu Interface。 */
#define xxx_GICD_REGS_GICD_ISENABLER_SPI_9_REG             (xxx_GICD_REGS_BASE + 0x128)  /* spi单个中断的使能寄存器，决定是否发给Cpu Interface。 */
#define xxx_GICD_REGS_GICD_ISENABLER_SPI_10_REG            (xxx_GICD_REGS_BASE + 0x12C)  /* spi单个中断的使能寄存器，决定是否发给Cpu Interface。 */
#define xxx_GICD_REGS_GICD_ISENABLER_SPI_11_REG            (xxx_GICD_REGS_BASE + 0x130)  /* spi单个中断的使能寄存器，决定是否发给Cpu Interface。 */
#define xxx_GICD_REGS_GICD_ISENABLER_SPI_12_REG            (xxx_GICD_REGS_BASE + 0x134)  /* spi单个中断的使能寄存器，决定是否发给Cpu Interface。 */
#define xxx_GICD_REGS_GICD_ISENABLER_SPI_13_REG            (xxx_GICD_REGS_BASE + 0x138)  /* spi单个中断的使能寄存器，决定是否发给Cpu Interface。 */
#define xxx_GICD_REGS_GICD_ISENABLER_SPI_14_REG            (xxx_GICD_REGS_BASE + 0x13C)  /* spi单个中断的使能寄存器，决定是否发给Cpu Interface。 */
#define xxx_GICD_REGS_GICD_ISENABLER_SPI_15_REG            (xxx_GICD_REGS_BASE + 0x140)  /* spi单个中断的使能寄存器，决定是否发给Cpu Interface。 */
#define xxx_GICD_REGS_GICD_ISENABLER_SPI_16_REG            (xxx_GICD_REGS_BASE + 0x144)  /* spi单个中断的使能寄存器，决定是否发给Cpu Interface。 */
#define xxx_GICD_REGS_GICD_ISENABLER_SPI_17_REG            (xxx_GICD_REGS_BASE + 0x148)  /* spi单个中断的使能寄存器，决定是否发给Cpu Interface。 */
#define xxx_GICD_REGS_GICD_ISENABLER_SPI_18_REG            (xxx_GICD_REGS_BASE + 0x14C)  /* spi单个中断的使能寄存器，决定是否发给Cpu Interface。 */
#define xxx_GICD_REGS_GICD_ISENABLER_SPI_19_REG            (xxx_GICD_REGS_BASE + 0x150)  /* spi单个中断的使能寄存器，决定是否发给Cpu Interface。 */
#define xxx_GICD_REGS_GICD_ISENABLER_SPI_20_REG            (xxx_GICD_REGS_BASE + 0x154)  /* spi单个中断的使能寄存器，决定是否发给Cpu Interface。 */
#define xxx_GICD_REGS_GICD_ISENABLER_SPI_21_REG            (xxx_GICD_REGS_BASE + 0x158)  /* spi单个中断的使能寄存器，决定是否发给Cpu Interface。 */
#define xxx_GICD_REGS_GICD_ISENABLER_SPI_22_REG            (xxx_GICD_REGS_BASE + 0x15C)  /* spi单个中断的使能寄存器，决定是否发给Cpu Interface。 */
#define xxx_GICD_REGS_GICD_ISENABLER_SPI_23_REG            (xxx_GICD_REGS_BASE + 0x160)  /* spi单个中断的使能寄存器，决定是否发给Cpu Interface。 */
#define xxx_GICD_REGS_GICD_ISENABLER_SPI_24_REG            (xxx_GICD_REGS_BASE + 0x164)  /* spi单个中断的使能寄存器，决定是否发给Cpu Interface。 */
#define xxx_GICD_REGS_GICD_ISENABLER_SPI_25_REG            (xxx_GICD_REGS_BASE + 0x168)  /* spi单个中断的使能寄存器，决定是否发给Cpu Interface。 */
#define xxx_GICD_REGS_GICD_ISENABLER_SPI_26_REG            (xxx_GICD_REGS_BASE + 0x16C)  /* spi单个中断的使能寄存器，决定是否发给Cpu Interface。 */
#define xxx_GICD_REGS_GICD_ISENABLER_SPI_27_REG            (xxx_GICD_REGS_BASE + 0x170)  /* spi单个中断的使能寄存器，决定是否发给Cpu Interface。 */
#define xxx_GICD_REGS_GICD_ISENABLER_SPI_28_REG            (xxx_GICD_REGS_BASE + 0x174)  /* spi单个中断的使能寄存器，决定是否发给Cpu Interface。 */
#define xxx_GICD_REGS_GICD_ISENABLER_SPI_29_REG            (xxx_GICD_REGS_BASE + 0x178)  /* spi单个中断的使能寄存器，决定是否发给Cpu Interface。 */
#define xxx_GICD_REGS_GICD_ISENABLER_SPI_30_REG            (xxx_GICD_REGS_BASE + 0x17C)  /* spi单个中断的使能寄存器，决定是否发给Cpu Interface。 */
#define xxx_GICD_REGS_GICD_ICENABLER_LOW_REG               (xxx_GICD_REGS_BASE + 0x180)  /* lowid中断中单个中断的清除使能的寄存器，确定是否发给CPU Interface。 */
#define xxx_GICD_REGS_GICD_ICENABLER_SPI_0_REG             (xxx_GICD_REGS_BASE + 0x184)  /* spi单个中断的清除使能的寄存器，确定是否发给CPU Interface。m的值由GICD_TYPER.ITLinesNumber决定,m{0,2} */
#define xxx_GICD_REGS_GICD_ICENABLER_SPI_1_REG             (xxx_GICD_REGS_BASE + 0x188)  /* spi单个中断的清除使能的寄存器，确定是否发给CPU Interface。m的值由GICD_TYPER.ITLinesNumber决定,m{0,2} */
#define xxx_GICD_REGS_GICD_ICENABLER_SPI_2_REG             (xxx_GICD_REGS_BASE + 0x18C)  /* spi单个中断的清除使能的寄存器，确定是否发给CPU Interface。m的值由GICD_TYPER.ITLinesNumber决定,m{0,2} */
#define xxx_GICD_REGS_GICD_ICENABLER_SPI_3_REG             (xxx_GICD_REGS_BASE + 0x190)  /* spi单个中断的清除使能的寄存器，确定是否发给CPU Interface。m的值由GICD_TYPER.ITLinesNumber决定,m{0,2} */
#define xxx_GICD_REGS_GICD_ICENABLER_SPI_4_REG             (xxx_GICD_REGS_BASE + 0x194)  /* spi单个中断的清除使能的寄存器，确定是否发给CPU Interface。m的值由GICD_TYPER.ITLinesNumber决定,m{0,2} */
#define xxx_GICD_REGS_GICD_ICENABLER_SPI_5_REG             (xxx_GICD_REGS_BASE + 0x198)  /* spi单个中断的清除使能的寄存器，确定是否发给CPU Interface。m的值由GICD_TYPER.ITLinesNumber决定,m{0,2} */
#define xxx_GICD_REGS_GICD_ICENABLER_SPI_6_REG             (xxx_GICD_REGS_BASE + 0x19C)  /* spi单个中断的清除使能的寄存器，确定是否发给CPU Interface。m的值由GICD_TYPER.ITLinesNumber决定,m{0,2} */
#define xxx_GICD_REGS_GICD_ICENABLER_SPI_7_REG             (xxx_GICD_REGS_BASE + 0x1A0)  /* spi单个中断的清除使能的寄存器，确定是否发给CPU Interface。m的值由GICD_TYPER.ITLinesNumber决定,m{0,2} */
#define xxx_GICD_REGS_GICD_ICENABLER_SPI_8_REG             (xxx_GICD_REGS_BASE + 0x1A4)  /* spi单个中断的清除使能的寄存器，确定是否发给CPU Interface。m的值由GICD_TYPER.ITLinesNumber决定,m{0,2} */
#define xxx_GICD_REGS_GICD_ICENABLER_SPI_9_REG             (xxx_GICD_REGS_BASE + 0x1A8)  /* spi单个中断的清除使能的寄存器，确定是否发给CPU Interface。m的值由GICD_TYPER.ITLinesNumber决定,m{0,2} */
#define xxx_GICD_REGS_GICD_ICENABLER_SPI_10_REG            (xxx_GICD_REGS_BASE + 0x1AC)  /* spi单个中断的清除使能的寄存器，确定是否发给CPU Interface。m的值由GICD_TYPER.ITLinesNumber决定,m{0,2} */
#define xxx_GICD_REGS_GICD_ICENABLER_SPI_11_REG            (xxx_GICD_REGS_BASE + 0x1B0)  /* spi单个中断的清除使能的寄存器，确定是否发给CPU Interface。m的值由GICD_TYPER.ITLinesNumber决定,m{0,2} */
#define xxx_GICD_REGS_GICD_ICENABLER_SPI_12_REG            (xxx_GICD_REGS_BASE + 0x1B4)  /* spi单个中断的清除使能的寄存器，确定是否发给CPU Interface。m的值由GICD_TYPER.ITLinesNumber决定,m{0,2} */
#define xxx_GICD_REGS_GICD_ICENABLER_SPI_13_REG            (xxx_GICD_REGS_BASE + 0x1B8)  /* spi单个中断的清除使能的寄存器，确定是否发给CPU Interface。m的值由GICD_TYPER.ITLinesNumber决定,m{0,2} */
#define xxx_GICD_REGS_GICD_ICENABLER_SPI_14_REG            (xxx_GICD_REGS_BASE + 0x1BC)  /* spi单个中断的清除使能的寄存器，确定是否发给CPU Interface。m的值由GICD_TYPER.ITLinesNumber决定,m{0,2} */
#define xxx_GICD_REGS_GICD_ICENABLER_SPI_15_REG            (xxx_GICD_REGS_BASE + 0x1C0)  /* spi单个中断的清除使能的寄存器，确定是否发给CPU Interface。m的值由GICD_TYPER.ITLinesNumber决定,m{0,2} */
#define xxx_GICD_REGS_GICD_ICENABLER_SPI_16_REG            (xxx_GICD_REGS_BASE + 0x1C4)  /* spi单个中断的清除使能的寄存器，确定是否发给CPU Interface。m的值由GICD_TYPER.ITLinesNumber决定,m{0,2} */
#define xxx_GICD_REGS_GICD_ICENABLER_SPI_17_REG            (xxx_GICD_REGS_BASE + 0x1C8)  /* spi单个中断的清除使能的寄存器，确定是否发给CPU Interface。m的值由GICD_TYPER.ITLinesNumber决定,m{0,2} */
#define xxx_GICD_REGS_GICD_ICENABLER_SPI_18_REG            (xxx_GICD_REGS_BASE + 0x1CC)  /* spi单个中断的清除使能的寄存器，确定是否发给CPU Interface。m的值由GICD_TYPER.ITLinesNumber决定,m{0,2} */
#define xxx_GICD_REGS_GICD_ICENABLER_SPI_19_REG            (xxx_GICD_REGS_BASE + 0x1D0)  /* spi单个中断的清除使能的寄存器，确定是否发给CPU Interface。m的值由GICD_TYPER.ITLinesNumber决定,m{0,2} */
#define xxx_GICD_REGS_GICD_ICENABLER_SPI_20_REG            (xxx_GICD_REGS_BASE + 0x1D4)  /* spi单个中断的清除使能的寄存器，确定是否发给CPU Interface。m的值由GICD_TYPER.ITLinesNumber决定,m{0,2} */
#define xxx_GICD_REGS_GICD_ICENABLER_SPI_21_REG            (xxx_GICD_REGS_BASE + 0x1D8)  /* spi单个中断的清除使能的寄存器，确定是否发给CPU Interface。m的值由GICD_TYPER.ITLinesNumber决定,m{0,2} */
#define xxx_GICD_REGS_GICD_ICENABLER_SPI_22_REG            (xxx_GICD_REGS_BASE + 0x1DC)  /* spi单个中断的清除使能的寄存器，确定是否发给CPU Interface。m的值由GICD_TYPER.ITLinesNumber决定,m{0,2} */
#define xxx_GICD_REGS_GICD_ICENABLER_SPI_23_REG            (xxx_GICD_REGS_BASE + 0x1E0)  /* spi单个中断的清除使能的寄存器，确定是否发给CPU Interface。m的值由GICD_TYPER.ITLinesNumber决定,m{0,2} */
#define xxx_GICD_REGS_GICD_ICENABLER_SPI_24_REG            (xxx_GICD_REGS_BASE + 0x1E4)  /* spi单个中断的清除使能的寄存器，确定是否发给CPU Interface。m的值由GICD_TYPER.ITLinesNumber决定,m{0,2} */
#define xxx_GICD_REGS_GICD_ICENABLER_SPI_25_REG            (xxx_GICD_REGS_BASE + 0x1E8)  /* spi单个中断的清除使能的寄存器，确定是否发给CPU Interface。m的值由GICD_TYPER.ITLinesNumber决定,m{0,2} */
#define xxx_GICD_REGS_GICD_ICENABLER_SPI_26_REG            (xxx_GICD_REGS_BASE + 0x1EC)  /* spi单个中断的清除使能的寄存器，确定是否发给CPU Interface。m的值由GICD_TYPER.ITLinesNumber决定,m{0,2} */
#define xxx_GICD_REGS_GICD_ICENABLER_SPI_27_REG            (xxx_GICD_REGS_BASE + 0x1F0)  /* spi单个中断的清除使能的寄存器，确定是否发给CPU Interface。m的值由GICD_TYPER.ITLinesNumber决定,m{0,2} */
#define xxx_GICD_REGS_GICD_ICENABLER_SPI_28_REG            (xxx_GICD_REGS_BASE + 0x1F4)  /* spi单个中断的清除使能的寄存器，确定是否发给CPU Interface。m的值由GICD_TYPER.ITLinesNumber决定,m{0,2} */
#define xxx_GICD_REGS_GICD_ICENABLER_SPI_29_REG            (xxx_GICD_REGS_BASE + 0x1F8)  /* spi单个中断的清除使能的寄存器，确定是否发给CPU Interface。m的值由GICD_TYPER.ITLinesNumber决定,m{0,2} */
#define xxx_GICD_REGS_GICD_ICENABLER_SPI_30_REG            (xxx_GICD_REGS_BASE + 0x1FC)  /* spi单个中断的清除使能的寄存器，确定是否发给CPU Interface。m的值由GICD_TYPER.ITLinesNumber决定,m{0,2} */
#define xxx_GICD_REGS_GICD_IPRIORITY_LOWSGI_NS_0_REG       (xxx_GICD_REGS_BASE + 0x400)  /* 每8bit对应一个中断的优先级。x{0,3} */
#define xxx_GICD_REGS_GICD_IPRIORITY_LOWSGI_NS_1_REG       (xxx_GICD_REGS_BASE + 0x404)  /* 每8bit对应一个中断的优先级。x{0,3} */
#define xxx_GICD_REGS_GICD_IPRIORITY_LOWSGI_NS_2_REG       (xxx_GICD_REGS_BASE + 0x408)  /* 每8bit对应一个中断的优先级。x{0,3} */
#define xxx_GICD_REGS_GICD_IPRIORITY_LOWSGI_NS_3_REG       (xxx_GICD_REGS_BASE + 0x40C)  /* 每8bit对应一个中断的优先级。x{0,3} */
#define xxx_GICD_REGS_GICD_IPRIORITY_LOWPPI2023_NS_REG     (xxx_GICD_REGS_BASE + 0x414)  /* 每8bit对应一个中断的优先级。 */
#define xxx_GICD_REGS_GICD_IPRIORITY_LOWPPI24TO31_NS_0_REG (xxx_GICD_REGS_BASE + 0x418)  /* 每8bit对应一个中断的优先级。y{0,1} */
#define xxx_GICD_REGS_GICD_IPRIORITY_LOWPPI24TO31_NS_1_REG (xxx_GICD_REGS_BASE + 0x41C)  /* 每8bit对应一个中断的优先级。y{0,1} */
#define xxx_GICD_REGS_GICD_IPRIORITYR_SPI_NS_0_REG         (xxx_GICD_REGS_BASE + 0x420)  /* 每8bit对应一个中断的优先级。n{0,23} */
#define xxx_GICD_REGS_GICD_IPRIORITYR_SPI_NS_1_REG         (xxx_GICD_REGS_BASE + 0x424)  /* 每8bit对应一个中断的优先级。n{0,23} */
#define xxx_GICD_REGS_GICD_IPRIORITYR_SPI_NS_2_REG         (xxx_GICD_REGS_BASE + 0x428)  /* 每8bit对应一个中断的优先级。n{0,23} */
#define xxx_GICD_REGS_GICD_IPRIORITYR_SPI_NS_3_REG         (xxx_GICD_REGS_BASE + 0x42C)  /* 每8bit对应一个中断的优先级。n{0,23} */
#define xxx_GICD_REGS_GICD_IPRIORITYR_SPI_NS_4_REG         (xxx_GICD_REGS_BASE + 0x430)  /* 每8bit对应一个中断的优先级。n{0,23} */
#define xxx_GICD_REGS_GICD_IPRIORITYR_SPI_NS_5_REG         (xxx_GICD_REGS_BASE + 0x434)  /* 每8bit对应一个中断的优先级。n{0,23} */
#define xxx_GICD_REGS_GICD_IPRIORITYR_SPI_NS_6_REG         (xxx_GICD_REGS_BASE + 0x438)  /* 每8bit对应一个中断的优先级。n{0,23} */
#define xxx_GICD_REGS_GICD_IPRIORITYR_SPI_NS_7_REG         (xxx_GICD_REGS_BASE + 0x43C)  /* 每8bit对应一个中断的优先级。n{0,23} */
#define xxx_GICD_REGS_GICD_IPRIORITYR_SPI_NS_8_REG         (xxx_GICD_REGS_BASE + 0x440)  /* 每8bit对应一个中断的优先级。n{0,23} */
#define xxx_GICD_REGS_GICD_IPRIORITYR_SPI_NS_9_REG         (xxx_GICD_REGS_BASE + 0x444)  /* 每8bit对应一个中断的优先级。n{0,23} */
#define xxx_GICD_REGS_GICD_IPRIORITYR_SPI_NS_10_REG        (xxx_GICD_REGS_BASE + 0x448)  /* 每8bit对应一个中断的优先级。n{0,23} */
#define xxx_GICD_REGS_GICD_IPRIORITYR_SPI_NS_11_REG        (xxx_GICD_REGS_BASE + 0x44C)  /* 每8bit对应一个中断的优先级。n{0,23} */
#define xxx_GICD_REGS_GICD_IPRIORITYR_SPI_NS_12_REG        (xxx_GICD_REGS_BASE + 0x450)  /* 每8bit对应一个中断的优先级。n{0,23} */
#define xxx_GICD_REGS_GICD_IPRIORITYR_SPI_NS_13_REG        (xxx_GICD_REGS_BASE + 0x454)  /* 每8bit对应一个中断的优先级。n{0,23} */
#define xxx_GICD_REGS_GICD_IPRIORITYR_SPI_NS_14_REG        (xxx_GICD_REGS_BASE + 0x458)  /* 每8bit对应一个中断的优先级。n{0,23} */
#define xxx_GICD_REGS_GICD_IPRIORITYR_SPI_NS_15_REG        (xxx_GICD_REGS_BASE + 0x45C)  /* 每8bit对应一个中断的优先级。n{0,23} */
#define xxx_GICD_REGS_GICD_IPRIORITYR_SPI_NS_16_REG        (xxx_GICD_REGS_BASE + 0x460)  /* 每8bit对应一个中断的优先级。n{0,23} */
#define xxx_GICD_REGS_GICD_IPRIORITYR_SPI_NS_17_REG        (xxx_GICD_REGS_BASE + 0x464)  /* 每8bit对应一个中断的优先级。n{0,23} */
#define xxx_GICD_REGS_GICD_IPRIORITYR_SPI_NS_18_REG        (xxx_GICD_REGS_BASE + 0x468)  /* 每8bit对应一个中断的优先级。n{0,23} */
#define xxx_GICD_REGS_GICD_IPRIORITYR_SPI_NS_19_REG        (xxx_GICD_REGS_BASE + 0x46C)  /* 每8bit对应一个中断的优先级。n{0,23} */
#define xxx_GICD_REGS_GICD_IPRIORITYR_SPI_NS_20_REG        (xxx_GICD_REGS_BASE + 0x470)  /* 每8bit对应一个中断的优先级。n{0,23} */
#define xxx_GICD_REGS_GICD_IPRIORITYR_SPI_NS_21_REG        (xxx_GICD_REGS_BASE + 0x474)  /* 每8bit对应一个中断的优先级。n{0,23} */
#define xxx_GICD_REGS_GICD_IPRIORITYR_SPI_NS_22_REG        (xxx_GICD_REGS_BASE + 0x478)  /* 每8bit对应一个中断的优先级。n{0,23} */
#define xxx_GICD_REGS_GICD_IPRIORITYR_SPI_NS_23_REG        (xxx_GICD_REGS_BASE + 0x47C)  /* 每8bit对应一个中断的优先级。n{0,23} */
#define xxx_GICD_REGS_GICD_IPRIORITYR_SPI_NS_24_REG        (xxx_GICD_REGS_BASE + 0x480)  /* 每8bit对应一个中断的优先级。n{0,23} */
#define xxx_GICD_REGS_GICD_IPRIORITYR_SPI_NS_25_REG        (xxx_GICD_REGS_BASE + 0x484)  /* 每8bit对应一个中断的优先级。n{0,23} */
#define xxx_GICD_REGS_GICD_IPRIORITYR_SPI_NS_26_REG        (xxx_GICD_REGS_BASE + 0x488)  /* 每8bit对应一个中断的优先级。n{0,23} */
#define xxx_GICD_REGS_GICD_IPRIORITYR_SPI_NS_27_REG        (xxx_GICD_REGS_BASE + 0x48C)  /* 每8bit对应一个中断的优先级。n{0,23} */
#define xxx_GICD_REGS_GICD_IPRIORITYR_SPI_NS_28_REG        (xxx_GICD_REGS_BASE + 0x490)  /* 每8bit对应一个中断的优先级。n{0,23} */
#define xxx_GICD_REGS_GICD_IPRIORITYR_SPI_NS_29_REG        (xxx_GICD_REGS_BASE + 0x494)  /* 每8bit对应一个中断的优先级。n{0,23} */
#define xxx_GICD_REGS_GICD_IPRIORITYR_SPI_NS_30_REG        (xxx_GICD_REGS_BASE + 0x498)  /* 每8bit对应一个中断的优先级。n{0,23} */
#define xxx_GICD_REGS_GICD_IPRIORITYR_SPI_NS_31_REG        (xxx_GICD_REGS_BASE + 0x49C)  /* 每8bit对应一个中断的优先级。n{0,23} */
#define xxx_GICD_REGS_GICD_IPRIORITYR_SPI_NS_32_REG        (xxx_GICD_REGS_BASE + 0x4A0)  /* 每8bit对应一个中断的优先级。n{0,23} */
#define xxx_GICD_REGS_GICD_IPRIORITYR_SPI_NS_33_REG        (xxx_GICD_REGS_BASE + 0x4A4)  /* 每8bit对应一个中断的优先级。n{0,23} */
#define xxx_GICD_REGS_GICD_IPRIORITYR_SPI_NS_34_REG        (xxx_GICD_REGS_BASE + 0x4A8)  /* 每8bit对应一个中断的优先级。n{0,23} */
#define xxx_GICD_REGS_GICD_IPRIORITYR_SPI_NS_35_REG        (xxx_GICD_REGS_BASE + 0x4AC)  /* 每8bit对应一个中断的优先级。n{0,23} */
#define xxx_GICD_REGS_GICD_IPRIORITYR_SPI_NS_36_REG        (xxx_GICD_REGS_BASE + 0x4B0)  /* 每8bit对应一个中断的优先级。n{0,23} */
#define xxx_GICD_REGS_GICD_IPRIORITYR_SPI_NS_37_REG        (xxx_GICD_REGS_BASE + 0x4B4)  /* 每8bit对应一个中断的优先级。n{0,23} */
#define xxx_GICD_REGS_GICD_IPRIORITYR_SPI_NS_38_REG        (xxx_GICD_REGS_BASE + 0x4B8)  /* 每8bit对应一个中断的优先级。n{0,23} */
#define xxx_GICD_REGS_GICD_IPRIORITYR_SPI_NS_39_REG        (xxx_GICD_REGS_BASE + 0x4BC)  /* 每8bit对应一个中断的优先级。n{0,23} */
#define xxx_GICD_REGS_GICD_IPRIORITYR_SPI_NS_40_REG        (xxx_GICD_REGS_BASE + 0x4C0)  /* 每8bit对应一个中断的优先级。n{0,23} */
#define xxx_GICD_REGS_GICD_IPRIORITYR_SPI_NS_41_REG        (xxx_GICD_REGS_BASE + 0x4C4)  /* 每8bit对应一个中断的优先级。n{0,23} */
#define xxx_GICD_REGS_GICD_IPRIORITYR_SPI_NS_42_REG        (xxx_GICD_REGS_BASE + 0x4C8)  /* 每8bit对应一个中断的优先级。n{0,23} */
#define xxx_GICD_REGS_GICD_IPRIORITYR_SPI_NS_43_REG        (xxx_GICD_REGS_BASE + 0x4CC)  /* 每8bit对应一个中断的优先级。n{0,23} */
#define xxx_GICD_REGS_GICD_IPRIORITYR_SPI_NS_44_REG        (xxx_GICD_REGS_BASE + 0x4D0)  /* 每8bit对应一个中断的优先级。n{0,23} */
#define xxx_GICD_REGS_GICD_IPRIORITYR_SPI_NS_45_REG        (xxx_GICD_REGS_BASE + 0x4D4)  /* 每8bit对应一个中断的优先级。n{0,23} */
#define xxx_GICD_REGS_GICD_IPRIORITYR_SPI_NS_46_REG        (xxx_GICD_REGS_BASE + 0x4D8)  /* 每8bit对应一个中断的优先级。n{0,23} */
#define xxx_GICD_REGS_GICD_IPRIORITYR_SPI_NS_47_REG        (xxx_GICD_REGS_BASE + 0x4DC)  /* 每8bit对应一个中断的优先级。n{0,23} */
#define xxx_GICD_REGS_GICD_IPRIORITYR_SPI_NS_48_REG        (xxx_GICD_REGS_BASE + 0x4E0)  /* 每8bit对应一个中断的优先级。n{0,23} */
#define xxx_GICD_REGS_GICD_IPRIORITYR_SPI_NS_49_REG        (xxx_GICD_REGS_BASE + 0x4E4)  /* 每8bit对应一个中断的优先级。n{0,23} */
#define xxx_GICD_REGS_GICD_IPRIORITYR_SPI_NS_50_REG        (xxx_GICD_REGS_BASE + 0x4E8)  /* 每8bit对应一个中断的优先级。n{0,23} */
#define xxx_GICD_REGS_GICD_IPRIORITYR_SPI_NS_51_REG        (xxx_GICD_REGS_BASE + 0x4EC)  /* 每8bit对应一个中断的优先级。n{0,23} */
#define xxx_GICD_REGS_GICD_IPRIORITYR_SPI_NS_52_REG        (xxx_GICD_REGS_BASE + 0x4F0)  /* 每8bit对应一个中断的优先级。n{0,23} */
#define xxx_GICD_REGS_GICD_IPRIORITYR_SPI_NS_53_REG        (xxx_GICD_REGS_BASE + 0x4F4)  /* 每8bit对应一个中断的优先级。n{0,23} */
#define xxx_GICD_REGS_GICD_IPRIORITYR_SPI_NS_54_REG        (xxx_GICD_REGS_BASE + 0x4F8)  /* 每8bit对应一个中断的优先级。n{0,23} */
#define xxx_GICD_REGS_GICD_IPRIORITYR_SPI_NS_55_REG        (xxx_GICD_REGS_BASE + 0x4FC)  /* 每8bit对应一个中断的优先级。n{0,23} */
#define xxx_GICD_REGS_GICD_IPRIORITYR_SPI_NS_56_REG        (xxx_GICD_REGS_BASE + 0x500)  /* 每8bit对应一个中断的优先级。n{0,23} */
#define xxx_GICD_REGS_GICD_IPRIORITYR_SPI_NS_57_REG        (xxx_GICD_REGS_BASE + 0x504)  /* 每8bit对应一个中断的优先级。n{0,23} */
#define xxx_GICD_REGS_GICD_IPRIORITYR_SPI_NS_58_REG        (xxx_GICD_REGS_BASE + 0x508)  /* 每8bit对应一个中断的优先级。n{0,23} */
#define xxx_GICD_REGS_GICD_IPRIORITYR_SPI_NS_59_REG        (xxx_GICD_REGS_BASE + 0x50C)  /* 每8bit对应一个中断的优先级。n{0,23} */
#define xxx_GICD_REGS_GICD_IPRIORITYR_SPI_NS_60_REG        (xxx_GICD_REGS_BASE + 0x510)  /* 每8bit对应一个中断的优先级。n{0,23} */
#define xxx_GICD_REGS_GICD_IPRIORITYR_SPI_NS_61_REG        (xxx_GICD_REGS_BASE + 0x514)  /* 每8bit对应一个中断的优先级。n{0,23} */
#define xxx_GICD_REGS_GICD_IPRIORITYR_SPI_NS_62_REG        (xxx_GICD_REGS_BASE + 0x518)  /* 每8bit对应一个中断的优先级。n{0,23} */
#define xxx_GICD_REGS_GICD_IPRIORITYR_SPI_NS_63_REG        (xxx_GICD_REGS_BASE + 0x51C)  /* 每8bit对应一个中断的优先级。n{0,23} */
#define xxx_GICD_REGS_GICD_IPRIORITYR_SPI_NS_64_REG        (xxx_GICD_REGS_BASE + 0x520)  /* 每8bit对应一个中断的优先级。n{0,23} */
#define xxx_GICD_REGS_GICD_IPRIORITYR_SPI_NS_65_REG        (xxx_GICD_REGS_BASE + 0x524)  /* 每8bit对应一个中断的优先级。n{0,23} */
#define xxx_GICD_REGS_GICD_IPRIORITYR_SPI_NS_66_REG        (xxx_GICD_REGS_BASE + 0x528)  /* 每8bit对应一个中断的优先级。n{0,23} */
#define xxx_GICD_REGS_GICD_IPRIORITYR_SPI_NS_67_REG        (xxx_GICD_REGS_BASE + 0x52C)  /* 每8bit对应一个中断的优先级。n{0,23} */
#define xxx_GICD_REGS_GICD_IPRIORITYR_SPI_NS_68_REG        (xxx_GICD_REGS_BASE + 0x530)  /* 每8bit对应一个中断的优先级。n{0,23} */
#define xxx_GICD_REGS_GICD_IPRIORITYR_SPI_NS_69_REG        (xxx_GICD_REGS_BASE + 0x534)  /* 每8bit对应一个中断的优先级。n{0,23} */
#define xxx_GICD_REGS_GICD_IPRIORITYR_SPI_NS_70_REG        (xxx_GICD_REGS_BASE + 0x538)  /* 每8bit对应一个中断的优先级。n{0,23} */
#define xxx_GICD_REGS_GICD_IPRIORITYR_SPI_NS_71_REG        (xxx_GICD_REGS_BASE + 0x53C)  /* 每8bit对应一个中断的优先级。n{0,23} */
#define xxx_GICD_REGS_GICD_IPRIORITYR_SPI_NS_72_REG        (xxx_GICD_REGS_BASE + 0x540)  /* 每8bit对应一个中断的优先级。n{0,23} */
#define xxx_GICD_REGS_GICD_IPRIORITYR_SPI_NS_73_REG        (xxx_GICD_REGS_BASE + 0x544)  /* 每8bit对应一个中断的优先级。n{0,23} */
#define xxx_GICD_REGS_GICD_IPRIORITYR_SPI_NS_74_REG        (xxx_GICD_REGS_BASE + 0x548)  /* 每8bit对应一个中断的优先级。n{0,23} */
#define xxx_GICD_REGS_GICD_IPRIORITYR_SPI_NS_75_REG        (xxx_GICD_REGS_BASE + 0x54C)  /* 每8bit对应一个中断的优先级。n{0,23} */
#define xxx_GICD_REGS_GICD_IPRIORITYR_SPI_NS_76_REG        (xxx_GICD_REGS_BASE + 0x550)  /* 每8bit对应一个中断的优先级。n{0,23} */
#define xxx_GICD_REGS_GICD_IPRIORITYR_SPI_NS_77_REG        (xxx_GICD_REGS_BASE + 0x554)  /* 每8bit对应一个中断的优先级。n{0,23} */
#define xxx_GICD_REGS_GICD_IPRIORITYR_SPI_NS_78_REG        (xxx_GICD_REGS_BASE + 0x558)  /* 每8bit对应一个中断的优先级。n{0,23} */
#define xxx_GICD_REGS_GICD_IPRIORITYR_SPI_NS_79_REG        (xxx_GICD_REGS_BASE + 0x55C)  /* 每8bit对应一个中断的优先级。n{0,23} */
#define xxx_GICD_REGS_GICD_IPRIORITYR_SPI_NS_80_REG        (xxx_GICD_REGS_BASE + 0x560)  /* 每8bit对应一个中断的优先级。n{0,23} */
#define xxx_GICD_REGS_GICD_IPRIORITYR_SPI_NS_81_REG        (xxx_GICD_REGS_BASE + 0x564)  /* 每8bit对应一个中断的优先级。n{0,23} */
#define xxx_GICD_REGS_GICD_IPRIORITYR_SPI_NS_82_REG        (xxx_GICD_REGS_BASE + 0x568)  /* 每8bit对应一个中断的优先级。n{0,23} */
#define xxx_GICD_REGS_GICD_IPRIORITYR_SPI_NS_83_REG        (xxx_GICD_REGS_BASE + 0x56C)  /* 每8bit对应一个中断的优先级。n{0,23} */
#define xxx_GICD_REGS_GICD_IPRIORITYR_SPI_NS_84_REG        (xxx_GICD_REGS_BASE + 0x570)  /* 每8bit对应一个中断的优先级。n{0,23} */
#define xxx_GICD_REGS_GICD_IPRIORITYR_SPI_NS_85_REG        (xxx_GICD_REGS_BASE + 0x574)  /* 每8bit对应一个中断的优先级。n{0,23} */
#define xxx_GICD_REGS_GICD_IPRIORITYR_SPI_NS_86_REG        (xxx_GICD_REGS_BASE + 0x578)  /* 每8bit对应一个中断的优先级。n{0,23} */
#define xxx_GICD_REGS_GICD_IPRIORITYR_SPI_NS_87_REG        (xxx_GICD_REGS_BASE + 0x57C)  /* 每8bit对应一个中断的优先级。n{0,23} */
#define xxx_GICD_REGS_GICD_IPRIORITYR_SPI_NS_88_REG        (xxx_GICD_REGS_BASE + 0x580)  /* 每8bit对应一个中断的优先级。n{0,23} */
#define xxx_GICD_REGS_GICD_IPRIORITYR_SPI_NS_89_REG        (xxx_GICD_REGS_BASE + 0x584)  /* 每8bit对应一个中断的优先级。n{0,23} */
#define xxx_GICD_REGS_GICD_IPRIORITYR_SPI_NS_90_REG        (xxx_GICD_REGS_BASE + 0x588)  /* 每8bit对应一个中断的优先级。n{0,23} */
#define xxx_GICD_REGS_GICD_IPRIORITYR_SPI_NS_91_REG        (xxx_GICD_REGS_BASE + 0x58C)  /* 每8bit对应一个中断的优先级。n{0,23} */
#define xxx_GICD_REGS_GICD_IPRIORITYR_SPI_NS_92_REG        (xxx_GICD_REGS_BASE + 0x590)  /* 每8bit对应一个中断的优先级。n{0,23} */
#define xxx_GICD_REGS_GICD_IPRIORITYR_SPI_NS_93_REG        (xxx_GICD_REGS_BASE + 0x594)  /* 每8bit对应一个中断的优先级。n{0,23} */
#define xxx_GICD_REGS_GICD_IPRIORITYR_SPI_NS_94_REG        (xxx_GICD_REGS_BASE + 0x598)  /* 每8bit对应一个中断的优先级。n{0,23} */
#define xxx_GICD_REGS_GICD_IPRIORITYR_SPI_NS_95_REG        (xxx_GICD_REGS_BASE + 0x59C)  /* 每8bit对应一个中断的优先级。n{0,23} */
#define xxx_GICD_REGS_GICD_IPRIORITYR_SPI_NS_96_REG        (xxx_GICD_REGS_BASE + 0x5A0)  /* 每8bit对应一个中断的优先级。n{0,23} */
#define xxx_GICD_REGS_GICD_IPRIORITYR_SPI_NS_97_REG        (xxx_GICD_REGS_BASE + 0x5A4)  /* 每8bit对应一个中断的优先级。n{0,23} */
#define xxx_GICD_REGS_GICD_IPRIORITYR_SPI_NS_98_REG        (xxx_GICD_REGS_BASE + 0x5A8)  /* 每8bit对应一个中断的优先级。n{0,23} */
#define xxx_GICD_REGS_GICD_IPRIORITYR_SPI_NS_99_REG        (xxx_GICD_REGS_BASE + 0x5AC)  /* 每8bit对应一个中断的优先级。n{0,23} */
#define xxx_GICD_REGS_GICD_IPRIORITYR_SPI_NS_100_REG       (xxx_GICD_REGS_BASE + 0x5B0)  /* 每8bit对应一个中断的优先级。n{0,23} */
#define xxx_GICD_REGS_GICD_IPRIORITYR_SPI_NS_101_REG       (xxx_GICD_REGS_BASE + 0x5B4)  /* 每8bit对应一个中断的优先级。n{0,23} */
#define xxx_GICD_REGS_GICD_IPRIORITYR_SPI_NS_102_REG       (xxx_GICD_REGS_BASE + 0x5B8)  /* 每8bit对应一个中断的优先级。n{0,23} */
#define xxx_GICD_REGS_GICD_IPRIORITYR_SPI_NS_103_REG       (xxx_GICD_REGS_BASE + 0x5BC)  /* 每8bit对应一个中断的优先级。n{0,23} */
#define xxx_GICD_REGS_GICD_IPRIORITYR_SPI_NS_104_REG       (xxx_GICD_REGS_BASE + 0x5C0)  /* 每8bit对应一个中断的优先级。n{0,23} */
#define xxx_GICD_REGS_GICD_IPRIORITYR_SPI_NS_105_REG       (xxx_GICD_REGS_BASE + 0x5C4)  /* 每8bit对应一个中断的优先级。n{0,23} */
#define xxx_GICD_REGS_GICD_IPRIORITYR_SPI_NS_106_REG       (xxx_GICD_REGS_BASE + 0x5C8)  /* 每8bit对应一个中断的优先级。n{0,23} */
#define xxx_GICD_REGS_GICD_IPRIORITYR_SPI_NS_107_REG       (xxx_GICD_REGS_BASE + 0x5CC)  /* 每8bit对应一个中断的优先级。n{0,23} */
#define xxx_GICD_REGS_GICD_IPRIORITYR_SPI_NS_108_REG       (xxx_GICD_REGS_BASE + 0x5D0)  /* 每8bit对应一个中断的优先级。n{0,23} */
#define xxx_GICD_REGS_GICD_IPRIORITYR_SPI_NS_109_REG       (xxx_GICD_REGS_BASE + 0x5D4)  /* 每8bit对应一个中断的优先级。n{0,23} */
#define xxx_GICD_REGS_GICD_IPRIORITYR_SPI_NS_110_REG       (xxx_GICD_REGS_BASE + 0x5D8)  /* 每8bit对应一个中断的优先级。n{0,23} */
#define xxx_GICD_REGS_GICD_IPRIORITYR_SPI_NS_111_REG       (xxx_GICD_REGS_BASE + 0x5DC)  /* 每8bit对应一个中断的优先级。n{0,23} */
#define xxx_GICD_REGS_GICD_IPRIORITYR_SPI_NS_112_REG       (xxx_GICD_REGS_BASE + 0x5E0)  /* 每8bit对应一个中断的优先级。n{0,23} */
#define xxx_GICD_REGS_GICD_IPRIORITYR_SPI_NS_113_REG       (xxx_GICD_REGS_BASE + 0x5E4)  /* 每8bit对应一个中断的优先级。n{0,23} */
#define xxx_GICD_REGS_GICD_IPRIORITYR_SPI_NS_114_REG       (xxx_GICD_REGS_BASE + 0x5E8)  /* 每8bit对应一个中断的优先级。n{0,23} */
#define xxx_GICD_REGS_GICD_IPRIORITYR_SPI_NS_115_REG       (xxx_GICD_REGS_BASE + 0x5EC)  /* 每8bit对应一个中断的优先级。n{0,23} */
#define xxx_GICD_REGS_GICD_IPRIORITYR_SPI_NS_116_REG       (xxx_GICD_REGS_BASE + 0x5F0)  /* 每8bit对应一个中断的优先级。n{0,23} */
#define xxx_GICD_REGS_GICD_IPRIORITYR_SPI_NS_117_REG       (xxx_GICD_REGS_BASE + 0x5F4)  /* 每8bit对应一个中断的优先级。n{0,23} */
#define xxx_GICD_REGS_GICD_IPRIORITYR_SPI_NS_118_REG       (xxx_GICD_REGS_BASE + 0x5F8)  /* 每8bit对应一个中断的优先级。n{0,23} */
#define xxx_GICD_REGS_GICD_IPRIORITYR_SPI_NS_119_REG       (xxx_GICD_REGS_BASE + 0x5FC)  /* 每8bit对应一个中断的优先级。n{0,23} */
#define xxx_GICD_REGS_GICD_IPRIORITYR_SPI_NS_120_REG       (xxx_GICD_REGS_BASE + 0x600)  /* 每8bit对应一个中断的优先级。n{0,23} */
#define xxx_GICD_REGS_GICD_IPRIORITYR_SPI_NS_121_REG       (xxx_GICD_REGS_BASE + 0x604)  /* 每8bit对应一个中断的优先级。n{0,23} */
#define xxx_GICD_REGS_GICD_IPRIORITYR_SPI_NS_122_REG       (xxx_GICD_REGS_BASE + 0x608)  /* 每8bit对应一个中断的优先级。n{0,23} */
#define xxx_GICD_REGS_GICD_IPRIORITYR_SPI_NS_123_REG       (xxx_GICD_REGS_BASE + 0x60C)  /* 每8bit对应一个中断的优先级。n{0,23} */
#define xxx_GICD_REGS_GICD_IPRIORITYR_SPI_NS_124_REG       (xxx_GICD_REGS_BASE + 0x610)  /* 每8bit对应一个中断的优先级。n{0,23} */
#define xxx_GICD_REGS_GICD_IPRIORITYR_SPI_NS_125_REG       (xxx_GICD_REGS_BASE + 0x614)  /* 每8bit对应一个中断的优先级。n{0,23} */
#define xxx_GICD_REGS_GICD_IPRIORITYR_SPI_NS_126_REG       (xxx_GICD_REGS_BASE + 0x618)  /* 每8bit对应一个中断的优先级。n{0,23} */
#define xxx_GICD_REGS_GICD_IPRIORITYR_SPI_NS_127_REG       (xxx_GICD_REGS_BASE + 0x61C)  /* 每8bit对应一个中断的优先级。n{0,23} */
#define xxx_GICD_REGS_GICD_IPRIORITYR_SPI_NS_128_REG       (xxx_GICD_REGS_BASE + 0x620)  /* 每8bit对应一个中断的优先级。n{0,23} */
#define xxx_GICD_REGS_GICD_IPRIORITYR_SPI_NS_129_REG       (xxx_GICD_REGS_BASE + 0x624)  /* 每8bit对应一个中断的优先级。n{0,23} */
#define xxx_GICD_REGS_GICD_IPRIORITYR_SPI_NS_130_REG       (xxx_GICD_REGS_BASE + 0x628)  /* 每8bit对应一个中断的优先级。n{0,23} */
#define xxx_GICD_REGS_GICD_IPRIORITYR_SPI_NS_131_REG       (xxx_GICD_REGS_BASE + 0x62C)  /* 每8bit对应一个中断的优先级。n{0,23} */
#define xxx_GICD_REGS_GICD_IPRIORITYR_SPI_NS_132_REG       (xxx_GICD_REGS_BASE + 0x630)  /* 每8bit对应一个中断的优先级。n{0,23} */
#define xxx_GICD_REGS_GICD_IPRIORITYR_SPI_NS_133_REG       (xxx_GICD_REGS_BASE + 0x634)  /* 每8bit对应一个中断的优先级。n{0,23} */
#define xxx_GICD_REGS_GICD_IPRIORITYR_SPI_NS_134_REG       (xxx_GICD_REGS_BASE + 0x638)  /* 每8bit对应一个中断的优先级。n{0,23} */
#define xxx_GICD_REGS_GICD_IPRIORITYR_SPI_NS_135_REG       (xxx_GICD_REGS_BASE + 0x63C)  /* 每8bit对应一个中断的优先级。n{0,23} */
#define xxx_GICD_REGS_GICD_IPRIORITYR_SPI_NS_136_REG       (xxx_GICD_REGS_BASE + 0x640)  /* 每8bit对应一个中断的优先级。n{0,23} */
#define xxx_GICD_REGS_GICD_IPRIORITYR_SPI_NS_137_REG       (xxx_GICD_REGS_BASE + 0x644)  /* 每8bit对应一个中断的优先级。n{0,23} */
#define xxx_GICD_REGS_GICD_IPRIORITYR_SPI_NS_138_REG       (xxx_GICD_REGS_BASE + 0x648)  /* 每8bit对应一个中断的优先级。n{0,23} */
#define xxx_GICD_REGS_GICD_IPRIORITYR_SPI_NS_139_REG       (xxx_GICD_REGS_BASE + 0x64C)  /* 每8bit对应一个中断的优先级。n{0,23} */
#define xxx_GICD_REGS_GICD_IPRIORITYR_SPI_NS_140_REG       (xxx_GICD_REGS_BASE + 0x650)  /* 每8bit对应一个中断的优先级。n{0,23} */
#define xxx_GICD_REGS_GICD_IPRIORITYR_SPI_NS_141_REG       (xxx_GICD_REGS_BASE + 0x654)  /* 每8bit对应一个中断的优先级。n{0,23} */
#define xxx_GICD_REGS_GICD_IPRIORITYR_SPI_NS_142_REG       (xxx_GICD_REGS_BASE + 0x658)  /* 每8bit对应一个中断的优先级。n{0,23} */
#define xxx_GICD_REGS_GICD_IPRIORITYR_SPI_NS_143_REG       (xxx_GICD_REGS_BASE + 0x65C)  /* 每8bit对应一个中断的优先级。n{0,23} */
#define xxx_GICD_REGS_GICD_IPRIORITYR_SPI_NS_144_REG       (xxx_GICD_REGS_BASE + 0x660)  /* 每8bit对应一个中断的优先级。n{0,23} */
#define xxx_GICD_REGS_GICD_IPRIORITYR_SPI_NS_145_REG       (xxx_GICD_REGS_BASE + 0x664)  /* 每8bit对应一个中断的优先级。n{0,23} */
#define xxx_GICD_REGS_GICD_IPRIORITYR_SPI_NS_146_REG       (xxx_GICD_REGS_BASE + 0x668)  /* 每8bit对应一个中断的优先级。n{0,23} */
#define xxx_GICD_REGS_GICD_IPRIORITYR_SPI_NS_147_REG       (xxx_GICD_REGS_BASE + 0x66C)  /* 每8bit对应一个中断的优先级。n{0,23} */
#define xxx_GICD_REGS_GICD_IPRIORITYR_SPI_NS_148_REG       (xxx_GICD_REGS_BASE + 0x670)  /* 每8bit对应一个中断的优先级。n{0,23} */
#define xxx_GICD_REGS_GICD_IPRIORITYR_SPI_NS_149_REG       (xxx_GICD_REGS_BASE + 0x674)  /* 每8bit对应一个中断的优先级。n{0,23} */
#define xxx_GICD_REGS_GICD_IPRIORITYR_SPI_NS_150_REG       (xxx_GICD_REGS_BASE + 0x678)  /* 每8bit对应一个中断的优先级。n{0,23} */
#define xxx_GICD_REGS_GICD_IPRIORITYR_SPI_NS_151_REG       (xxx_GICD_REGS_BASE + 0x67C)  /* 每8bit对应一个中断的优先级。n{0,23} */
#define xxx_GICD_REGS_GICD_IPRIORITYR_SPI_NS_152_REG       (xxx_GICD_REGS_BASE + 0x680)  /* 每8bit对应一个中断的优先级。n{0,23} */
#define xxx_GICD_REGS_GICD_IPRIORITYR_SPI_NS_153_REG       (xxx_GICD_REGS_BASE + 0x684)  /* 每8bit对应一个中断的优先级。n{0,23} */
#define xxx_GICD_REGS_GICD_IPRIORITYR_SPI_NS_154_REG       (xxx_GICD_REGS_BASE + 0x688)  /* 每8bit对应一个中断的优先级。n{0,23} */
#define xxx_GICD_REGS_GICD_IPRIORITYR_SPI_NS_155_REG       (xxx_GICD_REGS_BASE + 0x68C)  /* 每8bit对应一个中断的优先级。n{0,23} */
#define xxx_GICD_REGS_GICD_IPRIORITYR_SPI_NS_156_REG       (xxx_GICD_REGS_BASE + 0x690)  /* 每8bit对应一个中断的优先级。n{0,23} */
#define xxx_GICD_REGS_GICD_IPRIORITYR_SPI_NS_157_REG       (xxx_GICD_REGS_BASE + 0x694)  /* 每8bit对应一个中断的优先级。n{0,23} */
#define xxx_GICD_REGS_GICD_IPRIORITYR_SPI_NS_158_REG       (xxx_GICD_REGS_BASE + 0x698)  /* 每8bit对应一个中断的优先级。n{0,23} */
#define xxx_GICD_REGS_GICD_IPRIORITYR_SPI_NS_159_REG       (xxx_GICD_REGS_BASE + 0x69C)  /* 每8bit对应一个中断的优先级。n{0,23} */
#define xxx_GICD_REGS_GICD_IPRIORITYR_SPI_NS_160_REG       (xxx_GICD_REGS_BASE + 0x6A0)  /* 每8bit对应一个中断的优先级。n{0,23} */
#define xxx_GICD_REGS_GICD_IPRIORITYR_SPI_NS_161_REG       (xxx_GICD_REGS_BASE + 0x6A4)  /* 每8bit对应一个中断的优先级。n{0,23} */
#define xxx_GICD_REGS_GICD_IPRIORITYR_SPI_NS_162_REG       (xxx_GICD_REGS_BASE + 0x6A8)  /* 每8bit对应一个中断的优先级。n{0,23} */
#define xxx_GICD_REGS_GICD_IPRIORITYR_SPI_NS_163_REG       (xxx_GICD_REGS_BASE + 0x6AC)  /* 每8bit对应一个中断的优先级。n{0,23} */
#define xxx_GICD_REGS_GICD_IPRIORITYR_SPI_NS_164_REG       (xxx_GICD_REGS_BASE + 0x6B0)  /* 每8bit对应一个中断的优先级。n{0,23} */
#define xxx_GICD_REGS_GICD_IPRIORITYR_SPI_NS_165_REG       (xxx_GICD_REGS_BASE + 0x6B4)  /* 每8bit对应一个中断的优先级。n{0,23} */
#define xxx_GICD_REGS_GICD_IPRIORITYR_SPI_NS_166_REG       (xxx_GICD_REGS_BASE + 0x6B8)  /* 每8bit对应一个中断的优先级。n{0,23} */
#define xxx_GICD_REGS_GICD_IPRIORITYR_SPI_NS_167_REG       (xxx_GICD_REGS_BASE + 0x6BC)  /* 每8bit对应一个中断的优先级。n{0,23} */
#define xxx_GICD_REGS_GICD_IPRIORITYR_SPI_NS_168_REG       (xxx_GICD_REGS_BASE + 0x6C0)  /* 每8bit对应一个中断的优先级。n{0,23} */
#define xxx_GICD_REGS_GICD_IPRIORITYR_SPI_NS_169_REG       (xxx_GICD_REGS_BASE + 0x6C4)  /* 每8bit对应一个中断的优先级。n{0,23} */
#define xxx_GICD_REGS_GICD_IPRIORITYR_SPI_NS_170_REG       (xxx_GICD_REGS_BASE + 0x6C8)  /* 每8bit对应一个中断的优先级。n{0,23} */
#define xxx_GICD_REGS_GICD_IPRIORITYR_SPI_NS_171_REG       (xxx_GICD_REGS_BASE + 0x6CC)  /* 每8bit对应一个中断的优先级。n{0,23} */
#define xxx_GICD_REGS_GICD_IPRIORITYR_SPI_NS_172_REG       (xxx_GICD_REGS_BASE + 0x6D0)  /* 每8bit对应一个中断的优先级。n{0,23} */
#define xxx_GICD_REGS_GICD_IPRIORITYR_SPI_NS_173_REG       (xxx_GICD_REGS_BASE + 0x6D4)  /* 每8bit对应一个中断的优先级。n{0,23} */
#define xxx_GICD_REGS_GICD_IPRIORITYR_SPI_NS_174_REG       (xxx_GICD_REGS_BASE + 0x6D8)  /* 每8bit对应一个中断的优先级。n{0,23} */
#define xxx_GICD_REGS_GICD_IPRIORITYR_SPI_NS_175_REG       (xxx_GICD_REGS_BASE + 0x6DC)  /* 每8bit对应一个中断的优先级。n{0,23} */
#define xxx_GICD_REGS_GICD_IPRIORITYR_SPI_NS_176_REG       (xxx_GICD_REGS_BASE + 0x6E0)  /* 每8bit对应一个中断的优先级。n{0,23} */
#define xxx_GICD_REGS_GICD_IPRIORITYR_SPI_NS_177_REG       (xxx_GICD_REGS_BASE + 0x6E4)  /* 每8bit对应一个中断的优先级。n{0,23} */
#define xxx_GICD_REGS_GICD_IPRIORITYR_SPI_NS_178_REG       (xxx_GICD_REGS_BASE + 0x6E8)  /* 每8bit对应一个中断的优先级。n{0,23} */
#define xxx_GICD_REGS_GICD_IPRIORITYR_SPI_NS_179_REG       (xxx_GICD_REGS_BASE + 0x6EC)  /* 每8bit对应一个中断的优先级。n{0,23} */
#define xxx_GICD_REGS_GICD_IPRIORITYR_SPI_NS_180_REG       (xxx_GICD_REGS_BASE + 0x6F0)  /* 每8bit对应一个中断的优先级。n{0,23} */
#define xxx_GICD_REGS_GICD_IPRIORITYR_SPI_NS_181_REG       (xxx_GICD_REGS_BASE + 0x6F4)  /* 每8bit对应一个中断的优先级。n{0,23} */
#define xxx_GICD_REGS_GICD_IPRIORITYR_SPI_NS_182_REG       (xxx_GICD_REGS_BASE + 0x6F8)  /* 每8bit对应一个中断的优先级。n{0,23} */
#define xxx_GICD_REGS_GICD_IPRIORITYR_SPI_NS_183_REG       (xxx_GICD_REGS_BASE + 0x6FC)  /* 每8bit对应一个中断的优先级。n{0,23} */
#define xxx_GICD_REGS_GICD_IPRIORITYR_SPI_NS_184_REG       (xxx_GICD_REGS_BASE + 0x700)  /* 每8bit对应一个中断的优先级。n{0,23} */
#define xxx_GICD_REGS_GICD_IPRIORITYR_SPI_NS_185_REG       (xxx_GICD_REGS_BASE + 0x704)  /* 每8bit对应一个中断的优先级。n{0,23} */
#define xxx_GICD_REGS_GICD_IPRIORITYR_SPI_NS_186_REG       (xxx_GICD_REGS_BASE + 0x708)  /* 每8bit对应一个中断的优先级。n{0,23} */
#define xxx_GICD_REGS_GICD_IPRIORITYR_SPI_NS_187_REG       (xxx_GICD_REGS_BASE + 0x70C)  /* 每8bit对应一个中断的优先级。n{0,23} */
#define xxx_GICD_REGS_GICD_IPRIORITYR_SPI_NS_188_REG       (xxx_GICD_REGS_BASE + 0x710)  /* 每8bit对应一个中断的优先级。n{0,23} */
#define xxx_GICD_REGS_GICD_IPRIORITYR_SPI_NS_189_REG       (xxx_GICD_REGS_BASE + 0x714)  /* 每8bit对应一个中断的优先级。n{0,23} */
#define xxx_GICD_REGS_GICD_IPRIORITYR_SPI_NS_190_REG       (xxx_GICD_REGS_BASE + 0x718)  /* 每8bit对应一个中断的优先级。n{0,23} */
#define xxx_GICD_REGS_GICD_IPRIORITYR_SPI_NS_191_REG       (xxx_GICD_REGS_BASE + 0x71C)  /* 每8bit对应一个中断的优先级。n{0,23} */
#define xxx_GICD_REGS_GICD_IPRIORITYR_SPI_NS_192_REG       (xxx_GICD_REGS_BASE + 0x720)  /* 每8bit对应一个中断的优先级。n{0,23} */
#define xxx_GICD_REGS_GICD_IPRIORITYR_SPI_NS_193_REG       (xxx_GICD_REGS_BASE + 0x724)  /* 每8bit对应一个中断的优先级。n{0,23} */
#define xxx_GICD_REGS_GICD_IPRIORITYR_SPI_NS_194_REG       (xxx_GICD_REGS_BASE + 0x728)  /* 每8bit对应一个中断的优先级。n{0,23} */
#define xxx_GICD_REGS_GICD_IPRIORITYR_SPI_NS_195_REG       (xxx_GICD_REGS_BASE + 0x72C)  /* 每8bit对应一个中断的优先级。n{0,23} */
#define xxx_GICD_REGS_GICD_IPRIORITYR_SPI_NS_196_REG       (xxx_GICD_REGS_BASE + 0x730)  /* 每8bit对应一个中断的优先级。n{0,23} */
#define xxx_GICD_REGS_GICD_IPRIORITYR_SPI_NS_197_REG       (xxx_GICD_REGS_BASE + 0x734)  /* 每8bit对应一个中断的优先级。n{0,23} */
#define xxx_GICD_REGS_GICD_IPRIORITYR_SPI_NS_198_REG       (xxx_GICD_REGS_BASE + 0x738)  /* 每8bit对应一个中断的优先级。n{0,23} */
#define xxx_GICD_REGS_GICD_IPRIORITYR_SPI_NS_199_REG       (xxx_GICD_REGS_BASE + 0x73C)  /* 每8bit对应一个中断的优先级。n{0,23} */
#define xxx_GICD_REGS_GICD_IPRIORITYR_SPI_NS_200_REG       (xxx_GICD_REGS_BASE + 0x740)  /* 每8bit对应一个中断的优先级。n{0,23} */
#define xxx_GICD_REGS_GICD_IPRIORITYR_SPI_NS_201_REG       (xxx_GICD_REGS_BASE + 0x744)  /* 每8bit对应一个中断的优先级。n{0,23} */
#define xxx_GICD_REGS_GICD_IPRIORITYR_SPI_NS_202_REG       (xxx_GICD_REGS_BASE + 0x748)  /* 每8bit对应一个中断的优先级。n{0,23} */
#define xxx_GICD_REGS_GICD_IPRIORITYR_SPI_NS_203_REG       (xxx_GICD_REGS_BASE + 0x74C)  /* 每8bit对应一个中断的优先级。n{0,23} */
#define xxx_GICD_REGS_GICD_IPRIORITYR_SPI_NS_204_REG       (xxx_GICD_REGS_BASE + 0x750)  /* 每8bit对应一个中断的优先级。n{0,23} */
#define xxx_GICD_REGS_GICD_IPRIORITYR_SPI_NS_205_REG       (xxx_GICD_REGS_BASE + 0x754)  /* 每8bit对应一个中断的优先级。n{0,23} */
#define xxx_GICD_REGS_GICD_IPRIORITYR_SPI_NS_206_REG       (xxx_GICD_REGS_BASE + 0x758)  /* 每8bit对应一个中断的优先级。n{0,23} */
#define xxx_GICD_REGS_GICD_IPRIORITYR_SPI_NS_207_REG       (xxx_GICD_REGS_BASE + 0x75C)  /* 每8bit对应一个中断的优先级。n{0,23} */
#define xxx_GICD_REGS_GICD_IPRIORITYR_SPI_NS_208_REG       (xxx_GICD_REGS_BASE + 0x760)  /* 每8bit对应一个中断的优先级。n{0,23} */
#define xxx_GICD_REGS_GICD_IPRIORITYR_SPI_NS_209_REG       (xxx_GICD_REGS_BASE + 0x764)  /* 每8bit对应一个中断的优先级。n{0,23} */
#define xxx_GICD_REGS_GICD_IPRIORITYR_SPI_NS_210_REG       (xxx_GICD_REGS_BASE + 0x768)  /* 每8bit对应一个中断的优先级。n{0,23} */
#define xxx_GICD_REGS_GICD_IPRIORITYR_SPI_NS_211_REG       (xxx_GICD_REGS_BASE + 0x76C)  /* 每8bit对应一个中断的优先级。n{0,23} */
#define xxx_GICD_REGS_GICD_IPRIORITYR_SPI_NS_212_REG       (xxx_GICD_REGS_BASE + 0x770)  /* 每8bit对应一个中断的优先级。n{0,23} */
#define xxx_GICD_REGS_GICD_IPRIORITYR_SPI_NS_213_REG       (xxx_GICD_REGS_BASE + 0x774)  /* 每8bit对应一个中断的优先级。n{0,23} */
#define xxx_GICD_REGS_GICD_IPRIORITYR_SPI_NS_214_REG       (xxx_GICD_REGS_BASE + 0x778)  /* 每8bit对应一个中断的优先级。n{0,23} */
#define xxx_GICD_REGS_GICD_IPRIORITYR_SPI_NS_215_REG       (xxx_GICD_REGS_BASE + 0x77C)  /* 每8bit对应一个中断的优先级。n{0,23} */
#define xxx_GICD_REGS_GICD_IPRIORITYR_SPI_NS_216_REG       (xxx_GICD_REGS_BASE + 0x780)  /* 每8bit对应一个中断的优先级。n{0,23} */
#define xxx_GICD_REGS_GICD_IPRIORITYR_SPI_NS_217_REG       (xxx_GICD_REGS_BASE + 0x784)  /* 每8bit对应一个中断的优先级。n{0,23} */
#define xxx_GICD_REGS_GICD_IPRIORITYR_SPI_NS_218_REG       (xxx_GICD_REGS_BASE + 0x788)  /* 每8bit对应一个中断的优先级。n{0,23} */
#define xxx_GICD_REGS_GICD_IPRIORITYR_SPI_NS_219_REG       (xxx_GICD_REGS_BASE + 0x78C)  /* 每8bit对应一个中断的优先级。n{0,23} */
#define xxx_GICD_REGS_GICD_IPRIORITYR_SPI_NS_220_REG       (xxx_GICD_REGS_BASE + 0x790)  /* 每8bit对应一个中断的优先级。n{0,23} */
#define xxx_GICD_REGS_GICD_IPRIORITYR_SPI_NS_221_REG       (xxx_GICD_REGS_BASE + 0x794)  /* 每8bit对应一个中断的优先级。n{0,23} */
#define xxx_GICD_REGS_GICD_IPRIORITYR_SPI_NS_222_REG       (xxx_GICD_REGS_BASE + 0x798)  /* 每8bit对应一个中断的优先级。n{0,23} */
#define xxx_GICD_REGS_GICD_IPRIORITYR_SPI_NS_223_REG       (xxx_GICD_REGS_BASE + 0x79C)  /* 每8bit对应一个中断的优先级。n{0,23} */
#define xxx_GICD_REGS_GICD_IPRIORITYR_SPI_NS_224_REG       (xxx_GICD_REGS_BASE + 0x7A0)  /* 每8bit对应一个中断的优先级。n{0,23} */
#define xxx_GICD_REGS_GICD_IPRIORITYR_SPI_NS_225_REG       (xxx_GICD_REGS_BASE + 0x7A4)  /* 每8bit对应一个中断的优先级。n{0,23} */
#define xxx_GICD_REGS_GICD_IPRIORITYR_SPI_NS_226_REG       (xxx_GICD_REGS_BASE + 0x7A8)  /* 每8bit对应一个中断的优先级。n{0,23} */
#define xxx_GICD_REGS_GICD_IPRIORITYR_SPI_NS_227_REG       (xxx_GICD_REGS_BASE + 0x7AC)  /* 每8bit对应一个中断的优先级。n{0,23} */
#define xxx_GICD_REGS_GICD_IPRIORITYR_SPI_NS_228_REG       (xxx_GICD_REGS_BASE + 0x7B0)  /* 每8bit对应一个中断的优先级。n{0,23} */
#define xxx_GICD_REGS_GICD_IPRIORITYR_SPI_NS_229_REG       (xxx_GICD_REGS_BASE + 0x7B4)  /* 每8bit对应一个中断的优先级。n{0,23} */
#define xxx_GICD_REGS_GICD_IPRIORITYR_SPI_NS_230_REG       (xxx_GICD_REGS_BASE + 0x7B8)  /* 每8bit对应一个中断的优先级。n{0,23} */
#define xxx_GICD_REGS_GICD_IPRIORITYR_SPI_NS_231_REG       (xxx_GICD_REGS_BASE + 0x7BC)  /* 每8bit对应一个中断的优先级。n{0,23} */
#define xxx_GICD_REGS_GICD_IPRIORITYR_SPI_NS_232_REG       (xxx_GICD_REGS_BASE + 0x7C0)  /* 每8bit对应一个中断的优先级。n{0,23} */
#define xxx_GICD_REGS_GICD_IPRIORITYR_SPI_NS_233_REG       (xxx_GICD_REGS_BASE + 0x7C4)  /* 每8bit对应一个中断的优先级。n{0,23} */
#define xxx_GICD_REGS_GICD_IPRIORITYR_SPI_NS_234_REG       (xxx_GICD_REGS_BASE + 0x7C8)  /* 每8bit对应一个中断的优先级。n{0,23} */
#define xxx_GICD_REGS_GICD_IPRIORITYR_SPI_NS_235_REG       (xxx_GICD_REGS_BASE + 0x7CC)  /* 每8bit对应一个中断的优先级。n{0,23} */
#define xxx_GICD_REGS_GICD_IPRIORITYR_SPI_NS_236_REG       (xxx_GICD_REGS_BASE + 0x7D0)  /* 每8bit对应一个中断的优先级。n{0,23} */
#define xxx_GICD_REGS_GICD_IPRIORITYR_SPI_NS_237_REG       (xxx_GICD_REGS_BASE + 0x7D4)  /* 每8bit对应一个中断的优先级。n{0,23} */
#define xxx_GICD_REGS_GICD_IPRIORITYR_SPI_NS_238_REG       (xxx_GICD_REGS_BASE + 0x7D8)  /* 每8bit对应一个中断的优先级。n{0,23} */
#define xxx_GICD_REGS_GICD_IPRIORITYR_SPI_NS_239_REG       (xxx_GICD_REGS_BASE + 0x7DC)  /* 每8bit对应一个中断的优先级。n{0,23} */
#define xxx_GICD_REGS_GICD_IPRIORITYR_SPI_NS_240_REG       (xxx_GICD_REGS_BASE + 0x7E0)  /* 每8bit对应一个中断的优先级。n{0,23} */
#define xxx_GICD_REGS_GICD_IPRIORITYR_SPI_NS_241_REG       (xxx_GICD_REGS_BASE + 0x7E4)  /* 每8bit对应一个中断的优先级。n{0,23} */
#define xxx_GICD_REGS_GICD_IPRIORITYR_SPI_NS_242_REG       (xxx_GICD_REGS_BASE + 0x7E8)  /* 每8bit对应一个中断的优先级。n{0,23} */
#define xxx_GICD_REGS_GICD_IPRIORITYR_SPI_NS_243_REG       (xxx_GICD_REGS_BASE + 0x7EC)  /* 每8bit对应一个中断的优先级。n{0,23} */
#define xxx_GICD_REGS_GICD_IPRIORITYR_SPI_NS_244_REG       (xxx_GICD_REGS_BASE + 0x7F0)  /* 每8bit对应一个中断的优先级。n{0,23} */
#define xxx_GICD_REGS_GICD_IPRIORITYR_SPI_NS_245_REG       (xxx_GICD_REGS_BASE + 0x7F4)  /* 每8bit对应一个中断的优先级。n{0,23} */
#define xxx_GICD_REGS_GICD_IPRIORITYR_SPI_NS_246_REG       (xxx_GICD_REGS_BASE + 0x7F8)  /* 每8bit对应一个中断的优先级。n{0,23} */
#define xxx_GICD_REGS_GICD_IPRIORITYR_SPI_NS_247_REG       (xxx_GICD_REGS_BASE + 0x7FC)  /* 每8bit对应一个中断的优先级。n{0,23} */
#define xxx_GICD_REGS_GICD_ITARGETSR_SPI_0_REG             (xxx_GICD_REGS_BASE + 0x880)  /* SPI_BLOCK每32bit对应一个中断的所有相应的CPU Interface。h{0,223} */
#define xxx_GICD_REGS_GICD_ITARGETSR_SPI_1_REG             (xxx_GICD_REGS_BASE + 0x884)  /* SPI_BLOCK每32bit对应一个中断的所有相应的CPU Interface。h{0,223} */
#define xxx_GICD_REGS_GICD_ITARGETSR_SPI_2_REG             (xxx_GICD_REGS_BASE + 0x888)  /* SPI_BLOCK每32bit对应一个中断的所有相应的CPU Interface。h{0,223} */
#define xxx_GICD_REGS_GICD_ITARGETSR_SPI_3_REG             (xxx_GICD_REGS_BASE + 0x88C)  /* SPI_BLOCK每32bit对应一个中断的所有相应的CPU Interface。h{0,223} */
#define xxx_GICD_REGS_GICD_ITARGETSR_SPI_4_REG             (xxx_GICD_REGS_BASE + 0x890)  /* SPI_BLOCK每32bit对应一个中断的所有相应的CPU Interface。h{0,223} */
#define xxx_GICD_REGS_GICD_ITARGETSR_SPI_5_REG             (xxx_GICD_REGS_BASE + 0x894)  /* SPI_BLOCK每32bit对应一个中断的所有相应的CPU Interface。h{0,223} */
#define xxx_GICD_REGS_GICD_ITARGETSR_SPI_6_REG             (xxx_GICD_REGS_BASE + 0x898)  /* SPI_BLOCK每32bit对应一个中断的所有相应的CPU Interface。h{0,223} */
#define xxx_GICD_REGS_GICD_ITARGETSR_SPI_7_REG             (xxx_GICD_REGS_BASE + 0x89C)  /* SPI_BLOCK每32bit对应一个中断的所有相应的CPU Interface。h{0,223} */
#define xxx_GICD_REGS_GICD_ITARGETSR_SPI_8_REG             (xxx_GICD_REGS_BASE + 0x8A0)  /* SPI_BLOCK每32bit对应一个中断的所有相应的CPU Interface。h{0,223} */
#define xxx_GICD_REGS_GICD_ITARGETSR_SPI_9_REG             (xxx_GICD_REGS_BASE + 0x8A4)  /* SPI_BLOCK每32bit对应一个中断的所有相应的CPU Interface。h{0,223} */
#define xxx_GICD_REGS_GICD_ITARGETSR_SPI_10_REG            (xxx_GICD_REGS_BASE + 0x8A8)  /* SPI_BLOCK每32bit对应一个中断的所有相应的CPU Interface。h{0,223} */
#define xxx_GICD_REGS_GICD_ITARGETSR_SPI_11_REG            (xxx_GICD_REGS_BASE + 0x8AC)  /* SPI_BLOCK每32bit对应一个中断的所有相应的CPU Interface。h{0,223} */
#define xxx_GICD_REGS_GICD_ITARGETSR_SPI_12_REG            (xxx_GICD_REGS_BASE + 0x8B0)  /* SPI_BLOCK每32bit对应一个中断的所有相应的CPU Interface。h{0,223} */
#define xxx_GICD_REGS_GICD_ITARGETSR_SPI_13_REG            (xxx_GICD_REGS_BASE + 0x8B4)  /* SPI_BLOCK每32bit对应一个中断的所有相应的CPU Interface。h{0,223} */
#define xxx_GICD_REGS_GICD_ITARGETSR_SPI_14_REG            (xxx_GICD_REGS_BASE + 0x8B8)  /* SPI_BLOCK每32bit对应一个中断的所有相应的CPU Interface。h{0,223} */
#define xxx_GICD_REGS_GICD_ITARGETSR_SPI_15_REG            (xxx_GICD_REGS_BASE + 0x8BC)  /* SPI_BLOCK每32bit对应一个中断的所有相应的CPU Interface。h{0,223} */
#define xxx_GICD_REGS_GICD_ITARGETSR_SPI_16_REG            (xxx_GICD_REGS_BASE + 0x8C0)  /* SPI_BLOCK每32bit对应一个中断的所有相应的CPU Interface。h{0,223} */
#define xxx_GICD_REGS_GICD_ITARGETSR_SPI_17_REG            (xxx_GICD_REGS_BASE + 0x8C4)  /* SPI_BLOCK每32bit对应一个中断的所有相应的CPU Interface。h{0,223} */
#define xxx_GICD_REGS_GICD_ITARGETSR_SPI_18_REG            (xxx_GICD_REGS_BASE + 0x8C8)  /* SPI_BLOCK每32bit对应一个中断的所有相应的CPU Interface。h{0,223} */
#define xxx_GICD_REGS_GICD_ITARGETSR_SPI_19_REG            (xxx_GICD_REGS_BASE + 0x8CC)  /* SPI_BLOCK每32bit对应一个中断的所有相应的CPU Interface。h{0,223} */
#define xxx_GICD_REGS_GICD_ITARGETSR_SPI_20_REG            (xxx_GICD_REGS_BASE + 0x8D0)  /* SPI_BLOCK每32bit对应一个中断的所有相应的CPU Interface。h{0,223} */
#define xxx_GICD_REGS_GICD_ITARGETSR_SPI_21_REG            (xxx_GICD_REGS_BASE + 0x8D4)  /* SPI_BLOCK每32bit对应一个中断的所有相应的CPU Interface。h{0,223} */
#define xxx_GICD_REGS_GICD_ITARGETSR_SPI_22_REG            (xxx_GICD_REGS_BASE + 0x8D8)  /* SPI_BLOCK每32bit对应一个中断的所有相应的CPU Interface。h{0,223} */
#define xxx_GICD_REGS_GICD_ITARGETSR_SPI_23_REG            (xxx_GICD_REGS_BASE + 0x8DC)  /* SPI_BLOCK每32bit对应一个中断的所有相应的CPU Interface。h{0,223} */
#define xxx_GICD_REGS_GICD_ITARGETSR_SPI_24_REG            (xxx_GICD_REGS_BASE + 0x8E0)  /* SPI_BLOCK每32bit对应一个中断的所有相应的CPU Interface。h{0,223} */
#define xxx_GICD_REGS_GICD_ITARGETSR_SPI_25_REG            (xxx_GICD_REGS_BASE + 0x8E4)  /* SPI_BLOCK每32bit对应一个中断的所有相应的CPU Interface。h{0,223} */
#define xxx_GICD_REGS_GICD_ITARGETSR_SPI_26_REG            (xxx_GICD_REGS_BASE + 0x8E8)  /* SPI_BLOCK每32bit对应一个中断的所有相应的CPU Interface。h{0,223} */
#define xxx_GICD_REGS_GICD_ITARGETSR_SPI_27_REG            (xxx_GICD_REGS_BASE + 0x8EC)  /* SPI_BLOCK每32bit对应一个中断的所有相应的CPU Interface。h{0,223} */
#define xxx_GICD_REGS_GICD_ITARGETSR_SPI_28_REG            (xxx_GICD_REGS_BASE + 0x8F0)  /* SPI_BLOCK每32bit对应一个中断的所有相应的CPU Interface。h{0,223} */
#define xxx_GICD_REGS_GICD_ITARGETSR_SPI_29_REG            (xxx_GICD_REGS_BASE + 0x8F4)  /* SPI_BLOCK每32bit对应一个中断的所有相应的CPU Interface。h{0,223} */
#define xxx_GICD_REGS_GICD_ITARGETSR_SPI_30_REG            (xxx_GICD_REGS_BASE + 0x8F8)  /* SPI_BLOCK每32bit对应一个中断的所有相应的CPU Interface。h{0,223} */
#define xxx_GICD_REGS_GICD_ITARGETSR_SPI_31_REG            (xxx_GICD_REGS_BASE + 0x8FC)  /* SPI_BLOCK每32bit对应一个中断的所有相应的CPU Interface。h{0,223} */
#define xxx_GICD_REGS_GICD_ITARGETSR_SPI_32_REG            (xxx_GICD_REGS_BASE + 0x900)  /* SPI_BLOCK每32bit对应一个中断的所有相应的CPU Interface。h{0,223} */
#define xxx_GICD_REGS_GICD_ITARGETSR_SPI_33_REG            (xxx_GICD_REGS_BASE + 0x904)  /* SPI_BLOCK每32bit对应一个中断的所有相应的CPU Interface。h{0,223} */
#define xxx_GICD_REGS_GICD_ITARGETSR_SPI_34_REG            (xxx_GICD_REGS_BASE + 0x908)  /* SPI_BLOCK每32bit对应一个中断的所有相应的CPU Interface。h{0,223} */
#define xxx_GICD_REGS_GICD_ITARGETSR_SPI_35_REG            (xxx_GICD_REGS_BASE + 0x90C)  /* SPI_BLOCK每32bit对应一个中断的所有相应的CPU Interface。h{0,223} */
#define xxx_GICD_REGS_GICD_ITARGETSR_SPI_36_REG            (xxx_GICD_REGS_BASE + 0x910)  /* SPI_BLOCK每32bit对应一个中断的所有相应的CPU Interface。h{0,223} */
#define xxx_GICD_REGS_GICD_ITARGETSR_SPI_37_REG            (xxx_GICD_REGS_BASE + 0x914)  /* SPI_BLOCK每32bit对应一个中断的所有相应的CPU Interface。h{0,223} */
#define xxx_GICD_REGS_GICD_ITARGETSR_SPI_38_REG            (xxx_GICD_REGS_BASE + 0x918)  /* SPI_BLOCK每32bit对应一个中断的所有相应的CPU Interface。h{0,223} */
#define xxx_GICD_REGS_GICD_ITARGETSR_SPI_39_REG            (xxx_GICD_REGS_BASE + 0x91C)  /* SPI_BLOCK每32bit对应一个中断的所有相应的CPU Interface。h{0,223} */
#define xxx_GICD_REGS_GICD_ITARGETSR_SPI_40_REG            (xxx_GICD_REGS_BASE + 0x920)  /* SPI_BLOCK每32bit对应一个中断的所有相应的CPU Interface。h{0,223} */
#define xxx_GICD_REGS_GICD_ITARGETSR_SPI_41_REG            (xxx_GICD_REGS_BASE + 0x924)  /* SPI_BLOCK每32bit对应一个中断的所有相应的CPU Interface。h{0,223} */
#define xxx_GICD_REGS_GICD_ITARGETSR_SPI_42_REG            (xxx_GICD_REGS_BASE + 0x928)  /* SPI_BLOCK每32bit对应一个中断的所有相应的CPU Interface。h{0,223} */
#define xxx_GICD_REGS_GICD_ITARGETSR_SPI_43_REG            (xxx_GICD_REGS_BASE + 0x92C)  /* SPI_BLOCK每32bit对应一个中断的所有相应的CPU Interface。h{0,223} */
#define xxx_GICD_REGS_GICD_ITARGETSR_SPI_44_REG            (xxx_GICD_REGS_BASE + 0x930)  /* SPI_BLOCK每32bit对应一个中断的所有相应的CPU Interface。h{0,223} */
#define xxx_GICD_REGS_GICD_ITARGETSR_SPI_45_REG            (xxx_GICD_REGS_BASE + 0x934)  /* SPI_BLOCK每32bit对应一个中断的所有相应的CPU Interface。h{0,223} */
#define xxx_GICD_REGS_GICD_ITARGETSR_SPI_46_REG            (xxx_GICD_REGS_BASE + 0x938)  /* SPI_BLOCK每32bit对应一个中断的所有相应的CPU Interface。h{0,223} */
#define xxx_GICD_REGS_GICD_ITARGETSR_SPI_47_REG            (xxx_GICD_REGS_BASE + 0x93C)  /* SPI_BLOCK每32bit对应一个中断的所有相应的CPU Interface。h{0,223} */
#define xxx_GICD_REGS_GICD_ITARGETSR_SPI_48_REG            (xxx_GICD_REGS_BASE + 0x940)  /* SPI_BLOCK每32bit对应一个中断的所有相应的CPU Interface。h{0,223} */
#define xxx_GICD_REGS_GICD_ITARGETSR_SPI_49_REG            (xxx_GICD_REGS_BASE + 0x944)  /* SPI_BLOCK每32bit对应一个中断的所有相应的CPU Interface。h{0,223} */
#define xxx_GICD_REGS_GICD_ITARGETSR_SPI_50_REG            (xxx_GICD_REGS_BASE + 0x948)  /* SPI_BLOCK每32bit对应一个中断的所有相应的CPU Interface。h{0,223} */
#define xxx_GICD_REGS_GICD_ITARGETSR_SPI_51_REG            (xxx_GICD_REGS_BASE + 0x94C)  /* SPI_BLOCK每32bit对应一个中断的所有相应的CPU Interface。h{0,223} */
#define xxx_GICD_REGS_GICD_ITARGETSR_SPI_52_REG            (xxx_GICD_REGS_BASE + 0x950)  /* SPI_BLOCK每32bit对应一个中断的所有相应的CPU Interface。h{0,223} */
#define xxx_GICD_REGS_GICD_ITARGETSR_SPI_53_REG            (xxx_GICD_REGS_BASE + 0x954)  /* SPI_BLOCK每32bit对应一个中断的所有相应的CPU Interface。h{0,223} */
#define xxx_GICD_REGS_GICD_ITARGETSR_SPI_54_REG            (xxx_GICD_REGS_BASE + 0x958)  /* SPI_BLOCK每32bit对应一个中断的所有相应的CPU Interface。h{0,223} */
#define xxx_GICD_REGS_GICD_ITARGETSR_SPI_55_REG            (xxx_GICD_REGS_BASE + 0x95C)  /* SPI_BLOCK每32bit对应一个中断的所有相应的CPU Interface。h{0,223} */
#define xxx_GICD_REGS_GICD_ITARGETSR_SPI_56_REG            (xxx_GICD_REGS_BASE + 0x960)  /* SPI_BLOCK每32bit对应一个中断的所有相应的CPU Interface。h{0,223} */
#define xxx_GICD_REGS_GICD_ITARGETSR_SPI_57_REG            (xxx_GICD_REGS_BASE + 0x964)  /* SPI_BLOCK每32bit对应一个中断的所有相应的CPU Interface。h{0,223} */
#define xxx_GICD_REGS_GICD_ITARGETSR_SPI_58_REG            (xxx_GICD_REGS_BASE + 0x968)  /* SPI_BLOCK每32bit对应一个中断的所有相应的CPU Interface。h{0,223} */
#define xxx_GICD_REGS_GICD_ITARGETSR_SPI_59_REG            (xxx_GICD_REGS_BASE + 0x96C)  /* SPI_BLOCK每32bit对应一个中断的所有相应的CPU Interface。h{0,223} */
#define xxx_GICD_REGS_GICD_ITARGETSR_SPI_60_REG            (xxx_GICD_REGS_BASE + 0x970)  /* SPI_BLOCK每32bit对应一个中断的所有相应的CPU Interface。h{0,223} */
#define xxx_GICD_REGS_GICD_ITARGETSR_SPI_61_REG            (xxx_GICD_REGS_BASE + 0x974)  /* SPI_BLOCK每32bit对应一个中断的所有相应的CPU Interface。h{0,223} */
#define xxx_GICD_REGS_GICD_ITARGETSR_SPI_62_REG            (xxx_GICD_REGS_BASE + 0x978)  /* SPI_BLOCK每32bit对应一个中断的所有相应的CPU Interface。h{0,223} */
#define xxx_GICD_REGS_GICD_ITARGETSR_SPI_63_REG            (xxx_GICD_REGS_BASE + 0x97C)  /* SPI_BLOCK每32bit对应一个中断的所有相应的CPU Interface。h{0,223} */
#define xxx_GICD_REGS_GICD_ITARGETSR_SPI_64_REG            (xxx_GICD_REGS_BASE + 0x980)  /* SPI_BLOCK每32bit对应一个中断的所有相应的CPU Interface。h{0,223} */
#define xxx_GICD_REGS_GICD_ITARGETSR_SPI_65_REG            (xxx_GICD_REGS_BASE + 0x984)  /* SPI_BLOCK每32bit对应一个中断的所有相应的CPU Interface。h{0,223} */
#define xxx_GICD_REGS_GICD_ITARGETSR_SPI_66_REG            (xxx_GICD_REGS_BASE + 0x988)  /* SPI_BLOCK每32bit对应一个中断的所有相应的CPU Interface。h{0,223} */
#define xxx_GICD_REGS_GICD_ITARGETSR_SPI_67_REG            (xxx_GICD_REGS_BASE + 0x98C)  /* SPI_BLOCK每32bit对应一个中断的所有相应的CPU Interface。h{0,223} */
#define xxx_GICD_REGS_GICD_ITARGETSR_SPI_68_REG            (xxx_GICD_REGS_BASE + 0x990)  /* SPI_BLOCK每32bit对应一个中断的所有相应的CPU Interface。h{0,223} */
#define xxx_GICD_REGS_GICD_ITARGETSR_SPI_69_REG            (xxx_GICD_REGS_BASE + 0x994)  /* SPI_BLOCK每32bit对应一个中断的所有相应的CPU Interface。h{0,223} */
#define xxx_GICD_REGS_GICD_ITARGETSR_SPI_70_REG            (xxx_GICD_REGS_BASE + 0x998)  /* SPI_BLOCK每32bit对应一个中断的所有相应的CPU Interface。h{0,223} */
#define xxx_GICD_REGS_GICD_ITARGETSR_SPI_71_REG            (xxx_GICD_REGS_BASE + 0x99C)  /* SPI_BLOCK每32bit对应一个中断的所有相应的CPU Interface。h{0,223} */
#define xxx_GICD_REGS_GICD_ITARGETSR_SPI_72_REG            (xxx_GICD_REGS_BASE + 0x9A0)  /* SPI_BLOCK每32bit对应一个中断的所有相应的CPU Interface。h{0,223} */
#define xxx_GICD_REGS_GICD_ITARGETSR_SPI_73_REG            (xxx_GICD_REGS_BASE + 0x9A4)  /* SPI_BLOCK每32bit对应一个中断的所有相应的CPU Interface。h{0,223} */
#define xxx_GICD_REGS_GICD_ITARGETSR_SPI_74_REG            (xxx_GICD_REGS_BASE + 0x9A8)  /* SPI_BLOCK每32bit对应一个中断的所有相应的CPU Interface。h{0,223} */
#define xxx_GICD_REGS_GICD_ITARGETSR_SPI_75_REG            (xxx_GICD_REGS_BASE + 0x9AC)  /* SPI_BLOCK每32bit对应一个中断的所有相应的CPU Interface。h{0,223} */
#define xxx_GICD_REGS_GICD_ITARGETSR_SPI_76_REG            (xxx_GICD_REGS_BASE + 0x9B0)  /* SPI_BLOCK每32bit对应一个中断的所有相应的CPU Interface。h{0,223} */
#define xxx_GICD_REGS_GICD_ITARGETSR_SPI_77_REG            (xxx_GICD_REGS_BASE + 0x9B4)  /* SPI_BLOCK每32bit对应一个中断的所有相应的CPU Interface。h{0,223} */
#define xxx_GICD_REGS_GICD_ITARGETSR_SPI_78_REG            (xxx_GICD_REGS_BASE + 0x9B8)  /* SPI_BLOCK每32bit对应一个中断的所有相应的CPU Interface。h{0,223} */
#define xxx_GICD_REGS_GICD_ITARGETSR_SPI_79_REG            (xxx_GICD_REGS_BASE + 0x9BC)  /* SPI_BLOCK每32bit对应一个中断的所有相应的CPU Interface。h{0,223} */
#define xxx_GICD_REGS_GICD_ITARGETSR_SPI_80_REG            (xxx_GICD_REGS_BASE + 0x9C0)  /* SPI_BLOCK每32bit对应一个中断的所有相应的CPU Interface。h{0,223} */
#define xxx_GICD_REGS_GICD_ITARGETSR_SPI_81_REG            (xxx_GICD_REGS_BASE + 0x9C4)  /* SPI_BLOCK每32bit对应一个中断的所有相应的CPU Interface。h{0,223} */
#define xxx_GICD_REGS_GICD_ITARGETSR_SPI_82_REG            (xxx_GICD_REGS_BASE + 0x9C8)  /* SPI_BLOCK每32bit对应一个中断的所有相应的CPU Interface。h{0,223} */
#define xxx_GICD_REGS_GICD_ITARGETSR_SPI_83_REG            (xxx_GICD_REGS_BASE + 0x9CC)  /* SPI_BLOCK每32bit对应一个中断的所有相应的CPU Interface。h{0,223} */
#define xxx_GICD_REGS_GICD_ITARGETSR_SPI_84_REG            (xxx_GICD_REGS_BASE + 0x9D0)  /* SPI_BLOCK每32bit对应一个中断的所有相应的CPU Interface。h{0,223} */
#define xxx_GICD_REGS_GICD_ITARGETSR_SPI_85_REG            (xxx_GICD_REGS_BASE + 0x9D4)  /* SPI_BLOCK每32bit对应一个中断的所有相应的CPU Interface。h{0,223} */
#define xxx_GICD_REGS_GICD_ITARGETSR_SPI_86_REG            (xxx_GICD_REGS_BASE + 0x9D8)  /* SPI_BLOCK每32bit对应一个中断的所有相应的CPU Interface。h{0,223} */
#define xxx_GICD_REGS_GICD_ITARGETSR_SPI_87_REG            (xxx_GICD_REGS_BASE + 0x9DC)  /* SPI_BLOCK每32bit对应一个中断的所有相应的CPU Interface。h{0,223} */
#define xxx_GICD_REGS_GICD_ITARGETSR_SPI_88_REG            (xxx_GICD_REGS_BASE + 0x9E0)  /* SPI_BLOCK每32bit对应一个中断的所有相应的CPU Interface。h{0,223} */
#define xxx_GICD_REGS_GICD_ITARGETSR_SPI_89_REG            (xxx_GICD_REGS_BASE + 0x9E4)  /* SPI_BLOCK每32bit对应一个中断的所有相应的CPU Interface。h{0,223} */
#define xxx_GICD_REGS_GICD_ITARGETSR_SPI_90_REG            (xxx_GICD_REGS_BASE + 0x9E8)  /* SPI_BLOCK每32bit对应一个中断的所有相应的CPU Interface。h{0,223} */
#define xxx_GICD_REGS_GICD_ITARGETSR_SPI_91_REG            (xxx_GICD_REGS_BASE + 0x9EC)  /* SPI_BLOCK每32bit对应一个中断的所有相应的CPU Interface。h{0,223} */
#define xxx_GICD_REGS_GICD_ITARGETSR_SPI_92_REG            (xxx_GICD_REGS_BASE + 0x9F0)  /* SPI_BLOCK每32bit对应一个中断的所有相应的CPU Interface。h{0,223} */
#define xxx_GICD_REGS_GICD_ITARGETSR_SPI_93_REG            (xxx_GICD_REGS_BASE + 0x9F4)  /* SPI_BLOCK每32bit对应一个中断的所有相应的CPU Interface。h{0,223} */
#define xxx_GICD_REGS_GICD_ITARGETSR_SPI_94_REG            (xxx_GICD_REGS_BASE + 0x9F8)  /* SPI_BLOCK每32bit对应一个中断的所有相应的CPU Interface。h{0,223} */
#define xxx_GICD_REGS_GICD_ITARGETSR_SPI_95_REG            (xxx_GICD_REGS_BASE + 0x9FC)  /* SPI_BLOCK每32bit对应一个中断的所有相应的CPU Interface。h{0,223} */
#define xxx_GICD_REGS_GICD_ITARGETSR_SPI_96_REG            (xxx_GICD_REGS_BASE + 0xA00)  /* SPI_BLOCK每32bit对应一个中断的所有相应的CPU Interface。h{0,223} */
#define xxx_GICD_REGS_GICD_ITARGETSR_SPI_97_REG            (xxx_GICD_REGS_BASE + 0xA04)  /* SPI_BLOCK每32bit对应一个中断的所有相应的CPU Interface。h{0,223} */
#define xxx_GICD_REGS_GICD_ITARGETSR_SPI_98_REG            (xxx_GICD_REGS_BASE + 0xA08)  /* SPI_BLOCK每32bit对应一个中断的所有相应的CPU Interface。h{0,223} */
#define xxx_GICD_REGS_GICD_ITARGETSR_SPI_99_REG            (xxx_GICD_REGS_BASE + 0xA0C)  /* SPI_BLOCK每32bit对应一个中断的所有相应的CPU Interface。h{0,223} */
#define xxx_GICD_REGS_GICD_ITARGETSR_SPI_100_REG           (xxx_GICD_REGS_BASE + 0xA10)  /* SPI_BLOCK每32bit对应一个中断的所有相应的CPU Interface。h{0,223} */
#define xxx_GICD_REGS_GICD_ITARGETSR_SPI_101_REG           (xxx_GICD_REGS_BASE + 0xA14)  /* SPI_BLOCK每32bit对应一个中断的所有相应的CPU Interface。h{0,223} */
#define xxx_GICD_REGS_GICD_ITARGETSR_SPI_102_REG           (xxx_GICD_REGS_BASE + 0xA18)  /* SPI_BLOCK每32bit对应一个中断的所有相应的CPU Interface。h{0,223} */
#define xxx_GICD_REGS_GICD_ITARGETSR_SPI_103_REG           (xxx_GICD_REGS_BASE + 0xA1C)  /* SPI_BLOCK每32bit对应一个中断的所有相应的CPU Interface。h{0,223} */
#define xxx_GICD_REGS_GICD_ITARGETSR_SPI_104_REG           (xxx_GICD_REGS_BASE + 0xA20)  /* SPI_BLOCK每32bit对应一个中断的所有相应的CPU Interface。h{0,223} */
#define xxx_GICD_REGS_GICD_ITARGETSR_SPI_105_REG           (xxx_GICD_REGS_BASE + 0xA24)  /* SPI_BLOCK每32bit对应一个中断的所有相应的CPU Interface。h{0,223} */
#define xxx_GICD_REGS_GICD_ITARGETSR_SPI_106_REG           (xxx_GICD_REGS_BASE + 0xA28)  /* SPI_BLOCK每32bit对应一个中断的所有相应的CPU Interface。h{0,223} */
#define xxx_GICD_REGS_GICD_ITARGETSR_SPI_107_REG           (xxx_GICD_REGS_BASE + 0xA2C)  /* SPI_BLOCK每32bit对应一个中断的所有相应的CPU Interface。h{0,223} */
#define xxx_GICD_REGS_GICD_ITARGETSR_SPI_108_REG           (xxx_GICD_REGS_BASE + 0xA30)  /* SPI_BLOCK每32bit对应一个中断的所有相应的CPU Interface。h{0,223} */
#define xxx_GICD_REGS_GICD_ITARGETSR_SPI_109_REG           (xxx_GICD_REGS_BASE + 0xA34)  /* SPI_BLOCK每32bit对应一个中断的所有相应的CPU Interface。h{0,223} */
#define xxx_GICD_REGS_GICD_ITARGETSR_SPI_110_REG           (xxx_GICD_REGS_BASE + 0xA38)  /* SPI_BLOCK每32bit对应一个中断的所有相应的CPU Interface。h{0,223} */
#define xxx_GICD_REGS_GICD_ITARGETSR_SPI_111_REG           (xxx_GICD_REGS_BASE + 0xA3C)  /* SPI_BLOCK每32bit对应一个中断的所有相应的CPU Interface。h{0,223} */
#define xxx_GICD_REGS_GICD_ITARGETSR_SPI_112_REG           (xxx_GICD_REGS_BASE + 0xA40)  /* SPI_BLOCK每32bit对应一个中断的所有相应的CPU Interface。h{0,223} */
#define xxx_GICD_REGS_GICD_ITARGETSR_SPI_113_REG           (xxx_GICD_REGS_BASE + 0xA44)  /* SPI_BLOCK每32bit对应一个中断的所有相应的CPU Interface。h{0,223} */
#define xxx_GICD_REGS_GICD_ITARGETSR_SPI_114_REG           (xxx_GICD_REGS_BASE + 0xA48)  /* SPI_BLOCK每32bit对应一个中断的所有相应的CPU Interface。h{0,223} */
#define xxx_GICD_REGS_GICD_ITARGETSR_SPI_115_REG           (xxx_GICD_REGS_BASE + 0xA4C)  /* SPI_BLOCK每32bit对应一个中断的所有相应的CPU Interface。h{0,223} */
#define xxx_GICD_REGS_GICD_ITARGETSR_SPI_116_REG           (xxx_GICD_REGS_BASE + 0xA50)  /* SPI_BLOCK每32bit对应一个中断的所有相应的CPU Interface。h{0,223} */
#define xxx_GICD_REGS_GICD_ITARGETSR_SPI_117_REG           (xxx_GICD_REGS_BASE + 0xA54)  /* SPI_BLOCK每32bit对应一个中断的所有相应的CPU Interface。h{0,223} */
#define xxx_GICD_REGS_GICD_ITARGETSR_SPI_118_REG           (xxx_GICD_REGS_BASE + 0xA58)  /* SPI_BLOCK每32bit对应一个中断的所有相应的CPU Interface。h{0,223} */
#define xxx_GICD_REGS_GICD_ITARGETSR_SPI_119_REG           (xxx_GICD_REGS_BASE + 0xA5C)  /* SPI_BLOCK每32bit对应一个中断的所有相应的CPU Interface。h{0,223} */
#define xxx_GICD_REGS_GICD_ITARGETSR_SPI_120_REG           (xxx_GICD_REGS_BASE + 0xA60)  /* SPI_BLOCK每32bit对应一个中断的所有相应的CPU Interface。h{0,223} */
#define xxx_GICD_REGS_GICD_ITARGETSR_SPI_121_REG           (xxx_GICD_REGS_BASE + 0xA64)  /* SPI_BLOCK每32bit对应一个中断的所有相应的CPU Interface。h{0,223} */
#define xxx_GICD_REGS_GICD_ITARGETSR_SPI_122_REG           (xxx_GICD_REGS_BASE + 0xA68)  /* SPI_BLOCK每32bit对应一个中断的所有相应的CPU Interface。h{0,223} */
#define xxx_GICD_REGS_GICD_ITARGETSR_SPI_123_REG           (xxx_GICD_REGS_BASE + 0xA6C)  /* SPI_BLOCK每32bit对应一个中断的所有相应的CPU Interface。h{0,223} */
#define xxx_GICD_REGS_GICD_ITARGETSR_SPI_124_REG           (xxx_GICD_REGS_BASE + 0xA70)  /* SPI_BLOCK每32bit对应一个中断的所有相应的CPU Interface。h{0,223} */
#define xxx_GICD_REGS_GICD_ITARGETSR_SPI_125_REG           (xxx_GICD_REGS_BASE + 0xA74)  /* SPI_BLOCK每32bit对应一个中断的所有相应的CPU Interface。h{0,223} */
#define xxx_GICD_REGS_GICD_ITARGETSR_SPI_126_REG           (xxx_GICD_REGS_BASE + 0xA78)  /* SPI_BLOCK每32bit对应一个中断的所有相应的CPU Interface。h{0,223} */
#define xxx_GICD_REGS_GICD_ITARGETSR_SPI_127_REG           (xxx_GICD_REGS_BASE + 0xA7C)  /* SPI_BLOCK每32bit对应一个中断的所有相应的CPU Interface。h{0,223} */
#define xxx_GICD_REGS_GICD_ITARGETSR_SPI_128_REG           (xxx_GICD_REGS_BASE + 0xA80)  /* SPI_BLOCK每32bit对应一个中断的所有相应的CPU Interface。h{0,223} */
#define xxx_GICD_REGS_GICD_ITARGETSR_SPI_129_REG           (xxx_GICD_REGS_BASE + 0xA84)  /* SPI_BLOCK每32bit对应一个中断的所有相应的CPU Interface。h{0,223} */
#define xxx_GICD_REGS_GICD_ITARGETSR_SPI_130_REG           (xxx_GICD_REGS_BASE + 0xA88)  /* SPI_BLOCK每32bit对应一个中断的所有相应的CPU Interface。h{0,223} */
#define xxx_GICD_REGS_GICD_ITARGETSR_SPI_131_REG           (xxx_GICD_REGS_BASE + 0xA8C)  /* SPI_BLOCK每32bit对应一个中断的所有相应的CPU Interface。h{0,223} */
#define xxx_GICD_REGS_GICD_ITARGETSR_SPI_132_REG           (xxx_GICD_REGS_BASE + 0xA90)  /* SPI_BLOCK每32bit对应一个中断的所有相应的CPU Interface。h{0,223} */
#define xxx_GICD_REGS_GICD_ITARGETSR_SPI_133_REG           (xxx_GICD_REGS_BASE + 0xA94)  /* SPI_BLOCK每32bit对应一个中断的所有相应的CPU Interface。h{0,223} */
#define xxx_GICD_REGS_GICD_ITARGETSR_SPI_134_REG           (xxx_GICD_REGS_BASE + 0xA98)  /* SPI_BLOCK每32bit对应一个中断的所有相应的CPU Interface。h{0,223} */
#define xxx_GICD_REGS_GICD_ITARGETSR_SPI_135_REG           (xxx_GICD_REGS_BASE + 0xA9C)  /* SPI_BLOCK每32bit对应一个中断的所有相应的CPU Interface。h{0,223} */
#define xxx_GICD_REGS_GICD_ITARGETSR_SPI_136_REG           (xxx_GICD_REGS_BASE + 0xAA0)  /* SPI_BLOCK每32bit对应一个中断的所有相应的CPU Interface。h{0,223} */
#define xxx_GICD_REGS_GICD_ITARGETSR_SPI_137_REG           (xxx_GICD_REGS_BASE + 0xAA4)  /* SPI_BLOCK每32bit对应一个中断的所有相应的CPU Interface。h{0,223} */
#define xxx_GICD_REGS_GICD_ITARGETSR_SPI_138_REG           (xxx_GICD_REGS_BASE + 0xAA8)  /* SPI_BLOCK每32bit对应一个中断的所有相应的CPU Interface。h{0,223} */
#define xxx_GICD_REGS_GICD_ITARGETSR_SPI_139_REG           (xxx_GICD_REGS_BASE + 0xAAC)  /* SPI_BLOCK每32bit对应一个中断的所有相应的CPU Interface。h{0,223} */
#define xxx_GICD_REGS_GICD_ITARGETSR_SPI_140_REG           (xxx_GICD_REGS_BASE + 0xAB0)  /* SPI_BLOCK每32bit对应一个中断的所有相应的CPU Interface。h{0,223} */
#define xxx_GICD_REGS_GICD_ITARGETSR_SPI_141_REG           (xxx_GICD_REGS_BASE + 0xAB4)  /* SPI_BLOCK每32bit对应一个中断的所有相应的CPU Interface。h{0,223} */
#define xxx_GICD_REGS_GICD_ITARGETSR_SPI_142_REG           (xxx_GICD_REGS_BASE + 0xAB8)  /* SPI_BLOCK每32bit对应一个中断的所有相应的CPU Interface。h{0,223} */
#define xxx_GICD_REGS_GICD_ITARGETSR_SPI_143_REG           (xxx_GICD_REGS_BASE + 0xABC)  /* SPI_BLOCK每32bit对应一个中断的所有相应的CPU Interface。h{0,223} */
#define xxx_GICD_REGS_GICD_ITARGETSR_SPI_144_REG           (xxx_GICD_REGS_BASE + 0xAC0)  /* SPI_BLOCK每32bit对应一个中断的所有相应的CPU Interface。h{0,223} */
#define xxx_GICD_REGS_GICD_ITARGETSR_SPI_145_REG           (xxx_GICD_REGS_BASE + 0xAC4)  /* SPI_BLOCK每32bit对应一个中断的所有相应的CPU Interface。h{0,223} */
#define xxx_GICD_REGS_GICD_ITARGETSR_SPI_146_REG           (xxx_GICD_REGS_BASE + 0xAC8)  /* SPI_BLOCK每32bit对应一个中断的所有相应的CPU Interface。h{0,223} */
#define xxx_GICD_REGS_GICD_ITARGETSR_SPI_147_REG           (xxx_GICD_REGS_BASE + 0xACC)  /* SPI_BLOCK每32bit对应一个中断的所有相应的CPU Interface。h{0,223} */
#define xxx_GICD_REGS_GICD_ITARGETSR_SPI_148_REG           (xxx_GICD_REGS_BASE + 0xAD0)  /* SPI_BLOCK每32bit对应一个中断的所有相应的CPU Interface。h{0,223} */
#define xxx_GICD_REGS_GICD_ITARGETSR_SPI_149_REG           (xxx_GICD_REGS_BASE + 0xAD4)  /* SPI_BLOCK每32bit对应一个中断的所有相应的CPU Interface。h{0,223} */
#define xxx_GICD_REGS_GICD_ITARGETSR_SPI_150_REG           (xxx_GICD_REGS_BASE + 0xAD8)  /* SPI_BLOCK每32bit对应一个中断的所有相应的CPU Interface。h{0,223} */
#define xxx_GICD_REGS_GICD_ITARGETSR_SPI_151_REG           (xxx_GICD_REGS_BASE + 0xADC)  /* SPI_BLOCK每32bit对应一个中断的所有相应的CPU Interface。h{0,223} */
#define xxx_GICD_REGS_GICD_ITARGETSR_SPI_152_REG           (xxx_GICD_REGS_BASE + 0xAE0)  /* SPI_BLOCK每32bit对应一个中断的所有相应的CPU Interface。h{0,223} */
#define xxx_GICD_REGS_GICD_ITARGETSR_SPI_153_REG           (xxx_GICD_REGS_BASE + 0xAE4)  /* SPI_BLOCK每32bit对应一个中断的所有相应的CPU Interface。h{0,223} */
#define xxx_GICD_REGS_GICD_ITARGETSR_SPI_154_REG           (xxx_GICD_REGS_BASE + 0xAE8)  /* SPI_BLOCK每32bit对应一个中断的所有相应的CPU Interface。h{0,223} */
#define xxx_GICD_REGS_GICD_ITARGETSR_SPI_155_REG           (xxx_GICD_REGS_BASE + 0xAEC)  /* SPI_BLOCK每32bit对应一个中断的所有相应的CPU Interface。h{0,223} */
#define xxx_GICD_REGS_GICD_ITARGETSR_SPI_156_REG           (xxx_GICD_REGS_BASE + 0xAF0)  /* SPI_BLOCK每32bit对应一个中断的所有相应的CPU Interface。h{0,223} */
#define xxx_GICD_REGS_GICD_ITARGETSR_SPI_157_REG           (xxx_GICD_REGS_BASE + 0xAF4)  /* SPI_BLOCK每32bit对应一个中断的所有相应的CPU Interface。h{0,223} */
#define xxx_GICD_REGS_GICD_ITARGETSR_SPI_158_REG           (xxx_GICD_REGS_BASE + 0xAF8)  /* SPI_BLOCK每32bit对应一个中断的所有相应的CPU Interface。h{0,223} */
#define xxx_GICD_REGS_GICD_ITARGETSR_SPI_159_REG           (xxx_GICD_REGS_BASE + 0xAFC)  /* SPI_BLOCK每32bit对应一个中断的所有相应的CPU Interface。h{0,223} */
#define xxx_GICD_REGS_GICD_ITARGETSR_SPI_160_REG           (xxx_GICD_REGS_BASE + 0xB00)  /* SPI_BLOCK每32bit对应一个中断的所有相应的CPU Interface。h{0,223} */
#define xxx_GICD_REGS_GICD_ITARGETSR_SPI_161_REG           (xxx_GICD_REGS_BASE + 0xB04)  /* SPI_BLOCK每32bit对应一个中断的所有相应的CPU Interface。h{0,223} */
#define xxx_GICD_REGS_GICD_ITARGETSR_SPI_162_REG           (xxx_GICD_REGS_BASE + 0xB08)  /* SPI_BLOCK每32bit对应一个中断的所有相应的CPU Interface。h{0,223} */
#define xxx_GICD_REGS_GICD_ITARGETSR_SPI_163_REG           (xxx_GICD_REGS_BASE + 0xB0C)  /* SPI_BLOCK每32bit对应一个中断的所有相应的CPU Interface。h{0,223} */
#define xxx_GICD_REGS_GICD_ITARGETSR_SPI_164_REG           (xxx_GICD_REGS_BASE + 0xB10)  /* SPI_BLOCK每32bit对应一个中断的所有相应的CPU Interface。h{0,223} */
#define xxx_GICD_REGS_GICD_ITARGETSR_SPI_165_REG           (xxx_GICD_REGS_BASE + 0xB14)  /* SPI_BLOCK每32bit对应一个中断的所有相应的CPU Interface。h{0,223} */
#define xxx_GICD_REGS_GICD_ITARGETSR_SPI_166_REG           (xxx_GICD_REGS_BASE + 0xB18)  /* SPI_BLOCK每32bit对应一个中断的所有相应的CPU Interface。h{0,223} */
#define xxx_GICD_REGS_GICD_ITARGETSR_SPI_167_REG           (xxx_GICD_REGS_BASE + 0xB1C)  /* SPI_BLOCK每32bit对应一个中断的所有相应的CPU Interface。h{0,223} */
#define xxx_GICD_REGS_GICD_ITARGETSR_SPI_168_REG           (xxx_GICD_REGS_BASE + 0xB20)  /* SPI_BLOCK每32bit对应一个中断的所有相应的CPU Interface。h{0,223} */
#define xxx_GICD_REGS_GICD_ITARGETSR_SPI_169_REG           (xxx_GICD_REGS_BASE + 0xB24)  /* SPI_BLOCK每32bit对应一个中断的所有相应的CPU Interface。h{0,223} */
#define xxx_GICD_REGS_GICD_ITARGETSR_SPI_170_REG           (xxx_GICD_REGS_BASE + 0xB28)  /* SPI_BLOCK每32bit对应一个中断的所有相应的CPU Interface。h{0,223} */
#define xxx_GICD_REGS_GICD_ITARGETSR_SPI_171_REG           (xxx_GICD_REGS_BASE + 0xB2C)  /* SPI_BLOCK每32bit对应一个中断的所有相应的CPU Interface。h{0,223} */
#define xxx_GICD_REGS_GICD_ITARGETSR_SPI_172_REG           (xxx_GICD_REGS_BASE + 0xB30)  /* SPI_BLOCK每32bit对应一个中断的所有相应的CPU Interface。h{0,223} */
#define xxx_GICD_REGS_GICD_ITARGETSR_SPI_173_REG           (xxx_GICD_REGS_BASE + 0xB34)  /* SPI_BLOCK每32bit对应一个中断的所有相应的CPU Interface。h{0,223} */
#define xxx_GICD_REGS_GICD_ITARGETSR_SPI_174_REG           (xxx_GICD_REGS_BASE + 0xB38)  /* SPI_BLOCK每32bit对应一个中断的所有相应的CPU Interface。h{0,223} */
#define xxx_GICD_REGS_GICD_ITARGETSR_SPI_175_REG           (xxx_GICD_REGS_BASE + 0xB3C)  /* SPI_BLOCK每32bit对应一个中断的所有相应的CPU Interface。h{0,223} */
#define xxx_GICD_REGS_GICD_ITARGETSR_SPI_176_REG           (xxx_GICD_REGS_BASE + 0xB40)  /* SPI_BLOCK每32bit对应一个中断的所有相应的CPU Interface。h{0,223} */
#define xxx_GICD_REGS_GICD_ITARGETSR_SPI_177_REG           (xxx_GICD_REGS_BASE + 0xB44)  /* SPI_BLOCK每32bit对应一个中断的所有相应的CPU Interface。h{0,223} */
#define xxx_GICD_REGS_GICD_ITARGETSR_SPI_178_REG           (xxx_GICD_REGS_BASE + 0xB48)  /* SPI_BLOCK每32bit对应一个中断的所有相应的CPU Interface。h{0,223} */
#define xxx_GICD_REGS_GICD_ITARGETSR_SPI_179_REG           (xxx_GICD_REGS_BASE + 0xB4C)  /* SPI_BLOCK每32bit对应一个中断的所有相应的CPU Interface。h{0,223} */
#define xxx_GICD_REGS_GICD_ITARGETSR_SPI_180_REG           (xxx_GICD_REGS_BASE + 0xB50)  /* SPI_BLOCK每32bit对应一个中断的所有相应的CPU Interface。h{0,223} */
#define xxx_GICD_REGS_GICD_ITARGETSR_SPI_181_REG           (xxx_GICD_REGS_BASE + 0xB54)  /* SPI_BLOCK每32bit对应一个中断的所有相应的CPU Interface。h{0,223} */
#define xxx_GICD_REGS_GICD_ITARGETSR_SPI_182_REG           (xxx_GICD_REGS_BASE + 0xB58)  /* SPI_BLOCK每32bit对应一个中断的所有相应的CPU Interface。h{0,223} */
#define xxx_GICD_REGS_GICD_ITARGETSR_SPI_183_REG           (xxx_GICD_REGS_BASE + 0xB5C)  /* SPI_BLOCK每32bit对应一个中断的所有相应的CPU Interface。h{0,223} */
#define xxx_GICD_REGS_GICD_ITARGETSR_SPI_184_REG           (xxx_GICD_REGS_BASE + 0xB60)  /* SPI_BLOCK每32bit对应一个中断的所有相应的CPU Interface。h{0,223} */
#define xxx_GICD_REGS_GICD_ITARGETSR_SPI_185_REG           (xxx_GICD_REGS_BASE + 0xB64)  /* SPI_BLOCK每32bit对应一个中断的所有相应的CPU Interface。h{0,223} */
#define xxx_GICD_REGS_GICD_ITARGETSR_SPI_186_REG           (xxx_GICD_REGS_BASE + 0xB68)  /* SPI_BLOCK每32bit对应一个中断的所有相应的CPU Interface。h{0,223} */
#define xxx_GICD_REGS_GICD_ITARGETSR_SPI_187_REG           (xxx_GICD_REGS_BASE + 0xB6C)  /* SPI_BLOCK每32bit对应一个中断的所有相应的CPU Interface。h{0,223} */
#define xxx_GICD_REGS_GICD_ITARGETSR_SPI_188_REG           (xxx_GICD_REGS_BASE + 0xB70)  /* SPI_BLOCK每32bit对应一个中断的所有相应的CPU Interface。h{0,223} */
#define xxx_GICD_REGS_GICD_ITARGETSR_SPI_189_REG           (xxx_GICD_REGS_BASE + 0xB74)  /* SPI_BLOCK每32bit对应一个中断的所有相应的CPU Interface。h{0,223} */
#define xxx_GICD_REGS_GICD_ITARGETSR_SPI_190_REG           (xxx_GICD_REGS_BASE + 0xB78)  /* SPI_BLOCK每32bit对应一个中断的所有相应的CPU Interface。h{0,223} */
#define xxx_GICD_REGS_GICD_ITARGETSR_SPI_191_REG           (xxx_GICD_REGS_BASE + 0xB7C)  /* SPI_BLOCK每32bit对应一个中断的所有相应的CPU Interface。h{0,223} */
#define xxx_GICD_REGS_GICD_ITARGETSR_SPI_192_REG           (xxx_GICD_REGS_BASE + 0xB80)  /* SPI_BLOCK每32bit对应一个中断的所有相应的CPU Interface。h{0,223} */
#define xxx_GICD_REGS_GICD_ITARGETSR_SPI_193_REG           (xxx_GICD_REGS_BASE + 0xB84)  /* SPI_BLOCK每32bit对应一个中断的所有相应的CPU Interface。h{0,223} */
#define xxx_GICD_REGS_GICD_ITARGETSR_SPI_194_REG           (xxx_GICD_REGS_BASE + 0xB88)  /* SPI_BLOCK每32bit对应一个中断的所有相应的CPU Interface。h{0,223} */
#define xxx_GICD_REGS_GICD_ITARGETSR_SPI_195_REG           (xxx_GICD_REGS_BASE + 0xB8C)  /* SPI_BLOCK每32bit对应一个中断的所有相应的CPU Interface。h{0,223} */
#define xxx_GICD_REGS_GICD_ITARGETSR_SPI_196_REG           (xxx_GICD_REGS_BASE + 0xB90)  /* SPI_BLOCK每32bit对应一个中断的所有相应的CPU Interface。h{0,223} */
#define xxx_GICD_REGS_GICD_ITARGETSR_SPI_197_REG           (xxx_GICD_REGS_BASE + 0xB94)  /* SPI_BLOCK每32bit对应一个中断的所有相应的CPU Interface。h{0,223} */
#define xxx_GICD_REGS_GICD_ITARGETSR_SPI_198_REG           (xxx_GICD_REGS_BASE + 0xB98)  /* SPI_BLOCK每32bit对应一个中断的所有相应的CPU Interface。h{0,223} */
#define xxx_GICD_REGS_GICD_ITARGETSR_SPI_199_REG           (xxx_GICD_REGS_BASE + 0xB9C)  /* SPI_BLOCK每32bit对应一个中断的所有相应的CPU Interface。h{0,223} */
#define xxx_GICD_REGS_GICD_ITARGETSR_SPI_200_REG           (xxx_GICD_REGS_BASE + 0xBA0)  /* SPI_BLOCK每32bit对应一个中断的所有相应的CPU Interface。h{0,223} */
#define xxx_GICD_REGS_GICD_ITARGETSR_SPI_201_REG           (xxx_GICD_REGS_BASE + 0xBA4)  /* SPI_BLOCK每32bit对应一个中断的所有相应的CPU Interface。h{0,223} */
#define xxx_GICD_REGS_GICD_ITARGETSR_SPI_202_REG           (xxx_GICD_REGS_BASE + 0xBA8)  /* SPI_BLOCK每32bit对应一个中断的所有相应的CPU Interface。h{0,223} */
#define xxx_GICD_REGS_GICD_ITARGETSR_SPI_203_REG           (xxx_GICD_REGS_BASE + 0xBAC)  /* SPI_BLOCK每32bit对应一个中断的所有相应的CPU Interface。h{0,223} */
#define xxx_GICD_REGS_GICD_ITARGETSR_SPI_204_REG           (xxx_GICD_REGS_BASE + 0xBB0)  /* SPI_BLOCK每32bit对应一个中断的所有相应的CPU Interface。h{0,223} */
#define xxx_GICD_REGS_GICD_ITARGETSR_SPI_205_REG           (xxx_GICD_REGS_BASE + 0xBB4)  /* SPI_BLOCK每32bit对应一个中断的所有相应的CPU Interface。h{0,223} */
#define xxx_GICD_REGS_GICD_ITARGETSR_SPI_206_REG           (xxx_GICD_REGS_BASE + 0xBB8)  /* SPI_BLOCK每32bit对应一个中断的所有相应的CPU Interface。h{0,223} */
#define xxx_GICD_REGS_GICD_ITARGETSR_SPI_207_REG           (xxx_GICD_REGS_BASE + 0xBBC)  /* SPI_BLOCK每32bit对应一个中断的所有相应的CPU Interface。h{0,223} */
#define xxx_GICD_REGS_GICD_ITARGETSR_SPI_208_REG           (xxx_GICD_REGS_BASE + 0xBC0)  /* SPI_BLOCK每32bit对应一个中断的所有相应的CPU Interface。h{0,223} */
#define xxx_GICD_REGS_GICD_ITARGETSR_SPI_209_REG           (xxx_GICD_REGS_BASE + 0xBC4)  /* SPI_BLOCK每32bit对应一个中断的所有相应的CPU Interface。h{0,223} */
#define xxx_GICD_REGS_GICD_ITARGETSR_SPI_210_REG           (xxx_GICD_REGS_BASE + 0xBC8)  /* SPI_BLOCK每32bit对应一个中断的所有相应的CPU Interface。h{0,223} */
#define xxx_GICD_REGS_GICD_ITARGETSR_SPI_211_REG           (xxx_GICD_REGS_BASE + 0xBCC)  /* SPI_BLOCK每32bit对应一个中断的所有相应的CPU Interface。h{0,223} */
#define xxx_GICD_REGS_GICD_ITARGETSR_SPI_212_REG           (xxx_GICD_REGS_BASE + 0xBD0)  /* SPI_BLOCK每32bit对应一个中断的所有相应的CPU Interface。h{0,223} */
#define xxx_GICD_REGS_GICD_ITARGETSR_SPI_213_REG           (xxx_GICD_REGS_BASE + 0xBD4)  /* SPI_BLOCK每32bit对应一个中断的所有相应的CPU Interface。h{0,223} */
#define xxx_GICD_REGS_GICD_ITARGETSR_SPI_214_REG           (xxx_GICD_REGS_BASE + 0xBD8)  /* SPI_BLOCK每32bit对应一个中断的所有相应的CPU Interface。h{0,223} */
#define xxx_GICD_REGS_GICD_ITARGETSR_SPI_215_REG           (xxx_GICD_REGS_BASE + 0xBDC)  /* SPI_BLOCK每32bit对应一个中断的所有相应的CPU Interface。h{0,223} */
#define xxx_GICD_REGS_GICD_ITARGETSR_SPI_216_REG           (xxx_GICD_REGS_BASE + 0xBE0)  /* SPI_BLOCK每32bit对应一个中断的所有相应的CPU Interface。h{0,223} */
#define xxx_GICD_REGS_GICD_ITARGETSR_SPI_217_REG           (xxx_GICD_REGS_BASE + 0xBE4)  /* SPI_BLOCK每32bit对应一个中断的所有相应的CPU Interface。h{0,223} */
#define xxx_GICD_REGS_GICD_ITARGETSR_SPI_218_REG           (xxx_GICD_REGS_BASE + 0xBE8)  /* SPI_BLOCK每32bit对应一个中断的所有相应的CPU Interface。h{0,223} */
#define xxx_GICD_REGS_GICD_ITARGETSR_SPI_219_REG           (xxx_GICD_REGS_BASE + 0xBEC)  /* SPI_BLOCK每32bit对应一个中断的所有相应的CPU Interface。h{0,223} */
#define xxx_GICD_REGS_GICD_ITARGETSR_SPI_220_REG           (xxx_GICD_REGS_BASE + 0xBF0)  /* SPI_BLOCK每32bit对应一个中断的所有相应的CPU Interface。h{0,223} */
#define xxx_GICD_REGS_GICD_ITARGETSR_SPI_221_REG           (xxx_GICD_REGS_BASE + 0xBF4)  /* SPI_BLOCK每32bit对应一个中断的所有相应的CPU Interface。h{0,223} */
#define xxx_GICD_REGS_GICD_ITARGETSR_SPI_222_REG           (xxx_GICD_REGS_BASE + 0xBF8)  /* SPI_BLOCK每32bit对应一个中断的所有相应的CPU Interface。h{0,223} */
#define xxx_GICD_REGS_GICD_ITARGETSR_SPI_223_REG           (xxx_GICD_REGS_BASE + 0xBFC)  /* SPI_BLOCK每32bit对应一个中断的所有相应的CPU Interface。h{0,223} */
#define xxx_GICD_REGS_GICD_ICFGR_LOW_SGI_REG               (xxx_GICD_REGS_BASE + 0xC00)  /* 该寄存器每2bit表示一个SGI中断是电平敏感还是边沿触发 */
#define xxx_GICD_REGS_GICD_ICFGR_LOW_PPI_REG               (xxx_GICD_REGS_BASE + 0xC04)  /* 该寄存器每2bit表示一个PPI中断是电平敏感还是边沿触发 */
#define xxx_GICD_REGS_GICD_ICFGR_SPI_COMMON_0_REG          (xxx_GICD_REGS_BASE + 0xC10)  /* 该寄存器每2bit表示一个1-N SPI中断是电平敏感还是边沿触发,g{0,59} */
#define xxx_GICD_REGS_GICD_ICFGR_SPI_COMMON_1_REG          (xxx_GICD_REGS_BASE + 0xC14)  /* 该寄存器每2bit表示一个1-N SPI中断是电平敏感还是边沿触发,g{0,59} */
#define xxx_GICD_REGS_GICD_ICFGR_SPI_COMMON_2_REG          (xxx_GICD_REGS_BASE + 0xC18)  /* 该寄存器每2bit表示一个1-N SPI中断是电平敏感还是边沿触发,g{0,59} */
#define xxx_GICD_REGS_GICD_ICFGR_SPI_COMMON_3_REG          (xxx_GICD_REGS_BASE + 0xC1C)  /* 该寄存器每2bit表示一个1-N SPI中断是电平敏感还是边沿触发,g{0,59} */
#define xxx_GICD_REGS_GICD_ICFGR_SPI_COMMON_4_REG          (xxx_GICD_REGS_BASE + 0xC20)  /* 该寄存器每2bit表示一个1-N SPI中断是电平敏感还是边沿触发,g{0,59} */
#define xxx_GICD_REGS_GICD_ICFGR_SPI_COMMON_5_REG          (xxx_GICD_REGS_BASE + 0xC24)  /* 该寄存器每2bit表示一个1-N SPI中断是电平敏感还是边沿触发,g{0,59} */
#define xxx_GICD_REGS_GICD_ICFGR_SPI_COMMON_6_REG          (xxx_GICD_REGS_BASE + 0xC28)  /* 该寄存器每2bit表示一个1-N SPI中断是电平敏感还是边沿触发,g{0,59} */
#define xxx_GICD_REGS_GICD_ICFGR_SPI_COMMON_7_REG          (xxx_GICD_REGS_BASE + 0xC2C)  /* 该寄存器每2bit表示一个1-N SPI中断是电平敏感还是边沿触发,g{0,59} */
#define xxx_GICD_REGS_GICD_ICFGR_SPI_COMMON_8_REG          (xxx_GICD_REGS_BASE + 0xC30)  /* 该寄存器每2bit表示一个1-N SPI中断是电平敏感还是边沿触发,g{0,59} */
#define xxx_GICD_REGS_GICD_ICFGR_SPI_COMMON_9_REG          (xxx_GICD_REGS_BASE + 0xC34)  /* 该寄存器每2bit表示一个1-N SPI中断是电平敏感还是边沿触发,g{0,59} */
#define xxx_GICD_REGS_GICD_ICFGR_SPI_COMMON_10_REG         (xxx_GICD_REGS_BASE + 0xC38)  /* 该寄存器每2bit表示一个1-N SPI中断是电平敏感还是边沿触发,g{0,59} */
#define xxx_GICD_REGS_GICD_ICFGR_SPI_COMMON_11_REG         (xxx_GICD_REGS_BASE + 0xC3C)  /* 该寄存器每2bit表示一个1-N SPI中断是电平敏感还是边沿触发,g{0,59} */
#define xxx_GICD_REGS_GICD_ICFGR_SPI_COMMON_12_REG         (xxx_GICD_REGS_BASE + 0xC40)  /* 该寄存器每2bit表示一个1-N SPI中断是电平敏感还是边沿触发,g{0,59} */
#define xxx_GICD_REGS_GICD_ICFGR_SPI_COMMON_13_REG         (xxx_GICD_REGS_BASE + 0xC44)  /* 该寄存器每2bit表示一个1-N SPI中断是电平敏感还是边沿触发,g{0,59} */
#define xxx_GICD_REGS_GICD_ICFGR_SPI_COMMON_14_REG         (xxx_GICD_REGS_BASE + 0xC48)  /* 该寄存器每2bit表示一个1-N SPI中断是电平敏感还是边沿触发,g{0,59} */
#define xxx_GICD_REGS_GICD_ICFGR_SPI_COMMON_15_REG         (xxx_GICD_REGS_BASE + 0xC4C)  /* 该寄存器每2bit表示一个1-N SPI中断是电平敏感还是边沿触发,g{0,59} */
#define xxx_GICD_REGS_GICD_ICFGR_SPI_COMMON_16_REG         (xxx_GICD_REGS_BASE + 0xC50)  /* 该寄存器每2bit表示一个1-N SPI中断是电平敏感还是边沿触发,g{0,59} */
#define xxx_GICD_REGS_GICD_ICFGR_SPI_COMMON_17_REG         (xxx_GICD_REGS_BASE + 0xC54)  /* 该寄存器每2bit表示一个1-N SPI中断是电平敏感还是边沿触发,g{0,59} */
#define xxx_GICD_REGS_GICD_ICFGR_SPI_COMMON_18_REG         (xxx_GICD_REGS_BASE + 0xC58)  /* 该寄存器每2bit表示一个1-N SPI中断是电平敏感还是边沿触发,g{0,59} */
#define xxx_GICD_REGS_GICD_ICFGR_SPI_COMMON_19_REG         (xxx_GICD_REGS_BASE + 0xC5C)  /* 该寄存器每2bit表示一个1-N SPI中断是电平敏感还是边沿触发,g{0,59} */
#define xxx_GICD_REGS_GICD_ICFGR_SPI_COMMON_20_REG         (xxx_GICD_REGS_BASE + 0xC60)  /* 该寄存器每2bit表示一个1-N SPI中断是电平敏感还是边沿触发,g{0,59} */
#define xxx_GICD_REGS_GICD_ICFGR_SPI_COMMON_21_REG         (xxx_GICD_REGS_BASE + 0xC64)  /* 该寄存器每2bit表示一个1-N SPI中断是电平敏感还是边沿触发,g{0,59} */
#define xxx_GICD_REGS_GICD_ICFGR_SPI_COMMON_22_REG         (xxx_GICD_REGS_BASE + 0xC68)  /* 该寄存器每2bit表示一个1-N SPI中断是电平敏感还是边沿触发,g{0,59} */
#define xxx_GICD_REGS_GICD_ICFGR_SPI_COMMON_23_REG         (xxx_GICD_REGS_BASE + 0xC6C)  /* 该寄存器每2bit表示一个1-N SPI中断是电平敏感还是边沿触发,g{0,59} */
#define xxx_GICD_REGS_GICD_ICFGR_SPI_COMMON_24_REG         (xxx_GICD_REGS_BASE + 0xC70)  /* 该寄存器每2bit表示一个1-N SPI中断是电平敏感还是边沿触发,g{0,59} */
#define xxx_GICD_REGS_GICD_ICFGR_SPI_COMMON_25_REG         (xxx_GICD_REGS_BASE + 0xC74)  /* 该寄存器每2bit表示一个1-N SPI中断是电平敏感还是边沿触发,g{0,59} */
#define xxx_GICD_REGS_GICD_ICFGR_SPI_COMMON_26_REG         (xxx_GICD_REGS_BASE + 0xC78)  /* 该寄存器每2bit表示一个1-N SPI中断是电平敏感还是边沿触发,g{0,59} */
#define xxx_GICD_REGS_GICD_ICFGR_SPI_COMMON_27_REG         (xxx_GICD_REGS_BASE + 0xC7C)  /* 该寄存器每2bit表示一个1-N SPI中断是电平敏感还是边沿触发,g{0,59} */
#define xxx_GICD_REGS_GICD_ICFGR_SPI_COMMON_28_REG         (xxx_GICD_REGS_BASE + 0xC80)  /* 该寄存器每2bit表示一个1-N SPI中断是电平敏感还是边沿触发,g{0,59} */
#define xxx_GICD_REGS_GICD_ICFGR_SPI_COMMON_29_REG         (xxx_GICD_REGS_BASE + 0xC84)  /* 该寄存器每2bit表示一个1-N SPI中断是电平敏感还是边沿触发,g{0,59} */
#define xxx_GICD_REGS_GICD_ICFGR_SPI_COMMON_30_REG         (xxx_GICD_REGS_BASE + 0xC88)  /* 该寄存器每2bit表示一个1-N SPI中断是电平敏感还是边沿触发,g{0,59} */
#define xxx_GICD_REGS_GICD_ICFGR_SPI_COMMON_31_REG         (xxx_GICD_REGS_BASE + 0xC8C)  /* 该寄存器每2bit表示一个1-N SPI中断是电平敏感还是边沿触发,g{0,59} */
#define xxx_GICD_REGS_GICD_ICFGR_SPI_COMMON_32_REG         (xxx_GICD_REGS_BASE + 0xC90)  /* 该寄存器每2bit表示一个1-N SPI中断是电平敏感还是边沿触发,g{0,59} */
#define xxx_GICD_REGS_GICD_ICFGR_SPI_COMMON_33_REG         (xxx_GICD_REGS_BASE + 0xC94)  /* 该寄存器每2bit表示一个1-N SPI中断是电平敏感还是边沿触发,g{0,59} */
#define xxx_GICD_REGS_GICD_ICFGR_SPI_COMMON_34_REG         (xxx_GICD_REGS_BASE + 0xC98)  /* 该寄存器每2bit表示一个1-N SPI中断是电平敏感还是边沿触发,g{0,59} */
#define xxx_GICD_REGS_GICD_ICFGR_SPI_COMMON_35_REG         (xxx_GICD_REGS_BASE + 0xC9C)  /* 该寄存器每2bit表示一个1-N SPI中断是电平敏感还是边沿触发,g{0,59} */
#define xxx_GICD_REGS_GICD_ICFGR_SPI_COMMON_36_REG         (xxx_GICD_REGS_BASE + 0xCA0)  /* 该寄存器每2bit表示一个1-N SPI中断是电平敏感还是边沿触发,g{0,59} */
#define xxx_GICD_REGS_GICD_ICFGR_SPI_COMMON_37_REG         (xxx_GICD_REGS_BASE + 0xCA4)  /* 该寄存器每2bit表示一个1-N SPI中断是电平敏感还是边沿触发,g{0,59} */
#define xxx_GICD_REGS_GICD_ICFGR_SPI_COMMON_38_REG         (xxx_GICD_REGS_BASE + 0xCA8)  /* 该寄存器每2bit表示一个1-N SPI中断是电平敏感还是边沿触发,g{0,59} */
#define xxx_GICD_REGS_GICD_ICFGR_SPI_COMMON_39_REG         (xxx_GICD_REGS_BASE + 0xCAC)  /* 该寄存器每2bit表示一个1-N SPI中断是电平敏感还是边沿触发,g{0,59} */
#define xxx_GICD_REGS_GICD_ICFGR_SPI_COMMON_40_REG         (xxx_GICD_REGS_BASE + 0xCB0)  /* 该寄存器每2bit表示一个1-N SPI中断是电平敏感还是边沿触发,g{0,59} */
#define xxx_GICD_REGS_GICD_ICFGR_SPI_COMMON_41_REG         (xxx_GICD_REGS_BASE + 0xCB4)  /* 该寄存器每2bit表示一个1-N SPI中断是电平敏感还是边沿触发,g{0,59} */
#define xxx_GICD_REGS_GICD_ICFGR_SPI_COMMON_42_REG         (xxx_GICD_REGS_BASE + 0xCB8)  /* 该寄存器每2bit表示一个1-N SPI中断是电平敏感还是边沿触发,g{0,59} */
#define xxx_GICD_REGS_GICD_ICFGR_SPI_COMMON_43_REG         (xxx_GICD_REGS_BASE + 0xCBC)  /* 该寄存器每2bit表示一个1-N SPI中断是电平敏感还是边沿触发,g{0,59} */
#define xxx_GICD_REGS_GICD_ICFGR_SPI_COMMON_44_REG         (xxx_GICD_REGS_BASE + 0xCC0)  /* 该寄存器每2bit表示一个1-N SPI中断是电平敏感还是边沿触发,g{0,59} */
#define xxx_GICD_REGS_GICD_ICFGR_SPI_COMMON_45_REG         (xxx_GICD_REGS_BASE + 0xCC4)  /* 该寄存器每2bit表示一个1-N SPI中断是电平敏感还是边沿触发,g{0,59} */
#define xxx_GICD_REGS_GICD_ICFGR_SPI_COMMON_46_REG         (xxx_GICD_REGS_BASE + 0xCC8)  /* 该寄存器每2bit表示一个1-N SPI中断是电平敏感还是边沿触发,g{0,59} */
#define xxx_GICD_REGS_GICD_ICFGR_SPI_COMMON_47_REG         (xxx_GICD_REGS_BASE + 0xCCC)  /* 该寄存器每2bit表示一个1-N SPI中断是电平敏感还是边沿触发,g{0,59} */
#define xxx_GICD_REGS_GICD_ICFGR_SPI_COMMON_48_REG         (xxx_GICD_REGS_BASE + 0xCD0)  /* 该寄存器每2bit表示一个1-N SPI中断是电平敏感还是边沿触发,g{0,59} */
#define xxx_GICD_REGS_GICD_ICFGR_SPI_COMMON_49_REG         (xxx_GICD_REGS_BASE + 0xCD4)  /* 该寄存器每2bit表示一个1-N SPI中断是电平敏感还是边沿触发,g{0,59} */
#define xxx_GICD_REGS_GICD_ICFGR_SPI_COMMON_50_REG         (xxx_GICD_REGS_BASE + 0xCD8)  /* 该寄存器每2bit表示一个1-N SPI中断是电平敏感还是边沿触发,g{0,59} */
#define xxx_GICD_REGS_GICD_ICFGR_SPI_COMMON_51_REG         (xxx_GICD_REGS_BASE + 0xCDC)  /* 该寄存器每2bit表示一个1-N SPI中断是电平敏感还是边沿触发,g{0,59} */
#define xxx_GICD_REGS_GICD_ICFGR_SPI_COMMON_52_REG         (xxx_GICD_REGS_BASE + 0xCE0)  /* 该寄存器每2bit表示一个1-N SPI中断是电平敏感还是边沿触发,g{0,59} */
#define xxx_GICD_REGS_GICD_ICFGR_SPI_COMMON_53_REG         (xxx_GICD_REGS_BASE + 0xCE4)  /* 该寄存器每2bit表示一个1-N SPI中断是电平敏感还是边沿触发,g{0,59} */
#define xxx_GICD_REGS_GICD_ICFGR_SPI_COMMON_54_REG         (xxx_GICD_REGS_BASE + 0xCE8)  /* 该寄存器每2bit表示一个1-N SPI中断是电平敏感还是边沿触发,g{0,59} */
#define xxx_GICD_REGS_GICD_ICFGR_SPI_COMMON_55_REG         (xxx_GICD_REGS_BASE + 0xCEC)  /* 该寄存器每2bit表示一个1-N SPI中断是电平敏感还是边沿触发,g{0,59} */
#define xxx_GICD_REGS_GICD_ICFGR_SPI_COMMON_56_REG         (xxx_GICD_REGS_BASE + 0xCF0)  /* 该寄存器每2bit表示一个1-N SPI中断是电平敏感还是边沿触发,g{0,59} */
#define xxx_GICD_REGS_GICD_ICFGR_SPI_COMMON_57_REG         (xxx_GICD_REGS_BASE + 0xCF4)  /* 该寄存器每2bit表示一个1-N SPI中断是电平敏感还是边沿触发,g{0,59} */
#define xxx_GICD_REGS_GICD_ICFGR_SPI_COMMON_58_REG         (xxx_GICD_REGS_BASE + 0xCF8)  /* 该寄存器每2bit表示一个1-N SPI中断是电平敏感还是边沿触发,g{0,59} */
#define xxx_GICD_REGS_GICD_ICFGR_SPI_COMMON_59_REG         (xxx_GICD_REGS_BASE + 0xCFC)  /* 该寄存器每2bit表示一个1-N SPI中断是电平敏感还是边沿触发,g{0,59} */
#define xxx_GICD_REGS_GICD_IGRPMODR_LOW_REG                (xxx_GICD_REGS_BASE + 0xD00)  /* 标识lowid中断（对应中断ID的范围为{0，31}）分组的修饰 */
#define xxx_GICD_REGS_GICD_IGRPMODR_SPI_0_REG              (xxx_GICD_REGS_BASE + 0xD04)  /* 标识中断分组的修饰，n的值由GICD_TYPER.ITLinesNumber决定。m{0,2} */
#define xxx_GICD_REGS_GICD_IGRPMODR_SPI_1_REG              (xxx_GICD_REGS_BASE + 0xD08)  /* 标识中断分组的修饰，n的值由GICD_TYPER.ITLinesNumber决定。m{0,2} */
#define xxx_GICD_REGS_GICD_IGRPMODR_SPI_2_REG              (xxx_GICD_REGS_BASE + 0xD0C)  /* 标识中断分组的修饰，n的值由GICD_TYPER.ITLinesNumber决定。m{0,2} */
#define xxx_GICD_REGS_GICD_IGRPMODR_SPI_3_REG              (xxx_GICD_REGS_BASE + 0xD10)  /* 标识中断分组的修饰，n的值由GICD_TYPER.ITLinesNumber决定。m{0,2} */
#define xxx_GICD_REGS_GICD_IGRPMODR_SPI_4_REG              (xxx_GICD_REGS_BASE + 0xD14)  /* 标识中断分组的修饰，n的值由GICD_TYPER.ITLinesNumber决定。m{0,2} */
#define xxx_GICD_REGS_GICD_IGRPMODR_SPI_5_REG              (xxx_GICD_REGS_BASE + 0xD18)  /* 标识中断分组的修饰，n的值由GICD_TYPER.ITLinesNumber决定。m{0,2} */
#define xxx_GICD_REGS_GICD_IGRPMODR_SPI_6_REG              (xxx_GICD_REGS_BASE + 0xD1C)  /* 标识中断分组的修饰，n的值由GICD_TYPER.ITLinesNumber决定。m{0,2} */
#define xxx_GICD_REGS_GICD_IGRPMODR_SPI_7_REG              (xxx_GICD_REGS_BASE + 0xD20)  /* 标识中断分组的修饰，n的值由GICD_TYPER.ITLinesNumber决定。m{0,2} */
#define xxx_GICD_REGS_GICD_IGRPMODR_SPI_8_REG              (xxx_GICD_REGS_BASE + 0xD24)  /* 标识中断分组的修饰，n的值由GICD_TYPER.ITLinesNumber决定。m{0,2} */
#define xxx_GICD_REGS_GICD_IGRPMODR_SPI_9_REG              (xxx_GICD_REGS_BASE + 0xD28)  /* 标识中断分组的修饰，n的值由GICD_TYPER.ITLinesNumber决定。m{0,2} */
#define xxx_GICD_REGS_GICD_IGRPMODR_SPI_10_REG             (xxx_GICD_REGS_BASE + 0xD2C)  /* 标识中断分组的修饰，n的值由GICD_TYPER.ITLinesNumber决定。m{0,2} */
#define xxx_GICD_REGS_GICD_IGRPMODR_SPI_11_REG             (xxx_GICD_REGS_BASE + 0xD30)  /* 标识中断分组的修饰，n的值由GICD_TYPER.ITLinesNumber决定。m{0,2} */
#define xxx_GICD_REGS_GICD_IGRPMODR_SPI_12_REG             (xxx_GICD_REGS_BASE + 0xD34)  /* 标识中断分组的修饰，n的值由GICD_TYPER.ITLinesNumber决定。m{0,2} */
#define xxx_GICD_REGS_GICD_IGRPMODR_SPI_13_REG             (xxx_GICD_REGS_BASE + 0xD38)  /* 标识中断分组的修饰，n的值由GICD_TYPER.ITLinesNumber决定。m{0,2} */
#define xxx_GICD_REGS_GICD_IGRPMODR_SPI_14_REG             (xxx_GICD_REGS_BASE + 0xD3C)  /* 标识中断分组的修饰，n的值由GICD_TYPER.ITLinesNumber决定。m{0,2} */
#define xxx_GICD_REGS_GICD_IGRPMODR_SPI_15_REG             (xxx_GICD_REGS_BASE + 0xD40)  /* 标识中断分组的修饰，n的值由GICD_TYPER.ITLinesNumber决定。m{0,2} */
#define xxx_GICD_REGS_GICD_IGRPMODR_SPI_16_REG             (xxx_GICD_REGS_BASE + 0xD44)  /* 标识中断分组的修饰，n的值由GICD_TYPER.ITLinesNumber决定。m{0,2} */
#define xxx_GICD_REGS_GICD_IGRPMODR_SPI_17_REG             (xxx_GICD_REGS_BASE + 0xD48)  /* 标识中断分组的修饰，n的值由GICD_TYPER.ITLinesNumber决定。m{0,2} */
#define xxx_GICD_REGS_GICD_IGRPMODR_SPI_18_REG             (xxx_GICD_REGS_BASE + 0xD4C)  /* 标识中断分组的修饰，n的值由GICD_TYPER.ITLinesNumber决定。m{0,2} */
#define xxx_GICD_REGS_GICD_IGRPMODR_SPI_19_REG             (xxx_GICD_REGS_BASE + 0xD50)  /* 标识中断分组的修饰，n的值由GICD_TYPER.ITLinesNumber决定。m{0,2} */
#define xxx_GICD_REGS_GICD_IGRPMODR_SPI_20_REG             (xxx_GICD_REGS_BASE + 0xD54)  /* 标识中断分组的修饰，n的值由GICD_TYPER.ITLinesNumber决定。m{0,2} */
#define xxx_GICD_REGS_GICD_IGRPMODR_SPI_21_REG             (xxx_GICD_REGS_BASE + 0xD58)  /* 标识中断分组的修饰，n的值由GICD_TYPER.ITLinesNumber决定。m{0,2} */
#define xxx_GICD_REGS_GICD_IGRPMODR_SPI_22_REG             (xxx_GICD_REGS_BASE + 0xD5C)  /* 标识中断分组的修饰，n的值由GICD_TYPER.ITLinesNumber决定。m{0,2} */
#define xxx_GICD_REGS_GICD_IGRPMODR_SPI_23_REG             (xxx_GICD_REGS_BASE + 0xD60)  /* 标识中断分组的修饰，n的值由GICD_TYPER.ITLinesNumber决定。m{0,2} */
#define xxx_GICD_REGS_GICD_IGRPMODR_SPI_24_REG             (xxx_GICD_REGS_BASE + 0xD64)  /* 标识中断分组的修饰，n的值由GICD_TYPER.ITLinesNumber决定。m{0,2} */
#define xxx_GICD_REGS_GICD_IGRPMODR_SPI_25_REG             (xxx_GICD_REGS_BASE + 0xD68)  /* 标识中断分组的修饰，n的值由GICD_TYPER.ITLinesNumber决定。m{0,2} */
#define xxx_GICD_REGS_GICD_IGRPMODR_SPI_26_REG             (xxx_GICD_REGS_BASE + 0xD6C)  /* 标识中断分组的修饰，n的值由GICD_TYPER.ITLinesNumber决定。m{0,2} */
#define xxx_GICD_REGS_GICD_IGRPMODR_SPI_27_REG             (xxx_GICD_REGS_BASE + 0xD70)  /* 标识中断分组的修饰，n的值由GICD_TYPER.ITLinesNumber决定。m{0,2} */
#define xxx_GICD_REGS_GICD_IGRPMODR_SPI_28_REG             (xxx_GICD_REGS_BASE + 0xD74)  /* 标识中断分组的修饰，n的值由GICD_TYPER.ITLinesNumber决定。m{0,2} */
#define xxx_GICD_REGS_GICD_IGRPMODR_SPI_29_REG             (xxx_GICD_REGS_BASE + 0xD78)  /* 标识中断分组的修饰，n的值由GICD_TYPER.ITLinesNumber决定。m{0,2} */
#define xxx_GICD_REGS_GICD_IGRPMODR_SPI_30_REG             (xxx_GICD_REGS_BASE + 0xD7C)  /* 标识中断分组的修饰，n的值由GICD_TYPER.ITLinesNumber决定。m{0,2} */
#define xxx_GICD_REGS_GICD_NSACR_0_REG                     (xxx_GICD_REGS_BASE + 0xE00)  /* 非安全软件访问权限管理d{0,63} */
#define xxx_GICD_REGS_GICD_NSACR_1_REG                     (xxx_GICD_REGS_BASE + 0xE04)  /* 非安全软件访问权限管理d{0,63} */
#define xxx_GICD_REGS_GICD_NSACR_2_REG                     (xxx_GICD_REGS_BASE + 0xE08)  /* 非安全软件访问权限管理d{0,63} */
#define xxx_GICD_REGS_GICD_NSACR_3_REG                     (xxx_GICD_REGS_BASE + 0xE0C)  /* 非安全软件访问权限管理d{0,63} */
#define xxx_GICD_REGS_GICD_NSACR_4_REG                     (xxx_GICD_REGS_BASE + 0xE10)  /* 非安全软件访问权限管理d{0,63} */
#define xxx_GICD_REGS_GICD_NSACR_5_REG                     (xxx_GICD_REGS_BASE + 0xE14)  /* 非安全软件访问权限管理d{0,63} */
#define xxx_GICD_REGS_GICD_NSACR_6_REG                     (xxx_GICD_REGS_BASE + 0xE18)  /* 非安全软件访问权限管理d{0,63} */
#define xxx_GICD_REGS_GICD_NSACR_7_REG                     (xxx_GICD_REGS_BASE + 0xE1C)  /* 非安全软件访问权限管理d{0,63} */
#define xxx_GICD_REGS_GICD_NSACR_8_REG                     (xxx_GICD_REGS_BASE + 0xE20)  /* 非安全软件访问权限管理d{0,63} */
#define xxx_GICD_REGS_GICD_NSACR_9_REG                     (xxx_GICD_REGS_BASE + 0xE24)  /* 非安全软件访问权限管理d{0,63} */
#define xxx_GICD_REGS_GICD_NSACR_10_REG                    (xxx_GICD_REGS_BASE + 0xE28)  /* 非安全软件访问权限管理d{0,63} */
#define xxx_GICD_REGS_GICD_NSACR_11_REG                    (xxx_GICD_REGS_BASE + 0xE2C)  /* 非安全软件访问权限管理d{0,63} */
#define xxx_GICD_REGS_GICD_NSACR_12_REG                    (xxx_GICD_REGS_BASE + 0xE30)  /* 非安全软件访问权限管理d{0,63} */
#define xxx_GICD_REGS_GICD_NSACR_13_REG                    (xxx_GICD_REGS_BASE + 0xE34)  /* 非安全软件访问权限管理d{0,63} */
#define xxx_GICD_REGS_GICD_NSACR_14_REG                    (xxx_GICD_REGS_BASE + 0xE38)  /* 非安全软件访问权限管理d{0,63} */
#define xxx_GICD_REGS_GICD_NSACR_15_REG                    (xxx_GICD_REGS_BASE + 0xE3C)  /* 非安全软件访问权限管理d{0,63} */
#define xxx_GICD_REGS_GICD_NSACR_16_REG                    (xxx_GICD_REGS_BASE + 0xE40)  /* 非安全软件访问权限管理d{0,63} */
#define xxx_GICD_REGS_GICD_NSACR_17_REG                    (xxx_GICD_REGS_BASE + 0xE44)  /* 非安全软件访问权限管理d{0,63} */
#define xxx_GICD_REGS_GICD_NSACR_18_REG                    (xxx_GICD_REGS_BASE + 0xE48)  /* 非安全软件访问权限管理d{0,63} */
#define xxx_GICD_REGS_GICD_NSACR_19_REG                    (xxx_GICD_REGS_BASE + 0xE4C)  /* 非安全软件访问权限管理d{0,63} */
#define xxx_GICD_REGS_GICD_NSACR_20_REG                    (xxx_GICD_REGS_BASE + 0xE50)  /* 非安全软件访问权限管理d{0,63} */
#define xxx_GICD_REGS_GICD_NSACR_21_REG                    (xxx_GICD_REGS_BASE + 0xE54)  /* 非安全软件访问权限管理d{0,63} */
#define xxx_GICD_REGS_GICD_NSACR_22_REG                    (xxx_GICD_REGS_BASE + 0xE58)  /* 非安全软件访问权限管理d{0,63} */
#define xxx_GICD_REGS_GICD_NSACR_23_REG                    (xxx_GICD_REGS_BASE + 0xE5C)  /* 非安全软件访问权限管理d{0,63} */
#define xxx_GICD_REGS_GICD_NSACR_24_REG                    (xxx_GICD_REGS_BASE + 0xE60)  /* 非安全软件访问权限管理d{0,63} */
#define xxx_GICD_REGS_GICD_NSACR_25_REG                    (xxx_GICD_REGS_BASE + 0xE64)  /* 非安全软件访问权限管理d{0,63} */
#define xxx_GICD_REGS_GICD_NSACR_26_REG                    (xxx_GICD_REGS_BASE + 0xE68)  /* 非安全软件访问权限管理d{0,63} */
#define xxx_GICD_REGS_GICD_NSACR_27_REG                    (xxx_GICD_REGS_BASE + 0xE6C)  /* 非安全软件访问权限管理d{0,63} */
#define xxx_GICD_REGS_GICD_NSACR_28_REG                    (xxx_GICD_REGS_BASE + 0xE70)  /* 非安全软件访问权限管理d{0,63} */
#define xxx_GICD_REGS_GICD_NSACR_29_REG                    (xxx_GICD_REGS_BASE + 0xE74)  /* 非安全软件访问权限管理d{0,63} */
#define xxx_GICD_REGS_GICD_NSACR_30_REG                    (xxx_GICD_REGS_BASE + 0xE78)  /* 非安全软件访问权限管理d{0,63} */
#define xxx_GICD_REGS_GICD_NSACR_31_REG                    (xxx_GICD_REGS_BASE + 0xE7C)  /* 非安全软件访问权限管理d{0,63} */
#define xxx_GICD_REGS_GICD_NSACR_32_REG                    (xxx_GICD_REGS_BASE + 0xE80)  /* 非安全软件访问权限管理d{0,63} */
#define xxx_GICD_REGS_GICD_NSACR_33_REG                    (xxx_GICD_REGS_BASE + 0xE84)  /* 非安全软件访问权限管理d{0,63} */
#define xxx_GICD_REGS_GICD_NSACR_34_REG                    (xxx_GICD_REGS_BASE + 0xE88)  /* 非安全软件访问权限管理d{0,63} */
#define xxx_GICD_REGS_GICD_NSACR_35_REG                    (xxx_GICD_REGS_BASE + 0xE8C)  /* 非安全软件访问权限管理d{0,63} */
#define xxx_GICD_REGS_GICD_NSACR_36_REG                    (xxx_GICD_REGS_BASE + 0xE90)  /* 非安全软件访问权限管理d{0,63} */
#define xxx_GICD_REGS_GICD_NSACR_37_REG                    (xxx_GICD_REGS_BASE + 0xE94)  /* 非安全软件访问权限管理d{0,63} */
#define xxx_GICD_REGS_GICD_NSACR_38_REG                    (xxx_GICD_REGS_BASE + 0xE98)  /* 非安全软件访问权限管理d{0,63} */
#define xxx_GICD_REGS_GICD_NSACR_39_REG                    (xxx_GICD_REGS_BASE + 0xE9C)  /* 非安全软件访问权限管理d{0,63} */
#define xxx_GICD_REGS_GICD_NSACR_40_REG                    (xxx_GICD_REGS_BASE + 0xEA0)  /* 非安全软件访问权限管理d{0,63} */
#define xxx_GICD_REGS_GICD_NSACR_41_REG                    (xxx_GICD_REGS_BASE + 0xEA4)  /* 非安全软件访问权限管理d{0,63} */
#define xxx_GICD_REGS_GICD_NSACR_42_REG                    (xxx_GICD_REGS_BASE + 0xEA8)  /* 非安全软件访问权限管理d{0,63} */
#define xxx_GICD_REGS_GICD_NSACR_43_REG                    (xxx_GICD_REGS_BASE + 0xEAC)  /* 非安全软件访问权限管理d{0,63} */
#define xxx_GICD_REGS_GICD_NSACR_44_REG                    (xxx_GICD_REGS_BASE + 0xEB0)  /* 非安全软件访问权限管理d{0,63} */
#define xxx_GICD_REGS_GICD_NSACR_45_REG                    (xxx_GICD_REGS_BASE + 0xEB4)  /* 非安全软件访问权限管理d{0,63} */
#define xxx_GICD_REGS_GICD_NSACR_46_REG                    (xxx_GICD_REGS_BASE + 0xEB8)  /* 非安全软件访问权限管理d{0,63} */
#define xxx_GICD_REGS_GICD_NSACR_47_REG                    (xxx_GICD_REGS_BASE + 0xEBC)  /* 非安全软件访问权限管理d{0,63} */
#define xxx_GICD_REGS_GICD_NSACR_48_REG                    (xxx_GICD_REGS_BASE + 0xEC0)  /* 非安全软件访问权限管理d{0,63} */
#define xxx_GICD_REGS_GICD_NSACR_49_REG                    (xxx_GICD_REGS_BASE + 0xEC4)  /* 非安全软件访问权限管理d{0,63} */
#define xxx_GICD_REGS_GICD_NSACR_50_REG                    (xxx_GICD_REGS_BASE + 0xEC8)  /* 非安全软件访问权限管理d{0,63} */
#define xxx_GICD_REGS_GICD_NSACR_51_REG                    (xxx_GICD_REGS_BASE + 0xECC)  /* 非安全软件访问权限管理d{0,63} */
#define xxx_GICD_REGS_GICD_NSACR_52_REG                    (xxx_GICD_REGS_BASE + 0xED0)  /* 非安全软件访问权限管理d{0,63} */
#define xxx_GICD_REGS_GICD_NSACR_53_REG                    (xxx_GICD_REGS_BASE + 0xED4)  /* 非安全软件访问权限管理d{0,63} */
#define xxx_GICD_REGS_GICD_NSACR_54_REG                    (xxx_GICD_REGS_BASE + 0xED8)  /* 非安全软件访问权限管理d{0,63} */
#define xxx_GICD_REGS_GICD_NSACR_55_REG                    (xxx_GICD_REGS_BASE + 0xEDC)  /* 非安全软件访问权限管理d{0,63} */
#define xxx_GICD_REGS_GICD_NSACR_56_REG                    (xxx_GICD_REGS_BASE + 0xEE0)  /* 非安全软件访问权限管理d{0,63} */
#define xxx_GICD_REGS_GICD_NSACR_57_REG                    (xxx_GICD_REGS_BASE + 0xEE4)  /* 非安全软件访问权限管理d{0,63} */
#define xxx_GICD_REGS_GICD_NSACR_58_REG                    (xxx_GICD_REGS_BASE + 0xEE8)  /* 非安全软件访问权限管理d{0,63} */
#define xxx_GICD_REGS_GICD_NSACR_59_REG                    (xxx_GICD_REGS_BASE + 0xEEC)  /* 非安全软件访问权限管理d{0,63} */
#define xxx_GICD_REGS_GICD_NSACR_60_REG                    (xxx_GICD_REGS_BASE + 0xEF0)  /* 非安全软件访问权限管理d{0,63} */
#define xxx_GICD_REGS_GICD_NSACR_61_REG                    (xxx_GICD_REGS_BASE + 0xEF4)  /* 非安全软件访问权限管理d{0,63} */
#define xxx_GICD_REGS_GICD_NSACR_62_REG                    (xxx_GICD_REGS_BASE + 0xEF8)  /* 非安全软件访问权限管理d{0,63} */
#define xxx_GICD_REGS_GICD_NSACR_63_REG                    (xxx_GICD_REGS_BASE + 0xEFC)  /* 非安全软件访问权限管理d{0,63} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_0_REG                (xxx_GICD_REGS_BASE + 0x6000) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_1_REG                (xxx_GICD_REGS_BASE + 0x6008) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_2_REG                (xxx_GICD_REGS_BASE + 0x6010) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_3_REG                (xxx_GICD_REGS_BASE + 0x6018) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_4_REG                (xxx_GICD_REGS_BASE + 0x6020) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_5_REG                (xxx_GICD_REGS_BASE + 0x6028) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_6_REG                (xxx_GICD_REGS_BASE + 0x6030) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_7_REG                (xxx_GICD_REGS_BASE + 0x6038) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_8_REG                (xxx_GICD_REGS_BASE + 0x6040) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_9_REG                (xxx_GICD_REGS_BASE + 0x6048) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_10_REG               (xxx_GICD_REGS_BASE + 0x6050) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_11_REG               (xxx_GICD_REGS_BASE + 0x6058) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_12_REG               (xxx_GICD_REGS_BASE + 0x6060) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_13_REG               (xxx_GICD_REGS_BASE + 0x6068) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_14_REG               (xxx_GICD_REGS_BASE + 0x6070) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_15_REG               (xxx_GICD_REGS_BASE + 0x6078) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_16_REG               (xxx_GICD_REGS_BASE + 0x6080) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_17_REG               (xxx_GICD_REGS_BASE + 0x6088) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_18_REG               (xxx_GICD_REGS_BASE + 0x6090) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_19_REG               (xxx_GICD_REGS_BASE + 0x6098) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_20_REG               (xxx_GICD_REGS_BASE + 0x60A0) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_21_REG               (xxx_GICD_REGS_BASE + 0x60A8) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_22_REG               (xxx_GICD_REGS_BASE + 0x60B0) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_23_REG               (xxx_GICD_REGS_BASE + 0x60B8) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_24_REG               (xxx_GICD_REGS_BASE + 0x60C0) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_25_REG               (xxx_GICD_REGS_BASE + 0x60C8) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_26_REG               (xxx_GICD_REGS_BASE + 0x60D0) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_27_REG               (xxx_GICD_REGS_BASE + 0x60D8) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_28_REG               (xxx_GICD_REGS_BASE + 0x60E0) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_29_REG               (xxx_GICD_REGS_BASE + 0x60E8) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_30_REG               (xxx_GICD_REGS_BASE + 0x60F0) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_31_REG               (xxx_GICD_REGS_BASE + 0x60F8) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_32_REG               (xxx_GICD_REGS_BASE + 0x6100) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_33_REG               (xxx_GICD_REGS_BASE + 0x6108) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_34_REG               (xxx_GICD_REGS_BASE + 0x6110) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_35_REG               (xxx_GICD_REGS_BASE + 0x6118) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_36_REG               (xxx_GICD_REGS_BASE + 0x6120) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_37_REG               (xxx_GICD_REGS_BASE + 0x6128) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_38_REG               (xxx_GICD_REGS_BASE + 0x6130) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_39_REG               (xxx_GICD_REGS_BASE + 0x6138) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_40_REG               (xxx_GICD_REGS_BASE + 0x6140) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_41_REG               (xxx_GICD_REGS_BASE + 0x6148) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_42_REG               (xxx_GICD_REGS_BASE + 0x6150) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_43_REG               (xxx_GICD_REGS_BASE + 0x6158) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_44_REG               (xxx_GICD_REGS_BASE + 0x6160) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_45_REG               (xxx_GICD_REGS_BASE + 0x6168) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_46_REG               (xxx_GICD_REGS_BASE + 0x6170) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_47_REG               (xxx_GICD_REGS_BASE + 0x6178) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_48_REG               (xxx_GICD_REGS_BASE + 0x6180) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_49_REG               (xxx_GICD_REGS_BASE + 0x6188) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_50_REG               (xxx_GICD_REGS_BASE + 0x6190) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_51_REG               (xxx_GICD_REGS_BASE + 0x6198) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_52_REG               (xxx_GICD_REGS_BASE + 0x61A0) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_53_REG               (xxx_GICD_REGS_BASE + 0x61A8) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_54_REG               (xxx_GICD_REGS_BASE + 0x61B0) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_55_REG               (xxx_GICD_REGS_BASE + 0x61B8) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_56_REG               (xxx_GICD_REGS_BASE + 0x61C0) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_57_REG               (xxx_GICD_REGS_BASE + 0x61C8) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_58_REG               (xxx_GICD_REGS_BASE + 0x61D0) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_59_REG               (xxx_GICD_REGS_BASE + 0x61D8) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_60_REG               (xxx_GICD_REGS_BASE + 0x61E0) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_61_REG               (xxx_GICD_REGS_BASE + 0x61E8) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_62_REG               (xxx_GICD_REGS_BASE + 0x61F0) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_63_REG               (xxx_GICD_REGS_BASE + 0x61F8) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_64_REG               (xxx_GICD_REGS_BASE + 0x6200) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_65_REG               (xxx_GICD_REGS_BASE + 0x6208) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_66_REG               (xxx_GICD_REGS_BASE + 0x6210) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_67_REG               (xxx_GICD_REGS_BASE + 0x6218) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_68_REG               (xxx_GICD_REGS_BASE + 0x6220) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_69_REG               (xxx_GICD_REGS_BASE + 0x6228) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_70_REG               (xxx_GICD_REGS_BASE + 0x6230) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_71_REG               (xxx_GICD_REGS_BASE + 0x6238) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_72_REG               (xxx_GICD_REGS_BASE + 0x6240) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_73_REG               (xxx_GICD_REGS_BASE + 0x6248) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_74_REG               (xxx_GICD_REGS_BASE + 0x6250) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_75_REG               (xxx_GICD_REGS_BASE + 0x6258) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_76_REG               (xxx_GICD_REGS_BASE + 0x6260) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_77_REG               (xxx_GICD_REGS_BASE + 0x6268) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_78_REG               (xxx_GICD_REGS_BASE + 0x6270) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_79_REG               (xxx_GICD_REGS_BASE + 0x6278) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_80_REG               (xxx_GICD_REGS_BASE + 0x6280) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_81_REG               (xxx_GICD_REGS_BASE + 0x6288) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_82_REG               (xxx_GICD_REGS_BASE + 0x6290) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_83_REG               (xxx_GICD_REGS_BASE + 0x6298) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_84_REG               (xxx_GICD_REGS_BASE + 0x62A0) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_85_REG               (xxx_GICD_REGS_BASE + 0x62A8) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_86_REG               (xxx_GICD_REGS_BASE + 0x62B0) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_87_REG               (xxx_GICD_REGS_BASE + 0x62B8) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_88_REG               (xxx_GICD_REGS_BASE + 0x62C0) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_89_REG               (xxx_GICD_REGS_BASE + 0x62C8) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_90_REG               (xxx_GICD_REGS_BASE + 0x62D0) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_91_REG               (xxx_GICD_REGS_BASE + 0x62D8) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_92_REG               (xxx_GICD_REGS_BASE + 0x62E0) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_93_REG               (xxx_GICD_REGS_BASE + 0x62E8) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_94_REG               (xxx_GICD_REGS_BASE + 0x62F0) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_95_REG               (xxx_GICD_REGS_BASE + 0x62F8) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_96_REG               (xxx_GICD_REGS_BASE + 0x6300) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_97_REG               (xxx_GICD_REGS_BASE + 0x6308) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_98_REG               (xxx_GICD_REGS_BASE + 0x6310) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_99_REG               (xxx_GICD_REGS_BASE + 0x6318) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_100_REG              (xxx_GICD_REGS_BASE + 0x6320) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_101_REG              (xxx_GICD_REGS_BASE + 0x6328) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_102_REG              (xxx_GICD_REGS_BASE + 0x6330) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_103_REG              (xxx_GICD_REGS_BASE + 0x6338) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_104_REG              (xxx_GICD_REGS_BASE + 0x6340) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_105_REG              (xxx_GICD_REGS_BASE + 0x6348) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_106_REG              (xxx_GICD_REGS_BASE + 0x6350) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_107_REG              (xxx_GICD_REGS_BASE + 0x6358) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_108_REG              (xxx_GICD_REGS_BASE + 0x6360) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_109_REG              (xxx_GICD_REGS_BASE + 0x6368) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_110_REG              (xxx_GICD_REGS_BASE + 0x6370) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_111_REG              (xxx_GICD_REGS_BASE + 0x6378) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_112_REG              (xxx_GICD_REGS_BASE + 0x6380) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_113_REG              (xxx_GICD_REGS_BASE + 0x6388) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_114_REG              (xxx_GICD_REGS_BASE + 0x6390) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_115_REG              (xxx_GICD_REGS_BASE + 0x6398) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_116_REG              (xxx_GICD_REGS_BASE + 0x63A0) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_117_REG              (xxx_GICD_REGS_BASE + 0x63A8) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_118_REG              (xxx_GICD_REGS_BASE + 0x63B0) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_119_REG              (xxx_GICD_REGS_BASE + 0x63B8) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_120_REG              (xxx_GICD_REGS_BASE + 0x63C0) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_121_REG              (xxx_GICD_REGS_BASE + 0x63C8) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_122_REG              (xxx_GICD_REGS_BASE + 0x63D0) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_123_REG              (xxx_GICD_REGS_BASE + 0x63D8) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_124_REG              (xxx_GICD_REGS_BASE + 0x63E0) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_125_REG              (xxx_GICD_REGS_BASE + 0x63E8) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_126_REG              (xxx_GICD_REGS_BASE + 0x63F0) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_127_REG              (xxx_GICD_REGS_BASE + 0x63F8) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_128_REG              (xxx_GICD_REGS_BASE + 0x6400) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_129_REG              (xxx_GICD_REGS_BASE + 0x6408) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_130_REG              (xxx_GICD_REGS_BASE + 0x6410) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_131_REG              (xxx_GICD_REGS_BASE + 0x6418) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_132_REG              (xxx_GICD_REGS_BASE + 0x6420) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_133_REG              (xxx_GICD_REGS_BASE + 0x6428) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_134_REG              (xxx_GICD_REGS_BASE + 0x6430) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_135_REG              (xxx_GICD_REGS_BASE + 0x6438) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_136_REG              (xxx_GICD_REGS_BASE + 0x6440) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_137_REG              (xxx_GICD_REGS_BASE + 0x6448) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_138_REG              (xxx_GICD_REGS_BASE + 0x6450) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_139_REG              (xxx_GICD_REGS_BASE + 0x6458) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_140_REG              (xxx_GICD_REGS_BASE + 0x6460) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_141_REG              (xxx_GICD_REGS_BASE + 0x6468) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_142_REG              (xxx_GICD_REGS_BASE + 0x6470) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_143_REG              (xxx_GICD_REGS_BASE + 0x6478) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_144_REG              (xxx_GICD_REGS_BASE + 0x6480) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_145_REG              (xxx_GICD_REGS_BASE + 0x6488) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_146_REG              (xxx_GICD_REGS_BASE + 0x6490) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_147_REG              (xxx_GICD_REGS_BASE + 0x6498) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_148_REG              (xxx_GICD_REGS_BASE + 0x64A0) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_149_REG              (xxx_GICD_REGS_BASE + 0x64A8) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_150_REG              (xxx_GICD_REGS_BASE + 0x64B0) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_151_REG              (xxx_GICD_REGS_BASE + 0x64B8) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_152_REG              (xxx_GICD_REGS_BASE + 0x64C0) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_153_REG              (xxx_GICD_REGS_BASE + 0x64C8) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_154_REG              (xxx_GICD_REGS_BASE + 0x64D0) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_155_REG              (xxx_GICD_REGS_BASE + 0x64D8) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_156_REG              (xxx_GICD_REGS_BASE + 0x64E0) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_157_REG              (xxx_GICD_REGS_BASE + 0x64E8) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_158_REG              (xxx_GICD_REGS_BASE + 0x64F0) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_159_REG              (xxx_GICD_REGS_BASE + 0x64F8) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_160_REG              (xxx_GICD_REGS_BASE + 0x6500) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_161_REG              (xxx_GICD_REGS_BASE + 0x6508) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_162_REG              (xxx_GICD_REGS_BASE + 0x6510) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_163_REG              (xxx_GICD_REGS_BASE + 0x6518) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_164_REG              (xxx_GICD_REGS_BASE + 0x6520) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_165_REG              (xxx_GICD_REGS_BASE + 0x6528) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_166_REG              (xxx_GICD_REGS_BASE + 0x6530) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_167_REG              (xxx_GICD_REGS_BASE + 0x6538) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_168_REG              (xxx_GICD_REGS_BASE + 0x6540) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_169_REG              (xxx_GICD_REGS_BASE + 0x6548) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_170_REG              (xxx_GICD_REGS_BASE + 0x6550) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_171_REG              (xxx_GICD_REGS_BASE + 0x6558) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_172_REG              (xxx_GICD_REGS_BASE + 0x6560) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_173_REG              (xxx_GICD_REGS_BASE + 0x6568) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_174_REG              (xxx_GICD_REGS_BASE + 0x6570) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_175_REG              (xxx_GICD_REGS_BASE + 0x6578) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_176_REG              (xxx_GICD_REGS_BASE + 0x6580) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_177_REG              (xxx_GICD_REGS_BASE + 0x6588) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_178_REG              (xxx_GICD_REGS_BASE + 0x6590) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_179_REG              (xxx_GICD_REGS_BASE + 0x6598) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_180_REG              (xxx_GICD_REGS_BASE + 0x65A0) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_181_REG              (xxx_GICD_REGS_BASE + 0x65A8) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_182_REG              (xxx_GICD_REGS_BASE + 0x65B0) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_183_REG              (xxx_GICD_REGS_BASE + 0x65B8) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_184_REG              (xxx_GICD_REGS_BASE + 0x65C0) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_185_REG              (xxx_GICD_REGS_BASE + 0x65C8) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_186_REG              (xxx_GICD_REGS_BASE + 0x65D0) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_187_REG              (xxx_GICD_REGS_BASE + 0x65D8) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_188_REG              (xxx_GICD_REGS_BASE + 0x65E0) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_189_REG              (xxx_GICD_REGS_BASE + 0x65E8) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_190_REG              (xxx_GICD_REGS_BASE + 0x65F0) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_191_REG              (xxx_GICD_REGS_BASE + 0x65F8) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_192_REG              (xxx_GICD_REGS_BASE + 0x6600) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_193_REG              (xxx_GICD_REGS_BASE + 0x6608) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_194_REG              (xxx_GICD_REGS_BASE + 0x6610) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_195_REG              (xxx_GICD_REGS_BASE + 0x6618) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_196_REG              (xxx_GICD_REGS_BASE + 0x6620) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_197_REG              (xxx_GICD_REGS_BASE + 0x6628) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_198_REG              (xxx_GICD_REGS_BASE + 0x6630) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_199_REG              (xxx_GICD_REGS_BASE + 0x6638) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_200_REG              (xxx_GICD_REGS_BASE + 0x6640) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_201_REG              (xxx_GICD_REGS_BASE + 0x6648) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_202_REG              (xxx_GICD_REGS_BASE + 0x6650) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_203_REG              (xxx_GICD_REGS_BASE + 0x6658) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_204_REG              (xxx_GICD_REGS_BASE + 0x6660) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_205_REG              (xxx_GICD_REGS_BASE + 0x6668) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_206_REG              (xxx_GICD_REGS_BASE + 0x6670) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_207_REG              (xxx_GICD_REGS_BASE + 0x6678) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_208_REG              (xxx_GICD_REGS_BASE + 0x6680) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_209_REG              (xxx_GICD_REGS_BASE + 0x6688) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_210_REG              (xxx_GICD_REGS_BASE + 0x6690) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_211_REG              (xxx_GICD_REGS_BASE + 0x6698) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_212_REG              (xxx_GICD_REGS_BASE + 0x66A0) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_213_REG              (xxx_GICD_REGS_BASE + 0x66A8) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_214_REG              (xxx_GICD_REGS_BASE + 0x66B0) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_215_REG              (xxx_GICD_REGS_BASE + 0x66B8) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_216_REG              (xxx_GICD_REGS_BASE + 0x66C0) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_217_REG              (xxx_GICD_REGS_BASE + 0x66C8) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_218_REG              (xxx_GICD_REGS_BASE + 0x66D0) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_219_REG              (xxx_GICD_REGS_BASE + 0x66D8) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_220_REG              (xxx_GICD_REGS_BASE + 0x66E0) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_221_REG              (xxx_GICD_REGS_BASE + 0x66E8) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_222_REG              (xxx_GICD_REGS_BASE + 0x66F0) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_223_REG              (xxx_GICD_REGS_BASE + 0x66F8) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_224_REG              (xxx_GICD_REGS_BASE + 0x6700) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_225_REG              (xxx_GICD_REGS_BASE + 0x6708) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_226_REG              (xxx_GICD_REGS_BASE + 0x6710) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_227_REG              (xxx_GICD_REGS_BASE + 0x6718) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_228_REG              (xxx_GICD_REGS_BASE + 0x6720) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_229_REG              (xxx_GICD_REGS_BASE + 0x6728) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_230_REG              (xxx_GICD_REGS_BASE + 0x6730) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_231_REG              (xxx_GICD_REGS_BASE + 0x6738) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_232_REG              (xxx_GICD_REGS_BASE + 0x6740) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_233_REG              (xxx_GICD_REGS_BASE + 0x6748) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_234_REG              (xxx_GICD_REGS_BASE + 0x6750) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_235_REG              (xxx_GICD_REGS_BASE + 0x6758) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_236_REG              (xxx_GICD_REGS_BASE + 0x6760) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_237_REG              (xxx_GICD_REGS_BASE + 0x6768) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_238_REG              (xxx_GICD_REGS_BASE + 0x6770) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_239_REG              (xxx_GICD_REGS_BASE + 0x6778) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_240_REG              (xxx_GICD_REGS_BASE + 0x6780) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_241_REG              (xxx_GICD_REGS_BASE + 0x6788) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_242_REG              (xxx_GICD_REGS_BASE + 0x6790) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_243_REG              (xxx_GICD_REGS_BASE + 0x6798) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_244_REG              (xxx_GICD_REGS_BASE + 0x67A0) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_245_REG              (xxx_GICD_REGS_BASE + 0x67A8) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_246_REG              (xxx_GICD_REGS_BASE + 0x67B0) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_247_REG              (xxx_GICD_REGS_BASE + 0x67B8) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_248_REG              (xxx_GICD_REGS_BASE + 0x67C0) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_249_REG              (xxx_GICD_REGS_BASE + 0x67C8) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_250_REG              (xxx_GICD_REGS_BASE + 0x67D0) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_251_REG              (xxx_GICD_REGS_BASE + 0x67D8) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_252_REG              (xxx_GICD_REGS_BASE + 0x67E0) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_253_REG              (xxx_GICD_REGS_BASE + 0x67E8) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_254_REG              (xxx_GICD_REGS_BASE + 0x67F0) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_255_REG              (xxx_GICD_REGS_BASE + 0x67F8) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_256_REG              (xxx_GICD_REGS_BASE + 0x6800) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_257_REG              (xxx_GICD_REGS_BASE + 0x6808) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_258_REG              (xxx_GICD_REGS_BASE + 0x6810) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_259_REG              (xxx_GICD_REGS_BASE + 0x6818) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_260_REG              (xxx_GICD_REGS_BASE + 0x6820) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_261_REG              (xxx_GICD_REGS_BASE + 0x6828) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_262_REG              (xxx_GICD_REGS_BASE + 0x6830) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_263_REG              (xxx_GICD_REGS_BASE + 0x6838) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_264_REG              (xxx_GICD_REGS_BASE + 0x6840) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_265_REG              (xxx_GICD_REGS_BASE + 0x6848) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_266_REG              (xxx_GICD_REGS_BASE + 0x6850) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_267_REG              (xxx_GICD_REGS_BASE + 0x6858) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_268_REG              (xxx_GICD_REGS_BASE + 0x6860) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_269_REG              (xxx_GICD_REGS_BASE + 0x6868) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_270_REG              (xxx_GICD_REGS_BASE + 0x6870) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_271_REG              (xxx_GICD_REGS_BASE + 0x6878) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_272_REG              (xxx_GICD_REGS_BASE + 0x6880) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_273_REG              (xxx_GICD_REGS_BASE + 0x6888) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_274_REG              (xxx_GICD_REGS_BASE + 0x6890) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_275_REG              (xxx_GICD_REGS_BASE + 0x6898) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_276_REG              (xxx_GICD_REGS_BASE + 0x68A0) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_277_REG              (xxx_GICD_REGS_BASE + 0x68A8) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_278_REG              (xxx_GICD_REGS_BASE + 0x68B0) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_279_REG              (xxx_GICD_REGS_BASE + 0x68B8) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_280_REG              (xxx_GICD_REGS_BASE + 0x68C0) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_281_REG              (xxx_GICD_REGS_BASE + 0x68C8) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_282_REG              (xxx_GICD_REGS_BASE + 0x68D0) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_283_REG              (xxx_GICD_REGS_BASE + 0x68D8) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_284_REG              (xxx_GICD_REGS_BASE + 0x68E0) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_285_REG              (xxx_GICD_REGS_BASE + 0x68E8) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_286_REG              (xxx_GICD_REGS_BASE + 0x68F0) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_287_REG              (xxx_GICD_REGS_BASE + 0x68F8) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_288_REG              (xxx_GICD_REGS_BASE + 0x6900) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_289_REG              (xxx_GICD_REGS_BASE + 0x6908) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_290_REG              (xxx_GICD_REGS_BASE + 0x6910) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_291_REG              (xxx_GICD_REGS_BASE + 0x6918) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_292_REG              (xxx_GICD_REGS_BASE + 0x6920) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_293_REG              (xxx_GICD_REGS_BASE + 0x6928) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_294_REG              (xxx_GICD_REGS_BASE + 0x6930) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_295_REG              (xxx_GICD_REGS_BASE + 0x6938) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_296_REG              (xxx_GICD_REGS_BASE + 0x6940) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_297_REG              (xxx_GICD_REGS_BASE + 0x6948) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_298_REG              (xxx_GICD_REGS_BASE + 0x6950) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_299_REG              (xxx_GICD_REGS_BASE + 0x6958) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_300_REG              (xxx_GICD_REGS_BASE + 0x6960) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_301_REG              (xxx_GICD_REGS_BASE + 0x6968) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_302_REG              (xxx_GICD_REGS_BASE + 0x6970) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_303_REG              (xxx_GICD_REGS_BASE + 0x6978) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_304_REG              (xxx_GICD_REGS_BASE + 0x6980) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_305_REG              (xxx_GICD_REGS_BASE + 0x6988) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_306_REG              (xxx_GICD_REGS_BASE + 0x6990) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_307_REG              (xxx_GICD_REGS_BASE + 0x6998) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_308_REG              (xxx_GICD_REGS_BASE + 0x69A0) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_309_REG              (xxx_GICD_REGS_BASE + 0x69A8) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_310_REG              (xxx_GICD_REGS_BASE + 0x69B0) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_311_REG              (xxx_GICD_REGS_BASE + 0x69B8) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_312_REG              (xxx_GICD_REGS_BASE + 0x69C0) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_313_REG              (xxx_GICD_REGS_BASE + 0x69C8) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_314_REG              (xxx_GICD_REGS_BASE + 0x69D0) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_315_REG              (xxx_GICD_REGS_BASE + 0x69D8) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_316_REG              (xxx_GICD_REGS_BASE + 0x69E0) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_317_REG              (xxx_GICD_REGS_BASE + 0x69E8) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_318_REG              (xxx_GICD_REGS_BASE + 0x69F0) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_319_REG              (xxx_GICD_REGS_BASE + 0x69F8) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_320_REG              (xxx_GICD_REGS_BASE + 0x6A00) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_321_REG              (xxx_GICD_REGS_BASE + 0x6A08) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_322_REG              (xxx_GICD_REGS_BASE + 0x6A10) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_323_REG              (xxx_GICD_REGS_BASE + 0x6A18) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_324_REG              (xxx_GICD_REGS_BASE + 0x6A20) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_325_REG              (xxx_GICD_REGS_BASE + 0x6A28) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_326_REG              (xxx_GICD_REGS_BASE + 0x6A30) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_327_REG              (xxx_GICD_REGS_BASE + 0x6A38) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_328_REG              (xxx_GICD_REGS_BASE + 0x6A40) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_329_REG              (xxx_GICD_REGS_BASE + 0x6A48) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_330_REG              (xxx_GICD_REGS_BASE + 0x6A50) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_331_REG              (xxx_GICD_REGS_BASE + 0x6A58) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_332_REG              (xxx_GICD_REGS_BASE + 0x6A60) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_333_REG              (xxx_GICD_REGS_BASE + 0x6A68) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_334_REG              (xxx_GICD_REGS_BASE + 0x6A70) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_335_REG              (xxx_GICD_REGS_BASE + 0x6A78) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_336_REG              (xxx_GICD_REGS_BASE + 0x6A80) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_337_REG              (xxx_GICD_REGS_BASE + 0x6A88) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_338_REG              (xxx_GICD_REGS_BASE + 0x6A90) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_339_REG              (xxx_GICD_REGS_BASE + 0x6A98) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_340_REG              (xxx_GICD_REGS_BASE + 0x6AA0) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_341_REG              (xxx_GICD_REGS_BASE + 0x6AA8) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_342_REG              (xxx_GICD_REGS_BASE + 0x6AB0) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_343_REG              (xxx_GICD_REGS_BASE + 0x6AB8) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_344_REG              (xxx_GICD_REGS_BASE + 0x6AC0) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_345_REG              (xxx_GICD_REGS_BASE + 0x6AC8) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_346_REG              (xxx_GICD_REGS_BASE + 0x6AD0) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_347_REG              (xxx_GICD_REGS_BASE + 0x6AD8) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_348_REG              (xxx_GICD_REGS_BASE + 0x6AE0) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_349_REG              (xxx_GICD_REGS_BASE + 0x6AE8) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_350_REG              (xxx_GICD_REGS_BASE + 0x6AF0) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_351_REG              (xxx_GICD_REGS_BASE + 0x6AF8) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_352_REG              (xxx_GICD_REGS_BASE + 0x6B00) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_353_REG              (xxx_GICD_REGS_BASE + 0x6B08) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_354_REG              (xxx_GICD_REGS_BASE + 0x6B10) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_355_REG              (xxx_GICD_REGS_BASE + 0x6B18) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_356_REG              (xxx_GICD_REGS_BASE + 0x6B20) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_357_REG              (xxx_GICD_REGS_BASE + 0x6B28) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_358_REG              (xxx_GICD_REGS_BASE + 0x6B30) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_359_REG              (xxx_GICD_REGS_BASE + 0x6B38) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_360_REG              (xxx_GICD_REGS_BASE + 0x6B40) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_361_REG              (xxx_GICD_REGS_BASE + 0x6B48) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_362_REG              (xxx_GICD_REGS_BASE + 0x6B50) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_363_REG              (xxx_GICD_REGS_BASE + 0x6B58) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_364_REG              (xxx_GICD_REGS_BASE + 0x6B60) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_365_REG              (xxx_GICD_REGS_BASE + 0x6B68) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_366_REG              (xxx_GICD_REGS_BASE + 0x6B70) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_367_REG              (xxx_GICD_REGS_BASE + 0x6B78) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_368_REG              (xxx_GICD_REGS_BASE + 0x6B80) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_369_REG              (xxx_GICD_REGS_BASE + 0x6B88) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_370_REG              (xxx_GICD_REGS_BASE + 0x6B90) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_371_REG              (xxx_GICD_REGS_BASE + 0x6B98) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_372_REG              (xxx_GICD_REGS_BASE + 0x6BA0) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_373_REG              (xxx_GICD_REGS_BASE + 0x6BA8) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_374_REG              (xxx_GICD_REGS_BASE + 0x6BB0) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_375_REG              (xxx_GICD_REGS_BASE + 0x6BB8) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_376_REG              (xxx_GICD_REGS_BASE + 0x6BC0) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_377_REG              (xxx_GICD_REGS_BASE + 0x6BC8) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_378_REG              (xxx_GICD_REGS_BASE + 0x6BD0) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_379_REG              (xxx_GICD_REGS_BASE + 0x6BD8) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_380_REG              (xxx_GICD_REGS_BASE + 0x6BE0) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_381_REG              (xxx_GICD_REGS_BASE + 0x6BE8) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_382_REG              (xxx_GICD_REGS_BASE + 0x6BF0) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_383_REG              (xxx_GICD_REGS_BASE + 0x6BF8) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_384_REG              (xxx_GICD_REGS_BASE + 0x6C00) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_385_REG              (xxx_GICD_REGS_BASE + 0x6C08) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_386_REG              (xxx_GICD_REGS_BASE + 0x6C10) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_387_REG              (xxx_GICD_REGS_BASE + 0x6C18) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_388_REG              (xxx_GICD_REGS_BASE + 0x6C20) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_389_REG              (xxx_GICD_REGS_BASE + 0x6C28) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_390_REG              (xxx_GICD_REGS_BASE + 0x6C30) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_391_REG              (xxx_GICD_REGS_BASE + 0x6C38) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_392_REG              (xxx_GICD_REGS_BASE + 0x6C40) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_393_REG              (xxx_GICD_REGS_BASE + 0x6C48) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_394_REG              (xxx_GICD_REGS_BASE + 0x6C50) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_395_REG              (xxx_GICD_REGS_BASE + 0x6C58) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_396_REG              (xxx_GICD_REGS_BASE + 0x6C60) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_397_REG              (xxx_GICD_REGS_BASE + 0x6C68) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_398_REG              (xxx_GICD_REGS_BASE + 0x6C70) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_399_REG              (xxx_GICD_REGS_BASE + 0x6C78) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_400_REG              (xxx_GICD_REGS_BASE + 0x6C80) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_401_REG              (xxx_GICD_REGS_BASE + 0x6C88) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_402_REG              (xxx_GICD_REGS_BASE + 0x6C90) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_403_REG              (xxx_GICD_REGS_BASE + 0x6C98) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_404_REG              (xxx_GICD_REGS_BASE + 0x6CA0) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_405_REG              (xxx_GICD_REGS_BASE + 0x6CA8) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_406_REG              (xxx_GICD_REGS_BASE + 0x6CB0) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_407_REG              (xxx_GICD_REGS_BASE + 0x6CB8) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_408_REG              (xxx_GICD_REGS_BASE + 0x6CC0) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_409_REG              (xxx_GICD_REGS_BASE + 0x6CC8) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_410_REG              (xxx_GICD_REGS_BASE + 0x6CD0) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_411_REG              (xxx_GICD_REGS_BASE + 0x6CD8) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_412_REG              (xxx_GICD_REGS_BASE + 0x6CE0) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_413_REG              (xxx_GICD_REGS_BASE + 0x6CE8) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_414_REG              (xxx_GICD_REGS_BASE + 0x6CF0) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_415_REG              (xxx_GICD_REGS_BASE + 0x6CF8) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_416_REG              (xxx_GICD_REGS_BASE + 0x6D00) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_417_REG              (xxx_GICD_REGS_BASE + 0x6D08) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_418_REG              (xxx_GICD_REGS_BASE + 0x6D10) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_419_REG              (xxx_GICD_REGS_BASE + 0x6D18) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_420_REG              (xxx_GICD_REGS_BASE + 0x6D20) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_421_REG              (xxx_GICD_REGS_BASE + 0x6D28) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_422_REG              (xxx_GICD_REGS_BASE + 0x6D30) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_423_REG              (xxx_GICD_REGS_BASE + 0x6D38) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_424_REG              (xxx_GICD_REGS_BASE + 0x6D40) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_425_REG              (xxx_GICD_REGS_BASE + 0x6D48) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_426_REG              (xxx_GICD_REGS_BASE + 0x6D50) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_427_REG              (xxx_GICD_REGS_BASE + 0x6D58) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_428_REG              (xxx_GICD_REGS_BASE + 0x6D60) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_429_REG              (xxx_GICD_REGS_BASE + 0x6D68) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_430_REG              (xxx_GICD_REGS_BASE + 0x6D70) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_431_REG              (xxx_GICD_REGS_BASE + 0x6D78) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_432_REG              (xxx_GICD_REGS_BASE + 0x6D80) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_433_REG              (xxx_GICD_REGS_BASE + 0x6D88) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_434_REG              (xxx_GICD_REGS_BASE + 0x6D90) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_435_REG              (xxx_GICD_REGS_BASE + 0x6D98) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_436_REG              (xxx_GICD_REGS_BASE + 0x6DA0) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_437_REG              (xxx_GICD_REGS_BASE + 0x6DA8) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_438_REG              (xxx_GICD_REGS_BASE + 0x6DB0) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_439_REG              (xxx_GICD_REGS_BASE + 0x6DB8) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_440_REG              (xxx_GICD_REGS_BASE + 0x6DC0) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_441_REG              (xxx_GICD_REGS_BASE + 0x6DC8) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_442_REG              (xxx_GICD_REGS_BASE + 0x6DD0) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_443_REG              (xxx_GICD_REGS_BASE + 0x6DD8) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_444_REG              (xxx_GICD_REGS_BASE + 0x6DE0) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_445_REG              (xxx_GICD_REGS_BASE + 0x6DE8) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_446_REG              (xxx_GICD_REGS_BASE + 0x6DF0) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_447_REG              (xxx_GICD_REGS_BASE + 0x6DF8) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_448_REG              (xxx_GICD_REGS_BASE + 0x6E00) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_449_REG              (xxx_GICD_REGS_BASE + 0x6E08) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_450_REG              (xxx_GICD_REGS_BASE + 0x6E10) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_451_REG              (xxx_GICD_REGS_BASE + 0x6E18) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_452_REG              (xxx_GICD_REGS_BASE + 0x6E20) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_453_REG              (xxx_GICD_REGS_BASE + 0x6E28) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_454_REG              (xxx_GICD_REGS_BASE + 0x6E30) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_455_REG              (xxx_GICD_REGS_BASE + 0x6E38) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_456_REG              (xxx_GICD_REGS_BASE + 0x6E40) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_457_REG              (xxx_GICD_REGS_BASE + 0x6E48) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_458_REG              (xxx_GICD_REGS_BASE + 0x6E50) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_459_REG              (xxx_GICD_REGS_BASE + 0x6E58) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_460_REG              (xxx_GICD_REGS_BASE + 0x6E60) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_461_REG              (xxx_GICD_REGS_BASE + 0x6E68) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_462_REG              (xxx_GICD_REGS_BASE + 0x6E70) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_463_REG              (xxx_GICD_REGS_BASE + 0x6E78) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_464_REG              (xxx_GICD_REGS_BASE + 0x6E80) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_465_REG              (xxx_GICD_REGS_BASE + 0x6E88) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_466_REG              (xxx_GICD_REGS_BASE + 0x6E90) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_467_REG              (xxx_GICD_REGS_BASE + 0x6E98) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_468_REG              (xxx_GICD_REGS_BASE + 0x6EA0) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_469_REG              (xxx_GICD_REGS_BASE + 0x6EA8) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_470_REG              (xxx_GICD_REGS_BASE + 0x6EB0) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_471_REG              (xxx_GICD_REGS_BASE + 0x6EB8) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_472_REG              (xxx_GICD_REGS_BASE + 0x6EC0) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_473_REG              (xxx_GICD_REGS_BASE + 0x6EC8) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_474_REG              (xxx_GICD_REGS_BASE + 0x6ED0) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_475_REG              (xxx_GICD_REGS_BASE + 0x6ED8) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_476_REG              (xxx_GICD_REGS_BASE + 0x6EE0) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_477_REG              (xxx_GICD_REGS_BASE + 0x6EE8) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_478_REG              (xxx_GICD_REGS_BASE + 0x6EF0) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_479_REG              (xxx_GICD_REGS_BASE + 0x6EF8) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_480_REG              (xxx_GICD_REGS_BASE + 0x6F00) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_481_REG              (xxx_GICD_REGS_BASE + 0x6F08) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_482_REG              (xxx_GICD_REGS_BASE + 0x6F10) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_483_REG              (xxx_GICD_REGS_BASE + 0x6F18) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_484_REG              (xxx_GICD_REGS_BASE + 0x6F20) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_485_REG              (xxx_GICD_REGS_BASE + 0x6F28) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_486_REG              (xxx_GICD_REGS_BASE + 0x6F30) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_487_REG              (xxx_GICD_REGS_BASE + 0x6F38) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_488_REG              (xxx_GICD_REGS_BASE + 0x6F40) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_489_REG              (xxx_GICD_REGS_BASE + 0x6F48) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_490_REG              (xxx_GICD_REGS_BASE + 0x6F50) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_491_REG              (xxx_GICD_REGS_BASE + 0x6F58) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_492_REG              (xxx_GICD_REGS_BASE + 0x6F60) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_493_REG              (xxx_GICD_REGS_BASE + 0x6F68) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_494_REG              (xxx_GICD_REGS_BASE + 0x6F70) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_495_REG              (xxx_GICD_REGS_BASE + 0x6F78) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_496_REG              (xxx_GICD_REGS_BASE + 0x6F80) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_497_REG              (xxx_GICD_REGS_BASE + 0x6F88) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_498_REG              (xxx_GICD_REGS_BASE + 0x6F90) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_499_REG              (xxx_GICD_REGS_BASE + 0x6F98) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_500_REG              (xxx_GICD_REGS_BASE + 0x6FA0) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_501_REG              (xxx_GICD_REGS_BASE + 0x6FA8) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_502_REG              (xxx_GICD_REGS_BASE + 0x6FB0) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_503_REG              (xxx_GICD_REGS_BASE + 0x6FB8) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_504_REG              (xxx_GICD_REGS_BASE + 0x6FC0) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_505_REG              (xxx_GICD_REGS_BASE + 0x6FC8) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_506_REG              (xxx_GICD_REGS_BASE + 0x6FD0) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_507_REG              (xxx_GICD_REGS_BASE + 0x6FD8) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_508_REG              (xxx_GICD_REGS_BASE + 0x6FE0) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_509_REG              (xxx_GICD_REGS_BASE + 0x6FE8) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_510_REG              (xxx_GICD_REGS_BASE + 0x6FF0) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_511_REG              (xxx_GICD_REGS_BASE + 0x6FF8) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_512_REG              (xxx_GICD_REGS_BASE + 0x7000) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_513_REG              (xxx_GICD_REGS_BASE + 0x7008) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_514_REG              (xxx_GICD_REGS_BASE + 0x7010) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_515_REG              (xxx_GICD_REGS_BASE + 0x7018) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_516_REG              (xxx_GICD_REGS_BASE + 0x7020) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_517_REG              (xxx_GICD_REGS_BASE + 0x7028) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_518_REG              (xxx_GICD_REGS_BASE + 0x7030) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_519_REG              (xxx_GICD_REGS_BASE + 0x7038) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_520_REG              (xxx_GICD_REGS_BASE + 0x7040) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_521_REG              (xxx_GICD_REGS_BASE + 0x7048) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_522_REG              (xxx_GICD_REGS_BASE + 0x7050) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_523_REG              (xxx_GICD_REGS_BASE + 0x7058) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_524_REG              (xxx_GICD_REGS_BASE + 0x7060) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_525_REG              (xxx_GICD_REGS_BASE + 0x7068) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_526_REG              (xxx_GICD_REGS_BASE + 0x7070) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_527_REG              (xxx_GICD_REGS_BASE + 0x7078) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_528_REG              (xxx_GICD_REGS_BASE + 0x7080) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_529_REG              (xxx_GICD_REGS_BASE + 0x7088) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_530_REG              (xxx_GICD_REGS_BASE + 0x7090) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_531_REG              (xxx_GICD_REGS_BASE + 0x7098) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_532_REG              (xxx_GICD_REGS_BASE + 0x70A0) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_533_REG              (xxx_GICD_REGS_BASE + 0x70A8) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_534_REG              (xxx_GICD_REGS_BASE + 0x70B0) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_535_REG              (xxx_GICD_REGS_BASE + 0x70B8) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_536_REG              (xxx_GICD_REGS_BASE + 0x70C0) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_537_REG              (xxx_GICD_REGS_BASE + 0x70C8) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_538_REG              (xxx_GICD_REGS_BASE + 0x70D0) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_539_REG              (xxx_GICD_REGS_BASE + 0x70D8) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_540_REG              (xxx_GICD_REGS_BASE + 0x70E0) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_541_REG              (xxx_GICD_REGS_BASE + 0x70E8) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_542_REG              (xxx_GICD_REGS_BASE + 0x70F0) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_543_REG              (xxx_GICD_REGS_BASE + 0x70F8) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_544_REG              (xxx_GICD_REGS_BASE + 0x7100) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_545_REG              (xxx_GICD_REGS_BASE + 0x7108) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_546_REG              (xxx_GICD_REGS_BASE + 0x7110) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_547_REG              (xxx_GICD_REGS_BASE + 0x7118) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_548_REG              (xxx_GICD_REGS_BASE + 0x7120) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_549_REG              (xxx_GICD_REGS_BASE + 0x7128) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_550_REG              (xxx_GICD_REGS_BASE + 0x7130) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_551_REG              (xxx_GICD_REGS_BASE + 0x7138) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_552_REG              (xxx_GICD_REGS_BASE + 0x7140) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_553_REG              (xxx_GICD_REGS_BASE + 0x7148) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_554_REG              (xxx_GICD_REGS_BASE + 0x7150) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_555_REG              (xxx_GICD_REGS_BASE + 0x7158) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_556_REG              (xxx_GICD_REGS_BASE + 0x7160) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_557_REG              (xxx_GICD_REGS_BASE + 0x7168) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_558_REG              (xxx_GICD_REGS_BASE + 0x7170) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_559_REG              (xxx_GICD_REGS_BASE + 0x7178) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_560_REG              (xxx_GICD_REGS_BASE + 0x7180) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_561_REG              (xxx_GICD_REGS_BASE + 0x7188) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_562_REG              (xxx_GICD_REGS_BASE + 0x7190) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_563_REG              (xxx_GICD_REGS_BASE + 0x7198) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_564_REG              (xxx_GICD_REGS_BASE + 0x71A0) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_565_REG              (xxx_GICD_REGS_BASE + 0x71A8) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_566_REG              (xxx_GICD_REGS_BASE + 0x71B0) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_567_REG              (xxx_GICD_REGS_BASE + 0x71B8) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_568_REG              (xxx_GICD_REGS_BASE + 0x71C0) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_569_REG              (xxx_GICD_REGS_BASE + 0x71C8) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_570_REG              (xxx_GICD_REGS_BASE + 0x71D0) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_571_REG              (xxx_GICD_REGS_BASE + 0x71D8) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_572_REG              (xxx_GICD_REGS_BASE + 0x71E0) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_573_REG              (xxx_GICD_REGS_BASE + 0x71E8) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_574_REG              (xxx_GICD_REGS_BASE + 0x71F0) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_575_REG              (xxx_GICD_REGS_BASE + 0x71F8) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_576_REG              (xxx_GICD_REGS_BASE + 0x7200) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_577_REG              (xxx_GICD_REGS_BASE + 0x7208) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_578_REG              (xxx_GICD_REGS_BASE + 0x7210) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_579_REG              (xxx_GICD_REGS_BASE + 0x7218) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_580_REG              (xxx_GICD_REGS_BASE + 0x7220) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_581_REG              (xxx_GICD_REGS_BASE + 0x7228) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_582_REG              (xxx_GICD_REGS_BASE + 0x7230) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_583_REG              (xxx_GICD_REGS_BASE + 0x7238) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_584_REG              (xxx_GICD_REGS_BASE + 0x7240) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_585_REG              (xxx_GICD_REGS_BASE + 0x7248) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_586_REG              (xxx_GICD_REGS_BASE + 0x7250) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_587_REG              (xxx_GICD_REGS_BASE + 0x7258) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_588_REG              (xxx_GICD_REGS_BASE + 0x7260) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_589_REG              (xxx_GICD_REGS_BASE + 0x7268) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_590_REG              (xxx_GICD_REGS_BASE + 0x7270) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_591_REG              (xxx_GICD_REGS_BASE + 0x7278) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_592_REG              (xxx_GICD_REGS_BASE + 0x7280) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_593_REG              (xxx_GICD_REGS_BASE + 0x7288) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_594_REG              (xxx_GICD_REGS_BASE + 0x7290) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_595_REG              (xxx_GICD_REGS_BASE + 0x7298) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_596_REG              (xxx_GICD_REGS_BASE + 0x72A0) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_597_REG              (xxx_GICD_REGS_BASE + 0x72A8) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_598_REG              (xxx_GICD_REGS_BASE + 0x72B0) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_599_REG              (xxx_GICD_REGS_BASE + 0x72B8) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_600_REG              (xxx_GICD_REGS_BASE + 0x72C0) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_601_REG              (xxx_GICD_REGS_BASE + 0x72C8) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_602_REG              (xxx_GICD_REGS_BASE + 0x72D0) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_603_REG              (xxx_GICD_REGS_BASE + 0x72D8) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_604_REG              (xxx_GICD_REGS_BASE + 0x72E0) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_605_REG              (xxx_GICD_REGS_BASE + 0x72E8) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_606_REG              (xxx_GICD_REGS_BASE + 0x72F0) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_607_REG              (xxx_GICD_REGS_BASE + 0x72F8) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_608_REG              (xxx_GICD_REGS_BASE + 0x7300) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_609_REG              (xxx_GICD_REGS_BASE + 0x7308) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_610_REG              (xxx_GICD_REGS_BASE + 0x7310) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_611_REG              (xxx_GICD_REGS_BASE + 0x7318) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_612_REG              (xxx_GICD_REGS_BASE + 0x7320) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_613_REG              (xxx_GICD_REGS_BASE + 0x7328) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_614_REG              (xxx_GICD_REGS_BASE + 0x7330) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_615_REG              (xxx_GICD_REGS_BASE + 0x7338) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_616_REG              (xxx_GICD_REGS_BASE + 0x7340) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_617_REG              (xxx_GICD_REGS_BASE + 0x7348) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_618_REG              (xxx_GICD_REGS_BASE + 0x7350) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_619_REG              (xxx_GICD_REGS_BASE + 0x7358) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_620_REG              (xxx_GICD_REGS_BASE + 0x7360) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_621_REG              (xxx_GICD_REGS_BASE + 0x7368) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_622_REG              (xxx_GICD_REGS_BASE + 0x7370) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_623_REG              (xxx_GICD_REGS_BASE + 0x7378) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_624_REG              (xxx_GICD_REGS_BASE + 0x7380) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_625_REG              (xxx_GICD_REGS_BASE + 0x7388) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_626_REG              (xxx_GICD_REGS_BASE + 0x7390) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_627_REG              (xxx_GICD_REGS_BASE + 0x7398) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_628_REG              (xxx_GICD_REGS_BASE + 0x73A0) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_629_REG              (xxx_GICD_REGS_BASE + 0x73A8) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_630_REG              (xxx_GICD_REGS_BASE + 0x73B0) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_631_REG              (xxx_GICD_REGS_BASE + 0x73B8) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_632_REG              (xxx_GICD_REGS_BASE + 0x73C0) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_633_REG              (xxx_GICD_REGS_BASE + 0x73C8) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_634_REG              (xxx_GICD_REGS_BASE + 0x73D0) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_635_REG              (xxx_GICD_REGS_BASE + 0x73D8) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_636_REG              (xxx_GICD_REGS_BASE + 0x73E0) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_637_REG              (xxx_GICD_REGS_BASE + 0x73E8) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_638_REG              (xxx_GICD_REGS_BASE + 0x73F0) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_639_REG              (xxx_GICD_REGS_BASE + 0x73F8) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_640_REG              (xxx_GICD_REGS_BASE + 0x7400) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_641_REG              (xxx_GICD_REGS_BASE + 0x7408) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_642_REG              (xxx_GICD_REGS_BASE + 0x7410) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_643_REG              (xxx_GICD_REGS_BASE + 0x7418) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_644_REG              (xxx_GICD_REGS_BASE + 0x7420) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_645_REG              (xxx_GICD_REGS_BASE + 0x7428) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_646_REG              (xxx_GICD_REGS_BASE + 0x7430) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_647_REG              (xxx_GICD_REGS_BASE + 0x7438) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_648_REG              (xxx_GICD_REGS_BASE + 0x7440) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_649_REG              (xxx_GICD_REGS_BASE + 0x7448) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_650_REG              (xxx_GICD_REGS_BASE + 0x7450) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_651_REG              (xxx_GICD_REGS_BASE + 0x7458) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_652_REG              (xxx_GICD_REGS_BASE + 0x7460) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_653_REG              (xxx_GICD_REGS_BASE + 0x7468) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_654_REG              (xxx_GICD_REGS_BASE + 0x7470) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_655_REG              (xxx_GICD_REGS_BASE + 0x7478) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_656_REG              (xxx_GICD_REGS_BASE + 0x7480) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_657_REG              (xxx_GICD_REGS_BASE + 0x7488) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_658_REG              (xxx_GICD_REGS_BASE + 0x7490) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_659_REG              (xxx_GICD_REGS_BASE + 0x7498) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_660_REG              (xxx_GICD_REGS_BASE + 0x74A0) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_661_REG              (xxx_GICD_REGS_BASE + 0x74A8) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_662_REG              (xxx_GICD_REGS_BASE + 0x74B0) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_663_REG              (xxx_GICD_REGS_BASE + 0x74B8) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_664_REG              (xxx_GICD_REGS_BASE + 0x74C0) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_665_REG              (xxx_GICD_REGS_BASE + 0x74C8) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_666_REG              (xxx_GICD_REGS_BASE + 0x74D0) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_667_REG              (xxx_GICD_REGS_BASE + 0x74D8) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_668_REG              (xxx_GICD_REGS_BASE + 0x74E0) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_669_REG              (xxx_GICD_REGS_BASE + 0x74E8) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_670_REG              (xxx_GICD_REGS_BASE + 0x74F0) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_671_REG              (xxx_GICD_REGS_BASE + 0x74F8) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_672_REG              (xxx_GICD_REGS_BASE + 0x7500) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_673_REG              (xxx_GICD_REGS_BASE + 0x7508) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_674_REG              (xxx_GICD_REGS_BASE + 0x7510) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_675_REG              (xxx_GICD_REGS_BASE + 0x7518) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_676_REG              (xxx_GICD_REGS_BASE + 0x7520) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_677_REG              (xxx_GICD_REGS_BASE + 0x7528) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_678_REG              (xxx_GICD_REGS_BASE + 0x7530) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_679_REG              (xxx_GICD_REGS_BASE + 0x7538) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_680_REG              (xxx_GICD_REGS_BASE + 0x7540) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_681_REG              (xxx_GICD_REGS_BASE + 0x7548) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_682_REG              (xxx_GICD_REGS_BASE + 0x7550) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_683_REG              (xxx_GICD_REGS_BASE + 0x7558) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_684_REG              (xxx_GICD_REGS_BASE + 0x7560) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_685_REG              (xxx_GICD_REGS_BASE + 0x7568) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_686_REG              (xxx_GICD_REGS_BASE + 0x7570) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_687_REG              (xxx_GICD_REGS_BASE + 0x7578) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_688_REG              (xxx_GICD_REGS_BASE + 0x7580) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_689_REG              (xxx_GICD_REGS_BASE + 0x7588) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_690_REG              (xxx_GICD_REGS_BASE + 0x7590) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_691_REG              (xxx_GICD_REGS_BASE + 0x7598) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_692_REG              (xxx_GICD_REGS_BASE + 0x75A0) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_693_REG              (xxx_GICD_REGS_BASE + 0x75A8) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_694_REG              (xxx_GICD_REGS_BASE + 0x75B0) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_695_REG              (xxx_GICD_REGS_BASE + 0x75B8) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_696_REG              (xxx_GICD_REGS_BASE + 0x75C0) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_697_REG              (xxx_GICD_REGS_BASE + 0x75C8) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_698_REG              (xxx_GICD_REGS_BASE + 0x75D0) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_699_REG              (xxx_GICD_REGS_BASE + 0x75D8) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_700_REG              (xxx_GICD_REGS_BASE + 0x75E0) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_701_REG              (xxx_GICD_REGS_BASE + 0x75E8) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_702_REG              (xxx_GICD_REGS_BASE + 0x75F0) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_703_REG              (xxx_GICD_REGS_BASE + 0x75F8) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_704_REG              (xxx_GICD_REGS_BASE + 0x7600) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_705_REG              (xxx_GICD_REGS_BASE + 0x7608) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_706_REG              (xxx_GICD_REGS_BASE + 0x7610) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_707_REG              (xxx_GICD_REGS_BASE + 0x7618) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_708_REG              (xxx_GICD_REGS_BASE + 0x7620) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_709_REG              (xxx_GICD_REGS_BASE + 0x7628) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_710_REG              (xxx_GICD_REGS_BASE + 0x7630) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_711_REG              (xxx_GICD_REGS_BASE + 0x7638) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_712_REG              (xxx_GICD_REGS_BASE + 0x7640) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_713_REG              (xxx_GICD_REGS_BASE + 0x7648) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_714_REG              (xxx_GICD_REGS_BASE + 0x7650) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_715_REG              (xxx_GICD_REGS_BASE + 0x7658) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_716_REG              (xxx_GICD_REGS_BASE + 0x7660) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_717_REG              (xxx_GICD_REGS_BASE + 0x7668) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_718_REG              (xxx_GICD_REGS_BASE + 0x7670) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_719_REG              (xxx_GICD_REGS_BASE + 0x7678) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_720_REG              (xxx_GICD_REGS_BASE + 0x7680) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_721_REG              (xxx_GICD_REGS_BASE + 0x7688) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_722_REG              (xxx_GICD_REGS_BASE + 0x7690) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_723_REG              (xxx_GICD_REGS_BASE + 0x7698) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_724_REG              (xxx_GICD_REGS_BASE + 0x76A0) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_725_REG              (xxx_GICD_REGS_BASE + 0x76A8) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_726_REG              (xxx_GICD_REGS_BASE + 0x76B0) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_727_REG              (xxx_GICD_REGS_BASE + 0x76B8) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_728_REG              (xxx_GICD_REGS_BASE + 0x76C0) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_729_REG              (xxx_GICD_REGS_BASE + 0x76C8) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_730_REG              (xxx_GICD_REGS_BASE + 0x76D0) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_731_REG              (xxx_GICD_REGS_BASE + 0x76D8) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_732_REG              (xxx_GICD_REGS_BASE + 0x76E0) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_733_REG              (xxx_GICD_REGS_BASE + 0x76E8) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_734_REG              (xxx_GICD_REGS_BASE + 0x76F0) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_735_REG              (xxx_GICD_REGS_BASE + 0x76F8) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_736_REG              (xxx_GICD_REGS_BASE + 0x7700) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_737_REG              (xxx_GICD_REGS_BASE + 0x7708) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_738_REG              (xxx_GICD_REGS_BASE + 0x7710) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_739_REG              (xxx_GICD_REGS_BASE + 0x7718) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_740_REG              (xxx_GICD_REGS_BASE + 0x7720) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_741_REG              (xxx_GICD_REGS_BASE + 0x7728) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_742_REG              (xxx_GICD_REGS_BASE + 0x7730) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_743_REG              (xxx_GICD_REGS_BASE + 0x7738) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_744_REG              (xxx_GICD_REGS_BASE + 0x7740) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_745_REG              (xxx_GICD_REGS_BASE + 0x7748) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_746_REG              (xxx_GICD_REGS_BASE + 0x7750) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_747_REG              (xxx_GICD_REGS_BASE + 0x7758) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_748_REG              (xxx_GICD_REGS_BASE + 0x7760) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_749_REG              (xxx_GICD_REGS_BASE + 0x7768) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_750_REG              (xxx_GICD_REGS_BASE + 0x7770) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_751_REG              (xxx_GICD_REGS_BASE + 0x7778) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_752_REG              (xxx_GICD_REGS_BASE + 0x7780) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_753_REG              (xxx_GICD_REGS_BASE + 0x7788) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_754_REG              (xxx_GICD_REGS_BASE + 0x7790) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_755_REG              (xxx_GICD_REGS_BASE + 0x7798) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_756_REG              (xxx_GICD_REGS_BASE + 0x77A0) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_757_REG              (xxx_GICD_REGS_BASE + 0x77A8) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_758_REG              (xxx_GICD_REGS_BASE + 0x77B0) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_759_REG              (xxx_GICD_REGS_BASE + 0x77B8) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_760_REG              (xxx_GICD_REGS_BASE + 0x77C0) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_761_REG              (xxx_GICD_REGS_BASE + 0x77C8) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_762_REG              (xxx_GICD_REGS_BASE + 0x77D0) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_763_REG              (xxx_GICD_REGS_BASE + 0x77D8) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_764_REG              (xxx_GICD_REGS_BASE + 0x77E0) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_765_REG              (xxx_GICD_REGS_BASE + 0x77E8) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_766_REG              (xxx_GICD_REGS_BASE + 0x77F0) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_767_REG              (xxx_GICD_REGS_BASE + 0x77F8) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_768_REG              (xxx_GICD_REGS_BASE + 0x7800) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_769_REG              (xxx_GICD_REGS_BASE + 0x7808) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_770_REG              (xxx_GICD_REGS_BASE + 0x7810) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_771_REG              (xxx_GICD_REGS_BASE + 0x7818) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_772_REG              (xxx_GICD_REGS_BASE + 0x7820) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_773_REG              (xxx_GICD_REGS_BASE + 0x7828) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_774_REG              (xxx_GICD_REGS_BASE + 0x7830) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_775_REG              (xxx_GICD_REGS_BASE + 0x7838) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_776_REG              (xxx_GICD_REGS_BASE + 0x7840) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_777_REG              (xxx_GICD_REGS_BASE + 0x7848) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_778_REG              (xxx_GICD_REGS_BASE + 0x7850) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_779_REG              (xxx_GICD_REGS_BASE + 0x7858) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_780_REG              (xxx_GICD_REGS_BASE + 0x7860) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_781_REG              (xxx_GICD_REGS_BASE + 0x7868) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_782_REG              (xxx_GICD_REGS_BASE + 0x7870) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_783_REG              (xxx_GICD_REGS_BASE + 0x7878) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_784_REG              (xxx_GICD_REGS_BASE + 0x7880) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_785_REG              (xxx_GICD_REGS_BASE + 0x7888) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_786_REG              (xxx_GICD_REGS_BASE + 0x7890) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_787_REG              (xxx_GICD_REGS_BASE + 0x7898) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_788_REG              (xxx_GICD_REGS_BASE + 0x78A0) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_789_REG              (xxx_GICD_REGS_BASE + 0x78A8) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_790_REG              (xxx_GICD_REGS_BASE + 0x78B0) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_791_REG              (xxx_GICD_REGS_BASE + 0x78B8) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_792_REG              (xxx_GICD_REGS_BASE + 0x78C0) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_793_REG              (xxx_GICD_REGS_BASE + 0x78C8) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_794_REG              (xxx_GICD_REGS_BASE + 0x78D0) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_795_REG              (xxx_GICD_REGS_BASE + 0x78D8) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_796_REG              (xxx_GICD_REGS_BASE + 0x78E0) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_797_REG              (xxx_GICD_REGS_BASE + 0x78E8) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_798_REG              (xxx_GICD_REGS_BASE + 0x78F0) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_799_REG              (xxx_GICD_REGS_BASE + 0x78F8) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_800_REG              (xxx_GICD_REGS_BASE + 0x7900) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_801_REG              (xxx_GICD_REGS_BASE + 0x7908) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_802_REG              (xxx_GICD_REGS_BASE + 0x7910) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_803_REG              (xxx_GICD_REGS_BASE + 0x7918) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_804_REG              (xxx_GICD_REGS_BASE + 0x7920) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_805_REG              (xxx_GICD_REGS_BASE + 0x7928) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_806_REG              (xxx_GICD_REGS_BASE + 0x7930) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_807_REG              (xxx_GICD_REGS_BASE + 0x7938) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_808_REG              (xxx_GICD_REGS_BASE + 0x7940) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_809_REG              (xxx_GICD_REGS_BASE + 0x7948) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_810_REG              (xxx_GICD_REGS_BASE + 0x7950) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_811_REG              (xxx_GICD_REGS_BASE + 0x7958) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_812_REG              (xxx_GICD_REGS_BASE + 0x7960) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_813_REG              (xxx_GICD_REGS_BASE + 0x7968) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_814_REG              (xxx_GICD_REGS_BASE + 0x7970) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_815_REG              (xxx_GICD_REGS_BASE + 0x7978) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_816_REG              (xxx_GICD_REGS_BASE + 0x7980) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_817_REG              (xxx_GICD_REGS_BASE + 0x7988) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_818_REG              (xxx_GICD_REGS_BASE + 0x7990) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_819_REG              (xxx_GICD_REGS_BASE + 0x7998) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_820_REG              (xxx_GICD_REGS_BASE + 0x79A0) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_821_REG              (xxx_GICD_REGS_BASE + 0x79A8) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_822_REG              (xxx_GICD_REGS_BASE + 0x79B0) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_823_REG              (xxx_GICD_REGS_BASE + 0x79B8) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_824_REG              (xxx_GICD_REGS_BASE + 0x79C0) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_825_REG              (xxx_GICD_REGS_BASE + 0x79C8) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_826_REG              (xxx_GICD_REGS_BASE + 0x79D0) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_827_REG              (xxx_GICD_REGS_BASE + 0x79D8) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_828_REG              (xxx_GICD_REGS_BASE + 0x79E0) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_829_REG              (xxx_GICD_REGS_BASE + 0x79E8) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_830_REG              (xxx_GICD_REGS_BASE + 0x79F0) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_831_REG              (xxx_GICD_REGS_BASE + 0x79F8) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_832_REG              (xxx_GICD_REGS_BASE + 0x7A00) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_833_REG              (xxx_GICD_REGS_BASE + 0x7A08) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_834_REG              (xxx_GICD_REGS_BASE + 0x7A10) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_835_REG              (xxx_GICD_REGS_BASE + 0x7A18) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_836_REG              (xxx_GICD_REGS_BASE + 0x7A20) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_837_REG              (xxx_GICD_REGS_BASE + 0x7A28) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_838_REG              (xxx_GICD_REGS_BASE + 0x7A30) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_839_REG              (xxx_GICD_REGS_BASE + 0x7A38) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_840_REG              (xxx_GICD_REGS_BASE + 0x7A40) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_841_REG              (xxx_GICD_REGS_BASE + 0x7A48) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_842_REG              (xxx_GICD_REGS_BASE + 0x7A50) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_843_REG              (xxx_GICD_REGS_BASE + 0x7A58) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_844_REG              (xxx_GICD_REGS_BASE + 0x7A60) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_845_REG              (xxx_GICD_REGS_BASE + 0x7A68) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_846_REG              (xxx_GICD_REGS_BASE + 0x7A70) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_847_REG              (xxx_GICD_REGS_BASE + 0x7A78) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_848_REG              (xxx_GICD_REGS_BASE + 0x7A80) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_849_REG              (xxx_GICD_REGS_BASE + 0x7A88) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_850_REG              (xxx_GICD_REGS_BASE + 0x7A90) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_851_REG              (xxx_GICD_REGS_BASE + 0x7A98) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_852_REG              (xxx_GICD_REGS_BASE + 0x7AA0) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_853_REG              (xxx_GICD_REGS_BASE + 0x7AA8) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_854_REG              (xxx_GICD_REGS_BASE + 0x7AB0) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_855_REG              (xxx_GICD_REGS_BASE + 0x7AB8) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_856_REG              (xxx_GICD_REGS_BASE + 0x7AC0) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_857_REG              (xxx_GICD_REGS_BASE + 0x7AC8) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_858_REG              (xxx_GICD_REGS_BASE + 0x7AD0) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_859_REG              (xxx_GICD_REGS_BASE + 0x7AD8) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_860_REG              (xxx_GICD_REGS_BASE + 0x7AE0) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_861_REG              (xxx_GICD_REGS_BASE + 0x7AE8) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_862_REG              (xxx_GICD_REGS_BASE + 0x7AF0) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_863_REG              (xxx_GICD_REGS_BASE + 0x7AF8) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_864_REG              (xxx_GICD_REGS_BASE + 0x7B00) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_865_REG              (xxx_GICD_REGS_BASE + 0x7B08) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_866_REG              (xxx_GICD_REGS_BASE + 0x7B10) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_867_REG              (xxx_GICD_REGS_BASE + 0x7B18) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_868_REG              (xxx_GICD_REGS_BASE + 0x7B20) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_869_REG              (xxx_GICD_REGS_BASE + 0x7B28) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_870_REG              (xxx_GICD_REGS_BASE + 0x7B30) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_871_REG              (xxx_GICD_REGS_BASE + 0x7B38) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_872_REG              (xxx_GICD_REGS_BASE + 0x7B40) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_873_REG              (xxx_GICD_REGS_BASE + 0x7B48) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_874_REG              (xxx_GICD_REGS_BASE + 0x7B50) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_875_REG              (xxx_GICD_REGS_BASE + 0x7B58) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_876_REG              (xxx_GICD_REGS_BASE + 0x7B60) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_877_REG              (xxx_GICD_REGS_BASE + 0x7B68) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_878_REG              (xxx_GICD_REGS_BASE + 0x7B70) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_879_REG              (xxx_GICD_REGS_BASE + 0x7B78) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_880_REG              (xxx_GICD_REGS_BASE + 0x7B80) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_881_REG              (xxx_GICD_REGS_BASE + 0x7B88) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_882_REG              (xxx_GICD_REGS_BASE + 0x7B90) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_883_REG              (xxx_GICD_REGS_BASE + 0x7B98) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_884_REG              (xxx_GICD_REGS_BASE + 0x7BA0) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_885_REG              (xxx_GICD_REGS_BASE + 0x7BA8) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_886_REG              (xxx_GICD_REGS_BASE + 0x7BB0) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_887_REG              (xxx_GICD_REGS_BASE + 0x7BB8) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_888_REG              (xxx_GICD_REGS_BASE + 0x7BC0) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_889_REG              (xxx_GICD_REGS_BASE + 0x7BC8) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_890_REG              (xxx_GICD_REGS_BASE + 0x7BD0) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_891_REG              (xxx_GICD_REGS_BASE + 0x7BD8) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_892_REG              (xxx_GICD_REGS_BASE + 0x7BE0) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_893_REG              (xxx_GICD_REGS_BASE + 0x7BE8) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_894_REG              (xxx_GICD_REGS_BASE + 0x7BF0) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_895_REG              (xxx_GICD_REGS_BASE + 0x7BF8) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_896_REG              (xxx_GICD_REGS_BASE + 0x7C00) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_897_REG              (xxx_GICD_REGS_BASE + 0x7C08) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_898_REG              (xxx_GICD_REGS_BASE + 0x7C10) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_899_REG              (xxx_GICD_REGS_BASE + 0x7C18) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_900_REG              (xxx_GICD_REGS_BASE + 0x7C20) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_901_REG              (xxx_GICD_REGS_BASE + 0x7C28) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_902_REG              (xxx_GICD_REGS_BASE + 0x7C30) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_903_REG              (xxx_GICD_REGS_BASE + 0x7C38) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_904_REG              (xxx_GICD_REGS_BASE + 0x7C40) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_905_REG              (xxx_GICD_REGS_BASE + 0x7C48) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_906_REG              (xxx_GICD_REGS_BASE + 0x7C50) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_907_REG              (xxx_GICD_REGS_BASE + 0x7C58) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_908_REG              (xxx_GICD_REGS_BASE + 0x7C60) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_909_REG              (xxx_GICD_REGS_BASE + 0x7C68) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_910_REG              (xxx_GICD_REGS_BASE + 0x7C70) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_911_REG              (xxx_GICD_REGS_BASE + 0x7C78) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_912_REG              (xxx_GICD_REGS_BASE + 0x7C80) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_913_REG              (xxx_GICD_REGS_BASE + 0x7C88) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_914_REG              (xxx_GICD_REGS_BASE + 0x7C90) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_915_REG              (xxx_GICD_REGS_BASE + 0x7C98) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_916_REG              (xxx_GICD_REGS_BASE + 0x7CA0) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_917_REG              (xxx_GICD_REGS_BASE + 0x7CA8) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_918_REG              (xxx_GICD_REGS_BASE + 0x7CB0) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_919_REG              (xxx_GICD_REGS_BASE + 0x7CB8) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_920_REG              (xxx_GICD_REGS_BASE + 0x7CC0) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_921_REG              (xxx_GICD_REGS_BASE + 0x7CC8) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_922_REG              (xxx_GICD_REGS_BASE + 0x7CD0) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_923_REG              (xxx_GICD_REGS_BASE + 0x7CD8) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_924_REG              (xxx_GICD_REGS_BASE + 0x7CE0) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_925_REG              (xxx_GICD_REGS_BASE + 0x7CE8) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_926_REG              (xxx_GICD_REGS_BASE + 0x7CF0) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_927_REG              (xxx_GICD_REGS_BASE + 0x7CF8) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_928_REG              (xxx_GICD_REGS_BASE + 0x7D00) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_929_REG              (xxx_GICD_REGS_BASE + 0x7D08) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_930_REG              (xxx_GICD_REGS_BASE + 0x7D10) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_931_REG              (xxx_GICD_REGS_BASE + 0x7D18) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_932_REG              (xxx_GICD_REGS_BASE + 0x7D20) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_933_REG              (xxx_GICD_REGS_BASE + 0x7D28) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_934_REG              (xxx_GICD_REGS_BASE + 0x7D30) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_935_REG              (xxx_GICD_REGS_BASE + 0x7D38) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_936_REG              (xxx_GICD_REGS_BASE + 0x7D40) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_937_REG              (xxx_GICD_REGS_BASE + 0x7D48) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_938_REG              (xxx_GICD_REGS_BASE + 0x7D50) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_939_REG              (xxx_GICD_REGS_BASE + 0x7D58) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_940_REG              (xxx_GICD_REGS_BASE + 0x7D60) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_941_REG              (xxx_GICD_REGS_BASE + 0x7D68) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_942_REG              (xxx_GICD_REGS_BASE + 0x7D70) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_943_REG              (xxx_GICD_REGS_BASE + 0x7D78) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_944_REG              (xxx_GICD_REGS_BASE + 0x7D80) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_945_REG              (xxx_GICD_REGS_BASE + 0x7D88) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_946_REG              (xxx_GICD_REGS_BASE + 0x7D90) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_947_REG              (xxx_GICD_REGS_BASE + 0x7D98) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_948_REG              (xxx_GICD_REGS_BASE + 0x7DA0) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_949_REG              (xxx_GICD_REGS_BASE + 0x7DA8) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_950_REG              (xxx_GICD_REGS_BASE + 0x7DB0) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_951_REG              (xxx_GICD_REGS_BASE + 0x7DB8) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_952_REG              (xxx_GICD_REGS_BASE + 0x7DC0) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_953_REG              (xxx_GICD_REGS_BASE + 0x7DC8) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_954_REG              (xxx_GICD_REGS_BASE + 0x7DD0) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_955_REG              (xxx_GICD_REGS_BASE + 0x7DD8) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_956_REG              (xxx_GICD_REGS_BASE + 0x7DE0) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_957_REG              (xxx_GICD_REGS_BASE + 0x7DE8) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_958_REG              (xxx_GICD_REGS_BASE + 0x7DF0) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_959_REG              (xxx_GICD_REGS_BASE + 0x7DF8) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_960_REG              (xxx_GICD_REGS_BASE + 0x7E00) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_961_REG              (xxx_GICD_REGS_BASE + 0x7E08) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_962_REG              (xxx_GICD_REGS_BASE + 0x7E10) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_963_REG              (xxx_GICD_REGS_BASE + 0x7E18) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_964_REG              (xxx_GICD_REGS_BASE + 0x7E20) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_965_REG              (xxx_GICD_REGS_BASE + 0x7E28) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_966_REG              (xxx_GICD_REGS_BASE + 0x7E30) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_967_REG              (xxx_GICD_REGS_BASE + 0x7E38) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_968_REG              (xxx_GICD_REGS_BASE + 0x7E40) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_969_REG              (xxx_GICD_REGS_BASE + 0x7E48) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_970_REG              (xxx_GICD_REGS_BASE + 0x7E50) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_971_REG              (xxx_GICD_REGS_BASE + 0x7E58) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_972_REG              (xxx_GICD_REGS_BASE + 0x7E60) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_973_REG              (xxx_GICD_REGS_BASE + 0x7E68) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_974_REG              (xxx_GICD_REGS_BASE + 0x7E70) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_975_REG              (xxx_GICD_REGS_BASE + 0x7E78) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_976_REG              (xxx_GICD_REGS_BASE + 0x7E80) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_977_REG              (xxx_GICD_REGS_BASE + 0x7E88) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_978_REG              (xxx_GICD_REGS_BASE + 0x7E90) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_979_REG              (xxx_GICD_REGS_BASE + 0x7E98) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_980_REG              (xxx_GICD_REGS_BASE + 0x7EA0) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_981_REG              (xxx_GICD_REGS_BASE + 0x7EA8) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_982_REG              (xxx_GICD_REGS_BASE + 0x7EB0) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_983_REG              (xxx_GICD_REGS_BASE + 0x7EB8) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_984_REG              (xxx_GICD_REGS_BASE + 0x7EC0) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_985_REG              (xxx_GICD_REGS_BASE + 0x7EC8) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_986_REG              (xxx_GICD_REGS_BASE + 0x7ED0) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_987_REG              (xxx_GICD_REGS_BASE + 0x7ED8) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_988_REG              (xxx_GICD_REGS_BASE + 0x7EE0) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_989_REG              (xxx_GICD_REGS_BASE + 0x7EE8) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_990_REG              (xxx_GICD_REGS_BASE + 0x7EF0) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_991_REG              (xxx_GICD_REGS_BASE + 0x7EF8) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_992_REG              (xxx_GICD_REGS_BASE + 0x7F00) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_993_REG              (xxx_GICD_REGS_BASE + 0x7F08) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_994_REG              (xxx_GICD_REGS_BASE + 0x7F10) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_995_REG              (xxx_GICD_REGS_BASE + 0x7F18) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_996_REG              (xxx_GICD_REGS_BASE + 0x7F20) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_997_REG              (xxx_GICD_REGS_BASE + 0x7F28) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_998_REG              (xxx_GICD_REGS_BASE + 0x7F30) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_999_REG              (xxx_GICD_REGS_BASE + 0x7F38) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_1000_REG             (xxx_GICD_REGS_BASE + 0x7F40) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_1001_REG             (xxx_GICD_REGS_BASE + 0x7F48) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_1002_REG             (xxx_GICD_REGS_BASE + 0x7F50) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_1003_REG             (xxx_GICD_REGS_BASE + 0x7F58) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_1004_REG             (xxx_GICD_REGS_BASE + 0x7F60) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_1005_REG             (xxx_GICD_REGS_BASE + 0x7F68) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_1006_REG             (xxx_GICD_REGS_BASE + 0x7F70) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_1007_REG             (xxx_GICD_REGS_BASE + 0x7F78) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_1008_REG             (xxx_GICD_REGS_BASE + 0x7F80) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_1009_REG             (xxx_GICD_REGS_BASE + 0x7F88) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_1010_REG             (xxx_GICD_REGS_BASE + 0x7F90) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_1011_REG             (xxx_GICD_REGS_BASE + 0x7F98) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_1012_REG             (xxx_GICD_REGS_BASE + 0x7FA0) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_1013_REG             (xxx_GICD_REGS_BASE + 0x7FA8) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_1014_REG             (xxx_GICD_REGS_BASE + 0x7FB0) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_1015_REG             (xxx_GICD_REGS_BASE + 0x7FB8) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_1016_REG             (xxx_GICD_REGS_BASE + 0x7FC0) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_1017_REG             (xxx_GICD_REGS_BASE + 0x7FC8) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_1018_REG             (xxx_GICD_REGS_BASE + 0x7FD0) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_1019_REG             (xxx_GICD_REGS_BASE + 0x7FD8) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_1020_REG             (xxx_GICD_REGS_BASE + 0x7FE0) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_1021_REG             (xxx_GICD_REGS_BASE + 0x7FE8) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_1022_REG             (xxx_GICD_REGS_BASE + 0x7FF0) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_L_1023_REG             (xxx_GICD_REGS_BASE + 0x7FF8) /* 亲和性路由寄存器（[31:0]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_0_REG                (xxx_GICD_REGS_BASE + 0x6004) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_1_REG                (xxx_GICD_REGS_BASE + 0x600C) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_2_REG                (xxx_GICD_REGS_BASE + 0x6014) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_3_REG                (xxx_GICD_REGS_BASE + 0x601C) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_4_REG                (xxx_GICD_REGS_BASE + 0x6024) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_5_REG                (xxx_GICD_REGS_BASE + 0x602C) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_6_REG                (xxx_GICD_REGS_BASE + 0x6034) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_7_REG                (xxx_GICD_REGS_BASE + 0x603C) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_8_REG                (xxx_GICD_REGS_BASE + 0x6044) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_9_REG                (xxx_GICD_REGS_BASE + 0x604C) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_10_REG               (xxx_GICD_REGS_BASE + 0x6054) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_11_REG               (xxx_GICD_REGS_BASE + 0x605C) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_12_REG               (xxx_GICD_REGS_BASE + 0x6064) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_13_REG               (xxx_GICD_REGS_BASE + 0x606C) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_14_REG               (xxx_GICD_REGS_BASE + 0x6074) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_15_REG               (xxx_GICD_REGS_BASE + 0x607C) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_16_REG               (xxx_GICD_REGS_BASE + 0x6084) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_17_REG               (xxx_GICD_REGS_BASE + 0x608C) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_18_REG               (xxx_GICD_REGS_BASE + 0x6094) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_19_REG               (xxx_GICD_REGS_BASE + 0x609C) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_20_REG               (xxx_GICD_REGS_BASE + 0x60A4) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_21_REG               (xxx_GICD_REGS_BASE + 0x60AC) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_22_REG               (xxx_GICD_REGS_BASE + 0x60B4) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_23_REG               (xxx_GICD_REGS_BASE + 0x60BC) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_24_REG               (xxx_GICD_REGS_BASE + 0x60C4) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_25_REG               (xxx_GICD_REGS_BASE + 0x60CC) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_26_REG               (xxx_GICD_REGS_BASE + 0x60D4) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_27_REG               (xxx_GICD_REGS_BASE + 0x60DC) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_28_REG               (xxx_GICD_REGS_BASE + 0x60E4) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_29_REG               (xxx_GICD_REGS_BASE + 0x60EC) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_30_REG               (xxx_GICD_REGS_BASE + 0x60F4) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_31_REG               (xxx_GICD_REGS_BASE + 0x60FC) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_32_REG               (xxx_GICD_REGS_BASE + 0x6104) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_33_REG               (xxx_GICD_REGS_BASE + 0x610C) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_34_REG               (xxx_GICD_REGS_BASE + 0x6114) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_35_REG               (xxx_GICD_REGS_BASE + 0x611C) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_36_REG               (xxx_GICD_REGS_BASE + 0x6124) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_37_REG               (xxx_GICD_REGS_BASE + 0x612C) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_38_REG               (xxx_GICD_REGS_BASE + 0x6134) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_39_REG               (xxx_GICD_REGS_BASE + 0x613C) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_40_REG               (xxx_GICD_REGS_BASE + 0x6144) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_41_REG               (xxx_GICD_REGS_BASE + 0x614C) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_42_REG               (xxx_GICD_REGS_BASE + 0x6154) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_43_REG               (xxx_GICD_REGS_BASE + 0x615C) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_44_REG               (xxx_GICD_REGS_BASE + 0x6164) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_45_REG               (xxx_GICD_REGS_BASE + 0x616C) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_46_REG               (xxx_GICD_REGS_BASE + 0x6174) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_47_REG               (xxx_GICD_REGS_BASE + 0x617C) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_48_REG               (xxx_GICD_REGS_BASE + 0x6184) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_49_REG               (xxx_GICD_REGS_BASE + 0x618C) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_50_REG               (xxx_GICD_REGS_BASE + 0x6194) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_51_REG               (xxx_GICD_REGS_BASE + 0x619C) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_52_REG               (xxx_GICD_REGS_BASE + 0x61A4) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_53_REG               (xxx_GICD_REGS_BASE + 0x61AC) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_54_REG               (xxx_GICD_REGS_BASE + 0x61B4) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_55_REG               (xxx_GICD_REGS_BASE + 0x61BC) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_56_REG               (xxx_GICD_REGS_BASE + 0x61C4) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_57_REG               (xxx_GICD_REGS_BASE + 0x61CC) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_58_REG               (xxx_GICD_REGS_BASE + 0x61D4) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_59_REG               (xxx_GICD_REGS_BASE + 0x61DC) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_60_REG               (xxx_GICD_REGS_BASE + 0x61E4) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_61_REG               (xxx_GICD_REGS_BASE + 0x61EC) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_62_REG               (xxx_GICD_REGS_BASE + 0x61F4) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_63_REG               (xxx_GICD_REGS_BASE + 0x61FC) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_64_REG               (xxx_GICD_REGS_BASE + 0x6204) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_65_REG               (xxx_GICD_REGS_BASE + 0x620C) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_66_REG               (xxx_GICD_REGS_BASE + 0x6214) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_67_REG               (xxx_GICD_REGS_BASE + 0x621C) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_68_REG               (xxx_GICD_REGS_BASE + 0x6224) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_69_REG               (xxx_GICD_REGS_BASE + 0x622C) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_70_REG               (xxx_GICD_REGS_BASE + 0x6234) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_71_REG               (xxx_GICD_REGS_BASE + 0x623C) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_72_REG               (xxx_GICD_REGS_BASE + 0x6244) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_73_REG               (xxx_GICD_REGS_BASE + 0x624C) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_74_REG               (xxx_GICD_REGS_BASE + 0x6254) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_75_REG               (xxx_GICD_REGS_BASE + 0x625C) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_76_REG               (xxx_GICD_REGS_BASE + 0x6264) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_77_REG               (xxx_GICD_REGS_BASE + 0x626C) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_78_REG               (xxx_GICD_REGS_BASE + 0x6274) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_79_REG               (xxx_GICD_REGS_BASE + 0x627C) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_80_REG               (xxx_GICD_REGS_BASE + 0x6284) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_81_REG               (xxx_GICD_REGS_BASE + 0x628C) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_82_REG               (xxx_GICD_REGS_BASE + 0x6294) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_83_REG               (xxx_GICD_REGS_BASE + 0x629C) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_84_REG               (xxx_GICD_REGS_BASE + 0x62A4) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_85_REG               (xxx_GICD_REGS_BASE + 0x62AC) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_86_REG               (xxx_GICD_REGS_BASE + 0x62B4) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_87_REG               (xxx_GICD_REGS_BASE + 0x62BC) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_88_REG               (xxx_GICD_REGS_BASE + 0x62C4) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_89_REG               (xxx_GICD_REGS_BASE + 0x62CC) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_90_REG               (xxx_GICD_REGS_BASE + 0x62D4) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_91_REG               (xxx_GICD_REGS_BASE + 0x62DC) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_92_REG               (xxx_GICD_REGS_BASE + 0x62E4) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_93_REG               (xxx_GICD_REGS_BASE + 0x62EC) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_94_REG               (xxx_GICD_REGS_BASE + 0x62F4) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_95_REG               (xxx_GICD_REGS_BASE + 0x62FC) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_96_REG               (xxx_GICD_REGS_BASE + 0x6304) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_97_REG               (xxx_GICD_REGS_BASE + 0x630C) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_98_REG               (xxx_GICD_REGS_BASE + 0x6314) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_99_REG               (xxx_GICD_REGS_BASE + 0x631C) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_100_REG              (xxx_GICD_REGS_BASE + 0x6324) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_101_REG              (xxx_GICD_REGS_BASE + 0x632C) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_102_REG              (xxx_GICD_REGS_BASE + 0x6334) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_103_REG              (xxx_GICD_REGS_BASE + 0x633C) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_104_REG              (xxx_GICD_REGS_BASE + 0x6344) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_105_REG              (xxx_GICD_REGS_BASE + 0x634C) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_106_REG              (xxx_GICD_REGS_BASE + 0x6354) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_107_REG              (xxx_GICD_REGS_BASE + 0x635C) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_108_REG              (xxx_GICD_REGS_BASE + 0x6364) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_109_REG              (xxx_GICD_REGS_BASE + 0x636C) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_110_REG              (xxx_GICD_REGS_BASE + 0x6374) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_111_REG              (xxx_GICD_REGS_BASE + 0x637C) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_112_REG              (xxx_GICD_REGS_BASE + 0x6384) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_113_REG              (xxx_GICD_REGS_BASE + 0x638C) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_114_REG              (xxx_GICD_REGS_BASE + 0x6394) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_115_REG              (xxx_GICD_REGS_BASE + 0x639C) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_116_REG              (xxx_GICD_REGS_BASE + 0x63A4) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_117_REG              (xxx_GICD_REGS_BASE + 0x63AC) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_118_REG              (xxx_GICD_REGS_BASE + 0x63B4) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_119_REG              (xxx_GICD_REGS_BASE + 0x63BC) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_120_REG              (xxx_GICD_REGS_BASE + 0x63C4) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_121_REG              (xxx_GICD_REGS_BASE + 0x63CC) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_122_REG              (xxx_GICD_REGS_BASE + 0x63D4) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_123_REG              (xxx_GICD_REGS_BASE + 0x63DC) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_124_REG              (xxx_GICD_REGS_BASE + 0x63E4) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_125_REG              (xxx_GICD_REGS_BASE + 0x63EC) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_126_REG              (xxx_GICD_REGS_BASE + 0x63F4) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_127_REG              (xxx_GICD_REGS_BASE + 0x63FC) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_128_REG              (xxx_GICD_REGS_BASE + 0x6404) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_129_REG              (xxx_GICD_REGS_BASE + 0x640C) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_130_REG              (xxx_GICD_REGS_BASE + 0x6414) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_131_REG              (xxx_GICD_REGS_BASE + 0x641C) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_132_REG              (xxx_GICD_REGS_BASE + 0x6424) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_133_REG              (xxx_GICD_REGS_BASE + 0x642C) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_134_REG              (xxx_GICD_REGS_BASE + 0x6434) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_135_REG              (xxx_GICD_REGS_BASE + 0x643C) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_136_REG              (xxx_GICD_REGS_BASE + 0x6444) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_137_REG              (xxx_GICD_REGS_BASE + 0x644C) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_138_REG              (xxx_GICD_REGS_BASE + 0x6454) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_139_REG              (xxx_GICD_REGS_BASE + 0x645C) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_140_REG              (xxx_GICD_REGS_BASE + 0x6464) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_141_REG              (xxx_GICD_REGS_BASE + 0x646C) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_142_REG              (xxx_GICD_REGS_BASE + 0x6474) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_143_REG              (xxx_GICD_REGS_BASE + 0x647C) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_144_REG              (xxx_GICD_REGS_BASE + 0x6484) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_145_REG              (xxx_GICD_REGS_BASE + 0x648C) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_146_REG              (xxx_GICD_REGS_BASE + 0x6494) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_147_REG              (xxx_GICD_REGS_BASE + 0x649C) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_148_REG              (xxx_GICD_REGS_BASE + 0x64A4) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_149_REG              (xxx_GICD_REGS_BASE + 0x64AC) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_150_REG              (xxx_GICD_REGS_BASE + 0x64B4) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_151_REG              (xxx_GICD_REGS_BASE + 0x64BC) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_152_REG              (xxx_GICD_REGS_BASE + 0x64C4) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_153_REG              (xxx_GICD_REGS_BASE + 0x64CC) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_154_REG              (xxx_GICD_REGS_BASE + 0x64D4) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_155_REG              (xxx_GICD_REGS_BASE + 0x64DC) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_156_REG              (xxx_GICD_REGS_BASE + 0x64E4) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_157_REG              (xxx_GICD_REGS_BASE + 0x64EC) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_158_REG              (xxx_GICD_REGS_BASE + 0x64F4) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_159_REG              (xxx_GICD_REGS_BASE + 0x64FC) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_160_REG              (xxx_GICD_REGS_BASE + 0x6504) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_161_REG              (xxx_GICD_REGS_BASE + 0x650C) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_162_REG              (xxx_GICD_REGS_BASE + 0x6514) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_163_REG              (xxx_GICD_REGS_BASE + 0x651C) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_164_REG              (xxx_GICD_REGS_BASE + 0x6524) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_165_REG              (xxx_GICD_REGS_BASE + 0x652C) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_166_REG              (xxx_GICD_REGS_BASE + 0x6534) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_167_REG              (xxx_GICD_REGS_BASE + 0x653C) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_168_REG              (xxx_GICD_REGS_BASE + 0x6544) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_169_REG              (xxx_GICD_REGS_BASE + 0x654C) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_170_REG              (xxx_GICD_REGS_BASE + 0x6554) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_171_REG              (xxx_GICD_REGS_BASE + 0x655C) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_172_REG              (xxx_GICD_REGS_BASE + 0x6564) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_173_REG              (xxx_GICD_REGS_BASE + 0x656C) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_174_REG              (xxx_GICD_REGS_BASE + 0x6574) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_175_REG              (xxx_GICD_REGS_BASE + 0x657C) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_176_REG              (xxx_GICD_REGS_BASE + 0x6584) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_177_REG              (xxx_GICD_REGS_BASE + 0x658C) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_178_REG              (xxx_GICD_REGS_BASE + 0x6594) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_179_REG              (xxx_GICD_REGS_BASE + 0x659C) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_180_REG              (xxx_GICD_REGS_BASE + 0x65A4) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_181_REG              (xxx_GICD_REGS_BASE + 0x65AC) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_182_REG              (xxx_GICD_REGS_BASE + 0x65B4) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_183_REG              (xxx_GICD_REGS_BASE + 0x65BC) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_184_REG              (xxx_GICD_REGS_BASE + 0x65C4) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_185_REG              (xxx_GICD_REGS_BASE + 0x65CC) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_186_REG              (xxx_GICD_REGS_BASE + 0x65D4) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_187_REG              (xxx_GICD_REGS_BASE + 0x65DC) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_188_REG              (xxx_GICD_REGS_BASE + 0x65E4) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_189_REG              (xxx_GICD_REGS_BASE + 0x65EC) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_190_REG              (xxx_GICD_REGS_BASE + 0x65F4) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_191_REG              (xxx_GICD_REGS_BASE + 0x65FC) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_192_REG              (xxx_GICD_REGS_BASE + 0x6604) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_193_REG              (xxx_GICD_REGS_BASE + 0x660C) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_194_REG              (xxx_GICD_REGS_BASE + 0x6614) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_195_REG              (xxx_GICD_REGS_BASE + 0x661C) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_196_REG              (xxx_GICD_REGS_BASE + 0x6624) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_197_REG              (xxx_GICD_REGS_BASE + 0x662C) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_198_REG              (xxx_GICD_REGS_BASE + 0x6634) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_199_REG              (xxx_GICD_REGS_BASE + 0x663C) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_200_REG              (xxx_GICD_REGS_BASE + 0x6644) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_201_REG              (xxx_GICD_REGS_BASE + 0x664C) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_202_REG              (xxx_GICD_REGS_BASE + 0x6654) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_203_REG              (xxx_GICD_REGS_BASE + 0x665C) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_204_REG              (xxx_GICD_REGS_BASE + 0x6664) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_205_REG              (xxx_GICD_REGS_BASE + 0x666C) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_206_REG              (xxx_GICD_REGS_BASE + 0x6674) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_207_REG              (xxx_GICD_REGS_BASE + 0x667C) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_208_REG              (xxx_GICD_REGS_BASE + 0x6684) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_209_REG              (xxx_GICD_REGS_BASE + 0x668C) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_210_REG              (xxx_GICD_REGS_BASE + 0x6694) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_211_REG              (xxx_GICD_REGS_BASE + 0x669C) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_212_REG              (xxx_GICD_REGS_BASE + 0x66A4) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_213_REG              (xxx_GICD_REGS_BASE + 0x66AC) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_214_REG              (xxx_GICD_REGS_BASE + 0x66B4) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_215_REG              (xxx_GICD_REGS_BASE + 0x66BC) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_216_REG              (xxx_GICD_REGS_BASE + 0x66C4) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_217_REG              (xxx_GICD_REGS_BASE + 0x66CC) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_218_REG              (xxx_GICD_REGS_BASE + 0x66D4) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_219_REG              (xxx_GICD_REGS_BASE + 0x66DC) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_220_REG              (xxx_GICD_REGS_BASE + 0x66E4) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_221_REG              (xxx_GICD_REGS_BASE + 0x66EC) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_222_REG              (xxx_GICD_REGS_BASE + 0x66F4) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_223_REG              (xxx_GICD_REGS_BASE + 0x66FC) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_224_REG              (xxx_GICD_REGS_BASE + 0x6704) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_225_REG              (xxx_GICD_REGS_BASE + 0x670C) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_226_REG              (xxx_GICD_REGS_BASE + 0x6714) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_227_REG              (xxx_GICD_REGS_BASE + 0x671C) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_228_REG              (xxx_GICD_REGS_BASE + 0x6724) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_229_REG              (xxx_GICD_REGS_BASE + 0x672C) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_230_REG              (xxx_GICD_REGS_BASE + 0x6734) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_231_REG              (xxx_GICD_REGS_BASE + 0x673C) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_232_REG              (xxx_GICD_REGS_BASE + 0x6744) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_233_REG              (xxx_GICD_REGS_BASE + 0x674C) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_234_REG              (xxx_GICD_REGS_BASE + 0x6754) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_235_REG              (xxx_GICD_REGS_BASE + 0x675C) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_236_REG              (xxx_GICD_REGS_BASE + 0x6764) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_237_REG              (xxx_GICD_REGS_BASE + 0x676C) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_238_REG              (xxx_GICD_REGS_BASE + 0x6774) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_239_REG              (xxx_GICD_REGS_BASE + 0x677C) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_240_REG              (xxx_GICD_REGS_BASE + 0x6784) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_241_REG              (xxx_GICD_REGS_BASE + 0x678C) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_242_REG              (xxx_GICD_REGS_BASE + 0x6794) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_243_REG              (xxx_GICD_REGS_BASE + 0x679C) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_244_REG              (xxx_GICD_REGS_BASE + 0x67A4) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_245_REG              (xxx_GICD_REGS_BASE + 0x67AC) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_246_REG              (xxx_GICD_REGS_BASE + 0x67B4) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_247_REG              (xxx_GICD_REGS_BASE + 0x67BC) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_248_REG              (xxx_GICD_REGS_BASE + 0x67C4) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_249_REG              (xxx_GICD_REGS_BASE + 0x67CC) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_250_REG              (xxx_GICD_REGS_BASE + 0x67D4) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_251_REG              (xxx_GICD_REGS_BASE + 0x67DC) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_252_REG              (xxx_GICD_REGS_BASE + 0x67E4) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_253_REG              (xxx_GICD_REGS_BASE + 0x67EC) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_254_REG              (xxx_GICD_REGS_BASE + 0x67F4) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_255_REG              (xxx_GICD_REGS_BASE + 0x67FC) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_256_REG              (xxx_GICD_REGS_BASE + 0x6804) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_257_REG              (xxx_GICD_REGS_BASE + 0x680C) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_258_REG              (xxx_GICD_REGS_BASE + 0x6814) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_259_REG              (xxx_GICD_REGS_BASE + 0x681C) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_260_REG              (xxx_GICD_REGS_BASE + 0x6824) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_261_REG              (xxx_GICD_REGS_BASE + 0x682C) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_262_REG              (xxx_GICD_REGS_BASE + 0x6834) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_263_REG              (xxx_GICD_REGS_BASE + 0x683C) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_264_REG              (xxx_GICD_REGS_BASE + 0x6844) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_265_REG              (xxx_GICD_REGS_BASE + 0x684C) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_266_REG              (xxx_GICD_REGS_BASE + 0x6854) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_267_REG              (xxx_GICD_REGS_BASE + 0x685C) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_268_REG              (xxx_GICD_REGS_BASE + 0x6864) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_269_REG              (xxx_GICD_REGS_BASE + 0x686C) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_270_REG              (xxx_GICD_REGS_BASE + 0x6874) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_271_REG              (xxx_GICD_REGS_BASE + 0x687C) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_272_REG              (xxx_GICD_REGS_BASE + 0x6884) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_273_REG              (xxx_GICD_REGS_BASE + 0x688C) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_274_REG              (xxx_GICD_REGS_BASE + 0x6894) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_275_REG              (xxx_GICD_REGS_BASE + 0x689C) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_276_REG              (xxx_GICD_REGS_BASE + 0x68A4) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_277_REG              (xxx_GICD_REGS_BASE + 0x68AC) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_278_REG              (xxx_GICD_REGS_BASE + 0x68B4) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_279_REG              (xxx_GICD_REGS_BASE + 0x68BC) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_280_REG              (xxx_GICD_REGS_BASE + 0x68C4) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_281_REG              (xxx_GICD_REGS_BASE + 0x68CC) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_282_REG              (xxx_GICD_REGS_BASE + 0x68D4) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_283_REG              (xxx_GICD_REGS_BASE + 0x68DC) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_284_REG              (xxx_GICD_REGS_BASE + 0x68E4) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_285_REG              (xxx_GICD_REGS_BASE + 0x68EC) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_286_REG              (xxx_GICD_REGS_BASE + 0x68F4) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_287_REG              (xxx_GICD_REGS_BASE + 0x68FC) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_288_REG              (xxx_GICD_REGS_BASE + 0x6904) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_289_REG              (xxx_GICD_REGS_BASE + 0x690C) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_290_REG              (xxx_GICD_REGS_BASE + 0x6914) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_291_REG              (xxx_GICD_REGS_BASE + 0x691C) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_292_REG              (xxx_GICD_REGS_BASE + 0x6924) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_293_REG              (xxx_GICD_REGS_BASE + 0x692C) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_294_REG              (xxx_GICD_REGS_BASE + 0x6934) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_295_REG              (xxx_GICD_REGS_BASE + 0x693C) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_296_REG              (xxx_GICD_REGS_BASE + 0x6944) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_297_REG              (xxx_GICD_REGS_BASE + 0x694C) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_298_REG              (xxx_GICD_REGS_BASE + 0x6954) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_299_REG              (xxx_GICD_REGS_BASE + 0x695C) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_300_REG              (xxx_GICD_REGS_BASE + 0x6964) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_301_REG              (xxx_GICD_REGS_BASE + 0x696C) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_302_REG              (xxx_GICD_REGS_BASE + 0x6974) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_303_REG              (xxx_GICD_REGS_BASE + 0x697C) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_304_REG              (xxx_GICD_REGS_BASE + 0x6984) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_305_REG              (xxx_GICD_REGS_BASE + 0x698C) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_306_REG              (xxx_GICD_REGS_BASE + 0x6994) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_307_REG              (xxx_GICD_REGS_BASE + 0x699C) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_308_REG              (xxx_GICD_REGS_BASE + 0x69A4) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_309_REG              (xxx_GICD_REGS_BASE + 0x69AC) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_310_REG              (xxx_GICD_REGS_BASE + 0x69B4) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_311_REG              (xxx_GICD_REGS_BASE + 0x69BC) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_312_REG              (xxx_GICD_REGS_BASE + 0x69C4) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_313_REG              (xxx_GICD_REGS_BASE + 0x69CC) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_314_REG              (xxx_GICD_REGS_BASE + 0x69D4) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_315_REG              (xxx_GICD_REGS_BASE + 0x69DC) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_316_REG              (xxx_GICD_REGS_BASE + 0x69E4) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_317_REG              (xxx_GICD_REGS_BASE + 0x69EC) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_318_REG              (xxx_GICD_REGS_BASE + 0x69F4) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_319_REG              (xxx_GICD_REGS_BASE + 0x69FC) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_320_REG              (xxx_GICD_REGS_BASE + 0x6A04) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_321_REG              (xxx_GICD_REGS_BASE + 0x6A0C) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_322_REG              (xxx_GICD_REGS_BASE + 0x6A14) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_323_REG              (xxx_GICD_REGS_BASE + 0x6A1C) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_324_REG              (xxx_GICD_REGS_BASE + 0x6A24) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_325_REG              (xxx_GICD_REGS_BASE + 0x6A2C) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_326_REG              (xxx_GICD_REGS_BASE + 0x6A34) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_327_REG              (xxx_GICD_REGS_BASE + 0x6A3C) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_328_REG              (xxx_GICD_REGS_BASE + 0x6A44) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_329_REG              (xxx_GICD_REGS_BASE + 0x6A4C) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_330_REG              (xxx_GICD_REGS_BASE + 0x6A54) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_331_REG              (xxx_GICD_REGS_BASE + 0x6A5C) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_332_REG              (xxx_GICD_REGS_BASE + 0x6A64) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_333_REG              (xxx_GICD_REGS_BASE + 0x6A6C) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_334_REG              (xxx_GICD_REGS_BASE + 0x6A74) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_335_REG              (xxx_GICD_REGS_BASE + 0x6A7C) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_336_REG              (xxx_GICD_REGS_BASE + 0x6A84) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_337_REG              (xxx_GICD_REGS_BASE + 0x6A8C) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_338_REG              (xxx_GICD_REGS_BASE + 0x6A94) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_339_REG              (xxx_GICD_REGS_BASE + 0x6A9C) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_340_REG              (xxx_GICD_REGS_BASE + 0x6AA4) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_341_REG              (xxx_GICD_REGS_BASE + 0x6AAC) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_342_REG              (xxx_GICD_REGS_BASE + 0x6AB4) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_343_REG              (xxx_GICD_REGS_BASE + 0x6ABC) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_344_REG              (xxx_GICD_REGS_BASE + 0x6AC4) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_345_REG              (xxx_GICD_REGS_BASE + 0x6ACC) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_346_REG              (xxx_GICD_REGS_BASE + 0x6AD4) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_347_REG              (xxx_GICD_REGS_BASE + 0x6ADC) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_348_REG              (xxx_GICD_REGS_BASE + 0x6AE4) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_349_REG              (xxx_GICD_REGS_BASE + 0x6AEC) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_350_REG              (xxx_GICD_REGS_BASE + 0x6AF4) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_351_REG              (xxx_GICD_REGS_BASE + 0x6AFC) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_352_REG              (xxx_GICD_REGS_BASE + 0x6B04) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_353_REG              (xxx_GICD_REGS_BASE + 0x6B0C) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_354_REG              (xxx_GICD_REGS_BASE + 0x6B14) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_355_REG              (xxx_GICD_REGS_BASE + 0x6B1C) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_356_REG              (xxx_GICD_REGS_BASE + 0x6B24) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_357_REG              (xxx_GICD_REGS_BASE + 0x6B2C) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_358_REG              (xxx_GICD_REGS_BASE + 0x6B34) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_359_REG              (xxx_GICD_REGS_BASE + 0x6B3C) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_360_REG              (xxx_GICD_REGS_BASE + 0x6B44) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_361_REG              (xxx_GICD_REGS_BASE + 0x6B4C) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_362_REG              (xxx_GICD_REGS_BASE + 0x6B54) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_363_REG              (xxx_GICD_REGS_BASE + 0x6B5C) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_364_REG              (xxx_GICD_REGS_BASE + 0x6B64) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_365_REG              (xxx_GICD_REGS_BASE + 0x6B6C) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_366_REG              (xxx_GICD_REGS_BASE + 0x6B74) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_367_REG              (xxx_GICD_REGS_BASE + 0x6B7C) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_368_REG              (xxx_GICD_REGS_BASE + 0x6B84) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_369_REG              (xxx_GICD_REGS_BASE + 0x6B8C) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_370_REG              (xxx_GICD_REGS_BASE + 0x6B94) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_371_REG              (xxx_GICD_REGS_BASE + 0x6B9C) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_372_REG              (xxx_GICD_REGS_BASE + 0x6BA4) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_373_REG              (xxx_GICD_REGS_BASE + 0x6BAC) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_374_REG              (xxx_GICD_REGS_BASE + 0x6BB4) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_375_REG              (xxx_GICD_REGS_BASE + 0x6BBC) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_376_REG              (xxx_GICD_REGS_BASE + 0x6BC4) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_377_REG              (xxx_GICD_REGS_BASE + 0x6BCC) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_378_REG              (xxx_GICD_REGS_BASE + 0x6BD4) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_379_REG              (xxx_GICD_REGS_BASE + 0x6BDC) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_380_REG              (xxx_GICD_REGS_BASE + 0x6BE4) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_381_REG              (xxx_GICD_REGS_BASE + 0x6BEC) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_382_REG              (xxx_GICD_REGS_BASE + 0x6BF4) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_383_REG              (xxx_GICD_REGS_BASE + 0x6BFC) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_384_REG              (xxx_GICD_REGS_BASE + 0x6C04) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_385_REG              (xxx_GICD_REGS_BASE + 0x6C0C) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_386_REG              (xxx_GICD_REGS_BASE + 0x6C14) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_387_REG              (xxx_GICD_REGS_BASE + 0x6C1C) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_388_REG              (xxx_GICD_REGS_BASE + 0x6C24) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_389_REG              (xxx_GICD_REGS_BASE + 0x6C2C) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_390_REG              (xxx_GICD_REGS_BASE + 0x6C34) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_391_REG              (xxx_GICD_REGS_BASE + 0x6C3C) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_392_REG              (xxx_GICD_REGS_BASE + 0x6C44) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_393_REG              (xxx_GICD_REGS_BASE + 0x6C4C) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_394_REG              (xxx_GICD_REGS_BASE + 0x6C54) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_395_REG              (xxx_GICD_REGS_BASE + 0x6C5C) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_396_REG              (xxx_GICD_REGS_BASE + 0x6C64) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_397_REG              (xxx_GICD_REGS_BASE + 0x6C6C) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_398_REG              (xxx_GICD_REGS_BASE + 0x6C74) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_399_REG              (xxx_GICD_REGS_BASE + 0x6C7C) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_400_REG              (xxx_GICD_REGS_BASE + 0x6C84) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_401_REG              (xxx_GICD_REGS_BASE + 0x6C8C) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_402_REG              (xxx_GICD_REGS_BASE + 0x6C94) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_403_REG              (xxx_GICD_REGS_BASE + 0x6C9C) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_404_REG              (xxx_GICD_REGS_BASE + 0x6CA4) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_405_REG              (xxx_GICD_REGS_BASE + 0x6CAC) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_406_REG              (xxx_GICD_REGS_BASE + 0x6CB4) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_407_REG              (xxx_GICD_REGS_BASE + 0x6CBC) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_408_REG              (xxx_GICD_REGS_BASE + 0x6CC4) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_409_REG              (xxx_GICD_REGS_BASE + 0x6CCC) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_410_REG              (xxx_GICD_REGS_BASE + 0x6CD4) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_411_REG              (xxx_GICD_REGS_BASE + 0x6CDC) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_412_REG              (xxx_GICD_REGS_BASE + 0x6CE4) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_413_REG              (xxx_GICD_REGS_BASE + 0x6CEC) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_414_REG              (xxx_GICD_REGS_BASE + 0x6CF4) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_415_REG              (xxx_GICD_REGS_BASE + 0x6CFC) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_416_REG              (xxx_GICD_REGS_BASE + 0x6D04) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_417_REG              (xxx_GICD_REGS_BASE + 0x6D0C) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_418_REG              (xxx_GICD_REGS_BASE + 0x6D14) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_419_REG              (xxx_GICD_REGS_BASE + 0x6D1C) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_420_REG              (xxx_GICD_REGS_BASE + 0x6D24) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_421_REG              (xxx_GICD_REGS_BASE + 0x6D2C) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_422_REG              (xxx_GICD_REGS_BASE + 0x6D34) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_423_REG              (xxx_GICD_REGS_BASE + 0x6D3C) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_424_REG              (xxx_GICD_REGS_BASE + 0x6D44) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_425_REG              (xxx_GICD_REGS_BASE + 0x6D4C) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_426_REG              (xxx_GICD_REGS_BASE + 0x6D54) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_427_REG              (xxx_GICD_REGS_BASE + 0x6D5C) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_428_REG              (xxx_GICD_REGS_BASE + 0x6D64) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_429_REG              (xxx_GICD_REGS_BASE + 0x6D6C) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_430_REG              (xxx_GICD_REGS_BASE + 0x6D74) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_431_REG              (xxx_GICD_REGS_BASE + 0x6D7C) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_432_REG              (xxx_GICD_REGS_BASE + 0x6D84) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_433_REG              (xxx_GICD_REGS_BASE + 0x6D8C) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_434_REG              (xxx_GICD_REGS_BASE + 0x6D94) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_435_REG              (xxx_GICD_REGS_BASE + 0x6D9C) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_436_REG              (xxx_GICD_REGS_BASE + 0x6DA4) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_437_REG              (xxx_GICD_REGS_BASE + 0x6DAC) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_438_REG              (xxx_GICD_REGS_BASE + 0x6DB4) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_439_REG              (xxx_GICD_REGS_BASE + 0x6DBC) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_440_REG              (xxx_GICD_REGS_BASE + 0x6DC4) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_441_REG              (xxx_GICD_REGS_BASE + 0x6DCC) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_442_REG              (xxx_GICD_REGS_BASE + 0x6DD4) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_443_REG              (xxx_GICD_REGS_BASE + 0x6DDC) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_444_REG              (xxx_GICD_REGS_BASE + 0x6DE4) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_445_REG              (xxx_GICD_REGS_BASE + 0x6DEC) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_446_REG              (xxx_GICD_REGS_BASE + 0x6DF4) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_447_REG              (xxx_GICD_REGS_BASE + 0x6DFC) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_448_REG              (xxx_GICD_REGS_BASE + 0x6E04) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_449_REG              (xxx_GICD_REGS_BASE + 0x6E0C) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_450_REG              (xxx_GICD_REGS_BASE + 0x6E14) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_451_REG              (xxx_GICD_REGS_BASE + 0x6E1C) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_452_REG              (xxx_GICD_REGS_BASE + 0x6E24) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_453_REG              (xxx_GICD_REGS_BASE + 0x6E2C) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_454_REG              (xxx_GICD_REGS_BASE + 0x6E34) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_455_REG              (xxx_GICD_REGS_BASE + 0x6E3C) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_456_REG              (xxx_GICD_REGS_BASE + 0x6E44) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_457_REG              (xxx_GICD_REGS_BASE + 0x6E4C) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_458_REG              (xxx_GICD_REGS_BASE + 0x6E54) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_459_REG              (xxx_GICD_REGS_BASE + 0x6E5C) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_460_REG              (xxx_GICD_REGS_BASE + 0x6E64) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_461_REG              (xxx_GICD_REGS_BASE + 0x6E6C) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_462_REG              (xxx_GICD_REGS_BASE + 0x6E74) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_463_REG              (xxx_GICD_REGS_BASE + 0x6E7C) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_464_REG              (xxx_GICD_REGS_BASE + 0x6E84) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_465_REG              (xxx_GICD_REGS_BASE + 0x6E8C) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_466_REG              (xxx_GICD_REGS_BASE + 0x6E94) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_467_REG              (xxx_GICD_REGS_BASE + 0x6E9C) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_468_REG              (xxx_GICD_REGS_BASE + 0x6EA4) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_469_REG              (xxx_GICD_REGS_BASE + 0x6EAC) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_470_REG              (xxx_GICD_REGS_BASE + 0x6EB4) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_471_REG              (xxx_GICD_REGS_BASE + 0x6EBC) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_472_REG              (xxx_GICD_REGS_BASE + 0x6EC4) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_473_REG              (xxx_GICD_REGS_BASE + 0x6ECC) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_474_REG              (xxx_GICD_REGS_BASE + 0x6ED4) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_475_REG              (xxx_GICD_REGS_BASE + 0x6EDC) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_476_REG              (xxx_GICD_REGS_BASE + 0x6EE4) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_477_REG              (xxx_GICD_REGS_BASE + 0x6EEC) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_478_REG              (xxx_GICD_REGS_BASE + 0x6EF4) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_479_REG              (xxx_GICD_REGS_BASE + 0x6EFC) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_480_REG              (xxx_GICD_REGS_BASE + 0x6F04) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_481_REG              (xxx_GICD_REGS_BASE + 0x6F0C) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_482_REG              (xxx_GICD_REGS_BASE + 0x6F14) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_483_REG              (xxx_GICD_REGS_BASE + 0x6F1C) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_484_REG              (xxx_GICD_REGS_BASE + 0x6F24) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_485_REG              (xxx_GICD_REGS_BASE + 0x6F2C) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_486_REG              (xxx_GICD_REGS_BASE + 0x6F34) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_487_REG              (xxx_GICD_REGS_BASE + 0x6F3C) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_488_REG              (xxx_GICD_REGS_BASE + 0x6F44) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_489_REG              (xxx_GICD_REGS_BASE + 0x6F4C) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_490_REG              (xxx_GICD_REGS_BASE + 0x6F54) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_491_REG              (xxx_GICD_REGS_BASE + 0x6F5C) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_492_REG              (xxx_GICD_REGS_BASE + 0x6F64) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_493_REG              (xxx_GICD_REGS_BASE + 0x6F6C) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_494_REG              (xxx_GICD_REGS_BASE + 0x6F74) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_495_REG              (xxx_GICD_REGS_BASE + 0x6F7C) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_496_REG              (xxx_GICD_REGS_BASE + 0x6F84) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_497_REG              (xxx_GICD_REGS_BASE + 0x6F8C) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_498_REG              (xxx_GICD_REGS_BASE + 0x6F94) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_499_REG              (xxx_GICD_REGS_BASE + 0x6F9C) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_500_REG              (xxx_GICD_REGS_BASE + 0x6FA4) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_501_REG              (xxx_GICD_REGS_BASE + 0x6FAC) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_502_REG              (xxx_GICD_REGS_BASE + 0x6FB4) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_503_REG              (xxx_GICD_REGS_BASE + 0x6FBC) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_504_REG              (xxx_GICD_REGS_BASE + 0x6FC4) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_505_REG              (xxx_GICD_REGS_BASE + 0x6FCC) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_506_REG              (xxx_GICD_REGS_BASE + 0x6FD4) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_507_REG              (xxx_GICD_REGS_BASE + 0x6FDC) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_508_REG              (xxx_GICD_REGS_BASE + 0x6FE4) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_509_REG              (xxx_GICD_REGS_BASE + 0x6FEC) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_510_REG              (xxx_GICD_REGS_BASE + 0x6FF4) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_511_REG              (xxx_GICD_REGS_BASE + 0x6FFC) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_512_REG              (xxx_GICD_REGS_BASE + 0x7004) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_513_REG              (xxx_GICD_REGS_BASE + 0x700C) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_514_REG              (xxx_GICD_REGS_BASE + 0x7014) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_515_REG              (xxx_GICD_REGS_BASE + 0x701C) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_516_REG              (xxx_GICD_REGS_BASE + 0x7024) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_517_REG              (xxx_GICD_REGS_BASE + 0x702C) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_518_REG              (xxx_GICD_REGS_BASE + 0x7034) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_519_REG              (xxx_GICD_REGS_BASE + 0x703C) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_520_REG              (xxx_GICD_REGS_BASE + 0x7044) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_521_REG              (xxx_GICD_REGS_BASE + 0x704C) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_522_REG              (xxx_GICD_REGS_BASE + 0x7054) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_523_REG              (xxx_GICD_REGS_BASE + 0x705C) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_524_REG              (xxx_GICD_REGS_BASE + 0x7064) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_525_REG              (xxx_GICD_REGS_BASE + 0x706C) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_526_REG              (xxx_GICD_REGS_BASE + 0x7074) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_527_REG              (xxx_GICD_REGS_BASE + 0x707C) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_528_REG              (xxx_GICD_REGS_BASE + 0x7084) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_529_REG              (xxx_GICD_REGS_BASE + 0x708C) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_530_REG              (xxx_GICD_REGS_BASE + 0x7094) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_531_REG              (xxx_GICD_REGS_BASE + 0x709C) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_532_REG              (xxx_GICD_REGS_BASE + 0x70A4) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_533_REG              (xxx_GICD_REGS_BASE + 0x70AC) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_534_REG              (xxx_GICD_REGS_BASE + 0x70B4) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_535_REG              (xxx_GICD_REGS_BASE + 0x70BC) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_536_REG              (xxx_GICD_REGS_BASE + 0x70C4) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_537_REG              (xxx_GICD_REGS_BASE + 0x70CC) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_538_REG              (xxx_GICD_REGS_BASE + 0x70D4) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_539_REG              (xxx_GICD_REGS_BASE + 0x70DC) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_540_REG              (xxx_GICD_REGS_BASE + 0x70E4) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_541_REG              (xxx_GICD_REGS_BASE + 0x70EC) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_542_REG              (xxx_GICD_REGS_BASE + 0x70F4) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_543_REG              (xxx_GICD_REGS_BASE + 0x70FC) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_544_REG              (xxx_GICD_REGS_BASE + 0x7104) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_545_REG              (xxx_GICD_REGS_BASE + 0x710C) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_546_REG              (xxx_GICD_REGS_BASE + 0x7114) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_547_REG              (xxx_GICD_REGS_BASE + 0x711C) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_548_REG              (xxx_GICD_REGS_BASE + 0x7124) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_549_REG              (xxx_GICD_REGS_BASE + 0x712C) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_550_REG              (xxx_GICD_REGS_BASE + 0x7134) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_551_REG              (xxx_GICD_REGS_BASE + 0x713C) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_552_REG              (xxx_GICD_REGS_BASE + 0x7144) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_553_REG              (xxx_GICD_REGS_BASE + 0x714C) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_554_REG              (xxx_GICD_REGS_BASE + 0x7154) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_555_REG              (xxx_GICD_REGS_BASE + 0x715C) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_556_REG              (xxx_GICD_REGS_BASE + 0x7164) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_557_REG              (xxx_GICD_REGS_BASE + 0x716C) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_558_REG              (xxx_GICD_REGS_BASE + 0x7174) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_559_REG              (xxx_GICD_REGS_BASE + 0x717C) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_560_REG              (xxx_GICD_REGS_BASE + 0x7184) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_561_REG              (xxx_GICD_REGS_BASE + 0x718C) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_562_REG              (xxx_GICD_REGS_BASE + 0x7194) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_563_REG              (xxx_GICD_REGS_BASE + 0x719C) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_564_REG              (xxx_GICD_REGS_BASE + 0x71A4) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_565_REG              (xxx_GICD_REGS_BASE + 0x71AC) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_566_REG              (xxx_GICD_REGS_BASE + 0x71B4) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_567_REG              (xxx_GICD_REGS_BASE + 0x71BC) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_568_REG              (xxx_GICD_REGS_BASE + 0x71C4) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_569_REG              (xxx_GICD_REGS_BASE + 0x71CC) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_570_REG              (xxx_GICD_REGS_BASE + 0x71D4) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_571_REG              (xxx_GICD_REGS_BASE + 0x71DC) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_572_REG              (xxx_GICD_REGS_BASE + 0x71E4) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_573_REG              (xxx_GICD_REGS_BASE + 0x71EC) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_574_REG              (xxx_GICD_REGS_BASE + 0x71F4) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_575_REG              (xxx_GICD_REGS_BASE + 0x71FC) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_576_REG              (xxx_GICD_REGS_BASE + 0x7204) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_577_REG              (xxx_GICD_REGS_BASE + 0x720C) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_578_REG              (xxx_GICD_REGS_BASE + 0x7214) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_579_REG              (xxx_GICD_REGS_BASE + 0x721C) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_580_REG              (xxx_GICD_REGS_BASE + 0x7224) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_581_REG              (xxx_GICD_REGS_BASE + 0x722C) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_582_REG              (xxx_GICD_REGS_BASE + 0x7234) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_583_REG              (xxx_GICD_REGS_BASE + 0x723C) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_584_REG              (xxx_GICD_REGS_BASE + 0x7244) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_585_REG              (xxx_GICD_REGS_BASE + 0x724C) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_586_REG              (xxx_GICD_REGS_BASE + 0x7254) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_587_REG              (xxx_GICD_REGS_BASE + 0x725C) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_588_REG              (xxx_GICD_REGS_BASE + 0x7264) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_589_REG              (xxx_GICD_REGS_BASE + 0x726C) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_590_REG              (xxx_GICD_REGS_BASE + 0x7274) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_591_REG              (xxx_GICD_REGS_BASE + 0x727C) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_592_REG              (xxx_GICD_REGS_BASE + 0x7284) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_593_REG              (xxx_GICD_REGS_BASE + 0x728C) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_594_REG              (xxx_GICD_REGS_BASE + 0x7294) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_595_REG              (xxx_GICD_REGS_BASE + 0x729C) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_596_REG              (xxx_GICD_REGS_BASE + 0x72A4) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_597_REG              (xxx_GICD_REGS_BASE + 0x72AC) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_598_REG              (xxx_GICD_REGS_BASE + 0x72B4) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_599_REG              (xxx_GICD_REGS_BASE + 0x72BC) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_600_REG              (xxx_GICD_REGS_BASE + 0x72C4) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_601_REG              (xxx_GICD_REGS_BASE + 0x72CC) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_602_REG              (xxx_GICD_REGS_BASE + 0x72D4) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_603_REG              (xxx_GICD_REGS_BASE + 0x72DC) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_604_REG              (xxx_GICD_REGS_BASE + 0x72E4) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_605_REG              (xxx_GICD_REGS_BASE + 0x72EC) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_606_REG              (xxx_GICD_REGS_BASE + 0x72F4) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_607_REG              (xxx_GICD_REGS_BASE + 0x72FC) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_608_REG              (xxx_GICD_REGS_BASE + 0x7304) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_609_REG              (xxx_GICD_REGS_BASE + 0x730C) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_610_REG              (xxx_GICD_REGS_BASE + 0x7314) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_611_REG              (xxx_GICD_REGS_BASE + 0x731C) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_612_REG              (xxx_GICD_REGS_BASE + 0x7324) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_613_REG              (xxx_GICD_REGS_BASE + 0x732C) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_614_REG              (xxx_GICD_REGS_BASE + 0x7334) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_615_REG              (xxx_GICD_REGS_BASE + 0x733C) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_616_REG              (xxx_GICD_REGS_BASE + 0x7344) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_617_REG              (xxx_GICD_REGS_BASE + 0x734C) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_618_REG              (xxx_GICD_REGS_BASE + 0x7354) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_619_REG              (xxx_GICD_REGS_BASE + 0x735C) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_620_REG              (xxx_GICD_REGS_BASE + 0x7364) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_621_REG              (xxx_GICD_REGS_BASE + 0x736C) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_622_REG              (xxx_GICD_REGS_BASE + 0x7374) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_623_REG              (xxx_GICD_REGS_BASE + 0x737C) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_624_REG              (xxx_GICD_REGS_BASE + 0x7384) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_625_REG              (xxx_GICD_REGS_BASE + 0x738C) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_626_REG              (xxx_GICD_REGS_BASE + 0x7394) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_627_REG              (xxx_GICD_REGS_BASE + 0x739C) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_628_REG              (xxx_GICD_REGS_BASE + 0x73A4) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_629_REG              (xxx_GICD_REGS_BASE + 0x73AC) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_630_REG              (xxx_GICD_REGS_BASE + 0x73B4) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_631_REG              (xxx_GICD_REGS_BASE + 0x73BC) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_632_REG              (xxx_GICD_REGS_BASE + 0x73C4) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_633_REG              (xxx_GICD_REGS_BASE + 0x73CC) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_634_REG              (xxx_GICD_REGS_BASE + 0x73D4) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_635_REG              (xxx_GICD_REGS_BASE + 0x73DC) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_636_REG              (xxx_GICD_REGS_BASE + 0x73E4) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_637_REG              (xxx_GICD_REGS_BASE + 0x73EC) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_638_REG              (xxx_GICD_REGS_BASE + 0x73F4) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_639_REG              (xxx_GICD_REGS_BASE + 0x73FC) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_640_REG              (xxx_GICD_REGS_BASE + 0x7404) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_641_REG              (xxx_GICD_REGS_BASE + 0x740C) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_642_REG              (xxx_GICD_REGS_BASE + 0x7414) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_643_REG              (xxx_GICD_REGS_BASE + 0x741C) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_644_REG              (xxx_GICD_REGS_BASE + 0x7424) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_645_REG              (xxx_GICD_REGS_BASE + 0x742C) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_646_REG              (xxx_GICD_REGS_BASE + 0x7434) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_647_REG              (xxx_GICD_REGS_BASE + 0x743C) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_648_REG              (xxx_GICD_REGS_BASE + 0x7444) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_649_REG              (xxx_GICD_REGS_BASE + 0x744C) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_650_REG              (xxx_GICD_REGS_BASE + 0x7454) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_651_REG              (xxx_GICD_REGS_BASE + 0x745C) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_652_REG              (xxx_GICD_REGS_BASE + 0x7464) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_653_REG              (xxx_GICD_REGS_BASE + 0x746C) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_654_REG              (xxx_GICD_REGS_BASE + 0x7474) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_655_REG              (xxx_GICD_REGS_BASE + 0x747C) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_656_REG              (xxx_GICD_REGS_BASE + 0x7484) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_657_REG              (xxx_GICD_REGS_BASE + 0x748C) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_658_REG              (xxx_GICD_REGS_BASE + 0x7494) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_659_REG              (xxx_GICD_REGS_BASE + 0x749C) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_660_REG              (xxx_GICD_REGS_BASE + 0x74A4) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_661_REG              (xxx_GICD_REGS_BASE + 0x74AC) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_662_REG              (xxx_GICD_REGS_BASE + 0x74B4) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_663_REG              (xxx_GICD_REGS_BASE + 0x74BC) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_664_REG              (xxx_GICD_REGS_BASE + 0x74C4) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_665_REG              (xxx_GICD_REGS_BASE + 0x74CC) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_666_REG              (xxx_GICD_REGS_BASE + 0x74D4) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_667_REG              (xxx_GICD_REGS_BASE + 0x74DC) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_668_REG              (xxx_GICD_REGS_BASE + 0x74E4) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_669_REG              (xxx_GICD_REGS_BASE + 0x74EC) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_670_REG              (xxx_GICD_REGS_BASE + 0x74F4) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_671_REG              (xxx_GICD_REGS_BASE + 0x74FC) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_672_REG              (xxx_GICD_REGS_BASE + 0x7504) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_673_REG              (xxx_GICD_REGS_BASE + 0x750C) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_674_REG              (xxx_GICD_REGS_BASE + 0x7514) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_675_REG              (xxx_GICD_REGS_BASE + 0x751C) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_676_REG              (xxx_GICD_REGS_BASE + 0x7524) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_677_REG              (xxx_GICD_REGS_BASE + 0x752C) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_678_REG              (xxx_GICD_REGS_BASE + 0x7534) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_679_REG              (xxx_GICD_REGS_BASE + 0x753C) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_680_REG              (xxx_GICD_REGS_BASE + 0x7544) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_681_REG              (xxx_GICD_REGS_BASE + 0x754C) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_682_REG              (xxx_GICD_REGS_BASE + 0x7554) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_683_REG              (xxx_GICD_REGS_BASE + 0x755C) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_684_REG              (xxx_GICD_REGS_BASE + 0x7564) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_685_REG              (xxx_GICD_REGS_BASE + 0x756C) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_686_REG              (xxx_GICD_REGS_BASE + 0x7574) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_687_REG              (xxx_GICD_REGS_BASE + 0x757C) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_688_REG              (xxx_GICD_REGS_BASE + 0x7584) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_689_REG              (xxx_GICD_REGS_BASE + 0x758C) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_690_REG              (xxx_GICD_REGS_BASE + 0x7594) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_691_REG              (xxx_GICD_REGS_BASE + 0x759C) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_692_REG              (xxx_GICD_REGS_BASE + 0x75A4) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_693_REG              (xxx_GICD_REGS_BASE + 0x75AC) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_694_REG              (xxx_GICD_REGS_BASE + 0x75B4) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_695_REG              (xxx_GICD_REGS_BASE + 0x75BC) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_696_REG              (xxx_GICD_REGS_BASE + 0x75C4) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_697_REG              (xxx_GICD_REGS_BASE + 0x75CC) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_698_REG              (xxx_GICD_REGS_BASE + 0x75D4) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_699_REG              (xxx_GICD_REGS_BASE + 0x75DC) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_700_REG              (xxx_GICD_REGS_BASE + 0x75E4) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_701_REG              (xxx_GICD_REGS_BASE + 0x75EC) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_702_REG              (xxx_GICD_REGS_BASE + 0x75F4) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_703_REG              (xxx_GICD_REGS_BASE + 0x75FC) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_704_REG              (xxx_GICD_REGS_BASE + 0x7604) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_705_REG              (xxx_GICD_REGS_BASE + 0x760C) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_706_REG              (xxx_GICD_REGS_BASE + 0x7614) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_707_REG              (xxx_GICD_REGS_BASE + 0x761C) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_708_REG              (xxx_GICD_REGS_BASE + 0x7624) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_709_REG              (xxx_GICD_REGS_BASE + 0x762C) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_710_REG              (xxx_GICD_REGS_BASE + 0x7634) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_711_REG              (xxx_GICD_REGS_BASE + 0x763C) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_712_REG              (xxx_GICD_REGS_BASE + 0x7644) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_713_REG              (xxx_GICD_REGS_BASE + 0x764C) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_714_REG              (xxx_GICD_REGS_BASE + 0x7654) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_715_REG              (xxx_GICD_REGS_BASE + 0x765C) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_716_REG              (xxx_GICD_REGS_BASE + 0x7664) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_717_REG              (xxx_GICD_REGS_BASE + 0x766C) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_718_REG              (xxx_GICD_REGS_BASE + 0x7674) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_719_REG              (xxx_GICD_REGS_BASE + 0x767C) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_720_REG              (xxx_GICD_REGS_BASE + 0x7684) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_721_REG              (xxx_GICD_REGS_BASE + 0x768C) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_722_REG              (xxx_GICD_REGS_BASE + 0x7694) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_723_REG              (xxx_GICD_REGS_BASE + 0x769C) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_724_REG              (xxx_GICD_REGS_BASE + 0x76A4) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_725_REG              (xxx_GICD_REGS_BASE + 0x76AC) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_726_REG              (xxx_GICD_REGS_BASE + 0x76B4) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_727_REG              (xxx_GICD_REGS_BASE + 0x76BC) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_728_REG              (xxx_GICD_REGS_BASE + 0x76C4) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_729_REG              (xxx_GICD_REGS_BASE + 0x76CC) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_730_REG              (xxx_GICD_REGS_BASE + 0x76D4) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_731_REG              (xxx_GICD_REGS_BASE + 0x76DC) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_732_REG              (xxx_GICD_REGS_BASE + 0x76E4) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_733_REG              (xxx_GICD_REGS_BASE + 0x76EC) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_734_REG              (xxx_GICD_REGS_BASE + 0x76F4) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_735_REG              (xxx_GICD_REGS_BASE + 0x76FC) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_736_REG              (xxx_GICD_REGS_BASE + 0x7704) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_737_REG              (xxx_GICD_REGS_BASE + 0x770C) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_738_REG              (xxx_GICD_REGS_BASE + 0x7714) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_739_REG              (xxx_GICD_REGS_BASE + 0x771C) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_740_REG              (xxx_GICD_REGS_BASE + 0x7724) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_741_REG              (xxx_GICD_REGS_BASE + 0x772C) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_742_REG              (xxx_GICD_REGS_BASE + 0x7734) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_743_REG              (xxx_GICD_REGS_BASE + 0x773C) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_744_REG              (xxx_GICD_REGS_BASE + 0x7744) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_745_REG              (xxx_GICD_REGS_BASE + 0x774C) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_746_REG              (xxx_GICD_REGS_BASE + 0x7754) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_747_REG              (xxx_GICD_REGS_BASE + 0x775C) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_748_REG              (xxx_GICD_REGS_BASE + 0x7764) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_749_REG              (xxx_GICD_REGS_BASE + 0x776C) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_750_REG              (xxx_GICD_REGS_BASE + 0x7774) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_751_REG              (xxx_GICD_REGS_BASE + 0x777C) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_752_REG              (xxx_GICD_REGS_BASE + 0x7784) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_753_REG              (xxx_GICD_REGS_BASE + 0x778C) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_754_REG              (xxx_GICD_REGS_BASE + 0x7794) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_755_REG              (xxx_GICD_REGS_BASE + 0x779C) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_756_REG              (xxx_GICD_REGS_BASE + 0x77A4) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_757_REG              (xxx_GICD_REGS_BASE + 0x77AC) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_758_REG              (xxx_GICD_REGS_BASE + 0x77B4) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_759_REG              (xxx_GICD_REGS_BASE + 0x77BC) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_760_REG              (xxx_GICD_REGS_BASE + 0x77C4) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_761_REG              (xxx_GICD_REGS_BASE + 0x77CC) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_762_REG              (xxx_GICD_REGS_BASE + 0x77D4) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_763_REG              (xxx_GICD_REGS_BASE + 0x77DC) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_764_REG              (xxx_GICD_REGS_BASE + 0x77E4) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_765_REG              (xxx_GICD_REGS_BASE + 0x77EC) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_766_REG              (xxx_GICD_REGS_BASE + 0x77F4) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_767_REG              (xxx_GICD_REGS_BASE + 0x77FC) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_768_REG              (xxx_GICD_REGS_BASE + 0x7804) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_769_REG              (xxx_GICD_REGS_BASE + 0x780C) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_770_REG              (xxx_GICD_REGS_BASE + 0x7814) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_771_REG              (xxx_GICD_REGS_BASE + 0x781C) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_772_REG              (xxx_GICD_REGS_BASE + 0x7824) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_773_REG              (xxx_GICD_REGS_BASE + 0x782C) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_774_REG              (xxx_GICD_REGS_BASE + 0x7834) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_775_REG              (xxx_GICD_REGS_BASE + 0x783C) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_776_REG              (xxx_GICD_REGS_BASE + 0x7844) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_777_REG              (xxx_GICD_REGS_BASE + 0x784C) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_778_REG              (xxx_GICD_REGS_BASE + 0x7854) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_779_REG              (xxx_GICD_REGS_BASE + 0x785C) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_780_REG              (xxx_GICD_REGS_BASE + 0x7864) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_781_REG              (xxx_GICD_REGS_BASE + 0x786C) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_782_REG              (xxx_GICD_REGS_BASE + 0x7874) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_783_REG              (xxx_GICD_REGS_BASE + 0x787C) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_784_REG              (xxx_GICD_REGS_BASE + 0x7884) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_785_REG              (xxx_GICD_REGS_BASE + 0x788C) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_786_REG              (xxx_GICD_REGS_BASE + 0x7894) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_787_REG              (xxx_GICD_REGS_BASE + 0x789C) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_788_REG              (xxx_GICD_REGS_BASE + 0x78A4) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_789_REG              (xxx_GICD_REGS_BASE + 0x78AC) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_790_REG              (xxx_GICD_REGS_BASE + 0x78B4) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_791_REG              (xxx_GICD_REGS_BASE + 0x78BC) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_792_REG              (xxx_GICD_REGS_BASE + 0x78C4) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_793_REG              (xxx_GICD_REGS_BASE + 0x78CC) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_794_REG              (xxx_GICD_REGS_BASE + 0x78D4) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_795_REG              (xxx_GICD_REGS_BASE + 0x78DC) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_796_REG              (xxx_GICD_REGS_BASE + 0x78E4) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_797_REG              (xxx_GICD_REGS_BASE + 0x78EC) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_798_REG              (xxx_GICD_REGS_BASE + 0x78F4) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_799_REG              (xxx_GICD_REGS_BASE + 0x78FC) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_800_REG              (xxx_GICD_REGS_BASE + 0x7904) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_801_REG              (xxx_GICD_REGS_BASE + 0x790C) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_802_REG              (xxx_GICD_REGS_BASE + 0x7914) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_803_REG              (xxx_GICD_REGS_BASE + 0x791C) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_804_REG              (xxx_GICD_REGS_BASE + 0x7924) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_805_REG              (xxx_GICD_REGS_BASE + 0x792C) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_806_REG              (xxx_GICD_REGS_BASE + 0x7934) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_807_REG              (xxx_GICD_REGS_BASE + 0x793C) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_808_REG              (xxx_GICD_REGS_BASE + 0x7944) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_809_REG              (xxx_GICD_REGS_BASE + 0x794C) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_810_REG              (xxx_GICD_REGS_BASE + 0x7954) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_811_REG              (xxx_GICD_REGS_BASE + 0x795C) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_812_REG              (xxx_GICD_REGS_BASE + 0x7964) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_813_REG              (xxx_GICD_REGS_BASE + 0x796C) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_814_REG              (xxx_GICD_REGS_BASE + 0x7974) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_815_REG              (xxx_GICD_REGS_BASE + 0x797C) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_816_REG              (xxx_GICD_REGS_BASE + 0x7984) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_817_REG              (xxx_GICD_REGS_BASE + 0x798C) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_818_REG              (xxx_GICD_REGS_BASE + 0x7994) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_819_REG              (xxx_GICD_REGS_BASE + 0x799C) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_820_REG              (xxx_GICD_REGS_BASE + 0x79A4) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_821_REG              (xxx_GICD_REGS_BASE + 0x79AC) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_822_REG              (xxx_GICD_REGS_BASE + 0x79B4) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_823_REG              (xxx_GICD_REGS_BASE + 0x79BC) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_824_REG              (xxx_GICD_REGS_BASE + 0x79C4) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_825_REG              (xxx_GICD_REGS_BASE + 0x79CC) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_826_REG              (xxx_GICD_REGS_BASE + 0x79D4) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_827_REG              (xxx_GICD_REGS_BASE + 0x79DC) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_828_REG              (xxx_GICD_REGS_BASE + 0x79E4) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_829_REG              (xxx_GICD_REGS_BASE + 0x79EC) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_830_REG              (xxx_GICD_REGS_BASE + 0x79F4) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_831_REG              (xxx_GICD_REGS_BASE + 0x79FC) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_832_REG              (xxx_GICD_REGS_BASE + 0x7A04) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_833_REG              (xxx_GICD_REGS_BASE + 0x7A0C) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_834_REG              (xxx_GICD_REGS_BASE + 0x7A14) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_835_REG              (xxx_GICD_REGS_BASE + 0x7A1C) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_836_REG              (xxx_GICD_REGS_BASE + 0x7A24) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_837_REG              (xxx_GICD_REGS_BASE + 0x7A2C) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_838_REG              (xxx_GICD_REGS_BASE + 0x7A34) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_839_REG              (xxx_GICD_REGS_BASE + 0x7A3C) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_840_REG              (xxx_GICD_REGS_BASE + 0x7A44) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_841_REG              (xxx_GICD_REGS_BASE + 0x7A4C) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_842_REG              (xxx_GICD_REGS_BASE + 0x7A54) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_843_REG              (xxx_GICD_REGS_BASE + 0x7A5C) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_844_REG              (xxx_GICD_REGS_BASE + 0x7A64) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_845_REG              (xxx_GICD_REGS_BASE + 0x7A6C) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_846_REG              (xxx_GICD_REGS_BASE + 0x7A74) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_847_REG              (xxx_GICD_REGS_BASE + 0x7A7C) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_848_REG              (xxx_GICD_REGS_BASE + 0x7A84) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_849_REG              (xxx_GICD_REGS_BASE + 0x7A8C) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_850_REG              (xxx_GICD_REGS_BASE + 0x7A94) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_851_REG              (xxx_GICD_REGS_BASE + 0x7A9C) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_852_REG              (xxx_GICD_REGS_BASE + 0x7AA4) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_853_REG              (xxx_GICD_REGS_BASE + 0x7AAC) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_854_REG              (xxx_GICD_REGS_BASE + 0x7AB4) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_855_REG              (xxx_GICD_REGS_BASE + 0x7ABC) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_856_REG              (xxx_GICD_REGS_BASE + 0x7AC4) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_857_REG              (xxx_GICD_REGS_BASE + 0x7ACC) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_858_REG              (xxx_GICD_REGS_BASE + 0x7AD4) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_859_REG              (xxx_GICD_REGS_BASE + 0x7ADC) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_860_REG              (xxx_GICD_REGS_BASE + 0x7AE4) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_861_REG              (xxx_GICD_REGS_BASE + 0x7AEC) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_862_REG              (xxx_GICD_REGS_BASE + 0x7AF4) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_863_REG              (xxx_GICD_REGS_BASE + 0x7AFC) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_864_REG              (xxx_GICD_REGS_BASE + 0x7B04) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_865_REG              (xxx_GICD_REGS_BASE + 0x7B0C) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_866_REG              (xxx_GICD_REGS_BASE + 0x7B14) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_867_REG              (xxx_GICD_REGS_BASE + 0x7B1C) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_868_REG              (xxx_GICD_REGS_BASE + 0x7B24) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_869_REG              (xxx_GICD_REGS_BASE + 0x7B2C) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_870_REG              (xxx_GICD_REGS_BASE + 0x7B34) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_871_REG              (xxx_GICD_REGS_BASE + 0x7B3C) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_872_REG              (xxx_GICD_REGS_BASE + 0x7B44) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_873_REG              (xxx_GICD_REGS_BASE + 0x7B4C) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_874_REG              (xxx_GICD_REGS_BASE + 0x7B54) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_875_REG              (xxx_GICD_REGS_BASE + 0x7B5C) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_876_REG              (xxx_GICD_REGS_BASE + 0x7B64) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_877_REG              (xxx_GICD_REGS_BASE + 0x7B6C) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_878_REG              (xxx_GICD_REGS_BASE + 0x7B74) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_879_REG              (xxx_GICD_REGS_BASE + 0x7B7C) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_880_REG              (xxx_GICD_REGS_BASE + 0x7B84) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_881_REG              (xxx_GICD_REGS_BASE + 0x7B8C) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_882_REG              (xxx_GICD_REGS_BASE + 0x7B94) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_883_REG              (xxx_GICD_REGS_BASE + 0x7B9C) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_884_REG              (xxx_GICD_REGS_BASE + 0x7BA4) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_885_REG              (xxx_GICD_REGS_BASE + 0x7BAC) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_886_REG              (xxx_GICD_REGS_BASE + 0x7BB4) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_887_REG              (xxx_GICD_REGS_BASE + 0x7BBC) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_888_REG              (xxx_GICD_REGS_BASE + 0x7BC4) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_889_REG              (xxx_GICD_REGS_BASE + 0x7BCC) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_890_REG              (xxx_GICD_REGS_BASE + 0x7BD4) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_891_REG              (xxx_GICD_REGS_BASE + 0x7BDC) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_892_REG              (xxx_GICD_REGS_BASE + 0x7BE4) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_893_REG              (xxx_GICD_REGS_BASE + 0x7BEC) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_894_REG              (xxx_GICD_REGS_BASE + 0x7BF4) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_895_REG              (xxx_GICD_REGS_BASE + 0x7BFC) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_896_REG              (xxx_GICD_REGS_BASE + 0x7C04) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_897_REG              (xxx_GICD_REGS_BASE + 0x7C0C) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_898_REG              (xxx_GICD_REGS_BASE + 0x7C14) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_899_REG              (xxx_GICD_REGS_BASE + 0x7C1C) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_900_REG              (xxx_GICD_REGS_BASE + 0x7C24) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_901_REG              (xxx_GICD_REGS_BASE + 0x7C2C) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_902_REG              (xxx_GICD_REGS_BASE + 0x7C34) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_903_REG              (xxx_GICD_REGS_BASE + 0x7C3C) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_904_REG              (xxx_GICD_REGS_BASE + 0x7C44) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_905_REG              (xxx_GICD_REGS_BASE + 0x7C4C) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_906_REG              (xxx_GICD_REGS_BASE + 0x7C54) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_907_REG              (xxx_GICD_REGS_BASE + 0x7C5C) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_908_REG              (xxx_GICD_REGS_BASE + 0x7C64) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_909_REG              (xxx_GICD_REGS_BASE + 0x7C6C) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_910_REG              (xxx_GICD_REGS_BASE + 0x7C74) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_911_REG              (xxx_GICD_REGS_BASE + 0x7C7C) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_912_REG              (xxx_GICD_REGS_BASE + 0x7C84) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_913_REG              (xxx_GICD_REGS_BASE + 0x7C8C) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_914_REG              (xxx_GICD_REGS_BASE + 0x7C94) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_915_REG              (xxx_GICD_REGS_BASE + 0x7C9C) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_916_REG              (xxx_GICD_REGS_BASE + 0x7CA4) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_917_REG              (xxx_GICD_REGS_BASE + 0x7CAC) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_918_REG              (xxx_GICD_REGS_BASE + 0x7CB4) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_919_REG              (xxx_GICD_REGS_BASE + 0x7CBC) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_920_REG              (xxx_GICD_REGS_BASE + 0x7CC4) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_921_REG              (xxx_GICD_REGS_BASE + 0x7CCC) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_922_REG              (xxx_GICD_REGS_BASE + 0x7CD4) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_923_REG              (xxx_GICD_REGS_BASE + 0x7CDC) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_924_REG              (xxx_GICD_REGS_BASE + 0x7CE4) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_925_REG              (xxx_GICD_REGS_BASE + 0x7CEC) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_926_REG              (xxx_GICD_REGS_BASE + 0x7CF4) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_927_REG              (xxx_GICD_REGS_BASE + 0x7CFC) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_928_REG              (xxx_GICD_REGS_BASE + 0x7D04) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_929_REG              (xxx_GICD_REGS_BASE + 0x7D0C) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_930_REG              (xxx_GICD_REGS_BASE + 0x7D14) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_931_REG              (xxx_GICD_REGS_BASE + 0x7D1C) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_932_REG              (xxx_GICD_REGS_BASE + 0x7D24) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_933_REG              (xxx_GICD_REGS_BASE + 0x7D2C) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_934_REG              (xxx_GICD_REGS_BASE + 0x7D34) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_935_REG              (xxx_GICD_REGS_BASE + 0x7D3C) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_936_REG              (xxx_GICD_REGS_BASE + 0x7D44) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_937_REG              (xxx_GICD_REGS_BASE + 0x7D4C) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_938_REG              (xxx_GICD_REGS_BASE + 0x7D54) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_939_REG              (xxx_GICD_REGS_BASE + 0x7D5C) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_940_REG              (xxx_GICD_REGS_BASE + 0x7D64) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_941_REG              (xxx_GICD_REGS_BASE + 0x7D6C) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_942_REG              (xxx_GICD_REGS_BASE + 0x7D74) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_943_REG              (xxx_GICD_REGS_BASE + 0x7D7C) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_944_REG              (xxx_GICD_REGS_BASE + 0x7D84) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_945_REG              (xxx_GICD_REGS_BASE + 0x7D8C) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_946_REG              (xxx_GICD_REGS_BASE + 0x7D94) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_947_REG              (xxx_GICD_REGS_BASE + 0x7D9C) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_948_REG              (xxx_GICD_REGS_BASE + 0x7DA4) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_949_REG              (xxx_GICD_REGS_BASE + 0x7DAC) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_950_REG              (xxx_GICD_REGS_BASE + 0x7DB4) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_951_REG              (xxx_GICD_REGS_BASE + 0x7DBC) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_952_REG              (xxx_GICD_REGS_BASE + 0x7DC4) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_953_REG              (xxx_GICD_REGS_BASE + 0x7DCC) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_954_REG              (xxx_GICD_REGS_BASE + 0x7DD4) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_955_REG              (xxx_GICD_REGS_BASE + 0x7DDC) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_956_REG              (xxx_GICD_REGS_BASE + 0x7DE4) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_957_REG              (xxx_GICD_REGS_BASE + 0x7DEC) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_958_REG              (xxx_GICD_REGS_BASE + 0x7DF4) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_959_REG              (xxx_GICD_REGS_BASE + 0x7DFC) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_960_REG              (xxx_GICD_REGS_BASE + 0x7E04) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_961_REG              (xxx_GICD_REGS_BASE + 0x7E0C) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_962_REG              (xxx_GICD_REGS_BASE + 0x7E14) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_963_REG              (xxx_GICD_REGS_BASE + 0x7E1C) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_964_REG              (xxx_GICD_REGS_BASE + 0x7E24) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_965_REG              (xxx_GICD_REGS_BASE + 0x7E2C) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_966_REG              (xxx_GICD_REGS_BASE + 0x7E34) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_967_REG              (xxx_GICD_REGS_BASE + 0x7E3C) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_968_REG              (xxx_GICD_REGS_BASE + 0x7E44) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_969_REG              (xxx_GICD_REGS_BASE + 0x7E4C) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_970_REG              (xxx_GICD_REGS_BASE + 0x7E54) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_971_REG              (xxx_GICD_REGS_BASE + 0x7E5C) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_972_REG              (xxx_GICD_REGS_BASE + 0x7E64) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_973_REG              (xxx_GICD_REGS_BASE + 0x7E6C) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_974_REG              (xxx_GICD_REGS_BASE + 0x7E74) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_975_REG              (xxx_GICD_REGS_BASE + 0x7E7C) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_976_REG              (xxx_GICD_REGS_BASE + 0x7E84) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_977_REG              (xxx_GICD_REGS_BASE + 0x7E8C) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_978_REG              (xxx_GICD_REGS_BASE + 0x7E94) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_979_REG              (xxx_GICD_REGS_BASE + 0x7E9C) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_980_REG              (xxx_GICD_REGS_BASE + 0x7EA4) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_981_REG              (xxx_GICD_REGS_BASE + 0x7EAC) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_982_REG              (xxx_GICD_REGS_BASE + 0x7EB4) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_983_REG              (xxx_GICD_REGS_BASE + 0x7EBC) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_984_REG              (xxx_GICD_REGS_BASE + 0x7EC4) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_985_REG              (xxx_GICD_REGS_BASE + 0x7ECC) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_986_REG              (xxx_GICD_REGS_BASE + 0x7ED4) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_987_REG              (xxx_GICD_REGS_BASE + 0x7EDC) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_988_REG              (xxx_GICD_REGS_BASE + 0x7EE4) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_989_REG              (xxx_GICD_REGS_BASE + 0x7EEC) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_990_REG              (xxx_GICD_REGS_BASE + 0x7EF4) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_991_REG              (xxx_GICD_REGS_BASE + 0x7EFC) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_992_REG              (xxx_GICD_REGS_BASE + 0x7F04) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_993_REG              (xxx_GICD_REGS_BASE + 0x7F0C) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_994_REG              (xxx_GICD_REGS_BASE + 0x7F14) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_995_REG              (xxx_GICD_REGS_BASE + 0x7F1C) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_996_REG              (xxx_GICD_REGS_BASE + 0x7F24) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_997_REG              (xxx_GICD_REGS_BASE + 0x7F2C) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_998_REG              (xxx_GICD_REGS_BASE + 0x7F34) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_999_REG              (xxx_GICD_REGS_BASE + 0x7F3C) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_1000_REG             (xxx_GICD_REGS_BASE + 0x7F44) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_1001_REG             (xxx_GICD_REGS_BASE + 0x7F4C) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_1002_REG             (xxx_GICD_REGS_BASE + 0x7F54) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_1003_REG             (xxx_GICD_REGS_BASE + 0x7F5C) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_1004_REG             (xxx_GICD_REGS_BASE + 0x7F64) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_1005_REG             (xxx_GICD_REGS_BASE + 0x7F6C) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_1006_REG             (xxx_GICD_REGS_BASE + 0x7F74) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_1007_REG             (xxx_GICD_REGS_BASE + 0x7F7C) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_1008_REG             (xxx_GICD_REGS_BASE + 0x7F84) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_1009_REG             (xxx_GICD_REGS_BASE + 0x7F8C) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_1010_REG             (xxx_GICD_REGS_BASE + 0x7F94) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_1011_REG             (xxx_GICD_REGS_BASE + 0x7F9C) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_1012_REG             (xxx_GICD_REGS_BASE + 0x7FA4) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_1013_REG             (xxx_GICD_REGS_BASE + 0x7FAC) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_1014_REG             (xxx_GICD_REGS_BASE + 0x7FB4) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_1015_REG             (xxx_GICD_REGS_BASE + 0x7FBC) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_1016_REG             (xxx_GICD_REGS_BASE + 0x7FC4) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_1017_REG             (xxx_GICD_REGS_BASE + 0x7FCC) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_1018_REG             (xxx_GICD_REGS_BASE + 0x7FD4) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_1019_REG             (xxx_GICD_REGS_BASE + 0x7FDC) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_1020_REG             (xxx_GICD_REGS_BASE + 0x7FE4) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_1021_REG             (xxx_GICD_REGS_BASE + 0x7FEC) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_1022_REG             (xxx_GICD_REGS_BASE + 0x7FF4) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_GICD_IROUTERN_H_1023_REG             (xxx_GICD_REGS_BASE + 0x7FFC) /* 亲和性路由寄存器（[63:32]）。a{0,127} */
#define xxx_GICD_REGS_INT_TMOUT_REG                        (xxx_GICD_REGS_BASE + 0x1000) /* 中断TimeOut时间配置寄存器 */
#define xxx_GICD_REGS_GIC_FUNC_EN_REG                      (xxx_GICD_REGS_BASE + 0x2080) /* GICD功能使能开关 */
#define xxx_GICD_REGS_GICD_1NSPI_CTRL_REG                  (xxx_GICD_REGS_BASE + 0x2084) /* 1-N SPI中断调度控制 */
#define xxx_GICD_REGS_DFX_INT_MONITOR_CTRL_REG             (xxx_GICD_REGS_BASE + 0x3500) /* 中断统计计数器统计选择控制寄存器。 */
#define xxx_GICD_REGS_GICD_REGS_CNT_CLR_CE_REG             (xxx_GICD_REGS_BASE + 0x3504) /* GICD中CNT_CYC类型寄存器读清控制信号 */
#define xxx_GICD_REGS_DFX_CPUIF_INT_ACP_CNT_REG            (xxx_GICD_REGS_BASE + 0x3508) /* 中断被FSM接受数量统计计数器。 */
#define xxx_GICD_REGS_DFX_CPUIF_LEGACY_RDY_CNT_REG         (xxx_GICD_REGS_BASE + 0x350C) /* 中断请求有效数量统计计数器。 */
#define xxx_GICD_REGS_DFX_CPUIF_LEGACY_ACK_CNT_REG         (xxx_GICD_REGS_BASE + 0x3510) /* 中断请求有效数量统计计数器。 */
#define xxx_GICD_REGS_DFX_CPUIF_DEGACY_DEACT_CNT_REG       (xxx_GICD_REGS_BASE + 0x3514) /* 中断上报后收到DEACTIVATE响应数量统计计数器。 */
#define xxx_GICD_REGS_DFX_CPUIF_LPI_RDY_CNT_REG            (xxx_GICD_REGS_BASE + 0x3518) /* 中断请求有效数量统计计数器。 */
#define xxx_GICD_REGS_DFX_CPUIF_LPI_ACK_CNT_REG            (xxx_GICD_REGS_BASE + 0x351C) /* 中断请求有效数量统计计数器。 */
#define xxx_GICD_REGS_DFX_CPUIF_VLPI_RDY_CNT_REG           (xxx_GICD_REGS_BASE + 0x3520) /* 中断上报后收到ACTIVATE响应数量统计计数器。 */
#define xxx_GICD_REGS_DFX_CPUIF_VLPI_ACK_CNT_REG           (xxx_GICD_REGS_BASE + 0x3524) /* 中断上报后收到ACTIVATE响应数量统计计数器。 */
#define xxx_GICD_REGS_DFX_SPI_MONITOR_CTRL_REG             (xxx_GICD_REGS_BASE + 0x3644) /* SPI中断统计计数器统计选择控制寄存器。 */
#define xxx_GICD_REGS_DFX_GICD_CFG_ERR_REG                 (xxx_GICD_REGS_BASE + 0x3648) /* GICD配置错误状态寄存器。 */
#define xxx_GICD_REGS_DFX_LOWIDS_MERGE_CNT_REG             (xxx_GICD_REGS_BASE + 0x3650) /* SGI/PPI中断被合并的事件记录寄存器 */
#define xxx_GICD_REGS_DFX_LOWIDS_SPLIT_CNT_REG             (xxx_GICD_REGS_BASE + 0x3654) /* SGI/中断完成但中断源未清除的事件记录寄存器 */
#define xxx_GICD_REGS_DFX_LOWIDS_DROP_CNT_REG              (xxx_GICD_REGS_BASE + 0x3658) /* SGI/PPI中断被丢弃的事件记录寄存器 */
#define xxx_GICD_REGS_DFX_SPIS_MERGE_CNT_REG               (xxx_GICD_REGS_BASE + 0x365C) /* SPI中断被合并的事件记录寄存器 */
#define xxx_GICD_REGS_DFX_SPIS_SPLIT_CNT_REG               (xxx_GICD_REGS_BASE + 0x3660) /* SPI中断完成但中断源未清除的事件记录寄存器 */
#define xxx_GICD_REGS_DFX_SPIS_DROP_CNT_REG                (xxx_GICD_REGS_BASE + 0x3664) /* SPI中断被丢弃的事件记录寄存器 */
#define xxx_GICD_REGS_DFX_ID_SET_MAXTIME_STAT_0_REG        (xxx_GICD_REGS_BASE + 0x3924) /* 中断上报最大延时统计寄存器0 */
#define xxx_GICD_REGS_DFX_ID_SET_MAXTIME_STAT_1_REG        (xxx_GICD_REGS_BASE + 0x3928) /* 中断上报最大延时统计寄存器1 */
#define xxx_GICD_REGS_DFX_STREAM_NODE_STATUS_REG           (xxx_GICD_REGS_BASE + 0x3B20) /* STREAM总线NODE状态寄存器。 */
#define xxx_GICD_REGS_DFX_CPUIF_FSM_INFO_0_REG             (xxx_GICD_REGS_BASE + 0xB000) /* GICD_CPUIF接口状态机0状态机指示。 */
#define xxx_GICD_REGS_DFX_CPUIF_FSM_INFO_1_REG             (xxx_GICD_REGS_BASE + 0xB040) /* GICD_CPUIF接口状态机0状态机指示。 */
#define xxx_GICD_REGS_DFX_CPUIF_FSM_INFO_2_REG             (xxx_GICD_REGS_BASE + 0xB080) /* GICD_CPUIF接口状态机0状态机指示。 */
#define xxx_GICD_REGS_DFX_CPUIF_FSM_INFO_3_REG             (xxx_GICD_REGS_BASE + 0xB0C0) /* GICD_CPUIF接口状态机0状态机指示。 */
#define xxx_GICD_REGS_DFX_CPUIF_FSM_INFO_4_REG             (xxx_GICD_REGS_BASE + 0xB100) /* GICD_CPUIF接口状态机0状态机指示。 */
#define xxx_GICD_REGS_DFX_CPUIF_FSM_INFO_5_REG             (xxx_GICD_REGS_BASE + 0xB140) /* GICD_CPUIF接口状态机0状态机指示。 */
#define xxx_GICD_REGS_DFX_CPUIF_FSM_INFO_6_REG             (xxx_GICD_REGS_BASE + 0xB180) /* GICD_CPUIF接口状态机0状态机指示。 */
#define xxx_GICD_REGS_DFX_CPUIF_FSM_INFO_7_REG             (xxx_GICD_REGS_BASE + 0xB1C0) /* GICD_CPUIF接口状态机0状态机指示。 */
#define xxx_GICD_REGS_DFX_CPUIF_FSM_INFO_8_REG             (xxx_GICD_REGS_BASE + 0xB200) /* GICD_CPUIF接口状态机0状态机指示。 */
#define xxx_GICD_REGS_DFX_CPUIF_FSM_INFO_9_REG             (xxx_GICD_REGS_BASE + 0xB240) /* GICD_CPUIF接口状态机0状态机指示。 */
#define xxx_GICD_REGS_DFX_CPUIF_FSM_INFO_10_REG            (xxx_GICD_REGS_BASE + 0xB280) /* GICD_CPUIF接口状态机0状态机指示。 */
#define xxx_GICD_REGS_DFX_CPUIF_FSM_INFO_11_REG            (xxx_GICD_REGS_BASE + 0xB2C0) /* GICD_CPUIF接口状态机0状态机指示。 */
#define xxx_GICD_REGS_DFX_CPUIF_FSM_INFO_12_REG            (xxx_GICD_REGS_BASE + 0xB300) /* GICD_CPUIF接口状态机0状态机指示。 */
#define xxx_GICD_REGS_DFX_CPUIF_FSM_INFO_13_REG            (xxx_GICD_REGS_BASE + 0xB340) /* GICD_CPUIF接口状态机0状态机指示。 */
#define xxx_GICD_REGS_DFX_CPUIF_FSM_INFO_14_REG            (xxx_GICD_REGS_BASE + 0xB380) /* GICD_CPUIF接口状态机0状态机指示。 */
#define xxx_GICD_REGS_DFX_CPUIF_FSM_INFO_15_REG            (xxx_GICD_REGS_BASE + 0xB3C0) /* GICD_CPUIF接口状态机0状态机指示。 */
#define xxx_GICD_REGS_DFX_CPUIF_FSM_INFO_16_REG            (xxx_GICD_REGS_BASE + 0xB400) /* GICD_CPUIF接口状态机0状态机指示。 */
#define xxx_GICD_REGS_DFX_CPUIF_FSM_INFO_17_REG            (xxx_GICD_REGS_BASE + 0xB440) /* GICD_CPUIF接口状态机0状态机指示。 */
#define xxx_GICD_REGS_DFX_CPUIF_FSM_INFO_18_REG            (xxx_GICD_REGS_BASE + 0xB480) /* GICD_CPUIF接口状态机0状态机指示。 */
#define xxx_GICD_REGS_DFX_CPUIF_FSM_INFO_19_REG            (xxx_GICD_REGS_BASE + 0xB4C0) /* GICD_CPUIF接口状态机0状态机指示。 */
#define xxx_GICD_REGS_DFX_CPUIF_FSM_INFO_20_REG            (xxx_GICD_REGS_BASE + 0xB500) /* GICD_CPUIF接口状态机0状态机指示。 */
#define xxx_GICD_REGS_DFX_CPUIF_FSM_INFO_21_REG            (xxx_GICD_REGS_BASE + 0xB540) /* GICD_CPUIF接口状态机0状态机指示。 */
#define xxx_GICD_REGS_DFX_CPUIF_FSM_INFO_22_REG            (xxx_GICD_REGS_BASE + 0xB580) /* GICD_CPUIF接口状态机0状态机指示。 */
#define xxx_GICD_REGS_DFX_CPUIF_FSM_INFO_23_REG            (xxx_GICD_REGS_BASE + 0xB5C0) /* GICD_CPUIF接口状态机0状态机指示。 */
#define xxx_GICD_REGS_DFX_CPUIF_FSM_INFO_24_REG            (xxx_GICD_REGS_BASE + 0xB600) /* GICD_CPUIF接口状态机0状态机指示。 */
#define xxx_GICD_REGS_DFX_CPUIF_FSM_INFO_25_REG            (xxx_GICD_REGS_BASE + 0xB640) /* GICD_CPUIF接口状态机0状态机指示。 */
#define xxx_GICD_REGS_DFX_CPUIF_FSM_INFO_26_REG            (xxx_GICD_REGS_BASE + 0xB680) /* GICD_CPUIF接口状态机0状态机指示。 */
#define xxx_GICD_REGS_DFX_CPUIF_FSM_INFO_27_REG            (xxx_GICD_REGS_BASE + 0xB6C0) /* GICD_CPUIF接口状态机0状态机指示。 */
#define xxx_GICD_REGS_DFX_CPUIF_FSM_INFO_28_REG            (xxx_GICD_REGS_BASE + 0xB700) /* GICD_CPUIF接口状态机0状态机指示。 */
#define xxx_GICD_REGS_DFX_CPUIF_FSM_INFO_29_REG            (xxx_GICD_REGS_BASE + 0xB740) /* GICD_CPUIF接口状态机0状态机指示。 */
#define xxx_GICD_REGS_DFX_CPUIF_FSM_INFO_30_REG            (xxx_GICD_REGS_BASE + 0xB780) /* GICD_CPUIF接口状态机0状态机指示。 */
#define xxx_GICD_REGS_DFX_CPUIF_FSM_INFO_31_REG            (xxx_GICD_REGS_BASE + 0xB7C0) /* GICD_CPUIF接口状态机0状态机指示。 */
#define xxx_GICD_REGS_DFX_CPUIF_FSM_INFO1_0_REG            (xxx_GICD_REGS_BASE + 0xB004) /* GICD_CPUIF接口状态机0发送中断ID及优先级信息寄存器。 */
#define xxx_GICD_REGS_DFX_CPUIF_FSM_INFO1_1_REG            (xxx_GICD_REGS_BASE + 0xB044) /* GICD_CPUIF接口状态机0发送中断ID及优先级信息寄存器。 */
#define xxx_GICD_REGS_DFX_CPUIF_FSM_INFO1_2_REG            (xxx_GICD_REGS_BASE + 0xB084) /* GICD_CPUIF接口状态机0发送中断ID及优先级信息寄存器。 */
#define xxx_GICD_REGS_DFX_CPUIF_FSM_INFO1_3_REG            (xxx_GICD_REGS_BASE + 0xB0C4) /* GICD_CPUIF接口状态机0发送中断ID及优先级信息寄存器。 */
#define xxx_GICD_REGS_DFX_CPUIF_FSM_INFO1_4_REG            (xxx_GICD_REGS_BASE + 0xB104) /* GICD_CPUIF接口状态机0发送中断ID及优先级信息寄存器。 */
#define xxx_GICD_REGS_DFX_CPUIF_FSM_INFO1_5_REG            (xxx_GICD_REGS_BASE + 0xB144) /* GICD_CPUIF接口状态机0发送中断ID及优先级信息寄存器。 */
#define xxx_GICD_REGS_DFX_CPUIF_FSM_INFO1_6_REG            (xxx_GICD_REGS_BASE + 0xB184) /* GICD_CPUIF接口状态机0发送中断ID及优先级信息寄存器。 */
#define xxx_GICD_REGS_DFX_CPUIF_FSM_INFO1_7_REG            (xxx_GICD_REGS_BASE + 0xB1C4) /* GICD_CPUIF接口状态机0发送中断ID及优先级信息寄存器。 */
#define xxx_GICD_REGS_DFX_CPUIF_FSM_INFO1_8_REG            (xxx_GICD_REGS_BASE + 0xB204) /* GICD_CPUIF接口状态机0发送中断ID及优先级信息寄存器。 */
#define xxx_GICD_REGS_DFX_CPUIF_FSM_INFO1_9_REG            (xxx_GICD_REGS_BASE + 0xB244) /* GICD_CPUIF接口状态机0发送中断ID及优先级信息寄存器。 */
#define xxx_GICD_REGS_DFX_CPUIF_FSM_INFO1_10_REG           (xxx_GICD_REGS_BASE + 0xB284) /* GICD_CPUIF接口状态机0发送中断ID及优先级信息寄存器。 */
#define xxx_GICD_REGS_DFX_CPUIF_FSM_INFO1_11_REG           (xxx_GICD_REGS_BASE + 0xB2C4) /* GICD_CPUIF接口状态机0发送中断ID及优先级信息寄存器。 */
#define xxx_GICD_REGS_DFX_CPUIF_FSM_INFO1_12_REG           (xxx_GICD_REGS_BASE + 0xB304) /* GICD_CPUIF接口状态机0发送中断ID及优先级信息寄存器。 */
#define xxx_GICD_REGS_DFX_CPUIF_FSM_INFO1_13_REG           (xxx_GICD_REGS_BASE + 0xB344) /* GICD_CPUIF接口状态机0发送中断ID及优先级信息寄存器。 */
#define xxx_GICD_REGS_DFX_CPUIF_FSM_INFO1_14_REG           (xxx_GICD_REGS_BASE + 0xB384) /* GICD_CPUIF接口状态机0发送中断ID及优先级信息寄存器。 */
#define xxx_GICD_REGS_DFX_CPUIF_FSM_INFO1_15_REG           (xxx_GICD_REGS_BASE + 0xB3C4) /* GICD_CPUIF接口状态机0发送中断ID及优先级信息寄存器。 */
#define xxx_GICD_REGS_DFX_CPUIF_FSM_INFO1_16_REG           (xxx_GICD_REGS_BASE + 0xB404) /* GICD_CPUIF接口状态机0发送中断ID及优先级信息寄存器。 */
#define xxx_GICD_REGS_DFX_CPUIF_FSM_INFO1_17_REG           (xxx_GICD_REGS_BASE + 0xB444) /* GICD_CPUIF接口状态机0发送中断ID及优先级信息寄存器。 */
#define xxx_GICD_REGS_DFX_CPUIF_FSM_INFO1_18_REG           (xxx_GICD_REGS_BASE + 0xB484) /* GICD_CPUIF接口状态机0发送中断ID及优先级信息寄存器。 */
#define xxx_GICD_REGS_DFX_CPUIF_FSM_INFO1_19_REG           (xxx_GICD_REGS_BASE + 0xB4C4) /* GICD_CPUIF接口状态机0发送中断ID及优先级信息寄存器。 */
#define xxx_GICD_REGS_DFX_CPUIF_FSM_INFO1_20_REG           (xxx_GICD_REGS_BASE + 0xB504) /* GICD_CPUIF接口状态机0发送中断ID及优先级信息寄存器。 */
#define xxx_GICD_REGS_DFX_CPUIF_FSM_INFO1_21_REG           (xxx_GICD_REGS_BASE + 0xB544) /* GICD_CPUIF接口状态机0发送中断ID及优先级信息寄存器。 */
#define xxx_GICD_REGS_DFX_CPUIF_FSM_INFO1_22_REG           (xxx_GICD_REGS_BASE + 0xB584) /* GICD_CPUIF接口状态机0发送中断ID及优先级信息寄存器。 */
#define xxx_GICD_REGS_DFX_CPUIF_FSM_INFO1_23_REG           (xxx_GICD_REGS_BASE + 0xB5C4) /* GICD_CPUIF接口状态机0发送中断ID及优先级信息寄存器。 */
#define xxx_GICD_REGS_DFX_CPUIF_FSM_INFO1_24_REG           (xxx_GICD_REGS_BASE + 0xB604) /* GICD_CPUIF接口状态机0发送中断ID及优先级信息寄存器。 */
#define xxx_GICD_REGS_DFX_CPUIF_FSM_INFO1_25_REG           (xxx_GICD_REGS_BASE + 0xB644) /* GICD_CPUIF接口状态机0发送中断ID及优先级信息寄存器。 */
#define xxx_GICD_REGS_DFX_CPUIF_FSM_INFO1_26_REG           (xxx_GICD_REGS_BASE + 0xB684) /* GICD_CPUIF接口状态机0发送中断ID及优先级信息寄存器。 */
#define xxx_GICD_REGS_DFX_CPUIF_FSM_INFO1_27_REG           (xxx_GICD_REGS_BASE + 0xB6C4) /* GICD_CPUIF接口状态机0发送中断ID及优先级信息寄存器。 */
#define xxx_GICD_REGS_DFX_CPUIF_FSM_INFO1_28_REG           (xxx_GICD_REGS_BASE + 0xB704) /* GICD_CPUIF接口状态机0发送中断ID及优先级信息寄存器。 */
#define xxx_GICD_REGS_DFX_CPUIF_FSM_INFO1_29_REG           (xxx_GICD_REGS_BASE + 0xB744) /* GICD_CPUIF接口状态机0发送中断ID及优先级信息寄存器。 */
#define xxx_GICD_REGS_DFX_CPUIF_FSM_INFO1_30_REG           (xxx_GICD_REGS_BASE + 0xB784) /* GICD_CPUIF接口状态机0发送中断ID及优先级信息寄存器。 */
#define xxx_GICD_REGS_DFX_CPUIF_FSM_INFO1_31_REG           (xxx_GICD_REGS_BASE + 0xB7C4) /* GICD_CPUIF接口状态机0发送中断ID及优先级信息寄存器。 */
#define xxx_GICD_REGS_DFX_CPUIF_STATUS0_0_REG              (xxx_GICD_REGS_BASE + 0xB010) /* GICD_CPUIF接口发送SET命令指示寄存器。 */
#define xxx_GICD_REGS_DFX_CPUIF_STATUS0_1_REG              (xxx_GICD_REGS_BASE + 0xB050) /* GICD_CPUIF接口发送SET命令指示寄存器。 */
#define xxx_GICD_REGS_DFX_CPUIF_STATUS0_2_REG              (xxx_GICD_REGS_BASE + 0xB090) /* GICD_CPUIF接口发送SET命令指示寄存器。 */
#define xxx_GICD_REGS_DFX_CPUIF_STATUS0_3_REG              (xxx_GICD_REGS_BASE + 0xB0D0) /* GICD_CPUIF接口发送SET命令指示寄存器。 */
#define xxx_GICD_REGS_DFX_CPUIF_STATUS0_4_REG              (xxx_GICD_REGS_BASE + 0xB110) /* GICD_CPUIF接口发送SET命令指示寄存器。 */
#define xxx_GICD_REGS_DFX_CPUIF_STATUS0_5_REG              (xxx_GICD_REGS_BASE + 0xB150) /* GICD_CPUIF接口发送SET命令指示寄存器。 */
#define xxx_GICD_REGS_DFX_CPUIF_STATUS0_6_REG              (xxx_GICD_REGS_BASE + 0xB190) /* GICD_CPUIF接口发送SET命令指示寄存器。 */
#define xxx_GICD_REGS_DFX_CPUIF_STATUS0_7_REG              (xxx_GICD_REGS_BASE + 0xB1D0) /* GICD_CPUIF接口发送SET命令指示寄存器。 */
#define xxx_GICD_REGS_DFX_CPUIF_STATUS0_8_REG              (xxx_GICD_REGS_BASE + 0xB210) /* GICD_CPUIF接口发送SET命令指示寄存器。 */
#define xxx_GICD_REGS_DFX_CPUIF_STATUS0_9_REG              (xxx_GICD_REGS_BASE + 0xB250) /* GICD_CPUIF接口发送SET命令指示寄存器。 */
#define xxx_GICD_REGS_DFX_CPUIF_STATUS0_10_REG             (xxx_GICD_REGS_BASE + 0xB290) /* GICD_CPUIF接口发送SET命令指示寄存器。 */
#define xxx_GICD_REGS_DFX_CPUIF_STATUS0_11_REG             (xxx_GICD_REGS_BASE + 0xB2D0) /* GICD_CPUIF接口发送SET命令指示寄存器。 */
#define xxx_GICD_REGS_DFX_CPUIF_STATUS0_12_REG             (xxx_GICD_REGS_BASE + 0xB310) /* GICD_CPUIF接口发送SET命令指示寄存器。 */
#define xxx_GICD_REGS_DFX_CPUIF_STATUS0_13_REG             (xxx_GICD_REGS_BASE + 0xB350) /* GICD_CPUIF接口发送SET命令指示寄存器。 */
#define xxx_GICD_REGS_DFX_CPUIF_STATUS0_14_REG             (xxx_GICD_REGS_BASE + 0xB390) /* GICD_CPUIF接口发送SET命令指示寄存器。 */
#define xxx_GICD_REGS_DFX_CPUIF_STATUS0_15_REG             (xxx_GICD_REGS_BASE + 0xB3D0) /* GICD_CPUIF接口发送SET命令指示寄存器。 */
#define xxx_GICD_REGS_DFX_CPUIF_STATUS0_16_REG             (xxx_GICD_REGS_BASE + 0xB410) /* GICD_CPUIF接口发送SET命令指示寄存器。 */
#define xxx_GICD_REGS_DFX_CPUIF_STATUS0_17_REG             (xxx_GICD_REGS_BASE + 0xB450) /* GICD_CPUIF接口发送SET命令指示寄存器。 */
#define xxx_GICD_REGS_DFX_CPUIF_STATUS0_18_REG             (xxx_GICD_REGS_BASE + 0xB490) /* GICD_CPUIF接口发送SET命令指示寄存器。 */
#define xxx_GICD_REGS_DFX_CPUIF_STATUS0_19_REG             (xxx_GICD_REGS_BASE + 0xB4D0) /* GICD_CPUIF接口发送SET命令指示寄存器。 */
#define xxx_GICD_REGS_DFX_CPUIF_STATUS0_20_REG             (xxx_GICD_REGS_BASE + 0xB510) /* GICD_CPUIF接口发送SET命令指示寄存器。 */
#define xxx_GICD_REGS_DFX_CPUIF_STATUS0_21_REG             (xxx_GICD_REGS_BASE + 0xB550) /* GICD_CPUIF接口发送SET命令指示寄存器。 */
#define xxx_GICD_REGS_DFX_CPUIF_STATUS0_22_REG             (xxx_GICD_REGS_BASE + 0xB590) /* GICD_CPUIF接口发送SET命令指示寄存器。 */
#define xxx_GICD_REGS_DFX_CPUIF_STATUS0_23_REG             (xxx_GICD_REGS_BASE + 0xB5D0) /* GICD_CPUIF接口发送SET命令指示寄存器。 */
#define xxx_GICD_REGS_DFX_CPUIF_STATUS0_24_REG             (xxx_GICD_REGS_BASE + 0xB610) /* GICD_CPUIF接口发送SET命令指示寄存器。 */
#define xxx_GICD_REGS_DFX_CPUIF_STATUS0_25_REG             (xxx_GICD_REGS_BASE + 0xB650) /* GICD_CPUIF接口发送SET命令指示寄存器。 */
#define xxx_GICD_REGS_DFX_CPUIF_STATUS0_26_REG             (xxx_GICD_REGS_BASE + 0xB690) /* GICD_CPUIF接口发送SET命令指示寄存器。 */
#define xxx_GICD_REGS_DFX_CPUIF_STATUS0_27_REG             (xxx_GICD_REGS_BASE + 0xB6D0) /* GICD_CPUIF接口发送SET命令指示寄存器。 */
#define xxx_GICD_REGS_DFX_CPUIF_STATUS0_28_REG             (xxx_GICD_REGS_BASE + 0xB710) /* GICD_CPUIF接口发送SET命令指示寄存器。 */
#define xxx_GICD_REGS_DFX_CPUIF_STATUS0_29_REG             (xxx_GICD_REGS_BASE + 0xB750) /* GICD_CPUIF接口发送SET命令指示寄存器。 */
#define xxx_GICD_REGS_DFX_CPUIF_STATUS0_30_REG             (xxx_GICD_REGS_BASE + 0xB790) /* GICD_CPUIF接口发送SET命令指示寄存器。 */
#define xxx_GICD_REGS_DFX_CPUIF_STATUS0_31_REG             (xxx_GICD_REGS_BASE + 0xB7D0) /* GICD_CPUIF接口发送SET命令指示寄存器。 */
#define xxx_GICD_REGS_DFX_CPUIF_STATUS1_0_REG              (xxx_GICD_REGS_BASE + 0xB014) /* GICD_CPUIF接口接收命令状态FIFO及接收PMR信息寄存器。 */
#define xxx_GICD_REGS_DFX_CPUIF_STATUS1_1_REG              (xxx_GICD_REGS_BASE + 0xB054) /* GICD_CPUIF接口接收命令状态FIFO及接收PMR信息寄存器。 */
#define xxx_GICD_REGS_DFX_CPUIF_STATUS1_2_REG              (xxx_GICD_REGS_BASE + 0xB094) /* GICD_CPUIF接口接收命令状态FIFO及接收PMR信息寄存器。 */
#define xxx_GICD_REGS_DFX_CPUIF_STATUS1_3_REG              (xxx_GICD_REGS_BASE + 0xB0D4) /* GICD_CPUIF接口接收命令状态FIFO及接收PMR信息寄存器。 */
#define xxx_GICD_REGS_DFX_CPUIF_STATUS1_4_REG              (xxx_GICD_REGS_BASE + 0xB114) /* GICD_CPUIF接口接收命令状态FIFO及接收PMR信息寄存器。 */
#define xxx_GICD_REGS_DFX_CPUIF_STATUS1_5_REG              (xxx_GICD_REGS_BASE + 0xB154) /* GICD_CPUIF接口接收命令状态FIFO及接收PMR信息寄存器。 */
#define xxx_GICD_REGS_DFX_CPUIF_STATUS1_6_REG              (xxx_GICD_REGS_BASE + 0xB194) /* GICD_CPUIF接口接收命令状态FIFO及接收PMR信息寄存器。 */
#define xxx_GICD_REGS_DFX_CPUIF_STATUS1_7_REG              (xxx_GICD_REGS_BASE + 0xB1D4) /* GICD_CPUIF接口接收命令状态FIFO及接收PMR信息寄存器。 */
#define xxx_GICD_REGS_DFX_CPUIF_STATUS1_8_REG              (xxx_GICD_REGS_BASE + 0xB214) /* GICD_CPUIF接口接收命令状态FIFO及接收PMR信息寄存器。 */
#define xxx_GICD_REGS_DFX_CPUIF_STATUS1_9_REG              (xxx_GICD_REGS_BASE + 0xB254) /* GICD_CPUIF接口接收命令状态FIFO及接收PMR信息寄存器。 */
#define xxx_GICD_REGS_DFX_CPUIF_STATUS1_10_REG             (xxx_GICD_REGS_BASE + 0xB294) /* GICD_CPUIF接口接收命令状态FIFO及接收PMR信息寄存器。 */
#define xxx_GICD_REGS_DFX_CPUIF_STATUS1_11_REG             (xxx_GICD_REGS_BASE + 0xB2D4) /* GICD_CPUIF接口接收命令状态FIFO及接收PMR信息寄存器。 */
#define xxx_GICD_REGS_DFX_CPUIF_STATUS1_12_REG             (xxx_GICD_REGS_BASE + 0xB314) /* GICD_CPUIF接口接收命令状态FIFO及接收PMR信息寄存器。 */
#define xxx_GICD_REGS_DFX_CPUIF_STATUS1_13_REG             (xxx_GICD_REGS_BASE + 0xB354) /* GICD_CPUIF接口接收命令状态FIFO及接收PMR信息寄存器。 */
#define xxx_GICD_REGS_DFX_CPUIF_STATUS1_14_REG             (xxx_GICD_REGS_BASE + 0xB394) /* GICD_CPUIF接口接收命令状态FIFO及接收PMR信息寄存器。 */
#define xxx_GICD_REGS_DFX_CPUIF_STATUS1_15_REG             (xxx_GICD_REGS_BASE + 0xB3D4) /* GICD_CPUIF接口接收命令状态FIFO及接收PMR信息寄存器。 */
#define xxx_GICD_REGS_DFX_CPUIF_STATUS1_16_REG             (xxx_GICD_REGS_BASE + 0xB414) /* GICD_CPUIF接口接收命令状态FIFO及接收PMR信息寄存器。 */
#define xxx_GICD_REGS_DFX_CPUIF_STATUS1_17_REG             (xxx_GICD_REGS_BASE + 0xB454) /* GICD_CPUIF接口接收命令状态FIFO及接收PMR信息寄存器。 */
#define xxx_GICD_REGS_DFX_CPUIF_STATUS1_18_REG             (xxx_GICD_REGS_BASE + 0xB494) /* GICD_CPUIF接口接收命令状态FIFO及接收PMR信息寄存器。 */
#define xxx_GICD_REGS_DFX_CPUIF_STATUS1_19_REG             (xxx_GICD_REGS_BASE + 0xB4D4) /* GICD_CPUIF接口接收命令状态FIFO及接收PMR信息寄存器。 */
#define xxx_GICD_REGS_DFX_CPUIF_STATUS1_20_REG             (xxx_GICD_REGS_BASE + 0xB514) /* GICD_CPUIF接口接收命令状态FIFO及接收PMR信息寄存器。 */
#define xxx_GICD_REGS_DFX_CPUIF_STATUS1_21_REG             (xxx_GICD_REGS_BASE + 0xB554) /* GICD_CPUIF接口接收命令状态FIFO及接收PMR信息寄存器。 */
#define xxx_GICD_REGS_DFX_CPUIF_STATUS1_22_REG             (xxx_GICD_REGS_BASE + 0xB594) /* GICD_CPUIF接口接收命令状态FIFO及接收PMR信息寄存器。 */
#define xxx_GICD_REGS_DFX_CPUIF_STATUS1_23_REG             (xxx_GICD_REGS_BASE + 0xB5D4) /* GICD_CPUIF接口接收命令状态FIFO及接收PMR信息寄存器。 */
#define xxx_GICD_REGS_DFX_CPUIF_STATUS1_24_REG             (xxx_GICD_REGS_BASE + 0xB614) /* GICD_CPUIF接口接收命令状态FIFO及接收PMR信息寄存器。 */
#define xxx_GICD_REGS_DFX_CPUIF_STATUS1_25_REG             (xxx_GICD_REGS_BASE + 0xB654) /* GICD_CPUIF接口接收命令状态FIFO及接收PMR信息寄存器。 */
#define xxx_GICD_REGS_DFX_CPUIF_STATUS1_26_REG             (xxx_GICD_REGS_BASE + 0xB694) /* GICD_CPUIF接口接收命令状态FIFO及接收PMR信息寄存器。 */
#define xxx_GICD_REGS_DFX_CPUIF_STATUS1_27_REG             (xxx_GICD_REGS_BASE + 0xB6D4) /* GICD_CPUIF接口接收命令状态FIFO及接收PMR信息寄存器。 */
#define xxx_GICD_REGS_DFX_CPUIF_STATUS1_28_REG             (xxx_GICD_REGS_BASE + 0xB714) /* GICD_CPUIF接口接收命令状态FIFO及接收PMR信息寄存器。 */
#define xxx_GICD_REGS_DFX_CPUIF_STATUS1_29_REG             (xxx_GICD_REGS_BASE + 0xB754) /* GICD_CPUIF接口接收命令状态FIFO及接收PMR信息寄存器。 */
#define xxx_GICD_REGS_DFX_CPUIF_STATUS1_30_REG             (xxx_GICD_REGS_BASE + 0xB794) /* GICD_CPUIF接口接收命令状态FIFO及接收PMR信息寄存器。 */
#define xxx_GICD_REGS_DFX_CPUIF_STATUS1_31_REG             (xxx_GICD_REGS_BASE + 0xB7D4) /* GICD_CPUIF接口接收命令状态FIFO及接收PMR信息寄存器。 */
#define xxx_GICD_REGS_DFX_CPUIF_STATUS2_0_REG              (xxx_GICD_REGS_BASE + 0xB018) /* GICD_CPUIF接口接收命令数据指示寄存器。 */
#define xxx_GICD_REGS_DFX_CPUIF_STATUS2_1_REG              (xxx_GICD_REGS_BASE + 0xB058) /* GICD_CPUIF接口接收命令数据指示寄存器。 */
#define xxx_GICD_REGS_DFX_CPUIF_STATUS2_2_REG              (xxx_GICD_REGS_BASE + 0xB098) /* GICD_CPUIF接口接收命令数据指示寄存器。 */
#define xxx_GICD_REGS_DFX_CPUIF_STATUS2_3_REG              (xxx_GICD_REGS_BASE + 0xB0D8) /* GICD_CPUIF接口接收命令数据指示寄存器。 */
#define xxx_GICD_REGS_DFX_CPUIF_STATUS2_4_REG              (xxx_GICD_REGS_BASE + 0xB118) /* GICD_CPUIF接口接收命令数据指示寄存器。 */
#define xxx_GICD_REGS_DFX_CPUIF_STATUS2_5_REG              (xxx_GICD_REGS_BASE + 0xB158) /* GICD_CPUIF接口接收命令数据指示寄存器。 */
#define xxx_GICD_REGS_DFX_CPUIF_STATUS2_6_REG              (xxx_GICD_REGS_BASE + 0xB198) /* GICD_CPUIF接口接收命令数据指示寄存器。 */
#define xxx_GICD_REGS_DFX_CPUIF_STATUS2_7_REG              (xxx_GICD_REGS_BASE + 0xB1D8) /* GICD_CPUIF接口接收命令数据指示寄存器。 */
#define xxx_GICD_REGS_DFX_CPUIF_STATUS2_8_REG              (xxx_GICD_REGS_BASE + 0xB218) /* GICD_CPUIF接口接收命令数据指示寄存器。 */
#define xxx_GICD_REGS_DFX_CPUIF_STATUS2_9_REG              (xxx_GICD_REGS_BASE + 0xB258) /* GICD_CPUIF接口接收命令数据指示寄存器。 */
#define xxx_GICD_REGS_DFX_CPUIF_STATUS2_10_REG             (xxx_GICD_REGS_BASE + 0xB298) /* GICD_CPUIF接口接收命令数据指示寄存器。 */
#define xxx_GICD_REGS_DFX_CPUIF_STATUS2_11_REG             (xxx_GICD_REGS_BASE + 0xB2D8) /* GICD_CPUIF接口接收命令数据指示寄存器。 */
#define xxx_GICD_REGS_DFX_CPUIF_STATUS2_12_REG             (xxx_GICD_REGS_BASE + 0xB318) /* GICD_CPUIF接口接收命令数据指示寄存器。 */
#define xxx_GICD_REGS_DFX_CPUIF_STATUS2_13_REG             (xxx_GICD_REGS_BASE + 0xB358) /* GICD_CPUIF接口接收命令数据指示寄存器。 */
#define xxx_GICD_REGS_DFX_CPUIF_STATUS2_14_REG             (xxx_GICD_REGS_BASE + 0xB398) /* GICD_CPUIF接口接收命令数据指示寄存器。 */
#define xxx_GICD_REGS_DFX_CPUIF_STATUS2_15_REG             (xxx_GICD_REGS_BASE + 0xB3D8) /* GICD_CPUIF接口接收命令数据指示寄存器。 */
#define xxx_GICD_REGS_DFX_CPUIF_STATUS2_16_REG             (xxx_GICD_REGS_BASE + 0xB418) /* GICD_CPUIF接口接收命令数据指示寄存器。 */
#define xxx_GICD_REGS_DFX_CPUIF_STATUS2_17_REG             (xxx_GICD_REGS_BASE + 0xB458) /* GICD_CPUIF接口接收命令数据指示寄存器。 */
#define xxx_GICD_REGS_DFX_CPUIF_STATUS2_18_REG             (xxx_GICD_REGS_BASE + 0xB498) /* GICD_CPUIF接口接收命令数据指示寄存器。 */
#define xxx_GICD_REGS_DFX_CPUIF_STATUS2_19_REG             (xxx_GICD_REGS_BASE + 0xB4D8) /* GICD_CPUIF接口接收命令数据指示寄存器。 */
#define xxx_GICD_REGS_DFX_CPUIF_STATUS2_20_REG             (xxx_GICD_REGS_BASE + 0xB518) /* GICD_CPUIF接口接收命令数据指示寄存器。 */
#define xxx_GICD_REGS_DFX_CPUIF_STATUS2_21_REG             (xxx_GICD_REGS_BASE + 0xB558) /* GICD_CPUIF接口接收命令数据指示寄存器。 */
#define xxx_GICD_REGS_DFX_CPUIF_STATUS2_22_REG             (xxx_GICD_REGS_BASE + 0xB598) /* GICD_CPUIF接口接收命令数据指示寄存器。 */
#define xxx_GICD_REGS_DFX_CPUIF_STATUS2_23_REG             (xxx_GICD_REGS_BASE + 0xB5D8) /* GICD_CPUIF接口接收命令数据指示寄存器。 */
#define xxx_GICD_REGS_DFX_CPUIF_STATUS2_24_REG             (xxx_GICD_REGS_BASE + 0xB618) /* GICD_CPUIF接口接收命令数据指示寄存器。 */
#define xxx_GICD_REGS_DFX_CPUIF_STATUS2_25_REG             (xxx_GICD_REGS_BASE + 0xB658) /* GICD_CPUIF接口接收命令数据指示寄存器。 */
#define xxx_GICD_REGS_DFX_CPUIF_STATUS2_26_REG             (xxx_GICD_REGS_BASE + 0xB698) /* GICD_CPUIF接口接收命令数据指示寄存器。 */
#define xxx_GICD_REGS_DFX_CPUIF_STATUS2_27_REG             (xxx_GICD_REGS_BASE + 0xB6D8) /* GICD_CPUIF接口接收命令数据指示寄存器。 */
#define xxx_GICD_REGS_DFX_CPUIF_STATUS2_28_REG             (xxx_GICD_REGS_BASE + 0xB718) /* GICD_CPUIF接口接收命令数据指示寄存器。 */
#define xxx_GICD_REGS_DFX_CPUIF_STATUS2_29_REG             (xxx_GICD_REGS_BASE + 0xB758) /* GICD_CPUIF接口接收命令数据指示寄存器。 */
#define xxx_GICD_REGS_DFX_CPUIF_STATUS2_30_REG             (xxx_GICD_REGS_BASE + 0xB798) /* GICD_CPUIF接口接收命令数据指示寄存器。 */
#define xxx_GICD_REGS_DFX_CPUIF_STATUS2_31_REG             (xxx_GICD_REGS_BASE + 0xB7D8) /* GICD_CPUIF接口接收命令数据指示寄存器。 */
#define xxx_GICD_REGS_DFX_CPUIF_STATUS3_0_REG              (xxx_GICD_REGS_BASE + 0xB01C) /* GICD_CPUIF接口接收Upstream命令状态指示寄存器。 */
#define xxx_GICD_REGS_DFX_CPUIF_STATUS3_1_REG              (xxx_GICD_REGS_BASE + 0xB05C) /* GICD_CPUIF接口接收Upstream命令状态指示寄存器。 */
#define xxx_GICD_REGS_DFX_CPUIF_STATUS3_2_REG              (xxx_GICD_REGS_BASE + 0xB09C) /* GICD_CPUIF接口接收Upstream命令状态指示寄存器。 */
#define xxx_GICD_REGS_DFX_CPUIF_STATUS3_3_REG              (xxx_GICD_REGS_BASE + 0xB0DC) /* GICD_CPUIF接口接收Upstream命令状态指示寄存器。 */
#define xxx_GICD_REGS_DFX_CPUIF_STATUS3_4_REG              (xxx_GICD_REGS_BASE + 0xB11C) /* GICD_CPUIF接口接收Upstream命令状态指示寄存器。 */
#define xxx_GICD_REGS_DFX_CPUIF_STATUS3_5_REG              (xxx_GICD_REGS_BASE + 0xB15C) /* GICD_CPUIF接口接收Upstream命令状态指示寄存器。 */
#define xxx_GICD_REGS_DFX_CPUIF_STATUS3_6_REG              (xxx_GICD_REGS_BASE + 0xB19C) /* GICD_CPUIF接口接收Upstream命令状态指示寄存器。 */
#define xxx_GICD_REGS_DFX_CPUIF_STATUS3_7_REG              (xxx_GICD_REGS_BASE + 0xB1DC) /* GICD_CPUIF接口接收Upstream命令状态指示寄存器。 */
#define xxx_GICD_REGS_DFX_CPUIF_STATUS3_8_REG              (xxx_GICD_REGS_BASE + 0xB21C) /* GICD_CPUIF接口接收Upstream命令状态指示寄存器。 */
#define xxx_GICD_REGS_DFX_CPUIF_STATUS3_9_REG              (xxx_GICD_REGS_BASE + 0xB25C) /* GICD_CPUIF接口接收Upstream命令状态指示寄存器。 */
#define xxx_GICD_REGS_DFX_CPUIF_STATUS3_10_REG             (xxx_GICD_REGS_BASE + 0xB29C) /* GICD_CPUIF接口接收Upstream命令状态指示寄存器。 */
#define xxx_GICD_REGS_DFX_CPUIF_STATUS3_11_REG             (xxx_GICD_REGS_BASE + 0xB2DC) /* GICD_CPUIF接口接收Upstream命令状态指示寄存器。 */
#define xxx_GICD_REGS_DFX_CPUIF_STATUS3_12_REG             (xxx_GICD_REGS_BASE + 0xB31C) /* GICD_CPUIF接口接收Upstream命令状态指示寄存器。 */
#define xxx_GICD_REGS_DFX_CPUIF_STATUS3_13_REG             (xxx_GICD_REGS_BASE + 0xB35C) /* GICD_CPUIF接口接收Upstream命令状态指示寄存器。 */
#define xxx_GICD_REGS_DFX_CPUIF_STATUS3_14_REG             (xxx_GICD_REGS_BASE + 0xB39C) /* GICD_CPUIF接口接收Upstream命令状态指示寄存器。 */
#define xxx_GICD_REGS_DFX_CPUIF_STATUS3_15_REG             (xxx_GICD_REGS_BASE + 0xB3DC) /* GICD_CPUIF接口接收Upstream命令状态指示寄存器。 */
#define xxx_GICD_REGS_DFX_CPUIF_STATUS3_16_REG             (xxx_GICD_REGS_BASE + 0xB41C) /* GICD_CPUIF接口接收Upstream命令状态指示寄存器。 */
#define xxx_GICD_REGS_DFX_CPUIF_STATUS3_17_REG             (xxx_GICD_REGS_BASE + 0xB45C) /* GICD_CPUIF接口接收Upstream命令状态指示寄存器。 */
#define xxx_GICD_REGS_DFX_CPUIF_STATUS3_18_REG             (xxx_GICD_REGS_BASE + 0xB49C) /* GICD_CPUIF接口接收Upstream命令状态指示寄存器。 */
#define xxx_GICD_REGS_DFX_CPUIF_STATUS3_19_REG             (xxx_GICD_REGS_BASE + 0xB4DC) /* GICD_CPUIF接口接收Upstream命令状态指示寄存器。 */
#define xxx_GICD_REGS_DFX_CPUIF_STATUS3_20_REG             (xxx_GICD_REGS_BASE + 0xB51C) /* GICD_CPUIF接口接收Upstream命令状态指示寄存器。 */
#define xxx_GICD_REGS_DFX_CPUIF_STATUS3_21_REG             (xxx_GICD_REGS_BASE + 0xB55C) /* GICD_CPUIF接口接收Upstream命令状态指示寄存器。 */
#define xxx_GICD_REGS_DFX_CPUIF_STATUS3_22_REG             (xxx_GICD_REGS_BASE + 0xB59C) /* GICD_CPUIF接口接收Upstream命令状态指示寄存器。 */
#define xxx_GICD_REGS_DFX_CPUIF_STATUS3_23_REG             (xxx_GICD_REGS_BASE + 0xB5DC) /* GICD_CPUIF接口接收Upstream命令状态指示寄存器。 */
#define xxx_GICD_REGS_DFX_CPUIF_STATUS3_24_REG             (xxx_GICD_REGS_BASE + 0xB61C) /* GICD_CPUIF接口接收Upstream命令状态指示寄存器。 */
#define xxx_GICD_REGS_DFX_CPUIF_STATUS3_25_REG             (xxx_GICD_REGS_BASE + 0xB65C) /* GICD_CPUIF接口接收Upstream命令状态指示寄存器。 */
#define xxx_GICD_REGS_DFX_CPUIF_STATUS3_26_REG             (xxx_GICD_REGS_BASE + 0xB69C) /* GICD_CPUIF接口接收Upstream命令状态指示寄存器。 */
#define xxx_GICD_REGS_DFX_CPUIF_STATUS3_27_REG             (xxx_GICD_REGS_BASE + 0xB6DC) /* GICD_CPUIF接口接收Upstream命令状态指示寄存器。 */
#define xxx_GICD_REGS_DFX_CPUIF_STATUS3_28_REG             (xxx_GICD_REGS_BASE + 0xB71C) /* GICD_CPUIF接口接收Upstream命令状态指示寄存器。 */
#define xxx_GICD_REGS_DFX_CPUIF_STATUS3_29_REG             (xxx_GICD_REGS_BASE + 0xB75C) /* GICD_CPUIF接口接收Upstream命令状态指示寄存器。 */
#define xxx_GICD_REGS_DFX_CPUIF_STATUS3_30_REG             (xxx_GICD_REGS_BASE + 0xB79C) /* GICD_CPUIF接口接收Upstream命令状态指示寄存器。 */
#define xxx_GICD_REGS_DFX_CPUIF_STATUS3_31_REG             (xxx_GICD_REGS_BASE + 0xB7DC) /* GICD_CPUIF接口接收Upstream命令状态指示寄存器。 */
#define xxx_GICD_REGS_GICD_DFX_INFO0_REG                   (xxx_GICD_REGS_BASE + 0x3C00) /* 被监控的中断接收统计 */
#define xxx_GICD_REGS_GICD_DFX_INFO1_REG                   (xxx_GICD_REGS_BASE + 0x3C04) /* 被监控的中断被ACTIVE统计 */
#define xxx_GICD_REGS_GICD_DFX_INFO2_REG                   (xxx_GICD_REGS_BASE + 0x3C08) /* 被监控的中断被CLR统计 */
#define xxx_GICD_REGS_GICD_DFX_INFO3_REG                   (xxx_GICD_REGS_BASE + 0x3C0C) /* 被监控的中断被合并的统计 */

#endif // __GICD_REGS_REG_OFFSET_H__
