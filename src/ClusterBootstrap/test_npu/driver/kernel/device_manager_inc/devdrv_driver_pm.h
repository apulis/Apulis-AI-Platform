/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2012-2019. All rights reserved.
 * Description:
 * Author: huawei
 * Create: 2019-10-15
 */
#ifndef __DEVDRV_DRIVER_PM_H
#define __DEVDRV_DRIVER_PM_H

#include "devdrv_common.h"

void devdrv_set_power_mode(struct devdrv_cce_ctrl *cce_ctrl,
    enum devdrv_power_mode mode);
int devdrv_is_low_power(struct devdrv_cce_ctrl *cce_ctrl);
int devdrv_is_ts_work(struct devdrv_ts_mng *ts_mng);
void devdrv_set_ts_sleep(struct devdrv_ts_mng *ts_mng);
void devdrv_set_ts_work(struct devdrv_ts_mng *ts_mng);
void devdrv_set_ts_down(struct devdrv_ts_mng *ts_mng);
void devdrv_set_ts_initing(struct devdrv_ts_mng *ts_mng);
void devdrv_set_ts_booting(struct devdrv_ts_mng *ts_mng);
int devdrv_set_ts_status(struct devdrv_ts_mng *ts_mng,
    enum devdrv_ts_status status);
int devdrv_driver_try_suspend(struct devdrv_cce_ctrl *cce_ctrl);
int devdrv_driver_try_resume(struct devdrv_cce_ctrl *cce_ctrl);
void devdrv_driver_hardware_exception(struct devdrv_info *info,
    uint32_t tsid);
int devdrv_is_ts_ready(int devid);
#endif
