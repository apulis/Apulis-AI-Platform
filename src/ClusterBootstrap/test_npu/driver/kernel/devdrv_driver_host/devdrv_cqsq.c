/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2012-2019. All rights reserved.
 * Description:
 * Author: huawei
 * Create: 2019-10-15
 */

#include <linux/slab.h>
#include <linux/spinlock.h>
#include <linux/errno.h>
#include <linux/list.h>
#include <linux/platform_device.h>
#include <linux/vmalloc.h>

#include "devdrv_driver_pm.h"

int devdrv_cqsq_init(struct devdrv_info *dev_info, u32 tsid, u32 num_sq, u32 num_cq)
{
    struct devdrv_cce_ctrl *cce_ctrl = dev_info->cce_ctrl[tsid];
    struct devdrv_platform_data *pdata = dev_info->pdata;
    struct devdrv_sq_sub_info *sq_sub_info = NULL;
    struct devdrv_cq_sub_info *cq_sub_info = NULL;
    struct devdrv_ts_sq_info *sq_info = NULL;
    struct devdrv_ts_cq_info *cq_info = NULL;
    phys_addr_t info_mem_addr;
    unsigned long size;
    u32 i;

    if (!list_empty_careful(&cce_ctrl->sq_available_list) || !list_empty_careful(&cce_ctrl->cq_available_list)) {
        return -ENODEV;
    }

    info_mem_addr = cce_ctrl->mem_info[DEVDRV_INFO_MEM].virt_addr;
    cce_ctrl->sq_num = 0;
    cce_ctrl->cq_num = 0;

    size = (long)(unsigned)sizeof(struct devdrv_sq_sub_info) * num_sq;
    sq_sub_info = vzalloc(size);
    if (sq_sub_info == NULL) {
        return -ENOMEM;
    }

    cce_ctrl->sq_sub_addr = (void *)sq_sub_info;
    size = (long)(unsigned)sizeof(struct devdrv_cq_sub_info) * num_cq;
    cq_sub_info = vzalloc(size);
    if (cq_sub_info == NULL) {
        cce_ctrl->sq_sub_addr = NULL;
        vfree(sq_sub_info);
        sq_sub_info = NULL;
        return -ENOMEM;
    }
    cce_ctrl->cq_sub_addr = (void *)cq_sub_info;

    for (i = 0; i < num_sq; i++) {
        sq_info = devdrv_calc_sq_info(info_mem_addr, i);
        sq_info->head = 0;
        sq_info->tail = 0;
        sq_info->credit = DEVDRV_MAX_SQ_DEPTH - 1;
        sq_info->index = i;
        sq_info->uio_addr = NULL;
        sq_info->uio_num = DEVDRV_INVALID_FD_OR_NUM;
        sq_info->uio_fd = DEVDRV_INVALID_FD_OR_NUM;
        sq_info->uio_map = DEVDRV_SQ_CQ_MAP;
        sq_info->uio_size = DEVDRV_MAX_SQ_DEPTH * DEVDRV_SQ_SLOT_SIZE;
        sq_info->stream_num = 0;
        sq_info->send_count = 0;

        sq_info->sq_sub = (void *)(sq_sub_info + i);
        sq_sub_info[i].index = sq_info->index;
        sq_sub_info[i].uio = NULL;
        sq_sub_info[i].bar_addr = (phys_addr_t)NULL;
        sq_sub_info[i].phy_addr = (phys_addr_t)NULL;
        INIT_LIST_HEAD(&sq_sub_info[i].list_stream);
        if ((i >= pdata->ts_pdata[tsid].id_data.min_sq_id) && (i < pdata->ts_pdata[tsid].id_data.max_sq_id)) {
            list_add_tail(&sq_sub_info[i].list, &cce_ctrl->sq_available_list);
            cce_ctrl->sq_num++;
        }
    }

    for (i = 0; i < num_cq; i++) {
        cq_info = devdrv_calc_cq_info(info_mem_addr, i);
        cq_info->head = 0;
        cq_info->tail = 0;
        cq_info->index = i;
        cq_info->count_report = 0;
        cq_info->uio_addr = NULL;
        cq_info->uio_num = DEVDRV_INVALID_FD_OR_NUM;
        cq_info->uio_fd = DEVDRV_INVALID_FD_OR_NUM;
        cq_info->uio_map = DEVDRV_SQ_CQ_MAP;
        cq_info->uio_size = DEVDRV_MAX_CQ_DEPTH * DEVDRV_CQ_SLOT_SIZE;
        cq_info->stream_num = 0;
        cq_info->receive_count = 0;
        cq_info->phase = 1;
        cq_info->slot_size = DEVDRV_CQ_SLOT_SIZE;
        cq_info->cq_sub = (void *)(cq_sub_info + i);
        cq_sub_info[i].cce_context = NULL;
        cq_sub_info[i].index = cq_info->index;
        cq_sub_info[i].uio = NULL;
        cq_sub_info[i].virt_addr = (phys_addr_t)NULL;
        cq_sub_info[i].phy_addr = (phys_addr_t)NULL;
        INIT_LIST_HEAD(&cq_sub_info[i].list_sq);
        if ((i >= pdata->ts_pdata[tsid].id_data.min_cq_id) && (i < pdata->ts_pdata[tsid].id_data.max_cq_id)) {
            list_add_tail(&cq_sub_info[i].list, &cce_ctrl->cq_available_list);
            cce_ctrl->cq_num++;
        }
        spin_lock_init(&cq_sub_info[i].spinlock);
    }

    return 0;
}

