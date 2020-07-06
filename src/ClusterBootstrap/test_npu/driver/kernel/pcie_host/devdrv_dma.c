/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2012-2019. All rights reserved.
 * Description: DMA Base Function
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
#include <asm/io.h>
#include <linux/dma-mapping.h>
#include <linux/delay.h>
#include <linux/errno.h>
#include <linux/jiffies.h>
#include <linux/module.h>
#include <linux/platform_device.h>
#include <linux/sched.h>
#include <linux/slab.h>
#include <linux/types.h>
#include <linux/vmalloc.h>
#ifdef PCIE_ENV_DEVICE
#include <linux/securec.h>
#else
#include "securec.h"
#endif

#include "devdrv_dma.h"
#include <linux/timex.h>
#include <linux/rtc.h>
#include "devdrv_util.h"

/* dma channel sq submit interface */
void devdrv_dma_ch_sq_submit(struct devdrv_dma_channel *dma_chan)
{
    devdrv_set_dma_sq_tail(dma_chan->io_base, dma_chan->sq_tail);
}

u32 devdrv_get_dma_sqcq_side(struct devdrv_dma_channel *dma_chan)
{
    return (dma_chan->flag >> DEVDRV_DMA_SQCQ_SIDE_BIT) & 1;
}

void devdrv_set_dma_status(struct devdrv_dma_dev *dma_dev, u32 status)
{
    if (dma_dev != NULL) {
        dma_dev->dev_status = status;
    }
}

/* not all sq bds will respond to cq, and multiple sq may be merged. Need to consider the merged sq */
void devdrv_dma_done_sq_proc(struct devdrv_dma_channel *dma_chan, u32 cq_sqhd, u32 status)
{
    struct devdrv_dma_soft_bd *soft_bd = NULL;
    u32 sq_index;
    u32 cur_status;
    u32 sq_cnt = (cq_sqhd + dma_chan->sq_depth - dma_chan->sq_head + 1) % dma_chan->sq_depth;
    int ret;

    for (sq_index = 0; sq_index < sq_cnt; sq_index++) {
        /* merged cq status is ok */
        cur_status = (dma_chan->sq_head == cq_sqhd) ? status : 0;

        soft_bd = dma_chan->dma_soft_bd + dma_chan->sq_head;
        dma_chan->sq_head = (dma_chan->sq_head + 1) % dma_chan->sq_depth;

        /* do not pay attention to soft bd, like the second bd sent by small packet */
        if (soft_bd->valid == DEVDRV_DISABLE) {
            continue;
        }
        rmb();
        /* the front bd in chain copy */
        if (soft_bd->owner_bd >= 0) {
            /* status error needs to be set to the last bd */
            if (cur_status != 0) {
                soft_bd = dma_chan->dma_soft_bd + soft_bd->owner_bd;
                soft_bd->status = cur_status;
            }
        } else {
            /* if there is no error in front of bd, assign the status of the last sq. */
            if (soft_bd->status == -1) {
                soft_bd->status = cur_status;
            }

            ret = devdrv_dma_is_axi_write_error(dma_chan->io_base);
            if (ret) {
                devdrv_err("local_id %u axi error\n", dma_chan->local_id);
                soft_bd->status = DEVDRV_SOFTBD_STATUS_INVALID;
            }

            if (soft_bd->wait_type != DEVDRV_DMA_WAIT_INTR) {
                soft_bd->valid = DEVDRV_DISABLE;
                continue;
            }

            /* synchronous mode release semaphore wake-up waiting task */
            if (soft_bd->copy_type == DEVDRV_DMA_SYNC) {
                up(&soft_bd->sync_sem);
                dma_chan->status.sync_sem_up_cnt++;
                goto proc_finish;
            }
            /* asynchronous mode callback completion function */
            if (soft_bd->callback_func) {
                soft_bd->callback_func(soft_bd->priv, soft_bd->trans_id, soft_bd->status);
                dma_chan->status.async_proc_cnt++;
            }
        }
proc_finish:
        soft_bd->valid = DEVDRV_DISABLE;
    }

    if (status) {
        devdrv_err("local_id %u, dma copy failed, sq:%d, error code: 0x%x\n", dma_chan->local_id, cq_sqhd, status);
    }
}

void devdrv_dma_done_task(unsigned long data)
{
    struct devdrv_dma_cq_node *p_cur_last_cq = NULL;
    struct devdrv_dma_channel *dma_chan = (struct devdrv_dma_channel *)((uintptr_t)data);
    u32 head;
    int cnt = 0;
    u64 ivl;

    spin_lock_bh(&dma_chan->cq_lock);

    dma_chan->status.done_tasklet_in_cnt++;
    dma_chan->status.done_tasklet_in_time = jiffies;

    while (1) {
        head = (dma_chan->cq_head + 1) % (dma_chan->cq_depth);
        p_cur_last_cq = dma_chan->cq_desc_base + head;

        /* invalid cq, break */
        if (!devdrv_dma_get_cq_valid(p_cur_last_cq, dma_chan->rounds)) {
            break;
        }

        /* Reach the threshold and schedule out */
        if (cnt >= DMA_DONE_BUDGET) {
            tasklet_schedule(&dma_chan->dma_done_task);
            dma_chan->status.re_schedule_cnt++;
            break;
        }

        devdrv_dma_done_sq_proc(dma_chan, devdrv_dma_get_cq_sqhd(p_cur_last_cq),
                                devdrv_dma_get_cq_status(p_cur_last_cq));

        devdrv_dma_set_cq_invalid(p_cur_last_cq);
        dma_chan->cq_head = head;

        if (dma_chan->cq_head == (dma_chan->cq_depth - 1)) {
            dma_chan->rounds++;
        }

        cnt++;
    }

    dma_chan->status.done_tasklet_out_time = jiffies;
    ivl = jiffies_to_msecs(dma_chan->status.done_tasklet_out_time - dma_chan->status.done_tasklet_in_time);
    if (ivl > dma_chan->status.max_task_op_time) {
            dma_chan->status.max_task_op_time = ivl;
    }
    devdrv_set_dma_cq_head(dma_chan->io_base, dma_chan->cq_head);

    spin_unlock_bh(&dma_chan->cq_lock);
}

