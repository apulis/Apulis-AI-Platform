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

#include <linux/errno.h>
#include <linux/version.h>
#include <linux/jiffies.h>
#include <linux/ioctl.h>
#include <linux/device.h>
#include <linux/dma-mapping.h>
#include <linux/slab.h>
#include <linux/fs.h>
#include <asm/uaccess.h>
#include <linux/wait.h>
#include <linux/sched.h>
#include <linux/delay.h>
#include <linux/uaccess.h>
#include <linux/mm.h>
#include <linux/time.h>
#include <linux/random.h>
#include <linux/jiffies.h>
#include <linux/vmalloc.h>

#include "hdcdrv_core.h"

struct hdcdrv_ctrl *hdc_ctrl = NULL;

static struct {
    const char str[HDCDRV_STR_NAME_LEN];
} hdcdrv_service_type_str[HDCDRV_SURPORT_MAX_SERVICE + 1] = {
    { "service_dmp" },          /* HDCDRV_SERVICE_TYPE_DMP */
    { "service_matrix" },       /* HDCDRV_SERVICE_TYPE_MATRIX */
    { "service_IDE1" },         /* HDCDRV_SERVICE_TYPE_IDE1 */
    { "service_file_trans" },   /* HDCDRV_SERVICE_TYPE_FILE_TRANS */
    { "service_IDE2" },         /* HDCDRV_SERVICE_TYPE_IDE2 */
    { "service_log" },          /* HDCDRV_SERVICE_TYPE_LOG */
    { "service_rdma" },         /* HDCDRV_SERVICE_TYPE_RDMA */
    { "service_bbox" },         /* HDCDRV_SERVICE_TYPE_BBOX */
    { "service_framework" },    /* HDCDRV_SERVICE_TYPE_FRAMEWORK */
    { "service_TSD" },          /* HDCDRV_SERVICE_TYPE_TSD */
    { "service_TDT" },          /* HDCDRV_SERVICE_TYPE_TDT */
    { "service_debug" },        /* HDCDRV_SERVICE_TYPE_DEBUG */
    { "service_user1" },        /* HDCDRV_SERVICE_TYPE_USER1 */
    { "service_user2" },        /* HDCDRV_SERVICE_TYPE_USER2 */
    { "service_user3" },        /* HDCDRV_SERVICE_TYPE_USER3 */
    { "service_user4" },        /* HDCDRV_SERVICE_TYPE_USER4 */
    { "service_max" }           /* HDCDRV_SURPORT_MAX_SERVICE */
};

static struct {
    const char str[HDCDRV_STR_NAME_LEN];
} hdcdrv_close_state_str[HDCDRV_CLOSE_TYPE_MAX + 1] = {
    { "state_none" },                   /* HDCDRV_CLOSE_TYPE_NONE */
    { "closed_by_user" },               /* HDCDRV_CLOSE_TYPE_USER */
    { "closed_by_user_form_kernel" },   /* HDCDRV_CLOSE_TYPE_KERNEL */
    { "closed_by_release" },            /* HDCDRV_CLOSE_TYPE_RELEASE */
    { "closed_by_set_owner_timeout" },  /* HDCDRV_CLOSE_TYPE_NOT_SET_OWNER */
    { "remote_close_pre" },             /* HDCDRV_CLOSE_TYPE_REMOTE_CLOSED_PRE */
    { "remote_close_post" },            /* HDCDRV_CLOSE_TYPE_REMOTE_CLOSED_POST */
    { "state_max" }                     /* HDCDRV_CLOSE_TYPE_MAX */
};

const char *hdcdrv_sevice_str(int service_type)
{
    if ((service_type < HDCDRV_SERVICE_TYPE_DMP) || (service_type >= HDCDRV_SURPORT_MAX_SERVICE)) {
        return hdcdrv_service_type_str[HDCDRV_SURPORT_MAX_SERVICE].str;
    } else {
        return hdcdrv_service_type_str[service_type].str;
    }
}

const char *hdcdrv_close_str(int close_state)
{
    if ((close_state < HDCDRV_CLOSE_TYPE_NONE) || (close_state > HDCDRV_CLOSE_TYPE_MAX)) {
        return hdcdrv_close_state_str[HDCDRV_CLOSE_TYPE_MAX].str;
    } else {
        return hdcdrv_close_state_str[close_state].str;
    }
}

void hdcdrv_print_status(void)
{
    int status;
    int count[4] = {0};
    int index;
    int i;

    mutex_lock(&hdc_ctrl->mutex);
    index = hdc_ctrl->cur_alloc_session;
    for (i = 0; i < HDCDRV_SURPORT_MAX_SESSION; i++) {
        status = hdcdrv_get_session_status(&hdc_ctrl->sessions[i]);
        if (status == HDCDRV_SESSION_STATUS_IDLE) {
            count[0]++;
        } else if (status == HDCDRV_SESSION_STATUS_CONN) {
            count[1]++;
        } else if (status == HDCDRV_SESSION_STATUS_REMOTE_CLOSED) {
            count[2]++;
        } else {
            count[3]++;
        }
    }
    mutex_unlock(&hdc_ctrl->mutex);

    hdcdrv_err("#### session info : index %d, state idle %d, connected %d, remote close %d, others %d\n", index,
               count[0], count[1], count[2], count[3]);
}


int hdcdrv_get_service_level(int service_type)
{
    return hdc_ctrl->service_level[service_type];
}

int hdcdrv_get_service_conn_feature(int service_type)
{
    return hdc_ctrl->service_conn_feature[service_type];
}

int hdcdrv_tasklet_status_check(struct hdcdrv_msg_chan_tasklet_status *tasklet_status)
{
    if (tasklet_status->schedule_in_last != tasklet_status->schedule_in) {
        tasklet_status->schedule_in_last = tasklet_status->schedule_in;
        tasklet_status->no_schedule_cnt = 0;
        return HDCDRV_OK;
    }

    tasklet_status->no_schedule_cnt++;

    if (tasklet_status->no_schedule_cnt <= HDCDRV_TASKLET_STATUS_CHECK_TIME) {
        return HDCDRV_OK;
    }

    tasklet_status->no_schedule_cnt = 0;

    return HDCDRV_ERR;
}

int hdcdrv_get_wait_flag(int timeout)
{
    int wait_flag;

    if (timeout < 0) {
        wait_flag = HDCDRV_WAIT_ALWAYS;
    } else if (timeout == 0) {
        wait_flag = HDCDRV_NOWAIT;
    } else {
        wait_flag = HDCDRV_WAIT_TIMEOUT;
    }

    return wait_flag;
}

void hdcdrv_delay_work_set(struct hdcdrv_session *session, struct delayed_work *work, unsigned int bit_offset,
                           int timeout)
{
    if (hdc_ctrl->debug_state.valid == HDCDRV_VALID) {
        timeout = HDCDRV_DEBUG_MODE_TIMEOUT;
    }
    spin_lock_bh(&session->lock);
    if (session->delay_work_flag & BIT(bit_offset)) {
        spin_unlock_bh(&session->lock);
        hdcdrv_warn("session work(flag offset:%d) is setted\n", bit_offset);
        return;
    }
    session->delay_work_flag |= BIT(bit_offset);
    spin_unlock_bh(&session->lock);

    schedule_delayed_work(work, timeout);
}

void hdcdrv_delay_work_cancel(struct hdcdrv_session *session, struct delayed_work *work, unsigned int bit_offset)
{
    spin_lock_bh(&session->lock);
    if (!(session->delay_work_flag & BIT(bit_offset))) {
        spin_unlock_bh(&session->lock);
        return;
    }

    session->delay_work_flag &= ~BIT(bit_offset);
    spin_unlock_bh(&session->lock);

    cancel_delayed_work_sync(work);
}

void hdcdrv_delay_work_flag_clear(struct hdcdrv_session *session, unsigned int bit_offset)
{
    spin_lock_bh(&session->lock);
    session->delay_work_flag &= ~BIT(bit_offset);
    spin_unlock_bh(&session->lock);
}

long hdcdrv_session_alive_check(int session_fd, char *session_id)
{
    struct hdcdrv_session *session = NULL;

    if ((session_fd >= HDCDRV_SURPORT_MAX_SESSION) || (session_fd < 0)) {
        hdcdrv_err("session_fd %d is illegal\n", session_fd);
        return HDCDRV_PARA_ERR;
    }

    session = &hdc_ctrl->sessions[session_fd];

    if (hdcdrv_get_session_status(session) == HDCDRV_SESSION_STATUS_IDLE) {
        hdcdrv_warn("status IDLE : session(%d) has already closed, dev %d service_type %s,"
            "local_close_state %s, remote_close_state %s, local_session_fd %d, remote_session_fd %d.\n",
            session_fd, session->dev_id, hdcdrv_sevice_str(session->service_type),
            hdcdrv_close_str(session->local_close_state), hdcdrv_close_str(session->remote_close_state),
            session->local_session_fd, session->remote_session_fd);
        return HDCDRV_SESSION_HAS_CLOSED;
    }

    if ((session_id != NULL) &&
        hdcdrv_sid_check(session->session_id, HDCDRV_SID_LEN, session_id, HDCDRV_SID_LEN)) {
        hdcdrv_warn("session_fd %d has already closed, sesssion_id miss match\n", session_fd);
        return HDCDRV_SESSION_ID_MISS_MATCH;
    }

    return HDCDRV_OK;
}

long hdcdrv_session_state_to_remote_close(int session_fd, char *session_id)
{
    struct hdcdrv_session *session = NULL;
    int session_state;

    mutex_lock(&hdc_ctrl->mutex);
    session = &hdc_ctrl->sessions[session_fd];

    session_state = hdcdrv_get_session_status(session);
    if ((session_state == HDCDRV_SESSION_STATUS_IDLE)
        || (session_state == HDCDRV_SESSION_STATUS_REMOTE_CLOSED)
        || (session_state == HDCDRV_SESSION_STATUS_CLOSING)) {
        mutex_unlock(&hdc_ctrl->mutex);
        hdcdrv_warn("status IDLE : session has already closed, dev %d service_type %s,"
            "local_close_state %s, remote_close_state %s, local_session_fd %d,"
            "remote_session_fd %d, session state %d.\n", session->dev_id,
            hdcdrv_sevice_str(session->service_type), hdcdrv_close_str(session->local_close_state),
            hdcdrv_close_str(session->remote_close_state), session->local_session_fd,
            session->remote_session_fd, session_state);
        return HDCDRV_SESSION_HAS_CLOSED;
    }

    if ((session_id != NULL) &&
        hdcdrv_sid_check(session->session_id, HDCDRV_SID_LEN, session_id, HDCDRV_SID_LEN)) {
        mutex_unlock(&hdc_ctrl->mutex);
        hdcdrv_warn("session_fd %d, sesssion_id miss match\n", session->local_session_fd);
        return HDCDRV_SESSION_ID_MISS_MATCH;
    }

    hdcdrv_set_session_status(session, HDCDRV_SESSION_STATUS_REMOTE_CLOSED);
    mutex_unlock(&hdc_ctrl->mutex);

    return HDCDRV_OK;
}

long hdcdrv_session_state_to_closing(int session_fd, char *session_id,
    int pid, int *last_status, int close_state)
{
    struct hdcdrv_session *session = NULL;
    int session_state;

    mutex_lock(&hdc_ctrl->mutex);

    session = &hdc_ctrl->sessions[session_fd];

    session_state = hdcdrv_get_session_status(session);
    if ((session_state == HDCDRV_SESSION_STATUS_IDLE)
        || (session_state == HDCDRV_SESSION_STATUS_CLOSING)) {
        mutex_unlock(&hdc_ctrl->mutex);
        hdcdrv_warn("status IDLE : session(%d) has already closed, dev %d service_type %s,"
            "local_close_state %s, remote_close_state %s, local_session_fd %d,"
            "remote_session_fd %d, session state %d.\n",
            session_fd, session->dev_id, hdcdrv_sevice_str(session->service_type),
            hdcdrv_close_str(session->local_close_state), hdcdrv_close_str(session->remote_close_state),
            session->local_session_fd, session->remote_session_fd, session_state);
        return HDCDRV_SESSION_HAS_CLOSED;
    }

    if (session->pid != pid) {
        mutex_unlock(&hdc_ctrl->mutex);
        hdcdrv_warn("pid not match : session(%d) pid %lld close pid %d\n", session_fd,
                    session->pid, pid);
        return HDCDRV_PARA_ERR;
    }

    if ((close_state == HDCDRV_CLOSE_TYPE_USER) && (hdcdrv_get_container_id() != session->container_id)) {
        mutex_unlock(&hdc_ctrl->mutex);
        hdcdrv_err("session(%d) container id verify failed\n", session_fd);
        return HDCDRV_PARA_ERR;
    }

    if (hdcdrv_sid_check(session->session_id, HDCDRV_SID_LEN, session_id, HDCDRV_SID_LEN)) {
        mutex_unlock(&hdc_ctrl->mutex);
        hdcdrv_err("session_fd %d sesssion_id is incorrect, sesssion_id miss match\n", session_fd);
        return HDCDRV_SESSION_ID_MISS_MATCH;
    }

    *last_status = session_state;
    hdcdrv_set_session_status(session, HDCDRV_SESSION_STATUS_CLOSING);

    mutex_unlock(&hdc_ctrl->mutex);

    return HDCDRV_OK;
}

long hdcdrv_session_para_check(int session_fd, char *session_id)
{
    struct hdcdrv_session *session = NULL;

    if ((session_fd >= HDCDRV_SURPORT_MAX_SESSION) || (session_fd < 0)) {
        hdcdrv_err("session_fd %d is illegal\n", session_fd);
        return HDCDRV_PARA_ERR;
    }

    session = &hdc_ctrl->sessions[session_fd];

    if (hdcdrv_get_session_status(session) == HDCDRV_SESSION_STATUS_IDLE) {
        hdcdrv_warn("session_fd %d is invalid, dev %d service_type %s,"
            "local_close_state %s, remote_close_state %s, local_session_fd %d, remote_session_fd %d.\n",
            session_fd, session->dev_id, hdcdrv_sevice_str(session->service_type),
            hdcdrv_close_str(session->local_close_state), hdcdrv_close_str(session->remote_close_state),
            session->local_session_fd, session->remote_session_fd);
        return HDCDRV_SESSION_HAS_CLOSED;
    }

    if (hdcdrv_get_container_id() != session->container_id) {
        hdcdrv_err("session(%d) container id verify failed\n", session_fd);
        return HDCDRV_PARA_ERR;
    }

    if (hdcdrv_sid_check(session->session_id, HDCDRV_SID_LEN, session_id, HDCDRV_SID_LEN)) {
        hdcdrv_err("session_fd %d sesssion_id is incorrect, sesssion_id miss match\n", session_fd);
        return HDCDRV_SESSION_ID_MISS_MATCH;
    }

    return HDCDRV_OK;
}

long hdcdrv_session_inner_check(int session_fd, u32 checker)
{
    struct hdcdrv_session *session = NULL;

    if ((session_fd >= HDCDRV_SURPORT_MAX_SESSION) || (session_fd < 0)) {
        hdcdrv_err("session_fd %d is illegal\n", session_fd);
        return HDCDRV_PARA_ERR;
    }

    session = &hdc_ctrl->sessions[session_fd];
    if (hdcdrv_get_session_status(session) == HDCDRV_SESSION_STATUS_IDLE) {
        hdcdrv_warn("status IDLE : session(%d) has already closed, dev %d service_type %s,"
            "local_close_state %s, remote_close_state %s, local_session_fd %d, remote_session_fd %d.\n",
            session_fd, session->dev_id, hdcdrv_sevice_str(session->service_type),
            hdcdrv_close_str(session->local_close_state), hdcdrv_close_str(session->remote_close_state),
            session->local_session_fd, session->remote_session_fd);
        return HDCDRV_PARA_ERR;
    }

    if (session->inner_checker != checker) {
        hdcdrv_err("session_fd %d inner check failed, session may closed before\n", session_fd);
        return HDCDRV_PARA_ERR;
    }

    return HDCDRV_OK;
}

struct hdcdrv_session_fast_rx *hdcdrv_get_session_fast_rx(int session)
{
    return &hdc_ctrl->sessions[session].fast_rx;
}

void hdcdrv_bind_sessoin_ctx(struct hdcdrv_ctx *ctx, struct hdcdrv_session *session)
{
    if (ctx == HDCDRV_KERNEL_WITHOUT_CTX) {
        return;
    }

    ctx->session_fd = session->local_session_fd;
    hdcdrv_sid_copy(ctx->session_id, HDCDRV_SID_LEN, session->session_id, HDCDRV_SID_LEN);
    ctx->pid = session->pid;
    ctx->service = NULL;
    ctx->session = session;
    ctx->dev_id = session->dev_id;
    session->ctx = ctx;

    return;
}

void hdcdrv_unbind_session_ctx(struct hdcdrv_session *session)
{
    if (session->ctx != NULL) {
        session->ctx->session = NULL;
        session->ctx = NULL;
    }
}

void hdcdrv_bind_server_ctx(struct hdcdrv_ctx *ctx, struct hdcdrv_service *service, int dev_id, int service_type)
{
    if (ctx == HDCDRV_KERNEL_WITHOUT_CTX) {
        return;
    }

    ctx->dev_id = dev_id;
    ctx->pid = service->listen_pid;
    ctx->service_type = service_type;
    ctx->session = NULL;
    ctx->service = service;

    service->ctx = ctx;

    return;
}

void hdcdrv_unbind_server_ctx(struct hdcdrv_service *server)
{
    if (server->ctx != NULL) {
        server->ctx->service = NULL;
        server->ctx = NULL;
    }
}

long hdcdrv_bind_mem_ctx(struct hdcdrv_ctx *ctx, struct hdcdrv_fast_node *f_node)
{
    struct hdcdrv_mem_fd_list *new_node = NULL;

    if (ctx == HDCDRV_KERNEL_WITHOUT_CTX) {
        return HDCDRV_OK;
    }

    new_node = (struct hdcdrv_mem_fd_list *)kzalloc(sizeof(struct hdcdrv_mem_fd_list), GFP_KERNEL | __GFP_ACCOUNT);
    if (new_node == NULL) {
        hdcdrv_err("malloc mem context failed.\n");
        return HDCDRV_MEM_ALLOC_FAIL;
    }

    new_node->f_node = f_node;
    new_node->ctx = ctx;

    spin_lock_bh(&ctx->mem_lock);
    list_add(&new_node->list, &ctx->mlist.list);
    spin_unlock_bh(&ctx->mem_lock);

    f_node->mem_fd_node = new_node;

    ctx->count++;

    return HDCDRV_OK;
}

void hdcdrv_unbind_mem_ctx(struct hdcdrv_fast_node *f_node)
{
    struct hdcdrv_mem_fd_list *node = f_node->mem_fd_node;
    struct hdcdrv_ctx *ctx = NULL;

    if (node != NULL) {
        ctx = node->ctx;

        spin_lock_bh(&ctx->mem_lock);
        list_del(&node->list);
        spin_unlock_bh(&ctx->mem_lock);

        kfree(node);
        node = NULL;

        ctx->count--;
    }

    f_node->mem_fd_node = NULL;
}

void hdcdrv_sid_gen(char *session_id, int len)
{
    get_random_bytes(session_id, len);
}

void hdcdrv_sid_copy(char *dst_session_id, int dst_len, char *src_session_id, int src_len)
{
    u64 *dst = (u64 *)dst_session_id;
    u64 *src = (u64 *)src_session_id;

    (void)dst_len;
    (void)src_len;

    dst[0] = src[0];
    dst[1] = src[1];
    dst[2] = src[2];
    dst[3] = src[3];
}

bool hdcdrv_sid_check(char *dst_session_id, int dst_len, char *src_session_id, int src_len)
{
    u64 *dst = (u64 *)dst_session_id;
    u64 *src = (u64 *)src_session_id;

    (void)dst_len;
    (void)src_len;

    if ((dst[0] != src[0]) || (dst[1] != src[1]) || (dst[2] != src[2]) || (dst[3] != src[3])) {
        // not match
        return true;
    }

    return false;
}

void hdcdrv_sid_clear(struct hdcdrv_session *session)
{
    u64 *dst =  (u64 *)session->session_id;

    dst[0] = 0;
    dst[1] = 0;
    dst[2] = 0;
    dst[3] = 0;
}

long hdcdrv_set_send_timeout(struct hdcdrv_session *session, unsigned int timeout)
{
    if (timeout == 0) {
        return session->timeout_jiffies.send_timeout;
    } else {
        return msecs_to_jiffies(timeout);
    }
}

long hdcdrv_set_fast_send_timeout(struct hdcdrv_session *session, unsigned int timeout)
{
    if (timeout == 0) {
        return session->timeout_jiffies.fast_send_timeout;
    } else {
        return msecs_to_jiffies(timeout);
    }
}

void hdcdrv_inner_checker_set(struct hdcdrv_session *session)
{
    session->inner_checker = *(u32 *)session->session_id;
}

void hdcdrv_inner_checker_clear(struct hdcdrv_session *session)
{
    session->inner_checker = 0;
}

void hdcdrv_node_status_busy(struct hdcdrv_fast_node *node)
{
    node->stamp = jiffies;
    atomic_set(&node->status, HDCDRV_NODE_BUSY);
}

void hdcdrv_node_status_idle(struct hdcdrv_fast_node *node)
{
    u32 cost_time;

    if (node->stamp != 0) {
        cost_time = jiffies_to_msecs(jiffies - node->stamp);
        if (cost_time > node->max_cost) {
            node->max_cost = cost_time;
        }

        if (cost_time > HDCDRV_NODE_BUSY_WARING) {
            hdcdrv_info_limit("node busy for %u ms, max cost %u ms\n", cost_time, node->max_cost);
        }
    }

    node->stamp = 0;
    atomic_set(&node->status, HDCDRV_NODE_IDLE);
}

void hdcdrv_node_status_idle_by_mem(struct hdcdrv_fast_mem *f_mem)
{
    struct hdcdrv_fast_node *f_node = NULL;

    if (f_mem != NULL) {
        f_node = container_of(f_mem, struct hdcdrv_fast_node, fast_mem);
        hdcdrv_node_status_idle(f_node);
    }
}

bool hdcdrv_node_is_busy(struct hdcdrv_fast_node *node)
{
    if (atomic_read(&node->status) == HDCDRV_NODE_BUSY)
        return true;
    else
        return false;
}

bool hdcdrv_node_is_timeout(int node_stamp)
{
    if (jiffies_to_msecs(jiffies - node_stamp) > HDCDRV_NODE_BUSY_TIMEOUT)
        return true;
    else
        return false;
}

void hdcdrv_fill_sq_desc(struct hdcdrv_sq_desc *sq_desc,
    struct hdcdrv_session *session, struct hdcdrv_buf_desc *buf_d, int type)
{
    sq_desc->local_session = session->local_session_fd;
    sq_desc->remote_session = session->remote_session_fd;
    sq_desc->src_data_addr = (u64)buf_d->addr;
    sq_desc->data_len = buf_d->len;
    sq_desc->inner_checker = session->inner_checker;
    if (type == HDCDRV_MSG_CHAN_TYPE_FAST) {
        sq_desc->dst_data_addr = buf_d->dst_data_addr;
        sq_desc->src_ctrl_addr = buf_d->src_ctrl_addr;
        sq_desc->dst_ctrl_addr = buf_d->dst_ctrl_addr;
        sq_desc->ctrl_len = buf_d->ctrl_len;
    }

    wmb();
    sq_desc->valid = HDCDRV_VALID;
}

long hdcdrv_msg_chan_send_wait(struct hdcdrv_msg_chan *msg_chan, struct hdcdrv_session *session,
    int wait_flag, u64 timeout)
{
    long ret;
    struct hdcdrv_dev *dev = NULL;

    dev = &hdc_ctrl->devices[msg_chan->dev_id];

    if (wait_flag == HDCDRV_NOWAIT) {
        return HDCDRV_NO_BLOCK;
    } else if (wait_flag == HDCDRV_WAIT_ALWAYS) {
        /*lint -e666*/
        ret = wait_event_interruptible_exclusive(msg_chan->send_wait,
            (!(hdcdrv_w_sq_full_check(msg_chan->chan))) ||
            (dev->valid != HDCDRV_VALID) ||
            (hdcdrv_get_session_status(session) == HDCDRV_SESSION_STATUS_IDLE));
        /*lint +e666*/
    } else {
        /*lint -e666*/
        ret = wait_event_interruptible_timeout(msg_chan->send_wait,
            (!(hdcdrv_w_sq_full_check(msg_chan->chan))) ||
            (dev->valid != HDCDRV_VALID) ||
            (hdcdrv_get_session_status(session) == HDCDRV_SESSION_STATUS_IDLE),
            timeout);
        /*lint +e666*/
        if (ret == 0) {
            return HDCDRV_TX_TIMEOUT;
        } else if (ret > 0) {
            ret = 0;
        }
    }

    if (ret) {
        hdcdrv_warn("device %u session %d service type %s, wait ret %ld\n", dev->dev_id, session->local_session_fd,
                    hdcdrv_sevice_str(session->service_type), ret);
        return ret;
    }

    if (dev->valid != HDCDRV_VALID) {
        /* Clear the packets in the waiting queue */
        wake_up_interruptible(&msg_chan->send_wait);
        hdcdrv_err("session %d service type %s device %u reset.\n",
                   session->local_session_fd, hdcdrv_sevice_str(session->service_type), dev->dev_id);

        return HDCDRV_DEVICE_RESET;
    }

    if (hdcdrv_get_session_status(session) == HDCDRV_SESSION_STATUS_IDLE) {
        hdcdrv_warn("device %u chan_id %d session %d service type %s has closed, local_close_state %s,"
            "remote_close_state %s, local_session_fd %d, remote_session_fd %d.\n",
            dev->dev_id, msg_chan->chan_id, session->local_session_fd, hdcdrv_sevice_str(session->service_type),
            hdcdrv_close_str(session->local_close_state), hdcdrv_close_str(session->remote_close_state),
            session->local_session_fd, session->remote_session_fd);
        return HDCDRV_SESSION_HAS_CLOSED;
    }

    return HDCDRV_OK;
}