void devdrv_cqsq_destroy(struct devdrv_cce_ctrl *cce_ctrl)
{
    struct devdrv_sq_sub_info *sq_sub_info = NULL;
    struct devdrv_cq_sub_info *cq_sub_info = NULL;
    struct list_head *pos = NULL, *n = NULL;

    if (!list_empty_careful(&cce_ctrl->sq_available_list)) {
        list_for_each_safe(pos, n, &cce_ctrl->sq_available_list)
        {
            cce_ctrl->sq_num--;
            sq_sub_info = list_entry(pos, struct devdrv_sq_sub_info, list);
            list_del(pos);
        }
    }

    if (!list_empty_careful(&cce_ctrl->cq_available_list)) {
        list_for_each_safe(pos, n, &cce_ctrl->cq_available_list)
        {
            cce_ctrl->cq_num--;
            cq_sub_info = list_entry(pos, struct devdrv_cq_sub_info, list);
            list_del(pos);
        }
    }
    vfree(cce_ctrl->sq_sub_addr);
    cce_ctrl->sq_sub_addr = NULL;
    vfree(cce_ctrl->cq_sub_addr);
    cce_ctrl->cq_sub_addr = NULL;
    return;
}

STATIC struct devdrv_ts_sq_info *devdrv_get_sq_evenly(const struct devdrv_cce_ctrl *cce_ctrl,
                                                      const struct devdrv_cce_context *cce_context,
                                                      struct devdrv_stream_info *id)
{
    struct devdrv_stream_sub_info *stream_sub = NULL;
    struct devdrv_sq_sub_info *sq_sub = NULL;
    struct devdrv_ts_sq_info *sq_info = NULL;
    struct list_head *pos = NULL, *n = NULL;
    u32 tsid = cce_ctrl->tsid;
    phys_addr_t info_mem_addr;
    u32 stream_num;
    int sq_index;

    info_mem_addr = cce_ctrl->mem_info[DEVDRV_INFO_MEM].virt_addr;
    stream_sub = (struct devdrv_stream_sub_info *)id->stream_sub;

    sq_sub = list_first_entry(&cce_context->ts_context[tsid].sq_list, struct devdrv_sq_sub_info, list);
    sq_info = devdrv_calc_sq_info(info_mem_addr, sq_sub->index);
    stream_num = sq_info->stream_num;
    sq_index = sq_info->index;

