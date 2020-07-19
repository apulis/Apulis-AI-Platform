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
#include <linux/jiffies.h>
#include <linux/wait.h>
#include <linux/sched.h>
#include <linux/uaccess.h>
#include <linux/slab.h>

#include "hdcdrv_cmd.h"
#include "hdcdrv_epoll.h"
#include "hdcdrv_core.h"

struct hdcdrv_epoll *hdc_epoll = NULL;

long hdcdrv_epoll_fd_check(int fd, u32 docker_id, int magic_num)
{
    struct hdcdrv_epoll_fd *epfd = NULL;

    if ((fd >= HDCDRV_EPOLL_FD_NUM) || (fd < 0)) {
        hdcdrv_err("fd %d is illegal, magic %x\n", fd, magic_num);
        return HDCDRV_PARA_ERR;
    }

    if (docker_id >= HDCDRV_DOCKER_MAX_NUM) {
        hdcdrv_err("docker_id %d is illegal, magic %x\n", docker_id, magic_num);
        return HDCDRV_PARA_ERR;
    }

    epfd = &hdc_epoll->epfds[docker_id][fd];

    if (epfd->valid == HDCDRV_INVALID) {
        hdcdrv_err("epfd %d is invalid, magic %x\n", fd, magic_num);
        return HDCDRV_PARA_ERR;
    }

    if (epfd->magic_num != magic_num) {
        hdcdrv_err("epfd %d magic_num is not correct\n", fd);
        return HDCDRV_PARA_ERR;
    }

    return HDCDRV_OK;
}

long hdcdrv_epoll_alloc_fd(struct hdcdrv_ctx *ctx, struct hdcdrv_cmd_epoll_alloc_fd *cmd)
{
    struct hdcdrv_event *event = NULL;
    struct hdcdrv_epoll_fd *epfd = NULL;
    int i;
    int fd = -1;
    u32 docker_id;

    if (((cmd->size > HDCDRV_EPOLL_FD_EVENT_NUM) || (cmd->size <= 0))) {
        hdcdrv_err("size %d invalid.\n", cmd->size);
        return HDCDRV_PARA_ERR;
    }

    docker_id = hdcdrv_get_container_id();
    if (docker_id >= HDCDRV_DOCKER_MAX_NUM) {
        hdcdrv_err("docker_id %u illegal.\n", docker_id);
        return HDCDRV_ERR;
    }

    /*lint -e647 */
    event = (struct hdcdrv_event *)kzalloc((u64)(sizeof(struct hdcdrv_event) * cmd->size), GFP_KERNEL | __GFP_ACCOUNT);
    /*lint +e647 */
    if (event == NULL) {
        hdcdrv_err("malloc failed.\n");
        return HDCDRV_ERR;
    }

    mutex_lock(&hdc_epoll->mutex);

    /* First find the session in the idle state */
    for (i = 0; i < HDCDRV_EPOLL_FD_NUM; i++) {
        epfd = &hdc_epoll->epfds[docker_id][i];
        if (epfd->valid == HDCDRV_INVALID) {
            fd = i;
            epfd->valid = HDCDRV_VALID;
            break;
        }
    }

    mutex_unlock(&hdc_epoll->mutex);

    if (fd < 0) {
        kfree(event);
        event = NULL;
        hdcdrv_err("no more epfd.\n");
        return HDCDRV_NO_EPOLL_FD;
    }

    cmd->epfd = fd;
    hdcdrv_sid_gen((char *)&cmd->magic_num, sizeof(cmd->magic_num));

    if (epfd != NULL) { /* just for clear fortify warning */
        epfd->magic_num = cmd->magic_num;
        epfd->size = cmd->size;
        epfd->event_num = 0;
        epfd->wait_flag = 0;
        epfd->events = event;
        if (ctx != NULL) {
            epfd->ctx = ctx;
            ctx->epfd = epfd;
        }
    }

    return HDCDRV_OK;
}

