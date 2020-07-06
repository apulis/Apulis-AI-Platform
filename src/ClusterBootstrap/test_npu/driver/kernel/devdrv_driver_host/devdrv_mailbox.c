/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2012-2019. All rights reserved.
 * Description:
 * Author: huawei
 * Create: 2019-10-15
 */

#include <linux/irq.h>
#include <linux/jiffies.h>
#include <linux/list.h>
#include <linux/semaphore.h>
#include <linux/slab.h>
#include <linux/workqueue.h>
#include <linux/io.h>
#include <linux/delay.h>

#include "devdrv_driver_pm.h"
#include "devdrv_manager_comm.h"

/* trigger irq to inform TS */
STATIC void devdrv_mailbox_trigger_irq(struct devdrv_info *dev_info, u32 tsid, int irq_num)
{
    if ((dev_info == NULL) || (dev_info->drv_ops == NULL)) {
        devdrv_drv_err("dev_info = %pK\n", dev_info);
        return;
    }
    if (dev_info->drv_ops->irq_trigger == NULL) {
        devdrv_drv_err("[dev_id = %u]:irq trigger is NULL\n", dev_info->dev_id);
        return;
    }
    dev_info->drv_ops->irq_trigger(dev_info, tsid, irq_num);
}

STATIC void devdrv_delete_message(struct devdrv_mailbox_message *message)
{
    struct devdrv_cce_ctrl *cce_ctrl = message->cce_ctrl;
    u32 tsid = cce_ctrl->tsid;

    message->process_result = -1;
    if (message->abandon == DEVDRV_MAILBOX_RECYCLE_MESSAGE)
        goto out;

    message->abandon = DEVDRV_MAILBOX_RECYCLE_MESSAGE;

    if (message->sync_type == DEVDRV_MAILBOX_SYNC) {
        up(&message->wait);
        up(&message->wait);
        return;
    }

    if (message->cce_context == NULL) {
        if (message->callback != NULL)
            message->callback(message);
        return;
    }

    message->cce_context->should_stop_thread = 1;
    up(&message->cce_context->ts_context[tsid].mailbox_wait);

out:
    kfree(message->message_payload);
    message->message_payload = NULL;
    if (message->feedback_buffer != NULL) {
        kfree(message->feedback_buffer);
        message->feedback_buffer = NULL;
    }
    kfree(message);
    message = NULL;
}

void devdrv_mailbox_recycle(struct devdrv_mailbox *mailbox)
{
    struct devdrv_mailbox_message *message = NULL;
    struct list_head *pos = NULL, *n = NULL;

    spin_lock(&mailbox->send_queue.spinlock);
    spin_lock(&mailbox->wait_queue.spinlock);

    if (!list_empty_careful(&mailbox->send_queue.list_header)) {
        list_for_each_safe(pos, n, &mailbox->send_queue.list_header) {
            message = list_entry(pos, struct devdrv_mailbox_message, send_list_node);
            list_del(pos);
            if (message->feedback == DEVDRV_MAILBOX_NO_FEEDBACK)
                devdrv_delete_message(message);
        }
    }
    if (!list_empty_careful(&mailbox->wait_queue.list_header)) {
        list_for_each_safe(pos, n, &mailbox->wait_queue.list_header) {
            message = list_entry(pos, struct devdrv_mailbox_message, wait_list_node);
            list_del(pos);
            devdrv_delete_message(message);
        }
    }

    spin_unlock(&mailbox->send_queue.spinlock);
    spin_unlock(&mailbox->wait_queue.spinlock);
}

void devdrv_mailbox_exit(struct devdrv_mailbox *mailbox)
{
    struct devdrv_cce_ctrl *cce_ctrl = NULL;
    if (mailbox == NULL) {
        devdrv_drv_err("mailbox is null.\n");
        return;
    }
    cce_ctrl = container_of(mailbox, struct devdrv_cce_ctrl, mailbox);

    free_irq(mailbox->ack_irq, mailbox);

#if defined(CONFIG_DATA_ACK_IRQ) /* not used irq, there is no data ack msi-x irq */
    free_irq(mailbox->data_ack_irq, mailbox);
#endif
    destroy_workqueue(mailbox->send_queue.wq);
    destroy_workqueue(mailbox->wait_queue.wq);

    mutex_destroy(&mailbox->wait_mutex);

    mailbox->working = 0;
    devdrv_mailbox_recycle(mailbox);
}

