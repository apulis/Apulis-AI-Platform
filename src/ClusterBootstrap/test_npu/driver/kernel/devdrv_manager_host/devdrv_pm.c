/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2012-2019. All rights reserved.
 * Description:
 * Author: huawei
 * Create: 2019-10-15
 */
#include <linux/delay.h>
#include <linux/device.h>
#include <linux/pm.h>
#include <linux/slab.h>

#include "devdrv_mailbox.h"
#include "devdrv_common.h"
#include "devdrv_driver_pm.h"
#include "devdrv_manager_common.h"
#include "devdrv_pm.h"
#include "devdrv_manager.h"
#include "devdrv_interface.h"
#include "devdrv_black_box.h"

struct devdrv_pm *devdrv_manager_register_pm(int (*suspend)(u32 devid, u32 status), int (*resume)(u32 devid))
{
    struct devdrv_manager_info *d_info = NULL;
    struct devdrv_pm *pm = NULL;
    unsigned long flags;

    if ((suspend == NULL) || (resume == NULL)) {
        devdrv_drv_err("suspend or resume is NULL.\n");
        return NULL;
    }

    pm = kzalloc(sizeof(struct devdrv_pm), GFP_KERNEL);
    if (pm == NULL) {
        devdrv_drv_err("kmalloc failed.\n");
        return NULL;
    }

    pm->suspend = suspend;
    pm->resume = resume;

    d_info = devdrv_get_manager_info();

    spin_lock_irqsave(&d_info->pm_list_lock, flags);
    list_add(&pm->list, &d_info->pm_list_header);
    spin_unlock_irqrestore(&d_info->pm_list_lock, flags);

    return pm;
}
EXPORT_SYMBOL(devdrv_manager_register_pm);

void devdrv_manager_unregister_pm(struct devdrv_pm *pm)
{
    struct devdrv_manager_info *d_info = NULL;
    unsigned long flags;

    if (pm == NULL) {
        devdrv_drv_err("invalid input argument.\n");
        return;
    }
    if ((pm->suspend == NULL) || (pm->resume == NULL)) {
        devdrv_drv_err("callback func is null.\n");
        return;
    }

    d_info = devdrv_get_manager_info();

    spin_lock_irqsave(&d_info->pm_list_lock, flags);
    list_del(&pm->list);
    spin_unlock_irqrestore(&d_info->pm_list_lock, flags);

    kfree(pm);
    pm = NULL;
}
EXPORT_SYMBOL(devdrv_manager_unregister_pm);

STATIC int devdrv_host_manager_suspend(struct devdrv_info *info)
{
    struct devdrv_manager_info *d_info = NULL;
    struct list_head *pos = NULL, *n = NULL;
    struct list_head *stop = NULL;
    struct devdrv_pm *pm = NULL;
    unsigned long flags;
    int ret;

    d_info = devdrv_get_manager_info();

    devdrv_heart_beat_stop(info);
    devdrv_set_ts_sleep(&info->ts_mng[0]);
    devdrv_set_power_mode(info->cce_ctrl[0], DEVDRV_LOW_POWER);

    if (info->cce_ctrl[0] != NULL) {
        info->drv_ops->mailbox_recycle(&info->cce_ctrl[0]->mailbox);
    }

    spin_lock_irqsave(&d_info->pm_list_lock, flags);
    list_for_each_safe(pos, n, &d_info->pm_list_header) {
        stop = pos;
        pm = list_entry(pos, struct devdrv_pm, list);
        if (pm->suspend != NULL) {
            ret = pm->suspend(info->dev_id, DEVDRV_TS_SLEEP);
            if (ret) {
                spin_unlock_irqrestore(&d_info->pm_list_lock, flags);
                devdrv_drv_err("one suspend callback func failed. devid(%u)\n", info->dev_id);
                goto error;
            }
        }
    }
    spin_unlock_irqrestore(&d_info->pm_list_lock, flags);

    return 0;

error:
    spin_lock_irqsave(&d_info->pm_list_lock, flags);
    list_for_each_safe(pos, n, &d_info->pm_list_header) {
        pm = list_entry(pos, struct devdrv_pm, list);
        if (pm->resume != NULL)
            (void)pm->resume(info->dev_id);
        if (pos == stop)
            break;
    }
    spin_unlock_irqrestore(&d_info->pm_list_lock, flags);

    devdrv_set_ts_work(&info->ts_mng[0]);
    devdrv_set_power_mode(info->cce_ctrl[0], DEVDRV_NORMAL_POWER);
    devdrv_heart_beat_start(info);

    return ret;
}

