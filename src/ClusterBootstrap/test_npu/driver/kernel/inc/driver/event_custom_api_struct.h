/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2012-2019. All rights reserved.
 * Description:
 * Author: huawei
 * Create: 2020-02-12
 */
#ifndef AICPUFW_CUSTOM_API_STRUCT_H
#define AICPUFW_CUSTOM_API_STRUCT_H

#ifdef __cplusplus
extern "C" {
#endif

struct event_ack {
    unsigned int device_id;
    unsigned int event_id;
    unsigned int subevent_id;
    char* msg;
    unsigned int msg_len;
};

#ifdef __cplusplus
}
#endif
#endif