long hdcdrv_msg_chan_send(struct hdcdrv_msg_chan *msg_chan, struct hdcdrv_buf_desc *buf_d,
    int wait_flag, u64 timeout)
{
    struct hdcdrv_session *session = NULL;
    struct hdcdrv_service *service = NULL;
    struct hdcdrv_sq_desc *sq_desc = NULL;
    struct hdcdrv_dev *dev = NULL;
    u32 tail = 0;
    long ret;

    session = &hdc_ctrl->sessions[buf_d->local_session];
    service = session->service;
    dev = &hdc_ctrl->devices[msg_chan->dev_id];

    mutex_lock(&msg_chan->mutex);
    msg_chan->dbg_stat.hdcdrv_msg_chan_send1++;

    /* Wait until there is space to send */
    while (hdcdrv_w_sq_full_check(msg_chan->chan)) {
        msg_chan->stat.tx_full++;
        session->stat.tx_full++;
        service->stat.tx_full++;

        mutex_unlock(&msg_chan->mutex);
        ret = hdcdrv_msg_chan_send_wait(msg_chan, session, wait_flag, timeout);
        if (ret != HDCDRV_OK) {
            return ret;
        }
        mutex_lock(&msg_chan->mutex);
    }

    sq_desc = hdcdrv_get_w_sq_desc(msg_chan->chan, &tail);
    if (sq_desc == NULL) {
        hdcdrv_err("hdcdrv_get_w_sq_desc return fail\n");
        mutex_unlock(&msg_chan->mutex);
        return HDCDRV_SQ_DESC_NULL;
    }

#ifdef HDCDRV_DEBUG
    hdcdrv_info("device %u hdcdrv_net_xmit local_session %d, tail %d len %d\n", dev->dev_id, session->local_session_fd,
                tail, buf_d->len);
#endif

    /* stored locally for release after subsequent sending */
    msg_chan->tx[tail] = *buf_d;

    hdcdrv_fill_sq_desc(sq_desc, session, buf_d, msg_chan->type);

    msg_chan->stat.tx++;
    msg_chan->stat.tx_bytes += (u32)buf_d->len;
    session->stat.tx++;
    session->stat.tx_bytes += (u32)buf_d->len;
    service->stat.tx++;
    service->stat.tx_bytes += (u32)buf_d->len;

    /* data and ctrl use diff dma channel */
    hdcdrv_copy_sq_desc_to_remote(msg_chan, sq_desc, DEVDRV_DMA_DATA_PCIE_MSG);

    mutex_unlock(&msg_chan->mutex);

    return HDCDRV_OK;
}

void hdcdrv_tx_finish_handle(struct hdcdrv_msg_chan *msg_chan, struct hdcdrv_cq_desc *cq_desc)
{
    void *buf = NULL;
    u16 tx_head = cq_desc->sq_head;
    struct hdcdrv_session *session = NULL;
    struct hdcdrv_service *service = NULL;

    if (msg_chan->type == HDCDRV_MSG_CHAN_TYPE_NORMAL) {
        buf = msg_chan->tx[tx_head].buf;
        if (unlikely(buf == NULL)) {
            msg_chan->dbg_stat.hdcdrv_tx_finish_notify_task5++;
            hdcdrv_err("device %u session %d tx failed tx_head = %d buf is null.\n",
                       msg_chan->dev_id, cq_desc->session, tx_head);
            goto out;
        }

        free_mem(buf);
    }

    if ((cq_desc->session >= HDCDRV_SURPORT_MAX_SESSION) || (cq_desc->session < 0)) {
        msg_chan->dbg_stat.hdcdrv_tx_finish_notify_task6++;
        hdcdrv_err("device %u cq desc invalid session %d, sq head %d\n",
            msg_chan->dev_id, cq_desc->session, tx_head);
        goto out;
    }

    session = &hdc_ctrl->sessions[cq_desc->session];
    service = session->service;

    if (cq_desc->status != 0) {
        msg_chan->stat.tx_fail++;
        session->stat.tx_fail++;
        service->stat.tx_fail++;

        hdcdrv_err("device %u session %d service type %s, tx failed tx_head = %d status %u.\n", msg_chan->dev_id,
                   cq_desc->session, hdcdrv_sevice_str(session->service_type), tx_head, cq_desc->status);
    } else {
        msg_chan->stat.tx_finish++;
        session->stat.tx_finish++;
        service->stat.tx_finish++;
    }

out:
    msg_chan->tx[tx_head].buf = NULL;
    msg_chan->tx[tx_head].addr = 0;
    msg_chan->tx[tx_head].len = 0;
}

void hdcdrv_tx_finish_notify_task(unsigned long data)
{
    struct hdcdrv_msg_chan *msg_chan = (struct hdcdrv_msg_chan *)((uintptr_t)data);
    u16 tx_head;
    int cnt = 0;

    struct hdcdrv_cq_desc *cq_desc = NULL;

    msg_chan->tx_finish_task_status.schedule_in++;

    msg_chan->dbg_stat.hdcdrv_tx_finish_notify_task1++;
    do {
        cq_desc = hdcdrv_get_r_cq_desc(msg_chan->chan);
        if ((cq_desc == NULL) || (cq_desc->valid != HDCDRV_VALID)) {
            break;
        }

        rmb();

        msg_chan->dbg_stat.hdcdrv_tx_finish_notify_task2++;

        /* Reach the threshold and schedule out */
        if (cnt >= HDCDRV_TX_BUDGET) {
            msg_chan->dbg_stat.hdcdrv_tx_finish_notify_task3++;
            tasklet_schedule(&msg_chan->tx_finish_task);
            break;
        }

        tx_head = cq_desc->sq_head;

        /* the branch should not enter */
        if (tx_head >= HDCDRV_DESC_QUEUE_DEPTH) {
            msg_chan->dbg_stat.hdcdrv_tx_finish_notify_task4++;
            hdcdrv_err("device %d cq desc sq head %d invalid, session id %d\n",
                msg_chan->dev_id, tx_head, cq_desc->session);
            break;
        }

        hdcdrv_tx_finish_handle(msg_chan, cq_desc);

        cq_desc->session = HDCDRV_SESSION_FD_INVALID;
        cq_desc->valid = HDCDRV_INVALID;
        hdcdrv_move_r_cq_desc(msg_chan->chan);

        /* update the sq head pointer to continue send packet */
        hdcdrv_set_w_sq_desc_head(msg_chan->chan, tx_head);
        msg_chan->sq_head = tx_head;
        cnt++;
        wake_up_interruptible(&msg_chan->send_wait);
        msg_chan->dbg_stat.hdcdrv_tx_finish_notify_task7++;
    } while (1);
}

void hdcdrv_tx_finish_notify(void *chan)
{
    struct hdcdrv_dev *dev = NULL;
    struct hdcdrv_msg_chan *msg_chan = hdcdrv_get_msg_chan_priv(chan);

    if (msg_chan == NULL) {
        hdcdrv_err("msg_dev is NULL!");
        return;
    }

    dev = &hdc_ctrl->devices[msg_chan->dev_id];

    if (dev->valid != HDCDRV_VALID) {
        hdcdrv_err("tx finish notify device %u reset.\n", dev->dev_id);
        return;
    }

    msg_chan->dbg_stat.hdcdrv_tx_finish_notify1++;

    tasklet_schedule(&msg_chan->tx_finish_task);
}

void hdcdrv_tx_finish_task_check(struct hdcdrv_msg_chan *msg_chan)
{
    struct hdcdrv_msg_chan_tasklet_status *tasklet_status = NULL;
    struct hdcdrv_cq_desc *cq_desc = NULL;

    tasklet_status = &msg_chan->tx_finish_task_status;

    msg_chan->dbg_stat.hdcdrv_tx_finish_task_check1++;

    if (hdcdrv_tasklet_status_check(tasklet_status) == HDCDRV_OK) {
        return;
    }

    cq_desc = hdcdrv_get_r_cq_desc(msg_chan->chan);
    if (cq_desc == NULL) {
        return;
    }

    /* no valid cq */
    if (cq_desc->valid != HDCDRV_VALID) {
        return;
    }

    msg_chan->dbg_stat.hdcdrv_tx_finish_task_check2++;

    hdcdrv_info("dev %u msg chan %d tx finish notify tasklet check schedule\r\n", msg_chan->dev_id, msg_chan->chan_id);

    /* has finish cq but not schedule */
    tasklet_schedule(&msg_chan->tx_finish_task);
}

void hdcdrv_response_cq(struct hdcdrv_msg_chan *msg_chan, u32 sq_head, u32 status, int local_session,
                        int remote_session)
{
    struct hdcdrv_cq_desc *cq_desc = NULL;

    /* response cq */
    cq_desc = hdcdrv_get_w_cq_desc(msg_chan->chan);
    if (cq_desc == NULL) {
        hdcdrv_err("dev %u hdcdrv_get_w_cq_desc error, sq_desc is null\n", msg_chan->dev_id);
        return;
    }
    cq_desc->sq_head = sq_head;
    cq_desc->status = status;
    cq_desc->session = remote_session;
    wmb();
    cq_desc->valid = HDCDRV_VALID;

    /* data and ctrl use diff dma channel */
    hdcdrv_copy_cq_desc_to_remote(msg_chan, cq_desc, DEVDRV_DMA_DATA_PCIE_MSG);
}

void hdcdrv_update_msg_chan_head(struct hdcdrv_msg_chan *msg_chan, u32 sq_head,
    u32 status, int local_session, int remote_session)
{
    msg_chan->rx[sq_head].buf = NULL;
    msg_chan->rx[sq_head].addr = 0;
    msg_chan->rx[sq_head].len = 0;

    hdcdrv_response_cq(msg_chan, sq_head, status, local_session, remote_session);

    msg_chan->rx_head = sq_head;
}

int hdcdrv_session_rx_list_is_full(const int session_fd, const int chan_type)
{
    struct hdcdrv_session *session = &hdc_ctrl->sessions[session_fd];
    if (chan_type == HDCDRV_MSG_CHAN_TYPE_NORMAL) {
        return ((session->normal_rx.tail + 1) % HDCDRV_SESSION_RX_LIST_MAX_PKT == session->normal_rx.head);
    }
    return ((session->fast_rx.tail + 1) % HDCDRV_BUF_MAX_CNT == session->fast_rx.head);
}

int hdcdrv_msg_chan_recv_handle(struct hdcdrv_msg_chan *msg_chan, struct hdcdrv_buf_desc *rx_desc)
{
    struct hdcdrv_fast_rx *rx_buf = NULL;
    struct hdcdrv_session *session = NULL;
    struct hdcdrv_service *service = NULL;

    session = &hdc_ctrl->sessions[rx_desc->local_session];
    service = session->service;

    spin_lock_bh(&session->lock);

    /* The session received too many packets, stop recv...
       Wait for the upper layer to take the packet first
       Note: There will be mutual influence between different
       sessions in the same queue */
    if (hdcdrv_session_rx_list_is_full(rx_desc->local_session, msg_chan->type)) {
        msg_chan->dbg_stat.hdcdrv_msg_chan_recv_task6++;
        msg_chan->stat.rx_full++;
        session->stat.rx_full++;
        service->stat.rx_full++;
        msg_chan->rx_trigger_flag = HDCDRV_VALID;
        spin_unlock_bh(&session->lock);
#ifdef HDCDRV_DEBUG
        hdcdrv_err("device %u session %d chan %d received too many packets.\n", msg_chan->dev_id,
            rx_desc->local_session, msg_chan->chan_id);
#endif
        return HDCDRV_ERR;
    }

    /* update stats */
    msg_chan->stat.rx++;
    msg_chan->stat.rx_bytes += rx_desc->len;
    session->stat.rx++;
    session->stat.rx_bytes += rx_desc->len;
    service->stat.rx++;
    service->stat.rx_bytes += rx_desc->len;

    /* Insert into the tail of the receive list */
    if (msg_chan->type == HDCDRV_MSG_CHAN_TYPE_NORMAL) {
        session->normal_rx.rx_list[session->normal_rx.tail] = *rx_desc;
        session->normal_rx.tail = (session->normal_rx.tail + 1) % HDCDRV_SESSION_RX_LIST_MAX_PKT;
    } else {
        rx_buf = &session->fast_rx.rx_list[session->fast_rx.tail];
        rx_buf->data_addr = rx_desc->dst_data_addr;
        rx_buf->ctrl_addr = rx_desc->dst_ctrl_addr;
        rx_buf->data_len = rx_desc->len;
        rx_buf->ctrl_len = rx_desc->ctrl_len;
        session->fast_rx.tail = (session->fast_rx.tail + 1) % HDCDRV_BUF_MAX_CNT;
    }
    spin_unlock_bh(&session->lock);

    msg_chan->dbg_stat.hdcdrv_msg_chan_recv_task7++;

    /* Trigger the upper reception process */
    wake_up_interruptible(&session->wq_rx);

    hdcdrv_epoll_wake_up(session->epfd);

    return HDCDRV_OK;
}
void hdcdrv_msg_chan_recv_task(unsigned long data)
{
    struct hdcdrv_msg_chan *msg_chan = (struct hdcdrv_msg_chan *)((uintptr_t)data);
    struct hdcdrv_buf_desc *rx_desc = NULL;
    void *buf = NULL;
    int rx_head, status, local_session, remote_session;
    long ret;

    msg_chan->rx_task_status.schedule_in++;
    msg_chan->dbg_stat.hdcdrv_msg_chan_recv_task1++;
    while (msg_chan->rx_head != msg_chan->dma_head) {
        rx_head = (msg_chan->rx_head + 1) % HDCDRV_DESC_QUEUE_DEPTH;
        rx_desc = &msg_chan->rx[rx_head];
        rmb();
        buf = rx_desc->buf;
        status = rx_desc->status;
        msg_chan->dbg_stat.hdcdrv_msg_chan_recv_task2++;

        local_session = rx_desc->local_session;
        remote_session = rx_desc->remote_session;
        if (rx_desc->skip_flag == HDCDRV_VALID) {
            msg_chan->dbg_stat.hdcdrv_msg_chan_recv_task3++;
            goto next;
        }

        /* This branch should not enter */
        if (unlikely((msg_chan->type == HDCDRV_MSG_CHAN_TYPE_NORMAL) && (buf == NULL))) {
            msg_chan->dbg_stat.hdcdrv_msg_chan_recv_task4++;
            hdcdrv_err("dev %u, session %d rx head %d buf is null.\n", msg_chan->dev_id, local_session, rx_head);
            msg_chan->rx_head = rx_head;
            continue;
        }

        ret = hdcdrv_session_inner_check(local_session, rx_desc->inner_checker);
        if ((ret != HDCDRV_OK) || (status != 0)) {
            msg_chan->stat.rx_fail++;
            msg_chan->dbg_stat.hdcdrv_msg_chan_recv_task5++;
            if (msg_chan->type == HDCDRV_MSG_CHAN_TYPE_NORMAL) {
                free_mem(buf);
            }
            hdcdrv_warn("device %u, local_session %d, remote_session %d, chan id %d, rx head %d, status %u ret %ld\n",
                        msg_chan->dev_id, local_session, remote_session, msg_chan->chan_id, rx_head, status, ret);
            goto next;
        }

        if (hdcdrv_msg_chan_recv_handle(msg_chan, rx_desc) != HDCDRV_OK) {
            break;
        }

    next:
        rx_desc->buf = NULL;
        rx_desc->addr = 0;
        rx_desc->len = 0;
        hdcdrv_response_cq(msg_chan, rx_head, status, local_session, remote_session);
        msg_chan->rx_head = rx_head;
    }
}

void hdcdrv_rx_msg_callback(void *data, u32 trans_id, u32 status)
{
    struct hdcdrv_msg_chan *msg_chan = (struct hdcdrv_msg_chan *)data;
    struct hdcdrv_dev *dev = &hdc_ctrl->devices[msg_chan->dev_id];

    if (msg_chan->dma_need_submit_flag == HDCDRV_VALID) {
        tasklet_schedule(&msg_chan->rx_notify_task);
        hdcdrv_info("dev %d msg chan %d schedule.\n", dev->dev_id, msg_chan->chan_id);
    }

    msg_chan->dbg_stat.hdcdrv_rx_msg_callback1++;
    /* This branch should not enter */
    if (unlikely(trans_id >= HDCDRV_DESC_QUEUE_DEPTH)) {
        msg_chan->dbg_stat.hdcdrv_rx_msg_callback2++;
        hdcdrv_err("device %u hdcdrv rx callback trans id %u error.\n", dev->dev_id, trans_id);
        return;
    }

#ifdef HDCDRV_DEBUG
    hdcdrv_info("device %u hdcdrv_rx_msg_callback chanid %d trans_id %u, status %u\n",
        dev->dev_id, msg_chan->chan_id, trans_id, status);
#endif

    if (msg_chan->type == HDCDRV_MSG_CHAN_TYPE_FAST) {
        hdcdrv_node_status_idle_by_mem(msg_chan->rx[trans_id].src_data);
        hdcdrv_node_status_idle_by_mem(msg_chan->rx[trans_id].dst_data);
        hdcdrv_node_status_idle_by_mem(msg_chan->rx[trans_id].src_ctrl);
        hdcdrv_node_status_idle_by_mem(msg_chan->rx[trans_id].dst_ctrl);

        msg_chan->rx[trans_id].src_data = NULL;
        msg_chan->rx[trans_id].dst_data = NULL;
        msg_chan->rx[trans_id].src_ctrl = NULL;
        msg_chan->rx[trans_id].dst_ctrl = NULL;
    }

    if (dev->valid != HDCDRV_VALID) {
        hdcdrv_err("rx msg callback device %u reset.\n", dev->dev_id);
        return;
    }

    msg_chan->rx[trans_id].status = status;

    wmb();
    msg_chan->dma_head = trans_id;
    msg_chan->dbg_stat.hdcdrv_rx_msg_callback3++;
    tasklet_schedule(&msg_chan->rx_task);
}

void hdcdrv_rx_msg_task_check(struct hdcdrv_msg_chan *msg_chan)
{
    struct hdcdrv_msg_chan_tasklet_status *tasklet_status = NULL;

    tasklet_status = &msg_chan->rx_task_status;

    msg_chan->dbg_stat.hdcdrv_rx_msg_task_check1++;

    if (hdcdrv_tasklet_status_check(tasklet_status) == HDCDRV_OK) {
        return;
    }

    /* no valid msg */
    if (msg_chan->rx_head == msg_chan->dma_head) {
        return;
    }

    msg_chan->dbg_stat.hdcdrv_rx_msg_task_check2++;

    hdcdrv_info("dev %d msg chan %d rx msg tasklet check schedule\r\n", msg_chan->dev_id, msg_chan->chan_id);

    /* has msg but not schedule */
    tasklet_schedule(&msg_chan->rx_task);
}

int hdcdrv_normal_dma_copy(struct hdcdrv_msg_chan *msg_chan, u32 head, struct hdcdrv_sq_desc *sq_desc,
                           struct devdrv_asyn_dma_para_info *para, enum devdrv_dma_data_type data_type)
{
    /* the receiving end cannot apply for memory, too many receive
       buffers, and waits for the upper layer to receive packets.
       Block live send.
       malloc fixed len memory to avoid memory fragmentation */
    int ret;

    ret = alloc_mem(HDCDRV_MEM_POOL_TYPE_RX, msg_chan->dev_id, sq_desc->data_len, &msg_chan->rx[head].buf,
                    (dma_addr_t *)&msg_chan->rx[head].addr, &msg_chan->wait_mem_list);
    if (ret != HDCDRV_OK) {
        msg_chan->dbg_stat.hdcdrv_normal_dma_copy1++;
#ifdef HDCDRV_DEBUG
        hdcdrv_info("dev_id %d msg chan %d: session %d recv alloc dma mem failed\n", msg_chan->dev_id,
                    msg_chan->chan_id, sq_desc->remote_session);
#endif
        return HDCDRV_DMA_MEM_ALLOC_FAIL;
    }

    /* record recv buf desc */
    msg_chan->rx[head].len = sq_desc->data_len;
    msg_chan->rx[head].local_session = sq_desc->remote_session;
    msg_chan->rx[head].remote_session = sq_desc->local_session;

    /* copy the packet */
    ret = devdrv_dma_async_copy_plus(msg_chan->dev_id, data_type, msg_chan->chan_id, (dma_addr_t)sq_desc->src_data_addr,
                                     msg_chan->rx[head].addr, sq_desc->data_len, hdcdrv_get_dma_direction(), para);
    if (ret == -ENOSPC) {
        free_mem(msg_chan->rx[head].buf);
        ret = HDCDRV_DMA_QUE_FULL;
    } else  if (ret != 0) {
        ret = HDCDRV_DMA_COPY_FAIL;
    }

    return ret;
}

int hdcdrv_add_mem_info(int devid, struct hdcdrv_ctrl_msg_sync_mem_info *msg)
{
    struct hdcdrv_fast_node *f_node = NULL;
    u64 len;
    int i, ret;
    struct rb_root *remote_rb = &hdc_ctrl->devices[devid].remote_rb_mems;;

    len = (u64)(sizeof(struct hdcdrv_fast_node) +
                          ((u64)(unsigned int)msg->phy_addr_num * sizeof(struct hdcdrv_mem_f)));
    f_node = hdcdrv_fast_node_search_timeout(remote_rb, msg->hash_va, HDCDRV_NODE_WAIT_TIME_MAX);
        if (f_node != NULL) {
            hdcdrv_warn("this fast node has exit, free it firstly.\n");
            hdcdrv_fast_node_erase(&hdc_ctrl->lock, remote_rb, f_node);
            hdcdrv_node_status_idle(f_node);
            hdcdrv_fast_node_free(f_node);
        }

    f_node = (struct hdcdrv_fast_node *)kzalloc(len, GFP_KERNEL | __GFP_ACCOUNT);
    if (f_node == NULL) {
        hdcdrv_err("dev %d f_node kzalloc failed!\n", devid);
        return HDCDRV_MEM_ALLOC_FAIL;
    }

    f_node->fast_mem.phy_addr_num = msg->phy_addr_num;
    f_node->fast_mem.free_flag = HDCDRV_FALSE_FLAG;  // No need to free buf.
    f_node->fast_mem.alloc_len = msg->alloc_len;
    f_node->fast_mem.mem_type = msg->mem_type;
    f_node->pid = msg->pid;
    f_node->hash_va = msg->hash_va;
    f_node->fast_mem.mem = (struct hdcdrv_mem_f *)(f_node + 1);
    for (i = 0; i < msg->phy_addr_num; i++) {
        f_node->fast_mem.mem[i].addr = msg->mem[i].addr;
        f_node->fast_mem.mem[i].len = msg->mem[i].len;
    }

    ret = hdcdrv_fast_node_insert(remote_rb, f_node);
    if (ret != HDCDRV_OK) {
        hdcdrv_err_limit("Insert failed, node exist! devid %dmsg type %d, flag %d, mem_num %d, pid 0x%llx\n",
                         devid, msg->type, msg->flag, msg->phy_addr_num, msg->pid);
        kfree(f_node);
        f_node = NULL;

        return ret;
    }

    hdcdrv_dfx_node_insert(&hdc_ctrl->devices[devid].remote_mem_rb_mems, f_node->hash_va, HDCDRV_MEM_DFX_REMOTE);
    return HDCDRV_OK;
}

int hdcdrv_del_mem_info(int devid, struct hdcdrv_ctrl_msg_sync_mem_info *msg)
{
    struct hdcdrv_fast_node *f_node = NULL;
    struct rb_root *remote_rb = &hdc_ctrl->devices[devid].remote_rb_mems;;

    f_node = hdcdrv_fast_node_search_timeout(remote_rb, msg->hash_va, HDCDRV_NODE_WAIT_TIME_MAX);
    if (f_node == NULL) {
        hdcdrv_err("dev %d fast node search va fail.\n", devid);
        hdcdrv_dfx_node_info(&hdc_ctrl->devices[devid].remote_mem_rb_mems, msg->hash_va);
        return HDCDRV_F_NODE_SEARCH_FAIL;
    }

    hdcdrv_dfx_node_upgrade(&hdc_ctrl->devices[devid].remote_mem_rb_mems, f_node->hash_va,
                            HDCDRV_MEM_DFX_REMOTE_FREE);

    hdcdrv_fast_node_erase(&hdc_ctrl->lock, remote_rb, f_node);
    hdcdrv_node_status_idle(f_node);
    hdcdrv_fast_node_free(f_node);

    return HDCDRV_OK;
}

int hdcdrv_set_mem_info(int devid, struct hdcdrv_ctrl_msg_sync_mem_info *msg)
{
    if ((u32)devid >= HDCDRV_SURPORT_MAX_DEV) {
        hdcdrv_err("dev_id (%d) invalid\n", devid);
        return HDCDRV_PARA_ERR;
    }

    if ((u32)msg->phy_addr_num > HDCDRV_MEM_MAX_PHY_NUM) {
        hdcdrv_err("dev %d phy addr num overflow.phy addr num %d\n", devid, msg->phy_addr_num);
        return HDCDRV_PARA_ERR;
    }

    if (msg->flag == HDCDRV_ADD_FLAG) {
        return hdcdrv_add_mem_info(devid, msg);
    } else {
        return hdcdrv_del_mem_info(devid, msg);
    }
}

