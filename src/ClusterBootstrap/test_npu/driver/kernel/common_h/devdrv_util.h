/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2012-2019. All rights reserved.
 * Description: Common Definition
 * Author: Huawei Technologies Co., Ltd.
 * Create: 2018/8/18
 */
#ifndef _DEVDRV_UTIL_H_
#define _DEVDRV_UTIL_H_

#include <linux/types.h>
#include "drv_log.h"

#define module_devdrv "drv_pcie"
#define devdrv_err(fmt...) drv_err(module_devdrv, fmt)
#define devdrv_warn(fmt...) drv_warn(module_devdrv, fmt)
#define devdrv_info(fmt...) drv_info(module_devdrv, fmt)
#define devdrv_debug(fmt...)

#define UNUSED(expr) do { \
    (void)(expr); \
} while (0)

#ifdef DRV_UT
#define STATIC
#else
#define STATIC static
#endif

#define DEVDRV_INVALID 0
#define DEVDRV_VALID 1
#define AGENTDRV_INVALID 0
#define AGENTDRV_EVAID 1

#define DEVDRV_DISABLE 0
#define DEVDRV_ENABLE 1
#define AGENTDRV_DISABLE 0
#define AGENTDRV_ENABLE 1

#define AGENTDRV_MSLEEP_10 10

#endif
