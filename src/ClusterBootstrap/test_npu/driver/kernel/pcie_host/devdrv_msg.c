/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2012-2019. All rights reserved.
 * Description: Host Msg
 * Author: Huawei Technologies Co., Ltd.
 * Create: 2018/8/18
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

#include <linux/kernel.h>
#include <linux/delay.h>
#include <linux/interrupt.h>
#include <linux/pci.h>
#include <linux/sched.h>
#include <linux/version.h>

#include "devdrv_dma.h"
#include "devdrv_ctrl.h"
#include "devdrv_common_msg.h"
#include "devdrv_msg.h"
#include "devdrv_pci.h"
#include "devdrv_msg_def.h"
#include "devdrv_util.h"

void devdrv_set_device_status(struct devdrv_pci_ctrl *pci_ctrl, u32 status)
{
    pci_ctrl->device_status = status;
}

u32 devdrv_get_device_status(struct devdrv_msg_chan *msg_chan)
{
    return msg_chan->msg_dev->pci_ctrl->device_status;
}

u32 devdrv_msg_alloc_slave_mem(struct devdrv_msg_dev *msg_dev, u32 len)
{
    struct list_head *pos = NULL;
    struct list_head *n = NULL;
    struct devdrv_msg_slave_mem_node *node = NULL;
    u32 offset = 0;

    len = roundup(len, DEVDRV_MSG_QUEUE_MEM_ALIGN);

    mutex_lock(&msg_dev->mutex);

    /* alloc memory from the allocated queue first */
    if (!list_empty_careful(&msg_dev->slave_mem_list)) {
        list_for_each_safe(pos, n, &msg_dev->slave_mem_list)
        {
            node = list_entry(pos, struct devdrv_msg_slave_mem_node, list);
            if (node->mem.len == len) {
                devdrv_debug("dev %d reuse len 0x%x\r\n", msg_dev->pci_ctrl->dev_id, len);
                offset = node->mem.offset;
                list_del(&node->list);
                kfree(node);
                node = NULL;
                break;
            }
        }
    }

    /* alloc memory for the first time */
    if (offset == 0) {
        if (len <= msg_dev->slave_mem.len) {
            offset = msg_dev->slave_mem.offset;
            msg_dev->slave_mem.offset += len;
            msg_dev->slave_mem.len -= len;
            devdrv_debug("dev %d slave mem alloc 0x%x remain 0x%x\r\n",
                msg_dev->pci_ctrl->dev_id, len, msg_dev->slave_mem.len);
        } else {
            devdrv_err("dev %d slave mem is used up, alloc 0x%x remain 0x%x\r\n",
                msg_dev->pci_ctrl->dev_id, len, msg_dev->slave_mem.len);
        }
    }

    mutex_unlock(&msg_dev->mutex);

    return offset;
}

void devdrv_msg_free_slave_mem(struct devdrv_msg_dev *msg_dev, u32 offset, u32 len)
{
    struct devdrv_msg_slave_mem_node *node = NULL;

    len = roundup(len, DEVDRV_MSG_QUEUE_MEM_ALIGN);

    node = (struct devdrv_msg_slave_mem_node *)kzalloc(sizeof(struct devdrv_msg_slave_mem_node), GFP_KERNEL);
    if (node == NULL) {
        devdrv_err("dev %d alloc fail, free slave mem offset %d len %d\r\n", msg_dev->pci_ctrl->dev_id, offset, len);
        return;
    }

    node->mem.offset = offset;
    node->mem.len = len;

    /* Repeat alloc the msg chan after release is a debugging function. It is simple to implement here.
        Join the linked list. Re-take directly from here next time. not support length changes in next time */
    mutex_lock(&msg_dev->mutex);
    list_add(&node->list, &msg_dev->slave_mem_list);
    mutex_unlock(&msg_dev->mutex);
}

STATIC int devdrv_msg_alloc_host_sq(struct devdrv_msg_chan *chan, u32 depth, u32 bd_size)
{
    u32 size = depth * bd_size;

    chan->sq_info.desc_h = dma_zalloc_coherent(chan->msg_dev->dev, size, &chan->sq_info.dma_handle,
                                               GFP_KERNEL | __GFP_DMA);

    if (chan->sq_info.desc_h == NULL) {
        devdrv_err("devid:%u, msg_alloc_sq fail\n", chan->msg_dev->pci_ctrl->dev_id);
        return -ENOMEM;
    }

    chan->sq_info.desc_size = bd_size;
    chan->sq_info.irq_vector = -1;
    chan->sq_info.depth = depth;
    chan->sq_info.head_h = 0;
    chan->sq_info.tail_h = 0;

    return 0;
}

STATIC int devdrv_msg_free_host_sq(struct devdrv_msg_chan *msg_chan)
{
    u32 free_size;

    if (msg_chan->sq_info.desc_h != NULL) {
        free_size = msg_chan->sq_info.desc_size * msg_chan->sq_info.depth;
        dma_free_coherent(msg_chan->msg_dev->dev, free_size, msg_chan->sq_info.desc_h, msg_chan->sq_info.dma_handle);
        msg_chan->sq_info.desc_h = NULL;
    }

    return 0;
}

STATIC int devdrv_msg_alloc_host_cq(struct devdrv_msg_chan *chan, int depth, int bd_size)
{
    u32 alloc_size = depth * bd_size;

    chan->cq_info.desc_h = dma_zalloc_coherent(chan->msg_dev->dev, alloc_size, &chan->cq_info.dma_handle,
                                               GFP_KERNEL | __GFP_DMA);

    if (chan->cq_info.desc_h == NULL) {
        devdrv_err("devid:%u, msg_alloc_cq fail\n", chan->msg_dev->pci_ctrl->dev_id);
        return -ENOMEM;
    }

    chan->cq_info.depth = depth;
    chan->cq_info.desc_size = bd_size;
    chan->cq_info.irq_vector = -1;
    chan->cq_info.head_h = 0;
    chan->cq_info.tail_h = 0;

    return 0;
}

STATIC int devdrv_msg_free_host_cq(struct devdrv_msg_chan *msg_chan)
{
    u32 free_size;

    if (msg_chan->cq_info.desc_h != NULL) {
        free_size = msg_chan->cq_info.desc_size * msg_chan->cq_info.depth;
        dma_free_coherent(msg_chan->msg_dev->dev, free_size, msg_chan->cq_info.desc_h, msg_chan->cq_info.dma_handle);
        msg_chan->cq_info.desc_h = NULL;
    }

    return 0;
}

STATIC int devdrv_msg_alloc_slave_sq(struct devdrv_msg_chan *chan, u32 depth, u32 bd_size)
{
    struct devdrv_msg_dev *msg_dev = NULL;
    u32 queue_size;
    u32 offset;

    msg_dev = chan->msg_dev;

    queue_size = depth * bd_size;
    offset = devdrv_msg_alloc_slave_mem(msg_dev, queue_size);
    if (offset == 0) {
        devdrv_err("devid:%u, queue size %x alloc failed.\n", chan->msg_dev->pci_ctrl->dev_id, queue_size);
        return -ENOMEM;
    }

    chan->sq_info.desc_d = msg_dev->reserve_mem_base + offset;
    chan->sq_info.head_d = 0;
    chan->sq_info.tail_d = 0;
    chan->sq_info.slave_mem_offset = offset;

    return 0;
}

static int devdrv_msg_free_slave_sq(struct devdrv_msg_chan *msg_chan)
{
    u32 queue_size = msg_chan->sq_info.depth * msg_chan->sq_info.desc_size;

    msg_chan->sq_info.desc_d = 0;

    devdrv_msg_free_slave_mem(msg_chan->msg_dev, msg_chan->sq_info.slave_mem_offset, queue_size);

    return 0;
}

STATIC int devdrv_msg_alloc_slave_cq(struct devdrv_msg_chan *chan, u32 depth, u32 bd_size)
{
    struct devdrv_msg_dev *msg_dev = NULL;
    u32 queue_size;
    u32 offset;

    msg_dev = chan->msg_dev;

    queue_size = depth * bd_size;
    offset = devdrv_msg_alloc_slave_mem(msg_dev, queue_size);
    if (offset == 0) {
        devdrv_err("devid:%u, queue size %d alloc failed.\n", msg_dev->pci_ctrl->dev_id, queue_size);
        return -ENOMEM;
    }

    chan->cq_info.desc_d = msg_dev->reserve_mem_base + offset;
    chan->cq_info.head_d = 0;
    chan->cq_info.tail_d = 0;
    chan->cq_info.slave_mem_offset = offset;

    return 0;
}

static int devdrv_msg_free_slave_cq(struct devdrv_msg_chan *msg_chan)
{
    u32 queue_size = msg_chan->cq_info.depth * msg_chan->cq_info.desc_size;
    msg_chan->cq_info.desc_d = 0;

    devdrv_msg_free_slave_mem(msg_chan->msg_dev, msg_chan->cq_info.slave_mem_offset, queue_size);

    return 0;
}

