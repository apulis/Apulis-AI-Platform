/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2012-2019. All rights reserved.
 * Description:
 * Author: huawei
 * Create: 2019-10-15
 */

#include <linux/uaccess.h>
#include <linux/device.h>
#include <linux/errno.h>
#include <linux/fs.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/mutex.h>
#include <linux/semaphore.h>
#include <linux/slab.h>
#include <linux/spinlock.h>
#include <linux/swap.h>
#include <linux/types.h>
#include <linux/uio_driver.h>
#include <linux/irq.h>
#include <linux/pci.h>
#include <linux/delay.h>

#include "devdrv_cqsq.h"
#include "devdrv_devinit.h"
#include "devdrv_driver_pm.h"
#include "devdrv_id.h"
#include "devdrv_recycle.h"
#include "devdrv_shm.h"
#include "devdrv_svmapi.h"
#include "devdrv_driver_adapt.h"
#include "devdrv_config_p2p.h"

#define RECYCLE_RESOURCE 0xF
STATIC int devdrv_ioctl_alloc_stream(struct devdrv_cce_ctrl *cce_ctrl, struct devdrv_cce_context *cce_context,
                                     struct devdrv_ioctl_arg *arg);
STATIC int devdrv_ioctl_alloc_event(struct devdrv_cce_ctrl *cce_ctrl, struct devdrv_cce_context *cce_context,
                                    struct devdrv_ioctl_arg *arg);
STATIC int devdrv_ioctl_free_stream(struct devdrv_cce_ctrl *cce_ctrl, struct devdrv_cce_context *cce_context,
                                    struct devdrv_ioctl_arg *arg);
STATIC int devdrv_ioctl_free_event(struct devdrv_cce_ctrl *cce_ctrl, struct devdrv_cce_context *cce_context,
                                   struct devdrv_ioctl_arg *arg);
STATIC int devdrv_ioctl_alloc_model(struct devdrv_cce_ctrl *cce_ctrl, struct devdrv_cce_context *cce_context,
                                    struct devdrv_ioctl_arg *arg);
STATIC int devdrv_ioctl_free_model(struct devdrv_cce_ctrl *cce_ctrl, struct devdrv_cce_context *cce_context,
                                   struct devdrv_ioctl_arg *arg);

STATIC int devdrv_ioctl_alloc_task(struct devdrv_cce_ctrl *cce_ctrl, struct devdrv_cce_context *cce_context,
                                   struct devdrv_ioctl_arg *arg);
STATIC int devdrv_ioctl_free_task(struct devdrv_cce_ctrl *cce_ctrl, struct devdrv_cce_context *cce_context,
                                  struct devdrv_ioctl_arg *arg);
STATIC int devdrv_ioctl_report_wait(struct devdrv_cce_ctrl *cce_ctrl, struct devdrv_cce_context *cce_context,
                                    struct devdrv_ioctl_arg *arg);

STATIC int devdrv_ioctl_ipcmsg_rcv_async(struct devdrv_cce_ctrl *cce_ctrl, struct devdrv_cce_context *cce_context,
                                         unsigned long arg);

STATIC int devdrv_ioctl_ipcmsg_send_async(struct devdrv_cce_ctrl *cce_ctrl, struct devdrv_cce_context *cce_context,
                                          unsigned long arg);

int (*const devdrv_ioctl_ipc_handlers[DEVDRV_MAX_CMD])(struct devdrv_cce_ctrl *cce_ctrl,
    struct devdrv_cce_context *cce_context, unsigned long arg) = {
        [_IOC_NR(DEVDRV_TSMSG_RECV_ASYNC)] = devdrv_ioctl_ipcmsg_rcv_async,
        [_IOC_NR(DEVDRV_TSMSG_SEND_ASYNC)] = devdrv_ioctl_ipcmsg_send_async,
};  //lint !e167

int (*const devdrv_ioctl_handlers[DEVDRV_MAX_CMD])(struct devdrv_cce_ctrl *cce_ctrl,
    struct devdrv_cce_context *cce_context,
    struct devdrv_ioctl_arg *arg) = {
        [_IOC_NR(DEVDRV_ALLOC_STREAM_ID)] = devdrv_ioctl_alloc_stream,
        [_IOC_NR(DEVDRV_FREE_STREAM_ID)] = devdrv_ioctl_free_stream,
        [_IOC_NR(DEVDRV_ALLOC_EVENT_ID)] = devdrv_ioctl_alloc_event,
        [_IOC_NR(DEVDRV_FREE_EVENT_ID)] = devdrv_ioctl_free_event,
        [_IOC_NR(DEVDRV_REPORT_WAIT)] = devdrv_ioctl_report_wait,
        [_IOC_NR(DEVDRV_GET_OCCUPY_STREAM_ID)] = NULL,
        [_IOC_NR(DEVDRV_ALLOC_MODEL_ID)] = devdrv_ioctl_alloc_model,
        [_IOC_NR(DEVDRV_FREE_MODEL_ID)] = devdrv_ioctl_free_model,
        [_IOC_NR(DEVDRV_ALLOC_NOTIFY_ID)] = devdrv_alloc_notify_id,
        [_IOC_NR(DEVDRV_FREE_NOTIFY_ID)] = devdrv_free_notify_id,
        [_IOC_NR(DEVDRV_ALLOC_TASK_ID)] = devdrv_ioctl_alloc_task,
        [_IOC_NR(DEVDRV_FREE_TASK_ID)] = devdrv_ioctl_free_task,
};  //lint !e167

static const struct pci_device_id devdrv_driver_tbl[] = {
    { PCI_VDEVICE(HUAWEI, 0xd100),           0 },
    { PCI_VDEVICE(HUAWEI, PCI_DEVICE_CLOUD), 0 },
    { PCI_VDEVICE(HUAWEI, 0xd801),           0 },
    {}
};
MODULE_DEVICE_TABLE(pci, devdrv_driver_tbl);

#define devdrv_cce_context_list_init(cce_context, tsid) do { \
    INIT_LIST_HEAD(&((cce_context)->ts_context[(tsid)].sq_list));    \
    INIT_LIST_HEAD(&((cce_context)->ts_context[(tsid)].cq_list));    \
    INIT_LIST_HEAD(&((cce_context)->ts_context[(tsid)].stream_list)); \
    INIT_LIST_HEAD(&((cce_context)->ts_context[(tsid)].event_list));  \
    INIT_LIST_HEAD(&((cce_context)->ts_context[(tsid)].model_list));  \
    INIT_LIST_HEAD(&((cce_context)->ts_context[(tsid)].notify_list)); \
    INIT_LIST_HEAD(&((cce_context)->ts_context[(tsid)].task_list));   \
} while (0)

struct devdrv_info *devdrv_get_devdrv_info(u32 devid)
{
    return devdrv_manager_get_devdrv_info(devid);
}

u32 *devdrv_doorbell_get_cq_head(struct devdrv_info *dev_info, u32 tsid, u32 index)
{
    struct devdrv_platform_data *pdata = dev_info->pdata;
    u8 *ptr = (u8 *)pdata->ts_pdata[tsid].doorbell_vaddr;
    u32 len;

    len = (index + DEVDRV_TS_DOORBELL_SQ_NUM) * DEVDRV_TS_DOORBELL_STRIDE;
    ptr = ptr + len;

    return (u32 *)ptr;
}

STATIC struct devdrv_cce_context *devdrv_find_process(const struct devdrv_info *dev_info, pid_t pid)
{
    struct rb_node *node = dev_info->rb_root.rb_node;

    while (node != NULL) {
        struct devdrv_cce_context *context = NULL;
        context = rb_entry(node, struct devdrv_cce_context, rb_node);
        if (pid < context->pid)
            node = node->rb_left;
        else if (pid > context->pid)
            node = node->rb_right;
        else
            return context;
    }
    return NULL;
}

int devdrv_set_cce_context_status(pid_t pid, u32 dev_id, u32 status)
{
    struct devdrv_cce_context *context = NULL;
    struct devdrv_info *dev_info = NULL;

    if (pid <= 0) {
        devdrv_drv_err("wrong pid, pid = %d\n", pid);
        return -ESRCH;
    }

    dev_info = devdrv_manager_get_devdrv_info(dev_id);
    if (dev_info == NULL) {
        devdrv_drv_err("get dev_info failed, "
                       "dev_id = %u,(dev_info == NULL) = %d\n",
                       dev_id, (dev_info == NULL));
        return -ENODEV;
    }
    if (dev_info->plat_type == DEVDRV_MANAGER_DEVICE_ENV)
        return -ESRCH;

    spin_lock_bh(&dev_info->spinlock);
    context = devdrv_find_process(dev_info, pid);
    if (context == NULL) {
        spin_unlock_bh(&dev_info->spinlock);
        devdrv_drv_info("[dev_id=%u]:not find process, pid = %d\n", dev_id, pid);
        return -EFAULT;
    }
    context->process_status = status;
    spin_unlock_bh(&dev_info->spinlock);

    devdrv_drv_info("set process pid = %d, status = %u\n", context->pid, status);
    return 0;
}
EXPORT_SYMBOL(devdrv_set_cce_context_status);

STATIC u32 devdrv_get_cce_context_status(const struct devdrv_cce_context *context)
{
    struct devdrv_info *dev_info = NULL;
    u32 process_status;
    u32 tsid = 0;

    dev_info = context->cce_ctrl[tsid]->dev_info;

    spin_lock_bh(&dev_info->spinlock);
    process_status = context->process_status;
    spin_unlock_bh(&dev_info->spinlock);

    return process_status;
}

STATIC int devdrv_process_insert(struct devdrv_info *dev_info, struct devdrv_cce_context *cce_context)
{
    struct rb_node **node = &dev_info->rb_root.rb_node;
    struct rb_node *parent = NULL;

    if (dev_info->plat_type == DEVDRV_MANAGER_DEVICE_ENV)
        return 0;

    spin_lock_bh(&dev_info->spinlock);
    while (*node) {
        struct devdrv_cce_context *context = NULL;
        parent = *node;
        context = rb_entry(parent, struct devdrv_cce_context, rb_node);
        if (cce_context->pid < context->pid) {
            node = &(*node)->rb_left;
        } else if (cce_context->pid > context->pid) {
            node = &(*node)->rb_right;
        } else {
            spin_unlock_bh(&dev_info->spinlock);
            devdrv_drv_err("pid already exists\n");
            return -EFAULT;
        }
    }
    rb_link_node(&cce_context->rb_node, parent, node);
    rb_insert_color(&cce_context->rb_node, &dev_info->rb_root);
    spin_unlock_bh(&dev_info->spinlock);

    return 0;
}

STATIC void devdrv_delete_process(struct devdrv_info *dev_info, struct devdrv_cce_context *cce_context)
{
    struct devdrv_cce_context *tmp_cce_context = NULL;

    if (dev_info->plat_type == DEVDRV_MANAGER_DEVICE_ENV)
        return;

    spin_lock_bh(&dev_info->spinlock);
    tmp_cce_context = devdrv_find_process(dev_info, cce_context->pid);
    if (tmp_cce_context == NULL) {
        spin_unlock_bh(&dev_info->spinlock);
        devdrv_drv_err("find process failed\n");
        return;
    }

    rb_erase(&cce_context->rb_node, &dev_info->rb_root);
    RB_CLEAR_NODE(&cce_context->rb_node);
    spin_unlock_bh(&dev_info->spinlock);
}

STATIC int devdrv_cce_context_init(struct file *filep, struct devdrv_info *dev_info,
                                   struct devdrv_cce_context *cce_context, u32 tsid)
{
    struct devdrv_cq_sub_info *cq_sub_info = NULL;
    struct devdrv_ts_cq_info *cq_info = NULL;
    struct devdrv_cce_ctrl *cce_ctrl = NULL;
    int ret;

    if (dev_info->ts_mng[tsid].ts_work_status == NULL) {
        devdrv_drv_err("tsid=%d, ts_work_status is NULL, driver is not ready!!!!\n", tsid);
        return -ENOMEM;
    }

    cce_ctrl = dev_info->cce_ctrl[tsid];
    if (cce_ctrl == NULL) {
        devdrv_drv_err("tsid=%d, cce ctrl is null.\n", tsid);
        return -ENODEV;
    }

