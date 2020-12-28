/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2012-2019. All rights reserved.
 * Description:
 * Author: huawei
 * Create: 2019-10-15
 */
#include <linux/list.h>
#include <linux/sched.h>
#include <linux/slab.h>
#include <linux/vmalloc.h>
#include <linux/errno.h>

#include "devdrv_common.h"
#include "devdrv_manager_common.h"
#include "devdrv_manager.h"
#include "devdrv_interface.h"
#include "devdrv_black_box.h"

#define DEVDRV_BLACK_BOX_MAX_EXCEPTION_NUM 1024

STATIC int devdrv_host_pcie_add_exception(u32 pci_devid, u32 code, struct timespec stamp)
{
    struct devdrv_manager_info *manager_info = devdrv_get_manager_info();

    if (manager_info == NULL) {
        devdrv_drv_err("device manager is not inited. dev_id(%u)\n", pci_devid);
        return -EFAULT;
    }

    if (pci_devid < DEVDRV_MAX_DAVINCI_NUM) {
        return devdrv_host_black_box_add_exception(pci_devid, code, stamp, NULL);
    }

    devdrv_drv_err("no such device, pci device id: %u.\n", pci_devid);
    return -ENODEV;
}

void devdrv_host_black_box_init(void)
{
    struct devdrv_manager_info *manager_info = devdrv_get_manager_info();
    struct devdrv_black_callback black_callback;
    unsigned long flags;
    int ret;

    if (manager_info == NULL) {
        devdrv_drv_err("device manager is not inited.\n");
        return;
    }

    spin_lock_init(&manager_info->black_box.spinlock);
    sema_init(&manager_info->black_box.black_box_sema, 0);
    spin_lock_irqsave(&manager_info->black_box.spinlock, flags);
    manager_info->black_box.thread_should_stop = 0;
    manager_info->black_box.black_box_pid = 0;
    manager_info->black_box.exception_num = 0;
    spin_unlock_irqrestore(&manager_info->black_box.spinlock, flags);
    INIT_LIST_HEAD(&manager_info->black_box.exception_list);

    black_callback.callback = devdrv_host_pcie_add_exception;

    ret = devdrv_register_black_callback(&black_callback);
    if (ret) {
        devdrv_drv_err("devdrv_register_black_callback failed.\n");
    }
}

void devdrv_host_black_box_exit(void)
{
    struct devdrv_manager_info *manager_info = devdrv_get_manager_info();
    struct devdrv_black_callback black_callback;
    struct devdrv_exception *exception = NULL;
    struct list_head *pos = NULL, *n = NULL;
    unsigned long flags;

    if (manager_info == NULL) {
        devdrv_drv_err("invalid.\n");
        return;
    }

    black_callback.callback = devdrv_host_pcie_add_exception;
    devdrv_unregister_black_callback(&black_callback);

    spin_lock_irqsave(&manager_info->black_box.spinlock, flags);
    manager_info->black_box.thread_should_stop = 1;
    list_for_each_safe(pos, n, &manager_info->black_box.exception_list)
    {
        exception = list_entry(pos, struct devdrv_exception, list);
        list_del(&exception->list);
        if (exception->data != NULL) {
            kfree(exception->data);
            exception->data = NULL;
        }
        kfree(exception);
        exception = NULL;
        manager_info->black_box.exception_num--;
    }
    up(&manager_info->black_box.black_box_sema);
    spin_unlock_irqrestore(&manager_info->black_box.spinlock, flags);
}

void devdrv_host_black_box_close_check(pid_t pid)
{
    struct devdrv_manager_info *manager_info = devdrv_get_manager_info();
    struct devdrv_exception *exception = NULL;
    struct list_head *pos = NULL, *n = NULL;
    unsigned long flags;

    if (manager_info == NULL) {
        devdrv_drv_err("invalid.\n");
        return;
    }

    spin_lock_irqsave(&manager_info->black_box.spinlock, flags);
    if (manager_info->black_box.black_box_pid == pid) {
        manager_info->black_box.black_box_pid = 0;
        list_for_each_safe(pos, n, &manager_info->black_box.exception_list)
        {
            exception = list_entry(pos, struct devdrv_exception, list);
            list_del(&exception->list);
            if (exception->data != NULL) {
                kfree(exception->data);
                exception->data = NULL;
            }
            kfree(exception);
            exception = NULL;
            manager_info->black_box.exception_num--;
        }
        up(&manager_info->black_box.black_box_sema);
    }
    spin_unlock_irqrestore(&manager_info->black_box.spinlock, flags);
}

/**
 * @data: must be alloced by user
 *
 */
int devdrv_host_black_box_add_exception(u32 devid, u32 code, struct timespec stamp, void *data)
{
    struct devdrv_manager_info *manager_info = devdrv_get_manager_info();
    struct devdrv_exception *exception = NULL;
    struct devdrv_exception *old = NULL;
    unsigned long flags;
    u32 more = 1;

    if ((devid >= DEVDRV_MAX_DAVINCI_NUM) || (manager_info == NULL)) {
        devdrv_drv_err("invalid devid(%u).\n", devid);
        return -EINVAL;
    }

    spin_lock_irqsave(&manager_info->black_box.spinlock, flags);
    exception = kzalloc(sizeof(struct devdrv_exception), GFP_ATOMIC);
    if (exception == NULL) {
        spin_unlock_irqrestore(&manager_info->black_box.spinlock, flags);
        devdrv_drv_err("alloc exception node failed. devid(%u)\n", devid);
        return -ENOMEM;
    }

    exception->devid = devid;
    exception->code = code;
    exception->stamp = stamp;
    exception->data = data;

    if (manager_info->black_box.exception_num >= DEVDRV_BLACK_BOX_MAX_EXCEPTION_NUM) {
        old = list_first_entry(&manager_info->black_box.exception_list, struct devdrv_exception, list);
        list_del(&old->list);
        if (old->data != NULL) {
            kfree(old->data);
            old->data = NULL;
        }
        kfree(old);
        old = NULL;
        manager_info->black_box.exception_num--;
        more = 0;
    }
    list_add_tail(&exception->list, &manager_info->black_box.exception_list);
    manager_info->black_box.exception_num++;

    spin_unlock_irqrestore(&manager_info->black_box.spinlock, flags);

    if (more)
        up(&manager_info->black_box.black_box_sema);

    return 0;
}

