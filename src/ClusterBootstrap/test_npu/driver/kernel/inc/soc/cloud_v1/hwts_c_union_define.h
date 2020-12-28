// ****************************************************************************** 
// Copyright     :  Copyright (C) 2018, Hisilicon Technologies Co. Ltd.
// File name     :  c_union_define.h
// Department    :  CAD Development Department
// Author        :  xxx
// Version       :  1
// Date          :  2013/3/10
// Description   :  The description of xxx project
// Others        :  Generated automatically by nManager V4.2 
// History       :  xxx 2018/11/30 20:03:46 Create file
// ******************************************************************************

#ifndef __C_UNION_DEFINE_H__
#define __C_UNION_DEFINE_H__

/* Define the union U_HWTS_GLB_CTRL0 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned long long    hwts_bypass           : 1   ; /* [0]  */
        unsigned long long    hwts_fastpath_en      : 1   ; /* [1]  */
        unsigned long long    hwts_force_icache_inv : 1   ; /* [2]  */
        unsigned long long    hwts_skip_reset       : 1   ; /* [3]  */
        unsigned long long    reserved_0            : 1   ; /* [4]  */
        unsigned long long    hwts_skip_warning_check : 1   ; /* [5]  */
        unsigned long long    reserved_1            : 2   ; /* [7..6]  */
        unsigned long long    syscnt_gray           : 1   ; /* [8]  */
        unsigned long long    axi_slv_resp_err      : 1   ; /* [9]  */
        unsigned long long    ecc_bypass            : 1   ; /* [10]  */
        unsigned long long    task_timeout_bypass   : 1   ; /* [11]  */
        unsigned long long    bus_err_bypass        : 1   ; /* [12]  */
        unsigned long long    sdma_task_timeout_bypass : 1   ; /* [13]  */
        unsigned long long    reserved_2            : 2   ; /* [15..14]  */
        unsigned long long    reset_dly_cnt         : 8   ; /* [23..16]  */
        unsigned long long    reserved_3            : 8   ; /* [31..24]  */
        unsigned long long    task_runtime_limit_exp : 6   ; /* [37..32]  */
        unsigned long long    reserved_4            : 2   ; /* [39..38]  */
        unsigned long long    batch_mode_timeout_man : 8   ; /* [47..40]  */
        unsigned long long    reserved_5            : 16  ; /* [63..48]  */
    } bits;

    /* Define an unsigned member */
    unsigned long long    u64;

} U_HWTS_GLB_CTRL0;

/* Define the union U_HWTS_BYPASS_CODE */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned long long    reserved_0                  ; /* [31..0]  */
        unsigned long long    reserved_1                  ; /* [63..32]  */
    } bits;

    /* Define an unsigned member */
    unsigned long long    u64;

} U_HWTS_BYPASS_CODE;

/* Define the union U_HWTS_PRIORITY_CTRL0 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned long long    lp_mode               : 3   ; /* [2..0]  */
        unsigned long long    reserved_0            : 5   ; /* [7..3]  */
        unsigned long long    sp_level              : 3   ; /* [10..8]  */
        unsigned long long    reserved_1            : 5   ; /* [15..11]  */
        unsigned long long    override_mode         : 1   ; /* [16]  */
        /* The 2 members below is corresponding to the member "reserved_2          " of the struct "" */        unsigned long long    reserved_2_0          : 15  ; /* [63..17]  */        unsigned long long    reserved_2_1                ; /* [63..17]  */
    } bits;

    /* Define an unsigned member */
    unsigned long long    u64;

} U_HWTS_PRIORITY_CTRL0;

/* Define the union U_HWTS_PRIORITY_CTRL1 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned long long    wrr_weight0           : 8   ; /* [7..0]  */
        unsigned long long    wrr_weight1           : 8   ; /* [15..8]  */
        unsigned long long    wrr_weight2           : 8   ; /* [23..16]  */
        unsigned long long    wrr_weight3           : 8   ; /* [31..24]  */
        unsigned long long    wrr_weight4           : 8   ; /* [39..32]  */
        unsigned long long    wrr_weight5           : 8   ; /* [47..40]  */
        unsigned long long    wrr_weight6           : 8   ; /* [55..48]  */
        unsigned long long    wrr_weight7           : 8   ; /* [63..56]  */
    } bits;

    /* Define an unsigned member */
    unsigned long long    u64;

} U_HWTS_PRIORITY_CTRL1;

/* Define the union U_HWTS_HL_PRIORITY_OVERRIDE_TIME_SETTING0 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        /* The 2 members below is corresponding to the member "hp_override_period_cyc" of the struct "" */        unsigned long long    hp_override_period_cyc_0    ; /* [39..0]  */        unsigned long long    hp_override_period_cyc_1 : 8   ; /* [39..0]  */
        unsigned long long    reserved_0            : 24  ; /* [63..40]  */
    } bits;

    /* Define an unsigned member */
    unsigned long long    u64;

} U_HWTS_HL_PRIORITY_OVERRIDE_TIME_SETTING0;

/* Define the union U_HWTS_HL_PRIORITY_OVERRIDE_TIME_SETTING1 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        /* The 2 members below is corresponding to the member "hp_override_cyc     " of the struct "" */        unsigned long long    hp_override_cyc_0           ; /* [39..0]  */        unsigned long long    hp_override_cyc_1     : 8   ; /* [39..0]  */
        unsigned long long    reserved_0            : 24  ; /* [63..40]  */
    } bits;

    /* Define an unsigned member */
    unsigned long long    u64;

} U_HWTS_HL_PRIORITY_OVERRIDE_TIME_SETTING1;

/* Define the union U_HWTS_HL_PRIORITY_OVERRIDE_TASK_SETTING0 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        /* The 2 members below is corresponding to the member "hp_cont_blk_cnt_limit" of the struct "" */        unsigned long long    hp_cont_blk_cnt_limit_0     ; /* [39..0]  */        unsigned long long    hp_cont_blk_cnt_limit_1 : 8   ; /* [39..0]  */
        unsigned long long    reserved_0            : 24  ; /* [63..40]  */
    } bits;

    /* Define an unsigned member */
    unsigned long long    u64;

} U_HWTS_HL_PRIORITY_OVERRIDE_TASK_SETTING0;

/* Define the union U_HWTS_HL_PRIORITY_OVERRIDE_TASK_SETTING1 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        /* The 2 members below is corresponding to the member "lp_cont_blk_cnt_limit" of the struct "" */        unsigned long long    lp_cont_blk_cnt_limit_0     ; /* [39..0]  */        unsigned long long    lp_cont_blk_cnt_limit_1 : 8   ; /* [39..0]  */
        unsigned long long    reserved_0            : 24  ; /* [63..40]  */
    } bits;

    /* Define an unsigned member */
    unsigned long long    u64;

} U_HWTS_HL_PRIORITY_OVERRIDE_TASK_SETTING1;

/* Define the union U_HWTS_AICORE_ENABLE_CTRL */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned long long    aic_enable_ctrl             ; /* [31..0]  */
        unsigned long long    reserved_0                  ; /* [63..32]  */
    } bits;

    /* Define an unsigned member */
    unsigned long long    u64;

} U_HWTS_AICORE_ENABLE_CTRL;

/* Define the union U_HWTS_AICORE_DISABLE_CTRL */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned long long    aic_disable_ctrl            ; /* [31..0]  */
        unsigned long long    reserved_0                  ; /* [63..32]  */
    } bits;

    /* Define an unsigned member */
    unsigned long long    u64;

} U_HWTS_AICORE_DISABLE_CTRL;

/* Define the union U_HWTS_AICORE_STATUS */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned long long    aic_enabled_status          ; /* [31..0]  */
        unsigned long long    reserved_0                  ; /* [63..32]  */
    } bits;

    /* Define an unsigned member */
    unsigned long long    u64;

} U_HWTS_AICORE_STATUS;

/* Define the union U_HWTS_AXCACHE_SETTING */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned long long    hwts_awcache_aic      : 4   ; /* [3..0]  */
        unsigned long long    hwts_awsnoop_aic      : 1   ; /* [4]  */
        unsigned long long    reserved_0            : 3   ; /* [7..5]  */
        unsigned long long    hwts_awcache_cq       : 4   ; /* [11..8]  */
        unsigned long long    hwts_awsnoop_cq       : 1   ; /* [12]  */
        unsigned long long    reserved_1            : 3   ; /* [15..13]  */
        unsigned long long    hwts_awcache_log      : 4   ; /* [19..16]  */
        unsigned long long    hwts_awsnoop_log      : 1   ; /* [20]  */
        unsigned long long    reserved_2            : 3   ; /* [23..21]  */
        unsigned long long    hwts_awcache_pct      : 4   ; /* [27..24]  */
        unsigned long long    hwts_awsnoop_pct      : 1   ; /* [28]  */
        unsigned long long    reserved_3            : 3   ; /* [31..29]  */
        unsigned long long    hwts_awcache_prof     : 4   ; /* [35..32]  */
        unsigned long long    hwts_awsnoop_prof     : 1   ; /* [36]  */
        unsigned long long    reserved_4            : 3   ; /* [39..37]  */
        unsigned long long    hwts_arcache_aic      : 4   ; /* [43..40]  */
        unsigned long long    hwts_arsnoop_aic      : 1   ; /* [44]  */
        unsigned long long    reserved_5            : 3   ; /* [47..45]  */
        unsigned long long    hwts_arcache_sq       : 4   ; /* [51..48]  */
        unsigned long long    hwts_arsnoop_sq       : 1   ; /* [52]  */
        unsigned long long    reserved_6            : 1   ; /* [53]  */
        unsigned long long    hwts_awsnoop_sdma_sq  : 1   ; /* [54]  */
        unsigned long long    hwts_awsnoop_sdma_sqcq_db : 1   ; /* [55]  */
        unsigned long long    hwts_awcache_sdma_sq  : 4   ; /* [59..56]  */
        unsigned long long    hwts_awcache_sdma_sqcq_db : 4   ; /* [63..60]  */
    } bits;

    /* Define an unsigned member */
    unsigned long long    u64;

} U_HWTS_AXCACHE_SETTING;

/* Define the union U_HWTS_AWUSER_L_SETTING */
typedef union
{
    /* Define the struct bits */
    struct
    {
        /* The 2 members below is corresponding to the member "hwts_awuser_l       " of the struct "" */        unsigned long long    hwts_awuser_l_0             ; /* [63..0]  */        unsigned long long    hwts_awuser_l_1             ; /* [63..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned long long    u64;

} U_HWTS_AWUSER_L_SETTING;

/* Define the union U_HWTS_AWUSER_H_SETTING */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned long long    hwts_awuser_h         : 4   ; /* [3..0]  */
        /* The 2 members below is corresponding to the member "reserved_0          " of the struct "" */        unsigned long long    reserved_0_0          : 28  ; /* [63..4]  */        unsigned long long    reserved_0_1                ; /* [63..4]  */
    } bits;

    /* Define an unsigned member */
    unsigned long long    u64;

} U_HWTS_AWUSER_H_SETTING;

/* Define the union U_HWTS_ARUSER_L_SETTING */
typedef union
{
    /* Define the struct bits */
    struct
    {
        /* The 2 members below is corresponding to the member "hwts_aruser_l       " of the struct "" */        unsigned long long    hwts_aruser_l_0             ; /* [63..0]  */        unsigned long long    hwts_aruser_l_1             ; /* [63..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned long long    u64;

} U_HWTS_ARUSER_L_SETTING;

/* Define the union U_HWTS_ARUSER_H_SETTING */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned long long    hwts_aruser_h         : 4   ; /* [3..0]  */
        /* The 2 members below is corresponding to the member "reserved_0          " of the struct "" */        unsigned long long    reserved_0_0          : 28  ; /* [63..4]  */        unsigned long long    reserved_0_1                ; /* [63..4]  */
    } bits;

    /* Define an unsigned member */
    unsigned long long    u64;

} U_HWTS_ARUSER_H_SETTING;

/* Define the union U_HWTS_SEC_EN */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned long long    hwts_sec_en           : 1   ; /* [0]  */
        unsigned long long    hwts_cqe_buf_sec_en   : 1   ; /* [1]  */
        unsigned long long    write_value_bypass    : 1   ; /* [2]  */
        /* The 2 members below is corresponding to the member "reserved_0          " of the struct "" */        unsigned long long    reserved_0_0          : 29  ; /* [63..3]  */        unsigned long long    reserved_0_1                ; /* [63..3]  */
    } bits;

    /* Define an unsigned member */
    unsigned long long    u64;

} U_HWTS_SEC_EN;

/* Define the union U_HWTS_NS_SETTING */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned long long    hwts_awns_aic         : 1   ; /* [0]  */
        unsigned long long    reserved_0            : 7   ; /* [7..1]  */
        unsigned long long    hwts_awns_cq          : 1   ; /* [8]  */
        unsigned long long    reserved_1            : 7   ; /* [15..9]  */
        unsigned long long    hwts_awns_log         : 1   ; /* [16]  */
        unsigned long long    reserved_2            : 7   ; /* [23..17]  */
        unsigned long long    hwts_awns_pct         : 1   ; /* [24]  */
        unsigned long long    reserved_3            : 7   ; /* [31..25]  */
        unsigned long long    hwts_awns_prof        : 1   ; /* [32]  */
        unsigned long long    reserved_4            : 7   ; /* [39..33]  */
        unsigned long long    hwts_arns_aic         : 1   ; /* [40]  */
        unsigned long long    reserved_5            : 7   ; /* [47..41]  */
        unsigned long long    hwts_arns_sq          : 1   ; /* [48]  */
        unsigned long long    reserved_6            : 7   ; /* [55..49]  */
        unsigned long long    hwts_awns_sdma_sq     : 1   ; /* [56]  */
        unsigned long long    hwts_awns_sdma_sqcq_db : 1   ; /* [57]  */
        unsigned long long    hwts_awns_write_value : 1   ; /* [58]  */
        unsigned long long    reserved_7            : 5   ; /* [63..59]  */
    } bits;

    /* Define an unsigned member */
    unsigned long long    u64;

} U_HWTS_NS_SETTING;

/* Define the union U_HWTS_SDMA_CH_EN_CTRL */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned long long    sdma_ch_enable        : 8   ; /* [7..0]  */
        /* The 2 members below is corresponding to the member "reserved_0          " of the struct "" */        unsigned long long    reserved_0_0          : 24  ; /* [63..8]  */        unsigned long long    reserved_0_1                ; /* [63..8]  */
    } bits;

    /* Define an unsigned member */
    unsigned long long    u64;

} U_HWTS_SDMA_CH_EN_CTRL;

/* Define the union U_HWTS_AIC00_BASE_ADDR */
typedef union
{
    /* Define the struct bits */
    struct
    {
        /* The 2 members below is corresponding to the member "aic00_base_addr     " of the struct "" */        unsigned long long    aic00_base_addr_0           ; /* [47..0]  */        unsigned long long    aic00_base_addr_1     : 16  ; /* [47..0]  */
        unsigned long long    reserved_0            : 16  ; /* [63..48]  */
    } bits;

    /* Define an unsigned member */
    unsigned long long    u64;

} U_HWTS_AIC00_BASE_ADDR;

/* Define the union U_HWTS_AIC01_BASE_ADDR */
typedef union
{
    /* Define the struct bits */
    struct
    {
        /* The 2 members below is corresponding to the member "aic01_base_addr     " of the struct "" */        unsigned long long    aic01_base_addr_0           ; /* [47..0]  */        unsigned long long    aic01_base_addr_1     : 16  ; /* [47..0]  */
        unsigned long long    reserved_0            : 16  ; /* [63..48]  */
    } bits;

    /* Define an unsigned member */
    unsigned long long    u64;

} U_HWTS_AIC01_BASE_ADDR;

/* Define the union U_HWTS_AIC02_BASE_ADDR */
typedef union
{
    /* Define the struct bits */
    struct
    {
        /* The 2 members below is corresponding to the member "aic02_base_addr     " of the struct "" */        unsigned long long    aic02_base_addr_0           ; /* [47..0]  */        unsigned long long    aic02_base_addr_1     : 16  ; /* [47..0]  */
        unsigned long long    reserved_0            : 16  ; /* [63..48]  */
    } bits;

    /* Define an unsigned member */
    unsigned long long    u64;

} U_HWTS_AIC02_BASE_ADDR;

/* Define the union U_HWTS_AIC03_BASE_ADDR */
typedef union
{
    /* Define the struct bits */
    struct
    {
        /* The 2 members below is corresponding to the member "aic03_base_addr     " of the struct "" */        unsigned long long    aic03_base_addr_0           ; /* [47..0]  */        unsigned long long    aic03_base_addr_1     : 16  ; /* [47..0]  */
        unsigned long long    reserved_0            : 16  ; /* [63..48]  */
    } bits;

    /* Define an unsigned member */
    unsigned long long    u64;

} U_HWTS_AIC03_BASE_ADDR;

/* Define the union U_HWTS_AIC04_BASE_ADDR */
typedef union
{
    /* Define the struct bits */
    struct
    {
        /* The 2 members below is corresponding to the member "aic04_base_addr     " of the struct "" */        unsigned long long    aic04_base_addr_0           ; /* [47..0]  */        unsigned long long    aic04_base_addr_1     : 16  ; /* [47..0]  */
        unsigned long long    reserved_0            : 16  ; /* [63..48]  */
    } bits;

    /* Define an unsigned member */
    unsigned long long    u64;

} U_HWTS_AIC04_BASE_ADDR;

/* Define the union U_HWTS_AIC05_BASE_ADDR */
typedef union
{
    /* Define the struct bits */
    struct
    {
        /* The 2 members below is corresponding to the member "aic05_base_addr     " of the struct "" */        unsigned long long    aic05_base_addr_0           ; /* [47..0]  */        unsigned long long    aic05_base_addr_1     : 16  ; /* [47..0]  */
        unsigned long long    reserved_0            : 16  ; /* [63..48]  */
    } bits;

    /* Define an unsigned member */
    unsigned long long    u64;

} U_HWTS_AIC05_BASE_ADDR;

/* Define the union U_HWTS_AIC06_BASE_ADDR */
typedef union
{
    /* Define the struct bits */
    struct
    {
        /* The 2 members below is corresponding to the member "aic06_base_addr     " of the struct "" */        unsigned long long    aic06_base_addr_0           ; /* [47..0]  */        unsigned long long    aic06_base_addr_1     : 16  ; /* [47..0]  */
        unsigned long long    reserved_0            : 16  ; /* [63..48]  */
    } bits;

    /* Define an unsigned member */
    unsigned long long    u64;

} U_HWTS_AIC06_BASE_ADDR;

/* Define the union U_HWTS_AIC07_BASE_ADDR */
typedef union
{
    /* Define the struct bits */
    struct
    {
        /* The 2 members below is corresponding to the member "aic07_base_addr     " of the struct "" */        unsigned long long    aic07_base_addr_0           ; /* [47..0]  */        unsigned long long    aic07_base_addr_1     : 16  ; /* [47..0]  */
        unsigned long long    reserved_0            : 16  ; /* [63..48]  */
    } bits;

    /* Define an unsigned member */
    unsigned long long    u64;

} U_HWTS_AIC07_BASE_ADDR;

/* Define the union U_HWTS_AIC08_BASE_ADDR */
typedef union
{
    /* Define the struct bits */
    struct
    {
        /* The 2 members below is corresponding to the member "aic08_base_addr     " of the struct "" */        unsigned long long    aic08_base_addr_0           ; /* [47..0]  */        unsigned long long    aic08_base_addr_1     : 16  ; /* [47..0]  */
        unsigned long long    reserved_0            : 16  ; /* [63..48]  */
    } bits;

    /* Define an unsigned member */
    unsigned long long    u64;

} U_HWTS_AIC08_BASE_ADDR;

/* Define the union U_HWTS_AIC09_BASE_ADDR */
typedef union
{
    /* Define the struct bits */
    struct
    {
        /* The 2 members below is corresponding to the member "aic09_base_addr     " of the struct "" */        unsigned long long    aic09_base_addr_0           ; /* [47..0]  */        unsigned long long    aic09_base_addr_1     : 16  ; /* [47..0]  */
        unsigned long long    reserved_0            : 16  ; /* [63..48]  */
    } bits;

    /* Define an unsigned member */
    unsigned long long    u64;

} U_HWTS_AIC09_BASE_ADDR;

/* Define the union U_HWTS_AIC10_BASE_ADDR */
typedef union
{
    /* Define the struct bits */
    struct
    {
        /* The 2 members below is corresponding to the member "aic10_base_addr     " of the struct "" */        unsigned long long    aic10_base_addr_0           ; /* [47..0]  */        unsigned long long    aic10_base_addr_1     : 16  ; /* [47..0]  */
        unsigned long long    reserved_0            : 16  ; /* [63..48]  */
    } bits;

    /* Define an unsigned member */
    unsigned long long    u64;

} U_HWTS_AIC10_BASE_ADDR;

/* Define the union U_HWTS_AIC11_BASE_ADDR */
typedef union
{
    /* Define the struct bits */
    struct
    {
        /* The 2 members below is corresponding to the member "aic11_base_addr     " of the struct "" */        unsigned long long    aic11_base_addr_0           ; /* [47..0]  */        unsigned long long    aic11_base_addr_1     : 16  ; /* [47..0]  */
        unsigned long long    reserved_0            : 16  ; /* [63..48]  */
    } bits;

    /* Define an unsigned member */
    unsigned long long    u64;

} U_HWTS_AIC11_BASE_ADDR;

/* Define the union U_HWTS_AIC12_BASE_ADDR */
typedef union
{
    /* Define the struct bits */
    struct
    {
        /* The 2 members below is corresponding to the member "aic12_base_addr     " of the struct "" */        unsigned long long    aic12_base_addr_0           ; /* [47..0]  */        unsigned long long    aic12_base_addr_1     : 16  ; /* [47..0]  */
        unsigned long long    reserved_0            : 16  ; /* [63..48]  */
    } bits;

    /* Define an unsigned member */
    unsigned long long    u64;

} U_HWTS_AIC12_BASE_ADDR;

/* Define the union U_HWTS_AIC13_BASE_ADDR */
typedef union
{
    /* Define the struct bits */
    struct
    {
        /* The 2 members below is corresponding to the member "aic13_base_addr     " of the struct "" */        unsigned long long    aic13_base_addr_0           ; /* [47..0]  */        unsigned long long    aic13_base_addr_1     : 16  ; /* [47..0]  */
        unsigned long long    reserved_0            : 16  ; /* [63..48]  */
    } bits;

    /* Define an unsigned member */
    unsigned long long    u64;

} U_HWTS_AIC13_BASE_ADDR;

/* Define the union U_HWTS_AIC14_BASE_ADDR */
typedef union
{
    /* Define the struct bits */
    struct
    {
        /* The 2 members below is corresponding to the member "aic14_base_addr     " of the struct "" */        unsigned long long    aic14_base_addr_0           ; /* [47..0]  */        unsigned long long    aic14_base_addr_1     : 16  ; /* [47..0]  */
        unsigned long long    reserved_0            : 16  ; /* [63..48]  */
    } bits;

    /* Define an unsigned member */
    unsigned long long    u64;

} U_HWTS_AIC14_BASE_ADDR;

/* Define the union U_HWTS_AIC15_BASE_ADDR */
typedef union
{
    /* Define the struct bits */
    struct
    {
        /* The 2 members below is corresponding to the member "aic15_base_addr     " of the struct "" */        unsigned long long    aic15_base_addr_0           ; /* [47..0]  */        unsigned long long    aic15_base_addr_1     : 16  ; /* [47..0]  */
        unsigned long long    reserved_0            : 16  ; /* [63..48]  */
    } bits;

    /* Define an unsigned member */
    unsigned long long    u64;

} U_HWTS_AIC15_BASE_ADDR;

