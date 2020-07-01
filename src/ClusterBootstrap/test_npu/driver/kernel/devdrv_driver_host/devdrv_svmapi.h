/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2012-2019. All rights reserved.
 * Description:
 * Author: huawei
 * Create: 2019-10-15
 */
#ifndef _DEVDRV_SVMAPI_H_
#define _DEVDRV_SVMAPI_H_

#include "devdrv_common.h"

u32 devdrv_manager_va_to_devid(unsigned long va);
void devdrv_manager_set_svm_report_handle(devmm_report_handle_t p);

extern int devmm_alloc_device_mem(unsigned int devid, phys_addr_t *pp, size_t bytesize);
extern int devmm_free_device_mem(unsigned int devid, phys_addr_t p);
extern int devdrv_devmem_addr_h2d(u32 devid, phys_addr_t host_bar_addr, phys_addr_t *device_phy_addr);
extern u32 devmm_svm_va_to_devid(unsigned long va);
extern void devmm_set_report_handle(devmm_report_handle_t p);

#endif
