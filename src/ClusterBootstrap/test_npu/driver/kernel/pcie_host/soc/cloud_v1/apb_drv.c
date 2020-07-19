/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2012-2019. All rights reserved.
 * Description: This program is free software; you can redistribute it and /or modify it
 *                     under the terms of the GNU General Public License as published by the Free
 *                     Software Foundation; either version 2 of the License, or (at your option)
 *                     any later version
 * Author: huawei 
 * Create:2019
 */

#include <asm/io.h>

#include "apb_drv.h"

#define CHIP_0_ECAM_BUS_ID 0x7B
#define CHIP_1_ECAM_BUS_ID 0xBB
#define CHIP_2_ECAM_BUS_ID 0xDB
#define CHIP_3_ECAM_BUS_ID 0xFB

int devdrv_get_devid_by_bus(unsigned char bus_number, int *devid)
{
    int ret = 0;

    switch (bus_number) {
        case CHIP_0_ECAM_BUS_ID:
            *devid = 0;
            break;
        case CHIP_1_ECAM_BUS_ID:
            *devid = 1;
            break;
        case CHIP_2_ECAM_BUS_ID:
            *devid = 2; /* CHIP2 */
            break;
        case CHIP_3_ECAM_BUS_ID:
            *devid = 3; /* CHIP3 */
            break;
        default:
            ret = -1;
            break;
    }

    return ret;
}

int agentdrv_get_rc_ep_mode(u32 *mode)
{
    *mode = DEVDRV_PCIE_EP_MODE;

    return 0;
}

u32 agentdrv_soc_get_func_total(void)
{
    return 1;
}
