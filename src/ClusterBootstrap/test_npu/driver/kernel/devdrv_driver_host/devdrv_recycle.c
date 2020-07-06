/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2012-2019. All rights reserved.
 * Description:
 * Author: huawei
 * Create: 2019-10-15
 */

#include <linux/atomic.h>
#include <linux/device.h>
#include <linux/list.h>
#include <linux/swap.h>
#include <linux/slab.h>

#include "devdrv_cqsq.h"
#include "devdrv_id.h"
#include "devdrv_recycle.h"

#ifdef CFG_SOC_PLATFORM_CLOUD
#define DEVDRV_RECYCLE_MAX_TIME_THRESHOLD 180
#else
#define DEVDRV_RECYCLE_MAX_TIME_THRESHOLD 60
#endif

#define DEVDRV_REPORT_TYPE 0x038
#define DEVDRV_REPORT_SQ_ID 0x1FF
#define devdrv_report_get_sq_index(report) ((report)->c & DEVDRV_REPORT_SQ_ID)
#define devdrv_report_get_type(report) (((report)->a & DEVDRV_REPORT_TYPE) >> 3)
#define DEVDRV_TS_RECYCLE_SQ_FINISHED 3


STATIC void devdrv_recycle_delete_message(struct devdrv_mailbox_message *message)
{
    struct devdrv_cce_ctrl *cce_ctrl = NULL;
    u32 tsid;

    if (message == NULL) {
        devdrv_drv_err("param is null.\n");
        return;
    }
    cce_ctrl = message->cce_ctrl;
    tsid = cce_ctrl->tsid;

    message->abandon = 1;

    message->cce_context->should_stop_thread = 1;
    up(&message->cce_context->ts_context[tsid].mailbox_wait);
    if (message->message_payload != NULL) {
        kfree(message->message_payload);
        message->message_payload = NULL;
    }
    if (message->feedback_buffer != NULL) {
        kfree(message->feedback_buffer);
        message->feedback_buffer = NULL;
    }
    kfree(message);
    message = NULL;
}

STATIC void devdrv_recycle_cce_context_message(struct devdrv_cce_context *cce_context, u32 tsid)
{
    struct devdrv_mailbox_message *message = NULL;
    struct list_head *pos = NULL, *n = NULL;

    if (cce_context == NULL) {
        devdrv_drv_err("param is null.\n");
        return;
    }

    spin_lock(&cce_context->ts_context[tsid].mailbox_wait_spinlock);
    list_for_each_safe(pos, n, &cce_context->ts_context[tsid].message_list_header) {
        message = list_entry(pos, struct devdrv_mailbox_message, wait_list_node);
        list_del(pos);
        devdrv_recycle_delete_message(message);
    }
    spin_unlock(&cce_context->ts_context[tsid].mailbox_wait_spinlock);
}

STATIC void devdrv_recycle_message(struct devdrv_cce_context *cce_context, u32 tsid)
{
    struct devdrv_mailbox_message *message = NULL;
    struct devdrv_mailbox *mailbox = NULL;
    struct list_head *pos = NULL, *n = NULL;
    int first_entry;

    if (cce_context == NULL) {
        devdrv_drv_err("param is null.\n");
        return;
    }

    mailbox = &cce_context->cce_ctrl[tsid]->mailbox;

    spin_lock(&mailbox->send_queue.spinlock);
    spin_lock(&mailbox->wait_queue.spinlock);
    if (!list_empty_careful(&mailbox->send_queue.list_header)) {
        first_entry = 0;
        list_for_each_safe(pos, n, &mailbox->send_queue.list_header) {
            message = list_entry(pos, struct devdrv_mailbox_message, send_list_node);
            message->process_result = -1;
            first_entry++;
            if (message->cce_context == cce_context) {
                if (message->feedback == DEVDRV_MAILBOX_NEED_FEEDBACK)
                    list_del(&message->wait_list_node);
                if (first_entry == 1) {
                    message->abandon = DEVDRV_MAILBOX_RECYCLE_MESSAGE;
                } else {
                    list_del(pos);
                    devdrv_recycle_delete_message(message);
                }
            }
        }
    }
    if (!list_empty_careful(&mailbox->wait_queue.list_header)) {
        first_entry = 0;
        list_for_each_safe(pos, n, &mailbox->wait_queue.list_header) {
            message = list_entry(pos, struct devdrv_mailbox_message, wait_list_node);
            message->process_result = -1;
            first_entry++;
            if (message->cce_context == cce_context) {
                if (first_entry == 1) {
                    message->abandon = DEVDRV_MAILBOX_RECYCLE_MESSAGE;
                } else {
                    list_del(pos);
                    devdrv_recycle_delete_message(message);
                }
            }
        }
    }
    spin_unlock(&mailbox->wait_queue.spinlock);
    spin_unlock(&mailbox->send_queue.spinlock);
}

