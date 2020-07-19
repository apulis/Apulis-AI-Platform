/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2012-2019. All rights reserved.
 * Description:
 * Author: huawei
 * Create: 2019-10-15
 */
#include <linux/uaccess.h>
#include <linux/slab.h>
#include <linux/errno.h>

#include "devdrv_common.h"
#include "devdrv_user_common.h"
#include "devdrv_manager_dfx.h"
#include "devdrv_manager_common.h"
#include "devdrv_manager_container.h"
#include "devdrv_manager.h"

#define DEVDRV_CQ_SLOT_START (DEVDRV_VM_MEM_START + DEVDRV_VM_BLOCK_OFFSET * 3)
#define DEVDRV_CQ_SLOT_END (DEVDRV_VM_MEM_START + DEVDRV_VM_BLOCK_OFFSET * 3 + DEVDRV_VM_CQ_QUEUE_SIZE)

STATIC void devdrv_dfx_ts_status(struct devdrv_status_info *status)
{
    devdrv_drv_warn(" ******************* ts debug info ************************ \n");

    switch (status->ts_status) {
        case DEVDRV_TS_WORK:
            devdrv_drv_warn("TS is working.\n");
            break;
        case DEVDRV_TS_SLEEP:
            devdrv_drv_warn("TS is sleeping.\n");
            break;
        case DEVDRV_TS_DOWN:
            devdrv_drv_warn("TS is in exception.\n");
            break;
        case DEVDRV_TS_INITING:
            devdrv_drv_warn("TS is in initing.\n");
            break;
        case DEVDRV_TS_BOOTING:
            devdrv_drv_warn("TS is in booting.\n");
            break;
        case DEVDRV_TS_FAIL_TO_SUSPEND:
            devdrv_drv_warn("TS failed to suspend.\n");
            break;
        default:
            devdrv_drv_warn("invalid ts status status number:%u \n", status->ts_status);
            break;
    };

    devdrv_drv_warn(" \n\n\n");
}

STATIC void devdrv_dfx_check_heart_beat(struct devdrv_status_info *status)
{
    devdrv_drv_warn(" ******************* heart beat debug info ********************** \n");

    devdrv_drv_warn("ts_beat_en: %d.\n", status->ts_beat_en);
    devdrv_drv_warn("m3_beat_en: %d.\n", status->m3_beat_en);
    devdrv_drv_warn("ts_beat_count: %u.\n", status->ts_beat_count);
    devdrv_drv_warn("m3_beat_count: %u.\n", status->m3_beat_count);
    devdrv_drv_warn("ts_status: %u.\n", status->ts_status);

    devdrv_drv_warn(" \n\n\n");
}

STATIC void devdrv_dfx_check_sqcq_info(struct devdrv_status_info *status)
{
    int i = 0;
    devdrv_drv_warn(" ******************* heart beat debug info ********************** \n");
    devdrv_drv_warn(" user address start\n");

    for (i = 0; i < DEVDRV_MAX_STREAM_ID; i++) {
        devdrv_drv_warn("stream id: %d, sq: %d, cq: %d.\n", i, status->stream_sq[i], status->stream_cq[i]);
    }

    for (i = 0; i < DEVDRV_MAX_SQ_NUM; i++) {
        devdrv_drv_warn("sq id: %d, head: %d, tail: %d.\n", i, status->sq_head[i], status->sq_tail[i]);
    }

    for (i = 0; i < DEVDRV_MAX_CQ_NUM; i++) {
        devdrv_drv_warn("cq id: %d, head: %d, tail: %d, phase: %d.\n", i, status->cq_head[i], status->cq_tail[i],
                        status->cq_phase[i]);
    }

    for (i = 0; i < DEVDRV_MAX_FUNCTIONAL_SQ_NUM; i++) {
        devdrv_drv_warn("func sq id: %d, head: %d, tail: %d.\n", i, status->func_sq_head[i], status->func_sq_tail[i]);
    }

    for (i = 0; i < DEVDRV_MAX_FUNCTIONAL_CQ_NUM; i++) {
        devdrv_drv_warn("func cq id: %d, head: %d, tail: %d, phase: %d.\n", i, status->func_cq_head[i],
                        status->func_cq_tail[i], status->func_cq_phase[i]);
    }

    devdrv_drv_warn(" \n\n\n");
}

