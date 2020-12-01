/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2012-2019. All rights reserved.
 * Description:
 * Author: huawei
 * Create: 2019-10-15
 */
#include <linux/delay.h>
#include <linux/irq.h>
#include <linux/module.h>
#include <linux/of_address.h>
#include <linux/platform_device.h>
#include <linux/slab.h>
#include <linux/uaccess.h>
#include <linux/idr.h>
#include <linux/errno.h>
#include <linux/fs.h>
#include <linux/sched.h>
#include <linux/vmalloc.h>

#include "devdrv_manager_common.h"
#include "devdrv_nsm.h"
#include "devdrv_manager_container.h"

#if LINUX_VERSION_CODE >= KERNEL_VERSION(4, 15, 0)
#include <linux/sched/signal.h>
#endif

STATIC int devdrv_manager_ipc_notify_param_check(u32 dev_id, u32 notify_id, struct ipc_notify_info *ipc_notify_info)
{
    if ((dev_id >= DEVDRV_MAX_DAVINCI_NUM) || (ipc_notify_info == NULL) || (notify_id >= DEVDRV_MAX_NOTIFY_ID)) {
        devdrv_drv_err("invalid parameter, "
                       "dev_id = %u, "
                       "notify_id = %u, "
                       "(ipc_notify_info == NULL)(%d).\n",
                       dev_id, notify_id, (ipc_notify_info == NULL));
        return -ENODEV;
    }

    return 0;
}

STATIC int devdrv_manager_ipc_notify_id_range_check(struct ipc_notify_node *notify_node)
{
    if ((notify_node == NULL) || (notify_node->dev_id >= DEVDRV_MAX_DAVINCI_NUM) ||
        (notify_node->notify_id >= DEVDRV_MAX_NOTIFY_ID)) {
        devdrv_drv_err("invalid dev_id(%u), or notify_id(%u)\n", (notify_node == NULL) ? 0 : notify_node->dev_id,
                       (notify_node == NULL) ? 0 : notify_node->notify_id);
        return -ENODEV;
    }
    return 0;
}

STATIC int devdrv_manager_ipc_notify_id_check(struct devdrv_notify_ioctl_info *notify_ioctl_info,
                                              struct ipc_notify_node *notify_node)
{
    if ((notify_node == NULL) || (notify_node->dev_id >= DEVDRV_MAX_DAVINCI_NUM) ||
        (notify_node->notify_id >= DEVDRV_MAX_NOTIFY_ID)) {
        devdrv_drv_err("notify id range check failed, dev_id(%u), notify_id(%u)\n",
                       (notify_node == NULL) ? 0 : notify_node->dev_id,
                       (notify_node == NULL) ? 0 : notify_node->notify_id);
        return -ENODEV;
    }

    if ((notify_ioctl_info->dev_id != notify_node->dev_id) ||
        (notify_ioctl_info->notify_id != notify_node->notify_id)) {
        devdrv_drv_err("notify id check failed,  "
                       "notify_ioctl_info.dev_id = %u, "
                       "notify_node->dev_id = %u, "
                       "notify_ioctl_info.notify_id = %u, "
                       "notify_node->notify_id = %u\n",
                       notify_ioctl_info->dev_id, notify_node->dev_id, notify_ioctl_info->notify_id,
                       notify_node->notify_id);
        return -EFAULT;
    }

    return 0;
}

STATIC struct devdrv_notify_id_info *devdrv_manager_check_notify_info(struct devdrv_cce_ctrl *cce_ctrl,
    u32 notify_id)
{
    struct devdrv_notify_id_info *notify_info = NULL;

    if ((cce_ctrl == NULL) || (notify_id >= DEVDRV_MAX_NOTIFY_ID)) {
        devdrv_drv_err("cce_ctrl(%pK) is NULL, or invalid notify_id(%u)\n", cce_ctrl, notify_id);
        return NULL;
    }

    notify_info = (struct devdrv_notify_id_info *)(cce_ctrl->notify_addr +
                                                   (long)sizeof(struct devdrv_notify_id_info) * notify_id);
    spin_lock(&notify_info->spinlock);
    if (notify_info->cce_context == NULL) {
        spin_unlock(&notify_info->spinlock);
        devdrv_drv_err("notify id is not allocated yet, notify_id(%u)\n", notify_id);
        return NULL;
    }
    if (notify_info->id != notify_id) {
        spin_unlock(&notify_info->spinlock);
        devdrv_drv_err("invalid notify id(%d), notify_id = %u\n", notify_info->id, notify_id);
        return NULL;
    }
    spin_unlock(&notify_info->spinlock);

    return notify_info;
}