STATIC int devdrv_msg_alloc_s_queue(struct devdrv_msg_chan *msg_chan, u32 depth, u32 size)
{
    int ret;

    ret = devdrv_msg_alloc_host_sq(msg_chan, depth, size);
    if (ret) {
        devdrv_err("devdrv_msg_alloc_host_sq failed ret = %d\n", ret);
        return ret;
    }

    ret = devdrv_msg_alloc_slave_sq(msg_chan, depth, size);
    return ret;
}

STATIC int devdrv_msg_alloc_c_queue(struct devdrv_msg_chan *msg_chan, u32 depth, u32 size)
{
    int ret;

    ret = devdrv_msg_alloc_host_cq(msg_chan, depth, size);
    if (ret) {
        devdrv_err("devdrv_msg_alloc_c_queue failed ret = %d\n", ret);
        return ret;
    }

    ret = devdrv_msg_alloc_slave_cq(msg_chan, depth, size);
    return ret;
}

STATIC int devdrv_msg_free_sq(struct devdrv_msg_chan *msg_chan)
{
    (void)devdrv_msg_free_host_sq(msg_chan);

    (void)devdrv_msg_free_slave_sq(msg_chan);
    return 0;
}

STATIC int devdrv_msg_free_cq(struct devdrv_msg_chan *msg_chan)
{
    (void)devdrv_msg_free_host_cq(msg_chan);

    (void)devdrv_msg_free_slave_cq(msg_chan);
    return 0;
}

/* write doorbell to notify dev */
void devdrv_msg_ring_doorbell(void *msg_chan)
{
    struct devdrv_msg_chan *chan = (struct devdrv_msg_chan *)msg_chan;
    if (chan == NULL) {
        devdrv_err("devdrv_msg_ring_doorbell: msg_chan is null\n");
        return;
    }
    devdrv_set_sq_doorbell(chan->io_base, chan->sq_info.tail_d);
}
EXPORT_SYMBOL(devdrv_msg_ring_doorbell);

void devdrv_msg_ring_cq_doorbell(void *msg_chan)
{
    struct devdrv_msg_chan *chan = (struct devdrv_msg_chan *)msg_chan;
    if (chan == NULL) {
        devdrv_err("devdrv_msg_ring_cq_doorbell: msg_chan is null\n");
        return;
    }
    devdrv_set_cq_doorbell(chan->io_base, chan->cq_info.head_d);
}
EXPORT_SYMBOL(devdrv_msg_ring_cq_doorbell);

/* devid */
int devdrv_get_msg_chan_devid(void *msg_chan)
{
    struct devdrv_msg_chan *chan = (struct devdrv_msg_chan *)msg_chan;
    if (chan == NULL) {
        devdrv_err("devdrv_get_msg_chan_devid: msg_chan is null\n");
        return -EINVAL;
    }
    return chan->msg_dev->pci_ctrl->dev_id;
}
EXPORT_SYMBOL(devdrv_get_msg_chan_devid);

/* priv */
int devdrv_set_msg_chan_priv(void *msg_chan, void *priv)
{
    struct devdrv_msg_chan *chan = (struct devdrv_msg_chan *)msg_chan;
    if (chan == NULL) {
        devdrv_err("set_msg_chan_priv: msg_chan is null\n");
        return -EINVAL;
    }
    chan->priv = priv;
    return 0;
}
EXPORT_SYMBOL(devdrv_set_msg_chan_priv);

void *devdrv_get_msg_chan_priv(void *msg_chan)
{
    struct devdrv_msg_chan *chan = (struct devdrv_msg_chan *)msg_chan;
    if (chan == NULL) {
        devdrv_err("get_msg_chan_priv: msg_chan is null\n");
        return NULL;
    }
    return chan->priv;
}
EXPORT_SYMBOL(devdrv_get_msg_chan_priv);

/* host sq */
void *devdrv_get_msg_chan_host_sq_head(void *msg_chan, u32 *head)
{
    struct devdrv_msg_chan *chan = (struct devdrv_msg_chan *)msg_chan;
    u64 offset;

    if ((chan == NULL) || (head == NULL)) {
        devdrv_err("devdrv_get_msg_chan_host_sq_head: msg_chan is null\n");
        return NULL;
    }
    *head = chan->sq_info.head_h;
    offset = (u64)chan->sq_info.head_h * chan->sq_info.desc_size;
    return (void *)((char *)chan->sq_info.desc_h + offset);
}
EXPORT_SYMBOL(devdrv_get_msg_chan_host_sq_head);

void devdrv_move_msg_chan_host_sq_head(void *msg_chan)
{
    struct devdrv_msg_chan *chan = (struct devdrv_msg_chan *)msg_chan;
    if (chan == NULL) {
        devdrv_err("devdrv_move_msg_chan_host_sq_head: msg_chan is null\n");
        return;
    }
    if (chan->sq_info.depth != 0) {
        chan->sq_info.head_h = (chan->sq_info.head_h + 1) % chan->sq_info.depth;
    }
}
EXPORT_SYMBOL(devdrv_move_msg_chan_host_sq_head);

/* host cq */
void *devdrv_get_msg_chan_host_cq_head(void *msg_chan)
{
    struct devdrv_msg_chan *chan = (struct devdrv_msg_chan *)msg_chan;
    u64 offset;

    if (chan == NULL) {
        devdrv_err("devdrv_get_msg_chan_host_cq_head: msg_chan is null\n");
        return NULL;
    }
    offset = (u64)chan->cq_info.head_h * chan->cq_info.desc_size;
    return (void *)((char *)chan->cq_info.desc_h + offset);
}
EXPORT_SYMBOL(devdrv_get_msg_chan_host_cq_head);

void devdrv_move_msg_chan_host_cq_head(void *msg_chan)
{
    struct devdrv_msg_chan *chan = (struct devdrv_msg_chan *)msg_chan;
    if (chan == NULL) {
        devdrv_err("devdrv_move_msg_chan_host_cq_head: msg_chan is null\n");
        return;
    }
    if (chan->cq_info.depth != 0) {
        chan->cq_info.head_h = (chan->cq_info.head_h + 1) % chan->cq_info.depth;
    }
}
EXPORT_SYMBOL(devdrv_move_msg_chan_host_cq_head);

/* slave sq */
void devdrv_set_msg_chan_slave_sq_head(void *msg_chan, u32 head)
{
    struct devdrv_msg_chan *chan = (struct devdrv_msg_chan *)msg_chan;
    if (chan == NULL) {
        devdrv_err("devdrv_set_msg_chan_slave_sq_head: msg_chan is null\n");
        return;
    }
    chan->sq_info.head_d = head;
}
EXPORT_SYMBOL(devdrv_set_msg_chan_slave_sq_head);

void *devdrv_get_msg_chan_slave_sq_tail(void *msg_chan, u32 *tail)
{
    struct devdrv_msg_chan *chan = (struct devdrv_msg_chan *)msg_chan;
    u64 offset;

    if ((chan == NULL) || (tail == NULL)) {
        devdrv_err("devdrv_get_msg_chan_slave_sq_tail: msg_chan or tail is null\n");
        return NULL;
    }
    *tail = chan->sq_info.tail_d;
    offset = (u64)chan->sq_info.tail_d * chan->sq_info.desc_size;
    return (void *)((char *)chan->sq_info.desc_d + offset);
}
EXPORT_SYMBOL(devdrv_get_msg_chan_slave_sq_tail);

void devdrv_move_msg_chan_slave_sq_tail(void *msg_chan)
{
    struct devdrv_msg_chan *chan = (struct devdrv_msg_chan *)msg_chan;
    if (chan == NULL) {
        devdrv_err("devdrv_move_msg_chan_slave_sq_tail: msg_chan is null\n");
        return;
    }
    if (chan->sq_info.depth != 0) {
        chan->sq_info.tail_d = (chan->sq_info.tail_d + 1) % chan->sq_info.depth;
    }
}
EXPORT_SYMBOL(devdrv_move_msg_chan_slave_sq_tail);

bool devdrv_msg_chan_slave_sq_full_check(void *msg_chan)
{
    struct devdrv_msg_chan *chan = (struct devdrv_msg_chan *)msg_chan;
    if (chan == NULL) {
        devdrv_err("devdrv_msg_chan_slave_sq_full_check: msg_chan is null\n");
        return true;
    }
    if ((chan->sq_info.depth != 0) && (((chan->sq_info.tail_d + 1) % chan->sq_info.depth) == chan->sq_info.head_d)) {
        return true;
    } else {
        return false;
    }
}
EXPORT_SYMBOL(devdrv_msg_chan_slave_sq_full_check);

/* slave cq */
void *devdrv_get_msg_chan_slave_cq_tail(void *msg_chan)
{
    struct devdrv_msg_chan *chan = (struct devdrv_msg_chan *)msg_chan;
    u64 offset;

    if (chan == NULL) {
        devdrv_err("devdrv_get_msg_chan_slave_cq_tail: msg_chan is null\n");
        return NULL;
    }
    offset = (u64)chan->cq_info.tail_d * chan->cq_info.desc_size;
    return (void *)((char *)chan->cq_info.desc_d + offset);
}
EXPORT_SYMBOL(devdrv_get_msg_chan_slave_cq_tail);

