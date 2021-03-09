/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2012-2019. All rights reserved.
 * Description:
 * Author: huawei
 * Create: 2019-10-15
 */
#include <linux/atomic.h>
#include <linux/kernel.h>
#include <linux/list.h>
#include <linux/types.h>
#include <linux/sched.h>
#include <linux/wait.h>

#include "devdrv_common.h"
#include "devdrv_manager.h"
#include "devdrv_driver_pm.h"

void devdrv_set_power_mode(struct devdrv_cce_ctrl *cce_ctrl, enum devdrv_power_mode mode)
{
    if ((cce_ctrl != NULL) && (mode < DEVDRV_MAX_MODE)) {
        atomic_set(&cce_ctrl->power_mode, mode);
    } else {
        devdrv_drv_err("invalid param, (cce_ctrl == NULL)=%d, mode=%d.\n",
            (cce_ctrl == NULL), mode);
    }
}

int devdrv_is_low_power(struct devdrv_cce_ctrl *cce_ctrl)
{
    enum devdrv_power_mode mode;

    if (cce_ctrl == NULL) {
        devdrv_drv_err("invalid input argument.\n");
        return 0;
    }

    mode = (enum devdrv_power_mode)atomic_read(&cce_ctrl->power_mode);

    return (mode == DEVDRV_LOW_POWER) ? IS : 0;
}
EXPORT_SYMBOL(devdrv_is_low_power);

int devdrv_is_ts_work(struct devdrv_ts_mng *ts_mng)
{
    unsigned long flags;
    u32 status;

    if (ts_mng == NULL) {
        devdrv_drv_err("invalid input argument.\n");
        return 0;
    }
    spin_lock_irqsave(&ts_mng->ts_spinlock, flags);
    status = ts_mng->ts_work_status_shadow;
    spin_unlock_irqrestore(&ts_mng->ts_spinlock, flags);

    return (status == DEVDRV_TS_WORK) ? IS : 0;
}
EXPORT_SYMBOL(devdrv_is_ts_work);

int devdrv_is_ts_ready(int devid)
{
    struct devdrv_info *info = NULL;

    info = devdrv_manager_get_devdrv_info(devid);
    if (info == NULL) {
        devdrv_drv_info("device is not ready.\n");
        return 0;
    }

    return devdrv_is_ts_work(&info->ts_mng[0]);
}
EXPORT_SYMBOL(devdrv_is_ts_ready);

void devdrv_set_ts_sleep(struct devdrv_ts_mng *ts_mng)
{
    unsigned long flags;

    if (ts_mng == NULL) {
        devdrv_drv_err("invalid input argument.\n");
        return;
    }

    spin_lock_irqsave(&ts_mng->ts_spinlock, flags);
    ts_mng->ts_work_status_shadow = DEVDRV_TS_SLEEP;
    if (ts_mng->ts_work_status)
        *ts_mng->ts_work_status = DEVDRV_TS_SLEEP;
    spin_unlock_irqrestore(&ts_mng->ts_spinlock, flags);
}

void devdrv_set_ts_work(struct devdrv_ts_mng *ts_mng)
{
    unsigned long flags;

    if (ts_mng == NULL) {
        devdrv_drv_err("invalid input argument.\n");
        return;
    }

    spin_lock_irqsave(&ts_mng->ts_spinlock, flags);
    ts_mng->ts_work_status_shadow = DEVDRV_TS_WORK;
    if (ts_mng->ts_work_status)
        *ts_mng->ts_work_status = DEVDRV_TS_WORK;
    spin_unlock_irqrestore(&ts_mng->ts_spinlock, flags);
}

void devdrv_set_ts_down(struct devdrv_ts_mng *ts_mng)
{
    unsigned long flags;

    if (ts_mng == NULL) {
        devdrv_drv_err("invalid input argument.\n");
        return;
    }

    spin_lock_irqsave(&ts_mng->ts_spinlock, flags);
    ts_mng->ts_work_status_shadow = DEVDRV_TS_DOWN;
    if (ts_mng->ts_work_status)
        *ts_mng->ts_work_status = DEVDRV_TS_DOWN;
    spin_unlock_irqrestore(&ts_mng->ts_spinlock, flags);
}