    if (devdrv_is_low_power(cce_ctrl)) {
        devdrv_drv_err("tsid=%d, device is at low power mode.\n", tsid);
        return -ENODEV;
    }

#ifndef CFG_SOC_PLATFORM_MINIV2
    if (devdrv_is_ts_work(&dev_info->ts_mng[tsid]) == 0) {
        devdrv_drv_err("tsid=%d, device is not working.\n", tsid);
        return -ENODEV;
    }
#endif /* CFG_SOC_PLATFORM_MINIV2 */

    devdrv_cce_context_list_init(cce_context, tsid);
    cce_context->ts_context[tsid].stream_num = 0;
    cce_context->ts_context[tsid].event_num = 0;
    cce_context->ts_context[tsid].sq_num = 0;
    cce_context->ts_context[tsid].cq_num = 0;
    cce_context->ts_context[tsid].model_id_num = 0;
    cce_context->ts_context[tsid].task_id_num = 0;
    cce_context->ts_context[tsid].notify_id_num = 0;
    cce_context->ts_context[tsid].send_count = 0;
    cce_context->ts_context[tsid].receive_count = 0;
    cce_context->ts_context[tsid].last_ts_status = DEVDRV_TS_WORK;
    cce_context->ts_context[tsid].cq_tail_updated = 0;
    cce_context->ts_context[tsid].cce_context = cce_context;
    cce_context->ts_context[tsid].tsid = tsid;

    init_waitqueue_head(&cce_context->ts_context[tsid].report_wait);
    INIT_LIST_HEAD(&cce_context->ts_context[tsid].message_list_header);

    spin_lock_init(&cce_context->ts_context[tsid].mailbox_wait_spinlock);
    sema_init(&cce_context->ts_context[tsid].mailbox_wait, 0);

    cce_context->ts_context[tsid].mailbox_message_count.counter = 0;
    cce_context->cce_ctrl[tsid] = cce_ctrl;

    mutex_lock(&cce_ctrl->cq_mutex_t);
    cq_info = devdrv_get_new_cq(cce_ctrl, cce_context);
    if (cq_info == NULL) {
        mutex_unlock(&cce_ctrl->cq_mutex_t);
        devdrv_drv_err("tsid=%d, alloc persistent cq for cce_context failed.\n", tsid);
        return -ENOMEM;
    }
    mutex_unlock(&cce_ctrl->cq_mutex_t);

    cq_info->stream_num = 0;
    cq_sub_info = (struct devdrv_cq_sub_info *)cq_info->cq_sub;

    ret = devdrv_init_cq_uio(cce_ctrl, cq_sub_info, DEVDRV_MAX_CQ_DEPTH * cq_info->slot_size);
    if (ret != 0) {
        devdrv_drv_err("tsid=%d, init uio for persistent cq failed.\n", tsid);
        devdrv_free_cq(cce_ctrl, cce_context, cq_info);
        return -ENOMEM;
    }

    return 0;
}

STATIC int devdrv_open(struct inode *inode, struct file *filep)
{
    struct devdrv_manager_info *manager_info = devdrv_get_manager_info();
    struct devdrv_cce_context *cce_context = NULL;
    struct devdrv_info *dev_info = NULL;
    unsigned int minor = iminor(inode);
    u32 tsid;
    u32 ts_num;
    int ret;

    if ((manager_info == NULL) || (minor >= DEVDRV_MAX_DAVINCI_NUM)) {
        devdrv_drv_err("invalid parameter, minor = %d\n", minor);
        return -ENODEV;
    }

    dev_info = manager_info->dev_info[minor];
    if (dev_info == NULL) {
        devdrv_drv_err("minor = %d, dev_info NULL !!!!\n", minor);
        return -ENOMEM;
    }

    if (dev_info->status == 1) {
        devdrv_drv_warn("dev %d has been reset\n", dev_info->dev_id);
        return -EINVAL;
    }

    ts_num = devdrv_manager_get_ts_num(dev_info);
    if ((ts_num == (u32)-1) || (ts_num > DEVDRV_MAX_TS_NUM)) {
        devdrv_drv_err("ts_num(%u) error.\n", ts_num);
        return -ENODEV;
    }

    cce_context = kzalloc(sizeof(struct devdrv_cce_context), GFP_KERNEL);
    if (cce_context == NULL) {
        devdrv_drv_err("alloc memory for cce_context failed.\n");
        return -ENOMEM;
    }

    cce_context->pid = current->tgid;
    cce_context->devid = dev_info->dev_id;
    cce_context->ipc_port = -1;
    cce_context->process_status = DEVMM_SVM_NORMAL_EXITED_FLAG;
    cce_context->should_stop_thread = 0;

    mutex_init(&cce_context->stream_mutex);
    INIT_LIST_HEAD(&cce_context->ipc_msg_send_head);
    INIT_LIST_HEAD(&cce_context->ipc_msg_return_head);

    for (tsid = 0; tsid < ts_num; tsid++) {
        ret = devdrv_cce_context_init(filep, dev_info, cce_context, tsid);
        if (ret) {
            kfree(cce_context);
            cce_context = NULL;
            devdrv_drv_err("minor=%d, devdrv_cce_context_init failed\n", minor);
            return -ENOMEM;
        }
    }
    spin_lock_bh(&dev_info->spinlock);
    list_add(&cce_context->cce_ctrl_list, &dev_info->cce_context_list);
    spin_unlock_bh(&dev_info->spinlock);

    if (devdrv_process_insert(dev_info, cce_context)) {
        kfree(cce_context);
        cce_context = NULL;
        devdrv_drv_err("devdrv_process_insert failed\n");
        return -ENOMEM;
    }
    filep->private_data = cce_context;

    return 0;
}

STATIC int devdrv_report_wait(struct devdrv_cce_context *cce_context, u32 tsid, int timeout)
{
    unsigned long tmp;
    int ret;

    if (timeout == -1) {
        wait_event(cce_context->ts_context[tsid].report_wait,
                   cce_context->ts_context[tsid].cq_tail_updated == CQ_HEAD_UPDATE_FLAG);
        ret = 1;
    } else if (timeout > 0) {
        tmp = msecs_to_jiffies(timeout);

        ret = wait_event_interruptible_timeout(cce_context->ts_context[tsid].report_wait,
                                               cce_context->ts_context[tsid].cq_tail_updated == CQ_HEAD_UPDATE_FLAG,
                                               tmp);
        if (ret > 0) {
            ret = 1; /* condition became true */
        } else if (ret < 0) {
            devdrv_drv_info("wait_event_interruptible return: %d.\n", ret);
        }
    } else {
        ret = -EINVAL;
    }

    return ret;
}

STATIC void devdrv_resource_leak_print(struct devdrv_cce_ctrl *cce_ctrl, struct devdrv_cce_context *cce_context,
                                       u32 tsid)
{
    devdrv_drv_warn("SOME RESOURCE are not released. Process Name: %s "
                    "PID: %d, TGID: %d\n",
                    current->comm, current->pid, current->tgid);
    if (!list_empty_careful(&cce_context->ts_context[tsid].message_list_header))
        devdrv_drv_warn("message_list_header is NOT empty\n");
    if (atomic_read(&cce_context->ts_context[tsid].mailbox_message_count))
        devdrv_drv_warn("mailbox_message_count is NOT zero\n");
    if (!list_empty_careful(&cce_context->ts_context[tsid].stream_list))
        devdrv_drv_warn("some stream id are not released !! stream num = %d\n",
                        cce_context->ts_context[tsid].stream_num);
    if (!list_empty_careful(&cce_context->ts_context[tsid].event_list))
        devdrv_drv_warn("some event id are not released !! event num = %d\n", cce_context->ts_context[tsid].event_num);
    if (!list_empty_careful(&cce_context->ts_context[tsid].model_list))
        devdrv_drv_warn("some model id are not released !! model num = %d\n",
                        cce_context->ts_context[tsid].model_id_num);
    if (!list_empty_careful(&cce_context->ts_context[tsid].notify_list))
        devdrv_drv_warn("some notify id are not released !! notify num = %d\n",
                        cce_context->ts_context[tsid].notify_id_num);
    if (!list_empty_careful(&cce_context->ts_context[tsid].task_list))
        devdrv_drv_warn("some task id are not released !! task num = %d\n", cce_context->ts_context[tsid].task_id_num);
}

STATIC int devdrv_cce_context_list_empty_check(struct devdrv_cce_context *cce_context, u32 tsid)
{
    if (!list_empty_careful(&cce_context->ts_context[tsid].message_list_header) ||
        atomic_read(&cce_context->ts_context[tsid].mailbox_message_count) ||
        !list_empty_careful(&cce_context->ts_context[tsid].stream_list) ||
        !list_empty_careful(&cce_context->ts_context[tsid].event_list) ||
        !list_empty_careful(&cce_context->ts_context[tsid].model_list) ||
        !list_empty_careful(&cce_context->ts_context[tsid].notify_list) ||
        !list_empty_careful(&cce_context->ts_context[tsid].task_list)) {
        return 0;
    }

    return -1;
}

STATIC int devdrv_cce_context_deinit(struct file *filep, struct devdrv_info *dev_info,
                                     struct devdrv_cce_context *cce_context, u32 tsid)
{
    struct devdrv_cq_sub_info *cq_sub_info = NULL;
    struct devdrv_ts_cq_info *cq_info = NULL;
    struct devdrv_cce_ctrl *cce_ctrl = NULL;
    phys_addr_t info_mem_addr;
    int result = 0;

    cce_ctrl = dev_info->cce_ctrl[tsid];
    if (cce_ctrl == NULL) {
        devdrv_drv_err("null pointer cce_ctrl.\n");
        return -EINVAL;
    }
    up(&cce_context->ts_context[tsid].mailbox_wait);

    if (!devdrv_cce_context_list_empty_check(cce_context, tsid)) {
        filep->private_data = NULL;
        devdrv_resource_leak_print(cce_ctrl, cce_context, tsid);

        /* start a work queue to recycle source */
        devdrv_drv_warn("detect some sources are not released, "
                        "start a work for recycling tsid(%u).\n",
                        tsid);
        (void)devdrv_send_recycle_info_to_ts(dev_info->dev_id, tsid, current->tgid, &result);
        atomic_inc(&cce_ctrl->recycle_work_num);
        INIT_WORK(&cce_context->ts_context[tsid].recycle_work, devdrv_recycle_work);
        queue_work(cce_context->cce_ctrl[tsid]->wq, &cce_context->ts_context[tsid].recycle_work);

        return RECYCLE_RESOURCE;
    }

    if (!list_empty_careful(&cce_context->ts_context[tsid].cq_list)) {
        cq_sub_info = list_first_entry(&cce_context->ts_context[tsid].cq_list, struct devdrv_cq_sub_info, list);
        info_mem_addr = cce_ctrl->mem_info[DEVDRV_INFO_MEM].virt_addr;
        cq_info = devdrv_calc_cq_info(info_mem_addr, cq_sub_info->index);
        devdrv_exit_cq_uio(cce_ctrl, cq_info);
        devdrv_free_cq(cce_ctrl, cce_context, cq_info);
    }

    return 0;
}

STATIC int devdrv_release(struct inode *inode, struct file *filep)
{
    struct devdrv_manager_info *manager_info = devdrv_get_manager_info();
    struct devdrv_cce_context *cce_context = NULL;
    unsigned int minor = iminor(inode);
    struct devdrv_info *dev_info = NULL;
    u32 tsid;
    u32 ts_num;
    int ret;

    if ((manager_info == NULL) || (minor >= DEVDRV_MAX_DAVINCI_NUM)) {
        devdrv_drv_err("invalid parameter, minor = %d\n", minor);
        return -ENODEV;
    }
    dev_info = manager_info->dev_info[minor];
    if (dev_info == NULL) {
        devdrv_drv_err("minor = %d, dev_info NULL !!!!\n", minor);
        return -ENOMEM;
    }

    ts_num = devdrv_manager_get_ts_num(dev_info);
    if (ts_num > DEVDRV_MAX_TS_NUM) {
        devdrv_drv_err("ts_num(%u) error.\n", ts_num);
        return -ENODEV;
    }

    cce_context = filep->private_data;
    if (cce_context == NULL) {
        devdrv_drv_err("get cce_context fail.\n");
        return -EINVAL;
    }

    devdrv_delete_process(dev_info, cce_context);

    atomic_inc(&dev_info->occupy_ref);
    if (dev_info->status == 1) {
        devdrv_drv_warn("dev %d has been reset\n", dev_info->dev_id);
    } else {
        cce_context->should_stop_thread = 1;
        mutex_destroy(&cce_context->stream_mutex);

        for (tsid = 0; tsid < ts_num; tsid++) {
            ret = devdrv_cce_context_deinit(filep, dev_info, cce_context, tsid);
            if (ret == RECYCLE_RESOURCE) {
                atomic_dec(&dev_info->occupy_ref);
                devdrv_drv_info("recycling resources, tsid(%u)\n", tsid);
                return 0;
            }
            if (ret != 0) {
                atomic_dec(&dev_info->occupy_ref);
                devdrv_drv_err("devdrv_cce_context_deinit failed\n");
                return -ENOMEM;
            }
        }
    }

    spin_lock_bh(&dev_info->spinlock);
    list_del(&cce_context->cce_ctrl_list);
    spin_unlock_bh(&dev_info->spinlock);

    kfree(cce_context);
    cce_context = NULL;

    atomic_dec(&dev_info->occupy_ref);

    return 0;
}

