/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2012-2020. All rights reserved.
 * Description:
 * Author: huawei
 * Create: 2020-02-04
 *
 * This program is free software; you can redistribute it and /or modify it
 * under the terms of the GNU General Public License as published by the Free
 * Software Foundation; either version 2 of the License, or (at your option)
 * any later version
 */

#ifndef _HDCDRV_HOST_H_
#define _HDCDRV_HOST_H_

#define PCI_VENDOR_ID_HUAWEI 0x19e5
#define STATIC

typedef int (*container_virtual_to_physical_devid)(u32, u32 *);
typedef int (*devdrv_manager_get_run_env)(struct mnt_namespace *mnt_ns);
typedef int (*hdcdrv_is_in_container)(void);
typedef int (*get_container_id)(u32  *docker_id);


#endif