void devdrv_move_msg_chan_slave_cq_tail(void *msg_chan)
{
    struct devdrv_msg_chan *chan = (struct devdrv_msg_chan *)msg_chan;
    if (chan == NULL) {
        devdrv_err("devdrv_move_msg_chan_slave_cq_tail: msg_chan is null\n");
        return;
    }
    if (chan->cq_info.depth != 0) {
        chan->cq_info.tail_d = (chan->cq_info.tail_d + 1) % chan->cq_info.depth;
    }
}
EXPORT_SYMBOL(devdrv_move_msg_chan_slave_cq_tail);

STATIC bool devdrv_judge_chan_invalid_by_level(u32 level, u32 chan_id)
{
    if (level == DEVDRV_MSG_CHAN_LEVEL_LOW) {
        if ((chan_id % AGENTDRV_NVME_DB_IRQ_STRDE) >= AGENTDRV_NVME_LOW_LEVEL_DB_IRQ_NUM) {
            return true;
        }
    } else {
        if ((chan_id % AGENTDRV_NVME_DB_IRQ_STRDE) < AGENTDRV_NVME_LOW_LEVEL_DB_IRQ_NUM) {
            return true;
        }
    }
    return false;
}

/* alloc a msg_chan node from msg_dev struct */
STATIC struct devdrv_msg_chan *devdrv_alloc_msg_chan(struct devdrv_msg_dev *msg_dev, u32 level)
{
    u32 i;
    int level_check = DEVDRV_ENABLE;
    struct devdrv_msg_chan *msg_chan = NULL;

    if (devdrv_get_chip_type() == HISI_MINI) {
        (void)level;
        level_check = DEVDRV_DISABLE;
    }

retry:

    mutex_lock(&msg_dev->mutex);
    for (i = 0; i < msg_dev->chan_cnt; i++) {
        if (level_check == DEVDRV_ENABLE) {
            if (devdrv_judge_chan_invalid_by_level(level, i) == true) {
                continue;
            }
        }
        if (msg_dev->msg_chan[i].status == DEVDRV_DISABLE) {
            msg_dev->msg_chan[i].status = DEVDRV_ENABLE;
            msg_chan = &msg_dev->msg_chan[i];
            break;
        }
    }
    mutex_unlock(&msg_dev->mutex);
    if (msg_chan == NULL) {
        if (level_check == DEVDRV_ENABLE) {
            level_check = DEVDRV_DISABLE;
            devdrv_info("dev_id %d level %d alloc msg chan failed, try again\n", msg_dev->pci_ctrl->dev_id, level);
            goto retry;
        }

        devdrv_err("dev_id %d level %d alloc msg chan failed\n", msg_dev->pci_ctrl->dev_id, level);
    } else {
        devdrv_debug("dev_id %d level %d alloc msg chan id %d\n", msg_dev->pci_ctrl->dev_id, level, msg_chan->chan_id);
    }

    return msg_chan;
}

int devdrv_device_status_abnormal_check(void *msg_chan)
{
    struct devdrv_msg_chan *msg_chan_tmp = (struct devdrv_msg_chan *)msg_chan;
    if ((msg_chan_tmp == NULL) || (msg_chan_tmp->msg_dev == NULL) || (msg_chan_tmp->msg_dev->pci_ctrl == NULL) ||
        (msg_chan_tmp->msg_dev->pci_ctrl->device_status == DEVDRV_DEVICE_DEAD)) {
        devdrv_err("devdrv_device_status_abnormal_check failed\n");
        return -EINVAL;
    }

    return 0;
}
EXPORT_SYMBOL(devdrv_device_status_abnormal_check);

void devdrv_device_mutex_lock(struct devdrv_msg_chan *msg_chan)
{
    if ((msg_chan != NULL) && (msg_chan->msg_dev != NULL)) {
        mutex_lock(&msg_chan->mutex);
    }
}

void devdrv_device_mutex_unlock(struct devdrv_msg_chan *msg_chan)
{
    if ((msg_chan != NULL) && (msg_chan->msg_dev != NULL)) {
        mutex_unlock(&msg_chan->mutex);
    }
}

void devdrv_non_trans_rx_msg_task_resq_record(struct devdrv_msg_chan *msg_chan)
{
    u32 cost_time;

    cost_time = jiffies_to_msecs(jiffies - msg_chan->stamp);
    if (cost_time > msg_chan->chan_stat.rx_work_max_time) {
        msg_chan->chan_stat.rx_work_max_time = cost_time;
    }
    if (cost_time > DEVDRV_MSG_TIME_VOERFLOW) {
        msg_chan->chan_stat.rx_work_delay_cnt++;
        devdrv_info("msg_type %d host schedule msg work:%u ms\n", msg_chan->msg_type, cost_time);
    }
}

STATIC void devdrv_non_trans_rx_msg_handle(struct devdrv_msg_chan *msg_chan, struct devdrv_non_trans_msg_desc *bd_desc)
{
    enum devdrv_msg_client_type msg_type_tmp = msg_chan->msg_type;
    struct devdrv_non_trans_msg_desc *bd_desc_d = NULL;
    int ret, i;
    int dev_id;
    u64 seq_num;

    dev_id = msg_chan->msg_dev->pci_ctrl->dev_id;
    seq_num = bd_desc->seq_num;
    if (seq_num == msg_chan->seq_num) {
        devdrv_warn("dev_id %d msg_type %d seq_num %lld\n", dev_id, msg_type_tmp, seq_num);
    }
    msg_chan->seq_num = seq_num;
    /* the dma moves the command data and the msix interrupt takes the same pcie path,
       which guarantees that the data has arrived when the interrupt arrives. */
    if ((bd_desc->status == DEVDRV_MSG_CMD_BEGIN) && (msg_chan->rx_msg_process != NULL)) {
        msg_chan->chan_stat.rx_total_cnt++;
        ret = msg_chan->rx_msg_process((void *)msg_chan, bd_desc->data, bd_desc->in_data_len, bd_desc->out_data_len,
                                       &bd_desc->real_out_len);

        if (devdrv_device_status_abnormal_check(msg_chan)) {
            devdrv_info("device is abnormal! msg_type is %d.\n", msg_type_tmp);
            return;
        }

        if ((ret == 0) && (bd_desc->real_out_len <= bd_desc->out_data_len)) {
            bd_desc->status = DEVDRV_MSG_CMD_FINISH_SUCCESS;
            msg_chan->chan_stat.rx_success_cnt++;
        } else if (ret == -EINVAL) {
            msg_chan->chan_stat.rx_para_err++;
            devdrv_warn("dev id %d rx_msg_process: msg_type %d, ret %d, out buf %d, out len %d\n", dev_id,
                        msg_chan->msg_type, ret, bd_desc->out_data_len, bd_desc->real_out_len);
            bd_desc->status = DEVDRV_MSG_CMD_INVALID_PARA;
        } else {
            devdrv_err("dev id %d rx_msg_process: msg_type %d, ret %d, out buf %d, out len %d\n", dev_id,
                       msg_chan->msg_type, ret, bd_desc->out_data_len, bd_desc->real_out_len);
            bd_desc->status = DEVDRV_MSG_CMD_FINISH_FAILED;
        }
    } else {
        devdrv_err("dev id %d desc status:%d\n", dev_id, bd_desc->status);
        bd_desc->status = DEVDRV_MSG_CMD_FINISH_FAILED;
    }
    /* copy the execution result to shared reserved memory pointed to by cq desc_h */
    bd_desc_d = (struct devdrv_non_trans_msg_desc *)msg_chan->cq_info.desc_d;
    if ((bd_desc->status == DEVDRV_MSG_CMD_FINISH_SUCCESS) && (bd_desc->real_out_len > 0)) {
        /* copy data */
        for (i = 0; i < bd_desc->real_out_len; i++) {
            bd_desc_d->data[i] = bd_desc->data[i];
        }

        wmb();
    }

    /* copy msg head, the status field must be placed at the end of the
       structure so that other data is already written when it is in effect */
    bd_desc_d->in_data_len = bd_desc->in_data_len;
    bd_desc_d->out_data_len = bd_desc->out_data_len;
    bd_desc_d->real_out_len = bd_desc->real_out_len;
    bd_desc_d->seq_num = seq_num;
    bd_desc_d->msg_type = bd_desc->msg_type;
    wmb();

    bd_desc_d->status = bd_desc->status;
    return;
}

/* the large lock control of the sending function at the device side only
   has one command executed at the same time, no lock is needed here */
