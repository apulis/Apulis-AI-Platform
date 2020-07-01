/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2012-2019. All rights reserved.
 * Description:
 * Author: huawei
 * Create: 2019-10-15
 */

#ifdef CONFIG_GENERIC_BUG
#undef CONFIG_GENERIC_BUG
#endif
#ifdef CONFIG_BUG
#undef CONFIG_BUG
#endif
#ifdef CONFIG_DEBUG_BUGVERBOSE
#undef CONFIG_DEBUG_BUGVERBOSE
#endif

#include <linux/irq.h>
#include <linux/list.h>
#include <linux/slab.h>
#include <linux/spinlock.h>
#include <linux/workqueue.h>

#include "devdrv_driver_pm.h"
#ifdef CFG_SOC_PLATFORM_CLOUD
#include "devdrv_parse_pdata.h"
#endif /* CFG_SOC_PLATFORM_CLOUD */

#define DEVDRV_FUNCTIONAL_HOST_SQ_FIRST_INDEX (DEVDRV_FUNCTIONAL_SQ_FIRST_INDEX - 16)
#define DEVDRV_FUNCTIONAL_HOST_CQ_FIRST_INDEX (DEVDRV_FUNCTIONAL_CQ_FIRST_INDEX - 16)

#ifdef  CFG_SOC_PLATFORM_MINIV2
#define DEVDRV_CQSQ_RESERVED_MEM_ADDR (0x2BD81000 + 0x200000) /* reserve mem: start addr + offset */
#define DEVDRV_CQSQ_RESERVED_MEM_END 0x31700000
#define DEVDRV_CQSQ_MAX_SIZE (128 * 1024)

STATIC int devdrv_get_cqsq_reserved_mem(u64 *rsv_addr, u32 size)
{
    static u64 malloc_addr = DEVDRV_CQSQ_RESERVED_MEM_ADDR;
    u64 rsv_malloc_addr;

    if (rsv_addr == NULL || size > DEVDRV_CQSQ_MAX_SIZE) {
        devdrv_drv_err("invalid input para.\n");
        return -EINVAL;
    }

    rsv_malloc_addr = malloc_addr + size;
    if (rsv_malloc_addr < DEVDRV_CQSQ_RESERVED_MEM_END) {
        *rsv_addr = malloc_addr;
        malloc_addr += size;
    } else {
        devdrv_drv_err("devdrv cqsq reserved memory have no space!\n");
        return -ENOMEM;
    }
    return 0;
}
#endif

STATIC void devdrv_flush_cache_check_null(const struct devdrv_info *dev_info, u64 base, u32 len)
{
    if (dev_info->drv_ops->flush_cache != NULL)
        dev_info->drv_ops->flush_cache(base, len);
}

STATIC void devdrv_functional_work(struct work_struct *work)
{
    struct devdrv_functional_cq_report *report = NULL;
    struct devdrv_functional_sq_info *sq_info = NULL;
    struct devdrv_functional_cq_info *cq_info = NULL;
    struct devdrv_functional_cqsq *cqsq = NULL;
    struct devdrv_info *dev_info = NULL;
    u32 sq_slot_index;
    u32 sq_index;
    u8 *sq_slot = NULL;
    u8 *cq_slot = NULL;
    u8 *cq_data = NULL;
    u32 len;

    cq_info = container_of(work, struct devdrv_functional_cq_info, work);
    cqsq = &cq_info->cce_ctrl->functional_cqsq;

    mutex_lock(&cq_info->lock);
    if (cq_info->addr == NULL) {
        mutex_unlock(&cq_info->lock);
        return;
    }
    cq_slot = cq_info->addr + (unsigned long)cq_info->tail * cq_info->slot_len;
    dev_info = cq_info->cce_ctrl->dev_info;
    len = cq_info->slot_len * DEVDRV_MAX_FUNCTIONAL_CQ_DEPTH;

    if (dev_info->drv_ops->flush_cache != NULL)
        dev_info->drv_ops->flush_cache((u64)(uintptr_t)cq_info->addr, len);

    while (cq_slot[0] == cq_info->phase) {
        if (cq_info->type == DEVDRV_FUNCTIONAL_DETAILED_CQ) {
            report = (struct devdrv_functional_cq_report *)cq_slot;

            sq_index = report->sq_index;
            if (sq_index >= DEVDRV_MAX_SQ_NUM) {
                mutex_unlock(&cq_info->lock);
                devdrv_drv_debug("sq_index:%u is invalid !\n", sq_index);
                return;
            }
            sq_info = &cqsq->sq_info[sq_index];

            sq_slot_index = (report->sq_head == 0) ? (DEVDRV_MAX_FUNCTIONAL_SQ_DEPTH - 1) : (report->sq_head - 1);

            sq_slot = sq_info->addr + (unsigned long)sq_slot_index * sq_info->slot_len;
            cq_data = cq_slot + DEVDRV_FUNCTIONAL_DETAILED_CQ_OFFSET;
            devdrv_drv_debug("call cq callback, cq id: %d.\n", cq_info->index);

            if (cq_info->callback != NULL)
                cq_info->callback(cq_data, sq_slot);
            sq_info->head = report->sq_head;
        } else {
            cq_data = cq_slot + DEVDRV_FUNCTIONAL_BRIEF_CQ_OFFSET;
            devdrv_drv_debug("call cq callback, cq id: %d.\n", cq_info->index);

            if (cq_info->callback != NULL)
                cq_info->callback(cq_data, NULL);
        }

        if (cq_info->tail >= DEVDRV_MAX_FUNCTIONAL_CQ_DEPTH - 1) {
            cq_info->phase = (cq_info->phase == DEVDRV_FUNCTIONAL_PHASE_ONE) ? DEVDRV_FUNCTIONAL_PHASE_ZERO
                                                                                : DEVDRV_FUNCTIONAL_PHASE_ONE;
            cq_info->tail = 0;
        } else {
            cq_info->tail++;
        }
        cq_slot = cq_info->addr + (unsigned long)cq_info->tail * cq_info->slot_len;
    }

    cq_info->head = cq_info->tail;
    *cq_info->doorbell = (u32)cq_info->tail;
    mutex_unlock(&cq_info->lock);
}

