// ****************************************************************************** 
// Copyright     :  Copyright (C) 2018, Hisilicon Technologies Co. Ltd.
// File name     :  sdmam_ch_regs_c_union_define.h
// Department    :  CAD Development Department
// Author        :  xxx
// Version       :  1
// Date          :  2013/3/10
// Description   :  The description of xxx project
// Others        :  Generated automatically by nManager V4.2 
// History       :  xxx 2018/04/10 11:32:30 Create file
// ******************************************************************************

#ifndef __SDMAM_CH_REGS_C_UNION_DEFINE_H__
#define __SDMAM_CH_REGS_C_UNION_DEFINE_H__

/* Define the union U_SDMAM_CH_CTLR */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    ch_quiescent    : 1  ; /* [31] */
        unsigned int    rsv_0           : 11  ; /* [30:20] */
        unsigned int    ch_fsm_status   : 4  ; /* [19:16] */
        unsigned int    rsv_1           : 9  ; /* [15:7] */
        unsigned int    ch_retry        : 1  ; /* [6] */
        unsigned int    ch_abort_en     : 1  ; /* [5] */
        unsigned int    ch_ns           : 1  ; /* [4] */
        unsigned int    ch_reset        : 1  ; /* [3] */
        unsigned int    ch_pause_resume : 1  ; /* [2] */
        unsigned int    ch_pause        : 1  ; /* [1] */
        unsigned int    ch_enable       : 1  ; /* [0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_SDMAM_CH_CTLR;

/* Define the union U_SDMAM_CH_IIDR */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    productid          : 8  ; /* [31:24] */
        unsigned int    rsv_2              : 4  ; /* [23:20] */
        unsigned int    variant            : 4  ; /* [19:16] */
        unsigned int    revision           : 4  ; /* [15:12] */
        unsigned int    jedec_continuation : 4  ; /* [11:8] */
        unsigned int    rsv_3              : 1  ; /* [7] */
        unsigned int    jedec_id           : 7  ; /* [6:0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_SDMAM_CH_IIDR;

/* Define the union U_SDMAM_CH_TYPER */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    rsv_4             : 27  ; /* [31:5] */
        unsigned int    typer_qrs         : 1  ; /* [4] */
        unsigned int    typer_srs         : 1  ; /* [3] */
        unsigned int    typer_ras         : 1  ; /* [2] */
        unsigned int    typer_distributed : 1  ; /* [1] */
        unsigned int    typer_sq_abort    : 1  ; /* [0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_SDMAM_CH_TYPER;

/* Define the union U_SDMAM_INT_CTLR */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    rsv_5         : 4  ; /* [31:28] */
        unsigned int    ch_err_status : 8  ; /* [27:20] */
        unsigned int    rsv_6         : 2  ; /* [19:18] */
        unsigned int    ch_ioe_status : 1  ; /* [17] */
        unsigned int    ch_ioc_status : 1  ; /* [16] */
        unsigned int    rsv_7         : 14  ; /* [15:2] */
        unsigned int    ch_ioe_mask   : 1  ; /* [1] */
        unsigned int    ch_ioc_mask   : 1  ; /* [0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_SDMAM_INT_CTLR;

/* Define the union U_SDMAM_CH_SQBASER_L */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    sq_ba_l : 20  ; /* [31:12] */
        unsigned int    rsv_8   : 12  ; /* [11:0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_SDMAM_CH_SQBASER_L;

/* Define the union U_SDMAM_CH_SQBASER_H */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    rsv_9   : 16  ; /* [31:16] */
        unsigned int    sq_ba_h : 16  ; /* [15:0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_SDMAM_CH_SQBASER_H;

/* Define the union U_SDMAM_CH_SQ_ATTR */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    rsv_10          : 10  ; /* [31:22] */
        unsigned int    sq_shareability : 2  ; /* [21:20] */
        unsigned int    rsv_11          : 1  ; /* [19] */
        unsigned int    sq_cacheability : 3  ; /* [18:16] */
        unsigned int    sq_size         : 16  ; /* [15:0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_SDMAM_CH_SQ_ATTR;

/* Define the union U_SDMAM_CH_SQTDBR */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    rsv_12 : 16  ; /* [31:16] */
        unsigned int    sq_tdb : 16  ; /* [15:0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_SDMAM_CH_SQTDBR;

/* Define the union U_SDMAM_CH_SQHDBR */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    rsv_13 : 16  ; /* [31:16] */
        unsigned int    sq_hdb : 16  ; /* [15:0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_SDMAM_CH_SQHDBR;

/* Define the union U_SDMAM_CH_CQBASER_L */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    cq_ba_l : 20  ; /* [31:12] */
        unsigned int    rsv_14  : 12  ; /* [11:0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_SDMAM_CH_CQBASER_L;

/* Define the union U_SDMAM_CH_CQBASER_H */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    rsv_15  : 16  ; /* [31:16] */
        unsigned int    cq_ba_h : 16  ; /* [15:0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_SDMAM_CH_CQBASER_H;

/* Define the union U_SDMAM_CH_CQ_ATTR */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    rsv_16          : 10  ; /* [31:22] */
        unsigned int    cq_shareability : 2  ; /* [21:20] */
        unsigned int    rsv_17          : 1  ; /* [19] */
        unsigned int    cq_cacheability : 3  ; /* [18:16] */
        unsigned int    cq_size         : 16  ; /* [15:0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_SDMAM_CH_CQ_ATTR;

/* Define the union U_SDMAM_CH_CQTDBR */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    rsv_18 : 16  ; /* [31:16] */
        unsigned int    cq_tdb : 16  ; /* [15:0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_SDMAM_CH_CQTDBR;

/* Define the union U_SDMAM_CH_CQHDBR */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    rsv_19 : 16  ; /* [31:16] */
        unsigned int    cq_hdb : 16  ; /* [15:0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_SDMAM_CH_CQHDBR;

/* Define the union U_DFX_CTRL0 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    dfx_ctrl0 : 32  ; /* [31:0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_DFX_CTRL0;

/* Define the union U_DFX_INF0 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    dfx_inf0_h : 8  ; /* [31:24] */
        unsigned int    dfx_inf0_l : 24  ; /* [23:0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_DFX_INF0;

/* Define the union U_DFX_INF1 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    dfx_inf1 : 32  ; /* [31:0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_DFX_INF1;

/* Define the union U_DFX_INF2 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    dfx_inf2 : 32  ; /* [31:0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_DFX_INF2;

/* Define the union U_DFX_INF3 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    dfx_inf3 : 32  ; /* [31:0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_DFX_INF3;

/* Define the union U_DFX_INF4 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    dfx_inf4 : 32  ; /* [31:0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_DFX_INF4;

/* Define the union U_DFX_INF5 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    dfx_inf5 : 32  ; /* [31:0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_DFX_INF5;

/* Define the union U_DFX_INF6 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    dfx_inf6 : 32  ; /* [31:0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_DFX_INF6;

/* Define the union U_DFX_INF7 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    dfx_inf7 : 32  ; /* [31:0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_DFX_INF7;

/* Define the union U_DFX_EMU_INFO0 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    rsv_20             : 16  ; /* [31:16] */
        unsigned int    dfx_emu_press_cnt0 : 16  ; /* [15:0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_DFX_EMU_INFO0;

/* Define the union U_DFX_EMU_INFO1 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    rsv_21             : 16  ; /* [31:16] */
        unsigned int    dfx_emu_press_cnt1 : 16  ; /* [15:0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_DFX_EMU_INFO1;

/* Define the union U_DFX_EMU_INFO2 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    rsv_22             : 16  ; /* [31:16] */
        unsigned int    dfx_emu_press_cnt2 : 16  ; /* [15:0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_DFX_EMU_INFO2;

/* Define the union U_DFX_EMU_INFO3 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    rsv_23             : 16  ; /* [31:16] */
        unsigned int    dfx_emu_press_cnt3 : 16  ; /* [15:0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_DFX_EMU_INFO3;

/* Define the union U_DFX_EMU_INFO4 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    rsv_24             : 16  ; /* [31:16] */
        unsigned int    dfx_emu_press_cnt4 : 16  ; /* [15:0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_DFX_EMU_INFO4;

/* Define the union U_DFX_EMU_INFO5 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    rsv_25             : 16  ; /* [31:16] */
        unsigned int    dfx_emu_press_cnt5 : 16  ; /* [15:0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_DFX_EMU_INFO5;

/* Define the union U_DFX_EMU_INFO6 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    rsv_26             : 16  ; /* [31:16] */
        unsigned int    dfx_emu_press_cnt6 : 16  ; /* [15:0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_DFX_EMU_INFO6;

/* Define the union U_DFX_EMU_INFO7 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    rsv_27             : 16  ; /* [31:16] */
        unsigned int    dfx_emu_press_cnt7 : 16  ; /* [15:0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_DFX_EMU_INFO7;

/* Define the union U_DFX_EMU_INFO8 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    rsv_28             : 16  ; /* [31:16] */
        unsigned int    dfx_emu_press_cnt8 : 16  ; /* [15:0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_DFX_EMU_INFO8;

/* Define the union U_DFX_EMU_INFO9 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    rsv_29             : 16  ; /* [31:16] */
        unsigned int    dfx_emu_press_cnt9 : 16  ; /* [15:0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_DFX_EMU_INFO9;

/* Define the union U_DFX_EMU_INFO10 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    rsv_30              : 16  ; /* [31:16] */
        unsigned int    dfx_emu_press_cnt10 : 16  ; /* [15:0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_DFX_EMU_INFO10;

/* Define the union U_DFX_EMU_INFO11 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    rsv_31              : 16  ; /* [31:16] */
        unsigned int    dfx_emu_press_cnt11 : 16  ; /* [15:0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_DFX_EMU_INFO11;

/* Define the union U_DFX_EMU_INFO12 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    rsv_32              : 16  ; /* [31:16] */
        unsigned int    dfx_emu_press_cnt12 : 16  ; /* [15:0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_DFX_EMU_INFO12;

/* Define the union U_DFX_EMU_INFO13 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    rsv_33              : 16  ; /* [31:16] */
        unsigned int    dfx_emu_press_cnt13 : 16  ; /* [15:0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_DFX_EMU_INFO13;

/* Define the union U_DFX_EMU_INFO14 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    rsv_34              : 16  ; /* [31:16] */
        unsigned int    dfx_emu_press_cnt14 : 16  ; /* [15:0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_DFX_EMU_INFO14;

/* Define the union U_DFX_EMU_INFO15 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    rsv_35              : 16  ; /* [31:16] */
        unsigned int    dfx_emu_press_cnt15 : 16  ; /* [15:0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_DFX_EMU_INFO15;


//==============================================================================
/* Define the global struct */
typedef struct
{
    volatile U_SDMAM_CH_CTLR      SDMAM_CH_CTLR      ; /* 0 */
    volatile U_SDMAM_CH_IIDR      SDMAM_CH_IIDR      ; /* 4 */
    volatile U_SDMAM_CH_TYPER     SDMAM_CH_TYPER     ; /* 8 */
    volatile U_SDMAM_INT_CTLR     SDMAM_INT_CTLR     ; /* C */
    volatile U_SDMAM_CH_SQBASER_L SDMAM_CH_SQBASER_L ; /* 40 */
    volatile U_SDMAM_CH_SQBASER_H SDMAM_CH_SQBASER_H ; /* 44 */
    volatile U_SDMAM_CH_SQ_ATTR   SDMAM_CH_SQ_ATTR   ; /* 48 */
    volatile U_SDMAM_CH_SQTDBR    SDMAM_CH_SQTDBR    ; /* 4C */
    volatile U_SDMAM_CH_SQHDBR    SDMAM_CH_SQHDBR    ; /* 50 */
    volatile U_SDMAM_CH_CQBASER_L SDMAM_CH_CQBASER_L ; /* 80 */
    volatile U_SDMAM_CH_CQBASER_H SDMAM_CH_CQBASER_H ; /* 84 */
    volatile U_SDMAM_CH_CQ_ATTR   SDMAM_CH_CQ_ATTR   ; /* 88 */
    volatile U_SDMAM_CH_CQTDBR    SDMAM_CH_CQTDBR    ; /* 8C */
    volatile U_SDMAM_CH_CQHDBR    SDMAM_CH_CQHDBR    ; /* 90 */
    volatile U_DFX_CTRL0          DFX_CTRL0          ; /* 400 */
    volatile U_DFX_INF0           DFX_INF0           ; /* 404 */
    volatile U_DFX_INF1           DFX_INF1           ; /* 408 */
    volatile U_DFX_INF2           DFX_INF2           ; /* 40C */
    volatile U_DFX_INF3           DFX_INF3           ; /* 410 */
    volatile U_DFX_INF4           DFX_INF4           ; /* 414 */
    volatile U_DFX_INF5           DFX_INF5           ; /* 418 */
    volatile U_DFX_INF6           DFX_INF6           ; /* 41C */
    volatile U_DFX_INF7           DFX_INF7           ; /* 420 */
    volatile U_DFX_EMU_INFO0      DFX_EMU_INFO0      ; /* 500 */
    volatile U_DFX_EMU_INFO1      DFX_EMU_INFO1      ; /* 504 */
    volatile U_DFX_EMU_INFO2      DFX_EMU_INFO2      ; /* 508 */
    volatile U_DFX_EMU_INFO3      DFX_EMU_INFO3      ; /* 50C */
    volatile U_DFX_EMU_INFO4      DFX_EMU_INFO4      ; /* 510 */
    volatile U_DFX_EMU_INFO5      DFX_EMU_INFO5      ; /* 514 */
    volatile U_DFX_EMU_INFO6      DFX_EMU_INFO6      ; /* 518 */
    volatile U_DFX_EMU_INFO7      DFX_EMU_INFO7      ; /* 51C */
    volatile U_DFX_EMU_INFO8      DFX_EMU_INFO8      ; /* 520 */
    volatile U_DFX_EMU_INFO9      DFX_EMU_INFO9      ; /* 524 */
    volatile U_DFX_EMU_INFO10     DFX_EMU_INFO10     ; /* 528 */
    volatile U_DFX_EMU_INFO11     DFX_EMU_INFO11     ; /* 52C */
    volatile U_DFX_EMU_INFO12     DFX_EMU_INFO12     ; /* 530 */
    volatile U_DFX_EMU_INFO13     DFX_EMU_INFO13     ; /* 534 */
    volatile U_DFX_EMU_INFO14     DFX_EMU_INFO14     ; /* 538 */
    volatile U_DFX_EMU_INFO15     DFX_EMU_INFO15     ; /* 53C */

} S_sdmam_ch_regs_REGS_TYPE;

