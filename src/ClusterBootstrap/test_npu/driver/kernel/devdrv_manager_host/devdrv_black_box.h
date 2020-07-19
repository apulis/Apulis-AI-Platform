/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2012-2019. All rights reserved.
 * Description:
 * Author: huawei
 * Create: 2019-10-15
 */

#ifndef __DEVDRV_BLACK_BOX_H__
#define __DEVDRV_BLACK_BOX_H__

#include "devdrv_interface.h"
#include "devdrv_user_common.h"

void devdrv_host_black_box_init(void);
void devdrv_host_black_box_exit(void);
void devdrv_host_black_box_close_check(pid_t pid);
int devdrv_host_black_box_add_exception(u32 devid, u32 code, struct timespec stamp, void *data);
void devdrv_host_black_box_get_exception(struct devdrv_black_box_user *black_box_user);
extern int devdrv_register_black_callback(struct devdrv_black_callback *black_callback);
extern void devdrv_unregister_black_callback(struct devdrv_black_callback *black_callback);
#endif