STATIC void devdrv_heart_beat_cq_callback(struct devdrv_functional_cq_info *cq_info)
{
    struct devdrv_functional_cq_report *report = NULL;
    struct devdrv_functional_sq_info *sq_info = NULL;
    struct devdrv_functional_cqsq *cqsq = NULL;
    u32 sq_slot_index;
    u32 sq_index;
    u8 *sq_slot = NULL;
    u8 *cq_slot = NULL;
    u8 *cq_data = NULL;

    cqsq = &cq_info->cce_ctrl->functional_cqsq;
    cq_slot = cq_info->addr + (unsigned long)cq_info->tail * cq_info->slot_len;

    while (cq_slot[0] == cq_info->phase) {
        report = (struct devdrv_functional_cq_report *)cq_slot;

        sq_index = report->sq_index;
        if (sq_index >= DEVDRV_MAX_FUNCTIONAL_SQ_NUM) {
            devdrv_drv_err("invalid index:%u.\n", sq_index);
            return;
        }
        sq_info = &cqsq->sq_info[sq_index];

        sq_slot_index = (report->sq_head == 0) ? (DEVDRV_MAX_FUNCTIONAL_SQ_DEPTH - 1) : (report->sq_head - 1);

        sq_slot = sq_info->addr + (unsigned long)sq_slot_index * sq_info->slot_len;

        cq_data = cq_slot + DEVDRV_FUNCTIONAL_DETAILED_CQ_OFFSET;

        devdrv_drv_debug("call cq callback, cq id: %d.\n", cq_info->index);

        if (cq_info->callback != NULL)
            cq_info->callback(cq_data, sq_slot);
        sq_info->head = report->sq_head;

        if (cq_info->tail >= DEVDRV_MAX_FUNCTIONAL_CQ_DEPTH - 1) {
            cq_info->phase = (cq_info->phase == DEVDRV_FUNCTIONAL_PHASE_ONE) ? DEVDRV_FUNCTIONAL_PHASE_ZERO
                                                                                : DEVDRV_FUNCTIONAL_PHASE_ONE;
            cq_info->tail = 0;
        } else {
            cq_info->tail++;
        }

        cq_slot = cq_info->addr + (unsigned long)cq_info->tail * cq_info->slot_len;
    }

    cq_info->head = cq_info->tail;
    *cq_info->doorbell = (u32)cq_info->tail;
}

STATIC void devdrv_cq_callback_handle(struct devdrv_functional_cq_info *cq_info)
{
    u8 *cq_data = NULL;
    u8 *cq_slot = NULL;

    cq_slot = cq_info->addr + (unsigned long)cq_info->tail * cq_info->slot_len;
    while (cq_slot[0] == cq_info->phase) {
        cq_data = cq_slot + DEVDRV_FUNCTIONAL_BRIEF_CQ_OFFSET;

        if (cq_info->callback != NULL)
            cq_info->callback(cq_data, NULL);

        if (cq_info->tail >= DEVDRV_MAX_FUNCTIONAL_CQ_DEPTH - 1) {
            cq_info->phase = (cq_info->phase == DEVDRV_FUNCTIONAL_PHASE_ONE) ? DEVDRV_FUNCTIONAL_PHASE_ZERO
                                                                                : DEVDRV_FUNCTIONAL_PHASE_ONE;
            cq_info->tail = 0;
        } else {
            cq_info->tail++;
        }

        cq_slot = cq_info->addr + (unsigned long)cq_info->tail * cq_info->slot_len;
    }
    cq_info->head = cq_info->tail;
    *cq_info->doorbell = (u32)cq_info->tail;
}

#define REPORT_HS_CQ devdrv_heart_beat_cq_callback
STATIC void devdrv_get_functional_cq_report(struct devdrv_functional_int_context *int_context,
                                            struct devdrv_functional_cq_info *cq_info, const u8 *addr)
{
    if (cq_info->function != DEVDRV_CQSQ_HEART_BEAT) {
        devdrv_drv_debug("receive irq find one report, "
        "cq id: %d, addr: %pK, cq_info->type=%u.\n", cq_info->index, (void *)addr, cq_info->type);
        if (cq_info->type == DEVDRV_FUNCTIONAL_CALLBACK_HS_CQ) {
            /* used for profiling log cq-sq channel callback */
            devdrv_cq_callback_handle(cq_info);
            return;
        } else if (cq_info->type == DEVDRV_FUNCTIONAL_REPORT_HS_CQ) {
            /* used for profiling log cq-sq channel report */
            REPORT_HS_CQ(cq_info);
            return;
        } else {
            queue_work(int_context->wq, &cq_info->work);
            return;
        }
    }

    devdrv_heart_beat_cq_callback(cq_info);
}

void devdrv_functional_cqsq_irq_handler(unsigned long data)
{
    struct devdrv_functional_int_context *int_context = NULL;
    struct devdrv_functional_cq_info *cq_info = NULL;
    struct devdrv_info *dev_info = NULL;
    struct list_head *pos = NULL, *n = NULL;
    unsigned long flags;
    u8 *addr = NULL;
    u32 get;
    u32 len;

    int_context = (struct devdrv_functional_int_context *)(uintptr_t)data;

    spin_lock_irqsave(&int_context->spinlock, flags);

    get = 0;
    list_for_each_safe(pos, n, &int_context->int_list_header)
    {
        cq_info = list_entry(pos, struct devdrv_functional_cq_info, int_list_node);

        addr = cq_info->addr + (unsigned long)cq_info->tail * cq_info->slot_len;

        len = cq_info->slot_len * DEVDRV_MAX_FUNCTIONAL_CQ_DEPTH;
        dev_info = cq_info->cce_ctrl->dev_info;
        if (dev_info->drv_ops->flush_cache != NULL)
            dev_info->drv_ops->flush_cache ((u64)(uintptr_t)cq_info->addr, len);

        if (addr[0] == cq_info->phase) {
            devdrv_drv_debug("cq_info->index = %d.\n", cq_info->index);
            devdrv_get_functional_cq_report(int_context, cq_info, addr);
            get++;
        }
    }
    spin_unlock_irqrestore(&int_context->spinlock, flags);

    if (get == 0) {
        devdrv_drv_debug("receive irq but no report found.\n");
    }
    return;
}

