// ****************************************************************************** 
// Copyright     :  Copyright (C) 2018, Hisilicon Technologies Co. Ltd.
// File name     :  hipciec_ap_dma_reg_c_union_define.h
// Department    :  CAD Development Department
// Author        :  xxx
// Version       :  1.0
// Date          :  2017/10/24
// Description   :  The description of xxx project
// Others        :  Generated automatically by nManager V4.2 
// History       :  xxx 2018/07/13 11:20:03 Create file
// ******************************************************************************

#ifndef __HIPCIEC_AP_DMA_REG_C_UNION_DEFINE_H__
#define __HIPCIEC_AP_DMA_REG_C_UNION_DEFINE_H__

/* Define the union U_DMA_QUEUE_SQ_BASE_L */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    dma_queue_sq_base_l : 32  ; /* [31:0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_DMA_QUEUE_SQ_BASE_L;

/* Define the union U_DMA_QUEUE_SQ_BASE_H */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    dma_queue_sq_base_h : 32  ; /* [31:0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_DMA_QUEUE_SQ_BASE_H;

/* Define the union U_DMA_QUEUE_SQ_DEPTH */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    rsv_0              : 16  ; /* [31:16] */
        unsigned int    dma_queue_sq_depth : 16  ; /* [15:0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_DMA_QUEUE_SQ_DEPTH;

/* Define the union U_DMA_QUEUE_SQ_TAIL_PTR */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    rsv_1                 : 16  ; /* [31:16] */
        unsigned int    dma_queue_sq_tail_ptr : 16  ; /* [15:0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_DMA_QUEUE_SQ_TAIL_PTR;

/* Define the union U_DMA_QUEUE_CQ_BASE_L */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    dma_queue_cq_base_l : 32  ; /* [31:0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_DMA_QUEUE_CQ_BASE_L;

/* Define the union U_DMA_QUEUE_CQ_BASE_H */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    dma_queue_cq_base_h : 32  ; /* [31:0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_DMA_QUEUE_CQ_BASE_H;

/* Define the union U_DMA_QUEUE_CQ_DEPTH */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    rsv_2              : 16  ; /* [31:16] */
        unsigned int    dma_queue_cq_depth : 16  ; /* [15:0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_DMA_QUEUE_CQ_DEPTH;

/* Define the union U_DMA_QUEUE_CQ_HEAD_PTR */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    rsv_3                 : 16  ; /* [31:16] */
        unsigned int    dma_queue_cq_head_ptr : 16  ; /* [15:0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_DMA_QUEUE_CQ_HEAD_PTR;

/* Define the union U_DMA_QUEUE_CTRL0 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    rsv_4                            : 6  ; /* [31:26] */
        unsigned int    dma_queue_cq_full_disable        : 1  ; /* [25] */
        unsigned int    dma_queue_sqcq_drct_sel          : 1  ; /* [24] */
        unsigned int    dma_queue_remote_err_done_int_en : 1  ; /* [23] */
        unsigned int    dma_queue_local_err_done_int_en  : 1  ; /* [22] */
        unsigned int    dma_queue_cq_mrg_time            : 2  ; /* [21:20] */
        unsigned int    dma_queue_cq_mrg_en              : 1  ; /* [19] */
        unsigned int    rsv_5                            : 1  ; /* [18] */
        unsigned int    rsv_6                            : 1  ; /* [17] */
        unsigned int    rsv_7                            : 1  ; /* [16] */
        unsigned int    dma_queue_arb_weight             : 8  ; /* [15:8] */
        unsigned int    rsv_8                            : 3  ; /* [7:5] */
        unsigned int    dma_queue_pause                  : 1  ; /* [4] */
        unsigned int    rsv_9                            : 1  ; /* [3] */
        unsigned int    dma_queue_err_abort_en           : 1  ; /* [2] */
        unsigned int    dma_ep_port_sel                  : 1  ; /* [1] */
        unsigned int    dma_queue_en                     : 1  ; /* [0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_DMA_QUEUE_CTRL0;

/* Define the union U_DMA_QUEUE_CTRL1 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    rsv_10               : 30  ; /* [31:2] */
        unsigned int    dma_queue_abort_exit : 1  ; /* [1] */
        unsigned int    dma_queue_reset      : 1  ; /* [0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_DMA_QUEUE_CTRL1;

/* Define the union U_DMA_QUEUE_RSV */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    dma_queue_rsv : 32  ; /* [31:0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_DMA_QUEUE_RSV;

/* Define the union U_DMA_QUEUE_FSM_STS */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    rsv_11                      : 21  ; /* [31:11] */
        unsigned int    dma_queue_sub_fsm_sts       : 3  ; /* [10:8] */
        unsigned int    rsv_12                      : 1  ; /* [7] */
        unsigned int    rsv_13                      : 1  ; /* [6] */
        unsigned int    dma_queue_wait_spd_data_sts : 1  ; /* [5] */
        unsigned int    dma_queue_not_work          : 1  ; /* [4] */
        unsigned int    dma_queue_sts               : 4  ; /* [3:0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_DMA_QUEUE_FSM_STS;

/* Define the union U_DMA_QUEUE_SQ_STS */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    dma_queue_cq_send_cnt     : 7  ; /* [31:25] */
        unsigned int    dma_queue_resp_rec_finish : 1  ; /* [24] */
        unsigned int    dma_queue_sq_pre_num      : 8  ; /* [23:16] */
        unsigned int    dma_queue_sq_head_ptr     : 16  ; /* [15:0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_DMA_QUEUE_SQ_STS;

/* Define the union U_DMA_QUEUE_BYTE_CNT */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    dma_queue_byte_cnt : 32  ; /* [31:0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_DMA_QUEUE_BYTE_CNT;

/* Define the union U_DMA_QUEUE_CQ_TAIL_PTR */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    dma_queue_sqhd        : 16  ; /* [31:16] */
        unsigned int    dma_queue_cq_tail_ptr : 16  ; /* [15:0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_DMA_QUEUE_CQ_TAIL_PTR;

/* Define the union U_DMA_QUEUE_INT_STS */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    rsv_14                  : 17  ; /* [31:15] */
        unsigned int    dma_queue_err13_int_sts : 1  ; /* [14] */
        unsigned int    dma_queue_err12_int_sts : 1  ; /* [13] */
        unsigned int    dma_queue_err11_int_sts : 1  ; /* [12] */
        unsigned int    dma_queue_err10_int_sts : 1  ; /* [11] */
        unsigned int    dma_queue_err09_int_sts : 1  ; /* [10] */
        unsigned int    dma_queue_err08_int_sts : 1  ; /* [9] */
        unsigned int    dma_queue_err07_int_sts : 1  ; /* [8] */
        unsigned int    dma_queue_err06_int_sts : 1  ; /* [7] */
        unsigned int    dma_queue_err05_int_sts : 1  ; /* [6] */
        unsigned int    dma_queue_err04_int_sts : 1  ; /* [5] */
        unsigned int    dma_queue_err03_int_sts : 1  ; /* [4] */
        unsigned int    dma_queue_err02_int_sts : 1  ; /* [3] */
        unsigned int    dma_queue_err01_int_sts : 1  ; /* [2] */
        unsigned int    dma_queue_err00_int_sts : 1  ; /* [1] */
        unsigned int    dma_queue_done_int_sts  : 1  ; /* [0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_DMA_QUEUE_INT_STS;

/* Define the union U_DMA_QUEUE_INT_MSK */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    rsv_15                  : 17  ; /* [31:15] */
        unsigned int    dma_queue_err13_int_msk : 1  ; /* [14] */
        unsigned int    dma_queue_err12_int_msk : 1  ; /* [13] */
        unsigned int    dma_queue_err11_int_msk : 1  ; /* [12] */
        unsigned int    dma_queue_err10_int_msk : 1  ; /* [11] */
        unsigned int    dma_queue_err09_int_msk : 1  ; /* [10] */
        unsigned int    dma_queue_err08_int_msk : 1  ; /* [9] */
        unsigned int    dma_queue_err07_int_msk : 1  ; /* [8] */
        unsigned int    dma_queue_err06_int_msk : 1  ; /* [7] */
        unsigned int    dma_queue_err05_int_msk : 1  ; /* [6] */
        unsigned int    dma_queue_err04_int_msk : 1  ; /* [5] */
        unsigned int    dma_queue_err03_int_msk : 1  ; /* [4] */
        unsigned int    dma_queue_err02_int_msk : 1  ; /* [3] */
        unsigned int    dma_queue_err01_int_msk : 1  ; /* [2] */
        unsigned int    dma_queue_err00_int_msk : 1  ; /* [1] */
        unsigned int    dma_queue_done_int_msk  : 1  ; /* [0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_DMA_QUEUE_INT_MSK;

/* Define the union U_DMA_ERR_INT_NUM6 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    dma_queue_err13_num : 16  ; /* [31:16] */
        unsigned int    dma_queue_err12_num : 16  ; /* [15:0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_DMA_ERR_INT_NUM6;

/* Define the union U_DMA_QUEUE_DESP0 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    dma_queue_curr_desp_0 : 32  ; /* [31:0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_DMA_QUEUE_DESP0;

/* Define the union U_DMA_QUEUE_DESP1 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    dma_queue_curr_desp_1 : 32  ; /* [31:0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_DMA_QUEUE_DESP1;

/* Define the union U_DMA_QUEUE_DESP2 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    dma_queue_curr_desp_2 : 32  ; /* [31:0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_DMA_QUEUE_DESP2;

/* Define the union U_DMA_QUEUE_DESP3 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    dma_queue_curr_desp_3 : 32  ; /* [31:0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_DMA_QUEUE_DESP3;

/* Define the union U_DMA_QUEUE_ERR_ADDR_L */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    dma_queue_err_addr_l : 32  ; /* [31:0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_DMA_QUEUE_ERR_ADDR_L;

/* Define the union U_DMA_QUEUE_ERR_ADDR_H */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    dma_queue_err_addr_h : 32  ; /* [31:0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_DMA_QUEUE_ERR_ADDR_H;

/* Define the union U_DMA_QUEUE_SQ_READ_ERR_PTR */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    rsv_16               : 16  ; /* [31:16] */
        unsigned int    dma_queue_sq_ptr_err : 16  ; /* [15:0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_DMA_QUEUE_SQ_READ_ERR_PTR;

/* Define the union U_DMA_QUEUE_INT_RO */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    rsv_17              : 17  ; /* [31:15] */
        unsigned int    dma_queue_sts_err13 : 1  ; /* [14] */
        unsigned int    dma_queue_sts_err12 : 1  ; /* [13] */
        unsigned int    dma_queue_sts_err11 : 1  ; /* [12] */
        unsigned int    dma_queue_sts_err10 : 1  ; /* [11] */
        unsigned int    dma_queue_sts_err09 : 1  ; /* [10] */
        unsigned int    dma_queue_sts_err08 : 1  ; /* [9] */
        unsigned int    dma_queue_sts_err07 : 1  ; /* [8] */
        unsigned int    dma_queue_sts_err06 : 1  ; /* [7] */
        unsigned int    dma_queue_sts_err05 : 1  ; /* [6] */
        unsigned int    dma_queue_sts_err04 : 1  ; /* [5] */
        unsigned int    dma_queue_sts_err03 : 1  ; /* [4] */
        unsigned int    dma_queue_sts_err02 : 1  ; /* [3] */
        unsigned int    dma_queue_sts_err01 : 1  ; /* [2] */
        unsigned int    dma_queue_sts_err00 : 1  ; /* [1] */
        unsigned int    dma_queue_sts_done  : 1  ; /* [0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_DMA_QUEUE_INT_RO;

/* Define the union U_DMA_QUEUE_INT_SET */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    rsv_18                  : 17  ; /* [31:15] */
        unsigned int    dma_queue_err13_int_set : 1  ; /* [14] */
        unsigned int    dma_queue_err12_int_set : 1  ; /* [13] */
        unsigned int    dma_queue_err11_int_set : 1  ; /* [12] */
        unsigned int    dma_queue_err10_int_set : 1  ; /* [11] */
        unsigned int    dma_queue_err09_int_set : 1  ; /* [10] */
        unsigned int    dma_queue_err08_int_set : 1  ; /* [9] */
        unsigned int    dma_queue_err07_int_set : 1  ; /* [8] */
        unsigned int    dma_queue_err06_int_set : 1  ; /* [7] */
        unsigned int    dma_queue_err05_int_set : 1  ; /* [6] */
        unsigned int    dma_queue_err04_int_set : 1  ; /* [5] */
        unsigned int    dma_queue_err03_int_set : 1  ; /* [4] */
        unsigned int    dma_queue_err02_int_set : 1  ; /* [3] */
        unsigned int    dma_queue_err01_int_set : 1  ; /* [2] */
        unsigned int    dma_queue_err00_int_set : 1  ; /* [1] */
        unsigned int    dma_queue_done_int_set  : 1  ; /* [0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_DMA_QUEUE_INT_SET;

/* Define the union U_DMA_QUEUE_DESP4 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    dma_queue_curr_desp_4 : 32  ; /* [31:0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_DMA_QUEUE_DESP4;

/* Define the union U_DMA_QUEUE_DESP5 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    dma_queue_curr_desp_5 : 32  ; /* [31:0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_DMA_QUEUE_DESP5;

/* Define the union U_DMA_QUEUE_DESP6 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    dma_queue_curr_desp_6 : 32  ; /* [31:0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_DMA_QUEUE_DESP6;

/* Define the union U_DMA_QUEUE_DESP7 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    dma_queue_curr_desp_7 : 32  ; /* [31:0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_DMA_QUEUE_DESP7;

/* Define the union U_DMA_ERR_INT_NUM0 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    dma_queue_err01_num : 16  ; /* [31:16] */
        unsigned int    dma_queue_err00_num : 16  ; /* [15:0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_DMA_ERR_INT_NUM0;

/* Define the union U_DMA_ERR_INT_NUM1 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    dma_queue_err03_num : 16  ; /* [31:16] */
        unsigned int    dma_queue_err02_num : 16  ; /* [15:0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_DMA_ERR_INT_NUM1;

/* Define the union U_DMA_ERR_INT_NUM2 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    dma_queue_err05_num : 16  ; /* [31:16] */
        unsigned int    dma_queue_err04_num : 16  ; /* [15:0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_DMA_ERR_INT_NUM2;

/* Define the union U_DMA_ERR_INT_NUM3 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    dma_queue_err07_num : 16  ; /* [31:16] */
        unsigned int    dma_queue_err06_num : 16  ; /* [15:0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_DMA_ERR_INT_NUM3;

/* Define the union U_DMA_ERR_INT_NUM4 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    dma_queue_err09_num : 16  ; /* [31:16] */
        unsigned int    dma_queue_err08_num : 16  ; /* [15:0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_DMA_ERR_INT_NUM4;

/* Define the union U_DMA_ERR_INT_NUM5 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    dma_queue_err11_num : 16  ; /* [31:16] */
        unsigned int    dma_queue_err10_num : 16  ; /* [15:0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_DMA_ERR_INT_NUM5;

/* Define the union U_DMA_QUEUE_CTRL2 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    rsv_19               : 22  ; /* [31:10] */
        unsigned int    dma_queue_rmt_vf_cfg : 7  ; /* [9:3] */
        unsigned int    dma_queue_rmt_pf_cfg : 3  ; /* [2:0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_DMA_QUEUE_CTRL2;

/* Define the union U_DONE_INT_MERGE */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    rsv_20                   : 15  ; /* [31:17] */
        unsigned int    dma_done_int_merge_en    : 1  ; /* [16] */
        unsigned int    dma_queue_int_merge_time : 16  ; /* [15:0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_DONE_INT_MERGE;

/* Define the union U_DMA_QUEUE_SQ_STS2 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    rsv_21                   : 26  ; /* [31:6] */
        unsigned int    dma_queue_barrier_rd_wst : 1  ; /* [5] */
        unsigned int    dma_queue_barrier_wd_wst : 1  ; /* [4] */
        unsigned int    dma_queue_sq_bd_allow    : 1  ; /* [3] */
        unsigned int    dma_queue_sq_req_allow   : 1  ; /* [2] */
        unsigned int    dma_queue_sq_clean_ot    : 1  ; /* [1] */
        unsigned int    dma_queue_sq_clean       : 1  ; /* [0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_DMA_QUEUE_SQ_STS2;

/* Define the union U_DMA_ECC */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    rsv_22          : 30  ; /* [31:2] */
        unsigned int    ecc_2bit_inject : 1  ; /* [1] */
        unsigned int    ecc_1bit_inject : 1  ; /* [0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_DMA_ECC;

/* Define the union U_DMA_ECC_ERR_ADDR */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    ecc_2bit_err_addr : 16  ; /* [31:16] */
        unsigned int    ecc_1bit_err_addr : 16  ; /* [15:0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_DMA_ECC_ERR_ADDR;

/* Define the union U_DMA_ECC_ECC_CNT */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    ecc_2bit_cnt : 16  ; /* [31:16] */
        unsigned int    ecc_1bit_cnt : 16  ; /* [15:0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_DMA_ECC_ECC_CNT;

/* Define the union U_DMA_EP_INT_SET */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    rsv_23                     : 28  ; /* [31:4] */
        unsigned int    remote_cpl_tag_err_int_set : 1  ; /* [3] */
        unsigned int    local_cpl_tag_err_int_set  : 1  ; /* [2] */
        unsigned int    ecc_2bit_err_int_set       : 1  ; /* [1] */
        unsigned int    ecc_1bit_err_int_set       : 1  ; /* [0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_DMA_EP_INT_SET;

/* Define the union U_DMA_EP_INT_MSK */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    rsv_24                     : 26  ; /* [31:6] */
        unsigned int    rsv_25                     : 1  ; /* [5] */
        unsigned int    rsv_26                     : 1  ; /* [4] */
        unsigned int    remote_cpl_tag_err_int_msk : 1  ; /* [3] */
        unsigned int    local_cpl_tag_err_int_msk  : 1  ; /* [2] */
        unsigned int    ecc_2bit_err_int_msk       : 1  ; /* [1] */
        unsigned int    ecc_1bit_err_int_msk       : 1  ; /* [0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_DMA_EP_INT_MSK;

/* Define the union U_DMA_EP_INT */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    rsv_27                 : 26  ; /* [31:6] */
        unsigned int    rsv_28                 : 1  ; /* [5] */
        unsigned int    rsv_29                 : 1  ; /* [4] */
        unsigned int    remote_cpl_tag_err_int : 1  ; /* [3] */
        unsigned int    local_cpl_tag_err_int  : 1  ; /* [2] */
        unsigned int    ecc_2bit_err_int       : 1  ; /* [1] */
        unsigned int    ecc_1bit_err_int       : 1  ; /* [0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_DMA_EP_INT;

/* Define the union U_DMA_EP_INT_STS */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    rsv_30                : 26  ; /* [31:6] */
        unsigned int    rsv_31                : 1  ; /* [5] */
        unsigned int    rsv_32                : 1  ; /* [4] */
        unsigned int    remote_cpl_tag_err_st : 1  ; /* [3] */
        unsigned int    local_cpl_tag_err_st  : 1  ; /* [2] */
        unsigned int    ecc_2bit_err_st       : 1  ; /* [1] */
        unsigned int    ecc_1bit_err_st       : 1  ; /* [0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_DMA_EP_INT_STS;

/* Define the union U_COMMON_AND_CH_ERR_STS */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    rsv_33             : 15  ; /* [31:17] */
        unsigned int    dma_sram_init_done : 1  ; /* [16] */
        unsigned int    rsv_34             : 14  ; /* [15:2] */
        unsigned int    dma_queue_err_sts  : 1  ; /* [1] */
        unsigned int    cm_err_sts         : 1  ; /* [0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_COMMON_AND_CH_ERR_STS;

/* Define the union U_ATOMIC_CTRL */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    rsv_35               : 30  ; /* [31:2] */
        unsigned int    ep1_atomic_data_ctrl : 1  ; /* [1] */
        unsigned int    ep0_atomic_data_ctrl : 1  ; /* [0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ATOMIC_CTRL;

/* Define the union U_EP0_ATOMIC_HEADER_0 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    ep0_atomic_header_0 : 32  ; /* [31:0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_EP0_ATOMIC_HEADER_0;

/* Define the union U_EP0_ATOMIC_HEADER_1 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    ep0_atomic_header_1 : 32  ; /* [31:0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_EP0_ATOMIC_HEADER_1;

/* Define the union U_EP0_ATOMIC_HEADER_2 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    ep0_atomic_header_2 : 32  ; /* [31:0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_EP0_ATOMIC_HEADER_2;

/* Define the union U_EP0_ATOMIC_HEADER_3 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    ep0_atomic_header_3 : 32  ; /* [31:0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_EP0_ATOMIC_HEADER_3;

/* Define the union U_EP0_ATOMIC_DATA_0 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    ep0_atomic_data_0 : 32  ; /* [31:0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_EP0_ATOMIC_DATA_0;

/* Define the union U_EP0_ATOMIC_DATA_1 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    ep0_atomic_data_1 : 32  ; /* [31:0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_EP0_ATOMIC_DATA_1;

/* Define the union U_EP0_ATOMIC_DATA_2 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    ep0_atomic_data_2 : 32  ; /* [31:0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_EP0_ATOMIC_DATA_2;

/* Define the union U_EP0_ATOMIC_DATA_3 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    ep0_atomic_data_3 : 32  ; /* [31:0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_EP0_ATOMIC_DATA_3;

/* Define the union U_EP0_ATOMIC_DATA_4 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    ep0_atomic_data_4 : 32  ; /* [31:0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_EP0_ATOMIC_DATA_4;

/* Define the union U_EP0_ATOMIC_DATA_5 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    ep0_atomic_data_5 : 32  ; /* [31:0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_EP0_ATOMIC_DATA_5;

/* Define the union U_EP0_ATOMIC_DATA_6 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    ep0_atomic_data_6 : 32  ; /* [31:0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_EP0_ATOMIC_DATA_6;

/* Define the union U_EP0_ATOMIC_DATA_7 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    ep0_atomic_data_7 : 32  ; /* [31:0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_EP0_ATOMIC_DATA_7;

/* Define the union U_EP0_ATOMIC_RESP_DATA_0 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    ep0_atomic_resp_data_0 : 32  ; /* [31:0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_EP0_ATOMIC_RESP_DATA_0;

/* Define the union U_EP0_ATOMIC_RESP_DATA_1 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    ep0_atomic_resp_data_1 : 32  ; /* [31:0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_EP0_ATOMIC_RESP_DATA_1;

/* Define the union U_EP0_ATOMIC_RESP_DATA_2 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    ep0_atomic_resp_data_2 : 32  ; /* [31:0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_EP0_ATOMIC_RESP_DATA_2;

/* Define the union U_EP0_ATOMIC_RESP_DATA_3 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    ep0_atomic_resp_data_3 : 32  ; /* [31:0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_EP0_ATOMIC_RESP_DATA_3;

/* Define the union U_EP0_ATOMIC_RESP_DATA_4 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    ep0_atomic_resp_data_4 : 32  ; /* [31:0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_EP0_ATOMIC_RESP_DATA_4;

/* Define the union U_EP0_ATOMIC_RESP_DATA_5 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    ep0_atomic_resp_data_5 : 32  ; /* [31:0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_EP0_ATOMIC_RESP_DATA_5;

/* Define the union U_EP0_ATOMIC_RESP_DATA_6 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    ep0_atomic_resp_data_6 : 32  ; /* [31:0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_EP0_ATOMIC_RESP_DATA_6;

/* Define the union U_EP0_ATOMIC_RESP_DATA_7 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    ep0_atomic_resp_data_7 : 32  ; /* [31:0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_EP0_ATOMIC_RESP_DATA_7;

/* Define the union U_EP1_ATOMIC_HEADER_0 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    ep1_atomic_header_0 : 32  ; /* [31:0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_EP1_ATOMIC_HEADER_0;

/* Define the union U_EP1_ATOMIC_HEADER_1 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    ep1_atomic_header_1 : 32  ; /* [31:0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_EP1_ATOMIC_HEADER_1;

/* Define the union U_EP1_ATOMIC_HEADER_2 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    ep1_atomic_header_2 : 32  ; /* [31:0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_EP1_ATOMIC_HEADER_2;

/* Define the union U_EP1_ATOMIC_HEADER_3 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    ep1_atomic_header_3 : 32  ; /* [31:0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_EP1_ATOMIC_HEADER_3;

/* Define the union U_EP1_ATOMIC_DATA_0 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    ep1_atomic_data_0 : 32  ; /* [31:0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_EP1_ATOMIC_DATA_0;

/* Define the union U_EP1_ATOMIC_DATA_1 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    ep1_atomic_data_1 : 32  ; /* [31:0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_EP1_ATOMIC_DATA_1;

/* Define the union U_EP1_ATOMIC_DATA_2 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    ep1_atomic_data_2 : 32  ; /* [31:0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_EP1_ATOMIC_DATA_2;

/* Define the union U_EP1_ATOMIC_DATA_3 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    ep1_atomic_data_3 : 32  ; /* [31:0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_EP1_ATOMIC_DATA_3;

/* Define the union U_EP1_ATOMIC_DATA_4 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    ep1_atomic_data_4 : 32  ; /* [31:0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_EP1_ATOMIC_DATA_4;

/* Define the union U_EP1_ATOMIC_DATA_5 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    ep1_atomic_data_5 : 32  ; /* [31:0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_EP1_ATOMIC_DATA_5;

/* Define the union U_EP1_ATOMIC_DATA_6 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    ep1_atomic_data_6 : 32  ; /* [31:0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_EP1_ATOMIC_DATA_6;

/* Define the union U_EP1_ATOMIC_DATA_7 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    ep1_atomic_data_7 : 32  ; /* [31:0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_EP1_ATOMIC_DATA_7;

/* Define the union U_EP1_ATOMIC_RESP_DATA_0 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    ep1_atomic_resp_data_0 : 32  ; /* [31:0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_EP1_ATOMIC_RESP_DATA_0;

/* Define the union U_EP1_ATOMIC_RESP_DATA_1 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    ep1_atomic_resp_data_1 : 32  ; /* [31:0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_EP1_ATOMIC_RESP_DATA_1;

/* Define the union U_EP1_ATOMIC_RESP_DATA_2 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    ep1_atomic_resp_data_2 : 32  ; /* [31:0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_EP1_ATOMIC_RESP_DATA_2;

/* Define the union U_EP1_ATOMIC_RESP_DATA_3 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    ep1_atomic_resp_data_3 : 32  ; /* [31:0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_EP1_ATOMIC_RESP_DATA_3;

/* Define the union U_EP1_ATOMIC_RESP_DATA_4 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    ep1_atomic_resp_data_4 : 32  ; /* [31:0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_EP1_ATOMIC_RESP_DATA_4;

/* Define the union U_EP1_ATOMIC_RESP_DATA_5 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    ep1_atomic_resp_data_5 : 32  ; /* [31:0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_EP1_ATOMIC_RESP_DATA_5;

/* Define the union U_EP1_ATOMIC_RESP_DATA_6 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    ep1_atomic_resp_data_6 : 32  ; /* [31:0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_EP1_ATOMIC_RESP_DATA_6;

/* Define the union U_EP1_ATOMIC_RESP_DATA_7 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    ep1_atomic_resp_data_7 : 32  ; /* [31:0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_EP1_ATOMIC_RESP_DATA_7;

/* Define the union U_ATOMIC_RESP_DATA_ST */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    rsv_36                   : 28  ; /* [31:4] */
        unsigned int    ep1_atomic_resp_data_err : 1  ; /* [3] */
        unsigned int    ep0_atomic_resp_data_err : 1  ; /* [2] */
        unsigned int    ep1_atomic_resp_data_vld : 1  ; /* [1] */
        unsigned int    ep0_atomic_resp_data_vld : 1  ; /* [0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ATOMIC_RESP_DATA_ST;

/* Define the union U_LOCAL_CPL_ID_STS_0 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    local_cpl_tag_id_status_0 : 32  ; /* [31:0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_LOCAL_CPL_ID_STS_0;

/* Define the union U_LOCAL_CPL_ID_STS_1 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    local_cpl_tag_id_status_1 : 32  ; /* [31:0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_LOCAL_CPL_ID_STS_1;

/* Define the union U_LOCAL_CPL_ID_STS_2 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    local_cpl_tag_id_status_2 : 32  ; /* [31:0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_LOCAL_CPL_ID_STS_2;

/* Define the union U_LOCAL_CPL_ID_STS_3 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    local_cpl_tag_id_status_3 : 32  ; /* [31:0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_LOCAL_CPL_ID_STS_3;

/* Define the union U_EP1_LOCAL_CPL_ID_STS_0 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    rsv_37 : 32  ; /* [31:0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_EP1_LOCAL_CPL_ID_STS_0;

/* Define the union U_EP1_LOCAL_CPL_ID_STS_1 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    rsv_38 : 32  ; /* [31:0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_EP1_LOCAL_CPL_ID_STS_1;

/* Define the union U_EP1_LOCAL_CPL_ID_STS_2 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    rsv_39 : 32  ; /* [31:0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_EP1_LOCAL_CPL_ID_STS_2;

/* Define the union U_EP1_LOCAL_CPL_ID_STS_3 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    rsv_40 : 32  ; /* [31:0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_EP1_LOCAL_CPL_ID_STS_3;

/* Define the union U_REMOTE_CPL_ID_STS_0 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    remote_cpl_tag_id_status_0 : 32  ; /* [31:0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_REMOTE_CPL_ID_STS_0;

/* Define the union U_REMOTE_CPL_ID_STS_1 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    remote_cpl_tag_id_status_1 : 32  ; /* [31:0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_REMOTE_CPL_ID_STS_1;

/* Define the union U_REMOTE_CPL_ID_STS_2 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    remote_cpl_tag_id_status_2 : 32  ; /* [31:0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_REMOTE_CPL_ID_STS_2;

/* Define the union U_REMOTE_CPL_ID_STS_3 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    remote_cpl_tag_id_status_3 : 32  ; /* [31:0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_REMOTE_CPL_ID_STS_3;

/* Define the union U_EP1_REMOTE_CPL_ID_STS_0 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    rsv_41 : 32  ; /* [31:0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_EP1_REMOTE_CPL_ID_STS_0;

/* Define the union U_EP1_REMOTE_CPL_ID_STS_1 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    rsv_42 : 32  ; /* [31:0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_EP1_REMOTE_CPL_ID_STS_1;

/* Define the union U_EP1_REMOTE_CPL_ID_STS_2 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    rsv_43 : 32  ; /* [31:0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_EP1_REMOTE_CPL_ID_STS_2;

/* Define the union U_EP1_REMOTE_CPL_ID_STS_3 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    rsv_44 : 32  ; /* [31:0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_EP1_REMOTE_CPL_ID_STS_3;

/* Define the union U_REMOTE_P_ID_STS_0 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    remote_p_tag_id_status_0 : 32  ; /* [31:0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_REMOTE_P_ID_STS_0;

/* Define the union U_REMOTE_P_ID_STS_1 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    remote_p_tag_id_status_1 : 32  ; /* [31:0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_REMOTE_P_ID_STS_1;

/* Define the union U_REMOTE_P_ID_STS_2 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    remote_p_tag_id_status_2 : 32  ; /* [31:0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_REMOTE_P_ID_STS_2;

/* Define the union U_REMOTE_P_ID_STS_3 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    remote_p_tag_id_status_3 : 32  ; /* [31:0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_REMOTE_P_ID_STS_3;

/* Define the union U_EP1_REMOTE_P_ID_STS_0 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    rsv_45 : 32  ; /* [31:0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_EP1_REMOTE_P_ID_STS_0;

/* Define the union U_EP1_REMOTE_P_ID_STS_1 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    rsv_46 : 32  ; /* [31:0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_EP1_REMOTE_P_ID_STS_1;

/* Define the union U_EP1_REMOTE_P_ID_STS_2 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    rsv_47 : 32  ; /* [31:0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_EP1_REMOTE_P_ID_STS_2;

/* Define the union U_EP1_REMOTE_P_ID_STS_3 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    rsv_48 : 32  ; /* [31:0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_EP1_REMOTE_P_ID_STS_3;

/* Define the union U_LOCAL_TLP_P_ST_CFG */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    rsv_49                 : 12  ; /* [31:20] */
        unsigned int    local_ep1_tlp_p_ph_cfg : 2  ; /* [19:18] */
        unsigned int    local_ep0_tlp_p_ph_cfg : 2  ; /* [17:16] */
        unsigned int    local_ep1_tlp_p_st_cfg : 8  ; /* [15:8] */
        unsigned int    local_ep0_tlp_p_st_cfg : 8  ; /* [7:0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_LOCAL_TLP_P_ST_CFG;

/* Define the union U_EP0_ATOMIC_PREFIX_INF */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    rsv_50               : 9  ; /* [31:23] */
        unsigned int    ep0_atomic_prfx_dwen : 1  ; /* [22] */
        unsigned int    ep0_atomic_prfx_pmr  : 1  ; /* [21] */
        unsigned int    ep0_atomic_prfx_er   : 1  ; /* [20] */
        unsigned int    ep0_atomic_pasid     : 20  ; /* [19:0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_EP0_ATOMIC_PREFIX_INF;

/* Define the union U_EP1_ATOMIC_PREFIX_INF */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    rsv_51               : 9  ; /* [31:23] */
        unsigned int    ep1_atomic_prfx_dwen : 1  ; /* [22] */
        unsigned int    ep1_atomic_prfx_pmr  : 1  ; /* [21] */
        unsigned int    ep1_atomic_prfx_er   : 1  ; /* [20] */
        unsigned int    ep1_atomic_pasid     : 20  ; /* [19:0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_EP1_ATOMIC_PREFIX_INF;

/* Define the union U_ATOMIC_PF_VF */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    rsv_52        : 10  ; /* [31:22] */
        unsigned int    ep1_atomic_pf : 3  ; /* [21:19] */
        unsigned int    ep1_atomic_vf : 8  ; /* [18:11] */
        unsigned int    ep0_atomic_pf : 3  ; /* [10:8] */
        unsigned int    ep0_atomic_vf : 8  ; /* [7:0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ATOMIC_PF_VF;

/* Define the union U_PORT_IDLE_STS */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    rsv_53       : 30  ; /* [31:2] */
        unsigned int    ep1_sts_idle : 1  ; /* [1] */
        unsigned int    ep0_sts_idle : 1  ; /* [0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_PORT_IDLE_STS;

/* Define the union U_REMOTE_TLP_NUM */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    remote_np_cnt : 16  ; /* [31:16] */
        unsigned int    remote_p_cnt  : 16  ; /* [15:0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_REMOTE_TLP_NUM;

/* Define the union U_LOCAL_TLP_NUM */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    local_np_cnt : 16  ; /* [31:16] */
        unsigned int    local_p_cnt  : 16  ; /* [15:0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_LOCAL_TLP_NUM;

/* Define the union U_EP1_REMOTE_TLP_NUM */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    rsv_54 : 16  ; /* [31:16] */
        unsigned int    rsv_55 : 16  ; /* [15:0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_EP1_REMOTE_TLP_NUM;

/* Define the union U_EP1_LOCAL_TLP_NUM */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    rsv_56 : 16  ; /* [31:16] */
        unsigned int    rsv_57 : 16  ; /* [15:0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_EP1_LOCAL_TLP_NUM;

/* Define the union U_SQCQ_TLP_NUM */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    sq_np_cnt : 16  ; /* [31:16] */
        unsigned int    cq_p_cnt  : 16  ; /* [15:0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_SQCQ_TLP_NUM;

/* Define the union U_CPL_NUM */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    remote_cpl_cnt : 16  ; /* [31:16] */
        unsigned int    local_cpl_cnt  : 16  ; /* [15:0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_CPL_NUM;

/* Define the union U_EP1_CPL_NUM */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    rsv_58 : 16  ; /* [31:16] */
        unsigned int    rsv_59 : 16  ; /* [15:0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_EP1_CPL_NUM;

/* Define the union U_INF_BACK_PRESS_STS */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    rsv_60          : 17  ; /* [31:15] */
        unsigned int    rsv_61          : 1  ; /* [14] */
        unsigned int    rsv_62          : 1  ; /* [13] */
        unsigned int    rsv_63          : 1  ; /* [12] */
        unsigned int    rsv_64          : 1  ; /* [11] */
        unsigned int    rsv_65          : 1  ; /* [10] */
        unsigned int    rsv_66          : 1  ; /* [9] */
        unsigned int    remote_np_bkpr  : 1  ; /* [8] */
        unsigned int    remote_p_bkpr   : 1  ; /* [7] */
        unsigned int    remote_cpl_bkpr : 1  ; /* [6] */
        unsigned int    local_np_bkpr   : 1  ; /* [5] */
        unsigned int    local_p_bkpr    : 1  ; /* [4] */
        unsigned int    local_cpl_bkpr  : 1  ; /* [3] */
        unsigned int    sq_np_bkpr      : 1  ; /* [2] */
        unsigned int    sq_cpl_bkpr     : 1  ; /* [1] */
        unsigned int    cq_p_bkpr       : 1  ; /* [0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_INF_BACK_PRESS_STS;

/* Define the union U_PORT_LINKDOWN_EVENT */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    rsv_67           : 16  ; /* [31:16] */
        unsigned int    ep1_linkdown_cnt : 8  ; /* [15:8] */
        unsigned int    ep0_linkdown_cnt : 8  ; /* [7:0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_PORT_LINKDOWN_EVENT;

/* Define the union U_DMA_MODE_SEL */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    rsv_68          : 31  ; /* [31:1] */
        unsigned int    dma_rc_mode_sel : 1  ; /* [0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_DMA_MODE_SEL;

/* Define the union U_DMA_GLOBAL_CTRL */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    rsv_69             : 12  ; /* [31:20] */
        unsigned int    mrrs_in_rc_mode    : 3  ; /* [19:17] */
        unsigned int    ep1_link_down_mask : 1  ; /* [16] */
        unsigned int    ep0_link_down_mask : 1  ; /* [15] */
        unsigned int    mps_in_rc_mode     : 3  ; /* [14:12] */
        unsigned int    clk_frequency      : 12  ; /* [11:0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_DMA_GLOBAL_CTRL;

/* Define the union U_DMA_CH_RAS_LEVEL */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    rsv_70                    : 17  ; /* [31:15] */
        unsigned int    dma_queue_sts_err13_level : 1  ; /* [14] */
        unsigned int    dma_queue_sts_err12_level : 1  ; /* [13] */
        unsigned int    dma_queue_sts_err11_level : 1  ; /* [12] */
        unsigned int    dma_queue_sts_err10_level : 1  ; /* [11] */
        unsigned int    dma_queue_sts_err09_level : 1  ; /* [10] */
        unsigned int    dma_queue_sts_err08_level : 1  ; /* [9] */
        unsigned int    dma_queue_sts_err07_level : 1  ; /* [8] */
        unsigned int    dma_queue_sts_err06_level : 1  ; /* [7] */
        unsigned int    dma_queue_sts_err05_level : 1  ; /* [6] */
        unsigned int    dma_queue_sts_err04_level : 1  ; /* [5] */
        unsigned int    dma_queue_sts_err03_level : 1  ; /* [4] */
        unsigned int    dma_queue_sts_err02_level : 1  ; /* [3] */
        unsigned int    dma_queue_sts_err01_level : 1  ; /* [2] */
        unsigned int    dma_queue_sts_err00_level : 1  ; /* [1] */
        unsigned int    rsv_71                    : 1  ; /* [0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_DMA_CH_RAS_LEVEL;

/* Define the union U_DMA_CM_RAS_LEVEL */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    rsv_72                      : 28  ; /* [31:4] */
        unsigned int    remote_cpl_tag_err_st_level : 1  ; /* [3] */
        unsigned int    local_cpl_tag_err_st_level  : 1  ; /* [2] */
        unsigned int    ecc_2bit_err_st_level       : 1  ; /* [1] */
        unsigned int    ecc_1bit_err_st_level       : 1  ; /* [0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_DMA_CM_RAS_LEVEL;

/* Define the union U_DMA_INT_MODE_SEL */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    rsv_73                : 30  ; /* [31:2] */
        unsigned int    int_mode_sel_abnormal : 1  ; /* [1] */
        unsigned int    int_mode_sel_normal   : 1  ; /* [0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_DMA_INT_MODE_SEL;

/* Define the union U_DMA_CH_ERR_STS */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    ch31_err : 1  ; /* [31] */
        unsigned int    ch30_err : 1  ; /* [30] */
        unsigned int    ch29_err : 1  ; /* [29] */
        unsigned int    ch28_err : 1  ; /* [28] */
        unsigned int    ch27_err : 1  ; /* [27] */
        unsigned int    ch26_err : 1  ; /* [26] */
        unsigned int    ch25_err : 1  ; /* [25] */
        unsigned int    ch24_err : 1  ; /* [24] */
        unsigned int    ch23_err : 1  ; /* [23] */
        unsigned int    ch22_err : 1  ; /* [22] */
        unsigned int    ch21_err : 1  ; /* [21] */
        unsigned int    ch20_err : 1  ; /* [20] */
        unsigned int    ch19_err : 1  ; /* [19] */
        unsigned int    ch18_err : 1  ; /* [18] */
        unsigned int    ch17_err : 1  ; /* [17] */
        unsigned int    ch16_err : 1  ; /* [16] */
        unsigned int    ch15_err : 1  ; /* [15] */
        unsigned int    ch14_err : 1  ; /* [14] */
        unsigned int    ch13_err : 1  ; /* [13] */
        unsigned int    ch12_err : 1  ; /* [12] */
        unsigned int    ch11_err : 1  ; /* [11] */
        unsigned int    ch10_err : 1  ; /* [10] */
        unsigned int    ch9_err  : 1  ; /* [9] */
        unsigned int    ch8_err  : 1  ; /* [8] */
        unsigned int    ch7_err  : 1  ; /* [7] */
        unsigned int    ch6_err  : 1  ; /* [6] */
        unsigned int    ch5_err  : 1  ; /* [5] */
        unsigned int    ch4_err  : 1  ; /* [4] */
        unsigned int    ch3_err  : 1  ; /* [3] */
        unsigned int    ch2_err  : 1  ; /* [2] */
        unsigned int    ch1_err  : 1  ; /* [1] */
        unsigned int    ch0_err  : 1  ; /* [0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_DMA_CH_ERR_STS;

/* Define the union U_DMA_CH_DONE_STS */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    ch31_done : 1  ; /* [31] */
        unsigned int    ch30_done : 1  ; /* [30] */
        unsigned int    ch29_done : 1  ; /* [29] */
        unsigned int    ch28_done : 1  ; /* [28] */
        unsigned int    ch27_done : 1  ; /* [27] */
        unsigned int    ch26_done : 1  ; /* [26] */
        unsigned int    ch25_done : 1  ; /* [25] */
        unsigned int    ch24_done : 1  ; /* [24] */
        unsigned int    ch23_done : 1  ; /* [23] */
        unsigned int    ch22_done : 1  ; /* [22] */
        unsigned int    ch21_done : 1  ; /* [21] */
        unsigned int    ch20_done : 1  ; /* [20] */
        unsigned int    ch19_done : 1  ; /* [19] */
        unsigned int    ch18_done : 1  ; /* [18] */
        unsigned int    ch17_done : 1  ; /* [17] */
        unsigned int    ch16_done : 1  ; /* [16] */
        unsigned int    ch15_done : 1  ; /* [15] */
        unsigned int    ch14_done : 1  ; /* [14] */
        unsigned int    ch13_done : 1  ; /* [13] */
        unsigned int    ch12_done : 1  ; /* [12] */
        unsigned int    ch11_done : 1  ; /* [11] */
        unsigned int    ch10_done : 1  ; /* [10] */
        unsigned int    ch9_done  : 1  ; /* [9] */
        unsigned int    ch8_done  : 1  ; /* [8] */
        unsigned int    ch7_done  : 1  ; /* [7] */
        unsigned int    ch6_done  : 1  ; /* [6] */
        unsigned int    ch5_done  : 1  ; /* [5] */
        unsigned int    ch4_done  : 1  ; /* [4] */
        unsigned int    ch3_done  : 1  ; /* [3] */
        unsigned int    ch2_done  : 1  ; /* [2] */
        unsigned int    ch1_done  : 1  ; /* [1] */
        unsigned int    ch0_done  : 1  ; /* [0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_DMA_CH_DONE_STS;

/* Define the union U_DMA_SQ_TAG_STS_0 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    dma_sq_tag_sts_0 : 32  ; /* [31:0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_DMA_SQ_TAG_STS_0;

/* Define the union U_DMA_SQ_TAG_STS_1 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    dma_sq_tag_sts_1 : 32  ; /* [31:0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_DMA_SQ_TAG_STS_1;

/* Define the union U_DMA_SQ_TAG_STS_2 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    dma_sq_tag_sts_2 : 32  ; /* [31:0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_DMA_SQ_TAG_STS_2;

/* Define the union U_DMA_SQ_TAG_STS_3 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    dma_sq_tag_sts_3 : 32  ; /* [31:0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_DMA_SQ_TAG_STS_3;

/* Define the union U_LOCAL_P_ID_STS_0 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    local_p_tag_id_status_0 : 32  ; /* [31:0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_LOCAL_P_ID_STS_0;

/* Define the union U_LOCAL_P_ID_STS_1 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    local_p_tag_id_status_1 : 32  ; /* [31:0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_LOCAL_P_ID_STS_1;

/* Define the union U_LOCAL_P_ID_STS_2 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    local_p_tag_id_status_2 : 32  ; /* [31:0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_LOCAL_P_ID_STS_2;

/* Define the union U_LOCAL_P_ID_STS_3 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    local_p_tag_id_status_3 : 32  ; /* [31:0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_LOCAL_P_ID_STS_3;

/* Define the union U_EP0_BLOCK_INFO */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    rsv_74            : 30  ; /* [31:2] */
        unsigned int    remote_ep0_p_blk  : 1  ; /* [1] */
        unsigned int    remote_ep0_np_blk : 1  ; /* [0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_EP0_BLOCK_INFO;

/* Define the union U_EP1_BLOCK_INFO */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    rsv_75            : 30  ; /* [31:2] */
        unsigned int    remote_ep1_p_blk  : 1  ; /* [1] */
        unsigned int    remote_ep1_np_blk : 1  ; /* [0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_EP1_BLOCK_INFO;

/* Define the union U_DMA_PREBUFF_INFO_0 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    rsv_76                 : 16  ; /* [31:16] */
        unsigned int    dma_prebuff_entry_used : 16  ; /* [15:0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_DMA_PREBUFF_INFO_0;

/* Define the union U_DMA_CM_TABLE_INFO_0 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    rsv_77                  : 16  ; /* [31:16] */
        unsigned int    dma_cm_table_entry_used : 16  ; /* [15:0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_DMA_CM_TABLE_INFO_0;

/* Define the union U_DMA_CM_CE_RO */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    rsv_78             : 31  ; /* [31:1] */
        unsigned int    ecc_1bit_err_st_ce : 1  ; /* [0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_DMA_CM_CE_RO;

/* Define the union U_DMA_CM_NFE_RO */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    rsv_79                    : 28  ; /* [31:4] */
        unsigned int    remote_cpl_tag_err_st_nfe : 1  ; /* [3] */
        unsigned int    local_cpl_tag_err_st_nfe  : 1  ; /* [2] */
        unsigned int    ecc_2bit_err_st_nfe       : 1  ; /* [1] */
        unsigned int    ecc_1bit_err_st_nfe       : 1  ; /* [0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_DMA_CM_NFE_RO;

/* Define the union U_DMA_CM_FE_RO */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    rsv_80                   : 28  ; /* [31:4] */
        unsigned int    remote_cpl_tag_err_st_fe : 1  ; /* [3] */
        unsigned int    local_cpl_tag_err_st_fe  : 1  ; /* [2] */
        unsigned int    ecc_2bit_err_st_fe       : 1  ; /* [1] */
        unsigned int    rsv_81                   : 1  ; /* [0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_DMA_CM_FE_RO;

/* Define the union U_DMA_CH_EP0_CE_RO */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    rsv_82                     : 20  ; /* [31:12] */
        unsigned int    dma_queue_sts_err10_ep0_ce : 1  ; /* [11] */
        unsigned int    rsv_83                     : 6  ; /* [10:5] */
        unsigned int    dma_queue_sts_err03_ep0_ce : 1  ; /* [4] */
        unsigned int    dma_queue_sts_err02_ep0_ce : 1  ; /* [3] */
        unsigned int    dma_queue_sts_err01_ep0_ce : 1  ; /* [2] */
        unsigned int    dma_queue_sts_err00_ep0_ce : 1  ; /* [1] */
        unsigned int    rsv_84                     : 1  ; /* [0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_DMA_CH_EP0_CE_RO;

/* Define the union U_DMA_CH_EP0_NFE_RO */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    rsv_85                      : 17  ; /* [31:15] */
        unsigned int    dma_queue_sts_err13_ep0_nfe : 1  ; /* [14] */
        unsigned int    dma_queue_sts_err12_ep0_nfe : 1  ; /* [13] */
        unsigned int    dma_queue_sts_err11_ep0_nfe : 1  ; /* [12] */
        unsigned int    dma_queue_sts_err10_ep0_nfe : 1  ; /* [11] */
        unsigned int    dma_queue_sts_err09_ep0_nfe : 1  ; /* [10] */
        unsigned int    dma_queue_sts_err08_ep0_nfe : 1  ; /* [9] */
        unsigned int    dma_queue_sts_err07_ep0_nfe : 1  ; /* [8] */
        unsigned int    dma_queue_sts_err06_ep0_nfe : 1  ; /* [7] */
        unsigned int    dma_queue_sts_err05_ep0_nfe : 1  ; /* [6] */
        unsigned int    dma_queue_sts_err04_ep0_nfe : 1  ; /* [5] */
        unsigned int    dma_queue_sts_err03_ep0_nfe : 1  ; /* [4] */
        unsigned int    dma_queue_sts_err02_ep0_nfe : 1  ; /* [3] */
        unsigned int    dma_queue_sts_err01_ep0_nfe : 1  ; /* [2] */
        unsigned int    dma_queue_sts_err00_ep0_nfe : 1  ; /* [1] */
        unsigned int    rsv_86                      : 1  ; /* [0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_DMA_CH_EP0_NFE_RO;

/* Define the union U_DMA_CH_EP0_FE_RO */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    rsv_87                     : 17  ; /* [31:15] */
        unsigned int    dma_queue_sts_err13_ep0_fe : 1  ; /* [14] */
        unsigned int    dma_queue_sts_err12_ep0_fe : 1  ; /* [13] */
        unsigned int    dma_queue_sts_err11_ep0_fe : 1  ; /* [12] */
        unsigned int    rsv_88                     : 1  ; /* [11] */
        unsigned int    dma_queue_sts_err09_ep0_fe : 1  ; /* [10] */
        unsigned int    dma_queue_sts_err08_ep0_fe : 1  ; /* [9] */
        unsigned int    dma_queue_sts_err07_ep0_fe : 1  ; /* [8] */
        unsigned int    dma_queue_sts_err06_ep0_fe : 1  ; /* [7] */
        unsigned int    dma_queue_sts_err05_ep0_fe : 1  ; /* [6] */
        unsigned int    dma_queue_sts_err04_ep0_fe : 1  ; /* [5] */
        unsigned int    rsv_89                     : 5  ; /* [4:0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_DMA_CH_EP0_FE_RO;

/* Define the union U_DMA_CH_EP1_CE_RO */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    rsv_90                     : 20  ; /* [31:12] */
        unsigned int    dma_queue_sts_err10_ep1_ce : 1  ; /* [11] */
        unsigned int    rsv_91                     : 6  ; /* [10:5] */
        unsigned int    dma_queue_sts_err03_ep1_ce : 1  ; /* [4] */
        unsigned int    dma_queue_sts_err02_ep1_ce : 1  ; /* [3] */
        unsigned int    dma_queue_sts_err01_ep1_ce : 1  ; /* [2] */
        unsigned int    dma_queue_sts_err00_ep1_ce : 1  ; /* [1] */
        unsigned int    rsv_92                     : 1  ; /* [0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_DMA_CH_EP1_CE_RO;

/* Define the union U_DMA_CH_EP1_NFE_RO */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    rsv_93                      : 17  ; /* [31:15] */
        unsigned int    dma_queue_sts_err13_ep1_nfe : 1  ; /* [14] */
        unsigned int    dma_queue_sts_err12_ep1_nfe : 1  ; /* [13] */
        unsigned int    dma_queue_sts_err11_ep1_nfe : 1  ; /* [12] */
        unsigned int    dma_queue_sts_err10_ep1_nfe : 1  ; /* [11] */
        unsigned int    dma_queue_sts_err09_ep1_nfe : 1  ; /* [10] */
        unsigned int    dma_queue_sts_err08_ep1_nfe : 1  ; /* [9] */
        unsigned int    dma_queue_sts_err07_ep1_nfe : 1  ; /* [8] */
        unsigned int    dma_queue_sts_err06_ep1_nfe : 1  ; /* [7] */
        unsigned int    dma_queue_sts_err05_ep1_nfe : 1  ; /* [6] */
        unsigned int    dma_queue_sts_err04_ep1_nfe : 1  ; /* [5] */
        unsigned int    dma_queue_sts_err03_ep1_nfe : 1  ; /* [4] */
        unsigned int    dma_queue_sts_err02_ep1_nfe : 1  ; /* [3] */
        unsigned int    dma_queue_sts_err01_ep1_nfe : 1  ; /* [2] */
        unsigned int    dma_queue_sts_err00_ep1_nfe : 1  ; /* [1] */
        unsigned int    rsv_94                      : 1  ; /* [0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_DMA_CH_EP1_NFE_RO;

/* Define the union U_DMA_CH_EP1_FE_RO */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    rsv_95                     : 17  ; /* [31:15] */
        unsigned int    dma_queue_sts_err13_ep0_fe : 1  ; /* [14] */
        unsigned int    dma_queue_sts_err12_ep0_fe : 1  ; /* [13] */
        unsigned int    dma_queue_sts_err11_ep0_fe : 1  ; /* [12] */
        unsigned int    rsv_96                     : 1  ; /* [11] */
        unsigned int    dma_queue_sts_err09_ep1_fe : 1  ; /* [10] */
        unsigned int    dma_queue_sts_err08_ep1_fe : 1  ; /* [9] */
        unsigned int    dma_queue_sts_err07_ep1_fe : 1  ; /* [8] */
        unsigned int    dma_queue_sts_err06_ep1_fe : 1  ; /* [7] */
        unsigned int    dma_queue_sts_err05_ep1_fe : 1  ; /* [6] */
        unsigned int    dma_queue_sts_err04_ep1_fe : 1  ; /* [5] */
        unsigned int    rsv_97                     : 5  ; /* [4:0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_DMA_CH_EP1_FE_RO;


//==============================================================================
/* Define the global struct */
typedef struct
{
    volatile U_DMA_QUEUE_SQ_BASE_L       DMA_QUEUE_SQ_BASE_L[32]       ; /* 0 */
    volatile U_DMA_QUEUE_SQ_BASE_H       DMA_QUEUE_SQ_BASE_H[32]       ; /* 4 */
    volatile U_DMA_QUEUE_SQ_DEPTH        DMA_QUEUE_SQ_DEPTH[32]        ; /* 8 */
    volatile U_DMA_QUEUE_SQ_TAIL_PTR     DMA_QUEUE_SQ_TAIL_PTR[32]     ; /* C */
    volatile U_DMA_QUEUE_CQ_BASE_L       DMA_QUEUE_CQ_BASE_L[32]       ; /* 10 */
    volatile U_DMA_QUEUE_CQ_BASE_H       DMA_QUEUE_CQ_BASE_H[32]       ; /* 14 */
    volatile U_DMA_QUEUE_CQ_DEPTH        DMA_QUEUE_CQ_DEPTH[32]        ; /* 18 */
    volatile U_DMA_QUEUE_CQ_HEAD_PTR     DMA_QUEUE_CQ_HEAD_PTR[32]     ; /* 1C */
    volatile U_DMA_QUEUE_CTRL0           DMA_QUEUE_CTRL0[32]           ; /* 20 */
    volatile U_DMA_QUEUE_CTRL1           DMA_QUEUE_CTRL1[32]           ; /* 24 */
    volatile U_DMA_QUEUE_RSV             DMA_QUEUE_RSV[32]             ; /* 28 */
    volatile U_DMA_QUEUE_FSM_STS         DMA_QUEUE_FSM_STS[32]         ; /* 30 */
    volatile U_DMA_QUEUE_SQ_STS          DMA_QUEUE_SQ_STS[32]          ; /* 34 */
    volatile U_DMA_QUEUE_BYTE_CNT        DMA_QUEUE_BYTE_CNT[32]        ; /* 38 */
    volatile U_DMA_QUEUE_CQ_TAIL_PTR     DMA_QUEUE_CQ_TAIL_PTR[32]     ; /* 3C */
    volatile U_DMA_QUEUE_INT_STS         DMA_QUEUE_INT_STS[32]         ; /* 40 */
    volatile U_DMA_QUEUE_INT_MSK         DMA_QUEUE_INT_MSK[32]         ; /* 44 */
    volatile U_DMA_ERR_INT_NUM6          DMA_ERR_INT_NUM6[32]          ; /* 48 */
    volatile U_DMA_QUEUE_DESP0           DMA_QUEUE_DESP0[32]           ; /* 50 */
    volatile U_DMA_QUEUE_DESP1           DMA_QUEUE_DESP1[32]           ; /* 54 */
    volatile U_DMA_QUEUE_DESP2           DMA_QUEUE_DESP2[32]           ; /* 58 */
    volatile U_DMA_QUEUE_DESP3           DMA_QUEUE_DESP3[32]           ; /* 5C */
    volatile U_DMA_QUEUE_ERR_ADDR_L      DMA_QUEUE_ERR_ADDR_L[32]      ; /* 60 */
    volatile U_DMA_QUEUE_ERR_ADDR_H      DMA_QUEUE_ERR_ADDR_H[32]      ; /* 64 */
    volatile U_DMA_QUEUE_SQ_READ_ERR_PTR DMA_QUEUE_SQ_READ_ERR_PTR[32] ; /* 68 */
    volatile U_DMA_QUEUE_INT_RO          DMA_QUEUE_INT_RO[32]          ; /* 6C */
    volatile U_DMA_QUEUE_INT_SET         DMA_QUEUE_INT_SET[32]         ; /* 70 */
    volatile U_DMA_QUEUE_DESP4           DMA_QUEUE_DESP4[32]           ; /* 74 */
    volatile U_DMA_QUEUE_DESP5           DMA_QUEUE_DESP5[32]           ; /* 78 */
    volatile U_DMA_QUEUE_DESP6           DMA_QUEUE_DESP6[32]           ; /* 7C */
    volatile U_DMA_QUEUE_DESP7           DMA_QUEUE_DESP7[32]           ; /* 80 */
    volatile U_DMA_ERR_INT_NUM0          DMA_ERR_INT_NUM0[32]          ; /* 84 */
    volatile U_DMA_ERR_INT_NUM1          DMA_ERR_INT_NUM1[32]          ; /* 88 */
    volatile U_DMA_ERR_INT_NUM2          DMA_ERR_INT_NUM2[32]          ; /* 8C */
    volatile U_DMA_ERR_INT_NUM3          DMA_ERR_INT_NUM3[32]          ; /* 90 */
    volatile U_DMA_ERR_INT_NUM4          DMA_ERR_INT_NUM4[32]          ; /* 94 */
    volatile U_DMA_ERR_INT_NUM5          DMA_ERR_INT_NUM5[32]          ; /* 98 */
    volatile U_DMA_QUEUE_CTRL2           DMA_QUEUE_CTRL2[32]           ; /* 9C */
    volatile U_DONE_INT_MERGE            DONE_INT_MERGE[32]            ; /* A0 */
    volatile U_DMA_QUEUE_SQ_STS2         DMA_QUEUE_SQ_STS2[32]         ; /* A4 */
    volatile U_DMA_ECC                   DMA_ECC                       ; /* 2000 */
    volatile U_DMA_ECC_ERR_ADDR          DMA_ECC_ERR_ADDR              ; /* 2004 */
    volatile U_DMA_ECC_ECC_CNT           DMA_ECC_ECC_CNT               ; /* 2014 */
    volatile U_DMA_EP_INT_SET            DMA_EP_INT_SET                ; /* 2020 */
    volatile U_DMA_EP_INT_MSK            DMA_EP_INT_MSK                ; /* 2024 */
    volatile U_DMA_EP_INT                DMA_EP_INT                    ; /* 2028 */
    volatile U_DMA_EP_INT_STS            DMA_EP_INT_STS                ; /* 202C */
    volatile U_COMMON_AND_CH_ERR_STS     COMMON_AND_CH_ERR_STS         ; /* 2030 */
    volatile U_ATOMIC_CTRL               ATOMIC_CTRL                   ; /* 2038 */
    volatile U_EP0_ATOMIC_HEADER_0       EP0_ATOMIC_HEADER_0           ; /* 203C */
    volatile U_EP0_ATOMIC_HEADER_1       EP0_ATOMIC_HEADER_1           ; /* 2040 */
    volatile U_EP0_ATOMIC_HEADER_2       EP0_ATOMIC_HEADER_2           ; /* 2044 */
    volatile U_EP0_ATOMIC_HEADER_3       EP0_ATOMIC_HEADER_3           ; /* 2048 */
    volatile U_EP0_ATOMIC_DATA_0         EP0_ATOMIC_DATA_0             ; /* 204C */
    volatile U_EP0_ATOMIC_DATA_1         EP0_ATOMIC_DATA_1             ; /* 2050 */
    volatile U_EP0_ATOMIC_DATA_2         EP0_ATOMIC_DATA_2             ; /* 2054 */
    volatile U_EP0_ATOMIC_DATA_3         EP0_ATOMIC_DATA_3             ; /* 2058 */
    volatile U_EP0_ATOMIC_DATA_4         EP0_ATOMIC_DATA_4             ; /* 205C */
    volatile U_EP0_ATOMIC_DATA_5         EP0_ATOMIC_DATA_5             ; /* 2060 */
    volatile U_EP0_ATOMIC_DATA_6         EP0_ATOMIC_DATA_6             ; /* 2064 */
    volatile U_EP0_ATOMIC_DATA_7         EP0_ATOMIC_DATA_7             ; /* 2068 */
    volatile U_EP0_ATOMIC_RESP_DATA_0    EP0_ATOMIC_RESP_DATA_0        ; /* 206C */
    volatile U_EP0_ATOMIC_RESP_DATA_1    EP0_ATOMIC_RESP_DATA_1        ; /* 2070 */
    volatile U_EP0_ATOMIC_RESP_DATA_2    EP0_ATOMIC_RESP_DATA_2        ; /* 2074 */
    volatile U_EP0_ATOMIC_RESP_DATA_3    EP0_ATOMIC_RESP_DATA_3        ; /* 2078 */
    volatile U_EP0_ATOMIC_RESP_DATA_4    EP0_ATOMIC_RESP_DATA_4        ; /* 207C */
    volatile U_EP0_ATOMIC_RESP_DATA_5    EP0_ATOMIC_RESP_DATA_5        ; /* 2080 */
    volatile U_EP0_ATOMIC_RESP_DATA_6    EP0_ATOMIC_RESP_DATA_6        ; /* 2084 */
    volatile U_EP0_ATOMIC_RESP_DATA_7    EP0_ATOMIC_RESP_DATA_7        ; /* 2088 */
    volatile U_EP1_ATOMIC_HEADER_0       EP1_ATOMIC_HEADER_0           ; /* 208C */
    volatile U_EP1_ATOMIC_HEADER_1       EP1_ATOMIC_HEADER_1           ; /* 2090 */
    volatile U_EP1_ATOMIC_HEADER_2       EP1_ATOMIC_HEADER_2           ; /* 2094 */
    volatile U_EP1_ATOMIC_HEADER_3       EP1_ATOMIC_HEADER_3           ; /* 2098 */
    volatile U_EP1_ATOMIC_DATA_0         EP1_ATOMIC_DATA_0             ; /* 209C */
    volatile U_EP1_ATOMIC_DATA_1         EP1_ATOMIC_DATA_1             ; /* 20A0 */
    volatile U_EP1_ATOMIC_DATA_2         EP1_ATOMIC_DATA_2             ; /* 20A4 */
    volatile U_EP1_ATOMIC_DATA_3         EP1_ATOMIC_DATA_3             ; /* 20A8 */
    volatile U_EP1_ATOMIC_DATA_4         EP1_ATOMIC_DATA_4             ; /* 20AC */
    volatile U_EP1_ATOMIC_DATA_5         EP1_ATOMIC_DATA_5             ; /* 20B0 */
    volatile U_EP1_ATOMIC_DATA_6         EP1_ATOMIC_DATA_6             ; /* 20B4 */
    volatile U_EP1_ATOMIC_DATA_7         EP1_ATOMIC_DATA_7             ; /* 20B8 */
    volatile U_EP1_ATOMIC_RESP_DATA_0    EP1_ATOMIC_RESP_DATA_0        ; /* 20BC */
    volatile U_EP1_ATOMIC_RESP_DATA_1    EP1_ATOMIC_RESP_DATA_1        ; /* 20C0 */
    volatile U_EP1_ATOMIC_RESP_DATA_2    EP1_ATOMIC_RESP_DATA_2        ; /* 20C4 */
    volatile U_EP1_ATOMIC_RESP_DATA_3    EP1_ATOMIC_RESP_DATA_3        ; /* 20C8 */
    volatile U_EP1_ATOMIC_RESP_DATA_4    EP1_ATOMIC_RESP_DATA_4        ; /* 20CC */
    volatile U_EP1_ATOMIC_RESP_DATA_5    EP1_ATOMIC_RESP_DATA_5        ; /* 20D0 */
    volatile U_EP1_ATOMIC_RESP_DATA_6    EP1_ATOMIC_RESP_DATA_6        ; /* 20D4 */
    volatile U_EP1_ATOMIC_RESP_DATA_7    EP1_ATOMIC_RESP_DATA_7        ; /* 20D8 */
    volatile U_ATOMIC_RESP_DATA_ST       ATOMIC_RESP_DATA_ST           ; /* 20DC */
    volatile U_LOCAL_CPL_ID_STS_0        LOCAL_CPL_ID_STS_0            ; /* 20E0 */
    volatile U_LOCAL_CPL_ID_STS_1        LOCAL_CPL_ID_STS_1            ; /* 20E4 */
    volatile U_LOCAL_CPL_ID_STS_2        LOCAL_CPL_ID_STS_2            ; /* 20E8 */
    volatile U_LOCAL_CPL_ID_STS_3        LOCAL_CPL_ID_STS_3            ; /* 20EC */
    volatile U_EP1_LOCAL_CPL_ID_STS_0    EP1_LOCAL_CPL_ID_STS_0        ; /* 20F0 */
    volatile U_EP1_LOCAL_CPL_ID_STS_1    EP1_LOCAL_CPL_ID_STS_1        ; /* 20F4 */
    volatile U_EP1_LOCAL_CPL_ID_STS_2    EP1_LOCAL_CPL_ID_STS_2        ; /* 20F8 */
    volatile U_EP1_LOCAL_CPL_ID_STS_3    EP1_LOCAL_CPL_ID_STS_3        ; /* 20FC */
    volatile U_REMOTE_CPL_ID_STS_0       REMOTE_CPL_ID_STS_0           ; /* 2100 */
    volatile U_REMOTE_CPL_ID_STS_1       REMOTE_CPL_ID_STS_1           ; /* 2104 */
    volatile U_REMOTE_CPL_ID_STS_2       REMOTE_CPL_ID_STS_2           ; /* 2108 */
    volatile U_REMOTE_CPL_ID_STS_3       REMOTE_CPL_ID_STS_3           ; /* 210C */
    volatile U_EP1_REMOTE_CPL_ID_STS_0   EP1_REMOTE_CPL_ID_STS_0       ; /* 2110 */
    volatile U_EP1_REMOTE_CPL_ID_STS_1   EP1_REMOTE_CPL_ID_STS_1       ; /* 2114 */
    volatile U_EP1_REMOTE_CPL_ID_STS_2   EP1_REMOTE_CPL_ID_STS_2       ; /* 2118 */
    volatile U_EP1_REMOTE_CPL_ID_STS_3   EP1_REMOTE_CPL_ID_STS_3       ; /* 211C */
    volatile U_REMOTE_P_ID_STS_0         REMOTE_P_ID_STS_0             ; /* 2120 */
    volatile U_REMOTE_P_ID_STS_1         REMOTE_P_ID_STS_1             ; /* 2124 */
    volatile U_REMOTE_P_ID_STS_2         REMOTE_P_ID_STS_2             ; /* 2128 */
    volatile U_REMOTE_P_ID_STS_3         REMOTE_P_ID_STS_3             ; /* 212C */
    volatile U_EP1_REMOTE_P_ID_STS_0     EP1_REMOTE_P_ID_STS_0         ; /* 2130 */
    volatile U_EP1_REMOTE_P_ID_STS_1     EP1_REMOTE_P_ID_STS_1         ; /* 2134 */
    volatile U_EP1_REMOTE_P_ID_STS_2     EP1_REMOTE_P_ID_STS_2         ; /* 2138 */
    volatile U_EP1_REMOTE_P_ID_STS_3     EP1_REMOTE_P_ID_STS_3         ; /* 213C */
    volatile U_LOCAL_TLP_P_ST_CFG        LOCAL_TLP_P_ST_CFG            ; /* 2140 */
    volatile U_EP0_ATOMIC_PREFIX_INF     EP0_ATOMIC_PREFIX_INF         ; /* 2144 */
    volatile U_EP1_ATOMIC_PREFIX_INF     EP1_ATOMIC_PREFIX_INF         ; /* 2148 */
    volatile U_ATOMIC_PF_VF              ATOMIC_PF_VF                  ; /* 214C */
    volatile U_PORT_IDLE_STS             PORT_IDLE_STS                 ; /* 2150 */
    volatile U_REMOTE_TLP_NUM            REMOTE_TLP_NUM                ; /* 2154 */
    volatile U_LOCAL_TLP_NUM             LOCAL_TLP_NUM                 ; /* 2158 */
    volatile U_EP1_REMOTE_TLP_NUM        EP1_REMOTE_TLP_NUM            ; /* 215C */
    volatile U_EP1_LOCAL_TLP_NUM         EP1_LOCAL_TLP_NUM             ; /* 2160 */
    volatile U_SQCQ_TLP_NUM              SQCQ_TLP_NUM                  ; /* 2164 */
    volatile U_CPL_NUM                   CPL_NUM                       ; /* 2168 */
    volatile U_EP1_CPL_NUM               EP1_CPL_NUM                   ; /* 216C */
    volatile U_INF_BACK_PRESS_STS        INF_BACK_PRESS_STS            ; /* 2170 */
    volatile U_PORT_LINKDOWN_EVENT       PORT_LINKDOWN_EVENT           ; /* 2174 */
    volatile U_DMA_MODE_SEL              DMA_MODE_SEL                  ; /* 217C */
    volatile U_DMA_GLOBAL_CTRL           DMA_GLOBAL_CTRL               ; /* 2180 */
    volatile U_DMA_CH_RAS_LEVEL          DMA_CH_RAS_LEVEL              ; /* 2184 */
    volatile U_DMA_CM_RAS_LEVEL          DMA_CM_RAS_LEVEL              ; /* 2188 */
    volatile U_DMA_INT_MODE_SEL          DMA_INT_MODE_SEL              ; /* 218C */
    volatile U_DMA_CH_ERR_STS            DMA_CH_ERR_STS                ; /* 2190 */
    volatile U_DMA_CH_DONE_STS           DMA_CH_DONE_STS               ; /* 2194 */
    volatile U_DMA_SQ_TAG_STS_0          DMA_SQ_TAG_STS_0              ; /* 21A0 */
    volatile U_DMA_SQ_TAG_STS_1          DMA_SQ_TAG_STS_1              ; /* 21A4 */
    volatile U_DMA_SQ_TAG_STS_2          DMA_SQ_TAG_STS_2              ; /* 21A8 */
    volatile U_DMA_SQ_TAG_STS_3          DMA_SQ_TAG_STS_3              ; /* 21AC */
    volatile U_LOCAL_P_ID_STS_0          LOCAL_P_ID_STS_0              ; /* 21B0 */
    volatile U_LOCAL_P_ID_STS_1          LOCAL_P_ID_STS_1              ; /* 21B4 */
    volatile U_LOCAL_P_ID_STS_2          LOCAL_P_ID_STS_2              ; /* 21B8 */
    volatile U_LOCAL_P_ID_STS_3          LOCAL_P_ID_STS_3              ; /* 21BC */
    volatile U_EP0_BLOCK_INFO            EP0_BLOCK_INFO                ; /* 21C0 */
    volatile U_EP1_BLOCK_INFO            EP1_BLOCK_INFO                ; /* 21C4 */
    volatile U_DMA_PREBUFF_INFO_0        DMA_PREBUFF_INFO_0            ; /* 2200 */
    volatile U_DMA_CM_TABLE_INFO_0       DMA_CM_TABLE_INFO_0           ; /* 2220 */
    volatile U_DMA_CM_CE_RO              DMA_CM_CE_RO                  ; /* 2244 */
    volatile U_DMA_CM_NFE_RO             DMA_CM_NFE_RO                 ; /* 2248 */
    volatile U_DMA_CM_FE_RO              DMA_CM_FE_RO                  ; /* 224C */
    volatile U_DMA_CH_EP0_CE_RO          DMA_CH_EP0_CE_RO              ; /* 2254 */
    volatile U_DMA_CH_EP0_NFE_RO         DMA_CH_EP0_NFE_RO             ; /* 2258 */
    volatile U_DMA_CH_EP0_FE_RO          DMA_CH_EP0_FE_RO              ; /* 225C */
    volatile U_DMA_CH_EP1_CE_RO          DMA_CH_EP1_CE_RO              ; /* 2264 */
    volatile U_DMA_CH_EP1_NFE_RO         DMA_CH_EP1_NFE_RO             ; /* 2268 */
    volatile U_DMA_CH_EP1_FE_RO          DMA_CH_EP1_FE_RO              ; /* 226C */

} S_hipciec_ap_dma_reg_REGS_TYPE;