STATIC int devdrv_inform_recycle_event_id(struct devdrv_cce_ctrl *cce_ctrl, struct devdrv_cce_context *cce_context)
{
    struct devdrv_event_info *event_info = NULL;
    struct devdrv_event_recycle event;
    struct list_head *pos = NULL, *n = NULL;
    int result = 0;
    int ret;
    u32 tsid;

    if ((cce_ctrl == NULL) || (cce_context == NULL)) {
        devdrv_drv_err("param is null.cce_ctrl = %pK, cce_context = %pK\n", cce_ctrl, cce_context);
        return -EINVAL;
    }
    tsid = cce_ctrl->tsid;
    ret = memset_s(&event, sizeof(struct devdrv_event_recycle), 0xFF, sizeof(struct devdrv_event_recycle));
    if (ret != 0) {
        devdrv_drv_warn("[dev_id = %u]:set event to 0xff failed.\n", cce_ctrl->devid);
        return -ENOMEM;
    }

    if (cce_context->ts_context[tsid].last_ts_status == DEVDRV_TS_DOWN)
        return 0;

    if (list_empty_careful(&cce_context->ts_context[tsid].event_list))
        return 0;

    event.count = 0;
    list_for_each_safe(pos, n, &cce_context->ts_context[tsid].event_list) {
        event_info = list_entry(pos, struct devdrv_event_info, list);
        event.event[event.count] = event_info->id;

        event.count++;
        if (event.count >= DEVDRV_RECYCLE_MAX_EVENT_NUM) {
            event.header.valid = DEVDRV_MAILBOX_MESSAGE_VALID;
            event.header.cmd_type = DEVDRV_MAILBOX_RECYCLE_EVENT_ID;
            event.header.result = 0;
            event.plat_type = cce_ctrl->dev_info->plat_type;
            ret = devdrv_mailbox_kernel_sync_no_feedback(&cce_ctrl->mailbox, (u8 *)&event,
                                                         sizeof(struct devdrv_event_recycle), &result);
            if (ret || result) {
                devdrv_drv_warn("[dev_id = %u]:send recycle event id message failed.\n", cce_ctrl->devid);
                return -EFAULT;
            }
            ret = memset_s(&event, sizeof(struct devdrv_event_recycle), 0xFF, sizeof(struct devdrv_event_recycle));
            if (ret != 0) {
                devdrv_drv_warn("[dev_id = %u]:set event to 0xff failed.\n", cce_ctrl->devid);
                return -EFAULT;
            }
            event.count = 0;
        }
    }

    if (event.count == 0)
        return 0;

    event.header.valid = DEVDRV_MAILBOX_MESSAGE_VALID;
    event.header.cmd_type = DEVDRV_MAILBOX_RECYCLE_EVENT_ID;
    event.header.result = 0;
    event.plat_type = cce_ctrl->dev_info->plat_type;
    ret = devdrv_mailbox_kernel_sync_no_feedback(&cce_ctrl->mailbox, (u8 *)&event, sizeof(struct devdrv_event_recycle),
                                                 &result);
    if (ret || result) {
        devdrv_drv_warn("[dev_id = %u]:send recycle event id message failed.\n", cce_ctrl->devid);
        return -EFAULT;
    } else
        return 0;
}

STATIC void devdrv_recycle_task_id(struct devdrv_cce_ctrl *cce_ctrl, struct devdrv_cce_context *cce_context)
{
    struct devdrv_taskpool_id_info *task_info = NULL;
    struct list_head *pos = NULL, *n = NULL;
    u32 tsid;

    if ((cce_ctrl == NULL) || (cce_context == NULL)) {
        devdrv_drv_err("param is null.cce_ctrl = %pK, cce_context = %pK\n", cce_ctrl, cce_context);
        return;
    }
    tsid = cce_ctrl->tsid;
    if (list_empty_careful(&cce_context->ts_context[tsid].task_list))
        return;

    list_for_each_safe(pos, n, &cce_context->ts_context[tsid].task_list) {
        task_info = list_entry(pos, struct devdrv_taskpool_id_info, list);
        (void)devdrv_free_task_id(cce_ctrl, cce_context, task_info->id);
    }

    return;
}