irqreturn_t devdrv_function_cqsq_hwirq_proc(int irq, void *data)
{
    struct devdrv_functional_int_context *int_context = NULL;
    unsigned long flags;

    if (data == NULL) {
        devdrv_drv_warn("param is null .irq = %d.\n", irq);
        return IRQ_NONE;
    }

    local_irq_save(flags);
    int_context = (struct devdrv_functional_int_context *)data;
    tasklet_schedule(&int_context->cqsq_tasklet);
    local_irq_restore(flags);
    return IRQ_HANDLED;
}

int devdrv_init_functional_cqsq(struct devdrv_info *info, u32 tsid)
{
    struct devdrv_functional_sq_info *sq_info = NULL;
    struct devdrv_functional_cq_info *cq_info = NULL;
    struct devdrv_functional_cqsq *cqsq = NULL;
#ifdef CFG_SOC_PLATFORM_CLOUD
    int ccpu_id;
#endif /* CFG_SOC_PLATFORM_CLOUD */
    u32 index;
    int ret;
    int i;

    DRV_CHECK_PTR(info, return -ENOMEM, "dev_info is NULL\n");

    cqsq = &info->cce_ctrl[tsid]->functional_cqsq;
    INIT_LIST_HEAD(&cqsq->int_context.int_list_header);
    spin_lock_init(&cqsq->int_context.spinlock);
    cqsq->int_context.wq = create_workqueue("devdrv-cqsq-work");
    if (cqsq->int_context.wq == NULL) {
        devdrv_drv_err("[dev_id = %u]:create_workqueue error.\n", info->dev_id);
        return -ENOMEM;
    }

    sq_info = kzalloc(sizeof(struct devdrv_functional_sq_info) * DEVDRV_MAX_FUNCTIONAL_SQ_NUM, GFP_KERNEL);
    if (sq_info == NULL) {
        destroy_workqueue(cqsq->int_context.wq);
        devdrv_drv_err("[dev_id = %u]:kmalloc failed.\n", info->dev_id);
        return -ENOMEM;
    }
    cq_info = kzalloc(sizeof(struct devdrv_functional_cq_info) * DEVDRV_MAX_FUNCTIONAL_CQ_NUM, GFP_KERNEL);
    if (cq_info == NULL) {
        destroy_workqueue(cqsq->int_context.wq);
        kfree(sq_info);
        sq_info = NULL;
        devdrv_drv_err("[dev_id = %u]:kmalloc failed.\n", info->dev_id);
        return -ENOMEM;
    }

    for (i = 0; i < DEVDRV_MAX_FUNCTIONAL_SQ_NUM; i++) {
        sq_info[i].devid = cqsq->int_context.cce_ctrl->devid;
        sq_info[i].index = i;
        sq_info[i].depth = DEVDRV_MAX_FUNCTIONAL_SQ_DEPTH;
        sq_info[i].slot_len = 0;
        sq_info[i].addr = NULL;
        sq_info[i].head = 0;
        sq_info[i].tail = 0;
        sq_info[i].credit = DEVDRV_MAX_FUNCTIONAL_SQ_DEPTH;
        sq_info[i].host_dma_addr = 0;
        sq_info[i].phy_addr = 0;
        sq_info[i].host_dma_buffer = NULL;

        if (info->plat_type == DEVDRV_MANAGER_DEVICE_ENV)
            index = DEVDRV_FUNCTIONAL_SQ_FIRST_INDEX + i;
        else
            index = DEVDRV_FUNCTIONAL_HOST_SQ_FIRST_INDEX + i;

        /* use cq head's doorbell for functional sq */
        sq_info[i].doorbell = devdrv_doorbell_get_cq_head(info, tsid, index);

        sq_info[i].function = DEVDRV_MAX_CQSQ_FUNC;
    }

    for (i = 0; i < DEVDRV_MAX_FUNCTIONAL_CQ_NUM; i++) {
        cq_info[i].devid = cqsq->int_context.cce_ctrl->devid;
        cq_info[i].index = i;
        cq_info[i].depth = DEVDRV_MAX_FUNCTIONAL_CQ_DEPTH;
        cq_info[i].slot_len = 0;
        cq_info[i].type = 0;
        cq_info[i].cce_ctrl = cqsq->int_context.cce_ctrl;

        INIT_WORK(&cq_info[i].work, devdrv_functional_work);
        mutex_init(&cq_info[i].lock);

        cq_info[i].addr = NULL;
        cq_info[i].head = 0;
        cq_info[i].tail = 0;
        cq_info[i].phase = DEVDRV_FUNCTIONAL_PHASE_ONE;

        if (info->plat_type == DEVDRV_MANAGER_DEVICE_ENV)
            index = DEVDRV_FUNCTIONAL_CQ_FIRST_INDEX + i;
        else
            index = DEVDRV_FUNCTIONAL_HOST_CQ_FIRST_INDEX + i;
        cq_info[i].doorbell = devdrv_doorbell_get_cq_head(info, tsid, index);
        cq_info[i].callback = NULL;

        cq_info[i].function = DEVDRV_MAX_CQSQ_FUNC;
    }

    cqsq->sq_info = sq_info;
    cqsq->sq_num = DEVDRV_MAX_FUNCTIONAL_SQ_NUM;
    cqsq->cq_info = cq_info;
    cqsq->cq_num = DEVDRV_MAX_FUNCTIONAL_CQ_NUM;

    tasklet_init(&cqsq->int_context.cqsq_tasklet, devdrv_functional_cqsq_irq_handler,
                 (unsigned long)(uintptr_t)&cqsq->int_context);
    ret = request_irq(cqsq->int_context.irq_num, devdrv_function_cqsq_hwirq_proc, IRQF_TRIGGER_NONE,
                      "devdrv-functional_cq", &cqsq->int_context);
    if (ret) {
        cqsq->sq_info = NULL;
        cqsq->sq_num = 0;
        cqsq->cq_info = NULL;
        cqsq->cq_num = 0;
        tasklet_kill(&cqsq->int_context.cqsq_tasklet);
        destroy_workqueue(cqsq->int_context.wq);
        kfree(sq_info);
        sq_info = NULL;
        kfree(cq_info);
        cq_info = NULL;
        devdrv_drv_err("[dev_id = %u]:request_irq failed,ret:%d\n", info->dev_id, ret);
        return -EFAULT;
    }
    spin_lock_init(&cqsq->spinlock);

#ifdef CFG_SOC_PLATFORM_CLOUD
    ccpu_id = get_devdrv_affinity_cpuid(info->dev_id);
    devdrv_drv_info("******* request_irq %d\n", ccpu_id);
    cpumask_set_cpu(ccpu_id, &(info->ccpumask));
    irq_set_affinity_hint(cqsq->int_context.irq_num, &(info->ccpumask));
#endif /* CFG_SOC_PLATFORM_CLOUD */

    return 0;
}

