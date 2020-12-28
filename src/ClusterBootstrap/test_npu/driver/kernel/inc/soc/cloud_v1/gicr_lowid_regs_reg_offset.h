// ****************************************************************************** 
// Copyright     :  Copyright (C) 2018, Hisilicon Technologies Co. Ltd.
// File name     :  gicr_lowid_regs_reg_offset.h
// Department    :  CAD Development Department
// Author        :  xxx
// Version       :  1
// Date          :  2013/3/10
// Description   :  The description of xxx project
// Others        :  Generated automatically by nManager V4.2 
// History       :  xxx 2018/07/19 11:13:37 Create file
// ******************************************************************************

#ifndef __GICR_LOWID_REGS_REG_OFFSET_H__
#define __GICR_LOWID_REGS_REG_OFFSET_H__

/* GICR_LOWID_REGS Base address of Module's Register */
#define xxx_GICR_LOWID_REGS_BASE                       (0x60000000)

/******************************************************************************/
/*                      xxx GICR_LOWID_REGS Registers' Definitions                            */
/******************************************************************************/

#define xxx_GICR_LOWID_REGS_GICR_IGROUPR0_REG                    (xxx_GICR_LOWID_REGS_BASE + 0x80)  /* 标识对应中断ID的范围为{0，31}分组至Group0或Group1 */
#define xxx_GICR_LOWID_REGS_GICR_ISENABLER0_REG                  (xxx_GICR_LOWID_REGS_BASE + 0x100) /* 中断ID{0，31}中单个中断的使能寄存器，决定是否发给Cpu Interface。 */
#define xxx_GICR_LOWID_REGS_GICR_ICENABLER0_REG                  (xxx_GICR_LOWID_REGS_BASE + 0x180) /* 中断ID{0，31}单个中断的清除使能的寄存器，确定是否发给CPU Interface。 */
#define xxx_GICR_LOWID_REGS_GICR_IPRIORITY_LOWSGI_NS_0_REG       (xxx_GICR_LOWID_REGS_BASE + 0x400) /* 每8bit对应一个中断的优先级。x{0,3} */
#define xxx_GICR_LOWID_REGS_GICR_IPRIORITY_LOWSGI_NS_1_REG       (xxx_GICR_LOWID_REGS_BASE + 0x404) /* 每8bit对应一个中断的优先级。x{0,3} */
#define xxx_GICR_LOWID_REGS_GICR_IPRIORITY_LOWSGI_NS_2_REG       (xxx_GICR_LOWID_REGS_BASE + 0x408) /* 每8bit对应一个中断的优先级。x{0,3} */
#define xxx_GICR_LOWID_REGS_GICR_IPRIORITY_LOWSGI_NS_3_REG       (xxx_GICR_LOWID_REGS_BASE + 0x40C) /* 每8bit对应一个中断的优先级。x{0,3} */
#define xxx_GICR_LOWID_REGS_GICR_IPRIORITY_LOWPPI2023_NS_REG     (xxx_GICR_LOWID_REGS_BASE + 0x414) /* 每8bit对应一个中断的优先级。 */
#define xxx_GICR_LOWID_REGS_GICR_IPRIORITY_LOWPPI23TO31_NS_0_REG (xxx_GICR_LOWID_REGS_BASE + 0x418) /* 每8bit对应一个中断的优先级。y{0,1} */
#define xxx_GICR_LOWID_REGS_GICR_IPRIORITY_LOWPPI23TO31_NS_1_REG (xxx_GICR_LOWID_REGS_BASE + 0x41C) /* 每8bit对应一个中断的优先级。y{0,1} */
#define xxx_GICR_LOWID_REGS_GICR_ICFGR_LOW_SGI_REG               (xxx_GICR_LOWID_REGS_BASE + 0xC00) /* 该寄存器每2bit表示一个SGI中断是电平敏感还是边沿触发 */
#define xxx_GICR_LOWID_REGS_GICR_ICFGR_LOW_PPI_REG               (xxx_GICR_LOWID_REGS_BASE + 0xC04) /* 该寄存器每2bit表示一个PPI中断是电平敏感还是边沿触发 */
#define xxx_GICR_LOWID_REGS_GICR_NSACR_REG                       (xxx_GICR_LOWID_REGS_BASE + 0xE00) /* 控制是否允许非安全软件产生安全SGI中断寄存器 */
#define xxx_GICR_LOWID_REGS_GICR_IGRPMODR0_REG                   (xxx_GICR_LOWID_REGS_BASE + 0xD00) /* 标识对应中断ID的范围为{0，31}分组的修饰 */

#endif // __GICR_LOWID_REGS_REG_OFFSET_H__
