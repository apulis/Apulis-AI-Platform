// ****************************************************************************** 
// Copyright     :  Copyright (C) 2018, Hisilicon Technologies Co. Ltd.
// File name     :  hipciec_nvme_global_reg_c_union_define.h
// Department    :  CAD Development Department
// Author        :  xxx
// Version       :  1.0
// Date          :  2017/10/24
// Description   :  The description of xxx project
// Others        :  Generated automatically by nManager V4.2 
// History       :  xxx 2018/07/13 11:19:10 Create file
// ******************************************************************************

#ifndef __HIPCIEC_NVME_GLOBAL_REG_C_UNION_DEFINE_H__
#define __HIPCIEC_NVME_GLOBAL_REG_C_UNION_DEFINE_H__

/* Define the union U_PF_WR_CTRL_REG_INT_STS */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    rsv_0           : 24  ; /* [31:8] */
        unsigned int    wr_cc_int_mask  : 1  ; /* [7] */
        unsigned int    wr_aqa_int_mask : 1  ; /* [6] */
        unsigned int    wr_asq_int_mask : 1  ; /* [5] */
        unsigned int    rsv_1           : 1  ; /* [4] */
        unsigned int    wr_cc_int_sts   : 1  ; /* [3] */
        unsigned int    wr_aqa_int_sts  : 1  ; /* [2] */
        unsigned int    wr_asq_int_sts  : 1  ; /* [1] */
        unsigned int    rsv_2           : 1  ; /* [0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_PF_WR_CTRL_REG_INT_STS;

/* Define the union U_VF_WR_CTRL_REG_INT_STS */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    rsv_3           : 24  ; /* [31:8] */
        unsigned int    wr_cc_int_mask  : 1  ; /* [7] */
        unsigned int    wr_aqa_int_mask : 1  ; /* [6] */
        unsigned int    wr_asq_int_mask : 1  ; /* [5] */
        unsigned int    rsv_4           : 1  ; /* [4] */
        unsigned int    wr_cc_int_sts   : 1  ; /* [3] */
        unsigned int    wr_aqa_int_sts  : 1  ; /* [2] */
        unsigned int    wr_asq_int_sts  : 1  ; /* [1] */
        unsigned int    rsv_5           : 1  ; /* [0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VF_WR_CTRL_REG_INT_STS;

/* Define the union U_NVME_CTRL_MISC */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    rsv_6       : 31  ; /* [31:1] */
        unsigned int    int_coal_en : 1  ; /* [0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_NVME_CTRL_MISC;

/* Define the union U_SRIOV_MODE_CTRL */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    rsv_7          : 29  ; /* [31:3] */
        unsigned int    sriov_mode_sel : 3  ; /* [2:0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_SRIOV_MODE_CTRL;

/* Define the union U_PF2_BAR0_DB_ATU_LOW */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    pf2_bar0_db_atu_low : 32  ; /* [31:0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_PF2_BAR0_DB_ATU_LOW;

/* Define the union U_PF2_BAR0_DB_ATU_HIGH */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    pf2_bar0_db_atu_high : 32  ; /* [31:0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_PF2_BAR0_DB_ATU_HIGH;

/* Define the union U_PF2_VF_BAR0_DB_ATU_LOW */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    pf2_vf_bar0_db_atu_low : 32  ; /* [31:0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_PF2_VF_BAR0_DB_ATU_LOW;

/* Define the union U_PF2_VF_BAR0_DB_ATU_HIGH */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    pf2_vf_bar0_db_atu_high : 32  ; /* [31:0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_PF2_VF_BAR0_DB_ATU_HIGH;

/* Define the union U_PF3_BAR0_DB_ATU_LOW */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    pf3_bar0_db_atu_low : 32  ; /* [31:0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_PF3_BAR0_DB_ATU_LOW;

/* Define the union U_PF3_BAR0_DB_ATU_HIGH */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    pf3_bar0_db_atu_high : 32  ; /* [31:0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_PF3_BAR0_DB_ATU_HIGH;

/* Define the union U_PF3_VF_BAR0_DB_ATU_LOW */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    pf3_vf_bar0_db_atu_low : 32  ; /* [31:0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_PF3_VF_BAR0_DB_ATU_LOW;

/* Define the union U_PF3_VF_BAR0_DB_ATU_HIGH */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    pf3_vf_bar0_db_atu_high : 32  ; /* [31:0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_PF3_VF_BAR0_DB_ATU_HIGH;

/* Define the union U_INT_COAL_TIME_CNT_UNIT */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    rsv_8         : 16  ; /* [31:16] */
        unsigned int    time_cnt_unit : 16  ; /* [15:0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_INT_COAL_TIME_CNT_UNIT;

/* Define the union U_INT_COAL_MASK0 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    rsv_9          : 16  ; /* [31:16] */
        unsigned int    int_coal_mask0 : 16  ; /* [15:0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_INT_COAL_MASK0;

/* Define the union U_INT_COAL_MASK1 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    rsv_10         : 16  ; /* [31:16] */
        unsigned int    int_coal_mask1 : 16  ; /* [15:0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_INT_COAL_MASK1;

/* Define the union U_INT_COAL_MASK2 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    rsv_11         : 16  ; /* [31:16] */
        unsigned int    int_coal_mask2 : 16  ; /* [15:0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_INT_COAL_MASK2;

/* Define the union U_INT_COAL_MASK3 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    rsv_12         : 16  ; /* [31:16] */
        unsigned int    int_coal_mask3 : 16  ; /* [15:0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_INT_COAL_MASK3;

/* Define the union U_INT_COAL_MASK4 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    rsv_13         : 16  ; /* [31:16] */
        unsigned int    int_coal_mask4 : 16  ; /* [15:0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_INT_COAL_MASK4;

/* Define the union U_INT_COAL_MASK5 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    rsv_14         : 16  ; /* [31:16] */
        unsigned int    int_coal_mask5 : 16  ; /* [15:0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_INT_COAL_MASK5;

/* Define the union U_INT_COAL_MASK6 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    rsv_15         : 16  ; /* [31:16] */
        unsigned int    int_coal_mask6 : 16  ; /* [15:0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_INT_COAL_MASK6;

/* Define the union U_INT_COAL_MASK7 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    rsv_16         : 16  ; /* [31:16] */
        unsigned int    int_coal_mask7 : 16  ; /* [15:0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_INT_COAL_MASK7;

/* Define the union U_INT_COAL_MASK8 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    rsv_17         : 16  ; /* [31:16] */
        unsigned int    int_coal_mask8 : 16  ; /* [15:0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_INT_COAL_MASK8;

/* Define the union U_INT_COAL_MASK9 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    rsv_18         : 16  ; /* [31:16] */
        unsigned int    int_coal_mask9 : 16  ; /* [15:0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_INT_COAL_MASK9;

/* Define the union U_INT_COAL_MASK10 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    rsv_19          : 16  ; /* [31:16] */
        unsigned int    int_coal_mask10 : 16  ; /* [15:0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_INT_COAL_MASK10;

/* Define the union U_INT_COAL_MASK11 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    rsv_20          : 16  ; /* [31:16] */
        unsigned int    int_coal_mask11 : 16  ; /* [15:0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_INT_COAL_MASK11;

/* Define the union U_INT_COAL_MASK12 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    rsv_21          : 16  ; /* [31:16] */
        unsigned int    int_coal_mask12 : 16  ; /* [15:0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_INT_COAL_MASK12;

/* Define the union U_INT_COAL_MASK13 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    rsv_22          : 16  ; /* [31:16] */
        unsigned int    int_coal_mask13 : 16  ; /* [15:0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_INT_COAL_MASK13;

/* Define the union U_INT_COAL_MASK14 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    rsv_23          : 16  ; /* [31:16] */
        unsigned int    int_coal_mask14 : 16  ; /* [15:0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_INT_COAL_MASK14;

/* Define the union U_INT_COAL_MASK15 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    rsv_24          : 16  ; /* [31:16] */
        unsigned int    int_coal_mask15 : 16  ; /* [15:0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_INT_COAL_MASK15;

/* Define the union U_CQDB_INT_MASK0 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    rsv_25            : 16  ; /* [31:16] */
        unsigned int    cq_doorbell_mask0 : 16  ; /* [15:0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_CQDB_INT_MASK0;

/* Define the union U_CQDB_INT_MASK1 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    rsv_26            : 16  ; /* [31:16] */
        unsigned int    cq_doorbell_mask1 : 16  ; /* [15:0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_CQDB_INT_MASK1;

/* Define the union U_CQDB_INT_MASK2 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    rsv_27            : 16  ; /* [31:16] */
        unsigned int    cq_doorbell_mask2 : 16  ; /* [15:0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_CQDB_INT_MASK2;

/* Define the union U_CQDB_INT_MASK3 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    rsv_28            : 16  ; /* [31:16] */
        unsigned int    cq_doorbell_mask3 : 16  ; /* [15:0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_CQDB_INT_MASK3;

/* Define the union U_CQDB_INT_MASK4 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    rsv_29            : 16  ; /* [31:16] */
        unsigned int    cq_doorbell_mask4 : 16  ; /* [15:0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_CQDB_INT_MASK4;

/* Define the union U_CQDB_INT_MASK5 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    rsv_30            : 16  ; /* [31:16] */
        unsigned int    cq_doorbell_mask5 : 16  ; /* [15:0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_CQDB_INT_MASK5;

/* Define the union U_CQDB_INT_MASK6 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    rsv_31            : 16  ; /* [31:16] */
        unsigned int    cq_doorbell_mask6 : 16  ; /* [15:0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_CQDB_INT_MASK6;

/* Define the union U_CQDB_INT_MASK7 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    rsv_32            : 16  ; /* [31:16] */
        unsigned int    cq_doorbell_mask7 : 16  ; /* [15:0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_CQDB_INT_MASK7;

/* Define the union U_CQDB_INT_MASK8 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    rsv_33            : 16  ; /* [31:16] */
        unsigned int    cq_doorbell_mask8 : 16  ; /* [15:0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_CQDB_INT_MASK8;

/* Define the union U_CQDB_INT_MASK9 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    rsv_34            : 16  ; /* [31:16] */
        unsigned int    cq_doorbell_mask9 : 16  ; /* [15:0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_CQDB_INT_MASK9;

/* Define the union U_CQDB_INT_MASK10 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    rsv_35             : 16  ; /* [31:16] */
        unsigned int    cq_doorbell_mask10 : 16  ; /* [15:0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_CQDB_INT_MASK10;

/* Define the union U_CQDB_INT_MASK11 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    rsv_36             : 16  ; /* [31:16] */
        unsigned int    cq_doorbell_mask11 : 16  ; /* [15:0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_CQDB_INT_MASK11;

/* Define the union U_CQDB_INT_MASK12 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    rsv_37             : 16  ; /* [31:16] */
        unsigned int    cq_doorbell_mask12 : 16  ; /* [15:0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_CQDB_INT_MASK12;

/* Define the union U_CQDB_INT_MASK13 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    rsv_38             : 16  ; /* [31:16] */
        unsigned int    cq_doorbell_mask13 : 16  ; /* [15:0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_CQDB_INT_MASK13;

/* Define the union U_CQDB_INT_MASK14 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    rsv_39             : 16  ; /* [31:16] */
        unsigned int    cq_doorbell_mask14 : 16  ; /* [15:0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_CQDB_INT_MASK14;

/* Define the union U_CQDB_INT_MASK15 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    rsv_40             : 16  ; /* [31:16] */
        unsigned int    cq_doorbell_mask15 : 16  ; /* [15:0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_CQDB_INT_MASK15;

/* Define the union U_WRPTR_INT_MASK0 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    rsv_41      : 16  ; /* [31:16] */
        unsigned int    wrptr_mask0 : 16  ; /* [15:0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_WRPTR_INT_MASK0;

/* Define the union U_WRPTR_INT_MASK1 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    rsv_42      : 16  ; /* [31:16] */
        unsigned int    wrptr_mask1 : 16  ; /* [15:0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_WRPTR_INT_MASK1;

/* Define the union U_WRPTR_INT_MASK2 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    rsv_43      : 16  ; /* [31:16] */
        unsigned int    wrptr_mask2 : 16  ; /* [15:0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_WRPTR_INT_MASK2;

/* Define the union U_WRPTR_INT_MASK3 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    rsv_44      : 16  ; /* [31:16] */
        unsigned int    wrptr_mask3 : 16  ; /* [15:0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_WRPTR_INT_MASK3;

/* Define the union U_WRPTR_INT_MASK4 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    rsv_45      : 16  ; /* [31:16] */
        unsigned int    wrptr_mask4 : 16  ; /* [15:0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_WRPTR_INT_MASK4;

/* Define the union U_WRPTR_INT_MASK5 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    rsv_46      : 16  ; /* [31:16] */
        unsigned int    wrptr_mask5 : 16  ; /* [15:0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_WRPTR_INT_MASK5;

/* Define the union U_WRPTR_INT_MASK6 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    rsv_47      : 16  ; /* [31:16] */
        unsigned int    wrptr_mask6 : 16  ; /* [15:0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_WRPTR_INT_MASK6;

/* Define the union U_WRPTR_INT_MASK7 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    rsv_48      : 16  ; /* [31:16] */
        unsigned int    wrptr_mask7 : 16  ; /* [15:0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_WRPTR_INT_MASK7;

/* Define the union U_DB_INT_COAL_CFG */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    pf3_cfg_int_aggre_time : 8  ; /* [31:24] */
        unsigned int    pf3_cfg_int_aggre_thr  : 8  ; /* [23:16] */
        unsigned int    pf2_cfg_int_aggre_time : 8  ; /* [15:8] */
        unsigned int    pf2_cfg_int_aggre_thr  : 8  ; /* [7:0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_DB_INT_COAL_CFG;

/* Define the union U_ENG_WORK_MODE_SEL */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    rsv_49            : 31  ; /* [31:1] */
        unsigned int    eng_work_mode_sel : 1  ; /* [0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ENG_WORK_MODE_SEL;

/* Define the union U_WRPTR_INT_MASK8 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    rsv_50      : 16  ; /* [31:16] */
        unsigned int    wrptr_mask8 : 16  ; /* [15:0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_WRPTR_INT_MASK8;

/* Define the union U_WRPTR_INT_MASK9 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    rsv_51      : 16  ; /* [31:16] */
        unsigned int    wrptr_mask9 : 16  ; /* [15:0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_WRPTR_INT_MASK9;

/* Define the union U_WRPTR_INT_MASK10 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    rsv_52       : 16  ; /* [31:16] */
        unsigned int    wrptr_mask10 : 16  ; /* [15:0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_WRPTR_INT_MASK10;

/* Define the union U_WRPTR_INT_MASK11 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    rsv_53       : 16  ; /* [31:16] */
        unsigned int    wrptr_mask11 : 16  ; /* [15:0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_WRPTR_INT_MASK11;

/* Define the union U_WRPTR_INT_MASK12 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    rsv_54       : 16  ; /* [31:16] */
        unsigned int    wrptr_mask12 : 16  ; /* [15:0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_WRPTR_INT_MASK12;

/* Define the union U_WRPTR_INT_MASK13 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    rsv_55       : 16  ; /* [31:16] */
        unsigned int    wrptr_mask13 : 16  ; /* [15:0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_WRPTR_INT_MASK13;

/* Define the union U_WRPTR_INT_MASK14 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    rsv_56       : 16  ; /* [31:16] */
        unsigned int    wrptr_mask14 : 16  ; /* [15:0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_WRPTR_INT_MASK14;

/* Define the union U_WRPTR_INT_MASK15 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    rsv_57       : 16  ; /* [31:16] */
        unsigned int    wrptr_mask15 : 16  ; /* [15:0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_WRPTR_INT_MASK15;

/* Define the union U_SQDB_INT_STS0 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    rsv_58        : 16  ; /* [31:16] */
        unsigned int    sqdb_int_sts0 : 16  ; /* [15:0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_SQDB_INT_STS0;

/* Define the union U_SQDB_INT_STS1 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    rsv_59        : 16  ; /* [31:16] */
        unsigned int    sqdb_int_sts1 : 16  ; /* [15:0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_SQDB_INT_STS1;

/* Define the union U_SQDB_INT_STS2 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    rsv_60        : 16  ; /* [31:16] */
        unsigned int    sqdb_int_sts2 : 16  ; /* [15:0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_SQDB_INT_STS2;

/* Define the union U_SQDB_INT_STS3 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    rsv_61        : 16  ; /* [31:16] */
        unsigned int    sqdb_int_sts3 : 16  ; /* [15:0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_SQDB_INT_STS3;

/* Define the union U_SQDB_INT_STS4 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    rsv_62        : 16  ; /* [31:16] */
        unsigned int    sqdb_int_sts4 : 16  ; /* [15:0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_SQDB_INT_STS4;

/* Define the union U_SQDB_INT_STS5 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    rsv_63        : 16  ; /* [31:16] */
        unsigned int    sqdb_int_sts5 : 16  ; /* [15:0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_SQDB_INT_STS5;

/* Define the union U_SQDB_INT_STS6 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    rsv_64        : 16  ; /* [31:16] */
        unsigned int    sqdb_int_sts6 : 16  ; /* [15:0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_SQDB_INT_STS6;

/* Define the union U_SQDB_INT_STS7 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    rsv_65        : 16  ; /* [31:16] */
        unsigned int    sqdb_int_sts7 : 16  ; /* [15:0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_SQDB_INT_STS7;

/* Define the union U_SQDB_INT_STS8 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    rsv_66        : 16  ; /* [31:16] */
        unsigned int    sqdb_int_sts8 : 16  ; /* [15:0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_SQDB_INT_STS8;

/* Define the union U_SQDB_INT_STS9 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    rsv_67        : 16  ; /* [31:16] */
        unsigned int    sqdb_int_sts9 : 16  ; /* [15:0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_SQDB_INT_STS9;

/* Define the union U_SQDB_INT_STS10 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    rsv_68         : 16  ; /* [31:16] */
        unsigned int    sqdb_int_sts10 : 16  ; /* [15:0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_SQDB_INT_STS10;

/* Define the union U_SQDB_INT_STS11 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    rsv_69         : 16  ; /* [31:16] */
        unsigned int    sqdb_int_sts11 : 16  ; /* [15:0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_SQDB_INT_STS11;

/* Define the union U_SQDB_INT_STS12 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    rsv_70         : 16  ; /* [31:16] */
        unsigned int    sqdb_int_sts12 : 16  ; /* [15:0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_SQDB_INT_STS12;

/* Define the union U_SQDB_INT_STS13 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    rsv_71         : 16  ; /* [31:16] */
        unsigned int    sqdb_int_sts13 : 16  ; /* [15:0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_SQDB_INT_STS13;

/* Define the union U_SQDB_INT_STS14 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    rsv_72         : 16  ; /* [31:16] */
        unsigned int    sqdb_int_sts14 : 16  ; /* [15:0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_SQDB_INT_STS14;

/* Define the union U_SQDB_INT_STS15 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    rsv_73         : 16  ; /* [31:16] */
        unsigned int    sqdb_int_sts15 : 16  ; /* [15:0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_SQDB_INT_STS15;

/* Define the union U_CQDB_INT_STS0 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    rsv_74           : 16  ; /* [31:16] */
        unsigned int    cq_doorbell_sts0 : 16  ; /* [15:0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_CQDB_INT_STS0;

/* Define the union U_CQDB_INT_STS1 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    rsv_75           : 16  ; /* [31:16] */
        unsigned int    cq_doorbell_sts1 : 16  ; /* [15:0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_CQDB_INT_STS1;

/* Define the union U_CQDB_INT_STS2 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    rsv_76           : 16  ; /* [31:16] */
        unsigned int    cq_doorbell_sts2 : 16  ; /* [15:0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_CQDB_INT_STS2;

/* Define the union U_CQDB_INT_STS3 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    rsv_77           : 16  ; /* [31:16] */
        unsigned int    cq_doorbell_sts3 : 16  ; /* [15:0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_CQDB_INT_STS3;

/* Define the union U_CQDB_INT_STS4 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    rsv_78           : 16  ; /* [31:16] */
        unsigned int    cq_doorbell_sts4 : 16  ; /* [15:0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_CQDB_INT_STS4;

/* Define the union U_CQDB_INT_STS5 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    rsv_79           : 16  ; /* [31:16] */
        unsigned int    cq_doorbell_sts5 : 16  ; /* [15:0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_CQDB_INT_STS5;

/* Define the union U_CQDB_INT_STS6 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    rsv_80           : 16  ; /* [31:16] */
        unsigned int    cq_doorbell_sts6 : 16  ; /* [15:0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_CQDB_INT_STS6;

/* Define the union U_CQDB_INT_STS7 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    rsv_81           : 16  ; /* [31:16] */
        unsigned int    cq_doorbell_sts7 : 16  ; /* [15:0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_CQDB_INT_STS7;

/* Define the union U_CQDB_INT_STS8 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    rsv_82           : 16  ; /* [31:16] */
        unsigned int    cq_doorbell_sts8 : 16  ; /* [15:0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_CQDB_INT_STS8;

/* Define the union U_CQDB_INT_STS9 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    rsv_83           : 16  ; /* [31:16] */
        unsigned int    cq_doorbell_sts9 : 16  ; /* [15:0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_CQDB_INT_STS9;

/* Define the union U_CQDB_INT_STS10 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    rsv_84            : 16  ; /* [31:16] */
        unsigned int    cq_doorbell_sts10 : 16  ; /* [15:0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_CQDB_INT_STS10;

/* Define the union U_CQDB_INT_STS11 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    rsv_85            : 16  ; /* [31:16] */
        unsigned int    cq_doorbell_sts11 : 16  ; /* [15:0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_CQDB_INT_STS11;

/* Define the union U_CQDB_INT_STS12 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    rsv_86            : 16  ; /* [31:16] */
        unsigned int    cq_doorbell_sts12 : 16  ; /* [15:0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_CQDB_INT_STS12;

/* Define the union U_CQDB_INT_STS13 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    rsv_87            : 16  ; /* [31:16] */
        unsigned int    cq_doorbell_sts13 : 16  ; /* [15:0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_CQDB_INT_STS13;

/* Define the union U_CQDB_INT_STS14 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    rsv_88            : 16  ; /* [31:16] */
        unsigned int    cq_doorbell_sts14 : 16  ; /* [15:0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_CQDB_INT_STS14;

/* Define the union U_CQDB_INT_STS15 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    rsv_89            : 16  ; /* [31:16] */
        unsigned int    cq_doorbell_sts15 : 16  ; /* [15:0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_CQDB_INT_STS15;

/* Define the union U_WRPTR_INT_STS0 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    rsv_90     : 16  ; /* [31:16] */
        unsigned int    wrptr_sts0 : 16  ; /* [15:0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_WRPTR_INT_STS0;

/* Define the union U_WRPTR_INT_STS1 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    rsv_91     : 16  ; /* [31:16] */
        unsigned int    wrptr_sts1 : 16  ; /* [15:0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_WRPTR_INT_STS1;

/* Define the union U_WRPTR_INT_STS2 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    rsv_92     : 16  ; /* [31:16] */
        unsigned int    wrptr_sts2 : 16  ; /* [15:0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_WRPTR_INT_STS2;

/* Define the union U_WRPTR_INT_STS3 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    rsv_93     : 16  ; /* [31:16] */
        unsigned int    wrptr_sts3 : 16  ; /* [15:0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_WRPTR_INT_STS3;

/* Define the union U_WRPTR_INT_STS4 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    rsv_94     : 16  ; /* [31:16] */
        unsigned int    wrptr_sts4 : 16  ; /* [15:0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_WRPTR_INT_STS4;

/* Define the union U_WRPTR_INT_STS5 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    rsv_95     : 16  ; /* [31:16] */
        unsigned int    wrptr_sts5 : 16  ; /* [15:0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_WRPTR_INT_STS5;

/* Define the union U_WRPTR_INT_STS6 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    rsv_96     : 16  ; /* [31:16] */
        unsigned int    wrptr_sts6 : 16  ; /* [15:0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_WRPTR_INT_STS6;

/* Define the union U_WRPTR_INT_STS7 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    rsv_97     : 16  ; /* [31:16] */
        unsigned int    wrptr_sts7 : 16  ; /* [15:0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_WRPTR_INT_STS7;

/* Define the union U_WRPTR_INT_STS8 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    rsv_98     : 16  ; /* [31:16] */
        unsigned int    wrptr_sts8 : 16  ; /* [15:0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_WRPTR_INT_STS8;

/* Define the union U_WRPTR_INT_STS9 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    rsv_99     : 16  ; /* [31:16] */
        unsigned int    wrptr_sts9 : 16  ; /* [15:0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_WRPTR_INT_STS9;

/* Define the union U_WRPTR_INT_STS10 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    rsv_100     : 16  ; /* [31:16] */
        unsigned int    wrptr_sts10 : 16  ; /* [15:0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_WRPTR_INT_STS10;

/* Define the union U_WRPTR_INT_STS11 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    rsv_101     : 16  ; /* [31:16] */
        unsigned int    wrptr_sts11 : 16  ; /* [15:0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_WRPTR_INT_STS11;

/* Define the union U_WRPTR_INT_STS12 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    rsv_102     : 16  ; /* [31:16] */
        unsigned int    wrptr_sts12 : 16  ; /* [15:0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_WRPTR_INT_STS12;

/* Define the union U_WRPTR_INT_STS13 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    rsv_103     : 16  ; /* [31:16] */
        unsigned int    wrptr_sts13 : 16  ; /* [15:0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_WRPTR_INT_STS13;

/* Define the union U_WRPTR_INT_STS14 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    rsv_104     : 16  ; /* [31:16] */
        unsigned int    wrptr_sts14 : 16  ; /* [15:0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_WRPTR_INT_STS14;

/* Define the union U_WRPTR_INT_STS15 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    rsv_105     : 16  ; /* [31:16] */
        unsigned int    wrptr_sts15 : 16  ; /* [15:0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_WRPTR_INT_STS15;

/* Define the union U_SQDB_INT_MASK0 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    rsv_106           : 16  ; /* [31:16] */
        unsigned int    sq_doorbell_mask0 : 16  ; /* [15:0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_SQDB_INT_MASK0;

/* Define the union U_SQDB_INT_MASK1 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    rsv_107           : 16  ; /* [31:16] */
        unsigned int    sq_doorbell_mask1 : 16  ; /* [15:0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_SQDB_INT_MASK1;

/* Define the union U_SQDB_INT_MASK2 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    rsv_108           : 16  ; /* [31:16] */
        unsigned int    sq_doorbell_mask2 : 16  ; /* [15:0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_SQDB_INT_MASK2;

/* Define the union U_SQDB_INT_MASK3 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    rsv_109           : 16  ; /* [31:16] */
        unsigned int    sq_doorbell_mask3 : 16  ; /* [15:0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_SQDB_INT_MASK3;

/* Define the union U_SQDB_INT_MASK4 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    rsv_110           : 16  ; /* [31:16] */
        unsigned int    sq_doorbell_mask4 : 16  ; /* [15:0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_SQDB_INT_MASK4;

/* Define the union U_SQDB_INT_MASK5 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    rsv_111           : 16  ; /* [31:16] */
        unsigned int    sq_doorbell_mask5 : 16  ; /* [15:0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_SQDB_INT_MASK5;

/* Define the union U_SQDB_INT_MASK6 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    rsv_112           : 16  ; /* [31:16] */
        unsigned int    sq_doorbell_mask6 : 16  ; /* [15:0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_SQDB_INT_MASK6;

/* Define the union U_SQDB_INT_MASK7 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    rsv_113           : 16  ; /* [31:16] */
        unsigned int    sq_doorbell_mask7 : 16  ; /* [15:0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_SQDB_INT_MASK7;

/* Define the union U_SQDB_INT_MASK8 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    rsv_114           : 16  ; /* [31:16] */
        unsigned int    sq_doorbell_mask8 : 16  ; /* [15:0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_SQDB_INT_MASK8;

/* Define the union U_SQDB_INT_MASK9 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    rsv_115           : 16  ; /* [31:16] */
        unsigned int    sq_doorbell_mask9 : 16  ; /* [15:0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_SQDB_INT_MASK9;

/* Define the union U_SQDB_INT_MASK10 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    rsv_116            : 16  ; /* [31:16] */
        unsigned int    sq_doorbell_mask10 : 16  ; /* [15:0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_SQDB_INT_MASK10;

/* Define the union U_SQDB_INT_MASK11 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    rsv_117            : 16  ; /* [31:16] */
        unsigned int    sq_doorbell_mask11 : 16  ; /* [15:0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_SQDB_INT_MASK11;

/* Define the union U_SQDB_INT_MASK12 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    rsv_118            : 16  ; /* [31:16] */
        unsigned int    sq_doorbell_mask12 : 16  ; /* [15:0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_SQDB_INT_MASK12;

/* Define the union U_SQDB_INT_MASK13 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    rsv_119            : 16  ; /* [31:16] */
        unsigned int    sq_doorbell_mask13 : 16  ; /* [15:0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_SQDB_INT_MASK13;

/* Define the union U_SQDB_INT_MASK14 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    rsv_120            : 16  ; /* [31:16] */
        unsigned int    sq_doorbell_mask14 : 16  ; /* [15:0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_SQDB_INT_MASK14;

/* Define the union U_SQDB_INT_MASK15 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    rsv_121            : 16  ; /* [31:16] */
        unsigned int    sq_doorbell_mask15 : 16  ; /* [15:0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_SQDB_INT_MASK15;

/* Define the union U_SQDB_INT_CFG0 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    sq_doorbell_ro0  : 16  ; /* [31:16] */
        unsigned int    sq_doorbell_set0 : 16  ; /* [15:0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_SQDB_INT_CFG0;

/* Define the union U_SQDB_INT_CFG1 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    sq_doorbell_ro1  : 16  ; /* [31:16] */
        unsigned int    sq_doorbell_set1 : 16  ; /* [15:0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_SQDB_INT_CFG1;

/* Define the union U_SQDB_INT_CFG2 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    sq_doorbell_ro2  : 16  ; /* [31:16] */
        unsigned int    sq_doorbell_set2 : 16  ; /* [15:0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_SQDB_INT_CFG2;

/* Define the union U_SQDB_INT_CFG3 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    sq_doorbell_ro3  : 16  ; /* [31:16] */
        unsigned int    sq_doorbell_set3 : 16  ; /* [15:0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_SQDB_INT_CFG3;

/* Define the union U_SQDB_INT_CFG4 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    sq_doorbell_ro4  : 16  ; /* [31:16] */
        unsigned int    sq_doorbell_set4 : 16  ; /* [15:0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_SQDB_INT_CFG4;

/* Define the union U_SQDB_INT_CFG5 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    sq_doorbell_ro5  : 16  ; /* [31:16] */
        unsigned int    sq_doorbell_set5 : 16  ; /* [15:0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_SQDB_INT_CFG5;

/* Define the union U_SQDB_INT_CFG6 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    sq_doorbell_ro6  : 16  ; /* [31:16] */
        unsigned int    sq_doorbell_set6 : 16  ; /* [15:0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_SQDB_INT_CFG6;

/* Define the union U_SQDB_INT_CFG7 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    sq_doorbell_ro7  : 16  ; /* [31:16] */
        unsigned int    sq_doorbell_set7 : 16  ; /* [15:0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_SQDB_INT_CFG7;

/* Define the union U_SQDB_INT_CFG8 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    sq_doorbell_ro8  : 16  ; /* [31:16] */
        unsigned int    sq_doorbell_set8 : 16  ; /* [15:0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_SQDB_INT_CFG8;

/* Define the union U_SQDB_INT_CFG9 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    sq_doorbell_ro9  : 16  ; /* [31:16] */
        unsigned int    sq_doorbell_set9 : 16  ; /* [15:0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_SQDB_INT_CFG9;

/* Define the union U_SQDB_INT_CFG10 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    sq_doorbell_ro10  : 16  ; /* [31:16] */
        unsigned int    sq_doorbell_set10 : 16  ; /* [15:0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_SQDB_INT_CFG10;

/* Define the union U_SQDB_INT_CFG11 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    sq_doorbell_ro11  : 16  ; /* [31:16] */
        unsigned int    sq_doorbell_set11 : 16  ; /* [15:0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_SQDB_INT_CFG11;

/* Define the union U_SQDB_INT_CFG12 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    sq_doorbell_ro12  : 16  ; /* [31:16] */
        unsigned int    sq_doorbell_set12 : 16  ; /* [15:0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_SQDB_INT_CFG12;

/* Define the union U_SQDB_INT_CFG13 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    sq_doorbell_ro13  : 16  ; /* [31:16] */
        unsigned int    sq_doorbell_set13 : 16  ; /* [15:0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_SQDB_INT_CFG13;

/* Define the union U_SQDB_INT_CFG14 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    sq_doorbell_ro14  : 16  ; /* [31:16] */
        unsigned int    sq_doorbell_set14 : 16  ; /* [15:0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_SQDB_INT_CFG14;

/* Define the union U_SQDB_INT_CFG15 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    sq_doorbell_ro15  : 16  ; /* [31:16] */
        unsigned int    sq_doorbell_set15 : 16  ; /* [15:0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_SQDB_INT_CFG15;

/* Define the union U_CQDB_INT_CFG0 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    cq_doorbell_ro0  : 16  ; /* [31:16] */
        unsigned int    cq_doorbell_set0 : 16  ; /* [15:0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_CQDB_INT_CFG0;

/* Define the union U_CQDB_INT_CFG1 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    cq_doorbell_ro1  : 16  ; /* [31:16] */
        unsigned int    cq_doorbell_set1 : 16  ; /* [15:0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_CQDB_INT_CFG1;

/* Define the union U_CQDB_INT_CFG2 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    cq_doorbell_ro2  : 16  ; /* [31:16] */
        unsigned int    cq_doorbell_set2 : 16  ; /* [15:0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_CQDB_INT_CFG2;

/* Define the union U_CQDB_INT_CFG3 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    cq_doorbell_ro3  : 16  ; /* [31:16] */
        unsigned int    cq_doorbell_set3 : 16  ; /* [15:0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_CQDB_INT_CFG3;

/* Define the union U_CQDB_INT_CFG4 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    cq_doorbell_ro4  : 16  ; /* [31:16] */
        unsigned int    cq_doorbell_set4 : 16  ; /* [15:0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_CQDB_INT_CFG4;

/* Define the union U_CQDB_INT_CFG5 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    cq_doorbell_ro5  : 16  ; /* [31:16] */
        unsigned int    cq_doorbell_set5 : 16  ; /* [15:0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_CQDB_INT_CFG5;

/* Define the union U_CQDB_INT_CFG6 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    cq_doorbell_ro6  : 16  ; /* [31:16] */
        unsigned int    cq_doorbell_set6 : 16  ; /* [15:0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_CQDB_INT_CFG6;

/* Define the union U_CQDB_INT_CFG7 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    cq_doorbell_ro7  : 16  ; /* [31:16] */
        unsigned int    cq_doorbell_set7 : 16  ; /* [15:0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_CQDB_INT_CFG7;

/* Define the union U_CQDB_INT_CFG8 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    cq_doorbell_ro8  : 16  ; /* [31:16] */
        unsigned int    cq_doorbell_set8 : 16  ; /* [15:0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_CQDB_INT_CFG8;

/* Define the union U_CQDB_INT_CFG9 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    cq_doorbell_ro9  : 16  ; /* [31:16] */
        unsigned int    cq_doorbell_set9 : 16  ; /* [15:0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_CQDB_INT_CFG9;

/* Define the union U_CQDB_INT_CFG10 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    cq_doorbell_ro10  : 16  ; /* [31:16] */
        unsigned int    cq_doorbell_set10 : 16  ; /* [15:0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_CQDB_INT_CFG10;

/* Define the union U_CQDB_INT_CFG11 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    cq_doorbell_ro11  : 16  ; /* [31:16] */
        unsigned int    cq_doorbell_set11 : 16  ; /* [15:0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_CQDB_INT_CFG11;

/* Define the union U_CQDB_INT_CFG12 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    cq_doorbell_ro12  : 16  ; /* [31:16] */
        unsigned int    cq_doorbell_set12 : 16  ; /* [15:0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_CQDB_INT_CFG12;

/* Define the union U_CQDB_INT_CFG13 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    cq_doorbell_ro13  : 16  ; /* [31:16] */
        unsigned int    cq_doorbell_set13 : 16  ; /* [15:0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_CQDB_INT_CFG13;

/* Define the union U_CQDB_INT_CFG14 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    cq_doorbell_ro14  : 16  ; /* [31:16] */
        unsigned int    cq_doorbell_set14 : 16  ; /* [15:0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_CQDB_INT_CFG14;

/* Define the union U_CQDB_INT_CFG15 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    cq_doorbell_ro15  : 16  ; /* [31:16] */
        unsigned int    cq_doorbell_set15 : 16  ; /* [15:0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_CQDB_INT_CFG15;

/* Define the union U_WRPTR_DB_INT_CFG0 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    wrptr_doorbell_ro0  : 16  ; /* [31:16] */
        unsigned int    wrptr_doorbell_set0 : 16  ; /* [15:0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_WRPTR_DB_INT_CFG0;

/* Define the union U_WRPTR_DB_INT_CFG1 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    wrptr_doorbell_ro1  : 16  ; /* [31:16] */
        unsigned int    wrptr_doorbell_set1 : 16  ; /* [15:0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_WRPTR_DB_INT_CFG1;

/* Define the union U_WRPTR_DB_INT_CFG2 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    wrptr_doorbell_ro2  : 16  ; /* [31:16] */
        unsigned int    wrptr_doorbell_set2 : 16  ; /* [15:0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_WRPTR_DB_INT_CFG2;

/* Define the union U_WRPTR_DB_INT_CFG3 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    wrptr_doorbell_ro3  : 16  ; /* [31:16] */
        unsigned int    wrptr_doorbell_set3 : 16  ; /* [15:0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_WRPTR_DB_INT_CFG3;

/* Define the union U_WRPTR_DB_INT_CFG4 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    wrptr_doorbell_ro4  : 16  ; /* [31:16] */
        unsigned int    wrptr_doorbell_set4 : 16  ; /* [15:0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_WRPTR_DB_INT_CFG4;

/* Define the union U_WRPTR_DB_INT_CFG5 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    wrptr_doorbell_ro5  : 16  ; /* [31:16] */
        unsigned int    wrptr_doorbell_set5 : 16  ; /* [15:0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_WRPTR_DB_INT_CFG5;

/* Define the union U_WRPTR_DB_INT_CFG6 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    wrptr_doorbell_ro6  : 16  ; /* [31:16] */
        unsigned int    wrptr_doorbell_set6 : 16  ; /* [15:0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_WRPTR_DB_INT_CFG6;

/* Define the union U_WRPTR_DB_INT_CFG7 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    wrptr_doorbell_ro7  : 16  ; /* [31:16] */
        unsigned int    wrptr_doorbell_set7 : 16  ; /* [15:0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_WRPTR_DB_INT_CFG7;

/* Define the union U_WRPTR_DB_INT_CFG8 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    wrptr_doorbell_ro8  : 16  ; /* [31:16] */
        unsigned int    wrptr_doorbell_set8 : 16  ; /* [15:0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_WRPTR_DB_INT_CFG8;

/* Define the union U_WRPTR_DB_INT_CFG9 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    wrptr_doorbell_ro9  : 16  ; /* [31:16] */
        unsigned int    wrptr_doorbell_set9 : 16  ; /* [15:0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_WRPTR_DB_INT_CFG9;

/* Define the union U_WRPTR_DB_INT_CFG10 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    wrptr_doorbell_ro10  : 16  ; /* [31:16] */
        unsigned int    wrptr_doorbell_set10 : 16  ; /* [15:0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_WRPTR_DB_INT_CFG10;

/* Define the union U_WRPTR_DB_INT_CFG11 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    wrptr_doorbell_ro11  : 16  ; /* [31:16] */
        unsigned int    wrptr_doorbell_set11 : 16  ; /* [15:0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_WRPTR_DB_INT_CFG11;

/* Define the union U_WRPTR_DB_INT_CFG12 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    wrptr_doorbell_ro12  : 16  ; /* [31:16] */
        unsigned int    wrptr_doorbell_set12 : 16  ; /* [15:0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_WRPTR_DB_INT_CFG12;

/* Define the union U_WRPTR_DB_INT_CFG13 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    wrptr_doorbell_ro13  : 16  ; /* [31:16] */
        unsigned int    wrptr_doorbell_set13 : 16  ; /* [15:0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_WRPTR_DB_INT_CFG13;

/* Define the union U_WRPTR_DB_INT_CFG14 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    wrptr_doorbell_ro14  : 16  ; /* [31:16] */
        unsigned int    wrptr_doorbell_set14 : 16  ; /* [15:0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_WRPTR_DB_INT_CFG14;

/* Define the union U_WRPTR_DB_INT_CFG15 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    wrptr_doorbell_ro15  : 16  ; /* [31:16] */
        unsigned int    wrptr_doorbell_set15 : 16  ; /* [15:0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_WRPTR_DB_INT_CFG15;

/* Define the union U_NVME_INT_MOD_SEL */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    rsv_122                : 30  ; /* [31:2] */
        unsigned int    nvme_nomral_int_mode   : 1  ; /* [1] */
        unsigned int    nvme_abnormal_int_mode : 1  ; /* [0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_NVME_INT_MOD_SEL;

/* Define the union U_NVME_INT_LEVEL_SEL */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    rsv_123                   : 28  ; /* [31:4] */
        unsigned int    nvme_sq_err_level_cfg     : 1  ; /* [3] */
        unsigned int    nvme_2bit_ecc_level_cfg   : 1  ; /* [2] */
        unsigned int    nvme_1bit_ecc_level_cfg   : 1  ; /* [1] */
        unsigned int    nvme_cq_err_int_level_cfg : 1  ; /* [0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_NVME_INT_LEVEL_SEL;

/* Define the union U_NVME_QUEUE_DEL */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    rsv_124            : 23  ; /* [31:9] */
        unsigned int    nvme_queue_del     : 1  ; /* [8] */
        unsigned int    nvme_queue_del_num : 8  ; /* [7:0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_NVME_QUEUE_DEL;

/* Define the union U_SQ_ERR_STS0 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    rsv_125            : 16  ; /* [31:16] */
        unsigned int    sqh_int_sts        : 1  ; /* [15] */
        unsigned int    sqt_int_sts        : 1  ; /* [14] */
        unsigned int    sqh_int_mask       : 1  ; /* [13] */
        unsigned int    sqt_int_mask       : 1  ; /* [12] */
        unsigned int    sqh_int_ro         : 1  ; /* [11] */
        unsigned int    sqt_int_ro         : 1  ; /* [10] */
        unsigned int    sqh_int_set        : 1  ; /* [9] */
        unsigned int    sqt_int_set        : 1  ; /* [8] */
        unsigned int    sqdb_err_queue_num : 8  ; /* [7:0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_SQ_ERR_STS0;

/* Define the union U_SQ_ERR_STS1 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    sq_err_head_ptr : 16  ; /* [31:16] */
        unsigned int    sq_err_tail_ptr : 16  ; /* [15:0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_SQ_ERR_STS1;

/* Define the union U_RO_CFG */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    rsv_126       : 26  ; /* [31:6] */
        unsigned int    sq_des_ro_cfg : 3  ; /* [5:3] */
        unsigned int    wrptr_ro_cfg  : 3  ; /* [2:0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_RO_CFG;

/* Define the union U_QUEUE_EN0 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    rsv_127       : 16  ; /* [31:16] */
        unsigned int    queue_enable0 : 16  ; /* [15:0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_QUEUE_EN0;

/* Define the union U_QUEUE_EN1 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    rsv_128       : 16  ; /* [31:16] */
        unsigned int    queue_enable1 : 16  ; /* [15:0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_QUEUE_EN1;

/* Define the union U_QUEUE_EN2 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    rsv_129       : 16  ; /* [31:16] */
        unsigned int    queue_enable2 : 16  ; /* [15:0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_QUEUE_EN2;

/* Define the union U_QUEUE_EN3 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    rsv_130       : 16  ; /* [31:16] */
        unsigned int    queue_enable3 : 16  ; /* [15:0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_QUEUE_EN3;

/* Define the union U_QUEUE_EN4 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    rsv_131       : 16  ; /* [31:16] */
        unsigned int    queue_enable4 : 16  ; /* [15:0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_QUEUE_EN4;

/* Define the union U_QUEUE_EN5 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    rsv_132       : 16  ; /* [31:16] */
        unsigned int    queue_enable5 : 16  ; /* [15:0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_QUEUE_EN5;

/* Define the union U_QUEUE_EN6 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    rsv_133       : 16  ; /* [31:16] */
        unsigned int    queue_enable6 : 16  ; /* [15:0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_QUEUE_EN6;

/* Define the union U_QUEUE_EN7 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    rsv_134       : 16  ; /* [31:16] */
        unsigned int    queue_enable7 : 16  ; /* [15:0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_QUEUE_EN7;

/* Define the union U_QUEUE_EN8 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    rsv_135       : 16  ; /* [31:16] */
        unsigned int    queue_enable8 : 16  ; /* [15:0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_QUEUE_EN8;

/* Define the union U_QUEUE_EN9 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    rsv_136       : 16  ; /* [31:16] */
        unsigned int    queue_enable9 : 16  ; /* [15:0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_QUEUE_EN9;

/* Define the union U_QUEUE_EN10 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    rsv_137        : 16  ; /* [31:16] */
        unsigned int    queue_enable10 : 16  ; /* [15:0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_QUEUE_EN10;

/* Define the union U_QUEUE_EN11 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    rsv_138        : 16  ; /* [31:16] */
        unsigned int    queue_enable11 : 16  ; /* [15:0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_QUEUE_EN11;

/* Define the union U_QUEUE_EN12 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    rsv_139        : 16  ; /* [31:16] */
        unsigned int    queue_enable12 : 16  ; /* [15:0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_QUEUE_EN12;

/* Define the union U_QUEUE_EN13 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    rsv_140        : 16  ; /* [31:16] */
        unsigned int    queue_enable13 : 16  ; /* [15:0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_QUEUE_EN13;

/* Define the union U_QUEUE_EN14 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    rsv_141        : 16  ; /* [31:16] */
        unsigned int    queue_enable14 : 16  ; /* [15:0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_QUEUE_EN14;

/* Define the union U_QUEUE_EN15 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    rsv_142        : 16  ; /* [31:16] */
        unsigned int    queue_enable15 : 16  ; /* [15:0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_QUEUE_EN15;

/* Define the union U_CQ_ERR_INT0 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    cq_err_int_sts   : 16  ; /* [31:16] */
        unsigned int    cq_err_int_mask0 : 16  ; /* [15:0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_CQ_ERR_INT0;

/* Define the union U_CQ_ERR_INT1 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    cq_err_int_sts   : 16  ; /* [31:16] */
        unsigned int    cq_err_int_mask1 : 16  ; /* [15:0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_CQ_ERR_INT1;

/* Define the union U_CQ_ERR_INT2 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    cq_err_int_sts   : 16  ; /* [31:16] */
        unsigned int    cq_err_int_mask2 : 16  ; /* [15:0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_CQ_ERR_INT2;

/* Define the union U_CQ_ERR_INT3 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    cq_err_int_sts   : 16  ; /* [31:16] */
        unsigned int    cq_err_int_mask3 : 16  ; /* [15:0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_CQ_ERR_INT3;

/* Define the union U_CQ_ERR_INT4 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    cq_err_int_sts   : 16  ; /* [31:16] */
        unsigned int    cq_err_int_mask4 : 16  ; /* [15:0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_CQ_ERR_INT4;

/* Define the union U_CQ_ERR_INT5 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    cq_err_int_sts   : 16  ; /* [31:16] */
        unsigned int    cq_err_int_mask5 : 16  ; /* [15:0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_CQ_ERR_INT5;

/* Define the union U_CQ_ERR_INT6 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    cq_err_int_sts   : 16  ; /* [31:16] */
        unsigned int    cq_err_int_mask6 : 16  ; /* [15:0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_CQ_ERR_INT6;

/* Define the union U_CQ_ERR_INT7 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    cq_err_int_sts   : 16  ; /* [31:16] */
        unsigned int    cq_err_int_mask7 : 16  ; /* [15:0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_CQ_ERR_INT7;

/* Define the union U_CQ_ERR_INT8 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    cq_err_int_sts   : 16  ; /* [31:16] */
        unsigned int    cq_err_int_mask8 : 16  ; /* [15:0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_CQ_ERR_INT8;

/* Define the union U_CQ_ERR_INT9 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    cq_err_int_sts   : 16  ; /* [31:16] */
        unsigned int    cq_err_int_mask9 : 16  ; /* [15:0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_CQ_ERR_INT9;

/* Define the union U_CQ_ERR_INT10 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    cq_err_int_sts    : 16  ; /* [31:16] */
        unsigned int    cq_err_int_mask10 : 16  ; /* [15:0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_CQ_ERR_INT10;

/* Define the union U_CQ_ERR_INT11 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    cq_err_int_sts    : 16  ; /* [31:16] */
        unsigned int    cq_err_int_mask11 : 16  ; /* [15:0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_CQ_ERR_INT11;

/* Define the union U_CQ_ERR_INT12 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    cq_err_int_sts    : 16  ; /* [31:16] */
        unsigned int    cq_err_int_mask12 : 16  ; /* [15:0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_CQ_ERR_INT12;

/* Define the union U_CQ_ERR_INT13 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    cq_err_int_sts    : 16  ; /* [31:16] */
        unsigned int    cq_err_int_mask13 : 16  ; /* [15:0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_CQ_ERR_INT13;

/* Define the union U_CQ_ERR_INT14 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    cq_err_int_sts    : 16  ; /* [31:16] */
        unsigned int    cq_err_int_mask14 : 16  ; /* [15:0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_CQ_ERR_INT14;

/* Define the union U_CQ_ERR_INT15 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    cq_err_int_sts    : 16  ; /* [31:16] */
        unsigned int    cq_err_int_mask15 : 16  ; /* [15:0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_CQ_ERR_INT15;

/* Define the union U_CQ_ERR_INT_CFG0 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    cq_err_int_ro0  : 16  ; /* [31:16] */
        unsigned int    cq_err_int_set0 : 16  ; /* [15:0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_CQ_ERR_INT_CFG0;

/* Define the union U_CQ_ERR_INT_CFG1 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    cq_err_int_ro1  : 16  ; /* [31:16] */
        unsigned int    cq_err_int_set1 : 16  ; /* [15:0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_CQ_ERR_INT_CFG1;

/* Define the union U_CQ_ERR_INT_CFG2 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    cq_err_int_ro2  : 16  ; /* [31:16] */
        unsigned int    cq_err_int_set2 : 16  ; /* [15:0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_CQ_ERR_INT_CFG2;

/* Define the union U_CQ_ERR_INT_CFG3 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    cq_err_int_ro3  : 16  ; /* [31:16] */
        unsigned int    cq_err_int_set3 : 16  ; /* [15:0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_CQ_ERR_INT_CFG3;

/* Define the union U_CQ_ERR_INT_CFG4 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    cq_err_int_ro4  : 16  ; /* [31:16] */
        unsigned int    cq_err_int_set4 : 16  ; /* [15:0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_CQ_ERR_INT_CFG4;

/* Define the union U_CQ_ERR_INT_CFG5 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    cq_err_int_ro5  : 16  ; /* [31:16] */
        unsigned int    cq_err_int_set5 : 16  ; /* [15:0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_CQ_ERR_INT_CFG5;

/* Define the union U_CQ_ERR_INT_CFG6 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    cq_err_int_ro6  : 16  ; /* [31:16] */
        unsigned int    cq_err_int_set6 : 16  ; /* [15:0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_CQ_ERR_INT_CFG6;

/* Define the union U_CQ_ERR_INT_CFG7 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    cq_err_int_ro7  : 16  ; /* [31:16] */
        unsigned int    cq_err_int_set7 : 16  ; /* [15:0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_CQ_ERR_INT_CFG7;

/* Define the union U_CQ_ERR_INT_CFG8 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    cq_err_int_ro8  : 16  ; /* [31:16] */
        unsigned int    cq_err_int_set8 : 16  ; /* [15:0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_CQ_ERR_INT_CFG8;

/* Define the union U_CQ_ERR_INT_CFG9 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    cq_err_int_ro9  : 16  ; /* [31:16] */
        unsigned int    cq_err_int_set9 : 16  ; /* [15:0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_CQ_ERR_INT_CFG9;

/* Define the union U_CQ_ERR_INT_CFG10 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    cq_err_int_ro10  : 16  ; /* [31:16] */
        unsigned int    cq_err_int_set10 : 16  ; /* [15:0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_CQ_ERR_INT_CFG10;

/* Define the union U_CQ_ERR_INT_CFG11 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    cq_err_int_ro11  : 16  ; /* [31:16] */
        unsigned int    cq_err_int_set11 : 16  ; /* [15:0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_CQ_ERR_INT_CFG11;

/* Define the union U_CQ_ERR_INT_CFG12 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    cq_err_int_ro12  : 16  ; /* [31:16] */
        unsigned int    cq_err_int_set12 : 16  ; /* [15:0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_CQ_ERR_INT_CFG12;

/* Define the union U_CQ_ERR_INT_CFG13 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    cq_err_int_ro13  : 16  ; /* [31:16] */
        unsigned int    cq_err_int_set13 : 16  ; /* [15:0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_CQ_ERR_INT_CFG13;

/* Define the union U_CQ_ERR_INT_CFG14 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    cq_err_int_ro14  : 16  ; /* [31:16] */
        unsigned int    cq_err_int_set14 : 16  ; /* [15:0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_CQ_ERR_INT_CFG14;

/* Define the union U_CQ_ERR_INT_CFG15 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    cq_err_int_ro15  : 16  ; /* [31:16] */
        unsigned int    cq_err_int_set15 : 16  ; /* [15:0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_CQ_ERR_INT_CFG15;

/* Define the union U_WR_PTR_BASE_ADDDR_LOW */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    wr_ptr_base_addr_low : 30  ; /* [31:2] */
        unsigned int    rsv_143              : 2  ; /* [1:0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_WR_PTR_BASE_ADDDR_LOW;

/* Define the union U_WR_PTR_BASE_ADDDR_HIGH */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    wr_ptr_base_addr_high : 32  ; /* [31:0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_WR_PTR_BASE_ADDDR_HIGH;

/* Define the union U_MEMORY_BASE_ADDDR_LOW */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    memory_base_addr_low : 26  ; /* [31:6] */
        unsigned int    rsv_144              : 6  ; /* [5:0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_MEMORY_BASE_ADDDR_LOW;

/* Define the union U_MEMORY_BASE_ADDDR_HIGH */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    memory_base_addr_high : 32  ; /* [31:0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_MEMORY_BASE_ADDDR_HIGH;

/* Define the union U_MEMORY_SPACE_LOW */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    rsv_145          : 28  ; /* [31:4] */
        unsigned int    memory_space_low : 4  ; /* [3:0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_MEMORY_SPACE_LOW;

/* Define the union U_MEMORY_SPACE_HIGH */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    rsv_146 : 32  ; /* [31:0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_MEMORY_SPACE_HIGH;

/* Define the union U_DB_STRD */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    rsv_147     : 16  ; /* [31:16] */
        unsigned int    pf0_strd    : 4  ; /* [15:12] */
        unsigned int    pf_1_strd   : 4  ; /* [11:8] */
        unsigned int    pf0_vf_strd : 4  ; /* [7:4] */
        unsigned int    pf1_vf_strd : 4  ; /* [3:0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_DB_STRD;

/* Define the union U_VF_SIZE */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    rsv_148 : 26  ; /* [31:6] */
        unsigned int    vf_size : 6  ; /* [5:0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VF_SIZE;

/* Define the union U_SQE_RD_MAX */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    rsv_149    : 29  ; /* [31:3] */
        unsigned int    sqe_rd_max : 3  ; /* [2:0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_SQE_RD_MAX;

/* Define the union U_RAM_INITIAL */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    rsv_150         : 18  ; /* [31:14] */
        unsigned int    ram_inital_done : 7  ; /* [13:7] */
        unsigned int    ram_initial     : 7  ; /* [6:0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_RAM_INITIAL;

/* Define the union U_INT_TIME_THR */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    rsv_151 : 29  ; /* [31:3] */
        unsigned int    rsv_152 : 3  ; /* [2:0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_INT_TIME_THR;

/* Define the union U_DDR_STRD */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    rsv_153        : 28  ; /* [31:4] */
        unsigned int    ddr_wrptr_strd : 4  ; /* [3:0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_DDR_STRD;

/* Define the union U_INT_REQ */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    rsv_154      : 5  ; /* [31:27] */
        unsigned int    pf_num       : 3  ; /* [26:24] */
        unsigned int    vf_num       : 8  ; /* [23:16] */
        unsigned int    rsv_155      : 3  ; /* [15:13] */
        unsigned int    int_vector   : 11  ; /* [12:2] */
        unsigned int    int_deassert : 1  ; /* [1] */
        unsigned int    req_set      : 1  ; /* [0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_INT_REQ;

/* Define the union U_NVME_VECTOR_PENDING */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    rsv_156        : 31  ; /* [31:1] */
        unsigned int    vector_pending : 1  ; /* [0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_NVME_VECTOR_PENDING;

/* Define the union U_NVME_VECTOR_MASK */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    rsv_157     : 31  ; /* [31:1] */
        unsigned int    vector_mask : 1  ; /* [0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_NVME_VECTOR_MASK;

/* Define the union U_IOSQS */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    completion_queue_size : 16  ; /* [31:16] */
        unsigned int    submission_queue_size : 16  ; /* [15:0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_IOSQS;

/* Define the union U_IOSQ_BASE_ADDR_LOW */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    sqr_base_addr_low : 32  ; /* [31:0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_IOSQ_BASE_ADDR_LOW;

/* Define the union U_IOSQ_BASE_ADDR_HIGH */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    sqr_base_addr_high : 32  ; /* [31:0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_IOSQ_BASE_ADDR_HIGH;

/* Define the union U_SQR_MEMORY_RD_PTR */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    memory_rd_ptr : 32  ; /* [31:0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_SQR_MEMORY_RD_PTR;

/* Define the union U_SQ_SIZE_RAM_ECC_INJECT */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    sq_size_ram_ecc_inject    : 2  ; /* [31:30] */
        unsigned int    rsv_158                   : 6  ; /* [29:24] */
        unsigned int    sq_size_ram_2bit_ecc_ro   : 1  ; /* [23] */
        unsigned int    sq_size_ram_1bit_ecc_ro   : 1  ; /* [22] */
        unsigned int    sq_size_ram_2bit_ecc_sts  : 1  ; /* [21] */
        unsigned int    sq_size_ram_1bit_ecc_sts  : 1  ; /* [20] */
        unsigned int    sq_size_ram_2bit_ecc_mask : 1  ; /* [19] */
        unsigned int    sq_size_ram_1bit_ecc_mask : 1  ; /* [18] */
        unsigned int    sq_size_ram_2bit_ecc_set  : 1  ; /* [17] */
        unsigned int    sq_size_ram_1bit_ecc_set  : 1  ; /* [16] */
        unsigned int    sq_size_ram_2bit_ecc_cnt  : 8  ; /* [15:8] */
        unsigned int    sq_size_ram_1bit_ecc_cnt  : 8  ; /* [7:0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_SQ_SIZE_RAM_ECC_INJECT;

/* Define the union U_SQ_SIZE_RAM_ECC_ADDR */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    rsv_159                   : 8  ; /* [31:24] */
        unsigned int    sq_size_ram_2bit_ecc_addr : 8  ; /* [23:16] */
        unsigned int    rsv_160                   : 8  ; /* [15:8] */
        unsigned int    sq_size_ram_1bit_ecc_addr : 8  ; /* [7:0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_SQ_SIZE_RAM_ECC_ADDR;

/* Define the union U_SQ_BAL_RAM_ECC_INJECT */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    sq_bal_ram_ecc_inject    : 2  ; /* [31:30] */
        unsigned int    rsv_161                  : 6  ; /* [29:24] */
        unsigned int    sq_bal_ram_2bit_ecc_ro   : 1  ; /* [23] */
        unsigned int    sq_bal_ram_1bit_ecc_ro   : 1  ; /* [22] */
        unsigned int    sq_bal_ram_2bit_ecc_sts  : 1  ; /* [21] */
        unsigned int    sq_bal_ram_1bit_ecc_sts  : 1  ; /* [20] */
        unsigned int    sq_bal_ram_2bit_ecc_mask : 1  ; /* [19] */
        unsigned int    sq_bal_ram_1bit_ecc_mask : 1  ; /* [18] */
        unsigned int    sq_bal_ram_2bit_ecc_set  : 1  ; /* [17] */
        unsigned int    sq_bal_ram_1bit_ecc_set  : 1  ; /* [16] */
        unsigned int    sq_bal_ram_2bit_ecc_cnt  : 8  ; /* [15:8] */
        unsigned int    sq_bal_ram_1bit_ecc_cnt  : 8  ; /* [7:0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_SQ_BAL_RAM_ECC_INJECT;

/* Define the union U_SQ_BAL_RAM_ECC_ADDR */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    rsv_162                  : 8  ; /* [31:24] */
        unsigned int    sq_bal_ram_2bit_ecc_addr : 8  ; /* [23:16] */
        unsigned int    rsv_163                  : 8  ; /* [15:8] */
        unsigned int    sq_bal_ram_1bit_ecc_addr : 8  ; /* [7:0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_SQ_BAL_RAM_ECC_ADDR;

/* Define the union U_SQ_BAH_RAM_ECC_INJECT */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    sq_bah_ram_ecc_inject    : 2  ; /* [31:30] */
        unsigned int    rsv_164                  : 6  ; /* [29:24] */
        unsigned int    sq_bah_ram_2bit_ecc_ro   : 1  ; /* [23] */
        unsigned int    sq_bah_ram_1bit_ecc_ro   : 1  ; /* [22] */
        unsigned int    sq_bah_ram_2bit_ecc_sts  : 1  ; /* [21] */
        unsigned int    sq_bah_ram_1bit_ecc_sts  : 1  ; /* [20] */
        unsigned int    sq_bah_ram_2bit_ecc_mask : 1  ; /* [19] */
        unsigned int    sq_bah_ram_1bit_ecc_mask : 1  ; /* [18] */
        unsigned int    sq_bah_ram_2bit_ecc_set  : 1  ; /* [17] */
        unsigned int    sq_bah_ram_1bit_ecc_set  : 1  ; /* [16] */
        unsigned int    sq_bah_ram_2bit_ecc_cnt  : 8  ; /* [15:8] */
        unsigned int    sq_bah_ram_1bit_ecc_cnt  : 8  ; /* [7:0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_SQ_BAH_RAM_ECC_INJECT;

/* Define the union U_SQ_BAH_RAM_ECC_ADDR */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    rsv_165                  : 8  ; /* [31:24] */
        unsigned int    sq_bah_ram_2bit_ecc_addr : 8  ; /* [23:16] */
        unsigned int    rsv_166                  : 8  ; /* [15:8] */
        unsigned int    sq_bah_ram_1bit_ecc_addr : 8  ; /* [7:0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_SQ_BAH_RAM_ECC_ADDR;

/* Define the union U_RD_PTR_RAM_ECC_INJECT */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    rd_ptr_ram_ecc_inject    : 2  ; /* [31:30] */
        unsigned int    rsv_167                  : 6  ; /* [29:24] */
        unsigned int    rd_ptr_ram_2bit_ecc_ro   : 1  ; /* [23] */
        unsigned int    rd_ptr_ram_1bit_ecc_ro   : 1  ; /* [22] */
        unsigned int    rd_ptr_ram_2bit_ecc_sts  : 1  ; /* [21] */
        unsigned int    rd_ptr_ram_1bit_ecc_sts  : 1  ; /* [20] */
        unsigned int    rd_ptr_ram_2bit_ecc_mask : 1  ; /* [19] */
        unsigned int    rd_ptr_ram_1bit_ecc_mask : 1  ; /* [18] */
        unsigned int    rd_ptr_ram_2bit_ecc_set  : 1  ; /* [17] */
        unsigned int    rd_ptr_ram_1bit_ecc_set  : 1  ; /* [16] */
        unsigned int    rd_ptr_ram_2bit_ecc_cnt  : 8  ; /* [15:8] */
        unsigned int    rd_ptr_ram_1bit_ecc_cnt  : 8  ; /* [7:0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_RD_PTR_RAM_ECC_INJECT;

/* Define the union U_RD_PTR_RAM_ECC_ADDR */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    rsv_168                  : 8  ; /* [31:24] */
        unsigned int    rd_ptr_ram_2bit_ecc_addr : 8  ; /* [23:16] */
        unsigned int    rsv_169                  : 8  ; /* [15:8] */
        unsigned int    rd_ptr_ram_1bit_ecc_addr : 8  ; /* [7:0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_RD_PTR_RAM_ECC_ADDR;

/* Define the union U_SQ_DES_RAM_ECC_INJECT */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    sq_des_ram_ecc_inject    : 2  ; /* [31:30] */
        unsigned int    rsv_170                  : 6  ; /* [29:24] */
        unsigned int    sq_des_ram_2bit_ecc_ro   : 1  ; /* [23] */
        unsigned int    sq_des_ram_1bit_ecc_ro   : 1  ; /* [22] */
        unsigned int    sq_des_ram_2bit_ecc_sts  : 1  ; /* [21] */
        unsigned int    sq_des_ram_1bit_ecc_sts  : 1  ; /* [20] */
        unsigned int    sq_des_ram_2bit_ecc_mask : 1  ; /* [19] */
        unsigned int    sq_des_ram_1bit_ecc_mask : 1  ; /* [18] */
        unsigned int    sq_des_ram_2bit_ecc_set  : 1  ; /* [17] */
        unsigned int    sq_des_ram_1bit_ecc_set  : 1  ; /* [16] */
        unsigned int    sq_des_ram_2bit_ecc_cnt  : 8  ; /* [15:8] */
        unsigned int    sq_des_ram_1bit_ecc_cnt  : 8  ; /* [7:0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_SQ_DES_RAM_ECC_INJECT;

/* Define the union U_SQ_DES_RAM_ECC_ADDR */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    rsv_171                  : 8  ; /* [31:24] */
        unsigned int    sq_des_ram_2bit_ecc_addr : 8  ; /* [23:16] */
        unsigned int    rsv_172                  : 8  ; /* [15:8] */
        unsigned int    sq_des_ram_1bit_ecc_addr : 8  ; /* [7:0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_SQ_DES_RAM_ECC_ADDR;

/* Define the union U_PRFETCH_FIFO_RAM_ECC_INJECT */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    prefetch_fifo_ram_ecc_inject    : 2  ; /* [31:30] */
        unsigned int    rsv_173                         : 6  ; /* [29:24] */
        unsigned int    prefetch_fifo_ram_2bit_ecc_ro   : 1  ; /* [23] */
        unsigned int    prefetch_fifo_ram_1bit_ecc_ro   : 1  ; /* [22] */
        unsigned int    prefetch_fifo_ram_2bit_ecc_sts  : 1  ; /* [21] */
        unsigned int    prefetch_fifo_ram_1bit_ecc_sts  : 1  ; /* [20] */
        unsigned int    prefetch_fifo_ram_2bit_ecc_mask : 1  ; /* [19] */
        unsigned int    prefetch_fifo_ram_1bit_ecc_mask : 1  ; /* [18] */
        unsigned int    prefetch_fifo_ram_2bit_ecc_set  : 1  ; /* [17] */
        unsigned int    prefetch_fifo_ram_1bit_ecc_set  : 1  ; /* [16] */
        unsigned int    prefetch_fifo_ram_2bit_ecc_cnt  : 8  ; /* [15:8] */
        unsigned int    prefetch_fifo_ram_1bit_ecc_cnt  : 8  ; /* [7:0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_PRFETCH_FIFO_RAM_ECC_INJECT;

/* Define the union U_PRFETCH_FIFO_RAM_ECC_ADDR */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    rsv_174                         : 8  ; /* [31:24] */
        unsigned int    prefetch_fifo_ram_2bit_ecc_addr : 8  ; /* [23:16] */
        unsigned int    rsv_175                         : 8  ; /* [15:8] */
        unsigned int    prefetch_fifo_ram_1bit_ecc_addr : 8  ; /* [7:0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_PRFETCH_FIFO_RAM_ECC_ADDR;

/* Define the union U_NVME_LINK_DOWN_PROC */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    nvme_link_up_sts           : 1  ; /* [31] */
        unsigned int    rsv_176                    : 15  ; /* [30:16] */
        unsigned int    nvme_link_down_prt_initial : 1  ; /* [15] */
        unsigned int    rsv_177                    : 15  ; /* [14:0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_NVME_LINK_DOWN_PROC;

/* Define the union U_NVME_FLR_STS */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    rsv_178             : 30  ; /* [31:2] */
        unsigned int    nvme_flr__proc_cmpl : 1  ; /* [1] */
        unsigned int    nvme_flr_ptr_inital : 1  ; /* [0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_NVME_FLR_STS;

/* Define the union U_NVME_DMACQ_ERR_STS */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    rsv_179                 : 31  ; /* [31:1] */
        unsigned int    nvme_cq_err_ptr_initial : 1  ; /* [0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_NVME_DMACQ_ERR_STS;

/* Define the union U_NVME_SQ_DES_RAM_CFG */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    nvme_sq_des_ctrl_ram_wr   : 1  ; /* [31] */
        unsigned int    nvme_sq_des_ctrl_ram_rd   : 1  ; /* [30] */
        unsigned int    rsv_180                   : 22  ; /* [29:8] */
        unsigned int    nvme_sq_des_ctrl_ram_addr : 8  ; /* [7:0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_NVME_SQ_DES_RAM_CFG;

/* Define the union U_NVME_SQ_DES_RAM_WDAT_L */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    sq_des_ram_wr_ptr : 32  ; /* [31:0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_NVME_SQ_DES_RAM_WDAT_L;

/* Define the union U_NVME_SQ_DES_RAM_WDAT_H */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    rsv_181           : 16  ; /* [31:16] */
        unsigned int    sq_des_ram_hd_ptr : 16  ; /* [15:0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_NVME_SQ_DES_RAM_WDAT_H;

/* Define the union U_NVME_SQ_DES_RAM_RDAT_L */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    sq_des_ram_wr_ptr : 32  ; /* [31:0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_NVME_SQ_DES_RAM_RDAT_L;

/* Define the union U_NVME_SQ_DES_RAM_RDAT_H */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    rsv_182           : 16  ; /* [31:16] */
        unsigned int    sq_des_ram_hd_ptr : 16  ; /* [15:0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_NVME_SQ_DES_RAM_RDAT_H;

/* Define the union U_NVME_FIFO_STS */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    rsv_183                : 26  ; /* [31:6] */
        unsigned int    sq_des_ctrl_fifo_full  : 1  ; /* [5] */
        unsigned int    sq_des_ctrl_fifo_empty : 1  ; /* [4] */
        unsigned int    cq_des_ctrl_fifo_full  : 1  ; /* [3] */
        unsigned int    cq_des_ctrl_fifo_empty : 1  ; /* [2] */
        unsigned int    cq_err_sts_fifo_full   : 1  ; /* [1] */
        unsigned int    cq_err_sts_fifo_empty  : 1  ; /* [0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_NVME_FIFO_STS;

/* Define the union U_NVME_FSM_STS */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    rsv_184           : 22  ; /* [31:10] */
        unsigned int    tail_ram_fsm_sts  : 2  ; /* [9:8] */
        unsigned int    sqs_ram_fsm_sts   : 2  ; /* [7:6] */
        unsigned int    sqbal_ram_fsm_sts : 2  ; /* [5:4] */
        unsigned int    sqbah_ram_fsm_sts : 2  ; /* [3:2] */
        unsigned int    rdptr_ram_fsm_sts : 2  ; /* [1:0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_NVME_FSM_STS;

/* Define the union U_NVME_FLOW_STS0 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    sq_cnt : 16  ; /* [31:16] */
        unsigned int    cq_cnt : 16  ; /* [15:0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_NVME_FLOW_STS0;

/* Define the union U_NVME_FLOW_STS1 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    wrptr_req_cnt   : 16  ; /* [31:16] */
        unsigned int    wrptr_bresp_cnt : 16  ; /* [15:0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_NVME_FLOW_STS1;

/* Define the union U_NVME_QUEUE_STS0 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    nvme_queue_sts0 : 32  ; /* [31:0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_NVME_QUEUE_STS0;

/* Define the union U_NVME_QUEUE_STS1 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    nvme_queue_sts1 : 32  ; /* [31:0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_NVME_QUEUE_STS1;

/* Define the union U_NVME_QUEUE_STS2 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    nvme_queue_sts2 : 32  ; /* [31:0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_NVME_QUEUE_STS2;

/* Define the union U_NVME_QUEUE_STS3 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    nvme_queue_sts3 : 32  ; /* [31:0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_NVME_QUEUE_STS3;

/* Define the union U_NVME_QUEUE_STS4 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    nvme_queue_sts4 : 32  ; /* [31:0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_NVME_QUEUE_STS4;

/* Define the union U_NVME_QUEUE_STS5 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    nvme_queue_sts5 : 32  ; /* [31:0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_NVME_QUEUE_STS5;

/* Define the union U_NVME_QUEUE_STS6 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    nvme_queue_sts6 : 32  ; /* [31:0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_NVME_QUEUE_STS6;

/* Define the union U_NVME_QUEUE_STS7 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    nvme_queue_sts7 : 32  ; /* [31:0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_NVME_QUEUE_STS7;


//==============================================================================
/* Define the global struct */
typedef struct
{
    volatile U_PF_WR_CTRL_REG_INT_STS      PF_WR_CTRL_REG_INT_STS[2]   ; /* 40 */
    volatile U_VF_WR_CTRL_REG_INT_STS      VF_WR_CTRL_REG_INT_STS[127] ; /* 240 */
    volatile U_NVME_CTRL_MISC              NVME_CTRL_MISC              ; /* A000 */
    volatile U_SRIOV_MODE_CTRL             SRIOV_MODE_CTRL             ; /* A004 */
    volatile U_PF2_BAR0_DB_ATU_LOW         PF2_BAR0_DB_ATU_LOW         ; /* A008 */
    volatile U_PF2_BAR0_DB_ATU_HIGH        PF2_BAR0_DB_ATU_HIGH        ; /* A00C */
    volatile U_PF2_VF_BAR0_DB_ATU_LOW      PF2_VF_BAR0_DB_ATU_LOW      ; /* A010 */
    volatile U_PF2_VF_BAR0_DB_ATU_HIGH     PF2_VF_BAR0_DB_ATU_HIGH     ; /* A014 */
    volatile U_PF3_BAR0_DB_ATU_LOW         PF3_BAR0_DB_ATU_LOW         ; /* A018 */
    volatile U_PF3_BAR0_DB_ATU_HIGH        PF3_BAR0_DB_ATU_HIGH        ; /* A01C */
    volatile U_PF3_VF_BAR0_DB_ATU_LOW      PF3_VF_BAR0_DB_ATU_LOW      ; /* A020 */
    volatile U_PF3_VF_BAR0_DB_ATU_HIGH     PF3_VF_BAR0_DB_ATU_HIGH     ; /* A024 */
    volatile U_INT_COAL_TIME_CNT_UNIT      INT_COAL_TIME_CNT_UNIT      ; /* A028 */
    volatile U_INT_COAL_MASK0              INT_COAL_MASK0              ; /* A02C */
    volatile U_INT_COAL_MASK1              INT_COAL_MASK1              ; /* A030 */
    volatile U_INT_COAL_MASK2              INT_COAL_MASK2              ; /* A034 */
    volatile U_INT_COAL_MASK3              INT_COAL_MASK3              ; /* A038 */
    volatile U_INT_COAL_MASK4              INT_COAL_MASK4              ; /* A03C */
    volatile U_INT_COAL_MASK5              INT_COAL_MASK5              ; /* A040 */
    volatile U_INT_COAL_MASK6              INT_COAL_MASK6              ; /* A044 */
    volatile U_INT_COAL_MASK7              INT_COAL_MASK7              ; /* A048 */
    volatile U_INT_COAL_MASK8              INT_COAL_MASK8              ; /* A04C */
    volatile U_INT_COAL_MASK9              INT_COAL_MASK9              ; /* A050 */
    volatile U_INT_COAL_MASK10             INT_COAL_MASK10             ; /* A054 */
    volatile U_INT_COAL_MASK11             INT_COAL_MASK11             ; /* A058 */
    volatile U_INT_COAL_MASK12             INT_COAL_MASK12             ; /* A05C */
    volatile U_INT_COAL_MASK13             INT_COAL_MASK13             ; /* A060 */
    volatile U_INT_COAL_MASK14             INT_COAL_MASK14             ; /* A064 */
    volatile U_INT_COAL_MASK15             INT_COAL_MASK15             ; /* A068 */
    volatile U_CQDB_INT_MASK0              CQDB_INT_MASK0              ; /* A06C */
    volatile U_CQDB_INT_MASK1              CQDB_INT_MASK1              ; /* A070 */
    volatile U_CQDB_INT_MASK2              CQDB_INT_MASK2              ; /* A074 */
    volatile U_CQDB_INT_MASK3              CQDB_INT_MASK3              ; /* A078 */
    volatile U_CQDB_INT_MASK4              CQDB_INT_MASK4              ; /* A07C */
    volatile U_CQDB_INT_MASK5              CQDB_INT_MASK5              ; /* A080 */
    volatile U_CQDB_INT_MASK6              CQDB_INT_MASK6              ; /* A084 */
    volatile U_CQDB_INT_MASK7              CQDB_INT_MASK7              ; /* A088 */
    volatile U_CQDB_INT_MASK8              CQDB_INT_MASK8              ; /* A08C */
    volatile U_CQDB_INT_MASK9              CQDB_INT_MASK9              ; /* A090 */
    volatile U_CQDB_INT_MASK10             CQDB_INT_MASK10             ; /* A094 */
    volatile U_CQDB_INT_MASK11             CQDB_INT_MASK11             ; /* A098 */
    volatile U_CQDB_INT_MASK12             CQDB_INT_MASK12             ; /* A09C */
    volatile U_CQDB_INT_MASK13             CQDB_INT_MASK13             ; /* A0A0 */
    volatile U_CQDB_INT_MASK14             CQDB_INT_MASK14             ; /* A0A4 */
    volatile U_CQDB_INT_MASK15             CQDB_INT_MASK15             ; /* A0A8 */
    volatile U_WRPTR_INT_MASK0             WRPTR_INT_MASK0             ; /* A0AC */
    volatile U_WRPTR_INT_MASK1             WRPTR_INT_MASK1             ; /* A0B0 */
    volatile U_WRPTR_INT_MASK2             WRPTR_INT_MASK2             ; /* A0B4 */
    volatile U_WRPTR_INT_MASK3             WRPTR_INT_MASK3             ; /* A0B8 */
    volatile U_WRPTR_INT_MASK4             WRPTR_INT_MASK4             ; /* A0BC */
    volatile U_WRPTR_INT_MASK5             WRPTR_INT_MASK5             ; /* A0C0 */
    volatile U_WRPTR_INT_MASK6             WRPTR_INT_MASK6             ; /* A0C4 */
    volatile U_WRPTR_INT_MASK7             WRPTR_INT_MASK7             ; /* A0C8 */
    volatile U_DB_INT_COAL_CFG             DB_INT_COAL_CFG             ; /* A0CC */
    volatile U_ENG_WORK_MODE_SEL           ENG_WORK_MODE_SEL           ; /* A0D0 */
    volatile U_WRPTR_INT_MASK8             WRPTR_INT_MASK8             ; /* A0D4 */
    volatile U_WRPTR_INT_MASK9             WRPTR_INT_MASK9             ; /* A0D8 */
    volatile U_WRPTR_INT_MASK10            WRPTR_INT_MASK10            ; /* A0DC */
    volatile U_WRPTR_INT_MASK11            WRPTR_INT_MASK11            ; /* A0E0 */
    volatile U_WRPTR_INT_MASK12            WRPTR_INT_MASK12            ; /* A0E4 */
    volatile U_WRPTR_INT_MASK13            WRPTR_INT_MASK13            ; /* A0E8 */
    volatile U_WRPTR_INT_MASK14            WRPTR_INT_MASK14            ; /* A0EC */
    volatile U_WRPTR_INT_MASK15            WRPTR_INT_MASK15            ; /* A0F0 */
    volatile U_SQDB_INT_STS0               SQDB_INT_STS0               ; /* A0F4 */
    volatile U_SQDB_INT_STS1               SQDB_INT_STS1               ; /* A0F8 */
    volatile U_SQDB_INT_STS2               SQDB_INT_STS2               ; /* A0FC */
    volatile U_SQDB_INT_STS3               SQDB_INT_STS3               ; /* A100 */
    volatile U_SQDB_INT_STS4               SQDB_INT_STS4               ; /* A104 */
    volatile U_SQDB_INT_STS5               SQDB_INT_STS5               ; /* A108 */
    volatile U_SQDB_INT_STS6               SQDB_INT_STS6               ; /* A10C */
    volatile U_SQDB_INT_STS7               SQDB_INT_STS7               ; /* A110 */
    volatile U_SQDB_INT_STS8               SQDB_INT_STS8               ; /* A114 */
    volatile U_SQDB_INT_STS9               SQDB_INT_STS9               ; /* A118 */
    volatile U_SQDB_INT_STS10              SQDB_INT_STS10              ; /* A11C */
    volatile U_SQDB_INT_STS11              SQDB_INT_STS11              ; /* A120 */
    volatile U_SQDB_INT_STS12              SQDB_INT_STS12              ; /* A124 */
    volatile U_SQDB_INT_STS13              SQDB_INT_STS13              ; /* A128 */
    volatile U_SQDB_INT_STS14              SQDB_INT_STS14              ; /* A12C */
    volatile U_SQDB_INT_STS15              SQDB_INT_STS15              ; /* A130 */
    volatile U_CQDB_INT_STS0               CQDB_INT_STS0               ; /* A134 */
    volatile U_CQDB_INT_STS1               CQDB_INT_STS1               ; /* A138 */
    volatile U_CQDB_INT_STS2               CQDB_INT_STS2               ; /* A13C */
    volatile U_CQDB_INT_STS3               CQDB_INT_STS3               ; /* A140 */
    volatile U_CQDB_INT_STS4               CQDB_INT_STS4               ; /* A144 */
    volatile U_CQDB_INT_STS5               CQDB_INT_STS5               ; /* A148 */
    volatile U_CQDB_INT_STS6               CQDB_INT_STS6               ; /* A14C */
    volatile U_CQDB_INT_STS7               CQDB_INT_STS7               ; /* A150 */
    volatile U_CQDB_INT_STS8               CQDB_INT_STS8               ; /* A154 */
    volatile U_CQDB_INT_STS9               CQDB_INT_STS9               ; /* A158 */
    volatile U_CQDB_INT_STS10              CQDB_INT_STS10              ; /* A15C */
    volatile U_CQDB_INT_STS11              CQDB_INT_STS11              ; /* A160 */
    volatile U_CQDB_INT_STS12              CQDB_INT_STS12              ; /* A164 */
    volatile U_CQDB_INT_STS13              CQDB_INT_STS13              ; /* A168 */
    volatile U_CQDB_INT_STS14              CQDB_INT_STS14              ; /* A16C */
    volatile U_CQDB_INT_STS15              CQDB_INT_STS15              ; /* A170 */
    volatile U_WRPTR_INT_STS0              WRPTR_INT_STS0              ; /* A174 */
    volatile U_WRPTR_INT_STS1              WRPTR_INT_STS1              ; /* A178 */
    volatile U_WRPTR_INT_STS2              WRPTR_INT_STS2              ; /* A17C */
    volatile U_WRPTR_INT_STS3              WRPTR_INT_STS3              ; /* A180 */
    volatile U_WRPTR_INT_STS4              WRPTR_INT_STS4              ; /* A184 */
    volatile U_WRPTR_INT_STS5              WRPTR_INT_STS5              ; /* A188 */
    volatile U_WRPTR_INT_STS6              WRPTR_INT_STS6              ; /* A18C */
    volatile U_WRPTR_INT_STS7              WRPTR_INT_STS7              ; /* A190 */
    volatile U_WRPTR_INT_STS8              WRPTR_INT_STS8              ; /* A194 */
    volatile U_WRPTR_INT_STS9              WRPTR_INT_STS9              ; /* A198 */
    volatile U_WRPTR_INT_STS10             WRPTR_INT_STS10             ; /* A19C */
    volatile U_WRPTR_INT_STS11             WRPTR_INT_STS11             ; /* A1A0 */
    volatile U_WRPTR_INT_STS12             WRPTR_INT_STS12             ; /* A1A4 */
    volatile U_WRPTR_INT_STS13             WRPTR_INT_STS13             ; /* A1A8 */
    volatile U_WRPTR_INT_STS14             WRPTR_INT_STS14             ; /* A1AC */
    volatile U_WRPTR_INT_STS15             WRPTR_INT_STS15             ; /* A1B0 */
    volatile U_SQDB_INT_MASK0              SQDB_INT_MASK0              ; /* A1F4 */
    volatile U_SQDB_INT_MASK1              SQDB_INT_MASK1              ; /* A1F8 */
    volatile U_SQDB_INT_MASK2              SQDB_INT_MASK2              ; /* A1FC */
    volatile U_SQDB_INT_MASK3              SQDB_INT_MASK3              ; /* A200 */
    volatile U_SQDB_INT_MASK4              SQDB_INT_MASK4              ; /* A204 */
    volatile U_SQDB_INT_MASK5              SQDB_INT_MASK5              ; /* A208 */
    volatile U_SQDB_INT_MASK6              SQDB_INT_MASK6              ; /* A20C */
    volatile U_SQDB_INT_MASK7              SQDB_INT_MASK7              ; /* A210 */
    volatile U_SQDB_INT_MASK8              SQDB_INT_MASK8              ; /* A214 */
    volatile U_SQDB_INT_MASK9              SQDB_INT_MASK9              ; /* A218 */
    volatile U_SQDB_INT_MASK10             SQDB_INT_MASK10             ; /* A21C */
    volatile U_SQDB_INT_MASK11             SQDB_INT_MASK11             ; /* A220 */
    volatile U_SQDB_INT_MASK12             SQDB_INT_MASK12             ; /* A224 */
    volatile U_SQDB_INT_MASK13             SQDB_INT_MASK13             ; /* A228 */
    volatile U_SQDB_INT_MASK14             SQDB_INT_MASK14             ; /* A22C */
    volatile U_SQDB_INT_MASK15             SQDB_INT_MASK15             ; /* A230 */
    volatile U_SQDB_INT_CFG0               SQDB_INT_CFG0               ; /* A234 */
    volatile U_SQDB_INT_CFG1               SQDB_INT_CFG1               ; /* A238 */
    volatile U_SQDB_INT_CFG2               SQDB_INT_CFG2               ; /* A23C */
    volatile U_SQDB_INT_CFG3               SQDB_INT_CFG3               ; /* A240 */
    volatile U_SQDB_INT_CFG4               SQDB_INT_CFG4               ; /* A244 */
    volatile U_SQDB_INT_CFG5               SQDB_INT_CFG5               ; /* A248 */
    volatile U_SQDB_INT_CFG6               SQDB_INT_CFG6               ; /* A24C */
    volatile U_SQDB_INT_CFG7               SQDB_INT_CFG7               ; /* A250 */
    volatile U_SQDB_INT_CFG8               SQDB_INT_CFG8               ; /* A254 */
    volatile U_SQDB_INT_CFG9               SQDB_INT_CFG9               ; /* A258 */
    volatile U_SQDB_INT_CFG10              SQDB_INT_CFG10              ; /* A25C */
    volatile U_SQDB_INT_CFG11              SQDB_INT_CFG11              ; /* A260 */
    volatile U_SQDB_INT_CFG12              SQDB_INT_CFG12              ; /* A264 */
    volatile U_SQDB_INT_CFG13              SQDB_INT_CFG13              ; /* A268 */
    volatile U_SQDB_INT_CFG14              SQDB_INT_CFG14              ; /* A26C */
    volatile U_SQDB_INT_CFG15              SQDB_INT_CFG15              ; /* A270 */
    volatile U_CQDB_INT_CFG0               CQDB_INT_CFG0               ; /* A274 */
    volatile U_CQDB_INT_CFG1               CQDB_INT_CFG1               ; /* A278 */
    volatile U_CQDB_INT_CFG2               CQDB_INT_CFG2               ; /* A27C */
    volatile U_CQDB_INT_CFG3               CQDB_INT_CFG3               ; /* A280 */
    volatile U_CQDB_INT_CFG4               CQDB_INT_CFG4               ; /* A284 */
    volatile U_CQDB_INT_CFG5               CQDB_INT_CFG5               ; /* A288 */
    volatile U_CQDB_INT_CFG6               CQDB_INT_CFG6               ; /* A28C */
    volatile U_CQDB_INT_CFG7               CQDB_INT_CFG7               ; /* A290 */
    volatile U_CQDB_INT_CFG8               CQDB_INT_CFG8               ; /* A294 */
    volatile U_CQDB_INT_CFG9               CQDB_INT_CFG9               ; /* A298 */
    volatile U_CQDB_INT_CFG10              CQDB_INT_CFG10              ; /* A29C */
    volatile U_CQDB_INT_CFG11              CQDB_INT_CFG11              ; /* A2A0 */
    volatile U_CQDB_INT_CFG12              CQDB_INT_CFG12              ; /* A2A4 */
    volatile U_CQDB_INT_CFG13              CQDB_INT_CFG13              ; /* A2A8 */
    volatile U_CQDB_INT_CFG14              CQDB_INT_CFG14              ; /* A2AC */
    volatile U_CQDB_INT_CFG15              CQDB_INT_CFG15              ; /* A2B0 */
    volatile U_WRPTR_DB_INT_CFG0           WRPTR_DB_INT_CFG0           ; /* A2B4 */
    volatile U_WRPTR_DB_INT_CFG1           WRPTR_DB_INT_CFG1           ; /* A2B8 */
    volatile U_WRPTR_DB_INT_CFG2           WRPTR_DB_INT_CFG2           ; /* A2BC */
    volatile U_WRPTR_DB_INT_CFG3           WRPTR_DB_INT_CFG3           ; /* A2C0 */
    volatile U_WRPTR_DB_INT_CFG4           WRPTR_DB_INT_CFG4           ; /* A2C4 */
    volatile U_WRPTR_DB_INT_CFG5           WRPTR_DB_INT_CFG5           ; /* A2C8 */
    volatile U_WRPTR_DB_INT_CFG6           WRPTR_DB_INT_CFG6           ; /* A2CC */
    volatile U_WRPTR_DB_INT_CFG7           WRPTR_DB_INT_CFG7           ; /* A2D0 */
    volatile U_WRPTR_DB_INT_CFG8           WRPTR_DB_INT_CFG8           ; /* A2D4 */
    volatile U_WRPTR_DB_INT_CFG9           WRPTR_DB_INT_CFG9           ; /* A2D8 */
    volatile U_WRPTR_DB_INT_CFG10          WRPTR_DB_INT_CFG10          ; /* A2DC */
    volatile U_WRPTR_DB_INT_CFG11          WRPTR_DB_INT_CFG11          ; /* A2E0 */
    volatile U_WRPTR_DB_INT_CFG12          WRPTR_DB_INT_CFG12          ; /* A2E4 */
    volatile U_WRPTR_DB_INT_CFG13          WRPTR_DB_INT_CFG13          ; /* A2E8 */
    volatile U_WRPTR_DB_INT_CFG14          WRPTR_DB_INT_CFG14          ; /* A2EC */
    volatile U_WRPTR_DB_INT_CFG15          WRPTR_DB_INT_CFG15          ; /* A2F0 */
    volatile U_NVME_INT_MOD_SEL            NVME_INT_MOD_SEL            ; /* A2F4 */
    volatile U_NVME_INT_LEVEL_SEL          NVME_INT_LEVEL_SEL          ; /* A2F8 */
    volatile U_NVME_QUEUE_DEL              NVME_QUEUE_DEL              ; /* A2FC */
    volatile U_SQ_ERR_STS0                 SQ_ERR_STS0                 ; /* A300 */
    volatile U_SQ_ERR_STS1                 SQ_ERR_STS1                 ; /* A304 */
    volatile U_RO_CFG                      RO_CFG                      ; /* A308 */
    volatile U_QUEUE_EN0                   QUEUE_EN0                   ; /* A30C */
    volatile U_QUEUE_EN1                   QUEUE_EN1                   ; /* A310 */
    volatile U_QUEUE_EN2                   QUEUE_EN2                   ; /* A314 */
    volatile U_QUEUE_EN3                   QUEUE_EN3                   ; /* A318 */
    volatile U_QUEUE_EN4                   QUEUE_EN4                   ; /* A31C */
    volatile U_QUEUE_EN5                   QUEUE_EN5                   ; /* A320 */
    volatile U_QUEUE_EN6                   QUEUE_EN6                   ; /* A324 */
    volatile U_QUEUE_EN7                   QUEUE_EN7                   ; /* A328 */
    volatile U_QUEUE_EN8                   QUEUE_EN8                   ; /* A32C */
    volatile U_QUEUE_EN9                   QUEUE_EN9                   ; /* A330 */
    volatile U_QUEUE_EN10                  QUEUE_EN10                  ; /* A334 */
    volatile U_QUEUE_EN11                  QUEUE_EN11                  ; /* A338 */
    volatile U_QUEUE_EN12                  QUEUE_EN12                  ; /* A33C */
    volatile U_QUEUE_EN13                  QUEUE_EN13                  ; /* A340 */
    volatile U_QUEUE_EN14                  QUEUE_EN14                  ; /* A344 */
    volatile U_QUEUE_EN15                  QUEUE_EN15                  ; /* A348 */
    volatile U_CQ_ERR_INT0                 CQ_ERR_INT0                 ; /* A34C */
    volatile U_CQ_ERR_INT1                 CQ_ERR_INT1                 ; /* A350 */
    volatile U_CQ_ERR_INT2                 CQ_ERR_INT2                 ; /* A354 */
    volatile U_CQ_ERR_INT3                 CQ_ERR_INT3                 ; /* A358 */
    volatile U_CQ_ERR_INT4                 CQ_ERR_INT4                 ; /* A35C */
    volatile U_CQ_ERR_INT5                 CQ_ERR_INT5                 ; /* A360 */
    volatile U_CQ_ERR_INT6                 CQ_ERR_INT6                 ; /* A364 */
    volatile U_CQ_ERR_INT7                 CQ_ERR_INT7                 ; /* A368 */
    volatile U_CQ_ERR_INT8                 CQ_ERR_INT8                 ; /* A36C */
    volatile U_CQ_ERR_INT9                 CQ_ERR_INT9                 ; /* A370 */
    volatile U_CQ_ERR_INT10                CQ_ERR_INT10                ; /* A374 */
    volatile U_CQ_ERR_INT11                CQ_ERR_INT11                ; /* A378 */
    volatile U_CQ_ERR_INT12                CQ_ERR_INT12                ; /* A37C */
    volatile U_CQ_ERR_INT13                CQ_ERR_INT13                ; /* A380 */
    volatile U_CQ_ERR_INT14                CQ_ERR_INT14                ; /* A384 */
    volatile U_CQ_ERR_INT15                CQ_ERR_INT15                ; /* A388 */
    volatile U_CQ_ERR_INT_CFG0             CQ_ERR_INT_CFG0             ; /* A38C */
    volatile U_CQ_ERR_INT_CFG1             CQ_ERR_INT_CFG1             ; /* A390 */
    volatile U_CQ_ERR_INT_CFG2             CQ_ERR_INT_CFG2             ; /* A394 */
    volatile U_CQ_ERR_INT_CFG3             CQ_ERR_INT_CFG3             ; /* A398 */
    volatile U_CQ_ERR_INT_CFG4             CQ_ERR_INT_CFG4             ; /* A39C */
    volatile U_CQ_ERR_INT_CFG5             CQ_ERR_INT_CFG5             ; /* A3A0 */
    volatile U_CQ_ERR_INT_CFG6             CQ_ERR_INT_CFG6             ; /* A3A4 */
    volatile U_CQ_ERR_INT_CFG7             CQ_ERR_INT_CFG7             ; /* A3A8 */
    volatile U_CQ_ERR_INT_CFG8             CQ_ERR_INT_CFG8             ; /* A3AC */
    volatile U_CQ_ERR_INT_CFG9             CQ_ERR_INT_CFG9             ; /* A3B0 */
    volatile U_CQ_ERR_INT_CFG10            CQ_ERR_INT_CFG10            ; /* A3B4 */
    volatile U_CQ_ERR_INT_CFG11            CQ_ERR_INT_CFG11            ; /* A3B8 */
    volatile U_CQ_ERR_INT_CFG12            CQ_ERR_INT_CFG12            ; /* A3BC */
    volatile U_CQ_ERR_INT_CFG13            CQ_ERR_INT_CFG13            ; /* A3C0 */
    volatile U_CQ_ERR_INT_CFG14            CQ_ERR_INT_CFG14            ; /* A3C4 */
    volatile U_CQ_ERR_INT_CFG15            CQ_ERR_INT_CFG15            ; /* A3C8 */
    volatile U_WR_PTR_BASE_ADDDR_LOW       WR_PTR_BASE_ADDDR_LOW       ; /* A400 */
    volatile U_WR_PTR_BASE_ADDDR_HIGH      WR_PTR_BASE_ADDDR_HIGH      ; /* A404 */
    volatile U_MEMORY_BASE_ADDDR_LOW       MEMORY_BASE_ADDDR_LOW       ; /* A408 */
    volatile U_MEMORY_BASE_ADDDR_HIGH      MEMORY_BASE_ADDDR_HIGH      ; /* A40C */
    volatile U_MEMORY_SPACE_LOW            MEMORY_SPACE_LOW            ; /* A410 */
    volatile U_MEMORY_SPACE_HIGH           MEMORY_SPACE_HIGH           ; /* A414 */
    volatile U_DB_STRD                     DB_STRD                     ; /* A418 */
    volatile U_VF_SIZE                     VF_SIZE                     ; /* A41C */
    volatile U_SQE_RD_MAX                  SQE_RD_MAX                  ; /* A420 */
    volatile U_RAM_INITIAL                 RAM_INITIAL                 ; /* A424 */
    volatile U_INT_TIME_THR                INT_TIME_THR                ; /* A428 */
    volatile U_DDR_STRD                    DDR_STRD                    ; /* A42C */
    volatile U_INT_REQ                     INT_REQ                     ; /* A430 */
    volatile U_NVME_VECTOR_PENDING         NVME_VECTOR_PENDING[16]     ; /* A434 */
    volatile U_NVME_VECTOR_MASK            NVME_VECTOR_MASK[16]        ; /* A474 */
    volatile U_IOSQS                       IOSQS[256]                  ; /* A800 */
    volatile U_IOSQ_BASE_ADDR_LOW          IOSQ_BASE_ADDR_LOW[256]     ; /* AC00 */
    volatile U_IOSQ_BASE_ADDR_HIGH         IOSQ_BASE_ADDR_HIGH[256]    ; /* AC04 */
    volatile U_SQR_MEMORY_RD_PTR           SQR_MEMORY_RD_PTR[256]      ; /* B400 */
    volatile U_SQ_SIZE_RAM_ECC_INJECT      SQ_SIZE_RAM_ECC_INJECT      ; /* C000 */
    volatile U_SQ_SIZE_RAM_ECC_ADDR        SQ_SIZE_RAM_ECC_ADDR        ; /* C004 */
    volatile U_SQ_BAL_RAM_ECC_INJECT       SQ_BAL_RAM_ECC_INJECT       ; /* C008 */
    volatile U_SQ_BAL_RAM_ECC_ADDR         SQ_BAL_RAM_ECC_ADDR         ; /* C00C */
    volatile U_SQ_BAH_RAM_ECC_INJECT       SQ_BAH_RAM_ECC_INJECT       ; /* C0010 */
    volatile U_SQ_BAH_RAM_ECC_ADDR         SQ_BAH_RAM_ECC_ADDR         ; /* C0014 */
    volatile U_RD_PTR_RAM_ECC_INJECT       RD_PTR_RAM_ECC_INJECT       ; /* C0018 */
    volatile U_RD_PTR_RAM_ECC_ADDR         RD_PTR_RAM_ECC_ADDR         ; /* C001C */
    volatile U_SQ_DES_RAM_ECC_INJECT       SQ_DES_RAM_ECC_INJECT       ; /* C020 */
    volatile U_SQ_DES_RAM_ECC_ADDR         SQ_DES_RAM_ECC_ADDR         ; /* C024 */
    volatile U_PRFETCH_FIFO_RAM_ECC_INJECT PRFETCH_FIFO_RAM_ECC_INJECT ; /* C028 */
    volatile U_PRFETCH_FIFO_RAM_ECC_ADDR   PRFETCH_FIFO_RAM_ECC_ADDR   ; /* C02C */
    volatile U_NVME_LINK_DOWN_PROC         NVME_LINK_DOWN_PROC         ; /* CC00 */
    volatile U_NVME_FLR_STS                NVME_FLR_STS                ; /* CD00 */
    volatile U_NVME_DMACQ_ERR_STS          NVME_DMACQ_ERR_STS          ; /* CE00 */
    volatile U_NVME_SQ_DES_RAM_CFG         NVME_SQ_DES_RAM_CFG         ; /* CF00 */
    volatile U_NVME_SQ_DES_RAM_WDAT_L      NVME_SQ_DES_RAM_WDAT_L      ; /* CF04 */
    volatile U_NVME_SQ_DES_RAM_WDAT_H      NVME_SQ_DES_RAM_WDAT_H      ; /* CF08 */
    volatile U_NVME_SQ_DES_RAM_RDAT_L      NVME_SQ_DES_RAM_RDAT_L      ; /* CF0C */
    volatile U_NVME_SQ_DES_RAM_RDAT_H      NVME_SQ_DES_RAM_RDAT_H      ; /* CF10 */
    volatile U_NVME_FIFO_STS               NVME_FIFO_STS               ; /* CF14 */
    volatile U_NVME_FSM_STS                NVME_FSM_STS                ; /* CF18 */
    volatile U_NVME_FLOW_STS0              NVME_FLOW_STS0              ; /* CF1C */
    volatile U_NVME_FLOW_STS1              NVME_FLOW_STS1              ; /* CF20 */
    volatile U_NVME_QUEUE_STS0             NVME_QUEUE_STS0             ; /* CF24 */
    volatile U_NVME_QUEUE_STS1             NVME_QUEUE_STS1             ; /* CF28 */
    volatile U_NVME_QUEUE_STS2             NVME_QUEUE_STS2             ; /* CF2C */
    volatile U_NVME_QUEUE_STS3             NVME_QUEUE_STS3             ; /* CF30 */
    volatile U_NVME_QUEUE_STS4             NVME_QUEUE_STS4             ; /* CF34 */
    volatile U_NVME_QUEUE_STS5             NVME_QUEUE_STS5             ; /* CF38 */
    volatile U_NVME_QUEUE_STS6             NVME_QUEUE_STS6             ; /* CF3C */
    volatile U_NVME_QUEUE_STS7             NVME_QUEUE_STS7             ; /* CF40 */

} S_hipciec_nvme_global_reg_REGS_TYPE;

