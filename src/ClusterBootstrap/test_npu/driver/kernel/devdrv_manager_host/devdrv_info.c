/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2012-2019. All rights reserved.
 * Description:
 * Author: huawei
 * Create:  2020-1-19
 */

#include "devdrv_common.h"

int devdrv_manager_get_transway(struct file *filep, unsigned int cmd, unsigned long arg)
{
    struct devdrv_trans_info devdrv_trans_info = {0};
    u32 dest_devid;
    u32 src_devid;
    int ret;

    ret = copy_from_user_safe(&devdrv_trans_info, (void *)(uintptr_t)arg, sizeof(devdrv_trans_info));
    if (ret) {
        devdrv_drv_err("copy from user failed, ret(%d).\n", ret);
        return -EFAULT;
    }

    dest_devid = devdrv_trans_info.dest_devid;
    src_devid = devdrv_trans_info.src_devid;

#if defined(CFG_SOC_PLATFORM_MINI)
#if defined(CFG_SOC_PLATFORM_MINIV2)
    devdrv_trans_info.ways = DRV_SDMA;
#else
    if (dest_devid == src_devid)
        devdrv_trans_info.ways = DRV_SDMA;
    else
        devdrv_trans_info.ways = DRV_PCIE_DMA;
#endif
#else
    devdrv_trans_info.ways = DRV_SDMA;
#endif
    devdrv_drv_info("transway is %u.\n", devdrv_trans_info.ways);

    ret = copy_to_user_safe((void *)(uintptr_t)arg, &devdrv_trans_info, sizeof(devdrv_trans_info));
    if (ret) {
        devdrv_drv_err("copy from user failed, ret(%d).\n", ret);
        return -EINVAL;
    }
    return 0;
}