void devdrv_destroy_functional_cqsq(struct devdrv_info *info, u32 tsid)
{
    struct devdrv_functional_cqsq *cqsq = NULL;
    int i;

    if (info == NULL) {
        devdrv_drv_err("device not exist");
        return;
    }

    cqsq = &info->cce_ctrl[tsid]->functional_cqsq;

    if ((cqsq->sq_num < DEVDRV_MAX_FUNCTIONAL_SQ_NUM) || (cqsq->cq_num < DEVDRV_MAX_FUNCTIONAL_CQ_NUM)) {
        devdrv_drv_err("[dev_id = %u]:some cq or sq still working, free first.\n", info->dev_id);
        return;
    }
    for (i = 0; i < DEVDRV_MAX_FUNCTIONAL_CQ_NUM; i++) {
        mutex_destroy(&cqsq->cq_info[i].lock);
    }

    free_irq(cqsq->int_context.irq_num, &cqsq->int_context);
    tasklet_kill(&cqsq->int_context.cqsq_tasklet);

    destroy_workqueue(cqsq->int_context.wq);

    kfree(cqsq->sq_info);
    kfree(cqsq->cq_info);

    cqsq->sq_info = NULL;
    cqsq->sq_num = 0;
    cqsq->cq_info = NULL;
    cqsq->cq_num = 0;
}

STATIC int devdrv_get_functional_sq_memory(struct devdrv_info *dev_info, u32 tsid,
                                           struct devdrv_functional_sq_info *sq_info, u32 size)
{
#ifdef  CFG_SOC_PLATFORM_MINIV2
    u64 rsv_sq_addr;
    int ret;
#endif

    if (dev_info->plat_type == DEVDRV_MANAGER_DEVICE_ENV) {
#ifndef  CFG_SOC_PLATFORM_MINIV2
        sq_info->addr = kzalloc(size, GFP_KERNEL);

        if (sq_info->addr == NULL) {
            devdrv_drv_err("kzalloc failed.\n");
            return -ENOMEM;
        }
        sq_info->phy_addr = (phys_addr_t)virt_to_phys((void *)sq_info->addr);
#else
        ret = devdrv_get_cqsq_reserved_mem(&rsv_sq_addr, size);
        if (ret) {
            devdrv_drv_err("devdrv get sq reserved memory error!\n");
            return -ENOMEM;
        }

        sq_info->phy_addr = rsv_sq_addr;

        sq_info->addr = (void *)ioremap_wc(sq_info->phy_addr, size);
        if ((void *)sq_info->addr == NULL) {
            devdrv_drv_err("ioremap sq addr failed\n");
            return -ENOMEM;
        }

        ret = memset_s(sq_info->addr, size, 0, size);
        if (ret) {
            devdrv_drv_err("memset sq memory failed.\n");
            return -ENOMEM;
        }

#endif
    } else {
        devdrv_drv_err("devid(%u):invalid plat type(%u)\n", dev_info->dev_id, dev_info->plat_type);
        return -ENOMEM;
    }

    return 0;
}

int devdrv_create_functional_sq(u32 devid, u32 tsid, u32 slot_len, u32 *sq_index, u64 *addr)
{
    struct devdrv_functional_sq_info *sq_info = NULL;
    struct devdrv_info *dev_info = NULL;
    u32 len;
    int ret;
    int i;

    if ((devid >= DEVDRV_MAX_DAVINCI_NUM) || (sq_index == NULL) || (addr == NULL) || (tsid >= DEVDRV_MAX_TS_NUM)) {
        devdrv_drv_err("[dev_id=%u]:invalid input argument,tsid :%u.\n", devid, tsid);
        return -EINVAL;
    }
    if ((slot_len <= 0) || (slot_len > DEVDRV_FUNCTIONAl_MAX_SQ_SLOT_LEN)) {
        devdrv_drv_err("[dev_id=%u]:invalid input argument.\n", devid);
        return -EINVAL;
    }

    dev_info = devdrv_get_devdrv_info(devid);
    if (dev_info == NULL) {
        devdrv_drv_err("[dev_id=%u]:device not exist.\n", devid);
        return -ENODEV;
    }

    if (dev_info->cce_ctrl[tsid]->functional_cqsq.sq_num <= 0) {
        devdrv_drv_err("[dev_id=%u]:no available sq num=%d.\n", devid,
                       dev_info->cce_ctrl[tsid]->functional_cqsq.sq_num);
        return -ENOMEM;
    }

    sq_info = dev_info->cce_ctrl[tsid]->functional_cqsq.sq_info;

    spin_lock(&dev_info->cce_ctrl[tsid]->functional_cqsq.spinlock);
    for (i = 0; i < DEVDRV_MAX_FUNCTIONAL_SQ_NUM; i++) {
        if (sq_info[i].addr)
            continue;

        sq_info[i].addr = (void *)1;
        spin_unlock(&dev_info->cce_ctrl[tsid]->functional_cqsq.spinlock);

        len = DEVDRV_MAX_FUNCTIONAL_SQ_DEPTH * slot_len;
        ret = devdrv_get_functional_sq_memory(dev_info, tsid, &sq_info[i], len);
        if (ret) {
            sq_info[i].addr = NULL;
            return -ENOMEM;
        }

        spin_lock(&dev_info->cce_ctrl[tsid]->functional_cqsq.spinlock);
        sq_info[i].depth = DEVDRV_MAX_FUNCTIONAL_SQ_DEPTH;
        sq_info[i].slot_len = slot_len;
        // set sq doorbell zero ? *sq_info[i].doorbell
        dev_info->cce_ctrl[tsid]->functional_cqsq.sq_num--;

        *sq_index = sq_info[i].index;
        *addr = (unsigned long)sq_info[i].phy_addr;

        spin_unlock(&dev_info->cce_ctrl[tsid]->functional_cqsq.spinlock);

        devdrv_drv_debug("dev[%d] functional sq is created, sq id: %d.\n", devid, *sq_index);
        return 0;
    }
    spin_unlock(&dev_info->cce_ctrl[tsid]->functional_cqsq.spinlock);
    return -ENOMEM;
}
EXPORT_SYMBOL(devdrv_create_functional_sq);