    list_for_each_safe(pos, n, &cce_context->ts_context[tsid].sq_list) {
        sq_sub = list_entry(pos, struct devdrv_sq_sub_info, list);
        sq_info = devdrv_calc_sq_info(info_mem_addr, sq_sub->index);
        if (sq_info->stream_num < stream_num) {
            stream_num = sq_info->stream_num;
            sq_index = sq_info->index;
        }
    }

    sq_info = devdrv_calc_sq_info(info_mem_addr, sq_index);
    sq_sub = (struct devdrv_sq_sub_info *)sq_info->sq_sub;
    list_add(&stream_sub->sq_list, &sq_sub->list_stream);

    id->sq_index = sq_sub->index;

    return sq_info;
}

STATIC u32 devdrv_get_available_cq_num(struct devdrv_cce_ctrl *cce_ctrl)
{
    u32 cq_num;

    spin_lock(&cce_ctrl->spinlock);
    cq_num = cce_ctrl->cq_num;
    spin_unlock(&cce_ctrl->spinlock);

    return cq_num;
}

struct devdrv_ts_cq_info *devdrv_get_new_cq(struct devdrv_cce_ctrl *cce_ctrl, struct devdrv_cce_context *cce_context)
{
    struct devdrv_info *dev_info = cce_ctrl->dev_info;
    struct devdrv_ts_cq_info *cq_info = NULL;
    struct devdrv_cq_sub_info *cq_sub = NULL;
    u32 tsid = cce_ctrl->tsid;
    phys_addr_t info_mem_addr;
    u32 cq_num;
    int ret;

    if (!list_empty_careful(&cce_context->ts_context[tsid].cq_list)) {
        devdrv_drv_err("[dev_id=%u]:context already has a cq\n", cce_ctrl->devid);
        return NULL;
    }

    cq_num = devdrv_get_available_cq_num(cce_ctrl);
    if (cq_num == 0) {
        devdrv_drv_debug("NO cq, try to get cq from opposite side!!\n");
        ret = dev_info->drv_ops->get_cq_sync(dev_info);
        if (ret) {
            devdrv_drv_err("[dev_id=%u]:get cq from opposite side failed\n", cce_ctrl->devid);
            return NULL;
        }
    }

    spin_lock(&cce_ctrl->spinlock);
    cq_sub = list_first_entry(&cce_ctrl->cq_available_list, struct devdrv_cq_sub_info, list);
    list_del(&cq_sub->list);
    list_add(&cq_sub->list, &cce_context->ts_context[tsid].cq_list);

    cq_sub->cce_context = cce_context;
    cce_ctrl->cq_num--;
    cce_context->ts_context[tsid].cq_num++;
    info_mem_addr = cce_ctrl->mem_info[DEVDRV_INFO_MEM].virt_addr;
    spin_unlock(&cce_ctrl->spinlock);

    cq_info = devdrv_calc_cq_info(info_mem_addr, cq_sub->index);

    return cq_info;
}

void devdrv_free_cq(struct devdrv_cce_ctrl *cce_ctrl, struct devdrv_cce_context *cce_context,
                    struct devdrv_ts_cq_info *cq_info)
{
    struct devdrv_cq_sub_info *cq_sub = NULL;
    unsigned long flags;
    u32 tsid = cce_ctrl->tsid;

    if (list_empty_careful(&cce_context->ts_context[tsid].cq_list)) {
        devdrv_drv_err("[dev_id=%u]:cce_context cq_list empty.\n", cce_ctrl->devid);
        return;
    }

    cq_sub = list_first_entry(&cce_context->ts_context[tsid].cq_list, struct devdrv_cq_sub_info, list);
    if (cq_sub->index != cq_info->index) {
        return;
    }