STATIC void devdrv_host_black_box_device_id_process(struct devdrv_black_box_user *black_box_user,
                                                    struct devdrv_exception *exception)
{
    struct devdrv_black_box_devids *bbox_devids = NULL;
    u32 dev_num;
    u32 i;

    bbox_devids = (struct devdrv_black_box_devids *)exception->data;
    if (bbox_devids == NULL) {
        devdrv_drv_err("exception->data is NULL\n");
        return;
    }
    dev_num = bbox_devids->dev_num;
    if (dev_num > DEVDRV_MAX_DAVINCI_NUM) {
        devdrv_drv_err("invalid dev_num(%u).\n", dev_num);
        return;
    }

    devdrv_drv_info("*** bbox inform dev_num = %d ***\n", dev_num);

    black_box_user->priv_data.bbox_devids.dev_num = dev_num;
    for (i = 0; i < dev_num; i++) {
        black_box_user->priv_data.bbox_devids.devids[i] = bbox_devids->devids[i];
    }
}

STATIC void devdrv_host_black_box_status_info_process(struct devdrv_black_box_user *black_box_user,
                                                      struct devdrv_exception *exception)
{
    struct devdrv_black_box_state_info *bbox_state_info = NULL;

    bbox_state_info = (struct devdrv_black_box_state_info *)exception->data;
    if (bbox_state_info == NULL) {
        devdrv_drv_err("bbox state info is NULL.\n");
        return;
    }

    if ((bbox_state_info->devId >= DEVDRV_MAX_DAVINCI_NUM) || (bbox_state_info->state >= STATE_TO_MAX)) {
        devdrv_drv_err("dev id(%u) or state(%u) is out of range.\n", bbox_state_info->devId, bbox_state_info->state);
        return;
    }

    devdrv_drv_info("status is inform through pcie driver with"
                    "dev id(%u), state(%u), code(0x%x).\n",
                    bbox_state_info->devId, (u32)bbox_state_info->state, black_box_user->exception_code);
    black_box_user->priv_data.bbox_state.state = bbox_state_info->state;
    black_box_user->priv_data.bbox_state.devId = bbox_state_info->devId;
}

STATIC void devdrv_host_black_box_priv_data_process(struct devdrv_black_box_user *black_box_user,
                                                    struct devdrv_exception *exception)
{
    switch (black_box_user->exception_code) {
        case DEVDRV_BB_DEVICE_ID_INFORM:
            devdrv_host_black_box_device_id_process(black_box_user, exception);
            break;
        case DEVDRV_BB_DEVICE_STATE_INFORM:
            devdrv_host_black_box_status_info_process(black_box_user, exception);
            break;
        default:
            break;
    }
    return;
}

void devdrv_host_black_box_get_exception(struct devdrv_black_box_user *black_box_user)
{
    struct devdrv_manager_info *manager_info = devdrv_get_manager_info();
    struct devdrv_exception *exception = NULL;
    unsigned long flags;
    int ret;

    if ((black_box_user == NULL) || (manager_info == NULL)) {
        devdrv_drv_err("invalid input handler(%d).\n", black_box_user == NULL ? 1 : 0);
        return;
    }

    spin_lock_irqsave(&manager_info->black_box.spinlock, flags);
    if (manager_info->black_box.thread_should_stop || (manager_info->black_box.black_box_pid == 0)) {
        black_box_user->thread_should_stop = 1;
        manager_info->black_box.thread_should_stop = 0;
        spin_unlock_irqrestore(&manager_info->black_box.spinlock, flags);

        devdrv_drv_err("thread should stop.\n");

        return;
    }

    ret = memset_s(black_box_user, sizeof(struct devdrv_black_box_user), 0, sizeof(struct devdrv_black_box_user));
    if (ret != 0) {
        spin_unlock_irqrestore(&manager_info->black_box.spinlock, flags);
        devdrv_drv_err("set black_box_user to 0 failed, ret(%d).\n", ret);
        return;
    }

    if (!list_empty_careful(&manager_info->black_box.exception_list)) {
        exception = list_first_entry(&manager_info->black_box.exception_list, struct devdrv_exception, list);
        list_del(&exception->list);
        manager_info->black_box.exception_num--;
        spin_unlock_irqrestore(&manager_info->black_box.spinlock, flags);

        black_box_user->thread_should_stop = 0;
        black_box_user->devid = exception->devid;
        black_box_user->exception_code = exception->code;
        black_box_user->tv_sec = exception->stamp.tv_sec;
        black_box_user->tv_nsec = exception->stamp.tv_nsec;

        devdrv_host_black_box_priv_data_process(black_box_user, exception);
        devdrv_drv_warn("exception code: %pK.\n", (void *)(uintptr_t)exception->code);

        if (black_box_user->exception_code == DEVDRV_BB_DEVICE_ID_INFORM ||
            black_box_user->exception_code == DEVDRV_BB_DEVICE_STATE_INFORM) {
            kfree(exception->data);
            exception->data = NULL;
        }

        kfree(exception);
        exception = NULL;

        return;
    }
    spin_unlock_irqrestore(&manager_info->black_box.spinlock, flags);
}
