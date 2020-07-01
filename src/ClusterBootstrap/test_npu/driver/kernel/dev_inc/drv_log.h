/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2012-2019. All rights reserved.
 * Description: This file contains drv_log Info.
 * Author: huawei
 * Create: 2017-10-15
 */

#ifndef _DRV_LOG_H_
#define _DRV_LOG_H_

#include <linux/types.h>

/* host ko depmod */
#define PCI_DEVICE_CLOUD 0xa126

#define drv_printk(level, module, fmt, ...) \
    printk(level " [%s] [%s %d] " fmt, module, __func__, __LINE__, ##__VA_ARGS__)

#ifdef LOG_UT
#define drv_err(module, fmt...) drv_printk(KERN_ERR, module, fmt)
#else
#ifdef CFG_SOC_PLATFORM_CLOUD_HOST
#define logflow_printk(level, module, fmt, ...) \
    printk(level "[ERROR] [%s] [%s %d] " fmt, module, __func__, __LINE__, ##__VA_ARGS__)
#define drv_err(module, fmt...) logflow_printk(KERN_NOTICE, module, fmt)
#else
#define drv_err(module, fmt...) drv_printk(KERN_ERR, module, fmt)
#endif
#endif

#define drv_warn(module, fmt...) drv_printk(KERN_WARNING, module, fmt)
#define drv_info(module, fmt...) drv_printk(KERN_INFO, module, fmt)
#define drv_debug(module, fmt...) drv_printk(KERN_DEBUG, module, fmt)
#define drv_event(module, fmt...) drv_printk(KERN_NOTICE, module, fmt)

#endif /* _DRV_LOG_H_ */