    spin_lock(&cce_ctrl->spinlock);
    if (list_empty_careful(&cq_sub->list_sq)) {
        list_del(&cq_sub->list);
        list_add(&cq_sub->list, &cce_ctrl->cq_available_list);

        cce_context->ts_context[tsid].receive_count += cq_info->receive_count;

        cce_ctrl->cq_num++;
        cce_context->ts_context[tsid].cq_num--;

        spin_lock_irqsave(&cq_sub->spinlock, flags);
        cq_sub->cce_context = NULL;
        spin_unlock_irqrestore(&cq_sub->spinlock, flags);
        cq_info->head = 0;
        cq_info->tail = 0;
        cq_info->count_report = 0;
        cq_info->stream_num = 0;
        cq_info->receive_count = 0;

        cq_info->slot_size = DEVDRV_CQ_SLOT_SIZE;

        cq_sub->uio = NULL;
        cq_info->uio_addr = NULL;
        cq_info->uio_num = DEVDRV_INVALID_FD_OR_NUM;
        cq_info->uio_map = DEVDRV_SQ_CQ_MAP;
        cq_info->uio_fd = DEVDRV_INVALID_FD_OR_NUM;
    }
    spin_unlock(&cce_ctrl->spinlock);
}

struct devdrv_ts_cq_info *devdrv_get_cq_exist(const struct devdrv_cce_ctrl *cce_ctrl,
                                              const struct devdrv_cce_context *cce_context)
{
    struct devdrv_cq_sub_info *cq_sub = NULL;
    phys_addr_t info_mem_addr;
    u32 tsid = cce_ctrl->tsid;

    if (list_empty_careful(&cce_context->ts_context[tsid].cq_list))
        return NULL;
    info_mem_addr = cce_ctrl->mem_info[DEVDRV_INFO_MEM].virt_addr;
    cq_sub = list_first_entry(&cce_context->ts_context[tsid].cq_list, struct devdrv_cq_sub_info, list);
    return devdrv_calc_cq_info(info_mem_addr, cq_sub->index);
}

STATIC struct devdrv_ts_sq_info *devdrv_get_new_sq(struct devdrv_cce_ctrl *cce_ctrl,
                                                   struct devdrv_cce_context *cce_context,
                                                   struct devdrv_stream_info *id)
{
    struct devdrv_stream_sub_info *stream_sub = NULL;
    struct devdrv_sq_sub_info *sq_sub = NULL;
    phys_addr_t info_mem_addr;
    u32 tsid = cce_ctrl->tsid;

    sq_sub = list_first_entry(&cce_ctrl->sq_available_list, struct devdrv_sq_sub_info, list);

    stream_sub = (struct devdrv_stream_sub_info *)id->stream_sub;
    list_del(&sq_sub->list);
    list_add(&sq_sub->list, &cce_context->ts_context[tsid].sq_list);
    list_add(&stream_sub->sq_list, &sq_sub->list_stream);
    cce_ctrl->sq_num--;
    cce_context->ts_context[tsid].sq_num++;
    info_mem_addr = cce_ctrl->mem_info[DEVDRV_INFO_MEM].virt_addr;

    id->sq_index = sq_sub->index;
    return devdrv_calc_sq_info(info_mem_addr, id->sq_index);
}

