/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2012-2019. All rights reserved.
 * Description:
 * Author: huawei
 * Create: 2017/8/18
 */

#ifndef _DEVDRV_COMMON_MSG_H_
#define _DEVDRV_COMMON_MSG_H_

#include "devdrv_pci.h"

#define DEVDRV_COMMON_MSG_CLIENT_CNT 64
#define DEVDRV_COMMON_WORK_RESQ_TIME 10

struct devdrv_common_msg_stat {
    u64 tx_total_cnt;
    u64 tx_success_cnt;
    u64 tx_einval_err;
    u64 tx_enodev_err;
    u64 tx_enosys_err;
    u64 tx_etimedout_err;
    u64 tx_default_err;
    u64 rx_total_cnt;
    u64 rx_success_cnt;
    u64 rx_para_err;
    u64 rx_work_max_time;
    u64 rx_work_delay_cnt;
};

struct devdrv_common_msg {
    struct devdrv_msg_chan *msg_chan;
    int (*common_fun[DEVDRV_COMMON_MSG_TYPE_MAX])(u32 devid, void *data, u32 in_data_len, u32 out_data_len,
                                                  u32 *real_out_len);
    struct devdrv_common_msg_stat com_msg_stat[DEVDRV_COMMON_MSG_TYPE_MAX];
};

int devdrv_alloc_common_msg_queue(struct devdrv_pci_ctrl *pci_ctrl);
void devdrv_free_common_msg_queue(struct devdrv_pci_ctrl *pci_ctrl);

#endif
