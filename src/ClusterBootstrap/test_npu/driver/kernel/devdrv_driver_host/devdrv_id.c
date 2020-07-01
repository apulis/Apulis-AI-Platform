/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2012-2019. All rights reserved.
 * Description:
 * Author: huawei
 * Create: 2019-10-15
 */

#include <linux/errno.h>
#include <linux/kernel.h>
#include <linux/list.h>
#include <linux/gfp.h>
#include <linux/platform_device.h>
#include <linux/slab.h>
#include <linux/spinlock.h>
#include <linux/vmalloc.h>
#include <linux/uaccess.h>

#include "devdrv_id.h"
#include "devdrv_cqsq.h"
#include "devdrv_devinit.h"

STATIC struct mutex devdrv_uio_mutex;

STATIC u32 devdrv_get_available_model_num(struct devdrv_cce_ctrl *cce_ctrl);
STATIC struct devdrv_model_id_info *devdrv_get_one_model_id(struct devdrv_cce_ctrl *cce_ctrl);
STATIC struct devdrv_model_id_info *devdrv_find_one_model_id(struct devdrv_cce_ctrl *cce_ctrl,
                                                             struct devdrv_cce_context *cce_context, int id);
STATIC struct devdrv_notify_id_info *devdrv_find_one_notify_id(struct devdrv_cce_ctrl *cce_ctrl,
                                                               struct devdrv_cce_context *cce_context, int id);
STATIC u32 devdrv_get_available_task_num(struct devdrv_cce_ctrl *cce_ctrl);
STATIC struct devdrv_taskpool_id_info *devdrv_get_one_task_id(struct devdrv_cce_ctrl *cce_ctrl);
STATIC struct devdrv_taskpool_id_info *devdrv_find_one_task_id(struct devdrv_cce_ctrl *cce_ctrl,
                                                               struct devdrv_cce_context *cce_context, int id);

int devdrv_task_id_init(struct devdrv_info *dev_info, u32 tsid, u32 task_id_num)
{
    struct devdrv_taskpool_id_info *task_info = NULL;
    struct devdrv_id_data *id_data = NULL;
    struct devdrv_cce_ctrl *cce_ctrl = NULL;
    unsigned long size;
    u32 i;

    if ((tsid >= DEVDRV_MAX_TS_NUM) || (dev_info == NULL) || (dev_info->cce_ctrl[tsid] == NULL)) {
        devdrv_drv_err("param is invalid.\n");
        return -ENOMEM;
    }
    id_data = devdrv_get_id_data(dev_info->pdata, tsid);
    cce_ctrl = dev_info->cce_ctrl[tsid];

    spin_lock(&cce_ctrl->task_spinlock);
    if (!list_empty_careful(&cce_ctrl->task_available_list)) {
        spin_unlock(&cce_ctrl->task_spinlock);
        devdrv_drv_err("[dev_id = %u]:available task list is no empty.\n", dev_info->dev_id);
        return -EEXIST;
    }
    spin_unlock(&cce_ctrl->task_spinlock);
    size = (long)(unsigned)sizeof(struct devdrv_taskpool_id_info) * task_id_num;
    task_info = vzalloc(size);
    if (task_info == NULL) {
        devdrv_drv_err("[dev_id = %u]:task_info vmalloc failed.\n", dev_info->dev_id);
        return -ENOMEM;
    }

    spin_lock(&cce_ctrl->task_spinlock);
    cce_ctrl->task_id_num = 0;
    for (i = 0; i < task_id_num; i++) {
        task_info[i].id = i;
        task_info[i].devid = dev_info->dev_id;
        task_info[i].cce_context = NULL;
        if ((i < id_data->max_task_id) && (i >= id_data->min_task_id)) {
            list_add_tail(&task_info[i].list, &cce_ctrl->task_available_list);
            cce_ctrl->task_id_num++;
        } else {
            INIT_LIST_HEAD(&task_info[i].list);
        }
    }
    cce_ctrl->task_addr = task_info;
    spin_unlock(&cce_ctrl->task_spinlock);

    return 0;
}

int devdrv_model_id_init(struct devdrv_info *dev_info, u32 tsid, u32 model_id_num)
{
    struct devdrv_model_id_info *model_info = NULL;
    struct devdrv_cce_ctrl *cce_ctrl = NULL;
    struct devdrv_id_data *id_data = NULL;
    unsigned long size;
    u32 i;

    if ((tsid >= DEVDRV_MAX_TS_NUM) || (dev_info == NULL) || (dev_info->cce_ctrl[tsid] == NULL)) {
        devdrv_drv_err("param is invalid\n");
        return -ENOMEM;
    }
    id_data = devdrv_get_id_data(dev_info->pdata, tsid);
    cce_ctrl = dev_info->cce_ctrl[tsid];

    spin_lock(&cce_ctrl->model_spinlock);
    if (!list_empty_careful(&cce_ctrl->model_available_list)) {
        spin_unlock(&cce_ctrl->model_spinlock);
        devdrv_drv_err("[dev_id = %u]:available model list is not empty.\n", dev_info->dev_id);
        return -EEXIST;
    }
    spin_unlock(&cce_ctrl->model_spinlock);

    size = (long)(unsigned)sizeof(struct devdrv_model_id_info) * model_id_num;
    model_info = vmalloc(size);
    if (model_info == NULL) {
        devdrv_drv_err("[dev_id = %u]:model_info vmalloc failed\n", dev_info->dev_id);
        return -ENOMEM;
    }

    spin_lock(&cce_ctrl->model_spinlock);
    cce_ctrl->model_id_num = 0;
    for (i = 0; i < model_id_num; i++) {
        model_info[i].id = i;
        model_info[i].devid = dev_info->dev_id;
        model_info[i].cce_context = NULL;
        if ((i < id_data->max_model_id) && (i >= id_data->min_model_id)) {
            list_add_tail(&model_info[i].list, &cce_ctrl->model_available_list);
            cce_ctrl->model_id_num++;
        } else {
            INIT_LIST_HEAD(&model_info[i].list);
        }
    }
    cce_ctrl->model_addr = model_info;
    spin_unlock(&cce_ctrl->model_spinlock);

    return 0;
}

void devdrv_task_id_destroy(struct devdrv_cce_ctrl *cce_ctrl)
{
    struct list_head *pos = NULL, *n = NULL;

    if ((cce_ctrl == NULL) || (cce_ctrl->task_id_num == 0)) {
        devdrv_drv_err("invalid input argument.\n");
        return;
    }
    spin_lock(&cce_ctrl->task_spinlock);
    if (!list_empty_careful(&cce_ctrl->task_available_list)) {
        list_for_each_safe(pos, n, &cce_ctrl->task_available_list)
        {
            cce_ctrl->task_id_num--;
            list_del(pos);
        }
    }
    spin_unlock(&cce_ctrl->task_spinlock);
    vfree(cce_ctrl->task_addr);
    cce_ctrl->task_addr = NULL;

    return;
}

/* must call after all id are released into available list */
/* release all id in devdrv_cce_ctrl available list */
void devdrv_model_id_destroy(struct devdrv_cce_ctrl *cce_ctrl)
{
    struct list_head *pos = NULL, *n = NULL;

    if ((cce_ctrl == NULL) || (cce_ctrl->model_id_num == 0)) {
        devdrv_drv_err("invalid input argument.\n");
        return;
    }

    spin_lock(&cce_ctrl->model_spinlock);
    if (!list_empty_careful(&cce_ctrl->model_available_list)) {
        list_for_each_safe(pos, n, &cce_ctrl->model_available_list)
        {
            cce_ctrl->model_id_num--;
            list_del(pos);
        }
    }
    spin_unlock(&cce_ctrl->model_spinlock);
    vfree(cce_ctrl->model_addr);
    cce_ctrl->model_addr = NULL;

    return;
}

int devdrv_alloc_task_id(struct devdrv_cce_ctrl *cce_ctrl, struct devdrv_cce_context *cce_context)
{
    struct devdrv_info *dev_info = cce_ctrl->dev_info;
    struct devdrv_taskpool_id_info *task_info = NULL;
    u32 task_num;
    int ret;

    mutex_lock(&cce_ctrl->task_mutex_t);
    task_num = devdrv_get_available_task_num(cce_ctrl);
    if (task_num == 0) {
        ret = dev_info->drv_ops->get_task_sync(cce_ctrl);
        if (ret) {
            mutex_unlock(&cce_ctrl->task_mutex_t);
            devdrv_drv_err("[dev_id=%u]:try to get task id from opposite side failed\n", cce_ctrl->devid);
            return DEVDRV_MAX_TASK_ID;
        }
    }
    mutex_unlock(&cce_ctrl->task_mutex_t);

    spin_lock(&cce_ctrl->task_spinlock);
    task_info = devdrv_get_one_task_id(cce_ctrl);
    if (task_info == NULL) {
        spin_unlock(&cce_ctrl->task_spinlock);
        devdrv_drv_err("[dev_id=%u]:devdrv_get_one_task_id return NULL.\n", cce_ctrl->devid);
        return DEVDRV_MAX_TASK_ID;
    }
    task_info->cce_context = cce_context;
    list_add(&task_info->list, &cce_context->ts_context[cce_ctrl->tsid].task_list);
    cce_context->ts_context[cce_ctrl->tsid].task_id_num++;
    spin_unlock(&cce_ctrl->task_spinlock);
    devdrv_drv_debug("got task id %d, cce_ctrl->task_id_num = %d\n", task_info->id, cce_ctrl->task_id_num);
    return task_info->id;
}

int devdrv_alloc_model_id(struct devdrv_cce_ctrl *cce_ctrl, struct devdrv_cce_context *cce_context)
{
    struct devdrv_info *dev_info = cce_ctrl->dev_info;
    struct devdrv_model_id_info *model_info = NULL;
    u32 model_num;
    int ret;

    mutex_lock(&cce_ctrl->model_mutex_t);
    model_num = devdrv_get_available_model_num(cce_ctrl);
    if (model_num == 0) {
        devdrv_drv_debug("no model, try to get model from opposite side, "
                         "cce_ctrl->model_id_num = %d\n",
                         cce_ctrl->model_id_num);
        ret = dev_info->drv_ops->get_model_sync(cce_ctrl);
        if (ret) {
            mutex_unlock(&cce_ctrl->model_mutex_t);
            devdrv_drv_err("[dev_id=%u]:try to get model id from opposite side failed\n", cce_ctrl->devid);
            return DEVDRV_MAX_MODEL_ID;
        }

        devdrv_drv_debug("got model id, cce_ctrl->model_id_num = %d\n", cce_ctrl->model_id_num);
    }
    mutex_unlock(&cce_ctrl->model_mutex_t);

    spin_lock(&cce_ctrl->model_spinlock);
    model_info = devdrv_get_one_model_id(cce_ctrl);
    if (model_info == NULL) {
        spin_unlock(&cce_ctrl->model_spinlock);
        devdrv_drv_err("[dev_id=%u]:devdrv_get_one_model_id return NULL.\n", cce_ctrl->devid);
        return DEVDRV_MAX_MODEL_ID;
    }
    model_info->cce_context = cce_context;
    list_add(&model_info->list, &cce_context->ts_context[cce_ctrl->tsid].model_list);
    cce_context->ts_context[cce_ctrl->tsid].model_id_num++;
    spin_unlock(&cce_ctrl->model_spinlock);

    return model_info->id;
}