STATIC void devdrv_dfx_check_all_info(struct devdrv_status_info *status)
{
    devdrv_dfx_ts_status(status);
    devdrv_dfx_check_heart_beat(status);
    devdrv_dfx_check_sqcq_info(status);
}

STATIC void devdrv_get_calculate_cqsq_status(struct devdrv_info *dev_info, struct devdrv_status_info *status)
{
    struct devdrv_cce_ctrl *cce_ctrl = dev_info->cce_ctrl[0];
    struct devdrv_sq_sub_info *sq_sub_info = NULL;
    struct devdrv_cq_sub_info *cq_sub_info = NULL;
    struct devdrv_stream_info *stream = NULL;
    struct devdrv_ts_sq_info *sq_info = NULL;
    struct devdrv_ts_cq_info *cq_info = NULL;
    phys_addr_t mem_addr;
    u32 i;

    mem_addr = cce_ctrl->mem_info[DEVDRV_INFO_MEM].virt_addr;

    for (i = 0; i < DEVDRV_MAX_STREAM_ID; i++) {
        stream = devdrv_calc_stream_info(mem_addr, i);
        status->stream_sq[i] = (u16)stream->sq_index;
        status->stream_cq[i] = (u16)stream->cq_index;
    }

    for (i = 0; i < DEVDRV_MAX_SQ_NUM; i++) {
        sq_info = devdrv_calc_sq_info(mem_addr, i);
        status->sq_head[i] = (u16)sq_info->head;
        status->sq_tail[i] = (u16)sq_info->tail;

        sq_sub_info = (struct devdrv_sq_sub_info *)sq_info->sq_sub;
        if (dev_info->plat_type == DEVDRV_MANAGER_HOST_ENV)
            status->sq_addr[i] = sq_sub_info->bar_addr;
        else
            status->sq_addr[i] = sq_sub_info->phy_addr;
    }

    for (i = 0; i < DEVDRV_MAX_CQ_NUM; i++) {
        cq_info = devdrv_calc_cq_info(mem_addr, i);
        status->cq_head[i] = (u16)cq_info->head;
        status->cq_tail[i] = (u16)cq_info->tail;
        status->cq_phase[i] = (u8)cq_info->phase;

        cq_sub_info = (struct devdrv_cq_sub_info *)cq_info->cq_sub;

        status->cq_virt_addr[i] = cq_sub_info->virt_addr;
        status->cq_addr[i] = cq_sub_info->phy_addr;
    }
}

STATIC void devdrv_get_functional_cqsq_status(struct devdrv_info *dev_info, struct devdrv_status_info *status)
{
    struct devdrv_functional_sq_info *sq_info = NULL;
    struct devdrv_functional_cq_info *cq_info = NULL;
    int i;

    for (i = 0; i < DEVDRV_MAX_FUNCTIONAL_SQ_NUM; i++) {
        sq_info = &dev_info->cce_ctrl[0]->functional_cqsq.sq_info[i];
        status->func_sq_head[i] = (u16)sq_info->head;
        status->func_sq_tail[i] = (u16)sq_info->tail;

        if (dev_info->plat_type == DEVDRV_MANAGER_HOST_ENV)
            status->func_sq_addr[i] = virt_to_phys(sq_info->host_dma_buffer);
        else
            status->func_sq_addr[i] = sq_info->phy_addr;
    }

    for (i = 0; i < DEVDRV_MAX_FUNCTIONAL_CQ_NUM; i++) {
        cq_info = &dev_info->cce_ctrl[0]->functional_cqsq.cq_info[i];
        status->func_cq_head[i] = (u16)cq_info->head;
        status->func_cq_tail[i] = (u16)cq_info->tail;
        status->func_cq_addr[i] = virt_to_phys(cq_info->addr);
        status->func_cq_phase[i] = (u8)cq_info->phase;
    }
}