STATIC int devdrv_ioctl_report_wait(struct devdrv_cce_ctrl *cce_ctrl, struct devdrv_cce_context *cce_context,
                                    struct devdrv_ioctl_arg *arg)
{
    struct devdrv_cq_sub_info *cq_sub_info = NULL;
    struct devdrv_ts_cq_info *cq_info = NULL;
    struct devdrv_report *report = NULL;
    struct devdrv_info *dev_info = NULL;
    struct uio_mem *mem = NULL;
    u32 tsid = cce_ctrl->tsid;
    phys_addr_t cq_base_addr;
    unsigned long flags;
    u32 phase;
    int ret;

    cq_info = devdrv_get_cq_exist(cce_ctrl, cce_context);
    if (cq_info == NULL) {
        devdrv_drv_err("[dev_id=%u]:cce_context got no cq, check it first !!\n", cce_ctrl->devid);
        return -EFAULT;
    }

    cq_sub_info = (struct devdrv_cq_sub_info *)cq_info->cq_sub;
    if (cq_sub_info->uio == NULL) {
        devdrv_drv_err("[dev_id=%u]:cq does not have uio, check it first !!\n", cce_ctrl->devid);
        return -EFAULT;
    }

    spin_lock_irqsave(&cq_sub_info->spinlock, flags);
    dev_info = cq_sub_info->cce_context->cce_ctrl[tsid]->dev_info;
    mem = cq_sub_info->uio->mem;

    cq_base_addr = mem[DEVDRV_SQ_CQ_MAP].addr;
    report = (struct devdrv_report *)(uintptr_t)(mem[DEVDRV_SQ_CQ_MAP].addr +
                                                 (long)(unsigned)cq_info->slot_size * cq_info->tail);
    phase = devdrv_report_get_phase(report);
    if (cq_info->phase == phase) {
        spin_unlock_irqrestore(&cq_sub_info->spinlock, flags);
        ret = 1;
    } else {
        cce_context->ts_context[tsid].cq_tail_updated = 0;
        spin_unlock_irqrestore(&cq_sub_info->spinlock, flags);
        ret = devdrv_report_wait(cce_context, tsid, arg->report_para.timeout);
        if (devdrv_is_ts_work(&dev_info->ts_mng[tsid]) == 0) {
            devdrv_drv_err("ts is down,ret:%d.\n", ret);
            ret = DEVDRV_BUS_DOWN;
        } else if (devdrv_get_cce_context_status(cce_context) == DEVMM_SVM_ABNORMAL_EXITED_FLAG) {
            devdrv_drv_err("[dev_id=%u]:detected device process hang pid = %d, report it to runtime process\n",
                           cce_ctrl->devid, cce_context->pid);
            ret = (int)DEDVRV_DEV_PROCESS_HANG;
        }
    }
    arg->report_para.timeout = ret;

    return 0;
}

STATIC int devdrv_get_occupy_stream_id(struct devdrv_cce_context *cce_context, unsigned long arg)
{
    struct devdrv_stream_sub_info *stream_sub_info = NULL;
    struct devdrv_occupy_stream_id *stream_id = NULL;
    struct list_head *pos = NULL, *n = NULL;
    struct devdrv_cce_ctrl *cce_ctrl = NULL;
    u32 tsid;
    int ret;

    stream_id = kzalloc(sizeof(struct devdrv_occupy_stream_id), GFP_KERNEL);
    if (stream_id == NULL) {
        devdrv_drv_err("[dev_id=%u]:kmalloc fail.\n", cce_context->devid);
        return -ENOMEM;
    }
    ret = copy_from_user_safe(stream_id, (void __user *)(uintptr_t)arg, sizeof(struct devdrv_occupy_stream_id));
    if (ret) {
        devdrv_drv_err("copy from suer fail\n");
        kfree(stream_id);
        return -ENOMEM;
    }
    tsid = stream_id->tsid;
    if (tsid >= DEVDRV_MAX_TS_NUM) {
        devdrv_drv_err("invalid tsid(%u)\n", tsid);
        kfree(stream_id);
        return -EFAULT;
    }
    cce_ctrl = cce_context->cce_ctrl[tsid];

    spin_lock(&cce_ctrl->spinlock);
    list_for_each_safe(pos, n, &cce_context->ts_context[tsid].stream_list) {
        stream_sub_info = list_entry(pos, struct devdrv_stream_sub_info, list);
        if (stream_id->count >= DEVDRV_MAX_STREAM_ID) {
            spin_unlock(&cce_ctrl->spinlock);
            devdrv_drv_err("id count is bigger than max stream id\n");
            kfree(stream_id);
            stream_id = NULL;
            return -EINVAL;
        }
        stream_id->id[stream_id->count] = stream_sub_info->id;
        stream_id->count++;
    }
    spin_unlock(&cce_ctrl->spinlock);

    ret = copy_to_user_safe((void __user *)(uintptr_t)arg, stream_id, sizeof(struct devdrv_occupy_stream_id));
    if (ret) {
        devdrv_drv_err("[dev_id=%u]:copy_to_user_safe fail.\n", cce_context->devid);
        kfree(stream_id);
        stream_id = NULL;
        return -ENOMEM;
    }
    kfree(stream_id);
    stream_id = NULL;

    return 0;
}

STATIC int devdrv_ioctl_alloc_stream(struct devdrv_cce_ctrl *cce_ctrl, struct devdrv_cce_context *cce_context,
                                     struct devdrv_ioctl_arg *arg)
{
    struct devdrv_stream_info *stream_info = NULL;
    int no_stream = 0;
    int stream_id;

    mutex_lock(&cce_context->stream_mutex);
    stream_info = devdrv_alloc_stream(cce_ctrl, cce_context, &no_stream, arg->stream_para.strategy);
    if (stream_info != NULL)
        stream_id = stream_info->id;
    else
        stream_id = DEVDRV_MAX_STREAM_ID;

    if ((stream_info == NULL) && !no_stream) {
        mutex_unlock(&cce_context->stream_mutex);
        devdrv_drv_err("devdrv_alloc_stream failed.\n");
        return -ENOKEY;
    }
    arg->stream_para.stream_id = stream_id;
    mutex_unlock(&cce_context->stream_mutex);

    return 0;
}

STATIC int devdrv_ioctl_free_stream(struct devdrv_cce_ctrl *cce_ctrl, struct devdrv_cce_context *cce_context,
                                    struct devdrv_ioctl_arg *arg)
{
    int stream_id;
    int ret;

    stream_id = arg->stream_para.stream_id;
    ret = devdrv_free_stream(cce_ctrl, cce_context, arg->stream_para.stream_id, DEVDRV_HAVE_TO_INFORM);
    if (ret) {
        devdrv_drv_err("free stream fail, ret(%d), stream_id(%d)\n", ret, stream_id);
    }

    return ret;
}

STATIC int devdrv_ioctl_alloc_event(struct devdrv_cce_ctrl *cce_ctrl, struct devdrv_cce_context *cce_context,
                                    struct devdrv_ioctl_arg *arg)
{
    struct devdrv_event_info *event_id = NULL;

    event_id = devdrv_alloc_event_id(cce_ctrl, cce_context);
    if (event_id == NULL) {
        devdrv_drv_err("alloc event fail\n");
        arg->event_para.event_id = DEVDRV_MAX_EVENT_ID;
        return -EFAULT;
    }
    arg->event_para.event_id = event_id->id;

    return 0;
}

STATIC int devdrv_ioctl_free_event(struct devdrv_cce_ctrl *cce_ctrl, struct devdrv_cce_context *cce_context,
                                   struct devdrv_ioctl_arg *arg)
{
    int ret;

    ret = devdrv_free_event_id(cce_ctrl, cce_context, arg->event_para.event_id);
    if (ret) {
        devdrv_drv_err("free event fail, ret(%d)\n", ret);
        return ret;
    }

    return 0;
}

STATIC int devdrv_ioctl_alloc_model(struct devdrv_cce_ctrl *cce_ctrl, struct devdrv_cce_context *cce_context,
                                    struct devdrv_ioctl_arg *arg)
{
    int model_id;

    model_id = devdrv_alloc_model_id(cce_ctrl, cce_context);
    if (model_id >= DEVDRV_MAX_MODEL_ID) {
        devdrv_drv_err("alloc model id fail model_id(%d)\n", model_id);
        return -EINVAL;
    }
    arg->model_para.model_id = model_id;

    return 0;
}

STATIC int devdrv_ioctl_free_model(struct devdrv_cce_ctrl *cce_ctrl, struct devdrv_cce_context *cce_context,
                                   struct devdrv_ioctl_arg *arg)
{
    int ret;

    ret = devdrv_free_model_id(cce_ctrl, cce_context, arg->model_para.model_id);
    if (ret) {
        devdrv_drv_err("free model id fail ret(%d)\n", ret);
        return ret;
    }

    return 0;
}

STATIC int devdrv_ioctl_alloc_task(struct devdrv_cce_ctrl *cce_ctrl, struct devdrv_cce_context *cce_context,
                                   struct devdrv_ioctl_arg *arg)
{
    int task_id;

    task_id = devdrv_alloc_task_id(cce_ctrl, cce_context);
    if (task_id >= DEVDRV_MAX_TASK_ID) {
        devdrv_drv_err("alloc task id fail task_id(%d)", task_id);
        return -EINVAL;
    }
    arg->taskpool_para.taskpool_id = task_id;

    return 0;
}

STATIC int devdrv_ioctl_free_task(struct devdrv_cce_ctrl *cce_ctrl, struct devdrv_cce_context *cce_context,
                                  struct devdrv_ioctl_arg *arg)
{
    int ret;

    ret = devdrv_free_task_id(cce_ctrl, cce_context, arg->taskpool_para.taskpool_id);
    if (ret) {
        devdrv_drv_err("free task id fail ret(%d)\n", ret);
        return -EINVAL;
    }
    return 0;
}

STATIC int devdrv_ioctl_ipcmsg_rcv_async(struct devdrv_cce_ctrl *cce_ctrl, struct devdrv_cce_context *cce_context,
                                         unsigned long arg)
{
    struct devdrv_info *dev_info = cce_ctrl->dev_info;
    int ret = -EINVAL;

    if (dev_info->plat_type == DEVDRV_MANAGER_HOST_ENV)
        return -EINVAL;

    if (dev_info->drv_ops->ipc_msg_recv_async != NULL)
        ret = dev_info->drv_ops->ipc_msg_recv_async(dev_info, arg);

    return ret;
}

STATIC int devdrv_ioctl_ipcmsg_send_async(struct devdrv_cce_ctrl *cce_ctrl, struct devdrv_cce_context *cce_context,
                                          unsigned long arg)
{
    struct devdrv_info *dev_info = cce_ctrl->dev_info;
    int ret = -EINVAL;

    if (dev_info->plat_type == DEVDRV_MANAGER_HOST_ENV)
        return -EINVAL;
    if (dev_info->drv_ops->ipc_msg_send_async != NULL)
        ret = dev_info->drv_ops->ipc_msg_send_async(dev_info, arg);

    return ret;
}

