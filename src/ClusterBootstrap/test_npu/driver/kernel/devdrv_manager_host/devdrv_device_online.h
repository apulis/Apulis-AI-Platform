/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2012-2019. All rights reserved.
 * Description:
 * Author: huawei
 * Create: 2020-03-10
 */
#ifndef __DEVDRV_DEVICE_ONLINE_H
#define __DEVDRV_DEVICE_ONLINE_H
#include "devdrv_manager_common.h"

void devdrv_manager_online_devid_update(u32 dev_id);
int devdrv_manager_online_get_devids(struct file *filep, unsigned int cmd, unsigned long arg);
u32 devdrv_manager_poll(struct file *filep, struct poll_table_struct *wait);
int devdrv_manager_online_kfifo_alloc(void);
void devdrv_manager_online_kfifo_free(void);

#endif