STATIC int devdrv_manager_dfx_query_status(struct devdrv_dfx_para *para)
{
    struct devdrv_status_info *status = NULL;
    struct devdrv_info *dev_info = NULL;
    int ret;

    if ((para == NULL) || (para->devid >= DEVDRV_MAX_DAVINCI_NUM)) {
        devdrv_drv_err("invalid devid: %u.\n", (para == NULL) ? 0 : (para->devid));
        return -EINVAL;
    }

    if (para->cmd != DEVDRV_DFX_QUERY_STATUS) {
        devdrv_drv_err("invalid cmd:(%u). devid(%u)\n", para->cmd, para->devid);
        return -EINVAL;
    }

    if (para->out == NULL) {
        devdrv_drv_err("para->out is NULL. devid(%u)\n", para->devid);
        return -EFAULT;
    }

    dev_info = devdrv_manager_get_devdrv_info(para->devid);
    if (dev_info == NULL) {
        devdrv_drv_err("can not find device(%u).\n", para->devid);
        return -EFAULT;
    }

    atomic_inc(&dev_info->occupy_ref);
    if (dev_info->status == 1) {
        atomic_dec(&dev_info->occupy_ref);
        devdrv_drv_warn("dev %d has been reset\n", dev_info->dev_id);
        return -EINVAL;
    }

    status = (struct devdrv_status_info *)kzalloc(sizeof(struct devdrv_status_info), GFP_KERNEL);
    if (status == NULL) {
        atomic_dec(&dev_info->occupy_ref);
        devdrv_drv_err("alloc memory for buffer failed. devid(%u)\n", para->devid);
        return -ENOMEM;
    }

    status->ts_beat_en = dev_info->config.ts_func.ts_heart_beat_en;
    status->m3_beat_en = dev_info->config.lpm3_func.lpm3_heart_beat_en;

    status->ts_beat_count = dev_info->heart_beat.cmd_inc_counter;
    status->m3_beat_count = dev_info->m3_heart_beat.inc_counter;

    status->ts_status = dev_info->ts_mng[0].ts_work_status_shadow;

    if (dev_info->cce_ctrl[0] != NULL) {
        devdrv_get_calculate_cqsq_status(dev_info, status);
        devdrv_get_functional_cqsq_status(dev_info, status);
    }

    atomic_dec(&dev_info->occupy_ref);

    ret = copy_to_user_safe(para->out, status, sizeof(struct devdrv_status_info));
    if (ret) {
        devdrv_drv_err("copy_to_user_safe failed(%d). devid(%u)\n", ret, para->devid);
        kfree(status);
        status = NULL;
        return ret;
    }

    kfree(status);
    status = NULL;
    return 0;
}

STATIC u64 devdrv_manager_dfx_get_cq_add_offset(u64 userspace_addr, int *cq_slot_index)
{
    *cq_slot_index = (int)((userspace_addr - DEVDRV_CQ_SLOT_START) / DEVDRV_VM_CQ_SLOT_SIZE);

    return (userspace_addr - (u64)DEVDRV_CQ_SLOT_START - (unsigned)(*cq_slot_index) * (u64)DEVDRV_VM_CQ_SLOT_SIZE);
}

STATIC int devdrv_manager_dfx_cq_slot_info(u64 userspace_addr, struct devdrv_status_info *status,
                                           struct devdrv_dfx_para *para)
{
    struct devdrv_cq_dfx_info cq_dfx_info = {0};
    int ret, cq_slot_index;
    u64 cq_addr_offset = 0;
    u64 cq_slot_kernel_virt_addr = 0;
    u64 cq_slot_phys_addr = 0;