irqreturn_t devdrv_dma_done_interrupt(int irq, void *data)
{
    struct devdrv_dma_channel *dma_chan = (struct devdrv_dma_channel *)data;

    if (dma_chan->dma_dev->dev_status != DEVDRV_DMA_ALIVE) {
        devdrv_err("dma chan %d disable\n", dma_chan->chan_id);
        return IRQ_HANDLED;
    }

    rmb();

    dma_chan->status.done_int_cnt++;
    dma_chan->status.done_int_in_time = jiffies;
    tasklet_schedule(&dma_chan->dma_done_task);

    return IRQ_HANDLED;
}

void devdrv_dma_err_proc(struct devdrv_dma_channel *dma_chan)
{
    u32 queue_init_sts = 0;
    devdrv_err("err chan id: %d\n", dma_chan->chan_id);

    devdrv_record_dma_dxf_info(dma_chan->io_base, &queue_init_sts);
    devdrv_err("sq vir base addr is 0x%pK\n", dma_chan->sq_desc_base);
    devdrv_err("cq vir base addr is 0x%pK\n", dma_chan->cq_desc_base);
    devdrv_err("software sq head %d\n", dma_chan->sq_head);
    devdrv_err("software sq tail %d\n", dma_chan->sq_tail);
    devdrv_err("software cq head %d\n", dma_chan->cq_head);

    devdrv_dfx_dma_report_to_bbox(dma_chan, queue_init_sts);
}

void devdrv_dma_err_task(struct work_struct *p_work)
{
    struct devdrv_dma_channel *dma_chan = container_of(p_work, struct devdrv_dma_channel, err_work);

    dma_chan->status.err_work_cnt++;

    devdrv_dma_err_proc(dma_chan);
}

irqreturn_t devdrv_dma_err_interrupt(int irq, void *data)
{
    struct devdrv_dma_channel *dma_chan = (struct devdrv_dma_channel *)data;

    if (dma_chan->dma_dev->dev_status != DEVDRV_DMA_ALIVE) {
        devdrv_err("dma chan %d disable\n", dma_chan->chan_id);
        return IRQ_HANDLED;
    }

    rmb();

    dma_chan->status.err_int_cnt++;

    /* start work queue */
    schedule_work(&dma_chan->err_work);

    return IRQ_HANDLED;
}

STATIC void devdrv_dma_parse_sq_interrupt_info(struct devdrv_dma_channel *dma_chan,
    struct devdrv_asyn_dma_para_info *para_info, u32 *ldie, u32 *rdie, u32 *msi)
{
    if (devdrv_get_dma_sqcq_side(dma_chan) == DEVDRV_DMA_REMOTE_SIDE) {
        *rdie = 1;
        *msi = dma_chan->done_irq;
        dma_chan->status.trigger_remot_int_cnt++;
        return;
    }
    if (para_info != NULL) {
        if (para_info->interrupt_and_attr_flag & DEVDRV_REMOTE_IRQ_FLAG) {
            *rdie = 1;
            *msi = para_info->remote_msi_vector;
            dma_chan->remote_irq_cnt++;
            dma_chan->status.trigger_remot_int_cnt++;

            /* add a local irq to update local SQ head and tail */
            if (dma_chan->remote_irq_cnt == DEVDRV_DMA_MAX_REMOTE_IRQ) {
                *ldie = 1;
                dma_chan->remote_irq_cnt = 0;
                dma_chan->status.trigger_local_128++;
            }
        }
        if (para_info->interrupt_and_attr_flag & DEVDRV_LOCAL_IRQ_FLAG) {
            *ldie = 1;
        }
    } else {
        *ldie = 1;
    }
}

void devdrv_dma_fill_sq_desc(struct devdrv_dma_channel *dma_chan, struct devdrv_dma_sq_node *sq_desc,
                             struct devdrv_dma_node *dma_node, struct devdrv_asyn_dma_para_info *para_info,
                             int intr_flag)
{
    u32 rdie = 0;
    u32 ldie = 0;
    u32 msi = 0;
    u32 attr = 0;
    u32 wd_barrier = 0;
    u32 rd_barrier = 0;
    u32 opcode;
    u32 vf_en;
    u32 pf_num;
    u32 vf_num;

    if (dma_node->direction == DEVDRV_DMA_DEVICE_TO_HOST) {
        opcode = DEVDRV_DMA_WRITE;
    } else {
        opcode = DEVDRV_DMA_READ;
    }

    if (para_info != NULL) {
        if (!(para_info->interrupt_and_attr_flag & DEVDRV_ATTR_FLAG)) {
            attr = DEVDRV_DMA_RO_RELEX_ORDER;
        }
        if (para_info->interrupt_and_attr_flag & DEVDRV_WD_BARRIER_FLAG) {
            wd_barrier = 1;
        }
        if (para_info->interrupt_and_attr_flag & DEVDRV_RD_BARRIER_FLAG) {
            rd_barrier = 1;
        }
    }

    if (intr_flag == 1) {
        devdrv_dma_parse_sq_interrupt_info(dma_chan, para_info, &ldie, &rdie, &msi);
    }

    /* fill addr */
    devdrv_dma_set_sq_addr_info(sq_desc, dma_node->src_addr, dma_node->dst_addr, dma_node->size);

    /* fill attr */
    devdrv_soc_func2vfpf(dma_chan->func_id, rdie, &vf_en, &pf_num, &vf_num);

    devdrv_dma_set_sq_attr(sq_desc, opcode, attr, pf_num, wd_barrier, rd_barrier);

    /* fill interrupt info */
    devdrv_dma_set_sq_irq(sq_desc, rdie, ldie, msi);
}