STATIC void devdrv_recycle_model_id(struct devdrv_cce_ctrl *cce_ctrl, struct devdrv_cce_context *cce_context)
{
    struct devdrv_model_id_info *model_info = NULL;
    struct list_head *pos = NULL, *n = NULL;
    u32 tsid;

    if ((cce_ctrl == NULL) || (cce_context == NULL)) {
        devdrv_drv_err("param is null.cce_ctrl = %pK, cce_context = %pK\n", cce_ctrl, cce_context);
        return;
    }
    tsid = cce_ctrl->tsid;
    if (list_empty_careful(&cce_context->ts_context[tsid].model_list))
        return;

    list_for_each_safe(pos, n, &cce_context->ts_context[tsid].model_list) {
        model_info = list_entry(pos, struct devdrv_model_id_info, list);
        (void)devdrv_free_model_id(cce_ctrl, cce_context, model_info->id);
    }

    return;
}

STATIC void devdrv_recycle_notify_id(struct devdrv_cce_ctrl *cce_ctrl, struct devdrv_cce_context *cce_context)
{
    struct devdrv_notify_id_info *notify_info = NULL;
    struct list_head *pos = NULL, *n = NULL;
    u32 tsid = cce_ctrl->tsid;

    if (list_empty_careful(&cce_context->ts_context[tsid].notify_list))
        return;

    list_for_each_safe(pos, n, &cce_context->ts_context[tsid].notify_list) {
        notify_info = list_entry(pos, struct devdrv_notify_id_info, list);
        (void)devdrv_free_one_notify_id(cce_ctrl, cce_context, notify_info->id, DEVDRV_NOTIFY_INFORM_TS);
    }

    return;
}

STATIC int devdrv_recycle_event_id(struct devdrv_cce_ctrl *cce_ctrl, struct devdrv_cce_context *cce_context)
{
    struct devdrv_event_info *event_info = NULL;
    struct list_head *pos = NULL, *n = NULL;
    u32 tsid;
    int ret;

    if ((cce_ctrl == NULL) || (cce_context == NULL)) {
        devdrv_drv_err("param is null.cce_ctrl = %pK, cce_context = %pK\n", cce_ctrl, cce_context);
        return -EINVAL;
    }
    tsid = cce_ctrl->tsid;
    if (list_empty_careful(&cce_context->ts_context[tsid].event_list))
        return 0;

    if (cce_context->ts_context[tsid].last_ts_status != DEVDRV_TS_DOWN) {
        ret = devdrv_inform_recycle_event_id(cce_ctrl, cce_context);
        if (ret) {
            devdrv_drv_warn("[dev_id = %u]:devdrv_recycle_event_id failed.\n", cce_ctrl->devid);
            return -EFAULT;
        }
    }

    list_for_each_safe(pos, n, &cce_context->ts_context[tsid].event_list) {
        event_info = list_entry(pos, struct devdrv_event_info, list);
        (void)devdrv_free_event_id(cce_ctrl, cce_context, event_info->id);
    }

    return 0;
}