STATIC int devdrv_host_manager_resume(struct devdrv_info *info)
{
    struct devdrv_manager_info *d_info = NULL;
    struct list_head *pos = NULL, *n = NULL;
    struct devdrv_pm *pm = NULL;
    unsigned long flags;

    d_info = devdrv_get_manager_info();

    /* 1. resume devdrv driver */
    devdrv_set_power_mode(info->cce_ctrl[0], DEVDRV_NORMAL_POWER);
    /* 2. set ts work */
    devdrv_set_ts_work(&info->ts_mng[0]);
    /* 3. resume all registered resume callback func */
    spin_lock_irqsave(&d_info->pm_list_lock, flags);
    list_for_each_safe(pos, n, &d_info->pm_list_header) {
        pm = list_entry(pos, struct devdrv_pm, list);
        if (pm->resume != NULL)
            (void)pm->resume(info->dev_id);
    }
    spin_unlock_irqrestore(&d_info->pm_list_lock, flags);

    devdrv_heart_beat_start(info);

    return 0;
}

int devdrv_host_manager_device_suspend(struct devdrv_info *dev_info)
{
    return devdrv_host_manager_suspend(dev_info);
}

int devdrv_host_manager_device_resume(struct devdrv_info *dev_info)
{
    return devdrv_host_manager_resume(dev_info);
}

void devdrv_host_manager_device_exception(struct devdrv_info *info)
{
    struct devdrv_manager_info *d_info = NULL;
    struct list_head *pos = NULL, *n = NULL;
    struct devdrv_pm *pm = NULL;
    unsigned long flags;

    d_info = devdrv_get_manager_info();
    (void)devdrv_set_ts_status(&info->ts_mng[0], DEVDRV_TS_DOWN);

    spin_lock_irqsave(&d_info->pm_list_lock, flags);
    if (!list_empty_careful(&d_info->pm_list_header)) {
        list_for_each_safe(pos, n, &d_info->pm_list_header) {
            pm = list_entry(pos, struct devdrv_pm, list);
            if (pm->suspend != NULL)
                (void)pm->suspend(info->dev_id, DEVDRV_TS_DOWN);
        }
    }
    spin_unlock_irqrestore(&d_info->pm_list_lock, flags);

    devdrv_driver_hardware_exception(info, 0);  // tsid
    if (info->cce_ctrl[0] != NULL)
        info->drv_ops->mailbox_recycle(&info->cce_ctrl[0]->mailbox);
}

/*
 * heart beat
 */
void devdrv_heart_beat_stop(struct devdrv_info *info)
{
    info->heart_beat.stop = 1;
}

void devdrv_heart_beat_start(struct devdrv_info *info)
{
    info->heart_beat.stop = 0;
}