void devdrv_dma_fill_soft_bd(int wait_type, int copy_type, struct devdrv_dma_soft_bd *soft_bd,
                             struct devdrv_asyn_dma_para_info *para_info)
{
    if (para_info != NULL) {
        soft_bd->priv = para_info->priv;
        soft_bd->trans_id = para_info->trans_id;
        soft_bd->callback_func = para_info->finish_notify;
    } else {
        soft_bd->priv = NULL;
        soft_bd->trans_id = 0;
        soft_bd->callback_func = NULL;
    }
    soft_bd->copy_type = copy_type;
    soft_bd->wait_type = wait_type;
    soft_bd->owner_bd = -1;
    soft_bd->status = -1;
    sema_init(&soft_bd->sync_sem, 0);
    soft_bd->valid = DEVDRV_ENABLE;
}

int devdrv_dma_para_check(u32 dev_id, enum devdrv_dma_data_type type, struct devdrv_dma_node *dma_node, u32 node_cnt,
                          int copy_type, struct devdrv_asyn_dma_para_info *para_info)
{
    u32 i;
    int type_tmp;

    type_tmp = (int)type;

    if ((type_tmp >= DEVDRV_DMA_DATA_TYPE_MAX) || (type_tmp < DEVDRV_DMA_DATA_COMMON)) {
        devdrv_err("devid %d type %d is invalid.\n", dev_id, type_tmp);
        return -EINVAL;
    }

    if ((node_cnt == 0) || (node_cnt > DEVDRV_MAX_DMA_SQ_SPACE)) {
        devdrv_err("devid %u node_cnt %u is illegal.\n", dev_id, node_cnt);
        return -EINVAL;
    }

    if (dma_node == NULL) {
        devdrv_err("devid %d dma_node is null.\n", dev_id);
        return -EINVAL;
    }

    for (i = 0; i < node_cnt; i++) {
        if (dma_node[i].size == 0) {
            devdrv_err("dma_node %d: size %x is error.\n", i, dma_node[i].size);
            return -EINVAL;
        }

        if ((dma_node[i].direction != DEVDRV_DMA_DEVICE_TO_HOST) &&
            (dma_node[i].direction != DEVDRV_DMA_HOST_TO_DEVICE)) {
            devdrv_err("dma_node %d: direction %d is error.\n", i, dma_node[i].direction);
            return -EINVAL;
        }
    }

    if (copy_type == DEVDRV_DMA_ASYNC) {
        if (para_info == NULL) {
            devdrv_err("devid %d async mode para_info is null.\n", dev_id);
            return -EINVAL;
        }

        if (!(para_info->interrupt_and_attr_flag & DEVDRV_LOCAL_REMOTE_IRQ_FLAG)) {
            if (para_info->finish_notify) {
                devdrv_info("devid %d para_info para is error\n", dev_id);
                return -EINVAL;
            }
        }
    }

    return 0;
}

struct devdrv_dma_channel *devdrv_dma_get_chan(u32 dev_id, enum devdrv_dma_data_type type)
{
    struct devdrv_dma_dev *dma_dev = NULL;
    struct data_type_chan *data_chan = NULL;
    int chan_id;

    if (type >= DEVDRV_DMA_DATA_TYPE_MAX) {
        return NULL;
    }

    dma_dev = devdrv_get_dma_dev(dev_id);
    if (dma_dev == NULL) {
        devdrv_err("dma_dev(dev_id %u) is NULL\n", dev_id);
        return NULL;
    }

    data_chan = &dma_dev->data_chan[type];

    chan_id = data_chan->chan_start_id + ((data_chan->last_use_chan + 1) % data_chan->chan_num);
    data_chan->last_use_chan = chan_id;

    return &dma_dev->dma_chan[chan_id];
}

int devdrv_dma_get_sq_idle_bd_cnt(struct devdrv_dma_channel *dma_chan)
{
    u32 sq_tail, sq_head, sq_depth, sq_access;

    sq_tail = dma_chan->sq_tail;
    sq_head = dma_chan->sq_head;
    sq_depth = dma_chan->sq_depth;

    sq_access = sq_depth - ((sq_tail + sq_depth - sq_head) % sq_depth) - DEVDRV_DMA_CH_SQ_DESC_RSV;
    dma_chan->status.sq_idle_bd_cnt = sq_access;
    return sq_access;
}

struct devdrv_dma_soft_bd *devdrv_get_soft_bd(struct devdrv_dma_channel *dma_chan)
{
    struct devdrv_dma_soft_bd *soft_bd = NULL;