/* Define the union U_HWTS_AIC16_BASE_ADDR */
typedef union
{
    /* Define the struct bits */
    struct
    {
        /* The 2 members below is corresponding to the member "aic16_base_addr     " of the struct "" */        unsigned long long    aic16_base_addr_0           ; /* [47..0]  */        unsigned long long    aic16_base_addr_1     : 16  ; /* [47..0]  */
        unsigned long long    reserved_0            : 16  ; /* [63..48]  */
    } bits;

    /* Define an unsigned member */
    unsigned long long    u64;

} U_HWTS_AIC16_BASE_ADDR;

/* Define the union U_HWTS_AIC17_BASE_ADDR */
typedef union
{
    /* Define the struct bits */
    struct
    {
        /* The 2 members below is corresponding to the member "aic17_base_addr     " of the struct "" */        unsigned long long    aic17_base_addr_0           ; /* [47..0]  */        unsigned long long    aic17_base_addr_1     : 16  ; /* [47..0]  */
        unsigned long long    reserved_0            : 16  ; /* [63..48]  */
    } bits;

    /* Define an unsigned member */
    unsigned long long    u64;

} U_HWTS_AIC17_BASE_ADDR;

/* Define the union U_HWTS_AIC18_BASE_ADDR */
typedef union
{
    /* Define the struct bits */
    struct
    {
        /* The 2 members below is corresponding to the member "aic18_base_addr     " of the struct "" */        unsigned long long    aic18_base_addr_0           ; /* [47..0]  */        unsigned long long    aic18_base_addr_1     : 16  ; /* [47..0]  */
        unsigned long long    reserved_0            : 16  ; /* [63..48]  */
    } bits;

    /* Define an unsigned member */
    unsigned long long    u64;

} U_HWTS_AIC18_BASE_ADDR;

/* Define the union U_HWTS_AIC19_BASE_ADDR */
typedef union
{
    /* Define the struct bits */
    struct
    {
        /* The 2 members below is corresponding to the member "aic19_base_addr     " of the struct "" */        unsigned long long    aic19_base_addr_0           ; /* [47..0]  */        unsigned long long    aic19_base_addr_1     : 16  ; /* [47..0]  */
        unsigned long long    reserved_0            : 16  ; /* [63..48]  */
    } bits;

    /* Define an unsigned member */
    unsigned long long    u64;

} U_HWTS_AIC19_BASE_ADDR;

/* Define the union U_HWTS_AIC20_BASE_ADDR */
typedef union
{
    /* Define the struct bits */
    struct
    {
        /* The 2 members below is corresponding to the member "aic20_base_addr     " of the struct "" */        unsigned long long    aic20_base_addr_0           ; /* [47..0]  */        unsigned long long    aic20_base_addr_1     : 16  ; /* [47..0]  */
        unsigned long long    reserved_0            : 16  ; /* [63..48]  */
    } bits;

    /* Define an unsigned member */
    unsigned long long    u64;

} U_HWTS_AIC20_BASE_ADDR;

/* Define the union U_HWTS_AIC21_BASE_ADDR */
typedef union
{
    /* Define the struct bits */
    struct
    {
        /* The 2 members below is corresponding to the member "aic21_base_addr     " of the struct "" */        unsigned long long    aic21_base_addr_0           ; /* [47..0]  */        unsigned long long    aic21_base_addr_1     : 16  ; /* [47..0]  */
        unsigned long long    reserved_0            : 16  ; /* [63..48]  */
    } bits;

    /* Define an unsigned member */
    unsigned long long    u64;

} U_HWTS_AIC21_BASE_ADDR;

/* Define the union U_HWTS_AIC22_BASE_ADDR */
typedef union
{
    /* Define the struct bits */
    struct
    {
        /* The 2 members below is corresponding to the member "aic22_base_addr     " of the struct "" */        unsigned long long    aic22_base_addr_0           ; /* [47..0]  */        unsigned long long    aic22_base_addr_1     : 16  ; /* [47..0]  */
        unsigned long long    reserved_0            : 16  ; /* [63..48]  */
    } bits;

    /* Define an unsigned member */
    unsigned long long    u64;

} U_HWTS_AIC22_BASE_ADDR;

/* Define the union U_HWTS_AIC23_BASE_ADDR */
typedef union
{
    /* Define the struct bits */
    struct
    {
        /* The 2 members below is corresponding to the member "aic23_base_addr     " of the struct "" */        unsigned long long    aic23_base_addr_0           ; /* [47..0]  */        unsigned long long    aic23_base_addr_1     : 16  ; /* [47..0]  */
        unsigned long long    reserved_0            : 16  ; /* [63..48]  */
    } bits;

    /* Define an unsigned member */
    unsigned long long    u64;

} U_HWTS_AIC23_BASE_ADDR;

/* Define the union U_HWTS_AIC24_BASE_ADDR */
typedef union
{
    /* Define the struct bits */
    struct
    {
        /* The 2 members below is corresponding to the member "aic24_base_addr     " of the struct "" */        unsigned long long    aic24_base_addr_0           ; /* [47..0]  */        unsigned long long    aic24_base_addr_1     : 16  ; /* [47..0]  */
        unsigned long long    reserved_0            : 16  ; /* [63..48]  */
    } bits;

    /* Define an unsigned member */
    unsigned long long    u64;

} U_HWTS_AIC24_BASE_ADDR;

/* Define the union U_HWTS_AIC25_BASE_ADDR */
typedef union
{
    /* Define the struct bits */
    struct
    {
        /* The 2 members below is corresponding to the member "aic25_base_addr     " of the struct "" */        unsigned long long    aic25_base_addr_0           ; /* [47..0]  */        unsigned long long    aic25_base_addr_1     : 16  ; /* [47..0]  */
        unsigned long long    reserved_0            : 16  ; /* [63..48]  */
    } bits;

    /* Define an unsigned member */
    unsigned long long    u64;

} U_HWTS_AIC25_BASE_ADDR;

/* Define the union U_HWTS_AIC26_BASE_ADDR */
typedef union
{
    /* Define the struct bits */
    struct
    {
        /* The 2 members below is corresponding to the member "aic26_base_addr     " of the struct "" */        unsigned long long    aic26_base_addr_0           ; /* [47..0]  */        unsigned long long    aic26_base_addr_1     : 16  ; /* [47..0]  */
        unsigned long long    reserved_0            : 16  ; /* [63..48]  */
    } bits;

    /* Define an unsigned member */
    unsigned long long    u64;

} U_HWTS_AIC26_BASE_ADDR;

/* Define the union U_HWTS_AIC27_BASE_ADDR */
typedef union
{
    /* Define the struct bits */
    struct
    {
        /* The 2 members below is corresponding to the member "aic27_base_addr     " of the struct "" */        unsigned long long    aic27_base_addr_0           ; /* [47..0]  */        unsigned long long    aic27_base_addr_1     : 16  ; /* [47..0]  */
        unsigned long long    reserved_0            : 16  ; /* [63..48]  */
    } bits;

    /* Define an unsigned member */
    unsigned long long    u64;

} U_HWTS_AIC27_BASE_ADDR;

/* Define the union U_HWTS_AIC28_BASE_ADDR */
typedef union
{
    /* Define the struct bits */
    struct
    {
        /* The 2 members below is corresponding to the member "aic28_base_addr     " of the struct "" */        unsigned long long    aic28_base_addr_0           ; /* [47..0]  */        unsigned long long    aic28_base_addr_1     : 16  ; /* [47..0]  */
        unsigned long long    reserved_0            : 16  ; /* [63..48]  */
    } bits;

    /* Define an unsigned member */
    unsigned long long    u64;

} U_HWTS_AIC28_BASE_ADDR;

/* Define the union U_HWTS_AIC29_BASE_ADDR */
typedef union
{
    /* Define the struct bits */
    struct
    {
        /* The 2 members below is corresponding to the member "aic29_base_addr     " of the struct "" */        unsigned long long    aic29_base_addr_0           ; /* [47..0]  */        unsigned long long    aic29_base_addr_1     : 16  ; /* [47..0]  */
        unsigned long long    reserved_0            : 16  ; /* [63..48]  */
    } bits;

    /* Define an unsigned member */
    unsigned long long    u64;

} U_HWTS_AIC29_BASE_ADDR;

/* Define the union U_HWTS_AIC30_BASE_ADDR */
typedef union
{
    /* Define the struct bits */
    struct
    {
        /* The 2 members below is corresponding to the member "aic30_base_addr     " of the struct "" */        unsigned long long    aic30_base_addr_0           ; /* [47..0]  */        unsigned long long    aic30_base_addr_1     : 16  ; /* [47..0]  */
        unsigned long long    reserved_0            : 16  ; /* [63..48]  */
    } bits;

    /* Define an unsigned member */
    unsigned long long    u64;

} U_HWTS_AIC30_BASE_ADDR;

/* Define the union U_HWTS_AIC31_BASE_ADDR */
typedef union
{
    /* Define the struct bits */
    struct
    {
        /* The 2 members below is corresponding to the member "aic31_base_addr     " of the struct "" */        unsigned long long    aic31_base_addr_0           ; /* [47..0]  */        unsigned long long    aic31_base_addr_1     : 16  ; /* [47..0]  */
        unsigned long long    reserved_0            : 16  ; /* [63..48]  */
    } bits;

    /* Define an unsigned member */
    unsigned long long    u64;

} U_HWTS_AIC31_BASE_ADDR;

/* Define the union U_HWTS_FREE_CORE_BITMAP */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned long long    aic_free_bitmap             ; /* [31..0]  */
        unsigned long long    reserved_0                  ; /* [63..32]  */
    } bits;

    /* Define an unsigned member */
    unsigned long long    u64;

} U_HWTS_FREE_CORE_BITMAP;

/* Define the union U_HWTS_BUS_ERR_INFO */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned long long    bus_err_type          : 3   ; /* [2..0]  */
        unsigned long long    reserved_0            : 13  ; /* [15..3]  */
        unsigned long long    bus_err_id            : 9   ; /* [24..16]  */
        /* The 2 members below is corresponding to the member "reserved_1          " of the struct "" */        unsigned long long    reserved_1_0          : 7   ; /* [63..25]  */        unsigned long long    reserved_1_1                ; /* [63..25]  */
    } bits;

    /* Define an unsigned member */
    unsigned long long    u64;

} U_HWTS_BUS_ERR_INFO;

/* Define the union U_HWTS_SQE_ERR_INFO */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned long long    sqe_err_sq_idx        : 6   ; /* [5..0]  */
        /* The 2 members below is corresponding to the member "reserved_0          " of the struct "" */        unsigned long long    reserved_0_0          : 26  ; /* [63..6]  */        unsigned long long    reserved_0_1                ; /* [63..6]  */
    } bits;

    /* Define an unsigned member */
    unsigned long long    u64;

} U_HWTS_SQE_ERR_INFO;

/* Define the union U_HWTS_ECC_ERR_INFO0 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned long long    ecc_err_type          : 1   ; /* [0]  */
        unsigned long long    reserved_0            : 15  ; /* [15..1]  */
        unsigned long long    prefetch_buf_ecc_addr : 10  ; /* [25..16]  */
        unsigned long long    reserved_1            : 6   ; /* [31..26]  */
        unsigned long long    profile_buf_ecc_addr  : 9   ; /* [40..32]  */
        unsigned long long    reserved_2            : 7   ; /* [47..41]  */
        unsigned long long    timestamp_buf_ecc_addr : 10  ; /* [57..48]  */
        unsigned long long    reserved_3            : 6   ; /* [63..58]  */
    } bits;

    /* Define an unsigned member */
    unsigned long long    u64;

} U_HWTS_ECC_ERR_INFO0;

/* Define the union U_HWTS_ECC_ERR_INFO1 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned long long    reserved_0            : 16  ; /* [15..0]  */
        unsigned long long    prefetch_buf_ecc_syn  : 9   ; /* [24..16]  */
        unsigned long long    reserved_1            : 7   ; /* [31..25]  */
        unsigned long long    profile_buf_ecc_syn   : 9   ; /* [40..32]  */
        unsigned long long    reserved_2            : 7   ; /* [47..41]  */
        unsigned long long    timestamp_buf_ecc_syn : 8   ; /* [55..48]  */
        unsigned long long    reserved_3            : 8   ; /* [63..56]  */
    } bits;

    /* Define an unsigned member */
    unsigned long long    u64;

} U_HWTS_ECC_ERR_INFO1;

/* Define the union U_HWTS_CFG_ECO0_RESERVED */
typedef union
{
    /* Define the struct bits */
    struct
    {
        /* The 2 members below is corresponding to the member "eco0_cfg            " of the struct "" */        unsigned long long    eco0_cfg_0                  ; /* [63..0]  */        unsigned long long    eco0_cfg_1                  ; /* [63..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned long long    u64;

} U_HWTS_CFG_ECO0_RESERVED;

/* Define the union U_HWTS_CFG_ECO1_RESERVED */
typedef union
{
    /* Define the struct bits */
    struct
    {
        /* The 2 members below is corresponding to the member "eco1_cfg            " of the struct "" */        unsigned long long    eco1_cfg_0                  ; /* [63..0]  */        unsigned long long    eco1_cfg_1                  ; /* [63..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned long long    u64;

} U_HWTS_CFG_ECO1_RESERVED;

/* Define the union U_HWTS_CFG_ECO2_RESERVED */
typedef union
{
    /* Define the struct bits */
    struct
    {
        /* The 2 members below is corresponding to the member "eco2_cfg            " of the struct "" */        unsigned long long    eco2_cfg_0                  ; /* [63..0]  */        unsigned long long    eco2_cfg_1                  ; /* [63..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned long long    u64;

} U_HWTS_CFG_ECO2_RESERVED;

/* Define the union U_HWTS_CFG_ECO3_RESERVED */
typedef union
{
    /* Define the struct bits */
    struct
    {
        /* The 2 members below is corresponding to the member "eco3_cfg            " of the struct "" */        unsigned long long    eco3_cfg_0                  ; /* [63..0]  */        unsigned long long    eco3_cfg_1                  ; /* [63..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned long long    u64;

} U_HWTS_CFG_ECO3_RESERVED;

/* Define the union U_HWTS_ECO0_STATE_RESERVED */
typedef union
{
    /* Define the struct bits */
    struct
    {
        /* The 2 members below is corresponding to the member "eco0_state          " of the struct "" */        unsigned long long    eco0_state_0                ; /* [63..0]  */        unsigned long long    eco0_state_1                ; /* [63..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned long long    u64;

} U_HWTS_ECO0_STATE_RESERVED;

/* Define the union U_HWTS_ECO1_STATE_RESERVED */
typedef union
{
    /* Define the struct bits */
    struct
    {
        /* The 2 members below is corresponding to the member "eco1_state          " of the struct "" */        unsigned long long    eco1_state_0                ; /* [63..0]  */        unsigned long long    eco1_state_1                ; /* [63..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned long long    u64;

} U_HWTS_ECO1_STATE_RESERVED;

/* Define the union U_HWTS_ECO2_STATE_RESERVED */
typedef union
{
    /* Define the struct bits */
    struct
    {
        /* The 2 members below is corresponding to the member "eco2_state          " of the struct "" */        unsigned long long    eco2_state_0                ; /* [63..0]  */        unsigned long long    eco2_state_1                ; /* [63..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned long long    u64;

} U_HWTS_ECO2_STATE_RESERVED;

/* Define the union U_HWTS_ECO3_STATE_RESERVED */
typedef union
{
    /* Define the struct bits */
    struct
    {
        /* The 2 members below is corresponding to the member "eco3_state          " of the struct "" */        unsigned long long    eco3_state_0                ; /* [63..0]  */        unsigned long long    eco3_state_1                ; /* [63..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned long long    u64;

} U_HWTS_ECO3_STATE_RESERVED;

/* Define the union U_HWTS_FREE_SDMA_SQ_BITMAP */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned long long    sdma_ch_free_bitmap   : 8   ; /* [7..0]  */
        /* The 2 members below is corresponding to the member "reserved_0          " of the struct "" */        unsigned long long    reserved_0_0          : 24  ; /* [63..8]  */        unsigned long long    reserved_0_1                ; /* [63..8]  */
    } bits;

    /* Define an unsigned member */
    unsigned long long    u64;

} U_HWTS_FREE_SDMA_SQ_BITMAP;

/* Define the union U_HWTS_BLK_FSM_STATE */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned long long    dfx_blk_fsm_state     : 5   ; /* [4..0]  */
        unsigned long long    reserved_0            : 3   ; /* [7..5]  */
        unsigned long long    dfx_blk_fsm_ost_cnt   : 1   ; /* [8]  */
        unsigned long long    dfx_blk_fsm_blk_log_ost_cnt : 2   ; /* [10..9]  */
        unsigned long long    dfx_blk_fsm_trace_ost_cnt : 1   ; /* [11]  */
        /* The 2 members below is corresponding to the member "reserved_1          " of the struct "" */        unsigned long long    reserved_1_0          : 20  ; /* [63..12]  */        unsigned long long    reserved_1_1                ; /* [63..12]  */
    } bits;

    /* Define an unsigned member */
    unsigned long long    u64;

} U_HWTS_BLK_FSM_STATE;

/* Define the union U_HWTS_L1_NORMAL_INTERRUPT */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned long long    l1_aic_done_status    : 1   ; /* [0]  */
        unsigned long long    l1_pre_paused_status  : 1   ; /* [1]  */
        unsigned long long    l1_post_paused_status : 1   ; /* [2]  */
        unsigned long long    l1_cq_full_status     : 1   ; /* [3]  */
        unsigned long long    l1_task_paused_status : 1   ; /* [4]  */
        unsigned long long    l1_event_record_status : 1   ; /* [5]  */
        unsigned long long    l1_event_wait_status  : 1   ; /* [6]  */
        unsigned long long    l1_sq_timeout_status  : 1   ; /* [7]  */
        unsigned long long    l1_profile_almost_full : 1   ; /* [8]  */
        unsigned long long    l1_pc_trace_almost_full : 1   ; /* [9]  */
        unsigned long long    l1_log_almost_full    : 1   ; /* [10]  */
        /* The 2 members below is corresponding to the member "reserved_0          " of the struct "" */        unsigned long long    reserved_0_0          : 21  ; /* [63..11]  */        unsigned long long    reserved_0_1                ; /* [63..11]  */
    } bits;

    /* Define an unsigned member */
    unsigned long long    u64;

} U_HWTS_L1_NORMAL_INTERRUPT;

/* Define the union U_HWTS_L1_DEBUG_INTERRUPT */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned long long    l1_aic_debug_status   : 1   ; /* [0]  */
        unsigned long long    l1_debug_paused_status : 1   ; /* [1]  */
        /* The 2 members below is corresponding to the member "reserved_0          " of the struct "" */        unsigned long long    reserved_0_0          : 30  ; /* [63..2]  */        unsigned long long    reserved_0_1                ; /* [63..2]  */
    } bits;

    /* Define an unsigned member */
    unsigned long long    u64;

} U_HWTS_L1_DEBUG_INTERRUPT;

/* Define the union U_HWTS_L1_ERROR_INTERRUPT */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned long long    l1_aic_error_status   : 1   ; /* [0]  */
        unsigned long long    l1_task_timeout_status : 1   ; /* [1]  */
        unsigned long long    l1_aic_trap_status    : 1   ; /* [2]  */
        unsigned long long    l1_bus_error          : 1   ; /* [3]  */
        unsigned long long    l1_sqe_error          : 1   ; /* [4]  */
        unsigned long long    l1_ecc_error          : 1   ; /* [5]  */
        unsigned long long    l1_sdma_error_status  : 1   ; /* [6]  */
        unsigned long long    l1_sdma_timeout_status : 1   ; /* [7]  */
        /* The 2 members below is corresponding to the member "reserved_0          " of the struct "" */        unsigned long long    reserved_0_0          : 24  ; /* [63..8]  */        unsigned long long    reserved_0_1                ; /* [63..8]  */
    } bits;

    /* Define an unsigned member */
    unsigned long long    u64;

} U_HWTS_L1_ERROR_INTERRUPT;

/* Define the union U_HWTS_L1_NORMAL_INTERRUPT_MASK */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned long long    l1_aic_done_mask      : 1   ; /* [0]  */
        unsigned long long    l1_pre_paused_mask    : 1   ; /* [1]  */
        unsigned long long    l1_post_paused_mask   : 1   ; /* [2]  */
        unsigned long long    l1_cq_full_mask       : 1   ; /* [3]  */
        unsigned long long    l1_task_paused_mask   : 1   ; /* [4]  */
        unsigned long long    l1_event_record_mask  : 1   ; /* [5]  */
        unsigned long long    l1_event_wait_mask    : 1   ; /* [6]  */
        unsigned long long    l1_sq_timeout_mask    : 1   ; /* [7]  */
        unsigned long long    l1_profile_almost_full_mask : 1   ; /* [8]  */
        unsigned long long    l1_trace_almost_full_mask : 1   ; /* [9]  */
        unsigned long long    l1_log_almost_full_mask : 1   ; /* [10]  */
        /* The 2 members below is corresponding to the member "reserved_0          " of the struct "" */        unsigned long long    reserved_0_0          : 21  ; /* [63..11]  */        unsigned long long    reserved_0_1                ; /* [63..11]  */
    } bits;

    /* Define an unsigned member */
    unsigned long long    u64;

} U_HWTS_L1_NORMAL_INTERRUPT_MASK;

/* Define the union U_HWTS_L1_DEBUG_INTERRUPT_MASK */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned long long    l1_aic_debug_mask     : 1   ; /* [0]  */
        unsigned long long    l1_debug_paused_mask  : 1   ; /* [1]  */
        /* The 2 members below is corresponding to the member "reserved_0          " of the struct "" */        unsigned long long    reserved_0_0          : 30  ; /* [63..2]  */        unsigned long long    reserved_0_1                ; /* [63..2]  */
    } bits;

    /* Define an unsigned member */
    unsigned long long    u64;

} U_HWTS_L1_DEBUG_INTERRUPT_MASK;

/* Define the union U_HWTS_L1_ERROR_INTERRUPT_MASK */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned long long    l1_aic_error_mask     : 1   ; /* [0]  */
        unsigned long long    l1_task_timeout_mask  : 1   ; /* [1]  */
        unsigned long long    l1_aic_trap_mask      : 1   ; /* [2]  */
        unsigned long long    l1_bus_error_mask     : 1   ; /* [3]  */
        unsigned long long    l1_sqe_error_mask     : 1   ; /* [4]  */
        unsigned long long    l1_ecc_error_mask     : 1   ; /* [5]  */
        unsigned long long    l1_sdma_error_mask    : 1   ; /* [6]  */
        unsigned long long    l1_sdma_timeout_mask  : 1   ; /* [7]  */
        /* The 2 members below is corresponding to the member "reserved_0          " of the struct "" */        unsigned long long    reserved_0_0          : 24  ; /* [63..8]  */        unsigned long long    reserved_0_1                ; /* [63..8]  */
    } bits;

    /* Define an unsigned member */
    unsigned long long    u64;

} U_HWTS_L1_ERROR_INTERRUPT_MASK;