STATIC void devdrv_heart_beat_device_down(struct devdrv_info *info)
{
    struct devdrv_heart_beat_node *beat_node = NULL;
    struct list_head *pos = NULL, *n = NULL;
    struct timespec stamp;
    unsigned long flags;
    int ret;

    devdrv_drv_err("device %d heart beat exception is detected, "
                   "device heart beat broken.\n",
                   info->dev_id);

    stamp = current_kernel_time();
    ret = devdrv_host_black_box_add_exception(info->dev_id, DEVDRV_BB_DEVICE_HEAT_BEAT_LOST, stamp, NULL);
    if (ret != 0) {
        devdrv_drv_err("devdrv_host_black_box_add_exception failed. dev_id(%u)\n", info->dev_id);
        return;
    }

    devdrv_hb_broken_stop_msg_send(info->dev_id);

    info->heart_beat.broken = 1;
    info->heart_beat.working = 0;
    devdrv_heart_beat_stop(info);
    devdrv_set_ts_down(&info->ts_mng[0]);
    devdrv_host_manager_device_exception(info);
    spin_lock_irqsave(&info->heart_beat.spinlock, flags);
    if (!list_empty_careful(&info->heart_beat.queue)) {
        list_for_each_safe(pos, n, &info->heart_beat.queue) {
            beat_node = list_entry(pos, struct devdrv_heart_beat_node, list);

            /* this function is called in workqueue,
             * some heart beat still in workqueue wait for schedule,
             * must not free memory.
             * and destroy workqueue cannot use in this function,
             * which will cause dead lock, you see destroy workqueue in
             * workqueue context.
             */
            beat_node->useless = 1;
        }
    }
    spin_unlock_irqrestore(&info->heart_beat.spinlock, flags);
}

STATIC void devdrv_heart_beat_ai_down(struct devdrv_info *info, const void *data)
{
    const struct devdrv_heart_beat_cq *cq = NULL;
    u32 core_bitmap;
    u32 core_count;
    u32 cpu_bitmap;
    u32 cpu_count;
    u32 i;

    cq = (struct devdrv_heart_beat_cq *)data;

    cpu_bitmap = 0;
    cpu_count = 0;
    core_bitmap = 0;
    core_count = 0;

    if (cq->aicpu_heart_beat_exception) {
        for (i = 0; i < info->ai_cpu_core_num; i++) {
            if (cq->aicpu_heart_beat_exception & (0x01U << i)) {
                cpu_bitmap |= (0x01U << i);
                if (!(info->inuse.ai_cpu_error_bitmap & (0x01U << i))) {
                    devdrv_drv_err("receive message ai cpu: %d heart beat exception. dev_id(%u)\n", i, info->dev_id);
                }
            } else
                cpu_count++;
        }
    }
    if (cq->aicore_bitmap) {
        for (i = 0; i < info->ai_core_num; i++) {
            if (cq->aicore_bitmap & (0x01U << i)) {
                core_bitmap |= (0x01U << i);
                if (!(info->inuse.ai_core_error_bitmap & (0x01U << i))) {
                    devdrv_drv_err("receive message ai core: %d exception. dev_id(%u)\n", i, info->dev_id);
                }
            } else
                core_count++;
        }
    }

    if (cq->syspcie_sysdma_status & 0xFFFF) {
        devdrv_drv_err("device(%u) ts sysdma is broken.\n", info->dev_id);
        info->ai_subsys_ip_broken_map |= (0x01U << DEVDRV_AI_SUBSYS_SDMA_WORKING_STATUS_OFFSET);
    }
    /* check the high 16 bits of syspcie_sysdma_status */
    if ((cq->syspcie_sysdma_status >> 16) & 0xFFFF) {
        devdrv_drv_err("device(%u) ts syspcie is broken.\n", info->dev_id);
        info->ai_subsys_ip_broken_map |= (0x01U << DEVDRV_AI_SUBSYS_SPCIE_WORKING_STATUS_OFFSET);
    }

    spin_lock_bh(&info->spinlock);
    info->inuse.ai_cpu_num = cpu_count;
    info->inuse.ai_cpu_error_bitmap = cpu_bitmap;
    info->inuse.ai_core_num = core_count;
    info->inuse.ai_core_error_bitmap = core_bitmap;
    spin_unlock_bh(&info->spinlock);
}