int hdcdrv_send_mem_info(struct hdcdrv_fast_mem *mem, int devid, int flag)
{
    struct hdcdrv_dev *hdc_dev = NULL;
    long ret;
    int i;
    u32 len = 0;
    u32 msg_size;
    struct hdcdrv_ctrl_msg_sync_mem_info *msg = NULL;

    if (!hdcdrv_mem_is_notify(mem)) {
        return HDCDRV_OK;
    }

    hdc_dev = &hdc_ctrl->devices[devid];

    mutex_lock(&hdc_dev->sync_mem_mutex);

    msg_size = sizeof(struct hdcdrv_ctrl_msg_sync_mem_info) + mem->phy_addr_num * sizeof(struct hdcdrv_dma_mem);
    msg = (struct hdcdrv_ctrl_msg_sync_mem_info *)hdc_dev->sync_mem_buf;

    msg->error_code = HDCDRV_OK;
    msg->type = HDCDRV_CTRL_MSG_TYPE_SYNC_MEM_INFO;
    msg->flag = flag;
    msg->phy_addr_num = mem->phy_addr_num;
    msg->alloc_len = mem->alloc_len;
    msg->mem_type = mem->mem_type;
    msg->pid = hdcdrv_get_pid();
    msg->hash_va = mem->hash_va;

    for (i = 0; i < msg->phy_addr_num; i++) {
        msg->mem[i].addr = mem->mem[i].addr;
        msg->mem[i].len = mem->mem[i].len;
        msg->mem[i].resv = 0;
    }

#ifdef HDCDRV_DEBUG
        hdcdrv_info("msg type %d, mem_type %d, flag %d, mem_num %d, pid 0x%lld, hash_va 0x%llx, len %d\n",
                    msg->type, mem->mem_type, msg->flag,
                    msg->phy_addr_num, msg->pid, msg->hash_va, msg->mem[0].len);
#endif

    ret = hdcdrv_non_trans_ctrl_msg_send(devid, (void *)msg, msg_size, msg_size, &len);
    if ((ret != HDCDRV_OK) || (len != sizeof(struct hdcdrv_ctrl_msg_sync_mem_info))
        || (msg->error_code != HDCDRV_OK)) {
        hdcdrv_err("dev_id %d mem info msg send failed. ret %ld ,len %d error code %d\n",
                   devid, ret, len, msg->error_code);
        ret = HDCDRV_SEND_CTRL_MSG_FAIL;
    }

    mutex_unlock(&hdc_dev->sync_mem_mutex);

    return ret;
}

int hdcdrv_mem_adapter(struct hdcdrv_fast_mem *src, struct hdcdrv_fast_mem *dst, struct devdrv_dma_node *node,
                       int *node_idx, int len)
{
    int i = 0;
    int j = 0;
    int idx;
    u32 total_len;
    enum devdrv_dma_direction dir;
    dma_addr_t src_addr = (dma_addr_t)src->mem[i].addr;
    dma_addr_t dst_addr = (dma_addr_t)dst->mem[j].addr;
    u32 src_len = src->mem[i].len;
    u32 dst_len = dst->mem[j].len;
    i++, j++;

    dir = hdcdrv_get_dma_direction();
    total_len = (u32)len;
    if ((total_len > dst->alloc_len) || (total_len > src->alloc_len)) {
        hdcdrv_err("send len too long, send_len %d, src_len %d, dst_len %d\n", total_len, src->alloc_len,
                   dst->alloc_len);
        return HDCDRV_PARA_ERR;
    }

    while (total_len > 0) {
        idx = *node_idx;
        if (src_len == dst_len) {
            if (total_len < src_len) {
                src_len = total_len;
            }
            node[idx].size = src_len;
            node[idx].src_addr = src_addr;
            node[idx].dst_addr = dst_addr;
            total_len -= src_len;

            if (total_len == 0) {
                goto finish;
            }

            src_addr = (dma_addr_t)src->mem[i].addr;
            src_len = src->mem[i].len;
            i++;

            dst_addr = (dma_addr_t)dst->mem[j].addr;
            dst_len = dst->mem[j].len;
            j++;
        } else if (src_len < dst_len) {
            if (total_len < src_len) {
                src_len = total_len;
            }
            node[idx].size = src_len;
            node[idx].src_addr = src_addr;
            node[idx].dst_addr = dst_addr;
            total_len -= src_len;

            if (total_len == 0) {
                goto finish;
            }

            dst_addr += src_len;
            dst_len -= src_len;

            src_addr = (dma_addr_t)src->mem[i].addr;
            src_len = src->mem[i].len;
            i++;
        } else {
            if (total_len < dst_len) {
                dst_len = total_len;
            }
            node[idx].size = dst_len;
            node[idx].src_addr = src_addr;
            node[idx].dst_addr = dst_addr;
            total_len -= dst_len;

            if (total_len == 0) {
                goto finish;
            }

            src_addr += dst_len;
            src_len -= dst_len;

            dst_addr = (dma_addr_t)dst->mem[j].addr;
            dst_len = dst->mem[j].len;
            j++;
        }

    finish:
        node[idx].direction = dir;
        *node_idx = (*node_idx) + 1;
    }

    return HDCDRV_OK;
}

int hdcdrv_get_dma_node_info(struct hdcdrv_fast_mem *src, struct hdcdrv_fast_mem *dst,
    struct hdcdrv_msg_chan *msg_chan, int *node_idx, int len)
{
    if ((src != NULL) && (dst != NULL)) {
        return hdcdrv_mem_adapter(src, dst, msg_chan->node, node_idx, len);
    }

    return HDCDRV_OK;
}

int hdcdrv_fast_mem_check(struct hdcdrv_sq_desc *sq_desc, struct hdcdrv_fast_mem *mem_src_data,
    struct hdcdrv_fast_mem *mem_src_ctrl, struct hdcdrv_fast_mem *mem_dst_data, struct hdcdrv_fast_mem *mem_dst_ctrl)
{
    if ((mem_src_data == NULL) && (mem_dst_data == NULL) && (mem_src_ctrl == NULL) && (mem_dst_ctrl == NULL)) {
        hdcdrv_err("Can't find fast memory, all is null. local session id %d, remote session id %d, "
                   "remote data addr = 0x%llx, remote ctrl addr = 0x%llx, local data addr = 0x%llx, "
                   "local ctrl addr = 0x%llx\n",
                   sq_desc->remote_session, sq_desc->local_session, sq_desc->src_data_addr, sq_desc->src_ctrl_addr,
                   sq_desc->dst_data_addr, sq_desc->dst_ctrl_addr);
        return HDCDRV_PARA_ERR;
    }

    if ((mem_src_data != NULL) && (mem_dst_data == NULL)) {
        hdcdrv_err("fast dst data buffer error, local session id %d, "
                   "remote session id %d, dst buf = 0x%llx\n",
                   sq_desc->remote_session, sq_desc->local_session, sq_desc->dst_data_addr);
        return HDCDRV_PARA_ERR;
    }

    if ((mem_src_data == NULL) && (mem_dst_data != NULL)) {
        hdcdrv_err("fast src data buffer error, local session id %d, "
                   "remote session id %d, src buf = 0x%llx\n",
                   sq_desc->remote_session, sq_desc->local_session, sq_desc->src_data_addr);
        return HDCDRV_PARA_ERR;
    }

    if ((mem_src_ctrl != NULL) && (mem_dst_ctrl == NULL)) {
        hdcdrv_err("fast dst ctrl buffer error, local session id %d, "
                   "remote session id %d, dst buf = 0x%llx\n",
                   sq_desc->remote_session, sq_desc->local_session, sq_desc->dst_ctrl_addr);
        return HDCDRV_PARA_ERR;
    }

    if ((mem_src_ctrl == NULL) && (mem_dst_ctrl != NULL)) {
        hdcdrv_err("fast src ctrl buffer error, local session id %d, "
                   "remote session id %d, src buf = 0x%llx\n",
                   sq_desc->remote_session, sq_desc->local_session, sq_desc->src_ctrl_addr);
        return HDCDRV_PARA_ERR;
    }

    return HDCDRV_OK;
}

/* fast node unlock is in hdcdrv_rx_msg_callback function */
int hdcdrv_fast_dma_copy(struct hdcdrv_msg_chan *msg_chan, u32 head, struct hdcdrv_sq_desc *sq_desc,
                         struct devdrv_asyn_dma_para_info *para, enum devdrv_dma_data_type data_type)
{
    struct hdcdrv_fast_mem *f_mem_src_data = NULL;
    struct hdcdrv_fast_mem *f_mem_src_ctrl = NULL;
    struct hdcdrv_fast_mem *f_mem_dst_data = NULL;
    struct hdcdrv_fast_mem *f_mem_dst_ctrl = NULL;
    struct rb_root *remote_rb = NULL;
    int ret = HDCDRV_OK;
    int node_idx = 0;
    u64 pid;

    pid = (u64)hdc_ctrl->sessions[sq_desc->remote_session].pid;
    remote_rb = &hdc_ctrl->devices[msg_chan->dev_id].remote_rb_mems;

    if (sq_desc->src_data_addr != 0) {
        f_mem_src_data = hdcdrv_get_fast_mem(remote_rb, HDCDRV_FAST_MEM_TYPE_TX_DATA, sq_desc->data_len,
                                             sq_desc->src_data_addr, HDCDRV_NODE_WAIT_TIME_MIN);
    }

    if (sq_desc->src_ctrl_addr != 0) {
        f_mem_src_ctrl = hdcdrv_get_fast_mem(remote_rb, HDCDRV_FAST_MEM_TYPE_TX_CTRL, sq_desc->ctrl_len,
                                             sq_desc->src_ctrl_addr, HDCDRV_NODE_WAIT_TIME_MIN);
    }

    if (sq_desc->dst_data_addr != 0) {
        f_mem_dst_data = hdcdrv_get_fast_mem(&hdc_ctrl->rbtree, HDCDRV_FAST_MEM_TYPE_RX_DATA, sq_desc->data_len,
                                             hdcdrv_get_hash(sq_desc->dst_data_addr, pid), HDCDRV_NODE_WAIT_TIME_MIN);
    }

    if (sq_desc->dst_ctrl_addr != 0) {
        f_mem_dst_ctrl = hdcdrv_get_fast_mem(&hdc_ctrl->rbtree, HDCDRV_FAST_MEM_TYPE_RX_CTRL, sq_desc->ctrl_len,
                                             hdcdrv_get_hash(sq_desc->dst_ctrl_addr, pid), HDCDRV_NODE_WAIT_TIME_MIN);
    }

    if (hdcdrv_fast_mem_check(sq_desc, f_mem_src_data, f_mem_src_ctrl, f_mem_dst_data, f_mem_dst_ctrl) != HDCDRV_OK) {
        if (sq_desc->dst_data_addr != 0) {
            hdcdrv_dfx_node_info(&hdc_ctrl->mem_rbtree, hdcdrv_get_hash(sq_desc->dst_data_addr, pid));
        }
        if (sq_desc->dst_ctrl_addr != 0) {
            hdcdrv_dfx_node_info(&hdc_ctrl->mem_rbtree, hdcdrv_get_hash(sq_desc->dst_ctrl_addr, pid));
        }

        if (sq_desc->src_data_addr != 0) {
            hdcdrv_dfx_node_info(&hdc_ctrl->devices[msg_chan->dev_id].remote_mem_rb_mems, sq_desc->src_data_addr);
        }
        if (sq_desc->src_ctrl_addr != 0) {
            hdcdrv_dfx_node_info(&hdc_ctrl->devices[msg_chan->dev_id].remote_mem_rb_mems, sq_desc->src_ctrl_addr);
        }
        ret = HDCDRV_PARA_ERR;
        goto check_err;
    }

    msg_chan->rx[head].dst_data_addr = sq_desc->dst_data_addr;
    msg_chan->rx[head].len = sq_desc->data_len;
    msg_chan->rx[head].dst_ctrl_addr = sq_desc->dst_ctrl_addr;
    msg_chan->rx[head].ctrl_len = sq_desc->ctrl_len;
    msg_chan->rx[head].local_session = sq_desc->remote_session;
    msg_chan->rx[head].remote_session = sq_desc->local_session;

    msg_chan->rx[head].src_data = f_mem_src_data;
    msg_chan->rx[head].dst_data = f_mem_dst_data;
    msg_chan->rx[head].src_ctrl = f_mem_src_ctrl;
    msg_chan->rx[head].dst_ctrl = f_mem_dst_ctrl;

    if (hdcdrv_get_dma_node_info(f_mem_src_data, f_mem_dst_data, msg_chan, &node_idx, sq_desc->data_len) != HDCDRV_OK) {
        hdcdrv_err("dev %u data memory get adapter failed.\n", msg_chan->dev_id);
        ret = HDCDRV_MEM_NOT_MATCH;
        goto check_err;
    }

    if (hdcdrv_get_dma_node_info(f_mem_src_ctrl, f_mem_dst_ctrl, msg_chan, &node_idx, sq_desc->ctrl_len) != HDCDRV_OK) {
        hdcdrv_err("dev %u data memory get adapter failed.\n", msg_chan->dev_id);
        ret = HDCDRV_MEM_NOT_MATCH;
        goto check_err;
    }

    ret = devdrv_dma_async_link_copy_plus(msg_chan->dev_id, data_type, msg_chan->chan_id, msg_chan->node,
                                          node_idx, para);
    if (ret != HDCDRV_OK) {
        hdcdrv_err("dev %u dma_async_link_copy failed may be the parameters error\n", msg_chan->dev_id);
        ret = (ret == -ENOSPC) ? HDCDRV_DMA_QUE_FULL : HDCDRV_DMA_COPY_FAIL;
        goto check_err;
    }

    return HDCDRV_OK;

check_err:
    hdcdrv_node_status_idle_by_mem(f_mem_src_data);
    hdcdrv_node_status_idle_by_mem(f_mem_dst_data);
    hdcdrv_node_status_idle_by_mem(f_mem_src_ctrl);
    hdcdrv_node_status_idle_by_mem(f_mem_dst_ctrl);

    msg_chan->rx[head].src_data = NULL;
    msg_chan->rx[head].dst_data = NULL;
    msg_chan->rx[head].src_ctrl = NULL;
    msg_chan->rx[head].dst_ctrl = NULL;

    return ret;
}

int hdcdrv_rx_msg_notify_task_handle(struct hdcdrv_msg_chan *msg_chan, struct hdcdrv_sq_desc *sq_desc,
    u32 head)
{
    struct devdrv_asyn_dma_para_info para;
    long ret;

    para.interrupt_and_attr_flag = DEVDRV_LOCAL_IRQ_FLAG;
    para.priv = (void *)msg_chan;
    para.finish_notify = hdcdrv_rx_msg_callback;

    msg_chan->rx[head].local_session = sq_desc->remote_session;
    msg_chan->rx[head].remote_session = sq_desc->local_session;
    if ((ret = hdcdrv_session_inner_check(sq_desc->remote_session, sq_desc->inner_checker)) != HDCDRV_OK) {
        msg_chan->dbg_stat.hdcdrv_rx_msg_notify_task4++;

        hdcdrv_warn("dev %u session(id %d) is closed, drop the msg\n", msg_chan->dev_id, sq_desc->remote_session);
        msg_chan->rx[head].skip_flag = HDCDRV_VALID;
        goto jump_next;
    }

    msg_chan->rx[head].inner_checker = sq_desc->inner_checker;
    para.trans_id = head;

    if (msg_chan->type == HDCDRV_MSG_CHAN_TYPE_NORMAL) {
        ret = hdcdrv_normal_dma_copy(msg_chan, head, sq_desc, &para, msg_chan->data_type);
        if (ret == HDCDRV_DMA_MEM_ALLOC_FAIL) {
            /* wait schedule again when free mem */
            msg_chan->dbg_stat.hdcdrv_rx_msg_notify_task5++;
            return HDCDRV_ERR;
        }
    } else {
        ret = hdcdrv_fast_dma_copy(msg_chan, head, sq_desc, &para, msg_chan->data_type);
    }

    if (ret == HDCDRV_DMA_QUE_FULL) {
        /* dma que full, wait sched again in the following cases:
           1. next send request in hdcdrv_rx_msg_notify
           2. dma completion interrupt in hdcdrv_rx_msg_callback
           3. received monitoring work in hdcdrv_rx_msg_notify_task_check  */
        msg_chan->dbg_stat.hdcdrv_rx_msg_notify_task6++;
        msg_chan->dma_need_submit_flag = HDCDRV_VALID;
        return HDCDRV_ERR;
    } else if (ret != HDCDRV_OK) {
        msg_chan->dbg_stat.hdcdrv_rx_msg_notify_task7++;
        hdcdrv_err("Dma copy failed, ret = %ld, msg_chan type %d, device %u hdcdrv_rx_msg_notify head %d,  \
                    after dma submit, data size %x\n", ret, msg_chan->type, msg_chan->dev_id, head, sq_desc->data_len);

        msg_chan->rx[head].skip_flag = HDCDRV_VALID;

        goto jump_next;
    }

    msg_chan->submit_dma_head = head;
    msg_chan->rx[head].skip_flag = HDCDRV_INVALID;
jump_next:
    if (msg_chan->rx[head].skip_flag == HDCDRV_VALID) {
        if (msg_chan->submit_dma_head == msg_chan->dma_head) {
            msg_chan->dma_head = head;
            msg_chan->submit_dma_head = head;
            tasklet_schedule(&msg_chan->rx_task);
        }
    }

    return HDCDRV_OK;
}

void hdcdrv_rx_msg_notify_task(unsigned long data)
{
    struct hdcdrv_msg_chan *msg_chan = (struct hdcdrv_msg_chan *)((uintptr_t)data);
    struct hdcdrv_sq_desc *sq_desc = NULL;
    u32 head = 0;
    int cnt = 0;
    long ret;

    msg_chan->dma_need_submit_flag = HDCDRV_INVALID;

    msg_chan->rx_notify_task_status.schedule_in++;

    msg_chan->dbg_stat.hdcdrv_rx_msg_notify_task1++;
    do {
        sq_desc = hdcdrv_get_r_sq_desc(msg_chan->chan, &head);
        if ((sq_desc == NULL) || (sq_desc->valid != HDCDRV_VALID)) {
            break;
        }

        rmb();

        msg_chan->dbg_stat.hdcdrv_rx_msg_notify_task2++;

        /* Reach the threshold and schedule out */
        if (cnt >= HDCDRV_RX_BUDGET) {
            msg_chan->dbg_stat.hdcdrv_rx_msg_notify_task3++;
            tasklet_schedule(&msg_chan->rx_notify_task);
            break;
        }

        ret = hdcdrv_rx_msg_notify_task_handle(msg_chan, sq_desc, head);
        if (ret) {
            break;
        }

        sq_desc->remote_session = HDCDRV_SESSION_FD_INVALID;
        sq_desc->local_session = HDCDRV_SESSION_FD_INVALID;
        sq_desc->valid = HDCDRV_INVALID;
        msg_chan->dbg_stat.hdcdrv_rx_msg_notify_task8++;

        hdcdrv_move_r_sq_desc(msg_chan->chan);
        cnt++;
    } while (1);
}

void hdcdrv_rx_msg_notify(void *chan)
{
    struct hdcdrv_dev *dev = NULL;
    struct hdcdrv_msg_chan *msg_chan = hdcdrv_get_msg_chan_priv(chan);

    if (msg_chan == NULL) {
        hdcdrv_err("msg_dev is null!");
        return;
    }

    dev = &hdc_ctrl->devices[msg_chan->dev_id];

    msg_chan->dbg_stat.hdcdrv_rx_msg_notify1++;

    if (dev->valid != HDCDRV_VALID) {
        hdcdrv_err("rx msg notify device %u reset.\n", dev->dev_id);
        return;
    }

    tasklet_schedule(&msg_chan->rx_notify_task);
}

void hdcdrv_rx_msg_notify_task_check(struct hdcdrv_msg_chan *msg_chan)
{
    struct hdcdrv_msg_chan_tasklet_status *tasklet_status = NULL;
    struct hdcdrv_sq_desc *sq_desc = NULL;
    u32 head = 0;

    tasklet_status = &msg_chan->rx_notify_task_status;

    msg_chan->dbg_stat.hdcdrv_rx_msg_notify_task_check1++;

    if (msg_chan->dma_need_submit_flag == HDCDRV_VALID) {
        tasklet_schedule(&msg_chan->rx_notify_task);
        hdcdrv_info("dev %d msg chan %d schedule.\n", msg_chan->dev_id, msg_chan->chan_id);
        return;
    }

    if (hdcdrv_tasklet_status_check(tasklet_status) == HDCDRV_OK) {
        return;
    }

    sq_desc = hdcdrv_get_r_sq_desc(msg_chan->chan, &head);
    if (sq_desc == NULL) {
        return;
    }

    /* no valid msg */
    if (sq_desc->valid != HDCDRV_VALID) {
        return;
    }

    msg_chan->dbg_stat.hdcdrv_rx_msg_notify_task_check2++;

    hdcdrv_info("dev %d msg chan %d rx notify tasklet check schedule\r\n", msg_chan->dev_id, msg_chan->chan_id);

    /* there has msg but not schedule */
    tasklet_schedule(&msg_chan->rx_notify_task);
}

void hdcdrv_mem_avail(int pool_type, struct list_head *target)
{
    struct hdcdrv_msg_chan *msg_chan = NULL;

    if (pool_type == HDCDRV_MEM_POOL_TYPE_RX) {
        msg_chan = list_entry(target, struct hdcdrv_msg_chan, wait_mem_list);
        msg_chan->dbg_stat.hdcdrv_mem_avail1++;
        tasklet_schedule(&msg_chan->rx_notify_task);
    }
}

void hdcdrv_clear_session_rx_buf(int session_fd)
{
    struct hdcdrv_session *session = NULL;
    struct hdcdrv_buf_desc *buf_desc = NULL;
    struct hdcdrv_msg_chan *msg_chan = NULL;
    struct hdcdrv_msg_chan *fast_msg_chan = NULL;
    /* Release the memory of a locally unclosed session */
    session = &hdc_ctrl->sessions[session_fd];
    msg_chan = hdc_ctrl->devices[session->dev_id].msg_chan[session->chan_id];
    fast_msg_chan = hdc_ctrl->devices[session->dev_id].msg_chan[session->fast_chan_id];
    spin_lock_bh(&session->lock);

    while (session->normal_rx.head != session->normal_rx.tail) {
        buf_desc = &session->normal_rx.rx_list[session->normal_rx.head];
        session->normal_rx.head = (session->normal_rx.head + 1) % HDCDRV_SESSION_RX_LIST_MAX_PKT;

        free_mem(buf_desc->buf);
        buf_desc->buf = NULL;
        buf_desc->addr = 0;
    }

    session->normal_rx.head = 0;
    session->normal_rx.tail = 0;

    /* fast session clear */
    session->fast_rx.head = 0;
    session->fast_rx.tail = 0;

    if (msg_chan != NULL) {
        if (msg_chan->rx_trigger_flag == HDCDRV_VALID) {
            msg_chan->rx_trigger_flag = HDCDRV_INVALID;
            tasklet_schedule(&msg_chan->rx_task);
        }
    }
    if (fast_msg_chan != NULL) {
        if (fast_msg_chan->rx_trigger_flag == HDCDRV_VALID) {
            fast_msg_chan->rx_trigger_flag = HDCDRV_INVALID;
            tasklet_schedule(&fast_msg_chan->rx_task);
        }
    }
    spin_unlock_bh(&session->lock);
}

int hdcdrv_alloc_session(int service_type, int run_env, int root_privilege)
{
    int i, max, base, fd;
    int session_fd = -1;
    int conn_feature = hdcdrv_get_service_conn_feature(service_type);
    int *cur = NULL;

    mutex_lock(&hdc_ctrl->mutex);

again:
    if (conn_feature == HDCDRV_SERVICE_LONG_CONN) {
        max = HDCDRV_SURPORT_MAX_LONG_SESSION;
        base = 0;
        cur = &hdc_ctrl->cur_alloc_session;
    } else {
        max = HDCDRV_SURPORT_MAX_SHORT_SESSION;
        base = HDCDRV_SURPORT_MAX_LONG_SESSION;
        cur = &hdc_ctrl->cur_alloc_short_session;
    }

    for (i = 0; i < max; i++) {
        fd = (*cur + i) % max + base;
        if (hdcdrv_get_session_status(&hdc_ctrl->sessions[fd]) == HDCDRV_SESSION_STATUS_IDLE) {
            session_fd = fd;
            hdcdrv_set_session_status(&hdc_ctrl->sessions[fd], HDCDRV_SESSION_STATUS_CONN);
            *cur = (fd + 1) % max + base;
            break;
        }
    }

    if ((session_fd == -1) && (conn_feature == HDCDRV_SERVICE_SHORT_CONN) &&
        (run_env != HDCDRV_SESSION_RUN_ENV_CONTAINER) && root_privilege) {
        hdcdrv_info("service %s conn_feature %d no session, try another",
            hdcdrv_sevice_str(service_type), conn_feature);
        conn_feature = HDCDRV_SERVICE_LONG_CONN;
        goto again;
    }

    mutex_unlock(&hdc_ctrl->mutex);

    if (session_fd < 0) {
        hdcdrv_print_status();
    }

    return session_fd;
}

u32 hdcdrv_alloc_fast_msg_chan(int dev_id, int service_type)
{
    struct hdcdrv_dev *dev = &hdc_ctrl->devices[dev_id];
    int i;
    u32 chan_id = dev->normal_chan_num;
    int session_cnt;

    mutex_lock(&dev->mutex);

    /* Find the msg chan with the least number of sessions */
    session_cnt = dev->msg_chan[dev->normal_chan_num]->session_cnt;
    for (i = dev->normal_chan_num; i < dev->msg_chan_cnt; i++) {
        if (session_cnt > dev->msg_chan[i]->session_cnt) {
            session_cnt = dev->msg_chan[i]->session_cnt;
            chan_id = i;
        }
    }

    dev->msg_chan[chan_id]->session_cnt++;

    mutex_unlock(&dev->mutex);

    return chan_id;
}

void hdcdrv_mod_msg_chan_session_cnt(int dev_id, u32 msg_chan_id, int num)
{
    struct hdcdrv_dev *dev = &hdc_ctrl->devices[dev_id];
    if (dev->msg_chan[msg_chan_id] == NULL) {
        hdcdrv_warn("dev %d msg_chan is null, msg chan have been already closed\n", dev_id);
        return;
    }
    mutex_lock(&dev->mutex);
    dev->msg_chan[msg_chan_id]->session_cnt += num;
    mutex_unlock(&dev->mutex);
}