int devdrv_free_task_id(struct devdrv_cce_ctrl *cce_ctrl, struct devdrv_cce_context *cce_context, int id)
{
    struct devdrv_taskpool_id_info *task_info = NULL;

    if ((id < 0) || (id >= DEVDRV_MAX_TASK_ID) || (cce_ctrl == NULL) || (cce_context == NULL)) {
        devdrv_drv_err("invalid input argument.\n");
        return -EINVAL;
    }
    spin_lock(&cce_ctrl->task_spinlock);
    task_info = devdrv_find_one_task_id(cce_ctrl, cce_context, id);
    if ((task_info == NULL) || (task_info->cce_context == NULL)) {
        spin_unlock(&cce_ctrl->task_spinlock);
        devdrv_drv_err("[dev_id=%u]:find task id failed\n", cce_ctrl->devid);
        return -ENODATA;
    }
    task_info->cce_context = NULL;
    list_del(&task_info->list);
    list_add_tail(&task_info->list, &cce_ctrl->task_available_list);
    cce_ctrl->task_id_num++;
    cce_context->ts_context[cce_ctrl->tsid].task_id_num--;
    spin_unlock(&cce_ctrl->task_spinlock);
    devdrv_drv_debug("free task id %d, cce_ctrl->task_id_num = %d\n", id, cce_ctrl->task_id_num);

    return 0;
}

int devdrv_free_model_id(struct devdrv_cce_ctrl *cce_ctrl, struct devdrv_cce_context *cce_context, int id)
{
    struct devdrv_model_id_info *model_info = NULL;

    if ((id < 0) || (id >= DEVDRV_MAX_MODEL_ID) || (cce_ctrl == NULL) || (cce_context == NULL)) {
        devdrv_drv_err("invalid input argument.\n");
        return -EINVAL;
    }
    spin_lock(&cce_ctrl->model_spinlock);
    model_info = devdrv_find_one_model_id(cce_ctrl, cce_context, id);
    if ((model_info == NULL) || (model_info->cce_context == NULL)) {
        spin_unlock(&cce_ctrl->model_spinlock);
        devdrv_drv_err("[dev_id=%u]:find model id failed\n", cce_ctrl->devid);
        return -ENODATA;
    }
    model_info->cce_context = NULL;

    list_del(&model_info->list);
    list_add(&model_info->list, &cce_ctrl->model_available_list);
    cce_ctrl->model_id_num++;
    cce_context->ts_context[cce_ctrl->tsid].model_id_num--;
    spin_unlock(&cce_ctrl->model_spinlock);

    return 0;
}

int devdrv_free_one_notify_id(struct devdrv_cce_ctrl *cce_ctrl, struct devdrv_cce_context *cce_context, int id,
                              int inform_type)
{
    struct devdrv_cce_context *context = NULL;
    struct devdrv_notify_id_info *notify_info = NULL;
    int ret;

    if ((id < 0) || (id >= DEVDRV_MAX_NOTIFY_ID) || (cce_ctrl == NULL) || (cce_context == NULL) ||
        (cce_ctrl->tsid >= DEVDRV_MAX_TS_NUM)) {
        devdrv_drv_err("invalid input argument.\n");
        return -EINVAL;
    }
    spin_lock(&cce_ctrl->notify_spinlock);
    notify_info = devdrv_find_one_notify_id(cce_ctrl, cce_context, id);
    if (notify_info == NULL) {
        spin_unlock(&cce_ctrl->notify_spinlock);
        devdrv_drv_err("[dev_id=%u]:find notify id failed\n", cce_ctrl->devid);
        return -EINVAL;
    }
    spin_lock(&notify_info->spinlock);
    context = (struct devdrv_cce_context *)notify_info->cce_context;
    if ((notify_info->id != id) || (context == NULL) || (context->pid != cce_context->pid)) {
        spin_unlock(&notify_info->spinlock);
        spin_unlock(&cce_ctrl->notify_spinlock);
        devdrv_drv_err("[dev_id=%u]:check notify info failed\n"
                       "id = %d, "
                       "notify_info->id = %d, "
                       "tmp_cce_context = %pK, "
                       "cce_context->pid = %d\n",
                       cce_ctrl->devid, id, notify_info->id, context, cce_context->pid);
        return -EINVAL;
    }
    notify_info->cce_context = NULL;

    list_del(&notify_info->list);
    list_add(&notify_info->list, &cce_ctrl->notify_available_list);
    spin_unlock(&notify_info->spinlock);
    cce_ctrl->notify_id_num++;
    cce_context->ts_context[cce_ctrl->tsid].notify_id_num--;

    if (atomic_read(&notify_info->ref) > 1) {
        /*
         * notify ref is bigger than 1 means current notify is still in use
         * just decrease notify ref and return free successed
         */
        atomic_dec(&notify_info->ref);
        spin_unlock(&cce_ctrl->notify_spinlock);
        return 0;
    }
    spin_unlock(&cce_ctrl->notify_spinlock);
    /*
     * notify ref value should be 1 here
     * try to send message to ts to reset current notify
     */
    if (inform_type == DEVDRV_NOTIFY_INFORM_TS) {
        ret = devdrv_notify_ts_msg(cce_ctrl->dev_info, cce_ctrl->tsid, notify_info->id);
        if (ret) {
            devdrv_drv_err("[dev_id=%u]:send ts notify alloc msg failed, notify_id = %d\n", cce_ctrl->devid,
                           notify_info->id);

            goto notify_ts_msg_failed;
        }
    }
    /* now we can decrease notify ref */
    atomic_dec(&notify_info->ref);
    return 0;

notify_ts_msg_failed:
    /* sent ts reset notify message failed, return notify back to current context */
    spin_lock(&cce_ctrl->notify_spinlock);
    spin_lock(&notify_info->spinlock);
    notify_info->cce_context = context;
    spin_unlock(&notify_info->spinlock);

    list_del(&notify_info->list);
    list_add(&notify_info->list, &context->ts_context[cce_ctrl->tsid].notify_list);
    cce_ctrl->notify_id_num--;
    cce_context->ts_context[cce_ctrl->tsid].notify_id_num++;
    spin_unlock(&cce_ctrl->notify_spinlock);

    return ret;
}

int devdrv_add_task_msg_chan(struct devdrv_info *dev_info, u32 tsid,
                             struct devdrv_manager_msg_resource_id *dev_msg_resource_id)
{
    struct devdrv_taskpool_id_info *task_info = NULL;
    struct devdrv_cce_ctrl *cce_ctrl = NULL;
    u32 ret_id_num;
    u16 task_id;
    u32 i;

    if ((tsid >= DEVDRV_MAX_TS_NUM) || (dev_info == NULL) || (dev_msg_resource_id == NULL)) {
        devdrv_drv_err("invalid input parameter.\n");
        return -EINVAL;
    }
    if (dev_info->cce_ctrl[tsid] == NULL) {
        devdrv_drv_err("[dev_id = %u]:invalid input parameter.\n", dev_info->dev_id);
        return -EINVAL;
    }

    cce_ctrl = dev_info->cce_ctrl[tsid];
    ret_id_num = dev_msg_resource_id->ret_id_num;

    if (ret_id_num > DEVDRV_MANAGER_MSG_ID_NUM_MAX) {
        devdrv_drv_err("[dev_id = %u]:ret_id_num = %u is invalid.\n", dev_info->dev_id, ret_id_num);
        return -ENODEV;
    }

    task_info = (struct devdrv_taskpool_id_info *)cce_ctrl->task_addr;
    spin_lock(&cce_ctrl->task_spinlock);
    for (i = 0; i < ret_id_num; i++) {
        task_id = dev_msg_resource_id->id[i];
        if (task_id >= DEVDRV_MAX_TASK_ID) {
            spin_unlock(&cce_ctrl->task_spinlock);
            devdrv_drv_err("[dev_id = %u]:task_id = %u is invalid.\n", dev_info->dev_id, task_id);
            return -ENODEV;
        }
        list_add_tail(&task_info[task_id].list, &cce_ctrl->task_available_list);
        cce_ctrl->task_id_num++;
        devdrv_drv_debug("task_id = %u, task_id_num = %u.\n", task_id, cce_ctrl->task_id_num);
    }
    spin_unlock(&cce_ctrl->task_spinlock);

    return 0;
}

int devdrv_add_model_msg_chan(struct devdrv_info *dev_info, u32 tsid,
                              struct devdrv_manager_msg_resource_id *dev_msg_resource_id)
{
    struct devdrv_model_id_info *model_info = NULL;
    struct devdrv_cce_ctrl *cce_ctrl = NULL;
    u32 ret_id_num;
    u16 model_id;
    u32 i;

    if ((tsid >= DEVDRV_MAX_TS_NUM) || (dev_info == NULL) || (dev_msg_resource_id == NULL)) {
        devdrv_drv_err("invalid input parameter.\n");
        return -EINVAL;
    }

    if (dev_info->cce_ctrl[tsid] == NULL) {
        devdrv_drv_err("[dev_id = %u]:invalid input parameter.\n", dev_info->dev_id);
        return -EINVAL;
    }

    cce_ctrl = dev_info->cce_ctrl[tsid];
    ret_id_num = dev_msg_resource_id->ret_id_num;

    if (ret_id_num > DEVDRV_MANAGER_MSG_ID_NUM_MAX) {
        devdrv_drv_err("[dev_id = %u]:ret_id_num = %u is invalid.\n", dev_info->dev_id, ret_id_num);
        return -ENODEV;
    }

    model_info = (struct devdrv_model_id_info *)cce_ctrl->model_addr;

    spin_lock(&cce_ctrl->model_spinlock);
    for (i = 0; i < ret_id_num; i++) {
        model_id = dev_msg_resource_id->id[i];
        if (model_id >= DEVDRV_MAX_MODEL_ID) {
            spin_unlock(&cce_ctrl->model_spinlock);
            devdrv_drv_err("[dev_id = %u]:model_id = %u is invalid.\n", dev_info->dev_id, model_id);
            return -ENODEV;
        }

        list_add_tail(&model_info[model_id].list, &cce_ctrl->model_available_list);
        cce_ctrl->model_id_num++;

        devdrv_drv_debug("model_id = %u, model_id_num = %u\n", model_id, cce_ctrl->model_id_num);
    }
    spin_unlock(&cce_ctrl->model_spinlock);

    return 0;
}

int devdrv_add_notify_msg_chan(struct devdrv_info *dev_info, u32 tsid,
                               struct devdrv_manager_msg_resource_id *dev_msg_resource_id)
{
    struct devdrv_notify_id_info *notify_info = NULL;
    struct devdrv_cce_ctrl *cce_ctrl = NULL;
    u32 ret_id_num;
    u16 notify_id;
    u32 i;

    if ((tsid >= DEVDRV_MAX_TS_NUM) || (dev_info == NULL) || (dev_msg_resource_id == NULL)) {
        devdrv_drv_err("invalid input parameter.\n");
        return -EINVAL;
    }

    if (dev_info->cce_ctrl[tsid] == NULL) {
        devdrv_drv_err("[dev_id = %u]:invalid input parameter.\n", dev_info->dev_id);
        return -EINVAL;
    }
    cce_ctrl = dev_info->cce_ctrl[tsid];
    ret_id_num = dev_msg_resource_id->ret_id_num;

    if (ret_id_num > DEVDRV_MANAGER_MSG_ID_NUM_MAX) {
        devdrv_drv_err("[dev_id = %u]:ret_id_num = %u is invalid.\n", dev_info->dev_id, ret_id_num);
        return -ENODEV;
    }

    notify_info = (struct devdrv_notify_id_info *)cce_ctrl->notify_addr;
    spin_lock(&cce_ctrl->notify_spinlock);
    for (i = 0; i < ret_id_num; i++) {
        notify_id = dev_msg_resource_id->id[i];
        if (notify_id >= DEVDRV_MAX_NOTIFY_ID) {
            spin_unlock(&cce_ctrl->notify_spinlock);
            devdrv_drv_err("[dev_id = %u]:notify_id = %u is invalid.\n", dev_info->dev_id, notify_id);
            return -ENODEV;
        }

        list_add_tail(&notify_info[notify_id].list, &cce_ctrl->notify_available_list);
        cce_ctrl->notify_id_num++;

        devdrv_drv_debug("model_id = %u, notify_id_num = %u.\n", notify_id, cce_ctrl->notify_id_num);
    }
    spin_unlock(&cce_ctrl->notify_spinlock);

    return 0;
}

