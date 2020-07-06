/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2012-2019. All rights reserved.
 * Description:
 * Author: huawei
 * Create: 2019-10-15
 */
#ifndef AICPUFW_API_H
#define AICPUFW_API_H

#include <sys/types.h>
#include "event_sched_api.h"
#include "ascend_hal.h"
#ifdef __cplusplus
extern "C" {
#endif

typedef enum tagAicpufwPlat {
    AICPUFW_ONLINE_PLAT = 0,
    AICPUFW_OFFLINE_PLAT,
    AICPUFW_MAX_PLAT,
} aicpufwPlat_t;

typedef struct {
    unsigned int chip_id;
    unsigned int aicpu_index;
    unsigned int ts_index;
    void *custom_param;
} aicpufw_thread_data_t;

//typedef EVENT_ID_E EVENT_ID;

// custom aicpufw thread task proc function
typedef void *(*FUNC_AICPU_THREAD_TASK_PROC)(void *);

drvError_t drvSetAicpuChipID(uint32_t chip_id);
drvError_t drvCreateAicpuWorkTasks(pid_t pid, int32_t mode);
drvError_t drvDevBindPid(pid_t hostpid, const char *sign, uint32_t len, int32_t mode);
int32_t aicpufw_get_aicpu(uint32_t chip_id, uint32_t *aicpu_num);
int32_t aicpufw_active_ts_stream(uint32_t streamid, uint32_t modelId);
int32_t aicpufw_dev_destroy_model(uint32_t modelId);
int32_t aicpufw_dev_get_event(uint32_t chip_id, uint32_t thread_index, uint32_t timeout, struct event_info *event);
int32_t aicpufw_dev_submit_event(uint32_t chip_id, struct event_summary *event);
int32_t aicpufw_dev_subscribe_event(uint32_t chip_id, uint32_t event_mask);
int32_t aicpufw_dev_set_process_priority(uint32_t chip_id, uint32_t pid, SCHEDULE_PRIORITY priority);
int32_t aicpufw_dev_set_process_weight(uint32_t chip_id, uint32_t pid, uint32_t weight);

void set_custom_aicpufw_thread_task_proc(unsigned int chip_id, FUNC_AICPU_THREAD_TASK_PROC custom_func,
                                         void *custom_param, unsigned int ts_ind);
int aicpufw_thread_callback(pid_t tid, unsigned int chip_id, unsigned int aicpu_index, unsigned int ts_ind);
int get_dcpu_range(unsigned int chip_id, unsigned int *dcpu_start, unsigned int *dcpu_num, unsigned int ts_ind);

#ifdef __cplusplus
}
#endif
#endif