enum devdrv_dma_data_type hdcdrv_get_dma_data_type(int msg_chan_type, int chan_id)
{
    enum devdrv_dma_data_type data_type;

    /* Low-level use common type */
    if (msg_chan_type == HDCDRV_MSG_CHAN_TYPE_NORMAL) {
        if (hdcdrv_get_service_level(chan_id) == HDCDRV_SERVICE_HIGH_LEVEL) {
#ifdef CFG_SOC_PLATFORM_CLOUD
            data_type = DEVDRV_DMA_DATA_TRAFFIC;
#else
            data_type = DEVDRV_DMA_DATA_COMMON;
#endif
        } else {
            data_type = DEVDRV_DMA_DATA_COMMON;
        }
    } else {
        data_type = DEVDRV_DMA_DATA_TRAFFIC;
    }
    return data_type;
}

void hdcdrv_session_init(int local_session, int remote_session, int dev_id, int service_type, int msg_chan_id,
    char *session_id, int run_env)
{
    struct hdcdrv_session *session = &hdc_ctrl->sessions[local_session];
    struct hdcdrv_dev *hdc_dev = &hdc_ctrl->devices[dev_id];

    hdcdrv_clear_session_rx_buf(local_session);
    hdcdrv_sid_copy(session->session_id, HDCDRV_SID_LEN, session_id, HDCDRV_SID_LEN);
    /* after hdcdrv_sid_copy */
    hdcdrv_inner_checker_set(session);
    session->local_session_fd = local_session;
    session->remote_session_fd = remote_session;
    session->dev_id = dev_id;
    session->service_type = service_type;
    session->chan_id = service_type % hdc_dev->normal_chan_num;
    session->msg_chan = hdc_dev->msg_chan[session->chan_id];
    session->fast_chan_id = msg_chan_id;
    session->fast_msg_chan = hdc_dev->msg_chan[msg_chan_id];
    session->service = &hdc_dev->service[service_type];
    session->run_env = run_env;
    session->timeout_jiffies.send_timeout = msecs_to_jiffies(HDCDRV_SESSION_DEFAULT_TIMEOUT);
    session->timeout_jiffies.recv_timeout = msecs_to_jiffies(HDCDRV_SESSION_DEFAULT_TIMEOUT);
    session->timeout_jiffies.fast_send_timeout = msecs_to_jiffies(HDCDRV_SESSION_DEFAULT_TIMEOUT);
    session->timeout_jiffies.fast_recv_timeout = msecs_to_jiffies(HDCDRV_SESSION_DEFAULT_TIMEOUT);

    if (memset_s((void *)&session->stat, sizeof(session->stat), 0, sizeof(session->stat)) != EOK) {
        hdcdrv_err("dev %d memset_s return failed\n", dev_id);
        return;
    }
    session->pid = HDCDRV_INVALID_VALUE;
    session->delay_work_flag = 0;
}


void hdcdrv_session_uid(int local_session, int current_euid, int current_uid, int current_root_privilege)
{
    struct hdcdrv_session *session = &hdc_ctrl->sessions[local_session];

    session->euid = current_euid;
    session->uid = current_uid;
    session->root_privilege = current_root_privilege;
}

void hdcdrv_free_service_conn_req(int dev_id, int service_type)
{
    struct hdcdrv_service *service = NULL;
    struct hdcdrv_connect_list *connect_t = NULL;

    service = &hdc_ctrl->devices[dev_id].service[service_type];

    mutex_lock(&service->mutex);

    while (service->conn_list_head != NULL) {
        connect_t = service->conn_list_head;
        service->conn_list_head = service->conn_list_head->next;
        kfree(connect_t);
        connect_t = NULL;
    }

    mutex_unlock(&service->mutex);
}

int hdcdrv_ctrl_msg_connect_handle(struct hdcdrv_ctrl_msg *msg, struct hdcdrv_service *service,
    u32 devid)
{
    struct hdcdrv_session *session = NULL;
    struct hdcdrv_connect_list *connect_n = NULL;
    struct hdcdrv_connect_list *connect_t = NULL;
    int session_fd, run_env;
    int service_type = msg->connect_msg.service_type;
    int msg_chan_id = msg->connect_msg.msg_chan_id;
    char *session_id = msg->connect_msg.session_id;

    connect_n = (struct hdcdrv_connect_list *)kzalloc(sizeof(struct hdcdrv_connect_list), GFP_KERNEL | __GFP_ACCOUNT);
    if (unlikely(connect_n == NULL)) {
        hdcdrv_err("dev %u alloc connect_t failed, client_session %d\n", devid, msg->connect_msg.client_session);
        return HDCDRV_MEM_ALLOC_FAIL;
    }

    session_fd = hdcdrv_alloc_session(service_type, msg->connect_msg.run_env, msg->connect_msg.root_privilege);
    if (session_fd < 0) {
        kfree(connect_n);
        connect_n = NULL;
        hdcdrv_err("dev_id %u service_type %s has no free session\n", devid, hdcdrv_sevice_str(service_type));
        return HDCDRV_NO_SESSION;
    }

    session = &hdc_ctrl->sessions[session_fd];

    run_env = msg->connect_msg.run_env;

    hdcdrv_session_uid(session_fd, msg->connect_msg.euid, msg->connect_msg.uid, msg->connect_msg.root_privilege);
    hdcdrv_session_init(session_fd, msg->connect_msg.client_session, devid, service_type, msg_chan_id, session_id,
                        run_env);

    hdcdrv_mod_msg_chan_session_cnt((int)devid, session->chan_id, 1);

    connect_n->session_fd = session_fd;
    connect_n->next = NULL;

    /* wakeup accept thread */
    mutex_lock(&service->mutex);
    if (service->conn_list_head == NULL) {
        service->conn_list_head = connect_n;
    } else {
        connect_t = service->conn_list_head;
        while (connect_t->next != NULL) {
            connect_t = connect_t->next;
        }
        connect_t->next = connect_n;
    }

    mutex_unlock(&service->mutex);

    hdcdrv_delay_work_set(session, &session->close_unknow_session, HDCDRV_DELAY_UNKNOWN_SESSION_BIT,
                          HDCDRV_SESSION_RECLAIM_TIMEOUT);

    wake_up_interruptible(&service->wq_conn_avail);
    hdcdrv_epoll_wake_up(service->epfd);
    return HDCDRV_OK;
}

int hdcdrv_ctrl_msg_connect(u32 devid, struct hdcdrv_ctrl_msg *msg)
{
    struct hdcdrv_dev *hdc_dev = NULL;
    struct hdcdrv_service *service = NULL;
    int service_type = msg->connect_msg.service_type;
    int msg_chan_id = msg->connect_msg.msg_chan_id;
    int ret;

    hdc_dev = &hdc_ctrl->devices[devid];

    if (msg_chan_id >= hdc_dev->msg_chan_cnt) {
        hdcdrv_err("dev_id %u msg_chan_id %d is error\n", devid, msg_chan_id);
        return HDCDRV_PARA_ERR;
    }

    if (service_type >= HDCDRV_SURPORT_MAX_SERVICE) {
        hdcdrv_err("dev_id %u service_type %s is error\n",
                   devid, hdcdrv_sevice_str(service_type));
        return HDCDRV_PARA_ERR;
    }

    service = &hdc_dev->service[service_type];
    if (service->listen_status == HDCDRV_INVALID) {
        // send to remote to info local service not listen.
        hdcdrv_warn_limit("dev_id %u service_type %s listen_status %d\n",
            devid, hdcdrv_sevice_str(service_type), service->listen_status);
        return HDCDRV_REMOTE_SERVICE_NO_LISTENING;
    }

    ret = hdcdrv_ctrl_msg_connect_handle(msg, service, devid);
    if (ret != HDCDRV_OK) {
        return ret;
    }

    return HDCDRV_OK;
}


int hdcdrv_ctrl_msg_connect_reply(u32 devid, struct hdcdrv_ctrl_msg *msg)
{
    struct hdcdrv_session *session = NULL;
    int client_session = msg->connect_msg_reply.client_session;
    int server_session = msg->connect_msg_reply.server_session;
    char *session_id = msg->connect_msg_reply.session_id;
    long ret;

    ret = hdcdrv_session_alive_check(client_session, session_id);
    if (ret != HDCDRV_OK) {
        /* 这里如果session已经close，也需要返回错误 */
        return ret;
    }

    session = &hdc_ctrl->sessions[client_session];
    session->remote_session_fd = server_session;

    session->run_env = msg->connect_msg_reply.run_env;

#ifdef HDCDRV_DEBUG
    hdcdrv_info("dev id %d client_session %d server_session %d connect reply wait up\n", devid, client_session,
                server_session);
#endif
    wake_up_interruptible(&session->wq_conn);

    return HDCDRV_OK;
}

void hdcdrv_remote_close_work(struct work_struct *p_work)
{
    int ret;
    struct hdcdrv_cmd_close close_cmd;
    struct hdcdrv_session *session = container_of(p_work, struct hdcdrv_session, remote_close.work);

    hdcdrv_delay_work_flag_clear(session, HDCDRV_DELAY_REMOTE_CLOSE_BIT);

    if (hdcdrv_get_session_status(session) == HDCDRV_SESSION_STATUS_REMOTE_CLOSED) {
#ifdef HDCDRV_DEBUG
        hdcdrv_info("remote close work. devid %d, chanid %d, sessionid %d\n", session->dev_id, session->chan_id,
                    session->local_session_fd);
#endif
        close_cmd.session = session->local_session_fd;
        hdcdrv_sid_copy(close_cmd.session_id, HDCDRV_SID_LEN, session->session_id, HDCDRV_SID_LEN);
        close_cmd.pid = session->pid;
        ret = hdcdrv_close(&close_cmd, HDCDRV_CLOSE_TYPE_REMOTE_CLOSED_POST);
        if (ret != HDCDRV_OK) {
            hdcdrv_warn("devid %d, chanid %d, sessionid %d close session failed\n", session->dev_id, session->chan_id,
                        session->local_session_fd);
        }
    }
}

int hdcdrv_ctrl_msg_close(u32 devid, struct hdcdrv_ctrl_msg *msg)
{
    struct hdcdrv_session *session = NULL;
    int session_fd = msg->close_msg.remote_session;
    long ret;

    if ((session_fd >= HDCDRV_SURPORT_MAX_SESSION) || (session_fd < 0)) {
        hdcdrv_err("session_fd %d is illegal\n", session_fd);
        return HDCDRV_PARA_ERR;
    }

    ret = hdcdrv_session_state_to_remote_close(session_fd, msg->close_msg.session_id);
    if (ret != HDCDRV_OK) {
        if (ret == HDCDRV_SESSION_HAS_CLOSED) {
            return HDCDRV_OK;
        } else {
            return ret;
        }
    }

    session = &hdc_ctrl->sessions[session_fd];
    session->local_close_state = HDCDRV_CLOSE_TYPE_REMOTE_CLOSED_PRE;
    session->remote_close_state = msg->close_msg.session_close_state;

    hdcdrv_delay_work_set(session, &session->remote_close, HDCDRV_DELAY_REMOTE_CLOSE_BIT,
                          HDCDRV_SESSION_REMOTE_CLOSED_TIMEOUT);

    return HDCDRV_OK;
}

void hdcdrv_set_device_status(int devid, u32 valid)
{
    struct hdcdrv_dev *dev = &hdc_ctrl->devices[devid];
    dev->valid = valid;
    hdcdrv_info("devid %d set status %d finished\n", devid, valid);
}

void hdcdrv_set_device_reset(u32 devid)
{
    struct hdcdrv_dev *dev = &hdc_ctrl->devices[devid];
    hdcdrv_reset_dev(dev);
}
void hdcdrv_set_device_para(u32 devid, int normal_chan_num)
{
    struct hdcdrv_dev *dev = &hdc_ctrl->devices[devid];
    if (normal_chan_num > 0) {
        dev->normal_chan_num = normal_chan_num;
    }
    hdcdrv_info("dev_id %d normal chan num %d\n", devid, normal_chan_num);
}

int hdcdrv_ctrl_msg_sync(u32 devid, struct hdcdrv_ctrl_msg *msg)
{
    int segment = msg->sync_msg.segment;
    int peer_dev_id = msg->sync_msg.peer_dev_id;

    if (segment < 0) {
        hdcdrv_err("dev %d segment(%d) is invalid\n", devid, segment);
        return HDCDRV_PARA_ERR;
    }
    if (hdc_ctrl->devices[devid].dev == NULL) {
        hdcdrv_info("devid %d is not init instance\n", devid);
        return HDCDRV_DEVICE_NOT_READY;
    }

    hdcdrv_set_segment(segment);
    hdcdrv_set_peer_dev_id(devid, peer_dev_id);

    /* notice host device devid */
    msg->sync_msg.peer_dev_id = devid;
    hdcdrv_info("dev_id %d peer dev id %d.\n", devid, peer_dev_id);
    return HDCDRV_OK;
}

int hdcdrv_ctrl_edge_check(u32 devid, void *data, u32 in_data_len, u32 out_data_len, u32 *p_real_out_len)
{
    struct hdcdrv_ctrl_msg *msg = (struct hdcdrv_ctrl_msg *)data;
    struct hdcdrv_ctrl_msg_sync_mem_info *mem = NULL;
    u32 in_len_min = 0;
    u32 out_len_min = 0;

    if ((devid >= HDCDRV_SURPORT_MAX_DEV) || (msg == NULL) || (p_real_out_len == NULL)) {
        hdcdrv_err("dev_id overflow, or msg and real_out_len is null\n");
        return HDCDRV_PARA_ERR;
    }

    if (msg->type != HDCDRV_CTRL_MSG_TYPE_SYNC_MEM_INFO) {
        in_len_min = sizeof(struct hdcdrv_ctrl_msg);
        out_len_min = sizeof(struct hdcdrv_ctrl_msg);
    } else {
        mem = (struct hdcdrv_ctrl_msg_sync_mem_info *)data;
        if ((u32)mem->phy_addr_num > HDCDRV_MEM_MAX_PHY_NUM) {
            hdcdrv_err("dev_id %d phy_addr_num is %d\n", devid, mem->phy_addr_num);
            return HDCDRV_PARA_ERR;
        }
        in_len_min = sizeof(struct hdcdrv_ctrl_msg_sync_mem_info) + mem->phy_addr_num * sizeof(struct hdcdrv_dma_mem);
        out_len_min = sizeof(struct hdcdrv_ctrl_msg_sync_mem_info);
    }

    if ((in_data_len < in_len_min) || (out_data_len < out_len_min)) {
        hdcdrv_err("dev %d buffer length error, data in len %d, min size %d, out len %d, min size %d\n", devid,
            in_data_len, in_len_min, out_data_len, out_len_min);
        return HDCDRV_PARA_ERR;
    }

    return HDCDRV_OK;
}

int hdcdrv_ctrl_msg_recv(u32 devid, void *data, u32 in_data_len, u32 out_data_len, u32 *real_out_len)
{
    struct hdcdrv_ctrl_msg *msg = (struct hdcdrv_ctrl_msg *)data;

    if (hdcdrv_ctrl_edge_check(devid, data, in_data_len, out_data_len, real_out_len) != HDCDRV_OK) {
        hdcdrv_err("dev %d ctrl msg parameters check failed\n", devid);
        return HDCDRV_PARA_ERR;
    }

    msg->error_code = HDCDRV_OK;
    *real_out_len = (sizeof(struct hdcdrv_ctrl_msg));

    switch (msg->type) {
        case HDCDRV_CTRL_MSG_TYPE_CONNECT:
            msg->error_code = hdcdrv_ctrl_msg_connect(devid, msg);
            break;
        case HDCDRV_CTRL_MSG_TYPE_CONNECT_REPLY:
            msg->error_code = hdcdrv_ctrl_msg_connect_reply(devid, msg);
            break;
        case HDCDRV_CTRL_MSG_TYPE_CLOSE:
            msg->error_code = hdcdrv_ctrl_msg_close(devid, msg);
            break;
        case HDCDRV_CTRL_MSG_TYPE_SYNC:
            msg->error_code = hdcdrv_ctrl_msg_sync(devid, msg);
            break;
        case HDCDRV_CTRL_MSG_TYPE_RESET:
            hdcdrv_set_device_reset(devid);
            break;
        case HDCDRV_CTRL_MSG_TYPE_CHAN_SET:
            hdcdrv_set_device_para(devid, msg->chan_set_msg.normal_chan_num);
            break;
        case HDCDRV_CTRL_MSG_TYPE_SYNC_MEM_INFO:
            *real_out_len = sizeof(struct hdcdrv_ctrl_msg_sync_mem_info);
            msg->error_code = hdcdrv_set_mem_info(devid, (struct hdcdrv_ctrl_msg_sync_mem_info *)data);
            break;
        default:
            hdcdrv_err("dev %d cmd %d is illegal\n", devid, msg->type);
            return HDCDRV_PARA_ERR;
    }

    if ((msg->error_code != HDCDRV_OK) && (msg->type != HDCDRV_CTRL_MSG_TYPE_CONNECT)) {
        hdcdrv_err_limit("dev %d cmd %d executed Failed, error_code %d.\n", devid, msg->type, msg->error_code);
    }

    return HDCDRV_OK;
}

long hdcdrv_dev_para_check(int dev_id, int service_type)
{
    if ((dev_id >= HDCDRV_SURPORT_MAX_DEV) || (dev_id < 0)) {
        hdcdrv_err("dev_id %d is illegal\n", dev_id);
        return HDCDRV_PARA_ERR;
    }

    if ((service_type >= HDCDRV_SURPORT_MAX_SERVICE) || (service_type < 0)) {
        hdcdrv_err("dev %d service_type %d is illegal\n", dev_id, service_type);
        return HDCDRV_PARA_ERR;
    }

    if (hdc_ctrl->devices[dev_id].valid != HDCDRV_VALID) {
        return HDCDRV_DEVICE_NOT_READY;
    }

    return HDCDRV_OK;
}

long hdcdrv_cmd_get_peer_dev_id(struct hdcdrv_cmd_get_peer_dev_id *cmd)
{
    long ret;

    /* reuse dev check func with default service type */
    ret = hdcdrv_dev_para_check(cmd->dev_id, 0);
    if (ret) {
        return ret;
    }

    cmd->peer_dev_id = hdcdrv_get_peer_dev_id(cmd->dev_id);

    return HDCDRV_OK;
}

long hdcdrv_config(struct hdcdrv_cmd_config *cmd)
{
    if (hdc_ctrl->segment == HDCDRV_INVALID_PACKET_SEGMENT) {
        return HDCDRV_PARA_ERR;
    }

    if (cmd->segment > hdcdrv_mem_block_capacity()) {
        cmd->segment = hdcdrv_mem_block_capacity();
    }

    hdcdrv_info("pid %d use segment %d.\n", cmd->pid, cmd->segment);

    return HDCDRV_OK;
}

long hdcdrv_set_service_level(struct hdcdrv_cmd_set_service_level *cmd)
{
    int service_type = cmd->service_type;

    if ((service_type >= HDCDRV_SURPORT_MAX_SERVICE) || (service_type < 0)) {
        hdcdrv_err("service_type %d is illegal\n", service_type);
        return HDCDRV_PARA_ERR;
    }

    if ((cmd->level != HDCDRV_SERVICE_HIGH_LEVEL) && (cmd->level != HDCDRV_SERVICE_LOW_LEVEL)) {
        hdcdrv_err("service level %d is illegal\n", cmd->level);
        return HDCDRV_PARA_ERR;
    }

    return HDCDRV_OK;
}

long hdcdrv_server_create(struct hdcdrv_ctx *ctx, struct hdcdrv_cmd_server_create *cmd)
{
    struct hdcdrv_dev *dev = NULL;
    struct hdcdrv_service *service = NULL;
    long ret;

    if (ctx == NULL) {
        hdcdrv_err("dev_id %d service_type %s, hdc ctx is null.\n", cmd->dev_id, hdcdrv_sevice_str(cmd->service_type));
        return HDCDRV_PARA_ERR;
    }

    ret = hdcdrv_dev_para_check(cmd->dev_id, cmd->service_type);
    if (ret) {
        return ret;
    }

    dev = &hdc_ctrl->devices[cmd->dev_id];
    service = &dev->service[cmd->service_type];

    hdcdrv_info("dev_id %d service_type %s server create\n",
                cmd->dev_id, hdcdrv_sevice_str(cmd->service_type));

    mutex_lock(&service->mutex);

    if (service->listen_status == HDCDRV_VALID) {
        mutex_unlock(&service->mutex);
        hdcdrv_err("dev_id %d service_type %s has already been created\n",
                   cmd->dev_id, hdcdrv_sevice_str(cmd->service_type));
        return HDCDRV_SERVICE_LISTENING;
    }

    service->listen_pid = cmd->pid;
    service->listen_status = HDCDRV_VALID;
    if (memset_s((void *)&service->stat, sizeof(service->stat), 0, sizeof(service->stat)) != EOK) {
        service->listen_pid = 0;
        service->listen_status = HDCDRV_INVALID;
        mutex_unlock(&service->mutex);
        hdcdrv_err("memset_s return failed\n");
        return HDCDRV_SAFE_MEM_OP_FAIL;
    }

    hdcdrv_bind_server_ctx(ctx, service, cmd->dev_id, cmd->service_type);

    mutex_unlock(&service->mutex);

    return HDCDRV_OK;
}

long hdcdrv_server_para_check(int dev_id, int service_type)
{
    if ((dev_id >= HDCDRV_SURPORT_MAX_DEV) || (dev_id < 0)) {
        hdcdrv_err("dev_id %d is illegal\n", dev_id);
        return HDCDRV_PARA_ERR;
    }

    if ((service_type >= HDCDRV_SURPORT_MAX_SERVICE)  || (service_type < 0)) {
        hdcdrv_err("service_type %d is illegal\n", service_type);
        return HDCDRV_PARA_ERR;
    }

    return HDCDRV_OK;
}

long hdcdrv_server_free(struct hdcdrv_service *service, int dev_id, int service_type)
{
    mutex_lock(&service->mutex);

    if (service->listen_status == HDCDRV_INVALID) {
        mutex_unlock(&service->mutex);
        hdcdrv_err("dev_id %d service_type %s has not been created\n",
                   dev_id, hdcdrv_sevice_str(service_type));
        return HDCDRV_SERVICE_NO_LISTENING;
    }

    hdcdrv_unbind_server_ctx(service);
    service->listen_status = HDCDRV_INVALID;

    mutex_unlock(&service->mutex);

    wake_up_interruptible(&service->wq_conn_avail);
    hdcdrv_free_service_conn_req(dev_id, service_type);

    return HDCDRV_OK;
}

long hdcdrv_server_destroy(struct hdcdrv_cmd_server_destroy *cmd)
{
    struct hdcdrv_dev *dev = NULL;
    struct hdcdrv_service *service = NULL;
    long ret;

    ret = hdcdrv_server_para_check(cmd->dev_id, cmd->service_type);
    if (ret) {
        return ret;
    }

    dev = &hdc_ctrl->devices[cmd->dev_id];
    service = &dev->service[cmd->service_type];

    if (service->listen_pid != cmd->pid) {
        hdcdrv_warn("dev_id %d service %s pid %d destroy failed owner pid %lld\n",
                    cmd->dev_id, hdcdrv_sevice_str(cmd->service_type), cmd->pid, service->listen_pid);
        return HDCDRV_OK;
    }

    hdcdrv_info("dev_id %d service_type %s server destroy\n", cmd->dev_id, hdcdrv_sevice_str(cmd->service_type));

    return hdcdrv_server_free(service, cmd->dev_id, cmd->service_type);
}