int devdrv_alloc_task_msg_chan(struct devdrv_info *dev_info, u32 tsid,
                               struct devdrv_manager_msg_resource_id *dev_msg_resource_id)
{
    struct devdrv_taskpool_id_info *task_info = NULL;
    struct devdrv_cce_ctrl *cce_ctrl = NULL;
    u32 req_id_num;
    u32 i;
    u32 j = 0;

    if ((tsid >= DEVDRV_MAX_TS_NUM) || (dev_info == NULL) || (dev_msg_resource_id == NULL)) {
        devdrv_drv_err("invalid input parameter.\n");
        return -EINVAL;
    }

    if (dev_info->cce_ctrl[tsid] == NULL) {
        devdrv_drv_err("[dev_id = %u]:invalid input parameter.\n", dev_info->dev_id);
        return -EINVAL;
    }
    cce_ctrl = dev_info->cce_ctrl[tsid];
    req_id_num = dev_msg_resource_id->req_id_num;

    spin_lock(&cce_ctrl->task_spinlock);
    if ((cce_ctrl->task_id_num < req_id_num) || (req_id_num > DEVDRV_MANAGER_MSG_ID_NUM_MAX)) {
        spin_unlock(&cce_ctrl->task_spinlock);
        devdrv_drv_err("[dev_id = %u]:no enough task id, task_id_num = %u, req_id_num = %u.\n", dev_info->dev_id,
                       cce_ctrl->task_id_num, req_id_num);
        return -EINVAL;
    }
    for (i = 0; i < req_id_num; i++) {
        task_info = list_first_entry(&cce_ctrl->task_available_list, struct devdrv_taskpool_id_info, list);
        list_del(&task_info->list);
        cce_ctrl->task_id_num--;
        dev_msg_resource_id->id[j++] = task_info->id;
        dev_msg_resource_id->ret_id_num++;
        devdrv_drv_debug("task id = %d, j = %u, "
                         "ret_id_num = %u, task_id_num = %u.\n",
                         task_info->id, j, dev_msg_resource_id->ret_id_num, cce_ctrl->task_id_num);
    }
    spin_unlock(&cce_ctrl->task_spinlock);

    return 0;
}

int devdrv_alloc_model_msg_chan(struct devdrv_info *dev_info, u32 tsid,
                                struct devdrv_manager_msg_resource_id *dev_msg_resource_id)
{
    struct devdrv_cce_ctrl *cce_ctrl = NULL;
    struct devdrv_model_id_info *model_info = NULL;
    u32 req_id_num;
    u32 i;
    u32 j = 0;

    if ((tsid >= DEVDRV_MAX_TS_NUM) || (dev_info == NULL) || (dev_msg_resource_id == NULL)) {
        devdrv_drv_err("invalid input parameter.\n");
        return -EINVAL;
    }

    if (dev_info->cce_ctrl[tsid] == NULL) {
        devdrv_drv_err("[dev_id = %u]:invalid input parameter.\n", dev_info->dev_id);
        return -EINVAL;
    }

    cce_ctrl = dev_info->cce_ctrl[tsid];
    req_id_num = dev_msg_resource_id->req_id_num;

    spin_lock(&cce_ctrl->model_spinlock);
    if ((cce_ctrl->model_id_num < req_id_num) || (req_id_num > DEVDRV_MANAGER_MSG_ID_NUM_MAX)) {
        spin_unlock(&cce_ctrl->model_spinlock);
        devdrv_drv_err("[dev_id = %u]:no enough model id, model_id_num = %u, req_id_num = %u.\n", dev_info->dev_id,
                       cce_ctrl->model_id_num, req_id_num);
        return -EINVAL;
    }

    for (i = 0; i < req_id_num; i++) {
        model_info = list_first_entry(&cce_ctrl->model_available_list, struct devdrv_model_id_info, list);
        list_del(&model_info->list);
        cce_ctrl->model_id_num--;
        dev_msg_resource_id->id[j++] = model_info->id;
        dev_msg_resource_id->ret_id_num++;

        devdrv_drv_debug("model id = %d, j = %u, "
                         "ret_id_num = %u , model_id_num = %u\n",
                         model_info->id, j, dev_msg_resource_id->ret_id_num, cce_ctrl->model_id_num);
    }
    spin_unlock(&cce_ctrl->model_spinlock);

    return 0;
}

int devdrv_alloc_notify_msg_chan(struct devdrv_info *dev_info, u32 tsid,
                                 struct devdrv_manager_msg_resource_id *dev_msg_resource_id)
{
    struct devdrv_notify_id_info *notify_info = NULL;
    struct devdrv_cce_ctrl *cce_ctrl = NULL;
    u32 req_id_num;
    u32 i;
    u32 j = 0;

    if ((tsid >= DEVDRV_MAX_TS_NUM) || (dev_info == NULL) || (dev_msg_resource_id == NULL)) {
        devdrv_drv_err("invalid input parameter.\n");
        return -EINVAL;
    }

    if (dev_info->cce_ctrl[tsid] == NULL) {
        devdrv_drv_err("[dev_id = %u]:invalid input parameter.\n", dev_info->dev_id);
        return -EINVAL;
    }

    cce_ctrl = dev_info->cce_ctrl[tsid];
    req_id_num = dev_msg_resource_id->req_id_num;

    spin_lock(&cce_ctrl->notify_spinlock);
    if ((cce_ctrl->notify_id_num < req_id_num) || (req_id_num > DEVDRV_MANAGER_MSG_ID_NUM_MAX)) {
        spin_unlock(&cce_ctrl->notify_spinlock);
        devdrv_drv_err("[dev_id = %u]:no enough model id, notify_id_num = %u, req_id_num = %u.\n", dev_info->dev_id,
                       cce_ctrl->notify_id_num, req_id_num);
        return -EINVAL;
    }

    for (i = 0; i < req_id_num; i++) {
        notify_info = list_first_entry(&cce_ctrl->notify_available_list, struct devdrv_notify_id_info, list);
        list_del(&notify_info->list);
        cce_ctrl->notify_id_num--;
        dev_msg_resource_id->id[j++] = notify_info->id;
        dev_msg_resource_id->ret_id_num++;

        devdrv_drv_debug("model id = %d, j = %u, ret_id_num = %u , notify_id_num = %u.\n", notify_info->id, j,
                         dev_msg_resource_id->ret_id_num, cce_ctrl->notify_id_num);
    }
    spin_unlock(&cce_ctrl->notify_spinlock);

    return 0;
}

int devdrv_notify_id_init(struct devdrv_info *dev_info, u32 tsid, u32 notify_id_num)
{
    struct devdrv_notify_id_info *notify_info = NULL;
    struct devdrv_cce_ctrl *cce_ctrl = NULL;
    struct devdrv_id_data *id_data = NULL;
    unsigned long size;
    u32 i;

    if ((tsid >= DEVDRV_MAX_TS_NUM) || (dev_info == NULL) || (dev_info->cce_ctrl[tsid] == NULL)) {
        devdrv_drv_err("null parameters\n");
        return -ENOMEM;
    }
    id_data = devdrv_get_id_data(dev_info->pdata, tsid);
    cce_ctrl = dev_info->cce_ctrl[tsid];
    spin_lock(&cce_ctrl->notify_spinlock);
    if (!list_empty_careful(&cce_ctrl->notify_available_list)) {
        spin_unlock(&cce_ctrl->notify_spinlock);
        devdrv_drv_err("[dev_id = %u]:available notify list is not empty.\n", dev_info->dev_id);
        return -EEXIST;
    }
    spin_unlock(&cce_ctrl->notify_spinlock);

    size = (long)(unsigned)sizeof(struct devdrv_notify_id_info) * notify_id_num;
    notify_info = vzalloc(size);
    if (notify_info == NULL) {
        devdrv_drv_err("[dev_id = %u]:model_info vzalloc failed\n", dev_info->dev_id);
        return -ENOMEM;
    }
    spin_lock(&cce_ctrl->notify_spinlock);
    cce_ctrl->notify_id_num = 0;
    for (i = 0; i < notify_id_num; i++) {
        notify_info[i].id = i;
        notify_info[i].devid = dev_info->dev_id;
        notify_info[i].cce_context = NULL;
        atomic_set(&notify_info[i].ref, 0);
        spin_lock_init(&notify_info[i].spinlock);
        if ((i < id_data->max_notify_id) && (i >= id_data->min_notify_id)) {
            list_add_tail(&notify_info[i].list, &cce_ctrl->notify_available_list);
            cce_ctrl->notify_id_num++;
        } else {
            INIT_LIST_HEAD(&notify_info[i].list);
        }
    }
    cce_ctrl->notify_addr = notify_info;
    spin_unlock(&cce_ctrl->notify_spinlock);

    return 0;
}

void devdrv_notify_id_destroy(struct devdrv_cce_ctrl *cce_ctrl)
{
    struct list_head *pos = NULL, *n = NULL;

    if ((cce_ctrl == NULL) || (cce_ctrl->notify_id_num == 0)) {
        devdrv_drv_err("invalid input argument.\n");
        return;
    }

    spin_lock(&cce_ctrl->notify_spinlock);
    if (!list_empty_careful(&cce_ctrl->notify_available_list)) {
        list_for_each_safe(pos, n, &cce_ctrl->notify_available_list)
        {
            cce_ctrl->notify_id_num--;
            list_del(pos);
        }
    }
    spin_unlock(&cce_ctrl->notify_spinlock);
    vfree(cce_ctrl->notify_addr);
    cce_ctrl->notify_addr = NULL;

    return;
}

STATIC int devdrv_id_init_param_check(struct devdrv_info *dev_info, u32 tsid, u32 streams, u32 events)
{
    if ((tsid >= DEVDRV_MAX_TS_NUM) || (dev_info == NULL) || (dev_info->cce_ctrl[tsid] == NULL) || (streams <= 0) ||
        (events <= 0)) {
        devdrv_drv_err("param check fail, tsid(%u), streams(%u), events(%u), dev_info(%pK)\n", tsid, streams, events,
                       dev_info);
        return -ENOMEM;
    }
    return 0;
}

/* init stream id and event id */
int devdrv_id_init(struct devdrv_info *dev_info, u32 tsid, u32 streams, u32 events)
{
    struct devdrv_cce_ctrl *cce_ctrl = NULL;
    struct devdrv_stream_sub_info *stream_sub_tmp = NULL;
    struct devdrv_stream_info *stream_tmp = NULL;
    struct devdrv_event_info *event_tmp = NULL;
    struct devdrv_id_data *id_data = NULL;
    phys_addr_t info_mem_addr;
    unsigned long size;
    int ret;
    u32 i;

    ret = devdrv_id_init_param_check(dev_info, tsid, streams, events);
    if (ret) {
        return -EINVAL;
    }
    cce_ctrl = dev_info->cce_ctrl[tsid];
    mutex_init(&devdrv_uio_mutex);

    cce_ctrl->stream_num = 0;
    cce_ctrl->event_num = 0;
    if (!list_empty_careful(&cce_ctrl->stream_available_list) || !list_empty_careful(&cce_ctrl->event_available_list)) {
        devdrv_drv_err("[dev_id=%u]:available list not empty.\n", cce_ctrl->devid);
        return -EEXIST;
    }
    info_mem_addr = cce_ctrl->mem_info[DEVDRV_INFO_MEM].virt_addr;
    size = (long)(unsigned)sizeof(*stream_sub_tmp) * streams;
    stream_sub_tmp = vmalloc(size);
    if (stream_sub_tmp == NULL) {
        return -ENOMEM;
    }
    size = (long)(unsigned)sizeof(*event_tmp) * events;
    event_tmp = vmalloc(size);
    if (event_tmp == NULL) {
        vfree(stream_sub_tmp);
        stream_sub_tmp = NULL;
        return -ENOMEM;
    }

    id_data = devdrv_get_id_data(dev_info->pdata, tsid);
    for (i = 0; i < streams; i++) {
        stream_tmp = devdrv_calc_stream_info(info_mem_addr, i);
        stream_tmp->id = i;
        stream_tmp->devid = cce_ctrl->devid;
        stream_tmp->cq_index = (u32)DEVDRV_CQSQ_INVALID_INDEX;
        stream_tmp->sq_index = (u32)DEVDRV_CQSQ_INVALID_INDEX;
        stream_tmp->strategy = (u32)STREAM_STRATEGY_NORMAL;

        stream_tmp->stream_sub = (void *)(stream_sub_tmp + i);
        stream_sub_tmp[i].cce_context = NULL;
        stream_sub_tmp[i].id = stream_tmp->id;
        if ((i < id_data->max_stream_id) && (i >= id_data->min_stream_id)) {
            list_add_tail(&stream_sub_tmp[i].list, &cce_ctrl->stream_available_list);
            cce_ctrl->stream_num++;
        } else {
            INIT_LIST_HEAD(&stream_sub_tmp[i].list);
        }
    }
    cce_ctrl->stream_sub_addr = stream_sub_tmp;

    for (i = 0; i < events; i++) {
        event_tmp[i].id = i;
        event_tmp[i].devid = cce_ctrl->devid;
        event_tmp[i].cce_context = NULL;
        INIT_LIST_HEAD(&event_tmp[i].ipc_wait_head);
        spin_lock_init(&event_tmp[i].spinlock);
        atomic_set(&event_tmp[i].ref, 0);
        if ((i < id_data->max_event_id) && (i >= id_data->min_event_id)) {
            list_add_tail(&event_tmp[i].list, &cce_ctrl->event_available_list);
            cce_ctrl->event_num++;
        } else {
            INIT_LIST_HEAD(&event_tmp[i].list);
        }
    }
    cce_ctrl->event_addr = (void *)event_tmp;

    return 0;
}

