/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2012-2019. All rights reserved.
 * Description:
 * Author: huawei
 * Create: 2019-10-15
 */
#ifndef __DEVDRV_SHM_H
#define __DEVDRV_SHM_H

#include "devdrv_common.h"

int devdrv_shm_init(struct devdrv_info *info, u32 tsid);
void devdrv_shm_destroy(struct devdrv_cce_ctrl *cce_ctrl);

#endif