int devdrv_cqsq_alloc(struct devdrv_cce_ctrl *cce_ctrl, struct devdrv_cce_context *cce_context,
                      struct devdrv_stream_info *id)
{
    struct devdrv_info *dev_info = cce_ctrl->dev_info;
    struct devdrv_sq_sub_info *sq_sub = NULL;
    struct devdrv_cq_sub_info *cq_sub = NULL;
    struct devdrv_ts_sq_info *sq = NULL;
    struct devdrv_ts_cq_info *cq = NULL;
    int ret = 0;
    u32 tsid = cce_ctrl->tsid;

sq_alloc_retry:
    spin_lock(&cce_ctrl->spinlock);
    if (cce_ctrl->sq_num > 0) {
        if (cce_ctrl->sq_num > DEVDRV_SQ_FLOOR) {
            cq = devdrv_get_cq_exist(cce_ctrl, cce_context);
            if (cq == NULL) {
                ret = -EINVAL;
                goto out;
            }
            id->cq_index = cq->index;
            sq = devdrv_get_new_sq(cce_ctrl, cce_context, id);
            sq_sub = (struct devdrv_sq_sub_info *)sq->sq_sub;
            cq_sub = (struct devdrv_cq_sub_info *)cq->cq_sub;
            list_add(&sq_sub->list_cq, &cq_sub->list_sq);
            cq->stream_num++;
            sq->stream_num++;
        } else {
            cq = devdrv_get_cq_exist(cce_ctrl, cce_context);
            if (cq == NULL) {
                ret = -EINVAL;
                goto out;
            }
            id->cq_index = cq->index;
            if (cce_context->ts_context[tsid].sq_num > 0) {
                sq = devdrv_get_sq_evenly(cce_ctrl, cce_context, id);
                cq->stream_num++;
                sq->stream_num++;
            } else {
                sq = devdrv_get_new_sq(cce_ctrl, cce_context, id);
                sq_sub = (struct devdrv_sq_sub_info *)sq->sq_sub;
                cq_sub = (struct devdrv_cq_sub_info *)cq->cq_sub;
                list_add(&sq_sub->list_cq, &cq_sub->list_sq);
                cq->stream_num++;
                sq->stream_num++;
            }
        }
    } else {
        if (cce_context->ts_context[tsid].sq_num > 0) {
            cq = devdrv_get_cq_exist(cce_ctrl, cce_context);
            if (cq == NULL) {
                ret = -EINVAL;
                goto out;
            }
            id->cq_index = cq->index;
            sq = devdrv_get_sq_evenly(cce_ctrl, cce_context, id);
            cq->stream_num++;
            sq->stream_num++;
            spin_unlock(&cce_ctrl->spinlock);
            return 0;
        } else {
            spin_unlock(&cce_ctrl->spinlock);
            devdrv_drv_warn("no sq available !!!!!\n");
            ret = dev_info->drv_ops->get_sq_sync(dev_info);
            if (ret) {
                devdrv_drv_err("[dev_id=%u]:get sq from opposite side failed !!!!!\n", cce_ctrl->devid);
                return -EINVAL;
            }
            goto sq_alloc_retry;
        }
    }

out:
    spin_unlock(&cce_ctrl->spinlock);
    return ret;
}

void devdrv_cqsq_free_id(struct devdrv_cce_ctrl *cce_ctrl, struct devdrv_cce_context *cce_context, int sq_index,
                         int cq_index)
{
    struct devdrv_sq_sub_info *sq_sub_tmp = NULL;
    struct devdrv_cq_sub_info *cq_sub_tmp = NULL;
    struct devdrv_ts_sq_info *sq_tmp = NULL;
    struct devdrv_ts_cq_info *cq_tmp = NULL;
    phys_addr_t info_mem_addr;
    u32 tsid = cce_ctrl->tsid;

    info_mem_addr = cce_ctrl->mem_info[DEVDRV_INFO_MEM].virt_addr;

    sq_tmp = devdrv_calc_sq_info(info_mem_addr, sq_index);
    cq_tmp = devdrv_calc_cq_info(info_mem_addr, cq_index);
    sq_sub_tmp = (struct devdrv_sq_sub_info *)sq_tmp->sq_sub;
    cq_sub_tmp = (struct devdrv_cq_sub_info *)cq_tmp->cq_sub;

    if (sq_tmp->stream_num > 0)
        sq_tmp->stream_num--;
    if (cq_tmp->stream_num > 0)
        cq_tmp->stream_num--;

    if (list_empty_careful(&sq_sub_tmp->list_stream)) {
        list_del(&sq_sub_tmp->list_cq);
        list_del(&sq_sub_tmp->list);
        list_add(&sq_sub_tmp->list, &cce_ctrl->sq_available_list);

        cce_context->ts_context[tsid].send_count += sq_tmp->send_count;

        cce_ctrl->sq_num++;
        cce_context->ts_context[tsid].sq_num--;

        sq_tmp->head = 0;
        sq_tmp->tail = 0;
        sq_tmp->credit = DEVDRV_MAX_SQ_DEPTH - 1;
        sq_tmp->stream_num = 0;
        sq_tmp->send_count = 0;
        sq_tmp->uio_addr = NULL;
        sq_tmp->uio_num = DEVDRV_INVALID_FD_OR_NUM;
        sq_tmp->uio_map = DEVDRV_SQ_CQ_MAP;
        sq_tmp->uio_fd = DEVDRV_INVALID_FD_OR_NUM;
    }
}