/* Define the union U_HWTS_L1_NORMAL_INTERRUPT_FORCE */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned long long    reserved_0            : 8   ; /* [7..0]  */
        unsigned long long    l1_profile_almost_full_force : 1   ; /* [8]  */
        unsigned long long    l1_trace_almost_full_force : 1   ; /* [9]  */
        unsigned long long    l1_log_almost_full_force : 1   ; /* [10]  */
        /* The 2 members below is corresponding to the member "reserved_1          " of the struct "" */        unsigned long long    reserved_1_0          : 21  ; /* [63..11]  */        unsigned long long    reserved_1_1                ; /* [63..11]  */
    } bits;

    /* Define an unsigned member */
    unsigned long long    u64;

} U_HWTS_L1_NORMAL_INTERRUPT_FORCE;

/* Define the union U_HWTS_L1_ERROR_INTERRUPT_FORCE */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned long long    reserved_0            : 3   ; /* [2..0]  */
        unsigned long long    l1_bus_error_force    : 1   ; /* [3]  */
        unsigned long long    l1_sqe_error_force    : 1   ; /* [4]  */
        unsigned long long    l1_ecc_error_force    : 1   ; /* [5]  */
        /* The 2 members below is corresponding to the member "reserved_1          " of the struct "" */        unsigned long long    reserved_1_0          : 26  ; /* [63..6]  */        unsigned long long    reserved_1_1                ; /* [63..6]  */
    } bits;

    /* Define an unsigned member */
    unsigned long long    u64;

} U_HWTS_L1_ERROR_INTERRUPT_FORCE;

/* Define the union U_HWTS_L1_NORMAL_INTERRUPT_MASK_SET */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned long long    l1_aic_done_mask_set  : 1   ; /* [0]  */
        unsigned long long    l1_pre_paused_mask_set : 1   ; /* [1]  */
        unsigned long long    l1_post_paused_mask_set : 1   ; /* [2]  */
        unsigned long long    l1_cq_full_mask_set   : 1   ; /* [3]  */
        unsigned long long    l1_task_paused_mask_set : 1   ; /* [4]  */
        unsigned long long    l1_event_record_mask_set : 1   ; /* [5]  */
        unsigned long long    l1_event_wait_mask_set : 1   ; /* [6]  */
        unsigned long long    l1_sq_timeout_mask_set : 1   ; /* [7]  */
        unsigned long long    l1_profile_almost_full_mask_set : 1   ; /* [8]  */
        unsigned long long    l1_trace_almost_full_mask_set : 1   ; /* [9]  */
        unsigned long long    l1_log_almost_full_mask_set : 1   ; /* [10]  */
        /* The 2 members below is corresponding to the member "reserved_0          " of the struct "" */        unsigned long long    reserved_0_0          : 21  ; /* [63..11]  */        unsigned long long    reserved_0_1                ; /* [63..11]  */
    } bits;

    /* Define an unsigned member */
    unsigned long long    u64;

} U_HWTS_L1_NORMAL_INTERRUPT_MASK_SET;

/* Define the union U_HWTS_L1_DEBUG_INTERRUPT_MASK_SET */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned long long    l1_aic_debug_mask_set : 1   ; /* [0]  */
        unsigned long long    l1_debug_paused_mask_set : 1   ; /* [1]  */
        /* The 2 members below is corresponding to the member "reserved_0          " of the struct "" */        unsigned long long    reserved_0_0          : 30  ; /* [63..2]  */        unsigned long long    reserved_0_1                ; /* [63..2]  */
    } bits;

    /* Define an unsigned member */
    unsigned long long    u64;

} U_HWTS_L1_DEBUG_INTERRUPT_MASK_SET;

/* Define the union U_HWTS_L1_ERROR_INTERRUPT_MASK_SET */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned long long    l1_aic_error_mask_set : 1   ; /* [0]  */
        unsigned long long    l1_task_timeout_mask_set : 1   ; /* [1]  */
        unsigned long long    l1_aic_trap_mask_set  : 1   ; /* [2]  */
        unsigned long long    l1_bus_error_mask_set : 1   ; /* [3]  */
        unsigned long long    l1_sqe_error_mask_set : 1   ; /* [4]  */
        unsigned long long    l1_ecc_error_mask_set : 1   ; /* [5]  */
        unsigned long long    l1_sdma_error_mask_set : 1   ; /* [6]  */
        unsigned long long    l1_sdma_timeout_mask_set : 1   ; /* [7]  */
        /* The 2 members below is corresponding to the member "reserved_0          " of the struct "" */        unsigned long long    reserved_0_0          : 24  ; /* [63..8]  */        unsigned long long    reserved_0_1                ; /* [63..8]  */
    } bits;

    /* Define an unsigned member */
    unsigned long long    u64;

} U_HWTS_L1_ERROR_INTERRUPT_MASK_SET;

/* Define the union U_HWTS_L1_NORMAL_INTERRUPT_MASK_CLR */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned long long    l1_aic_done_mask_clr  : 1   ; /* [0]  */
        unsigned long long    l1_pre_paused_mask_clr : 1   ; /* [1]  */
        unsigned long long    l1_post_paused_mask_clr : 1   ; /* [2]  */
        unsigned long long    l1_cq_full_mask_clr   : 1   ; /* [3]  */
        unsigned long long    l1_task_paused_mask_clr : 1   ; /* [4]  */
        unsigned long long    l1_event_record_mask_clr : 1   ; /* [5]  */
        unsigned long long    l1_event_wait_mask_clr : 1   ; /* [6]  */
        unsigned long long    l1_sq_timeout_mask_clr : 1   ; /* [7]  */
        unsigned long long    l1_profile_almost_full_mask_clr : 1   ; /* [8]  */
        unsigned long long    l1_trace_almost_full_mask_clr : 1   ; /* [9]  */
        unsigned long long    l1_log_almost_full_mask_clr : 1   ; /* [10]  */
        /* The 2 members below is corresponding to the member "reserved_0          " of the struct "" */        unsigned long long    reserved_0_0          : 21  ; /* [63..11]  */        unsigned long long    reserved_0_1                ; /* [63..11]  */
    } bits;

    /* Define an unsigned member */
    unsigned long long    u64;

} U_HWTS_L1_NORMAL_INTERRUPT_MASK_CLR;

/* Define the union U_HWTS_L1_DEBUG_INTERRUPT_MASK_CLR */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned long long    l1_aic_debug_mask_clr : 1   ; /* [0]  */
        unsigned long long    l1_debug_paused_mask_clr : 1   ; /* [1]  */
        /* The 2 members below is corresponding to the member "reserved_0          " of the struct "" */        unsigned long long    reserved_0_0          : 30  ; /* [63..2]  */        unsigned long long    reserved_0_1                ; /* [63..2]  */
    } bits;

    /* Define an unsigned member */
    unsigned long long    u64;

} U_HWTS_L1_DEBUG_INTERRUPT_MASK_CLR;

/* Define the union U_HWTS_L1_ERROR_INTERRUPT_MASK_CLR */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned long long    l1_aic_error_mask_clr : 1   ; /* [0]  */
        unsigned long long    l1_task_timeout_mask_clr : 1   ; /* [1]  */
        unsigned long long    l1_aic_trap_mask_clr  : 1   ; /* [2]  */
        unsigned long long    l1_bus_error_mask_clr : 1   ; /* [3]  */
        unsigned long long    l1_sqe_error_mask_clr : 1   ; /* [4]  */
        unsigned long long    l1_ecc_error_mask_clr : 1   ; /* [5]  */
        unsigned long long    l1_sdma_error_mask_clr : 1   ; /* [6]  */
        unsigned long long    l1_sdma_timeout_mask_clr : 1   ; /* [7]  */
        /* The 2 members below is corresponding to the member "reserved_0          " of the struct "" */        unsigned long long    reserved_0_0          : 24  ; /* [63..8]  */        unsigned long long    reserved_0_1                ; /* [63..8]  */
    } bits;

    /* Define an unsigned member */
    unsigned long long    u64;

} U_HWTS_L1_ERROR_INTERRUPT_MASK_CLR;

/* Define the union U_HWTS_L1_NORMAL_INTERRUPT_PENDING */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned long long    l1_aic_done_pending   : 1   ; /* [0]  */
        unsigned long long    l1_pre_paused_pending : 1   ; /* [1]  */
        unsigned long long    l1_post_paused_pending : 1   ; /* [2]  */
        unsigned long long    l1_cq_full_pending    : 1   ; /* [3]  */
        unsigned long long    l1_task_paused_pending : 1   ; /* [4]  */
        unsigned long long    l1_event_record_pending : 1   ; /* [5]  */
        unsigned long long    l1_event_wait_pending : 1   ; /* [6]  */
        unsigned long long    l1_sq_timeout_pending : 1   ; /* [7]  */
        unsigned long long    l1_profile_almost_full_pending : 1   ; /* [8]  */
        unsigned long long    l1_pc_trace_almost_full_pending : 1   ; /* [9]  */
        unsigned long long    l1_log_almost_full_pending : 1   ; /* [10]  */
        /* The 2 members below is corresponding to the member "reserved_0          " of the struct "" */        unsigned long long    reserved_0_0          : 21  ; /* [63..11]  */        unsigned long long    reserved_0_1                ; /* [63..11]  */
    } bits;

    /* Define an unsigned member */
    unsigned long long    u64;

} U_HWTS_L1_NORMAL_INTERRUPT_PENDING;

/* Define the union U_HWTS_L1_DEBUG_INTERRUPT_PENDING */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned long long    l1_aic_debug_pending  : 1   ; /* [0]  */
        unsigned long long    l1_debug_paused_pending : 1   ; /* [1]  */
        /* The 2 members below is corresponding to the member "reserved_0          " of the struct "" */        unsigned long long    reserved_0_0          : 30  ; /* [63..2]  */        unsigned long long    reserved_0_1                ; /* [63..2]  */
    } bits;

    /* Define an unsigned member */
    unsigned long long    u64;

} U_HWTS_L1_DEBUG_INTERRUPT_PENDING;

/* Define the union U_HWTS_L1_ERROR_INTERRUPT_PENDING */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned long long    l1_aic_error_pending  : 1   ; /* [0]  */
        unsigned long long    l1_task_timeout_pending : 1   ; /* [1]  */
        unsigned long long    l1_aic_trap_pending   : 1   ; /* [2]  */
        unsigned long long    l1_bus_error_pending  : 1   ; /* [3]  */
        unsigned long long    l1_sqe_error_pending  : 1   ; /* [4]  */
        unsigned long long    l1_ecc_error_pending  : 1   ; /* [5]  */
        unsigned long long    l1_sdma_error_pending : 1   ; /* [6]  */
        unsigned long long    l1_sdma_timeout_pending : 1   ; /* [7]  */
        /* The 2 members below is corresponding to the member "reserved_0          " of the struct "" */        unsigned long long    reserved_0_0          : 24  ; /* [63..8]  */        unsigned long long    reserved_0_1                ; /* [63..8]  */
    } bits;

    /* Define an unsigned member */
    unsigned long long    u64;

} U_HWTS_L1_ERROR_INTERRUPT_PENDING;

/* Define the union U_SQ_DONE_PENDING */
typedef union
{
    /* Define the struct bits */
    struct
    {
        /* The 2 members below is corresponding to the member "sq_norm_int_pending " of the struct "" */        unsigned long long    sq_norm_int_pending_0       ; /* [51..0]  */        unsigned long long    sq_norm_int_pending_1 : 20  ; /* [51..0]  */
        unsigned long long    reserved_0            : 12  ; /* [63..52]  */
    } bits;

    /* Define an unsigned member */
    unsigned long long    u64;

} U_SQ_DONE_PENDING;

/* Define the union U_SQ_DEBUG_PENDING */
typedef union
{
    /* Define the struct bits */
    struct
    {
        /* The 2 members below is corresponding to the member "sq_debug_int_pending" of the struct "" */        unsigned long long    sq_debug_int_pending_0      ; /* [51..0]  */        unsigned long long    sq_debug_int_pending_1 : 20  ; /* [51..0]  */
        unsigned long long    reserved_0            : 12  ; /* [63..52]  */
    } bits;

    /* Define an unsigned member */
    unsigned long long    u64;

} U_SQ_DEBUG_PENDING;

/* Define the union U_SQ_EXCEPTION_PENDING */
typedef union
{
    /* Define the struct bits */
    struct
    {
        /* The 2 members below is corresponding to the member "sq_exception_int_pending" of the struct "" */        unsigned long long    sq_exception_int_pending_0  ; /* [51..0]  */        unsigned long long    sq_exception_int_pending_1 : 20  ; /* [51..0]  */
        unsigned long long    reserved_0            : 12  ; /* [63..52]  */
    } bits;

    /* Define an unsigned member */
    unsigned long long    u64;

} U_SQ_EXCEPTION_PENDING;

/* Define the union U_HWTS_L2_NORMAL_AIC_DONE_INTERRUPT */
typedef union
{
    /* Define the struct bits */
    struct
    {
        /* The 2 members below is corresponding to the member "l2_aic_done_status  " of the struct "" */        unsigned long long    l2_aic_done_status_0        ; /* [51..0]  */        unsigned long long    l2_aic_done_status_1  : 20  ; /* [51..0]  */
        unsigned long long    reserved_0            : 12  ; /* [63..52]  */
    } bits;

    /* Define an unsigned member */
    unsigned long long    u64;

} U_HWTS_L2_NORMAL_AIC_DONE_INTERRUPT;

/* Define the union U_HWTS_L2_NORMAL_PRE_PAUSED_INTERRUPT */
typedef union
{
    /* Define the struct bits */
    struct
    {
        /* The 2 members below is corresponding to the member "l2_pre_paused_status" of the struct "" */        unsigned long long    l2_pre_paused_status_0      ; /* [51..0]  */        unsigned long long    l2_pre_paused_status_1 : 20  ; /* [51..0]  */
        unsigned long long    reserved_0            : 12  ; /* [63..52]  */
    } bits;

    /* Define an unsigned member */
    unsigned long long    u64;

} U_HWTS_L2_NORMAL_PRE_PAUSED_INTERRUPT;

/* Define the union U_HWTS_L2_NORMAL_POST_PAUSED_INTERRUPT */
typedef union
{
    /* Define the struct bits */
    struct
    {
        /* The 2 members below is corresponding to the member "l2_post_paused_status" of the struct "" */        unsigned long long    l2_post_paused_status_0     ; /* [51..0]  */        unsigned long long    l2_post_paused_status_1 : 20  ; /* [51..0]  */
        unsigned long long    reserved_0            : 12  ; /* [63..52]  */
    } bits;

    /* Define an unsigned member */
    unsigned long long    u64;

} U_HWTS_L2_NORMAL_POST_PAUSED_INTERRUPT;

/* Define the union U_HWTS_L2_NORMAL_CQ_FULL_INTERRUPT */
typedef union
{
    /* Define the struct bits */
    struct
    {
        /* The 2 members below is corresponding to the member "l2_cq_full_status   " of the struct "" */        unsigned long long    l2_cq_full_status_0         ; /* [51..0]  */        unsigned long long    l2_cq_full_status_1   : 20  ; /* [51..0]  */
        unsigned long long    reserved_0            : 12  ; /* [63..52]  */
    } bits;

    /* Define an unsigned member */
    unsigned long long    u64;

} U_HWTS_L2_NORMAL_CQ_FULL_INTERRUPT;

/* Define the union U_HWTS_L2_NORMAL_TASK_PAUSED_INTERRUPT */
typedef union
{
    /* Define the struct bits */
    struct
    {
        /* The 2 members below is corresponding to the member "l2_task_paused_status" of the struct "" */        unsigned long long    l2_task_paused_status_0     ; /* [51..0]  */        unsigned long long    l2_task_paused_status_1 : 20  ; /* [51..0]  */
        unsigned long long    reserved_0            : 12  ; /* [63..52]  */
    } bits;

    /* Define an unsigned member */
    unsigned long long    u64;

} U_HWTS_L2_NORMAL_TASK_PAUSED_INTERRUPT;

