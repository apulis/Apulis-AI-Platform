// ****************************************************************************** 
// copyright     :  copyright (c) 2017, hisilicon technologies co. ltd.
// file name     :  c_union_define_SDMAM_CH_REGS.h
// department    :  CAD development department
// author        :  xxx
// version       :  1
// date          :  2013/3/10
// description   :  the description of xxx project
// others        :  generated automatically by n_manager v4.2 
// history       :  xxx 2017/12/23 16:19:25 create file
// ******************************************************************************

#ifndef __c_UNION_DEFINE_SDMAM_CH_REGS_h__
#define __c_UNION_DEFINE_SDMAM_CH_REGS_h__

/* define the union u_SDMAM_CH_CTLR */
typedef union
{
    /* define the struct bits */
    struct
    {
        uint32_t    ch_enable             : 1   ; /* [0]  */
        uint32_t    ch_pause              : 1   ; /* [1]  */
        uint32_t    ch_pause_resume       : 1   ; /* [2]  */
        uint32_t    ch_reset              : 1   ; /* [3]  */
        uint32_t    ch_ns                 : 1   ; /* [4]  */
        uint32_t    ch_abort_en           : 1   ; /* [5]  */
        uint32_t    ch_retry              : 1   ; /* [6]  */
        uint32_t    reserved_0            : 9   ; /* [15..7]  */
        uint32_t    ch_fsm_status         : 4   ; /* [19..16]  */
        //uint32_t    ch_err_status         : 8   ; /* [27..20]  */   move to the INT_REG
        uint32_t    reserved_1            : 11   ; /* [30..20]  */
        uint32_t    ch_quiescent          : 1   ; /* [31]  */
    } bits;

    /* define an unsigned member */
    uint32_t    u32;

} u_SDMAM_CH_CTRL;

/* define the union u_SDMAM_CH_IIDR */
typedef union
{
    /* define the struct bits */
    struct
    {
        uint32_t    jedec_id              : 7   ; /* [6..0]  */
        uint32_t    reserved_0            : 1   ; /* [7]  */
        uint32_t    jedec_continuation    : 4   ; /* [11..8]  */
        uint32_t    revision              : 4   ; /* [15..12]  */
        uint32_t    variant               : 4   ; /* [19..16]  */
        uint32_t    reserved_1            : 4   ; /* [23..20]  */
        uint32_t    productid             : 8   ; /* [31..24]  */
    } bits;

    /* define an unsigned member */
    uint32_t    u32;

} u_SDMAM_CH_IIDR;

/* define the union u_SDMAM_CH_TYPER */
typedef union
{
    /* define the struct bits */
    struct
    {
        uint32_t    typer_sq_abort        : 1   ; /* [0]  */
        uint32_t    typer_distributed     : 1   ; /* [1]  */
        uint32_t    typer_ras             : 1   ; /* [2]  */
        uint32_t    typer_srs             : 1   ; /* [3]  */
        uint32_t    typer_qrs             : 1   ; /* [4]  */
        uint32_t    reserved_0            : 27  ; /* [31..5]  */
    } bits;

    /* define an unsigned member */
    uint32_t    u32;

} u_SDMAM_CH_TYPER;

/* define the union u_SDMAM_INT_CTLR */
typedef union
{
    /* define the struct bits */
    struct
    {
        uint32_t    ch_ioc_mask           : 1   ; /* [0]  */
        uint32_t    ch_ioe_mask           : 1   ; /* [1]  */
        uint32_t    reserved_0            : 14  ; /* [15..2]  */
        uint32_t    ch_ioc_status         : 1   ; /* [16]  */
        uint32_t    ch_ioe_status         : 1   ; /* [17]  */
        uint32_t    reserved_1            : 2   ; /* [19..18]  */
        uint32_t    ch_err_status         : 8   ; /* [27..20]  */
        uint32_t    reserved_2            : 4   ; /* [31..28]  */
    } bits;

    /* define an unsigned member */
    uint32_t    u32;

} u_SDMAM_INT_CTLR;

/* define the union u_SDMAM_CH_SQBASER_l */
typedef union
{
    /* define the struct bits */
    struct
    {
        uint32_t    reserved_0            : 12  ; /* [11..0]  */
        uint32_t    sq_ba_l               : 20  ; /* [31..12]  */
    } bits;

    /* define an unsigned member */
    uint32_t    u32;

} u_SDMAM_CH_SQBASER_l;

/* define the union u_SDMAM_CH_SQBASER_h */
typedef union
{
    /* define the struct bits */
    struct
    {
        uint32_t    sq_ba_h               : 16  ; /* [15..0]  */
        uint32_t    reserved_0            : 16  ; /* [31..16]  */
    } bits;

    /* define an unsigned member */
    uint32_t    u32;

} u_SDMAM_CH_SQBASER_h;

/* define the union u_SDMAM_CH_SQ_ATTR */
typedef union
{
    /* define the struct bits */
    struct
    {
        uint32_t    sq_size               : 16  ; /* [15..0]  */
        uint32_t    sq_cacheability       : 3   ; /* [18..16]  */
        uint32_t    reserved_0            : 1   ; /* [19]  */
        uint32_t    sq_shareability       : 2   ; /* [21..20]  */
        uint32_t    reserved_1            : 10  ; /* [31..22]  */
    } bits;

    /* define an unsigned member */
    uint32_t    u32;

} u_SDMAM_CH_SQ_ATTR;