void hdcdrv_epoll_clear_service(struct hdcdrv_epoll_fd *epfd)
{
    struct list_head *pos = NULL, *n = NULL;
    struct hdcdrv_epoll_list_node *node = NULL;
    struct hdcdrv_service *service = NULL;

    if (!list_empty_careful(&epfd->service_list)) {
        list_for_each_safe(pos, n, &epfd->service_list)
        {
            node = list_entry(pos, struct hdcdrv_epoll_list_node, list);
            service = (struct hdcdrv_service *)node->instance;
            service->epfd = NULL;
            list_del(&node->list);
            kfree(node);
            node = NULL;
        }
    }
}

void hdcdrv_epoll_clear_session(struct hdcdrv_epoll_fd *epfd)
{
    struct list_head *pos = NULL, *n = NULL;
    struct hdcdrv_epoll_list_node *node = NULL;
    struct hdcdrv_session *session = NULL;

    if (!list_empty_careful(&epfd->session_list)) {
        list_for_each_safe(pos, n, &epfd->session_list)
        {
            node = list_entry(pos, struct hdcdrv_epoll_list_node, list);
            session = (struct hdcdrv_session *)node->instance;
            session->epfd = NULL;
            list_del(&node->list);
            kfree(node);
            node = NULL;
        }
    }
}

void hdcdrv_epoll_free_fd_handle(struct hdcdrv_ctx *ctx, struct hdcdrv_epoll_fd *epfd)
{
    mutex_lock(&epfd->mutex);
    hdcdrv_epoll_clear_service(epfd);
    hdcdrv_epoll_clear_session(epfd);
    mutex_unlock(&epfd->mutex);

    mutex_lock(&hdc_epoll->mutex);
    if (epfd->valid == HDCDRV_VALID) {
        kfree(epfd->events);
        epfd->events = NULL;
        epfd->valid = HDCDRV_INVALID;
        if (ctx != NULL) {
            ctx->epfd = NULL;
            epfd->ctx = NULL;
        }
    }
    mutex_unlock(&hdc_epoll->mutex);
}

long hdcdrv_epoll_free_fd(struct hdcdrv_ctx *ctx, struct hdcdrv_cmd_epoll_free_fd *cmd)
{
    struct hdcdrv_epoll_fd *epfd = NULL;
    long ret;
    u32 docker_id;

    docker_id = hdcdrv_get_container_id();

    ret = hdcdrv_epoll_fd_check(cmd->epfd, docker_id, cmd->magic_num);
    if (ret != HDCDRV_OK) {
        hdcdrv_err("para error.\n");
        return ret;
    }

    epfd = &hdc_epoll->epfds[docker_id][cmd->epfd];

    hdcdrv_epoll_free_fd_handle(ctx, epfd);
    return HDCDRV_OK;
}

void hdcdrv_epoll_recycle_fd(struct hdcdrv_ctx *ctx)
{
    if (ctx->epfd != NULL) {
        hdcdrv_info("recycle epoll fd %d.\n", ctx->epfd->fd);
        hdcdrv_epoll_free_fd_handle(ctx, ctx->epfd);
    }
}

long hdcdrv_epoll_add_service(struct hdcdrv_epoll_fd *epfd, struct hdcdrv_epoll_list_node *node,
                              int dev_id, int service_type)
{
    struct hdcdrv_service *service = NULL;
    long ret;

    ret = hdcdrv_dev_para_check(dev_id, service_type);
    if (ret) {
        hdcdrv_err("epfd %d dev_id %d service_type %d invalid.\n", epfd->fd, dev_id, service_type);
        return ret;
    }

    service = &hdc_ctrl->devices[dev_id].service[service_type];
    if (service->listen_status == HDCDRV_INVALID) {
        hdcdrv_err("epfd %d dev_id %d service_type %d not listen.\n", epfd->fd, dev_id, service_type);
        return HDCDRV_ERR;
    }

    if (service->epfd != NULL) {
        hdcdrv_err("epfd %d dev_id %d service_type %d has been polled.\n", epfd->fd, dev_id, service_type);
        return HDCDRV_ERR;
    }

    service->epfd = epfd;
    node->instance = (void *)service;
    list_add(&node->list, &epfd->service_list);

    return HDCDRV_OK;
}

