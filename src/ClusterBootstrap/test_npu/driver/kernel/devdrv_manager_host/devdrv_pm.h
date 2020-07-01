/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2012-2019. All rights reserved.
 * Description:
 * Author: huawei
 * Create: 2019-10-15
 */
#ifndef __DEVDRV_PM_H
#define __DEVDRV_PM_H

#include <linux/list.h>

#define DEVDRV_WAKELOCK_TIMEOUT_SECOND 10 /* second */

struct devdrv_pm {
    int (*suspend)(u32 devid, u32 status);
    int (*resume)(u32 devid);
    struct list_head list;
};

struct devdrv_pm *devdrv_manager_register_pm(int (*suspend)(u32 devid, u32 status), int (*resume)(u32 devid));
void devdrv_manager_unregister_pm(struct devdrv_pm *pm);
int devdrv_host_manager_device_suspend(struct devdrv_info *info);
int devdrv_host_manager_device_resume(struct devdrv_info *info);
void devdrv_host_manager_device_exception(struct devdrv_info *info);
int devdrv_heart_beat_init(struct devdrv_info *info);
void devdrv_heart_beat_exit(struct devdrv_info *info);
void devdrv_heart_beat_stop(struct devdrv_info *info);
void devdrv_heart_beat_start(struct devdrv_info *info);

void set_time_update_flag(u32 dev_id);
void clear_time_update_flag(u32 dev_id);
int get_time_update_flag(u32 dev_id);

#endif