static inline struct devdrv_notify_id_info *devdrv_manager_get_notify_info(struct devdrv_cce_ctrl *cce_ctrl,
                                                                           u32 notify_id)
{
    struct devdrv_notify_id_info *notify_info = NULL;

    if ((cce_ctrl == NULL) || (notify_id >= DEVDRV_MAX_NOTIFY_ID)) {
        devdrv_drv_err("cce_ctrl(%pK) is NULL, or invalid notify_id(%u)\n", cce_ctrl, notify_id);
        return NULL;
    }

    notify_info = (struct devdrv_notify_id_info *)(cce_ctrl->notify_addr +
                                                   (long)sizeof(struct devdrv_notify_id_info) * notify_id);

    return notify_info;
}

int devdrv_manager_ipc_notify_create(struct devdrv_manager_context *context, unsigned long arg,
                                     struct devdrv_notify_ioctl_info *notify_ioctl_info)
{
    struct ipc_notify_info *ipc_notify_info = context->ipc_notify_info;
    struct devdrv_notify_id_info *notify_info = NULL;
    u32 notify_id = notify_ioctl_info->notify_id;
    struct ipc_notify_node *notify_node = NULL;
    struct devdrv_info *dev_info = NULL;
    u32 dev_id = 0;
    int ret;

    if (devdrv_manager_ipc_notify_param_check(notify_ioctl_info->dev_id, notify_id, ipc_notify_info)) {
        devdrv_drv_err("invalid parameter. logid(%u)\n", notify_ioctl_info->dev_id);
        return -ENODEV;
    }

    ret = devdrv_manager_container_logical_id_to_physical_id(notify_ioctl_info->dev_id, &dev_id);
    if (ret) {
        devdrv_drv_err("logical id convert to physical id failed, ret(%d), logid(%u), phyid(%u).\n",
            ret, notify_ioctl_info->dev_id, dev_id);
        return -EFAULT;
    }

    dev_info = devdrv_manager_get_devdrv_info(dev_id);
    if ((dev_info == NULL) || (dev_info->cce_ctrl[0] == NULL)) {
        devdrv_drv_err("dev_info(%pK) or cce_ctrl is NULL. phyid(%u)\n", dev_info, dev_id);
        return -EINVAL;
    }

    atomic_inc(&dev_info->occupy_ref);
    if (dev_info->status == 1) {
        atomic_dec(&dev_info->occupy_ref);
        devdrv_drv_warn("dev %d has been reset\n", dev_info->dev_id);
        return -EINVAL;
    }

    notify_info = devdrv_manager_check_notify_info(dev_info->cce_ctrl[0], notify_id);
    if (notify_info == NULL) {
        atomic_dec(&dev_info->occupy_ref);
        devdrv_drv_err("check notify id(%u) failed, phyid(%u)\n", notify_id, dev_id);
        return -ENODEV;
    }
    /* Set ref to 1 when calling devdrv_alloc_notify_id , each notify_id corresponds to an ipc_name */
    if (atomic_read(&notify_info->ref) != 1) {
        atomic_dec(&dev_info->occupy_ref);
        devdrv_drv_err("ipc_notify had been created. notify_id(%u), phyid(%u)\n", notify_id, dev_id);
        return -EINVAL;
    }

    if (devdrv_ipc_create(notify_ioctl_info->name, (void *)&notify_node, DEVDRV_IPC_NOTIFY)) {
        atomic_dec(&dev_info->occupy_ref);
        devdrv_drv_err("create ipc notify failed.\n");
        return -EINVAL;
    }