STATIC void devdrv_heart_beat_broken(struct work_struct *work)
{
    struct devdrv_heart_beat *hb = NULL;
    struct devdrv_info *info = NULL;

    hb = container_of(work, struct devdrv_heart_beat, work);
    info = container_of(hb, struct devdrv_info, heart_beat);

    /* jugde which exception is */
    if (hb->exception_info == NULL)
        devdrv_heart_beat_device_down(info);
    else
        devdrv_heart_beat_ai_down(info, hb->exception_info);
}

STATIC void devdrv_judge_device_work(struct devdrv_info *info, u32 number)
{
    struct devdrv_heart_beat_node *beat_node = NULL;
    struct devdrv_heart_beat_node *pprev_node = NULL;
    struct devdrv_heart_beat_node *prev_node = NULL;
    struct devdrv_heart_beat_sq *sq = NULL;
    struct list_head *pprev = NULL, *prev = NULL;
    struct list_head *pos = NULL, *n = NULL;
    unsigned long flags;
    int count;

    pprev = NULL;
    prev = NULL;
    pos = NULL;
    spin_lock_irqsave(&info->heart_beat.spinlock, flags);
    if (list_empty_careful(&info->heart_beat.queue)) {
        spin_unlock_irqrestore(&info->heart_beat.spinlock, flags);
        return;
    }
    count = 0;
    list_for_each_safe(pos, n, &info->heart_beat.queue)
    {
        beat_node = list_entry(pos, struct devdrv_heart_beat_node, list);
        sq = beat_node->sq;
        if (sq->number > number)
            count++;
        else {
            /* if heart beat failed continous, heart beat is out of control */
            if ((pprev != NULL) && (prev != NULL)) {
                pprev_node = list_entry(pprev, struct devdrv_heart_beat_node, list);
                prev_node = list_entry(prev, struct devdrv_heart_beat_node, list);
                if ((pprev_node->sq->number + 1 == prev_node->sq->number) &&
                    (prev_node->sq->number + 1 == beat_node->sq->number)) {
                    info->heart_beat.exception_info = NULL;
                    spin_unlock_irqrestore(&info->heart_beat.spinlock, flags);
                    devdrv_drv_err("device(%u) heart beat out of control.\n", info->dev_id);

                    devdrv_heart_beat_stop(info);
                    devdrv_heart_beat_broken(&info->heart_beat.work);
                    devdrv_drv_err("device(%u) devdrv_heart_beat_broken finish processing.\n", info->dev_id);
                    return;
                } else if (prev_node->sq->number + 1 == beat_node->sq->number) {
                    list_del(pprev);
                    kfree(pprev_node->sq);
                    kfree(pprev_node);
                    pprev_node = NULL;
                    pprev = prev;
                    prev = pos;
                } else {
                    list_del(pprev);
                    kfree(pprev_node->sq);
                    kfree(pprev_node);
                    pprev_node = NULL;
                    list_del(prev);
                    kfree(prev_node->sq);
                    kfree(prev_node);
                    prev_node = NULL;
                    pprev = NULL;
                    prev = pos;
                }
            } else {
                pprev = prev;
                prev = pos;
            }
        }
    }
    if (count > DEVDRV_HEART_BEAT_MAX_QUEUE)
        info->heart_beat.too_much = 1;
    else
        info->heart_beat.too_much = 0;
    spin_unlock_irqrestore(&info->heart_beat.spinlock, flags);
}

STATIC int is_cq_invalid(struct devdrv_heart_beat_cq *cq)
{
    if (cq->ts_status || cq->syspcie_sysdma_status || cq->aicpu_heart_beat_exception || cq->aicore_bitmap) {
        return -EINVAL;
    }
    return 0;
}

static unsigned int time_update_flag[DEVDRV_MAX_DAVINCI_NUM] = {0};
void set_time_update_flag(u32 dev_id)
{
    time_update_flag[dev_id] = DEVDRV_TIME_NEED_UPDATE;
}