/* Declare the struct pointor of the module hipciec_ap_dma_reg */
extern volatile S_hipciec_ap_dma_reg_REGS_TYPE *gophipciec_ap_dma_regAllReg;

/* Declare the functions that set the member value */
int iSetDMA_QUEUE_SQ_BASE_L_dma_queue_sq_base_l(unsigned int udma_queue_sq_base_l);
int iSetDMA_QUEUE_SQ_BASE_H_dma_queue_sq_base_h(unsigned int udma_queue_sq_base_h);
int iSetDMA_QUEUE_SQ_DEPTH_dma_queue_sq_depth(unsigned int udma_queue_sq_depth);
int iSetDMA_QUEUE_SQ_TAIL_PTR_dma_queue_sq_tail_ptr(unsigned int udma_queue_sq_tail_ptr);
int iSetDMA_QUEUE_CQ_BASE_L_dma_queue_cq_base_l(unsigned int udma_queue_cq_base_l);
int iSetDMA_QUEUE_CQ_BASE_H_dma_queue_cq_base_h(unsigned int udma_queue_cq_base_h);
int iSetDMA_QUEUE_CQ_DEPTH_dma_queue_cq_depth(unsigned int udma_queue_cq_depth);
int iSetDMA_QUEUE_CQ_HEAD_PTR_dma_queue_cq_head_ptr(unsigned int udma_queue_cq_head_ptr);
int iSetDMA_QUEUE_CTRL0_dma_queue_cq_full_disable(unsigned int udma_queue_cq_full_disable);
int iSetDMA_QUEUE_CTRL0_dma_queue_sqcq_drct_sel(unsigned int udma_queue_sqcq_drct_sel);
int iSetDMA_QUEUE_CTRL0_dma_queue_remote_err_done_int_en(unsigned int udma_queue_remote_err_done_int_en);
int iSetDMA_QUEUE_CTRL0_dma_queue_local_err_done_int_en(unsigned int udma_queue_local_err_done_int_en);
int iSetDMA_QUEUE_CTRL0_dma_queue_cq_mrg_time(unsigned int udma_queue_cq_mrg_time);
int iSetDMA_QUEUE_CTRL0_dma_queue_cq_mrg_en(unsigned int udma_queue_cq_mrg_en);
int iSetDMA_QUEUE_CTRL0_dma_queue_arb_weight(unsigned int udma_queue_arb_weight);
int iSetDMA_QUEUE_CTRL0_dma_queue_pause(unsigned int udma_queue_pause);
int iSetDMA_QUEUE_CTRL0_dma_queue_err_abort_en(unsigned int udma_queue_err_abort_en);
int iSetDMA_QUEUE_CTRL0_dma_ep_port_sel(unsigned int udma_ep_port_sel);
int iSetDMA_QUEUE_CTRL0_dma_queue_en(unsigned int udma_queue_en);
int iSetDMA_QUEUE_CTRL1_dma_queue_abort_exit(unsigned int udma_queue_abort_exit);
int iSetDMA_QUEUE_CTRL1_dma_queue_reset(unsigned int udma_queue_reset);
int iSetDMA_QUEUE_RSV_dma_queue_rsv(unsigned int udma_queue_rsv);
int iSetDMA_QUEUE_FSM_STS_dma_queue_sub_fsm_sts(unsigned int udma_queue_sub_fsm_sts);
int iSetDMA_QUEUE_FSM_STS_dma_queue_wait_spd_data_sts(unsigned int udma_queue_wait_spd_data_sts);
int iSetDMA_QUEUE_FSM_STS_dma_queue_not_work(unsigned int udma_queue_not_work);
int iSetDMA_QUEUE_FSM_STS_dma_queue_sts(unsigned int udma_queue_sts);
int iSetDMA_QUEUE_SQ_STS_dma_queue_cq_send_cnt(unsigned int udma_queue_cq_send_cnt);
int iSetDMA_QUEUE_SQ_STS_dma_queue_resp_rec_finish(unsigned int udma_queue_resp_rec_finish);
int iSetDMA_QUEUE_SQ_STS_dma_queue_sq_pre_num(unsigned int udma_queue_sq_pre_num);
int iSetDMA_QUEUE_SQ_STS_dma_queue_sq_head_ptr(unsigned int udma_queue_sq_head_ptr);
int iSetDMA_QUEUE_BYTE_CNT_dma_queue_byte_cnt(unsigned int udma_queue_byte_cnt);
int iSetDMA_QUEUE_CQ_TAIL_PTR_dma_queue_sqhd(unsigned int udma_queue_sqhd);
int iSetDMA_QUEUE_CQ_TAIL_PTR_dma_queue_cq_tail_ptr(unsigned int udma_queue_cq_tail_ptr);
int iSetDMA_QUEUE_INT_STS_dma_queue_err13_int_sts(unsigned int udma_queue_err13_int_sts);
int iSetDMA_QUEUE_INT_STS_dma_queue_err12_int_sts(unsigned int udma_queue_err12_int_sts);
int iSetDMA_QUEUE_INT_STS_dma_queue_err11_int_sts(unsigned int udma_queue_err11_int_sts);
int iSetDMA_QUEUE_INT_STS_dma_queue_err10_int_sts(unsigned int udma_queue_err10_int_sts);
int iSetDMA_QUEUE_INT_STS_dma_queue_err09_int_sts(unsigned int udma_queue_err09_int_sts);
int iSetDMA_QUEUE_INT_STS_dma_queue_err08_int_sts(unsigned int udma_queue_err08_int_sts);
int iSetDMA_QUEUE_INT_STS_dma_queue_err07_int_sts(unsigned int udma_queue_err07_int_sts);
int iSetDMA_QUEUE_INT_STS_dma_queue_err06_int_sts(unsigned int udma_queue_err06_int_sts);
int iSetDMA_QUEUE_INT_STS_dma_queue_err05_int_sts(unsigned int udma_queue_err05_int_sts);
int iSetDMA_QUEUE_INT_STS_dma_queue_err04_int_sts(unsigned int udma_queue_err04_int_sts);
int iSetDMA_QUEUE_INT_STS_dma_queue_err03_int_sts(unsigned int udma_queue_err03_int_sts);
int iSetDMA_QUEUE_INT_STS_dma_queue_err02_int_sts(unsigned int udma_queue_err02_int_sts);
int iSetDMA_QUEUE_INT_STS_dma_queue_err01_int_sts(unsigned int udma_queue_err01_int_sts);
int iSetDMA_QUEUE_INT_STS_dma_queue_err00_int_sts(unsigned int udma_queue_err00_int_sts);
int iSetDMA_QUEUE_INT_STS_dma_queue_done_int_sts(unsigned int udma_queue_done_int_sts);
int iSetDMA_QUEUE_INT_MSK_dma_queue_err13_int_msk(unsigned int udma_queue_err13_int_msk);
int iSetDMA_QUEUE_INT_MSK_dma_queue_err12_int_msk(unsigned int udma_queue_err12_int_msk);
int iSetDMA_QUEUE_INT_MSK_dma_queue_err11_int_msk(unsigned int udma_queue_err11_int_msk);
int iSetDMA_QUEUE_INT_MSK_dma_queue_err10_int_msk(unsigned int udma_queue_err10_int_msk);
int iSetDMA_QUEUE_INT_MSK_dma_queue_err09_int_msk(unsigned int udma_queue_err09_int_msk);
int iSetDMA_QUEUE_INT_MSK_dma_queue_err08_int_msk(unsigned int udma_queue_err08_int_msk);
int iSetDMA_QUEUE_INT_MSK_dma_queue_err07_int_msk(unsigned int udma_queue_err07_int_msk);
int iSetDMA_QUEUE_INT_MSK_dma_queue_err06_int_msk(unsigned int udma_queue_err06_int_msk);
int iSetDMA_QUEUE_INT_MSK_dma_queue_err05_int_msk(unsigned int udma_queue_err05_int_msk);
int iSetDMA_QUEUE_INT_MSK_dma_queue_err04_int_msk(unsigned int udma_queue_err04_int_msk);
int iSetDMA_QUEUE_INT_MSK_dma_queue_err03_int_msk(unsigned int udma_queue_err03_int_msk);
int iSetDMA_QUEUE_INT_MSK_dma_queue_err02_int_msk(unsigned int udma_queue_err02_int_msk);
int iSetDMA_QUEUE_INT_MSK_dma_queue_err01_int_msk(unsigned int udma_queue_err01_int_msk);
int iSetDMA_QUEUE_INT_MSK_dma_queue_err00_int_msk(unsigned int udma_queue_err00_int_msk);
int iSetDMA_QUEUE_INT_MSK_dma_queue_done_int_msk(unsigned int udma_queue_done_int_msk);
int iSetDMA_ERR_INT_NUM6_dma_queue_err13_num(unsigned int udma_queue_err13_num);
int iSetDMA_ERR_INT_NUM6_dma_queue_err12_num(unsigned int udma_queue_err12_num);
int iSetDMA_QUEUE_DESP0_dma_queue_curr_desp_0(unsigned int udma_queue_curr_desp_0);
int iSetDMA_QUEUE_DESP1_dma_queue_curr_desp_1(unsigned int udma_queue_curr_desp_1);
int iSetDMA_QUEUE_DESP2_dma_queue_curr_desp_2(unsigned int udma_queue_curr_desp_2);
int iSetDMA_QUEUE_DESP3_dma_queue_curr_desp_3(unsigned int udma_queue_curr_desp_3);
int iSetDMA_QUEUE_ERR_ADDR_L_dma_queue_err_addr_l(unsigned int udma_queue_err_addr_l);
int iSetDMA_QUEUE_ERR_ADDR_H_dma_queue_err_addr_h(unsigned int udma_queue_err_addr_h);
int iSetDMA_QUEUE_SQ_READ_ERR_PTR_dma_queue_sq_ptr_err(unsigned int udma_queue_sq_ptr_err);
int iSetDMA_QUEUE_INT_RO_dma_queue_sts_err13(unsigned int udma_queue_sts_err13);
int iSetDMA_QUEUE_INT_RO_dma_queue_sts_err12(unsigned int udma_queue_sts_err12);
int iSetDMA_QUEUE_INT_RO_dma_queue_sts_err11(unsigned int udma_queue_sts_err11);
int iSetDMA_QUEUE_INT_RO_dma_queue_sts_err10(unsigned int udma_queue_sts_err10);
int iSetDMA_QUEUE_INT_RO_dma_queue_sts_err09(unsigned int udma_queue_sts_err09);
int iSetDMA_QUEUE_INT_RO_dma_queue_sts_err08(unsigned int udma_queue_sts_err08);
int iSetDMA_QUEUE_INT_RO_dma_queue_sts_err07(unsigned int udma_queue_sts_err07);
int iSetDMA_QUEUE_INT_RO_dma_queue_sts_err06(unsigned int udma_queue_sts_err06);
int iSetDMA_QUEUE_INT_RO_dma_queue_sts_err05(unsigned int udma_queue_sts_err05);
int iSetDMA_QUEUE_INT_RO_dma_queue_sts_err04(unsigned int udma_queue_sts_err04);
int iSetDMA_QUEUE_INT_RO_dma_queue_sts_err03(unsigned int udma_queue_sts_err03);
int iSetDMA_QUEUE_INT_RO_dma_queue_sts_err02(unsigned int udma_queue_sts_err02);
int iSetDMA_QUEUE_INT_RO_dma_queue_sts_err01(unsigned int udma_queue_sts_err01);
int iSetDMA_QUEUE_INT_RO_dma_queue_sts_err00(unsigned int udma_queue_sts_err00);
int iSetDMA_QUEUE_INT_RO_dma_queue_sts_done(unsigned int udma_queue_sts_done);
int iSetDMA_QUEUE_INT_SET_dma_queue_err13_int_set(unsigned int udma_queue_err13_int_set);
int iSetDMA_QUEUE_INT_SET_dma_queue_err12_int_set(unsigned int udma_queue_err12_int_set);
int iSetDMA_QUEUE_INT_SET_dma_queue_err11_int_set(unsigned int udma_queue_err11_int_set);
int iSetDMA_QUEUE_INT_SET_dma_queue_err10_int_set(unsigned int udma_queue_err10_int_set);
int iSetDMA_QUEUE_INT_SET_dma_queue_err09_int_set(unsigned int udma_queue_err09_int_set);
int iSetDMA_QUEUE_INT_SET_dma_queue_err08_int_set(unsigned int udma_queue_err08_int_set);
int iSetDMA_QUEUE_INT_SET_dma_queue_err07_int_set(unsigned int udma_queue_err07_int_set);
int iSetDMA_QUEUE_INT_SET_dma_queue_err06_int_set(unsigned int udma_queue_err06_int_set);
int iSetDMA_QUEUE_INT_SET_dma_queue_err05_int_set(unsigned int udma_queue_err05_int_set);
int iSetDMA_QUEUE_INT_SET_dma_queue_err04_int_set(unsigned int udma_queue_err04_int_set);
int iSetDMA_QUEUE_INT_SET_dma_queue_err03_int_set(unsigned int udma_queue_err03_int_set);
int iSetDMA_QUEUE_INT_SET_dma_queue_err02_int_set(unsigned int udma_queue_err02_int_set);
int iSetDMA_QUEUE_INT_SET_dma_queue_err01_int_set(unsigned int udma_queue_err01_int_set);
int iSetDMA_QUEUE_INT_SET_dma_queue_err00_int_set(unsigned int udma_queue_err00_int_set);
int iSetDMA_QUEUE_INT_SET_dma_queue_done_int_set(unsigned int udma_queue_done_int_set);
int iSetDMA_QUEUE_DESP4_dma_queue_curr_desp_4(unsigned int udma_queue_curr_desp_4);
int iSetDMA_QUEUE_DESP5_dma_queue_curr_desp_5(unsigned int udma_queue_curr_desp_5);
int iSetDMA_QUEUE_DESP6_dma_queue_curr_desp_6(unsigned int udma_queue_curr_desp_6);
int iSetDMA_QUEUE_DESP7_dma_queue_curr_desp_7(unsigned int udma_queue_curr_desp_7);
int iSetDMA_ERR_INT_NUM0_dma_queue_err01_num(unsigned int udma_queue_err01_num);
int iSetDMA_ERR_INT_NUM0_dma_queue_err00_num(unsigned int udma_queue_err00_num);
int iSetDMA_ERR_INT_NUM1_dma_queue_err03_num(unsigned int udma_queue_err03_num);
int iSetDMA_ERR_INT_NUM1_dma_queue_err02_num(unsigned int udma_queue_err02_num);
int iSetDMA_ERR_INT_NUM2_dma_queue_err05_num(unsigned int udma_queue_err05_num);
int iSetDMA_ERR_INT_NUM2_dma_queue_err04_num(unsigned int udma_queue_err04_num);
int iSetDMA_ERR_INT_NUM3_dma_queue_err07_num(unsigned int udma_queue_err07_num);
int iSetDMA_ERR_INT_NUM3_dma_queue_err06_num(unsigned int udma_queue_err06_num);
int iSetDMA_ERR_INT_NUM4_dma_queue_err09_num(unsigned int udma_queue_err09_num);
int iSetDMA_ERR_INT_NUM4_dma_queue_err08_num(unsigned int udma_queue_err08_num);
int iSetDMA_ERR_INT_NUM5_dma_queue_err11_num(unsigned int udma_queue_err11_num);
int iSetDMA_ERR_INT_NUM5_dma_queue_err10_num(unsigned int udma_queue_err10_num);
int iSetDMA_QUEUE_CTRL2_dma_queue_rmt_vf_cfg(unsigned int udma_queue_rmt_vf_cfg);
int iSetDMA_QUEUE_CTRL2_dma_queue_rmt_pf_cfg(unsigned int udma_queue_rmt_pf_cfg);
int iSetDONE_INT_MERGE_dma_done_int_merge_en(unsigned int udma_done_int_merge_en);
int iSetDONE_INT_MERGE_dma_queue_int_merge_time(unsigned int udma_queue_int_merge_time);
int iSetDMA_QUEUE_SQ_STS2_dma_queue_barrier_rd_wst(unsigned int udma_queue_barrier_rd_wst);
int iSetDMA_QUEUE_SQ_STS2_dma_queue_barrier_wd_wst(unsigned int udma_queue_barrier_wd_wst);
int iSetDMA_QUEUE_SQ_STS2_dma_queue_sq_bd_allow(unsigned int udma_queue_sq_bd_allow);
int iSetDMA_QUEUE_SQ_STS2_dma_queue_sq_req_allow(unsigned int udma_queue_sq_req_allow);
int iSetDMA_QUEUE_SQ_STS2_dma_queue_sq_clean_ot(unsigned int udma_queue_sq_clean_ot);
int iSetDMA_QUEUE_SQ_STS2_dma_queue_sq_clean(unsigned int udma_queue_sq_clean);
int iSetDMA_ECC_ecc_2bit_inject(unsigned int uecc_2bit_inject);
int iSetDMA_ECC_ecc_1bit_inject(unsigned int uecc_1bit_inject);
int iSetDMA_ECC_ERR_ADDR_ecc_2bit_err_addr(unsigned int uecc_2bit_err_addr);
int iSetDMA_ECC_ERR_ADDR_ecc_1bit_err_addr(unsigned int uecc_1bit_err_addr);
int iSetDMA_ECC_ECC_CNT_ecc_2bit_cnt(unsigned int uecc_2bit_cnt);
int iSetDMA_ECC_ECC_CNT_ecc_1bit_cnt(unsigned int uecc_1bit_cnt);
int iSetDMA_EP_INT_SET_remote_cpl_tag_err_int_set(unsigned int uremote_cpl_tag_err_int_set);
int iSetDMA_EP_INT_SET_local_cpl_tag_err_int_set(unsigned int ulocal_cpl_tag_err_int_set);
int iSetDMA_EP_INT_SET_ecc_2bit_err_int_set(unsigned int uecc_2bit_err_int_set);
int iSetDMA_EP_INT_SET_ecc_1bit_err_int_set(unsigned int uecc_1bit_err_int_set);
int iSetDMA_EP_INT_MSK_remote_cpl_tag_err_int_msk(unsigned int uremote_cpl_tag_err_int_msk);
int iSetDMA_EP_INT_MSK_local_cpl_tag_err_int_msk(unsigned int ulocal_cpl_tag_err_int_msk);
int iSetDMA_EP_INT_MSK_ecc_2bit_err_int_msk(unsigned int uecc_2bit_err_int_msk);
int iSetDMA_EP_INT_MSK_ecc_1bit_err_int_msk(unsigned int uecc_1bit_err_int_msk);
int iSetDMA_EP_INT_remote_cpl_tag_err_int(unsigned int uremote_cpl_tag_err_int);
int iSetDMA_EP_INT_local_cpl_tag_err_int(unsigned int ulocal_cpl_tag_err_int);
int iSetDMA_EP_INT_ecc_2bit_err_int(unsigned int uecc_2bit_err_int);
int iSetDMA_EP_INT_ecc_1bit_err_int(unsigned int uecc_1bit_err_int);
int iSetDMA_EP_INT_STS_remote_cpl_tag_err_st(unsigned int uremote_cpl_tag_err_st);
int iSetDMA_EP_INT_STS_local_cpl_tag_err_st(unsigned int ulocal_cpl_tag_err_st);
int iSetDMA_EP_INT_STS_ecc_2bit_err_st(unsigned int uecc_2bit_err_st);
int iSetDMA_EP_INT_STS_ecc_1bit_err_st(unsigned int uecc_1bit_err_st);
int iSetCOMMON_AND_CH_ERR_STS_dma_sram_init_done(unsigned int udma_sram_init_done);
int iSetCOMMON_AND_CH_ERR_STS_dma_queue_err_sts(unsigned int udma_queue_err_sts);
int iSetCOMMON_AND_CH_ERR_STS_cm_err_sts(unsigned int ucm_err_sts);
int iSetATOMIC_CTRL_ep1_atomic_data_ctrl(unsigned int uep1_atomic_data_ctrl);
int iSetATOMIC_CTRL_ep0_atomic_data_ctrl(unsigned int uep0_atomic_data_ctrl);
int iSetEP0_ATOMIC_HEADER_0_ep0_atomic_header_0(unsigned int uep0_atomic_header_0);
int iSetEP0_ATOMIC_HEADER_1_ep0_atomic_header_1(unsigned int uep0_atomic_header_1);
int iSetEP0_ATOMIC_HEADER_2_ep0_atomic_header_2(unsigned int uep0_atomic_header_2);
int iSetEP0_ATOMIC_HEADER_3_ep0_atomic_header_3(unsigned int uep0_atomic_header_3);
int iSetEP0_ATOMIC_DATA_0_ep0_atomic_data_0(unsigned int uep0_atomic_data_0);
int iSetEP0_ATOMIC_DATA_1_ep0_atomic_data_1(unsigned int uep0_atomic_data_1);
int iSetEP0_ATOMIC_DATA_2_ep0_atomic_data_2(unsigned int uep0_atomic_data_2);
int iSetEP0_ATOMIC_DATA_3_ep0_atomic_data_3(unsigned int uep0_atomic_data_3);
int iSetEP0_ATOMIC_DATA_4_ep0_atomic_data_4(unsigned int uep0_atomic_data_4);
int iSetEP0_ATOMIC_DATA_5_ep0_atomic_data_5(unsigned int uep0_atomic_data_5);
int iSetEP0_ATOMIC_DATA_6_ep0_atomic_data_6(unsigned int uep0_atomic_data_6);
int iSetEP0_ATOMIC_DATA_7_ep0_atomic_data_7(unsigned int uep0_atomic_data_7);
int iSetEP0_ATOMIC_RESP_DATA_0_ep0_atomic_resp_data_0(unsigned int uep0_atomic_resp_data_0);
int iSetEP0_ATOMIC_RESP_DATA_1_ep0_atomic_resp_data_1(unsigned int uep0_atomic_resp_data_1);
int iSetEP0_ATOMIC_RESP_DATA_2_ep0_atomic_resp_data_2(unsigned int uep0_atomic_resp_data_2);
int iSetEP0_ATOMIC_RESP_DATA_3_ep0_atomic_resp_data_3(unsigned int uep0_atomic_resp_data_3);
int iSetEP0_ATOMIC_RESP_DATA_4_ep0_atomic_resp_data_4(unsigned int uep0_atomic_resp_data_4);
int iSetEP0_ATOMIC_RESP_DATA_5_ep0_atomic_resp_data_5(unsigned int uep0_atomic_resp_data_5);
int iSetEP0_ATOMIC_RESP_DATA_6_ep0_atomic_resp_data_6(unsigned int uep0_atomic_resp_data_6);
int iSetEP0_ATOMIC_RESP_DATA_7_ep0_atomic_resp_data_7(unsigned int uep0_atomic_resp_data_7);
int iSetEP1_ATOMIC_HEADER_0_ep1_atomic_header_0(unsigned int uep1_atomic_header_0);
int iSetEP1_ATOMIC_HEADER_1_ep1_atomic_header_1(unsigned int uep1_atomic_header_1);
int iSetEP1_ATOMIC_HEADER_2_ep1_atomic_header_2(unsigned int uep1_atomic_header_2);
int iSetEP1_ATOMIC_HEADER_3_ep1_atomic_header_3(unsigned int uep1_atomic_header_3);
int iSetEP1_ATOMIC_DATA_0_ep1_atomic_data_0(unsigned int uep1_atomic_data_0);
int iSetEP1_ATOMIC_DATA_1_ep1_atomic_data_1(unsigned int uep1_atomic_data_1);
int iSetEP1_ATOMIC_DATA_2_ep1_atomic_data_2(unsigned int uep1_atomic_data_2);
int iSetEP1_ATOMIC_DATA_3_ep1_atomic_data_3(unsigned int uep1_atomic_data_3);
int iSetEP1_ATOMIC_DATA_4_ep1_atomic_data_4(unsigned int uep1_atomic_data_4);
int iSetEP1_ATOMIC_DATA_5_ep1_atomic_data_5(unsigned int uep1_atomic_data_5);
int iSetEP1_ATOMIC_DATA_6_ep1_atomic_data_6(unsigned int uep1_atomic_data_6);
int iSetEP1_ATOMIC_DATA_7_ep1_atomic_data_7(unsigned int uep1_atomic_data_7);
int iSetEP1_ATOMIC_RESP_DATA_0_ep1_atomic_resp_data_0(unsigned int uep1_atomic_resp_data_0);
int iSetEP1_ATOMIC_RESP_DATA_1_ep1_atomic_resp_data_1(unsigned int uep1_atomic_resp_data_1);
int iSetEP1_ATOMIC_RESP_DATA_2_ep1_atomic_resp_data_2(unsigned int uep1_atomic_resp_data_2);
int iSetEP1_ATOMIC_RESP_DATA_3_ep1_atomic_resp_data_3(unsigned int uep1_atomic_resp_data_3);
int iSetEP1_ATOMIC_RESP_DATA_4_ep1_atomic_resp_data_4(unsigned int uep1_atomic_resp_data_4);
int iSetEP1_ATOMIC_RESP_DATA_5_ep1_atomic_resp_data_5(unsigned int uep1_atomic_resp_data_5);
int iSetEP1_ATOMIC_RESP_DATA_6_ep1_atomic_resp_data_6(unsigned int uep1_atomic_resp_data_6);
int iSetEP1_ATOMIC_RESP_DATA_7_ep1_atomic_resp_data_7(unsigned int uep1_atomic_resp_data_7);
int iSetATOMIC_RESP_DATA_ST_ep1_atomic_resp_data_err(unsigned int uep1_atomic_resp_data_err);
int iSetATOMIC_RESP_DATA_ST_ep0_atomic_resp_data_err(unsigned int uep0_atomic_resp_data_err);
int iSetATOMIC_RESP_DATA_ST_ep1_atomic_resp_data_vld(unsigned int uep1_atomic_resp_data_vld);
int iSetATOMIC_RESP_DATA_ST_ep0_atomic_resp_data_vld(unsigned int uep0_atomic_resp_data_vld);
int iSetLOCAL_CPL_ID_STS_0_local_cpl_tag_id_status_0(unsigned int ulocal_cpl_tag_id_status_0);
int iSetLOCAL_CPL_ID_STS_1_local_cpl_tag_id_status_1(unsigned int ulocal_cpl_tag_id_status_1);
int iSetLOCAL_CPL_ID_STS_2_local_cpl_tag_id_status_2(unsigned int ulocal_cpl_tag_id_status_2);
int iSetLOCAL_CPL_ID_STS_3_local_cpl_tag_id_status_3(unsigned int ulocal_cpl_tag_id_status_3);




