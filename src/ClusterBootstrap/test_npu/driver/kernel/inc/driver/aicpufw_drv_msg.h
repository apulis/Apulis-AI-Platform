/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2019-2021. All rights reserved.
 * Description: aicpufw header
 * Author: huawei
 * Create: 2019-12-25
 */

#ifndef __AICPUFW_DRV_MSG_H
#define __AICPUFW_DRV_MSG_H

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

typedef struct drv_hwts_cce_kernel {
    unsigned long long kernelName;
    unsigned long long kernelSo;
    unsigned long long paramBase;
    unsigned long long l2VaddrBase;
    unsigned int blockId;
    unsigned int blockNum;
    unsigned int l2Size;
    unsigned int l2InMain;
} drv_aicpufw_cce_kernel_t;

typedef struct drv_hwts_cce_kernel_hwts {
    unsigned long long kernelName;
    unsigned long long kernelSo;
    unsigned long long paramBase;
    unsigned long long l2VaddrBase;
    unsigned long long l2Ctrl;
    unsigned short blockId;
    unsigned short blockNum;
    unsigned int l2InMain;
    unsigned long long extra_field;
} drv_aicpufw_cce_kernel_hwts_t;

typedef struct drv_hwts_fwk_kernel {
    unsigned long long kernel; // a pointer point to kernel struct
    unsigned int size;
} drv_aicpufw_fwk_kernel_t;

typedef struct {
     volatile unsigned short model_id;
     volatile unsigned short stream_id;
     volatile unsigned short task_id;
     volatile unsigned char reserved[2];
} drv_aicpufw_datadump_t;

typedef struct {
    volatile unsigned long long dumpinfoPtr;
    volatile unsigned int length;
    volatile unsigned short stream_id;
    volatile unsigned short task_id;
} drv_aicpufw_datadump_loadinfo_t;

typedef struct{
     volatile unsigned int pid;
     volatile unsigned char cmd_type;
     volatile unsigned char seq;
     volatile unsigned char tid;
     volatile unsigned char ts_id;
     union
     {
        drv_aicpufw_datadump_t aicpu_datadump;
        drv_aicpufw_datadump_loadinfo_t aicpu_datadump_loadinfo;
     }u;
} drv_aicpu_tsctl_kernel_t;

typedef enum {
    AICPUFW_KERNEL_TYPE_CCE = 0,
    AICPUFW_KERNEL_TYPE_FWK = 1,
    AICPUFW_KERENEL_TYPE_CPU = 2,
    AICPUFW_KERNEL_TYPE_TSCTL = 3,
    AICPUFW_KERNEL_TYPE_RESERVED
} drv_aicpufw_kernelType_t;

/*Different kernelTypes match different kernelBase. In any case, must check the kernelType,
and use the corresponding kernelBase according to the different kernelType.*/
typedef struct drv_hwts_ts_kernel {
    unsigned int kernelType;
    union {
        drv_aicpufw_cce_kernel_t cce_kernel;
        drv_aicpufw_fwk_kernel_t fwk_kernel;
        drv_aicpufw_cce_kernel_hwts_t cce_kernel_hwts;
#ifndef CFG_SOC_PLATFORM_CLOUD
        struct hwts_ts_kernel hwts_kernel;
#endif
        drv_aicpu_tsctl_kernel_t tsctl_kernel;
    } kernelBase;
} drv_aicpufw_ts_kernel_t;

typedef struct drv_hwts_ts_task {
    volatile unsigned int valid;
    pid_t pid;
    volatile unsigned long long serial_no;
    drv_aicpufw_ts_kernel_t kernel_info;
} drv_aicpufw_ts_task_t;

#define RESERVED_ARRAY_SIZE         11
typedef struct drv_hwts_task_response {
    volatile unsigned int valid;
    volatile unsigned int state;
    volatile unsigned long long serial_no;
    volatile unsigned int reserved[RESERVED_ARRAY_SIZE];
} drv_aicpufw_task_response_t;

/*=========================== New add below===========================*/
/* aicpu_task -- event */
typedef struct hwts_event_msg {
    unsigned int mailbox_id;
    drv_aicpufw_ts_task_t mailbox;
} hwts_event_msg_t;

typedef struct hwts_response {
    unsigned int result;        /* RESPONSE_RESULE_E */
    unsigned int mailbox_id;
    unsigned long long serial_no;
} hwts_response_t;

#endif