STATIC long tsdrv_ioctl(unsigned int cmd, void __user *uarg, struct devdrv_cce_context *cce_context)
{
    struct devdrv_ioctl_arg ioctl_arg = {0};
    struct devdrv_cce_ctrl *cce_ctrl = NULL;
    struct devdrv_info *dev_info = NULL;
    int ret;
    if (devdrv_ioctl_handlers[_IOC_NR(cmd)] == NULL) {
        devdrv_drv_err("invalid cmd(%u)\n", _IOC_NR(cmd));
        return -EFAULT;
    }

    if (_IOC_DIR(cmd) & _IOC_WRITE) {
        if (copy_from_user_safe(&ioctl_arg, uarg, sizeof(struct devdrv_ioctl_arg))) {
            devdrv_drv_err("copy from user fail cmd(%u)\n", _IOC_NR(cmd));
            return -ENOMEM;
        }
    }

    ret = (ioctl_arg.tsid >= DEVDRV_MAX_TS_NUM) ? 1 : (cce_context->cce_ctrl[ioctl_arg.tsid] == NULL);
    if (ret) {
        devdrv_drv_err("param check fail tsid(%u) cmd(%u).\n", ioctl_arg.tsid, _IOC_NR(cmd));
        return -EFAULT;
    }

    cce_ctrl = cce_context->cce_ctrl[ioctl_arg.tsid];

    dev_info = cce_ctrl->dev_info;
    atomic_inc(&dev_info->occupy_ref);
    if (dev_info->status == 1) {
        atomic_dec(&dev_info->occupy_ref);
        devdrv_drv_warn("dev %d has been reset\n", dev_info->dev_id);
        return -EINVAL;
    }

    ret = devdrv_ioctl_handlers[_IOC_NR(cmd)](cce_ctrl, cce_context, &ioctl_arg);
    atomic_dec(&dev_info->occupy_ref);
    if (ret) {
        devdrv_drv_err("handle fail cmd(%u)\n", _IOC_NR(cmd));
        return -ENODEV;
    }

    if (_IOC_DIR(cmd) & _IOC_READ) {
        if (copy_to_user_safe(uarg, &ioctl_arg, sizeof(struct devdrv_ioctl_arg))) {
            devdrv_drv_err("copy to user fail cmd(%u)\n", _IOC_NR(cmd));
            return -EFAULT;
        }
    }
    return 0;
}

STATIC long devdrv_ioctl(struct file *filep, unsigned int cmd, unsigned long arg)
{
    struct devdrv_cce_context *cce_context = filep->private_data;
    void __user *uarg = (void __user *)(uintptr_t)arg;
    struct devdrv_cce_ctrl *cce_ctrl = NULL;
    int ret;

    ret = (_IOC_TYPE(cmd) != DEVDRV_ID_MAGIC) || (_IOC_NR(cmd) >= DEVDRV_MAX_CMD)
        || (uarg == NULL);
    if (ret) {
        devdrv_drv_err("cmd(%u) not supported.(%d)\n", _IOC_NR(cmd),
            uarg == NULL ? 1 : 0);
        return -EINVAL;
    }

    /* DEVDRV_GET_OCCUPY_STREAM_ID didn't use cce_ctrl, so didn't use lock for hot_reset
       DEVDRV_TSMSG_SEND_ASYNC and DEVDRV_TSMSG_RECV_ASYNC are used in device, didn't use lock either */
    if (_IOC_NR(cmd) == _IOC_NR(DEVDRV_GET_OCCUPY_STREAM_ID)) {
        return devdrv_get_occupy_stream_id(cce_context, arg);
    } else if ((_IOC_NR(cmd) >= _IOC_NR(DEVDRV_TSMSG_SEND_ASYNC)) &&
               (_IOC_NR(cmd) <= _IOC_NR(DEVDRV_TSMSG_RECV_ASYNC))) {
        cce_ctrl = cce_context->cce_ctrl[0];
        if (devdrv_ioctl_ipc_handlers[_IOC_NR(cmd)] != NULL)
            return devdrv_ioctl_ipc_handlers[_IOC_NR(cmd)](cce_ctrl, cce_context, arg);
    }

    return tsdrv_ioctl(cmd, uarg, cce_context);
}

/* mini mmap!! */
STATIC void devdrv_vm_open(struct vm_area_struct *vma)
{
    devdrv_drv_debug("devdrv vm open\n");
}

STATIC void devrv_vm_close(struct vm_area_struct *vma)
{
    devdrv_drv_debug("devdrv vm close\n");
}

STATIC int devdrv_vm_get_addr_size(const struct devdrv_info *dev_info, unsigned long offset, phys_addr_t *virt_addr,
                                   phys_addr_t *phy_addr, size_t *size)
{
    struct devdrv_cce_ctrl *cce_ctrl = dev_info->cce_ctrl[0];
    struct devdrv_cq_sub_info *cq_sub_info = NULL;
    struct devdrv_ts_cq_info *cq_info = NULL;
    u32 cq_index;

    cq_index = (offset - DEVDRV_VM_CQ_BLOCK_OFFSET) / DEVDRV_VM_CQ_SLOT_SIZE;
    if (cq_index >= DEVDRV_MAX_CQ_NUM) {
        devdrv_drv_err("[dev_id=%u]:cq index error, cq_index = %d\n", dev_info->dev_id, cq_index);
        return -EINVAL;
    }

    cq_info = devdrv_calc_cq_info(cce_ctrl->mem_info[DEVDRV_INFO_MEM].virt_addr, cq_index);
    if (cq_info->index != cq_index) {
        devdrv_drv_err("[dev_id=%u]:got wrong cq index, cq_info->index = %d\n", dev_info->dev_id, cq_info->index);
        return -ENOMEM;
    }

    cq_sub_info = cq_info->cq_sub;

    *virt_addr = cq_sub_info->virt_addr;
    *phy_addr = virt_to_phys((void *)(uintptr_t)cq_sub_info->virt_addr);
    *size = (size_t)cq_info->slot_size * DEVDRV_MAX_CQ_DEPTH;

    devdrv_drv_debug("cq_index = %d, "
                     "cq_sub_info->index = %d, "
                     "size = %lu\n",
                     cq_index, cq_sub_info->index, *size);

    return 0;
}

STATIC int devdrv_vm_fault(struct vm_area_struct *vma, struct vm_fault *vmf)
{
    struct devdrv_cce_context *cce_context = vma->vm_private_data;
    struct devdrv_cce_ctrl *cce_ctrl = cce_context->cce_ctrl[0];
    struct devdrv_info *dev_info = cce_ctrl->dev_info;
    unsigned long start_addr;
    phys_addr_t virt_addr;
    pgprot_t vm_page_prot;
    phys_addr_t phy_addr;
    unsigned long offset;
    size_t size;
    int ret;

#ifdef CFG_SOC_PLATFORM_MINIV2
    devdrv_drv_err("shouldn't be fault here\n");
#endif /* CFG_SOC_PLATFORM_MINIV2 */

    offset = vmf->pgoff << PAGE_SHIFT;
    if ((offset < DEVDRV_VM_CQ_BLOCK_OFFSET) || (offset > DEVDRV_VM_MEM_SIZE)) {
        return -EINVAL;
    }
    start_addr = vma->vm_start + offset;

    devdrv_drv_debug("dev_id = %u, "
                     "start_addr = 0x%pK, "
                     "vma->vm_start = 0x%pK, "
                     "offset = %lu, "
                     "vmf->pgoff = %lu\n",
                     dev_info->dev_id, (void *)(uintptr_t)start_addr, (void *)(uintptr_t)vma->vm_start, offset,
                     vmf->pgoff);

    ret = devdrv_vm_get_addr_size(dev_info, offset, &virt_addr, &phy_addr, &size);
    if (ret) {
        devdrv_drv_err("[dev_id=%u]:get addr & size failed %d.\n", cce_context->devid, ret);
        return -EINVAL;
    }

    if (size <= 0) {
        devdrv_drv_err("[dev_id=%u]:get size error, size = %lu\n", cce_context->devid, size);
        return -EINVAL;
    }

    vm_page_prot = pgprot_noncached(vma->vm_page_prot);  //lint !e446
    ret = remap_pfn_range(vma, start_addr, phy_addr >> PAGE_SHIFT, size, vm_page_prot);
    if (ret) {
        devdrv_drv_err("[dev_id=%u]:map cq slot failed %d.\n", cce_context->devid, ret);
        return -EINVAL;
    }

    return VM_FAULT_NOPAGE;
}

#if LINUX_VERSION_CODE >= KERNEL_VERSION(4, 11, 0)
STATIC int devdrv_vmf_fault(struct vm_fault *vmf)
{
    return devdrv_vm_fault(vmf->vma, vmf);
}
#endif

struct vm_operations_struct devdrv_vm_ops = {
    .open = devdrv_vm_open,
    .close = devrv_vm_close,
#if LINUX_VERSION_CODE >= KERNEL_VERSION(4, 11, 0)
    .fault = devdrv_vmf_fault,
#else
    .fault = devdrv_vm_fault,
#endif
};

#ifdef CFG_SOC_PLATFORM_MINI
#ifndef CFG_SOC_PLATFORM_MINIV2
STATIC int devdrv_mmap_ts_dma_buffer(struct devdrv_cce_ctrl *cce_ctrl, struct vm_area_struct *vma)
{
    struct devdrv_info *dev_info = cce_ctrl->dev_info;
    struct devdrv_platform_data *pdata = NULL;
    unsigned long start_addr;
    unsigned long pfn;
    int err;

    if ((dev_info == NULL) || (dev_info->plat_type == DEVDRV_MANAGER_HOST_ENV)) {
        devdrv_drv_warn("invalid Para(%pK, plat_type:%d).\n", dev_info, (dev_info == NULL) ? 0 : dev_info->plat_type);
        return 0;
    }
    pdata = dev_info->pdata;
    vma->vm_page_prot = pgprot_noncached(vma->vm_page_prot);  //lint !e446
    start_addr = vma->vm_start + DEVDRV_VM_MEM_SIZE;

    pfn = page_to_pfn(virt_to_page((uintptr_t)pdata->ts_pdata[0].ts_load_addr));  //lint !e648
    err = remap_pfn_range(vma, start_addr, pfn, DEVDRV_TS_MEMORY_SIZE, vma->vm_page_prot);
    if (err) {
        devdrv_drv_err("taskpool mmap failed, "
                       "taskpool mem: virt_addr= 0x%lx, "
                       "ts_load_addr = 0x%pK, pfn(%ld),"
                       "err = %d\n",
                       start_addr, pdata->ts_pdata[0].ts_load_addr, pfn, err);
        return -EINVAL;
    }
    return 0;
}
#endif /* CFG_SOC_PLATFORM_MINIV2 */
#endif

STATIC int devdrv_cce_context_check(struct devdrv_cce_context *cce_context)
{
    struct devdrv_cce_ctrl *cce_ctrl = NULL;
    struct devdrv_info *dev_info = NULL;
    u32 ts_num;
    u32 i;
    u32 tsid = 0;

    if (cce_context == NULL) {
        devdrv_drv_err("cce_context is NULL\n");
        return -EINVAL;
    }

    if (cce_context->vma[tsid] == NULL) {
        devdrv_drv_err("cce_context vma is NULL.\n");
        return -EFAULT;
    }

    if (cce_context->cce_ctrl[tsid] == NULL) {
        devdrv_drv_err("cce_ctrl is NULL.\n");
        return -EFAULT;
    }

    ts_num = devdrv_manager_get_ts_num(cce_context->cce_ctrl[tsid]->dev_info);
    if ((ts_num == (u32)-1) || (ts_num > DEVDRV_MAX_TS_NUM)) {
        devdrv_drv_err("ts_num(%u) error.\n", ts_num);
        return -EFAULT;
    }
    for (i = 0; i < ts_num; i++) {
        cce_ctrl = cce_context->cce_ctrl[i];
        if (cce_ctrl == NULL) {
            devdrv_drv_err("cce_ctrl is NULL, i(%d).\n", i);
            return -EINVAL;
        }
    }

    if (cce_ctrl == NULL) {
        devdrv_drv_err("[dev_id=%u]:cce_ctrl is NULL\n", cce_context->devid);
        return -EINVAL;
    }
    dev_info = cce_ctrl->dev_info;
    if (dev_info == NULL) {
        devdrv_drv_err("[dev_id=%u]:dev_info is NULL\n", cce_context->devid);
        return -EINVAL;
    }

    return 0;
}