void devdrv_non_trans_rx_msg_task(struct work_struct *p_work)
{
    struct devdrv_msg_chan *msg_chan = container_of(p_work, struct devdrv_msg_chan, rx_work);
    struct devdrv_non_trans_msg_desc *bd_desc = NULL;
    int dev_id;
    enum devdrv_msg_client_type msg_type_tmp = msg_chan->msg_type;
    int timeout = DEVDRV_MSG_WAIT_DMA_FINISH_TIMEOUT;

    devdrv_non_trans_rx_msg_task_resq_record(msg_chan);

    if (devdrv_device_status_abnormal_check(msg_chan)) {
        devdrv_info("device is abnormal! msg_type is %d.\n", msg_type_tmp);
        return;
    }

    dev_id = msg_chan->msg_dev->pci_ctrl->dev_id;

    if (msg_chan->status == DEVDRV_DISABLE) {
        devdrv_err("dev id %d msg type %d rx msg: msg chan %d disable\n", dev_id, msg_type_tmp, msg_chan->chan_id);
        return;
    }

    /* the host side cq buf is used as a buf to receive the command */
    bd_desc = (struct devdrv_non_trans_msg_desc *)msg_chan->cq_info.desc_h;
    /* msi-x maybe has been trigged, but dma data has not been move to ddr */
    while (timeout > 0) {
        if (bd_desc->status == DEVDRV_MSG_CMD_BEGIN) {
            break;
        }
        msleep(1);
        timeout--;
    }

    devdrv_non_trans_rx_msg_handle(msg_chan, bd_desc);

    return;
}

STATIC irqreturn_t devdrv_rx_msg_notify_handler(int irq, void *data)
{
    struct devdrv_msg_chan *msg_chan = (struct devdrv_msg_chan *)data;

    if (msg_chan->status == DEVDRV_DISABLE) {
        devdrv_err("rx msg notify: msg chan %d disable\n", msg_chan->chan_id);
        return IRQ_HANDLED;
    }

    if (devdrv_get_device_status(msg_chan) != DEVDRV_DEVICE_ALIVE) {
        devdrv_err("msg chan %d device is not alive\n", msg_chan->chan_id);
        return IRQ_HANDLED;
    }

    rmb();

    if (msg_chan->rx_msg_notify != NULL)
        msg_chan->rx_msg_notify((void *)msg_chan);

    return IRQ_HANDLED;
}

STATIC irqreturn_t devdrv_tx_fnsh_notify_handler(int irq, void *data)
{
    struct devdrv_msg_chan *msg_chan = (struct devdrv_msg_chan *)data;

    if (msg_chan->status == DEVDRV_DISABLE) {
        devdrv_err("tx msg finish: msg chan %d disable\n", msg_chan->chan_id);
        return IRQ_HANDLED;
    }

    if (devdrv_get_device_status(msg_chan) != DEVDRV_DEVICE_ALIVE) {
        devdrv_err("msg chan %d device is not alive\n", msg_chan->chan_id);
        return IRQ_HANDLED;
    }

    rmb();

    if (msg_chan->tx_finish_notify != NULL)
        msg_chan->tx_finish_notify((void *)msg_chan);

    return IRQ_HANDLED;
}

STATIC irqreturn_t devdrv_wakeup_rx_work(int irq, void *data)
{
    struct devdrv_msg_chan *msg_chan = (struct devdrv_msg_chan *)data;

    if (msg_chan->status == DEVDRV_DISABLE) {
        devdrv_err("msg chan %d disable\n", msg_chan->chan_id);
        return IRQ_HANDLED;
    }

    if (devdrv_get_device_status(msg_chan) != DEVDRV_DEVICE_ALIVE) {
        devdrv_err("msg chan %d device is not alive\n", msg_chan->chan_id);
        return IRQ_HANDLED;
    }

    rmb();

    msg_chan->stamp = jiffies;
    queue_work(msg_chan->msg_dev->work_queue, &msg_chan->rx_work);
    return IRQ_HANDLED;
}

int devdrv_sync_non_trans_status_handle(struct devdrv_msg_chan *msg_chan, int status,
    enum devdrv_common_msg_type msg_type)
{
    struct devdrv_msg_chan_stat *chan_stat = &(msg_chan->chan_stat);
    int ret;

    if (status == DEVDRV_MSG_CMD_FINISH_SUCCESS) {
        ret = 0;
    } else if (status == DEVDRV_MSG_CMD_BEGIN) {
        ret = -ENOSYS;
        chan_stat->tx_timeout_err++;
        devdrv_err("devid:%u, msg_type %d common_type %d, msg send finish status %d,ret %d.\n",
                   msg_chan->msg_dev->pci_ctrl->dev_id, msg_chan->msg_type, msg_type, status, ret);
    } else if (status == DEVDRV_MSG_CMD_FINISH_FAILED) {
        ret = -ETIMEDOUT;
        chan_stat->tx_process_err++;
        devdrv_err("devid:%u, msg_type %d common_type %d, msg send finish status %d,ret %d.\n",
                   msg_chan->msg_dev->pci_ctrl->dev_id, msg_chan->msg_type, msg_type, status, ret);
    } else {
        ret = -ETIMEDOUT;
        chan_stat->tx_invalid_para_err++;
        devdrv_warn("devid:%u, msg_type %d common_type %d, msg send finish status %d,ret %d.\n",
                    msg_chan->msg_dev->pci_ctrl->dev_id, msg_chan->msg_type, msg_type, status, ret);
    }

    return ret;
}

STATIC int devdrv_sync_non_trans_msg_chan_send(struct devdrv_msg_chan *msg_chan, enum devdrv_common_msg_type msg_type,
    struct devdrv_non_trans_msg_send_data_para *data_para)
{
    struct devdrv_non_trans_msg_desc *bd_desc = NULL;
    int timeout;
    u32 status = 0;
    int ret;
    struct devdrv_msg_chan_stat *chan_stat = &(msg_chan->chan_stat);
    u64 seq_num = chan_stat->tx_total_cnt;
    enum devdrv_msg_client_type msg_type_tmp = msg_chan->msg_type;
    u32 *real_out_len = data_para->real_out_len;

msg_retry:
    /* result is in the sq buf(desc_h) on the host side */
    bd_desc = (struct devdrv_non_trans_msg_desc *)msg_chan->sq_info.desc_h;
    bd_desc->status = DEVDRV_MSG_CMD_BEGIN;

    wmb();
    /* inform device */
    devdrv_msg_ring_doorbell((void *)msg_chan);

    /* wait for the device irq set status, when device dead, timeout quickly */
    bd_desc = (struct devdrv_non_trans_msg_desc *)msg_chan->sq_info.desc_d;
    timeout = DEVDRV_MSG_IRQ_TIMEOUT;
    while (timeout > 0) {
        status = bd_desc->status;
        if (status == DEVDRV_MSG_CMD_IRQ_BEGIN) {
            break;
        }
        rmb();
        usleep_range(DEVDRV_MSG_WAIT_MIN_TIME, DEVDRV_MSG_WAIT_MAX_TIME);

        if (devdrv_device_status_abnormal_check(msg_chan)) {
            chan_stat->tx_status_abnormal_err++;
            devdrv_info("device is abnormal! msg_type is %d. common_type is %d\n", msg_type_tmp, msg_type);
            return -ENODEV;
        }

        timeout -= DEVDRV_MSG_WAIT_MIN_TIME;
    }

    if (timeout <= 0) {
        chan_stat->tx_irq_timeout_err++;
        devdrv_err("devid:%u, msg_type %d common_type %d, device irq not resp status %d.\n",
                   msg_chan->msg_dev->pci_ctrl->dev_id, msg_chan->msg_type, msg_type, status);
        return -ENOSYS;
    }

    /* wait for the device to use the dma to move result to the sq buf on the host side */
    bd_desc = (struct devdrv_non_trans_msg_desc *)msg_chan->sq_info.desc_h;
    timeout = DEVDRV_MSG_TIMEOUT - (DEVDRV_MSG_IRQ_TIMEOUT - timeout);
    while (timeout > 0) {
        status = bd_desc->status;
        if (status != DEVDRV_MSG_CMD_BEGIN) {
            break;
        }
        rmb();
        usleep_range(DEVDRV_MSG_WAIT_MIN_TIME, DEVDRV_MSG_WAIT_MAX_TIME);

        if (devdrv_device_status_abnormal_check(msg_chan)) {
            chan_stat->tx_status_abnormal_err++;
            devdrv_info("device is abnormal! msg_type is %d. common_type is %d\n", msg_type_tmp, msg_type);
            return -ENODEV;
        }

        timeout -= DEVDRV_MSG_WAIT_MIN_TIME;
    }
    mb();
    if ((status != DEVDRV_MSG_CMD_BEGIN) && (bd_desc->seq_num != seq_num)) {
        devdrv_warn("devid %d msg_type is %d. common_type is %d num %lld reply num %lld error status %d timeout %d\n",
            msg_chan->msg_dev->pci_ctrl->dev_id, msg_type_tmp, msg_type, seq_num, bd_desc->seq_num, status, timeout);
        goto msg_retry;
    }
    ret = devdrv_sync_non_trans_status_handle(msg_chan, status, msg_type);
    if (ret == 0) {
        *real_out_len = bd_desc->real_out_len;
        if (*real_out_len > data_para->out_data_len) {
            chan_stat->tx_reply_len_check_err++;
            devdrv_err("devid %d msg_type is %d. common_type is %d, real out len %d bigger than out len %d\n",
                msg_chan->msg_dev->pci_ctrl->dev_id, msg_type_tmp, msg_type, *real_out_len, data_para->out_data_len);
            return -EINVAL;
        }
        /* real_out_len could be zero */
        if (*real_out_len > 0) {
            ret = memcpy_s(data_para->data, data_para->out_data_len, (void *)bd_desc->data, *real_out_len);
            if (ret) {
                devdrv_err("memcpy_s failed, ret = %d\n", ret);
                ret = -EINVAL;
            }
            chan_stat->tx_success_cnt++;
        }
    }
    return ret;
}