long hdcdrv_epoll_add_session(struct hdcdrv_epoll_fd *epfd, struct hdcdrv_epoll_list_node *node,
                              int session_fd, const char *session_id)
{
    struct hdcdrv_session *session = NULL;
    long ret;

    ret = hdcdrv_session_alive_check(session_fd, (char *)session_id);
    if (ret) {
        hdcdrv_err("epfd %d session_fd %d invalid.\n", epfd->fd, session_fd);
        return ret;
    }

    session = &hdc_ctrl->sessions[session_fd];

    if (session->epfd != NULL) {
        hdcdrv_err("epfd %d session_fd %d has been polled.\n", epfd->fd, session_fd);
        return HDCDRV_ERR;
    }

    session->epfd = epfd;
    node->instance = (void *)session;
    hdcdrv_sid_copy(node->session_id, HDCDRV_SID_LEN, (char *)session_id, HDCDRV_SID_LEN);
    list_add(&node->list, &epfd->session_list);

    return HDCDRV_OK;
}

long hdcdrv_epoll_add_event(struct hdcdrv_epoll_fd *epfd, struct hdcdrv_event *event, int para1, const char *para2)
{
    struct hdcdrv_epoll_list_node *node = NULL;
    long ret;

    node = (struct hdcdrv_epoll_list_node *)kzalloc(sizeof(struct hdcdrv_epoll_list_node), GFP_KERNEL | __GFP_ACCOUNT);
    if (node == NULL) {
        hdcdrv_err("malloc failed.\n");
        return HDCDRV_ERR;
    }

    node->events = *event;

    mutex_lock(&epfd->mutex);

    if (epfd->event_num >= epfd->size) {
        hdcdrv_err("epfd %d event full.\n", epfd->fd);
        ret = HDCDRV_ERR;
        goto ERROR;
    }

    if (event->events & HDCDRV_EPOLL_CONN_IN) {
        ret = hdcdrv_epoll_add_service(epfd, node, para1, *(int *)para2);
        if (ret) {
            goto ERROR;
        }
    } else {
        ret = hdcdrv_epoll_add_session(epfd, node, para1, para2);
        if (ret) {
            goto ERROR;
        }
    }

    epfd->event_num++;

    mutex_unlock(&epfd->mutex);

    return HDCDRV_OK;

ERROR:
    mutex_unlock(&epfd->mutex);
    kfree(node);
    node = NULL;
    return ret;
}

long hdcdrv_epoll_del_service(struct hdcdrv_epoll_fd *epfd, int dev_id, int service_type)
{
    struct list_head *pos = NULL, *n = NULL;
    struct hdcdrv_epoll_list_node *node = NULL;
    struct hdcdrv_service *service = NULL;
    long ret = HDCDRV_ERR;

    if ((dev_id >= HDCDRV_SURPORT_MAX_DEV) || (dev_id < 0)) {
        hdcdrv_err("dev_id %d is illegal\n", dev_id);
        return HDCDRV_PARA_ERR;
    }

    if ((service_type >= HDCDRV_SURPORT_MAX_SERVICE) || (service_type < 0)) {
        hdcdrv_err("dev %d service_type %d is illegal\n", dev_id, service_type);
        return HDCDRV_PARA_ERR;
    }

    service = &hdc_ctrl->devices[dev_id].service[service_type];
    if (!list_empty_careful(&epfd->service_list)) {
        list_for_each_safe(pos, n, &epfd->service_list)
        {
            node = list_entry(pos, struct hdcdrv_epoll_list_node, list);
            if ((struct hdcdrv_service *)node->instance == service) {
                service->epfd = NULL;
                list_del(&node->list);
                kfree(node);
                node = NULL;
                ret = HDCDRV_OK;
                break;
            }
        }
    }

    return ret;
}