/* Declare the struct pointor of the module hipciec_nvme_global_reg */
extern volatile S_hipciec_nvme_global_reg_REGS_TYPE *gophipciec_nvme_global_regAllReg;

/* Declare the functions that set the member value */
int iSetPF_WR_CTRL_REG_INT_STS_wr_cc_int_mask(unsigned int uwr_cc_int_mask);
int iSetPF_WR_CTRL_REG_INT_STS_wr_aqa_int_mask(unsigned int uwr_aqa_int_mask);
int iSetPF_WR_CTRL_REG_INT_STS_wr_asq_int_mask(unsigned int uwr_asq_int_mask);
int iSetPF_WR_CTRL_REG_INT_STS_wr_cc_int_sts(unsigned int uwr_cc_int_sts);
int iSetPF_WR_CTRL_REG_INT_STS_wr_aqa_int_sts(unsigned int uwr_aqa_int_sts);
int iSetPF_WR_CTRL_REG_INT_STS_wr_asq_int_sts(unsigned int uwr_asq_int_sts);
int iSetVF_WR_CTRL_REG_INT_STS_wr_cc_int_mask(unsigned int uwr_cc_int_mask);
int iSetVF_WR_CTRL_REG_INT_STS_wr_aqa_int_mask(unsigned int uwr_aqa_int_mask);
int iSetVF_WR_CTRL_REG_INT_STS_wr_asq_int_mask(unsigned int uwr_asq_int_mask);
int iSetVF_WR_CTRL_REG_INT_STS_wr_cc_int_sts(unsigned int uwr_cc_int_sts);
int iSetVF_WR_CTRL_REG_INT_STS_wr_aqa_int_sts(unsigned int uwr_aqa_int_sts);
int iSetVF_WR_CTRL_REG_INT_STS_wr_asq_int_sts(unsigned int uwr_asq_int_sts);
int iSetNVME_CTRL_MISC_int_coal_en(unsigned int uint_coal_en);
int iSetSRIOV_MODE_CTRL_sriov_mode_sel(unsigned int usriov_mode_sel);
int iSetPF2_BAR0_DB_ATU_LOW_pf2_bar0_db_atu_low(unsigned int upf2_bar0_db_atu_low);
int iSetPF2_BAR0_DB_ATU_HIGH_pf2_bar0_db_atu_high(unsigned int upf2_bar0_db_atu_high);
int iSetPF2_VF_BAR0_DB_ATU_LOW_pf2_vf_bar0_db_atu_low(unsigned int upf2_vf_bar0_db_atu_low);
int iSetPF2_VF_BAR0_DB_ATU_HIGH_pf2_vf_bar0_db_atu_high(unsigned int upf2_vf_bar0_db_atu_high);
int iSetPF3_BAR0_DB_ATU_LOW_pf3_bar0_db_atu_low(unsigned int upf3_bar0_db_atu_low);
int iSetPF3_BAR0_DB_ATU_HIGH_pf3_bar0_db_atu_high(unsigned int upf3_bar0_db_atu_high);
int iSetPF3_VF_BAR0_DB_ATU_LOW_pf3_vf_bar0_db_atu_low(unsigned int upf3_vf_bar0_db_atu_low);
int iSetPF3_VF_BAR0_DB_ATU_HIGH_pf3_vf_bar0_db_atu_high(unsigned int upf3_vf_bar0_db_atu_high);
int iSetINT_COAL_TIME_CNT_UNIT_time_cnt_unit(unsigned int utime_cnt_unit);
int iSetINT_COAL_MASK0_int_coal_mask0(unsigned int uint_coal_mask0);
int iSetINT_COAL_MASK1_int_coal_mask1(unsigned int uint_coal_mask1);
int iSetINT_COAL_MASK2_int_coal_mask2(unsigned int uint_coal_mask2);
int iSetINT_COAL_MASK3_int_coal_mask3(unsigned int uint_coal_mask3);
int iSetINT_COAL_MASK4_int_coal_mask4(unsigned int uint_coal_mask4);
int iSetINT_COAL_MASK5_int_coal_mask5(unsigned int uint_coal_mask5);
int iSetINT_COAL_MASK6_int_coal_mask6(unsigned int uint_coal_mask6);
int iSetINT_COAL_MASK7_int_coal_mask7(unsigned int uint_coal_mask7);
int iSetINT_COAL_MASK8_int_coal_mask8(unsigned int uint_coal_mask8);
int iSetINT_COAL_MASK9_int_coal_mask9(unsigned int uint_coal_mask9);
int iSetINT_COAL_MASK10_int_coal_mask10(unsigned int uint_coal_mask10);
int iSetINT_COAL_MASK11_int_coal_mask11(unsigned int uint_coal_mask11);
int iSetINT_COAL_MASK12_int_coal_mask12(unsigned int uint_coal_mask12);
int iSetINT_COAL_MASK13_int_coal_mask13(unsigned int uint_coal_mask13);
int iSetINT_COAL_MASK14_int_coal_mask14(unsigned int uint_coal_mask14);
int iSetINT_COAL_MASK15_int_coal_mask15(unsigned int uint_coal_mask15);
int iSetCQDB_INT_MASK0_cq_doorbell_mask0(unsigned int ucq_doorbell_mask0);
int iSetCQDB_INT_MASK1_cq_doorbell_mask1(unsigned int ucq_doorbell_mask1);
int iSetCQDB_INT_MASK2_cq_doorbell_mask2(unsigned int ucq_doorbell_mask2);
int iSetCQDB_INT_MASK3_cq_doorbell_mask3(unsigned int ucq_doorbell_mask3);
int iSetCQDB_INT_MASK4_cq_doorbell_mask4(unsigned int ucq_doorbell_mask4);
int iSetCQDB_INT_MASK5_cq_doorbell_mask5(unsigned int ucq_doorbell_mask5);
int iSetCQDB_INT_MASK6_cq_doorbell_mask6(unsigned int ucq_doorbell_mask6);
int iSetCQDB_INT_MASK7_cq_doorbell_mask7(unsigned int ucq_doorbell_mask7);
int iSetCQDB_INT_MASK8_cq_doorbell_mask8(unsigned int ucq_doorbell_mask8);
int iSetCQDB_INT_MASK9_cq_doorbell_mask9(unsigned int ucq_doorbell_mask9);
int iSetCQDB_INT_MASK10_cq_doorbell_mask10(unsigned int ucq_doorbell_mask10);
int iSetCQDB_INT_MASK11_cq_doorbell_mask11(unsigned int ucq_doorbell_mask11);
int iSetCQDB_INT_MASK12_cq_doorbell_mask12(unsigned int ucq_doorbell_mask12);
int iSetCQDB_INT_MASK13_cq_doorbell_mask13(unsigned int ucq_doorbell_mask13);
int iSetCQDB_INT_MASK14_cq_doorbell_mask14(unsigned int ucq_doorbell_mask14);
int iSetCQDB_INT_MASK15_cq_doorbell_mask15(unsigned int ucq_doorbell_mask15);
int iSetWRPTR_INT_MASK0_wrptr_mask0(unsigned int uwrptr_mask0);
int iSetWRPTR_INT_MASK1_wrptr_mask1(unsigned int uwrptr_mask1);
int iSetWRPTR_INT_MASK2_wrptr_mask2(unsigned int uwrptr_mask2);
int iSetWRPTR_INT_MASK3_wrptr_mask3(unsigned int uwrptr_mask3);
int iSetWRPTR_INT_MASK4_wrptr_mask4(unsigned int uwrptr_mask4);
int iSetWRPTR_INT_MASK5_wrptr_mask5(unsigned int uwrptr_mask5);
int iSetWRPTR_INT_MASK6_wrptr_mask6(unsigned int uwrptr_mask6);
int iSetWRPTR_INT_MASK7_wrptr_mask7(unsigned int uwrptr_mask7);
int iSetDB_INT_COAL_CFG_pf3_cfg_int_aggre_time(unsigned int upf3_cfg_int_aggre_time);
int iSetDB_INT_COAL_CFG_pf3_cfg_int_aggre_thr(unsigned int upf3_cfg_int_aggre_thr);
int iSetDB_INT_COAL_CFG_pf2_cfg_int_aggre_time(unsigned int upf2_cfg_int_aggre_time);
int iSetDB_INT_COAL_CFG_pf2_cfg_int_aggre_thr(unsigned int upf2_cfg_int_aggre_thr);
int iSetENG_WORK_MODE_SEL_eng_work_mode_sel(unsigned int ueng_work_mode_sel);
int iSetWRPTR_INT_MASK8_wrptr_mask8(unsigned int uwrptr_mask8);
int iSetWRPTR_INT_MASK9_wrptr_mask9(unsigned int uwrptr_mask9);
int iSetWRPTR_INT_MASK10_wrptr_mask10(unsigned int uwrptr_mask10);
int iSetWRPTR_INT_MASK11_wrptr_mask11(unsigned int uwrptr_mask11);
int iSetWRPTR_INT_MASK12_wrptr_mask12(unsigned int uwrptr_mask12);
int iSetWRPTR_INT_MASK13_wrptr_mask13(unsigned int uwrptr_mask13);
int iSetWRPTR_INT_MASK14_wrptr_mask14(unsigned int uwrptr_mask14);
int iSetWRPTR_INT_MASK15_wrptr_mask15(unsigned int uwrptr_mask15);
int iSetSQDB_INT_STS0_sqdb_int_sts0(unsigned int usqdb_int_sts0);
int iSetSQDB_INT_STS1_sqdb_int_sts1(unsigned int usqdb_int_sts1);
int iSetSQDB_INT_STS2_sqdb_int_sts2(unsigned int usqdb_int_sts2);
int iSetSQDB_INT_STS3_sqdb_int_sts3(unsigned int usqdb_int_sts3);
int iSetSQDB_INT_STS4_sqdb_int_sts4(unsigned int usqdb_int_sts4);
int iSetSQDB_INT_STS5_sqdb_int_sts5(unsigned int usqdb_int_sts5);
int iSetSQDB_INT_STS6_sqdb_int_sts6(unsigned int usqdb_int_sts6);
int iSetSQDB_INT_STS7_sqdb_int_sts7(unsigned int usqdb_int_sts7);
int iSetSQDB_INT_STS8_sqdb_int_sts8(unsigned int usqdb_int_sts8);
int iSetSQDB_INT_STS9_sqdb_int_sts9(unsigned int usqdb_int_sts9);
int iSetSQDB_INT_STS10_sqdb_int_sts10(unsigned int usqdb_int_sts10);
int iSetSQDB_INT_STS11_sqdb_int_sts11(unsigned int usqdb_int_sts11);
int iSetSQDB_INT_STS12_sqdb_int_sts12(unsigned int usqdb_int_sts12);
int iSetSQDB_INT_STS13_sqdb_int_sts13(unsigned int usqdb_int_sts13);
int iSetSQDB_INT_STS14_sqdb_int_sts14(unsigned int usqdb_int_sts14);
int iSetSQDB_INT_STS15_sqdb_int_sts15(unsigned int usqdb_int_sts15);
int iSetCQDB_INT_STS0_cq_doorbell_sts0(unsigned int ucq_doorbell_sts0);
int iSetCQDB_INT_STS1_cq_doorbell_sts1(unsigned int ucq_doorbell_sts1);
int iSetCQDB_INT_STS2_cq_doorbell_sts2(unsigned int ucq_doorbell_sts2);
int iSetCQDB_INT_STS3_cq_doorbell_sts3(unsigned int ucq_doorbell_sts3);
int iSetCQDB_INT_STS4_cq_doorbell_sts4(unsigned int ucq_doorbell_sts4);
int iSetCQDB_INT_STS5_cq_doorbell_sts5(unsigned int ucq_doorbell_sts5);
int iSetCQDB_INT_STS6_cq_doorbell_sts6(unsigned int ucq_doorbell_sts6);
int iSetCQDB_INT_STS7_cq_doorbell_sts7(unsigned int ucq_doorbell_sts7);
int iSetCQDB_INT_STS8_cq_doorbell_sts8(unsigned int ucq_doorbell_sts8);
int iSetCQDB_INT_STS9_cq_doorbell_sts9(unsigned int ucq_doorbell_sts9);
int iSetCQDB_INT_STS10_cq_doorbell_sts10(unsigned int ucq_doorbell_sts10);
int iSetCQDB_INT_STS11_cq_doorbell_sts11(unsigned int ucq_doorbell_sts11);
int iSetCQDB_INT_STS12_cq_doorbell_sts12(unsigned int ucq_doorbell_sts12);
int iSetCQDB_INT_STS13_cq_doorbell_sts13(unsigned int ucq_doorbell_sts13);
int iSetCQDB_INT_STS14_cq_doorbell_sts14(unsigned int ucq_doorbell_sts14);
int iSetCQDB_INT_STS15_cq_doorbell_sts15(unsigned int ucq_doorbell_sts15);
int iSetWRPTR_INT_STS0_wrptr_sts0(unsigned int uwrptr_sts0);
int iSetWRPTR_INT_STS1_wrptr_sts1(unsigned int uwrptr_sts1);
int iSetWRPTR_INT_STS2_wrptr_sts2(unsigned int uwrptr_sts2);
int iSetWRPTR_INT_STS3_wrptr_sts3(unsigned int uwrptr_sts3);
int iSetWRPTR_INT_STS4_wrptr_sts4(unsigned int uwrptr_sts4);
int iSetWRPTR_INT_STS5_wrptr_sts5(unsigned int uwrptr_sts5);
int iSetWRPTR_INT_STS6_wrptr_sts6(unsigned int uwrptr_sts6);
int iSetWRPTR_INT_STS7_wrptr_sts7(unsigned int uwrptr_sts7);
int iSetWRPTR_INT_STS8_wrptr_sts8(unsigned int uwrptr_sts8);
int iSetWRPTR_INT_STS9_wrptr_sts9(unsigned int uwrptr_sts9);
int iSetWRPTR_INT_STS10_wrptr_sts10(unsigned int uwrptr_sts10);
int iSetWRPTR_INT_STS11_wrptr_sts11(unsigned int uwrptr_sts11);
int iSetWRPTR_INT_STS12_wrptr_sts12(unsigned int uwrptr_sts12);
int iSetWRPTR_INT_STS13_wrptr_sts13(unsigned int uwrptr_sts13);
int iSetWRPTR_INT_STS14_wrptr_sts14(unsigned int uwrptr_sts14);
int iSetWRPTR_INT_STS15_wrptr_sts15(unsigned int uwrptr_sts15);
int iSetSQDB_INT_MASK0_sq_doorbell_mask0(unsigned int usq_doorbell_mask0);
int iSetSQDB_INT_MASK1_sq_doorbell_mask1(unsigned int usq_doorbell_mask1);
int iSetSQDB_INT_MASK2_sq_doorbell_mask2(unsigned int usq_doorbell_mask2);
int iSetSQDB_INT_MASK3_sq_doorbell_mask3(unsigned int usq_doorbell_mask3);
int iSetSQDB_INT_MASK4_sq_doorbell_mask4(unsigned int usq_doorbell_mask4);
int iSetSQDB_INT_MASK5_sq_doorbell_mask5(unsigned int usq_doorbell_mask5);
int iSetSQDB_INT_MASK6_sq_doorbell_mask6(unsigned int usq_doorbell_mask6);
int iSetSQDB_INT_MASK7_sq_doorbell_mask7(unsigned int usq_doorbell_mask7);
int iSetSQDB_INT_MASK8_sq_doorbell_mask8(unsigned int usq_doorbell_mask8);
int iSetSQDB_INT_MASK9_sq_doorbell_mask9(unsigned int usq_doorbell_mask9);
int iSetSQDB_INT_MASK10_sq_doorbell_mask10(unsigned int usq_doorbell_mask10);
int iSetSQDB_INT_MASK11_sq_doorbell_mask11(unsigned int usq_doorbell_mask11);
int iSetSQDB_INT_MASK12_sq_doorbell_mask12(unsigned int usq_doorbell_mask12);
int iSetSQDB_INT_MASK13_sq_doorbell_mask13(unsigned int usq_doorbell_mask13);
int iSetSQDB_INT_MASK14_sq_doorbell_mask14(unsigned int usq_doorbell_mask14);
int iSetSQDB_INT_MASK15_sq_doorbell_mask15(unsigned int usq_doorbell_mask15);
int iSetSQDB_INT_CFG0_sq_doorbell_ro0(unsigned int usq_doorbell_ro0);
int iSetSQDB_INT_CFG0_sq_doorbell_set0(unsigned int usq_doorbell_set0);
int iSetSQDB_INT_CFG1_sq_doorbell_ro1(unsigned int usq_doorbell_ro1);
int iSetSQDB_INT_CFG1_sq_doorbell_set1(unsigned int usq_doorbell_set1);
int iSetSQDB_INT_CFG2_sq_doorbell_ro2(unsigned int usq_doorbell_ro2);
int iSetSQDB_INT_CFG2_sq_doorbell_set2(unsigned int usq_doorbell_set2);
int iSetSQDB_INT_CFG3_sq_doorbell_ro3(unsigned int usq_doorbell_ro3);
int iSetSQDB_INT_CFG3_sq_doorbell_set3(unsigned int usq_doorbell_set3);
int iSetSQDB_INT_CFG4_sq_doorbell_ro4(unsigned int usq_doorbell_ro4);
int iSetSQDB_INT_CFG4_sq_doorbell_set4(unsigned int usq_doorbell_set4);
int iSetSQDB_INT_CFG5_sq_doorbell_ro5(unsigned int usq_doorbell_ro5);
int iSetSQDB_INT_CFG5_sq_doorbell_set5(unsigned int usq_doorbell_set5);
int iSetSQDB_INT_CFG6_sq_doorbell_ro6(unsigned int usq_doorbell_ro6);
int iSetSQDB_INT_CFG6_sq_doorbell_set6(unsigned int usq_doorbell_set6);
int iSetSQDB_INT_CFG7_sq_doorbell_ro7(unsigned int usq_doorbell_ro7);
int iSetSQDB_INT_CFG7_sq_doorbell_set7(unsigned int usq_doorbell_set7);
int iSetSQDB_INT_CFG8_sq_doorbell_ro8(unsigned int usq_doorbell_ro8);
int iSetSQDB_INT_CFG8_sq_doorbell_set8(unsigned int usq_doorbell_set8);
int iSetSQDB_INT_CFG9_sq_doorbell_ro9(unsigned int usq_doorbell_ro9);
int iSetSQDB_INT_CFG9_sq_doorbell_set9(unsigned int usq_doorbell_set9);
int iSetSQDB_INT_CFG10_sq_doorbell_ro10(unsigned int usq_doorbell_ro10);
int iSetSQDB_INT_CFG10_sq_doorbell_set10(unsigned int usq_doorbell_set10);
int iSetSQDB_INT_CFG11_sq_doorbell_ro11(unsigned int usq_doorbell_ro11);
int iSetSQDB_INT_CFG11_sq_doorbell_set11(unsigned int usq_doorbell_set11);
int iSetSQDB_INT_CFG12_sq_doorbell_ro12(unsigned int usq_doorbell_ro12);
int iSetSQDB_INT_CFG12_sq_doorbell_set12(unsigned int usq_doorbell_set12);
int iSetSQDB_INT_CFG13_sq_doorbell_ro13(unsigned int usq_doorbell_ro13);
int iSetSQDB_INT_CFG13_sq_doorbell_set13(unsigned int usq_doorbell_set13);
int iSetSQDB_INT_CFG14_sq_doorbell_ro14(unsigned int usq_doorbell_ro14);
int iSetSQDB_INT_CFG14_sq_doorbell_set14(unsigned int usq_doorbell_set14);
int iSetSQDB_INT_CFG15_sq_doorbell_ro15(unsigned int usq_doorbell_ro15);
int iSetSQDB_INT_CFG15_sq_doorbell_set15(unsigned int usq_doorbell_set15);
int iSetCQDB_INT_CFG0_cq_doorbell_ro0(unsigned int ucq_doorbell_ro0);
int iSetCQDB_INT_CFG0_cq_doorbell_set0(unsigned int ucq_doorbell_set0);
int iSetCQDB_INT_CFG1_cq_doorbell_ro1(unsigned int ucq_doorbell_ro1);
int iSetCQDB_INT_CFG1_cq_doorbell_set1(unsigned int ucq_doorbell_set1);
int iSetCQDB_INT_CFG2_cq_doorbell_ro2(unsigned int ucq_doorbell_ro2);
int iSetCQDB_INT_CFG2_cq_doorbell_set2(unsigned int ucq_doorbell_set2);
int iSetCQDB_INT_CFG3_cq_doorbell_ro3(unsigned int ucq_doorbell_ro3);
int iSetCQDB_INT_CFG3_cq_doorbell_set3(unsigned int ucq_doorbell_set3);
int iSetCQDB_INT_CFG4_cq_doorbell_ro4(unsigned int ucq_doorbell_ro4);
int iSetCQDB_INT_CFG4_cq_doorbell_set4(unsigned int ucq_doorbell_set4);
int iSetCQDB_INT_CFG5_cq_doorbell_ro5(unsigned int ucq_doorbell_ro5);
int iSetCQDB_INT_CFG5_cq_doorbell_set5(unsigned int ucq_doorbell_set5);
int iSetCQDB_INT_CFG6_cq_doorbell_ro6(unsigned int ucq_doorbell_ro6);
int iSetCQDB_INT_CFG6_cq_doorbell_set6(unsigned int ucq_doorbell_set6);
int iSetCQDB_INT_CFG7_cq_doorbell_ro7(unsigned int ucq_doorbell_ro7);
int iSetCQDB_INT_CFG7_cq_doorbell_set7(unsigned int ucq_doorbell_set7);
int iSetCQDB_INT_CFG8_cq_doorbell_ro8(unsigned int ucq_doorbell_ro8);
int iSetCQDB_INT_CFG8_cq_doorbell_set8(unsigned int ucq_doorbell_set8);
int iSetCQDB_INT_CFG9_cq_doorbell_ro9(unsigned int ucq_doorbell_ro9);
int iSetCQDB_INT_CFG9_cq_doorbell_set9(unsigned int ucq_doorbell_set9);
int iSetCQDB_INT_CFG10_cq_doorbell_ro10(unsigned int ucq_doorbell_ro10);
int iSetCQDB_INT_CFG10_cq_doorbell_set10(unsigned int ucq_doorbell_set10);
int iSetCQDB_INT_CFG11_cq_doorbell_ro11(unsigned int ucq_doorbell_ro11);
int iSetCQDB_INT_CFG11_cq_doorbell_set11(unsigned int ucq_doorbell_set11);
int iSetCQDB_INT_CFG12_cq_doorbell_ro12(unsigned int ucq_doorbell_ro12);
int iSetCQDB_INT_CFG12_cq_doorbell_set12(unsigned int ucq_doorbell_set12);
int iSetCQDB_INT_CFG13_cq_doorbell_ro13(unsigned int ucq_doorbell_ro13);
int iSetCQDB_INT_CFG13_cq_doorbell_set13(unsigned int ucq_doorbell_set13);
int iSetCQDB_INT_CFG14_cq_doorbell_ro14(unsigned int ucq_doorbell_ro14);
int iSetCQDB_INT_CFG14_cq_doorbell_set14(unsigned int ucq_doorbell_set14);
int iSetCQDB_INT_CFG15_cq_doorbell_ro15(unsigned int ucq_doorbell_ro15);
int iSetCQDB_INT_CFG15_cq_doorbell_set15(unsigned int ucq_doorbell_set15);
int iSetWRPTR_DB_INT_CFG0_wrptr_doorbell_ro0(unsigned int uwrptr_doorbell_ro0);
int iSetWRPTR_DB_INT_CFG0_wrptr_doorbell_set0(unsigned int uwrptr_doorbell_set0);
int iSetWRPTR_DB_INT_CFG1_wrptr_doorbell_ro1(unsigned int uwrptr_doorbell_ro1);
int iSetWRPTR_DB_INT_CFG1_wrptr_doorbell_set1(unsigned int uwrptr_doorbell_set1);
int iSetWRPTR_DB_INT_CFG2_wrptr_doorbell_ro2(unsigned int uwrptr_doorbell_ro2);
int iSetWRPTR_DB_INT_CFG2_wrptr_doorbell_set2(unsigned int uwrptr_doorbell_set2);
int iSetWRPTR_DB_INT_CFG3_wrptr_doorbell_ro3(unsigned int uwrptr_doorbell_ro3);
int iSetWRPTR_DB_INT_CFG3_wrptr_doorbell_set3(unsigned int uwrptr_doorbell_set3);
int iSetWRPTR_DB_INT_CFG4_wrptr_doorbell_ro4(unsigned int uwrptr_doorbell_ro4);
int iSetWRPTR_DB_INT_CFG4_wrptr_doorbell_set4(unsigned int uwrptr_doorbell_set4);
int iSetWRPTR_DB_INT_CFG5_wrptr_doorbell_ro5(unsigned int uwrptr_doorbell_ro5);
int iSetWRPTR_DB_INT_CFG5_wrptr_doorbell_set5(unsigned int uwrptr_doorbell_set5);
int iSetWRPTR_DB_INT_CFG6_wrptr_doorbell_ro6(unsigned int uwrptr_doorbell_ro6);
int iSetWRPTR_DB_INT_CFG6_wrptr_doorbell_set6(unsigned int uwrptr_doorbell_set6);
int iSetWRPTR_DB_INT_CFG7_wrptr_doorbell_ro7(unsigned int uwrptr_doorbell_ro7);
int iSetWRPTR_DB_INT_CFG7_wrptr_doorbell_set7(unsigned int uwrptr_doorbell_set7);
int iSetWRPTR_DB_INT_CFG8_wrptr_doorbell_ro8(unsigned int uwrptr_doorbell_ro8);
int iSetWRPTR_DB_INT_CFG8_wrptr_doorbell_set8(unsigned int uwrptr_doorbell_set8);
int iSetWRPTR_DB_INT_CFG9_wrptr_doorbell_ro9(unsigned int uwrptr_doorbell_ro9);
int iSetWRPTR_DB_INT_CFG9_wrptr_doorbell_set9(unsigned int uwrptr_doorbell_set9);
int iSetWRPTR_DB_INT_CFG10_wrptr_doorbell_ro10(unsigned int uwrptr_doorbell_ro10);
int iSetWRPTR_DB_INT_CFG10_wrptr_doorbell_set10(unsigned int uwrptr_doorbell_set10);
int iSetWRPTR_DB_INT_CFG11_wrptr_doorbell_ro11(unsigned int uwrptr_doorbell_ro11);
int iSetWRPTR_DB_INT_CFG11_wrptr_doorbell_set11(unsigned int uwrptr_doorbell_set11);
int iSetWRPTR_DB_INT_CFG12_wrptr_doorbell_ro12(unsigned int uwrptr_doorbell_ro12);
int iSetWRPTR_DB_INT_CFG12_wrptr_doorbell_set12(unsigned int uwrptr_doorbell_set12);
int iSetWRPTR_DB_INT_CFG13_wrptr_doorbell_ro13(unsigned int uwrptr_doorbell_ro13);
int iSetWRPTR_DB_INT_CFG13_wrptr_doorbell_set13(unsigned int uwrptr_doorbell_set13);
int iSetWRPTR_DB_INT_CFG14_wrptr_doorbell_ro14(unsigned int uwrptr_doorbell_ro14);
int iSetWRPTR_DB_INT_CFG14_wrptr_doorbell_set14(unsigned int uwrptr_doorbell_set14);
int iSetWRPTR_DB_INT_CFG15_wrptr_doorbell_ro15(unsigned int uwrptr_doorbell_ro15);
int iSetWRPTR_DB_INT_CFG15_wrptr_doorbell_set15(unsigned int uwrptr_doorbell_set15);
int iSetNVME_INT_MOD_SEL_nvme_nomral_int_mode(unsigned int unvme_nomral_int_mode);
int iSetNVME_INT_MOD_SEL_nvme_abnormal_int_mode(unsigned int unvme_abnormal_int_mode);
int iSetNVME_INT_LEVEL_SEL_nvme_sq_err_level_cfg(unsigned int unvme_sq_err_level_cfg);
int iSetNVME_INT_LEVEL_SEL_nvme_2bit_ecc_level_cfg(unsigned int unvme_2bit_ecc_level_cfg);
int iSetNVME_INT_LEVEL_SEL_nvme_1bit_ecc_level_cfg(unsigned int unvme_1bit_ecc_level_cfg);
int iSetNVME_INT_LEVEL_SEL_nvme_cq_err_int_level_cfg(unsigned int unvme_cq_err_int_level_cfg);
int iSetNVME_QUEUE_DEL_nvme_queue_del(unsigned int unvme_queue_del);
int iSetNVME_QUEUE_DEL_nvme_queue_del_num(unsigned int unvme_queue_del_num);
int iSetSQ_ERR_STS0_sqh_int_sts(unsigned int usqh_int_sts);
int iSetSQ_ERR_STS0_sqt_int_sts(unsigned int usqt_int_sts);
int iSetSQ_ERR_STS0_sqh_int_mask(unsigned int usqh_int_mask);
int iSetSQ_ERR_STS0_sqt_int_mask(unsigned int usqt_int_mask);
int iSetSQ_ERR_STS0_sqh_int_ro(unsigned int usqh_int_ro);
int iSetSQ_ERR_STS0_sqt_int_ro(unsigned int usqt_int_ro);
int iSetSQ_ERR_STS0_sqh_int_set(unsigned int usqh_int_set);
int iSetSQ_ERR_STS0_sqt_int_set(unsigned int usqt_int_set);
int iSetSQ_ERR_STS0_sqdb_err_queue_num(unsigned int usqdb_err_queue_num);
int iSetSQ_ERR_STS1_sq_err_head_ptr(unsigned int usq_err_head_ptr);
int iSetSQ_ERR_STS1_sq_err_tail_ptr(unsigned int usq_err_tail_ptr);
int iSetRO_CFG_sq_des_ro_cfg(unsigned int usq_des_ro_cfg);
int iSetRO_CFG_wrptr_ro_cfg(unsigned int uwrptr_ro_cfg);
int iSetQUEUE_EN0_queue_enable0(unsigned int uqueue_enable0);
int iSetQUEUE_EN1_queue_enable1(unsigned int uqueue_enable1);
int iSetQUEUE_EN2_queue_enable2(unsigned int uqueue_enable2);
int iSetQUEUE_EN3_queue_enable3(unsigned int uqueue_enable3);
int iSetQUEUE_EN4_queue_enable4(unsigned int uqueue_enable4);
int iSetQUEUE_EN5_queue_enable5(unsigned int uqueue_enable5);
int iSetQUEUE_EN6_queue_enable6(unsigned int uqueue_enable6);
int iSetQUEUE_EN7_queue_enable7(unsigned int uqueue_enable7);
int iSetQUEUE_EN8_queue_enable8(unsigned int uqueue_enable8);
int iSetQUEUE_EN9_queue_enable9(unsigned int uqueue_enable9);
int iSetQUEUE_EN10_queue_enable10(unsigned int uqueue_enable10);
int iSetQUEUE_EN11_queue_enable11(unsigned int uqueue_enable11);
int iSetQUEUE_EN12_queue_enable12(unsigned int uqueue_enable12);
int iSetQUEUE_EN13_queue_enable13(unsigned int uqueue_enable13);
int iSetQUEUE_EN14_queue_enable14(unsigned int uqueue_enable14);
int iSetQUEUE_EN15_queue_enable15(unsigned int uqueue_enable15);
int iSetCQ_ERR_INT0_cq_err_int_sts(unsigned int ucq_err_int_sts);
int iSetCQ_ERR_INT0_cq_err_int_mask0(unsigned int ucq_err_int_mask0);
int iSetCQ_ERR_INT1_cq_err_int_sts(unsigned int ucq_err_int_sts);
int iSetCQ_ERR_INT1_cq_err_int_mask1(unsigned int ucq_err_int_mask1);
int iSetCQ_ERR_INT2_cq_err_int_sts(unsigned int ucq_err_int_sts);
int iSetCQ_ERR_INT2_cq_err_int_mask2(unsigned int ucq_err_int_mask2);
int iSetCQ_ERR_INT3_cq_err_int_sts(unsigned int ucq_err_int_sts);
int iSetCQ_ERR_INT3_cq_err_int_mask3(unsigned int ucq_err_int_mask3);
int iSetCQ_ERR_INT4_cq_err_int_sts(unsigned int ucq_err_int_sts);
int iSetCQ_ERR_INT4_cq_err_int_mask4(unsigned int ucq_err_int_mask4);
int iSetCQ_ERR_INT5_cq_err_int_sts(unsigned int ucq_err_int_sts);
int iSetCQ_ERR_INT5_cq_err_int_mask5(unsigned int ucq_err_int_mask5);
int iSetCQ_ERR_INT6_cq_err_int_sts(unsigned int ucq_err_int_sts);
int iSetCQ_ERR_INT6_cq_err_int_mask6(unsigned int ucq_err_int_mask6);
int iSetCQ_ERR_INT7_cq_err_int_sts(unsigned int ucq_err_int_sts);
int iSetCQ_ERR_INT7_cq_err_int_mask7(unsigned int ucq_err_int_mask7);
int iSetCQ_ERR_INT8_cq_err_int_sts(unsigned int ucq_err_int_sts);
int iSetCQ_ERR_INT8_cq_err_int_mask8(unsigned int ucq_err_int_mask8);
int iSetCQ_ERR_INT9_cq_err_int_sts(unsigned int ucq_err_int_sts);
int iSetCQ_ERR_INT9_cq_err_int_mask9(unsigned int ucq_err_int_mask9);
int iSetCQ_ERR_INT10_cq_err_int_sts(unsigned int ucq_err_int_sts);
int iSetCQ_ERR_INT10_cq_err_int_mask10(unsigned int ucq_err_int_mask10);
int iSetCQ_ERR_INT11_cq_err_int_sts(unsigned int ucq_err_int_sts);
int iSetCQ_ERR_INT11_cq_err_int_mask11(unsigned int ucq_err_int_mask11);
int iSetCQ_ERR_INT12_cq_err_int_sts(unsigned int ucq_err_int_sts);
int iSetCQ_ERR_INT12_cq_err_int_mask12(unsigned int ucq_err_int_mask12);
int iSetCQ_ERR_INT13_cq_err_int_sts(unsigned int ucq_err_int_sts);
int iSetCQ_ERR_INT13_cq_err_int_mask13(unsigned int ucq_err_int_mask13);
int iSetCQ_ERR_INT14_cq_err_int_sts(unsigned int ucq_err_int_sts);
int iSetCQ_ERR_INT14_cq_err_int_mask14(unsigned int ucq_err_int_mask14);
int iSetCQ_ERR_INT15_cq_err_int_sts(unsigned int ucq_err_int_sts);
int iSetCQ_ERR_INT15_cq_err_int_mask15(unsigned int ucq_err_int_mask15);
int iSetCQ_ERR_INT_CFG0_cq_err_int_ro0(unsigned int ucq_err_int_ro0);
int iSetCQ_ERR_INT_CFG0_cq_err_int_set0(unsigned int ucq_err_int_set0);
int iSetCQ_ERR_INT_CFG1_cq_err_int_ro1(unsigned int ucq_err_int_ro1);
int iSetCQ_ERR_INT_CFG1_cq_err_int_set1(unsigned int ucq_err_int_set1);
int iSetCQ_ERR_INT_CFG2_cq_err_int_ro2(unsigned int ucq_err_int_ro2);
int iSetCQ_ERR_INT_CFG2_cq_err_int_set2(unsigned int ucq_err_int_set2);
int iSetCQ_ERR_INT_CFG3_cq_err_int_ro3(unsigned int ucq_err_int_ro3);
int iSetCQ_ERR_INT_CFG3_cq_err_int_set3(unsigned int ucq_err_int_set3);
int iSetCQ_ERR_INT_CFG4_cq_err_int_ro4(unsigned int ucq_err_int_ro4);
int iSetCQ_ERR_INT_CFG4_cq_err_int_set4(unsigned int ucq_err_int_set4);
int iSetCQ_ERR_INT_CFG5_cq_err_int_ro5(unsigned int ucq_err_int_ro5);
int iSetCQ_ERR_INT_CFG5_cq_err_int_set5(unsigned int ucq_err_int_set5);
int iSetCQ_ERR_INT_CFG6_cq_err_int_ro6(unsigned int ucq_err_int_ro6);
int iSetCQ_ERR_INT_CFG6_cq_err_int_set6(unsigned int ucq_err_int_set6);
int iSetCQ_ERR_INT_CFG7_cq_err_int_ro7(unsigned int ucq_err_int_ro7);
int iSetCQ_ERR_INT_CFG7_cq_err_int_set7(unsigned int ucq_err_int_set7);
int iSetCQ_ERR_INT_CFG8_cq_err_int_ro8(unsigned int ucq_err_int_ro8);
int iSetCQ_ERR_INT_CFG8_cq_err_int_set8(unsigned int ucq_err_int_set8);
int iSetCQ_ERR_INT_CFG9_cq_err_int_ro9(unsigned int ucq_err_int_ro9);
int iSetCQ_ERR_INT_CFG9_cq_err_int_set9(unsigned int ucq_err_int_set9);
int iSetCQ_ERR_INT_CFG10_cq_err_int_ro10(unsigned int ucq_err_int_ro10);
int iSetCQ_ERR_INT_CFG10_cq_err_int_set10(unsigned int ucq_err_int_set10);
int iSetCQ_ERR_INT_CFG11_cq_err_int_ro11(unsigned int ucq_err_int_ro11);
int iSetCQ_ERR_INT_CFG11_cq_err_int_set11(unsigned int ucq_err_int_set11);
int iSetCQ_ERR_INT_CFG12_cq_err_int_ro12(unsigned int ucq_err_int_ro12);
int iSetCQ_ERR_INT_CFG12_cq_err_int_set12(unsigned int ucq_err_int_set12);
int iSetCQ_ERR_INT_CFG13_cq_err_int_ro13(unsigned int ucq_err_int_ro13);
int iSetCQ_ERR_INT_CFG13_cq_err_int_set13(unsigned int ucq_err_int_set13);
int iSetCQ_ERR_INT_CFG14_cq_err_int_ro14(unsigned int ucq_err_int_ro14);
int iSetCQ_ERR_INT_CFG14_cq_err_int_set14(unsigned int ucq_err_int_set14);
int iSetCQ_ERR_INT_CFG15_cq_err_int_ro15(unsigned int ucq_err_int_ro15);
int iSetCQ_ERR_INT_CFG15_cq_err_int_set15(unsigned int ucq_err_int_set15);
int iSetWR_PTR_BASE_ADDDR_LOW_wr_ptr_base_addr_low(unsigned int uwr_ptr_base_addr_low);
int iSetWR_PTR_BASE_ADDDR_HIGH_wr_ptr_base_addr_high(unsigned int uwr_ptr_base_addr_high);
int iSetMEMORY_BASE_ADDDR_LOW_memory_base_addr_low(unsigned int umemory_base_addr_low);
int iSetMEMORY_BASE_ADDDR_HIGH_memory_base_addr_high(unsigned int umemory_base_addr_high);
int iSetMEMORY_SPACE_LOW_memory_space_low(unsigned int umemory_space_low);