int devdrv_sync_non_trans_msg_send(struct devdrv_msg_chan *msg_chan, void *data, u32 in_data_len, u32 out_data_len,
                                   u32 *real_out_len, enum devdrv_common_msg_type msg_type)
{
    struct devdrv_non_trans_msg_send_data_para data_para;
    struct devdrv_non_trans_msg_desc *bd_desc = NULL;
    int ret;
    u32 i;
    u64 seq_num;
    struct devdrv_msg_chan_stat *chan_stat = NULL;
    enum devdrv_msg_client_type msg_type_tmp = msg_chan->msg_type;
    u32 max_data_len = msg_chan->sq_info.desc_size - DEVDRV_NON_TRANS_MSG_HEAD_LEN;

    devdrv_device_mutex_lock(msg_chan);

    chan_stat = &(msg_chan->chan_stat);

    chan_stat->tx_total_cnt++;
    seq_num = chan_stat->tx_total_cnt;
    if ((in_data_len > max_data_len) || (out_data_len > max_data_len)) {
        chan_stat->tx_len_check_err++;
        devdrv_device_mutex_unlock(msg_chan);
        devdrv_err("devid:%u, msg_type %d common_type %d, in_data_len %d or out_data_len %d "
                   "is bigger than desc len %d.\n",
                   msg_chan->msg_dev->pci_ctrl->dev_id, msg_chan->msg_type, msg_type,
                   in_data_len, out_data_len, max_data_len);
        return -EINVAL;
    }

    devdrv_debug("msg_type %d, common_type %d, in_data_len %d, out_data_len %d\n", msg_chan->msg_type, msg_type,
                 in_data_len, out_data_len);

    if (devdrv_device_status_abnormal_check(msg_chan)) {
        chan_stat->tx_status_abnormal_err++;
        devdrv_device_mutex_unlock(msg_chan);
        devdrv_info("devid:%u, device is abnormal! msg_type is %d. common_type is %d\n",
                    msg_chan->msg_dev->pci_ctrl->dev_id, msg_type_tmp, msg_type);
        return -ENODEV;
    }
    data_para.data = data;
    data_para.in_data_len = in_data_len;
    data_para.out_data_len = out_data_len;
    data_para.real_out_len = real_out_len;

    /* put messages to send to shared reserved memory pointed to by sq desc_d */
    bd_desc = (struct devdrv_non_trans_msg_desc *)msg_chan->sq_info.desc_d;
    bd_desc->in_data_len = in_data_len;
    bd_desc->out_data_len = out_data_len;
    bd_desc->real_out_len = 0;
    bd_desc->msg_type = msg_type;
    bd_desc->seq_num = seq_num;
    bd_desc->status = DEVDRV_MSG_CMD_BEGIN;

    for (i = 0; i < in_data_len; i++) {
        bd_desc->data[i] = ((char *)data)[i];
    }

    ret = devdrv_sync_non_trans_msg_chan_send(msg_chan, msg_type, &data_para);

    devdrv_device_mutex_unlock(msg_chan);

    return ret;
}

int devdrv_sync_msg_send(void *msg_chan, void *data, u32 in_data_len, u32 out_data_len, u32 *real_out_len)
{
    int ret;

    struct devdrv_msg_chan *chan = (struct devdrv_msg_chan *)msg_chan;

    if (chan == NULL) {
        devdrv_err("sync_msg_send, msg_chan is null\n");
        return -EINVAL;
    }
    if ((chan->msg_dev == NULL) || (chan->msg_dev->dev == NULL)) {
        devdrv_err("chan->msg_dev or chan->msg_dev->dev is null\n");
        return -EINVAL;
    }
    if (data == NULL) {
        devdrv_err("devid:%u, data is null\n", chan->msg_dev->pci_ctrl->dev_id);
        return -EINVAL;
    }
    if (real_out_len == NULL) {
        devdrv_err("devid:%u, real_out_len is null\n", chan->msg_dev->pci_ctrl->dev_id);
        return -EINVAL;
    }

    ret = devdrv_sync_non_trans_msg_send(chan, data, in_data_len, out_data_len, real_out_len,
                                         DEVDRV_COMMON_MSG_TYPE_MAX);
    if (ret) {
        devdrv_err("devid:%u, send failed, chan id %d in_data_len %d ret = %d\n", chan->msg_dev->pci_ctrl->dev_id,
                   chan->chan_id, in_data_len, ret);
    }

    return ret;
}
EXPORT_SYMBOL(devdrv_sync_msg_send);

void devdrv_admin_record_wait_time(int dev_id, enum devdrv_admin_msg_opcode opcode, u32 time_use, u32 time_log)
{
    if (time_use > time_log) {
        devdrv_info("dev id %d op %d, wait time %d(us) log threshold %d(us).\n", dev_id, opcode, time_use, time_log);
    }
}

int devdrv_admin_send_wait_resq(struct devdrv_msg_chan *msg_chan, int *time)
{
    struct devdrv_admin_msg_command *msg_head = (struct devdrv_admin_msg_command *)msg_chan->sq_info.desc_h;
    int dev_id = msg_chan->msg_dev->pci_ctrl->dev_id;
    int status;
    int timeout;

    /* wait for the device irq set status, when device dead, timeout quickly */
    timeout = DEVDRV_MSG_IRQ_TIMEOUT;
    while (timeout > 0) {
        status = msg_chan->msg_dev->pci_ctrl->shr_para->admin_msg_status;
        if (status == DEVDRV_MSG_CMD_IRQ_BEGIN)
            break;

        usleep_range(DEVDRV_MSG_WAIT_MIN_TIME, DEVDRV_MSG_WAIT_MAX_TIME);

        if (devdrv_device_status_abnormal_check(msg_chan)) {
            devdrv_info("device %d is abnormal! msg_type is %d.\n", dev_id, msg_chan->msg_type);
            return -ENODEV;
        }

        timeout -= DEVDRV_MSG_WAIT_MIN_TIME;
    }

    devdrv_admin_record_wait_time(dev_id, msg_head->opcode, (DEVDRV_MSG_IRQ_TIMEOUT - timeout),
                                  DEVDRV_MSG_IRQ_TIMEOUT_LOG);

    if (timeout <= 0) {
        devdrv_err("dev id %d opcode %d, device not resp. status %d.\n", dev_id, msg_head->opcode, status);
        return -ENOSYS;
    }

    timeout = DEVDRV_MSG_TIMEOUT;
    while (timeout > 0) {
        status = msg_head->status;
        if (status != DEVDRV_MSG_CMD_BEGIN)
            break;

        usleep_range(DEVDRV_MSG_WAIT_MIN_TIME, DEVDRV_MSG_WAIT_MAX_TIME);

        if (devdrv_device_status_abnormal_check(msg_chan)) {
            devdrv_info("device %d is abnormal! msg_type is %d.\n", dev_id, msg_chan->msg_type);
            return -ENODEV;
        }

        timeout -= DEVDRV_MSG_WAIT_MIN_TIME;
    }

    devdrv_admin_record_wait_time(dev_id, msg_head->opcode, (DEVDRV_MSG_TIMEOUT - timeout), DEVDRV_MSG_TIMEOUT_LOG);

    if (status != DEVDRV_MSG_CMD_FINISH_SUCCESS) {
        devdrv_err("dev id %d opcode %d, wait time %d(us), finish status %d.\n", dev_id, msg_head->opcode,
                   DEVDRV_MSG_TIMEOUT - timeout, status);
        return -ENODEV;
    }

    *time = timeout;

    return 0;
}