STATIC void devdrv_update_report(u32 tsid, struct devdrv_ts_cq_info *cq_info, const struct uio_mem *mem,
    const struct devdrv_cce_context *cce_context)
{
    struct devdrv_report *report = NULL;
    u32 next_tail;
    u32 phase;
    u32 len;

    if ((cq_info == NULL) || (mem == NULL) || (cce_context == NULL)) {
        devdrv_drv_err("param is null. cq_info = %pK, mem = %pK, cce_context = %pK\n", cq_info, mem, cce_context);
        return;
    }

    len = cq_info->slot_size * DEVDRV_MAX_CQ_DEPTH;
    cce_context->cce_ctrl[tsid]->dev_info->drv_ops->flush_cache((u64)mem[DEVDRV_SQ_CQ_MAP].addr, len);
    report = (struct devdrv_report *)(uintptr_t)((unsigned long)mem[DEVDRV_SQ_CQ_MAP].addr +
                                                 (unsigned long)cq_info->slot_size * cq_info->tail);
    phase = devdrv_report_get_phase(report);

    while (phase == cq_info->phase) {
        next_tail = (cq_info->tail + 1) % DEVDRV_MAX_CQ_DEPTH;
        if (next_tail == cq_info->head)
            break;

        next_tail = cq_info->tail + 1;
        if (next_tail > (DEVDRV_MAX_CQ_DEPTH - 1)) {
            cq_info->phase = ((cq_info->phase == 0) ? 1 : 0);
            cq_info->tail = 0;
        } else
            cq_info->tail++;
        report = (struct devdrv_report *)(uintptr_t)((unsigned long)(uintptr_t)mem[DEVDRV_SQ_CQ_MAP].addr +
                                                     (unsigned long)cq_info->slot_size * cq_info->tail);
        phase = devdrv_report_get_phase(report);
    }
}

STATIC void devdrv_wait_report(u32 tsid, struct devdrv_ts_cq_info *cq_info,
                               const struct devdrv_cq_sub_info *cq_sub_info, struct devdrv_cce_context *cce_context)
{
    struct devdrv_report *report = NULL;
    struct uio_mem *mem = NULL;
    unsigned long jiffy;
    u32 phase;
    u32 len;

    if ((cq_info == NULL) || (cq_sub_info == NULL) || (cce_context == NULL)) {
        devdrv_drv_err("param is null. cq_info = %pK, cq_sub_info = %pK, cce_context = %pK\n", cq_info, cq_sub_info,
                       cce_context);
        return;
    }

    mem = cq_sub_info->uio->mem;
    len = cq_info->slot_size * DEVDRV_MAX_CQ_DEPTH;
    cce_context->cce_ctrl[tsid]->dev_info->drv_ops->flush_cache((u64)mem[DEVDRV_SQ_CQ_MAP].addr, len);

    report = (struct devdrv_report *)(uintptr_t)((unsigned long)mem[DEVDRV_SQ_CQ_MAP].addr +
                                                 (unsigned long)cq_info->slot_size * cq_info->tail);

    phase = devdrv_report_get_phase(report);
    if (phase == cq_info->phase) {
        devdrv_update_report(tsid, cq_info, mem, cce_context);
    } else {
        jiffy = msecs_to_jiffies(1000);
        cce_context->ts_context[tsid].cq_tail_updated = 0;
        if (wait_event_interruptible_timeout(cce_context->ts_context[tsid].report_wait,
                                             cce_context->ts_context[tsid].cq_tail_updated == CQ_HEAD_UPDATE_FLAG,
                                             jiffy)) {
            cce_context->ts_context[tsid].cq_tail_updated = 0;
            devdrv_update_report(tsid, cq_info, mem, cce_context);
        }
    }
}

STATIC int devdrv_get_sq_recycle_report(u32 tsid, struct devdrv_ts_cq_info *cq_info,
                                        struct devdrv_cq_sub_info *cq_sub_info, struct devdrv_cce_context *cce_context,
                                        u32 sq_index)
{
    struct devdrv_report *report = NULL;
    u32 __iomem *cq_doorbell = NULL;
    struct uio_mem *mem = NULL;
    u16 report_type;
    u32 len;

    if ((cq_info == NULL) || (cq_sub_info == NULL) || (cce_context == NULL)) {
        devdrv_drv_err("param is null. cq_info = %pK, cq_sub_info = %pK, cce_context = %pK\n", cq_info, cq_sub_info,
                       cce_context);
        return -EINVAL;
    }
    if (cq_info->index >= DEVDRV_MAX_CQ_NUM) {
        devdrv_drv_err("[dev_id=%u]:cq_info->index = %d\n", cce_context->devid, cq_info->index);
        return -EINVAL;
    }

    mem = cq_sub_info->uio->mem;
    len = cq_info->slot_size * DEVDRV_MAX_CQ_DEPTH;
    cce_context->cce_ctrl[tsid]->dev_info->drv_ops->flush_cache((u64)mem[DEVDRV_SQ_CQ_MAP].addr, len);
    report = (struct devdrv_report *)(uintptr_t)((unsigned long)mem[DEVDRV_SQ_CQ_MAP].addr +
                                                 (unsigned long)cq_info->slot_size * cq_info->head);
    cq_doorbell = devdrv_doorbell_get_cq_head(cce_context->cce_ctrl[tsid]->dev_info, tsid, cq_info->index);
    if (cq_doorbell == NULL) {
        devdrv_drv_err("cq_doorbell is null.\n");
        return -EINVAL;
    }

    while (cq_info->head != cq_info->tail) {
        report = (struct devdrv_report *)(uintptr_t)((unsigned long)(uintptr_t)mem[DEVDRV_SQ_CQ_MAP].addr +
                                                     (unsigned long)cq_info->slot_size * cq_info->head);
        report_type = devdrv_report_get_type(report);
        cq_info->head = (cq_info->head + 1) % DEVDRV_MAX_CQ_DEPTH;
        *cq_doorbell = cq_info->head;

        if (report_type != DEVDRV_TS_RECYCLE_SQ_FINISHED)
            continue;

        if (devdrv_report_get_sq_index(report) == sq_index) {
            return 0;
        } else {
            devdrv_drv_warn("[dev_id = %u]:find recycle sq finished report, "
                            "but sq id does not match.\n",
                            cce_context->devid);
            return -EFAULT;
        }
    }
    return -EFAULT;
}