/* must call after all id are released into available list */
/* release all id in devdrv_cce_ctrl available list */
void devdrv_event_id_destroy(struct devdrv_cce_ctrl *cce_ctrl)
{
    struct list_head *pos = NULL, *n = NULL;
    struct devdrv_event_info *id = NULL;

    if (cce_ctrl == NULL) {
        devdrv_drv_err("invalid input argument.\n");
        return;
    }

    spin_lock(&cce_ctrl->event_spinlock);
    if (!list_empty_careful(&cce_ctrl->event_available_list)) {
        list_for_each_safe(pos, n, &cce_ctrl->event_available_list)
        {
            id = list_entry(pos, struct devdrv_event_info, list);
            list_del(pos);
        }
    }
    spin_unlock(&cce_ctrl->event_spinlock);
    vfree(cce_ctrl->event_addr);
    cce_ctrl->event_addr = NULL;

    return;
}

/* must call after all id are released into available list */
/* release all id in devdrv_cce_ctrl available list */
void devdrv_stream_id_destroy(struct devdrv_cce_ctrl *cce_ctrl)
{
    struct devdrv_stream_sub_info *stream_sub_info = NULL;
    struct list_head *pos = NULL, *n = NULL;

    if (cce_ctrl == NULL) {
        devdrv_drv_err("invalid input argument.\n");
        return;
    }

    spin_lock(&cce_ctrl->spinlock);
    if (!list_empty_careful(&cce_ctrl->stream_available_list)) {
        list_for_each_safe(pos, n, &cce_ctrl->stream_available_list)
        {
            stream_sub_info = list_entry(pos, struct devdrv_stream_sub_info, list);
            list_del(pos);
        }
    }
    spin_unlock(&cce_ctrl->spinlock);
    vfree(cce_ctrl->stream_sub_addr);
    cce_ctrl->stream_sub_addr = NULL;
    return;
}

STATIC struct devdrv_stream_info *devdrv_get_one_stream_id(struct devdrv_cce_ctrl *cce_ctrl)
{
    struct devdrv_stream_sub_info *sub_stream = NULL;
    phys_addr_t info_mem_addr;

    info_mem_addr = cce_ctrl->mem_info[DEVDRV_INFO_MEM].virt_addr;
    sub_stream = list_first_entry(&cce_ctrl->stream_available_list, struct devdrv_stream_sub_info, list);
    list_del(&sub_stream->list);
    cce_ctrl->stream_num--;
    return devdrv_calc_stream_info(info_mem_addr, sub_stream->id);
}

STATIC struct devdrv_notify_id_info *devdrv_get_one_notify_id(struct devdrv_cce_ctrl *cce_ctrl)
{
    struct devdrv_notify_id_info *notify_info = NULL;
    struct list_head *pos = NULL, *n = NULL;

    if (!list_empty_careful(&cce_ctrl->notify_available_list)) {
        list_for_each_safe(pos, n, &cce_ctrl->notify_available_list)
        {
            notify_info = list_entry(pos, struct devdrv_notify_id_info, list);
            if (atomic_read(&notify_info->ref) == 0) {
                list_del(&notify_info->list);
                cce_ctrl->notify_id_num--;
                return notify_info;
            }
        }
    }
    return NULL;
}

STATIC struct devdrv_event_info *devdrv_get_one_event_id(struct devdrv_cce_ctrl *cce_ctrl)
{
    struct devdrv_event_info *id = NULL;

    if (list_empty_careful(&cce_ctrl->event_available_list))
        return NULL;

    id = list_first_entry(&cce_ctrl->event_available_list, struct devdrv_event_info, list);
    list_del(&id->list);
    cce_ctrl->event_num--;
    return id;
}

STATIC struct devdrv_taskpool_id_info *devdrv_get_one_task_id(struct devdrv_cce_ctrl *cce_ctrl)
{
    struct devdrv_taskpool_id_info *id = NULL;
    if (list_empty_careful(&cce_ctrl->task_available_list))
        return NULL;
    id = list_first_entry(&cce_ctrl->task_available_list, struct devdrv_taskpool_id_info, list);
    list_del(&id->list);
    cce_ctrl->task_id_num--;
    return id;
}
STATIC struct devdrv_model_id_info *devdrv_get_one_model_id(struct devdrv_cce_ctrl *cce_ctrl)
{
    struct devdrv_model_id_info *id = NULL;

    if (list_empty_careful(&cce_ctrl->model_available_list))
        return NULL;

    id = list_first_entry(&cce_ctrl->model_available_list, struct devdrv_model_id_info, list);
    list_del(&id->list);
    cce_ctrl->model_id_num--;
    return id;
}

STATIC struct devdrv_stream_info *devdrv_find_one_stream_id(struct devdrv_cce_ctrl *cce_ctrl,
                                                            struct devdrv_cce_context *cce_context, int id)
{
    struct devdrv_stream_sub_info *sub_info = NULL;
    struct devdrv_stream_info *tmp = NULL;
    phys_addr_t info_mem_addr;

    info_mem_addr = cce_ctrl->mem_info[DEVDRV_INFO_MEM].virt_addr;
    tmp = devdrv_calc_stream_info(info_mem_addr, id);

    sub_info = (struct devdrv_stream_sub_info *)tmp->stream_sub;
    if ((sub_info->cce_context == NULL) || (sub_info->cce_context != cce_context))
        return NULL;
    else
        return tmp;
}

STATIC struct devdrv_event_info *devdrv_find_one_event_id(struct devdrv_cce_ctrl *cce_ctrl,
                                                          struct devdrv_cce_context *cce_context, int id)
{
    struct devdrv_event_info *tmp = NULL;
    struct devdrv_cce_context *tmp_cce_context = NULL;

    tmp = (struct devdrv_event_info *)(cce_ctrl->event_addr + (long)sizeof(struct devdrv_event_info) * id);
    tmp_cce_context = (struct devdrv_cce_context *)tmp->cce_context;

    if ((tmp->id != id) || (tmp_cce_context == NULL) || (tmp_cce_context != cce_context))
        return NULL;
    else
        return tmp;
}

STATIC struct devdrv_notify_id_info *devdrv_find_one_notify_id(struct devdrv_cce_ctrl *cce_ctrl,
                                                               struct devdrv_cce_context *cce_context, int id)
{
    struct devdrv_notify_id_info *notify_info = NULL;
    struct devdrv_cce_context *tmp_cce_context = NULL;

    notify_info =
        (struct devdrv_notify_id_info *)(cce_ctrl->notify_addr + (long)sizeof(struct devdrv_notify_id_info) * id);
    tmp_cce_context = (struct devdrv_cce_context *)notify_info->cce_context;
    if ((notify_info->id != id) || (tmp_cce_context == NULL) || (tmp_cce_context != cce_context))
        return NULL;
    else
        return notify_info;
}

STATIC struct devdrv_taskpool_id_info *devdrv_find_one_task_id(struct devdrv_cce_ctrl *cce_ctrl,
                                                               struct devdrv_cce_context *cce_context, int id)
{
    struct devdrv_taskpool_id_info *task_info = NULL;
    struct devdrv_cce_context *tmp_cce_context = NULL;

    task_info =
        (struct devdrv_taskpool_id_info *)(cce_ctrl->task_addr + (long)sizeof(struct devdrv_taskpool_id_info) * id);
    tmp_cce_context = (struct devdrv_cce_context *)task_info->cce_context;
    if ((task_info->id != id) || (tmp_cce_context == NULL) || (tmp_cce_context != cce_context))
        return NULL;
    else
        return task_info;
}

STATIC struct devdrv_model_id_info *devdrv_find_one_model_id(struct devdrv_cce_ctrl *cce_ctrl,
                                                             struct devdrv_cce_context *cce_context, int id)
{
    struct devdrv_model_id_info *model_info = NULL;
    struct devdrv_cce_context *tmp_cce_context = NULL;

    model_info = (struct devdrv_model_id_info *)(cce_ctrl->model_addr + (long)sizeof(struct devdrv_model_id_info) * id);
    tmp_cce_context = (struct devdrv_cce_context *)model_info->cce_context;

    if ((model_info->id != id) || (tmp_cce_context == NULL) || (tmp_cce_context != cce_context)) {
        return NULL;
    } else {
        return model_info;
    }
}

STATIC struct devdrv_stream_info *devdrv_alloc_stream_id(struct devdrv_cce_ctrl *cce_ctrl,
                                                         struct devdrv_cce_context *cce_context)
{
    struct devdrv_stream_sub_info *stream_sub_info = NULL;
    struct devdrv_stream_info *stream_info = NULL;
    u32 tsid = cce_ctrl->tsid;

    spin_lock(&cce_ctrl->spinlock);
    if (list_empty_careful(&cce_ctrl->stream_available_list)) {
        spin_unlock(&cce_ctrl->spinlock);
        devdrv_drv_err("[dev_id=%u]:available stream list empty!!!\n", cce_ctrl->devid);
        return NULL;
    }

    stream_info = devdrv_get_one_stream_id(cce_ctrl);
    stream_sub_info = (struct devdrv_stream_sub_info *)stream_info->stream_sub;
    stream_sub_info->cce_context = cce_context;
    list_add(&stream_sub_info->list, &cce_context->ts_context[tsid].stream_list);
    cce_context->ts_context[tsid].stream_num++;
    spin_unlock(&cce_ctrl->spinlock);

    return stream_info;
}

STATIC int devdrv_init_sq_uio(struct devdrv_cce_ctrl *cce_ctrl, struct devdrv_cce_context *cce_context,
                              struct devdrv_sq_sub_info *sq_sub_info, u32 size)
{
    struct devdrv_platform_data *pdata = NULL;
    struct devdrv_info *dev_info = NULL;
    struct uio_info *uio_tmp = NULL;
    phys_addr_t host_phys_addr = 0;
    u8 plat_type;

    if (sq_sub_info->uio != NULL)
        return 0;

    dev_info = cce_ctrl->dev_info;
    uio_tmp = (struct uio_info *)kzalloc(sizeof(struct uio_info), GFP_KERNEL);
    if (uio_tmp == NULL) {
        devdrv_drv_err("[dev_id=%u]:kzalloc fail: no mem for uio_tmp.\n", cce_ctrl->devid);
        return -ENOMEM;
    }
    uio_tmp->name = "devdrv-cqsq";
    uio_tmp->version = "0.0.1";