/* Define the union U_HWTS_L2_NORMAL_EVENT_RECORD_INTERRUPT */
typedef union
{
    /* Define the struct bits */
    struct
    {
        /* The 2 members below is corresponding to the member "l2_event_record_status" of the struct "" */        unsigned long long    l2_event_record_status_0    ; /* [63..0]  */        unsigned long long    l2_event_record_status_1    ; /* [63..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned long long    u64;

} U_HWTS_L2_NORMAL_EVENT_RECORD_INTERRUPT;

/* Define the union U_HWTS_L2_NORMAL_EVENT_WAIT_INTERRUPT */
typedef union
{
    /* Define the struct bits */
    struct
    {
        /* The 2 members below is corresponding to the member "l2_event_wait_status" of the struct "" */        unsigned long long    l2_event_wait_status_0      ; /* [51..0]  */        unsigned long long    l2_event_wait_status_1 : 20  ; /* [51..0]  */
        unsigned long long    reserved_0            : 12  ; /* [63..52]  */
    } bits;

    /* Define an unsigned member */
    unsigned long long    u64;

} U_HWTS_L2_NORMAL_EVENT_WAIT_INTERRUPT;

/* Define the union U_HWTS_L2_NORMAL_SQ_TIMEOUT_INTERRUPT */
typedef union
{
    /* Define the struct bits */
    struct
    {
        /* The 2 members below is corresponding to the member "l2_sq_timeout_status" of the struct "" */        unsigned long long    l2_sq_timeout_status_0      ; /* [51..0]  */        unsigned long long    l2_sq_timeout_status_1 : 20  ; /* [51..0]  */
        unsigned long long    reserved_0            : 12  ; /* [63..52]  */
    } bits;

    /* Define an unsigned member */
    unsigned long long    u64;

} U_HWTS_L2_NORMAL_SQ_TIMEOUT_INTERRUPT;

/* Define the union U_HWTS_L2_DEBUG_AIC_DEBUG_INTERRUPT */
typedef union
{
    /* Define the struct bits */
    struct
    {
        /* The 2 members below is corresponding to the member "l2_aic_debug_status " of the struct "" */        unsigned long long    l2_aic_debug_status_0       ; /* [51..0]  */        unsigned long long    l2_aic_debug_status_1 : 20  ; /* [51..0]  */
        unsigned long long    reserved_0            : 12  ; /* [63..52]  */
    } bits;

    /* Define an unsigned member */
    unsigned long long    u64;

} U_HWTS_L2_DEBUG_AIC_DEBUG_INTERRUPT;

/* Define the union U_HWTS_L2_DEBUG_DEBUG_PAUSED_INTERRUPT */
typedef union
{
    /* Define the struct bits */
    struct
    {
        /* The 2 members below is corresponding to the member "l2_debug_paused_status" of the struct "" */        unsigned long long    l2_debug_paused_status_0    ; /* [51..0]  */        unsigned long long    l2_debug_paused_status_1 : 20  ; /* [51..0]  */
        unsigned long long    reserved_0            : 12  ; /* [63..52]  */
    } bits;

    /* Define an unsigned member */
    unsigned long long    u64;

} U_HWTS_L2_DEBUG_DEBUG_PAUSED_INTERRUPT;

/* Define the union U_HWTS_L2_ERROR_AIC_ERROR_INTERRUPT */
typedef union
{
    /* Define the struct bits */
    struct
    {
        /* The 2 members below is corresponding to the member "l2_aic_error_status " of the struct "" */        unsigned long long    l2_aic_error_status_0       ; /* [51..0]  */        unsigned long long    l2_aic_error_status_1 : 20  ; /* [51..0]  */
        unsigned long long    reserved_0            : 12  ; /* [63..52]  */
    } bits;

    /* Define an unsigned member */
    unsigned long long    u64;

} U_HWTS_L2_ERROR_AIC_ERROR_INTERRUPT;

/* Define the union U_HWTS_L2_ERROR_TASK_TIMEOUT_INTERRUPT */
typedef union
{
    /* Define the struct bits */
    struct
    {
        /* The 2 members below is corresponding to the member "l2_task_timeout_status" of the struct "" */        unsigned long long    l2_task_timeout_status_0    ; /* [51..0]  */        unsigned long long    l2_task_timeout_status_1 : 20  ; /* [51..0]  */
        unsigned long long    reserved_0            : 12  ; /* [63..52]  */
    } bits;

    /* Define an unsigned member */
    unsigned long long    u64;

} U_HWTS_L2_ERROR_TASK_TIMEOUT_INTERRUPT;

/* Define the union U_HWTS_L2_ERROR_AIC_TRAP_INTERRUPT */
typedef union
{
    /* Define the struct bits */
    struct
    {
        /* The 2 members below is corresponding to the member "l2_aic_trap_status  " of the struct "" */        unsigned long long    l2_aic_trap_status_0        ; /* [51..0]  */        unsigned long long    l2_aic_trap_status_1  : 20  ; /* [51..0]  */
        unsigned long long    reserved_0            : 12  ; /* [63..52]  */
    } bits;

    /* Define an unsigned member */
    unsigned long long    u64;

} U_HWTS_L2_ERROR_AIC_TRAP_INTERRUPT;

/* Define the union U_HWTS_L2_NORMAL_AIC_DONE_INTERRUPT_MASK */
typedef union
{
    /* Define the struct bits */
    struct
    {
        /* The 2 members below is corresponding to the member "l2_aic_done_mask    " of the struct "" */        unsigned long long    l2_aic_done_mask_0          ; /* [51..0]  */        unsigned long long    l2_aic_done_mask_1    : 20  ; /* [51..0]  */
        unsigned long long    reserved_0            : 12  ; /* [63..52]  */
    } bits;

    /* Define an unsigned member */
    unsigned long long    u64;

} U_HWTS_L2_NORMAL_AIC_DONE_INTERRUPT_MASK;

/* Define the union U_HWTS_L2_NORMAL_PRE_PAUSED_INTERRUPT_MASK */
typedef union
{
    /* Define the struct bits */
    struct
    {
        /* The 2 members below is corresponding to the member "l2_pre_paused_mask  " of the struct "" */        unsigned long long    l2_pre_paused_mask_0        ; /* [51..0]  */        unsigned long long    l2_pre_paused_mask_1  : 20  ; /* [51..0]  */
        unsigned long long    reserved_0            : 12  ; /* [63..52]  */
    } bits;

    /* Define an unsigned member */
    unsigned long long    u64;

} U_HWTS_L2_NORMAL_PRE_PAUSED_INTERRUPT_MASK;

/* Define the union U_HWTS_L2_NORMAL_POST_PAUSED_INTERRUPT_MASK */
typedef union
{
    /* Define the struct bits */
    struct
    {
        /* The 2 members below is corresponding to the member "l2_post_paused_mask " of the struct "" */        unsigned long long    l2_post_paused_mask_0       ; /* [51..0]  */        unsigned long long    l2_post_paused_mask_1 : 20  ; /* [51..0]  */
        unsigned long long    reserved_0            : 12  ; /* [63..52]  */
    } bits;

    /* Define an unsigned member */
    unsigned long long    u64;

} U_HWTS_L2_NORMAL_POST_PAUSED_INTERRUPT_MASK;

/* Define the union U_HWTS_L2_NORMAL_CQ_FULL_INTERRUPT_MASK */
typedef union
{
    /* Define the struct bits */
    struct
    {
        /* The 2 members below is corresponding to the member "l2_cq_full_mask     " of the struct "" */        unsigned long long    l2_cq_full_mask_0           ; /* [51..0]  */        unsigned long long    l2_cq_full_mask_1     : 20  ; /* [51..0]  */
        unsigned long long    reserved_0            : 12  ; /* [63..52]  */
    } bits;

    /* Define an unsigned member */
    unsigned long long    u64;

} U_HWTS_L2_NORMAL_CQ_FULL_INTERRUPT_MASK;

/* Define the union U_HWTS_L2_NORMAL_TASK_PAUSED_INTERRUPT_MASK */
typedef union
{
    /* Define the struct bits */
    struct
    {
        /* The 2 members below is corresponding to the member "l2_task_paused_mask " of the struct "" */        unsigned long long    l2_task_paused_mask_0       ; /* [51..0]  */        unsigned long long    l2_task_paused_mask_1 : 20  ; /* [51..0]  */
        unsigned long long    reserved_0            : 12  ; /* [63..52]  */
    } bits;

    /* Define an unsigned member */
    unsigned long long    u64;

} U_HWTS_L2_NORMAL_TASK_PAUSED_INTERRUPT_MASK;

/* Define the union U_HWTS_L2_NORMAL_EVENT_RECORD_INTERRUPT_MASK */
typedef union
{
    /* Define the struct bits */
    struct
    {
        /* The 2 members below is corresponding to the member "l2_event_record_mask" of the struct "" */        unsigned long long    l2_event_record_mask_0      ; /* [63..0]  */        unsigned long long    l2_event_record_mask_1      ; /* [63..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned long long    u64;

} U_HWTS_L2_NORMAL_EVENT_RECORD_INTERRUPT_MASK;

/* Define the union U_HWTS_L2_NORMAL_EVENT_WAIT_INTERRUPT_MASK */
typedef union
{
    /* Define the struct bits */
    struct
    {
        /* The 2 members below is corresponding to the member "l2_event_wait_mask  " of the struct "" */        unsigned long long    l2_event_wait_mask_0        ; /* [51..0]  */        unsigned long long    l2_event_wait_mask_1  : 20  ; /* [51..0]  */
        unsigned long long    reserved_0            : 12  ; /* [63..52]  */
    } bits;

    /* Define an unsigned member */
    unsigned long long    u64;

} U_HWTS_L2_NORMAL_EVENT_WAIT_INTERRUPT_MASK;

/* Define the union U_HWTS_L2_NORMAL_SQ_TIMEOUT_INTERRUPT_MASK */
typedef union
{
    /* Define the struct bits */
    struct
    {
        /* The 2 members below is corresponding to the member "l2_sq_timeout_mask  " of the struct "" */        unsigned long long    l2_sq_timeout_mask_0        ; /* [51..0]  */        unsigned long long    l2_sq_timeout_mask_1  : 20  ; /* [51..0]  */
        unsigned long long    reserved_0            : 12  ; /* [63..52]  */
    } bits;

    /* Define an unsigned member */
    unsigned long long    u64;

} U_HWTS_L2_NORMAL_SQ_TIMEOUT_INTERRUPT_MASK;

/* Define the union U_HWTS_L2_DEBUG_AIC_DEBUG_INTERRUPT_MASK */
typedef union
{
    /* Define the struct bits */
    struct
    {
        /* The 2 members below is corresponding to the member "l2_aic_debug_mask   " of the struct "" */        unsigned long long    l2_aic_debug_mask_0         ; /* [51..0]  */        unsigned long long    l2_aic_debug_mask_1   : 20  ; /* [51..0]  */
        unsigned long long    reserved_0            : 12  ; /* [63..52]  */
    } bits;

    /* Define an unsigned member */
    unsigned long long    u64;

} U_HWTS_L2_DEBUG_AIC_DEBUG_INTERRUPT_MASK;

/* Define the union U_HWTS_L2_DEBUG_DEBUG_PAUSED_INTERRUPT_MASK */
typedef union
{
    /* Define the struct bits */
    struct
    {
        /* The 2 members below is corresponding to the member "l2_debug_paused_mask" of the struct "" */        unsigned long long    l2_debug_paused_mask_0      ; /* [51..0]  */        unsigned long long    l2_debug_paused_mask_1 : 20  ; /* [51..0]  */
        unsigned long long    reserved_0            : 12  ; /* [63..52]  */
    } bits;

    /* Define an unsigned member */
    unsigned long long    u64;

} U_HWTS_L2_DEBUG_DEBUG_PAUSED_INTERRUPT_MASK;

/* Define the union U_HWTS_L2_ERROR_AIC_ERROR_INTERRUPT_MASK */
typedef union
{
    /* Define the struct bits */
    struct
    {
        /* The 2 members below is corresponding to the member "l2_aic_error_mask   " of the struct "" */        unsigned long long    l2_aic_error_mask_0         ; /* [51..0]  */        unsigned long long    l2_aic_error_mask_1   : 20  ; /* [51..0]  */
        unsigned long long    reserved_0            : 12  ; /* [63..52]  */
    } bits;

    /* Define an unsigned member */
    unsigned long long    u64;

} U_HWTS_L2_ERROR_AIC_ERROR_INTERRUPT_MASK;

/* Define the union U_HWTS_L2_ERROR_TASK_TIMEOUT_INTERRUPT_MASK */
typedef union
{
    /* Define the struct bits */
    struct
    {
        /* The 2 members below is corresponding to the member "l2_task_timeout_mask" of the struct "" */        unsigned long long    l2_task_timeout_mask_0      ; /* [51..0]  */        unsigned long long    l2_task_timeout_mask_1 : 20  ; /* [51..0]  */
        unsigned long long    reserved_0            : 12  ; /* [63..52]  */
    } bits;

    /* Define an unsigned member */
    unsigned long long    u64;

} U_HWTS_L2_ERROR_TASK_TIMEOUT_INTERRUPT_MASK;

/* Define the union U_HWTS_L2_ERROR_AIC_TRAP_INTERRUPT_MASK */
typedef union
{
    /* Define the struct bits */
    struct
    {
        /* The 2 members below is corresponding to the member "l2_aic_trap_mask    " of the struct "" */        unsigned long long    l2_aic_trap_mask_0          ; /* [51..0]  */        unsigned long long    l2_aic_trap_mask_1    : 20  ; /* [51..0]  */
        unsigned long long    reserved_0            : 12  ; /* [63..52]  */
    } bits;

    /* Define an unsigned member */
    unsigned long long    u64;

} U_HWTS_L2_ERROR_AIC_TRAP_INTERRUPT_MASK;

/* Define the union U_HWTS_L2_NORMAL_AIC_DONE_INTERRUPT_FORCE */
typedef union
{
    /* Define the struct bits */
    struct
    {
        /* The 2 members below is corresponding to the member "l2_aic_done_force   " of the struct "" */        unsigned long long    l2_aic_done_force_0         ; /* [51..0]  */        unsigned long long    l2_aic_done_force_1   : 20  ; /* [51..0]  */
        unsigned long long    reserved_0            : 12  ; /* [63..52]  */
    } bits;

    /* Define an unsigned member */
    unsigned long long    u64;

} U_HWTS_L2_NORMAL_AIC_DONE_INTERRUPT_FORCE;

/* Define the union U_HWTS_L2_NORMAL_PRE_PAUSED_INTERRUPT_FORCE */
typedef union
{
    /* Define the struct bits */
    struct
    {
        /* The 2 members below is corresponding to the member "l2_pre_paused_force " of the struct "" */        unsigned long long    l2_pre_paused_force_0       ; /* [51..0]  */        unsigned long long    l2_pre_paused_force_1 : 20  ; /* [51..0]  */
        unsigned long long    reserved_0            : 12  ; /* [63..52]  */
    } bits;

    /* Define an unsigned member */
    unsigned long long    u64;

} U_HWTS_L2_NORMAL_PRE_PAUSED_INTERRUPT_FORCE;

/* Define the union U_HWTS_L2_NORMAL_POST_PAUSED_INTERRUPT_FORCE */
typedef union
{
    /* Define the struct bits */
    struct
    {
        /* The 2 members below is corresponding to the member "l2_post_paused_force" of the struct "" */        unsigned long long    l2_post_paused_force_0      ; /* [51..0]  */        unsigned long long    l2_post_paused_force_1 : 20  ; /* [51..0]  */
        unsigned long long    reserved_0            : 12  ; /* [63..52]  */
    } bits;

    /* Define an unsigned member */
    unsigned long long    u64;

} U_HWTS_L2_NORMAL_POST_PAUSED_INTERRUPT_FORCE;

/* Define the union U_HWTS_L2_NORMAL_CQ_FULL_INTERRUPT_FORCE */
typedef union
{
    /* Define the struct bits */
    struct
    {
        /* The 2 members below is corresponding to the member "l2_cq_full_force    " of the struct "" */        unsigned long long    l2_cq_full_force_0          ; /* [51..0]  */        unsigned long long    l2_cq_full_force_1    : 20  ; /* [51..0]  */
        unsigned long long    reserved_0            : 12  ; /* [63..52]  */
    } bits;

    /* Define an unsigned member */
    unsigned long long    u64;

} U_HWTS_L2_NORMAL_CQ_FULL_INTERRUPT_FORCE;

/* Define the union U_HWTS_L2_NORMAL_TASK_PAUSED_INTERRUPT_FORCE */
typedef union
{
    /* Define the struct bits */
    struct
    {
        /* The 2 members below is corresponding to the member "l2_task_paused_force" of the struct "" */        unsigned long long    l2_task_paused_force_0      ; /* [51..0]  */        unsigned long long    l2_task_paused_force_1 : 20  ; /* [51..0]  */
        unsigned long long    reserved_0            : 12  ; /* [63..52]  */
    } bits;

    /* Define an unsigned member */
    unsigned long long    u64;

} U_HWTS_L2_NORMAL_TASK_PAUSED_INTERRUPT_FORCE;

/* Define the union U_HWTS_L2_NORMAL_EVENT_RECORD_INTERRUPT_FORCE */
typedef union
{
    /* Define the struct bits */
    struct
    {
        /* The 2 members below is corresponding to the member "l2_event_record_force" of the struct "" */        unsigned long long    l2_event_record_force_0     ; /* [63..0]  */        unsigned long long    l2_event_record_force_1     ; /* [63..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned long long    u64;

} U_HWTS_L2_NORMAL_EVENT_RECORD_INTERRUPT_FORCE;

/* Define the union U_HWTS_L2_NORMAL_EVENT_WAIT_INTERRUPT_FORCE */
typedef union
{
    /* Define the struct bits */
    struct
    {
        /* The 2 members below is corresponding to the member "l2_event_wait_force " of the struct "" */        unsigned long long    l2_event_wait_force_0       ; /* [51..0]  */        unsigned long long    l2_event_wait_force_1 : 20  ; /* [51..0]  */
        unsigned long long    reserved_0            : 12  ; /* [63..52]  */
    } bits;

    /* Define an unsigned member */
    unsigned long long    u64;

} U_HWTS_L2_NORMAL_EVENT_WAIT_INTERRUPT_FORCE;

/* Define the union U_HWTS_L2_NORMAL_SQ_TIMEOUT_INTERRUPT_FORCE */
typedef union
{
    /* Define the struct bits */
    struct
    {
        /* The 2 members below is corresponding to the member "l2_sq_timeout_force " of the struct "" */        unsigned long long    l2_sq_timeout_force_0       ; /* [51..0]  */        unsigned long long    l2_sq_timeout_force_1 : 20  ; /* [51..0]  */
        unsigned long long    reserved_0            : 12  ; /* [63..52]  */
    } bits;

    /* Define an unsigned member */
    unsigned long long    u64;

} U_HWTS_L2_NORMAL_SQ_TIMEOUT_INTERRUPT_FORCE;

/* Define the union U_HWTS_L2_DEBUG_AIC_DEBUG_INTERRUPT_FORCE */
typedef union
{
    /* Define the struct bits */
    struct
    {
        /* The 2 members below is corresponding to the member "l2_aic_debug_force  " of the struct "" */        unsigned long long    l2_aic_debug_force_0        ; /* [51..0]  */        unsigned long long    l2_aic_debug_force_1  : 20  ; /* [51..0]  */
        unsigned long long    reserved_0            : 12  ; /* [63..52]  */
    } bits;

    /* Define an unsigned member */
    unsigned long long    u64;

} U_HWTS_L2_DEBUG_AIC_DEBUG_INTERRUPT_FORCE;

/* Define the union U_HWTS_L2_DEBUG_DEBUG_PAUSED_INTERRUPT_FORCE */
typedef union
{
    /* Define the struct bits */
    struct
    {
        /* The 2 members below is corresponding to the member "l2_debug_paused_force" of the struct "" */        unsigned long long    l2_debug_paused_force_0     ; /* [51..0]  */        unsigned long long    l2_debug_paused_force_1 : 20  ; /* [51..0]  */
        unsigned long long    reserved_0            : 12  ; /* [63..52]  */
    } bits;

    /* Define an unsigned member */
    unsigned long long    u64;

} U_HWTS_L2_DEBUG_DEBUG_PAUSED_INTERRUPT_FORCE;

/* Define the union U_HWTS_L2_ERROR_AIC_ERROR_INTERRUPT_FORCE */
typedef union
{
    /* Define the struct bits */
    struct
    {
        /* The 2 members below is corresponding to the member "l2_aic_error_force  " of the struct "" */        unsigned long long    l2_aic_error_force_0        ; /* [51..0]  */        unsigned long long    l2_aic_error_force_1  : 20  ; /* [51..0]  */
        unsigned long long    reserved_0            : 12  ; /* [63..52]  */
    } bits;

    /* Define an unsigned member */
    unsigned long long    u64;

} U_HWTS_L2_ERROR_AIC_ERROR_INTERRUPT_FORCE;

/* Define the union U_HWTS_L2_ERROR_TASK_TIMEOUT_INTERRUPT_FORCE */
typedef union
{
    /* Define the struct bits */
    struct
    {
        /* The 2 members below is corresponding to the member "l2_task_timeout_force" of the struct "" */        unsigned long long    l2_task_timeout_force_0     ; /* [51..0]  */        unsigned long long    l2_task_timeout_force_1 : 20  ; /* [51..0]  */
        unsigned long long    reserved_0            : 12  ; /* [63..52]  */
    } bits;

    /* Define an unsigned member */
    unsigned long long    u64;

} U_HWTS_L2_ERROR_TASK_TIMEOUT_INTERRUPT_FORCE;

/* Define the union U_HWTS_L2_ERROR_AIC_TRAP_INTERRUPT_FORCE */
typedef union
{
    /* Define the struct bits */
    struct
    {
        /* The 2 members below is corresponding to the member "l2_aic_trap_force   " of the struct "" */        unsigned long long    l2_aic_trap_force_0         ; /* [51..0]  */        unsigned long long    l2_aic_trap_force_1   : 20  ; /* [51..0]  */
        unsigned long long    reserved_0            : 12  ; /* [63..52]  */
    } bits;

    /* Define an unsigned member */
    unsigned long long    u64;

} U_HWTS_L2_ERROR_AIC_TRAP_INTERRUPT_FORCE;

/* Define the union U_HWTS_L2_NORMAL_AIC_DONE_INTERRUPT_MASK_SET */
typedef union
{
    /* Define the struct bits */
    struct
    {
        /* The 2 members below is corresponding to the member "l2_aic_done_mask_set" of the struct "" */        unsigned long long    l2_aic_done_mask_set_0      ; /* [51..0]  */        unsigned long long    l2_aic_done_mask_set_1 : 20  ; /* [51..0]  */
        unsigned long long    reserved_0            : 12  ; /* [63..52]  */
    } bits;

    /* Define an unsigned member */
    unsigned long long    u64;

} U_HWTS_L2_NORMAL_AIC_DONE_INTERRUPT_MASK_SET;

/* Define the union U_HWTS_L2_NORMAL_PRE_PAUSED_INTERRUPT_MASK_SET */
typedef union
{
    /* Define the struct bits */
    struct
    {
        /* The 2 members below is corresponding to the member "l2_pre_paused_mask_set" of the struct "" */        unsigned long long    l2_pre_paused_mask_set_0    ; /* [51..0]  */        unsigned long long    l2_pre_paused_mask_set_1 : 20  ; /* [51..0]  */
        unsigned long long    reserved_0            : 12  ; /* [63..52]  */
    } bits;

    /* Define an unsigned member */
    unsigned long long    u64;

} U_HWTS_L2_NORMAL_PRE_PAUSED_INTERRUPT_MASK_SET;

/* Define the union U_HWTS_L2_NORMAL_POST_PAUSED_INTERRUPT_MASK_SET */
typedef union
{
    /* Define the struct bits */
    struct
    {
        /* The 2 members below is corresponding to the member "l2_post_paused_mask_set" of the struct "" */        unsigned long long    l2_post_paused_mask_set_0   ; /* [51..0]  */        unsigned long long    l2_post_paused_mask_set_1 : 20  ; /* [51..0]  */
        unsigned long long    reserved_0            : 12  ; /* [63..52]  */
    } bits;

    /* Define an unsigned member */
    unsigned long long    u64;

} U_HWTS_L2_NORMAL_POST_PAUSED_INTERRUPT_MASK_SET;

/* Define the union U_HWTS_L2_NORMAL_CQ_FULL_INTERRUPT_MASK_SET */
typedef union
{
    /* Define the struct bits */
    struct
    {
        /* The 2 members below is corresponding to the member "l2_cq_full_mask_set " of the struct "" */        unsigned long long    l2_cq_full_mask_set_0       ; /* [51..0]  */        unsigned long long    l2_cq_full_mask_set_1 : 20  ; /* [51..0]  */
        unsigned long long    reserved_0            : 12  ; /* [63..52]  */
    } bits;

    /* Define an unsigned member */
    unsigned long long    u64;

} U_HWTS_L2_NORMAL_CQ_FULL_INTERRUPT_MASK_SET;

/* Define the union U_HWTS_L2_NORMAL_TASK_PAUSED_INTERRUPT_MASK_SET */
typedef union
{
    /* Define the struct bits */
    struct
    {
        /* The 2 members below is corresponding to the member "l2_task_paused_mask_set" of the struct "" */        unsigned long long    l2_task_paused_mask_set_0   ; /* [51..0]  */        unsigned long long    l2_task_paused_mask_set_1 : 20  ; /* [51..0]  */
        unsigned long long    reserved_0            : 12  ; /* [63..52]  */
    } bits;

    /* Define an unsigned member */
    unsigned long long    u64;

} U_HWTS_L2_NORMAL_TASK_PAUSED_INTERRUPT_MASK_SET;

/* Define the union U_HWTS_L2_NORMAL_EVENT_RECORD_INTERRUPT_MASK_SET */
typedef union
{
    /* Define the struct bits */
    struct
    {
        /* The 2 members below is corresponding to the member "l2_event_record_mask_set" of the struct "" */        unsigned long long    l2_event_record_mask_set_0  ; /* [63..0]  */        unsigned long long    l2_event_record_mask_set_1  ; /* [63..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned long long    u64;

} U_HWTS_L2_NORMAL_EVENT_RECORD_INTERRUPT_MASK_SET;

/* Define the union U_HWTS_L2_NORMAL_EVENT_WAIT_INTERRUPT_MASK_SET */
typedef union
{
    /* Define the struct bits */
    struct
    {
        /* The 2 members below is corresponding to the member "l2_event_wait_mask_set" of the struct "" */        unsigned long long    l2_event_wait_mask_set_0    ; /* [51..0]  */        unsigned long long    l2_event_wait_mask_set_1 : 20  ; /* [51..0]  */
        unsigned long long    reserved_0            : 12  ; /* [63..52]  */
    } bits;

    /* Define an unsigned member */
    unsigned long long    u64;

} U_HWTS_L2_NORMAL_EVENT_WAIT_INTERRUPT_MASK_SET;

/* Define the union U_HWTS_L2_NORMAL_SQ_TIMEOUT_INTERRUPT_MASK_SET */
typedef union
{
    /* Define the struct bits */
    struct
    {
        /* The 2 members below is corresponding to the member "l2_sq_timeout_mask_set" of the struct "" */        unsigned long long    l2_sq_timeout_mask_set_0    ; /* [51..0]  */        unsigned long long    l2_sq_timeout_mask_set_1 : 20  ; /* [51..0]  */
        unsigned long long    reserved_0            : 12  ; /* [63..52]  */
    } bits;

    /* Define an unsigned member */
    unsigned long long    u64;

} U_HWTS_L2_NORMAL_SQ_TIMEOUT_INTERRUPT_MASK_SET;

/* Define the union U_HWTS_L2_DEBUG_AIC_DEBUG_INTERRUPT_MASK_SET */
typedef union
{
    /* Define the struct bits */
    struct
    {
        /* The 2 members below is corresponding to the member "l2_aic_debug_mask_set" of the struct "" */        unsigned long long    l2_aic_debug_mask_set_0     ; /* [51..0]  */        unsigned long long    l2_aic_debug_mask_set_1 : 20  ; /* [51..0]  */
        unsigned long long    reserved_0            : 12  ; /* [63..52]  */
    } bits;

    /* Define an unsigned member */
    unsigned long long    u64;

} U_HWTS_L2_DEBUG_AIC_DEBUG_INTERRUPT_MASK_SET;

/* Define the union U_HWTS_L2_DEBUG_DEBUG_PAUSED_INTERRUPT_MASK_SET */
typedef union
{
    /* Define the struct bits */
    struct
    {
        /* The 2 members below is corresponding to the member "l2_debug_paused_mask_set" of the struct "" */        unsigned long long    l2_debug_paused_mask_set_0  ; /* [51..0]  */        unsigned long long    l2_debug_paused_mask_set_1 : 20  ; /* [51..0]  */
        unsigned long long    reserved_0            : 12  ; /* [63..52]  */
    } bits;

    /* Define an unsigned member */
    unsigned long long    u64;

} U_HWTS_L2_DEBUG_DEBUG_PAUSED_INTERRUPT_MASK_SET;

/* Define the union U_HWTS_L2_ERROR_AIC_ERROR_INTERRUPT_MASK_SET */
typedef union
{
    /* Define the struct bits */
    struct
    {
        /* The 2 members below is corresponding to the member "l2_aic_error_mask_set" of the struct "" */        unsigned long long    l2_aic_error_mask_set_0     ; /* [51..0]  */        unsigned long long    l2_aic_error_mask_set_1 : 20  ; /* [51..0]  */
        unsigned long long    reserved_0            : 12  ; /* [63..52]  */
    } bits;

    /* Define an unsigned member */
    unsigned long long    u64;

} U_HWTS_L2_ERROR_AIC_ERROR_INTERRUPT_MASK_SET;

/* Define the union U_HWTS_L2_ERROR_TASK_TIMEOUT_INTERRUPT_MASK_SET */
typedef union
{
    /* Define the struct bits */
    struct
    {
        /* The 2 members below is corresponding to the member "l2_task_timeout_mask_set" of the struct "" */        unsigned long long    l2_task_timeout_mask_set_0  ; /* [51..0]  */        unsigned long long    l2_task_timeout_mask_set_1 : 20  ; /* [51..0]  */
        unsigned long long    reserved_0            : 12  ; /* [63..52]  */
    } bits;

    /* Define an unsigned member */
    unsigned long long    u64;

} U_HWTS_L2_ERROR_TASK_TIMEOUT_INTERRUPT_MASK_SET;

/* Define the union U_HWTS_L2_ERROR_AIC_TRAP_INTERRUPT_MASK_SET */
typedef union
{
    /* Define the struct bits */
    struct
    {
        /* The 2 members below is corresponding to the member "l2_aic_trap_mask_set" of the struct "" */        unsigned long long    l2_aic_trap_mask_set_0      ; /* [51..0]  */        unsigned long long    l2_aic_trap_mask_set_1 : 20  ; /* [51..0]  */
        unsigned long long    reserved_0            : 12  ; /* [63..52]  */
    } bits;

    /* Define an unsigned member */
    unsigned long long    u64;

} U_HWTS_L2_ERROR_AIC_TRAP_INTERRUPT_MASK_SET;

/* Define the union U_HWTS_L2_NORMAL_AIC_DONE_INTERRUPT_MASK_CLR */
typedef union
{
    /* Define the struct bits */
    struct
    {
        /* The 2 members below is corresponding to the member "l2_aic_done_mask_clr" of the struct "" */        unsigned long long    l2_aic_done_mask_clr_0      ; /* [51..0]  */        unsigned long long    l2_aic_done_mask_clr_1 : 20  ; /* [51..0]  */
        unsigned long long    reserved_0            : 12  ; /* [63..52]  */
    } bits;

    /* Define an unsigned member */
    unsigned long long    u64;

} U_HWTS_L2_NORMAL_AIC_DONE_INTERRUPT_MASK_CLR;

/* Define the union U_HWTS_L2_NORMAL_PRE_PAUSED_INTERRUPT_MASK_CLR */
typedef union
{
    /* Define the struct bits */
    struct
    {
        /* The 2 members below is corresponding to the member "l2_pre_paused_mask_clr" of the struct "" */        unsigned long long    l2_pre_paused_mask_clr_0    ; /* [51..0]  */        unsigned long long    l2_pre_paused_mask_clr_1 : 20  ; /* [51..0]  */
        unsigned long long    reserved_0            : 12  ; /* [63..52]  */
    } bits;

    /* Define an unsigned member */
    unsigned long long    u64;

} U_HWTS_L2_NORMAL_PRE_PAUSED_INTERRUPT_MASK_CLR;

/* Define the union U_HWTS_L2_NORMAL_POST_PAUSED_INTERRUPT_MASK_CLR */
typedef union
{
    /* Define the struct bits */
    struct
    {
        /* The 2 members below is corresponding to the member "l2_post_paused_mask_clr" of the struct "" */        unsigned long long    l2_post_paused_mask_clr_0   ; /* [51..0]  */        unsigned long long    l2_post_paused_mask_clr_1 : 20  ; /* [51..0]  */
        unsigned long long    reserved_0            : 12  ; /* [63..52]  */
    } bits;

    /* Define an unsigned member */
    unsigned long long    u64;

} U_HWTS_L2_NORMAL_POST_PAUSED_INTERRUPT_MASK_CLR;

/* Define the union U_HWTS_L2_NORMAL_CQ_FULL_INTERRUPT_MASK_CLR */
typedef union
{
    /* Define the struct bits */
    struct
    {
        /* The 2 members below is corresponding to the member "l2_cq_full_mask_clr " of the struct "" */        unsigned long long    l2_cq_full_mask_clr_0       ; /* [51..0]  */        unsigned long long    l2_cq_full_mask_clr_1 : 20  ; /* [51..0]  */
        unsigned long long    reserved_0            : 12  ; /* [63..52]  */
    } bits;

    /* Define an unsigned member */
    unsigned long long    u64;

} U_HWTS_L2_NORMAL_CQ_FULL_INTERRUPT_MASK_CLR;

/* Define the union U_HWTS_L2_NORMAL_TASK_PAUSED_INTERRUPT_MASK_CLR */
typedef union
{
    /* Define the struct bits */
    struct
    {
        /* The 2 members below is corresponding to the member "l2_task_paused_mask_clr" of the struct "" */        unsigned long long    l2_task_paused_mask_clr_0   ; /* [51..0]  */        unsigned long long    l2_task_paused_mask_clr_1 : 20  ; /* [51..0]  */
        unsigned long long    reserved_0            : 12  ; /* [63..52]  */
    } bits;

    /* Define an unsigned member */
    unsigned long long    u64;

} U_HWTS_L2_NORMAL_TASK_PAUSED_INTERRUPT_MASK_CLR;

/* Define the union U_HWTS_L2_NORMAL_EVENT_RECORD_INTERRUPT_MASK_CLR */
typedef union
{
    /* Define the struct bits */
    struct
    {
        /* The 2 members below is corresponding to the member "l2_event_record_mask_clr" of the struct "" */        unsigned long long    l2_event_record_mask_clr_0  ; /* [63..0]  */        unsigned long long    l2_event_record_mask_clr_1  ; /* [63..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned long long    u64;

} U_HWTS_L2_NORMAL_EVENT_RECORD_INTERRUPT_MASK_CLR;

/* Define the union U_HWTS_L2_NORMAL_EVENT_WAIT_INTERRUPT_MASK_CLR */
typedef union
{
    /* Define the struct bits */
    struct
    {
        /* The 2 members below is corresponding to the member "l2_event_wait_mask_clr" of the struct "" */        unsigned long long    l2_event_wait_mask_clr_0    ; /* [51..0]  */        unsigned long long    l2_event_wait_mask_clr_1 : 20  ; /* [51..0]  */
        unsigned long long    reserved_0            : 12  ; /* [63..52]  */
    } bits;

    /* Define an unsigned member */
    unsigned long long    u64;

} U_HWTS_L2_NORMAL_EVENT_WAIT_INTERRUPT_MASK_CLR;

/* Define the union U_HWTS_L2_NORMAL_SQ_TIMEOUT_INTERRUPT_MASK_CLR */
typedef union
{
    /* Define the struct bits */
    struct
    {
        /* The 2 members below is corresponding to the member "l2_sq_timeout_mask_clr" of the struct "" */        unsigned long long    l2_sq_timeout_mask_clr_0    ; /* [51..0]  */        unsigned long long    l2_sq_timeout_mask_clr_1 : 20  ; /* [51..0]  */
        unsigned long long    reserved_0            : 12  ; /* [63..52]  */
    } bits;

    /* Define an unsigned member */
    unsigned long long    u64;

} U_HWTS_L2_NORMAL_SQ_TIMEOUT_INTERRUPT_MASK_CLR;

/* Define the union U_HWTS_L2_DEBUG_AIC_DEBUG_INTERRUPT_MASK_CLR */
typedef union
{
    /* Define the struct bits */
    struct
    {
        /* The 2 members below is corresponding to the member "l2_aic_debug_mask_clr" of the struct "" */        unsigned long long    l2_aic_debug_mask_clr_0     ; /* [51..0]  */        unsigned long long    l2_aic_debug_mask_clr_1 : 20  ; /* [51..0]  */
        unsigned long long    reserved_0            : 12  ; /* [63..52]  */
    } bits;

    /* Define an unsigned member */
    unsigned long long    u64;

} U_HWTS_L2_DEBUG_AIC_DEBUG_INTERRUPT_MASK_CLR;

/* Define the union U_HWTS_L2_DEBUG_DEBUG_PAUSED_INTERRUPT_MASK_CLR */
typedef union
{
    /* Define the struct bits */
    struct
    {
        /* The 2 members below is corresponding to the member "l2_debug_paused_mask_clr" of the struct "" */        unsigned long long    l2_debug_paused_mask_clr_0  ; /* [51..0]  */        unsigned long long    l2_debug_paused_mask_clr_1 : 20  ; /* [51..0]  */
        unsigned long long    reserved_0            : 12  ; /* [63..52]  */
    } bits;

    /* Define an unsigned member */
    unsigned long long    u64;

} U_HWTS_L2_DEBUG_DEBUG_PAUSED_INTERRUPT_MASK_CLR;

/* Define the union U_HWTS_L2_ERROR_AIC_ERROR_INTERRUPT_MASK_CLR */
typedef union
{
    /* Define the struct bits */
    struct
    {
        /* The 2 members below is corresponding to the member "l2_aic_error_mask_clr" of the struct "" */        unsigned long long    l2_aic_error_mask_clr_0     ; /* [51..0]  */        unsigned long long    l2_aic_error_mask_clr_1 : 20  ; /* [51..0]  */
        unsigned long long    reserved_0            : 12  ; /* [63..52]  */
    } bits;

    /* Define an unsigned member */
    unsigned long long    u64;

} U_HWTS_L2_ERROR_AIC_ERROR_INTERRUPT_MASK_CLR;

/* Define the union U_HWTS_L2_ERROR_TASK_TIMEOUT_INTERRUPT_MASK_CLR */
typedef union
{
    /* Define the struct bits */
    struct
    {
        /* The 2 members below is corresponding to the member "l2_task_timeout_mask_clr" of the struct "" */        unsigned long long    l2_task_timeout_mask_clr_0  ; /* [51..0]  */        unsigned long long    l2_task_timeout_mask_clr_1 : 20  ; /* [51..0]  */
        unsigned long long    reserved_0            : 12  ; /* [63..52]  */
    } bits;

    /* Define an unsigned member */
    unsigned long long    u64;

} U_HWTS_L2_ERROR_TASK_TIMEOUT_INTERRUPT_MASK_CLR;

/* Define the union U_HWTS_L2_ERROR_AIC_TRAP_INTERRUPT_MASK_CLR */
typedef union
{
    /* Define the struct bits */
    struct
    {
        /* The 2 members below is corresponding to the member "l2_aic_trap_mask_clr" of the struct "" */        unsigned long long    l2_aic_trap_mask_clr_0      ; /* [51..0]  */        unsigned long long    l2_aic_trap_mask_clr_1 : 20  ; /* [51..0]  */
        unsigned long long    reserved_0            : 12  ; /* [63..52]  */
    } bits;

    /* Define an unsigned member */
    unsigned long long    u64;

} U_HWTS_L2_ERROR_AIC_TRAP_INTERRUPT_MASK_CLR;

/* Define the union U_HWTS_L2_NORMAL_AIC_DONE_INTERRUPT_PENDING */
typedef union
{
    /* Define the struct bits */
    struct
    {
        /* The 2 members below is corresponding to the member "l2_aic_done_pending " of the struct "" */        unsigned long long    l2_aic_done_pending_0       ; /* [51..0]  */        unsigned long long    l2_aic_done_pending_1 : 20  ; /* [51..0]  */
        unsigned long long    reserved_0            : 12  ; /* [63..52]  */
    } bits;

    /* Define an unsigned member */
    unsigned long long    u64;

} U_HWTS_L2_NORMAL_AIC_DONE_INTERRUPT_PENDING;

/* Define the union U_HWTS_L2_NORMAL_PRE_PAUSED_INTERRUPT_PENDING */
typedef union
{
    /* Define the struct bits */
    struct
    {
        /* The 2 members below is corresponding to the member "l2_pre_paused_pending" of the struct "" */        unsigned long long    l2_pre_paused_pending_0     ; /* [51..0]  */        unsigned long long    l2_pre_paused_pending_1 : 20  ; /* [51..0]  */
        unsigned long long    reserved_0            : 12  ; /* [63..52]  */
    } bits;

    /* Define an unsigned member */
    unsigned long long    u64;

} U_HWTS_L2_NORMAL_PRE_PAUSED_INTERRUPT_PENDING;

/* Define the union U_HWTS_L2_NORMAL_POST_PAUSED_INTERRUPT_PENDING */
typedef union
{
    /* Define the struct bits */
    struct
    {
        /* The 2 members below is corresponding to the member "l2_post_paused_pending" of the struct "" */        unsigned long long    l2_post_paused_pending_0    ; /* [51..0]  */        unsigned long long    l2_post_paused_pending_1 : 20  ; /* [51..0]  */
        unsigned long long    reserved_0            : 12  ; /* [63..52]  */
    } bits;

    /* Define an unsigned member */
    unsigned long long    u64;

} U_HWTS_L2_NORMAL_POST_PAUSED_INTERRUPT_PENDING;

/* Define the union U_HWTS_L2_NORMAL_CQ_FULL_INTERRUPT_PENDING */
typedef union
{
    /* Define the struct bits */
    struct
    {
        /* The 2 members below is corresponding to the member "l2_cq_full_pending  " of the struct "" */        unsigned long long    l2_cq_full_pending_0        ; /* [51..0]  */        unsigned long long    l2_cq_full_pending_1  : 20  ; /* [51..0]  */
        unsigned long long    reserved_0            : 12  ; /* [63..52]  */
    } bits;

    /* Define an unsigned member */
    unsigned long long    u64;

} U_HWTS_L2_NORMAL_CQ_FULL_INTERRUPT_PENDING;

/* Define the union U_HWTS_L2_NORMAL_TASK_PAUSED_INTERRUPT_PENDING */
typedef union
{
    /* Define the struct bits */
    struct
    {
        /* The 2 members below is corresponding to the member "l2_task_paused_pending" of the struct "" */        unsigned long long    l2_task_paused_pending_0    ; /* [51..0]  */        unsigned long long    l2_task_paused_pending_1 : 20  ; /* [51..0]  */
        unsigned long long    reserved_0            : 12  ; /* [63..52]  */
    } bits;

    /* Define an unsigned member */
    unsigned long long    u64;

} U_HWTS_L2_NORMAL_TASK_PAUSED_INTERRUPT_PENDING;

/* Define the union U_HWTS_L2_NORMAL_EVENT_RECORD_INTERRUPT_PENDING */
typedef union
{
    /* Define the struct bits */
    struct
    {
        /* The 2 members below is corresponding to the member "l2_event_record_pending" of the struct "" */        unsigned long long    l2_event_record_pending_0   ; /* [63..0]  */        unsigned long long    l2_event_record_pending_1   ; /* [63..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned long long    u64;

} U_HWTS_L2_NORMAL_EVENT_RECORD_INTERRUPT_PENDING;

/* Define the union U_HWTS_L2_NORMAL_EVENT_WAIT_INTERRUPT_PENDING */
typedef union
{
    /* Define the struct bits */
    struct
    {
        /* The 2 members below is corresponding to the member "l2_event_wait_pending" of the struct "" */        unsigned long long    l2_event_wait_pending_0     ; /* [51..0]  */        unsigned long long    l2_event_wait_pending_1 : 20  ; /* [51..0]  */
        unsigned long long    reserved_0            : 12  ; /* [63..52]  */
    } bits;

    /* Define an unsigned member */
    unsigned long long    u64;

} U_HWTS_L2_NORMAL_EVENT_WAIT_INTERRUPT_PENDING;

/* Define the union U_HWTS_L2_NORMAL_SQ_TIMEOUT_INTERRUPT_PENDING */
typedef union
{
    /* Define the struct bits */
    struct
    {
        /* The 2 members below is corresponding to the member "l2_sq_timeout_pending" of the struct "" */        unsigned long long    l2_sq_timeout_pending_0     ; /* [51..0]  */        unsigned long long    l2_sq_timeout_pending_1 : 20  ; /* [51..0]  */
        unsigned long long    reserved_0            : 12  ; /* [63..52]  */
    } bits;

    /* Define an unsigned member */
    unsigned long long    u64;

} U_HWTS_L2_NORMAL_SQ_TIMEOUT_INTERRUPT_PENDING;

/* Define the union U_HWTS_L2_DEBUG_AIC_DEBUG_INTERRUPT_PENDING */
typedef union
{
    /* Define the struct bits */
    struct
    {
        /* The 2 members below is corresponding to the member "l2_aic_debug_pending" of the struct "" */        unsigned long long    l2_aic_debug_pending_0      ; /* [51..0]  */        unsigned long long    l2_aic_debug_pending_1 : 20  ; /* [51..0]  */
        unsigned long long    reserved_0            : 12  ; /* [63..52]  */
    } bits;

    /* Define an unsigned member */
    unsigned long long    u64;

} U_HWTS_L2_DEBUG_AIC_DEBUG_INTERRUPT_PENDING;

/* Define the union U_HWTS_L2_DEBUG_DEBUG_PAUSED_INTERRUPT_PENDING */
typedef union
{
    /* Define the struct bits */
    struct
    {
        /* The 2 members below is corresponding to the member "l2_debug_paused_pending" of the struct "" */        unsigned long long    l2_debug_paused_pending_0   ; /* [51..0]  */        unsigned long long    l2_debug_paused_pending_1 : 20  ; /* [51..0]  */
        unsigned long long    reserved_0            : 12  ; /* [63..52]  */
    } bits;

    /* Define an unsigned member */
    unsigned long long    u64;

} U_HWTS_L2_DEBUG_DEBUG_PAUSED_INTERRUPT_PENDING;

/* Define the union U_HWTS_L2_ERROR_AIC_ERROR_INTERRUPT_PENDING */
typedef union
{
    /* Define the struct bits */
    struct
    {
        /* The 2 members below is corresponding to the member "l2_aic_error_pending" of the struct "" */        unsigned long long    l2_aic_error_pending_0      ; /* [51..0]  */        unsigned long long    l2_aic_error_pending_1 : 20  ; /* [51..0]  */
        unsigned long long    reserved_0            : 12  ; /* [63..52]  */
    } bits;

    /* Define an unsigned member */
    unsigned long long    u64;

} U_HWTS_L2_ERROR_AIC_ERROR_INTERRUPT_PENDING;

/* Define the union U_HWTS_L2_ERROR_TASK_TIMEOUT_INTERRUPT_PENDING */
typedef union
{
    /* Define the struct bits */
    struct
    {
        /* The 2 members below is corresponding to the member "l2_task_timeout_pending" of the struct "" */        unsigned long long    l2_task_timeout_pending_0   ; /* [51..0]  */        unsigned long long    l2_task_timeout_pending_1 : 20  ; /* [51..0]  */
        unsigned long long    reserved_0            : 12  ; /* [63..52]  */
    } bits;

    /* Define an unsigned member */
    unsigned long long    u64;

} U_HWTS_L2_ERROR_TASK_TIMEOUT_INTERRUPT_PENDING;

/* Define the union U_HWTS_L2_ERROR_AIC_TRAP_INTERRUPT_PENDING */
typedef union
{
    /* Define the struct bits */
    struct
    {
        /* The 2 members below is corresponding to the member "l2_aic_trap_pending " of the struct "" */        unsigned long long    l2_aic_trap_pending_0       ; /* [51..0]  */        unsigned long long    l2_aic_trap_pending_1 : 20  ; /* [51..0]  */
        unsigned long long    reserved_0            : 12  ; /* [63..52]  */
    } bits;

    /* Define an unsigned member */
    unsigned long long    u64;

} U_HWTS_L2_ERROR_AIC_TRAP_INTERRUPT_PENDING;

/* Define the union U_HWTS_L2_ERROR_SDMA_ERROR_INTERRUPT */
typedef union
{
    /* Define the struct bits */
    struct
    {
        /* The 2 members below is corresponding to the member "l2_sdma_error_status" of the struct "" */        unsigned long long    l2_sdma_error_status_0      ; /* [51..0]  */        unsigned long long    l2_sdma_error_status_1 : 20  ; /* [51..0]  */
        unsigned long long    reserved_0            : 12  ; /* [63..52]  */
    } bits;

    /* Define an unsigned member */
    unsigned long long    u64;

} U_HWTS_L2_ERROR_SDMA_ERROR_INTERRUPT;

/* Define the union U_HWTS_L2_ERROR_SDMA_TIMEOUT_INTERRUPT */
typedef union
{
    /* Define the struct bits */
    struct
    {
        /* The 2 members below is corresponding to the member "l2_sdma_timeout_status" of the struct "" */        unsigned long long    l2_sdma_timeout_status_0    ; /* [51..0]  */        unsigned long long    l2_sdma_timeout_status_1 : 20  ; /* [51..0]  */
        unsigned long long    reserved_0            : 12  ; /* [63..52]  */
    } bits;

    /* Define an unsigned member */
    unsigned long long    u64;

} U_HWTS_L2_ERROR_SDMA_TIMEOUT_INTERRUPT;

/* Define the union U_HWTS_L2_ERROR_SDMA_ERROR_MASK */
typedef union
{
    /* Define the struct bits */
    struct
    {
        /* The 2 members below is corresponding to the member "l2_sdma_error_mask  " of the struct "" */        unsigned long long    l2_sdma_error_mask_0        ; /* [51..0]  */        unsigned long long    l2_sdma_error_mask_1  : 20  ; /* [51..0]  */
        unsigned long long    reserved_0            : 12  ; /* [63..52]  */
    } bits;

    /* Define an unsigned member */
    unsigned long long    u64;

} U_HWTS_L2_ERROR_SDMA_ERROR_MASK;

/* Define the union U_HWTS_L2_ERROR_SDMA_TIMEOUT_MASK */
typedef union
{
    /* Define the struct bits */
    struct
    {
        /* The 2 members below is corresponding to the member "l2_sdma_timeout_mask" of the struct "" */        unsigned long long    l2_sdma_timeout_mask_0      ; /* [51..0]  */        unsigned long long    l2_sdma_timeout_mask_1 : 20  ; /* [51..0]  */
        unsigned long long    reserved_0            : 12  ; /* [63..52]  */
    } bits;

    /* Define an unsigned member */
    unsigned long long    u64;

} U_HWTS_L2_ERROR_SDMA_TIMEOUT_MASK;

/* Define the union U_HWTS_L2_ERROR_SDMA_ERROR_FORCE */
typedef union
{
    /* Define the struct bits */
    struct
    {
        /* The 2 members below is corresponding to the member "l2_sdma_error_force " of the struct "" */        unsigned long long    l2_sdma_error_force_0       ; /* [51..0]  */        unsigned long long    l2_sdma_error_force_1 : 20  ; /* [51..0]  */
        unsigned long long    reserved_0            : 12  ; /* [63..52]  */
    } bits;

    /* Define an unsigned member */
    unsigned long long    u64;

} U_HWTS_L2_ERROR_SDMA_ERROR_FORCE;

/* Define the union U_HWTS_L2_ERROR_SDMA_TIMEOUT_FORCE */
typedef union
{
    /* Define the struct bits */
    struct
    {
        /* The 2 members below is corresponding to the member "l2_sdma_timeout_force" of the struct "" */        unsigned long long    l2_sdma_timeout_force_0     ; /* [51..0]  */        unsigned long long    l2_sdma_timeout_force_1 : 20  ; /* [51..0]  */
        unsigned long long    reserved_0            : 12  ; /* [63..52]  */
    } bits;

    /* Define an unsigned member */
    unsigned long long    u64;

} U_HWTS_L2_ERROR_SDMA_TIMEOUT_FORCE;

/* Define the union U_HWTS_L2_ERROR_SDMA_ERROR_MASK_SET */
typedef union
{
    /* Define the struct bits */
    struct
    {
        /* The 2 members below is corresponding to the member "l2_sdma_error_mask_set" of the struct "" */        unsigned long long    l2_sdma_error_mask_set_0    ; /* [51..0]  */        unsigned long long    l2_sdma_error_mask_set_1 : 20  ; /* [51..0]  */
        unsigned long long    reserved_0            : 12  ; /* [63..52]  */
    } bits;

    /* Define an unsigned member */
    unsigned long long    u64;

} U_HWTS_L2_ERROR_SDMA_ERROR_MASK_SET;

/* Define the union U_HWTS_L2_ERROR_SDMA_TIMEOUT_MASK_SET */
typedef union
{
    /* Define the struct bits */
    struct
    {
        /* The 2 members below is corresponding to the member "l2_sdma_timeout_mask_set" of the struct "" */        unsigned long long    l2_sdma_timeout_mask_set_0  ; /* [51..0]  */        unsigned long long    l2_sdma_timeout_mask_set_1 : 20  ; /* [51..0]  */
        unsigned long long    reserved_0            : 12  ; /* [63..52]  */
    } bits;

    /* Define an unsigned member */
    unsigned long long    u64;

} U_HWTS_L2_ERROR_SDMA_TIMEOUT_MASK_SET;

/* Define the union U_HWTS_L2_ERROR_SDMA_ERROR_MASK_CLR */
typedef union
{
    /* Define the struct bits */
    struct
    {
        /* The 2 members below is corresponding to the member "l2_sdma_error_mask_clr" of the struct "" */        unsigned long long    l2_sdma_error_mask_clr_0    ; /* [51..0]  */        unsigned long long    l2_sdma_error_mask_clr_1 : 20  ; /* [51..0]  */
        unsigned long long    reserved_0            : 12  ; /* [63..52]  */
    } bits;

    /* Define an unsigned member */
    unsigned long long    u64;

} U_HWTS_L2_ERROR_SDMA_ERROR_MASK_CLR;

/* Define the union U_HWTS_L2_ERROR_SDMA_TIMEOUT_MASK_CLR */
typedef union
{
    /* Define the struct bits */
    struct
    {
        /* The 2 members below is corresponding to the member "l2_sdma_timeout_mask_clr" of the struct "" */        unsigned long long    l2_sdma_timeout_mask_clr_0  ; /* [51..0]  */        unsigned long long    l2_sdma_timeout_mask_clr_1 : 20  ; /* [51..0]  */
        unsigned long long    reserved_0            : 12  ; /* [63..52]  */
    } bits;

    /* Define an unsigned member */
    unsigned long long    u64;

} U_HWTS_L2_ERROR_SDMA_TIMEOUT_MASK_CLR;

/* Define the union U_HWTS_L2_ERROR_SDMA_ERROR_PENDING */
typedef union
{
    /* Define the struct bits */
    struct
    {
        /* The 2 members below is corresponding to the member "l2_sdma_error_pending" of the struct "" */        unsigned long long    l2_sdma_error_pending_0     ; /* [51..0]  */        unsigned long long    l2_sdma_error_pending_1 : 20  ; /* [51..0]  */
        unsigned long long    reserved_0            : 12  ; /* [63..52]  */
    } bits;

    /* Define an unsigned member */
    unsigned long long    u64;

} U_HWTS_L2_ERROR_SDMA_ERROR_PENDING;

/* Define the union U_HWTS_L2_ERROR_SDMA_TIMEOUT_PENDING */
typedef union
{
    /* Define the struct bits */
    struct
    {
        /* The 2 members below is corresponding to the member "l2_sdma_timeout_pending" of the struct "" */        unsigned long long    l2_sdma_timeout_pending_0   ; /* [51..0]  */        unsigned long long    l2_sdma_timeout_pending_1 : 20  ; /* [51..0]  */
        unsigned long long    reserved_0            : 12  ; /* [63..52]  */
    } bits;

    /* Define an unsigned member */
    unsigned long long    u64;

} U_HWTS_L2_ERROR_SDMA_TIMEOUT_PENDING;

/* Define the union U_HWTS_NOTIFY_PROFILE_GLOBAL_SETTING0 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        /* The 2 members below is corresponding to the member "profile_bin0_upper  " of the struct "" */        unsigned long long    profile_bin0_upper_0        ; /* [33..0]  */        unsigned long long    profile_bin0_upper_1  : 2   ; /* [33..0]  */
        unsigned long long    reserved_0            : 30  ; /* [63..34]  */
    } bits;

    /* Define an unsigned member */
    unsigned long long    u64;

} U_HWTS_NOTIFY_PROFILE_GLOBAL_SETTING0;

/* Define the union U_HWTS_NOTIFY_PROFILE_GLOBAL_SETTING1 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        /* The 2 members below is corresponding to the member "profile_bin1_upper  " of the struct "" */        unsigned long long    profile_bin1_upper_0        ; /* [33..0]  */        unsigned long long    profile_bin1_upper_1  : 2   ; /* [33..0]  */
        unsigned long long    reserved_0            : 30  ; /* [63..34]  */
    } bits;

    /* Define an unsigned member */
    unsigned long long    u64;

} U_HWTS_NOTIFY_PROFILE_GLOBAL_SETTING1;

/* Define the union U_HWTS_NOTIFY_PROFILER_0_SEL */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned long long    notify_profiler0_id   : 10  ; /* [9..0]  */
        unsigned long long    reserved_0            : 5   ; /* [14..10]  */
        unsigned long long    notify_profiler0_en   : 1   ; /* [15]  */
        unsigned long long    notify_profiler1_id   : 10  ; /* [25..16]  */
        unsigned long long    reserved_1            : 5   ; /* [30..26]  */
        unsigned long long    notify_profiler1_en   : 1   ; /* [31]  */
        unsigned long long    notify_profiler2_id   : 10  ; /* [41..32]  */
        unsigned long long    reserved_2            : 5   ; /* [46..42]  */
        unsigned long long    notify_profiler2_en   : 1   ; /* [47]  */
        unsigned long long    notify_profiler3_id   : 10  ; /* [57..48]  */
        unsigned long long    reserved_3            : 5   ; /* [62..58]  */
        unsigned long long    notify_profiler3_en   : 1   ; /* [63]  */
    } bits;

    /* Define an unsigned member */
    unsigned long long    u64;

} U_HWTS_NOTIFY_PROFILER_0_SEL;

/* Define the union U_HWTS_NOTIFY_PROFILER_1_SEL */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned long long    notify_profiler4_id   : 10  ; /* [9..0]  */
        unsigned long long    reserved_0            : 5   ; /* [14..10]  */
        unsigned long long    notify_profiler4_en   : 1   ; /* [15]  */
        unsigned long long    notify_profiler5_id   : 10  ; /* [25..16]  */
        unsigned long long    reserved_1            : 5   ; /* [30..26]  */
        unsigned long long    notify_profiler5_en   : 1   ; /* [31]  */
        unsigned long long    notify_profiler6_id   : 10  ; /* [41..32]  */
        unsigned long long    reserved_2            : 5   ; /* [46..42]  */
        unsigned long long    notify_profiler6_en   : 1   ; /* [47]  */
        unsigned long long    notify_profiler7_id   : 10  ; /* [57..48]  */
        unsigned long long    reserved_3            : 5   ; /* [62..58]  */
        unsigned long long    notify_profiler7_en   : 1   ; /* [63]  */
    } bits;

    /* Define an unsigned member */
    unsigned long long    u64;

} U_HWTS_NOTIFY_PROFILER_1_SEL;

/* Define the union U_HWTS_NOTIFY_PROFILER_2_SEL */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned long long    notify_profiler8_id   : 10  ; /* [9..0]  */
        unsigned long long    reserved_0            : 5   ; /* [14..10]  */
        unsigned long long    notify_profiler8_en   : 1   ; /* [15]  */
        unsigned long long    notify_profiler9_id   : 10  ; /* [25..16]  */
        unsigned long long    reserved_1            : 5   ; /* [30..26]  */
        unsigned long long    notify_profiler9_en   : 1   ; /* [31]  */
        unsigned long long    notify_profiler10_id  : 10  ; /* [41..32]  */
        unsigned long long    reserved_2            : 5   ; /* [46..42]  */
        unsigned long long    notify_profiler10_en  : 1   ; /* [47]  */
        unsigned long long    notify_profiler11_id  : 10  ; /* [57..48]  */
        unsigned long long    reserved_3            : 5   ; /* [62..58]  */
        unsigned long long    notify_profiler11_en  : 1   ; /* [63]  */
    } bits;

    /* Define an unsigned member */
    unsigned long long    u64;

} U_HWTS_NOTIFY_PROFILER_2_SEL;

/* Define the union U_HWTS_NOTIFY_PROFILER_3_SEL */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned long long    notify_profiler12_id  : 10  ; /* [9..0]  */
        unsigned long long    reserved_0            : 5   ; /* [14..10]  */
        unsigned long long    notify_profiler12_en  : 1   ; /* [15]  */
        unsigned long long    notify_profiler13_id  : 10  ; /* [25..16]  */
        unsigned long long    reserved_1            : 5   ; /* [30..26]  */
        unsigned long long    notify_profiler13_en  : 1   ; /* [31]  */
        unsigned long long    notify_profiler14_id  : 10  ; /* [41..32]  */
        unsigned long long    reserved_2            : 5   ; /* [46..42]  */
        unsigned long long    notify_profiler14_en  : 1   ; /* [47]  */
        unsigned long long    notify_profiler15_id  : 10  ; /* [57..48]  */
        unsigned long long    reserved_3            : 5   ; /* [62..58]  */
        unsigned long long    notify_profiler15_en  : 1   ; /* [63]  */
    } bits;

    /* Define an unsigned member */
    unsigned long long    u64;

} U_HWTS_NOTIFY_PROFILER_3_SEL;

