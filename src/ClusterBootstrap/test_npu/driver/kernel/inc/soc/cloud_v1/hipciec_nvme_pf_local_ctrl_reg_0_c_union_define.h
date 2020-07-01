// ****************************************************************************** 
// Copyright     :  Copyright (C) 2018, Hisilicon Technologies Co. Ltd.
// File name     :  hipciec_nvme_pf_local_ctrl_reg_0_c_union_define.h
// Department    :  CAD Development Department
// Author        :  xxx
// Version       :  1.0
// Date          :  2017/10/24
// Description   :  The description of xxx project
// Others        :  Generated automatically by nManager V4.2 
// History       :  xxx 2018/07/13 11:19:14 Create file
// ******************************************************************************

#ifndef __HIPCIEC_NVME_PF_LOCAL_CTRL_REG_0_C_UNION_DEFINE_H__
#define __HIPCIEC_NVME_PF_LOCAL_CTRL_REG_0_C_UNION_DEFINE_H__

/* Define the union U_NVME_CTRL_CAP_LOW */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    timeout : 8  ; /* [31:24] */
        unsigned int    rsv_0   : 5  ; /* [23:19] */
        unsigned int    ams     : 2  ; /* [18:17] */
        unsigned int    cqr     : 1  ; /* [16] */
        unsigned int    mqes    : 16  ; /* [15:0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_NVME_CTRL_CAP_LOW;

/* Define the union U_NVME_CTRL_CAP_HIGH */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    rsv_1  : 8  ; /* [31:24] */
        unsigned int    mpsmax : 4  ; /* [23:20] */
        unsigned int    mpsmin : 4  ; /* [19:16] */
        unsigned int    rsv_2  : 3  ; /* [15:13] */
        unsigned int    css    : 8  ; /* [12:5] */
        unsigned int    nssrs  : 1  ; /* [4] */
        unsigned int    dstrd  : 4  ; /* [3:0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_NVME_CTRL_CAP_HIGH;

/* Define the union U_NVME_CTRL_VS */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    mjr : 16  ; /* [31:16] */
        unsigned int    mnr : 8  ; /* [15:8] */
        unsigned int    ter : 8  ; /* [7:0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_NVME_CTRL_VS;

/* Define the union U_NVME_CTRL_INTMS */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    intms : 32  ; /* [31:0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_NVME_CTRL_INTMS;

/* Define the union U_NVME_CTRL_INTMC */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    intmc : 32  ; /* [31:0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_NVME_CTRL_INTMC;

/* Define the union U_NVME_CTRL_CONFIG */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    rsv_3  : 8  ; /* [31:24] */
        unsigned int    iocqes : 4  ; /* [23:20] */
        unsigned int    iosqes : 4  ; /* [19:16] */
        unsigned int    shn    : 2  ; /* [15:14] */
        unsigned int    ams    : 3  ; /* [13:11] */
        unsigned int    mps    : 4  ; /* [10:7] */
        unsigned int    css    : 3  ; /* [6:4] */
        unsigned int    rsv_4  : 3  ; /* [3:1] */
        unsigned int    enable : 1  ; /* [0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_NVME_CTRL_CONFIG;

/* Define the union U_NVME_CTRL_STATUS */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    rsv_5 : 26  ; /* [31:6] */
        unsigned int    pp    : 1  ; /* [5] */
        unsigned int    nssro : 1  ; /* [4] */
        unsigned int    shst  : 2  ; /* [3:2] */
        unsigned int    cfs   : 1  ; /* [1] */
        unsigned int    ready : 1  ; /* [0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_NVME_CTRL_STATUS;

/* Define the union U_NVME_SUBSYS_RST */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    nssrc : 32  ; /* [31:0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_NVME_SUBSYS_RST;

/* Define the union U_AQA */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    acqs : 16  ; /* [31:16] */
        unsigned int    asqs : 16  ; /* [15:0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_AQA;

/* Define the union U_ASQB_LOW */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    asqb_low : 32  ; /* [31:0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ASQB_LOW;

/* Define the union U_ASQB_HIGH */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    asqb_high : 32  ; /* [31:0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ASQB_HIGH;

/* Define the union U_ACQB_LOW */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    acqb_low : 20  ; /* [31:12] */
        unsigned int    rsv_6    : 12  ; /* [11:0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ACQB_LOW;

/* Define the union U_ACQB_HIGH */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    acqb_high : 32  ; /* [31:0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ACQB_HIGH;

/* Define the union U_CMBLOC */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    offset : 20  ; /* [31:12] */
        unsigned int    rsv_7  : 9  ; /* [11:3] */
        unsigned int    bir    : 3  ; /* [2:0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_CMBLOC;

/* Define the union U_CMBSZ */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    sz    : 20  ; /* [31:12] */
        unsigned int    szu   : 4  ; /* [11:8] */
        unsigned int    rsv_8 : 3  ; /* [7:5] */
        unsigned int    wds   : 1  ; /* [4] */
        unsigned int    rds   : 1  ; /* [3] */
        unsigned int    lists : 1  ; /* [2] */
        unsigned int    cqs   : 1  ; /* [1] */
        unsigned int    sqs   : 1  ; /* [0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_CMBSZ;

/* Define the union U_SQTDBL */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    rsv_9  : 16  ; /* [31:16] */
        unsigned int    sqtdbl : 16  ; /* [15:0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_SQTDBL;

/* Define the union U_CQHDBL */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    rsv_10 : 16  ; /* [31:16] */
        unsigned int    cqhdbl : 16  ; /* [15:0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_CQHDBL;


//==============================================================================
/* Define the global struct */
typedef struct
{
    volatile U_NVME_CTRL_CAP_LOW  NVME_CTRL_CAP_LOW  ; /* 0 */
    volatile U_NVME_CTRL_CAP_HIGH NVME_CTRL_CAP_HIGH ; /* 4 */
    volatile U_NVME_CTRL_VS       NVME_CTRL_VS       ; /* 8 */
    volatile U_NVME_CTRL_INTMS    NVME_CTRL_INTMS    ; /* C */
    volatile U_NVME_CTRL_INTMC    NVME_CTRL_INTMC    ; /* 10 */
    volatile U_NVME_CTRL_CONFIG   NVME_CTRL_CONFIG   ; /* 14 */
    volatile U_NVME_CTRL_STATUS   NVME_CTRL_STATUS   ; /* 1C */
    volatile U_NVME_SUBSYS_RST    NVME_SUBSYS_RST    ; /* 20 */
    volatile U_AQA                AQA                ; /* 24 */
    volatile U_ASQB_LOW           ASQB_LOW           ; /* 28 */
    volatile U_ASQB_HIGH          ASQB_HIGH          ; /* 2C */
    volatile U_ACQB_LOW           ACQB_LOW           ; /* 30 */
    volatile U_ACQB_HIGH          ACQB_HIGH          ; /* 34 */
    volatile U_CMBLOC             CMBLOC             ; /* 38 */
    volatile U_CMBSZ              CMBSZ              ; /* 3C */
    volatile U_SQTDBL             SQTDBL[128]        ; /* 1000 */
    volatile U_CQHDBL             CQHDBL[128]        ; /* 1004 */

} S_hipciec_nvme_pf_local_ctrl_reg_0_REGS_TYPE;