long hdcdrv_accept(struct hdcdrv_cmd_accept *cmd)
{
    struct hdcdrv_dev *dev = NULL;
    struct hdcdrv_service *service = NULL;
    struct hdcdrv_session *session = NULL;
    struct hdcdrv_connect_list *connect_t = NULL;
    struct hdcdrv_ctrl_msg msg;
    u32 container_id;
    u32 len = 0;
    long ret;

    ret = hdcdrv_dev_para_check(cmd->dev_id, cmd->service_type);
    if (ret) {
        return ret;
    }

    container_id = hdcdrv_get_container_id();
    if (container_id >= HDCDRV_DOCKER_MAX_NUM) {
        hdcdrv_err("dev_id %d service_type %s get container_id(%d) illegal\n",
                   cmd->dev_id, hdcdrv_sevice_str(cmd->service_type), container_id);
        return HDCDRV_PARA_ERR;
    }

    dev = &hdc_ctrl->devices[cmd->dev_id];
    service = &dev->service[cmd->service_type];

#ifdef HDCDRV_DEBUG
    hdcdrv_info("dev_id %d service_type %s accept\n", cmd->dev_id, hdcdrv_sevice_str(cmd->service_type));
#endif

retry_next:
    mutex_lock(&service->mutex);

    if (service->listen_status == HDCDRV_INVALID) {
        mutex_unlock(&service->mutex);
        hdcdrv_err("dev_id %d service_type %s has not been created\n",
                   cmd->dev_id, hdcdrv_sevice_str(cmd->service_type));
        return HDCDRV_SERVICE_NO_LISTENING;
    }

    while (service->conn_list_head == NULL) {
        mutex_unlock(&service->mutex);

#ifdef HDCDRV_DEBUG
        hdcdrv_info("dev %d service %s accept wait\n", cmd->dev_id, hdcdrv_sevice_str(cmd->service_type));
#endif
        ret = wait_event_interruptible(service->wq_conn_avail,
                                       ((service->conn_list_head != NULL) || (dev->valid != HDCDRV_VALID) ||
                                        (service->listen_status == HDCDRV_INVALID)));
        if (ret) {
            hdcdrv_warn("dev_id %d service_type %s accept wait ret %d\n",
                        cmd->dev_id, hdcdrv_sevice_str(cmd->service_type), (int)ret);
            return ret;
        }

        if ((dev->valid != HDCDRV_VALID) || (service->listen_status == HDCDRV_INVALID)) {
            hdcdrv_info("dev_id %d service_type %s accept wait dev %d quit, dev status %d, listen status %d\n",
                        cmd->dev_id, hdcdrv_sevice_str(cmd->service_type),
                        cmd->dev_id, dev->valid, service->listen_status);
            return HDCDRV_DEVICE_RESET;
        }

        mutex_lock(&service->mutex);
    }

    connect_t = service->conn_list_head;
    service->conn_list_head = service->conn_list_head->next;

    mutex_unlock(&service->mutex);

    session = &hdc_ctrl->sessions[connect_t->session_fd];
    kfree(connect_t);
    connect_t = NULL;

#ifndef HDC_ENV_DEVICE
    session->run_env = hdcdrv_get_session_run_env();
#endif

    msg.type = HDCDRV_CTRL_MSG_TYPE_CONNECT_REPLY;
    msg.connect_msg_reply.client_session = session->remote_session_fd;
    msg.connect_msg_reply.server_session = session->local_session_fd;
    hdcdrv_sid_copy(msg.connect_msg_reply.session_id, HDCDRV_SID_LEN, session->session_id, HDCDRV_SID_LEN);
    msg.connect_msg_reply.run_env = session->run_env;
    msg.error_code = HDCDRV_OK;

    ret = hdcdrv_non_trans_ctrl_msg_send((u32)cmd->dev_id, (void *)&msg, sizeof(msg), sizeof(msg), &len);
    if ((ret != HDCDRV_OK) || (len != sizeof(msg)) || (msg.error_code != HDCDRV_OK)) {
        hdcdrv_mod_msg_chan_session_cnt(session->dev_id, session->chan_id, -1);
        hdcdrv_set_session_status(session, HDCDRV_SESSION_STATUS_IDLE);
        hdcdrv_err("dev %d service %s conn reply msg send failed. ret %ld, len %d, error_code %d\n", cmd->dev_id,
                   hdcdrv_sevice_str(cmd->service_type), ret, len, msg.error_code);

        if (((ret == 0)) && (len == sizeof(msg)) && (msg.error_code != HDCDRV_OK)) {
            goto retry_next;
        }

        return HDCDRV_SEND_CTRL_MSG_FAIL;
    }

    session->container_id = container_id;

    /* response cmd */
    cmd->session = session->local_session_fd;
    hdcdrv_sid_copy(cmd->session_id, HDCDRV_SID_LEN, session->session_id, HDCDRV_SID_LEN);

    mutex_lock(&service->mutex);
    service->accept_session_num++;
    mutex_unlock(&service->mutex);

#ifdef HDCDRV_DEBUG
    hdcdrv_info("dev %d service %s accept session_fd %d\n",
                cmd->dev_id, hdcdrv_sevice_str(cmd->service_type), cmd->session);
#endif

    return HDCDRV_OK;
}

long hdcdrv_connect_wait_replay(struct hdcdrv_dev *dev, struct hdcdrv_session *session)
{
    long ret;

    ret = wait_event_interruptible_timeout(session->wq_conn, (session->remote_session_fd != HDCDRV_INVALID_VALUE) ||
                                           (dev->valid != HDCDRV_VALID),
                                           HDCDRV_CONN_TIMEOUT);
    if (ret <= 0) {
        hdcdrv_err("dev %d connect wait error %d, local session_fd %x, remote session_fd %x, pid %lld\n", dev->dev_id,
            (int)ret, session->local_session_fd, session->remote_session_fd, session->pid);
        return (ret == 0) ? HDCDRV_CONNECT_TIMEOUT : ret;
    }

    if (dev->valid != HDCDRV_VALID) {
        hdcdrv_err("device %d invalid, connect ret %d remote session %d\n", dev->dev_id, (int)ret,
                   session->remote_session_fd);
        return HDCDRV_DEVICE_RESET;
    }

    return HDCDRV_OK;
}

long hdcdrv_connect(struct hdcdrv_cmd_conncet *cmd)
{
    struct hdcdrv_dev *dev = NULL;
    struct hdcdrv_service *service = NULL;
    struct hdcdrv_session *session = NULL;
    struct hdcdrv_ctrl_msg msg;
    u32 container_id;
    u32 len = 0;
    int session_fd;
    long ret;
    int msg_chan_id;
    char session_id[HDCDRV_SID_LEN];
    int run_env;

    ret = hdcdrv_dev_para_check(cmd->dev_id, cmd->service_type);
    if (ret) {
        return ret;
    }

    container_id = hdcdrv_get_container_id();
    if (container_id >= HDCDRV_DOCKER_MAX_NUM) {
        hdcdrv_err("dev_id %d service_type %s get container_id(%d) illegal\n",
                   cmd->dev_id, hdcdrv_sevice_str(cmd->service_type), container_id);
        return HDCDRV_PARA_ERR;
    }

    dev = &hdc_ctrl->devices[cmd->dev_id];
    service = &dev->service[cmd->service_type];

    mutex_lock(&service->mutex);
    service->connect_session_num_total++;
    mutex_unlock(&service->mutex);

    /* host can get run evn, set it here when connet in host */
#ifdef HDC_ENV_DEVICE
    run_env = HDCDRV_SESSION_RUN_ENV_UNKNOW;
#else
    run_env = hdcdrv_get_session_run_env();
#endif

    if (current->cred == NULL) {
            hdcdrv_warn("current->cred is null\n");
            msg.connect_msg.euid = -1;
            msg.connect_msg.uid =  -1;
    } else {
            msg.connect_msg.euid = current->cred->euid.val;
            msg.connect_msg.uid = current->cred->uid.val;
    }

    msg.connect_msg.root_privilege = (msg.connect_msg.euid == 0) ? 1 : 0;

    session_fd = hdcdrv_alloc_session(cmd->service_type, run_env, msg.connect_msg.root_privilege);
    if (session_fd < 0) {
        hdcdrv_err("dev_id %d service_type %s has no session resources available\n",
                   cmd->dev_id, hdcdrv_sevice_str(cmd->service_type));
        return HDCDRV_NO_SESSION;
    }

    msg_chan_id = hdcdrv_alloc_fast_msg_chan(cmd->dev_id, cmd->service_type);
    hdcdrv_sid_gen(session_id, HDCDRV_SID_LEN);

    msg.type = HDCDRV_CTRL_MSG_TYPE_CONNECT;
    msg.error_code = HDCDRV_OK;
    msg.connect_msg.client_session = session_fd;
    msg.connect_msg.service_type = cmd->service_type;
    msg.connect_msg.msg_chan_id = msg_chan_id;
    hdcdrv_sid_copy(msg.connect_msg.session_id, HDCDRV_SID_LEN, session_id, HDCDRV_SID_LEN);
    msg.connect_msg.run_env = run_env;

    hdcdrv_session_uid(session_fd, msg.connect_msg.euid, msg.connect_msg.uid, msg.connect_msg.root_privilege);

    hdcdrv_session_init(session_fd, HDCDRV_INVALID_VALUE, cmd->dev_id, cmd->service_type, msg_chan_id, session_id,
                        run_env);

    session = &hdc_ctrl->sessions[session_fd];
    session->container_id = container_id;

    ret = hdcdrv_non_trans_ctrl_msg_send((u32)cmd->dev_id, (void *)&msg, sizeof(msg), sizeof(msg), &len);
    if ((ret != HDCDRV_OK) || (len != sizeof(msg)) || (msg.error_code != HDCDRV_OK)) {
        hdcdrv_mod_msg_chan_session_cnt(cmd->dev_id, msg_chan_id, -1);
        hdcdrv_set_session_status(&hdc_ctrl->sessions[session_fd], HDCDRV_SESSION_STATUS_IDLE);
#ifdef HDCDRV_DEBUG
        hdcdrv_err("dev %d service %s conn msg send failed. ret %ld, len %d, error_code %d\n", cmd->dev_id,
                   hdcdrv_sevice_str(cmd->service_type), ret, len, msg.error_code);
#endif

        if ((msg.error_code == HDCDRV_REMOTE_SERVICE_NO_LISTENING) || (msg.error_code == HDCDRV_NO_SESSION)) {
            return (long)msg.error_code;
        }

        return HDCDRV_SEND_CTRL_MSG_FAIL;
    }

    /* conn reply msg maybe come first */
    if (session->remote_session_fd == HDCDRV_INVALID_VALUE) {
#ifdef HDCDRV_DEBUG
        hdcdrv_info("dev %d service %s connect session %d wait\n",
                    cmd->dev_id, hdcdrv_sevice_str(cmd->service_type), session_fd);
#endif
        ret = hdcdrv_connect_wait_replay(dev, session);
        if (ret != HDCDRV_OK) {
            hdcdrv_mod_msg_chan_session_cnt(cmd->dev_id, msg_chan_id, -1);
            hdcdrv_set_session_status(session, HDCDRV_SESSION_STATUS_IDLE);
            hdcdrv_err(
                "device %d service %s connect wait error %d, local session_fd %x, remote session_fd %x, pid %lld\n",
                cmd->dev_id, hdcdrv_sevice_str(cmd->service_type), (int)ret,
                session->local_session_fd, session->remote_session_fd, session->pid);
            return ret;
        }
    }

    mutex_lock(&service->mutex);
    service->connect_session_num++;
    mutex_unlock(&service->mutex);

    /* response cmd */
    cmd->session = session_fd;
    hdcdrv_sid_copy(cmd->session_id, HDCDRV_SID_LEN, session_id, HDCDRV_SID_LEN);

    hdcdrv_delay_work_set(session, &session->close_unknow_session, HDCDRV_DELAY_UNKNOWN_SESSION_BIT,
                          HDCDRV_SESSION_RECLAIM_TIMEOUT);

#ifdef HDCDRV_DEBUG
    hdcdrv_info("dev %d service %s connect session %d, remote session_fd %d\n",
                cmd->dev_id, hdcdrv_sevice_str(cmd->service_type), cmd->session, session->remote_session_fd);
#endif

    return HDCDRV_OK;
}

void hdcdrv_session_free(struct hdcdrv_session *session)
{
    struct hdcdrv_service *service = NULL;

    session->container_id = HDCDRV_DOCKER_MAX_NUM;
    hdcdrv_delay_work_cancel(session, &session->remote_close, HDCDRV_DELAY_REMOTE_CLOSE_BIT);
    hdcdrv_delay_work_cancel(session, &session->close_unknow_session, HDCDRV_DELAY_UNKNOWN_SESSION_BIT);
    hdcdrv_mod_msg_chan_session_cnt(session->dev_id, session->chan_id, -1);
    hdcdrv_unbind_session_ctx(session);
    hdcdrv_sid_clear(session);
    hdcdrv_inner_checker_clear(session);
    hdcdrv_set_session_status(session, HDCDRV_SESSION_STATUS_IDLE);
    hdcdrv_clear_session_rx_buf(session->local_session_fd);

    wake_up_interruptible(&session->wq_rx);
    wake_up_interruptible(&session->wq_conn);
    if (session->msg_chan != NULL) {
        wake_up_interruptible(&session->msg_chan->send_wait);
    }
    if (session->fast_msg_chan != NULL) {
        wake_up_interruptible(&session->fast_msg_chan->send_wait);
    }

    hdcdrv_epoll_wake_up(session->epfd);

    service = session->service;
    if (service != NULL) {
        mutex_lock(&service->mutex);
        service->close_session_num++;
        mutex_unlock(&service->mutex);
    }
}

/* close函数可能被hdc   remote work和上层应用同时调用 */
long hdcdrv_close(struct hdcdrv_cmd_close *cmd, int close_state)
{
    struct hdcdrv_session *session = NULL;
    struct hdcdrv_ctrl_msg msg;
    int last_status = 0;
    u32 len = 0;
    long ret;

    if ((cmd->session >= HDCDRV_SURPORT_MAX_SESSION) || (cmd->session < 0)) {
        hdcdrv_err("session_fd %d is illegal\n", cmd->session);
        return HDCDRV_PARA_ERR;
    }

    ret = hdcdrv_session_state_to_closing(cmd->session, cmd->session_id, cmd->pid, &last_status, close_state);
    if (ret) {
        if (ret == HDCDRV_SESSION_HAS_CLOSED) {
            return HDCDRV_OK;
        } else {
            return ret;
        }
    }

    session = &hdc_ctrl->sessions[cmd->session];

    session->local_close_state = close_state;

    /* In case of connection notification peer */
    if (last_status == HDCDRV_SESSION_STATUS_CONN) {
        msg.type = HDCDRV_CTRL_MSG_TYPE_CLOSE;
        msg.close_msg.local_session = session->local_session_fd;
        msg.close_msg.remote_session = session->remote_session_fd;
        msg.close_msg.session_close_state = close_state;
        hdcdrv_sid_copy(msg.close_msg.session_id, HDCDRV_SID_LEN, session->session_id, HDCDRV_SID_LEN);
        msg.error_code = HDCDRV_OK;  // clear coverity warning

        ret = hdcdrv_non_trans_ctrl_msg_send((u32)session->dev_id, (void *)&msg, sizeof(msg), sizeof(msg), &len);
        if ((ret != HDCDRV_OK) || (len != sizeof(msg)) || (msg.error_code != HDCDRV_OK)) {
            hdcdrv_warn("dev_id %d session %d service type %s, close msg send failed. ret %ld, len %d\n",
                        session->dev_id, cmd->session, hdcdrv_sevice_str(session->service_type), ret, len);
            /* 控制命令失败 也不能影响close动作 */
        }
    }

    hdcdrv_session_free(session);

    return HDCDRV_OK;
}

long hdcdrv_send(struct hdcdrv_cmd_send *cmd, int mode)
{
    struct hdcdrv_session *session = NULL;
    struct hdcdrv_buf_desc buf_d = {0};
    unsigned int timeout;
    long ret;
    int retry_cnt = 0;

    if (cmd->buf == NULL) {
        hdcdrv_err("input para cmd->buf is null.\n");
        return HDCDRV_PARA_ERR;
    }

AGAIN:
    ret = hdcdrv_session_para_check(cmd->session, cmd->session_id);
    if (ret) {
        return ret;
    }

    if (unlikely((cmd->len > hdcdrv_mem_block_capacity()) || (cmd->len <= 0))) {
        hdcdrv_err("session %d send len %d is too bigger, max segment %d\n", cmd->session,
                   cmd->len, hdcdrv_mem_block_capacity());
        return HDCDRV_TX_LEN_ERR;
    }

    session = &hdc_ctrl->sessions[cmd->session];

    if (hdcdrv_get_session_status(session) == HDCDRV_SESSION_STATUS_REMOTE_CLOSED) {
        hdcdrv_warn_limit("dev %d session_fd %d service type %s remote is closed,"
            "local_close_state %s, remote_close_state %s,"
            "local_session_fd %d, remote_session_fd %d\n",
            session->dev_id, cmd->session, hdcdrv_sevice_str(session->service_type),
            hdcdrv_close_str(session->local_close_state), hdcdrv_close_str(session->remote_close_state),
            session->local_session_fd, session->remote_session_fd);

        return HDCDRV_TX_REMOTE_CLOSE;
    }

    ret = (long)alloc_mem(HDCDRV_MEM_POOL_TYPE_TX, session->dev_id, cmd->len, &buf_d.buf, (dma_addr_t *)&buf_d.addr,
                          NULL);
    if (ret != HDCDRV_OK) {
        retry_cnt++;
        if (retry_cnt > HDCDRV_SEND_ALLOC_MEM_RETRY_TIME) {
            hdcdrv_err("dev %d %s: session %d service type %s send alloc dma mem failed\n", session->dev_id,
                       dev_driver_string(session->msg_chan->dev), cmd->session,
                       hdcdrv_sevice_str(session->service_type));
            return HDCDRV_DMA_MEM_ALLOC_FAIL;
        }

        msleep(5);
        goto AGAIN;
    }

#ifdef HDCDRV_DEBUG
    hdcdrv_info("session %d send before copy, data size %x\n", cmd->session, cmd->len);
#endif

    if (mode == HDCDRV_MODE_USER) {
        if (copy_from_user(buf_d.buf, (void __user *)cmd->buf, cmd->len)) {
            free_mem(buf_d.buf);
            hdcdrv_err("dev %d %s: session %d service type %s send copy_from_user failed\n",
                       session->dev_id, dev_driver_string(session->msg_chan->dev), cmd->session,
                       hdcdrv_sevice_str(session->service_type));
            return HDCDRV_COPY_FROM_USER_FAIL;
        }
    } else {
        if (memcpy_s(buf_d.buf, cmd->len, cmd->buf, cmd->len) != EOK) {
            free_mem(buf_d.buf);
            hdcdrv_err("dev %d, memcpy_s failed", session->dev_id);
            return HDCDRV_SAFE_MEM_OP_FAIL;
        }
    }
#ifdef HDCDRV_DEBUG
    hdcdrv_info("session %d send after copy, data size %x, tx index %lld\n", cmd->session, cmd->len, session->stat.tx);
#endif

    buf_d.len = cmd->len;
    buf_d.local_session = session->local_session_fd;
    buf_d.remote_session = session->remote_session_fd;
    buf_d.skip_flag = HDCDRV_INVALID;
    timeout = hdcdrv_set_send_timeout(session, cmd->timeout);
    ret = hdcdrv_msg_chan_send(session->msg_chan, &buf_d, cmd->wait_flag, timeout);
    if (ret != HDCDRV_OK) {
        hdcdrv_warn("dev %d hdcdrv msg chan send ret = %ld\n", session->dev_id, ret);
        free_mem(buf_d.buf);
    }
    return ret;
}

long hdcdrv_recv_peek(struct hdcdrv_cmd_recv_peek *cmd)
{
    struct hdcdrv_session *session = NULL;
    long ret;

    ret = hdcdrv_session_para_check(cmd->session, cmd->session_id);
    if (ret) {
        return ret;
    }

    session = &hdc_ctrl->sessions[cmd->session];

    spin_lock_bh(&session->lock);

    while (((hdcdrv_get_session_status(session) == HDCDRV_SESSION_STATUS_CONN) ||
        (hdcdrv_get_session_status(session) == HDCDRV_SESSION_STATUS_REMOTE_CLOSED)) &&
        (session->normal_rx.head == session->normal_rx.tail)) {
        spin_unlock_bh(&session->lock);

        if (cmd->wait_flag == HDCDRV_NOWAIT) {
            return HDCDRV_NO_BLOCK;
        } else if (cmd->wait_flag == HDCDRV_WAIT_ALWAYS) {
            /*lint -e666*/
            ret = wait_event_interruptible(session->wq_rx,
                (session->normal_rx.head != session->normal_rx.tail) ||
                (hdcdrv_get_session_status(session) == HDCDRV_SESSION_STATUS_IDLE));
            /*lint +e666*/
        } else {
            /*lint -e666*/
            ret = wait_event_interruptible_timeout(session->wq_rx,
                (session->normal_rx.head != session->normal_rx.tail) ||
                (hdcdrv_get_session_status(session) == HDCDRV_SESSION_STATUS_IDLE),
                session->timeout_jiffies.recv_timeout);
            /*lint +e666*/
            if (ret == 0) {
                return HDCDRV_RX_TIMEOUT;
            } else if (ret > 0) {
                ret = 0;
            }
        }
        if (ret) {
            hdcdrv_warn("dev %d session %d service type %s recv peek wait ret %d, head %d, tail %d,"
                "local_close_state %s, remote_close_state %s, local_session_fd %d, remote_session_fd %d.\n",
                session->dev_id, cmd->session, hdcdrv_sevice_str(session->service_type), (int)ret,
                session->normal_rx.head, session->normal_rx.tail, hdcdrv_close_str(session->local_close_state),
                hdcdrv_close_str(session->remote_close_state), session->local_session_fd, session->remote_session_fd);
            return ret;
        }

        spin_lock_bh(&session->lock);
    }

    /* session is closed */
    if (session->normal_rx.head == session->normal_rx.tail) {
        cmd->len = 0;
        spin_unlock_bh(&session->lock);
        hdcdrv_info("dev %d session %d local or remote close, local_close_state %s, remote_close_state %s,"
            "local_session_fd %d, remote_session_fd %d.\n", session->dev_id, cmd->session,
            hdcdrv_close_str(session->local_close_state), hdcdrv_close_str(session->remote_close_state),
            session->local_session_fd, session->remote_session_fd);
        return HDCDRV_OK;
    }

    cmd->len = session->normal_rx.rx_list[session->normal_rx.head].len;

    spin_unlock_bh(&session->lock);

    return HDCDRV_OK;
}

long hdcdrv_recv(struct hdcdrv_cmd_recv *cmd, int mode)
{
    struct hdcdrv_session *session = NULL;
    struct hdcdrv_dev *hdc_dev = NULL;
    struct hdcdrv_buf_desc *buf_desc = NULL;
    struct hdcdrv_msg_chan *msg_chan = NULL;
    long ret;
    int len;
    void *buf = NULL;
    u64 addr;

    if (cmd->buf == NULL) {
        hdcdrv_err("input para cmd->buf is null.\n");
        return HDCDRV_PARA_ERR;
    }

    ret = hdcdrv_session_para_check(cmd->session, cmd->session_id);
    if (ret) {
        return ret;
    }

    session = &hdc_ctrl->sessions[cmd->session];
    hdc_dev = &hdc_ctrl->devices[session->dev_id];
    msg_chan = hdc_dev->msg_chan[session->chan_id];

    mutex_lock(&msg_chan->mutex);
    msg_chan->dbg_stat.hdcdrv_recv_data_times++;
    mutex_unlock(&msg_chan->mutex);

    spin_lock_bh(&session->lock);
    session->stat.rx_total++;
    /* session is closed */
    if (session->normal_rx.head == session->normal_rx.tail) {
        cmd->out_len = 0;
        spin_unlock_bh(&session->lock);
        return HDCDRV_OK;
    }

    buf_desc = &session->normal_rx.rx_list[session->normal_rx.head];

    /* we should get buf addr from the head postion of rx_list firstly.
       when move head, unlock, in the old head postion of rx_list will be use in other thread */
    len = buf_desc->len;
    buf = buf_desc->buf;
    addr = buf_desc->addr;

    if (cmd->len < len) {
        spin_unlock_bh(&session->lock);

        hdcdrv_err("dev %d %s: session %d service type %s, recv buf %d in not enought %d.\n", session->dev_id,
            dev_driver_string(hdc_dev->dev), cmd->session, hdcdrv_sevice_str(session->service_type), cmd->len, len);
        return HDCDRV_RX_BUF_SMALL;
    }

    session->normal_rx.head = (session->normal_rx.head + 1) % HDCDRV_SESSION_RX_LIST_MAX_PKT;
    if (msg_chan->rx_trigger_flag == HDCDRV_VALID) {
        msg_chan->rx_trigger_flag = HDCDRV_INVALID;
        tasklet_schedule(&msg_chan->rx_task);
    }

    buf_desc->buf = NULL;
    buf_desc->addr = 0;

    spin_unlock_bh(&session->lock);

    if (mode == HDCDRV_MODE_USER) {
        if (copy_to_user((void __user *)cmd->buf, buf, len)) {
            hdcdrv_err("dev %d %s: session %d recv copy_to_user failed\n",  session->dev_id,
                dev_driver_string(hdc_dev->dev), cmd->session);
            ret = HDCDRV_COPY_FROM_USER_FAIL;
            goto out;
        }
    } else {
        if (memcpy_s(cmd->buf, cmd->len, buf, len) != EOK) {
            hdcdrv_err("memcpy_s failed\n");
            ret = HDCDRV_ERR;
            goto out;
        }
    }

    ret = HDCDRV_OK;
    cmd->out_len = len;

out:
    free_mem(buf);
    buf = NULL;

    return ret;
}

void hdcdrv_sync_fast_mem(struct device *dev, int type, int len, u64 addr, u64 pid)
{
    struct hdcdrv_fast_mem *f_mem = NULL;
    int i;
    u64 hash_va;

    if (addr != 0) {
        hash_va = hdcdrv_get_hash(addr, pid);
        f_mem = hdcdrv_get_fast_mem(&hdc_ctrl->rbtree, type, len, hash_va, HDCDRV_NODE_WAIT_TIME_MAX);
    }

    if (f_mem == NULL) {
        return;
    }

    /* dma_map_page or dma_map_single address, cache consistency is not
       guaranteed(arm), need to cooperate with dma_sync_single */
    if ((type == HDCDRV_FAST_MEM_TYPE_TX_DATA) || (type == HDCDRV_FAST_MEM_TYPE_TX_CTRL)) {
        for (i = 0; i < f_mem->phy_addr_num; i++) {
            dma_sync_single_for_device(dev, f_mem->mem[i].addr, f_mem->mem[i].len, DMA_TO_DEVICE);
        }
    } else {
        for (i = 0; i < f_mem->phy_addr_num; i++) {
            dma_sync_single_for_cpu(dev, f_mem->mem[i].addr, f_mem->mem[i].len, DMA_FROM_DEVICE);
        }
    }

    hdcdrv_node_status_idle_by_mem(f_mem);
}

