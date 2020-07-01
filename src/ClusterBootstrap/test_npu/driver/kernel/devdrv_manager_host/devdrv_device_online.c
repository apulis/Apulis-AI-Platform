/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2012-2019. All rights reserved.
 * Description:
 * Author: huawei
 * Create: 2020-03-10
 */
#include <linux/types.h>
#include <linux/poll.h>
#include <linux/kfifo.h>

#include "devdrv_device_online.h"
#include "devdrv_manager_container.h"

DECLARE_WAIT_QUEUE_HEAD(devid_upgrade_waitq);
DEFINE_SPINLOCK(devdrv_online_fifo_lock);

#define FIFO_CELL_SIZE sizeof(u32)
#define DEVDRV_MAX_DEVID_BUFFER (DEVDRV_MAX_DAVINCI_NUM * FIFO_CELL_SIZE)

static struct kfifo devdrv_devid_kfifo;
volatile u32 devid_upgrade_flag = 0;

STATIC int devdrv_manager_online_save_devids(struct kfifo* pkfifo, u32 dev_id)
{
    spin_lock_bh(&devdrv_online_fifo_lock);
    if (kfifo_avail(pkfifo) < FIFO_CELL_SIZE) {
        spin_unlock_bh(&devdrv_online_fifo_lock);
        devdrv_drv_err("dev_id(%u) kfifo_avail failed, maybe user thread exit.\n", dev_id);
        return -ENOMEM;
    }
    kfifo_in(pkfifo, &dev_id, FIFO_CELL_SIZE);
    spin_unlock_bh(&devdrv_online_fifo_lock);

    devdrv_drv_info("dev_id(%u) save in devids.\n", dev_id);
    return 0;
}

/* devid update we need inform user */
void devdrv_manager_online_devid_update(u32 dev_id)
{
    int ret;
    ret = devdrv_manager_online_save_devids(&devdrv_devid_kfifo, dev_id);
    if (ret) {
        devdrv_drv_err("devdrv_save_devid failed, ret = %d.\n", ret);
    }
    devid_upgrade_flag = 1;
    wake_up_interruptible(&devid_upgrade_waitq);
    devdrv_drv_info("dev_id(%u) online!\n", dev_id);
}

int devdrv_manager_online_get_devids(struct file *filep, unsigned int cmd, unsigned long arg)
{
    u32 dev_ids[DEVDRV_MAX_DAVINCI_NUM];
    u32 dev_id;
    int j = 0;
    int ret;
    int i;

    if (devdrv_manager_container_is_in_container()) {
        devdrv_drv_err("device online interface is not support container\n");
        return -EPERM;
    }

    for (i = 0; i < DEVDRV_MAX_DAVINCI_NUM; i++) {
        dev_ids[i] = DEVDRV_MAX_DAVINCI_NUM;
    }

    i = 0;
    spin_lock_bh(&devdrv_online_fifo_lock);
    while (kfifo_len(&devdrv_devid_kfifo) >= FIFO_CELL_SIZE) {
        if (kfifo_out(&devdrv_devid_kfifo, &dev_id, FIFO_CELL_SIZE)) {
            devdrv_drv_info("online dev(%u), index(%d)\n", dev_id, i);
            dev_ids[i++] = dev_id;
        }
    }
    spin_unlock_bh(&devdrv_online_fifo_lock);
    if (copy_to_user_safe((void *)(uintptr_t)arg, dev_ids, FIFO_CELL_SIZE * DEVDRV_MAX_DAVINCI_NUM)) {
        devdrv_drv_err("copy_to_user_safe failed\n");
        /* copy to user failed, need to add devids to fifo */
        while (j < i) {
            ret = devdrv_manager_online_save_devids(&devdrv_devid_kfifo, dev_ids[j]);
            if (ret) {
                devdrv_drv_err("devdrv_manager_online_save_devids failed, ret(%d)\n", ret);
                return ret;
            }
            j++;
        }
        return -EFAULT;
    }
    devid_upgrade_flag = 0;

    return 0;
}

u32 devdrv_manager_poll(struct file *filep, struct poll_table_struct *wait)
{
    u32 ret = 0;

    if (devdrv_manager_container_is_in_container()) {
        devdrv_drv_err("device online poll interface is not support container\n");
        return POLLERR;
    }

    poll_wait(filep, &devid_upgrade_waitq, wait);
    if (devid_upgrade_flag) {
        ret = POLLIN | POLLRDNORM;
    }
    return ret;
}

int devdrv_manager_online_kfifo_alloc(void)
{
    return kfifo_alloc(&devdrv_devid_kfifo, DEVDRV_MAX_DEVID_BUFFER, GFP_KERNEL);
}

void devdrv_manager_online_kfifo_free(void)
{
    kfifo_free(&devdrv_devid_kfifo);
}