STATIC int devdrv_mmap_param_check(const struct file *filep, const struct vm_area_struct *vma)
{
    struct devdrv_cce_context *cce_context = NULL;
    struct devdrv_cce_ctrl *cce_ctrl = NULL;
    struct devdrv_info *dev_info = NULL;
    u32 ts_num;
    u32 i;
    u32 tsid = 0;

    if (filep == NULL) {
        devdrv_drv_err("filep check failed\n");
        return -ENODEV;
    }

    cce_context = filep->private_data;
    if ((cce_context == NULL) || (cce_context->cce_ctrl[tsid] == NULL)) {
        devdrv_drv_err("cce_context is null poniter\n");
        return -ENODEV;
    }

    dev_info = cce_context->cce_ctrl[tsid]->dev_info;
    if (dev_info == NULL) {
        devdrv_drv_err("dev_info is null poniter\n");
        return -ENODEV;
    }

    ts_num = devdrv_manager_get_ts_num(dev_info);
    if ((ts_num == (u32)-1) || (ts_num > DEVDRV_MAX_TS_NUM)) {
        devdrv_drv_err("ts_num(%u) error.\n", ts_num);
        return -EFAULT;
    }

    for (i = 0; i < ts_num; i++) {
        if ((cce_ctrl = cce_context->cce_ctrl[i]) == NULL) {
            devdrv_drv_err("cce_ctrl is NULL\n");
            return -EFAULT;
        }

        if ((dev_info = cce_ctrl->dev_info) == NULL) {
            devdrv_drv_err("dev_info is NULL, i(%d)\n", i);
            return -EFAULT;
        }
    }

    if ((vma == NULL) || (vma->vm_end <= vma->vm_start)) {
        devdrv_drv_err("[dev_id=%u]:vm range check failed, "
                       "vma->vm_end = 0x%lx,"
                       "vma->vm_start = 0x%lx.\n",
                       cce_context->devid, (vma == NULL ? 0x0 : vma->vm_end), (vma == NULL ? 0x0 : vma->vm_start));
        return -EINVAL;
    }

    return 0;
}

STATIC int devdrv_mmap_info_mem(struct devdrv_cce_ctrl *cce_ctrl, struct vm_area_struct *vma)
{
    unsigned long start_addr;
    phys_addr_t phy_addr;
    size_t size;
    int err;

    phy_addr = cce_ctrl->mem_info[DEVDRV_INFO_MEM].phy_addr;
    size = cce_ctrl->mem_info[DEVDRV_INFO_MEM].size;
    start_addr = vma->vm_start + DEVDRV_VM_BLOCK_OFFSET;
    if (size <= 0) {
        devdrv_drv_err("[dev_id=%u]:info mem size invalid, size = %lu\n", cce_ctrl->devid, size);
        return -ENOMEM;
    }
    err = remap_pfn_range(vma, start_addr, __phys_to_pfn(phy_addr), size, vma->vm_page_prot);
    if (err) {
        devdrv_drv_err("[dev_id=%u]:cq mmap failed, info mem: "
                       "virt_addr = 0x%lx, "
                       "size = %lu"
                       "err = %d\n",
                       cce_ctrl->devid, start_addr, size, err);
        return -EINVAL;
    }

    return 0;
}

STATIC struct devdrv_ts_cq_info *devdrv_mmap_cqinfo_get_and_check(const struct devdrv_info *dev_info,
                                                                  const struct devdrv_cce_ctrl *cce_ctrl,
                                                                  const struct devdrv_cce_context *cce_context)
{
    struct devdrv_ts_cq_info *cq_info = NULL;
    struct devdrv_cq_sub_info *cq_sub = NULL;
    phys_addr_t info_mem_addr;

    if (list_empty_careful(&cce_context->ts_context[cce_ctrl->tsid].cq_list)) {
        devdrv_drv_err("[dev_id=%u]:current process has no cq yet\n", cce_context->devid);
        return NULL;
    }

    /* only one cq for each process */
    cq_sub = list_first_entry(&cce_context->ts_context[cce_ctrl->tsid].cq_list, struct devdrv_cq_sub_info, list);
    if ((cq_sub->index >= DEVDRV_MAX_CQ_NUM) || !cq_sub->phy_addr) {
        devdrv_drv_err("[dev_id=%u]:cq index check failed, cq index = %u\n", cce_context->devid,
                       cq_sub->index);
        return NULL;
    }
    info_mem_addr = cce_ctrl->mem_info[DEVDRV_INFO_MEM].virt_addr;
    if (!info_mem_addr) {
        devdrv_drv_err("[dev_id=%u]:invalid info mem addr, addr = 0x%llx\n", cce_context->devid, info_mem_addr);
        return NULL;
    }
    cq_info = devdrv_calc_cq_info(info_mem_addr, cq_sub->index);
    if ((cq_info->cq_sub == NULL) || (cq_info->slot_size > DEVDRV_MAX_CQ_SLOT_SIZE)) {
        devdrv_drv_err("[dev_id=%u]:cq_info check failed, slot_size  = %u, cq_sub = %pK,"
                       " cq_info->index = %u, cq_sub->index = %u\n",
                       cce_context->devid, cq_info->slot_size, cq_info->cq_sub, cq_info->index, cq_sub->index);
        return NULL;
    }

    return cq_info;
}

STATIC phys_addr_t devdrv_get_base_sq_phyaddr(const struct devdrv_info *dev_info,
    const struct devdrv_cce_ctrl *cce_ctrl, u32 index)
{
    phys_addr_t phy_addr;

    switch (dev_info->plat_type) {
        case DEVDRV_MANAGER_DEVICE_ENV:
            phy_addr = cce_ctrl->mem_info[DEVDRV_SQ_MEM].phy_addr +
                       (unsigned long)index * (unsigned long)DEVDRV_SQ_SLOT_SIZE * (unsigned long)DEVDRV_MAX_SQ_DEPTH;
            break;
        case DEVDRV_MANAGER_HOST_ENV:
            phy_addr = cce_ctrl->mem_info[DEVDRV_SQ_MEM].bar_addr +
                       (unsigned long)index * (unsigned long)DEVDRV_SQ_SLOT_SIZE * (unsigned long)DEVDRV_MAX_SQ_DEPTH;
            break;
        default:
            devdrv_drv_err("dev_id = %u]:invalid platform type, plat_type = %u\n", dev_info->dev_id,
                           dev_info->plat_type);
            phy_addr = 0;
    }

    return phy_addr;
}

STATIC void devdrv_zap_vma_ptes(struct vm_area_struct *vma, unsigned long vaddr, size_t size)
{
#if LINUX_VERSION_CODE < KERNEL_VERSION(4, 18, 0)
    int ret;
    ret = zap_vma_ptes(vma, vaddr, size);
    if (ret)
        devdrv_drv_warn("zap_vma_ptes fail, va=0x%lx,"
                        " ret=%d,vma=%pK,vm_flags=0x%lx.\n",
                        vaddr, ret, vma, vma->vm_flags);

#else
    zap_vma_ptes(vma, vaddr, size);
#endif
}

STATIC int devdrv_mmap_doorbell_mem(struct devdrv_cce_context *cce_context,
    struct devdrv_cce_ctrl *cce_ctrl, u32 index)
{
    struct devdrv_info *dev_info = NULL;
    struct vm_area_struct *vma = NULL;
    pgprot_t vm_page_prot;
    unsigned long vaddr;
    phys_addr_t paddr;
    size_t size;
    int err;

    err = devdrv_cce_context_check(cce_context);
    if (err) {
        devdrv_drv_err("cce_context check failed\n");
        return -EINVAL;
    }
    if (index >= (DEVDRV_MAX_SQ_NUM + DEVDRV_MAX_CQ_NUM)) {
        devdrv_drv_err("[dev_id=%u]:invalid index = %u\n", cce_context->devid, index);
        return -EINVAL;
    }

    dev_info = cce_ctrl->dev_info;
    vma = cce_context->vma[cce_ctrl->tsid];

    vaddr = vma->vm_start + (long)(unsigned)DEVDRV_VM_DOORBELL_BLOCK_OFFSET +
            (long)(unsigned)index * DEVDRV_TS_DOORBELL_STRIDE;
    paddr = cce_ctrl->mem_info[DEVDRV_DOORBELL_MEM].phy_addr + (long)(unsigned)index * DEVDRV_TS_DOORBELL_STRIDE;
    size = PAGE_ALIGN(DEVDRV_TS_DOORBELL_STRIDE);
    if ((size == 0) || (size > DEVDRV_TS_DOORBELL_STRIDE)) {
        devdrv_drv_err("[dev_id=%u]:argment size is error, size = %lu\n", cce_context->devid, size);
        return -EINVAL;
    }
    vm_page_prot = pgprot_noncached(vma->vm_page_prot);  //lint !e446
    err = remap_pfn_range(vma, vaddr, __phys_to_pfn(paddr), size, vm_page_prot);
    if (err) {
        devdrv_drv_err("[dev_id=%u]:cq mmap failed, err = %d\n", cce_context->devid, err);
        return -EFAULT;
    }

    return 0;
}

STATIC int devdrv_mmap_sq_mem_param_check(struct devdrv_cce_context *cce_context,
    struct devdrv_cce_ctrl *cce_ctrl, u32 index)
{
    struct devdrv_info *dev_info = cce_ctrl->dev_info;
    struct vm_area_struct *vma = cce_context->vma[cce_ctrl->tsid];
    phys_addr_t phy_addr;

    if (index >= DEVDRV_MAX_SQ_NUM) {
        devdrv_drv_err("[dev_id=%u]:invalid index = %u\n", cce_context->devid, index);
        return -EINVAL;
    }
    if (vma->vm_end < vma->vm_start) {
        devdrv_drv_err("[dev_id=%u]:vm range check failed, "
                       "vma->vm_end = 0x%lx,"
                       "vma->vm_start = 0x%lx.\n",
                       cce_context->devid, vma->vm_end, vma->vm_start);
        return -ENODEV;
    }
    phy_addr = devdrv_get_base_sq_phyaddr(dev_info, cce_ctrl, index);
    if (!phy_addr) {
        devdrv_drv_err("[dev_id=%u]:invlaid sq base phy addr\n", cce_context->devid);
        return -EINVAL;
    }

    return 0;
}

STATIC void devdrv_unmap_doorbell_mem(struct devdrv_cce_ctrl *cce_ctrl, struct devdrv_cce_context *cce_context,
                                      u32 index)
{
    struct vm_area_struct *vma = NULL;
    unsigned long vaddr;
    size_t size;

    vma = cce_context->vma[cce_ctrl->tsid];
    vaddr = vma->vm_start + (long)(unsigned)DEVDRV_VM_DOORBELL_BLOCK_OFFSET +
            (long)(unsigned)index * DEVDRV_TS_DOORBELL_STRIDE;
    size = PAGE_ALIGN(DEVDRV_TS_DOORBELL_STRIDE);

    devdrv_zap_vma_ptes(vma, vaddr, size);
}

STATIC void devdrv_unmmap_sq_slot_mem(struct devdrv_cce_ctrl *cce_ctrl, struct devdrv_cce_context *cce_context,
                                      u32 index)
{
    struct vm_area_struct *vma = cce_context->vma[cce_ctrl->tsid];
    unsigned long vaddr;
    size_t size;

    vaddr = vma->vm_start + (long)(unsigned)DEVDRV_SQ_SLOT_SIZE * DEVDRV_MAX_SQ_DEPTH * index;
    size = PAGE_ALIGN(DEVDRV_SQ_SLOT_SIZE * DEVDRV_MAX_SQ_DEPTH);

    devdrv_zap_vma_ptes(vma, vaddr, size);
}

void devdrv_unmap_sq_mem(struct devdrv_cce_context *cce_context, struct devdrv_cce_ctrl *cce_ctrl, u32 index)
{
    int err;
    if (cce_context->recycle_status == DEVDRV_RECYCLE_STATUS) {
        /* add print will flood your screen, please don't add */
        return;
    }

    err = devdrv_cce_context_check(cce_context);
    if (err) {
        devdrv_drv_err("[dev_id=%u]:cce_context check failed, err = %d\n", cce_context->devid, err);
        return;
    }
    err = devdrv_mmap_sq_mem_param_check(cce_context, cce_ctrl, index);
    if (err) {
        devdrv_drv_err("[dev_id=%u]:sq mem param check failed, err = %d\n", cce_context->devid, err);
        return;
    }
    devdrv_unmmap_sq_slot_mem(cce_ctrl, cce_context, index);
    devdrv_unmap_doorbell_mem(cce_ctrl, cce_context, index);
}