long hdcdrv_fast_send(struct hdcdrv_cmd_fast_send *cmd)
{
    struct hdcdrv_session *session = NULL;
    struct hdcdrv_buf_desc buf_d = {0};
    unsigned int timeout;
    u64 pid;
    long ret;

    ret = hdcdrv_session_para_check(cmd->session, cmd->session_id);
    if (ret) {
        return ret;
    }

    session = &hdc_ctrl->sessions[cmd->session];

    if (hdcdrv_get_session_status(session) == HDCDRV_SESSION_STATUS_REMOTE_CLOSED) {
        hdcdrv_warn("dev %d session_fd %d service type %s remote is closed, local_close_state %s,"
            "remote_close_state %s, local_session_fd %d, remote_session_fd %d.\n",
            session->dev_id, cmd->session, hdcdrv_sevice_str(session->service_type),
            hdcdrv_close_str(session->local_close_state), hdcdrv_close_str(session->remote_close_state),
            session->local_session_fd, session->remote_session_fd);
        return HDCDRV_TX_REMOTE_CLOSE;
    }

    if (((cmd->src_data_addr == 0) && (cmd->src_ctrl_addr == 0)) || ((cmd->data_len == 0) && (cmd->ctrl_len == 0))) {
        hdcdrv_err("dev %d session_fd %d service type %s no data send.\n", session->dev_id,
                   cmd->session, hdcdrv_sevice_str(session->service_type));
        return HDCDRV_PARA_ERR;
    }

    if ((cmd->data_len < 0) || (cmd->ctrl_len < 0)) {
        hdcdrv_err("dev %d session_fd %d service type %s length invalid.\n", session->dev_id,
                   cmd->session, hdcdrv_sevice_str(session->service_type));
        return HDCDRV_PARA_ERR;
    }

    pid = (u64)hdcdrv_get_pid();

    buf_d.local_session = session->local_session_fd;
    buf_d.remote_session = session->remote_session_fd;

    /* data pair */
    buf_d.addr = hdcdrv_get_hash(cmd->src_data_addr, pid);
    buf_d.len = cmd->data_len;

    buf_d.dst_data_addr = cmd->dst_data_addr;

    /* strl msg pair */
    buf_d.src_ctrl_addr = hdcdrv_get_hash(cmd->src_ctrl_addr, pid);
    buf_d.ctrl_len = cmd->ctrl_len;

    buf_d.dst_ctrl_addr = cmd->dst_ctrl_addr;
    buf_d.skip_flag = HDCDRV_INVALID;
    buf_d.buf = NULL;
    buf_d.status = 0;
    timeout = hdcdrv_set_fast_send_timeout(session, cmd->timeout);

    hdcdrv_sync_fast_mem(session->fast_msg_chan->dev, HDCDRV_FAST_MEM_TYPE_TX_DATA, cmd->data_len, cmd->src_data_addr,
                         pid);

    hdcdrv_sync_fast_mem(session->fast_msg_chan->dev, HDCDRV_FAST_MEM_TYPE_TX_CTRL, cmd->ctrl_len, cmd->src_ctrl_addr,
                         pid);

    return hdcdrv_msg_chan_send(session->fast_msg_chan, &buf_d, cmd->wait_flag, timeout);
}

long hdcdrv_fast_recv(struct hdcdrv_cmd_fast_recv *cmd)
{
    struct hdcdrv_session *session = NULL;
    struct hdcdrv_msg_chan *msg_chan = NULL;
    struct hdcdrv_session_fast_rx *fast_rx = NULL;
    struct hdcdrv_fast_rx *rx_buf = NULL;
    long ret;

    ret = hdcdrv_session_para_check(cmd->session, cmd->session_id);
    if (ret) {
        return ret;
    }

    session = &hdc_ctrl->sessions[cmd->session];
    msg_chan = hdc_ctrl->devices[session->dev_id].msg_chan[session->fast_chan_id];

    mutex_lock(&msg_chan->mutex);
    msg_chan->dbg_stat.hdcdrv_recv_data_times++;
    mutex_unlock(&msg_chan->mutex);

    fast_rx = hdcdrv_get_session_fast_rx(cmd->session);
    if (fast_rx == NULL) {
        hdcdrv_err("dev %d session %d service type %s has no fast chan\n", session->dev_id,
                   cmd->session, hdcdrv_sevice_str(session->service_type));
        return HDCDRV_NOT_SURPORT;
    }

    spin_lock_bh(&session->lock);
    session->stat.rx_total++;
    while (
        ((hdcdrv_get_session_status(session) == HDCDRV_SESSION_STATUS_CONN) ||
        (hdcdrv_get_session_status(session) == HDCDRV_SESSION_STATUS_REMOTE_CLOSED)) &&
        (fast_rx->head == fast_rx->tail)) {
        spin_unlock_bh(&session->lock);
        if (cmd->wait_flag == HDCDRV_NOWAIT) {
            return HDCDRV_NO_BLOCK;
        } else if (cmd->wait_flag == HDCDRV_WAIT_ALWAYS) {
            /*lint -e666*/
            ret = wait_event_interruptible(session->wq_rx,
                (fast_rx->head != fast_rx->tail) ||
                (hdcdrv_get_session_status(session) == HDCDRV_SESSION_STATUS_IDLE));
            /*lint +e666*/
        } else {
            /*lint -e666*/
            ret = wait_event_interruptible_timeout(session->wq_rx,
                (fast_rx->head != fast_rx->tail) ||
                (hdcdrv_get_session_status(session) == HDCDRV_SESSION_STATUS_IDLE),
                session->timeout_jiffies.fast_recv_timeout);
            /*lint +e666*/
            if (ret == 0) {
                return HDCDRV_RX_TIMEOUT;
            } else if (ret > 0) {
                ret = 0;
            }
        }

        if (ret) {
            hdcdrv_warn("dev %d session %d service type %s fast recv wait head %d tail %d ret %d"
                "local_close_state %s, remote_close_state %s, local_session_fd %d,"
                "remote_session_fd %d.\n", session->dev_id, cmd->session,
                hdcdrv_sevice_str(session->service_type), fast_rx->head, fast_rx->tail, (int)ret,
                hdcdrv_close_str(session->local_close_state), hdcdrv_close_str(session->remote_close_state),
                session->local_session_fd, session->remote_session_fd);
            return ret;
        }

        spin_lock_bh(&session->lock);
    }

    /* session is closed */
    if (fast_rx->head == fast_rx->tail) {
        spin_unlock_bh(&session->lock);
        cmd->ctrl_addr = 0;
        cmd->data_addr = 0;
        cmd->data_len = 0;
        cmd->ctrl_len = 0;

        hdcdrv_info("dev %d session %d local or remote close, local_close_state %s, remote_close_state %s,"
            "local_session_fd %d, remote_session_fd %d.\n", session->dev_id, cmd->session,
            hdcdrv_close_str(session->local_close_state), hdcdrv_close_str(session->remote_close_state),
            session->local_session_fd, session->remote_session_fd);

        return HDCDRV_OK;
    }

    rx_buf = &fast_rx->rx_list[fast_rx->head];

    /* Insert into the tail of the receive list */
    cmd->data_addr = (u64)rx_buf->data_addr;
    cmd->ctrl_addr = (u64)rx_buf->ctrl_addr;
    cmd->data_len = rx_buf->data_len;
    cmd->ctrl_len = rx_buf->ctrl_len;
    fast_rx->head = (fast_rx->head + 1) % HDCDRV_BUF_MAX_CNT;
    if (msg_chan->rx_trigger_flag == HDCDRV_VALID) {
        msg_chan->rx_trigger_flag = HDCDRV_INVALID;
        tasklet_schedule(&msg_chan->rx_task);
    }
    spin_unlock_bh(&session->lock);

    hdcdrv_sync_fast_mem(session->fast_msg_chan->dev, HDCDRV_FAST_MEM_TYPE_RX_DATA, cmd->data_len, cmd->data_addr,
                         session->pid);

    hdcdrv_sync_fast_mem(session->fast_msg_chan->dev, HDCDRV_FAST_MEM_TYPE_RX_CTRL, cmd->ctrl_len, cmd->ctrl_addr,
                         session->pid);

    return HDCDRV_OK;
}

long hdcdrv_set_session_owner(struct hdcdrv_ctx *ctx, struct hdcdrv_cmd_set_session_owner *cmd)
{
    struct hdcdrv_session *session = NULL;
    long ret;

    if (ctx == NULL) {
        hdcdrv_err("session id %d, hdc ctx is null.\n", cmd->session);
        return HDCDRV_PARA_ERR;
    }

    ret = hdcdrv_session_para_check(cmd->session, cmd->session_id);
    if (ret) {
        return ret;
    }

    session = &hdc_ctrl->sessions[cmd->session];
    hdcdrv_delay_work_cancel(session, &session->close_unknow_session, HDCDRV_DELAY_UNKNOWN_SESSION_BIT);
    session->pid = cmd->owner_pid;
    hdcdrv_bind_sessoin_ctx(ctx, session);

#ifdef HDCDRV_DEBUG
    hdcdrv_info("devid %d session %d remote session %d, service type %s set owner pid %d.\n", session->dev_id,
                cmd->session, session->remote_session_fd, hdcdrv_sevice_str(session->service_type), cmd->owner_pid);
#endif

    return HDCDRV_OK;
}

long hdcdrv_session_run_env(struct hdcdrv_cmd_get_session_run_env *cmd)
{
    struct hdcdrv_session *session = NULL;
    long ret;

    ret = hdcdrv_session_para_check(cmd->session, cmd->session_id);
    if (ret) {
        return ret;
    }

    session = &hdc_ctrl->sessions[cmd->session];

    cmd->run_env = session->run_env;

    return HDCDRV_OK;
}

long hdcdrv_set_session_timeout(struct hdcdrv_cmd_set_session_timeout *cmd)
{
    struct hdcdrv_session *session = NULL;
    long ret;
    u64 send_timeout;
    u64 recv_timeout;
    u64 fast_send_timeout;
    u64 fast_recv_timeout;

    ret = hdcdrv_session_para_check(cmd->session, cmd->session_id);
    if (ret) {
        return ret;
    }

    session = &hdc_ctrl->sessions[cmd->session];

    send_timeout = msecs_to_jiffies(cmd->timeout.send_timeout);
    recv_timeout = msecs_to_jiffies(cmd->timeout.recv_timeout);
    fast_send_timeout = msecs_to_jiffies(cmd->timeout.fast_send_timeout);
    fast_recv_timeout = msecs_to_jiffies(cmd->timeout.fast_recv_timeout);

    if (cmd->timeout.send_timeout != 0)
        session->timeout_jiffies.send_timeout = send_timeout;
    if (cmd->timeout.recv_timeout != 0)
        session->timeout_jiffies.recv_timeout = recv_timeout;
    if (cmd->timeout.fast_send_timeout != 0)
        session->timeout_jiffies.fast_send_timeout = fast_send_timeout;
    if (cmd->timeout.fast_recv_timeout != 0)
        session->timeout_jiffies.fast_recv_timeout = fast_recv_timeout;

    return HDCDRV_OK;
}

void hdcdrv_add_stat(struct hdcdrv_stats *stat_all, struct hdcdrv_stats *stat)
{
    stat_all->tx += stat->tx;
    stat_all->tx_bytes += stat->tx_bytes;
    stat_all->rx += stat->rx;
    stat_all->rx_bytes += stat->rx_bytes;
    stat_all->tx_finish += stat->tx_finish;
    stat_all->tx_full += stat->tx_full;
    stat_all->tx_fail += stat->tx_fail;
    stat_all->rx_fail += stat->rx_fail;
    stat_all->rx_full += stat->rx_full;
    stat_all->rx_total += stat->rx_total;
}

long hdcdrv_get_all_stat(struct hdcdrv_cmd_get_stat *cmd)
{
    struct hdcdrv_service *serv = NULL;
    struct hdcdrv_cmd_stat_all *stat = NULL;
    int i, j;
    int status;

    if (cmd->outbuf == NULL) {
        hdcdrv_err("cmd->outbuf is null\n");
        return HDCDRV_PARA_ERR;
    }

    stat = (struct hdcdrv_cmd_stat_all *)kzalloc(sizeof(struct hdcdrv_cmd_stat_all), GFP_KERNEL | __GFP_ACCOUNT);
    if (stat == NULL) {
        return HDCDRV_MEM_ALLOC_FAIL;
    }

    for (i = 0; i < HDCDRV_SURPORT_MAX_DEV; i++) {
        if (hdc_ctrl->devices[i].valid != HDCDRV_VALID) {
            continue;
        }

        stat->dev_list[stat->dev_num++] = i;

        for (j = 0; j < HDCDRV_SURPORT_MAX_SERVICE; j++) {
            serv = &hdc_ctrl->devices[i].service[j];
            stat->s_brief.accept_num += serv->accept_session_num;
            stat->s_brief.connect_num += serv->connect_session_num;
            stat->s_brief.close_num += serv->close_session_num;
        }
    }

    for (i = 0; i < HDCDRV_SURPORT_MAX_SESSION; i++) {
        status = hdcdrv_get_session_status(&hdc_ctrl->sessions[i]);
        if (status == HDCDRV_SESSION_STATUS_CONN) {
            stat->s_brief.active_list[stat->s_brief.active_num++] = i;
        } else if (status == HDCDRV_SESSION_STATUS_REMOTE_CLOSED) {
            stat->s_brief.remote_close_list[stat->s_brief.remote_close_num++] = i;
        }
    }

    if (copy_to_user((void __user *)cmd->outbuf, stat, sizeof(struct hdcdrv_cmd_stat_all))) {
        kfree(stat);
        stat = NULL;
        hdcdrv_err("copy_to_user failed\n");
        return HDCDRV_COPY_TO_USER_FAIL;
    }

    kfree(stat);
    stat = NULL;
    return HDCDRV_OK;
}

int hdcdrv_get_dev_check(struct hdcdrv_cmd_get_stat *cmd)
{
    if ((cmd->dev_id >= HDCDRV_SURPORT_MAX_DEV) || (cmd->dev_id < 0)) {
        hdcdrv_err("dev id %d or ptr is illegal\n", cmd->dev_id);
        return HDCDRV_PARA_ERR;
    }

    if (cmd->outbuf == NULL) {
        hdcdrv_err("dev id %d outbuf is NULL\n", cmd->dev_id);
        return HDCDRV_PARA_ERR;
    }

    return HDCDRV_OK;
}

void hdcdrv_get_mem_pool_stat(struct hdcdrv_dev *hdc_dev, struct hdcdrv_cmd_stat_dev_service *stat)
{
    struct hdcdrv_mem_info *mem_info = NULL;
    struct hdcdrv_mem_pool *mem_pool = NULL;
    int i;

    for (i = 0; i < HDCDRV_MEM_POOL_TYPE_NUM; i++) {
        if (i == HDCDRV_MEM_POOL_TYPE_TX) {
            mem_info = &stat->tx_mem_info;
        } else {
            mem_info = &stat->rx_mem_info;
        }

        mem_pool = &hdc_dev->huge_mem_pool[i];
        mem_info->huge_pool_size = mem_pool->size;
        mem_info->huge_pool_remain_size = (unsigned int)(mem_pool->tail - mem_pool->head);

        mem_pool = &hdc_dev->small_mem_pool[i];
        mem_info->small_pool_size = mem_pool->size;
        mem_info->small_pool_remain_size = (unsigned int)(mem_pool->tail - mem_pool->head);
    }
}

long hdcdrv_get_dev_stat(struct hdcdrv_cmd_get_stat *cmd)
{
    struct hdcdrv_cmd_stat_dev_service *stat = NULL;
    struct hdcdrv_dev *hdc_dev = NULL;
    struct hdcdrv_service *serv = NULL;
    int i, j;
    int status;

    if (hdcdrv_get_dev_check(cmd)) {
        hdcdrv_err("dev id %d is illegal\n", cmd->dev_id);
        return HDCDRV_PARA_ERR;
    }

    hdc_dev = &hdc_ctrl->devices[cmd->dev_id];

    if (hdc_dev->valid != HDCDRV_VALID) {
        hdcdrv_err("dev id %d is invalid\n", cmd->dev_id);
        return HDCDRV_PARA_ERR;
    }

    stat = (struct hdcdrv_cmd_stat_dev_service *)kzalloc(sizeof(struct hdcdrv_cmd_stat_dev_service),
        GFP_KERNEL | __GFP_ACCOUNT);
    if (stat == NULL) {
        return HDCDRV_MEM_ALLOC_FAIL;
    }

    for (j = 0; j < HDCDRV_SURPORT_MAX_SERVICE; j++) {
        if ((cmd->service_type != -1) && (cmd->service_type != j)) {
            continue;
        }

        serv = &hdc_dev->service[j];
        stat->s_brief.accept_num += serv->accept_session_num;
        stat->s_brief.connect_num += serv->connect_session_num;
        stat->s_brief.close_num += serv->close_session_num;
        hdcdrv_add_stat(&stat->stat, &serv->stat);
    }

    for (i = 0; i < HDCDRV_SURPORT_MAX_SESSION; i++) {
        if (hdc_ctrl->sessions[i].dev_id != cmd->dev_id) {
            continue;
        }
        if ((cmd->service_type != -1) && (cmd->service_type != hdc_ctrl->sessions[i].service_type)) {
            continue;
        }

        status = hdcdrv_get_session_status(&hdc_ctrl->sessions[i]);
        if (status == HDCDRV_SESSION_STATUS_CONN) {
            stat->s_brief.active_list[stat->s_brief.active_num++] = i;
        } else if (status == HDCDRV_SESSION_STATUS_REMOTE_CLOSED) {
            stat->s_brief.remote_close_list[stat->s_brief.remote_close_num++] = i;
        }
    }

    hdcdrv_get_mem_pool_stat(hdc_dev, stat);

    if (copy_to_user((void __user *)cmd->outbuf, stat, sizeof(struct hdcdrv_cmd_stat_dev_service))) {
        kfree(stat);
        stat = NULL;
        hdcdrv_err("copy_to_user failed\n");
        return HDCDRV_COPY_TO_USER_FAIL;
    }

    kfree(stat);
    stat = NULL;
    return HDCDRV_OK;
}

long hdcdrv_get_service_stat(struct hdcdrv_cmd_get_stat *cmd)
{
    struct hdcdrv_cmd_stat_dev_service *stat = NULL;
    struct hdcdrv_service *serv = NULL;
    int i, j;
    int status;

    if ((cmd->service_type >= HDCDRV_SURPORT_MAX_SERVICE) || (cmd->service_type < 0) || (cmd->outbuf == NULL)) {
        hdcdrv_err("service_type %d is illegal\n", cmd->service_type);
        return HDCDRV_PARA_ERR;
    }

    stat = (struct hdcdrv_cmd_stat_dev_service *)kzalloc(sizeof(struct hdcdrv_cmd_stat_dev_service),
        GFP_KERNEL | __GFP_ACCOUNT);
    if (stat == NULL) {
        return HDCDRV_MEM_ALLOC_FAIL;
    }

    for (j = 0; j < HDCDRV_SURPORT_MAX_DEV; j++) {
        if (hdc_ctrl->devices[j].valid != HDCDRV_VALID) {
            continue;
        }

        serv = &hdc_ctrl->devices[j].service[cmd->service_type];
        stat->s_brief.accept_num += serv->accept_session_num;
        stat->s_brief.connect_num += serv->connect_session_num;
        stat->s_brief.close_num += serv->close_session_num;
        hdcdrv_add_stat(&stat->stat, &serv->stat);
    }

    for (i = 0; i < HDCDRV_SURPORT_MAX_SESSION; i++) {
        if (cmd->service_type != hdc_ctrl->sessions[i].service_type) {
            continue;
        }

        status = hdcdrv_get_session_status(&hdc_ctrl->sessions[i]);
        if (status == HDCDRV_SESSION_STATUS_CONN) {
            stat->s_brief.active_list[stat->s_brief.active_num++] = i;
        } else if (status == HDCDRV_SESSION_STATUS_REMOTE_CLOSED) {
            stat->s_brief.remote_close_list[stat->s_brief.remote_close_num++] = i;
        }
    }

    if (copy_to_user((void __user *)cmd->outbuf, stat, sizeof(struct hdcdrv_cmd_stat_dev_service))) {
        kfree(stat);
        stat = NULL;
        hdcdrv_err("copy_to_user failed\n");
        return HDCDRV_COPY_TO_USER_FAIL;
    }

    kfree(stat);
    stat = NULL;
    return HDCDRV_OK;
}

long hdcdrv_get_chan_stat(struct hdcdrv_cmd_get_stat *cmd)
{
    struct hdcdrv_cmd_stat_chan stat;
    struct hdcdrv_dev *hdc_dev = NULL;
    struct hdcdrv_msg_chan *msg_chan = NULL;
    int chan_id = cmd->chan_id;
    int dev_id = cmd->dev_id;

    if (dev_id == HDCDRV_INVALID_VALUE) {
        dev_id = 0;
    }
    if ((dev_id >= HDCDRV_SURPORT_MAX_DEV) || (dev_id < 0) || (cmd->outbuf == NULL)) {
        hdcdrv_err("dev id %d is illegal\n", dev_id);
        return HDCDRV_PARA_ERR;
    }
    hdc_dev = &hdc_ctrl->devices[dev_id];

    if ((chan_id >= hdc_dev->msg_chan_cnt) || (chan_id < 0)) {
        hdcdrv_err("dev %u chan id %d is illegal, msg_chan_num %d\n", hdc_dev->dev_id, chan_id, hdc_dev->msg_chan_cnt);
        return HDCDRV_PARA_ERR;
    }

    msg_chan = hdc_dev->msg_chan[chan_id];

    if (memset_s((void *)&stat, sizeof(stat), 0, sizeof(stat)) != EOK) {
        hdcdrv_err("memset_s failed");
        return HDCDRV_SAFE_MEM_OP_FAIL;
    }

    stat.stat = msg_chan->stat;
    stat.dbg_stat = msg_chan->dbg_stat;
    stat.dma_head = msg_chan->dma_head;
    stat.rx_head = msg_chan->rx_head;
    stat.w_sq_head = msg_chan->sq_head;
    stat.submit_dma_head = msg_chan->submit_dma_head;
    (void)hdcdrv_get_w_sq_desc(msg_chan->chan, (u32 *)(&stat.w_sq_tail));
    (void)hdcdrv_get_r_sq_desc(msg_chan->chan, (u32 *)(&stat.r_sq_head));

    if (copy_to_user((void __user *)cmd->outbuf, &stat, sizeof(stat))) {
        hdcdrv_err("dev %u copy_to_user failed\n", hdc_dev->dev_id);
        return HDCDRV_COPY_TO_USER_FAIL;
    }
    return HDCDRV_OK;
}
long hdcdrv_get_session_stat(struct hdcdrv_cmd_get_stat *cmd)
{
    struct hdcdrv_cmd_stat_session stat;
    struct hdcdrv_session *session = NULL;
    struct hdcdrv_session_fast_rx *fast_rx = NULL;
    int session_fd = cmd->session;

    if ((session_fd >= HDCDRV_SURPORT_MAX_SESSION) || (session_fd < 0) || (cmd->outbuf == NULL)) {
        hdcdrv_err("session_fd %d is illegal\n", session_fd);
        return HDCDRV_PARA_ERR;
    }

    session = &hdc_ctrl->sessions[session_fd];
    fast_rx = hdcdrv_get_session_fast_rx(session_fd);

    if (memset_s((void *)&stat, sizeof(stat), 0, sizeof(stat)) != EOK) {
        hdcdrv_err("memset_s failed");
        return HDCDRV_SAFE_MEM_OP_FAIL;
    }

    stat.status = hdcdrv_get_session_status(session);
    stat.local_session = session->local_session_fd;
    stat.remote_session = session->remote_session_fd;
    stat.dev_id = session->dev_id;
    stat.service_type = session->service_type;
    stat.chan_id = session->chan_id;
    stat.fast_chan_id = session->fast_chan_id;
    if (fast_rx != NULL) {
        stat.pkts_in_fast_list = (fast_rx->tail + HDCDRV_BUF_MAX_CNT - fast_rx->head) % HDCDRV_BUF_MAX_CNT;
    }

    stat.pkts_in_list = (session->normal_rx.tail + HDCDRV_SESSION_RX_LIST_MAX_PKT - session->normal_rx.head) %
                        HDCDRV_SESSION_RX_LIST_MAX_PKT;
    stat.stat = session->stat;
    stat.remote_close_state = session->remote_close_state;
    stat.local_close_state = session->local_close_state;
    stat.timeout.send_timeout = jiffies_to_msecs(session->timeout_jiffies.send_timeout);
    stat.timeout.recv_timeout = jiffies_to_msecs(session->timeout_jiffies.recv_timeout);
    stat.timeout.fast_send_timeout = jiffies_to_msecs(session->timeout_jiffies.fast_send_timeout);
    stat.timeout.fast_recv_timeout = jiffies_to_msecs(session->timeout_jiffies.fast_recv_timeout);

    if (copy_to_user((void __user *)cmd->outbuf, &stat, sizeof(stat))) {
        hdcdrv_err("dev %d copy_to_user failed\n", session->dev_id);
        return HDCDRV_COPY_TO_USER_FAIL;
    }
    return HDCDRV_OK;
}

bool hdcdrv_check_is_root(void)
{
    int root = HDCDRV_ERR;

    if (current->cred != NULL) {
        root = (int)current->cred->euid.val;
    }

    if (root != 0) {
        return false;
    }

    return true;
}

long hdcdrv_get_stat(struct hdcdrv_cmd_get_stat *cmd)
{
    if (cmd->outbuf == NULL) {
        hdcdrv_err("input para outbuf is null.\n");
        return HDCDRV_PARA_ERR;
    }

    if (!hdcdrv_check_is_root()) {
        hdcdrv_err("check user authority fail, not root.\n");
        return HDCDRV_NOT_SURPORT;
    }

#ifndef HDC_ENV_DEVICE
    if (hdcdrv_check_in_container() == 1) {
        hdcdrv_err("check user authority fail, in container.\n");
        return HDCDRV_NOT_SURPORT;
    }
#endif
    if (!capable(CAP_SYS_ADMIN)) {
        hdcdrv_err("check docker authority fail.\n");
        return HDCDRV_NOT_SURPORT;
    }

    hdcdrv_info("stat get: devid %d, service %s, session %d, chan id %d\n",
                cmd->dev_id, hdcdrv_sevice_str(cmd->service_type), cmd->session, cmd->chan_id);

    if (cmd->session != HDCDRV_INVALID_VALUE) {
        return hdcdrv_get_session_stat(cmd);
    }
    if (cmd->chan_id != HDCDRV_INVALID_VALUE) {
        return hdcdrv_get_chan_stat(cmd);
    } else if (cmd->dev_id != HDCDRV_INVALID_VALUE) {
        return hdcdrv_get_dev_stat(cmd);
    } else if (cmd->service_type != HDCDRV_INVALID_VALUE) {
        return hdcdrv_get_service_stat(cmd);
    } else {
        return hdcdrv_get_all_stat(cmd);
    }
}