    ret = copy_to_user_safe((void *)((uintptr_t)arg), (void *)notify_ioctl_info, sizeof(struct
        devdrv_notify_ioctl_info));
    if (ret) {
        atomic_dec(&dev_info->occupy_ref);
        devdrv_ipc_del(notify_ioctl_info->name, DEVDRV_IPC_NAME_SIZE, DEVDRV_IPC_NOTIFY);
        devdrv_drv_err("copy to user failed, ret(%d).\n", ret);
        return -EFAULT;
    }

    spin_lock_init(&notify_node->spinlock);
    /* set ipc notify node ref to 1 when it created */
    atomic_set(&notify_node->ref, DEVDRV_IPC_REF_INIT);
    atomic_inc(&notify_info->ref);

    mutex_lock(&ipc_notify_info->info_mutex);
    spin_lock(&notify_node->spinlock);
    notify_node->dev_id = dev_id;
    notify_node->notify_id = notify_id;
    notify_node->pid[0] = (pid_t)current->tgid;
    list_add(&notify_node->create_list_node, &ipc_notify_info->create_list_head);
    spin_unlock(&notify_node->spinlock);

    ipc_notify_info->create_fd_num++;

    devdrv_drv_debug("create ipc notify, "
                     "phyid(%u), "
                     "notify_id(%u), "
                     "create_fd_num(%u), "
                     "ipc_notify->ref(%d)\n",
                     dev_id, notify_id, ipc_notify_info->create_fd_num,
                     atomic_read(&notify_node->ref));
    mutex_unlock(&ipc_notify_info->info_mutex);
    atomic_dec(&dev_info->occupy_ref);

    return 0;
}

/* open func passed to the user the physical id */
int devdrv_manager_ipc_notify_open(struct devdrv_manager_context *context, unsigned long arg,
                                   struct devdrv_notify_ioctl_info *notify_ioctl_info)
{
    struct ipc_notify_info *ipc_notify_info = context->ipc_notify_info;
    struct devdrv_notify_id_info *notify_info = NULL;
    struct ipc_notify_node *notify_node = NULL;
    struct devdrv_info *dev_info = NULL;
#ifndef CFG_SOC_PLATFORM_MINIV2
    u32 i;
#endif /* CFG_SOC_PLATFORM_MINIV2 */
    if (devdrv_ipc_find(notify_ioctl_info->name, (void *)&notify_node, DEVDRV_IPC_NOTIFY)) {
        devdrv_drv_err("find ipc notify failed.\n");
        return -EINVAL;
    }

    /* ipc notify node can be opened by only one process at the same time */
    if (atomic_inc_return(&notify_node->ref) != DEVDRV_IPC_REF_MAX) {
        atomic_dec(&notify_node->ref);
        devdrv_drv_err("notify is already opened, ipc_notify.ref = %u\n", atomic_read(&notify_node->ref));
        return -ENODEV;
    }

    if (devdrv_manager_ipc_notify_id_range_check(notify_node)) {
        atomic_dec(&notify_node->ref);
        devdrv_drv_err("ipc notify id range check failed, "
                       "notify_node->dev_id = %u, notify_node->notify_id = %u\n",
                       notify_node->dev_id, notify_node->notify_id);
        return -EFAULT;
    }
#ifndef CFG_SOC_PLATFORM_MINIV2
    for (i = 0; i < DEVDRV_PID_MAX_NUM; i++) {
        if (notify_node->pid[i] == (pid_t)current->tgid) {
            break;
        }
    }

    if (i == DEVDRV_PID_MAX_NUM) {
        atomic_dec(&notify_node->ref);
        devdrv_drv_err("current id[%d] has no permission.\n", current->tgid);
        return -EINVAL;
    }
#endif /* CFG_SOC_PLATFORM_MINIV2 */

    dev_info = devdrv_manager_get_devdrv_info(notify_node->dev_id);
    if ((dev_info == NULL) || (dev_info->cce_ctrl[0] == NULL)) {
        atomic_dec(&notify_node->ref);
        devdrv_drv_err("dev_info(%pK) or cce_ctrl is NULL. dev_id(%u)\n", dev_info, notify_node->dev_id);
        return -EINVAL;
    }