int devdrv_mailbox_init(struct devdrv_mailbox *mailbox)
{
    struct devdrv_cce_ctrl *cce_ctrl = NULL;
    int ret;

    if (mailbox == NULL) {
        devdrv_drv_err("input argument error.\n");
        return -EINVAL;
    }
    cce_ctrl = container_of(mailbox, struct devdrv_cce_ctrl, mailbox);

    ret = 0;

    /* init mailbox */
    spin_lock_init(&mailbox->send_queue.spinlock);
    INIT_LIST_HEAD(&mailbox->send_queue.list_header);

    spin_lock(&mailbox->send_queue.spinlock);
    mailbox->send_queue.mailbox_type = DEVDRV_MAILBOX_SRAM;
    mailbox->send_queue.status = DEVDRV_MAILBOX_FREE;
    spin_unlock(&mailbox->send_queue.spinlock);

    mailbox->send_queue.wq = create_workqueue("devdrv-mb-send");
    if (mailbox->send_queue.wq == NULL) {
        devdrv_drv_err("[dev_id=%u]:create_workqueue error.\n", cce_ctrl->devid);
        ret = -ENOMEM;
        goto send_workqueue_fail;
    }

    mailbox->wait_queue.mailbox_type = DEVDRV_MAILBOX_SRAM;
    spin_lock_init(&mailbox->wait_queue.spinlock);
    INIT_LIST_HEAD(&mailbox->wait_queue.list_header);
    mutex_init(&mailbox->wait_mutex);
    mailbox->wait_queue.wq = create_workqueue("devdrv-mb-wait");
    if (mailbox->wait_queue.wq == NULL) {
        devdrv_drv_err("[dev_id=%u]:create_workqueue error.\n", cce_ctrl->devid);
        ret = -ENOMEM;
        goto wait_workqueue_fail;
    }

    /* register irq handler */
    ret = request_irq(mailbox->ack_irq, devdrv_mailbox_ack_irq, IRQF_TRIGGER_NONE, "devdrv-ack", mailbox);
    if (ret) {
        devdrv_drv_err("[dev_id=%u]:request_irq ack irq failed ret 0x%x.\n", cce_ctrl->devid, ret);
        goto register_ack_fail;
    }

#if defined(CONFIG_DATA_ACK_IRQ) /* not used irq, there is no data ack msi-x irq */
    ret = request_irq(mailbox->data_ack_irq, devdrv_mailbox_feedback_irq, IRQF_TRIGGER_NONE, "devdrv-feedback",
                      mailbox);
    if (ret) {
        devdrv_drv_err("[dev_id=%u]:request_irq data ack irq failed.\n", cce_ctrl->devid);
        goto register_data_ack_fail;
    }
#endif
    mailbox->working = 1;

    return 0;

#if defined(CONFIG_DATA_ACK_IRQ)
register_data_ack_fail:
    free_irq(mailbox->ack_irq, mailbox);
#endif

register_ack_fail:
    destroy_workqueue(mailbox->wait_queue.wq);
wait_workqueue_fail:
    destroy_workqueue(mailbox->send_queue.wq);
send_workqueue_fail:
    return ret;
}

STATIC void devdrv_mailbox_data_send(struct devdrv_info *dev_info, struct devdrv_mailbox *mailbox,
                                     struct devdrv_mailbox_message *message)
{
    struct devdrv_cce_ctrl *cce_ctrl = message->cce_ctrl;
    u32 tsid = cce_ctrl->tsid;
    int ret;
    u32 i;

    if (dev_info->plat_type == DEVDRV_MANAGER_DEVICE_ENV) {
        ret = memcpy_s(mailbox->send_sram, DEVDRV_MAILBOX_PAYLOAD_LENGTH, message->message_payload,
                       message->message_length);
        if (ret) {
            devdrv_drv_err("[dev_id = %u]:DEVICE: copy to mailbox's send_sram failed, ret = %d.\n", dev_info->dev_id,
                           ret);
            return;
        }

        for (i = message->message_length; i < DEVDRV_MAILBOX_PAYLOAD_LENGTH; i++) {
            mailbox->send_sram[i] = 0;
        }
        mb(); /* memory barrier */
        message->is_sent = 1;
        devdrv_mailbox_trigger_irq(dev_info, tsid, DEVDRV_MAILBOX_SEND_OFFLINE_IRQ);
        return;
    } else if (dev_info->plat_type == DEVDRV_MANAGER_HOST_ENV) {
        ret = memcpy_s(mailbox->receive_sram, DEVDRV_MAILBOX_PAYLOAD_LENGTH, message->message_payload,
                       message->message_length);
        if (ret) {
            devdrv_drv_err("[dev_id = %u]:HOST: copy to mailbox's send_sram failed, ret = %d.\n", dev_info->dev_id,
                           ret);
            return;
        }

        for (i = message->message_length; i < DEVDRV_MAILBOX_PAYLOAD_LENGTH; i++) {
            mailbox->receive_sram[i] = 0;
        }
        mb(); /* memory barrier */
        message->is_sent = 1;
        devdrv_mailbox_trigger_irq(dev_info, tsid, DEVDRV_MAILBOX_SEND_ONLINE_IRQ);
        return;
    }
}