void clear_time_update_flag(u32 dev_id)
{
    time_update_flag[dev_id] = DEVDRV_TIME_UPDATE_DONE;
}

int get_time_update_flag(u32 dev_id)
{
    return time_update_flag[dev_id];
}

STATIC void devdrv_heart_beat_work(struct work_struct *work)
{
    struct devdrv_manager_msg_info dev_manager_msg_info;
    struct timespec send_wall_time, check_wall_time;
    struct devdrv_heart_beat_node *beat_node = NULL;
    struct devdrv_manager_info *d_info = NULL;
    struct devdrv_heart_beat_sq *sq = NULL;
    struct devdrv_heart_beat_cq *cq = NULL;
    struct devdrv_info *info = NULL;
    static unsigned long new_time = 0;
    static unsigned long old_time;
    unsigned long flags;
    unsigned int i;
    u32 number;
    int out_len;
    int ret;

    beat_node = container_of(work, struct devdrv_heart_beat_node, work);
    d_info = devdrv_get_manager_info();
    sq = (struct devdrv_heart_beat_sq *)beat_node->sq;
    if (sq->devid >= DEVDRV_MAX_DAVINCI_NUM) {
        return;
    }
    info = d_info->dev_info[sq->devid];

    spin_lock_irqsave(&info->heart_beat.spinlock, flags);
    if (info->heart_beat.stop || beat_node->useless) {
        list_del(&beat_node->list);
        kfree(beat_node->sq);
        beat_node->sq = NULL;
        kfree(beat_node);
        beat_node = NULL;
        spin_unlock_irqrestore(&info->heart_beat.spinlock, flags);
        return;
    }
    spin_unlock_irqrestore(&info->heart_beat.spinlock, flags);

    dev_manager_msg_info.header.msg_id = DEVDRV_MANAGER_CHAN_H2D_HEART_BEAT;
    dev_manager_msg_info.header.valid = DEVDRV_MANAGER_MSG_VALID;
    /* give a random value for checking result later */
    dev_manager_msg_info.header.result = (u16)0x1A;
    /* inform corresponding devid to device side */
    dev_manager_msg_info.header.dev_id = info->dev_id;

    sq->reserved = DEVDRV_LINUX_LINUX_HOST;

    send_wall_time = current_kernel_time();
    sq->wall = timespec_to_timespec64(send_wall_time);

    old_time = new_time;
    new_time = send_wall_time.tv_sec;

    /* detect someone maybe change time */
    if ((new_time < old_time) || (new_time - old_time > DEVDRV_TIME_UPDATE_THRESHOLD)) {
        for (i = 0; i < DEVDRV_MAX_DAVINCI_NUM; i++) {
            set_time_update_flag(i);
        }
    }

    if (get_time_update_flag(sq->devid) == DEVDRV_TIME_NEED_UPDATE) {
        sq->time_update = DEVDRV_TIME_NEED_UPDATE;
    } else {
        sq->time_update = DEVDRV_TIME_UPDATE_DONE;
    }

    ret = memcpy_s(dev_manager_msg_info.payload, sizeof(dev_manager_msg_info.payload), sq,
                   sizeof(struct devdrv_heart_beat_sq));
    if (ret != 0) {
        devdrv_drv_err("copy from sq failed, ret(%d). dev_id(%u)\n", ret, info->dev_id);
        spin_lock_irqsave(&info->heart_beat.spinlock, flags);
        list_del(&beat_node->list);
        kfree(beat_node->sq);
        beat_node->sq = NULL;
        kfree(beat_node);
        beat_node = NULL;
        spin_unlock_irqrestore(&info->heart_beat.spinlock, flags);
        return;
    }
    number = sq->number;

    ret = devdrv_manager_send_msg(info, &dev_manager_msg_info, &out_len);
    if (ret || (dev_manager_msg_info.header.result != 0)) {
        devdrv_drv_err("devdrv_manager_send_msg failed, ret(%d), dev_id(%u).\n", ret, info->dev_id);
    } else {
        check_wall_time = current_kernel_time();
        if (check_wall_time.tv_sec == send_wall_time.tv_sec) {
            clear_time_update_flag(sq->devid);
        }

        spin_lock_irqsave(&info->heart_beat.spinlock, flags);
        list_del(&beat_node->list);
        kfree(beat_node->sq);
        beat_node->sq = NULL;
        kfree(beat_node);
        beat_node = NULL;
        spin_unlock_irqrestore(&info->heart_beat.spinlock, flags);

        cq = (struct devdrv_heart_beat_cq *)dev_manager_msg_info.payload;
        if (is_cq_invalid(cq) != 0) {
            info->heart_beat.exception_info = (void *)cq;
            devdrv_heart_beat_broken(&info->heart_beat.work);
        }
    }

    /* judge whether device is in exception */
    devdrv_drv_debug("judge whether device(%u) is in exception...\n", info->dev_id);
    devdrv_judge_device_work(info, number);
}