    atomic_inc(&dev_info->occupy_ref);
    if (dev_info->status == 1) {
        atomic_dec(&notify_node->ref);
        atomic_dec(&dev_info->occupy_ref);
        devdrv_drv_warn("dev %d has been reset\n", dev_info->dev_id);
        return -EINVAL;
    }

    notify_info = devdrv_manager_check_notify_info(dev_info->cce_ctrl[0], notify_node->notify_id);
    if (notify_info == NULL) {
        atomic_dec(&dev_info->occupy_ref);
        atomic_dec(&notify_node->ref);
        devdrv_drv_err("check notify id failed, notify_id(%u), dev_id(%u)\n", notify_node->notify_id,
                       notify_node->dev_id);
        return -ENODEV;
    }
    notify_ioctl_info->dev_id = notify_node->dev_id;
    notify_ioctl_info->notify_id = notify_node->notify_id;

    mutex_lock(&ipc_notify_info->info_mutex);
    ipc_notify_info->open_fd_num++;
    atomic_inc(&notify_info->ref);
    spin_lock(&notify_node->spinlock);
    list_add(&notify_node->open_list_node, &ipc_notify_info->open_list_head);
    spin_unlock(&notify_node->spinlock);

    atomic_dec(&dev_info->occupy_ref);

    if (copy_to_user_safe((void *)(uintptr_t)arg, (void *)notify_ioctl_info,
        sizeof(struct devdrv_notify_ioctl_info))) {
        ipc_notify_info->open_fd_num--;
        atomic_dec(&notify_info->ref);
        atomic_dec(&notify_node->ref);
        spin_lock(&notify_node->spinlock);
        list_del(&notify_node->open_list_node);
        spin_unlock(&notify_node->spinlock);

        devdrv_drv_err("copy to user failed. dev_id(%u)\n", notify_ioctl_info->dev_id);
        mutex_unlock(&ipc_notify_info->info_mutex);

        return -EFAULT;
    }

    devdrv_drv_debug("open ipc notify, "
                     "dev_id(%u), "
                     "notify_id(%u), "
                     "open_fd_num(%u), "
                     "ipc_notify.ref(%d)\n",
                     notify_ioctl_info->dev_id, notify_ioctl_info->notify_id, ipc_notify_info->open_fd_num,
                     atomic_read(&notify_node->ref));
    mutex_unlock(&ipc_notify_info->info_mutex);

    return 0;
}

STATIC int devdrv_manager_match_pid(struct ipc_notify_node *notify_node, pid_t pid)
{
    int i;
    for (i = 0; i < DEVDRV_PID_MAX_NUM; i++) {
        if (notify_node->pid[i] == pid) {
            return IS;
        }
    }
    return 0;
}

int devdrv_manager_ipc_notify_set_pid(struct devdrv_manager_context *context,
                                      struct devdrv_notify_ioctl_info *notify_ioctl_info)
{
    struct ipc_notify_node *notify_node = NULL;
    unsigned int i, j;

    if (devdrv_ipc_find(notify_ioctl_info->name, (void *)&notify_node, DEVDRV_IPC_NOTIFY)) {
        devdrv_drv_err("find ipc notify failed.\n");
        return -EINVAL;
    }

    if (notify_node->pid[0] != (pid_t)current->tgid) {
        devdrv_drv_err("Invalid process, curr_tgid[%d], creat_tgid[%d].\n", current->tgid, notify_node->pid[0]);
        return -EPERM;
    }

    if (devdrv_manager_ipc_notify_id_range_check(notify_node)) {
        devdrv_drv_err("ipc notify id range check failed, notify_node->dev_id(%u), notify_node->notify_id(%u)\n",
                       notify_node->dev_id, notify_node->notify_id);
        return -EINVAL;
    }

    /* set pid */
    for (i = 0; (i < DEVDRV_PID_MAX_NUM) && (notify_node->pid[i] != 0); i++) {
        ;
    }
    if (i == DEVDRV_PID_MAX_NUM) {
        devdrv_drv_err("set pid over the most max. dev_id(%u)\n", notify_node->dev_id);
        return -EFAULT;
    }