void devdrv_destroy_functional_sq(u32 devid, u32 tsid, u32 sq_index)
{
    struct devdrv_functional_sq_info *sq_info = NULL;
    struct devdrv_info *dev_info = NULL;

    if ((devid >= DEVDRV_MAX_DAVINCI_NUM) || (sq_index >= DEVDRV_MAX_FUNCTIONAL_SQ_NUM) ||
        (tsid >= DEVDRV_MAX_TS_NUM)) {
        devdrv_drv_err("[dev_id=%u]:invalid argument sq_index:%u tsid:%u.\n", devid, sq_index,
            tsid);
        return;
    }

    dev_info = devdrv_get_devdrv_info(devid);
    if (dev_info == NULL) {
        devdrv_drv_err("[dev_id=%u]:device not exist.\n", devid);
        return;
    }

    sq_info = dev_info->cce_ctrl[tsid]->functional_cqsq.sq_info;
    spin_lock(&dev_info->cce_ctrl[tsid]->functional_cqsq.spinlock);
    if (sq_info[sq_index].addr) {
        if (dev_info->plat_type == DEVDRV_MANAGER_DEVICE_ENV) {
#ifndef  CFG_SOC_PLATFORM_MINIV2
            kfree(sq_info[sq_index].addr);
            sq_info[sq_index].addr = NULL;
#else
            iounmap(sq_info[sq_index].addr);
            sq_info[sq_index].addr = NULL;
#endif
        } else {
            kfree(sq_info[sq_index].addr);
            sq_info[sq_index].addr = NULL;
            spin_unlock(&dev_info->cce_ctrl[tsid]->functional_cqsq.spinlock);
            devdrv_drv_err("devid(%u):invalid device type(%u).\n", devid, dev_info->plat_type);
            return;
        }
        sq_info[sq_index].slot_len = 0;
        sq_info[sq_index].head = 0;
        sq_info[sq_index].tail = 0;
        sq_info[sq_index].credit = DEVDRV_MAX_FUNCTIONAL_SQ_DEPTH;
        sq_info[sq_index].function = DEVDRV_MAX_CQSQ_FUNC;
        sq_info[sq_index].phy_addr = 0;
        dev_info->cce_ctrl[tsid]->functional_cqsq.sq_num++;
    }
    spin_unlock(&dev_info->cce_ctrl[tsid]->functional_cqsq.spinlock);
}
EXPORT_SYMBOL(devdrv_destroy_functional_sq);

int devdrv_functional_set_sq_func(u32 devid, u32 tsid, u32 sq_index, enum devdrv_cqsq_func function)
{
    struct devdrv_functional_sq_info *sq_info = NULL;
    struct devdrv_info *dev_info = NULL;
    if ((devid >= DEVDRV_MAX_DAVINCI_NUM) || (sq_index >= DEVDRV_MAX_FUNCTIONAL_SQ_NUM) ||
        (function >= DEVDRV_MAX_CQSQ_FUNC) || (tsid >= DEVDRV_MAX_TS_NUM)) {
        devdrv_drv_err("[dev_id=%u]:invalid input argument.sq_index:%u, function:%u, tsid:%u\n",
            devid, sq_index, function, tsid);
        return -EINVAL;
    }
    dev_info = devdrv_get_devdrv_info(devid);
    if (dev_info == NULL) {
        devdrv_drv_err("[dev_id=%u]:device not exist.\n", devid);
        return -ENODEV;
    }
    sq_info = &dev_info->cce_ctrl[tsid]->functional_cqsq.sq_info[sq_index];
    if (sq_info->addr == NULL) {
        devdrv_drv_err("[dev_id=%u]:sq is in the source pool, not use.\n", devid);
        return -EINVAL;
    }
    sq_info->function = function;
    return 0;
}
EXPORT_SYMBOL(devdrv_functional_set_sq_func);

int devdrv_functional_set_cq_func(u32 devid, u32 tsid, u32 cq_index, enum devdrv_cqsq_func function)
{
    struct devdrv_functional_cq_info *cq_info = NULL;
    struct devdrv_info *dev_info = NULL;
    if ((devid >= DEVDRV_MAX_DAVINCI_NUM) || (cq_index >= DEVDRV_MAX_FUNCTIONAL_CQ_NUM) ||
        (function >= DEVDRV_MAX_CQSQ_FUNC) || (tsid >= DEVDRV_MAX_TS_NUM)) {
        devdrv_drv_err("[dev_id=%u]:invalid input argument. cq_index:%u, funcion:%u, tsid:%u\n", devid,
            cq_index, function, tsid);
        return -EINVAL;
    }
    dev_info = devdrv_get_devdrv_info(devid);
    if (dev_info == NULL) {
        devdrv_drv_err("[dev_id=%u]:device not exist.\n", devid);
        return -ENODEV;
    }
    cq_info = &dev_info->cce_ctrl[tsid]->functional_cqsq.cq_info[cq_index];
    if (cq_info->addr == NULL) {
        devdrv_drv_err("[dev_id=%u]:cq is in the source pool, not use.\n", devid);
        return -EINVAL;
    }
    cq_info->function = function;
    return 0;
}
EXPORT_SYMBOL(devdrv_functional_set_cq_func);