STATIC int devdrv_wait_recycle_ok_report(const struct devdrv_cce_ctrl *cce_ctrl, struct devdrv_cce_context *cce_context,
                                         u32 sq_index)
{
    struct devdrv_cq_sub_info *cq_sub_info = NULL;
    struct devdrv_ts_cq_info *cq_info = NULL;
    phys_addr_t info_mem_addr;
    u32 times;
    u32 tsid;
    int ret;

    if ((cce_ctrl == NULL) || (cce_context == NULL)) {
        devdrv_drv_err("param is null. cce_ctrl = %pK, cce_context = %pK\n", cce_ctrl, cce_context);
        return -EINVAL;
    }

    tsid = cce_ctrl->tsid;
    cq_sub_info = list_first_entry(&cce_context->ts_context[tsid].cq_list, struct devdrv_cq_sub_info, list);
    info_mem_addr = cce_ctrl->mem_info[DEVDRV_INFO_MEM].virt_addr;
    cq_info = devdrv_calc_cq_info(info_mem_addr, cq_sub_info->index);

    times = 0;
    while (1) {
        if (cce_ctrl->dev_info->status == 1) {
            /* host hot reset happen, no need recycle source */
            devdrv_drv_warn("[dev_id = %u]: has been reset. times(%u)\n", cce_ctrl->devid, times);
            break;
        }

        times++;
        if (times >= DEVDRV_RECYCLE_MAX_TIME_THRESHOLD) {
            devdrv_drv_warn("[dev_id = %u]:wait too long to get [ts recycle sq ok] "
                            "report, sq id: %u.\n",
                            cce_ctrl->devid, sq_index);
            return -ETIMEDOUT;
        }
        /* view recycle process cost how many 5s */
        if (times % 5 == 0) {
            devdrv_drv_warn("[dev_id = %u]:wait ts 5s once\n", cce_ctrl->devid);
        }

        devdrv_wait_report(tsid, cq_info, cq_sub_info, cce_context);

        ret = devdrv_get_sq_recycle_report(tsid, cq_info, cq_sub_info, cce_context, sq_index);
        if (ret == 0) {
            break;
        }
    }

    devdrv_drv_info("[dev_id = %u]:times(%u)\n", cce_ctrl->devid, times);
    return 0;
}