/*
 * interrupt bottom half for mailbox ack and feedback
 * must be reentrant
 */
STATIC void devdrv_mailbox_ack_work(struct work_struct *work)
{
    struct devdrv_mailbox_message_header *header = NULL;
    struct devdrv_mailbox_message *next_message = NULL;
    struct devdrv_mailbox_message *message = NULL;
    struct devdrv_cce_ctrl *cce_ctrl = NULL;
    u32 tsid;

    message = container_of(work, struct devdrv_mailbox_message, send_work);
    cce_ctrl = message->cce_ctrl;
    tsid = cce_ctrl->tsid;

    spin_lock(&message->mailbox->send_queue.spinlock);
    list_del(&message->send_list_node);

    message->mailbox->send_queue.status = DEVDRV_MAILBOX_BUSY;
    if (!list_empty_careful(&message->mailbox->send_queue.list_header)) {
        next_message = list_first_entry(&message->mailbox->send_queue.list_header, struct devdrv_mailbox_message,
                                        send_list_node);
        devdrv_mailbox_data_send(message->dev_info, message->mailbox, next_message);
    } else {
        message->mailbox->send_queue.status = DEVDRV_MAILBOX_FREE;
    }
    spin_unlock(&message->mailbox->send_queue.spinlock);

    /*
     * recycle message if it's abandoned
     * if it's a feedback message,
     * we have delete it from wait queue when we set it as a recycle message
     */
    if (message->abandon == DEVDRV_MAILBOX_RECYCLE_MESSAGE) {
        kfree(message->message_payload);
        message->message_payload = NULL;
        if (message->feedback_buffer != NULL) {
            kfree(message->feedback_buffer);
            message->feedback_buffer = NULL;
        }
        kfree(message);
        message = NULL;
        return;
    }

    /* read process result add by TS */
    /* read process result add by TS */
    if (message->dev_info->plat_type == DEVDRV_MANAGER_DEVICE_ENV)
        header = (struct devdrv_mailbox_message_header *)message->mailbox->send_sram;
    else
        header = (struct devdrv_mailbox_message_header *)message->mailbox->receive_sram;

    message->process_result = header->result;

    /*
     * if process_result is not zero, feedback message will never receive feedback
     * take this into acnt.
     */
    if ((message->feedback == DEVDRV_MAILBOX_NEED_FEEDBACK) && (message->process_result == 0))
        return;
    if ((message->feedback == DEVDRV_MAILBOX_NEED_FEEDBACK) && message->process_result) {
        spin_lock(&message->mailbox->wait_queue.spinlock);
        list_del(&message->wait_list_node);
        spin_unlock(&message->mailbox->wait_queue.spinlock);
    }

    /* distribute message according to message's type */
    if (message->message_type == DEVDRV_MAILBOX_SYNC_MESSAGE) {
        up(&message->wait);
    } else if (message->message_type == DEVDRV_MAILBOX_CONTEXT_ASYNC_MESSAGE) {
        spin_lock(&message->cce_context->ts_context[tsid].mailbox_wait_spinlock);
        list_add_tail(&message->wait_list_node, &message->cce_context->ts_context[tsid].message_list_header);
        spin_unlock(&message->cce_context->ts_context[tsid].mailbox_wait_spinlock);
        up(&message->cce_context->ts_context[tsid].mailbox_wait);
    } else {
        if (message->callback != NULL)
            message->callback(message);
    }
}