long hdcdrv_epoll_del_session(struct hdcdrv_epoll_fd *epfd, int session_fd, const char *session_id)
{
    struct list_head *pos = NULL, *n = NULL;
    struct hdcdrv_epoll_list_node *node = NULL;
    struct hdcdrv_session *session = NULL;
    long ret = HDCDRV_ERR;

    if ((session_fd >= HDCDRV_SURPORT_MAX_SESSION) || (session_fd < 0)) {
        hdcdrv_err("session_fd %d is illegal\n", session_fd);
        return HDCDRV_PARA_ERR;
    }

    session = &hdc_ctrl->sessions[session_fd];
    if (!list_empty_careful(&epfd->session_list)) {
        list_for_each_safe(pos, n, &epfd->session_list)
        {
            node = list_entry(pos, struct hdcdrv_epoll_list_node, list);
            if ((struct hdcdrv_session *)node->instance == session) {
                session->epfd = NULL;
                list_del(&node->list);
                kfree(node);
                node = NULL;
                ret = HDCDRV_OK;
                break;
            }
        }
    }

    return ret;
}

long hdcdrv_epoll_del_event(struct hdcdrv_epoll_fd *epfd, struct hdcdrv_event *event, int para1, const char *para2)
{
    long ret;

    mutex_lock(&epfd->mutex);

    if (event->events & HDCDRV_EPOLL_CONN_IN) {
        ret = hdcdrv_epoll_del_service(epfd, para1, *(int *)para2);
    } else {
        ret = hdcdrv_epoll_del_session(epfd, para1, para2);
    }

    if (ret == HDCDRV_OK) {
        epfd->event_num--;
    }

    mutex_unlock(&epfd->mutex);

    return ret;
}

long hdcdrv_epoll_ctl(struct hdcdrv_cmd_epoll_ctl *cmd)
{
    struct hdcdrv_epoll_fd *epfd = NULL;
    long ret;
    u32 docker_id;

    docker_id = hdcdrv_get_container_id();

    ret = hdcdrv_epoll_fd_check(cmd->epfd, docker_id, cmd->magic_num);
    if (ret != HDCDRV_OK) {
        hdcdrv_err("para error.\n");
        return ret;
    }

    if ((cmd->op != HDCDRV_EPOLL_OP_ADD) && (cmd->op != HDCDRV_EPOLL_OP_DEL)) {
        hdcdrv_err("epfd %d op %d error.\n", cmd->epfd, cmd->op);
        return HDCDRV_PARA_ERR;
    }

    epfd = &hdc_epoll->epfds[docker_id][cmd->epfd];

    if (cmd->op == HDCDRV_EPOLL_OP_ADD) {
        ret = hdcdrv_epoll_add_event(epfd, &cmd->event, cmd->para1, cmd->para2);
    } else {
        ret = hdcdrv_epoll_del_event(epfd, &cmd->event, cmd->para1, cmd->para2);
    }

    return ret;
}

void hdcdrv_epoll_wake_up(struct hdcdrv_epoll_fd *epfd)
{
    if (epfd != NULL) {
        epfd->wait_flag = 1;
        wake_up_interruptible(&epfd->wq);
    }
}

int hdcdrv_epoll_service_event_num(struct hdcdrv_epoll_fd *epfd, int event_num)
{
    struct list_head *pos = NULL, *n = NULL;
    struct hdcdrv_epoll_list_node *node = NULL;
    struct hdcdrv_service *service = NULL;

    if (!list_empty_careful(&epfd->service_list)) {
        list_for_each_safe(pos, n, &epfd->service_list)
        {
            node = list_entry(pos, struct hdcdrv_epoll_list_node, list);
            service = (struct hdcdrv_service *)node->instance;
            if (service->conn_list_head != NULL) {
                epfd->events[event_num].events = HDCDRV_EPOLL_CONN_IN;
                epfd->events[event_num].data = node->events.data;
                epfd->events[event_num].sub_data = node->events.sub_data;
                event_num++;
            }
        }
    }

    return event_num;
}