/* Define the union U_HWTS_NOTIFY_PROFILER_4_SEL */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned long long    notify_profiler16_id  : 10  ; /* [9..0]  */
        unsigned long long    reserved_0            : 5   ; /* [14..10]  */
        unsigned long long    notify_profiler16_en  : 1   ; /* [15]  */
        unsigned long long    notify_profiler17_id  : 10  ; /* [25..16]  */
        unsigned long long    reserved_1            : 5   ; /* [30..26]  */
        unsigned long long    notify_profiler17_en  : 1   ; /* [31]  */
        unsigned long long    notify_profiler18_id  : 10  ; /* [41..32]  */
        unsigned long long    reserved_2            : 5   ; /* [46..42]  */
        unsigned long long    notify_profiler18_en  : 1   ; /* [47]  */
        unsigned long long    notify_profiler19_id  : 10  ; /* [57..48]  */
        unsigned long long    reserved_3            : 5   ; /* [62..58]  */
        unsigned long long    notify_profiler19_en  : 1   ; /* [63]  */
    } bits;

    /* Define an unsigned member */
    unsigned long long    u64;

} U_HWTS_NOTIFY_PROFILER_4_SEL;

/* Define the union U_HWTS_NOTIFY_PROFILER_5_SEL */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned long long    notify_profiler20_id  : 10  ; /* [9..0]  */
        unsigned long long    reserved_0            : 5   ; /* [14..10]  */
        unsigned long long    notify_profiler20_en  : 1   ; /* [15]  */
        unsigned long long    notify_profiler21_id  : 10  ; /* [25..16]  */
        unsigned long long    reserved_1            : 5   ; /* [30..26]  */
        unsigned long long    notify_profiler21_en  : 1   ; /* [31]  */
        unsigned long long    notify_profiler22_id  : 10  ; /* [41..32]  */
        unsigned long long    reserved_2            : 5   ; /* [46..42]  */
        unsigned long long    notify_profiler22_en  : 1   ; /* [47]  */
        unsigned long long    notify_profiler23_id  : 10  ; /* [57..48]  */
        unsigned long long    reserved_3            : 5   ; /* [62..58]  */
        unsigned long long    notify_profiler23_en  : 1   ; /* [63]  */
    } bits;

    /* Define an unsigned member */
    unsigned long long    u64;

} U_HWTS_NOTIFY_PROFILER_5_SEL;