STATIC void devdrv_mailbox_feedback_work(struct work_struct *work)
{
    struct devdrv_mailbox_message *message = NULL;
    struct devdrv_cce_ctrl *cce_ctrl = NULL;
    int feedback_buffer_size;
    u8 *addr = NULL;
    int offset;
    u32 tsid;
    int ret;

    message = container_of(work, struct devdrv_mailbox_message, wait_work);
    cce_ctrl = message->cce_ctrl;
    tsid = cce_ctrl->tsid;

    offset = message->feedback_count * DEVDRV_MAILBOX_PAYLOAD_LENGTH;
    feedback_buffer_size = message->feedback_num * DEVDRV_MAILBOX_PAYLOAD_LENGTH;
    if ((offset < 0) || (offset >= feedback_buffer_size)) {
        devdrv_drv_err("offset invalid %d.\n", offset);
        return;
    }

    addr = &message->feedback_buffer[offset];
    ret = memcpy_s(addr, feedback_buffer_size - offset, message->mailbox->receive_sram, DEVDRV_MAILBOX_PAYLOAD_LENGTH);
    if (ret) {
        devdrv_drv_err("copy to addr failed.\n");
        return;
    }
    message->feedback_count++;
    devdrv_mailbox_trigger_irq(message->dev_info, tsid, DEVDRV_MAILBOX_READ_DONE_IRQ);

    if (message->feedback_count < message->feedback_num)
        return;

    spin_lock(&message->mailbox->wait_queue.spinlock);
    list_del(&message->wait_list_node);
    spin_unlock(&message->mailbox->wait_queue.spinlock);

    if (message->abandon == DEVDRV_MAILBOX_RECYCLE_MESSAGE) {
        kfree(message->message_payload);
        message->message_payload = NULL;
        if (message->feedback_buffer != NULL) {
            kfree(message->feedback_buffer);
            message->feedback_buffer = NULL;
        }

        kfree(message);
        message = NULL;
        return;
    }

    if (message->message_type == DEVDRV_MAILBOX_SYNC_MESSAGE) {
        up(&message->wait);
    } else if (message->message_type == DEVDRV_MAILBOX_CONTEXT_ASYNC_MESSAGE) {
        /* user process, wake up app by semaphore, add message into context's wait queue */
        spin_lock(&message->cce_context->ts_context[tsid].mailbox_wait_spinlock);
        list_add_tail(&message->wait_list_node, &message->cce_context->ts_context[tsid].message_list_header);
        spin_unlock(&message->cce_context->ts_context[tsid].mailbox_wait_spinlock);
        up(&message->cce_context->ts_context[tsid].mailbox_wait);
    } else {
        /* kernel process */
        if (message->callback != NULL)
            message->callback(message);
    }
}

/*
 * try to send message
 * if mailbox is free, add message into send queue, write message payload into sram,
 *	create doorbell interrupt to inform TS
 * else if mailbox is busy, add message into send queue, return
 *
 * when add message into send queue, must make the new message be the last valid message
 *	Fist In First Out
 *
 * return:
 * 0  for succ
 * negative for error
 */
STATIC void devdrv_mailbox_try_to_send(struct devdrv_mailbox *mailbox, struct devdrv_mailbox_message *message_info)
{
    INIT_WORK(&message_info->send_work, devdrv_mailbox_ack_work);

    spin_lock(&mailbox->send_queue.spinlock);
    /* add into send queue */
    list_add_tail(&message_info->send_list_node, &mailbox->send_queue.list_header);

    /* if mailbox is free, send this message immediately */
    if (mailbox->send_queue.status == DEVDRV_MAILBOX_FREE) {
        mailbox->send_queue.status = DEVDRV_MAILBOX_BUSY;
        /* copy to sram   same name function but realize diff */
        devdrv_mailbox_data_send(message_info->dev_info, mailbox, message_info);
    }
    spin_unlock(&mailbox->send_queue.spinlock);
}

STATIC void devdrv_mailbox_is_sent(struct devdrv_mailbox_message *message)
{
    struct devdrv_mailbox_message *next_message = NULL;
    if (message->is_sent) {
        message->mailbox->send_queue.status = DEVDRV_MAILBOX_BUSY;
        if (!list_empty_careful(&message->mailbox->send_queue.list_header)) {
            next_message = list_first_entry(&message->mailbox->send_queue.list_header, struct devdrv_mailbox_message,
                                            send_list_node);
            devdrv_mailbox_data_send(message->dev_info, message->mailbox, next_message);
        } else {
            message->mailbox->send_queue.status = DEVDRV_MAILBOX_FREE;
        }
    }
}

/*
 * this func is use to judge the status of a sync message transmission is
 * 	successful or fail
 *
 * return: 0 -- ok
 *         1 -- error, caller must not release the message
 * 	  -1 -- error, caller have to release the message
 */