int hdcdrv_epoll_session_event_num(struct hdcdrv_epoll_fd *epfd, int event_num)
{
    struct list_head *pos = NULL, *n = NULL;
    struct hdcdrv_epoll_list_node *node = NULL;
    struct hdcdrv_session *session = NULL;
    int data_in_flag = 0;

    if (!list_empty_careful(&epfd->session_list)) {
        list_for_each_safe(pos, n, &epfd->session_list)
        {
            node = list_entry(pos, struct hdcdrv_epoll_list_node, list);

            data_in_flag = 0;
            session = (struct hdcdrv_session *)node->instance;

            epfd->events[event_num].events = 0;

            if ((hdcdrv_sid_check(session->session_id, HDCDRV_SID_LEN, node->session_id, HDCDRV_SID_LEN))
                || (hdcdrv_get_session_status(session) == HDCDRV_SESSION_STATUS_IDLE)) {
                epfd->events[event_num].events = HDCDRV_EPOLL_SESSION_CLOSE;
                epfd->events[event_num].data = node->events.data;
                epfd->events[event_num].sub_data = node->events.sub_data;
                event_num++;
                continue;
            }

            if ((node->events.events & HDCDRV_EPOLL_DATA_IN) && (session->normal_rx.head != session->normal_rx.tail)) {
                epfd->events[event_num].events |= HDCDRV_EPOLL_DATA_IN;
                epfd->events[event_num].data = node->events.data;
                epfd->events[event_num].sub_data = node->events.sub_data;
                data_in_flag = 1;
            }

            if ((node->events.events & HDCDRV_EPOLL_FAST_DATA_IN) && (session->fast_rx.head != session->fast_rx.tail)) {
                epfd->events[event_num].events |= HDCDRV_EPOLL_FAST_DATA_IN;
                epfd->events[event_num].data = node->events.data;
                epfd->events[event_num].sub_data = node->events.sub_data;
                data_in_flag = 1;
            }

            if (data_in_flag == 1) {
                event_num++;
            }
        }
    }

    return event_num;
}

int hdcdrv_epoll_event_num(struct hdcdrv_epoll_fd *epfd)
{
    int event_num = 0;

    mutex_lock(&epfd->mutex);

    event_num = hdcdrv_epoll_service_event_num(epfd, event_num);
    event_num = hdcdrv_epoll_session_event_num(epfd, event_num);

    mutex_unlock(&epfd->mutex);

    return event_num;
}

int hdcdrv_copy_event_to_user(struct hdcdrv_epoll_fd *epfd, int event_num, struct hdcdrv_cmd_epoll_wait *cmd,
    int mode)
{
    u64 copy_size;

    if (event_num > cmd->maxevents) {
        cmd->ready_event = cmd->maxevents;
    } else {
        cmd->ready_event = event_num;
    }

    copy_size = (u64)sizeof(struct hdcdrv_event) * (unsigned int)cmd->ready_event;
    if (copy_size > ((u64)sizeof(struct hdcdrv_event) * (unsigned int)epfd->size)) {
        hdcdrv_err("the size of copy_to_user is out of range. %d,%d.\n", cmd->ready_event, epfd->size);
        return HDCDRV_PARA_ERR;
    }

    if (mode == HDCDRV_MODE_USER) {
        if ((cmd->event != NULL) && copy_to_user((void __user *)cmd->event, epfd->events, (unsigned long)copy_size)) {
            hdcdrv_err("epfd %d copy_to_user fail\n", epfd->fd);
            return HDCDRV_COPY_FROM_USER_FAIL;
        }
    } else {
        if (memcpy_s(cmd->event, cmd->maxevents * sizeof(struct hdcdrv_event), epfd->events, copy_size) != EOK) {
            hdcdrv_err("epfd %d memcpy_s failed\n", epfd->fd);
            return HDCDRV_ERR;
        }
    }

    return HDCDRV_OK;
}