/* admin msg chan submit a command and wait reply */
int devdrv_admin_msg_chan_send(struct devdrv_msg_dev *msg_dev, enum devdrv_admin_msg_opcode opcode, const void *cmd,
                               int size, void *reply, int reply_size)
{
    struct devdrv_msg_chan *msg_chan = msg_dev->admin_msg_chan;
    struct devdrv_admin_msg_command *msg_head = NULL;
    struct devdrv_admin_msg_reply *msg_reply = NULL;
    int ret;
    int timeout;
    int dev_id = msg_dev->pci_ctrl->dev_id;
    enum devdrv_msg_client_type msg_type_tmp = msg_chan->msg_type;

    if ((u32)size > DEVDRV_ADMIN_MSG_DATA_LEN) {
        devdrv_err("dev id %d size %d is too big!\n", dev_id, size);
        return -EINVAL;
    }

    if (msg_dev->pci_ctrl->device_status == DEVDRV_DEVICE_DEAD) {
        devdrv_info("dev id %d is reset, opcode %d no need send.\n", dev_id, opcode);
        return 0;
    }
    devdrv_debug("dev id %d opcode %d, start.\n", dev_id, opcode);

    devdrv_device_mutex_lock(msg_chan);

    if (devdrv_device_status_abnormal_check(msg_chan)) {
        devdrv_device_mutex_unlock(msg_chan);
        devdrv_info("device is abnormal! msg_type is %d.\n", msg_type_tmp);
        return -ENODEV;
    }

    msg_dev->pci_ctrl->shr_para->admin_msg_status = DEVDRV_MSG_CMD_BEGIN;

    msg_head = (struct devdrv_admin_msg_command *)msg_chan->sq_info.desc_h;
    msg_head->opcode = opcode;
    msg_head->status = DEVDRV_MSG_CMD_BEGIN;

    if ((cmd != NULL) && (memcpy_s(msg_head->data, DEVDRV_ADMIN_MSG_DATA_LEN, cmd, size) != 0)) {
        devdrv_err("dev id %d memcpy failed\n", dev_id);
        return -EINVAL;
    }
    wmb();

#ifdef CFG_SOC_PLATFORM_ESL_FPGA
    msleep(100);
#endif
    devdrv_msg_ring_doorbell((void *)msg_chan);

    ret = devdrv_admin_send_wait_resq(msg_chan, &timeout);
    if (ret != 0) {
        devdrv_device_mutex_unlock(msg_chan);
        return ret;
    }

    ret = -1;
    if (reply == NULL) { /* not need response message */
        ret = 0;
    } else {
        msg_reply = (struct devdrv_admin_msg_reply *)msg_head->data;
        if (msg_reply->len - sizeof(msg_reply->len) > (u32)reply_size) {
            devdrv_info("dev id %d opcode %d, reply len %d is large than the reply buf %d.\n", dev_id, opcode,
                        msg_reply->len, reply_size);
        } else if (memcpy_s(reply, reply_size, msg_reply->data, msg_reply->len - sizeof(msg_reply->len)) == 0) {
            ret = 0;
        }
    }

    devdrv_device_mutex_unlock(msg_chan);

    devdrv_debug("dev id %d opcode %d, wait time %d(us), finish status %d.\n", dev_id, opcode,
                 DEVDRV_MSG_TIMEOUT - timeout, msg_head->status);

    return ret;
}

void devdrv_free_msg_queue_res(struct devdrv_msg_chan *msg_chan)
{
    msg_chan->status = DEVDRV_DISABLE;
    if (msg_chan->cq_info.irq_vector >= 0) {
        devdrv_unregister_irq_func(msg_chan->msg_dev->pci_ctrl, msg_chan->cq_info.irq_vector, msg_chan);
        msg_chan->cq_info.irq_vector = -1;
    }

    if (msg_chan->sq_info.irq_vector >= 0) {
        devdrv_unregister_irq_func(msg_chan->msg_dev->pci_ctrl, msg_chan->sq_info.irq_vector, msg_chan);
        msg_chan->sq_info.irq_vector = -1;
    }

    if (msg_chan->rx_work_flag) {
        cancel_work_sync(&msg_chan->rx_work);
        msg_chan->rx_work_flag = 0;
    }

    devdrv_msg_free_cq(msg_chan);
    devdrv_msg_free_sq(msg_chan);
}

struct devdrv_msg_chan *devdrv_alloc_msg_queue(void *priv, struct devdrv_msg_chan_info *chan_info)
{
    struct devdrv_pci_ctrl *pci_ctrl = (struct devdrv_pci_ctrl *)priv;
    struct devdrv_msg_dev *msg_dev = pci_ctrl->msg_dev;
    struct devdrv_msg_chan *msg_chan = NULL;
    struct devdrv_create_queue_command cmd_data;
    int ret;

    /* alloc msg_chan */
    msg_chan = devdrv_alloc_msg_chan(msg_dev, chan_info->level);
    if (msg_chan == NULL) {
        devdrv_err("alloc msg_chan failed!\n");
        return NULL;
    }

    cmd_data.msg_type = chan_info->msg_type;
    cmd_data.queue_type = chan_info->queue_type;
    cmd_data.queue_id = msg_chan->chan_id;
    cmd_data.sq_dma_base_host = 0;
    cmd_data.cq_dma_base_host = 0;
    cmd_data.sq_desc_size = 0;
    cmd_data.cq_desc_size = 0;
    cmd_data.sq_depth = 0;
    cmd_data.cq_depth = 0;
    cmd_data.sq_slave_mem_offset = 0;
    cmd_data.cq_slave_mem_offset = 0;
    /* alloc sq queue */
    if (chan_info->sq_desc_size && chan_info->queue_depth) {
        ret = devdrv_msg_alloc_s_queue(msg_chan, chan_info->queue_depth, chan_info->sq_desc_size);
        if (ret) {
            devdrv_err("dev_id %d, alloc s_queue failed ret = %d!\n", msg_dev->pci_ctrl->dev_id, ret);
            devdrv_msg_free_host_sq(msg_chan);
            msg_chan->status = DEVDRV_DISABLE;
            return NULL;
        }

        cmd_data.sq_desc_size = chan_info->sq_desc_size;
        cmd_data.sq_depth = chan_info->queue_depth;
        cmd_data.sq_dma_base_host = msg_chan->sq_info.dma_handle;
        cmd_data.sq_slave_mem_offset = msg_chan->sq_info.slave_mem_offset;
    }

    /* alloc cq queue */
    if (chan_info->cq_desc_size && chan_info->queue_depth) {
        ret = devdrv_msg_alloc_c_queue(msg_chan, chan_info->queue_depth, chan_info->cq_desc_size);
        if (ret) {
            devdrv_err("dev_id %d, alloc c_queue failed ret = %d!\n", msg_dev->pci_ctrl->dev_id, ret);
            devdrv_free_msg_queue_res(msg_chan);
            return NULL;
        }

        cmd_data.cq_desc_size = chan_info->cq_desc_size;
        cmd_data.cq_depth = chan_info->queue_depth;
        cmd_data.cq_dma_base_host = msg_chan->cq_info.dma_handle;
        cmd_data.cq_slave_mem_offset = msg_chan->cq_info.slave_mem_offset;
    }

    /* request irqs */
    if (chan_info->queue_type == TRANSPARENT_MSG_QUEUE) {
        if (chan_info->rx_msg_notify != NULL) {
            cmd_data.irq_rx_msg_notify = msg_chan->irq_rx_msg_notify;

            (void)devdrv_register_irq_func(priv, msg_chan->irq_rx_msg_notify, devdrv_rx_msg_notify_handler, msg_chan,
                                           "trans msg_chan_rx_msg_notify");
        } else {
            cmd_data.irq_rx_msg_notify = -1;
        }

        if (chan_info->tx_finish_notify != NULL) {
            cmd_data.irq_tx_finish_notify = msg_chan->irq_tx_finish_notity;

            (void)devdrv_register_irq_func(priv, msg_chan->irq_tx_finish_notity, devdrv_tx_fnsh_notify_handler,
                                           msg_chan, "trans msg_chan_tx_finish_notify");
        } else {
            cmd_data.irq_tx_finish_notify = -1;
        }
        msg_chan->rx_work_flag = 0;
    } else {
        cmd_data.irq_rx_msg_notify = msg_chan->irq_rx_msg_notify;
        cmd_data.irq_tx_finish_notify = -1;

        (void)devdrv_register_irq_func(priv, msg_chan->irq_rx_msg_notify, devdrv_wakeup_rx_work, msg_chan,
                                       "non-trans msg_chan_rx_msg_notify");

        INIT_WORK(&msg_chan->rx_work, devdrv_non_trans_rx_msg_task);
        msg_chan->rx_work_flag = 1;
    }

    msg_chan->msg_type = chan_info->msg_type;
    msg_chan->flag = chan_info->flag;
    msg_chan->cq_info.irq_vector = cmd_data.irq_rx_msg_notify;
    msg_chan->sq_info.irq_vector = cmd_data.irq_tx_finish_notify;
    msg_chan->rx_msg_process = chan_info->rx_msg_process;
    msg_chan->rx_msg_notify = chan_info->rx_msg_notify;
    msg_chan->tx_finish_notify = chan_info->tx_finish_notify;
    msg_chan->queue_type = chan_info->queue_type;

    devdrv_debug("dev_id %d, alloc msg chan: msg type %d, queue id %d.\n", msg_dev->pci_ctrl->dev_id, cmd_data.msg_type,
                 cmd_data.queue_id);

