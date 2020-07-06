// ****************************************************************************** 
// copyright     :  copyright (c) 2017, hisilicon technologies co. ltd.
// file name     :  c_union_define_SDMAM_COMMON_REGS.h
// department    :  CAD development department
// author        :  xxx
// version       :  1
// date          :  2013/3/10
// description   :  the description of xxx project
// others        :  generated automatically by n_manager v4.2 
// history       :  xxx 2017/12/23 16:19:25 create file
// ******************************************************************************

#ifndef __c_UNION_DEFINE_SDMAM_COMMON_REGS_h__
#define __c_UNION_DEFINE_SDMAM_COMMON_REGS_h__

/* define the union u_DMA_AXUSER_REG0 这部分手工生成，替换时注意保留*/
typedef union
{
    /* define the struct bits */
    struct
    {
        uint32_t    request_id : 16; /* [15..0]  */
        uint32_t    substream_id : 16; /* [31..16]  */
    } bits;

    /* define an unsigned member */
    uint32_t    u32;

} u_DMA_AXUSER_REG0;

/* define the union u_DMA_AXUSER_REG1 这部分手工生成，替换时注意保留*/
typedef union
{
    /* define the struct bits */
    struct
    {
        uint32_t    ssv : 1; /* [0]  */
        uint32_t    fa : 1; /* [1]  */
        uint32_t    fna : 1; /*2*/
        uint32_t    clean_invalid : 1;/*3*/
        uint32_t    fp : 1;/*4*/
        uint32_t    no : 1;   /*5*/
        uint32_t    ns : 1;   /*6*/
        uint32_t    th : 1;   /*7*/
        uint32_t    th_direct : 2;    /*8..9*/
        uint32_t    type : 2; /*10..11*/
        uint32_t    cmd_type : 3; /*12..14*/
        uint32_t    atomic_type : 2;  /*15..16*/
        uint32_t    stash : 1;    /*17*/
        uint32_t    stash_lpid : 8;   /*18..25*/
        uint32_t    so : 1;   /*26*/
        uint32_t    so_id : 3;    /*27..29*/
        uint32_t    snpattr : 1;  /*30*/
        uint32_t    tlb_lock : 1; /*31*/
    } bits;

    /* define an unsigned member */
    uint32_t    u32;

} u_DMA_AXUSER_REG1;

/* define the union u_DMA_AXUSER_REG2 */
typedef union
{
    /* define the struct bits */
    struct
    {
        uint32_t    axuser_bit64to67      : 4   ; /* [3..0]  */
        uint32_t    reserved_0            : 28  ; /* [31..4]  */
    } bits;

    /* define an unsigned member */
    uint32_t    u32;

} u_DMA_AXUSER_REG2;

/* define the union u_DFX_CNT_CLR_CE */
typedef union
{
    /* define the struct bits */
    struct
    {
        uint32_t    cnt_clr_ce            : 1   ; /* [0]  */
        uint32_t    snap_en               : 1   ; /* [1]  */
        uint32_t    reserved_0            : 30  ; /* [31..2]  */
    } bits;

    /* define an unsigned member */
    uint32_t    u32;

} u_DFX_CNT_CLR_CE;


/* define the union u_DFX_FETURE_EN */
typedef union
{
    /* define the struct bits */
    struct
    {
        uint32_t    reserved_0 : 2; /* [0-1]  */
        uint32_t    timeout_switch : 1; /* [2]  */
        uint32_t    smmu_dvm_sync_switch : 1; /* [3]  */
        uint32_t    smmu_bypass_switch : 1;/* [4]  */
        uint32_t    converge_interrupt_switch : 1;/* [5]  */
        uint32_t    reserved_2 : 26; /* [31..6]  */
    } bits;

    /* define an unsigned member */
    uint32_t    u32;

} u_DFX_FETURE_EN;

//==============================================================================
/* define the global struct */
typedef struct
{
    volatile uint32_t           DFX_PROBE_INFO0;
    volatile uint32_t           DFX_PROBE_INFO1;
    volatile uint32_t           DFX_PROBE_INFO2;
    volatile uint32_t           DFX_PROBE_INFO3;
    volatile uint32_t           DFX_PROBE_INFO4;
    volatile uint32_t           DFX_PROBE_INFO5;
    volatile uint32_t           DFX_PROBE_INFO6;
    volatile uint32_t           DFX_PROBE_INFO7;
    volatile uint32_t           DFX_PROBE_INFO8;
    volatile uint32_t           DFX_PROBE_INFO9;
    volatile uint32_t           DFX_PROBE_INFO10;
    volatile uint32_t           DFX_PROBE_INFO11;
    volatile uint32_t           DFX_PROBE_INFO12;
    volatile uint32_t           DFX_PROBE_INFO13;
    volatile uint32_t           DFX_PROBE_INFO14;
    volatile uint32_t           DFX_PROBE_INFO15;
    volatile uint32_t           DFX_PROBE_INFO16;
    volatile uint32_t           DFX_PROBE_INFO17;
    volatile uint32_t           DFX_PROBE_INFO18;
    volatile uint32_t           DFX_PROBE_INFO19;
    volatile uint32_t           DFX_PROBE_INFO20;
    volatile uint32_t           DFX_PROBE_INFO21;
    volatile uint32_t           DFX_PROBE_INFO22;
    volatile uint32_t           DFX_PROBE_INFO23;
    volatile uint32_t           DFX_CTRL0;
    volatile uint32_t           DFX_CTRL1;
    volatile uint32_t           DFX_CTRL2;
    volatile u_DMA_AXUSER_REG0      DMA_AXUSER_REG0;
    volatile u_DMA_AXUSER_REG1      DMA_AXUSER_REG1;
    volatile u_DMA_AXUSER_REG2      DMA_AXUSER_REG2;
    volatile uint32_t           DMA_ATSREQ_REG;
    volatile uint32_t           DFX_VERSION_ID;
    volatile u_DFX_CNT_CLR_CE       DFX_CNT_CLR_CE;
    volatile uint32_t           DFX_TEST_INJECT;
    volatile uint32_t           DFX_FETURE_EN;

} s_SDMAM_COMMON_REGS_REGS_TYPE;




#endif /* __c_UNION_DEFINE_SDMAM_COMMON_REGS_h__ */