    soft_bd = dma_chan->dma_soft_bd + dma_chan->sq_tail;

    return soft_bd;
}


int devdrv_dma_chan_sync_wait(u32 dev_id, struct devdrv_dma_channel *dma_chan, struct devdrv_dma_soft_bd *soft_bd)
{
    int ret;
    int wait_cnt;

    if (soft_bd->wait_type == DEVDRV_DMA_WAIT_INTR) {
        ret = down_timeout(&soft_bd->sync_sem, DEVDRV_DMA_COPY_TIMEOUT);
        if (ret) {
            devdrv_err("devid %u dma channel[%d] time out ret = %d\n", dev_id, dma_chan->chan_id, ret);
            ret = -EINVAL;
        }
    } else {
        /* query mode*/
        ret = 0;
        wait_cnt = 0;
        do {
            /* check cq status,update soft_bd*/
            devdrv_dma_done_task((unsigned long)(uintptr_t)dma_chan);
            /* check soft_bd valid*/
            if (soft_bd->valid == DEVDRV_DISABLE) {
                break;
            }

            if (wait_cnt++ > DEVDRV_DMA_QUERY_MAX_WAIT_TIME) {
                ret = -EINVAL;
                devdrv_err("devid %u dma channel[%d] wait timeout\n", dev_id, dma_chan->chan_id);
                break;
            }

            usleep_range(1, 2);
        } while (1);
    }

    if (soft_bd->status != 0) {
        devdrv_err("devid %u dma channel[%d] dma copy failed, status %x\n", dev_id, dma_chan->chan_id, soft_bd->status);
        ret = -EINVAL;
    }

    return ret;
}

int devdrv_dma_chan_copy(u32 dev_id, struct devdrv_dma_channel *dma_chan, struct devdrv_dma_node *dma_node,
    u32 node_cnt, int wait_type, int copy_type, struct devdrv_asyn_dma_para_info *para_info)
{
    struct devdrv_dma_soft_bd *soft_bd = NULL;
    struct devdrv_dma_sq_node *sq_desc = NULL;
    u32 chan_id, last_sq_id, sq_index;
    int sq_idle_bd_cnt;
    int ret = 0;
    int intr_flag = (wait_type == DEVDRV_DMA_WAIT_INTR) ? 1 : 0;

    /* wait till chan space enough */
    spin_lock_bh(&dma_chan->lock);
    chan_id = dma_chan->chan_id;

    dma_chan->status.dma_chan_copy_cnt++;

    sq_idle_bd_cnt = devdrv_dma_get_sq_idle_bd_cnt(dma_chan);
    if (sq_idle_bd_cnt < (int)node_cnt) {
        spin_unlock_bh(&dma_chan->lock);
        devdrv_warn("devid %d chan id %d no space, idle bd %d need %d\n", dev_id, chan_id, sq_idle_bd_cnt, node_cnt);
        return -ENOSPC;
    }

    last_sq_id = (dma_chan->sq_tail + node_cnt - 1) % dma_chan->sq_depth;
    for (sq_index = 0; sq_index < node_cnt; sq_index++) {
        sq_desc = dma_chan->sq_desc_base + dma_chan->sq_tail;
        soft_bd = devdrv_get_soft_bd(dma_chan);

        if (memset_s((void *)sq_desc, DEVDRV_DMA_SQ_DESC_SIZE, 0, DEVDRV_DMA_SQ_DESC_SIZE) != 0) {
            spin_unlock_bh(&dma_chan->lock);
            devdrv_err("devid %d chan id %d memset_s failed\n", dev_id, chan_id);
            return -EIO;
        }

        if (sq_index < node_cnt - 1) {
            devdrv_dma_fill_sq_desc(dma_chan, sq_desc, &dma_node[sq_index], para_info, 0);
            soft_bd->owner_bd = last_sq_id;
            soft_bd->valid = DEVDRV_ENABLE;
        } else {
            devdrv_dma_fill_sq_desc(dma_chan, sq_desc, &dma_node[sq_index], para_info, intr_flag);
            devdrv_dma_fill_soft_bd(wait_type, copy_type, soft_bd, para_info);
        }

        dma_chan->sq_tail = (dma_chan->sq_tail + 1) % dma_chan->sq_depth;
    }
    wmb();
    devdrv_dma_ch_sq_submit(dma_chan);

    if (copy_type == DEVDRV_DMA_SYNC) {
        dma_chan->status.sync_submit_cnt++;
    } else {
        dma_chan->status.async_submit_cnt++;
    }
    spin_unlock_bh(&dma_chan->lock);

    if ((copy_type == DEVDRV_DMA_SYNC) && (soft_bd != NULL)) {
        ret = devdrv_dma_chan_sync_wait(dev_id, dma_chan, soft_bd);
    }

    return ret;
}

int devdrv_dma_copy(struct devdrv_dma_dev *dma_dev, enum devdrv_dma_data_type type, int instance,
                    struct devdrv_dma_node *dma_node, u32 node_cnt, int wait_type,int copy_type,
                    struct devdrv_asyn_dma_para_info *para_info)
{
    struct devdrv_dma_channel *dma_chan = NULL;
    struct data_type_chan *data_chan = NULL;
    int chan_id, i;
    int dev_id;
    int ret = 0;

    if (dma_dev == NULL) {
        devdrv_err("dma_dev is null\n");
        return -EINVAL;
    }
    dev_id = dma_dev->dev_id;