/* Define the union U_HWTS_NOTIFY_PROFILER_CNT */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned long long    notify_profiler_bin0_cnt : 21  ; /* [20..0]  */
        /* The 2 members below is corresponding to the member "notify_profiler_bin1_cnt" of the struct "" */        unsigned long long    notify_profiler_bin1_cnt_0 : 11  ; /* [41..21]  */        unsigned long long    notify_profiler_bin1_cnt_1 : 10  ; /* [41..21]  */
        unsigned long long    notify_profiler_bin2_cnt : 21  ; /* [62..42]  */
        unsigned long long    reserved_0            : 1   ; /* [63]  */
    } bits;

    /* Define an unsigned member */
    unsigned long long    u64;

} U_HWTS_NOTIFY_PROFILER_CNT;

/* Define the union U_HWTS_SQ_BASE_ADDR */
typedef union
{
    /* Define the struct bits */
    struct
    {
        /* The 2 members below is corresponding to the member "sq_base_addr        " of the struct "" */        unsigned long long    sq_base_addr_0              ; /* [47..0]  */        unsigned long long    sq_base_addr_1        : 16  ; /* [47..0]  */
        unsigned long long    reserved_0            : 16  ; /* [63..48]  */
    } bits;

    /* Define an unsigned member */
    unsigned long long    u64;

} U_HWTS_SQ_BASE_ADDR;

/* Define the union U_HWTS_SQ_DB */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned long long    sq_tail               : 13  ; /* [12..0]  */
        /* The 2 members below is corresponding to the member "reserved_0          " of the struct "" */        unsigned long long    reserved_0_0          : 19  ; /* [63..13]  */        unsigned long long    reserved_0_1                ; /* [63..13]  */
    } bits;

    /* Define an unsigned member */
    unsigned long long    u64;

} U_HWTS_SQ_DB;

/* Define the union U_HWTS_SQ_STATUS */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned long long    sq_head               : 13  ; /* [12..0]  */
        unsigned long long    reserved_0            : 3   ; /* [15..13]  */
        unsigned long long    sq_length             : 14  ; /* [29..16]  */
        unsigned long long    reserved_1            : 2   ; /* [31..30]  */
        unsigned long long    sq_en                 : 1   ; /* [32]  */
        unsigned long long    sq_prefetch_busy      : 1   ; /* [33]  */
        unsigned long long    reserved_2            : 14  ; /* [47..34]  */
        unsigned long long    sq_profile_en         : 1   ; /* [48]  */
        unsigned long long    sq_pct_en             : 1   ; /* [49]  */
        unsigned long long    reserved_3            : 10  ; /* [59..50]  */
        unsigned long long    sq_priority           : 3   ; /* [62..60]  */
        unsigned long long    reserved_4            : 1   ; /* [63]  */
    } bits;

    /* Define an unsigned member */
    unsigned long long    u64;

} U_HWTS_SQ_STATUS;

/* Define the union U_HWTS_TASK_CTRL */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned long long    debug_pause           : 1   ; /* [0]  */
        unsigned long long    task_pause            : 1   ; /* [1]  */
        unsigned long long    task_kill             : 1   ; /* [2]  */
        unsigned long long    reserved_0            : 13  ; /* [15..3]  */
        unsigned long long    debug_resume          : 1   ; /* [16]  */
        unsigned long long    task_resume           : 1   ; /* [17]  */
        unsigned long long    exception_handled     : 1   ; /* [18]  */
        unsigned long long    task_terminate        : 1   ; /* [19]  */
        unsigned long long    reserved_1            : 12  ; /* [31..20]  */
        unsigned long long    reserved_2            : 1   ; /* [32]  */
        unsigned long long    prefetch_clear        : 1   ; /* [33]  */
        unsigned long long    reserved_3            : 30  ; /* [63..34]  */
    } bits;

    /* Define an unsigned member */
    unsigned long long    u64;

} U_HWTS_TASK_CTRL;

/* Define the union U_HWTS_SQ_TIMEOUT_THRESHOLD */
typedef union
{
    /* Define the struct bits */
    struct
    {
        /* The 2 members below is corresponding to the member "sq_timeout_threshold" of the struct "" */        unsigned long long    sq_timeout_threshold_0      ; /* [39..0]  */        unsigned long long    sq_timeout_threshold_1 : 8   ; /* [39..0]  */
        unsigned long long    reserved_0            : 24  ; /* [63..40]  */
    } bits;

    /* Define an unsigned member */
    unsigned long long    u64;

} U_HWTS_SQ_TIMEOUT_THRESHOLD;

/* Define the union U_HWTS_SQ_RUNTIME */
typedef union
{
    /* Define the struct bits */
    struct
    {
        /* The 2 members below is corresponding to the member "sq_runtime          " of the struct "" */        unsigned long long    sq_runtime_0                ; /* [39..0]  */        unsigned long long    sq_runtime_1          : 8   ; /* [39..0]  */
        unsigned long long    reserved_0            : 24  ; /* [63..40]  */
    } bits;

    /* Define an unsigned member */
    unsigned long long    u64;

} U_HWTS_SQ_RUNTIME;

/* Define the union U_HWTS_AXPROT_SETTING */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned long long    hwts_awprot_aic       : 3   ; /* [2..0]  */
        unsigned long long    reserved_0            : 5   ; /* [7..3]  */
        unsigned long long    hwts_awprot_cq        : 3   ; /* [10..8]  */
        unsigned long long    reserved_1            : 5   ; /* [15..11]  */
        unsigned long long    hwts_awprot_log       : 3   ; /* [18..16]  */
        unsigned long long    reserved_2            : 5   ; /* [23..19]  */
        unsigned long long    hwts_awprot_pct       : 3   ; /* [26..24]  */
        unsigned long long    reserved_3            : 5   ; /* [31..27]  */
        unsigned long long    hwts_awprot_prof      : 3   ; /* [34..32]  */
        unsigned long long    reserved_4            : 5   ; /* [39..35]  */
        unsigned long long    hwts_arprot_aic       : 3   ; /* [42..40]  */
        unsigned long long    reserved_5            : 5   ; /* [47..43]  */
        unsigned long long    hwts_arprot_sq        : 3   ; /* [50..48]  */
        unsigned long long    reserved_6            : 1   ; /* [51]  */
        unsigned long long    hwts_awprot_sdma_sq   : 3   ; /* [54..52]  */
        unsigned long long    reserved_7            : 1   ; /* [55]  */
        unsigned long long    hwts_awprot_sdma_sqcq_db : 3   ; /* [58..56]  */
        unsigned long long    reserved_8            : 1   ; /* [59]  */
        unsigned long long    hwts_awprot_write_value : 1   ; /* [60]  */
        unsigned long long    reserved_9            : 3   ; /* [63..61]  */
    } bits;

    /* Define an unsigned member */
    unsigned long long    u64;

} U_HWTS_AXPROT_SETTING;

/* Define the union U_HWTS_RT_SQID */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned long long    hwts_rt_streamid      : 16  ; /* [15..0]  */
        /* The 2 members below is corresponding to the member "reserved_0          " of the struct "" */        unsigned long long    reserved_0_0          : 16  ; /* [63..16]  */        unsigned long long    reserved_0_1                ; /* [63..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned long long    u64;

} U_HWTS_RT_SQID;

/* Define the union U_HWTS_CQ_BASE_ADDR */
typedef union
{
    /* Define the struct bits */
    struct
    {
        /* The 2 members below is corresponding to the member "cq_base_addr        " of the struct "" */        unsigned long long    cq_base_addr_0              ; /* [47..0]  */        unsigned long long    cq_base_addr_1        : 16  ; /* [47..0]  */
        unsigned long long    reserved_0            : 16  ; /* [63..48]  */
    } bits;

    /* Define an unsigned member */
    unsigned long long    u64;

} U_HWTS_CQ_BASE_ADDR;

/* Define the union U_HWTS_CQ_DB */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned long long    cq_head               : 12  ; /* [11..0]  */
        /* The 2 members below is corresponding to the member "reserved_0          " of the struct "" */        unsigned long long    reserved_0_0          : 20  ; /* [63..12]  */        unsigned long long    reserved_0_1                ; /* [63..12]  */
    } bits;

    /* Define an unsigned member */
    unsigned long long    u64;

} U_HWTS_CQ_DB;

/* Define the union U_HWTS_CQ_STATUS */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned long long    cq_tail               : 12  ; /* [11..0]  */
        unsigned long long    cq_phase_bit          : 1   ; /* [12]  */
        unsigned long long    reserved_0            : 3   ; /* [15..13]  */
        unsigned long long    cq_length             : 13  ; /* [28..16]  */
        /* The 2 members below is corresponding to the member "reserved_1          " of the struct "" */        unsigned long long    reserved_1_0          : 3   ; /* [63..29]  */        unsigned long long    reserved_1_1                ; /* [63..29]  */
    } bits;

    /* Define an unsigned member */
    unsigned long long    u64;

} U_HWTS_CQ_STATUS;

/* Define the union U_SQCQ_FSM_AIC_OWN_STATE */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned long long    aic_own_bitmap              ; /* [31..0]  */
        unsigned long long    reserved_0                  ; /* [63..32]  */
    } bits;

    /* Define an unsigned member */
    unsigned long long    u64;

} U_SQCQ_FSM_AIC_OWN_STATE;

/* Define the union U_SQCQ_FSM_AIC_DEBUG_STATE */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned long long    aic_debug_bitmap            ; /* [31..0]  */
        unsigned long long    reserved_0                  ; /* [63..32]  */
    } bits;

    /* Define an unsigned member */
    unsigned long long    u64;

} U_SQCQ_FSM_AIC_DEBUG_STATE;

/* Define the union U_SQCQ_FSM_AIC_EXCEPTION_STATE */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned long long    aic_exception_bitmap        ; /* [31..0]  */
        unsigned long long    reserved_0                  ; /* [63..32]  */
    } bits;

    /* Define an unsigned member */
    unsigned long long    u64;

} U_SQCQ_FSM_AIC_EXCEPTION_STATE;

/* Define the union U_SQCQ_FSM_AIC_TRAP_STATE */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned long long    aic_trap_bitmap             ; /* [31..0]  */
        unsigned long long    reserved_0                  ; /* [63..32]  */
    } bits;

    /* Define an unsigned member */
    unsigned long long    u64;

} U_SQCQ_FSM_AIC_TRAP_STATE;

/* Define the union U_SQCQ_FSM_AIC_DONE_STATE */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned long long    aic_done_bitmap             ; /* [31..0]  */
        unsigned long long    reserved_0                  ; /* [63..32]  */
    } bits;

    /* Define an unsigned member */
    unsigned long long    u64;

} U_SQCQ_FSM_AIC_DONE_STATE;

/* Define the union U_SQCQ_FSM_MISC_STATE */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned long long    generated_token       : 15  ; /* [14..0]  */
        unsigned long long    reserved_0            : 1   ; /* [15]  */
        unsigned long long    blk_id                : 16  ; /* [31..16]  */
        unsigned long long    blk_dim               : 16  ; /* [47..32]  */
        unsigned long long    num_aicore_own_cnt    : 6   ; /* [53..48]  */
        unsigned long long    reserved_1            : 2   ; /* [55..54]  */
        unsigned long long    kernel_credit         : 8   ; /* [63..56]  */
    } bits;

    /* Define an unsigned member */
    unsigned long long    u64;

} U_SQCQ_FSM_MISC_STATE;

/* Define the union U_SQCQ_FSM_STATE */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned long long    dfx_sqcq_fsm_state    : 5   ; /* [4..0]  */
        unsigned long long    reserved_0            : 3   ; /* [7..5]  */
        unsigned long long    dfx_sqcq_fsm_task_log_ost_cnt : 2   ; /* [9..8]  */
        unsigned long long    dfx_sqcq_fsm_profile_ost_cnt : 1   ; /* [10]  */
        unsigned long long    dfx_sqcq_fsm_write_value_ost_cnt : 1   ; /* [11]  */
        unsigned long long    dfx_sqcq_fsm_cq_write_ost_cnt : 1   ; /* [12]  */
        /* The 2 members below is corresponding to the member "reserved_1          " of the struct "" */        unsigned long long    reserved_1_0          : 19  ; /* [63..13]  */        unsigned long long    reserved_1_1                ; /* [63..13]  */
    } bits;

    /* Define an unsigned member */
    unsigned long long    u64;

} U_SQCQ_FSM_STATE;

/* Define the union U_SQCQ_FSM_SDMA_OWN_STATE */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned long long    owned_sdma_ch_id      : 4   ; /* [3..0]  */
        /* The 2 members below is corresponding to the member "reserved_0          " of the struct "" */        unsigned long long    reserved_0_0          : 28  ; /* [63..4]  */        unsigned long long    reserved_0_1                ; /* [63..4]  */
    } bits;

    /* Define an unsigned member */
    unsigned long long    u64;

} U_SQCQ_FSM_SDMA_OWN_STATE;