int iSetREMOTE_CPL_ID_STS_0_remote_cpl_tag_id_status_0(unsigned int uremote_cpl_tag_id_status_0);
int iSetREMOTE_CPL_ID_STS_1_remote_cpl_tag_id_status_1(unsigned int uremote_cpl_tag_id_status_1);
int iSetREMOTE_CPL_ID_STS_2_remote_cpl_tag_id_status_2(unsigned int uremote_cpl_tag_id_status_2);
int iSetREMOTE_CPL_ID_STS_3_remote_cpl_tag_id_status_3(unsigned int uremote_cpl_tag_id_status_3);




int iSetREMOTE_P_ID_STS_0_remote_p_tag_id_status_0(unsigned int uremote_p_tag_id_status_0);
int iSetREMOTE_P_ID_STS_1_remote_p_tag_id_status_1(unsigned int uremote_p_tag_id_status_1);
int iSetREMOTE_P_ID_STS_2_remote_p_tag_id_status_2(unsigned int uremote_p_tag_id_status_2);
int iSetREMOTE_P_ID_STS_3_remote_p_tag_id_status_3(unsigned int uremote_p_tag_id_status_3);




int iSetLOCAL_TLP_P_ST_CFG_local_ep1_tlp_p_ph_cfg(unsigned int ulocal_ep1_tlp_p_ph_cfg);
int iSetLOCAL_TLP_P_ST_CFG_local_ep0_tlp_p_ph_cfg(unsigned int ulocal_ep0_tlp_p_ph_cfg);
int iSetLOCAL_TLP_P_ST_CFG_local_ep1_tlp_p_st_cfg(unsigned int ulocal_ep1_tlp_p_st_cfg);
int iSetLOCAL_TLP_P_ST_CFG_local_ep0_tlp_p_st_cfg(unsigned int ulocal_ep0_tlp_p_st_cfg);
int iSetEP0_ATOMIC_PREFIX_INF_ep0_atomic_prfx_dwen(unsigned int uep0_atomic_prfx_dwen);
int iSetEP0_ATOMIC_PREFIX_INF_ep0_atomic_prfx_pmr(unsigned int uep0_atomic_prfx_pmr);
int iSetEP0_ATOMIC_PREFIX_INF_ep0_atomic_prfx_er(unsigned int uep0_atomic_prfx_er);
int iSetEP0_ATOMIC_PREFIX_INF_ep0_atomic_pasid(unsigned int uep0_atomic_pasid);
int iSetEP1_ATOMIC_PREFIX_INF_ep1_atomic_prfx_dwen(unsigned int uep1_atomic_prfx_dwen);
int iSetEP1_ATOMIC_PREFIX_INF_ep1_atomic_prfx_pmr(unsigned int uep1_atomic_prfx_pmr);
int iSetEP1_ATOMIC_PREFIX_INF_ep1_atomic_prfx_er(unsigned int uep1_atomic_prfx_er);
int iSetEP1_ATOMIC_PREFIX_INF_ep1_atomic_pasid(unsigned int uep1_atomic_pasid);
int iSetATOMIC_PF_VF_ep1_atomic_pf(unsigned int uep1_atomic_pf);
int iSetATOMIC_PF_VF_ep1_atomic_vf(unsigned int uep1_atomic_vf);
int iSetATOMIC_PF_VF_ep0_atomic_pf(unsigned int uep0_atomic_pf);
int iSetATOMIC_PF_VF_ep0_atomic_vf(unsigned int uep0_atomic_vf);
int iSetPORT_IDLE_STS_ep1_sts_idle(unsigned int uep1_sts_idle);
int iSetPORT_IDLE_STS_ep0_sts_idle(unsigned int uep0_sts_idle);
int iSetREMOTE_TLP_NUM_remote_np_cnt(unsigned int uremote_np_cnt);
int iSetREMOTE_TLP_NUM_remote_p_cnt(unsigned int uremote_p_cnt);
int iSetLOCAL_TLP_NUM_local_np_cnt(unsigned int ulocal_np_cnt);
int iSetLOCAL_TLP_NUM_local_p_cnt(unsigned int ulocal_p_cnt);