    ret = devdrv_admin_msg_chan_send(msg_dev, DEVDRV_CREATE_MSG_QUEUE, &cmd_data, sizeof(cmd_data), NULL, 0);
    if (ret) {
        devdrv_err("dev_id %d, alloc msg chan: msg type %d, queue type %d, queue id %d, "
                   "sq size %d, cq size %d, sq depth %d, cq depth %d, tx irq %d, rx irq %d ret = %d.\n",
                   msg_dev->pci_ctrl->dev_id, cmd_data.msg_type, cmd_data.queue_type, cmd_data.queue_id,
                   cmd_data.sq_desc_size, cmd_data.cq_desc_size, cmd_data.sq_depth, cmd_data.cq_depth,
                   cmd_data.irq_tx_finish_notify, cmd_data.irq_rx_msg_notify, ret);

        devdrv_free_msg_queue_res(msg_chan);
        return NULL;
    }

    return msg_chan;
}

int devdrv_free_msg_queue(struct devdrv_msg_chan *msg_chan, enum msg_queue_type queue_type)
{
    struct devdrv_free_queue_cmd cmd_data;
    struct devdrv_msg_dev *msg_dev = NULL;
    int ret;

    if (msg_chan == NULL) {
        devdrv_err("free_msg_queue,msg_chan is null\n");
        return -EINVAL;
    }

    msg_dev = msg_chan->msg_dev;

    /* fill the queue free cmd */
    cmd_data.queue_id = msg_chan->chan_id;

    /* send the cmd */
    ret = devdrv_admin_msg_chan_send(msg_dev, DEVDRV_FREE_MSG_QUEUE, &cmd_data, sizeof(cmd_data), NULL, 0);
    if (ret) {
        devdrv_err("devid:%u, cmd %d failed ret = %d!\n", msg_dev->pci_ctrl->dev_id, DEVDRV_FREE_MSG_QUEUE, ret);
    }

    devdrv_free_msg_queue_res(msg_chan);

    return ret;
}

struct devdrv_msg_chan *devdrv_alloc_trans_queue(void *priv, struct devdrv_trans_msg_chan_info *chan_info)
{
    struct devdrv_msg_chan_info msg_chan_info;
    struct devdrv_msg_chan *msg_chan = NULL;

    if (chan_info->msg_type >= devdrv_msg_client_max) {
        devdrv_err("alloc_trans_queue: chan_info->msg_type %d is error\n", chan_info->msg_type);
        return NULL;
    }

    if (memset_s((void *)&msg_chan_info, sizeof(struct devdrv_msg_chan_info), 0, sizeof(struct devdrv_msg_chan_info)) !=
        0) {
        devdrv_err("memset_s failed\n");
        return NULL;
    }

    msg_chan_info.msg_type = chan_info->msg_type;
    msg_chan_info.queue_depth = chan_info->queue_depth;
    msg_chan_info.sq_desc_size = chan_info->sq_desc_size;
    msg_chan_info.cq_desc_size = chan_info->cq_desc_size;
    msg_chan_info.rx_msg_notify = chan_info->rx_msg_notify;
    msg_chan_info.tx_finish_notify = chan_info->tx_finish_notify;
    msg_chan_info.queue_type = TRANSPARENT_MSG_QUEUE;
    msg_chan_info.level = chan_info->level;

    msg_chan = devdrv_alloc_msg_queue(priv, &msg_chan_info);
    if (msg_chan == NULL) {
        devdrv_err("devdrv_alloc_msg_queue fail\n");
        return NULL;
    }

    return msg_chan;
}

int devdrv_free_trans_queue(struct devdrv_msg_chan *msg_chan)
{
    return devdrv_free_msg_queue(msg_chan, TRANSPARENT_MSG_QUEUE);
}

struct devdrv_msg_chan *devdrv_alloc_non_trans_queue(void *priv, struct devdrv_non_trans_msg_chan_info *chan_info)
{
    struct devdrv_msg_chan_info msg_chan_info;
    struct devdrv_msg_chan *msg_chan = NULL;

    if (chan_info->msg_type >= devdrv_msg_client_max) {
        devdrv_err("alloc_non_trans_queue: chan_info->msg_type %d is error\n", chan_info->msg_type);
        return NULL;
    }

    if (chan_info->rx_msg_process == NULL) {
        devdrv_err("alloc_non_trans_queue: rx_msg_process must set\n");
        return NULL;
    }

    msg_chan_info.msg_type = chan_info->msg_type;
    msg_chan_info.flag = chan_info->flag;

    /* synchronization message mechanism, using a block of memory */
    msg_chan_info.sq_desc_size = chan_info->s_desc_size;
    msg_chan_info.cq_desc_size = chan_info->c_desc_size;
    msg_chan_info.queue_depth = 1;

    msg_chan_info.rx_msg_process = chan_info->rx_msg_process;
    msg_chan_info.queue_type = NON_TRANSPARENT_MSG_QUEUE;
    msg_chan_info.level = chan_info->level;
    msg_chan_info.rx_msg_notify = NULL;
    msg_chan_info.tx_finish_notify = NULL;

    msg_chan = devdrv_alloc_msg_queue(priv, &msg_chan_info);
    if (msg_chan == NULL) {
        devdrv_err("devdrv_alloc_msg_queue fail\n");
        return NULL;
    }

    return msg_chan;
}

int devdrv_free_non_trans_queue(struct devdrv_msg_chan *msg_chan)
{
    return devdrv_free_msg_queue(msg_chan, NON_TRANSPARENT_MSG_QUEUE);
}

int devdrv_notify_dma_err_irq(void *drvdata, u32 dma_chan_id, u32 err_irq)
{
    struct devdrv_pci_ctrl *pci_ctrl = (struct devdrv_pci_ctrl *)drvdata;
    struct devdrv_notify_dma_err_irq_cmd cmd_data;
    int ret;

    /* fill the cmd */
    cmd_data.dma_chan_id = dma_chan_id;
    cmd_data.err_irq = err_irq;

    /* send the cmd */
    ret = devdrv_admin_msg_chan_send(pci_ctrl->msg_dev, DEVDRV_NOTIFY_DMA_ERR_IRQ, &cmd_data, sizeof(cmd_data), NULL,
                                     0);
    if (ret) {
        devdrv_err("chanid:%u, cmd %d failed ret = %d!\n", dma_chan_id, DEVDRV_NOTIFY_DMA_ERR_IRQ,
                   ret);
    }

    return ret;
}

int devdrv_get_general_interrupt_db_info(struct devdrv_msg_dev *msg_dev, u32 *db_start, u32 *db_num)
{
    struct devdrv_general_interrupt_db_info reply_data = {0};
    int ret;

    /* send the cmd */
    ret = devdrv_admin_msg_chan_send(msg_dev, DEVDRV_GET_GELNERAL_INTERRUPT_DB_INFO, NULL, 0, &reply_data,
                                     sizeof(reply_data));
    if (ret) {
        devdrv_err("cmd %d failed ret = %d!\n", DEVDRV_GET_GELNERAL_INTERRUPT_DB_INFO, ret);
    } else {
        *db_start = reply_data.db_start;
        *db_num = reply_data.db_num;
    }

    return ret;
}

int devdrv_get_rx_atu_info(struct devdrv_pci_ctrl *pci_ctrl, u32 bar_num)
{
    struct devdrv_get_rx_atu_cmd cmd_data;
    struct devdrv_iob_atu *atu = NULL;
    struct devdrv_iob_atu io_rsv_atu[DEVDRV_MAX_RX_ATU_NUM];
    struct devdrv_iob_atu *reply = NULL;
    int ret;
    int i;

    cmd_data.devid = pci_ctrl->dev_id;
    cmd_data.bar_num = bar_num;

    if (bar_num == PCI_BAR_MEM) {
        reply = pci_ctrl->mem_rx_atu;
    } else {
        reply = io_rsv_atu;
    }

    ret = devdrv_admin_msg_chan_send(pci_ctrl->msg_dev, DEVDRV_GET_RX_ATU, &cmd_data, sizeof(cmd_data),
                                     reply, sizeof(struct devdrv_iob_atu) * DEVDRV_MAX_RX_ATU_NUM);
    if (ret) {
        devdrv_err("admin cmd %d get rx atu info fail, ret=%d\n", DEVDRV_GET_RX_ATU, ret);
        return -ENOMEM;
    }
    for (i = 0; i < DEVDRV_MAX_RX_ATU_NUM; i++) {
        atu = &(reply[i]);
        if (atu->valid == DEVDRV_VALID) {
            devdrv_info("rx atu index %d, atu id %d, size 0x%llx\r\n", i, atu->atu_id, atu->size);
        }
    }

    return 0;
}

int devdrv_notify_dev_online(struct devdrv_msg_dev *msg_dev, u32 devid, u32 status)
{
    struct devdrv_notify_dev_online_cmd cmd_data;
    int ret;

    /* fill the cmd */
    cmd_data.devid = devid;
    cmd_data.status = status;

    /* send the cmd */
    ret = devdrv_admin_msg_chan_send(msg_dev, DEVDRV_NOTIFY_DEV_ONLINE, &cmd_data, sizeof(cmd_data), NULL, 0);
    if (ret) {
        devdrv_err("dev %d, notify online cmd failed. dst dev %d ret %d\n", msg_dev->pci_ctrl->dev_id, devid, ret);
    }

    return ret;
}