    pdata = dev_info->pdata;
    plat_type = cce_ctrl->dev_info->plat_type;
    if (plat_type == DEVDRV_MANAGER_DEVICE_ENV) {
        uio_tmp->mem[DEVDRV_SQ_CQ_MAP].addr = cce_ctrl->mem_info[DEVDRV_SQ_MEM].phy_addr +
                                              (unsigned long)sq_sub_info->index * (unsigned long)DEVDRV_SQ_SLOT_SIZE *
                                                  (unsigned long)DEVDRV_MAX_SQ_DEPTH;
        uio_tmp->mem[DEVDRV_SQ_CQ_MAP].memtype = UIO_MEM_PHYS;
        uio_tmp->mem[DEVDRV_SQ_CQ_MAP].size = size;
        sq_sub_info->phy_addr = uio_tmp->mem[DEVDRV_SQ_CQ_MAP].addr;
    } else if (plat_type == DEVDRV_MANAGER_HOST_ENV) {
        if ((pdata->ts_pdata[cce_ctrl->tsid].ts_sq_static_addr == 0) ||
            (pdata->ts_pdata[cce_ctrl->tsid].ts_sq_static_size == 0)) {
            devdrv_drv_err("invalid ts sq static mem.\n");
            kfree(uio_tmp);
            uio_tmp = NULL;
            return -ENOMEM;
        }

        host_phys_addr = pdata->ts_pdata[cce_ctrl->tsid].ts_sq_static_addr +
                         ((unsigned long)sq_sub_info->index * (unsigned long)DEVDRV_SQ_SLOT_SIZE *
                          (unsigned long)DEVDRV_MAX_SQ_DEPTH);

        uio_tmp->mem[DEVDRV_SQ_CQ_MAP].addr = (unsigned long)host_phys_addr;
        uio_tmp->mem[DEVDRV_SQ_CQ_MAP].memtype = UIO_MEM_PHYS;
        uio_tmp->mem[DEVDRV_SQ_CQ_MAP].size = size;
        sq_sub_info->bar_addr = host_phys_addr;
        sq_sub_info->phy_addr = cce_ctrl->mem_info[DEVDRV_SQ_MEM].phy_addr + (unsigned long)sq_sub_info->index *
                                                                                 (unsigned long)DEVDRV_SQ_SLOT_SIZE *
                                                                                 (unsigned long)DEVDRV_MAX_SQ_DEPTH;
        devdrv_drv_debug("sq id: %d.\n", sq_sub_info->index);
    } else {
        devdrv_drv_err("[dev_id=%u]:invalid environment type & uio type\n", cce_ctrl->devid);
        kfree(uio_tmp);
        uio_tmp = NULL;
        return -EINVAL;
    }

    if (devdrv_mmap_sq_mem(cce_context, cce_ctrl, sq_sub_info->index)) {
        devdrv_drv_err("[dev_id=%u]:mmap sq index failed, index = %u\n", cce_ctrl->devid, sq_sub_info->index);
        kfree(uio_tmp);
        uio_tmp = NULL;
        return -EFAULT;
    }

    sq_sub_info->uio = uio_tmp;

    return 0;
}

int devdrv_init_cq_uio(struct devdrv_cce_ctrl *cce_ctrl, struct devdrv_cq_sub_info *cq_sub_info, u32 size)
{
    struct devdrv_info *dev_info = NULL;
    struct uio_info *uio_tmp = NULL;
    phys_addr_t phy_addr;
    u8 plat_type;
#ifdef CFG_SOC_PLATFORM_MINIV2
    u32 tsid = cce_ctrl->tsid;
    int ret;
#endif /* CFG_SOC_PLATFORM_MINIV2 */

    if (cq_sub_info->uio != NULL) {
        return 0;
    }
    if (size <= 0) {
        devdrv_drv_err("invalid size(%u)\n", size);
        return -EINVAL;
    }

    dev_info = cce_ctrl->dev_info;
    uio_tmp = (struct uio_info *)kzalloc(sizeof(struct uio_info), GFP_KERNEL);
    if (uio_tmp == NULL) {
        devdrv_drv_err("[dev_id=%u]:kzalloc fail: no mem for uio_tmp.\n", cce_ctrl->devid);
        return -ENOMEM;
    }
    uio_tmp->name = "devdrv-cqsq";
    uio_tmp->version = "0.0.1";

    plat_type = cce_ctrl->dev_info->plat_type;
    if (plat_type == DEVDRV_MANAGER_DEVICE_ENV) {
#ifdef CFG_SOC_PLATFORM_MINIV2
        phy_addr = (phys_addr_t)CQ_RESERVE_MEM_BASE + (phys_addr_t)cce_ctrl->devid * CHIP_BASEADDR_PA_OFFSET +
                   (phys_addr_t)tsid * (phys_addr_t)CQ_RESERVE_MEM_SIZE +
                   (phys_addr_t)cq_sub_info->index * (phys_addr_t)size;
        uio_tmp->mem[DEVDRV_SQ_CQ_MAP].addr = (phys_addr_t)(uintptr_t)ioremap_wc(phy_addr, size);
        if (!uio_tmp->mem[DEVDRV_SQ_CQ_MAP].addr) {
            devdrv_drv_err("ioremap fail, no mem for cq\n");
            kfree(uio_tmp);
            uio_tmp = NULL;
            return -ENOMEM;
        }
        ret = memset_s((void *)(uintptr_t)uio_tmp->mem[DEVDRV_SQ_CQ_MAP].addr, size, 0, size);
        if (ret != 0) {
            devdrv_drv_err("cq uio memset failed.\n");
            iounmap((void *)(uintptr_t)uio_tmp->mem[DEVDRV_SQ_CQ_MAP].addr);
            uio_tmp->mem[DEVDRV_SQ_CQ_MAP].addr = 0;
            kfree(uio_tmp);
            uio_tmp = NULL;
            return -EINVAL;
        }
        uio_tmp->mem[DEVDRV_SQ_CQ_MAP].memtype = UIO_MEM_LOGICAL;
        uio_tmp->mem[DEVDRV_SQ_CQ_MAP].size = size;
#else
        uio_tmp->mem[DEVDRV_SQ_CQ_MAP].addr = (phys_addr_t)(uintptr_t)kzalloc(size, GFP_KERNEL);
        if (!uio_tmp->mem[DEVDRV_SQ_CQ_MAP].addr) {
            devdrv_drv_err("[dev_id=%u]:kzalloc fail, no mem for cq\n", cce_ctrl->devid);
            kfree(uio_tmp);
            uio_tmp = NULL;
            return -ENOMEM;
        }

        dev_info->drv_ops->flush_cache((u64)uio_tmp->mem[DEVDRV_SQ_CQ_MAP].addr, size);

        uio_tmp->mem[DEVDRV_SQ_CQ_MAP].memtype = UIO_MEM_LOGICAL;
        uio_tmp->mem[DEVDRV_SQ_CQ_MAP].size = size;

        phy_addr = virt_to_phys((void *)(uintptr_t)uio_tmp->mem[DEVDRV_SQ_CQ_MAP].addr);
#endif /* CFG_SOC_PLATFORM_MINIV2 */
        cq_sub_info->virt_addr = uio_tmp->mem[DEVDRV_SQ_CQ_MAP].addr;
        cq_sub_info->phy_addr = phy_addr;

        devdrv_drv_debug("cq_sub_info->index = %d,\n ", cq_sub_info->index);
    } else if (plat_type == DEVDRV_MANAGER_HOST_ENV) {
        uio_tmp->mem[DEVDRV_SQ_CQ_MAP].addr = (phys_addr_t)(uintptr_t)kzalloc(size, GFP_KERNEL);
        if (!uio_tmp->mem[DEVDRV_SQ_CQ_MAP].addr) {
            devdrv_drv_err("[dev_id=%u]:kzalloc fail, no mem for cq\n", cce_ctrl->devid);
            kfree(uio_tmp);
            uio_tmp = NULL;
            return -ENOMEM;
        }
        uio_tmp->mem[DEVDRV_SQ_CQ_MAP].memtype = UIO_MEM_LOGICAL;
        uio_tmp->mem[DEVDRV_SQ_CQ_MAP].size = size;
        phy_addr = dma_map_single(dev_info->dev, (void *)(uintptr_t)uio_tmp->mem[DEVDRV_SQ_CQ_MAP].addr, size,
                                  DMA_BIDIRECTIONAL);
        if (dma_mapping_error(dev_info->dev, phy_addr)) {
            devdrv_drv_err("[dev_id=%u]:dma map single error !\n", cce_ctrl->devid);
            kfree((void *)(uintptr_t)uio_tmp->mem[DEVDRV_SQ_CQ_MAP].addr);
            uio_tmp->mem[DEVDRV_SQ_CQ_MAP].addr = 0;
            kfree(uio_tmp);
            uio_tmp = NULL;
            return -ENOMEM;
        }
        cq_sub_info->virt_addr = uio_tmp->mem[DEVDRV_SQ_CQ_MAP].addr;
        cq_sub_info->phy_addr = phy_addr;
        cq_sub_info->size = size;
        cq_sub_info->dev = dev_info->dev;

        devdrv_drv_debug("cq_sub_info->index = %d, ", cq_sub_info->index);
    } else {
        devdrv_drv_err("[dev_id=%u]:invalid environment type & uio type\n", cce_ctrl->devid);
        kfree(uio_tmp);
        uio_tmp = NULL;
        return -EINVAL;
    }

    cq_sub_info->uio = uio_tmp;

    return 0;
}

void devdrv_exit_cq_uio(struct devdrv_cce_ctrl *cce_ctrl, struct devdrv_ts_cq_info *cq_info)
{
    struct devdrv_cq_sub_info *cq_sub_info = NULL;
    unsigned long flags;
    struct uio_info *uio = NULL;

    cq_sub_info = (struct devdrv_cq_sub_info *)cq_info->cq_sub;
    spin_lock_irqsave(&cq_sub_info->spinlock, flags);
    if (cq_sub_info->uio == NULL) {
        spin_unlock_irqrestore(&cq_sub_info->spinlock, flags);
        return;
    }
    uio = cq_sub_info->uio;
    cq_sub_info->uio = NULL;
    spin_unlock_irqrestore(&cq_sub_info->spinlock, flags);

#ifdef CFG_SOC_PLATFORM_MINIV2
    iounmap((void *)(uintptr_t)cq_sub_info->virt_addr);
#else
    kfree((void *)(uintptr_t)cq_sub_info->virt_addr);
#endif /* CFG_SOC_PLATFORM_MINIV2 */
    kfree(uio);
    cq_sub_info->virt_addr = 0;
    uio = NULL;
}

STATIC void devdrv_exit_sq_uio(struct devdrv_cce_ctrl *cce_ctrl, struct devdrv_cce_context *cce_context,
                               struct devdrv_ts_sq_info *sq_info)
{
    struct devdrv_sq_sub_info *sq_sub_info = NULL;
    struct uio_info *uio = NULL;

    sq_sub_info = (struct devdrv_sq_sub_info *)sq_info->sq_sub;

    if ((sq_info->stream_num != 0) || (sq_sub_info->uio == NULL)) {
        return;
    }
    uio = sq_sub_info->uio;
    sq_sub_info->uio = NULL;

    devdrv_unmap_sq_mem(cce_context, cce_ctrl, sq_info->index);

    kfree(uio);
    uio = NULL;
}

STATIC u32 devdrv_get_available_stream_num(struct devdrv_cce_ctrl *cce_ctrl)
{
    u32 stream_num;

    spin_lock(&cce_ctrl->spinlock);
    stream_num = cce_ctrl->stream_num;
    spin_unlock(&cce_ctrl->spinlock);

    return stream_num;
}

STATIC u32 devdrv_get_available_event_num(struct devdrv_cce_ctrl *cce_ctrl)
{
    u32 event_num;

    spin_lock(&cce_ctrl->event_spinlock);
    event_num = cce_ctrl->event_num;
    spin_unlock(&cce_ctrl->event_spinlock);

    return event_num;
}

