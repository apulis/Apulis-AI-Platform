/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2012-2019. All rights reserved.
 * Description:
 * Author: huawei
 * Create: 2019-10-15
 */

#include "devdrv_svmapi.h"

void devdrv_manager_set_svm_report_handle(devmm_report_handle_t p)
{
    devmm_set_report_handle(p);
}

u32 devdrv_manager_va_to_devid(unsigned long va)
{
    return devmm_svm_va_to_devid(va);
}