STATIC int devdrv_mmap_sq_slot_mem(struct devdrv_cce_context *cce_context, struct devdrv_cce_ctrl *cce_ctrl, u32 index)
{
    struct vm_area_struct *vma = cce_context->vma[cce_ctrl->tsid];
    struct devdrv_info *dev_info = NULL;
    unsigned long vaddr;
    phys_addr_t paddr;
    size_t size;
    int err;
#ifdef CONFIG_ARM64
    pgprot_t vm_page_prot;
#endif

    dev_info = cce_ctrl->dev_info;

    paddr = devdrv_get_base_sq_phyaddr(dev_info, cce_ctrl, index);
    if (!paddr) {
        devdrv_drv_err("[dev_id=%u]:get sq phy addr failed, index = %u\n", cce_context->devid, index);
        return -ENODEV;
    }
    vaddr = vma->vm_start + (long)(unsigned)DEVDRV_SQ_SLOT_SIZE * DEVDRV_MAX_SQ_DEPTH * index;
    size = PAGE_ALIGN(DEVDRV_SQ_SLOT_SIZE * DEVDRV_MAX_SQ_DEPTH);
    if ((size == 0) || (size > DEVDRV_SQ_SLOT_SIZE * DEVDRV_MAX_SQ_DEPTH)) {
        devdrv_drv_err("[dev_id=%u]:argment size is error, size = %lu\n", cce_context->devid, size);
        return -EINVAL;
    }
#ifdef CONFIG_ARM64
    vm_page_prot = pgprot_device(vma->vm_page_prot);
    err = remap_pfn_range(vma, vaddr, __phys_to_pfn(paddr), size, vm_page_prot);

#else
    err = remap_pfn_range(vma, vaddr, __phys_to_pfn(paddr), size, vma->vm_page_prot);
#endif
    if (err) {
        devdrv_drv_err("[dev_id=%u]:remap sq mem failed, err = %d\n", cce_context->devid, err);
        return -EFAULT;
    }

    return 0;
}

int devdrv_mmap_sq_mem(struct devdrv_cce_context *cce_context, struct devdrv_cce_ctrl *cce_ctrl, u32 index)
{
    int err;

    err = devdrv_cce_context_check(cce_context);
    if (err) {
        devdrv_drv_err("cce_context check failed, err = %d\n", err);
        return -EINVAL;
    }
    err = devdrv_mmap_sq_mem_param_check(cce_context, cce_ctrl, index);
    if (err) {
        devdrv_drv_err("[dev_id=%u]:sq mem param check failed, err = %d\n", cce_context->devid, err);
        return -EINVAL;
    }
    err = devdrv_mmap_sq_slot_mem(cce_context, cce_ctrl, index);
    if (err) {
        devdrv_drv_err("[dev_id=%u]:sq slot mem mmap failed, err = %d\n", cce_context->devid, err);
        return -EINVAL;
    }
    err = devdrv_mmap_doorbell_mem(cce_context, cce_ctrl, index);
    if (err) {
        devdrv_drv_err("[dev_id=%u]:sq doorbell mmap failed, err = %d, index = %u\n", cce_context->devid, err, index);
        return -EINVAL;
    }

    return 0;
}

STATIC int devdrv_mmap_cq_slot_mem(struct devdrv_cce_ctrl *cce_ctrl, struct devdrv_ts_cq_info *cq_info,
                                   struct vm_area_struct *vma)
{
    struct devdrv_cq_sub_info *cq_sub = cq_info->cq_sub;
    pgprot_t vm_page_prot;
    unsigned long vaddr;
    phys_addr_t paddr;
    size_t size;
    int err;
#ifdef CFG_SOC_PLATFORM_MINIV2
    u32 tsid = cce_ctrl->tsid;
#endif
    u8 plat_type;

    cq_sub = cq_info->cq_sub;
    plat_type = cce_ctrl->dev_info->plat_type;

    if (plat_type == DEVDRV_MANAGER_DEVICE_ENV) {
#ifdef CFG_SOC_PLATFORM_MINIV2
    paddr = (phys_addr_t)CQ_RESERVE_MEM_BASE + (phys_addr_t)tsid * (phys_addr_t)CQ_RESERVE_MEM_SIZE +
            (phys_addr_t)cq_sub->index * (phys_addr_t)cq_info->slot_size * (phys_addr_t)DEVDRV_MAX_CQ_DEPTH;
#else
    paddr = (phys_addr_t)virt_to_phys((void *)(uintptr_t)cq_sub->virt_addr);
#endif /* CFG_SOC_PLATFORM_MINIV2 */
    } else if (plat_type == DEVDRV_MANAGER_HOST_ENV) {
        paddr = (phys_addr_t)virt_to_phys((void *)(uintptr_t)cq_sub->virt_addr);
    } else {
        devdrv_drv_err("invalid environment type & uio type\n");
        return -EINVAL;
    }

    size = PAGE_ALIGN(cq_info->slot_size * DEVDRV_MAX_CQ_DEPTH);
    if ((size == 0) || (size > DEVDRV_MAX_CQ_SLOT_SIZE * DEVDRV_MAX_SQ_DEPTH)) {
        devdrv_drv_err("argment size is error, size = %lu\n", size);
        return -EINVAL;
    }
    vaddr = vma->vm_start + (long)(unsigned)DEVDRV_VM_CQ_BLOCK_OFFSET +
            (long)(unsigned)DEVDRV_VM_CQ_SLOT_SIZE * cq_sub->index;

    /* mmap cq slot */
    if (cce_ctrl->dev_info->plat_type == DEVDRV_MANAGER_HOST_ENV) {
        vm_page_prot = vma->vm_page_prot;
    } else {
        vm_page_prot = pgprot_noncached(vma->vm_page_prot);  //lint !e446
    }
    err = remap_pfn_range(vma, vaddr, __phys_to_pfn(paddr), size, vm_page_prot);
    if (err) {
        devdrv_drv_err("cq mmap failed, err = %d\n", err);
        return -EFAULT;
    }
    return 0;
}

/* mmap cq slot and coresponding cq doorbell */
STATIC int devdrv_mmap_cq_mem(struct devdrv_cce_context *cce_context, struct devdrv_cce_ctrl *cce_ctrl,
                              struct vm_area_struct *vma)
{
    struct devdrv_info *dev_info = cce_ctrl->dev_info;
    struct devdrv_ts_cq_info *cq_info = NULL;
    int err;

    cq_info = devdrv_mmap_cqinfo_get_and_check(dev_info, cce_ctrl, cce_context);
    if (cq_info == NULL) {
        devdrv_drv_err("[dev_id=%u]:cq info get and check failed\n", cce_context->devid);
        return -ENODEV;
    }
    err = devdrv_mmap_cq_slot_mem(cce_ctrl, cq_info, vma);
    if (err) {
        devdrv_drv_err("[dev_id=%u]:cq slot mmap failed, err = %d\n", cce_context->devid, err);
        return -EFAULT;
    }

    err = devdrv_mmap_doorbell_mem(cce_context, cce_ctrl, DEVDRV_MAX_SQ_NUM + cq_info->index);
    if (err) {
        devdrv_drv_err("[dev_id=%u]:map cq doorbell failed, err = %d\n", cce_context->devid, err);
        return -EFAULT;
    }

    return 0;
}

STATIC int devdrv_mmap_tsinfo(struct devdrv_cce_context *cce_context, struct devdrv_cce_ctrl *cce_ctrl,
                              struct vm_area_struct *vma)
{
    int err;

    /*
     * mmap address ranges
     * |___SQ(32MB)___|____INFO(32MB)_____|__DOORBELL(32MB)___|___CQ(64MB)____|
     */
    /* remap info mem for user space */
    err = devdrv_mmap_info_mem(cce_ctrl, vma);
    if (err) {
        devdrv_drv_err("info mem mmap failed, err = %d\n", err);
        return -EFAULT;
    }

    err = devdrv_mmap_cq_mem(cce_context, cce_ctrl, vma);
    if (err) {
        devdrv_drv_err("cq mem mmap failed, err = %d\n", err);
        return -ENODEV;
    }

#ifdef CFG_SOC_PLATFORM_MINI
#ifndef CFG_SOC_PLATFORM_MINIV2
    err = devdrv_mmap_ts_dma_buffer(cce_ctrl, vma);
    if (err) {
        devdrv_drv_err("devdrv_mmap_ts_dma_buffer failed, err = %d\n", err);
        return err;
    }
#endif /* CFG_SOC_PLATFORM_MINIV2 */
#endif

    return 0;
}

STATIC int devdrv_mmap(struct file *filep, struct vm_area_struct *vma)
{
    struct devdrv_cce_context *cce_context = NULL;
    struct devdrv_cce_ctrl *cce_ctrl = NULL;
    struct devdrv_info *dev_info = NULL;
    u32 tsid = 0;
    int err;

    err = devdrv_mmap_param_check(filep, vma);
    if (err) {
        devdrv_drv_err("mmap param check failed, err = %d\n", err);
        return -EINVAL;
    }
    cce_context = filep->private_data;

#ifdef CFG_SOC_PLATFORM_MINIV2
    if (vma->vm_start != DEVDRV_VM_MEM_START) {  // only consider at most 2 ts
        tsid = 1;
    }
#endif /* CFG_SOC_PLATFORM_MINIV2 */

    if (tsid >= DEVDRV_MAX_TS_NUM) {
        devdrv_drv_err("tsid=%u\n", tsid);
        return -EINVAL;
    }
    cce_ctrl = cce_context->cce_ctrl[tsid];
    cce_context->vma[tsid] = vma;

    vma->vm_ops = &devdrv_vm_ops;
    vma->vm_flags |= VM_LOCKED;
    vma->vm_flags |= VM_DONTEXPAND;
    vma->vm_flags |= VM_PFNMAP;
    vma->vm_flags |= VM_WRITE;

    dev_info = cce_ctrl->dev_info;
    atomic_inc(&dev_info->occupy_ref);
    if (dev_info->status == 1) {
        atomic_dec(&dev_info->occupy_ref);
        devdrv_drv_warn("dev %d has been reset\n", dev_info->dev_id);
        return -EINVAL;
    }

    devdrv_drv_debug("vma->vm_start = 0x%lx, vma->vm_end = 0x%lx\n", vma->vm_start, vma->vm_end);
    err = devdrv_mmap_tsinfo(cce_context, cce_context->cce_ctrl[tsid], vma);
    atomic_dec(&dev_info->occupy_ref);
    if (err) {
        devdrv_drv_err("[dev_id=%u]:cq mem mmap failed, err = %d\n", cce_context->devid, err);
        return -ENODEV;
    }
    vma->vm_private_data = cce_context;

    return 0;
}

const struct file_operations devdrv_fops = {
    .owner = THIS_MODULE,
    .open = devdrv_open,
    .release = devdrv_release,
    .unlocked_ioctl = devdrv_ioctl,
    .mmap = devdrv_mmap,
};