    devdrv_debug("type %x, instance %d, node_cnt %x, copy_type %d\n", type, instance, node_cnt, copy_type);
    data_chan = &dma_dev->data_chan[type];

    /* If wait spinlock in the tasklet, the cq interrupt that updates the sq head also be processed in the tasklet,
        which will form a deadlock. So let the caller waits */
    if (instance == DEVDRV_INVALID_INSTANCE) {
        for (i = 0; i < data_chan->chan_num; i++) {
            chan_id = data_chan->chan_start_id + ((i + data_chan->last_use_chan) % data_chan->chan_num);
            dma_chan = &dma_dev->dma_chan[chan_id];

            ret = devdrv_dma_chan_copy(dev_id, dma_chan, dma_node, node_cnt, wait_type, copy_type, para_info);
            if (ret != -ENOSPC) {
                data_chan->last_use_chan = chan_id;
                break;
            }
        }
    } else {
        chan_id = data_chan->chan_start_id + (instance % data_chan->chan_num);
        dma_chan = &dma_dev->dma_chan[chan_id];
        ret = devdrv_dma_chan_copy(dev_id, dma_chan, dma_node, node_cnt, wait_type, copy_type, para_info);
    }

    return ret;
}

struct devdrv_dma_channel *devdrv_dma_get_chan_by_type(struct devdrv_dma_dev *dma_dev, enum devdrv_dma_data_type type)
{
    struct data_type_chan *data_chan = NULL;
    int chan_id;

    if (type >= DEVDRV_DMA_DATA_TYPE_MAX) {
        devdrv_err("type %d is out of range\n", type);
        return NULL;
    }
    if (dma_dev == NULL) {
        devdrv_err("parameter dma_dev is null\n");
        return NULL;
    }

    data_chan = &dma_dev->data_chan[type];
    chan_id = data_chan->chan_start_id + ((data_chan->last_use_chan + 1) % data_chan->chan_num);
    data_chan->last_use_chan = chan_id;

    return &dma_dev->dma_chan[chan_id];
}

STATIC int devdrv_dma_chan_copy_sml_pkt(int dev_id, struct devdrv_dma_channel *dma_chan, dma_addr_t dst,
    const void *data, u32 size)
{
    struct devdrv_dma_sq_node *sq_desc = NULL;
    struct devdrv_dma_soft_bd *soft_bd = NULL;
    int ret;

    spin_lock_bh(&dma_chan->lock);

    if (devdrv_dma_get_sq_idle_bd_cnt(dma_chan) < DEVDRV_DMA_SML_PKT_SQ_DESC_NUM) {
        spin_unlock_bh(&dma_chan->lock);
        devdrv_warn("devid %u dma channel[%d] sq_tail %d sq_head %d sq_depth %d sq space not enough in small pkt.\n",
            dev_id, dma_chan->chan_id, dma_chan->sq_tail, dma_chan->sq_head, dma_chan->sq_depth);
        return -ENOSPC;
    }

    sq_desc = dma_chan->sq_desc_base + dma_chan->sq_tail;
    if (memset_s((void *)sq_desc, DEVDRV_DMA_SQ_DESC_SIZE, 0, DEVDRV_DMA_SQ_DESC_SIZE) != 0) {
        spin_unlock_bh(&dma_chan->lock);
        devdrv_err("devid %d memset_s failed\n", dev_id);
        return -ENOMEM;
    }

    /* fill addr */
    devdrv_dma_set_sq_addr_info(sq_desc, 0, dst, size);

    /* fill attr */
    devdrv_dma_set_sq_attr(sq_desc, DEVDRV_DMA_SMALL_PACKET, 0, DEVDRV_PF_NUM, 1, 1);

    /* fill interrupt info */
    devdrv_dma_set_sq_irq(sq_desc, 0, 1, 0);

    soft_bd = dma_chan->dma_soft_bd + dma_chan->sq_tail;
    devdrv_dma_fill_soft_bd(DEVDRV_DMA_WAIT_QUREY, DEVDRV_DMA_SYNC, soft_bd, NULL);

    dma_chan->sq_tail = (dma_chan->sq_tail + 1) % dma_chan->sq_depth;
    sq_desc = dma_chan->sq_desc_base + dma_chan->sq_tail;

    if (memcpy_s((void *)sq_desc, sizeof(struct devdrv_dma_sq_node), data, size) != 0) {
        spin_unlock_bh(&dma_chan->lock);
        devdrv_err("devid %d memcpy_s failed\n", dev_id);
        return -ENOMEM;
    }
    dma_chan->sq_tail = (dma_chan->sq_tail + 1) % dma_chan->sq_depth;

    wmb();
    devdrv_dma_ch_sq_submit(dma_chan);

    dma_chan->status.sml_submit_cnt++;
    spin_unlock_bh(&dma_chan->lock);

    ret = devdrv_dma_chan_sync_wait(dev_id, dma_chan, soft_bd);
    return ret;
}