/* Declare the struct pointor of the module sdmam_ch_regs */
extern volatile S_sdmam_ch_regs_REGS_TYPE *gopsdmam_ch_regsAllReg;

/* Declare the functions that set the member value */
int iSetSDMAM_CH_CTLR_ch_quiescent(unsigned int uch_quiescent);
int iSetSDMAM_CH_CTLR_ch_fsm_status(unsigned int uch_fsm_status);
int iSetSDMAM_CH_CTLR_ch_retry(unsigned int uch_retry);
int iSetSDMAM_CH_CTLR_ch_abort_en(unsigned int uch_abort_en);
int iSetSDMAM_CH_CTLR_ch_ns(unsigned int uch_ns);
int iSetSDMAM_CH_CTLR_ch_reset(unsigned int uch_reset);
int iSetSDMAM_CH_CTLR_ch_pause_resume(unsigned int uch_pause_resume);
int iSetSDMAM_CH_CTLR_ch_pause(unsigned int uch_pause);
int iSetSDMAM_CH_CTLR_ch_enable(unsigned int uch_enable);
int iSetSDMAM_CH_IIDR_productid(unsigned int uproductid);
int iSetSDMAM_CH_IIDR_variant(unsigned int uvariant);
int iSetSDMAM_CH_IIDR_revision(unsigned int urevision);
int iSetSDMAM_CH_IIDR_jedec_continuation(unsigned int ujedec_continuation);
int iSetSDMAM_CH_IIDR_jedec_id(unsigned int ujedec_id);
int iSetSDMAM_CH_TYPER_typer_qrs(unsigned int utyper_qrs);
int iSetSDMAM_CH_TYPER_typer_srs(unsigned int utyper_srs);
int iSetSDMAM_CH_TYPER_typer_ras(unsigned int utyper_ras);
int iSetSDMAM_CH_TYPER_typer_distributed(unsigned int utyper_distributed);
int iSetSDMAM_CH_TYPER_typer_sq_abort(unsigned int utyper_sq_abort);
int iSetSDMAM_INT_CTLR_ch_err_status(unsigned int uch_err_status);
int iSetSDMAM_INT_CTLR_ch_ioe_status(unsigned int uch_ioe_status);
int iSetSDMAM_INT_CTLR_ch_ioc_status(unsigned int uch_ioc_status);
int iSetSDMAM_INT_CTLR_ch_ioe_mask(unsigned int uch_ioe_mask);
int iSetSDMAM_INT_CTLR_ch_ioc_mask(unsigned int uch_ioc_mask);
int iSetSDMAM_CH_SQBASER_L_sq_ba_l(unsigned int usq_ba_l);
int iSetSDMAM_CH_SQBASER_H_sq_ba_h(unsigned int usq_ba_h);
int iSetSDMAM_CH_SQ_ATTR_sq_shareability(unsigned int usq_shareability);
int iSetSDMAM_CH_SQ_ATTR_sq_cacheability(unsigned int usq_cacheability);
int iSetSDMAM_CH_SQ_ATTR_sq_size(unsigned int usq_size);
int iSetSDMAM_CH_SQTDBR_sq_tdb(unsigned int usq_tdb);
int iSetSDMAM_CH_SQHDBR_sq_hdb(unsigned int usq_hdb);
int iSetSDMAM_CH_CQBASER_L_cq_ba_l(unsigned int ucq_ba_l);
int iSetSDMAM_CH_CQBASER_H_cq_ba_h(unsigned int ucq_ba_h);
int iSetSDMAM_CH_CQ_ATTR_cq_shareability(unsigned int ucq_shareability);
int iSetSDMAM_CH_CQ_ATTR_cq_cacheability(unsigned int ucq_cacheability);
int iSetSDMAM_CH_CQ_ATTR_cq_size(unsigned int ucq_size);
int iSetSDMAM_CH_CQTDBR_cq_tdb(unsigned int ucq_tdb);
int iSetSDMAM_CH_CQHDBR_cq_hdb(unsigned int ucq_hdb);
int iSetDFX_CTRL0_dfx_ctrl0(unsigned int udfx_ctrl0);
int iSetDFX_INF0_dfx_inf0_h(unsigned int udfx_inf0_h);
int iSetDFX_INF0_dfx_inf0_l(unsigned int udfx_inf0_l);
int iSetDFX_INF1_dfx_inf1(unsigned int udfx_inf1);
int iSetDFX_INF2_dfx_inf2(unsigned int udfx_inf2);
int iSetDFX_INF3_dfx_inf3(unsigned int udfx_inf3);
int iSetDFX_INF4_dfx_inf4(unsigned int udfx_inf4);
int iSetDFX_INF5_dfx_inf5(unsigned int udfx_inf5);
int iSetDFX_INF6_dfx_inf6(unsigned int udfx_inf6);
int iSetDFX_INF7_dfx_inf7(unsigned int udfx_inf7);
int iSetDFX_EMU_INFO0_dfx_emu_press_cnt0(unsigned int udfx_emu_press_cnt0);
int iSetDFX_EMU_INFO1_dfx_emu_press_cnt1(unsigned int udfx_emu_press_cnt1);
int iSetDFX_EMU_INFO2_dfx_emu_press_cnt2(unsigned int udfx_emu_press_cnt2);
int iSetDFX_EMU_INFO3_dfx_emu_press_cnt3(unsigned int udfx_emu_press_cnt3);
int iSetDFX_EMU_INFO4_dfx_emu_press_cnt4(unsigned int udfx_emu_press_cnt4);
int iSetDFX_EMU_INFO5_dfx_emu_press_cnt5(unsigned int udfx_emu_press_cnt5);
int iSetDFX_EMU_INFO6_dfx_emu_press_cnt6(unsigned int udfx_emu_press_cnt6);
int iSetDFX_EMU_INFO7_dfx_emu_press_cnt7(unsigned int udfx_emu_press_cnt7);
int iSetDFX_EMU_INFO8_dfx_emu_press_cnt8(unsigned int udfx_emu_press_cnt8);
int iSetDFX_EMU_INFO9_dfx_emu_press_cnt9(unsigned int udfx_emu_press_cnt9);
int iSetDFX_EMU_INFO10_dfx_emu_press_cnt10(unsigned int udfx_emu_press_cnt10);
int iSetDFX_EMU_INFO11_dfx_emu_press_cnt11(unsigned int udfx_emu_press_cnt11);
int iSetDFX_EMU_INFO12_dfx_emu_press_cnt12(unsigned int udfx_emu_press_cnt12);
int iSetDFX_EMU_INFO13_dfx_emu_press_cnt13(unsigned int udfx_emu_press_cnt13);
int iSetDFX_EMU_INFO14_dfx_emu_press_cnt14(unsigned int udfx_emu_press_cnt14);
int iSetDFX_EMU_INFO15_dfx_emu_press_cnt15(unsigned int udfx_emu_press_cnt15);

#endif // __SDMAM_CH_REGS_C_UNION_DEFINE_H__