STATIC int devdrv_inform_recycle_one_sq(struct devdrv_cce_ctrl *cce_ctrl, struct devdrv_cce_context *cce_context,
                                        const struct devdrv_sq_sub_info *sq_sub_info)
{
    struct devdrv_cq_sub_info *cq_sub_info = NULL;
    struct devdrv_mailbox_cqsq cqsq;
    int result = 0;
    int ret;

    if ((cce_ctrl == NULL) || (cce_context == NULL) || (sq_sub_info == NULL)) {
        devdrv_drv_err("param is null. cce_ctrl = %pK, cce_context = %pK, sq_sub_info = %pK\n", cce_ctrl, cce_context,
                       sq_sub_info);
        return -EINVAL;
    }

    cq_sub_info = list_first_entry(&cce_context->ts_context[cce_ctrl->tsid].cq_list, struct devdrv_cq_sub_info, list);
    ret = memset_s(&cqsq, sizeof(struct devdrv_mailbox_cqsq), 0xFF, sizeof(struct devdrv_mailbox_cqsq));
    if (ret != 0) {
        devdrv_drv_warn("[dev_id = %u]:set cqsq to 0xff failed.\n", cce_ctrl->devid);
        return -EFAULT;
    }
    cqsq.valid = DEVDRV_MAILBOX_MESSAGE_VALID;
    cqsq.cmd_type = DEVDRV_MAILBOX_RECYCLE_STREAM_ID;
    cqsq.result = 0;
    cqsq.sq_index = sq_sub_info->index;
    cqsq.cq0_index = cq_sub_info->index;
    cqsq.plat_type = cce_ctrl->dev_info->plat_type;
    ret = devdrv_mailbox_kernel_sync_no_feedback(&cce_ctrl->mailbox, (u8 *)&cqsq, sizeof(struct devdrv_mailbox_cqsq),
                                                 &result);
    if (ret || result) {
        devdrv_drv_warn("[dev_id = %u]:devdrv_mailbox_kernel_sync_no_feedback failed.\n", cce_ctrl->devid);
        return -EFAULT;
    } else
        return 0;
}

STATIC int devdrv_wait_recycle_sq_ok(struct devdrv_cce_ctrl *cce_ctrl, struct devdrv_cce_context *cce_context,
                                     struct devdrv_sq_sub_info *sq_sub_info)
{
    return devdrv_wait_recycle_ok_report(cce_ctrl, cce_context, sq_sub_info->index);
}

STATIC int devdrv_recycle_one_sq(struct devdrv_cce_ctrl *cce_ctrl, struct devdrv_cce_context *cce_context,
                                 struct devdrv_sq_sub_info *sq_sub_info)
{
    struct devdrv_stream_sub_info *stream_sub_info = NULL;
    struct list_head *pos = NULL, *n = NULL;
    int error;
    int ret;

    if ((cce_ctrl == NULL) || (cce_context == NULL) || (sq_sub_info == NULL)) {
        devdrv_drv_err("param is null. cce_ctrl = %pK, cce_context = %pK, sq_sub_info = %pK\n", cce_ctrl, cce_context,
                       sq_sub_info);
        return -EINVAL;
    }

    ret = devdrv_inform_recycle_one_sq(cce_ctrl, cce_context, sq_sub_info);
    if (ret) {
        devdrv_drv_warn("[dev_id = %u]:devdrv_inform_recycle_one_sq failed, ret = %d.\n", cce_ctrl->devid, ret);
        return -EFAULT;
    }

    ret = devdrv_wait_recycle_sq_ok(cce_ctrl, cce_context, sq_sub_info);
    if (ret) {
        devdrv_drv_warn("[dev_id = %u]:devdrv_wait_recycle_sq_ok failed, ret = %d.\n", cce_ctrl->devid, ret);
        return -EFAULT;
    }

    error = 0;
    list_for_each_safe(pos, n, &sq_sub_info->list_stream)
    {
        stream_sub_info = list_entry(pos, struct devdrv_stream_sub_info, sq_list);
        ret = devdrv_free_stream(cce_context->cce_ctrl[0], cce_context, stream_sub_info->id, DEVDRV_HAVE_TO_INFORM);
        if (ret) {
            devdrv_drv_warn("[dev_id = %u]:devdrv_free_stream failed.\n", cce_ctrl->devid);
            error++;
        }
    }
    if (error)
        return -EFAULT;
    else
        return 0;
}