long hdcdrv_epoll_wait(struct hdcdrv_cmd_epoll_wait *cmd, int mode)
{
    struct hdcdrv_epoll_fd *epfd = NULL;
    int fd = cmd->epfd;
    long ret;
    int event_num;
    u64 timeout;
    u32 docker_id;

    docker_id = hdcdrv_get_container_id();

    ret = hdcdrv_epoll_fd_check(fd, docker_id, cmd->magic_num);
    if (ret != HDCDRV_OK) {
        hdcdrv_err("para error.\n");
        return ret;
    }

    if (cmd->event == NULL) {
        hdcdrv_err("null ptr.\n");
        return HDCDRV_PARA_ERR;
    }

    if (cmd->timeout < 0) {
        hdcdrv_err("timeout %d invalid.\n", cmd->timeout);
        return HDCDRV_PARA_ERR;
    }

    if (cmd->maxevents < 0) {
        hdcdrv_err("maxevents %d invalid.\n", cmd->maxevents);
        return HDCDRV_PARA_ERR;
    }

    cmd->ready_event = 0;

    epfd = &hdc_epoll->epfds[docker_id][fd];

    event_num = hdcdrv_epoll_event_num(epfd);
    if (event_num == 0) {
        epfd->wait_flag = 0;
        timeout = msecs_to_jiffies(cmd->timeout);

        ret = wait_event_interruptible_timeout(epfd->wq, (epfd->wait_flag != 0), timeout);
        if (ret < 0) {
            hdcdrv_err("epfd %d wait error %ld\n", fd, ret);
            return ret;
        }

        event_num = hdcdrv_epoll_event_num(epfd);
    }

    ret = HDCDRV_OK;

    if (event_num > 0) {
        ret = hdcdrv_copy_event_to_user(epfd, event_num, cmd, mode);
    }

    return ret;
}

long hdcdrv_epoll_operation(struct hdcdrv_ctx *ctx, u32 drv_cmd, union hdcdrv_cmd *cmd_data, bool *copy_to_user_flag)
{
    long ret = HDCDRV_OK;
    switch (drv_cmd) {
        case HDCDRV_CMD_EPOLL_ALLOC_FD:
            ret = hdcdrv_epoll_alloc_fd(ctx, &cmd_data->epoll_alloc_fd);
            *copy_to_user_flag = true;
            break;
        case HDCDRV_CMD_EPOLL_FREE_FD:
            ret = hdcdrv_epoll_free_fd(ctx, &cmd_data->epoll_free_fd);
            break;
        case HDCDRV_CMD_EPOLL_CTL:
            ret = hdcdrv_epoll_ctl(&cmd_data->epoll_ctl);
            break;
        case HDCDRV_CMD_EPOLL_WAIT:
            ret = hdcdrv_epoll_wait(&cmd_data->epoll_wait, HDCDRV_MODE_USER);
            *copy_to_user_flag = true;
            break;
        default:
            hdcdrv_err("cmd %d is illegal\n", drv_cmd);
            return HDCDRV_PARA_ERR;
    }
    return ret;
}

long hdcdrv_kernel_epoll_alloc_fd(int size, int *epfd, int *magic_num)
{
    struct hdcdrv_cmd_epoll_alloc_fd cmd;
    long ret;

    if ((epfd == NULL) || (magic_num == NULL)) {
        return HDCDRV_PARA_ERR;
    }

    cmd.size = size;

    ret = hdcdrv_epoll_alloc_fd(NULL, &cmd);
    if (ret == HDCDRV_OK) {
        *epfd = cmd.epfd;
        *magic_num = cmd.magic_num;
    }

    return ret;
}
EXPORT_SYMBOL(hdcdrv_kernel_epoll_alloc_fd);

long hdcdrv_kernel_epoll_free_fd(int epfd, int magic_num)
{
    struct hdcdrv_cmd_epoll_free_fd cmd;

    cmd.epfd = epfd;
    cmd.magic_num = magic_num;

    return hdcdrv_epoll_free_fd(NULL, &cmd);
}
EXPORT_SYMBOL(hdcdrv_kernel_epoll_free_fd);