/* Declare the struct pointor of the module hipciec_nvme_pf_local_ctrl_reg_0 */
extern volatile S_hipciec_nvme_pf_local_ctrl_reg_0_REGS_TYPE *gophipciec_nvme_pf_local_ctrl_reg_0AllReg;

/* Declare the functions that set the member value */
int iSetNVME_CTRL_CAP_LOW_timeout(unsigned int utimeout);
int iSetNVME_CTRL_CAP_LOW_ams(unsigned int uams);
int iSetNVME_CTRL_CAP_LOW_cqr(unsigned int ucqr);
int iSetNVME_CTRL_CAP_LOW_mqes(unsigned int umqes);
int iSetNVME_CTRL_CAP_HIGH_mpsmax(unsigned int umpsmax);
int iSetNVME_CTRL_CAP_HIGH_mpsmin(unsigned int umpsmin);
int iSetNVME_CTRL_CAP_HIGH_css(unsigned int ucss);
int iSetNVME_CTRL_CAP_HIGH_nssrs(unsigned int unssrs);
int iSetNVME_CTRL_CAP_HIGH_dstrd(unsigned int udstrd);
int iSetNVME_CTRL_VS_mjr(unsigned int umjr);
int iSetNVME_CTRL_VS_mnr(unsigned int umnr);
int iSetNVME_CTRL_VS_ter(unsigned int uter);
int iSetNVME_CTRL_INTMS_intms(unsigned int uintms);
int iSetNVME_CTRL_INTMC_intmc(unsigned int uintmc);
int iSetNVME_CTRL_CONFIG_iocqes(unsigned int uiocqes);
int iSetNVME_CTRL_CONFIG_iosqes(unsigned int uiosqes);
int iSetNVME_CTRL_CONFIG_shn(unsigned int ushn);
int iSetNVME_CTRL_CONFIG_ams(unsigned int uams);
int iSetNVME_CTRL_CONFIG_mps(unsigned int umps);
int iSetNVME_CTRL_CONFIG_css(unsigned int ucss);
int iSetNVME_CTRL_CONFIG_enable(unsigned int uenable);
int iSetNVME_CTRL_STATUS_pp(unsigned int upp);
int iSetNVME_CTRL_STATUS_nssro(unsigned int unssro);
int iSetNVME_CTRL_STATUS_shst(unsigned int ushst);
int iSetNVME_CTRL_STATUS_cfs(unsigned int ucfs);
int iSetNVME_CTRL_STATUS_ready(unsigned int uready);
int iSetNVME_SUBSYS_RST_nssrc(unsigned int unssrc);
int iSetAQA_acqs(unsigned int uacqs);
int iSetAQA_asqs(unsigned int uasqs);
int iSetASQB_LOW_asqb_low(unsigned int uasqb_low);
int iSetASQB_HIGH_asqb_high(unsigned int uasqb_high);
int iSetACQB_LOW_acqb_low(unsigned int uacqb_low);
int iSetACQB_HIGH_acqb_high(unsigned int uacqb_high);
int iSetCMBLOC_offset(unsigned int uoffset);
int iSetCMBLOC_bir(unsigned int ubir);
int iSetCMBSZ_sz(unsigned int usz);
int iSetCMBSZ_szu(unsigned int uszu);
int iSetCMBSZ_wds(unsigned int uwds);
int iSetCMBSZ_rds(unsigned int urds);
int iSetCMBSZ_lists(unsigned int ulists);
int iSetCMBSZ_cqs(unsigned int ucqs);
int iSetCMBSZ_sqs(unsigned int usqs);
int iSetSQTDBL_sqtdbl(unsigned int usqtdbl);
int iSetCQHDBL_cqhdbl(unsigned int ucqhdbl);

#endif // __HIPCIEC_NVME_PF_LOCAL_CTRL_REG_0_C_UNION_DEFINE_H__