/* Define the union U_HWTS_EVENT_TABLE */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned long long    event_table_flag      : 1   ; /* [0]  */
        unsigned long long    event_table_pending   : 1   ; /* [1]  */
        /* The 2 members below is corresponding to the member "reserved_0          " of the struct "" */        unsigned long long    reserved_0_0          : 30  ; /* [63..2]  */        unsigned long long    reserved_0_1                ; /* [63..2]  */
    } bits;

    /* Define an unsigned member */
    unsigned long long    u64;

} U_HWTS_EVENT_TABLE;

/* Define the union U_HWTS_CROSS_SOC_EVENT_TABLE */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned long long    cross_soc_event_table_flag : 1   ; /* [0]  */
        unsigned long long    cross_soc_event_table_pending : 1   ; /* [1]  */
        /* The 2 members below is corresponding to the member "reserved_0          " of the struct "" */        unsigned long long    reserved_0_0          : 30  ; /* [63..2]  */        unsigned long long    reserved_0_1                ; /* [63..2]  */
    } bits;

    /* Define an unsigned member */
    unsigned long long    u64;

} U_HWTS_CROSS_SOC_EVENT_TABLE;

/* Define the union U_DFX_PROFILE_CTRL */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned long long    profile_en            : 1   ; /* [0]  */
        unsigned long long    reserved_0            : 15  ; /* [15..1]  */
        unsigned long long    profile_buf_length    : 16  ; /* [31..16]  */
        unsigned long long    profile_af_threshold  : 16  ; /* [47..32]  */
        unsigned long long    reserved_1            : 16  ; /* [63..48]  */
    } bits;

    /* Define an unsigned member */
    unsigned long long    u64;

} U_DFX_PROFILE_CTRL;

/* Define the union U_DFX_PROFILE_BASE_ADDR */
typedef union
{
    /* Define the struct bits */
    struct
    {
        /* The 2 members below is corresponding to the member "profile_base_addr   " of the struct "" */        unsigned long long    profile_base_addr_0         ; /* [47..0]  */        unsigned long long    profile_base_addr_1   : 16  ; /* [47..0]  */
        unsigned long long    reserved_0            : 16  ; /* [63..48]  */
    } bits;

    /* Define an unsigned member */
    unsigned long long    u64;

} U_DFX_PROFILE_BASE_ADDR;

/* Define the union U_DFX_PROFILE_PTR */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned long long    profile_wptr          : 16  ; /* [15..0]  */
        unsigned long long    profile_rptr          : 16  ; /* [31..16]  */
        unsigned long long    reserved_0                  ; /* [63..32]  */
    } bits;

    /* Define an unsigned member */
    unsigned long long    u64;

} U_DFX_PROFILE_PTR;

/* Define the union U_DFX_PCT_CTRL */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned long long    pct_en                : 1   ; /* [0]  */
        unsigned long long    reserved_0            : 15  ; /* [15..1]  */
        unsigned long long    pct_buf_length        : 16  ; /* [31..16]  */
        unsigned long long    pct_af_threshold      : 16  ; /* [47..32]  */
        unsigned long long    reserved_1            : 16  ; /* [63..48]  */
    } bits;

    /* Define an unsigned member */
    unsigned long long    u64;

} U_DFX_PCT_CTRL;

/* Define the union U_DFX_PCT_BASE_ADDR */
typedef union
{
    /* Define the struct bits */
    struct
    {
        /* The 2 members below is corresponding to the member "pct_base_addr       " of the struct "" */        unsigned long long    pct_base_addr_0             ; /* [47..0]  */        unsigned long long    pct_base_addr_1       : 16  ; /* [47..0]  */
        unsigned long long    reserved_0            : 16  ; /* [63..48]  */
    } bits;

    /* Define an unsigned member */
    unsigned long long    u64;

} U_DFX_PCT_BASE_ADDR;

/* Define the union U_DFX_PCT_PTR */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned long long    pct_wptr              : 16  ; /* [15..0]  */
        unsigned long long    pct_rptr              : 16  ; /* [31..16]  */
        unsigned long long    reserved_0                  ; /* [63..32]  */
    } bits;

    /* Define an unsigned member */
    unsigned long long    u64;

} U_DFX_PCT_PTR;

/* Define the union U_DFX_LOG_CTRL */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned long long    task_log_en           : 1   ; /* [0]  */
        unsigned long long    block_log_en          : 1   ; /* [1]  */
        unsigned long long    pmu_log_en            : 1   ; /* [2]  */
        unsigned long long    reserved_0            : 13  ; /* [15..3]  */
        unsigned long long    log_buf_length        : 16  ; /* [31..16]  */
        unsigned long long    log_af_threshold      : 16  ; /* [47..32]  */
        unsigned long long    reserved_1            : 16  ; /* [63..48]  */
    } bits;

    /* Define an unsigned member */
    unsigned long long    u64;

} U_DFX_LOG_CTRL;

/* Define the union U_DFX_LOG_BASE_ADDR */
typedef union
{
    /* Define the struct bits */
    struct
    {
        /* The 2 members below is corresponding to the member "log_base_addr       " of the struct "" */        unsigned long long    log_base_addr_0             ; /* [47..0]  */        unsigned long long    log_base_addr_1       : 16  ; /* [47..0]  */
        unsigned long long    reserved_0            : 16  ; /* [63..48]  */
    } bits;

    /* Define an unsigned member */
    unsigned long long    u64;

} U_DFX_LOG_BASE_ADDR;

/* Define the union U_DFX_LOG_PTR */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned long long    log_wptr              : 16  ; /* [15..0]  */
        unsigned long long    log_rptr              : 16  ; /* [31..16]  */
        unsigned long long    reserved_0                  ; /* [63..32]  */
    } bits;

    /* Define an unsigned member */
    unsigned long long    u64;

} U_DFX_LOG_PTR;

/* Define the union U_SDMA_GLB_SETTING1 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        /* The 2 members below is corresponding to the member "sdma_cfg_addr_base  " of the struct "" */        unsigned long long    sdma_cfg_addr_base_0        ; /* [47..0]  */        unsigned long long    sdma_cfg_addr_base_1  : 16  ; /* [47..0]  */
        unsigned long long    reserved_0            : 8   ; /* [55..48]  */
        unsigned long long    sdma_ch_addr_shift    : 5   ; /* [60..56]  */
        unsigned long long    reserved_1            : 3   ; /* [63..61]  */
    } bits;

    /* Define an unsigned member */
    unsigned long long    u64;

} U_SDMA_GLB_SETTING1;

/* Define the union U_SDMA_GLB_SETTING2 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned long long    sdma_sq_tdb_offset    : 16  ; /* [15..0]  */
        unsigned long long    sdma_sq_len           : 16  ; /* [31..16]  */
        unsigned long long    sdma_cq_hdb_offset    : 16  ; /* [47..32]  */
        unsigned long long    sdma_cq_len           : 9   ; /* [56..48]  */
        unsigned long long    reserved_0            : 7   ; /* [63..57]  */
    } bits;

    /* Define an unsigned member */
    unsigned long long    u64;

} U_SDMA_GLB_SETTING2;

/* Define the union U_SDMA_SQCQ_DB */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned long long    sdma_sq_hdb           : 16  ; /* [15..0]  */
        unsigned long long    sdma_sq_tdb           : 16  ; /* [31..16]  */
        unsigned long long    sdma_cq_hdb           : 9   ; /* [40..32]  */
        unsigned long long    reserved_0            : 23  ; /* [63..41]  */
    } bits;

    /* Define an unsigned member */
    unsigned long long    u64;

} U_SDMA_SQCQ_DB;

/* Define the union U_SDMA_CQE_STATUS */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned long long    sdma_cqe_status       : 16  ; /* [15..0]  */
        /* The 2 members below is corresponding to the member "reserved_0          " of the struct "" */        unsigned long long    reserved_0_0          : 16  ; /* [63..16]  */        unsigned long long    reserved_0_1                ; /* [63..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned long long    u64;

} U_SDMA_CQE_STATUS;

/* Define the union U_DFX_SDMA_FSM_STATE */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned long long    dfx_sdma_fsm_state    : 4   ; /* [3..0]  */
        unsigned long long    reserved_0            : 4   ; /* [7..4]  */
        unsigned long long    dfx_sdma_fsm_ost_cnt  : 1   ; /* [8]  */
        /* The 2 members below is corresponding to the member "reserved_1          " of the struct "" */        unsigned long long    reserved_1_0          : 23  ; /* [63..9]  */        unsigned long long    reserved_1_1                ; /* [63..9]  */
    } bits;

    /* Define an unsigned member */
    unsigned long long    u64;

} U_DFX_SDMA_FSM_STATE;

/* Define the union U_HWTS_NOTIFY_TABLE */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned long long    notify_table_flag     : 1   ; /* [0]  */
        /* The 2 members below is corresponding to the member "reserved_0          " of the struct "" */        unsigned long long    reserved_0_0          : 31  ; /* [63..1]  */        unsigned long long    reserved_0_1                ; /* [63..1]  */
    } bits;

    /* Define an unsigned member */
    unsigned long long    u64;

} U_HWTS_NOTIFY_TABLE;