STATIC u32 devdrv_get_available_notify_num(struct devdrv_cce_ctrl *cce_ctrl)
{
    u32 notify_num;

    spin_lock(&cce_ctrl->notify_spinlock);
    notify_num = cce_ctrl->notify_id_num;
    spin_unlock(&cce_ctrl->notify_spinlock);

    return notify_num;
}

STATIC u32 devdrv_get_available_task_num(struct devdrv_cce_ctrl *cce_ctrl)
{
    u32 task_id_num;
    spin_lock(&cce_ctrl->task_spinlock);
    task_id_num = cce_ctrl->task_id_num;
    spin_unlock(&cce_ctrl->task_spinlock);
    return task_id_num;
}
STATIC u32 devdrv_get_available_model_num(struct devdrv_cce_ctrl *cce_ctrl)
{
    u32 model_id_num;

    spin_lock(&cce_ctrl->model_spinlock);
    model_id_num = cce_ctrl->model_id_num;
    spin_unlock(&cce_ctrl->model_spinlock);

    return model_id_num;
}

STATIC u32 devdrv_get_context_cq_num(struct devdrv_cce_ctrl *cce_ctrl, struct devdrv_cce_context *cce_context)
{
    u32 tsid = cce_ctrl->tsid;
    u32 cq_num;

    spin_lock(&cce_ctrl->spinlock);
    cq_num = cce_context->ts_context[tsid].cq_num;
    spin_unlock(&cce_ctrl->spinlock);

    return cq_num;
}
STATIC int devdrv_alloc_stream_check(struct devdrv_cce_ctrl *cce_ctrl, int *no_stream)
{
    struct devdrv_info *dev_info = cce_ctrl->dev_info;
    struct devdrv_id_data *id_data = NULL;
    u32 tsid = cce_ctrl->tsid;
    u32 stream_num;
    int ret;

    id_data = devdrv_get_id_data(dev_info->pdata, tsid);

    mutex_lock(&cce_ctrl->stream_mutex_t);
    stream_num = devdrv_get_available_stream_num(cce_ctrl);
    if (stream_num == 0) {
        devdrv_drv_debug("no stream, try to get stream from opposite side\n");

        ret = dev_info->drv_ops->get_stream_sync(cce_ctrl);
        if (ret) {
            mutex_unlock(&cce_ctrl->stream_mutex_t);

            if (id_data->cur_stream_num < DEVDRV_MAX_STREAM_ID) {
                devdrv_drv_err("[dev_id=%u]:try to get stream from opposite side failed\n", cce_ctrl->devid);
            }

            *no_stream = 1;
            return -EINVAL;
        }
        devdrv_drv_debug("got stream, cce_ctrl->stream_num = %d\n", cce_ctrl->stream_num);
    }
    mutex_unlock(&cce_ctrl->stream_mutex_t);

    return 0;
}

struct devdrv_stream_info *devdrv_alloc_stream(struct devdrv_cce_ctrl *cce_ctrl, struct devdrv_cce_context *cce_context,
                                               int *no_stream, u32 strategy)
{
    struct devdrv_stream_info *stream_info = NULL;
    struct devdrv_sq_sub_info *sq_sub_info = NULL;
    struct devdrv_cq_sub_info *cq_sub_info = NULL;
    struct devdrv_ts_sq_info *sq_info = NULL;
    struct devdrv_ts_cq_info *cq_info = NULL;
    phys_addr_t info_mem_addr;
    phys_addr_t dev_phy_addr;
    phys_addr_t sq_addr;
    phys_addr_t cq_addr;
    int sq_ready_flag;
    int cq_ready_flag;
    u32 cq_num;
    int ret;

    if (no_stream == NULL) {
        devdrv_drv_err("[dev_id=%u]:invalid input handler.\n", cce_ctrl->devid);
        return NULL;
    }
    *no_stream = 0;
    dev_phy_addr = 0;

    mutex_lock(&cce_ctrl->mm_mutex_t);
    cq_num = devdrv_get_context_cq_num(cce_ctrl, cce_context);
    if (cq_num == 0) {
        mutex_unlock(&cce_ctrl->mm_mutex_t);
        devdrv_drv_err("[dev_id=%u]:runtime does not own a cq.\n", cce_ctrl->devid);
        return NULL;
    }

    ret = devdrv_alloc_stream_check(cce_ctrl, no_stream);
    if (ret) {
        mutex_unlock(&cce_ctrl->mm_mutex_t);
        return NULL;
    }

    stream_info = devdrv_alloc_stream_id(cce_ctrl, cce_context);
    if ((stream_info == NULL) || (stream_info->id < 0) || (stream_info->id >= DEVDRV_MAX_STREAM_ID)) {
        mutex_unlock(&cce_ctrl->mm_mutex_t);
        devdrv_drv_err("[dev_id=%u]:got wrong stream id!\n", cce_ctrl->devid);
        *no_stream = 1;
        return NULL;
    }
    stream_info->strategy = strategy;

    if (strategy == STREAM_STRATEGY_SINK) {
        /*
         * under this strategy do not need
         * to inform ts     stream-id status
         * and set strategy-element to strategy,
         * used for free by strategy.
         */
        stream_info->pid = cce_context->pid;
        mutex_unlock(&cce_ctrl->mm_mutex_t);
        devdrv_drv_info("alloc stream id(%d), with strategy(%d).\n", stream_info->id, strategy);
        return stream_info;
    }

    if (devdrv_cqsq_alloc(cce_ctrl, cce_context, stream_info) != 0) {
        mutex_unlock(&cce_ctrl->mm_mutex_t);
        devdrv_free_stream_id(cce_ctrl, cce_context, stream_info->id);
        return NULL;
    }

    /* init sq's and cq's uio */
    info_mem_addr = cce_ctrl->mem_info[DEVDRV_INFO_MEM].virt_addr;
    sq_info = devdrv_calc_sq_info(info_mem_addr, stream_info->sq_index);
    cq_info = devdrv_calc_cq_info(info_mem_addr, stream_info->cq_index);

    sq_sub_info = (struct devdrv_sq_sub_info *)sq_info->sq_sub;
    cq_sub_info = (struct devdrv_cq_sub_info *)cq_info->cq_sub;

    sq_ready_flag = 0;
    cq_ready_flag = 0;

    if (sq_info->stream_num > 1)
        sq_ready_flag = 1;
    if (cq_info->stream_num > 1)
        cq_ready_flag = 1;

    if (devdrv_init_sq_uio(cce_ctrl, cce_context, sq_sub_info, DEVDRV_MAX_SQ_DEPTH * DEVDRV_SQ_SLOT_SIZE)) {
        mutex_unlock(&cce_ctrl->mm_mutex_t);
        devdrv_free_stream(cce_ctrl, cce_context, stream_info->id, DEVDRV_NO_NEED_TO_INFORM);
        devdrv_drv_err("[dev_id=%u]:devdrv_init_uio sq failed!\n", cce_ctrl->devid);
        return NULL;
    }
    sq_addr = sq_sub_info->phy_addr;
    cq_addr = cq_sub_info->phy_addr;

    /*
     * inform TS sq and cq's info
     * if sq or cq are not alloced newly (alloc before),
     * only inform ts the relation between stream id and sq cq, but not addr.
     */
    if (sq_ready_flag)
        sq_addr = 0;
    if (cq_ready_flag)
        cq_addr = 0;

    ret = devdrv_mailbox_cqsq_inform(&cce_ctrl->mailbox, cce_context, DEVDRV_MAILBOX_CREATE_CQSQ_CALC, sq_info->index,
                                     sq_addr, cq_info->index, cq_addr, stream_info->id);
    if (ret) {
        mutex_unlock(&cce_ctrl->mm_mutex_t);
        devdrv_free_stream(cce_ctrl, cce_context, stream_info->id, DEVDRV_NO_NEED_TO_INFORM);
        devdrv_drv_err("[dev_id=%u]:devdrv_mailbox_cqsq_inform failed.\n", cce_ctrl->devid);
        return NULL;
    }
    stream_info->pid = cce_context->pid;
    mutex_unlock(&cce_ctrl->mm_mutex_t);

    return stream_info;
}

STATIC int devdrv_free_stream_inform(struct devdrv_cce_ctrl *cce_ctrl, struct devdrv_cce_context *cce_context,
                                     struct devdrv_stream_info *stream_info, struct devdrv_ts_sq_info *sq_info,
                                     struct devdrv_ts_cq_info *cq_info, int inform)
{
    int ret;
    /*
     * when release sq cq, must inform TS first
     * error means whether this func is called in error branch.
     * if called in error branch, no need to tell TS,
     * because we haven't tell TS about this sq cq before.
     */
    if ((sq_info->stream_num <= 1) && (cq_info->stream_num <= 1) && (inform == DEVDRV_HAVE_TO_INFORM)) {
        ret = devdrv_mailbox_cqsq_inform(&cce_ctrl->mailbox, cce_context, DEVDRV_MAILBOX_RELEASE_CQSQ_CALC,
                                         sq_info->index, 0, cq_info->index, 0, stream_info->id);
        if (ret) {
            stream_info->sq_index = sq_info->index;
            stream_info->cq_index = cq_info->index;
            devdrv_drv_err("[dev_id=%u]:devdrv_mailbox_cqsq_inform failed.\n", cce_ctrl->devid);
            return ret;
        }
    } else if ((sq_info->stream_num <= 1) && (inform == DEVDRV_HAVE_TO_INFORM)) {
        ret = devdrv_mailbox_cqsq_inform(&cce_ctrl->mailbox, cce_context, DEVDRV_MAILBOX_RELEASE_CQSQ_CALC,
                                         sq_info->index, 0, DEVDRV_MAILBOX_INVALID_INDEX, 0, stream_info->id);
        if (ret) {
            stream_info->sq_index = sq_info->index;
            stream_info->cq_index = cq_info->index;
            devdrv_drv_err("[dev_id=%u]:devdrv_mailbox_cqsq_inform failed.\n", cce_ctrl->devid);
            return ret;
        }
    }
    return 0;
}

