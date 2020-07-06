// ****************************************************************************** 
// Copyright     :  Copyright (C) 2018, Hisilicon Technologies Co. Ltd.
// File name     :  aic_sc_reg_reg_offset.h
// Department    :  CAD Development Department
// Author        :  AIC_TOP
// Version       :  V110
// Date          :  2017/7/10
// Description   :  The description of Hi1910 project (B600)
// Others        :  Generated automatically by nManager V4.2 
// History       :  AIC_TOP 2018/03/20 21:35:31 Create file
// ******************************************************************************

#ifndef __AIC_SC_REG_REG_OFFSET_H__
#define __AIC_SC_REG_REG_OFFSET_H__

/* AIC_SC_REG Base address of Module's Register */
#define SOC_AIC_SC_REG_BASE                       (0x0)

/******************************************************************************/
/*                      SOC AIC_SC_REG Registers' Definitions                            */
/******************************************************************************/

#define SOC_AIC_SC_REG_RUN_STALL_REG             (SOC_AIC_SC_REG_BASE + 0x0)   /* CCU, IFU */
#define SOC_AIC_SC_REG_DBG_CTRL_REG              (SOC_AIC_SC_REG_BASE + 0x8)   /* DBG, CCU */
#define SOC_AIC_SC_REG_RST_CTRL_REG              (SOC_AIC_SC_REG_BASE + 0x10)  /* all */
#define SOC_AIC_SC_REG_FAST_PATH_CTRL_REG        (SOC_AIC_SC_REG_BASE + 0x18)  /* SC */
#define SOC_AIC_SC_REG_AI_CORE_INT_REG           (SOC_AIC_SC_REG_BASE + 0x20)  /* SC, CCU */
#define SOC_AIC_SC_REG_AI_CORE_INT_MASK_REG      (SOC_AIC_SC_REG_BASE + 0x28)  /* SC */
#define SOC_AIC_SC_REG_RST_CNT_REG               (SOC_AIC_SC_REG_BASE + 0x30)  /* SC */
#define SOC_AIC_SC_REG_ECC_EN_REG                (SOC_AIC_SC_REG_BASE + 0x38)  /* MTE, CUBE, VEC */
#define SOC_AIC_SC_REG_AXI_CLAMP_CTRL_REG        (SOC_AIC_SC_REG_BASE + 0x40)  
#define SOC_AIC_SC_REG_AXI_CLAMP_STATE_REG       (SOC_AIC_SC_REG_BASE + 0x48)  
#define SOC_AIC_SC_REG_CLK_GATE_MASK_REG         (SOC_AIC_SC_REG_BASE + 0x50)  
#define SOC_AIC_SC_REG_CLK_DELAY_CNT_REG         (SOC_AIC_SC_REG_BASE + 0x58)  
#define SOC_AIC_SC_REG_TOP_ICG_MBIST_REG         (SOC_AIC_SC_REG_BASE + 0x60)  
#define SOC_AIC_SC_REG_AXI_CLAMP_STATE_1_REG     (SOC_AIC_SC_REG_BASE + 0x68)  
#define SOC_AIC_SC_REG_LOCK_BYPASS_REG           (SOC_AIC_SC_REG_BASE + 0x70)  /* SC */
#define SOC_AIC_SC_REG_SYSCTRL_LOCK_REG          (SOC_AIC_SC_REG_BASE + 0x78)  /* SC */
#define SOC_AIC_SC_REG_PC_START_REG              (SOC_AIC_SC_REG_BASE + 0x80)  /* CCU, IFU */
#define SOC_AIC_SC_REG_PARA_BASE_REG             (SOC_AIC_SC_REG_BASE + 0x88)  /* CCU (SPR) */
#define SOC_AIC_SC_REG_SMMU_SUBSTREAMID_REG      (SOC_AIC_SC_REG_BASE + 0x90)  /* BIU, CCU */
#define SOC_AIC_SC_REG_TASK_CFG_REG              (SOC_AIC_SC_REG_BASE + 0x98)  /* SPR */
#define SOC_AIC_SC_REG_DATA_MAIN_BASE_REG        (SOC_AIC_SC_REG_BASE + 0xA0)  /* CCU (SPR) */
#define SOC_AIC_SC_REG_DATA_UB_BASE_REG          (SOC_AIC_SC_REG_BASE + 0xA8)  /* CCU (SPR) */
#define SOC_AIC_SC_REG_DATA_SIZE_REG             (SOC_AIC_SC_REG_BASE + 0xB0)  /* CCU (SPR) */
#define SOC_AIC_SC_REG_L2_REMAP_CFG0_REG         (SOC_AIC_SC_REG_BASE + 0xB8)  /* BIU */
#define SOC_AIC_SC_REG_L2_REMAP_CFG1_REG         (SOC_AIC_SC_REG_BASE + 0xC0)  /* BIU */
#define SOC_AIC_SC_REG_L2_REMAP_CFG2_REG         (SOC_AIC_SC_REG_BASE + 0xC8)  /* BIU */
#define SOC_AIC_SC_REG_L2_REMAP_CFG3_REG         (SOC_AIC_SC_REG_BASE + 0xD0)  /* BIU */
#define SOC_AIC_SC_REG_L2_REMAP_CFG4_REG         (SOC_AIC_SC_REG_BASE + 0xD8)  /* BIU */
#define SOC_AIC_SC_REG_L2_REMAP_CFG5_REG         (SOC_AIC_SC_REG_BASE + 0xE0)  /* BIU */
#define SOC_AIC_SC_REG_L2_REMAP_CFG6_REG         (SOC_AIC_SC_REG_BASE + 0xE8)  /* BIU */
#define SOC_AIC_SC_REG_L2_REMAP_CFG7_REG         (SOC_AIC_SC_REG_BASE + 0xF0)  /* BIU */
#define SOC_AIC_SC_REG_L2_VADDR_BASE_REG         (SOC_AIC_SC_REG_BASE + 0xF8)  /* BIU, CCU(SPR) */
#define SOC_AIC_SC_REG_DBG_RW_CTRL_REG           (SOC_AIC_SC_REG_BASE + 0x100) /* DBG */
#define SOC_AIC_SC_REG_DBG_ADDR_REG              (SOC_AIC_SC_REG_BASE + 0x108) /* DBG */
#define SOC_AIC_SC_REG_DBG_DATA0_REG             (SOC_AIC_SC_REG_BASE + 0x110) /* DBG */
#define SOC_AIC_SC_REG_DBG_DATA1_REG             (SOC_AIC_SC_REG_BASE + 0x118) /* DBG */
#define SOC_AIC_SC_REG_DBG_DATA2_REG             (SOC_AIC_SC_REG_BASE + 0x120) /* DBG */
#define SOC_AIC_SC_REG_DBG_DATA3_REG             (SOC_AIC_SC_REG_BASE + 0x128) /* DBG */
#define SOC_AIC_SC_REG_DFX_DATA_REG              (SOC_AIC_SC_REG_BASE + 0x130) /* DBG */
#define SOC_AIC_SC_REG_DBG_STATUS_REG            (SOC_AIC_SC_REG_BASE + 0x138) /* DBG */
#define SOC_AIC_SC_REG_RESERVED_REG00_REG        (SOC_AIC_SC_REG_BASE + 0x140) /* ±£Áô¼Ä´æÆ÷ */
#define SOC_AIC_SC_REG_RESERVED_REG01_REG        (SOC_AIC_SC_REG_BASE + 0x148) /* ±£Áô¼Ä´æÆ÷ */
#define SOC_AIC_SC_REG_PMU_CTRL_REG              (SOC_AIC_SC_REG_BASE + 0x200) /* PMU */
#define SOC_AIC_SC_REG_PMU_CNT0_REG              (SOC_AIC_SC_REG_BASE + 0x210) /* PMU */
#define SOC_AIC_SC_REG_PMU_CNT1_REG              (SOC_AIC_SC_REG_BASE + 0x218) /* PMU */
#define SOC_AIC_SC_REG_PMU_CNT2_REG              (SOC_AIC_SC_REG_BASE + 0x220) /* PMU */
#define SOC_AIC_SC_REG_PMU_CNT3_REG              (SOC_AIC_SC_REG_BASE + 0x228) /* PMU */
#define SOC_AIC_SC_REG_PMU_CNT4_REG              (SOC_AIC_SC_REG_BASE + 0x230) /* PMU */
#define SOC_AIC_SC_REG_PMU_CNT5_REG              (SOC_AIC_SC_REG_BASE + 0x238) /* PMU */
#define SOC_AIC_SC_REG_PMU_CNT6_REG              (SOC_AIC_SC_REG_BASE + 0x240) /* PMU */
#define SOC_AIC_SC_REG_PMU_CNT7_REG              (SOC_AIC_SC_REG_BASE + 0x248) /* PMU */
#define SOC_AIC_SC_REG_PMU_TASK_CYC_CNT_REG      (SOC_AIC_SC_REG_BASE + 0x250) /* PMU */
#define SOC_AIC_SC_REG_PMU_MIN_OV_CNT_REG        (SOC_AIC_SC_REG_BASE + 0x258) /* PMU */
#define SOC_AIC_SC_REG_PMU_CNT0_IDX_REG          (SOC_AIC_SC_REG_BASE + 0x260) /* PMU */
#define SOC_AIC_SC_REG_PMU_CNT1_IDX_REG          (SOC_AIC_SC_REG_BASE + 0x268) /* PMU */
#define SOC_AIC_SC_REG_PMU_CNT2_IDX_REG          (SOC_AIC_SC_REG_BASE + 0x270) /* PMU */
#define SOC_AIC_SC_REG_PMU_CNT3_IDX_REG          (SOC_AIC_SC_REG_BASE + 0x278) /* PMU */
#define SOC_AIC_SC_REG_PMU_CNT4_IDX_REG          (SOC_AIC_SC_REG_BASE + 0x280) /* PMU */
#define SOC_AIC_SC_REG_PMU_CNT5_IDX_REG          (SOC_AIC_SC_REG_BASE + 0x288) /* PMU */
#define SOC_AIC_SC_REG_PMU_CNT6_IDX_REG          (SOC_AIC_SC_REG_BASE + 0x290) /* PMU */
#define SOC_AIC_SC_REG_PMU_CNT7_IDX_REG          (SOC_AIC_SC_REG_BASE + 0x298) /* PMU */
#define SOC_AIC_SC_REG_PMU_START_CNT_CYC_REG     (SOC_AIC_SC_REG_BASE + 0x2A0) /* PMU */
#define SOC_AIC_SC_REG_PMU_STOP_CNT_CYC_REG      (SOC_AIC_SC_REG_BASE + 0x2A8) /* PMU */
#define SOC_AIC_SC_REG_IFU_CTRL_REG              (SOC_AIC_SC_REG_BASE + 0x300) /* IFU(DFX setting) */
#define SOC_AIC_SC_REG_IC_INV_CTRL_REG           (SOC_AIC_SC_REG_BASE + 0x308) /* IFU */
#define SOC_AIC_SC_REG_IC_INV_VA_REG             (SOC_AIC_SC_REG_BASE + 0x310) /* IFU */
#define SOC_AIC_SC_REG_IC_INV_STATUS_REG         (SOC_AIC_SC_REG_BASE + 0x318) /* IFU */
#define SOC_AIC_SC_REG_HW_BKPT_EN_REG            (SOC_AIC_SC_REG_BASE + 0x400) /* CCU */
#define SOC_AIC_SC_REG_HW_BKPT0_PC_REG           (SOC_AIC_SC_REG_BASE + 0x408) /* CCU */
#define SOC_AIC_SC_REG_HW_BKPT1_PC_REG           (SOC_AIC_SC_REG_BASE + 0x410) /* CCU */
#define SOC_AIC_SC_REG_HW_BKPT2_PC_REG           (SOC_AIC_SC_REG_BASE + 0x418) /* CCU */
#define SOC_AIC_SC_REG_HW_BKPT3_PC_REG           (SOC_AIC_SC_REG_BASE + 0x420) /* CCU */
#define SOC_AIC_SC_REG_HW_BKPT4_PC_REG           (SOC_AIC_SC_REG_BASE + 0x428) /* CCU */
#define SOC_AIC_SC_REG_HW_BKPT5_PC_REG           (SOC_AIC_SC_REG_BASE + 0x430) /* CCU */
#define SOC_AIC_SC_REG_HW_BKPT6_PC_REG           (SOC_AIC_SC_REG_BASE + 0x438) /* CCU */
#define SOC_AIC_SC_REG_HW_BKPT7_PC_REG           (SOC_AIC_SC_REG_BASE + 0x440) /* CCU */
#define SOC_AIC_SC_REG_HW_BKPT8_PC_REG           (SOC_AIC_SC_REG_BASE + 0x448) /* CCU */
#define SOC_AIC_SC_REG_HW_BKPT9_PC_REG           (SOC_AIC_SC_REG_BASE + 0x450) /* CCU */
#define SOC_AIC_SC_REG_HW_BKPT10_PC_REG          (SOC_AIC_SC_REG_BASE + 0x458) /* CCU */
#define SOC_AIC_SC_REG_HW_BKPT11_PC_REG          (SOC_AIC_SC_REG_BASE + 0x460) /* CCU */
#define SOC_AIC_SC_REG_HW_BKPT12_PC_REG          (SOC_AIC_SC_REG_BASE + 0x468) /* CCU */
#define SOC_AIC_SC_REG_HW_BKPT13_PC_REG          (SOC_AIC_SC_REG_BASE + 0x470) /* CCU */
#define SOC_AIC_SC_REG_HW_BKPT14_PC_REG          (SOC_AIC_SC_REG_BASE + 0x478) /* CCU */
#define SOC_AIC_SC_REG_HW_BKPT15_PC_REG          (SOC_AIC_SC_REG_BASE + 0x480) /* CCU */
#define SOC_AIC_SC_REG_CCU_IQ_TIMEOUT_REG        (SOC_AIC_SC_REG_BASE + 0x488) /* CCU(DFX setting) */
#define SOC_AIC_SC_REG_CCU_CTRL_REG              (SOC_AIC_SC_REG_BASE + 0x490) /* CCU (DFX setting) */
#define SOC_AIC_SC_REG_SPR_STATUS_REG            (SOC_AIC_SC_REG_BASE + 0x4C0) /* CCU */
#define SOC_AIC_SC_REG_CORE_ID_REG               (SOC_AIC_SC_REG_BASE + 0x600) /* TOPLEVEL */
#define SOC_AIC_SC_REG_AIC_VER_REG               (SOC_AIC_SC_REG_BASE + 0x608) /* TOPLEVEL */
#define SOC_AIC_SC_REG_SMMU_VER_REG              (SOC_AIC_SC_REG_BASE + 0x610) /* TOPLEVEL */
#define SOC_AIC_SC_REG_DISPATCH_VER_REG          (SOC_AIC_SC_REG_BASE + 0x618) /* TOPLEVEL */
#define SOC_AIC_SC_REG_AA_VER_REG                (SOC_AIC_SC_REG_BASE + 0x620) /* TOPLEVEL */
#define SOC_AIC_SC_REG_CRG_VER_REG               (SOC_AIC_SC_REG_BASE + 0x628) /* TOPLEVEL */
#define SOC_AIC_SC_REG_POWER_MODE_CTRL_REG       (SOC_AIC_SC_REG_BASE + 0x630) /* TOPLEVEL */
#define SOC_AIC_SC_REG_MEM_TMOD_CTRL_REG         (SOC_AIC_SC_REG_BASE + 0x638) /* TOPLEVEL */
#define SOC_AIC_SC_REG_AIC_ERROR_REG             (SOC_AIC_SC_REG_BASE + 0x700) /* ALL */
#define SOC_AIC_SC_REG_AIC_ERROR_MASK_REG        (SOC_AIC_SC_REG_BASE + 0x708) /* ALL */
#define SOC_AIC_SC_REG_BIU_ERR_INFO_REG          (SOC_AIC_SC_REG_BASE + 0x710) /* BIU */
#define SOC_AIC_SC_REG_CCU_ERR_INFO_REG          (SOC_AIC_SC_REG_BASE + 0x718) /* CCU */
#define SOC_AIC_SC_REG_CUBE_ERR_INFO_REG         (SOC_AIC_SC_REG_BASE + 0x720) /* CUBE */
#define SOC_AIC_SC_REG_IFU_ERR_INFO_REG          (SOC_AIC_SC_REG_BASE + 0x728) /* IFU */
#define SOC_AIC_SC_REG_MTE_ERR_INFO_REG          (SOC_AIC_SC_REG_BASE + 0x730) /* MTE */
#define SOC_AIC_SC_REG_VEC_ERR_INFO_REG          (SOC_AIC_SC_REG_BASE + 0x738) /* CCU */
#define SOC_AIC_SC_REG_MTE_ECC_1BIT_ERR_REG      (SOC_AIC_SC_REG_BASE + 0x740) /* MTE */
#define SOC_AIC_SC_REG_VEC_CUBE_ECC_1BIT_ERR_REG (SOC_AIC_SC_REG_BASE + 0x748) /* VEC, CUBE */
#define SOC_AIC_SC_REG_AIC_ERROR_FORCE_REG       (SOC_AIC_SC_REG_BASE + 0x750) /* ALL */
#define SOC_AIC_SC_REG_AIC_ERROR_FORCE2_REG       (SOC_AIC_SC_REG_BASE + 0x758) /* ALL */
#define SOC_AIC_SC_REG_AIC_ERROR2_REG             (SOC_AIC_SC_REG_BASE + 0x760) /* ALL */
#define SOC_AIC_SC_REG_AIC_ERROR_MASK2_REG        (SOC_AIC_SC_REG_BASE + 0x768) /* ALL */
#define SOC_AIC_SC_REG_PCT_CTRL_REG              (SOC_AIC_SC_REG_BASE + 0x800) /* PCT */
#define SOC_AIC_SC_REG_PCT_STATUS_REG            (SOC_AIC_SC_REG_BASE + 0x808) /* PCT */
#define SOC_AIC_SC_REG_PCT_NUM_ENTRIES_REG       (SOC_AIC_SC_REG_BASE + 0x810) /* PCT */
#define SOC_AIC_SC_REG_PCT_START_CNT_CYC_REG     (SOC_AIC_SC_REG_BASE + 0x820) /* PCT */
#define SOC_AIC_SC_REG_PCT_STOP_CNT_CYC_REG      (SOC_AIC_SC_REG_BASE + 0x828) /* PCT */
#define SOC_AIC_SC_REG_PCT_OV_TIMESTAMP_REG      (SOC_AIC_SC_REG_BASE + 0x830) /* PCT */
#define SOC_AIC_SC_REG_VEC_CTRL_REG              (SOC_AIC_SC_REG_BASE + 0x900) 
#define SOC_AIC_SC_REG_VEC_RESERVED_REG00_REG    (SOC_AIC_SC_REG_BASE + 0xA00) /* ±£Áô¼Ä´æÆ÷ */
#define SOC_AIC_SC_REG_VEC_RESERVED_REG01_REG    (SOC_AIC_SC_REG_BASE + 0xA08) /* ±£Áô¼Ä´æÆ÷ */
#define SOC_AIC_SC_REG_CUBE_RESERVED_REG00_REG   (SOC_AIC_SC_REG_BASE + 0xA10) /* ±£Áô¼Ä´æÆ÷ */
#define SOC_AIC_SC_REG_CUBE_RESERVED_REG01_REG   (SOC_AIC_SC_REG_BASE + 0xA18) /* ±£Áô¼Ä´æÆ÷ */
#define SOC_AIC_SC_REG_SC_RESERVED_REG00_REG     (SOC_AIC_SC_REG_BASE + 0xA20) /* ±£Áô¼Ä´æÆ÷ */
#define SOC_AIC_SC_REG_SC_RESERVED_REG01_REG     (SOC_AIC_SC_REG_BASE + 0xA28) /* ±£Áô¼Ä´æÆ÷ */
#define SOC_AIC_SC_REG_SC_RESERVED_RO_REG00_REG   (SOC_AIC_SC_REG_BASE + 0xA30) /* ±£Áô¼Ä´æÆ÷ */
#define SOC_AIC_SC_REG_SC_RESERVED_RO_REG01_REG   (SOC_AIC_SC_REG_BASE + 0xA38) /* ±£Áô¼Ä´æÆ÷ */
#define SOC_AIC_SC_REG_SC_RESERVED_REG02_REG      (SOC_AIC_SC_REG_BASE + 0xA40) /* ±£Áô¼Ä´æÆ÷ */
#define SOC_AIC_SC_REG_SC_RESERVED_REG03_REG      (SOC_AIC_SC_REG_BASE + 0xA48) /* ±£Áô¼Ä´æÆ÷ */
#define SOC_AIC_SC_REG_VEC_RESERVED_RO_REG00_REG  (SOC_AIC_SC_REG_BASE + 0xA50) /* ±£Áô¼Ä´æÆ÷ */
#define SOC_AIC_SC_REG_VEC_RESERVED_RO_REG01_REG  (SOC_AIC_SC_REG_BASE + 0xA58) /* ±£Áô¼Ä´æÆ÷ */
#define SOC_AIC_SC_REG_CUBE_RESERVED_RO_REG00_REG (SOC_AIC_SC_REG_BASE + 0xA60) /* ±£Áô¼Ä´æÆ÷ */
#define SOC_AIC_SC_REG_CUBE_RESERVED_RO_REG01_REG (SOC_AIC_SC_REG_BASE + 0xA68) /* ±£Áô¼Ä´æÆ÷ */
#define SOC_AIC_SC_REG_MTE_CTRL_REG              (SOC_AIC_SC_REG_BASE + 0xB00) 
#define SOC_AIC_SC_REG_CUBE_CTRL_REG             (SOC_AIC_SC_REG_BASE + 0xC00) 

#endif // __AIC_SC_REG_REG_OFFSET_H__