STATIC int devdrv_mailbox_sync_judge(struct devdrv_mailbox *mailbox, struct devdrv_mailbox_message *message, int ret)
{
    struct list_head *pos = NULL, *n = NULL;
    /* semaphore is up */
    if ((ret == 0) && (message->abandon == DEVDRV_MAILBOX_VALID_MESSAGE))
        /* return 0 tell caller message is sent succ, and to release message */
        return 0;

    /* mailbox exited */
    if ((ret == 0) && (message->abandon == DEVDRV_MAILBOX_RECYCLE_MESSAGE))
        /* return -1 tell caller to release message */
        return -EINVAL;

    /* semaphore is up because ctrl+c or other interruptible conditions */
    spin_lock(&mailbox->send_queue.spinlock);
    spin_lock(&mailbox->wait_queue.spinlock);

    message->abandon = DEVDRV_MAILBOX_RECYCLE_MESSAGE;
    if (message->feedback == DEVDRV_MAILBOX_NEED_FEEDBACK)
        list_del(&message->wait_list_node);

    if (!list_empty_careful(&mailbox->send_queue.list_header)) {
        list_for_each_safe(pos, n, &mailbox->send_queue.list_header)
        {
            if (pos == &message->send_list_node) {
                list_del(&message->send_list_node);
                break;
            }
        }
    }

    devdrv_mailbox_is_sent(message);

    spin_unlock(&mailbox->send_queue.spinlock);
    spin_unlock(&mailbox->wait_queue.spinlock);

    /* return -1 tell caller to release message */
    return -EFAULT;
}

STATIC int devdrv_mailbox_add_message_check(struct devdrv_mailbox *mailbox, struct devdrv_mailbox_message *message_info,
                                            int *result)
{
    if ((mailbox == NULL) || (message_info == NULL) || (message_info->dev_info == NULL) || (result == NULL)) {
        devdrv_drv_err("invalid input argument.\n");
        return -EINVAL;
    }

    if (!mailbox->working) {
        devdrv_drv_err("[dev_id = %u]:mailbox not working.\n", message_info->dev_info->dev_id);
        return -EINVAL;
    }

    if (message_info->message_length > DEVDRV_MAILBOX_PAYLOAD_LENGTH) {
        devdrv_drv_err("[dev_id = %u]:invalid input argument.\n", message_info->dev_info->dev_id);
        return -EINVAL;
    }
    return 0;
}
/*
 * block or nonblock depend on input message's field: sync_type
 * return: 0 -- ok
 *         1 -- error, caller must not release the message
 * 	  -1 -- error, caller have to release the message
 */
STATIC int devdrv_mailbox_message_send(struct devdrv_mailbox *mailbox, struct devdrv_mailbox_message *message,
                                       int *result)
{
    struct devdrv_mailbox_message_header *header = NULL;
    u64 jiffy;
    int ret;

    ret = devdrv_mailbox_add_message_check(mailbox, message, result);
    if (ret)
        return ret;

    message->process_result = 0;
    message->is_sent = 0;

    header = (struct devdrv_mailbox_message_header *)message->message_payload;
    header->result = 0;
    header->valid = DEVDRV_MAILBOX_MESSAGE_VALID;

    if (message->sync_type == DEVDRV_MAILBOX_SYNC) {
        sema_init(&message->wait, 0);
        message->message_type = DEVDRV_MAILBOX_SYNC_MESSAGE;
    } else if (message->cce_context != NULL) {
        message->message_type = DEVDRV_MAILBOX_CONTEXT_ASYNC_MESSAGE;
    } else {
        message->message_type = DEVDRV_MAILBOX_KERNEL_ASYNC_MESSAGE;
    }

    /* add into wait queue if needed */
    if (message->feedback == DEVDRV_MAILBOX_NEED_FEEDBACK) {
        if (message->feedback_buffer == NULL) {
            devdrv_drv_err("feedback_buffer must alloc before use this func.\n");
            return -ENOMEM;
        }
        message->feedback_count = 0;

        INIT_WORK(&message->wait_work, &devdrv_mailbox_feedback_work);

        spin_lock(&mailbox->wait_queue.spinlock);
        list_add_tail(&message->wait_list_node, &mailbox->wait_queue.list_header);
        spin_unlock(&mailbox->wait_queue.spinlock);
    }

    devdrv_mailbox_try_to_send(mailbox, message);

    jiffy = 0;

    if (message->sync_type == DEVDRV_MAILBOX_SYNC) {
        jiffy = msecs_to_jiffies(DEVDRV_MAILBOX_SEMA_TIMEOUT_SECOND * 1000);
        mutex_lock(&mailbox->wait_mutex);
        ret = down_timeout(&message->wait, jiffy);
        if (ret) {
            devdrv_drv_err("mailbox down timeout. ret = %d\n", ret);
            mutex_unlock(&mailbox->wait_mutex);
            goto wait_complete;
        }
        down(&message->wait);
        mutex_unlock(&mailbox->wait_mutex);
    wait_complete:

        ret = devdrv_mailbox_sync_judge(mailbox, message, ret);
        if (ret) {
            devdrv_drv_err("devdrv_mailbox_sync_judge failed.\n");
            return ret;
        } else {
            *result = message->process_result;
            return 0;
        }
    }

    return 0;
}