STATIC void devdrv_find_cq_index(unsigned long data)
{
    struct devdrv_cq_sub_info *cq_sub_info = NULL;
    struct devdrv_int_context *int_context = NULL;
    struct devdrv_cce_context *cce_context = NULL;
    struct devdrv_cce_ctrl *cce_ctrl = NULL;
    struct devdrv_ts_cq_info *cq_info = NULL;
    struct devdrv_report *report = NULL;
    phys_addr_t info_mem_addr;
    struct uio_mem *cq_uio_mem = NULL;
    unsigned long flags;
    phys_addr_t cq_base_addr;
    int cq_index;
    u32 phase;
    int end;
    int i;
    u32 tsid;

    int_context = (struct devdrv_int_context *)((uintptr_t)data);
    cce_ctrl = int_context->cce_ctrl;
    tsid = cce_ctrl->tsid;

    info_mem_addr = cce_ctrl->mem_info[DEVDRV_INFO_MEM].virt_addr;
#ifdef CFG_SOC_PLATFORM_MINIV2
    /* miniv2 only one int, it needs check all cq */
    end = int_context->first_cq_index + DEVDRV_MAX_CQ_NUM;
#else
    end = int_context->first_cq_index + DEVDRV_CQ_PER_IRQ;
#endif

    for (i = int_context->first_cq_index; i < end; i++) {
        cq_index = i;
        cq_info = devdrv_calc_cq_info(info_mem_addr, cq_index);
        if (cq_index != cq_info->index) {
            devdrv_drv_err("[dev_id = %u]:cq_index != cq_info->index, cq_index: %d, "
                           "cq_info->index: %d.\n",
                           cce_ctrl->devid, cq_index, cq_info->index);
            continue;
        }

        cq_sub_info = (struct devdrv_cq_sub_info *)cq_info->cq_sub;

        spin_lock_irqsave(&cq_sub_info->spinlock, flags);
        if ((cq_sub_info->cce_context == NULL) || (cq_sub_info->uio == NULL)) {
            spin_unlock_irqrestore(&cq_sub_info->spinlock, flags);
            continue;
        }

        cq_uio_mem = cq_sub_info->uio->mem;

        cq_base_addr = cq_uio_mem[DEVDRV_SQ_CQ_MAP].addr;
        report = (struct devdrv_report *)((uintptr_t)((unsigned long)cq_base_addr +
                                                      (unsigned long)cq_info->slot_size * cq_info->tail));

        if (cce_ctrl->dev_info->plat_type == DEVDRV_MANAGER_HOST_ENV) {
            dma_sync_single_for_cpu(cq_sub_info->dev, cq_sub_info->phy_addr, cq_sub_info->size, DMA_FROM_DEVICE);
        }

        phase = devdrv_report_get_phase(report);
        cce_context = cq_sub_info->cce_context;

        /* for checking a thread is waiting for wake up */
        if (cce_context->ts_context[tsid].cq_tail_updated != 0) { /* condition is true, continue */
            spin_unlock_irqrestore(&cq_sub_info->spinlock, flags);
            devdrv_drv_debug("[dev_id=%u]:receive report irq, cq id: %u"
                             "no runtime thread is waiting, not judge.\n",
                             cce_context->devid, cq_info->index);
            continue;
        }

        if (phase == cq_info->phase) {
            cce_context->ts_context[tsid].cq_tail_updated = CQ_HEAD_UPDATE_FLAG;
            if (waitqueue_active(&cce_context->ts_context[tsid].report_wait)) {
                wake_up(&cce_context->ts_context[tsid].report_wait);
            }
        }
        /* release spinlock after access cq's uio mem */
        spin_unlock_irqrestore(&cq_sub_info->spinlock, flags);
    }
}

STATIC irqreturn_t devdrv_irq_handler(int irq, void *data)
{
    struct devdrv_int_context *int_context = NULL;
    unsigned long flags;

    local_irq_save(flags);

    int_context = (struct devdrv_int_context *)data;
    tasklet_schedule(&int_context->find_cq_task);

    local_irq_restore(flags);

    return IRQ_HANDLED;
}

STATIC int devdrv_request_irq_bh(struct devdrv_cce_ctrl *cce_ctrl)
{
    struct devdrv_platform_data *pdata = cce_ctrl->dev_info->pdata;
    u32 tsid = cce_ctrl->tsid;
    int ret = 0;
    int i, j;

    for (i = 0; i < DEVDRV_CQ_UPDATE_IRQ_SUM; i++) {
        cce_ctrl->int_context[i].cce_ctrl = cce_ctrl;
        cce_ctrl->int_context[i].first_cq_index = i * DEVDRV_CQ_PER_IRQ;
        tasklet_init(&cce_ctrl->int_context[i].find_cq_task, devdrv_find_cq_index,
                     (unsigned long)(uintptr_t)&cce_ctrl->int_context[i]);
        ret = request_irq(pdata->ts_pdata[tsid].irq_cq_update[i], devdrv_irq_handler, IRQF_TRIGGER_NONE,
                          "devdrv_driver", &cce_ctrl->int_context[i]);
        if (ret) {
            devdrv_drv_err("request irq failed devdrv_drv_register, "
                           "ret(%d) i(%d), irq(%d)\n",
                           ret, i, pdata->ts_pdata[tsid].irq_cq_update[i]);
            goto request_failed;
        }
    }
    return ret;

request_failed:
    j = i;
    for (i = 0; i < j; i++)
        free_irq(pdata->ts_pdata[tsid].irq_cq_update[i], &cce_ctrl->int_context[i]);
    for (i = 0; i <= j; i++)
        tasklet_kill(&cce_ctrl->int_context[i].find_cq_task);
    return ret;
}

STATIC void devdrv_free_irq_bh(struct devdrv_info *dev_info, u32 tsid)
{
    struct devdrv_platform_data *pdata = dev_info->pdata;
    struct devdrv_cce_ctrl *cce_ctrl = dev_info->cce_ctrl[tsid];
    int i;

    for (i = 0; i < DEVDRV_CQ_UPDATE_IRQ_SUM; i++) {
        free_irq(pdata->ts_pdata[tsid].irq_cq_update[i], &cce_ctrl->int_context[i]);
        tasklet_kill(&cce_ctrl->int_context[i].find_cq_task);
    }
}

STATIC int devdrv_drv_register_sub(struct devdrv_info *dev_info, u32 tsid)
{
    struct devdrv_cce_ctrl *cce_ctrl = NULL;
    int ret = 0;

    cce_ctrl = dev_info->cce_ctrl[tsid];

    if (devdrv_mailbox_init(&cce_ctrl->mailbox)) {
        devdrv_drv_err("[dev_id=%u]:devdrv_mailbox_init fail.\n", cce_ctrl->devid);
        return -ENODEV;
    }

    if (devdrv_init_functional_cqsq(dev_info, tsid)) {
        devdrv_drv_err("[dev_id=%u]:devdrv_init_functional_cqsq fail.\n", cce_ctrl->devid);
        ret = -ENODEV;
        goto functional_cqsq_init_fail;
    }

    if (devdrv_request_irq_bh(cce_ctrl)) {
        devdrv_drv_err("[dev_id=%u]:devdrv_request_irq_bh failed\n", cce_ctrl->devid);
        ret = -ENODEV;
        goto request_irq_bh_failed;
    }

    return ret;

request_irq_bh_failed:
    devdrv_destroy_functional_cqsq(dev_info, tsid);

functional_cqsq_init_fail:
    devdrv_mailbox_exit(&cce_ctrl->mailbox);

    return ret;
}

STATIC void devdrv_drv_unregister_sub(struct devdrv_info *dev_info, u32 tsid)
{
    struct devdrv_cce_ctrl *cce_ctrl = NULL;

    cce_ctrl = dev_info->cce_ctrl[tsid];
    devdrv_destroy_functional_cqsq(dev_info, tsid);
    devdrv_mailbox_exit(&cce_ctrl->mailbox);

    return;
}

STATIC void devdrv_drv_ops_init(struct devdrv_info *dev_info)
{
    dev_info->drv_ops->add_stream_msg_chan = devdrv_add_stream_msg_chan;
    dev_info->drv_ops->add_event_msg_chan = devdrv_add_event_msg_chan;
    dev_info->drv_ops->add_sq_msg_chan = devdrv_add_sq_msg_chan;
    dev_info->drv_ops->add_cq_msg_chan = devdrv_add_cq_msg_chan;
    dev_info->drv_ops->add_model_msg_chan = devdrv_add_model_msg_chan;
    dev_info->drv_ops->add_notify_msg_chan = devdrv_add_notify_msg_chan;
    dev_info->drv_ops->add_task_msg_chan = devdrv_add_task_msg_chan;

    dev_info->drv_ops->alloc_stream_msg_chan = devdrv_alloc_stream_msg_chan;
    dev_info->drv_ops->alloc_event_msg_chan = devdrv_alloc_event_msg_chan;
    dev_info->drv_ops->alloc_sq_msg_chan = devdrv_alloc_sq_msg_chan;
    dev_info->drv_ops->alloc_cq_msg_chan = devdrv_alloc_cq_msg_chan;
    dev_info->drv_ops->alloc_model_msg_chan = devdrv_alloc_model_msg_chan;
    dev_info->drv_ops->alloc_notify_msg_chan = devdrv_alloc_notify_msg_chan;
    dev_info->drv_ops->alloc_task_msg_chan = devdrv_alloc_task_msg_chan;

    dev_info->drv_ops->create_functional_sq = devdrv_create_functional_sq;
    dev_info->drv_ops->functional_set_sq_func = devdrv_functional_set_sq_func;
    dev_info->drv_ops->functional_send_sq = devdrv_functional_send_sq;
    dev_info->drv_ops->create_functional_cq = devdrv_create_functional_cq;
    dev_info->drv_ops->functional_set_cq_func = devdrv_functional_set_cq_func;
    dev_info->drv_ops->destroy_functional_cq = devdrv_destroy_functional_cq;
    dev_info->drv_ops->destroy_functional_sq = devdrv_destroy_functional_sq;

    dev_info->drv_ops->mailbox_recycle = devdrv_mailbox_recycle;
    dev_info->drv_ops->mailbox_kernel_sync_no_feedback = devdrv_mailbox_kernel_sync_no_feedback;

    dev_info->drv_ops->get_event_id = devdrv_get_event_id;
    dev_info->drv_ops->free_one_notify_id = devdrv_free_one_notify_id;
    dev_info->drv_ops->notify_id_send_ts_msg = devdrv_notify_ts_msg;

    dev_info->drv_ops->svm_va_to_devid = devdrv_manager_va_to_devid;
}

STATIC inline void devdrv_cce_ctrl_list_init(struct devdrv_cce_ctrl *cce_ctrl)
{
    INIT_LIST_HEAD(&cce_ctrl->stream_available_list);
    INIT_LIST_HEAD(&cce_ctrl->event_available_list);
    INIT_LIST_HEAD(&cce_ctrl->sq_available_list);
    INIT_LIST_HEAD(&cce_ctrl->cq_available_list);
    INIT_LIST_HEAD(&cce_ctrl->model_available_list);
    INIT_LIST_HEAD(&cce_ctrl->notify_available_list);
    INIT_LIST_HEAD(&cce_ctrl->task_available_list);
}

STATIC inline void devdrv_cce_ctrl_spinlock_init(struct devdrv_cce_ctrl *cce_ctrl)
{
    spin_lock_init(&cce_ctrl->spinlock);
    spin_lock_init(&cce_ctrl->event_spinlock);
    spin_lock_init(&cce_ctrl->model_spinlock);
    spin_lock_init(&cce_ctrl->notify_spinlock);
    spin_lock_init(&cce_ctrl->task_spinlock);
}

STATIC void devdrv_cce_ctrl_mutex_init(struct devdrv_cce_ctrl *cce_ctrl)
{
    mutex_init(&cce_ctrl->cq_mutex_t);
    mutex_init(&cce_ctrl->sq_mutex_t);
    mutex_init(&cce_ctrl->stream_mutex_t);
    mutex_init(&cce_ctrl->event_mutex_t);
    mutex_init(&cce_ctrl->model_mutex_t);
    mutex_init(&cce_ctrl->notify_mutex_t);
    mutex_init(&cce_ctrl->task_mutex_t);
    mutex_init(&cce_ctrl->wake_num_mutex);
    mutex_init(&cce_ctrl->mm_mutex_t);
    mutex_init(&cce_ctrl->recycle_mutex);
}

STATIC void devdrv_cce_ctrl_mutex_destroy(struct devdrv_cce_ctrl *cce_ctrl)
{
    mutex_destroy(&cce_ctrl->cq_mutex_t);
    mutex_destroy(&cce_ctrl->sq_mutex_t);
    mutex_destroy(&cce_ctrl->stream_mutex_t);
    mutex_destroy(&cce_ctrl->event_mutex_t);
    mutex_destroy(&cce_ctrl->model_mutex_t);
    mutex_destroy(&cce_ctrl->notify_mutex_t);
    mutex_destroy(&cce_ctrl->task_mutex_t);
    mutex_destroy(&cce_ctrl->wake_num_mutex);
    mutex_destroy(&cce_ctrl->mm_mutex_t);
    mutex_destroy(&cce_ctrl->recycle_mutex);
}

STATIC void devdrv_cce_ctrl_destroy(struct devdrv_info *dev_info, u32 tsid)
{
    struct devdrv_cce_ctrl *cce_ctrl = NULL;

    cce_ctrl = dev_info->cce_ctrl[tsid];
    if (cce_ctrl == NULL)
        return;

    devdrv_drv_unregister_sub(dev_info, tsid);
    destroy_workqueue(cce_ctrl->wq);
    devdrv_cce_ctrl_mutex_destroy(cce_ctrl);
    kfree(cce_ctrl);
    cce_ctrl = NULL;
}