int devdrv_get_hdc_surport_msg_chan_cnt(u32 devid)
{
    struct devdrv_pci_ctrl *pci_ctrl = NULL;
    struct devdrv_ctrl *ctrl = NULL;
    int chan_cnt = -1;

    ctrl = devdrv_get_bottom_half_devctrl_by_id(devid);
    if ((ctrl != NULL) && (ctrl->priv != NULL)) {
        pci_ctrl = ctrl->priv;

#ifdef CFG_SOC_PLATFORM_CLOUD
        /* reserve one non trans chan for all types */
        chan_cnt = pci_ctrl->msg_dev->chan_cnt - AGENTDRV_QCNT_EACH_IRQ;
#else
        /* reserve one non trans chan for all types */
        chan_cnt = pci_ctrl->msg_dev->chan_cnt - devdrv_msg_client_max;
#endif

        /* for 3559 */
        if (chan_cnt < DEVDRV_MSG_CHAN_CNT_3559) {
            chan_cnt = DEVDRV_MSG_CHAN_CNT_3559;
        }
    }

    return chan_cnt;
}
EXPORT_SYMBOL(devdrv_get_hdc_surport_msg_chan_cnt);

int devdrv_init_admin_msg_chan(struct devdrv_msg_dev *msg_dev)
{
    /* The first application in the initialization, the fixed application is channel 0 */
    struct devdrv_msg_chan *msg_chan = NULL;
    void __iomem *sdi_io_base = NULL;
    dma_addr_t sq_dma_handle;
    int ret = -1;

    msg_chan = devdrv_alloc_msg_chan(msg_dev, DEVDRV_MSG_CHAN_LEVEL_LOW);
    if (msg_chan == NULL) {
        devdrv_err("devid:%u, admin msg chan alloc fail\n", msg_dev->pci_ctrl->dev_id);
        return ret;
    }
    /* admin queue communication only needs a sq buf  */
    ret = devdrv_msg_alloc_host_sq(msg_chan, DEVDRV_ADMIN_MSG_QUEUE_DEPTH, DEVDRV_ADMIN_MSG_QUEUE_BD_SIZE);
    if (ret != 0) {
        devdrv_err("devid:%u, admin sq msg queue alloc fail ret = %d\n", msg_dev->pci_ctrl->dev_id, ret);
        return ret;
    }

    msg_chan->cq_info.irq_vector = -1;
    msg_chan->rx_work_flag = 0;

    sq_dma_handle = msg_chan->sq_info.dma_handle;
    sdi_io_base = msg_dev->ctrl_io_base;

    /* notice base addr of SQ and CQ */
    devdrv_set_admin_sq_base(sdi_io_base, (u64)sq_dma_handle, msg_dev->func_id);

    msg_dev->admin_msg_chan = msg_chan;

    return 0;
}

int devdrv_msg_chan_init(struct devdrv_msg_dev *msg_dev, int chan_start, int chan_end, int irq_base)
{
    int i;
    struct devdrv_msg_chan *msg_chan = NULL;

    for (i = chan_start; i < chan_end; i++) {
        msg_chan = &msg_dev->msg_chan[i];
        msg_chan->chan_id = i;
        msg_chan->status = DEVDRV_DISABLE;
        msg_chan->msg_dev = msg_dev;

        msg_chan->io_base = msg_dev->db_io_base + (long)i * DEVDRV_DB_QUEUE_TYPE * DEVDRV_MSG_CHAN_DB_OFFSET;

        msg_chan->irq_rx_msg_notify = irq_base + (i - chan_start) * DEVDRV_MSG_CHAN_IRQ_NUM;
        msg_chan->irq_tx_finish_notity = irq_base + (i - chan_start) * DEVDRV_MSG_CHAN_IRQ_NUM + 1;

        devdrv_debug("devid:%u, msg chan db_io_base %pK rx_msg_notify vector %d, tx_finish_notity vector %d.\n",
                     msg_dev->pci_ctrl->dev_id, msg_chan->io_base, msg_chan->irq_rx_msg_notify,
                     msg_chan->irq_tx_finish_notity);

        mutex_init(&msg_chan->mutex);
    }
    return 0;
}

int devdrv_msg_init(struct devdrv_pci_ctrl *pci_ctrl, int irq_base, int irq_num)
{
    int ret;
    struct device *dev = &pci_ctrl->pdev->dev;
    struct devdrv_msg_dev *msg_dev = NULL;
    int irq2_num = (pci_ctrl->msix_irq_num >= DEVDRV_MSG_MSI_X_VECTOR_2_BASE)
                        ? pci_ctrl->msix_irq_num - DEVDRV_MSG_MSI_X_VECTOR_2_BASE
                        : 0;
    int chan_cnt = irq_num / DEVDRV_MSG_CHAN_IRQ_NUM + irq2_num / DEVDRV_MSG_CHAN_IRQ_NUM;

    chan_cnt = (chan_cnt > DEVDRV_MAX_MSG_CHAN_CNT) ? DEVDRV_MAX_MSG_CHAN_CNT : chan_cnt;
    devdrv_debug("devid:%u, create msg chan cnt:%d, db_io_base %pK.\n", pci_ctrl->dev_id, chan_cnt, pci_ctrl->msi_base);

    msg_dev = kzalloc(sizeof(struct devdrv_msg_dev) + sizeof(struct devdrv_msg_chan) * chan_cnt, GFP_KERNEL);
    if (msg_dev == NULL) {
        devdrv_err("devid:%u, msg_dev kzalloc fail\n", pci_ctrl->dev_id);
        return -ENOMEM;
    }
    pci_ctrl->msg_dev = msg_dev;
    msg_dev->pci_ctrl = pci_ctrl;
    msg_dev->db_io_base = pci_ctrl->msi_base;
    msg_dev->ctrl_io_base = pci_ctrl->io_base + DEVDRV_IEP_SDI0_OFFSET;
    msg_dev->reserve_mem_base = pci_ctrl->mem_base;

    msg_dev->chan_cnt = chan_cnt;
    msg_dev->dev = dev;
    mutex_init(&msg_dev->mutex);
    msg_dev->func_id = pci_ctrl->func_id;

    INIT_LIST_HEAD(&msg_dev->slave_mem_list);
    msg_dev->slave_mem.offset = DEVDRV_MSG_QUEUE_MEM_BASE;
    msg_dev->slave_mem.len = DEVDRV_MSG_QUEUE_MEM_SIZE;

    msg_dev->work_queue = create_workqueue("pcie_msg_workqueue");
    if (msg_dev->work_queue == NULL) {
        devdrv_err("devid:%u, create msg work_queue failed\n", pci_ctrl->dev_id);
        kfree(msg_dev);
        msg_dev = NULL;
        return -ENOMEM;
    }

    (void)devdrv_msg_chan_init(msg_dev, 0, irq_num / DEVDRV_MSG_CHAN_IRQ_NUM, irq_base);
    if (irq2_num / DEVDRV_MSG_CHAN_IRQ_NUM > 0)
        (void)devdrv_msg_chan_init(msg_dev, irq_num / DEVDRV_MSG_CHAN_IRQ_NUM, chan_cnt,
                                   DEVDRV_MSG_MSI_X_VECTOR_2_BASE);

    ret = devdrv_init_admin_msg_chan(msg_dev);
    if (ret) {
        devdrv_err("devid:%u, admin queue init fail ret = %d\n", pci_ctrl->dev_id, ret);
        destroy_workqueue(msg_dev->work_queue);
        kfree(msg_dev);
        msg_dev = NULL;
    }

    return ret;
}

void devdrv_msg_exit(struct devdrv_pci_ctrl *pci_ctrl)
{
    struct devdrv_msg_dev *msg_dev = pci_ctrl->msg_dev;
    u32 i;
    struct devdrv_msg_chan *msg_chan = NULL;
    struct list_head *pos = NULL;
    struct list_head *n = NULL;
    struct devdrv_msg_slave_mem_node *node = NULL;

    for (i = 0; i < msg_dev->chan_cnt; i++) {
        msg_chan = &msg_dev->msg_chan[i];
        if (msg_chan->status == DEVDRV_ENABLE)
            devdrv_free_msg_queue_res(msg_chan);
    }

    destroy_workqueue(msg_dev->work_queue);

    if (!list_empty_careful(&msg_dev->slave_mem_list)) {
        list_for_each_safe(pos, n, &msg_dev->slave_mem_list)
        {
            node = list_entry(pos, struct devdrv_msg_slave_mem_node, list);
            list_del(&node->list);
            kfree(node);
            node = NULL;
        }
    }

    kfree(msg_dev);
    msg_dev = NULL;
    pci_ctrl->msg_dev = NULL;
    devdrv_info("devdrv_msg_exit msg free\n");
}