int devdrv_free_stream(struct devdrv_cce_ctrl *cce_ctrl, struct devdrv_cce_context *cce_context, int id, int inform)
{
    struct devdrv_stream_sub_info *stream_sub_info = NULL;
    struct devdrv_stream_info *stream_info = NULL;
    struct devdrv_ts_sq_info *sq_info = NULL;
    struct devdrv_ts_cq_info *cq_info = NULL;
    phys_addr_t info_mem_addr;
    int ret;
    u32 tsid;

    if ((id < 0) || (id >= DEVDRV_MAX_STREAM_ID) || (cce_ctrl == NULL)) {
        devdrv_drv_err("invalid free stream id\n");
        return -EINVAL;
    }

    tsid = cce_ctrl->tsid;
    info_mem_addr = cce_ctrl->mem_info[DEVDRV_INFO_MEM].virt_addr;

    mutex_lock(&cce_ctrl->mm_mutex_t);
    spin_lock(&cce_ctrl->spinlock);
    stream_info = devdrv_find_one_stream_id(cce_ctrl, cce_context, id);
    if (stream_info == NULL) {
        spin_unlock(&cce_ctrl->spinlock);
        mutex_unlock(&cce_ctrl->mm_mutex_t);
        devdrv_drv_err("[dev_id=%u]:devdrv_hash_find_one_stream_id return NULL.\n", cce_ctrl->devid);
        return -ENODATA;
    }
    stream_sub_info = (struct devdrv_stream_sub_info *)stream_info->stream_sub;
    if (stream_sub_info == NULL) {
        spin_unlock(&cce_ctrl->spinlock);
        mutex_unlock(&cce_ctrl->mm_mutex_t);
        devdrv_drv_err("[dev_id=%u]:stream_sub_info return NULL with id(%d).\n", cce_ctrl->devid, id);
        return -ENODATA;
    }

    if (stream_info->strategy == (u32)STREAM_STRATEGY_SINK) {
        list_del(&stream_sub_info->list);
        list_add(&stream_sub_info->list, &cce_ctrl->stream_available_list);
        cce_ctrl->stream_num++;
        cce_context->ts_context[tsid].stream_num--;
        stream_sub_info->cce_context = NULL;
        /* set strategy method to       default value */
        stream_info->strategy = (u32)STREAM_STRATEGY_NORMAL;
        spin_unlock(&cce_ctrl->spinlock);
        mutex_unlock(&cce_ctrl->mm_mutex_t);
        devdrv_drv_info("free stream id(%d),with strategy(1)\n", id);
        return 0;
    }
    sq_info = devdrv_calc_sq_info(info_mem_addr, stream_info->sq_index);
    cq_info = devdrv_calc_cq_info(info_mem_addr, stream_info->cq_index);

    /* set invalid index, in case that runtime may free same stream id twice */
    stream_info->sq_index = (u32)DEVDRV_CQSQ_INVALID_INDEX;
    stream_info->cq_index = (u32)DEVDRV_CQSQ_INVALID_INDEX;
    spin_unlock(&cce_ctrl->spinlock);

    ret = devdrv_free_stream_inform(cce_ctrl, cce_context, stream_info, sq_info, cq_info, inform);
    if (ret) {
        mutex_unlock(&cce_ctrl->mm_mutex_t);
        return ret;
    }

    /* recycle stream id, sq, cq */
    spin_lock(&cce_ctrl->spinlock);
    list_del(&stream_sub_info->sq_list);

    devdrv_cqsq_free_id(cce_ctrl, cce_context, sq_info->index, cq_info->index);

    list_del(&stream_sub_info->list);
    list_add(&stream_sub_info->list, &cce_ctrl->stream_available_list);
    cce_ctrl->stream_num++;
    cce_context->ts_context[tsid].stream_num--;
    stream_sub_info->cce_context = NULL;

    /* must be set after devdrv_cqsq_free_id */
    stream_info->sq_index = (u32)DEVDRV_CQSQ_INVALID_INDEX;
    stream_info->cq_index = (u32)DEVDRV_CQSQ_INVALID_INDEX;
    spin_unlock(&cce_ctrl->spinlock);

    devdrv_exit_sq_uio(cce_ctrl, cce_context, sq_info);
    mutex_unlock(&cce_ctrl->mm_mutex_t);

    return 0;
}

int devdrv_notify_ts_msg(struct devdrv_info *dev_info, u32 tsid, int notifyId)
{
    struct devdrv_ts_notify_msg msg;
    int result = -1;
    int ret;

    if ((tsid >= DEVDRV_MAX_TS_NUM) || (dev_info == NULL) || (dev_info->cce_ctrl[tsid] == NULL)) {
        devdrv_drv_err("param is invalid\n");
        return -ENOMEM;
    }

    msg.header.valid = DEVDRV_MAILBOX_MESSAGE_VALID;
    msg.header.cmd_type = DEVDRV_MAILBOX_RESET_NOTIFY_ID;
    msg.header.result = 0;
    msg.notifyId = notifyId;
    msg.plat_type = dev_info->plat_type;

    ret = devdrv_mailbox_kernel_sync_no_feedback(&dev_info->cce_ctrl[tsid]->mailbox, (u8 *)&msg, sizeof(msg), &result);
    if (ret || result) {
        devdrv_drv_err("[dev_id = %u]:notify alloc inform ts failed\n", dev_info->dev_id);
        return -EFAULT;
    };
    return 0;
}

int devdrv_alloc_notify_id(struct devdrv_cce_ctrl *cce_ctrl, struct devdrv_cce_context *cce_context,
                           struct devdrv_ioctl_arg *arg)
{
    struct devdrv_notify_id_info *notify_info = NULL;
    struct devdrv_info *dev_info = NULL;
    u32 notify_num;
    int ret;

    if ((cce_ctrl == NULL) || (arg == NULL) || (cce_ctrl->tsid >= DEVDRV_MAX_TS_NUM)) {
        devdrv_drv_err("param is invalid\n");
        return -ENOMEM;
    }
    dev_info = cce_ctrl->dev_info;

    mutex_lock(&cce_ctrl->notify_mutex_t);
    notify_num = devdrv_get_available_notify_num(cce_ctrl);
    if (notify_num == 0) {
        ret = dev_info->drv_ops->get_notify_sync(cce_ctrl);
        if (ret) {
            mutex_unlock(&cce_ctrl->notify_mutex_t);
            devdrv_drv_err("[dev_id = %u]:try to get notify from opposite side failed\n", dev_info->dev_id);
            return -EINVAL;
        }
    }
    mutex_unlock(&cce_ctrl->notify_mutex_t);

    spin_lock(&cce_ctrl->notify_spinlock);
    notify_info = devdrv_get_one_notify_id(cce_ctrl);
    if (notify_info == NULL) {
        spin_unlock(&cce_ctrl->notify_spinlock);
        devdrv_drv_err("[dev_id = %u]:devdrv_get_one_event_id return NULL.\n", dev_info->dev_id);
        return -EINVAL;
    }
    spin_lock(&notify_info->spinlock);
    notify_info->cce_context = cce_context;
    spin_unlock(&notify_info->spinlock);

    list_add(&notify_info->list, &cce_context->ts_context[cce_ctrl->tsid].notify_list);
    cce_context->ts_context[cce_ctrl->tsid].notify_id_num++;
    spin_unlock(&cce_ctrl->notify_spinlock);
    atomic_inc(&notify_info->ref);

    ret = devdrv_notify_ts_msg(cce_ctrl->dev_info, cce_ctrl->tsid, notify_info->id);
    if (ret) {
        devdrv_drv_err("[dev_id = %u]:send ts notify alloc msg failed, notify_id = %d\n", dev_info->dev_id,
                       notify_info->id);
        goto notify_ts_msg_failed;
    }
    arg->notify_para.notify_id = notify_info->id;

    return 0;
notify_ts_msg_failed:
    if (devdrv_free_one_notify_id(cce_ctrl, cce_context, notify_info->id, DEVDRV_NOTIFY_NOT_INFORM_TS)) {
        devdrv_drv_err("[dev_id = %u]:free one notify id failed, id = %d\n", dev_info->dev_id, notify_info->id);
        return -EINVAL;
    }
    return -EFAULT;
}

int devdrv_free_notify_id(struct devdrv_cce_ctrl *cce_ctrl, struct devdrv_cce_context *cce_context,
                          struct devdrv_ioctl_arg *arg)
{
    int ret;

    if ((cce_ctrl == NULL) || (cce_context == NULL) || (arg == NULL)) {
        devdrv_drv_err("param check fail:"
                       "cce_ctrl == NULL(%d), cce_context == NULL(%d), arg == NULL(%d).\n",
                       (cce_ctrl == NULL), (cce_context == NULL), (arg == NULL));
        return -EFAULT;
    }

    ret = devdrv_free_one_notify_id(cce_ctrl, cce_context, arg->notify_para.notify_id, DEVDRV_NOTIFY_INFORM_TS);
    if (ret) {
        devdrv_drv_err("free one notify id fail reg(%d)\n", ret);
        return ret;
    }

    return 0;
}

int devdrv_get_notify_base_addr(u32 devid, u64 *phy_addr)
{
    if ((phy_addr == NULL) || (devid >= DEVDRV_MAX_DAVINCI_NUM)) {
        devdrv_drv_err("notify base: invalid null pointer. "
                       "dev_phy_addr = %pK, devid = %d\n",
                       phy_addr, devid);
        return -EINVAL;
    }
    *phy_addr = CHIP_BASEADDR_PA_OFFSET * (devid % MAX_NODE_NUM) + 0xAF100000ULL + 0xE000;

    return 0;
}
EXPORT_SYMBOL(devdrv_get_notify_base_addr);

struct devdrv_event_info *devdrv_alloc_event_id(struct devdrv_cce_ctrl *cce_ctrl,
                                                struct devdrv_cce_context *cce_context)
{
    struct devdrv_info *dev_info = cce_ctrl->dev_info;
    struct devdrv_event_info *event_info = NULL;
    struct devdrv_id_data *id_data = NULL;
    u32 event_num;
    int ret;
    u32 tsid = cce_ctrl->tsid;

    id_data = devdrv_get_id_data(dev_info->pdata, tsid);

    mutex_lock(&cce_ctrl->event_mutex_t);
    event_num = devdrv_get_available_event_num(cce_ctrl);
    if (event_num == 0) {
        devdrv_drv_debug("no event, try to get event from opposite side, "
                         "cce_ctrl->event = %d\n",
                         cce_ctrl->event_num);

        ret = dev_info->drv_ops->get_event_sync(cce_ctrl);
        if (ret) {
            mutex_unlock(&cce_ctrl->event_mutex_t);

            if (id_data->cur_event_num < DEVDRV_MAX_EVENT_ID) {
                devdrv_drv_err("[dev_id=%u]:try to get event from opposite side failed\n", cce_ctrl->devid);
            }

            return NULL;
        }
        devdrv_drv_debug("got event, cce_ctrl->event_num = %d\n", cce_ctrl->event_num);
    }
    mutex_unlock(&cce_ctrl->event_mutex_t);

    spin_lock(&cce_ctrl->event_spinlock);
    event_info = devdrv_get_one_event_id(cce_ctrl);
    if (event_info == NULL) {
        spin_unlock(&cce_ctrl->event_spinlock);
        devdrv_drv_err("[dev_id=%u]:devdrv_get_one_event_id return NULL.\n", cce_ctrl->devid);
        return NULL;
    }

    spin_lock(&event_info->spinlock);
    event_info->cce_context = cce_context;
    spin_unlock(&event_info->spinlock);

    list_add(&event_info->list, &cce_context->ts_context[tsid].event_list);
    cce_context->ts_context[tsid].event_num++;

    spin_unlock(&cce_ctrl->event_spinlock);

    return event_info;
}

int devdrv_free_stream_id(struct devdrv_cce_ctrl *cce_ctrl, struct devdrv_cce_context *cce_context, int id)
{
    struct devdrv_stream_sub_info *stream_sub_info = NULL;
    struct devdrv_stream_info *stream_info = NULL;
    u32 tsid = cce_ctrl->tsid;

    spin_lock(&cce_ctrl->spinlock);
    stream_info = devdrv_find_one_stream_id(cce_ctrl, cce_context, id);
    if (stream_info == NULL) {
        spin_unlock(&cce_ctrl->spinlock);
        devdrv_drv_err("devdrv free stream id got wrong stream info!\n");
        return -ENODATA;
    }
    stream_sub_info = (struct devdrv_stream_sub_info *)stream_info->stream_sub;
    list_del(&stream_sub_info->list);
    list_add(&stream_sub_info->list, &cce_ctrl->stream_available_list);
    cce_ctrl->stream_num++;
    cce_context->ts_context[tsid].stream_num--;
    stream_sub_info->cce_context = NULL;
    spin_unlock(&cce_ctrl->spinlock);

    return 0;
}

int devdrv_free_event_id(struct devdrv_cce_ctrl *cce_ctrl, struct devdrv_cce_context *cce_context, int id)
{
    struct devdrv_event_info *tmp = NULL;
    u32 tsid = 0;

    if ((id < 0) || (id >= DEVDRV_MAX_EVENT_ID) || (cce_ctrl == NULL) || (cce_context == NULL)) {
        devdrv_drv_err("invalid input argument.\n");
        return -EINVAL;
    }
    spin_lock(&cce_ctrl->event_spinlock);
    tmp = devdrv_find_one_event_id(cce_ctrl, cce_context, id);
    if ((tmp == NULL) || (tmp->cce_context == NULL)) {
        spin_unlock(&cce_ctrl->event_spinlock);
        devdrv_drv_err("[dev_id=%u]:devdrv_find_one_event_id return NULL.\n", cce_ctrl->devid);
        return -ENODATA;
    }

    spin_lock(&tmp->spinlock);
    tmp->cce_context = NULL;
    spin_unlock(&tmp->spinlock);

    list_del(&tmp->list);
    list_add(&tmp->list, &cce_ctrl->event_available_list);
    cce_ctrl->event_num++;
    cce_context->ts_context[tsid].event_num--;
    spin_unlock(&cce_ctrl->event_spinlock);

    return 0;
}