#if LINUX_VERSION_CODE >= KERNEL_VERSION(4, 15, 0)

STATIC void devdrv_heart_beat_event(struct timer_list *t)
{
    struct devdrv_heart_beat_node *beat_node = NULL;
    struct devdrv_manager_info *d_info = NULL;
    struct devdrv_heart_beat_sq *sq = NULL;
    struct devdrv_heart_beat *hb = NULL;
    struct devdrv_info *info = NULL;
    unsigned long flags;

    d_info = devdrv_get_manager_info();
    hb = from_timer(hb, t, timer);
    info = container_of(hb, struct devdrv_info, heart_beat);

#else /* LINUX_VERSION_CODE >= KERNEL_VERSION(4,15,0) */
STATIC void devdrv_heart_beat_event(unsigned long data)
{
    struct devdrv_heart_beat_node *beat_node = NULL;
    struct devdrv_manager_info *d_info = NULL;
    struct devdrv_heart_beat_sq *sq = NULL;
    struct devdrv_heart_beat *hb = NULL;
    struct devdrv_info *info = NULL;
    unsigned long flags;

    d_info = devdrv_get_manager_info();
    info = (struct devdrv_info *)(uintptr_t)data;

#endif /* LINUX_VERSION_CODE >= KERNEL_VERSION(4,15,0) */

    if (info->heart_beat.working == 0)
        return;

    if (info->heart_beat.stop)
        goto out;

    spin_lock_irqsave(&info->heart_beat.spinlock, flags);
    if (info->heart_beat.too_much) {
        spin_unlock_irqrestore(&info->heart_beat.spinlock, flags);
        devdrv_drv_err("device(%u) too much heart beat waiting, ignore this beat.\n", info->dev_id);
        goto out;
    }
    spin_unlock_irqrestore(&info->heart_beat.spinlock, flags);

    hb = &info->heart_beat;

    sq = kzalloc(sizeof(struct devdrv_heart_beat_sq), GFP_ATOMIC);
    if (sq == NULL) {
        devdrv_drv_err("device(%u) kmalloc in time event fail once,"
                       "give up sending heart beat this time.\n",
                       info->dev_id);
        goto out;
    }
    beat_node = kzalloc(sizeof(struct devdrv_heart_beat_node), GFP_ATOMIC);
    if (beat_node == NULL) {
        kfree(sq);
        sq = NULL;
        devdrv_drv_err("device(%u) kmalloc in time event fail once,"
                       "give up sending heart beat this time.\n",
                       info->dev_id);
        goto out;
    }

    sq->number = hb->cmd_inc_counter;
    sq->devid = info->dev_id;
    sq->cmd = DEVDRV_HEART_BEAT_SQ_CMD;

    beat_node->sq = sq;
    beat_node->useless = 0;
    spin_lock_irqsave(&info->heart_beat.spinlock, flags);
    list_add_tail(&beat_node->list, &hb->queue);
    spin_unlock_irqrestore(&info->heart_beat.spinlock, flags);

    hb->cmd_inc_counter++;
    INIT_WORK(&beat_node->work, devdrv_heart_beat_work);
    queue_work(info->heart_beat_wq, &beat_node->work);

out:
    info->heart_beat.timer.expires = jiffies + DEVDRV_HEART_BEAT_CYCLE * HZ;
    add_timer(&info->heart_beat.timer);
}

int devdrv_heart_beat_init(struct devdrv_info *info)
{
    if (!info->host_config.heart_beat_en) {
        devdrv_drv_err("host config: do not send heart beat to device(%u).\n", info->dev_id);
        info->heart_beat.stop = 1;
        info->heart_beat.sq = DEVDRV_MAX_FUNCTIONAL_SQ_NUM;
        info->heart_beat.cq = DEVDRV_MAX_FUNCTIONAL_CQ_NUM;
        return -EINVAL;
    }

    info->heart_beat_wq = create_singlethread_workqueue("heart_beat_work");
    if (info->heart_beat_wq == NULL) {
        devdrv_drv_err("create_workqueue error. device(%u)\n", info->dev_id);
        return -ENOMEM;
    }

    INIT_LIST_HEAD(&info->heart_beat.queue);
    spin_lock_init(&info->heart_beat.spinlock);
    INIT_WORK(&info->heart_beat.work, devdrv_heart_beat_broken);

    info->heart_beat.cmd_inc_counter = 0;
    info->heart_beat.sq = 0;
    info->heart_beat.cq = 0;
    info->heart_beat.exception_info = NULL;
    info->heart_beat.stop = 0;
    info->heart_beat.too_much = 0;
    info->heart_beat.broken = 0;
    info->heart_beat.working = 1;

#if LINUX_VERSION_CODE >= KERNEL_VERSION(4, 15, 0)
    timer_setup(&info->heart_beat.timer, devdrv_heart_beat_event, 0);
#else
    setup_timer(&info->heart_beat.timer, devdrv_heart_beat_event, (unsigned long)(uintptr_t)info);
#endif
    info->heart_beat.timer.expires = jiffies + DEVDRV_HEART_BEAT_CYCLE * HZ;
    add_timer(&info->heart_beat.timer);

    return 0;
}

void devdrv_heart_beat_exit(struct devdrv_info *info)
{
    struct devdrv_heart_beat_node *beat_node = NULL;
    struct list_head *pos = NULL, *n = NULL;
    unsigned long flags;

    if ((info->heart_beat.sq >= DEVDRV_MAX_FUNCTIONAL_SQ_NUM) || (info->heart_beat.cq >= DEVDRV_MAX_FUNCTIONAL_CQ_NUM))
        return; /* heart beat info has been cleaned */

    devdrv_heart_beat_stop(info);
    info->heart_beat.working = 0;
    del_timer_sync(&info->heart_beat.timer);

    info->heart_beat.sq = DEVDRV_MAX_FUNCTIONAL_SQ_NUM;
    info->heart_beat.cq = DEVDRV_MAX_FUNCTIONAL_CQ_NUM;

    destroy_workqueue(info->heart_beat_wq);

    spin_lock_irqsave(&info->heart_beat.spinlock, flags);
    if (!list_empty_careful(&info->heart_beat.queue)) {
        list_for_each_safe(pos, n, &info->heart_beat.queue)
        {
            beat_node = list_entry(pos, struct devdrv_heart_beat_node, list);
            list_del(&beat_node->list);
            kfree(beat_node->sq);
            kfree(beat_node);
        }
    }
    spin_unlock_irqrestore(&info->heart_beat.spinlock, flags);
}