STATIC int devdrv_cce_ctrl_init(struct devdrv_info *dev_info, u32 tsid)
{
    struct devdrv_platform_data *pdata = NULL;
    struct devdrv_cce_ctrl *cce_ctrl = NULL;
    struct devdrv_mailbox *mailbox = NULL;
    int tlen;
    int ret;

    pdata = dev_info->pdata;

    cce_ctrl = kzalloc(sizeof(struct devdrv_cce_ctrl), GFP_KERNEL);
    if (cce_ctrl == NULL) {
        devdrv_drv_err("kmalloc failed\n");
        goto kzalloc_fail;
    }
    dev_info->cce_ctrl[tsid] = cce_ctrl;
    devdrv_cce_ctrl_list_init(cce_ctrl);
    devdrv_cce_ctrl_spinlock_init(cce_ctrl);
    devdrv_cce_ctrl_mutex_init(cce_ctrl);

    cce_ctrl->wakelock_num = 0;
    cce_ctrl->devid = dev_info->dev_id;

    cce_ctrl->mailbox.ack_irq = pdata->ts_pdata[tsid].irq_mailbox_ack;
    cce_ctrl->mailbox.data_ack_irq = pdata->ts_pdata[tsid].irq_mailbox_data_ack;
    cce_ctrl->functional_cqsq.int_context.irq_num = pdata->ts_pdata[tsid].irq_functional_cq;
    cce_ctrl->functional_cqsq.int_context.cce_ctrl = cce_ctrl;

    cce_ctrl->dev_info = dev_info;
    cce_ctrl->cq_num = 0;
    cce_ctrl->tsid = tsid;

    cce_ctrl->wq = create_singlethread_workqueue("devdrv-recycle");
    if (cce_ctrl->wq == NULL) {
        devdrv_drv_err("create_workqueue error.\n");
        goto workqueue_fail;
    }
    atomic_set(&cce_ctrl->power_mode, DEVDRV_NORMAL_POWER);

    mailbox = &cce_ctrl->mailbox;

    mailbox->send_sram = pdata->ts_pdata[tsid].ts_mbox_send_vaddr;
    mailbox->receive_sram = pdata->ts_pdata[tsid].ts_mbox_rcv_vaddr;
    ret = devdrv_drv_register_sub(dev_info, tsid);
    if (ret)
        goto mailbox_init_fail;

    tlen = snprintf_s(cce_ctrl->wakelock_name, DEVDRV_WAKELOCK_SIZE, DEVDRV_WAKELOCK_SIZE - 1, "davinci-devdrv%u%c",
                      cce_ctrl->devid, '\0');
    if (tlen < 0) {
        devdrv_drv_err("davinci-devdrv%u snprintf_s fail\n", cce_ctrl->devid);
        ret = tlen;
        devdrv_free_irq_bh(dev_info, tsid);
        devdrv_drv_unregister_sub(dev_info, tsid);
        goto mailbox_init_fail;
    }

    return 0;

mailbox_init_fail:
    destroy_workqueue(cce_ctrl->wq);
workqueue_fail:
    devdrv_cce_ctrl_mutex_destroy(cce_ctrl);
    kfree(cce_ctrl);
    cce_ctrl = NULL;
kzalloc_fail:
    dev_info->cce_ctrl[tsid] = NULL;
    return -ENOMEM;
}

STATIC int devdrv_ids_init(struct devdrv_info *dev_info, u32 tsid)
{
    struct devdrv_cce_ctrl *cce_ctrl = dev_info->cce_ctrl[tsid];
    u8 *status_addr = NULL;
    unsigned long flags;
    int ret;

    ret = devdrv_shm_init(dev_info, tsid);
    if (ret) {
        devdrv_drv_err("shm init failed with ret(%d)\n", ret);
        goto shm_init_fail;
    }
    ret = devdrv_cqsq_init(dev_info, tsid, DEVDRV_MAX_SQ_NUM, DEVDRV_MAX_CQ_NUM);
    if (ret) {
        devdrv_drv_err("devdrv_cqsq_init error with ret(%d)\n", ret);
        goto sqcq_init_fail;
    }
    ret = devdrv_id_init(dev_info, tsid, DEVDRV_MAX_STREAM_ID, DEVDRV_MAX_EVENT_ID);
    if (ret) {
        devdrv_drv_err("devdrv_id_init fail with ret(%d)\n", ret);
        goto id_init_fail;
    }
    ret = devdrv_model_id_init(dev_info, tsid, DEVDRV_MAX_MODEL_ID);
    if (ret) {
        devdrv_drv_err("devdrv_model_id_init fail with ret(%d)\n", ret);
        goto model_id_init_failed;
    }
    ret = devdrv_task_id_init(dev_info, tsid, DEVDRV_MAX_TASK_ID);
    if (ret) {
        devdrv_drv_err("devdrv_task_id_init fail with ret(%d)\n", ret);
        goto task_id_init_failed;
    }
    ret = devdrv_notify_id_init(dev_info, tsid, DEVDRV_MAX_NOTIFY_ID);
    if (ret) {
        devdrv_drv_err("devdrv_model_id_init fail with ret(%d)\n", ret);
        goto notify_id_init_failed;
    }

    status_addr = (u8 *)(uintptr_t)dev_info->cce_ctrl[tsid]->devdrv_uio_info->mem[0].addr;
    status_addr += DEVDRV_MAX_STREAM_ID * sizeof(struct devdrv_stream_info);
    status_addr += DEVDRV_MAX_SQ_NUM * sizeof(struct devdrv_ts_sq_info);
    status_addr += DEVDRV_MAX_CQ_NUM * sizeof(struct devdrv_ts_cq_info);

    spin_lock_irqsave(&dev_info->ts_mng[tsid].ts_spinlock, flags);
    dev_info->ts_mng[tsid].ts_work_status = (u32 *)status_addr;
    *dev_info->ts_mng[tsid].ts_work_status = dev_info->ts_mng[tsid].ts_work_status_shadow;
    spin_unlock_irqrestore(&dev_info->ts_mng[tsid].ts_spinlock, flags);

    return 0;

notify_id_init_failed:
    devdrv_task_id_destroy(cce_ctrl);
task_id_init_failed:
    devdrv_model_id_destroy(cce_ctrl);
model_id_init_failed:
    devdrv_stream_id_destroy(cce_ctrl);
    devdrv_event_id_destroy(cce_ctrl);
id_init_fail:
    devdrv_cqsq_destroy(cce_ctrl);
sqcq_init_fail:
    devdrv_shm_destroy(cce_ctrl);
shm_init_fail:
    return ret;
}

STATIC void devdrv_ids_destroy(struct devdrv_info *dev_info, u32 tsid)
{
    struct devdrv_cce_ctrl *cce_ctrl = NULL;

    if (dev_info->cce_ctrl[tsid] == NULL)
        return;
    cce_ctrl = dev_info->cce_ctrl[tsid];
    devdrv_notify_id_destroy(cce_ctrl);
    devdrv_task_id_destroy(cce_ctrl);
    devdrv_model_id_destroy(cce_ctrl);
    devdrv_stream_id_destroy(cce_ctrl);
    devdrv_cqsq_destroy(cce_ctrl);
    devdrv_shm_destroy(cce_ctrl);
    devdrv_event_id_destroy(cce_ctrl);
}

/**
 * devdrv_drv_register - register a new devdrv device
 * @devdrv_info: devdrv device info
 *
 * returns zero on success
 */
STATIC int devdrv_drv_register(struct devdrv_info *dev_info)
{
    int ret = 0;
    u32 tsid;
    u32 ts_num;
    u32 i;

    ts_num = devdrv_manager_get_ts_num(dev_info);
    if (ts_num == (u32)-1) {
        devdrv_drv_err("ts_num(%u) error.\n", ts_num);
        return -EFAULT;
    }

    for (tsid = 0; tsid < ts_num; tsid++) {
        ret = devdrv_cce_ctrl_init(dev_info, tsid);
        if (ret) {
            devdrv_drv_err("cce_ctrl init failed\n");
            goto devdrv_cce_ctrl_init_fail;
        }
        ret = devdrv_ids_init(dev_info, tsid);
        if (ret) {
            devdrv_drv_err("id init failed\n");
            goto devdrv_ids_init_fail;
        }
    }
    devdrv_drv_ops_init(dev_info);

    INIT_LIST_HEAD(&dev_info->cce_context_list);
    dev_info->rb_root = RB_ROOT;

    return 0;

devdrv_ids_init_fail:
    devdrv_free_irq_bh(dev_info, tsid);
    devdrv_cce_ctrl_destroy(dev_info, tsid);

devdrv_cce_ctrl_init_fail:
    for (i = 0; i < tsid; i++) {
        devdrv_free_irq_bh(dev_info, i);
        devdrv_ids_destroy(dev_info, i);
        devdrv_cce_ctrl_destroy(dev_info, i);
    }

    return ret;
}

/**
 * devdrv_drv_unregister - unregister a devdrv device
 * @devdrv_info: devdrv device info
 *
 * returns zero on success
 */
STATIC void devdrv_drv_unregister(struct devdrv_info *dev_info)
{
    struct devdrv_cce_ctrl *cce_ctrl = NULL;
    u32 retry_cnt;
    u32 tsid = 0;
    u32 ts_num;

    if (dev_info == NULL)
        return;

    ts_num = devdrv_manager_get_ts_num(dev_info);
    if ((ts_num == (u32)-1) || (ts_num > DEVDRV_MAX_TS_NUM)) {
        devdrv_drv_err("ts_num(%u) error.\n", ts_num);
        return;
    }

    for (tsid = 0; tsid < ts_num; tsid++) {
        if (dev_info->cce_ctrl[tsid] == NULL) {
            devdrv_drv_err("cce_ctrl tsid(%u) is NULL\n", tsid);
            continue;
        }
        cce_ctrl = dev_info->cce_ctrl[tsid];
        if (!devdrv_manager_get_devdrv_info(dev_info->dev_id)) {
            devdrv_drv_err("[dev_id=%u]:device %d is not registered\n", cce_ctrl->devid, dev_info->dev_id);
            return;
        }

        devdrv_free_irq_bh(dev_info, tsid);

        retry_cnt = 0;
        devdrv_drv_info("dev_id(%u) recycle_work_num %d.\n", cce_ctrl->devid, atomic_read(&cce_ctrl->recycle_work_num));
        /* wait for all process exit */
        while (retry_cnt < WAIT_PROCESS_EXIT_TIME) {
            if (atomic_read(&cce_ctrl->recycle_work_num) == 0) {
                break;
            }
            retry_cnt++;
            msleep(1);
        }
        devdrv_drv_info("dev_id(%u) wait retry_cnt %d recycle_work_num %d.\n",
            cce_ctrl->devid, retry_cnt, atomic_read(&cce_ctrl->recycle_work_num));

        devdrv_ids_destroy(dev_info, tsid);
        devdrv_cce_ctrl_destroy(dev_info, tsid);
        dev_info->cce_ctrl[tsid] = NULL;
    }
}

STATIC int __init devdrv_devinit(void)
{
    struct devdrv_init_client dev_client;
    int ret;

    dev_client.register_client = devdrv_drv_register;
    dev_client.unregister_client = devdrv_drv_unregister;
    ret = devdrv_manager_register_client(&dev_client, &devdrv_fops);
    if (ret) {
        devdrv_drv_info("call devdrv_manager_register_client fail, if you can search "
            "<devdrv_manager_register_client success> in follow print, it's OK. ret=%d.\n", ret);
    }
    devdrv_drv_info("devdrv_manager_register_client success\n");
    devdrv_manager_set_svm_report_handle(devdrv_set_cce_context_status);
    devdrv_config_p2p();
    return 0;
}
module_init(devdrv_devinit);

STATIC void __exit devdrv_devexit(void)
{
    struct devdrv_init_client dev_client;

    devdrv_manager_set_svm_report_handle(NULL);
    dev_client.register_client = devdrv_drv_register;
    dev_client.unregister_client = devdrv_drv_unregister;
    (void)devdrv_manager_unregister_client(&dev_client);
}

module_exit(devdrv_devexit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Huawei Tech. Co., Ltd.");
MODULE_DESCRIPTION("DAVINCI driver");