//==============================================================================
/* Define the global struct */
typedef struct
{
    U_HWTS_GLB_CTRL0                HWTS_GLB_CTRL0                   ; /* 0x0 */
    U_HWTS_BYPASS_CODE              HWTS_BYPASS_CODE                 ; /* 0x8 */
    U_HWTS_PRIORITY_CTRL0           HWTS_PRIORITY_CTRL0              ; /* 0x10 */
    U_HWTS_PRIORITY_CTRL1           HWTS_PRIORITY_CTRL1              ; /* 0x18 */
    U_HWTS_HL_PRIORITY_OVERRIDE_TIME_SETTING0   HWTS_HL_PRIORITY_OVERRIDE_TIME_SETTING0 ; /* 0x20 */
    U_HWTS_HL_PRIORITY_OVERRIDE_TIME_SETTING1   HWTS_HL_PRIORITY_OVERRIDE_TIME_SETTING1 ; /* 0x28 */
    U_HWTS_HL_PRIORITY_OVERRIDE_TASK_SETTING0   HWTS_HL_PRIORITY_OVERRIDE_TASK_SETTING0 ; /* 0x30 */
    U_HWTS_HL_PRIORITY_OVERRIDE_TASK_SETTING1   HWTS_HL_PRIORITY_OVERRIDE_TASK_SETTING1 ; /* 0x38 */
    U_HWTS_AICORE_ENABLE_CTRL       HWTS_AICORE_ENABLE_CTRL          ; /* 0x40 */
    U_HWTS_AICORE_DISABLE_CTRL      HWTS_AICORE_DISABLE_CTRL         ; /* 0x48 */
    U_HWTS_AICORE_STATUS            HWTS_AICORE_STATUS               ; /* 0x50 */
    U_HWTS_AXCACHE_SETTING          HWTS_AXCACHE_SETTING             ; /* 0x58 */
    U_HWTS_AWUSER_L_SETTING         HWTS_AWUSER_L_SETTING            ; /* 0x60 */
    U_HWTS_AWUSER_H_SETTING         HWTS_AWUSER_H_SETTING            ; /* 0x68 */
    U_HWTS_ARUSER_L_SETTING         HWTS_ARUSER_L_SETTING            ; /* 0x70 */
    U_HWTS_ARUSER_H_SETTING         HWTS_ARUSER_H_SETTING            ; /* 0x78 */
    U_HWTS_SEC_EN                   HWTS_SEC_EN                      ; /* 0x80 */
    U_HWTS_NS_SETTING               HWTS_NS_SETTING                  ; /* 0x88 */
    U_HWTS_SDMA_CH_EN_CTRL          HWTS_SDMA_CH_EN_CTRL             ; /* 0x90 */
    unsigned long long                    reserved_0[13]                   ; /* 0x98~0xf8 */
    U_HWTS_AIC00_BASE_ADDR          HWTS_AIC00_BASE_ADDR             ; /* 0x100 */
    U_HWTS_AIC01_BASE_ADDR          HWTS_AIC01_BASE_ADDR             ; /* 0x108 */
    U_HWTS_AIC02_BASE_ADDR          HWTS_AIC02_BASE_ADDR             ; /* 0x110 */
    U_HWTS_AIC03_BASE_ADDR          HWTS_AIC03_BASE_ADDR             ; /* 0x118 */
    U_HWTS_AIC04_BASE_ADDR          HWTS_AIC04_BASE_ADDR             ; /* 0x120 */
    U_HWTS_AIC05_BASE_ADDR          HWTS_AIC05_BASE_ADDR             ; /* 0x128 */
    U_HWTS_AIC06_BASE_ADDR          HWTS_AIC06_BASE_ADDR             ; /* 0x130 */
    U_HWTS_AIC07_BASE_ADDR          HWTS_AIC07_BASE_ADDR             ; /* 0x138 */
    U_HWTS_AIC08_BASE_ADDR          HWTS_AIC08_BASE_ADDR             ; /* 0x140 */
    U_HWTS_AIC09_BASE_ADDR          HWTS_AIC09_BASE_ADDR             ; /* 0x148 */
    U_HWTS_AIC10_BASE_ADDR          HWTS_AIC10_BASE_ADDR             ; /* 0x150 */
    U_HWTS_AIC11_BASE_ADDR          HWTS_AIC11_BASE_ADDR             ; /* 0x158 */
    U_HWTS_AIC12_BASE_ADDR          HWTS_AIC12_BASE_ADDR             ; /* 0x160 */
    U_HWTS_AIC13_BASE_ADDR          HWTS_AIC13_BASE_ADDR             ; /* 0x168 */
    U_HWTS_AIC14_BASE_ADDR          HWTS_AIC14_BASE_ADDR             ; /* 0x170 */
    U_HWTS_AIC15_BASE_ADDR          HWTS_AIC15_BASE_ADDR             ; /* 0x178 */
    U_HWTS_AIC16_BASE_ADDR          HWTS_AIC16_BASE_ADDR             ; /* 0x180 */
    U_HWTS_AIC17_BASE_ADDR          HWTS_AIC17_BASE_ADDR             ; /* 0x188 */
    U_HWTS_AIC18_BASE_ADDR          HWTS_AIC18_BASE_ADDR             ; /* 0x190 */
    U_HWTS_AIC19_BASE_ADDR          HWTS_AIC19_BASE_ADDR             ; /* 0x198 */
    U_HWTS_AIC20_BASE_ADDR          HWTS_AIC20_BASE_ADDR             ; /* 0x1a0 */
    U_HWTS_AIC21_BASE_ADDR          HWTS_AIC21_BASE_ADDR             ; /* 0x1a8 */
    U_HWTS_AIC22_BASE_ADDR          HWTS_AIC22_BASE_ADDR             ; /* 0x1b0 */
    U_HWTS_AIC23_BASE_ADDR          HWTS_AIC23_BASE_ADDR             ; /* 0x1b8 */
    U_HWTS_AIC24_BASE_ADDR          HWTS_AIC24_BASE_ADDR             ; /* 0x1c0 */
    U_HWTS_AIC25_BASE_ADDR          HWTS_AIC25_BASE_ADDR             ; /* 0x1c8 */
    U_HWTS_AIC26_BASE_ADDR          HWTS_AIC26_BASE_ADDR             ; /* 0x1d0 */
    U_HWTS_AIC27_BASE_ADDR          HWTS_AIC27_BASE_ADDR             ; /* 0x1d8 */
    U_HWTS_AIC28_BASE_ADDR          HWTS_AIC28_BASE_ADDR             ; /* 0x1e0 */
    U_HWTS_AIC29_BASE_ADDR          HWTS_AIC29_BASE_ADDR             ; /* 0x1e8 */
    U_HWTS_AIC30_BASE_ADDR          HWTS_AIC30_BASE_ADDR             ; /* 0x1f0 */
    U_HWTS_AIC31_BASE_ADDR          HWTS_AIC31_BASE_ADDR             ; /* 0x1f8 */
    U_HWTS_FREE_CORE_BITMAP         HWTS_FREE_CORE_BITMAP            ; /* 0x200 */
    U_HWTS_BUS_ERR_INFO             HWTS_BUS_ERR_INFO                ; /* 0x208 */
    U_HWTS_SQE_ERR_INFO             HWTS_SQE_ERR_INFO                ; /* 0x210 */
    unsigned long long                    reserved_1                       ; /* 0x218 */
    U_HWTS_ECC_ERR_INFO0            HWTS_ECC_ERR_INFO0               ; /* 0x220 */
    U_HWTS_ECC_ERR_INFO1            HWTS_ECC_ERR_INFO1               ; /* 0x228 */
    U_HWTS_CFG_ECO0_RESERVED        HWTS_CFG_ECO0_RESERVED           ; /* 0x230 */
    U_HWTS_CFG_ECO1_RESERVED        HWTS_CFG_ECO1_RESERVED           ; /* 0x238 */
    U_HWTS_CFG_ECO2_RESERVED        HWTS_CFG_ECO2_RESERVED           ; /* 0x240 */
    U_HWTS_CFG_ECO3_RESERVED        HWTS_CFG_ECO3_RESERVED           ; /* 0x248 */
    U_HWTS_ECO0_STATE_RESERVED      HWTS_ECO0_STATE_RESERVED         ; /* 0x250 */
    U_HWTS_ECO1_STATE_RESERVED      HWTS_ECO1_STATE_RESERVED         ; /* 0x258 */
    U_HWTS_ECO2_STATE_RESERVED      HWTS_ECO2_STATE_RESERVED         ; /* 0x260 */
    U_HWTS_ECO3_STATE_RESERVED      HWTS_ECO3_STATE_RESERVED         ; /* 0x268 */
    U_HWTS_FREE_SDMA_SQ_BITMAP      HWTS_FREE_SDMA_SQ_BITMAP         ; /* 0x270 */
    unsigned long long                    reserved_2[17]                   ; /* 0x278~0x2f8 */
    U_HWTS_BLK_FSM_STATE            HWTS_BLK_FSM_STATE[32]           ; /* 0x300~0x3f8 */
    U_HWTS_L1_NORMAL_INTERRUPT      HWTS_L1_NORMAL_INTERRUPT         ; /* 0x400 */
    U_HWTS_L1_DEBUG_INTERRUPT       HWTS_L1_DEBUG_INTERRUPT          ; /* 0x408 */
    U_HWTS_L1_ERROR_INTERRUPT       HWTS_L1_ERROR_INTERRUPT          ; /* 0x410 */
    U_HWTS_L1_NORMAL_INTERRUPT_MASK   HWTS_L1_NORMAL_INTERRUPT_MASK    ; /* 0x418 */
    U_HWTS_L1_DEBUG_INTERRUPT_MASK   HWTS_L1_DEBUG_INTERRUPT_MASK     ; /* 0x420 */
    U_HWTS_L1_ERROR_INTERRUPT_MASK   HWTS_L1_ERROR_INTERRUPT_MASK     ; /* 0x428 */
    U_HWTS_L1_NORMAL_INTERRUPT_FORCE   HWTS_L1_NORMAL_INTERRUPT_FORCE   ; /* 0x430 */
    U_HWTS_L1_ERROR_INTERRUPT_FORCE   HWTS_L1_ERROR_INTERRUPT_FORCE    ; /* 0x438 */
    U_HWTS_L1_NORMAL_INTERRUPT_MASK_SET   HWTS_L1_NORMAL_INTERRUPT_MASK_SET ; /* 0x440 */
    U_HWTS_L1_DEBUG_INTERRUPT_MASK_SET   HWTS_L1_DEBUG_INTERRUPT_MASK_SET ; /* 0x448 */
    U_HWTS_L1_ERROR_INTERRUPT_MASK_SET   HWTS_L1_ERROR_INTERRUPT_MASK_SET ; /* 0x450 */
    U_HWTS_L1_NORMAL_INTERRUPT_MASK_CLR   HWTS_L1_NORMAL_INTERRUPT_MASK_CLR ; /* 0x458 */
    U_HWTS_L1_DEBUG_INTERRUPT_MASK_CLR   HWTS_L1_DEBUG_INTERRUPT_MASK_CLR ; /* 0x460 */
    U_HWTS_L1_ERROR_INTERRUPT_MASK_CLR   HWTS_L1_ERROR_INTERRUPT_MASK_CLR ; /* 0x468 */
    U_HWTS_L1_NORMAL_INTERRUPT_PENDING   HWTS_L1_NORMAL_INTERRUPT_PENDING ; /* 0x470 */
    U_HWTS_L1_DEBUG_INTERRUPT_PENDING   HWTS_L1_DEBUG_INTERRUPT_PENDING  ; /* 0x478 */
    U_HWTS_L1_ERROR_INTERRUPT_PENDING   HWTS_L1_ERROR_INTERRUPT_PENDING  ; /* 0x480 */
    U_SQ_DONE_PENDING               SQ_DONE_PENDING                  ; /* 0x488 */
    U_SQ_DEBUG_PENDING              SQ_DEBUG_PENDING                 ; /* 0x490 */
    U_SQ_EXCEPTION_PENDING          SQ_EXCEPTION_PENDING             ; /* 0x498 */
    unsigned long long                    reserved_3[12]                   ; /* 0x4a0~0x4f8 */
    U_HWTS_L2_NORMAL_AIC_DONE_INTERRUPT   HWTS_L2_NORMAL_AIC_DONE_INTERRUPT ; /* 0x500 */
    U_HWTS_L2_NORMAL_PRE_PAUSED_INTERRUPT   HWTS_L2_NORMAL_PRE_PAUSED_INTERRUPT ; /* 0x508 */
    U_HWTS_L2_NORMAL_POST_PAUSED_INTERRUPT   HWTS_L2_NORMAL_POST_PAUSED_INTERRUPT ; /* 0x510 */
    U_HWTS_L2_NORMAL_CQ_FULL_INTERRUPT   HWTS_L2_NORMAL_CQ_FULL_INTERRUPT ; /* 0x518 */
    U_HWTS_L2_NORMAL_TASK_PAUSED_INTERRUPT   HWTS_L2_NORMAL_TASK_PAUSED_INTERRUPT ; /* 0x520 */
    U_HWTS_L2_NORMAL_EVENT_RECORD_INTERRUPT   HWTS_L2_NORMAL_EVENT_RECORD_INTERRUPT[16] ; /* 0x528~0x5a0 */
    U_HWTS_L2_NORMAL_EVENT_WAIT_INTERRUPT   HWTS_L2_NORMAL_EVENT_WAIT_INTERRUPT ; /* 0x5a8 */
    U_HWTS_L2_NORMAL_SQ_TIMEOUT_INTERRUPT   HWTS_L2_NORMAL_SQ_TIMEOUT_INTERRUPT ; /* 0x5b0 */
    U_HWTS_L2_DEBUG_AIC_DEBUG_INTERRUPT   HWTS_L2_DEBUG_AIC_DEBUG_INTERRUPT ; /* 0x5b8 */
    U_HWTS_L2_DEBUG_DEBUG_PAUSED_INTERRUPT   HWTS_L2_DEBUG_DEBUG_PAUSED_INTERRUPT ; /* 0x5c0 */
    U_HWTS_L2_ERROR_AIC_ERROR_INTERRUPT   HWTS_L2_ERROR_AIC_ERROR_INTERRUPT ; /* 0x5c8 */
    U_HWTS_L2_ERROR_TASK_TIMEOUT_INTERRUPT   HWTS_L2_ERROR_TASK_TIMEOUT_INTERRUPT ; /* 0x5d0 */
    U_HWTS_L2_ERROR_AIC_TRAP_INTERRUPT   HWTS_L2_ERROR_AIC_TRAP_INTERRUPT ; /* 0x5d8 */
    U_HWTS_L2_NORMAL_AIC_DONE_INTERRUPT_MASK   HWTS_L2_NORMAL_AIC_DONE_INTERRUPT_MASK ; /* 0x5e0 */
    U_HWTS_L2_NORMAL_PRE_PAUSED_INTERRUPT_MASK   HWTS_L2_NORMAL_PRE_PAUSED_INTERRUPT_MASK ; /* 0x5e8 */
    U_HWTS_L2_NORMAL_POST_PAUSED_INTERRUPT_MASK   HWTS_L2_NORMAL_POST_PAUSED_INTERRUPT_MASK ; /* 0x5f0 */
    U_HWTS_L2_NORMAL_CQ_FULL_INTERRUPT_MASK   HWTS_L2_NORMAL_CQ_FULL_INTERRUPT_MASK ; /* 0x5f8 */
    U_HWTS_L2_NORMAL_TASK_PAUSED_INTERRUPT_MASK   HWTS_L2_NORMAL_TASK_PAUSED_INTERRUPT_MASK ; /* 0x600 */
    U_HWTS_L2_NORMAL_EVENT_RECORD_INTERRUPT_MASK   HWTS_L2_NORMAL_EVENT_RECORD_INTERRUPT_MASK[16] ; /* 0x608~0x680 */
    U_HWTS_L2_NORMAL_EVENT_WAIT_INTERRUPT_MASK   HWTS_L2_NORMAL_EVENT_WAIT_INTERRUPT_MASK ; /* 0x688 */
    U_HWTS_L2_NORMAL_SQ_TIMEOUT_INTERRUPT_MASK   HWTS_L2_NORMAL_SQ_TIMEOUT_INTERRUPT_MASK ; /* 0x690 */
    U_HWTS_L2_DEBUG_AIC_DEBUG_INTERRUPT_MASK   HWTS_L2_DEBUG_AIC_DEBUG_INTERRUPT_MASK ; /* 0x698 */
    U_HWTS_L2_DEBUG_DEBUG_PAUSED_INTERRUPT_MASK   HWTS_L2_DEBUG_DEBUG_PAUSED_INTERRUPT_MASK ; /* 0x6a0 */
    U_HWTS_L2_ERROR_AIC_ERROR_INTERRUPT_MASK   HWTS_L2_ERROR_AIC_ERROR_INTERRUPT_MASK ; /* 0x6a8 */
    U_HWTS_L2_ERROR_TASK_TIMEOUT_INTERRUPT_MASK   HWTS_L2_ERROR_TASK_TIMEOUT_INTERRUPT_MASK ; /* 0x6b0 */
    U_HWTS_L2_ERROR_AIC_TRAP_INTERRUPT_MASK   HWTS_L2_ERROR_AIC_TRAP_INTERRUPT_MASK ; /* 0x6b8 */
    U_HWTS_L2_NORMAL_AIC_DONE_INTERRUPT_FORCE   HWTS_L2_NORMAL_AIC_DONE_INTERRUPT_FORCE ; /* 0x6c0 */
    U_HWTS_L2_NORMAL_PRE_PAUSED_INTERRUPT_FORCE   HWTS_L2_NORMAL_PRE_PAUSED_INTERRUPT_FORCE ; /* 0x6c8 */
    U_HWTS_L2_NORMAL_POST_PAUSED_INTERRUPT_FORCE   HWTS_L2_NORMAL_POST_PAUSED_INTERRUPT_FORCE ; /* 0x6d0 */
    U_HWTS_L2_NORMAL_CQ_FULL_INTERRUPT_FORCE   HWTS_L2_NORMAL_CQ_FULL_INTERRUPT_FORCE ; /* 0x6d8 */
    U_HWTS_L2_NORMAL_TASK_PAUSED_INTERRUPT_FORCE   HWTS_L2_NORMAL_TASK_PAUSED_INTERRUPT_FORCE ; /* 0x6e0 */
    U_HWTS_L2_NORMAL_EVENT_RECORD_INTERRUPT_FORCE   HWTS_L2_NORMAL_EVENT_RECORD_INTERRUPT_FORCE[16] ; /* 0x6e8~0x760 */
    U_HWTS_L2_NORMAL_EVENT_WAIT_INTERRUPT_FORCE   HWTS_L2_NORMAL_EVENT_WAIT_INTERRUPT_FORCE ; /* 0x768 */
    U_HWTS_L2_NORMAL_SQ_TIMEOUT_INTERRUPT_FORCE   HWTS_L2_NORMAL_SQ_TIMEOUT_INTERRUPT_FORCE ; /* 0x770 */
    U_HWTS_L2_DEBUG_AIC_DEBUG_INTERRUPT_FORCE   HWTS_L2_DEBUG_AIC_DEBUG_INTERRUPT_FORCE ; /* 0x778 */
    U_HWTS_L2_DEBUG_DEBUG_PAUSED_INTERRUPT_FORCE   HWTS_L2_DEBUG_DEBUG_PAUSED_INTERRUPT_FORCE ; /* 0x780 */
    U_HWTS_L2_ERROR_AIC_ERROR_INTERRUPT_FORCE   HWTS_L2_ERROR_AIC_ERROR_INTERRUPT_FORCE ; /* 0x788 */
    U_HWTS_L2_ERROR_TASK_TIMEOUT_INTERRUPT_FORCE   HWTS_L2_ERROR_TASK_TIMEOUT_INTERRUPT_FORCE ; /* 0x790 */
    U_HWTS_L2_ERROR_AIC_TRAP_INTERRUPT_FORCE   HWTS_L2_ERROR_AIC_TRAP_INTERRUPT_FORCE ; /* 0x798 */
    U_HWTS_L2_NORMAL_AIC_DONE_INTERRUPT_MASK_SET   HWTS_L2_NORMAL_AIC_DONE_INTERRUPT_MASK_SET ; /* 0x7a0 */
    U_HWTS_L2_NORMAL_PRE_PAUSED_INTERRUPT_MASK_SET   HWTS_L2_NORMAL_PRE_PAUSED_INTERRUPT_MASK_SET ; /* 0x7a8 */
    U_HWTS_L2_NORMAL_POST_PAUSED_INTERRUPT_MASK_SET   HWTS_L2_NORMAL_POST_PAUSED_INTERRUPT_MASK_SET ; /* 0x7b0 */
    U_HWTS_L2_NORMAL_CQ_FULL_INTERRUPT_MASK_SET   HWTS_L2_NORMAL_CQ_FULL_INTERRUPT_MASK_SET ; /* 0x7b8 */
    U_HWTS_L2_NORMAL_TASK_PAUSED_INTERRUPT_MASK_SET   HWTS_L2_NORMAL_TASK_PAUSED_INTERRUPT_MASK_SET ; /* 0x7c0 */
    U_HWTS_L2_NORMAL_EVENT_RECORD_INTERRUPT_MASK_SET   HWTS_L2_NORMAL_EVENT_RECORD_INTERRUPT_MASK_SET[16] ; /* 0x7c8~0x840 */
    U_HWTS_L2_NORMAL_EVENT_WAIT_INTERRUPT_MASK_SET   HWTS_L2_NORMAL_EVENT_WAIT_INTERRUPT_MASK_SET ; /* 0x848 */
    U_HWTS_L2_NORMAL_SQ_TIMEOUT_INTERRUPT_MASK_SET   HWTS_L2_NORMAL_SQ_TIMEOUT_INTERRUPT_MASK_SET ; /* 0x850 */
    U_HWTS_L2_DEBUG_AIC_DEBUG_INTERRUPT_MASK_SET   HWTS_L2_DEBUG_AIC_DEBUG_INTERRUPT_MASK_SET ; /* 0x858 */
    U_HWTS_L2_DEBUG_DEBUG_PAUSED_INTERRUPT_MASK_SET   HWTS_L2_DEBUG_DEBUG_PAUSED_INTERRUPT_MASK_SET ; /* 0x860 */
    U_HWTS_L2_ERROR_AIC_ERROR_INTERRUPT_MASK_SET   HWTS_L2_ERROR_AIC_ERROR_INTERRUPT_MASK_SET ; /* 0x868 */
    U_HWTS_L2_ERROR_TASK_TIMEOUT_INTERRUPT_MASK_SET   HWTS_L2_ERROR_TASK_TIMEOUT_INTERRUPT_MASK_SET ; /* 0x870 */
    U_HWTS_L2_ERROR_AIC_TRAP_INTERRUPT_MASK_SET   HWTS_L2_ERROR_AIC_TRAP_INTERRUPT_MASK_SET ; /* 0x878 */
    U_HWTS_L2_NORMAL_AIC_DONE_INTERRUPT_MASK_CLR   HWTS_L2_NORMAL_AIC_DONE_INTERRUPT_MASK_CLR ; /* 0x880 */
    U_HWTS_L2_NORMAL_PRE_PAUSED_INTERRUPT_MASK_CLR   HWTS_L2_NORMAL_PRE_PAUSED_INTERRUPT_MASK_CLR ; /* 0x888 */
    U_HWTS_L2_NORMAL_POST_PAUSED_INTERRUPT_MASK_CLR   HWTS_L2_NORMAL_POST_PAUSED_INTERRUPT_MASK_CLR ; /* 0x890 */
    U_HWTS_L2_NORMAL_CQ_FULL_INTERRUPT_MASK_CLR   HWTS_L2_NORMAL_CQ_FULL_INTERRUPT_MASK_CLR ; /* 0x898 */
    U_HWTS_L2_NORMAL_TASK_PAUSED_INTERRUPT_MASK_CLR   HWTS_L2_NORMAL_TASK_PAUSED_INTERRUPT_MASK_CLR ; /* 0x8a0 */
    U_HWTS_L2_NORMAL_EVENT_RECORD_INTERRUPT_MASK_CLR   HWTS_L2_NORMAL_EVENT_RECORD_INTERRUPT_MASK_CLR[16] ; /* 0x8a8~0x920 */
    U_HWTS_L2_NORMAL_EVENT_WAIT_INTERRUPT_MASK_CLR   HWTS_L2_NORMAL_EVENT_WAIT_INTERRUPT_MASK_CLR ; /* 0x928 */
    U_HWTS_L2_NORMAL_SQ_TIMEOUT_INTERRUPT_MASK_CLR   HWTS_L2_NORMAL_SQ_TIMEOUT_INTERRUPT_MASK_CLR ; /* 0x930 */
    U_HWTS_L2_DEBUG_AIC_DEBUG_INTERRUPT_MASK_CLR   HWTS_L2_DEBUG_AIC_DEBUG_INTERRUPT_MASK_CLR ; /* 0x938 */
    U_HWTS_L2_DEBUG_DEBUG_PAUSED_INTERRUPT_MASK_CLR   HWTS_L2_DEBUG_DEBUG_PAUSED_INTERRUPT_MASK_CLR ; /* 0x940 */
    U_HWTS_L2_ERROR_AIC_ERROR_INTERRUPT_MASK_CLR   HWTS_L2_ERROR_AIC_ERROR_INTERRUPT_MASK_CLR ; /* 0x948 */
    U_HWTS_L2_ERROR_TASK_TIMEOUT_INTERRUPT_MASK_CLR   HWTS_L2_ERROR_TASK_TIMEOUT_INTERRUPT_MASK_CLR ; /* 0x950 */
    U_HWTS_L2_ERROR_AIC_TRAP_INTERRUPT_MASK_CLR   HWTS_L2_ERROR_AIC_TRAP_INTERRUPT_MASK_CLR ; /* 0x958 */
    U_HWTS_L2_NORMAL_AIC_DONE_INTERRUPT_PENDING   HWTS_L2_NORMAL_AIC_DONE_INTERRUPT_PENDING ; /* 0x960 */
    U_HWTS_L2_NORMAL_PRE_PAUSED_INTERRUPT_PENDING   HWTS_L2_NORMAL_PRE_PAUSED_INTERRUPT_PENDING ; /* 0x968 */
    U_HWTS_L2_NORMAL_POST_PAUSED_INTERRUPT_PENDING   HWTS_L2_NORMAL_POST_PAUSED_INTERRUPT_PENDING ; /* 0x970 */
    U_HWTS_L2_NORMAL_CQ_FULL_INTERRUPT_PENDING   HWTS_L2_NORMAL_CQ_FULL_INTERRUPT_PENDING ; /* 0x978 */
    U_HWTS_L2_NORMAL_TASK_PAUSED_INTERRUPT_PENDING   HWTS_L2_NORMAL_TASK_PAUSED_INTERRUPT_PENDING ; /* 0x980 */
    U_HWTS_L2_NORMAL_EVENT_RECORD_INTERRUPT_PENDING   HWTS_L2_NORMAL_EVENT_RECORD_INTERRUPT_PENDING[16] ; /* 0x988~0xa00 */
    U_HWTS_L2_NORMAL_EVENT_WAIT_INTERRUPT_PENDING   HWTS_L2_NORMAL_EVENT_WAIT_INTERRUPT_PENDING ; /* 0xa08 */
    U_HWTS_L2_NORMAL_SQ_TIMEOUT_INTERRUPT_PENDING   HWTS_L2_NORMAL_SQ_TIMEOUT_INTERRUPT_PENDING ; /* 0xa10 */
    U_HWTS_L2_DEBUG_AIC_DEBUG_INTERRUPT_PENDING   HWTS_L2_DEBUG_AIC_DEBUG_INTERRUPT_PENDING ; /* 0xa18 */
    U_HWTS_L2_DEBUG_DEBUG_PAUSED_INTERRUPT_PENDING   HWTS_L2_DEBUG_DEBUG_PAUSED_INTERRUPT_PENDING ; /* 0xa20 */
    U_HWTS_L2_ERROR_AIC_ERROR_INTERRUPT_PENDING   HWTS_L2_ERROR_AIC_ERROR_INTERRUPT_PENDING ; /* 0xa28 */
    U_HWTS_L2_ERROR_TASK_TIMEOUT_INTERRUPT_PENDING   HWTS_L2_ERROR_TASK_TIMEOUT_INTERRUPT_PENDING ; /* 0xa30 */
    U_HWTS_L2_ERROR_AIC_TRAP_INTERRUPT_PENDING   HWTS_L2_ERROR_AIC_TRAP_INTERRUPT_PENDING ; /* 0xa38 */
    U_HWTS_L2_ERROR_SDMA_ERROR_INTERRUPT   HWTS_L2_ERROR_SDMA_ERROR_INTERRUPT ; /* 0xa40 */
    U_HWTS_L2_ERROR_SDMA_TIMEOUT_INTERRUPT   HWTS_L2_ERROR_SDMA_TIMEOUT_INTERRUPT ; /* 0xa48 */
    U_HWTS_L2_ERROR_SDMA_ERROR_MASK   HWTS_L2_ERROR_SDMA_ERROR_MASK    ; /* 0xa50 */
    U_HWTS_L2_ERROR_SDMA_TIMEOUT_MASK   HWTS_L2_ERROR_SDMA_TIMEOUT_MASK  ; /* 0xa58 */
    U_HWTS_L2_ERROR_SDMA_ERROR_FORCE   HWTS_L2_ERROR_SDMA_ERROR_FORCE   ; /* 0xa60 */
    U_HWTS_L2_ERROR_SDMA_TIMEOUT_FORCE   HWTS_L2_ERROR_SDMA_TIMEOUT_FORCE ; /* 0xa68 */
    U_HWTS_L2_ERROR_SDMA_ERROR_MASK_SET   HWTS_L2_ERROR_SDMA_ERROR_MASK_SET ; /* 0xa70 */
    U_HWTS_L2_ERROR_SDMA_TIMEOUT_MASK_SET   HWTS_L2_ERROR_SDMA_TIMEOUT_MASK_SET ; /* 0xa78 */
    U_HWTS_L2_ERROR_SDMA_ERROR_MASK_CLR   HWTS_L2_ERROR_SDMA_ERROR_MASK_CLR ; /* 0xa80 */
    U_HWTS_L2_ERROR_SDMA_TIMEOUT_MASK_CLR   HWTS_L2_ERROR_SDMA_TIMEOUT_MASK_CLR ; /* 0xa88 */
    U_HWTS_L2_ERROR_SDMA_ERROR_PENDING   HWTS_L2_ERROR_SDMA_ERROR_PENDING ; /* 0xa90 */
    U_HWTS_L2_ERROR_SDMA_TIMEOUT_PENDING   HWTS_L2_ERROR_SDMA_TIMEOUT_PENDING ; /* 0xa98 */
    unsigned long long                    reserved_4[140]                  ; /* 0xaa0~0xef8 */
    U_HWTS_NOTIFY_PROFILE_GLOBAL_SETTING0   HWTS_NOTIFY_PROFILE_GLOBAL_SETTING0 ; /* 0xf00 */
    U_HWTS_NOTIFY_PROFILE_GLOBAL_SETTING1   HWTS_NOTIFY_PROFILE_GLOBAL_SETTING1 ; /* 0xf08 */
    U_HWTS_NOTIFY_PROFILER_0_SEL    HWTS_NOTIFY_PROFILER_0_SEL       ; /* 0xf10 */
    U_HWTS_NOTIFY_PROFILER_1_SEL    HWTS_NOTIFY_PROFILER_1_SEL       ; /* 0xf18 */
    U_HWTS_NOTIFY_PROFILER_2_SEL    HWTS_NOTIFY_PROFILER_2_SEL       ; /* 0xf20 */
    U_HWTS_NOTIFY_PROFILER_3_SEL    HWTS_NOTIFY_PROFILER_3_SEL       ; /* 0xf28 */
    U_HWTS_NOTIFY_PROFILER_4_SEL    HWTS_NOTIFY_PROFILER_4_SEL       ; /* 0xf30 */
    U_HWTS_NOTIFY_PROFILER_5_SEL    HWTS_NOTIFY_PROFILER_5_SEL       ; /* 0xf38 */
    U_HWTS_NOTIFY_PROFILER_CNT      HWTS_NOTIFY_PROFILER_CNT[24]     ; /* 0xf40~0xff8 */
    U_HWTS_SQ_BASE_ADDR             HWTS_SQ_BASE_ADDR[409]           ; /* 0x1000~0x1cc0 */
    U_HWTS_SQ_DB                    HWTS_SQ_DB[409]                  ; /* 0x1008~0x1cc8 */
    U_HWTS_SQ_STATUS                HWTS_SQ_STATUS[409]              ; /* 0x1010~0x1cd0 */
    U_HWTS_TASK_CTRL                HWTS_TASK_CTRL[409]              ; /* 0x1018~0x1cd8 */
    U_HWTS_SQ_TIMEOUT_THRESHOLD     HWTS_SQ_TIMEOUT_THRESHOLD[409]   ; /* 0x1020~0x1ce0 */
    U_HWTS_SQ_RUNTIME               HWTS_SQ_RUNTIME[409]             ; /* 0x1028~0x1ce8 */
    U_HWTS_AXPROT_SETTING           HWTS_AXPROT_SETTING[409]         ; /* 0x1030~0x1cf0 */
    U_HWTS_RT_SQID                  HWTS_RT_SQID[409]                ; /* 0x1038~0x1cf8 */
    unsigned long long                    reserved_5[96]                   ; /* 0x1d00~0x1ff8 */
    U_HWTS_CQ_BASE_ADDR             HWTS_CQ_BASE_ADDR[205]           ; /* 0x2000~0x2660 */
    U_HWTS_CQ_DB                    HWTS_CQ_DB[205]                  ; /* 0x2008~0x2668 */
    U_HWTS_CQ_STATUS                HWTS_CQ_STATUS[205]              ; /* 0x2010~0x2670 */
    unsigned long long                    reserved_6[305]                  ; /* 0x2678~0x2ff8 */
    U_SQCQ_FSM_AIC_OWN_STATE        SQCQ_FSM_AIC_OWN_STATE[409]      ; /* 0x3000~0x3cc0 */
    U_SQCQ_FSM_AIC_DEBUG_STATE      SQCQ_FSM_AIC_DEBUG_STATE[409]    ; /* 0x3008~0x3cc8 */
    U_SQCQ_FSM_AIC_EXCEPTION_STATE   SQCQ_FSM_AIC_EXCEPTION_STATE[409] ; /* 0x3010~0x3cd0 */
    U_SQCQ_FSM_AIC_TRAP_STATE       SQCQ_FSM_AIC_TRAP_STATE[409]     ; /* 0x3018~0x3cd8 */
    U_SQCQ_FSM_AIC_DONE_STATE       SQCQ_FSM_AIC_DONE_STATE[409]     ; /* 0x3020~0x3ce0 */
    U_SQCQ_FSM_MISC_STATE           SQCQ_FSM_MISC_STATE[409]         ; /* 0x3028~0x3ce8 */
    U_SQCQ_FSM_STATE                SQCQ_FSM_STATE[409]              ; /* 0x3030~0x3cf0 */
    U_SQCQ_FSM_SDMA_OWN_STATE       SQCQ_FSM_SDMA_OWN_STATE[409]     ; /* 0x3038~0x3cf8 */
    unsigned long long                    reserved_7[96]                   ; /* 0x3d00~0x3ff8 */
    U_HWTS_EVENT_TABLE              HWTS_EVENT_TABLE[1024]           ; /* 0x4000~0x5ff8 */
    U_HWTS_CROSS_SOC_EVENT_TABLE    HWTS_CROSS_SOC_EVENT_TABLE[1024] ; /* 0x6000~0x7ff8 */
    U_DFX_PROFILE_CTRL              DFX_PROFILE_CTRL                 ; /* 0x8000 */
    U_DFX_PROFILE_BASE_ADDR         DFX_PROFILE_BASE_ADDR            ; /* 0x8008 */
    U_DFX_PROFILE_PTR               DFX_PROFILE_PTR                  ; /* 0x8010 */
    unsigned long long                    reserved_8[509]                  ; /* 0x8018~0x8ff8 */
    U_DFX_PCT_CTRL                  DFX_PCT_CTRL                     ; /* 0x9000 */
    U_DFX_PCT_BASE_ADDR             DFX_PCT_BASE_ADDR                ; /* 0x9008 */
    U_DFX_PCT_PTR                   DFX_PCT_PTR                      ; /* 0x9010 */
    unsigned long long                    reserved_9[509]                  ; /* 0x9018~0x9ff8 */
    U_DFX_LOG_CTRL                  DFX_LOG_CTRL                     ; /* 0xa000 */
    U_DFX_LOG_BASE_ADDR             DFX_LOG_BASE_ADDR                ; /* 0xa008 */
    U_DFX_LOG_PTR                   DFX_LOG_PTR                      ; /* 0xa010 */
    unsigned long long                    reserved_10[509]                 ; /* 0xa018~0xaff8 */
    U_SDMA_GLB_SETTING1             SDMA_GLB_SETTING1                ; /* 0xb000 */
    U_SDMA_GLB_SETTING2             SDMA_GLB_SETTING2                ; /* 0xb008 */
    unsigned long long                    reserved_11[3]                   ; /* 0xb010~0xb020 */
    U_SDMA_SQCQ_DB                  SDMA_SQCQ_DB[29]                 ; /* 0xb028~0xb108 */
    U_SDMA_CQE_STATUS               SDMA_CQE_STATUS[29]              ; /* 0xb030~0xb110 */
    U_DFX_SDMA_FSM_STATE            DFX_SDMA_FSM_STATE[29]           ; /* 0xb038~0xb118 */
    unsigned long long                    reserved_12[1500]                ; /* 0xb120~0xdff8 */
    U_HWTS_NOTIFY_TABLE             HWTS_NOTIFY_TABLE[1024]          ; /* 0xe000~0xfff8 */

} S_HWTS_REG_REGS_TYPE;

/* Declare the struct pointor of the module HWTS_REG */
extern S_HWTS_REG_REGS_TYPE *gopHWTS_REGAllReg;


#endif /* __C_UNION_DEFINE_H__ */