int iSetDB_STRD_pf0_strd(unsigned int upf0_strd);
int iSetDB_STRD_pf_1_strd(unsigned int upf_1_strd);
int iSetDB_STRD_pf0_vf_strd(unsigned int upf0_vf_strd);
int iSetDB_STRD_pf1_vf_strd(unsigned int upf1_vf_strd);
int iSetVF_SIZE_vf_size(unsigned int uvf_size);
int iSetSQE_RD_MAX_sqe_rd_max(unsigned int usqe_rd_max);
int iSetRAM_INITIAL_ram_inital_done(unsigned int uram_inital_done);
int iSetRAM_INITIAL_ram_initial(unsigned int uram_initial);

int iSetDDR_STRD_ddr_wrptr_strd(unsigned int uddr_wrptr_strd);
int iSetINT_REQ_pf_num(unsigned int upf_num);
int iSetINT_REQ_vf_num(unsigned int uvf_num);
int iSetINT_REQ_int_vector(unsigned int uint_vector);
int iSetINT_REQ_int_deassert(unsigned int uint_deassert);
int iSetINT_REQ_req_set(unsigned int ureq_set);
int iSetNVME_VECTOR_PENDING_vector_pending(unsigned int uvector_pending);
int iSetNVME_VECTOR_MASK_vector_mask(unsigned int uvector_mask);
int iSetIOSQS_completion_queue_size(unsigned int ucompletion_queue_size);
int iSetIOSQS_submission_queue_size(unsigned int usubmission_queue_size);
int iSetIOSQ_BASE_ADDR_LOW_sqr_base_addr_low(unsigned int usqr_base_addr_low);
int iSetIOSQ_BASE_ADDR_HIGH_sqr_base_addr_high(unsigned int usqr_base_addr_high);
int iSetSQR_MEMORY_RD_PTR_memory_rd_ptr(unsigned int umemory_rd_ptr);
int iSetSQ_SIZE_RAM_ECC_INJECT_sq_size_ram_ecc_inject(unsigned int usq_size_ram_ecc_inject);
int iSetSQ_SIZE_RAM_ECC_INJECT_sq_size_ram_2bit_ecc_ro(unsigned int usq_size_ram_2bit_ecc_ro);
int iSetSQ_SIZE_RAM_ECC_INJECT_sq_size_ram_1bit_ecc_ro(unsigned int usq_size_ram_1bit_ecc_ro);
int iSetSQ_SIZE_RAM_ECC_INJECT_sq_size_ram_2bit_ecc_sts(unsigned int usq_size_ram_2bit_ecc_sts);
int iSetSQ_SIZE_RAM_ECC_INJECT_sq_size_ram_1bit_ecc_sts(unsigned int usq_size_ram_1bit_ecc_sts);
int iSetSQ_SIZE_RAM_ECC_INJECT_sq_size_ram_2bit_ecc_mask(unsigned int usq_size_ram_2bit_ecc_mask);
int iSetSQ_SIZE_RAM_ECC_INJECT_sq_size_ram_1bit_ecc_mask(unsigned int usq_size_ram_1bit_ecc_mask);
int iSetSQ_SIZE_RAM_ECC_INJECT_sq_size_ram_2bit_ecc_set(unsigned int usq_size_ram_2bit_ecc_set);
int iSetSQ_SIZE_RAM_ECC_INJECT_sq_size_ram_1bit_ecc_set(unsigned int usq_size_ram_1bit_ecc_set);
int iSetSQ_SIZE_RAM_ECC_INJECT_sq_size_ram_2bit_ecc_cnt(unsigned int usq_size_ram_2bit_ecc_cnt);
int iSetSQ_SIZE_RAM_ECC_INJECT_sq_size_ram_1bit_ecc_cnt(unsigned int usq_size_ram_1bit_ecc_cnt);
int iSetSQ_SIZE_RAM_ECC_ADDR_sq_size_ram_2bit_ecc_addr(unsigned int usq_size_ram_2bit_ecc_addr);
int iSetSQ_SIZE_RAM_ECC_ADDR_sq_size_ram_1bit_ecc_addr(unsigned int usq_size_ram_1bit_ecc_addr);
int iSetSQ_BAL_RAM_ECC_INJECT_sq_bal_ram_ecc_inject(unsigned int usq_bal_ram_ecc_inject);
int iSetSQ_BAL_RAM_ECC_INJECT_sq_bal_ram_2bit_ecc_ro(unsigned int usq_bal_ram_2bit_ecc_ro);
int iSetSQ_BAL_RAM_ECC_INJECT_sq_bal_ram_1bit_ecc_ro(unsigned int usq_bal_ram_1bit_ecc_ro);
int iSetSQ_BAL_RAM_ECC_INJECT_sq_bal_ram_2bit_ecc_sts(unsigned int usq_bal_ram_2bit_ecc_sts);
int iSetSQ_BAL_RAM_ECC_INJECT_sq_bal_ram_1bit_ecc_sts(unsigned int usq_bal_ram_1bit_ecc_sts);
int iSetSQ_BAL_RAM_ECC_INJECT_sq_bal_ram_2bit_ecc_mask(unsigned int usq_bal_ram_2bit_ecc_mask);
int iSetSQ_BAL_RAM_ECC_INJECT_sq_bal_ram_1bit_ecc_mask(unsigned int usq_bal_ram_1bit_ecc_mask);
int iSetSQ_BAL_RAM_ECC_INJECT_sq_bal_ram_2bit_ecc_set(unsigned int usq_bal_ram_2bit_ecc_set);
int iSetSQ_BAL_RAM_ECC_INJECT_sq_bal_ram_1bit_ecc_set(unsigned int usq_bal_ram_1bit_ecc_set);
int iSetSQ_BAL_RAM_ECC_INJECT_sq_bal_ram_2bit_ecc_cnt(unsigned int usq_bal_ram_2bit_ecc_cnt);
int iSetSQ_BAL_RAM_ECC_INJECT_sq_bal_ram_1bit_ecc_cnt(unsigned int usq_bal_ram_1bit_ecc_cnt);
int iSetSQ_BAL_RAM_ECC_ADDR_sq_bal_ram_2bit_ecc_addr(unsigned int usq_bal_ram_2bit_ecc_addr);
int iSetSQ_BAL_RAM_ECC_ADDR_sq_bal_ram_1bit_ecc_addr(unsigned int usq_bal_ram_1bit_ecc_addr);
int iSetSQ_BAH_RAM_ECC_INJECT_sq_bah_ram_ecc_inject(unsigned int usq_bah_ram_ecc_inject);
int iSetSQ_BAH_RAM_ECC_INJECT_sq_bah_ram_2bit_ecc_ro(unsigned int usq_bah_ram_2bit_ecc_ro);
int iSetSQ_BAH_RAM_ECC_INJECT_sq_bah_ram_1bit_ecc_ro(unsigned int usq_bah_ram_1bit_ecc_ro);
int iSetSQ_BAH_RAM_ECC_INJECT_sq_bah_ram_2bit_ecc_sts(unsigned int usq_bah_ram_2bit_ecc_sts);
int iSetSQ_BAH_RAM_ECC_INJECT_sq_bah_ram_1bit_ecc_sts(unsigned int usq_bah_ram_1bit_ecc_sts);
int iSetSQ_BAH_RAM_ECC_INJECT_sq_bah_ram_2bit_ecc_mask(unsigned int usq_bah_ram_2bit_ecc_mask);
int iSetSQ_BAH_RAM_ECC_INJECT_sq_bah_ram_1bit_ecc_mask(unsigned int usq_bah_ram_1bit_ecc_mask);
int iSetSQ_BAH_RAM_ECC_INJECT_sq_bah_ram_2bit_ecc_set(unsigned int usq_bah_ram_2bit_ecc_set);
int iSetSQ_BAH_RAM_ECC_INJECT_sq_bah_ram_1bit_ecc_set(unsigned int usq_bah_ram_1bit_ecc_set);
int iSetSQ_BAH_RAM_ECC_INJECT_sq_bah_ram_2bit_ecc_cnt(unsigned int usq_bah_ram_2bit_ecc_cnt);
int iSetSQ_BAH_RAM_ECC_INJECT_sq_bah_ram_1bit_ecc_cnt(unsigned int usq_bah_ram_1bit_ecc_cnt);
int iSetSQ_BAH_RAM_ECC_ADDR_sq_bah_ram_2bit_ecc_addr(unsigned int usq_bah_ram_2bit_ecc_addr);
int iSetSQ_BAH_RAM_ECC_ADDR_sq_bah_ram_1bit_ecc_addr(unsigned int usq_bah_ram_1bit_ecc_addr);
int iSetRD_PTR_RAM_ECC_INJECT_rd_ptr_ram_ecc_inject(unsigned int urd_ptr_ram_ecc_inject);
int iSetRD_PTR_RAM_ECC_INJECT_rd_ptr_ram_2bit_ecc_ro(unsigned int urd_ptr_ram_2bit_ecc_ro);
int iSetRD_PTR_RAM_ECC_INJECT_rd_ptr_ram_1bit_ecc_ro(unsigned int urd_ptr_ram_1bit_ecc_ro);
int iSetRD_PTR_RAM_ECC_INJECT_rd_ptr_ram_2bit_ecc_sts(unsigned int urd_ptr_ram_2bit_ecc_sts);
int iSetRD_PTR_RAM_ECC_INJECT_rd_ptr_ram_1bit_ecc_sts(unsigned int urd_ptr_ram_1bit_ecc_sts);
int iSetRD_PTR_RAM_ECC_INJECT_rd_ptr_ram_2bit_ecc_mask(unsigned int urd_ptr_ram_2bit_ecc_mask);
int iSetRD_PTR_RAM_ECC_INJECT_rd_ptr_ram_1bit_ecc_mask(unsigned int urd_ptr_ram_1bit_ecc_mask);
int iSetRD_PTR_RAM_ECC_INJECT_rd_ptr_ram_2bit_ecc_set(unsigned int urd_ptr_ram_2bit_ecc_set);
int iSetRD_PTR_RAM_ECC_INJECT_rd_ptr_ram_1bit_ecc_set(unsigned int urd_ptr_ram_1bit_ecc_set);
int iSetRD_PTR_RAM_ECC_INJECT_rd_ptr_ram_2bit_ecc_cnt(unsigned int urd_ptr_ram_2bit_ecc_cnt);
int iSetRD_PTR_RAM_ECC_INJECT_rd_ptr_ram_1bit_ecc_cnt(unsigned int urd_ptr_ram_1bit_ecc_cnt);
int iSetRD_PTR_RAM_ECC_ADDR_rd_ptr_ram_2bit_ecc_addr(unsigned int urd_ptr_ram_2bit_ecc_addr);
int iSetRD_PTR_RAM_ECC_ADDR_rd_ptr_ram_1bit_ecc_addr(unsigned int urd_ptr_ram_1bit_ecc_addr);
int iSetSQ_DES_RAM_ECC_INJECT_sq_des_ram_ecc_inject(unsigned int usq_des_ram_ecc_inject);
int iSetSQ_DES_RAM_ECC_INJECT_sq_des_ram_2bit_ecc_ro(unsigned int usq_des_ram_2bit_ecc_ro);
int iSetSQ_DES_RAM_ECC_INJECT_sq_des_ram_1bit_ecc_ro(unsigned int usq_des_ram_1bit_ecc_ro);
int iSetSQ_DES_RAM_ECC_INJECT_sq_des_ram_2bit_ecc_sts(unsigned int usq_des_ram_2bit_ecc_sts);
int iSetSQ_DES_RAM_ECC_INJECT_sq_des_ram_1bit_ecc_sts(unsigned int usq_des_ram_1bit_ecc_sts);
int iSetSQ_DES_RAM_ECC_INJECT_sq_des_ram_2bit_ecc_mask(unsigned int usq_des_ram_2bit_ecc_mask);
int iSetSQ_DES_RAM_ECC_INJECT_sq_des_ram_1bit_ecc_mask(unsigned int usq_des_ram_1bit_ecc_mask);
int iSetSQ_DES_RAM_ECC_INJECT_sq_des_ram_2bit_ecc_set(unsigned int usq_des_ram_2bit_ecc_set);
int iSetSQ_DES_RAM_ECC_INJECT_sq_des_ram_1bit_ecc_set(unsigned int usq_des_ram_1bit_ecc_set);
int iSetSQ_DES_RAM_ECC_INJECT_sq_des_ram_2bit_ecc_cnt(unsigned int usq_des_ram_2bit_ecc_cnt);
int iSetSQ_DES_RAM_ECC_INJECT_sq_des_ram_1bit_ecc_cnt(unsigned int usq_des_ram_1bit_ecc_cnt);
int iSetSQ_DES_RAM_ECC_ADDR_sq_des_ram_2bit_ecc_addr(unsigned int usq_des_ram_2bit_ecc_addr);
int iSetSQ_DES_RAM_ECC_ADDR_sq_des_ram_1bit_ecc_addr(unsigned int usq_des_ram_1bit_ecc_addr);
int iSetPRFETCH_FIFO_RAM_ECC_INJECT_prefetch_fifo_ram_ecc_inject(unsigned int uprefetch_fifo_ram_ecc_inject);
int iSetPRFETCH_FIFO_RAM_ECC_INJECT_prefetch_fifo_ram_2bit_ecc_ro(unsigned int uprefetch_fifo_ram_2bit_ecc_ro);
int iSetPRFETCH_FIFO_RAM_ECC_INJECT_prefetch_fifo_ram_1bit_ecc_ro(unsigned int uprefetch_fifo_ram_1bit_ecc_ro);
int iSetPRFETCH_FIFO_RAM_ECC_INJECT_prefetch_fifo_ram_2bit_ecc_sts(unsigned int uprefetch_fifo_ram_2bit_ecc_sts);
int iSetPRFETCH_FIFO_RAM_ECC_INJECT_prefetch_fifo_ram_1bit_ecc_sts(unsigned int uprefetch_fifo_ram_1bit_ecc_sts);
int iSetPRFETCH_FIFO_RAM_ECC_INJECT_prefetch_fifo_ram_2bit_ecc_mask(unsigned int uprefetch_fifo_ram_2bit_ecc_mask);
int iSetPRFETCH_FIFO_RAM_ECC_INJECT_prefetch_fifo_ram_1bit_ecc_mask(unsigned int uprefetch_fifo_ram_1bit_ecc_mask);
int iSetPRFETCH_FIFO_RAM_ECC_INJECT_prefetch_fifo_ram_2bit_ecc_set(unsigned int uprefetch_fifo_ram_2bit_ecc_set);
int iSetPRFETCH_FIFO_RAM_ECC_INJECT_prefetch_fifo_ram_1bit_ecc_set(unsigned int uprefetch_fifo_ram_1bit_ecc_set);
int iSetPRFETCH_FIFO_RAM_ECC_INJECT_prefetch_fifo_ram_2bit_ecc_cnt(unsigned int uprefetch_fifo_ram_2bit_ecc_cnt);
int iSetPRFETCH_FIFO_RAM_ECC_INJECT_prefetch_fifo_ram_1bit_ecc_cnt(unsigned int uprefetch_fifo_ram_1bit_ecc_cnt);
int iSetPRFETCH_FIFO_RAM_ECC_ADDR_prefetch_fifo_ram_2bit_ecc_addr(unsigned int uprefetch_fifo_ram_2bit_ecc_addr);
int iSetPRFETCH_FIFO_RAM_ECC_ADDR_prefetch_fifo_ram_1bit_ecc_addr(unsigned int uprefetch_fifo_ram_1bit_ecc_addr);
int iSetNVME_LINK_DOWN_PROC_nvme_link_up_sts(unsigned int unvme_link_up_sts);
int iSetNVME_LINK_DOWN_PROC_nvme_link_down_prt_initial(unsigned int unvme_link_down_prt_initial);
int iSetNVME_FLR_STS_nvme_flr__proc_cmpl(unsigned int unvme_flr__proc_cmpl);
int iSetNVME_FLR_STS_nvme_flr_ptr_inital(unsigned int unvme_flr_ptr_inital);
int iSetNVME_DMACQ_ERR_STS_nvme_cq_err_ptr_initial(unsigned int unvme_cq_err_ptr_initial);
int iSetNVME_SQ_DES_RAM_CFG_nvme_sq_des_ctrl_ram_wr(unsigned int unvme_sq_des_ctrl_ram_wr);
int iSetNVME_SQ_DES_RAM_CFG_nvme_sq_des_ctrl_ram_rd(unsigned int unvme_sq_des_ctrl_ram_rd);
int iSetNVME_SQ_DES_RAM_CFG_nvme_sq_des_ctrl_ram_addr(unsigned int unvme_sq_des_ctrl_ram_addr);
int iSetNVME_SQ_DES_RAM_WDAT_L_sq_des_ram_wr_ptr(unsigned int usq_des_ram_wr_ptr);
int iSetNVME_SQ_DES_RAM_WDAT_H_sq_des_ram_hd_ptr(unsigned int usq_des_ram_hd_ptr);
int iSetNVME_SQ_DES_RAM_RDAT_L_sq_des_ram_wr_ptr(unsigned int usq_des_ram_wr_ptr);
int iSetNVME_SQ_DES_RAM_RDAT_H_sq_des_ram_hd_ptr(unsigned int usq_des_ram_hd_ptr);
int iSetNVME_FIFO_STS_sq_des_ctrl_fifo_full(unsigned int usq_des_ctrl_fifo_full);
int iSetNVME_FIFO_STS_sq_des_ctrl_fifo_empty(unsigned int usq_des_ctrl_fifo_empty);
int iSetNVME_FIFO_STS_cq_des_ctrl_fifo_full(unsigned int ucq_des_ctrl_fifo_full);
int iSetNVME_FIFO_STS_cq_des_ctrl_fifo_empty(unsigned int ucq_des_ctrl_fifo_empty);
int iSetNVME_FIFO_STS_cq_err_sts_fifo_full(unsigned int ucq_err_sts_fifo_full);
int iSetNVME_FIFO_STS_cq_err_sts_fifo_empty(unsigned int ucq_err_sts_fifo_empty);
int iSetNVME_FSM_STS_tail_ram_fsm_sts(unsigned int utail_ram_fsm_sts);
int iSetNVME_FSM_STS_sqs_ram_fsm_sts(unsigned int usqs_ram_fsm_sts);
int iSetNVME_FSM_STS_sqbal_ram_fsm_sts(unsigned int usqbal_ram_fsm_sts);
int iSetNVME_FSM_STS_sqbah_ram_fsm_sts(unsigned int usqbah_ram_fsm_sts);
int iSetNVME_FSM_STS_rdptr_ram_fsm_sts(unsigned int urdptr_ram_fsm_sts);
int iSetNVME_FLOW_STS0_sq_cnt(unsigned int usq_cnt);
int iSetNVME_FLOW_STS0_cq_cnt(unsigned int ucq_cnt);
int iSetNVME_FLOW_STS1_wrptr_req_cnt(unsigned int uwrptr_req_cnt);
int iSetNVME_FLOW_STS1_wrptr_bresp_cnt(unsigned int uwrptr_bresp_cnt);
int iSetNVME_QUEUE_STS0_nvme_queue_sts0(unsigned int unvme_queue_sts0);
int iSetNVME_QUEUE_STS1_nvme_queue_sts1(unsigned int unvme_queue_sts1);
int iSetNVME_QUEUE_STS2_nvme_queue_sts2(unsigned int unvme_queue_sts2);
int iSetNVME_QUEUE_STS3_nvme_queue_sts3(unsigned int unvme_queue_sts3);
int iSetNVME_QUEUE_STS4_nvme_queue_sts4(unsigned int unvme_queue_sts4);
int iSetNVME_QUEUE_STS5_nvme_queue_sts5(unsigned int unvme_queue_sts5);
int iSetNVME_QUEUE_STS6_nvme_queue_sts6(unsigned int unvme_queue_sts6);
int iSetNVME_QUEUE_STS7_nvme_queue_sts7(unsigned int unvme_queue_sts7);

#endif // __HIPCIEC_NVME_GLOBAL_REG_C_UNION_DEFINE_H__