int devdrv_mailbox_cqsq_inform(struct devdrv_mailbox *mailbox, struct devdrv_cce_context *cce_context, u16 cmd,
                               u16 sq_index, u64 sq_addr, u16 cq_index, u64 cq_addr, u16 stream_id)
{
    struct devdrv_mailbox_cqsq *cqsq = NULL;
    struct devdrv_cce_ctrl *cce_ctrl = NULL;
    struct devdrv_ts_cq_info *cq_info = NULL;
    phys_addr_t info_mem_addr;
    int result = 0;
    u32 tsid;
    u32 len;
    int ret;

    cce_ctrl = container_of(mailbox, struct devdrv_cce_ctrl, mailbox);
    tsid = cce_ctrl->tsid;

    if ((devdrv_is_ts_work(&cce_ctrl->dev_info->ts_mng[tsid]) == 0) || devdrv_is_low_power(cce_ctrl)) {
        devdrv_drv_err("[dev_id=%u]:TS is not working.\n", cce_ctrl->devid);
        return -ENODEV;
    }

    cqsq = kzalloc(sizeof(struct devdrv_mailbox_cqsq), GFP_KERNEL);
    if (cqsq == NULL) {
        devdrv_drv_err("[dev_id=%u]:kmalloc failed.\n", cce_ctrl->devid);
        return -ENOMEM;
    }

    /* add message header */
    cqsq->valid = DEVDRV_MAILBOX_MESSAGE_VALID;
    cqsq->cmd_type = cmd;
    cqsq->result = 0;

    /* add payload */
    cqsq->sq_index = sq_index;
    cqsq->sq_addr = sq_addr;
    cqsq->cq0_index = cq_index;
    cqsq->cq0_addr = cq_addr;
    cqsq->stream_id = stream_id;
    cqsq->plat_type = cce_ctrl->dev_info->plat_type;

    if (cq_index < DEVDRV_MAX_CQ_NUM) {
        info_mem_addr = cce_ctrl->mem_info[DEVDRV_INFO_MEM].virt_addr;
        cq_info = devdrv_calc_cq_info(info_mem_addr, cq_index);
        cqsq->cq_slot_size = cq_info->slot_size;
    }

    devdrv_drv_debug("create calculate cqsq,plat: %d, "
                     "stream_id: %d, cq: %d, cq addr: %pK, sq: %d, sq addr: %pK.\n",
                     cce_ctrl->dev_info->plat_type, stream_id, cq_index, (void *)(uintptr_t)cq_addr, sq_index,
                     (void *)(uintptr_t)sq_addr);

    len = sizeof(struct devdrv_mailbox_cqsq);
    ret = devdrv_mailbox_kernel_sync_no_feedback(mailbox, (u8 *)cqsq, len, &result);
    kfree(cqsq);
    cqsq = NULL;

    if (ret == 0)
        ret = result;

    return ret;
}