int devdrv_dma_copy_sml_pkt(struct devdrv_dma_dev *dma_dev, enum devdrv_dma_data_type type, dma_addr_t dst,
                            const void *data, u32 size)
{
    struct devdrv_dma_channel *dma_chan = NULL;
    int dev_id = -1;
    int ret;

    dma_chan = devdrv_dma_get_chan_by_type(dma_dev, type);
    if (dma_chan == NULL) {
        devdrv_err("call devdrv_dma_get_chan failed, dev_id %d.\n", dev_id);
        return -EINVAL;
    }
    dev_id = dma_dev->dev_id;

    if (((dma_chan->flag >> DEVDRV_DMA_SML_PKT_BIT) & 1) == DEVDRV_DISABLE) {
        devdrv_err("devid %u this channel not support small packet\n", dev_id);
        return -EINVAL;
    }

    if (size > DEVDRV_DMA_SML_PKT_DATA_SIZE) {
        devdrv_err("devid %u pkt size %d is too big.\n", dev_id, size);
        return -EINVAL;
    }

    ret = devdrv_dma_chan_copy_sml_pkt(dev_id, dma_chan, dst, data, size);

    return ret;
}

STATIC void devdrv_free_dma_sq_cq(struct devdrv_dma_channel *dma_chan)
{
    u64 sq_size, cq_size;

    if (dma_chan->sq_desc_base) {
        sq_size = ((u64)sizeof(struct devdrv_dma_sq_node)) * dma_chan->sq_depth;

        dma_free_coherent(dma_chan->dev, sq_size, dma_chan->sq_desc_base, dma_chan->sq_desc_dma);

        dma_chan->sq_desc_base = NULL;
    }

    if (dma_chan->cq_desc_base) {
        cq_size = ((u64)sizeof(struct devdrv_dma_cq_node)) * dma_chan->cq_depth;

        dma_free_coherent(dma_chan->dev, cq_size, dma_chan->cq_desc_base, dma_chan->cq_desc_dma);

        dma_chan->cq_desc_base = NULL;
    }

    if (dma_chan->dma_soft_bd) {
        vfree(dma_chan->dma_soft_bd);
        dma_chan->dma_soft_bd = NULL;
    }
}

STATIC int devdrv_alloc_dma_sq_cq(struct devdrv_dma_channel *dma_chan)
{
    struct devdrv_dma_soft_bd *soft_virt_addr = NULL;
    void *sq_virt_addr = NULL;
    void *cq_virt_addr = NULL;
    struct device *dev = NULL;
    u64 soft_size;
    u64 sq_size;
    u64 cq_size;
    int i;

    dev = dma_chan->dev;
    sq_size = DEVDRV_DMA_SQ_DESC_SIZE * DEVDRV_MAX_DMA_CH_SQ_DEPTH;
    cq_size = DEVDRV_DMA_CQ_DESC_SIZE * DEVDRV_MAX_DMA_CH_CQ_DEPTH;
    soft_size = sizeof(struct devdrv_dma_soft_bd) * DEVDRV_MAX_DMA_CH_CQ_DEPTH;

    sq_virt_addr = dma_zalloc_coherent(dev, sq_size, &dma_chan->sq_desc_dma, GFP_KERNEL);
    if (sq_virt_addr == NULL) {
        devdrv_err("dma channel[%d] sq alloc fail\n", dma_chan->chan_id);
        return -ENOMEM;
    }
    dma_chan->sq_desc_base = (struct devdrv_dma_sq_node *)sq_virt_addr;
    dma_chan->sq_depth = DEVDRV_MAX_DMA_CH_SQ_DEPTH;

    cq_virt_addr = dma_zalloc_coherent(dev, cq_size, &dma_chan->cq_desc_dma, GFP_KERNEL);
    if (cq_virt_addr == NULL) {
        devdrv_err("dma channel[%d] cq alloc fail\n", dma_chan->chan_id);
        devdrv_free_dma_sq_cq(dma_chan);
        return -ENOMEM;
    }
    dma_chan->cq_desc_base = (struct devdrv_dma_cq_node *)cq_virt_addr;
    dma_chan->cq_depth = DEVDRV_MAX_DMA_CH_CQ_DEPTH;

    devdrv_debug("dma channel[%d]\n", dma_chan->chan_id);
    soft_virt_addr = (struct devdrv_dma_soft_bd *)vzalloc(soft_size);
    if (soft_virt_addr == NULL) {
        devdrv_err("dma channel[%d] cq alloc fail\n", dma_chan->chan_id);
        devdrv_free_dma_sq_cq(dma_chan);
        return -ENOMEM;
    }
    dma_chan->dma_soft_bd = soft_virt_addr;

    for (i = 0; i < DEVDRV_MAX_DMA_CH_CQ_DEPTH; i++) {
        soft_virt_addr->valid = DEVDRV_DISABLE;
    }

    return 0;
}

STATIC void devdrv_dma_interrupt_init_chan(struct devdrv_dma_dev *dma_dev, int channel_id, int dma_chan_id,
    struct devdrv_dma_chan_irq_info *dma_chan_irq_info)
{
    struct devdrv_dma_channel *dma_chan = &dma_dev->dma_chan[channel_id];
    int ret = 0;

    dma_chan->done_irq = (int)dma_chan_irq_info->done_irq;
    tasklet_init(&dma_chan->dma_done_task, devdrv_dma_done_task, (uintptr_t)dma_chan);
    (void)devdrv_register_irq_func(dma_dev->drvdata, dma_chan->done_irq, devdrv_dma_done_interrupt, dma_chan,
                                   "dma_done_irq");
    if (dma_chan_irq_info->err_irq_flag) {
        /* err interrupt we do some dfx words, so use wordqueue which can sleep */
        dma_chan->err_irq = (int)dma_chan_irq_info->err_irq;
        INIT_WORK(&dma_chan->err_work, devdrv_dma_err_task);
        (void)devdrv_register_irq_func(dma_dev->drvdata, dma_chan->err_irq, devdrv_dma_err_interrupt, dma_chan,
                                       "dma_err_irq");
    } else {
        dma_chan->err_irq = -1;
    }

