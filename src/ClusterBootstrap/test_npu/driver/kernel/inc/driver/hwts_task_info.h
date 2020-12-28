/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2017-2020. All rights reserved.
 * Description:
 * Author: Huawei
 * Create: 2020-01-15
 */
#ifndef __HWTS_DRV_TASK_INFO_H
#define __HWTS_DRV_TASK_INFO_H

struct hwts_ts_kernel {
    pid_t pid;
    unsigned short kernel_type;
    unsigned short streamID;
    unsigned long long kernelName;
    unsigned long long kernelSo;
    unsigned long long paramBase;
    unsigned long long l2VaddrBase;
    unsigned long long l2Ctrl;
    unsigned short blockId;
    unsigned short blockNum;
    unsigned int l2InMain;
    unsigned long long extra_field;
};

struct drv_hwts_ts_kernel {
    unsigned int kernelType;
    union {
        struct hwts_ts_kernel hwts_kernel;
    } kernelBase;
};

struct drv_hwts_ts_task {
    volatile unsigned int valid;
    pid_t pid;
    volatile unsigned long long serial_no;
    struct drv_hwts_ts_kernel kernel_info;
};

struct hwts_event_msg {
    unsigned int mailbox_id;
    struct drv_hwts_ts_task mailbox;
};

struct hwts_response {
    unsigned int result;
    unsigned int mailbox_id;
    unsigned long long serial_no;
};

#endif