void devdrv_set_ts_initing(struct devdrv_ts_mng *ts_mng)
{
    unsigned long flags;
    if (ts_mng == NULL) {
        devdrv_drv_err("invalid input argument.\n");
        return;
    }

    spin_lock_irqsave(&ts_mng->ts_spinlock, flags);
    ts_mng->ts_work_status_shadow = DEVDRV_TS_INITING;
    if (ts_mng->ts_work_status)
        *ts_mng->ts_work_status = DEVDRV_TS_INITING;
    spin_unlock_irqrestore(&ts_mng->ts_spinlock, flags);
}

void devdrv_set_ts_booting(struct devdrv_ts_mng *ts_mng)
{
    unsigned long flags;
    if (ts_mng == NULL) {
        devdrv_drv_err("invalid input argument.\n");
        return;
    }

    spin_lock_irqsave(&ts_mng->ts_spinlock, flags);
    ts_mng->ts_work_status_shadow = DEVDRV_TS_BOOTING;
    if (ts_mng->ts_work_status)
        *ts_mng->ts_work_status = DEVDRV_TS_BOOTING;
    spin_unlock_irqrestore(&ts_mng->ts_spinlock, flags);
}

int devdrv_set_ts_status(struct devdrv_ts_mng *ts_mng, enum devdrv_ts_status status)
{
    unsigned long flags;

    if ((ts_mng == NULL) || (status >= DEVDRV_TS_MAX_STATUS)) {
        devdrv_drv_err("invalid input argument.\n");
        return -EINVAL;
    }

    spin_lock_irqsave(&ts_mng->ts_spinlock, flags);
    ts_mng->ts_work_status_shadow = status;
    if (ts_mng->ts_work_status)
        *ts_mng->ts_work_status = status;
    spin_unlock_irqrestore(&ts_mng->ts_spinlock, flags);

    return 0;
}

int devdrv_driver_try_suspend(struct devdrv_cce_ctrl *cce_ctrl)
{
    if (cce_ctrl == NULL) {
        devdrv_drv_err("invalid input argument.\n");
        return -EINVAL;
    }

    atomic_set(&cce_ctrl->power_mode, DEVDRV_LOW_POWER);

    return 0;
}

int devdrv_driver_try_resume(struct devdrv_cce_ctrl *cce_ctrl)
{
    if (cce_ctrl == NULL) {
        devdrv_drv_err("invalid input argument.\n");
        return -EINVAL;
    }

    atomic_set(&cce_ctrl->power_mode, DEVDRV_NORMAL_POWER);

    return 0;
}

void devdrv_driver_hardware_exception(struct devdrv_info *info, uint32_t tsid)
{
    struct devdrv_cce_context *cce_context = NULL;
    struct list_head *pos = NULL, *n = NULL;

    if ((info == NULL) || (tsid >= DEVDRV_MAX_TS_NUM)) {
        devdrv_drv_err("invalid input argument.\n");
        return;
    }

    if (info->cce_ctrl[tsid] == NULL)
        return;

    spin_lock_bh(&info->spinlock);
    if (!list_empty_careful(&info->cce_context_list)) {
        list_for_each_safe(pos, n, &info->cce_context_list) {
            cce_context = list_entry(pos, struct devdrv_cce_context, cce_ctrl_list);
            cce_context->ts_context[tsid].last_ts_status = DEVDRV_TS_DOWN;
            cce_context->ts_context[tsid].cq_tail_updated = CQ_HEAD_UPDATE_FLAG;
            wake_up(&cce_context->ts_context[tsid].report_wait);
        }
    }
    spin_unlock_bh(&info->spinlock);
}