int devdrv_mailbox_kernel_sync_no_feedback(struct devdrv_mailbox *mailbox, const u8 *buf, u32 len, int *result)
{
    struct devdrv_mailbox_message *message = NULL;
    struct devdrv_cce_ctrl *cce_ctrl = NULL;
    u32 tsid;
    int ret;

    if ((mailbox == NULL) || (buf == NULL) || (len < sizeof(struct devdrv_mailbox_message_header)) ||
        (len > DEVDRV_MAILBOX_PAYLOAD_LENGTH) || (result == NULL)) {
        devdrv_drv_err("input argument invalid.\n");
        return -EINVAL;
    }

    cce_ctrl = container_of(mailbox, struct devdrv_cce_ctrl, mailbox);
    tsid = cce_ctrl->tsid;
    if (devdrv_is_ts_work(&cce_ctrl->dev_info->ts_mng[tsid]) == 0) {
        devdrv_drv_err("[dev_id=%u]:ts is unavailable.\n", cce_ctrl->devid);
        return -EINVAL;
    }

    message = kzalloc(sizeof(struct devdrv_mailbox_message), GFP_KERNEL);
    if (message == NULL) {
        devdrv_drv_err("[dev_id=%u]:kmalloc failed.\n", cce_ctrl->devid);
        return -ENOMEM;
    }

    message->message_payload = kzalloc(DEVDRV_MAILBOX_PAYLOAD_LENGTH, GFP_KERNEL);
    if (message->message_payload == NULL) {
        kfree(message);
        message = NULL;
        devdrv_drv_err("[dev_id=%u]:kmalloc failed.\n", cce_ctrl->devid);
        return -ENOMEM;
    }

    ret = memcpy_s(message->message_payload, DEVDRV_MAILBOX_PAYLOAD_LENGTH, buf, len);
    if (ret) {
        devdrv_drv_err("[dev_id=%u]:memcpy_s failed, ret = %d.\n", cce_ctrl->devid, ret);
        kfree(message->message_payload);
        message->message_payload = NULL;
        kfree(message);
        message = NULL;
        return ret;
    }

    message->message_length = DEVDRV_MAILBOX_PAYLOAD_LENGTH;
    message->feedback = DEVDRV_MAILBOX_NO_FEEDBACK;
    message->feedback_num = 0;
    message->feedback_buffer = NULL;
    message->feedback_count = 0;
    message->process_result = 0;
    message->sync_type = DEVDRV_MAILBOX_SYNC;
    message->cmd_type = 0;
    message->message_index = 0;
    message->message_pid = 0;
    message->callback = NULL;
    message->mailbox = mailbox;
    message->cce_context = NULL;
    message->dev_info = cce_ctrl->dev_info;
    message->cce_ctrl = cce_ctrl;
    message->abandon = DEVDRV_MAILBOX_VALID_MESSAGE;

    ret = devdrv_mailbox_message_send(mailbox, message, result);
    if (ret) {
        devdrv_drv_err("[dev_id=%u]:devdrv_mailbox_message_send failed.\n", cce_ctrl->devid);
        ret = -1;
    } else {
        ret = 0;
    }

    kfree(message->message_payload);
    message->message_payload = NULL;
    kfree(message);
    message = NULL;

    return ret;
}
EXPORT_SYMBOL(devdrv_mailbox_kernel_sync_no_feedback);

/* interrupt upper half for mailbox ack and feedback */
irqreturn_t devdrv_mailbox_ack_irq(int irq, void *data)
{
    struct devdrv_mailbox_message *message = NULL;
    struct devdrv_mailbox *mailbox = NULL;
    unsigned long flags;

    local_irq_save(flags);
    mailbox = (struct devdrv_mailbox *)data;

    if (list_empty_careful(&mailbox->send_queue.list_header)) {
        local_irq_restore(flags);
        return IRQ_HANDLED;
    }

    message = list_first_entry(&mailbox->send_queue.list_header, struct devdrv_mailbox_message, send_list_node);

    queue_work(mailbox->send_queue.wq, &message->send_work);
    if (message->sync_type == DEVDRV_MAILBOX_SYNC) {
        up(&message->wait);
    }
    local_irq_restore(flags);

    return IRQ_HANDLED;
}