    struct devdrv_report *tmp_report = NULL;
    struct devdrv_report *tmp_report1 = NULL;

    devdrv_drv_warn(" ******************* cq slot info ************************ \n");

    if ((userspace_addr >= DEVDRV_CQ_SLOT_START) && (userspace_addr < DEVDRV_CQ_SLOT_END)) {
        cq_addr_offset = devdrv_manager_dfx_get_cq_add_offset(userspace_addr, &cq_slot_index);
        if ((cq_addr_offset == (u64)-1) || (cq_slot_index >= DEVDRV_MAX_CQ_NUM) || (cq_slot_index < 0)) {
            devdrv_drv_warn("cq_slot_index = %d.\n", cq_slot_index);
            return -EINVAL;
        }

        if ((status->cq_virt_addr[cq_slot_index] == 0) || (status->cq_addr[cq_slot_index] == 0)) {
            devdrv_drv_warn("cq_slot_index = %d.\n", cq_slot_index);
            return -EINVAL;
        }

        cq_slot_kernel_virt_addr = status->cq_virt_addr[cq_slot_index] + cq_addr_offset;
        cq_slot_phys_addr = status->cq_addr[cq_slot_index] + cq_addr_offset;

        tmp_report = (struct devdrv_report *)(uintptr_t)cq_slot_kernel_virt_addr;

        cq_dfx_info.a = tmp_report->a;
        cq_dfx_info.b = tmp_report->b;
        cq_dfx_info.c = tmp_report->c;

        tmp_report1 = (struct devdrv_report *)ioremap(cq_slot_phys_addr, DEVDRV_CQ_SLOT_SIZE);
        if (tmp_report1 == NULL) {
            devdrv_drv_warn("cq_slot_phys_addr\n");
        } else {
            cq_dfx_info.a1 = tmp_report1->a;
            cq_dfx_info.b1 = tmp_report1->b;
            cq_dfx_info.c1 = tmp_report1->c;

            /*
             * check cq slot data
             */
            if ((cq_dfx_info.a != cq_dfx_info.a1) || (cq_dfx_info.b != cq_dfx_info.b1) ||
                (cq_dfx_info.c != cq_dfx_info.c1)) {
                cq_dfx_info.flag = 1;
            }

            iounmap((void *)tmp_report1);
            tmp_report1 = NULL;
        }
    } else {
        devdrv_drv_warn("error sec_cmd\n");
        return -EINVAL;
    }

    ret = copy_to_user_safe(para->out, &cq_dfx_info, sizeof(struct devdrv_cq_dfx_info));
    if (ret) {
        devdrv_drv_err("copy_to_user_safe failed(%d).\n", ret);
        return ret;
    }

    return 0;
}