long hdcdrv_get_uid_state(struct hdcdrv_cmd_get_uid_stat *cmd)
{
    struct hdcdrv_session *session = NULL;
    int ret;

    ret = hdcdrv_session_para_check(cmd->session, cmd->session_id);
    if (ret) {
        hdcdrv_info("para_check err\n");
        return ret;
    }
    session = &hdc_ctrl->sessions[cmd->session];

    cmd->euid = session->euid;
    cmd->uid = session->uid;
    cmd->root_privilege = session->root_privilege;

    return HDCDRV_OK;
}

long hdcdrv_get_page_size(struct hdcdrv_cmd_get_page_size *cmd)
{
    cmd->page_size = PAGE_SIZE;
    cmd->hpage_size = HPAGE_SIZE;
    cmd->page_bit = PAGE_SHIFT;
    return HDCDRV_OK;
}

long hdcdrv_convert_id_from_vir_to_phy(u32 drv_cmd, union hdcdrv_cmd *cmd_data)
{
    long ret = HDCDRV_OK;

#ifndef HDC_ENV_DEVICE
    int *dev_id = NULL;
    switch (drv_cmd) {
        case HDCDRV_CMD_GET_PEER_DEV_ID:
            dev_id = &cmd_data->get_peer_dev_id.dev_id;
            break;
        case HDCDRV_CMD_SERVER_CREATE:
            dev_id = &cmd_data->server_create.dev_id;
            break;
        case HDCDRV_CMD_SERVER_DESTROY:
            dev_id = &cmd_data->server_destroy.dev_id;
            break;
        case HDCDRV_CMD_ACCEPT:
            dev_id = &cmd_data->accept.dev_id;
            break;
        case HDCDRV_CMD_CONNECT:
            dev_id = &cmd_data->conncet.dev_id;
            break;
        case HDCDRV_CMD_ALLOC_MEM:
            dev_id = &cmd_data->alloc_mem.devid;
            break;
        case HDCDRV_CMD_DMA_MAP:
            dev_id = &cmd_data->dma_map.devid;
            break;
        case HDCDRV_CMD_DMA_REMAP:
            dev_id = &cmd_data->dma_remap.devid;
            break;
        default:
            return ret;
    }
    ret = hdcdrv_container_vir_to_phs_devid((u32)(*dev_id), (u32 *)dev_id);
#endif

    return ret;
}

long hdcdrv_operation(struct hdcdrv_ctx *ctx, u32 drv_cmd, union hdcdrv_cmd *cmd_data, bool *copy_to_user_flag)
{
    long ret = HDCDRV_OK;

    switch (drv_cmd) {
        case HDCDRV_CMD_GET_PEER_DEV_ID:
            ret = hdcdrv_cmd_get_peer_dev_id(&cmd_data->get_peer_dev_id);
            *copy_to_user_flag = true;
            break;
        case HDCDRV_CMD_CONFIG:
            cmd_data->config.pid = hdcdrv_get_pid();
            ret = hdcdrv_config(&cmd_data->config);
            *copy_to_user_flag = true;
            break;
        case HDCDRV_CMD_SET_SERVICE_LEVEL:
            ret = hdcdrv_set_service_level(&cmd_data->set_level);
            break;
        case HDCDRV_CMD_SERVER_CREATE:
            cmd_data->server_create.pid = hdcdrv_get_pid();
            ret = hdcdrv_server_create(ctx, &cmd_data->server_create);
            break;
        case HDCDRV_CMD_SERVER_DESTROY:
            cmd_data->server_destroy.pid = hdcdrv_get_pid();
            ret = hdcdrv_server_destroy(&cmd_data->server_destroy);
            break;
        case HDCDRV_CMD_ACCEPT:
            ret = hdcdrv_accept(&cmd_data->accept);
            *copy_to_user_flag = true;
            break;
        case HDCDRV_CMD_CONNECT:
            ret = hdcdrv_connect(&cmd_data->conncet);
            *copy_to_user_flag = true;
            break;
        case HDCDRV_CMD_CLOSE:
            cmd_data->close.pid = hdcdrv_get_pid();
            ret = hdcdrv_close(&cmd_data->close, HDCDRV_CLOSE_TYPE_USER);
            break;
        case HDCDRV_CMD_SEND:
            ret = hdcdrv_send(&cmd_data->send, HDCDRV_MODE_USER);
            break;
        case HDCDRV_CMD_RECV_PEEK:
            ret = hdcdrv_recv_peek(&cmd_data->recv_peek);
            *copy_to_user_flag = true;
            break;
        case HDCDRV_CMD_RECV:
            ret = hdcdrv_recv(&cmd_data->recv, HDCDRV_MODE_USER);
            *copy_to_user_flag = true;
            break;
        default:
            hdcdrv_err("cmd %u is illegal\n", drv_cmd);
            return HDCDRV_PARA_ERR;
    }

    return ret;
}

long hdcdrv_cfg_operation(struct hdcdrv_ctx *ctx, u32 drv_cmd, union hdcdrv_cmd *cmd_data,
    bool *copy_to_user_flag)
{
    long ret = HDCDRV_OK;

    switch (drv_cmd) {
        case HDCDRV_CMD_SET_SESSION_OWNER:
            cmd_data->set_owner.owner_pid = hdcdrv_get_pid();
            ret = hdcdrv_set_session_owner(ctx, &cmd_data->set_owner);
            break;
        case HDCDRV_CMD_GET_STAT:
            ret = hdcdrv_get_stat(&cmd_data->get_stat);
            *copy_to_user_flag = true;
            break;
        case HDCDRV_CMD_GET_SESSION_RUN_ENV:
            ret = hdcdrv_session_run_env(&cmd_data->get_session_run_env);
            *copy_to_user_flag = true;
            break;
        case HDCDRV_CMD_SET_SESSION_TIMEOUT:
            ret = hdcdrv_set_session_timeout(&cmd_data->set_session_timeout);
            break;
        case HDCDRV_CMD_GET_SESSION_UID:
            ret = hdcdrv_get_uid_state(&cmd_data->get_uid_stat);
            *copy_to_user_flag = true;
            break;
        case HDCDRV_CMD_GET_PAGE_SIZE:
            ret = hdcdrv_get_page_size(&cmd_data->get_page_size);
            *copy_to_user_flag = true;
            break;
        default:
            hdcdrv_err("cmd %u is illegal\n", drv_cmd);
            return HDCDRV_PARA_ERR;
    }

    return ret;
}

long hdcdrv_fast_operation(struct hdcdrv_ctx *ctx, u32 drv_cmd,
    union hdcdrv_cmd *cmd_data, bool *copy_to_user_flag)
{
    long ret = HDCDRV_OK;
    switch (drv_cmd) {
        case HDCDRV_CMD_ALLOC_MEM:
            ret = hdcdrv_fast_alloc_mem(ctx, &cmd_data->alloc_mem);
            break;
        case HDCDRV_CMD_FREE_MEM:
            ret = hdcdrv_fast_free_mem(&cmd_data->free_mem);
            *copy_to_user_flag = true;
            break;
        case HDCDRV_CMD_FAST_SEND:
            ret = hdcdrv_fast_send(&cmd_data->fast_send);
            break;
        case HDCDRV_CMD_FAST_RECV:
            ret = hdcdrv_fast_recv(&cmd_data->fast_recv);
            *copy_to_user_flag = true;
            break;
        case HDCDRV_CMD_DMA_MAP:
            ret = hdcdrv_fast_dma_map(&cmd_data->dma_map);
            break;
        case HDCDRV_CMD_DMA_UNMAP:
            ret = hdcdrv_fast_dma_unmap(&cmd_data->dma_unmap);
            break;
        case HDCDRV_CMD_DMA_REMAP:
            ret = hdcdrv_fast_dma_remap(&cmd_data->dma_remap);
            break;
        default:
            hdcdrv_err("cmd %u is illegal\n", drv_cmd);
            return HDCDRV_PARA_ERR;
    }
    return ret;
}

long hdcdrv_ioctl(struct file *file, unsigned int cmd, unsigned long arg)
{
    struct hdcdrv_ctx *ctx = NULL;
    void __user *argp = (void __user *)((uintptr_t)arg);
    u32 drv_cmd = _IOC_NR(cmd);
    bool copy_to_user_flag = false;
    union hdcdrv_cmd cmd_data;
    long ret;

    if (file == NULL) {
        hdcdrv_err("file is null pointer\n");
        return HDCDRV_PARA_ERR;
    }
    ctx = (struct hdcdrv_ctx *)file->private_data;

    if ((_IOC_TYPE(cmd) != HDCDRV_CMD_MAGIC) || (argp == NULL)) {
        hdcdrv_err("cmd %x type is error\n", cmd);
        return HDCDRV_PARA_ERR;
    }

    if (copy_from_user(&cmd_data, argp, sizeof(cmd_data))) {
        hdcdrv_err("cmd %u is copy_from_user failed\n", drv_cmd);
        return HDCDRV_COPY_FROM_USER_FAIL;
    }

    if (hdcdrv_convert_id_from_vir_to_phy(drv_cmd, &cmd_data) != HDCDRV_OK) {
        hdcdrv_err("convert virtual id to physical id failed\n");
        return HDCDRV_CONV_FAILED;
    }

    if (drv_cmd < HDCDRV_CMD_SET_SESSION_OWNER) {
        ret = hdcdrv_operation(ctx, drv_cmd, &cmd_data, &copy_to_user_flag);
    } else if(drv_cmd < HDCDRV_CMD_ALLOC_MEM) {
        ret = hdcdrv_cfg_operation(ctx, drv_cmd, &cmd_data, &copy_to_user_flag);
    } else if (drv_cmd < HDCDRV_CMD_EPOLL_ALLOC_FD) {
        ret = hdcdrv_fast_operation(ctx, drv_cmd, &cmd_data, &copy_to_user_flag);
    } else {
        ret = hdcdrv_epoll_operation(ctx, drv_cmd, &cmd_data, &copy_to_user_flag);
    }

    if ((ret == HDCDRV_OK) && copy_to_user_flag) {
        if (copy_to_user(argp, &cmd_data, sizeof(cmd_data))) {
            hdcdrv_err("cmd %d is copy_to_user failed\n", drv_cmd);
            return HDCDRV_COPY_TO_USER_FAIL;
        }
    }

    return ret;
}

/* Avoid the user process calling mlockall (MCL_FUTURE) to establish the page table in advance,
   which will cause our subsequent page table creation to fail */
int hdcdrv_mmap(struct file *filep, struct vm_area_struct *vma)
{
    return HDCDRV_OK;
}

long long hdcdrv_get_pid(void)
{
    return (long long)current->tgid;
}

void hdcdrv_set_debug_mode(int flag)
{
    mutex_lock(&hdc_ctrl->mutex);
    if (flag) {
        hdc_ctrl->debug_state.valid = HDCDRV_VALID;
        hdcdrv_info("Step into debug mode.\n");
    } else {
        hdc_ctrl->debug_state.valid = HDCDRV_INVALID;
        hdcdrv_info("Step out debug mode.\n");
    }
    hdc_ctrl->debug_state.pid = hdcdrv_get_pid();
    mutex_unlock(&hdc_ctrl->mutex);
    return;
}
EXPORT_SYMBOL(hdcdrv_set_debug_mode);

int hdcdrv_open(struct inode *node, struct file *file)
{
    struct hdcdrv_ctx *ctx = NULL;

    ctx = kzalloc(sizeof(struct hdcdrv_ctx), GFP_KERNEL | __GFP_ACCOUNT);
    if (ctx == NULL) {
        hdcdrv_err("hdcdrv_open kzalloc failed\n");
        return HDCDRV_MEM_ALLOC_FAIL;
    }

    ctx->service = NULL;
    ctx->session = NULL;

    spin_lock_init(&ctx->mem_lock);
    INIT_LIST_HEAD(&ctx->mlist.list);

    ctx->file = file;
    file->private_data = ctx;

    return HDCDRV_OK;
}

void hdcdrv_count_mem_info(const struct hdcdrv_fast_mem *fast_mem, struct hdcdrv_mem_stat *mem_info)
{
    if ((fast_mem->mem_type >= 0) && (fast_mem->mem_type < HDCDRV_FAST_MEM_TYPE_MAX)) {
        mem_info->mem_nums[fast_mem->mem_type]++;
        mem_info->mem_size[fast_mem->mem_type] += fast_mem->alloc_len;
    }
}

void hdcdrv_release_close_session(struct hdcdrv_ctx *ctx)
{
    int ret = 0;
    struct hdcdrv_cmd_close close_cmd;
    struct hdcdrv_session *session = ctx->session;

    /* session free */
    if (ctx->session != NULL) {
        hdcdrv_info("release session : dev %d task pid %lld, local session id %d, remote id %d session status %d\n",
            ctx->dev_id, ctx->pid, ctx->session_fd, session->remote_session_fd,
            hdcdrv_get_session_status(session));

        close_cmd.session = ctx->session_fd;
        hdcdrv_sid_copy(close_cmd.session_id, HDCDRV_SID_LEN, ctx->session_id, HDCDRV_SID_LEN);
        close_cmd.pid = ctx->pid;
        ret = hdcdrv_close(&close_cmd, HDCDRV_CLOSE_TYPE_RELEASE);
        if (ret != HDCDRV_OK) {
            hdcdrv_warn("close session failed: dev %d task pid %lld, local id %d, remote id %d session status %d\n",
                ctx->dev_id, ctx->pid, ctx->session_fd, session->remote_session_fd,
                hdcdrv_get_session_status(session));
        }
        ctx->session = NULL;
    }
}

void hdcdrv_release_destroy_server(struct hdcdrv_ctx *ctx)
{
    int ret = 0;
    struct hdcdrv_cmd_server_destroy destroy_cmd;

    /* service free */
    if (ctx->service != NULL) {
        hdcdrv_info("release server : task pid %lld, dev_id %d, service type %s\n", ctx->service->listen_pid,
                    ctx->dev_id, hdcdrv_sevice_str(ctx->service_type));

        destroy_cmd.dev_id = ctx->dev_id;
        destroy_cmd.service_type = ctx->service_type;
        destroy_cmd.pid = ctx->pid;
        ret = hdcdrv_server_destroy(&destroy_cmd);
        if (ret != HDCDRV_OK) {
            hdcdrv_warn("release server : task pid %lld, dev_id %d, service type %s\n", ctx->pid,
                ctx->dev_id, hdcdrv_sevice_str(ctx->service_type));
        }

        ctx->service = NULL;
    }
}

void hdcdrv_release_free_mem(struct hdcdrv_ctx *ctx)
{
    struct hdcdrv_mem_fd_list *entry = NULL;
    struct list_head *pos = NULL;
    struct list_head *n = NULL;
    struct hdcdrv_mem_stat mem_info = {{0}};

    /* memory free */
    if (!list_empty_careful(&ctx->mlist.list)) {
        hdcdrv_info("release memory : task pid %lld, count %d\n", hdcdrv_get_pid(), ctx->count);

        usleep_range(2000, 3000);
        list_for_each_safe(pos, n, &ctx->mlist.list)
        {
            entry = list_entry(pos, struct hdcdrv_mem_fd_list, list);
            hdcdrv_count_mem_info(&entry->f_node->fast_mem, &mem_info);
            hdcdrv_fast_mem_free_abnormal(entry->f_node);

            spin_lock_bh(&ctx->mem_lock);
            list_del(&entry->list);
            spin_unlock_bh(&ctx->mem_lock);

            kfree(entry);
        }
    }
}

int hdcdrv_release(struct inode *node, struct file *file)
{
    struct hdcdrv_ctx *ctx = file->private_data;

    if (ctx == NULL) {
        hdcdrv_err("ctx is null ! task pid %lld", hdcdrv_get_pid());
        return HDCDRV_PARA_ERR;
    }

    hdcdrv_release_close_session(ctx);

    hdcdrv_release_destroy_server(ctx);

    hdcdrv_release_free_mem(ctx);

    if (hdc_ctrl->debug_state.pid == hdcdrv_get_pid()) {
        hdcdrv_set_debug_mode(HDCDRV_INVALID);
    }

    hdcdrv_epoll_recycle_fd(ctx);

    file->private_data = NULL;
    kfree(ctx);
    ctx = NULL;

    return HDCDRV_OK;
}

/* The following interface is provided for the kernel to call hdc */
bool hdcdrv_is_service_init(unsigned int service_type)
{
    if (service_type >= HDCDRV_SURPORT_MAX_SERVICE) {
        return false;
    }

    if (hdc_ctrl == NULL) {
        return false;
    }

    return true;
}
EXPORT_SYMBOL(hdcdrv_is_service_init);

/* for ide type, set level to 1 */
long hdcdrv_kernel_set_service_level(int service_type, int level)
{
    struct hdcdrv_cmd_set_service_level cmd;

    cmd.service_type = service_type;
    cmd.level = level;

    return hdcdrv_set_service_level(&cmd);
}
EXPORT_SYMBOL(hdcdrv_kernel_set_service_level);

void hdcdrv_set_segment(int segment)
{
    hdc_ctrl->segment = segment;
    hdcdrv_info("hdc segment %d.\n", hdc_ctrl->segment);
}

int hdcdrv_get_segment(void)
{
    return hdcdrv_mem_block_capacity();
}
EXPORT_SYMBOL(hdcdrv_get_segment);

void hdcdrv_set_peer_dev_id(int dev_id, int peer_dev_id)
{
    hdc_ctrl->devices[dev_id].peer_dev_id = peer_dev_id;
}

int hdcdrv_get_peer_dev_id(int dev_id)
{
    return hdc_ctrl->devices[dev_id].peer_dev_id;
}

long hdcdrv_kernel_server_create(int dev_id, int service_type)
{
    struct hdcdrv_cmd_server_create cmd;

    cmd.dev_id = dev_id;
    cmd.service_type = service_type;
    cmd.pid = HDCDRV_KERNEL_DEFAULT_PID;

    return hdcdrv_server_create(HDCDRV_KERNEL_WITHOUT_CTX, &cmd);
}
EXPORT_SYMBOL(hdcdrv_kernel_server_create);

long hdcdrv_kernel_server_destroy(int dev_id, int service_type)
{
    struct hdcdrv_cmd_server_destroy cmd;

    cmd.dev_id = dev_id;
    cmd.service_type = service_type;
    cmd.pid = HDCDRV_KERNEL_DEFAULT_PID;

    return hdcdrv_server_destroy(&cmd);
}
EXPORT_SYMBOL(hdcdrv_kernel_server_destroy);

long hdcdrv_kernel_accept(int dev_id, int service_type, int *session, char *session_id)
{
    struct hdcdrv_cmd_set_session_owner set_pid_cmd;
    struct hdcdrv_cmd_accept cmd;
    long ret;
    long res;

    if ((session == NULL) || (session_id == NULL)) {
        return HDCDRV_PARA_ERR;
    }

    cmd.dev_id = dev_id;
    cmd.service_type = service_type;

    ret = hdcdrv_accept(&cmd);
    if (ret == HDCDRV_OK) {
        *session = cmd.session;
        hdcdrv_sid_copy(session_id, HDCDRV_SID_LEN, cmd.session_id, HDCDRV_SID_LEN);
        set_pid_cmd.session = cmd.session;
        hdcdrv_sid_copy(set_pid_cmd.session_id, HDCDRV_SID_LEN, cmd.session_id, HDCDRV_SID_LEN);
        set_pid_cmd.owner_pid = HDCDRV_KERNEL_DEFAULT_PID;

        res = hdcdrv_set_session_owner(HDCDRV_KERNEL_WITHOUT_CTX, &set_pid_cmd);
        if (res != HDCDRV_OK) {
            hdcdrv_err("dev %d session %d set owner failed.\n", dev_id, cmd.session);
        }
    }

    return ret;
}
EXPORT_SYMBOL(hdcdrv_kernel_accept);

long hdcdrv_kernel_connect(int dev_id, int service_type, int *session, char *session_id)
{
    struct hdcdrv_cmd_set_session_owner set_pid_cmd;
    struct hdcdrv_cmd_conncet cmd;
    long ret;
    long res;

    if ((session == NULL) || (session_id == NULL)) {
        return HDCDRV_PARA_ERR;
    }

    cmd.dev_id = dev_id;
    cmd.service_type = service_type;

    ret = hdcdrv_connect(&cmd);
    if (ret == HDCDRV_OK) {
        *session = cmd.session;
        hdcdrv_sid_copy(session_id, HDCDRV_SID_LEN, cmd.session_id, HDCDRV_SID_LEN);
        set_pid_cmd.session = cmd.session;
        hdcdrv_sid_copy(set_pid_cmd.session_id, HDCDRV_SID_LEN, cmd.session_id, HDCDRV_SID_LEN);
        set_pid_cmd.owner_pid = HDCDRV_KERNEL_DEFAULT_PID;

        res = hdcdrv_set_session_owner(HDCDRV_KERNEL_WITHOUT_CTX, &set_pid_cmd);
        if (res != HDCDRV_OK) {
            hdcdrv_err("dev %d session %d set owner failed.\n", dev_id, cmd.session);
        }
    }

    return ret;
}
EXPORT_SYMBOL(hdcdrv_kernel_connect);

long hdcdrv_kernel_close(int session, char *session_id)
{
    struct hdcdrv_cmd_close cmd;

    if (session_id == NULL) {
        return HDCDRV_PARA_ERR;
    }

    cmd.session = session;
    hdcdrv_sid_copy(cmd.session_id, HDCDRV_SID_LEN, session_id, HDCDRV_SID_LEN);
    cmd.pid = HDCDRV_KERNEL_DEFAULT_PID;

    return hdcdrv_close(&cmd, HDCDRV_CLOSE_TYPE_KERNEL);
}
EXPORT_SYMBOL(hdcdrv_kernel_close);

long hdcdrv_kernel_send_timeout(int session, char *session_id, void *buf, int len, int timeout)
{
    struct hdcdrv_cmd_send cmd;

    if ((buf == NULL) || (session_id == NULL)) {
        return HDCDRV_PARA_ERR;
    }

    cmd.session = session;
    hdcdrv_sid_copy(cmd.session_id, HDCDRV_SID_LEN, session_id, HDCDRV_SID_LEN);
    cmd.buf = buf;
    cmd.len = len;
    cmd.wait_flag = hdcdrv_get_wait_flag(timeout);
    cmd.timeout = timeout;

    return hdcdrv_send(&cmd, HDCDRV_MODE_KERNEL);
}
EXPORT_SYMBOL(hdcdrv_kernel_send_timeout);

long hdcdrv_kernel_send(int session, char *session_id, void *buf, int len)
{
    /* send block */
    return hdcdrv_kernel_send_timeout(session, session_id, buf, len, -1);
}
EXPORT_SYMBOL(hdcdrv_kernel_send);

long hdcdrv_kernel_recv_peek_timeout(int session, char *session_id, int *len, int timeout)
{
    struct hdcdrv_cmd_recv_peek cmd;
    long ret;

    if ((len == NULL) || (session_id == NULL)) {
        return HDCDRV_PARA_ERR;
    }

    cmd.session = session;
    hdcdrv_sid_copy(cmd.session_id, HDCDRV_SID_LEN, session_id, HDCDRV_SID_LEN);
    cmd.wait_flag = hdcdrv_get_wait_flag(timeout);

    ret = hdcdrv_recv_peek(&cmd);
    if (ret == HDCDRV_OK) {
        *len = cmd.len;
    }

    return ret;
}
EXPORT_SYMBOL(hdcdrv_kernel_recv_peek_timeout);

long hdcdrv_kernel_recv_peek(int session, char *session_id, int *len)
{
    /* recv peek block */
    return hdcdrv_kernel_recv_peek_timeout(session, session_id, len, -1);
}
EXPORT_SYMBOL(hdcdrv_kernel_recv_peek);

long hdcdrv_kernel_recv(int session, char *session_id, void *buf, int len, int *out_len)
{
    struct hdcdrv_cmd_recv cmd;
    long ret;

    if ((buf == NULL) || (out_len == NULL) || (session_id == NULL)) {
        return HDCDRV_PARA_ERR;
    }

    cmd.session = session;
    hdcdrv_sid_copy(cmd.session_id, HDCDRV_SID_LEN, session_id, HDCDRV_SID_LEN);
    cmd.buf = buf;
    cmd.len = len;

    ret = hdcdrv_recv(&cmd, HDCDRV_MODE_KERNEL);
    if (ret == HDCDRV_OK) {
        *out_len = cmd.out_len;
    }

    return ret;
}
EXPORT_SYMBOL(hdcdrv_kernel_recv);

long hdcdrv_kernel_get_session_run_env(int session, char *session_id, int *run_env)
{
    struct hdcdrv_cmd_get_session_run_env cmd;
    long ret;

    if ((run_env == NULL) || (session_id == NULL)) {
        return HDCDRV_PARA_ERR;
    }

    cmd.session = session;
    hdcdrv_sid_copy(cmd.session_id, HDCDRV_SID_LEN, session_id, HDCDRV_SID_LEN);

    ret = hdcdrv_session_run_env(&cmd);
    if (ret == HDCDRV_OK) {
        *run_env = cmd.run_env;
    }

    return ret;
}
EXPORT_SYMBOL(hdcdrv_kernel_get_session_run_env);


void hdcdrv_guard_work(struct work_struct *p_work)
{
    int i, j;
    struct hdcdrv_msg_chan *msg_chan = NULL;
    struct delayed_work *delayed_work = container_of(p_work, struct delayed_work, work);

    for (i = 0; i < HDCDRV_SURPORT_MAX_DEV; i++) {
        if (hdc_ctrl->devices[i].valid != HDCDRV_VALID) {
            continue;
        }

        for (j = 0; j < HDCDRV_SURPORT_MAX_DEV_MSG_CHAN; j++) {
            msg_chan = hdc_ctrl->devices[i].msg_chan[j];
            if (msg_chan == NULL) {
                continue;
            }

            hdcdrv_tx_finish_task_check(msg_chan);
            hdcdrv_rx_msg_task_check(msg_chan);
            hdcdrv_rx_msg_notify_task_check(msg_chan);
        }
    }

    schedule_delayed_work(delayed_work, 1 * HZ);
}