irqreturn_t devdrv_mailbox_feedback_irq(int irq, void *data)
{
    struct devdrv_mailbox_message *message = NULL;
    struct devdrv_mailbox *mailbox = NULL;
    unsigned long flags;

    local_irq_save(flags);
    mailbox = (struct devdrv_mailbox *)data;

    if (list_empty_careful(&mailbox->wait_queue.list_header)) {
        local_irq_restore(flags);
        return IRQ_HANDLED;
    }

    message = list_first_entry(&mailbox->wait_queue.list_header, struct devdrv_mailbox_message, wait_list_node);

    queue_work(mailbox->wait_queue.wq, &message->wait_work);
    local_irq_restore(flags);

    return IRQ_HANDLED;
}

int devdrv_send_hwts_addr_to_ts(u32 dev_id, u32 tsid, u32 host_id)
{
    struct devdrv_info *dev_info = NULL;
    struct hwts_addr_info data;
    int result = 0;
    int ret;

    dev_info = devdrv_manager_get_devdrv_info(dev_id);
    if ((dev_info == NULL)  || (tsid >= DEVDRV_MAX_TS_NUM) ||
        (dev_info->ts_mng[0].ts_work_status_shadow != DEVDRV_TS_WORK)) {
        devdrv_drv_err("devid(%u), tsid(%u), dev_info(%pK).\n", dev_id, tsid, dev_info);
        return -EINVAL;
    }

    data.header.valid = DEVDRV_MAILBOX_MESSAGE_VALID;
    data.header.cmd_type = DEVDRV_MAILBOX_SEND_HWTS_INFO;
    data.header.result = 0;
    data.local_devid = dev_id;
    data.host_devid = host_id;

    ret = devdrv_mailbox_kernel_sync_no_feedback(&dev_info->cce_ctrl[tsid]->mailbox, (u8 *)&data,
                                                 sizeof(struct hwts_addr_info), &result);
    if (ret || result) {
        devdrv_drv_err("send hwts addr info failed, devid(%d), tsid(%d), ret(%d), result(%d).\n",
            dev_id, tsid, ret, result);
        return -EINVAL;
    }
    return ret;
}

int devdrv_send_stream_info_to_ts(struct devdrv_mailbox *mailbox, u32 stream_id, u32 status)
{
    struct devdrv_mailbox_stream_info data;
    struct devdrv_cce_ctrl *cce_ctrl = NULL;
    int result = 0;
    int ret;

    if ((mailbox == NULL) || (stream_id >= DEVDRV_MAX_STREAM_ID) || (status >= (u32)DEVDRV_MAX_STREAM_STATUS)) {
        devdrv_drv_err("input argument invalid.\n");
        return -EINVAL;
    }
    cce_ctrl = container_of(mailbox, struct devdrv_cce_ctrl, mailbox);
    data.header.valid = DEVDRV_MAILBOX_MESSAGE_VALID;
    data.header.cmd_type = DEVDRV_MAILBOX_FREE_STREAM_ID;
    data.header.result = 0;
    data.stream_info.stream_cnt = 1;
    data.stream_info.stream[0] = stream_id;
    data.stream_info.status[0] = status;
    data.stream_info.plat_type = cce_ctrl->dev_info->plat_type;
    devdrv_drv_info("ts (%d) to stream(%d,%d). sizeof(%u,%u). \n", data.header.cmd_type, stream_id, status,
                    (u32)sizeof(struct devdrv_mailbox_stream_info), (u32)cce_ctrl->dev_info->plat_type);
    ret = devdrv_mailbox_kernel_sync_no_feedback(mailbox, (u8 *)&data, sizeof(struct devdrv_mailbox_stream_info),
                                                 &result);
    if (ret || result) {
        devdrv_drv_err("[dev_id=%u]:send stream info failed, ret = %d, result = %d.\n", cce_ctrl->devid, ret, result);
        return -EINVAL;
    }
    return ret;
}
EXPORT_SYMBOL(devdrv_send_stream_info_to_ts);

