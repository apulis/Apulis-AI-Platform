/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2012-2019. All rights reserved.
 * Description:
 * Author: huawei
 * Create: 2019-10-15
 */

#include <linux/idr.h>
#include <linux/device.h>
#include <linux/errno.h>
#include <linux/kernel.h>
#include <linux/spinlock.h>
#include <linux/types.h>
#include <linux/slab.h>
#include <linux/mutex.h>
#include <linux/swap.h>
#include <asm/uaccess.h>
#include <linux/gfp.h>

#include "devdrv_common.h"

int devdrv_shm_init(struct devdrv_info *dev_info, u32 tsid)
{
    gfp_t gfp_flags = GFP_KERNEL | __GFP_COMP | __GFP_ZERO;
    struct devdrv_platform_data *pdata = NULL;
    struct devdrv_cce_ctrl *cce_ctrl = NULL;
    char *tmp = NULL;
    u32 order;

    if ((dev_info == NULL) || (dev_info->cce_ctrl[tsid] == NULL) || (dev_info->pdata == NULL)) {
        devdrv_drv_err("invalid parameter\n");
        return -EINVAL;
    }
    pdata = dev_info->pdata;
    cce_ctrl = dev_info->cce_ctrl[tsid];

    cce_ctrl->devdrv_uio_info = kzalloc(sizeof(struct uio_info), GFP_KERNEL);
    if (cce_ctrl->devdrv_uio_info == NULL) {
        devdrv_drv_err("[dev_id = %u]:kzalloc failed\n", dev_info->dev_id);
        return -ENOMEM;
    }
    cce_ctrl->devdrv_uio_info->name = "devdrv";
    cce_ctrl->devdrv_uio_info->version = "0.0.1";

    order = DEVDRV_MAX_INFO_ORDER;

    tmp = (char *)(uintptr_t)__get_free_pages(gfp_flags, order);
    if (tmp == NULL) {
        kfree(cce_ctrl->devdrv_uio_info);
        cce_ctrl->devdrv_uio_info = NULL;
        devdrv_drv_err("[dev_id = %u]:devdrv_drv_register shm_add no enough memory !!!!\n", dev_info->dev_id);
        return -ENOMEM;
    }

    /*
     * MAP0: info memory
     * sq info
     * cq info
     * stream info
     */
    cce_ctrl->devdrv_uio_info->mem[DEVDRV_INFO_MAP].addr = (phys_addr_t)(uintptr_t)tmp;
    cce_ctrl->devdrv_uio_info->mem[DEVDRV_INFO_MAP].memtype = UIO_MEM_LOGICAL;
    cce_ctrl->devdrv_uio_info->mem[DEVDRV_INFO_MAP].size = (long)(unsigned)(1 << order) * PAGE_SIZE;

    /* MAP1: doorbell memory */
    cce_ctrl->devdrv_uio_info->mem[DEVDRV_DOORBELL_MAP].addr = pdata->ts_pdata[tsid].doorbell_paddr;
    cce_ctrl->devdrv_uio_info->mem[DEVDRV_DOORBELL_MAP].memtype = UIO_MEM_PHYS;
    cce_ctrl->devdrv_uio_info->mem[DEVDRV_DOORBELL_MAP].size = pdata->ts_pdata[tsid].doorbell_size;

    cce_ctrl->mem_info[DEVDRV_SQ_MEM].bar_addr = pdata->ts_pdata[tsid].ts_sq_static_addr;
    cce_ctrl->mem_info[DEVDRV_SQ_MEM].phy_addr = DEVDRV_RESERVE_MEM_BASE +
                                                 (phys_addr_t)tsid * (phys_addr_t)DEVDRV_RESERVE_MEM_SIZE +
                                                 (CHIP_BASEADDR_PA_OFFSET * dev_info->dev_id);
    cce_ctrl->mem_info[DEVDRV_SQ_MEM].size = DEVDRV_RESERVE_MEM_SIZE;

    cce_ctrl->mem_info[DEVDRV_INFO_MEM].phy_addr = virt_to_phys(tmp);
    cce_ctrl->mem_info[DEVDRV_INFO_MEM].virt_addr = (phys_addr_t)(uintptr_t)tmp;
    cce_ctrl->mem_info[DEVDRV_INFO_MEM].size = (long)(unsigned)(1 << order) * PAGE_SIZE;

    cce_ctrl->mem_info[DEVDRV_DOORBELL_MEM].phy_addr = pdata->ts_pdata[tsid].doorbell_paddr;
    cce_ctrl->mem_info[DEVDRV_DOORBELL_MEM].size = pdata->ts_pdata[tsid].doorbell_size;

    devdrv_drv_debug("tsid(%u)\n", cce_ctrl->tsid);
    return 0;
}

void devdrv_shm_destroy(struct devdrv_cce_ctrl *cce_ctrl)
{
    unsigned long addr;
    u32 order;

    if ((cce_ctrl == NULL) || (cce_ctrl->devdrv_uio_info == NULL)) {
        devdrv_drv_err("invalid parameter\n");
        return;
    }
    order = DEVDRV_MAX_INFO_ORDER;
    addr = (unsigned long)cce_ctrl->devdrv_uio_info->mem[DEVDRV_INFO_MAP].addr;

    free_pages(addr, order);
    addr = 0;
    kfree(cce_ctrl->devdrv_uio_info);
    cce_ctrl->devdrv_uio_info = NULL;
}
