/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2012-2019. All rights reserved.
 * Description:
 * Author: huawei
 * Create: 2017/8/18
 */

#include <linux/delay.h>
#include <asm/errno.h>
#include <linux/kernel.h>
#include <linux/string.h>
#include <linux/types.h>
#include <linux/pci.h>

#include "devdrv_ctrl.h"
#include "devdrv_dma.h"
#include "devdrv_msg.h"
#include "devdrv_common_msg.h"
#include "devdrv_msg_def.h"
#include "devdrv_util.h"

struct devdrv_msg_chan *devdrv_get_common_msg_chan_by_id(u32 dev_id)
{
    struct devdrv_pci_ctrl *pci_ctrl = NULL;
    struct devdrv_ctrl *ctrl = NULL;

    if (dev_id >= MAX_DEV_CNT) {
        devdrv_err("invalid dev id %d , id overflow!\n", dev_id);
        return NULL;
    }

    ctrl = devdrv_get_bottom_half_devctrl_by_id(dev_id);
    if ((ctrl == NULL) || (ctrl->priv == NULL)) {
        devdrv_err("find msg_chan by id failed, dev_id: %d\n", dev_id);
        return NULL;
    }
    pci_ctrl = (struct devdrv_pci_ctrl *)ctrl->priv;

    return pci_ctrl->msg_dev->common_msg.msg_chan;
}