int devdrv_alloc_sq_msg_chan(struct devdrv_info *dev_info, struct devdrv_manager_msg_resource_id *dev_msg_resource_id)
{
    struct devdrv_sq_sub_info *sq_sub = NULL;
    struct devdrv_cce_ctrl *cce_ctrl = NULL;
    u32 req_id_num;
    u32 i;
    u32 j = 0;
    u32 tsid = 0;

    if ((dev_info == NULL) || (dev_msg_resource_id == NULL) || (dev_info->cce_ctrl[tsid] == NULL) ||
        (dev_msg_resource_id->req_id_num > DEVDRV_MANAGER_MSG_ID_NUM_MAX)) {
        devdrv_drv_err("info input err,info=%pK, id=%pK.\n", dev_info, dev_msg_resource_id);
        return -EINVAL;
    }
    cce_ctrl = dev_info->cce_ctrl[tsid];
    req_id_num = dev_msg_resource_id->req_id_num;

    spin_lock(&cce_ctrl->spinlock);
    if (cce_ctrl->sq_num < req_id_num) {
        spin_unlock(&cce_ctrl->spinlock);
        devdrv_drv_err("sq_num = %u, req_id_num = %u, no enough sq\n", cce_ctrl->sq_num, req_id_num);
        return -ENODEV;
    }

    for (i = 0; i < req_id_num; i++) {
        sq_sub = list_first_entry(&cce_ctrl->sq_available_list, struct devdrv_sq_sub_info, list);
        list_del(&sq_sub->list);
        cce_ctrl->sq_num--;
        dev_msg_resource_id->id[j++] = sq_sub->index;
        dev_msg_resource_id->ret_id_num++;
        devdrv_drv_debug("sq_id = %u, sq_num = %u, ret_id_num = %u\n", sq_sub->index, cce_ctrl->sq_num,
                         dev_msg_resource_id->ret_id_num);
    }
    spin_unlock(&cce_ctrl->spinlock);

    return 0;
}
EXPORT_SYMBOL(devdrv_alloc_sq_msg_chan);

int devdrv_alloc_cq_msg_chan(struct devdrv_info *dev_info, struct devdrv_manager_msg_resource_id *dev_msg_resource_id)
{
    struct devdrv_cq_sub_info *cq_sub = NULL;
    struct devdrv_cce_ctrl *cce_ctrl = NULL;
    u32 req_id_num;
    u32 i;
    u32 j = 0;
    u32 tsid = 0;

    if ((dev_info == NULL) || (dev_msg_resource_id == NULL) || (dev_info->cce_ctrl[tsid] == NULL) ||
        (dev_msg_resource_id->req_id_num > DEVDRV_MANAGER_MSG_ID_NUM_MAX)) {
        devdrv_drv_err("info input err, info=%pK, id = %pK.\n", dev_info, dev_msg_resource_id);
        return -EINVAL;
    }

    cce_ctrl = dev_info->cce_ctrl[tsid];
    req_id_num = dev_msg_resource_id->req_id_num;

    spin_lock(&cce_ctrl->spinlock);
    if (cce_ctrl->cq_num < req_id_num) {
        spin_unlock(&cce_ctrl->spinlock);
        devdrv_drv_err("[dev_id=%u]:cq_num %u, req_id_num = %u, no enough cq\n", cce_ctrl->devid, cce_ctrl->cq_num,
                       req_id_num);
        return -ENODEV;
    }

    for (i = 0; i < req_id_num; i++) {
        cq_sub = list_first_entry(&cce_ctrl->cq_available_list, struct devdrv_cq_sub_info, list);
        list_del(&cq_sub->list);
        cce_ctrl->cq_num--;
        dev_msg_resource_id->id[j++] = cq_sub->index;
        dev_msg_resource_id->ret_id_num++;

        devdrv_drv_debug("cq_id = %u, cq_num = %u, ret_id_num = %u\n", cq_sub->index, cce_ctrl->cq_num,
                         dev_msg_resource_id->ret_id_num);
    }
    spin_unlock(&cce_ctrl->spinlock);

    return 0;
}
EXPORT_SYMBOL(devdrv_alloc_cq_msg_chan);