STATIC int devdrv_manager_dfx_get_status(struct devdrv_dfx_para *para)
{
    struct devdrv_status_info *status = NULL;
    struct devdrv_info *dev_info = NULL;
    u64 sec_cmd;
    int ret = 0;

    if ((para == NULL) || (para->devid >= DEVDRV_MAX_DAVINCI_NUM)) {
        devdrv_drv_err("para(%pK) is NULL or para->devid is invalid.\n", para);
        return -EINVAL;
    }

    if (para->cmd != DEVDRV_DFX_GET_STATUS) {
        devdrv_drv_err("invalid cmd:(%u). devid(%u)\n", para->cmd, para->devid);
        return -EINVAL;
    }

    dev_info = devdrv_manager_get_devdrv_info(para->devid);
    if (dev_info == NULL) {
        devdrv_drv_err("can not find device(%u).\n", para->devid);
        return -EFAULT;
    }

    atomic_inc(&dev_info->occupy_ref);
    if (dev_info->status == 1) {
        atomic_dec(&dev_info->occupy_ref);
        devdrv_drv_warn("dev %d has been reset\n", dev_info->dev_id);
        return -EINVAL;
    }

    status = (struct devdrv_status_info *)kzalloc(sizeof(struct devdrv_status_info), GFP_KERNEL);
    if (status == NULL) {
        atomic_dec(&dev_info->occupy_ref);
        devdrv_drv_err("alloc memory for buffer failed. devid(%u)\n", para->devid);
        return -ENOMEM;
    }

    status->ts_beat_en = dev_info->config.ts_func.ts_heart_beat_en;
    status->m3_beat_en = dev_info->config.lpm3_func.lpm3_heart_beat_en;

    status->ts_beat_count = dev_info->heart_beat.cmd_inc_counter;
    status->m3_beat_count = dev_info->m3_heart_beat.inc_counter;

    status->ts_status = dev_info->ts_mng[0].ts_work_status_shadow;

    if (dev_info->cce_ctrl[0] != NULL) {
        devdrv_get_calculate_cqsq_status(dev_info, status);
        devdrv_get_functional_cqsq_status(dev_info, status);
    }

    sec_cmd = (u64)(uintptr_t)para->in;
    switch (sec_cmd) {
        case DEVMNG_DFX_TS_WORKING:
            devdrv_dfx_ts_status(status);
            break;
        case DEVMNG_DFX_HEART_BEAT:
            devdrv_dfx_check_heart_beat(status);
            break;
        case DEVMNG_DFX_CQSQ:
            devdrv_dfx_check_sqcq_info(status);
            break;
        case DEVMNG_DFX_ALL_STATUS:
            devdrv_dfx_check_all_info(status);
            break;
        default:
            devdrv_drv_warn("sec_cmd=0x%pK. devid(%u)\n", (void *)(uintptr_t)sec_cmd, para->devid);
            /*
             * perhaps need dump cq slot
             * sec_cmd must be address of userspace
             */
            ret = devdrv_manager_dfx_cq_slot_info(sec_cmd, status, para);
            break;
    };

    atomic_dec(&dev_info->occupy_ref);

    kfree(status);
    status = NULL;
    return ret;
}

STATIC int (*const devdrv_manager_dfx_process_cmd[DEVDRV_DFX_MAX_CMD])(struct devdrv_dfx_para *para) = {
    [DEVDRV_DFX_QUERY_STATUS] = devdrv_manager_dfx_query_status,
    [DEVDRV_DFX_GET_STATUS] = devdrv_manager_dfx_get_status,
};

int devdrv_manager_dfx_process(unsigned long arg)
{
    struct devdrv_manager_info *manager_info = NULL;
    struct devdrv_dfx_para para;
    int ret;
    u32 phys_devid = 0;

    ret = copy_from_user_safe(&para, (void *)(uintptr_t)arg, sizeof(struct devdrv_dfx_para));
    if (ret) {
        devdrv_drv_err("copy_from_user_safe failed(%d).\n", ret);
        return ret;
    }

    if ((para.cmd >= DEVDRV_DFX_MAX_CMD) || (para.devid >= DEVDRV_MAX_DAVINCI_NUM)) {
        devdrv_drv_err("invalid cmd(%u) or devid(%u) from user space.\n", para.cmd, para.devid);
        return -EINVAL;
    }

    manager_info = devdrv_get_manager_info();
    if (manager_info == NULL) {
        devdrv_drv_err("dev_manager_info is null. devid(%u)\n", para.devid);
        return -EFAULT;
    }

    if (manager_info->plat_info == DEVDRV_MANAGER_HOST_ENV) {
        if (devdrv_manager_container_logical_id_to_physical_id(para.devid, &phys_devid) != 0) {
            devdrv_drv_err("can't get phys device id. virt id is %u\n", para.devid);
            return -EINVAL;
        }

        para.devid = phys_devid;
    }

    ret = devdrv_manager_dfx_process_cmd[para.cmd](&para);
    if (ret) {
        devdrv_drv_err("process cmd failed, ret(%d). devid(%u)\n", ret, para.devid);
        return ret;
    }

    return 0;
}
