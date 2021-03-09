/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2012-2019. All rights reserved.
 * Description:
 * Author: huawei
 * Create: 2017/8/18
 */

#ifndef _DEVDRV_SYSFS_H_
#define _DEVDRV_SYSFS_H_

#include <linux/interrupt.h>
#include <linux/module.h>
#include <linux/pci.h>
#include <linux/types.h>
#include <linux/workqueue.h>

#include "devdrv_device_load.h"
#include "devdrv_atu.h"
#include "devdrv_dma.h"
#include "devdrv_common_msg.h"
#include "securec.h"

#define DEVDRV_SYSFS_RX_LANE_MAX 4
#define DEVDRV_SYSFS_TX_LANE_MAX 4

#define DEVDRV_SYSFS_MSG_IN_BYTES 4
#define DEVDRV_SYSFS_MSG_OUT_DATA_LEN 128

#define DEVDRV_NVE1_MOVE 4
#define DEVDRV_NVE1_BIT 0x0000000F
#define DEVDRV_NVE2_BIT 0x0000000F
#define DEVDRV_NVE3_MOVE 8
#define DEVDRV_NVE3_BIT 0x000000FF
#define DEVDRV_NVE4_1_MOVE 8
#define DEVDRV_NVE4_1_BIT 0x0000FF00
#define DEVDRV_NVE4_2_MOVE 24
#define DEVDRV_NVE4_2_BIT 0x000000FF
#define DEVDRV_INDEX_0 0
#define DEVDRV_INDEX_1 1
#define DEVDRV_INDEX_2 2
#define DEVDRV_INDEX_3 3

#define DEVDRV_SYSFS_DMA_CHAN_NUM 6


#define DEVDRV_PCIVNIC  "   vnic"
#define DEVDRV_SMMU     "   smmu"
#define DEVDRV_DEVMM    "  devmm"
#define DEVDRV_PROFILE  "profile"
#define DEVDRV_DEVMAN   " devman"
#define DEVDRV_HDC      "    hdc"
#define DEVDRV_SYSFS    "  sysfs"
#define DEVDRV_COMMON   " common"
#define DEVDRV_RESERVE  "reserve"



enum devdrv_sysfs_msg_type {
    DEVDRV_SYSFS_RX_PARA = 0,
    DEVDRV_SYSFS_TX_PARA,
    DEVDRV_SYSFS_LINK_INFO,
    DEVDRV_SYSFS_AER_COUNT,
    DEVDRV_SYSFS_AER_CLEAR,
    DEVDRV_SYSFS_COMMON_MSG,
    DEVDRV_SYSFS_NON_TRANS_MSG,
    DEVDRV_SYSFS_SYNC_DMA_INFO,
    DEVDRV_SYSFS_MSG_TYPE_MAX
};

struct devdrv_sysfs_link_info {
    enum devdrv_sysfs_msg_type type;
    u32 link_speed;
    u32 link_width;
    u32 link_status;
};

struct devdrv_sysfs_rx_lane_para {
    u32 att;
    u32 gain;
    u32 boost;
    u32 tap1;
    u32 tap2;
    u32 valid;
};

struct devdrv_sysfs_rx_para {
    u32 lane_count;
    struct devdrv_sysfs_rx_lane_para lane_rx_para[DEVDRV_SYSFS_RX_LANE_MAX];
};

struct devdrv_sysfs_tx_lane_para {
    u32 pre;
    u32 main;
    u32 post;
    u32 valid;
};

struct devdrv_sysfs_tx_para {
    u32 lane_count;
    struct devdrv_sysfs_tx_lane_para lane_tx_para[DEVDRV_SYSFS_TX_LANE_MAX];
};

struct devdrv_device_non_trans_stat {
    u64 msg_type;
    u64 tx_total_cnt;
    u64 tx_success_cnt;
    u64 tx_len_check_err;
    u64 tx_reply_len_check_err;
    u64 tx_dma_copy_err;
    u64 tx_timeout_err;
    u64 tx_process_err;
    u64 tx_invalid_para_err;
    u64 rx_total_cnt;
    u64 rx_success_cnt;
    u64 rx_para_err;
    u64 rx_work_max_time;
    u64 rx_work_delay_cnt;
};


struct devdrv_sysfs_msg {
    enum devdrv_sysfs_msg_type type;
    union {
        u32 data[DEVDRV_SYSFS_MSG_OUT_DATA_LEN];
        struct devdrv_sysfs_link_info link_info;
        struct devdrv_sysfs_rx_para rx_para;
        struct devdrv_sysfs_tx_para tx_para;
        struct devdrv_common_msg_stat common_stat[DEVDRV_COMMON_MSG_TYPE_MAX];
        struct devdrv_device_non_trans_stat non_trans_stat[devdrv_msg_client_max];
        struct devdrv_sync_dma_stat sync_dma_stat[DEVDRV_SYSFS_DMA_CHAN_NUM];
    };
};

struct devdrv_msg_chan *devdrv_get_common_msg_chan_by_id(u32 dev_id);

#endif