int devdrv_add_sq_msg_chan(struct devdrv_info *dev_info, struct devdrv_manager_msg_resource_id *dev_msg_resource_id)
{
    struct devdrv_sq_sub_info *sq_sub_info = NULL;
    struct devdrv_ts_sq_info *sq_info = NULL;
    struct devdrv_cce_ctrl *cce_ctrl = NULL;
    phys_addr_t info_mem_addr;
    u32 tsid = 0;
    u32 i;

    if ((dev_info == NULL) || (dev_msg_resource_id == NULL) || (dev_info->cce_ctrl[tsid] == NULL) ||
        (dev_msg_resource_id->ret_id_num > DEVDRV_MANAGER_MSG_ID_NUM_MAX)) {
        devdrv_drv_err("info input err,info=%pK, id=%pK.\n", dev_info, dev_msg_resource_id);
        return -EINVAL;
    }
    cce_ctrl = dev_info->cce_ctrl[tsid];
    info_mem_addr = cce_ctrl->mem_info[DEVDRV_INFO_MEM].virt_addr;

    spin_lock(&cce_ctrl->spinlock);
    for (i = 0; i < dev_msg_resource_id->ret_id_num; i++) {
        sq_info = devdrv_calc_sq_info(info_mem_addr, dev_msg_resource_id->id[i]);
        sq_sub_info = sq_info->sq_sub;
        list_add_tail(&sq_sub_info->list, &cce_ctrl->sq_available_list);
        cce_ctrl->sq_num++;

        devdrv_drv_debug("sq_id = %u, sq_num = %u, ret_id_num = %u\n", sq_sub_info->index, cce_ctrl->sq_num,
                         dev_msg_resource_id->ret_id_num);
    }
    spin_unlock(&cce_ctrl->spinlock);

    return 0;
}
EXPORT_SYMBOL(devdrv_add_sq_msg_chan);

int devdrv_add_cq_msg_chan(struct devdrv_info *dev_info, struct devdrv_manager_msg_resource_id *dev_msg_resource_id)
{
    struct devdrv_cq_sub_info *cq_sub_info = NULL;
    struct devdrv_ts_cq_info *cq_info = NULL;
    struct devdrv_cce_ctrl *cce_ctrl = NULL;
    phys_addr_t info_mem_addr;
    u32 tsid = 0;
    u32 i;

    if ((dev_info == NULL) || (dev_msg_resource_id == NULL) || (dev_info->cce_ctrl[tsid] == NULL) ||
        (dev_msg_resource_id->ret_id_num > DEVDRV_MANAGER_MSG_ID_NUM_MAX)) {
        devdrv_drv_err("info input err,info=%pK, id=%pK.\n", dev_info, dev_msg_resource_id);
        return -EINVAL;
    }
    cce_ctrl = dev_info->cce_ctrl[tsid];
    info_mem_addr = cce_ctrl->mem_info[DEVDRV_INFO_MEM].virt_addr;

    spin_lock(&cce_ctrl->spinlock);
    for (i = 0; i < dev_msg_resource_id->ret_id_num; i++) {
        cq_info = devdrv_calc_cq_info(info_mem_addr, dev_msg_resource_id->id[i]);
        cq_sub_info = cq_info->cq_sub;
        list_add_tail(&cq_sub_info->list, &cce_ctrl->cq_available_list);
        cce_ctrl->cq_num++;

        devdrv_drv_debug("cq_id = %u, sq_num = %u, ret_id_num = %u\n", cq_sub_info->index, cce_ctrl->cq_num,
                         dev_msg_resource_id->ret_id_num);
    }
    spin_unlock(&cce_ctrl->spinlock);

    return 0;
}
EXPORT_SYMBOL(devdrv_add_cq_msg_chan);