STATIC int devdrv_recycle_sq(struct devdrv_cce_ctrl *cce_ctrl, struct devdrv_cce_context *cce_context)
{
    struct devdrv_sq_sub_info *sq_sub_info = NULL;
    struct list_head *pos = NULL, *n = NULL;
    int error;
    int ret;

    if (cce_context == NULL) {
        devdrv_drv_err("param is null. cce_context = %pK\n", cce_context);
        return -EINVAL;
    }

    if (list_empty_careful(&cce_context->ts_context[cce_ctrl->tsid].sq_list))
        return 0;

    error = 0;
    list_for_each_safe(pos, n, &cce_context->ts_context[cce_ctrl->tsid].sq_list) {
        sq_sub_info = list_entry(pos, struct devdrv_sq_sub_info, list);
        ret = devdrv_recycle_one_sq(cce_context->cce_ctrl[cce_ctrl->tsid], cce_context, sq_sub_info);
        if (ret) {
            devdrv_drv_warn("[dev_id = %u]:recycle sq: %d failed, TS doesn't "
                            "send ts recycle ok report in %ds.\n",
                            cce_ctrl->devid, sq_sub_info->index, DEVDRV_RECYCLE_MAX_TIME_THRESHOLD);
            error++;
        }
    }

    if (error) {
        devdrv_drv_warn("[dev_id = %u]:some sq are not recycled because TS can't recycle them.\n", cce_ctrl->devid);
        return -EFAULT;
    } else {
        return 0;
    }
}

STATIC void devdrv_update_phase_and_tail(struct devdrv_cce_ctrl *cce_ctrl, const struct devdrv_cce_context *cce_context)
{
    struct devdrv_cq_sub_info *cq_sub_info = NULL;
    struct devdrv_ts_cq_info *cq_info = NULL;
    struct devdrv_report *report = NULL;
    struct uio_mem *cq_mem = NULL;
    phys_addr_t info_mem_addr;
    u32 phase;
    u32 len;

    if ((cce_ctrl == NULL) || (cce_context == NULL)) {
        devdrv_drv_err("param is null. cce_ctrl = %pK, cce_context = %pK\n", cce_ctrl, cce_context);
        return;
    }

    cq_sub_info = list_first_entry(&cce_context->ts_context[cce_ctrl->tsid].cq_list, struct devdrv_cq_sub_info, list);
    info_mem_addr = cce_ctrl->mem_info[DEVDRV_INFO_MEM].virt_addr;
    cq_info = devdrv_calc_cq_info(info_mem_addr, cq_sub_info->index);

    cq_mem = cq_sub_info->uio->mem;
    len = cq_info->slot_size * DEVDRV_MAX_CQ_DEPTH;

    cce_ctrl->dev_info->drv_ops->flush_cache((u64)cq_mem[DEVDRV_SQ_CQ_MAP].addr, len);
    report = (struct devdrv_report *)(uintptr_t)((unsigned long)(uintptr_t)cq_mem[DEVDRV_SQ_CQ_MAP].addr +
                                                 (unsigned long)cq_info->slot_size * cq_info->head);

    phase = devdrv_report_get_phase(report);

    if (cq_info->head == cq_info->tail) {
        if (cq_info->head == DEVDRV_MAX_CQ_DEPTH - 1)
            cq_info->phase = ((phase == 1) ? 0 : 1);
    } else if (cq_info->head < cq_info->tail) {
        cq_info->phase = phase;
    } else {
        cq_info->phase = ((phase == 1) ? 0 : 1);
    }
}

STATIC int devdrv_recycle_stream(struct devdrv_cce_ctrl *cce_ctrl, struct devdrv_cce_context *cce_context)
{
    struct devdrv_stream_sub_info *stream_sub_info = NULL;
    struct list_head *pos = NULL, *n = NULL;
    int error;
    int ret;
    u32 tsid;

    if ((cce_ctrl == NULL) || (cce_context == NULL)) {
        devdrv_drv_err("param is null. cce_ctrl = %pK, cce_context = %pK\n", cce_ctrl, cce_context);
        return -EINVAL;
    }
    tsid = cce_ctrl->tsid;
    if (list_empty_careful(&cce_context->ts_context[tsid].stream_list))
        return 0;
    devdrv_update_phase_and_tail(cce_ctrl, cce_context);

    if (cce_context->ts_context[tsid].last_ts_status != DEVDRV_TS_DOWN) {
        ret = devdrv_recycle_sq(cce_ctrl, cce_context);
        if (ret) {
            devdrv_drv_warn("[dev_id = %u]:devdrv_recycle_sq failed.\n", cce_ctrl->devid);
            ret = -1;
        }
    } else {
        error = 0;
        list_for_each_safe(pos, n, &cce_context->ts_context[tsid].stream_list) {
            stream_sub_info = list_entry(pos, struct devdrv_stream_sub_info, list);
            ret = devdrv_free_stream(cce_context->cce_ctrl[tsid], cce_context,
                                     stream_sub_info->id, DEVDRV_NO_NEED_TO_INFORM);
            if (ret)
                error++;
        }
        if (error)
            ret = -1;
        else
            ret = 0;
    }
    return ret;
}