    for (j = 0; (i < DEVDRV_PID_MAX_NUM) && (j < DEVDRV_PID_MAX_NUM) && (notify_ioctl_info->pid[j] != 0); j++) {
        if (devdrv_manager_match_pid(notify_node, notify_ioctl_info->pid[j]) == 0) {
            notify_node->pid[i++] = notify_ioctl_info->pid[j];
            devdrv_drv_debug("set pid(%d). dev_id(%u)\n", notify_ioctl_info->pid[j], notify_node->dev_id);
        }
    }

    devdrv_drv_debug("set ipc pid, ipc_notify.ref(%d), dev_id(%u)\n", atomic_read(&notify_node->ref),
                     notify_node->dev_id);

    return 0;
}

STATIC struct ipc_notify_node *devdrv_manager_find_notify_node(struct ipc_notify_info *ipc_notify_info,
                                                               struct devdrv_notify_ioctl_info *notify_ioctl_info)
{
    u32 notify_id = notify_ioctl_info->notify_id;
    u32 dev_id = notify_ioctl_info->dev_id;
    struct ipc_notify_node *notify_node = NULL;

    if (devdrv_manager_ipc_notify_param_check(dev_id, notify_id, ipc_notify_info)) {
        devdrv_drv_err("invalid parameter\n");
        return NULL;
    }

    if (devdrv_ipc_find(notify_ioctl_info->name, (void *)&notify_node, DEVDRV_IPC_NOTIFY)) {
        devdrv_drv_err("find ipc notify failed.\n");
        return NULL;
    }

    if (devdrv_manager_ipc_notify_id_check(notify_ioctl_info, notify_node)) {
        devdrv_drv_err("notify id check failed. dev_id(%u)\n", dev_id);
        return NULL;
    }

    return notify_node;
}

STATIC void devdrv_manager_notify_ref_dec(struct ipc_notify_node *notify_node)
{
    if (atomic_dec_and_test(&notify_node->ref)) {
        devdrv_drv_debug("destroy ipc notify, "
                         "dev_id(%u), "
                         "notify_id(%u)\n",
                         notify_node->dev_id, notify_node->notify_id);
        if (devdrv_ipc_del(notify_node->name, DEVDRV_IPC_NAME_SIZE, DEVDRV_IPC_NOTIFY)) {
            devdrv_drv_err("ipc del failed, dev_id(%u)\n", notify_node->dev_id);
            return;
        }
    } else {
        devdrv_drv_debug("destroy ipc notify, "
                         "dev_id(%u), "
                         "notify_id(%u), "
                         "notify_node.ref(%d)\n",
                         notify_node->dev_id, notify_node->notify_id,
                         atomic_read(&notify_node->ref));
    }
}

STATIC void devdrv_manager_ipc_notify_free(struct devdrv_manager_context *context, u32 dev_id, u32 notify_id)
{
    struct devdrv_notify_id_info *notify_info = NULL;
    struct devdrv_info *dev_info = NULL;

    dev_info = devdrv_manager_get_devdrv_info(dev_id);
    if ((dev_info == NULL) || (dev_info->cce_ctrl[0] == NULL)) {
        devdrv_drv_err("dev_info(%pK) or cce_ctrl is NULL. dev_id(%u)\n", dev_info, dev_id);
        return;
    }

    atomic_inc(&dev_info->occupy_ref);
    if (dev_info->status == 1) {
        atomic_dec(&dev_info->occupy_ref);
        devdrv_drv_warn("dev %d has been reset\n", dev_info->dev_id);
        return;
    }

    notify_info = devdrv_manager_get_notify_info(dev_info->cce_ctrl[0], notify_id);
    if (notify_info == NULL) {
        atomic_dec(&dev_info->occupy_ref);
        devdrv_drv_info("check notify id failed, notify_id(%u), dev_id(%u)"
                        "do not care it in recycle process\n",
                        notify_id, dev_id);
        return;
    }

    if ((dev_info->drv_ops == NULL) || (dev_info->cce_ctrl[0] == NULL) ||
        (dev_info->drv_ops->notify_id_send_ts_msg == NULL)) {
        atomic_dec(&dev_info->occupy_ref);
        devdrv_drv_err("dev_info->drv_ops(%pK) or dev_info->drv_ops->notify_id_send_ts_msg is NULL. dev_id(%u)\n",
                       dev_info->drv_ops, dev_id);
        return;
    }
    if (atomic_dec_and_test(&notify_info->ref)) {
        if (dev_info->drv_ops->notify_id_send_ts_msg(dev_info, 0, notify_id)) {
            devdrv_drv_err("send ts notify reset msg failed. dev_id(%u)\n", dev_id);
        }
    }
    atomic_dec(&dev_info->occupy_ref);

    return;
}