STATIC int devdrv_get_functional_cq_memory(struct devdrv_info *dev_info, u32 tsid,
                                           struct devdrv_functional_cq_info *cq_info, u32 size)
{
#ifdef  CFG_SOC_PLATFORM_MINIV2
    u64 rsv_cq_addr;
    int ret;
#endif

    if (dev_info->plat_type == DEVDRV_MANAGER_DEVICE_ENV) {
#ifndef  CFG_SOC_PLATFORM_MINIV2
        cq_info->addr = kzalloc(size, GFP_KERNEL);
        if (cq_info->addr == NULL) {
            devdrv_drv_err("[dev_id = %u]:kzalloc failed.\n", dev_info->dev_id);
            return -ENOMEM;
        }
        cq_info->phy_addr = (u64)virt_to_phys((void *)cq_info->addr);
#else
        ret = devdrv_get_cqsq_reserved_mem(&rsv_cq_addr, size);
        if (ret) {
            devdrv_drv_err("devdrv get cqsq reserved memory error!\n");
            return -ENOMEM;
        }

        cq_info->phy_addr = rsv_cq_addr;
        cq_info->addr = (void *)ioremap_wc(cq_info->phy_addr, size);
        if ((void *)cq_info->addr == NULL) {
            devdrv_drv_err("ioremap cq addr failed\n");
            return -ENOMEM;
        }

        ret = memset_s(cq_info->addr, size, 0, size);
        if (ret) {
            devdrv_drv_err("memset sq memory failed.\n");
            return -ENOMEM;
        }

#endif
    } else if (dev_info->plat_type == DEVDRV_MANAGER_HOST_ENV) {
        cq_info->host_dma_buffer = dma_alloc_coherent(dev_info->dev, DEVDRV_FUNCTIONAL_MAX_CQ_SLOT_LEN,
                                                      &cq_info->host_dma_addr, GFP_KERNEL);
        if (cq_info->host_dma_buffer == NULL) {
            devdrv_drv_err("[dev_id = %u]:dma_alloc_coherent failed!\n", dev_info->dev_id);
            cq_info->addr = NULL;
            cq_info->host_dma_addr = 0;
            cq_info->host_dma_buffer = NULL;
            return -ENOMEM;
        }
        cq_info->phy_addr = cq_info->host_dma_addr;
        cq_info->addr = (u8 *)(uintptr_t)cq_info->host_dma_buffer;
        devdrv_drv_debug("cq_info->phy_addr:%pK,cq_info->addr:%pK\n", (void *)cq_info->phy_addr, (void *)cq_info->addr);
    } else {
        devdrv_drv_err("[dev_id = %u]:invalid plat type\n", dev_info->dev_id);
        return -ENOMEM;
    }
    devdrv_flush_cache_check_null(dev_info, (u64)((uintptr_t)cq_info->addr), size);

    return 0;
}

int devdrv_create_functional_cq(u32 devid, u32 tsid, u32 slot_len, u32 cq_type,
                                void (*callback)(const u8 *cq_slot, u8 *sq_slot), u32 *cq_index, u64 *addr)
{
    struct devdrv_functional_int_context *int_context = NULL;
    struct devdrv_functional_cq_info *cq_info = NULL;
    struct devdrv_info *dev_info = NULL;
    unsigned long flags;
    int ret;
    int len;
    int i;

    if ((devid >= DEVDRV_MAX_DAVINCI_NUM) || (callback == NULL) || (cq_type > DEVDRV_FUNCTIONAL_REPORT_HS_CQ) ||
        (cq_index == NULL) || (addr == NULL) || (tsid >= DEVDRV_MAX_TS_NUM)) {
        devdrv_drv_err("[dev_id=%u]:invalid argument tsid:%u cq_type:%u\n", devid, tsid, cq_type);
        return -EINVAL;
    }
    if ((slot_len <= 0) || (slot_len > DEVDRV_FUNCTIONAL_MAX_CQ_SLOT_LEN)) {
        devdrv_drv_err("[dev_id=%u]:invalid input argument.\n", devid);
        return -EINVAL;
    }

    dev_info = devdrv_get_devdrv_info(devid);
    if (dev_info == NULL) {
        devdrv_drv_err("[dev_id=%u]:device not exist.\n", devid);
        return -ENODEV;
    }

    if (dev_info->cce_ctrl[tsid]->functional_cqsq.cq_num <= 0) {
        devdrv_drv_err("[dev_id=%u]:no available cq num=%d.\n", devid,
                       dev_info->cce_ctrl[tsid]->functional_cqsq.cq_num);
        return -ENOMEM;
    }

    cq_info = dev_info->cce_ctrl[tsid]->functional_cqsq.cq_info;

    spin_lock(&dev_info->cce_ctrl[tsid]->functional_cqsq.spinlock);
    for (i = 0; i < DEVDRV_MAX_FUNCTIONAL_CQ_NUM; i++) {
        if (cq_info[i].addr)
            continue;
        cq_info[i].addr = (void *)1;
        spin_unlock(&dev_info->cce_ctrl[tsid]->functional_cqsq.spinlock);
        len = DEVDRV_MAX_FUNCTIONAL_CQ_DEPTH * slot_len;

        ret = devdrv_get_functional_cq_memory(dev_info, tsid, &cq_info[i], len);
        if (ret)
            return -ENOMEM;

        spin_lock(&dev_info->cce_ctrl[tsid]->functional_cqsq.spinlock);
        cq_info[i].depth = DEVDRV_MAX_FUNCTIONAL_CQ_DEPTH;
        cq_info[i].slot_len = slot_len;
        cq_info[i].type = cq_type;
        cq_info[i].phase = DEVDRV_FUNCTIONAL_PHASE_ONE;
        cq_info[i].callback = callback;
        *cq_info[i].doorbell = 0;

        dev_info->cce_ctrl[tsid]->functional_cqsq.cq_num--;
        *cq_index = cq_info[i].index;

        *addr = (unsigned long)cq_info[i].phy_addr;

        spin_unlock(&dev_info->cce_ctrl[tsid]->functional_cqsq.spinlock);
        int_context = &dev_info->cce_ctrl[tsid]->functional_cqsq.int_context;
        spin_lock_irqsave(&int_context->spinlock, flags);
        list_add(&cq_info[i].int_list_node, &int_context->int_list_header);
        spin_unlock_irqrestore(&int_context->spinlock, flags);

        devdrv_drv_debug("dev[%d] functional cq is created, cq id: %d.\n", devid, *cq_index);
        return 0;
    }
    spin_unlock(&dev_info->cce_ctrl[tsid]->functional_cqsq.spinlock);
    return -ENOMEM;
}
EXPORT_SYMBOL(devdrv_create_functional_cq);