    if (dma_dev->sq_cq_side == DEVDRV_DMA_REMOTE_SIDE) {
        ret = devdrv_notify_dma_err_irq(dma_dev->drvdata, dma_chan_id, dma_chan_irq_info->err_irq);
        if (ret) {
            devdrv_err("devid:%u, chan(%d) use dma chan(%d) notify err irq failed ret=%d\n", dma_dev->dev_id,
                       channel_id, dma_chan_id, ret);
        }
    }
    return;
}

int devdrv_dma_init_chan(struct devdrv_dma_dev *dma_dev, int channel_id, int dma_chan_id,
                         struct devdrv_dma_chan_irq_info *dma_chan_irq_info)
{
    int ret;
    u32 pf_num = 0;
    u32 vf_en = 0;
    u32 vf_num = 0;

    struct devdrv_dma_channel *dma_chan = &dma_dev->dma_chan[channel_id];

    dma_chan->dma_dev = dma_dev;
    dma_chan->dev = dma_dev->dev;
    dma_chan->io_base = dma_dev->io_base + (u64)dma_chan_id * DEVDRV_DMA_CHAN_OFFSET; //lint !e571
    /* reset DMA channel before init */
    ret = devdrv_dma_ch_cfg_reset(dma_chan->io_base);
    if (ret) {
        devdrv_err("devid:%u, chanid %d dma_ch_cfg_reset fail ret = %d\n", dma_dev->dev_id, channel_id, ret);
        return ret;
    }
    ret = devdrv_alloc_dma_sq_cq(dma_chan);
    if (ret) {
        devdrv_err("devid:%u, chanid %d alloc_dma_sq_cq failed\n", dma_dev->dev_id, channel_id);
        return ret;
    }
    dma_chan->local_id = (u32)channel_id;
    dma_chan->chan_id = (u32)dma_chan_id;
    dma_chan->sq_tail = 0;
    dma_chan->sq_head = 0;
    dma_chan->cq_head = dma_chan->cq_depth - 1;
    dma_chan->done_irq = -1;
    dma_chan->err_irq = -1;
    dma_chan->rounds = 0;
    dma_chan->func_id = dma_dev->func_id;

    if (dma_dev->sq_cq_side == DEVDRV_DMA_REMOTE_SIDE) {
        /* flags of DMA chan used in host */
        dma_chan->flag = (DEVDRV_DMA_REMOTE_SIDE << DEVDRV_DMA_SQCQ_SIDE_BIT) |
                         (DEVDRV_DISABLE << DEVDRV_DMA_SML_PKT_BIT);
    } else {
        /* flags of DMA chan used in device */
        dma_chan->flag = (DEVDRV_DMA_LOCAL_SIDE << DEVDRV_DMA_SQCQ_SIDE_BIT) |
                         (DEVDRV_ENABLE << DEVDRV_DMA_SML_PKT_BIT);
    }
    spin_lock_init(&dma_chan->lock);
    spin_lock_init(&dma_chan->cq_lock);

    devdrv_soc_func2vfpf(dma_chan->func_id, dma_dev->sq_cq_side, &vf_en, &pf_num, &vf_num);
    devdrv_dma_ch_cfg_init(dma_chan->io_base, dma_chan->sq_desc_dma, dma_chan->cq_desc_dma, dma_chan->sq_depth,
                           dma_chan->cq_depth, pf_num, devdrv_get_dma_sqcq_side(dma_chan));

    devdrv_dma_check_sram_init_status(dma_dev->io_base, DEVDRV_DMA_TIMEOUT);
    devdrv_dma_interrupt_init_chan(dma_dev, channel_id, dma_chan_id, dma_chan_irq_info);
    /* enable DMA channel */
    devdrv_set_dma_chan_en(dma_chan->io_base, 1);

    return ret;
}