void hdcdrv_close_unknow_session_work(struct work_struct *p_work)
{
    int ret = 0;
    struct hdcdrv_cmd_close close_cmd;
    struct hdcdrv_session *session = container_of(p_work, struct hdcdrv_session, close_unknow_session.work);

    hdcdrv_delay_work_flag_clear(session, HDCDRV_DELAY_UNKNOWN_SESSION_BIT);
    if (hdcdrv_get_session_status(session) == HDCDRV_SESSION_STATUS_CONN) {
        hdcdrv_info("hdcdrv close unknow session, dev %d service_type %s,"
            "local session_fd %d remote session fd %d\n",
            session->dev_id, hdcdrv_sevice_str(session->service_type),
            session->local_session_fd, session->remote_session_fd);

        close_cmd.session = session->local_session_fd;
        hdcdrv_sid_copy(close_cmd.session_id, HDCDRV_SID_LEN, session->session_id, HDCDRV_SID_LEN);
        close_cmd.pid = session->pid;

        ret = hdcdrv_close(&close_cmd, HDCDRV_CLOSE_TYPE_NOT_SET_OWNER);
        if (ret != HDCDRV_OK) {
            hdcdrv_warn("dev %d close unknown session failed, ret %d\n", session->dev_id, ret);
        }
    }
}

int hdcdrv_add_ctrl_msg_chan_to_dev(u32 dev_id, void *chan)
{
    struct hdcdrv_dev *hdc_dev = NULL;

    if (dev_id >= HDCDRV_SURPORT_MAX_DEV) {
        hdcdrv_err("dev_id %u out of range\n", dev_id);
        return HDCDRV_PARA_ERR;
    }

    hdc_dev = &hdc_ctrl->devices[dev_id];
    hdc_dev->ctrl_msg_chan = chan;
    return HDCDRV_OK;
}

int hdcdrv_add_msg_chan_to_dev(u32 dev_id, void *chan)
{
    struct hdcdrv_dev *hdc_dev = NULL;
    struct hdcdrv_msg_chan *msg_chan = NULL;
    int i;

    if (dev_id >= HDCDRV_SURPORT_MAX_DEV) {
        hdcdrv_err("dev_id %u out of range\n", dev_id);
        return HDCDRV_PARA_ERR;
    }

    hdc_dev = &hdc_ctrl->devices[dev_id];

    if ((hdc_dev->msg_chan_cnt >= HDCDRV_SURPORT_MAX_DEV_MSG_CHAN) || (hdc_dev->msg_chan_cnt < 0)) {
        hdcdrv_err("hdc_dev->msg_chan_cnt %u out of range\n", hdc_dev->msg_chan_cnt);
        return HDCDRV_PARA_ERR;
    }

    mutex_lock(&hdc_dev->mutex);

    msg_chan = hdc_dev->msg_chan[hdc_dev->msg_chan_cnt];

    if (msg_chan == NULL) {
        msg_chan = (struct hdcdrv_msg_chan *)vzalloc(sizeof(struct hdcdrv_msg_chan));
        if (msg_chan == NULL) {
            mutex_unlock(&hdc_dev->mutex);
            hdcdrv_err("dev %u alloc hdcdrv_msg_chan failed, size %ld\n", dev_id, sizeof(struct hdcdrv_msg_chan));
            return HDCDRV_MEM_ALLOC_FAIL;
        }

        mutex_init(&msg_chan->mutex);
    }

    if (hdc_dev->msg_chan_cnt == 0) {
        if (hdcdrv_init_mem_pool(hdc_dev->dev_id) != HDCDRV_OK) {
            mutex_unlock(&hdc_dev->mutex);
            vfree(msg_chan);
            msg_chan = NULL;
            return HDCDRV_DMA_MEM_ALLOC_FAIL;
        }
    }

    msg_chan->chan_id = hdc_dev->msg_chan_cnt;
    hdc_dev->msg_chan_cnt++;
    mutex_unlock(&hdc_dev->mutex);

    msg_chan->dev_id = dev_id;
    msg_chan->dev = hdc_dev->dev;

    msg_chan->dma_head = HDCDRV_DESC_QUEUE_DEPTH - 1;
    msg_chan->rx_head = HDCDRV_DESC_QUEUE_DEPTH - 1;
    msg_chan->submit_dma_head = HDCDRV_DESC_QUEUE_DEPTH - 1;

    msg_chan->chan = chan;
    msg_chan->session_cnt = 0;

    for (i = 0; i < HDCDRV_DESC_QUEUE_DEPTH; i++) {
        msg_chan->tx[i].buf = NULL;
        msg_chan->rx[i].buf = NULL;
    }

    msg_chan->rx_trigger_flag = HDCDRV_INVALID;
    msg_chan->dma_need_submit_flag = HDCDRV_INVALID;

    tasklet_init(&msg_chan->tx_finish_task, hdcdrv_tx_finish_notify_task, (uintptr_t)msg_chan);
    tasklet_init(&msg_chan->rx_notify_task, hdcdrv_rx_msg_notify_task, (uintptr_t)msg_chan);
    tasklet_init(&msg_chan->rx_task, hdcdrv_msg_chan_recv_task, (uintptr_t)msg_chan);

    if (hdc_dev->msg_chan_cnt <= hdc_dev->normal_chan_num) {
        msg_chan->type = HDCDRV_MSG_CHAN_TYPE_NORMAL;
    } else {
        msg_chan->type = HDCDRV_MSG_CHAN_TYPE_FAST;
    }

    msg_chan->data_type = hdcdrv_get_dma_data_type(msg_chan->type, msg_chan->chan_id);
    msg_chan->wait_mem_list.next = NULL;
    msg_chan->wait_mem_list.prev = NULL;

    init_waitqueue_head(&msg_chan->send_wait);

    hdcdrv_set_msg_chan_priv(chan, msg_chan);

    hdc_dev->msg_chan[msg_chan->chan_id] = msg_chan;

    return HDCDRV_OK;
}

struct hdcdrv_dev *hdcdrv_add_dev(struct device *dev, u32 dev_id)
{
    struct hdcdrv_dev *hdc_dev = NULL;
    int i;

    if (dev_id >= HDCDRV_SURPORT_MAX_DEV) {
        hdcdrv_err("dev_id %u, out of range %d\n", dev_id, HDCDRV_SURPORT_MAX_DEV);
        return NULL;
    }

    hdc_dev = &hdc_ctrl->devices[dev_id];

    if (hdc_dev->sync_mem_buf == NULL) {
        hdc_dev->sync_mem_buf = (void *)vzalloc(HDCDRV_NON_TRANS_MSG_S_DESC_SIZE);
        if (hdc_dev->sync_mem_buf == NULL) {
            hdcdrv_err("dev_id %d, alloc sync_mem_buf failed\n", dev_id);
            return NULL;
        }
    }

    mutex_init(&hdc_dev->sync_mem_mutex);
    mutex_init(&hdc_dev->mutex);

    for (i = 0; i < HDCDRV_SURPORT_MAX_SERVICE; i++) {
        mutex_init(&hdc_dev->service[i].mutex);
        init_waitqueue_head(&hdc_dev->service[i].wq_conn_avail);
        hdc_dev->service[i].conn_list_head = NULL;
    }

    hdc_dev->dev = dev;
    hdc_dev->dev_id = dev_id;

    return hdc_dev;
}

void hdcdrv_reset_session(int dev_id)
{
    struct hdcdrv_session *session = NULL;
    int id;

    for (id = 0; id < HDCDRV_SURPORT_MAX_SESSION; id++) {
        session = &hdc_ctrl->sessions[id];
        if ((session->dev_id != dev_id) ||
            (hdcdrv_get_session_status(session) == HDCDRV_SESSION_STATUS_IDLE)) {
            continue;
        }
        hdcdrv_info("reset info: dev %d session %d\n", dev_id, id);
        hdcdrv_session_free(session);
    }
}

void hdcdrv_reset_service(struct hdcdrv_dev *hdc_dev)
{
    struct hdcdrv_service *service = NULL;
    int i;
    int ret = 0;

    for (i = 0; i < HDCDRV_SURPORT_MAX_SERVICE; i++) {
        service = &hdc_dev->service[i];
        if (service->listen_status == HDCDRV_VALID) {
            hdcdrv_info("reset info: dev %d service %d wakeup accept.\n", hdc_dev->dev_id, i);
            ret = hdcdrv_server_free(service, hdc_dev->dev_id, i);
            if (ret != HDCDRV_OK) {
                hdcdrv_warn("reset failed: dev %d service %d wakeup accept.\n", hdc_dev->dev_id, i);
            }
        }
    }
}

void hdcdrv_reset_msgchain(struct hdcdrv_dev *hdc_dev)
{
    struct hdcdrv_msg_chan *msg_chan = NULL;
    int i;

    for (i = 0; i < hdc_dev->msg_chan_cnt; i++) {
        msg_chan = hdc_dev->msg_chan[i];
        /* Clear the packets in the waiting queue */
        wake_up_interruptible(&msg_chan->send_wait);
    }
}

void hdcdrv_kill_task(struct hdcdrv_dev *hdc_dev)
{
    struct hdcdrv_msg_chan *msg_chan = NULL;
    int msg_chan_num = hdc_dev->msg_chan_cnt;
    int i;

    for (i = 0; i < msg_chan_num; i++) {
        msg_chan = hdc_dev->msg_chan[i];
        hdcdrv_set_msg_chan_priv(msg_chan->chan, NULL);
        tasklet_kill(&msg_chan->tx_finish_task);
        tasklet_kill(&msg_chan->rx_notify_task);
        tasklet_kill(&msg_chan->rx_task);
    }
}

void hdcdrv_free_msg_chan(struct hdcdrv_dev *hdc_dev)
{
    int msg_chan_num = hdc_dev->msg_chan_cnt;
    int i;

    for (i = 0; i < msg_chan_num; i++) {
        mutex_lock(&hdc_dev->mutex);
        hdc_dev->msg_chan_cnt--;
        mutex_unlock(&hdc_dev->mutex);
    }
}

void hdcdrv_free_msg_chan_mem(struct hdcdrv_dev *hdc_dev)
{
    struct hdcdrv_msg_chan *msg_chan = NULL;
    int msg_chan_num = hdc_dev->msg_chan_cnt;
    int i, j;

    for (i = 0; i < msg_chan_num; i++) {
        msg_chan = hdc_dev->msg_chan[i];
        for (j = 0; j < HDCDRV_DESC_QUEUE_DEPTH; j++) {
            if (msg_chan->tx[j].buf != NULL) {
                free_mem(msg_chan->tx[j].buf);
            }

            if (msg_chan->rx[j].buf != NULL) {
                free_mem(msg_chan->rx[j].buf);
            }
        }
    }
}

void hdcdrv_stop_work(struct hdcdrv_dev *hdc_dev)
{
    int dev_id = hdc_dev->dev_id;

    hdcdrv_reset_session(dev_id);
    hdcdrv_reset_service(hdc_dev);
    hdcdrv_kill_task(hdc_dev);
}

void hdcdrv_remove_dev(struct hdcdrv_dev *hdc_dev)
{
    int dev_id = hdc_dev->dev_id;
    hdcdrv_free_msg_chan_mem(hdc_dev);
    hdcdrv_free_msg_chan(hdc_dev);

    free_mem_pool(HDCDRV_MEM_POOL_TYPE_TX, dev_id, hdc_dev->small_mem_pool[HDCDRV_MEM_POOL_TYPE_TX].segment);
    free_mem_pool(HDCDRV_MEM_POOL_TYPE_TX, dev_id, hdc_dev->huge_mem_pool[HDCDRV_MEM_POOL_TYPE_TX].segment);
    free_mem_pool(HDCDRV_MEM_POOL_TYPE_RX, dev_id, hdc_dev->small_mem_pool[HDCDRV_MEM_POOL_TYPE_RX].segment);
    free_mem_pool(HDCDRV_MEM_POOL_TYPE_RX, dev_id, hdc_dev->huge_mem_pool[HDCDRV_MEM_POOL_TYPE_RX].segment);

    hdcdrv_fast_mem_uninit(&hdc_ctrl->devices[dev_id].remote_rb_mems, HDCDRV_TRUE_FLAG);
    hdcdrv_dfx_node_uninit(&hdc_ctrl->devices[dev_id].remote_mem_rb_mems);
}

void hdcdrv_free_dev_mem(u32 dev_id)
{
    struct hdcdrv_dev *hdc_dev = &hdc_ctrl->devices[dev_id];
    int i;

    for (i = 0; i < HDCDRV_SURPORT_MAX_DEV_MSG_CHAN; i++) {
        if (hdc_dev->msg_chan[i] != NULL) {
            vfree(hdc_dev->msg_chan[i]);
            hdc_dev->msg_chan[i] = NULL;
        }
    }

    if (hdc_dev->sync_mem_buf != NULL) {
        vfree(hdc_dev->sync_mem_buf);
        hdc_dev->sync_mem_buf = NULL;
    }
}

void hdcdrv_del_dev(u32 dev_id)
{
    struct hdcdrv_dev *hdc_dev = &hdc_ctrl->devices[dev_id];

    if (hdc_dev->valid == HDCDRV_INVALID) {
        return;
    }

    hdcdrv_reset_dev(hdc_dev);
    msleep(500);
    hdcdrv_stop_work(hdc_dev);
    hdcdrv_remove_dev(hdc_dev);
}

void hdcdrv_reset_dev(struct hdcdrv_dev *hdc_dev)
{
    hdcdrv_info("dev %u reset...\n", hdc_dev->dev_id);

    hdcdrv_set_device_status(hdc_dev->dev_id, HDCDRV_INVALID);

    msleep(10);

    hdcdrv_reset_msgchain(hdc_dev);

    hdcdrv_reset_session(hdc_dev->dev_id);

    hdcdrv_reset_service(hdc_dev);
}

const struct file_operations hdcdrv_fops = {
    .owner = THIS_MODULE,
    .unlocked_ioctl = hdcdrv_ioctl,
    .open = hdcdrv_open,
    .mmap = hdcdrv_mmap,
    .release = hdcdrv_release,
};

char *hdcdrv_devnode(struct device *dev, umode_t *mode)
{
    if (mode != NULL) {
        *mode = HDCDRV_DEV_MODE;
    }
    return NULL;
}

int hdcdrv_register_cdev(void)
{
    int ret;
    struct device *dev = NULL;

    ret = alloc_chrdev_region(&hdc_ctrl->dev_no, 0, 1, HDCDRV_CHAR_DRIVER_NAME);
    if (ret) {
        hdcdrv_err("alloc char dev failed. ret %d\n", ret);
        return ret;
    }

    /* init and add char device */
    cdev_init(&hdc_ctrl->cdev, &hdcdrv_fops);
    hdc_ctrl->cdev.owner = THIS_MODULE;

    ret = cdev_add(&hdc_ctrl->cdev, hdc_ctrl->dev_no, 1);
    if (ret) {
        hdcdrv_err("add char dev failed. ret %d\n", ret);
        goto cdev_add_failed;
    }

    hdc_ctrl->cdev_class = class_create(THIS_MODULE, HDCDRV_CHAR_DRIVER_NAME);
    if (hdc_ctrl->cdev_class == NULL) {
        hdcdrv_err("class create failed.\n");
        ret = HDCDRV_CHAR_DEV_CREAT_FAIL;
        goto class_create_failed;
    }
    hdc_ctrl->cdev_class->devnode = hdcdrv_devnode;
    dev = device_create(hdc_ctrl->cdev_class, NULL, hdc_ctrl->dev_no, NULL, HDCDRV_CHAR_DRIVER_NAME);
    if (IS_ERR(dev)) {
        hdcdrv_err("device create failed.\n");
        ret = HDCDRV_CHAR_DEV_CREAT_FAIL;
        goto dev_create_failed;
    }

    return HDCDRV_OK;

dev_create_failed:
    class_destroy(hdc_ctrl->cdev_class);
class_create_failed:
    cdev_del(&hdc_ctrl->cdev);
cdev_add_failed:
    unregister_chrdev_region(hdc_ctrl->dev_no, 1);

    return ret;
}

void hdcdrv_free_cdev(void)
{
    (void)device_destroy(hdc_ctrl->cdev_class, hdc_ctrl->dev_no);
    (void)class_destroy(hdc_ctrl->cdev_class);
    (void)unregister_chrdev_region(hdc_ctrl->dev_no, 1);
    (void)cdev_del(&hdc_ctrl->cdev);
}

void hdcdrv_service_init(void)
{
    hdc_ctrl->service_level[HDCDRV_SERVICE_TYPE_DMP] = HDCDRV_SERVICE_LOW_LEVEL;
    hdc_ctrl->service_level[HDCDRV_SERVICE_TYPE_MATRIX] = HDCDRV_SERVICE_HIGH_LEVEL;
    hdc_ctrl->service_level[HDCDRV_SERVICE_TYPE_IDE1] = HDCDRV_SERVICE_LOW_LEVEL;
    hdc_ctrl->service_level[HDCDRV_SERVICE_TYPE_FILE_TRANS] = HDCDRV_SERVICE_LOW_LEVEL;
    hdc_ctrl->service_level[HDCDRV_SERVICE_TYPE_IDE2] = HDCDRV_SERVICE_LOW_LEVEL;
    hdc_ctrl->service_level[HDCDRV_SERVICE_TYPE_LOG] = HDCDRV_SERVICE_LOW_LEVEL;
    hdc_ctrl->service_level[HDCDRV_SERVICE_TYPE_RDMA] = HDCDRV_SERVICE_HIGH_LEVEL;
    hdc_ctrl->service_level[HDCDRV_SERVICE_TYPE_BBOX] = HDCDRV_SERVICE_LOW_LEVEL;
    hdc_ctrl->service_level[HDCDRV_SERVICE_TYPE_FRAMEWORK] = HDCDRV_SERVICE_HIGH_LEVEL;
    hdc_ctrl->service_level[HDCDRV_SERVICE_TYPE_TSD] = HDCDRV_SERVICE_HIGH_LEVEL;
    hdc_ctrl->service_level[HDCDRV_SERVICE_TYPE_TDT] = HDCDRV_SERVICE_HIGH_LEVEL;
    hdc_ctrl->service_level[HDCDRV_SERVICE_TYPE_DEBUG] = HDCDRV_SERVICE_LOW_LEVEL;
    hdc_ctrl->service_level[HDCDRV_SERVICE_TYPE_USER1] = HDCDRV_SERVICE_LOW_LEVEL;
    hdc_ctrl->service_level[HDCDRV_SERVICE_TYPE_USER2] = HDCDRV_SERVICE_LOW_LEVEL;
    hdc_ctrl->service_level[HDCDRV_SERVICE_TYPE_USER3] = HDCDRV_SERVICE_LOW_LEVEL;
    hdc_ctrl->service_level[HDCDRV_SERVICE_TYPE_USER4] = HDCDRV_SERVICE_LOW_LEVEL;

    hdc_ctrl->service_conn_feature[HDCDRV_SERVICE_TYPE_DMP] = HDCDRV_SERVICE_SHORT_CONN;
    hdc_ctrl->service_conn_feature[HDCDRV_SERVICE_TYPE_MATRIX] = HDCDRV_SERVICE_LONG_CONN;
    hdc_ctrl->service_conn_feature[HDCDRV_SERVICE_TYPE_IDE1] = HDCDRV_SERVICE_LONG_CONN;
    hdc_ctrl->service_conn_feature[HDCDRV_SERVICE_TYPE_FILE_TRANS] = HDCDRV_SERVICE_LONG_CONN;
    hdc_ctrl->service_conn_feature[HDCDRV_SERVICE_TYPE_IDE2] = HDCDRV_SERVICE_LONG_CONN;
    hdc_ctrl->service_conn_feature[HDCDRV_SERVICE_TYPE_LOG] = HDCDRV_SERVICE_LONG_CONN;
    hdc_ctrl->service_conn_feature[HDCDRV_SERVICE_TYPE_RDMA] = HDCDRV_SERVICE_LONG_CONN;
    hdc_ctrl->service_conn_feature[HDCDRV_SERVICE_TYPE_BBOX] = HDCDRV_SERVICE_LONG_CONN;
    hdc_ctrl->service_conn_feature[HDCDRV_SERVICE_TYPE_FRAMEWORK] = HDCDRV_SERVICE_LONG_CONN;
    hdc_ctrl->service_conn_feature[HDCDRV_SERVICE_TYPE_TSD] = HDCDRV_SERVICE_LONG_CONN;
    hdc_ctrl->service_conn_feature[HDCDRV_SERVICE_TYPE_TDT] = HDCDRV_SERVICE_LONG_CONN;
    hdc_ctrl->service_conn_feature[HDCDRV_SERVICE_TYPE_DEBUG] = HDCDRV_SERVICE_LONG_CONN;
    hdc_ctrl->service_conn_feature[HDCDRV_SERVICE_TYPE_USER1] = HDCDRV_SERVICE_LONG_CONN;
    hdc_ctrl->service_conn_feature[HDCDRV_SERVICE_TYPE_USER2] = HDCDRV_SERVICE_LONG_CONN;
    hdc_ctrl->service_conn_feature[HDCDRV_SERVICE_TYPE_USER3] = HDCDRV_SERVICE_LONG_CONN;
    hdc_ctrl->service_conn_feature[HDCDRV_SERVICE_TYPE_USER4] = HDCDRV_SERVICE_LONG_CONN;
}

int hdcdrv_init(void)
{
    int i;

    hdc_ctrl = (struct hdcdrv_ctrl *)vzalloc(sizeof(struct hdcdrv_ctrl));
    if (hdc_ctrl == NULL) {
        hdcdrv_err("alloc hdc_dev failed, size %ld\n", sizeof(struct hdcdrv_ctrl));
        return HDCDRV_MEM_ALLOC_FAIL;
    }

    mutex_init(&hdc_ctrl->mutex);

    hdc_ctrl->segment = HDCDRV_HUGE_PACKET_SEGMENT;
    hdc_ctrl->cur_alloc_short_session = HDCDRV_SURPORT_MAX_LONG_SESSION;
    hdc_ctrl->rbtree = RB_ROOT;

    for (i = 0; i < HDCDRV_SURPORT_MAX_DEV; i++) {
        hdcdrv_set_peer_dev_id(i, HDCDRV_INVALID_VALUE);
        hdc_ctrl->devices[i].remote_rb_mems = RB_ROOT;
    }
    hdc_ctrl->debug_state.valid = HDCDRV_INVALID;
    hdc_ctrl->debug_state.pid = HDCDRV_INVALID_VALUE;
    spin_lock_init(&hdc_ctrl->lock);
    spin_lock_init(&hdc_ctrl->mem_dfx_stat.lock);

    hdcdrv_service_init();

    for (i = 0; i < HDCDRV_SURPORT_MAX_DEV; i++) {
        hdc_ctrl->devices[i].valid = HDCDRV_INVALID;
    }

    for (i = 0; i < HDCDRV_SURPORT_MAX_SESSION; i++) {
        spin_lock_init(&hdc_ctrl->sessions[i].lock);
        init_waitqueue_head(&hdc_ctrl->sessions[i].wq_conn);
        init_waitqueue_head(&hdc_ctrl->sessions[i].wq_rx);
        INIT_DELAYED_WORK(&hdc_ctrl->sessions[i].remote_close, hdcdrv_remote_close_work);
        INIT_DELAYED_WORK(&hdc_ctrl->sessions[i].close_unknow_session, hdcdrv_close_unknow_session_work);

        hdcdrv_set_session_status(&hdc_ctrl->sessions[i], HDCDRV_SESSION_STATUS_IDLE);
        hdc_ctrl->sessions[i].pid = HDCDRV_INVALID_VALUE;
        hdc_ctrl->sessions[i].local_close_state = HDCDRV_CLOSE_TYPE_NONE;
        hdc_ctrl->sessions[i].remote_close_state = HDCDRV_CLOSE_TYPE_NONE;
    }

    if (hdcdrv_register_cdev()) {
        vfree(hdc_ctrl);
        hdc_ctrl = NULL;
        hdcdrv_err("hdcdrv_register_cdev failed!\n");
        return HDCDRV_CHAR_DEV_CREAT_FAIL;
    }

    hdcdrv_epoll_init(&hdc_ctrl->epolls);

    hdcdrv_dfx_node_init();

    INIT_DELAYED_WORK(&hdc_ctrl->recycle, hdcdrv_guard_work);
    INIT_DELAYED_WORK(&hdc_ctrl->recycle_mem, hdcdrv_recycle_mem_work);

    schedule_delayed_work(&hdc_ctrl->recycle, HDCDRV_RECYCLE_DELAY_TIME * HZ);

    hdcdrv_info("sq size %d, cq size %d\n", (int)HDCDRV_SQ_DESC_SIZE, (int)HDCDRV_CQ_DESC_SIZE);
    return HDCDRV_OK;
}

void hdcdrv_uninit(void)
{
    u32 dev_id;

    cancel_delayed_work_sync(&hdc_ctrl->recycle);
    cancel_delayed_work_sync(&hdc_ctrl->recycle_mem);
    hdcdrv_dfx_node_cancel_work(&hdc_ctrl->mem_dfx_cycle);
    hdcdrv_free_cdev();

    for (dev_id = 0; dev_id < HDCDRV_SURPORT_MAX_DEV; dev_id++) {
        hdcdrv_del_dev(dev_id);
        hdcdrv_free_dev_mem(dev_id);
    }

    hdcdrv_fast_mem_uninit(&hdc_ctrl->rbtree, HDCDRV_TRUE_FLAG);
    hdcdrv_dfx_node_uninit(&hdc_ctrl->mem_rbtree);

    vfree(hdc_ctrl);
    hdc_ctrl = NULL;
}