void devdrv_destroy_functional_cq(u32 devid, u32 tsid, u32 cq_index)
{
    struct devdrv_functional_int_context *int_context = NULL;
    struct devdrv_functional_cq_info *cq_info = NULL;
    struct devdrv_info *dev_info = NULL;
    unsigned long flags;

    if ((devid >= DEVDRV_MAX_DAVINCI_NUM) || (cq_index >= DEVDRV_MAX_FUNCTIONAL_CQ_NUM) ||
        (tsid >= DEVDRV_MAX_TS_NUM)) {
        devdrv_drv_err("[dev_id=%u]:invalid input argument tsid:%u.cq_index:%u\n", devid, tsid, cq_index);
        return;
    }

    dev_info = devdrv_get_devdrv_info(devid);
    if (dev_info == NULL) {
        devdrv_drv_err("[dev_id=%u]:device not exist.\n", devid);
        return;
    }

    cq_info = dev_info->cce_ctrl[tsid]->functional_cqsq.cq_info;
    mutex_lock(&cq_info[cq_index].lock);
    if (cq_info[cq_index].addr) {
        int_context = &dev_info->cce_ctrl[tsid]->functional_cqsq.int_context;

        spin_lock_irqsave(&int_context->spinlock, flags);
        list_del(&cq_info[cq_index].int_list_node);
        spin_unlock_irqrestore(&int_context->spinlock, flags);
        spin_lock(&dev_info->cce_ctrl[tsid]->functional_cqsq.spinlock);

        if (dev_info->plat_type == DEVDRV_MANAGER_DEVICE_ENV) {
#ifndef  CFG_SOC_PLATFORM_MINIV2
            kfree(cq_info[cq_index].addr);
            cq_info[cq_index].addr = NULL;
#else
            iounmap(cq_info[cq_index].addr);
            cq_info[cq_index].addr = NULL;
#endif
        } else if (dev_info->plat_type == DEVDRV_MANAGER_HOST_ENV) {
            dma_free_coherent(dev_info->dev, DEVDRV_FUNCTIONAL_MAX_CQ_SLOT_LEN, cq_info->host_dma_buffer,
                              cq_info->host_dma_addr);
            cq_info[cq_index].addr = NULL;
            cq_info[cq_index].host_dma_buffer = NULL;
            cq_info[cq_index].host_dma_addr = 0;
        } else {
            spin_unlock(&dev_info->cce_ctrl[tsid]->functional_cqsq.spinlock);
            mutex_unlock(&cq_info[cq_index].lock);
            devdrv_drv_err("[dev_id=%u]:invalid device type.\n", devid);
            return;
        }

        cq_info[cq_index].head = 0;
        cq_info[cq_index].tail = 0;
        cq_info[cq_index].depth = DEVDRV_MAX_FUNCTIONAL_CQ_DEPTH;
        cq_info[cq_index].slot_len = 0;
        cq_info[cq_index].type = 0;
        cq_info[cq_index].phase = DEVDRV_FUNCTIONAL_PHASE_ONE;
        cq_info[cq_index].callback = NULL;
        cq_info[cq_index].function = DEVDRV_MAX_CQSQ_FUNC;
        dev_info->cce_ctrl[tsid]->functional_cqsq.cq_num++;
        spin_unlock(&dev_info->cce_ctrl[tsid]->functional_cqsq.spinlock);
    }
    mutex_unlock(&cq_info[cq_index].lock);
}
EXPORT_SYMBOL(devdrv_destroy_functional_cq);

STATIC int devdrv_functional_send_sq_check(u32 devid, u32 tsid, u32 sq_index, const u8 *buffer, u32 buf_len)
{
    if ((devid >= DEVDRV_MAX_DAVINCI_NUM) || (sq_index >= DEVDRV_MAX_FUNCTIONAL_SQ_NUM) || (buffer == NULL) ||
        (buf_len <= 0) || (buf_len > DEVDRV_FUNCTIONAl_MAX_SQ_SLOT_LEN) || (tsid >= DEVDRV_MAX_TS_NUM)) {
        devdrv_drv_err("[dev_id=%u]:invalid argument.sq_index:%u, (buffer == NULL):%d, buf_len:%u, tsid:%u\n",
                       devid, sq_index, (buffer == NULL), buf_len, tsid);
        return -EINVAL;
    }

    return 0;
}

STATIC int devdrv_functional_judge_ts_status(struct devdrv_info *dev_info, u32 tsid)
{
    if (devdrv_is_low_power(dev_info->cce_ctrl[tsid])) {
        devdrv_drv_err("device is at low power.\n");
        return DEVDRV_TS_SLEEP;
    }
    if (devdrv_is_ts_work(&dev_info->ts_mng[tsid]) == 0) {
        devdrv_drv_err("[dev_id = %u]:device is not working.\n", dev_info->dev_id);
        return DEVDRV_TS_DOWN;
    }
    return DEVDRV_TS_WORK;
}