int devdrv_send_recycle_info_to_ts(u32 dev_id, u32 tsid, int pid, int *result)
{
    struct devdrv_info *dev_info = NULL;
    struct recycle_service_info data;
    int ret;

    dev_info = devdrv_manager_get_devdrv_info(dev_id);
    if ((dev_info == NULL)  || (tsid >= DEVDRV_MAX_TS_NUM) ||
        (dev_info->ts_mng[0].ts_work_status_shadow != DEVDRV_TS_WORK)) {
        devdrv_drv_err("devid(%u), tsid(%u), dev_info(%pK).\n", dev_id, tsid, dev_info);
        return -EINVAL;
    }

    data.header.valid = DEVDRV_MAILBOX_MESSAGE_VALID;
    data.header.cmd_type = DEVDRV_MAILBOX_RECYCLE_PID_ID;
    data.header.result = 0;
    data.pid_info.app_cnt = 1;
    data.pid_info.pid[0] = pid;
    data.pid_info.plat_type = dev_info->plat_type;

    devdrv_drv_info("devid[%d] inform ts (%d) to recycle pid %d. sizeof(%u,%u). \n", dev_id, data.header.cmd_type, pid,
                    (u32)sizeof(struct recycle_service_info), (u32)dev_info->plat_type);

    ret = devdrv_mailbox_kernel_sync_no_feedback(&dev_info->cce_ctrl[tsid]->mailbox, (u8 *)&data,
                                                 sizeof(struct recycle_service_info), result);
    if (ret || *result) {
        devdrv_drv_err("[dev_id = %u]:send recycle info failed,devid[%d], ret = %d, result = %d\n", dev_info->dev_id,
                       dev_id, ret, *result);
        return -EINVAL;
    }

    return ret;
}
EXPORT_SYMBOL(devdrv_send_recycle_info_to_ts);

int devdrv_send_rdmainfo_to_ts(u32 dev_id, const u8 *buf, u32 len, int *result)
{
    struct devdrv_info *dev_info = NULL;
    struct rdam_info data;
    int ret;

    if ((len > MAX_RDMA_INFO_LEN) || (buf == NULL) || (result == NULL)) {
        devdrv_drv_err("[dev_id=%u]:the length(%d) of buf out of range(%d), buf = %pK, result = %pK.\n", dev_id, len,
                       MAX_RDMA_INFO_LEN, buf, result);
        return -EINVAL;
    }

    dev_info = devdrv_manager_get_devdrv_info(dev_id);
    if ((dev_info == NULL) || (dev_info->ts_mng[0].ts_work_status_shadow != DEVDRV_TS_WORK)) {
        devdrv_drv_err("[dev_id=%u]:dev_info is null or ts is not working.\n", dev_id);
        return -EINVAL;
    }

    data.header.valid = DEVDRV_MAILBOX_MESSAGE_VALID;
    data.header.cmd_type = DEVDRV_MAILBOX_SEND_RDMA_INFO;
    data.header.result = 0;
    ret = memcpy_s(data.buf, sizeof(data.buf), buf, len);
    if (ret) {
        devdrv_drv_err("[dev_id=%u]:copy from buf failed, ret = %d. \n", dev_id, ret);
        return -EINVAL;
    }

    ret = devdrv_mailbox_kernel_sync_no_feedback(&dev_info->cce_ctrl[0]->mailbox, (u8 *)&data,
                                                 sizeof(struct devdrv_mailbox_message_header) + len, result);
    if (ret || *result) {
        devdrv_drv_err("send rdma info failed,devid[%d], ret = %d, result = %d\n", dev_id, ret, *result);
        return -EINVAL;
    }

    return ret;
}
EXPORT_SYMBOL(devdrv_send_rdmainfo_to_ts);

/* send devmm first addr of share memory to ts */
int devdrv_send_share_memery_info_to_ts(u32 dev_id, u64 first_addr, u32 size)
{
    struct devdrv_info *dev_info = NULL;
    struct share_memory_info data;
    int result = 0;
    int ret;

    dev_info = devdrv_manager_get_devdrv_info(dev_id);
    if (dev_info == NULL || dev_info->ts_mng[0].ts_work_status_shadow != DEVDRV_TS_WORK) {
        devdrv_drv_err("[dev_id=%u]:dev_info is null or ts is not working.\n", dev_id);
        return -EINVAL;
    }

    data.header.valid = DEVDRV_MAILBOX_MESSAGE_VALID;
    data.header.cmd_type = DEVDRV_MAILBOX_SEND_SHARE_MEMORY_INFO;
    data.header.result = 0;
    data.first_addr = first_addr;
    data.size = size;

    ret = devdrv_mailbox_kernel_sync_no_feedback(&dev_info->cce_ctrl[0]->mailbox, (u8 *)&data,
                                                 sizeof(struct share_memory_info), &result);
    if (ret || result) {
        devdrv_drv_err("send rdma info failed,devid[%d], ret = %d, result = %d\n", dev_id, ret, result);
        return -EINVAL;
    }

    return ret;
}
EXPORT_SYMBOL(devdrv_send_share_memery_info_to_ts);