void devdrv_res_dma_traffic(struct devdrv_dma_dev *dma_dev)
{
    /* init data type to dma chan map */
#ifndef CFG_SOC_PLATFORM_ESL_FPGA
    dma_dev->data_chan[DEVDRV_DMA_DATA_COMMON].chan_start_id = 0;
    dma_dev->data_chan[DEVDRV_DMA_DATA_COMMON].chan_num = DEVDRV_DMA_DATA_COMM_CHAN_NUM;
    dma_dev->data_chan[DEVDRV_DMA_DATA_COMMON].last_use_chan = dma_dev->data_chan[DEVDRV_DMA_DATA_COMMON].chan_start_id;

    dma_dev->data_chan[DEVDRV_DMA_DATA_PCIE_MSG].chan_start_id =
        dma_dev->data_chan[DEVDRV_DMA_DATA_COMMON].chan_start_id + dma_dev->data_chan[DEVDRV_DMA_DATA_COMMON].chan_num;
    dma_dev->data_chan[DEVDRV_DMA_DATA_PCIE_MSG].chan_num = DEVDRV_DMA_DATA_PCIE_MSG_CHAN_NUM;
    dma_dev->data_chan[DEVDRV_DMA_DATA_PCIE_MSG].last_use_chan =
        dma_dev->data_chan[DEVDRV_DMA_DATA_PCIE_MSG].chan_start_id;

    dma_dev->data_chan[DEVDRV_DMA_DATA_TRAFFIC].chan_start_id =
        dma_dev->data_chan[DEVDRV_DMA_DATA_PCIE_MSG].chan_start_id +
        dma_dev->data_chan[DEVDRV_DMA_DATA_PCIE_MSG].chan_num;
    dma_dev->data_chan[DEVDRV_DMA_DATA_TRAFFIC].chan_num =
        dma_dev->chan_count - (DEVDRV_DMA_DATA_COMM_CHAN_NUM + DEVDRV_DMA_DATA_PCIE_MSG_CHAN_NUM);
    dma_dev->data_chan[DEVDRV_DMA_DATA_TRAFFIC].last_use_chan =
        dma_dev->data_chan[DEVDRV_DMA_DATA_TRAFFIC].chan_start_id;
#else
    dma_dev->data_chan[DEVDRV_DMA_DATA_COMMON].chan_start_id = 0;
    dma_dev->data_chan[DEVDRV_DMA_DATA_COMMON].chan_num = 1;
    dma_dev->data_chan[DEVDRV_DMA_DATA_COMMON].last_use_chan = dma_dev->data_chan[DEVDRV_DMA_DATA_COMMON].chan_start_id;

    dma_dev->data_chan[DEVDRV_DMA_DATA_PCIE_MSG].chan_start_id = 0;
    dma_dev->data_chan[DEVDRV_DMA_DATA_PCIE_MSG].chan_num = 1;
    dma_dev->data_chan[DEVDRV_DMA_DATA_PCIE_MSG].last_use_chan =
        dma_dev->data_chan[DEVDRV_DMA_DATA_PCIE_MSG].chan_start_id;

    dma_dev->data_chan[DEVDRV_DMA_DATA_TRAFFIC].chan_start_id = 0;
    dma_dev->data_chan[DEVDRV_DMA_DATA_TRAFFIC].chan_num = 1;
    dma_dev->data_chan[DEVDRV_DMA_DATA_TRAFFIC].last_use_chan =
        dma_dev->data_chan[DEVDRV_DMA_DATA_TRAFFIC].chan_start_id;
#endif
}

struct devdrv_dma_dev *devdrv_dma_init(struct devdrv_dma_func_para *para_in, u32 sq_cq_side, u32 func_id)
{
    struct devdrv_dma_dev *dma_dev = NULL;
    struct devdrv_dma_chan_irq_info dma_chan_irq_info;
    u32 dma_dev_size;
    u32 i;

    /* 1 check dlcmsm */
    if (devdrv_check_dl_dlcmsm_state(para_in->drvdata) != 0) {
        devdrv_err("check dlcmsm state fail\n");
        return NULL;
    }

    /* 2 create dma dev */
    dma_dev_size = sizeof(struct devdrv_dma_dev) + sizeof(struct devdrv_dma_channel) * para_in->chan_num;
    dma_dev = (struct devdrv_dma_dev *)kzalloc(dma_dev_size, GFP_KERNEL);
    if (dma_dev == NULL) {
        devdrv_err("devid:%u, dma dev alloc failed!\n", para_in->dev_id);
        return NULL;
    }

    /* 3 init dma dev */
    dma_dev->dev = para_in->dev;
    dma_dev->io_base = para_in->io_base;
    dma_dev->drvdata = para_in->drvdata;
    dma_dev->sq_cq_side = sq_cq_side;
    dma_dev->chan_count = para_in->chan_num;
    dma_dev->dev_id = para_in->dev_id;
    dma_dev->func_id = func_id;
    dma_dev->dev_status = DEVDRV_DMA_ALIVE;

    for (i = 0; i < dma_dev->chan_count; i++) {
        devdrv_debug("dev %d fun %d, dma chan %d\n", dma_dev->dev_id, func_id, para_in->chan_begin + i);
        dma_chan_irq_info.done_irq = para_in->done_irq_base + i;
        dma_chan_irq_info.err_irq = para_in->err_irq_base + i;
        dma_chan_irq_info.err_irq_flag = para_in->err_flag;
        if (devdrv_dma_init_chan(dma_dev, i, (para_in->chan_begin + i), &dma_chan_irq_info) != 0) {
            devdrv_err("devid:%u, dma init chan %d fail\n", para_in->dev_id, i);
            devdrv_dma_exit(dma_dev);
            return NULL;
        }
    }

    (void)devdrv_res_dma_traffic(dma_dev);

    return dma_dev;
}

void devdrv_dma_exit(struct devdrv_dma_dev *dma_dev)
{
    u32 i;
    struct devdrv_dma_channel *dma_chan = NULL;

    if (dma_dev == NULL) {
        return;
    }

    for (i = 0; i < dma_dev->chan_count; i++) {
        dma_chan = &dma_dev->dma_chan[i];

        /* chan has not init */
        if (dma_chan->dev == NULL) {
            continue;
        }

        if (dma_chan->err_irq >= 0) {
            cancel_work_sync(&dma_chan->err_work);
            devdrv_unregister_irq_func(dma_dev->drvdata, dma_chan->err_irq, dma_chan);
        }

        if (dma_chan->done_irq > 0) {
            tasklet_kill(&dma_chan->dma_done_task);
            devdrv_unregister_irq_func(dma_dev->drvdata, dma_chan->done_irq, dma_chan);
        }

        (void)devdrv_dma_ch_cfg_reset(dma_chan);

        devdrv_free_dma_sq_cq(dma_chan);
    }
    kfree(dma_dev);
    dma_dev = NULL;
}