int devdrv_free_id(struct devdrv_cce_ctrl *cce_ctrl, struct devdrv_cce_context *cce_context, unsigned int cmd, int id,
                   unsigned long arg)
{
    int ret;

    switch (cmd) {
        case DEVDRV_FREE_STREAM_ID:
            mutex_lock(&cce_context->stream_mutex);
            ret = devdrv_free_stream(cce_ctrl, cce_context, id, DEVDRV_HAVE_TO_INFORM);
            mutex_unlock(&cce_context->stream_mutex);
            if (ret) {
                devdrv_drv_err("[dev_id=%u]:devdrv_free_stream error.\n", cce_context->devid);
                ret = -EINVAL;
            } else {
                ret = 0;
            }
            break;
        case DEVDRV_FREE_EVENT_ID:
            if (devdrv_free_event_id(cce_ctrl, cce_context, id) != 0) {
                devdrv_drv_err("[dev_id=%u]:devdrv_free_id error.\n", cce_context->devid);
                ret = -EINVAL;
            } else {
                ret = 0;
            }
            break;
        default:
            ret = -EINVAL;
    }
    return ret;
}

struct devdrv_event_info *devdrv_get_event_id(struct devdrv_cce_ctrl *cce_ctrl)
{
    struct devdrv_info *dev_info = NULL;
    struct devdrv_event_info *event_info = NULL;
    u32 event_num;
    int ret;

    if ((cce_ctrl == NULL) || (cce_ctrl->dev_info == NULL)) {
        devdrv_drv_err("cce info(%pK) is err.\n", cce_ctrl);
        return NULL;
    }

    dev_info = cce_ctrl->dev_info;

    mutex_lock(&cce_ctrl->event_mutex_t);
    event_num = devdrv_get_available_event_num(cce_ctrl);
    if (event_num == 0) {
        devdrv_drv_debug("no event, try to get event from opposite side, "
                         "cce_ctrl->event = %d\n",
                         cce_ctrl->event_num);

        if ((dev_info->drv_ops == NULL) || (dev_info->drv_ops->get_event_sync == NULL)) {
            mutex_unlock(&cce_ctrl->event_mutex_t);
            devdrv_drv_err("[dev_id=%u]:drv_ops(%pK) is err.\n", cce_ctrl->devid, dev_info->drv_ops);
            return NULL;
        }

        ret = dev_info->drv_ops->get_event_sync(cce_ctrl);
        if (ret) {
            mutex_unlock(&cce_ctrl->event_mutex_t);
            devdrv_drv_err("[dev_id=%u]:try to get event from opposite side failed\n", cce_ctrl->devid);
            return NULL;
        }
        devdrv_drv_debug("got event, cce_ctrl->event_num = %d\n", cce_ctrl->event_num);
    }
    mutex_unlock(&cce_ctrl->event_mutex_t);

    spin_lock(&cce_ctrl->event_spinlock);
    event_info = devdrv_get_one_event_id(cce_ctrl);
    if (event_info == NULL) {
        spin_unlock(&cce_ctrl->event_spinlock);
        devdrv_drv_err("[dev_id=%u]:devdrv_get_one_event_id return NULL.\n", cce_ctrl->devid);
        return NULL;
    }
    spin_unlock(&cce_ctrl->event_spinlock);

    return event_info;
}

int devdrv_alloc_stream_msg_chan(struct devdrv_info *dev_info, u32 tsid,
                                 struct devdrv_manager_msg_resource_id *dev_msg_resource_id)
{
    struct devdrv_cce_ctrl *cce_ctrl = NULL;
    u32 req_id_num = dev_msg_resource_id->req_id_num;
    struct devdrv_stream_sub_info *sub_stream = NULL;
    u32 i;
    u32 j = 0;

    if ((tsid >= DEVDRV_MAX_TS_NUM) || (dev_info == NULL) || (dev_info->cce_ctrl[tsid] == NULL)) {
        devdrv_drv_err("param is invalid\n");
        return -ENOMEM;
    }
    cce_ctrl = dev_info->cce_ctrl[tsid];

    devdrv_drv_debug("1. cce_ctrl->stream_num = %d, "
                     "dev_msg_resource_id->req_id_num = %d\n",
                     cce_ctrl->stream_num, req_id_num);

    spin_lock(&cce_ctrl->spinlock);
    if ((cce_ctrl->stream_num < req_id_num) || (req_id_num > DEVDRV_MANAGER_MSG_ID_NUM_MAX)) {
        spin_unlock(&cce_ctrl->spinlock);
        devdrv_drv_err("[dev_id=%u]:no enough stream id\n", cce_ctrl->devid);
        return -EINVAL;
    }

    for (i = 0; i < req_id_num; i++) {
        sub_stream = list_first_entry(&cce_ctrl->stream_available_list, struct devdrv_stream_sub_info, list);
        list_del(&sub_stream->list);
        cce_ctrl->stream_num--;
        dev_msg_resource_id->id[j++] = sub_stream->id;
        dev_msg_resource_id->ret_id_num++;

        devdrv_drv_debug("stream id = %d, j = %d, "
                         "ret_id_num = %d , stream_num = %d\n",
                         sub_stream->id, j, dev_msg_resource_id->ret_id_num, cce_ctrl->stream_num);
    }
    spin_unlock(&cce_ctrl->spinlock);

    devdrv_drv_debug("2. cce_ctrl->stream_num = %d, "
                     "dev_msg_resource_id->ret_id_num = %d\n",
                     cce_ctrl->stream_num, dev_msg_resource_id->ret_id_num);

    return 0;
}

int devdrv_add_stream_msg_chan(struct devdrv_info *dev_info, u32 tsid,
                               struct devdrv_manager_msg_resource_id *dev_msg_resource_id)
{
    struct devdrv_cce_ctrl *cce_ctrl = NULL;
    u32 ret_id_num = dev_msg_resource_id->ret_id_num;
    struct devdrv_stream_sub_info *stream_sub_info = NULL;
    struct devdrv_stream_info *stream_info_t = NULL;
    phys_addr_t info_mem_addr;
    u16 stream_id;
    u32 i;

    if ((tsid >= DEVDRV_MAX_TS_NUM) || (dev_info == NULL) || (dev_info->cce_ctrl[tsid] == NULL)) {
        devdrv_drv_err("param is invalid\n");
        return -ENOMEM;
    }
    cce_ctrl = dev_info->cce_ctrl[tsid];

    info_mem_addr = cce_ctrl->mem_info[DEVDRV_INFO_MEM].virt_addr;

    devdrv_drv_debug("1. stream num = %d\n", cce_ctrl->stream_num);

    spin_lock(&cce_ctrl->spinlock);
    for (i = 0; i < ret_id_num; i++) {
        stream_id = dev_msg_resource_id->id[i];
        if (stream_id >= DEVDRV_MAX_STREAM_ID) {
            spin_unlock(&cce_ctrl->spinlock);
            devdrv_drv_err("[dev_id=%u]:invliad stream id, stream_id = %u\n", cce_ctrl->devid, stream_id);
            return -EFAULT;
        }
        stream_info_t = devdrv_calc_stream_info(info_mem_addr, stream_id);
        stream_sub_info = (struct devdrv_stream_sub_info *)stream_info_t->stream_sub;
        list_add_tail(&stream_sub_info->list, &cce_ctrl->stream_available_list);

        devdrv_drv_debug("stream_id = %d, stream_num = %d\n", stream_id, cce_ctrl->stream_num);

        cce_ctrl->stream_num++;
    }
    spin_unlock(&cce_ctrl->spinlock);

    devdrv_drv_debug("2. stream num = %d\n", cce_ctrl->stream_num);

    return 0;
}

int devdrv_add_event_msg_chan(struct devdrv_info *dev_info, u32 tsid,
                              struct devdrv_manager_msg_resource_id *dev_msg_resource_id)
{
    struct devdrv_cce_ctrl *cce_ctrl = NULL;
    u32 ret_id_num = dev_msg_resource_id->ret_id_num;
    struct devdrv_event_info *event_info = NULL;
    u16 event_id;
    u32 i;

    if ((tsid >= DEVDRV_MAX_TS_NUM) || (dev_info == NULL) || (dev_info->cce_ctrl[tsid] == NULL)) {
        devdrv_drv_err("param is invalid\n");
        return -ENOMEM;
    }
    cce_ctrl = dev_info->cce_ctrl[tsid];

    if (ret_id_num > DEVDRV_MANAGER_MSG_ID_NUM_MAX) {
        devdrv_drv_err("[dev_id = %u]:invalid input parameter, ret_id_num = %u\n", dev_info->dev_id, ret_id_num);
        return -EINVAL;
    }

    event_info = (struct devdrv_event_info *)cce_ctrl->event_addr;
    devdrv_drv_debug("1. event_num = %d\n", cce_ctrl->event_num);

    spin_lock(&cce_ctrl->event_spinlock);
    for (i = 0; i < ret_id_num; i++) {
        event_id = dev_msg_resource_id->id[i];
        if (event_id >= DEVDRV_MAX_EVENT_ID) {
            spin_unlock(&cce_ctrl->event_spinlock);
            devdrv_drv_err("[dev_id = %u]:invalid event id, event_id = %u\n", dev_info->dev_id, event_id);
            return -EFAULT;
        }
        list_add_tail(&event_info[event_id].list, &cce_ctrl->event_available_list);
        cce_ctrl->event_num++;
        devdrv_drv_debug("event_id = %d, event_num = %d\n", event_id, cce_ctrl->event_num);
    }
    spin_unlock(&cce_ctrl->event_spinlock);
    devdrv_drv_debug("2. event_num = %d\n", cce_ctrl->event_num);

    return 0;
}

int devdrv_alloc_event_msg_chan(struct devdrv_info *dev_info, u32 tsid,
                                struct devdrv_manager_msg_resource_id *dev_msg_resource_id)
{
    struct devdrv_cce_ctrl *cce_ctrl = NULL;
    u32 req_id_num = dev_msg_resource_id->req_id_num;
    struct devdrv_event_info *event_info = NULL;
    u32 i;
    u32 j = 0;

    if ((tsid >= DEVDRV_MAX_TS_NUM) || (dev_info == NULL) || (dev_info->cce_ctrl[tsid] == NULL)) {
        devdrv_drv_err("param is invalid\n");
        return -ENOMEM;
    }
    cce_ctrl = dev_info->cce_ctrl[tsid];

    spin_lock(&cce_ctrl->event_spinlock);
    if ((cce_ctrl->event_num < req_id_num) || (req_id_num > DEVDRV_MANAGER_MSG_ID_NUM_MAX)) {
        spin_unlock(&cce_ctrl->event_spinlock);
        devdrv_drv_err("[dev_id=%u]:event_num = %u, req_id_num = %u, no enough event id\n", cce_ctrl->devid,
                       cce_ctrl->event_num, req_id_num);
        return -EINVAL;
    }

    for (i = 0; i < req_id_num; i++) {
        event_info = list_first_entry(&cce_ctrl->event_available_list, struct devdrv_event_info, list);
        list_del(&event_info->list);
        cce_ctrl->event_num--;
        dev_msg_resource_id->id[j++] = event_info->id;
        dev_msg_resource_id->ret_id_num++;

        devdrv_drv_debug("event id = %d, j = %d, "
                         "ret_id_num = %d , event_num = %d\n",
                         event_info->id, j, dev_msg_resource_id->ret_id_num, cce_ctrl->event_num);
    }
    spin_unlock(&cce_ctrl->event_spinlock);

    return 0;
}