/* define the union u_SDMAM_CH_SQTDBR */
typedef union
{
    /* define the struct bits */
    struct
    {
        uint32_t    sq_tdb                : 16  ; /* [15..0]  */
        uint32_t    reserved_0            : 16  ; /* [31..16]  */
    } bits;

    /* define an unsigned member */
    uint32_t    u32;

} u_SDMAM_CH_SQTDBR;

/* define the union u_SDMAM_CH_SQHDBR */
typedef union
{
    /* define the struct bits */
    struct
    {
        uint32_t    sq_hdb                : 16  ; /* [15..0]  */
        uint32_t    reserved_0            : 16  ; /* [31..16]  */
    } bits;

    /* define an unsigned member */
    uint32_t    u32;

} u_SDMAM_CH_SQHDBR;

/* define the union u_SDMAM_CH_CQBASER_l */
typedef union
{
    /* define the struct bits */
    struct
    {
        uint32_t    reserved_0            : 12  ; /* [11..0]  */
        uint32_t    cq_ba_l               : 20  ; /* [31..12]  */
    } bits;

    /* define an unsigned member */
    uint32_t    u32;

} u_SDMAM_CH_CQBASER_l;

/* define the union u_SDMAM_CH_CQBASER_h */
typedef union
{
    /* define the struct bits */
    struct
    {
        uint32_t    cq_ba_h               : 16  ; /* [15..0]  */
        uint32_t    reserved_0            : 16  ; /* [31..16]  */
    } bits;

    /* define an unsigned member */
    uint32_t    u32;

} u_SDMAM_CH_CQBASER_h;

/* define the union u_SDMAM_CH_CQ_ATTR */
typedef union
{
    /* define the struct bits */
    struct
    {
        uint32_t    cq_size               : 16  ; /* [15..0]  */
        uint32_t    cq_cacheability       : 3   ; /* [18..16]  */
        uint32_t    reserved_0            : 1   ; /* [19]  */
        uint32_t    cq_shareability       : 2   ; /* [21..20]  */
        uint32_t    reserved_1            : 10  ; /* [31..22]  */
    } bits;

    /* define an unsigned member */
    uint32_t    u32;

} u_SDMAM_CH_CQ_ATTR;

/* define the union u_SDMAM_CH_CQTDBR */
typedef union
{
    /* define the struct bits */
    struct
    {
        uint32_t    cq_tdb                : 16  ; /* [15..0]  */
        uint32_t    reserved_0            : 16  ; /* [31..16]  */
    } bits;

    /* define an unsigned member */
    uint32_t    u32;

} u_SDMAM_CH_CQTDBR;

/* define the union u_SDMAM_CH_CQHDBR */
typedef union
{
    /* define the struct bits */
    struct
    {
        uint32_t    cq_hdb                : 16  ; /* [15..0]  */
        uint32_t    reserved_0            : 16  ; /* [31..16]  */
    } bits;

    /* define an unsigned member */
    uint32_t    u32;

} u_SDMAM_CH_CQHDBR;

//==============================================================================
/* define the global struct */
typedef struct
{
    volatile u_SDMAM_CH_CTRL        SDMAM_CH_CTRL;
    volatile u_SDMAM_CH_IIDR        SDMAM_CH_IIDR;
    volatile u_SDMAM_CH_TYPER       SDMAM_CH_TYPER;
    volatile u_SDMAM_INT_CTLR       SDMAM_INT_CTLR;
    volatile u_SDMAM_CH_SQBASER_l   SDMAM_CH_SQBASER_l;
    volatile u_SDMAM_CH_SQBASER_h   SDMAM_CH_SQBASER_h;
    volatile u_SDMAM_CH_SQ_ATTR     SDMAM_CH_SQ_ATTR;
    volatile u_SDMAM_CH_SQTDBR      SDMAM_CH_SQTDBR;
    volatile u_SDMAM_CH_SQHDBR      SDMAM_CH_SQHDBR;
    volatile u_SDMAM_CH_CQBASER_l   SDMAM_CH_CQBASER_l;
    volatile u_SDMAM_CH_CQBASER_h   SDMAM_CH_CQBASER_h;
    volatile u_SDMAM_CH_CQ_ATTR     SDMAM_CH_CQ_ATTR;
    volatile u_SDMAM_CH_CQTDBR      SDMAM_CH_CQTDBR;
    volatile u_SDMAM_CH_CQHDBR      SDMAM_CH_CQHDBR;
    volatile uint32_t           DFX_CTRL0;
    volatile uint32_t           DFX_INF0;
    volatile uint32_t           DFX_INF1;
    volatile uint32_t           DFX_INF2;
    volatile uint32_t           DFX_INF3;
    volatile uint32_t           DFX_INF4;
    volatile uint32_t           DFX_INF5;
    volatile uint32_t           DFX_INF6;
    volatile uint32_t           DFX_INF7;

} s_SDMAM_CH_REGS_REGS_TYPE;



#endif /* __c_UNION_DEFINE_SDMAM_CH_REGS_h__ */