int iSetSQCQ_TLP_NUM_sq_np_cnt(unsigned int usq_np_cnt);
int iSetSQCQ_TLP_NUM_cq_p_cnt(unsigned int ucq_p_cnt);
int iSetCPL_NUM_remote_cpl_cnt(unsigned int uremote_cpl_cnt);
int iSetCPL_NUM_local_cpl_cnt(unsigned int ulocal_cpl_cnt);

int iSetINF_BACK_PRESS_STS_remote_np_bkpr(unsigned int uremote_np_bkpr);
int iSetINF_BACK_PRESS_STS_remote_p_bkpr(unsigned int uremote_p_bkpr);
int iSetINF_BACK_PRESS_STS_remote_cpl_bkpr(unsigned int uremote_cpl_bkpr);
int iSetINF_BACK_PRESS_STS_local_np_bkpr(unsigned int ulocal_np_bkpr);
int iSetINF_BACK_PRESS_STS_local_p_bkpr(unsigned int ulocal_p_bkpr);
int iSetINF_BACK_PRESS_STS_local_cpl_bkpr(unsigned int ulocal_cpl_bkpr);
int iSetINF_BACK_PRESS_STS_sq_np_bkpr(unsigned int usq_np_bkpr);
int iSetINF_BACK_PRESS_STS_sq_cpl_bkpr(unsigned int usq_cpl_bkpr);
int iSetINF_BACK_PRESS_STS_cq_p_bkpr(unsigned int ucq_p_bkpr);
int iSetPORT_LINKDOWN_EVENT_ep1_linkdown_cnt(unsigned int uep1_linkdown_cnt);
int iSetPORT_LINKDOWN_EVENT_ep0_linkdown_cnt(unsigned int uep0_linkdown_cnt);
int iSetDMA_MODE_SEL_dma_rc_mode_sel(unsigned int udma_rc_mode_sel);
int iSetDMA_GLOBAL_CTRL_mrrs_in_rc_mode(unsigned int umrrs_in_rc_mode);
int iSetDMA_GLOBAL_CTRL_ep1_link_down_mask(unsigned int uep1_link_down_mask);
int iSetDMA_GLOBAL_CTRL_ep0_link_down_mask(unsigned int uep0_link_down_mask);
int iSetDMA_GLOBAL_CTRL_mps_in_rc_mode(unsigned int umps_in_rc_mode);
int iSetDMA_GLOBAL_CTRL_clk_frequency(unsigned int uclk_frequency);
int iSetDMA_CH_RAS_LEVEL_dma_queue_sts_err13_level(unsigned int udma_queue_sts_err13_level);
int iSetDMA_CH_RAS_LEVEL_dma_queue_sts_err12_level(unsigned int udma_queue_sts_err12_level);
int iSetDMA_CH_RAS_LEVEL_dma_queue_sts_err11_level(unsigned int udma_queue_sts_err11_level);
int iSetDMA_CH_RAS_LEVEL_dma_queue_sts_err10_level(unsigned int udma_queue_sts_err10_level);
int iSetDMA_CH_RAS_LEVEL_dma_queue_sts_err09_level(unsigned int udma_queue_sts_err09_level);
int iSetDMA_CH_RAS_LEVEL_dma_queue_sts_err08_level(unsigned int udma_queue_sts_err08_level);
int iSetDMA_CH_RAS_LEVEL_dma_queue_sts_err07_level(unsigned int udma_queue_sts_err07_level);
int iSetDMA_CH_RAS_LEVEL_dma_queue_sts_err06_level(unsigned int udma_queue_sts_err06_level);
int iSetDMA_CH_RAS_LEVEL_dma_queue_sts_err05_level(unsigned int udma_queue_sts_err05_level);
int iSetDMA_CH_RAS_LEVEL_dma_queue_sts_err04_level(unsigned int udma_queue_sts_err04_level);
int iSetDMA_CH_RAS_LEVEL_dma_queue_sts_err03_level(unsigned int udma_queue_sts_err03_level);
int iSetDMA_CH_RAS_LEVEL_dma_queue_sts_err02_level(unsigned int udma_queue_sts_err02_level);
int iSetDMA_CH_RAS_LEVEL_dma_queue_sts_err01_level(unsigned int udma_queue_sts_err01_level);
int iSetDMA_CH_RAS_LEVEL_dma_queue_sts_err00_level(unsigned int udma_queue_sts_err00_level);
int iSetDMA_CM_RAS_LEVEL_remote_cpl_tag_err_st_level(unsigned int uremote_cpl_tag_err_st_level);
int iSetDMA_CM_RAS_LEVEL_local_cpl_tag_err_st_level(unsigned int ulocal_cpl_tag_err_st_level);
int iSetDMA_CM_RAS_LEVEL_ecc_2bit_err_st_level(unsigned int uecc_2bit_err_st_level);
int iSetDMA_CM_RAS_LEVEL_ecc_1bit_err_st_level(unsigned int uecc_1bit_err_st_level);
int iSetDMA_INT_MODE_SEL_int_mode_sel_abnormal(unsigned int uint_mode_sel_abnormal);
int iSetDMA_INT_MODE_SEL_int_mode_sel_normal(unsigned int uint_mode_sel_normal);
int iSetDMA_CH_ERR_STS_ch31_err(unsigned int uch31_err);
int iSetDMA_CH_ERR_STS_ch30_err(unsigned int uch30_err);
int iSetDMA_CH_ERR_STS_ch29_err(unsigned int uch29_err);
int iSetDMA_CH_ERR_STS_ch28_err(unsigned int uch28_err);
int iSetDMA_CH_ERR_STS_ch27_err(unsigned int uch27_err);
int iSetDMA_CH_ERR_STS_ch26_err(unsigned int uch26_err);
int iSetDMA_CH_ERR_STS_ch25_err(unsigned int uch25_err);
int iSetDMA_CH_ERR_STS_ch24_err(unsigned int uch24_err);
int iSetDMA_CH_ERR_STS_ch23_err(unsigned int uch23_err);
int iSetDMA_CH_ERR_STS_ch22_err(unsigned int uch22_err);
int iSetDMA_CH_ERR_STS_ch21_err(unsigned int uch21_err);
int iSetDMA_CH_ERR_STS_ch20_err(unsigned int uch20_err);
int iSetDMA_CH_ERR_STS_ch19_err(unsigned int uch19_err);
int iSetDMA_CH_ERR_STS_ch18_err(unsigned int uch18_err);
int iSetDMA_CH_ERR_STS_ch17_err(unsigned int uch17_err);
int iSetDMA_CH_ERR_STS_ch16_err(unsigned int uch16_err);
int iSetDMA_CH_ERR_STS_ch15_err(unsigned int uch15_err);
int iSetDMA_CH_ERR_STS_ch14_err(unsigned int uch14_err);
int iSetDMA_CH_ERR_STS_ch13_err(unsigned int uch13_err);
int iSetDMA_CH_ERR_STS_ch12_err(unsigned int uch12_err);
int iSetDMA_CH_ERR_STS_ch11_err(unsigned int uch11_err);
int iSetDMA_CH_ERR_STS_ch10_err(unsigned int uch10_err);
int iSetDMA_CH_ERR_STS_ch9_err(unsigned int uch9_err);
int iSetDMA_CH_ERR_STS_ch8_err(unsigned int uch8_err);
int iSetDMA_CH_ERR_STS_ch7_err(unsigned int uch7_err);
int iSetDMA_CH_ERR_STS_ch6_err(unsigned int uch6_err);
int iSetDMA_CH_ERR_STS_ch5_err(unsigned int uch5_err);
int iSetDMA_CH_ERR_STS_ch4_err(unsigned int uch4_err);
int iSetDMA_CH_ERR_STS_ch3_err(unsigned int uch3_err);
int iSetDMA_CH_ERR_STS_ch2_err(unsigned int uch2_err);
int iSetDMA_CH_ERR_STS_ch1_err(unsigned int uch1_err);
int iSetDMA_CH_ERR_STS_ch0_err(unsigned int uch0_err);
int iSetDMA_CH_DONE_STS_ch31_done(unsigned int uch31_done);
int iSetDMA_CH_DONE_STS_ch30_done(unsigned int uch30_done);
int iSetDMA_CH_DONE_STS_ch29_done(unsigned int uch29_done);
int iSetDMA_CH_DONE_STS_ch28_done(unsigned int uch28_done);
int iSetDMA_CH_DONE_STS_ch27_done(unsigned int uch27_done);
int iSetDMA_CH_DONE_STS_ch26_done(unsigned int uch26_done);
int iSetDMA_CH_DONE_STS_ch25_done(unsigned int uch25_done);
int iSetDMA_CH_DONE_STS_ch24_done(unsigned int uch24_done);
int iSetDMA_CH_DONE_STS_ch23_done(unsigned int uch23_done);
int iSetDMA_CH_DONE_STS_ch22_done(unsigned int uch22_done);
int iSetDMA_CH_DONE_STS_ch21_done(unsigned int uch21_done);
int iSetDMA_CH_DONE_STS_ch20_done(unsigned int uch20_done);
int iSetDMA_CH_DONE_STS_ch19_done(unsigned int uch19_done);
int iSetDMA_CH_DONE_STS_ch18_done(unsigned int uch18_done);
int iSetDMA_CH_DONE_STS_ch17_done(unsigned int uch17_done);
int iSetDMA_CH_DONE_STS_ch16_done(unsigned int uch16_done);
int iSetDMA_CH_DONE_STS_ch15_done(unsigned int uch15_done);
int iSetDMA_CH_DONE_STS_ch14_done(unsigned int uch14_done);
int iSetDMA_CH_DONE_STS_ch13_done(unsigned int uch13_done);
int iSetDMA_CH_DONE_STS_ch12_done(unsigned int uch12_done);
int iSetDMA_CH_DONE_STS_ch11_done(unsigned int uch11_done);
int iSetDMA_CH_DONE_STS_ch10_done(unsigned int uch10_done);
int iSetDMA_CH_DONE_STS_ch9_done(unsigned int uch9_done);
int iSetDMA_CH_DONE_STS_ch8_done(unsigned int uch8_done);
int iSetDMA_CH_DONE_STS_ch7_done(unsigned int uch7_done);
int iSetDMA_CH_DONE_STS_ch6_done(unsigned int uch6_done);
int iSetDMA_CH_DONE_STS_ch5_done(unsigned int uch5_done);
int iSetDMA_CH_DONE_STS_ch4_done(unsigned int uch4_done);
int iSetDMA_CH_DONE_STS_ch3_done(unsigned int uch3_done);
int iSetDMA_CH_DONE_STS_ch2_done(unsigned int uch2_done);
int iSetDMA_CH_DONE_STS_ch1_done(unsigned int uch1_done);
int iSetDMA_CH_DONE_STS_ch0_done(unsigned int uch0_done);
int iSetDMA_SQ_TAG_STS_0_dma_sq_tag_sts_0(unsigned int udma_sq_tag_sts_0);
int iSetDMA_SQ_TAG_STS_1_dma_sq_tag_sts_1(unsigned int udma_sq_tag_sts_1);
int iSetDMA_SQ_TAG_STS_2_dma_sq_tag_sts_2(unsigned int udma_sq_tag_sts_2);
int iSetDMA_SQ_TAG_STS_3_dma_sq_tag_sts_3(unsigned int udma_sq_tag_sts_3);
int iSetLOCAL_P_ID_STS_0_local_p_tag_id_status_0(unsigned int ulocal_p_tag_id_status_0);
int iSetLOCAL_P_ID_STS_1_local_p_tag_id_status_1(unsigned int ulocal_p_tag_id_status_1);
int iSetLOCAL_P_ID_STS_2_local_p_tag_id_status_2(unsigned int ulocal_p_tag_id_status_2);
int iSetLOCAL_P_ID_STS_3_local_p_tag_id_status_3(unsigned int ulocal_p_tag_id_status_3);
int iSetEP0_BLOCK_INFO_remote_ep0_p_blk(unsigned int uremote_ep0_p_blk);
int iSetEP0_BLOCK_INFO_remote_ep0_np_blk(unsigned int uremote_ep0_np_blk);
int iSetEP1_BLOCK_INFO_remote_ep1_p_blk(unsigned int uremote_ep1_p_blk);
int iSetEP1_BLOCK_INFO_remote_ep1_np_blk(unsigned int uremote_ep1_np_blk);
int iSetDMA_PREBUFF_INFO_0_dma_prebuff_entry_used(unsigned int udma_prebuff_entry_used);
int iSetDMA_CM_TABLE_INFO_0_dma_cm_table_entry_used(unsigned int udma_cm_table_entry_used);
int iSetDMA_CM_CE_RO_ecc_1bit_err_st_ce(unsigned int uecc_1bit_err_st_ce);
int iSetDMA_CM_NFE_RO_remote_cpl_tag_err_st_nfe(unsigned int uremote_cpl_tag_err_st_nfe);
int iSetDMA_CM_NFE_RO_local_cpl_tag_err_st_nfe(unsigned int ulocal_cpl_tag_err_st_nfe);
int iSetDMA_CM_NFE_RO_ecc_2bit_err_st_nfe(unsigned int uecc_2bit_err_st_nfe);
int iSetDMA_CM_NFE_RO_ecc_1bit_err_st_nfe(unsigned int uecc_1bit_err_st_nfe);
int iSetDMA_CM_FE_RO_remote_cpl_tag_err_st_fe(unsigned int uremote_cpl_tag_err_st_fe);
int iSetDMA_CM_FE_RO_local_cpl_tag_err_st_fe(unsigned int ulocal_cpl_tag_err_st_fe);
int iSetDMA_CM_FE_RO_ecc_2bit_err_st_fe(unsigned int uecc_2bit_err_st_fe);
int iSetDMA_CH_EP0_CE_RO_dma_queue_sts_err10_ep0_ce(unsigned int udma_queue_sts_err10_ep0_ce);
int iSetDMA_CH_EP0_CE_RO_dma_queue_sts_err03_ep0_ce(unsigned int udma_queue_sts_err03_ep0_ce);
int iSetDMA_CH_EP0_CE_RO_dma_queue_sts_err02_ep0_ce(unsigned int udma_queue_sts_err02_ep0_ce);
int iSetDMA_CH_EP0_CE_RO_dma_queue_sts_err01_ep0_ce(unsigned int udma_queue_sts_err01_ep0_ce);
int iSetDMA_CH_EP0_CE_RO_dma_queue_sts_err00_ep0_ce(unsigned int udma_queue_sts_err00_ep0_ce);
int iSetDMA_CH_EP0_NFE_RO_dma_queue_sts_err13_ep0_nfe(unsigned int udma_queue_sts_err13_ep0_nfe);
int iSetDMA_CH_EP0_NFE_RO_dma_queue_sts_err12_ep0_nfe(unsigned int udma_queue_sts_err12_ep0_nfe);
int iSetDMA_CH_EP0_NFE_RO_dma_queue_sts_err11_ep0_nfe(unsigned int udma_queue_sts_err11_ep0_nfe);
int iSetDMA_CH_EP0_NFE_RO_dma_queue_sts_err10_ep0_nfe(unsigned int udma_queue_sts_err10_ep0_nfe);
int iSetDMA_CH_EP0_NFE_RO_dma_queue_sts_err09_ep0_nfe(unsigned int udma_queue_sts_err09_ep0_nfe);
int iSetDMA_CH_EP0_NFE_RO_dma_queue_sts_err08_ep0_nfe(unsigned int udma_queue_sts_err08_ep0_nfe);
int iSetDMA_CH_EP0_NFE_RO_dma_queue_sts_err07_ep0_nfe(unsigned int udma_queue_sts_err07_ep0_nfe);
int iSetDMA_CH_EP0_NFE_RO_dma_queue_sts_err06_ep0_nfe(unsigned int udma_queue_sts_err06_ep0_nfe);
int iSetDMA_CH_EP0_NFE_RO_dma_queue_sts_err05_ep0_nfe(unsigned int udma_queue_sts_err05_ep0_nfe);
int iSetDMA_CH_EP0_NFE_RO_dma_queue_sts_err04_ep0_nfe(unsigned int udma_queue_sts_err04_ep0_nfe);
int iSetDMA_CH_EP0_NFE_RO_dma_queue_sts_err03_ep0_nfe(unsigned int udma_queue_sts_err03_ep0_nfe);
int iSetDMA_CH_EP0_NFE_RO_dma_queue_sts_err02_ep0_nfe(unsigned int udma_queue_sts_err02_ep0_nfe);
int iSetDMA_CH_EP0_NFE_RO_dma_queue_sts_err01_ep0_nfe(unsigned int udma_queue_sts_err01_ep0_nfe);
int iSetDMA_CH_EP0_NFE_RO_dma_queue_sts_err00_ep0_nfe(unsigned int udma_queue_sts_err00_ep0_nfe);
int iSetDMA_CH_EP0_FE_RO_dma_queue_sts_err13_ep0_fe(unsigned int udma_queue_sts_err13_ep0_fe);
int iSetDMA_CH_EP0_FE_RO_dma_queue_sts_err12_ep0_fe(unsigned int udma_queue_sts_err12_ep0_fe);
int iSetDMA_CH_EP0_FE_RO_dma_queue_sts_err11_ep0_fe(unsigned int udma_queue_sts_err11_ep0_fe);
int iSetDMA_CH_EP0_FE_RO_dma_queue_sts_err09_ep0_fe(unsigned int udma_queue_sts_err09_ep0_fe);
int iSetDMA_CH_EP0_FE_RO_dma_queue_sts_err08_ep0_fe(unsigned int udma_queue_sts_err08_ep0_fe);
int iSetDMA_CH_EP0_FE_RO_dma_queue_sts_err07_ep0_fe(unsigned int udma_queue_sts_err07_ep0_fe);
int iSetDMA_CH_EP0_FE_RO_dma_queue_sts_err06_ep0_fe(unsigned int udma_queue_sts_err06_ep0_fe);
int iSetDMA_CH_EP0_FE_RO_dma_queue_sts_err05_ep0_fe(unsigned int udma_queue_sts_err05_ep0_fe);
int iSetDMA_CH_EP0_FE_RO_dma_queue_sts_err04_ep0_fe(unsigned int udma_queue_sts_err04_ep0_fe);
int iSetDMA_CH_EP1_CE_RO_dma_queue_sts_err10_ep1_ce(unsigned int udma_queue_sts_err10_ep1_ce);
int iSetDMA_CH_EP1_CE_RO_dma_queue_sts_err03_ep1_ce(unsigned int udma_queue_sts_err03_ep1_ce);
int iSetDMA_CH_EP1_CE_RO_dma_queue_sts_err02_ep1_ce(unsigned int udma_queue_sts_err02_ep1_ce);
int iSetDMA_CH_EP1_CE_RO_dma_queue_sts_err01_ep1_ce(unsigned int udma_queue_sts_err01_ep1_ce);
int iSetDMA_CH_EP1_CE_RO_dma_queue_sts_err00_ep1_ce(unsigned int udma_queue_sts_err00_ep1_ce);
int iSetDMA_CH_EP1_NFE_RO_dma_queue_sts_err13_ep1_nfe(unsigned int udma_queue_sts_err13_ep1_nfe);
int iSetDMA_CH_EP1_NFE_RO_dma_queue_sts_err12_ep1_nfe(unsigned int udma_queue_sts_err12_ep1_nfe);
int iSetDMA_CH_EP1_NFE_RO_dma_queue_sts_err11_ep1_nfe(unsigned int udma_queue_sts_err11_ep1_nfe);
int iSetDMA_CH_EP1_NFE_RO_dma_queue_sts_err10_ep1_nfe(unsigned int udma_queue_sts_err10_ep1_nfe);
int iSetDMA_CH_EP1_NFE_RO_dma_queue_sts_err09_ep1_nfe(unsigned int udma_queue_sts_err09_ep1_nfe);
int iSetDMA_CH_EP1_NFE_RO_dma_queue_sts_err08_ep1_nfe(unsigned int udma_queue_sts_err08_ep1_nfe);
int iSetDMA_CH_EP1_NFE_RO_dma_queue_sts_err07_ep1_nfe(unsigned int udma_queue_sts_err07_ep1_nfe);
int iSetDMA_CH_EP1_NFE_RO_dma_queue_sts_err06_ep1_nfe(unsigned int udma_queue_sts_err06_ep1_nfe);
int iSetDMA_CH_EP1_NFE_RO_dma_queue_sts_err05_ep1_nfe(unsigned int udma_queue_sts_err05_ep1_nfe);
int iSetDMA_CH_EP1_NFE_RO_dma_queue_sts_err04_ep1_nfe(unsigned int udma_queue_sts_err04_ep1_nfe);
int iSetDMA_CH_EP1_NFE_RO_dma_queue_sts_err03_ep1_nfe(unsigned int udma_queue_sts_err03_ep1_nfe);
int iSetDMA_CH_EP1_NFE_RO_dma_queue_sts_err02_ep1_nfe(unsigned int udma_queue_sts_err02_ep1_nfe);
int iSetDMA_CH_EP1_NFE_RO_dma_queue_sts_err01_ep1_nfe(unsigned int udma_queue_sts_err01_ep1_nfe);
int iSetDMA_CH_EP1_NFE_RO_dma_queue_sts_err00_ep1_nfe(unsigned int udma_queue_sts_err00_ep1_nfe);
int iSetDMA_CH_EP1_FE_RO_dma_queue_sts_err13_ep0_fe(unsigned int udma_queue_sts_err13_ep0_fe);
int iSetDMA_CH_EP1_FE_RO_dma_queue_sts_err12_ep0_fe(unsigned int udma_queue_sts_err12_ep0_fe);
int iSetDMA_CH_EP1_FE_RO_dma_queue_sts_err11_ep0_fe(unsigned int udma_queue_sts_err11_ep0_fe);
int iSetDMA_CH_EP1_FE_RO_dma_queue_sts_err09_ep1_fe(unsigned int udma_queue_sts_err09_ep1_fe);
int iSetDMA_CH_EP1_FE_RO_dma_queue_sts_err08_ep1_fe(unsigned int udma_queue_sts_err08_ep1_fe);
int iSetDMA_CH_EP1_FE_RO_dma_queue_sts_err07_ep1_fe(unsigned int udma_queue_sts_err07_ep1_fe);
int iSetDMA_CH_EP1_FE_RO_dma_queue_sts_err06_ep1_fe(unsigned int udma_queue_sts_err06_ep1_fe);
int iSetDMA_CH_EP1_FE_RO_dma_queue_sts_err05_ep1_fe(unsigned int udma_queue_sts_err05_ep1_fe);
int iSetDMA_CH_EP1_FE_RO_dma_queue_sts_err04_ep1_fe(unsigned int udma_queue_sts_err04_ep1_fe);

#endif // __HIPCIEC_AP_DMA_REG_C_UNION_DEFINE_H__