int devdrv_manager_ipc_notify_destroy(struct devdrv_manager_context *context,
                                      struct devdrv_notify_ioctl_info *notify_ioctl_info)
{
    struct ipc_notify_info *ipc_notify_info = context->ipc_notify_info;
    struct ipc_notify_node *notify_node = NULL;
    u32 logic_id;
    int ret;

    devdrv_drv_debug("start, create_fd_num(%u)\n", ipc_notify_info->create_fd_num);
    logic_id = notify_ioctl_info->dev_id;

    ret = devdrv_manager_container_logical_id_to_physical_id(logic_id, &notify_ioctl_info->dev_id);
    if (ret) {
        devdrv_drv_err("logical id convert to physical id failed, ret(%d), logic_id(%u), phyid(%u).\n",
            ret, logic_id, notify_ioctl_info->dev_id);
        return -EFAULT;
    }

    notify_node = devdrv_manager_find_notify_node(ipc_notify_info, notify_ioctl_info);
    if (notify_node == NULL) {
        devdrv_drv_err("find notify node failed\n");
        return -EFAULT;
    }

    mutex_lock(&ipc_notify_info->info_mutex);
    if (ipc_notify_info->create_fd_num == 0) {
        devdrv_drv_err("create_fd_num(%d)\n", ipc_notify_info->create_fd_num);
        mutex_unlock(&ipc_notify_info->info_mutex);
        return -EFAULT;
    }

    spin_lock(&notify_node->spinlock);
    list_del(&notify_node->create_list_node);
    spin_unlock(&notify_node->spinlock);

    ipc_notify_info->create_fd_num--;
    mutex_unlock(&ipc_notify_info->info_mutex);

    devdrv_manager_ipc_notify_free(context, notify_node->dev_id, notify_node->notify_id);
    devdrv_manager_notify_ref_dec(notify_node);

    devdrv_drv_debug("finish, create_fd_num(%u). dev_id(%u)\n", ipc_notify_info->create_fd_num, notify_node->dev_id);

    return 0;
}

/* the user passed the physical id */
int devdrv_manager_ipc_notify_close(struct devdrv_manager_context *context,
                                    struct devdrv_notify_ioctl_info *notify_ioctl_info)
{
    struct ipc_notify_info *ipc_notify_info = context->ipc_notify_info;
    struct ipc_notify_node *notify_node = NULL;

    devdrv_drv_debug("start, open_fd_num(%u)\n", ipc_notify_info->open_fd_num);

    notify_node = devdrv_manager_find_notify_node(ipc_notify_info, notify_ioctl_info);
    if (notify_node == NULL) {
        devdrv_drv_err("find notify node failed\n");
        return -EFAULT;
    }

    mutex_lock(&ipc_notify_info->info_mutex);
    if (ipc_notify_info->open_fd_num == 0) {
        devdrv_drv_err("open_fd_num(%d)\n", ipc_notify_info->open_fd_num);
        mutex_unlock(&ipc_notify_info->info_mutex);
        return -EFAULT;
    }

    spin_lock(&notify_node->spinlock);
    list_del(&notify_node->open_list_node);
    spin_unlock(&notify_node->spinlock);
    ipc_notify_info->open_fd_num--;
    mutex_unlock(&ipc_notify_info->info_mutex);

    devdrv_manager_ipc_notify_free(context, notify_node->dev_id, notify_node->notify_id);
    devdrv_manager_notify_ref_dec(notify_node);

