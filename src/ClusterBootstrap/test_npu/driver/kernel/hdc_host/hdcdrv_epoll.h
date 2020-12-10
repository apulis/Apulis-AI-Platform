/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2012-2019. All rights reserved.
 * Description:
 * Author: huawei
 * Create: 2019/6/18
 *
 * This program is free software; you can redistribute it and /or modify it
 * under the terms of the GNU General Public License as published by the Free
 * Software Foundation; either version 2 of the License, or (at your option)
 * any later version
 */

#ifndef _HDCDRV_EPOLL_H_
#define _HDCDRV_EPOLL_H_

#include <linux/mutex.h>
#include <linux/spinlock.h>
#include <linux/semaphore.h>
#include <linux/wait.h>

#include "devdrv_manager_comm.h"
#include "hdcdrv_cmd.h"

#define HDCDRV_EPOLL_FD_NUM 128
#define HDCDRV_EPOLL_FD_EVENT_NUM 256
#define HDCDRV_INV_DIVISOR 0x1999999A /* this is (2^32)/10 */
#define HDCDRV_EPOLL_FD_EVENT_NUM 256

#define HDCDRV_DOCKER_MAX_NUM (MAX_DOCKER_NUM + 1) /* 1(host)+69(MAX_DOCKER_NUM) */
#define HDCDRV_PHY_HOST_ID (MAX_DOCKER_NUM) /* host id */


struct hdcdrv_ctx;

struct hdcdrv_epoll_list_node {
    struct hdcdrv_event events;
    void *instance;
    char session_id[HDCDRV_SID_LEN];
    struct list_head list;
};

struct hdcdrv_epoll_fd {
    int valid;
    int fd;
    int docker_id;
    int magic_num;
    int size;
    int event_num;
    int wait_flag;
    struct mutex mutex;
    wait_queue_head_t wq;
    struct list_head service_list;
    struct list_head session_list;
    struct hdcdrv_event *events;
    struct hdcdrv_ctx *ctx;
};

struct hdcdrv_epoll {
    struct mutex mutex;
    struct hdcdrv_epoll_fd epfds[HDCDRV_DOCKER_MAX_NUM][HDCDRV_EPOLL_FD_NUM];
};

extern void hdcdrv_epoll_recycle_fd(struct hdcdrv_ctx *ctx);

extern void hdcdrv_epoll_wake_up(struct hdcdrv_epoll_fd *epfd);
extern void hdcdrv_epoll_init(struct hdcdrv_epoll *epolls);

#endif  // _HDCDRV_EPOLL_H_