/*
        para1;                  input, service:dev_id, session:session_fd
        para2;                  input, service:service_type, session:magic_num
*/
long hdcdrv_kernel_epoll_ctl(int epfd, int magic_num, int op,
    unsigned int event, int para1, char *para2, unsigned int para2_len)
{
    struct hdcdrv_cmd_epoll_ctl cmd;

    /* no place call this interface, first give a param judgement */
    if (para2 == NULL || para2_len != HDCDRV_EPOLL_CTL_PARA_NUM) {
        hdcdrv_err("para error.\n");
        return HDCDRV_PARA_ERR;
    }

    cmd.epfd = epfd;
    cmd.magic_num = magic_num;
    cmd.op = op;
    cmd.para1 = para1;
    hdcdrv_sid_copy(cmd.para2, HDCDRV_SID_LEN, para2, HDCDRV_SID_LEN);
    cmd.event.events = event;
    /*lint -e571 */
    cmd.event.data = (u64)para1;
    /*lint +e571 */
    cmd.event.sub_data = *(int *)para2;

    return hdcdrv_epoll_ctl(&cmd);
}
EXPORT_SYMBOL(hdcdrv_kernel_epoll_ctl);

long hdcdrv_kernel_epoll_wait(int epfd, int magic_num, int timeout, int *event_num,
    unsigned int event[], unsigned int event_len, int para1[],
    unsigned int para1_len, int para2[], unsigned int para2_len)
{
    struct hdcdrv_cmd_epoll_wait cmd;
    long ret;
    int i;
    struct hdcdrv_event *events = NULL;

    if ((event_num == NULL) || (event == NULL) || (para1 == NULL) || (para2 == NULL)) {
        return HDCDRV_PARA_ERR;
    }

    if (*event_num <= 0) {
        return HDCDRV_PARA_ERR;
    }

    cmd.epfd = epfd;
    cmd.magic_num = magic_num;
    cmd.timeout = timeout;
    cmd.maxevents = *event_num;
    cmd.event = kzalloc((u64)(sizeof(struct hdcdrv_event) * (u64)(unsigned int)cmd.maxevents),
        GFP_KERNEL | __GFP_ACCOUNT);
    if (cmd.event == NULL) {
        hdcdrv_err("malloc failed.\n");
        return HDCDRV_ERR;
    }

    ret = hdcdrv_epoll_wait(&cmd, HDCDRV_MODE_KERNEL);
    if (ret == HDCDRV_OK) {
        *event_num = cmd.ready_event;
        events = (struct hdcdrv_event *)cmd.event;
        for (i = 0; i < cmd.ready_event; i++) {
            if ((unsigned int)i >= event_len || (unsigned int)i >= para1_len
                    || (unsigned int)i >= para2_len) {
                kfree(cmd.event);
                cmd.event = NULL;
                hdcdrv_err("ready_event len more than param array\n");
                return HDCDRV_ERR;
            }

            event[i] = events[i].events;
            para1[i] = (int)(events[i].data);
            para2[i] = events[i].sub_data;
        }
    }

    kfree(cmd.event);
    cmd.event = NULL;

    return ret;
}
EXPORT_SYMBOL(hdcdrv_kernel_epoll_wait);

void hdcdrv_epoll_init(struct hdcdrv_epoll *epolls)
{
    u32 i, j;
    struct hdcdrv_epoll_fd *epfds = NULL;
    hdc_epoll = epolls;

    mutex_init(&hdc_epoll->mutex);

    for (i = 0; i < HDCDRV_DOCKER_MAX_NUM; i++) {
        for (j = 0; j < HDCDRV_EPOLL_FD_NUM; j++) {
            epfds = &hdc_epoll->epfds[i][j];
            epfds->fd = j;
            epfds->docker_id = i;
            init_waitqueue_head(&epfds->wq);
            mutex_init(&epfds->mutex);
            INIT_LIST_HEAD(&epfds->service_list);
            INIT_LIST_HEAD(&epfds->session_list);
            epfds->valid = HDCDRV_INVALID;
        }
    }

    return;
}