int devdrv_functional_send_sq(u32 devid, u32 tsid, u32 sq_index, const u8 *buffer, u32 buf_len)
{
    struct devdrv_functional_sq_info *sq_info = NULL;
    struct devdrv_info *dev_info = NULL;
    int credit;
    u8 *addr = NULL;
    u32 tail;
    u32 len;
    int ret;

    ret = devdrv_functional_send_sq_check(devid, tsid, sq_index, buffer, buf_len);
    if (ret) {
        devdrv_drv_err("[dev_id=%u]:invalid input argument.\n", devid);
        return -EINVAL;
    }

    dev_info = devdrv_get_devdrv_info(devid);
    if (dev_info == NULL) {
        devdrv_drv_err("[dev_id=%u]:device not exist.\n", devid);
        return -ENODEV;
    }

    ret = devdrv_functional_judge_ts_status(dev_info, tsid);
    if (ret != DEVDRV_TS_WORK) {
        return ret;
    }

    sq_info = dev_info->cce_ctrl[tsid]->functional_cqsq.sq_info;
    if (sq_info[sq_index].addr == NULL) {
        devdrv_drv_err("[dev_id=%u]:invalid sq.\n", devid);
        return -ENOMEM;
    }

    tail = sq_info[sq_index].tail;
    credit = (sq_info->tail >= sq_info->head) ? (DEVDRV_MAX_FUNCTIONAL_SQ_DEPTH - (sq_info->tail - sq_info->head + 1))
                                                : (sq_info->head - sq_info->tail - 1);
    if (credit <= 0) {
        devdrv_drv_err("[dev_id=%u]:no available sq slot.\n", devid);
        return -ENOMEM;
    }

    addr = sq_info[sq_index].addr + (unsigned long)tail * sq_info[sq_index].slot_len;

    if (dev_info->plat_type == DEVDRV_MANAGER_DEVICE_ENV) {
        ret = memcpy_s(addr, sq_info[sq_index].slot_len, buffer, buf_len);
        if (ret != 0) {
            devdrv_drv_err("[dev_id=%u]:DEVICE: copy to addr failed, ret = %d.\n", devid, ret);
            return -EINVAL;
        }
    } else if (dev_info->plat_type == DEVDRV_MANAGER_HOST_ENV) {
        ret = memcpy_s(sq_info[sq_index].host_dma_buffer, DEVDRV_FUNCTIONAl_MAX_SQ_SLOT_LEN, buffer, buf_len);
        if (ret != 0) {
            devdrv_drv_err("[dev_id=%u]:HOST: copy to host_dma_buffer failed, "
                           "ret = %d.\n",
                           devid, ret);
            return -EINVAL;
        }
        ret = -EINVAL;
        if (dev_info->drv_ops->memcpy_to_device_sq != NULL) {
            /* may sleep, be careful!!! */
            ret = dev_info->drv_ops->memcpy_to_device_sq(dev_info, (u64)((uintptr_t)addr),
                                                         (u64)sq_info[sq_index].host_dma_addr, buf_len);
            if (ret) {
                devdrv_drv_err("[dev_id=%u]:devdrv_memcpy_to_device_sq failed.\n", devid);
                return -ENOMEM;
            }
        }
    } else {
        devdrv_drv_err("[dev_id=%u]:invalid plat type.\n", devid);
        return -EINVAL;
    }

    len = sq_info[sq_index].slot_len * DEVDRV_MAX_FUNCTIONAL_SQ_DEPTH;

    devdrv_flush_cache_check_null(dev_info, (u64)((uintptr_t)sq_info[sq_index].addr), len);

    tail = (tail >= (DEVDRV_MAX_FUNCTIONAL_SQ_DEPTH - 1)) ? (0) : (tail + 1);

    sq_info[sq_index].tail = tail;

    *sq_info[sq_index].doorbell = (u32)tail;
    devdrv_drv_debug("a new functional sq cmd is sent, id : %d, tsid = %u, device_id = %u, doorbell = 0x%llx.\n",
        sq_index, tsid, devid, sq_info[sq_index].doorbell);

    return 0;
}
EXPORT_SYMBOL(devdrv_functional_send_sq);

int devdrv_mailbox_send_cqsq(u32 devid, u32 tsid, struct devdrv_mailbox_cqsq *cqsq)
{
    struct devdrv_mailbox *mailbox = NULL;
    struct devdrv_info *dev_info = NULL;
    int result = -1;
    u32 len;
    int ret;

    if ((devid >= DEVDRV_MAX_DAVINCI_NUM) || (cqsq == NULL) || (tsid >= DEVDRV_MAX_TS_NUM)) {
        devdrv_drv_err("devid(%u), cqsq(%pK), tsid(%u), "
                       "invalid input argument.\n",
                       devid, cqsq, tsid);
        return -EINVAL;
    }

    dev_info = devdrv_get_devdrv_info(devid);
    if (dev_info == NULL) {
        devdrv_drv_err("[dev_id=%u]:device not exist.\n", devid);
        return -ENODEV;
    }
    if (devdrv_is_low_power(dev_info->cce_ctrl[tsid])) {
        devdrv_drv_err("[dev_id=%u]:device is at low power.\n", devid);
        return DEVDRV_TS_SLEEP;
    }
    if (devdrv_is_ts_work(&dev_info->ts_mng[tsid]) == 0) {
        devdrv_drv_err("[dev_id=%u]:device is not working.\n", devid);
        return DEVDRV_TS_DOWN;
    }

    mailbox = &dev_info->cce_ctrl[tsid]->mailbox;
    len = sizeof(struct devdrv_mailbox_cqsq);

    devdrv_drv_debug("send functional cqsq to TS, cmdType: 0x%x, "
                     "sq id: %d, sq addr: %pK,"
                     "cq0 id: %d, cq0 addr: %pK,"
                     "cq1 id: %d, cq1 addr: %pK,"
                     "cq2 id: %d, cq2 addr: %pK.\n",
                     cqsq->cmd_type, cqsq->sq_index, (void *)cqsq->sq_addr, cqsq->cq0_index, (void *)cqsq->cq0_addr,
                     cqsq->cq1_index, (void *)cqsq->cq1_addr, cqsq->cq2_index, (void *)cqsq->cq2_addr);

    cqsq->plat_type = dev_info->plat_type;

    ret = devdrv_mailbox_kernel_sync_no_feedback(mailbox, (u8 *)cqsq, len, &result);
    if (ret == 0) {
        ret = result;
    }
    return ret;
}
EXPORT_SYMBOL(devdrv_mailbox_send_cqsq);