int rx_msg_common_msg_process(void *msg_chan, void *data, u32 in_data_len, u32 out_data_len,
    u32 *real_out_len)
{
    struct devdrv_non_trans_msg_desc *msg_desc = NULL;
    struct devdrv_msg_chan *chan = (struct devdrv_msg_chan *)msg_chan;
    int ret;
    u32 cost_time;

    if (chan == NULL) {
        devdrv_err("msg_chan is null\n");
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

    msg_desc = container_of(data, struct devdrv_non_trans_msg_desc, data);
    if (msg_desc->msg_type >= (u32)DEVDRV_COMMON_MSG_TYPE_MAX) {
        devdrv_err("devid:%u, msg_type is invalid!\n", chan->msg_dev->pci_ctrl->dev_id);
        return -EINVAL;
    }
    if (chan->msg_dev->common_msg.common_fun[msg_desc->msg_type] == NULL) {
        devdrv_warn("devid:%u, common_type:%d, rx callback func is null\n", chan->msg_dev->pci_ctrl->dev_id,
                    msg_desc->msg_type);
        return -EINVAL;
    }

    cost_time = jiffies_to_msecs(jiffies - chan->stamp);
    if (cost_time > chan->msg_dev->common_msg.com_msg_stat[msg_desc->msg_type].rx_work_max_time) {
        chan->msg_dev->common_msg.com_msg_stat[msg_desc->msg_type].rx_work_max_time = cost_time;
    }
    if (cost_time > DEVDRV_COMMON_WORK_RESQ_TIME) {
        chan->msg_dev->common_msg.com_msg_stat[msg_desc->msg_type].rx_work_delay_cnt++;
        devdrv_info("devid:%u, msg_type %d:%u ms\n", chan->msg_dev->pci_ctrl->dev_id, msg_desc->msg_type, cost_time);
    }

    chan->msg_dev->common_msg.com_msg_stat[msg_desc->msg_type].rx_total_cnt++;
    ret = chan->msg_dev->common_msg.common_fun[msg_desc->msg_type](devdrv_get_devid_by_dev(chan->msg_dev), data,
                                                                   in_data_len, out_data_len, real_out_len);
    if (ret == 0) {
        chan->msg_dev->common_msg.com_msg_stat[msg_desc->msg_type].rx_success_cnt++;
    }

    return ret;
}

int devdrv_common_msg_send(u32 devid, void *data, u32 in_data_len, u32 out_data_len, u32 *real_out_len,
                           enum devdrv_common_msg_type msg_type)
{
    struct devdrv_msg_chan *msg_chan = NULL;
    struct devdrv_common_msg_stat *com_msg_stat = NULL;
    int msg_type_tmp;
    int ret;

    msg_type_tmp = (int)msg_type;
    if (devid >= MAX_DEV_CNT) {
        devdrv_err("invalid dev id %d, id overflow!\n", devid);
        return -EINVAL;
    }
    if (data == NULL) {
        devdrv_err("devid %d data is null\n", devid);
        return -EINVAL;
    }
    if (real_out_len == NULL) {
        devdrv_err("devid %d real_out_len is null\n", devid);
        return -EINVAL;
    }
    if ((msg_type_tmp < DEVDRV_COMMON_MSG_PCIVNIC) || (msg_type_tmp >= DEVDRV_COMMON_MSG_TYPE_MAX)) {
        devdrv_err("devid %d msg_type %d is invalid\n", devid, msg_type_tmp);
        return -EINVAL;
    }

    msg_chan = devdrv_get_common_msg_chan_by_id(devid);
    if (msg_chan == NULL) {
        devdrv_warn("devid %d msg_chan is NULL, please wait for create common msg que\n", devid);
        return -EINVAL;
    }

    com_msg_stat = &(msg_chan->msg_dev->common_msg.com_msg_stat[msg_type]);
    com_msg_stat->tx_total_cnt++;

    ret = devdrv_sync_non_trans_msg_send(msg_chan, data, in_data_len, out_data_len, real_out_len, msg_type);
    if (ret == 0) {
        com_msg_stat->tx_success_cnt++;
    } else if (ret == -EINVAL) {
        com_msg_stat->tx_einval_err++;
    } else if (ret == -ENODEV) {
        com_msg_stat->tx_enodev_err++;
    } else if (ret == -ENOSYS) {
        com_msg_stat->tx_enosys_err++;
    } else if (ret == -ETIMEDOUT) {
        com_msg_stat->tx_etimedout_err++;
    } else {
        com_msg_stat->tx_default_err++;
    }

    return ret;
}
EXPORT_SYMBOL(devdrv_common_msg_send);

int devdrv_register_common_msg_client(struct devdrv_common_msg_client *msg_client)
{
    struct devdrv_ctrl *ctrl = NULL;
    struct devdrv_pci_ctrl *pci_ctrl = NULL;
    u32 i;

    if (msg_client == NULL) {
        devdrv_err("msg_client is null\n");
        return -EINVAL;
    }

    if (msg_client->type >= DEVDRV_COMMON_MSG_TYPE_MAX) {
        devdrv_err("msg_client type %d is error\n", msg_client->type);
        return -EINVAL;
    }

    for (i = 0; i < MAX_DEV_CNT; i++) {
        ctrl = devdrv_get_devctrl_by_id(i);
        if (ctrl == NULL) {
            continue;
        }
        pci_ctrl = (struct devdrv_pci_ctrl *)ctrl->priv;
        if (pci_ctrl == NULL) {
            continue;
        }
        if (ctrl->startup_flg != DEVDRV_DEV_STARTUP_BOTTOM_HALF_OK) {
            continue;
        }
        pci_ctrl->msg_dev->common_msg.common_fun[msg_client->type] = msg_client->common_msg_recv;
        if (msg_client->init_notify) {
            devdrv_debug("register_common_msg_client, type:%d, devid:%d, before init_notify!\n", msg_client->type,
                         pci_ctrl->dev_id);
            msg_client->init_notify(pci_ctrl->dev_id, 0);
            devdrv_debug("register_common_msg_client,after init_notify!\n");
        }
    }

    return 0;
}
EXPORT_SYMBOL(devdrv_register_common_msg_client);

int devdrv_unregister_common_msg_client(u32 devid, struct devdrv_common_msg_client *msg_client)
{
    struct devdrv_pci_ctrl *pci_ctrl = NULL;
    struct devdrv_ctrl *ctrl = NULL;

    if (devid >= MAX_DEV_CNT) {
        devdrv_err("invalid dev id %d, id overflow!\n", devid);
        return -EINVAL;
    }
    if (msg_client == NULL) {
        devdrv_err("devid %d msg_client is null\n", devid);
        return -EINVAL;
    }

    if (msg_client->type >= DEVDRV_COMMON_MSG_TYPE_MAX) {
        devdrv_err("devid %d msg_client type %d is error\n", devid, msg_client->type);
        return -EINVAL;
    }

    ctrl = devdrv_get_bottom_half_devctrl_by_id(devid);
    if (ctrl == NULL) {
        devdrv_err("unregister_common_msg_client,ctrl is null,devid:%d, type:%d!\n", devid, msg_client->type);
        return -EINVAL;
    }
    pci_ctrl = ctrl->priv;
    pci_ctrl->msg_dev->common_msg.common_fun[msg_client->type] = NULL;
    devdrv_debug("unregister_common_msg_client, devid:%d, type:%d.\n", devid, msg_client->type);

    return 0;
}
EXPORT_SYMBOL(devdrv_unregister_common_msg_client);

struct devdrv_non_trans_msg_chan_info g_common_msg_chan_info = {
    .msg_type = devdrv_msg_client_common,
    .flag = DEVDRV_MSG_SYNC,
    .level = DEVDRV_MSG_CHAN_LEVEL_LOW,
    .s_desc_size = DEVDRV_NON_TRANS_MSG_DEFAULT_DESC_SIZE,
    .c_desc_size = DEVDRV_NON_TRANS_MSG_DEFAULT_DESC_SIZE,
    .rx_msg_process = rx_msg_common_msg_process,
};

int devdrv_alloc_common_msg_queue(struct devdrv_pci_ctrl *pci_ctrl)
{
    void *msg_chan = NULL;

    msg_chan = devdrv_pcimsg_alloc_non_trans_queue(pci_ctrl->dev_id, &g_common_msg_chan_info);
    if (msg_chan == NULL) {
        devdrv_err("devid %d alloc_common_msg_queue, msg_chan is null\n", pci_ctrl->dev_id);
        return -EINVAL;
    }

    /* save common msg_chan to msg_dev */
    pci_ctrl->msg_dev->common_msg.msg_chan = (struct devdrv_msg_chan *)msg_chan;

    return 0;
}

void devdrv_free_common_msg_queue(struct devdrv_pci_ctrl *pci_ctrl)
{
    int ret;
    if ((pci_ctrl->msg_dev == NULL) || (pci_ctrl->msg_dev->common_msg.msg_chan == NULL)) {
        devdrv_info("devid %u msg_dev or msg_chan is null,just return.\n", pci_ctrl->dev_id);
        return;
    }
    ret = devdrv_pcimsg_free_non_trans_queue((void *)(pci_ctrl->msg_dev->common_msg.msg_chan));
    if (ret) {
        devdrv_info("devid %u free_common_msg_queue fail, only free host resource ret = %d.\n", pci_ctrl->dev_id, ret);
    }
    devdrv_info("devid %u free common msg queue success.\n", pci_ctrl->dev_id);
    pci_ctrl->msg_dev->common_msg.msg_chan = NULL;

    return;
}