    devdrv_drv_debug("finish, open_fd_num(%u). dev_id(%u)\n", ipc_notify_info->open_fd_num, notify_node->dev_id);

    return 0;
}

static inline void devdrv_manager_ipc_notify_leak_print(struct devdrv_manager_context *context)
{
    struct ipc_notify_info *ipc_notify_info = context->ipc_notify_info;

    devdrv_drv_warn("Process Name: %s  "
                    "PID: %d  "
                    "TGID: %d\n",
                    context->task->comm, context->pid, context->tgid);

    devdrv_drv_warn("ipc_notify_info->create_fd_num = %d, "
                    "ipc_notify_info->open_fd_num = %d\n",
                    ipc_notify_info->create_fd_num, ipc_notify_info->open_fd_num);
}

STATIC void devdrv_manager_ipc_notify_recycle_opened(struct devdrv_manager_context *context,
                                                     struct ipc_notify_info *ipc_notify_info)
{
    struct ipc_notify_node *notify_node = NULL;
    struct list_head *pos = NULL, *n = NULL;

    devdrv_drv_info("start recycle opened ipc node, open_fd_num(%u)\n", ipc_notify_info->open_fd_num);

    mutex_lock(&ipc_notify_info->info_mutex);
    if (!list_empty_careful(&ipc_notify_info->open_list_head)) {
        list_for_each_safe(pos, n, &ipc_notify_info->open_list_head)
        {
            notify_node = list_entry(pos, struct ipc_notify_node, open_list_node);
            ipc_notify_info->open_fd_num--;
            devdrv_manager_ipc_notify_free(context, notify_node->dev_id, notify_node->notify_id);
            devdrv_manager_notify_ref_dec(notify_node);
        }
    }
    devdrv_drv_info("finish recycle opened ipc node, open_fd_num(%u)\n", ipc_notify_info->open_fd_num);
    mutex_unlock(&ipc_notify_info->info_mutex);
}

STATIC void devdrv_manager_ipc_notify_recycle_created(struct devdrv_manager_context *context,
                                                      struct ipc_notify_info *ipc_notify_info)
{
    struct ipc_notify_node *notify_node = NULL;
    struct list_head *pos = NULL, *n = NULL;

    devdrv_drv_info("start recycle created ipc node, create_fd_num(%u)\n", ipc_notify_info->create_fd_num);

    mutex_lock(&ipc_notify_info->info_mutex);
    if (!list_empty_careful(&ipc_notify_info->create_list_head)) {
        list_for_each_safe(pos, n, &ipc_notify_info->create_list_head)
        {
            notify_node = list_entry(pos, struct ipc_notify_node, create_list_node);
            ipc_notify_info->create_fd_num--;
            devdrv_manager_ipc_notify_free(context, notify_node->dev_id, notify_node->notify_id);
            devdrv_manager_notify_ref_dec(notify_node);
        }
    }
    devdrv_drv_info("finish recycle created ipc node, create_fd_num(%u)\n", ipc_notify_info->create_fd_num);
    mutex_unlock(&ipc_notify_info->info_mutex);
}

STATIC void devdrv_manager_ipc_notify_recycle_work(struct devdrv_manager_context *context,
                                                   struct ipc_notify_info *ipc_notify_info)
{
    devdrv_manager_ipc_notify_recycle_opened(context, ipc_notify_info);
    devdrv_manager_ipc_notify_recycle_created(context, ipc_notify_info);
}

void devdrv_manager_ipc_notify_release_recycle(struct devdrv_manager_context *context)
{
    struct ipc_notify_info *ipc_notify_info = NULL;

    if ((context == NULL) || (context->ipc_notify_info == NULL)) {
        devdrv_drv_err("context(%pK) or context->ipc_notify_info is NULL\n", context);
        return;
    }
    ipc_notify_info = context->ipc_notify_info;

    if ((ipc_notify_info->create_fd_num > 0) || (ipc_notify_info->open_fd_num > 0)) {
        devdrv_manager_ipc_notify_leak_print(context);
        devdrv_manager_ipc_notify_recycle_work(context, ipc_notify_info);
    }
}