STATIC void devdrv_recycle_cq(struct devdrv_cce_ctrl *cce_ctrl, struct devdrv_cce_context *cce_context)
{
    struct devdrv_cq_sub_info *cq_sub_info = NULL;
    struct devdrv_ts_cq_info *cq_info = NULL;
    phys_addr_t info_mem_addr;
    u32 tsid;

    if ((cce_ctrl == NULL) || (cce_context == NULL)) {
        devdrv_drv_err("param is null. cce_ctrl = %pK, cce_context = %pK\n", cce_ctrl, cce_context);
        return;
    }
    tsid = cce_ctrl->tsid;
    if (list_empty_careful(&cce_context->ts_context[tsid].cq_list))
        return;

    cq_sub_info = list_first_entry(&cce_context->ts_context[tsid].cq_list, struct devdrv_cq_sub_info, list);
    info_mem_addr = cce_ctrl->mem_info[DEVDRV_INFO_MEM].virt_addr;
    cq_info = devdrv_calc_cq_info(info_mem_addr, cq_sub_info->index);
    devdrv_exit_cq_uio(cce_ctrl, cq_info);
    devdrv_free_cq(cce_ctrl, cce_context, cq_info);
}

void devdrv_recycle_work(struct work_struct *work)
{
    struct devdrv_cce_context *cce_context = NULL;
    struct devdrv_ts_context *ts_context = NULL;
    struct devdrv_cce_ctrl *cce_ctrl = NULL;
    struct devdrv_info *dev_info = NULL;
    u32 tsid;
    int ret;

    if (work == NULL) {
        devdrv_drv_err("work is null.\n");
        return;
    }
    ts_context = container_of(work, struct devdrv_ts_context, recycle_work);
    cce_context = ts_context->cce_context;
    tsid = ts_context->tsid;

    cce_ctrl = cce_context->cce_ctrl[tsid];
    dev_info = cce_ctrl->dev_info;

    /* process do not need clear pte table */
    cce_context->vma[tsid] = NULL;
    cce_context->recycle_status = DEVDRV_RECYCLE_STATUS;

    /* recycle cce_context mailbox message */
    if (!list_empty_careful(&cce_context->ts_context[tsid].message_list_header))
        devdrv_recycle_cce_context_message(cce_context, tsid);
    if (atomic_read(&cce_context->ts_context[tsid].mailbox_message_count))
        devdrv_recycle_message(cce_context, tsid);

    devdrv_recycle_model_id(cce_ctrl, cce_context);

    devdrv_drv_info("[dev_id = %u]:start_recycle_notify.\n", cce_ctrl->devid);
    devdrv_recycle_notify_id(cce_ctrl, cce_context);

    devdrv_recycle_task_id(cce_ctrl, cce_context);

    /* recycle stream */
    devdrv_drv_info("[dev_id = %u]:start_recycle_stream.\n", cce_ctrl->devid);
    ret = devdrv_recycle_stream(cce_ctrl, cce_context);
    if (ret) {
        devdrv_drv_warn("[dev_id = %u]:devdrv_recycle_stream failed, ret(%d).\n", cce_ctrl->devid, ret);
        goto out;
    }

    /* recycle event id */
    devdrv_drv_info("[dev_id = %u]:start_recycle_event.\n", cce_ctrl->devid);
    ret = devdrv_recycle_event_id(cce_ctrl, cce_context);
    if (ret) {
        devdrv_drv_warn("[dev_id = %u]:devdrv_recycle_event_id failed, ret(%d).\n", cce_ctrl->devid, ret);
        goto out;
    }

    /* recycle cq */
    devdrv_recycle_cq(cce_ctrl, cce_context);
out:
    spin_lock_bh(&dev_info->spinlock);
    list_del(&cce_context->cce_ctrl_list);
    spin_unlock_bh(&dev_info->spinlock);
    kfree(cce_context);
    cce_context = NULL;
    atomic_dec(&cce_ctrl->recycle_work_num);

    return;
}
