/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2012-2019. All rights reserved.
 * Description: atu common interfaces
 * Author: huawei
 * Create: 2017/8/18
 */
#include "devdrv_atu_interface.h"
#include "devdrv_atu.h"
#include "devdrv_util.h"

int devdrv_atu_base_to_target(struct devdrv_iob_atu atu[], int num, u64 base_addr, u64 *target_addr)
{
    int i;

    for (i = 0; i < num; i++) {
        if (atu[i].valid == ATU_INVALID)
            continue;

        if ((atu[i].base_addr <= base_addr) && ((atu[i].base_addr + atu[i].size) > base_addr)) {
            *target_addr = base_addr - atu[i].base_addr + atu[i].target_addr;
            return 0;
        }
    }
    return -EINVAL;
}

int devdrv_atu_target_to_base(struct devdrv_iob_atu atu[], int num, u64 target_addr, u64 *base_addr)
{
    int i;

    for (i = 0; i < num; i++) {
        if (atu[i].valid == ATU_INVALID)
            continue;

        if ((atu[i].target_addr <= target_addr) && ((atu[i].target_addr + atu[i].size) > target_addr)) {
            *base_addr = target_addr - atu[i].target_addr + atu[i].base_addr;
            return 0;
        }
    }

    return -EINVAL;
}

int devdrv_devmem_addr_d2h(u32 devid, phys_addr_t device_phy_addr, phys_addr_t *host_bar_addr)
{
    struct devdrv_iob_atu *atu = NULL;
    u64 host_phy_base = 0;

    if (host_bar_addr == NULL) {
        devdrv_err("dev %u host_bar_addr is null\n", devid);
        return -EINVAL;
    }

    if (devdrv_get_atu_info(devid, ATU_TYPE_RX_MEM, &atu, &host_phy_base)) {
        devdrv_err("dev %u find atu failed.\n", devid);
        return -EINVAL;
    }

    if (devdrv_atu_target_to_base(atu, DEVDRV_MAX_RX_ATU_NUM, (u64)device_phy_addr, (u64 *)host_bar_addr)) {
        devdrv_err("dev %u device_phy_addr not found.\n", devid);
        return -EINVAL;
    }

    *host_bar_addr += host_phy_base;

    return 0;
}
EXPORT_SYMBOL(devdrv_devmem_addr_d2h);

int devdrv_devmem_addr_h2d(u32 devid, phys_addr_t host_bar_addr, phys_addr_t *device_phy_addr)
{
    struct devdrv_iob_atu *atu = NULL;
    u64 host_phy_base = 0;

    if (device_phy_addr == NULL) {
        devdrv_err("dev %u device_phy_addr is null\n", devid);
        return -EINVAL;
    }

    if (devdrv_get_atu_info(devid, ATU_TYPE_RX_MEM, &atu, &host_phy_base)) {
        devdrv_err("dev %u find atu failed.\n", devid);
        return -EINVAL;
    }

    if (host_bar_addr < host_phy_base) {
        devdrv_err("dev %u host_bar_addr is small than host_phy_base\n", devid);
        return -EINVAL;
    }

    if (devdrv_atu_base_to_target(atu, DEVDRV_MAX_RX_ATU_NUM, (u64)(host_bar_addr - host_phy_base),
        (u64 *)device_phy_addr)) {
        devdrv_err("dev %u host_bar_addr not found.\n", devid);
        return -EINVAL;
    }

    return 0;
}
EXPORT_SYMBOL(devdrv_devmem_addr_h2d);
