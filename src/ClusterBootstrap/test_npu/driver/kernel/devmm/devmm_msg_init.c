/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2012-2019. All rights reserved.
 * Description: This file contains api definition for memory manager driver.
 * Author: huawei
 * Create: 2019-06-11
 */

#include <linux/pci.h>
#include <linux/types.h>

#include "devmm_dev.h"
#include "devmm_proc_info.h"
#include "devmm_channel.h"
#include "devmm_msg_init.h"
#include "devdrv_interface.h"

/*
 * devmm register msg process
 */
STATIC void *devmm_msg_queues[DEVMM_MAX_DEVICE_NUM] = {NULL};
STATIC struct device *devmm_devices[DEVMM_MAX_DEVICE_NUM] = {NULL};
STATIC struct devdrv_client_instance *devmm_client_instance[DEVMM_MAX_DEVICE_NUM] = {NULL};
STATIC struct devmm_init_work devmm_rdy_work[DEVMM_MAX_DEVICE_NUM] = {{0}};
STATIC struct rw_semaphore devmm_chan_sem[DEVMM_MAX_DEVICE_NUM];

void *devmm_devid_to_msg_chan(u32 dev_id)
{
    if (dev_id >= DEVMM_MAX_DEVICE_NUM) {
        devmm_drv_err("devid invalid, did:%d.\n", dev_id);
        return NULL;
    }

    return devmm_msg_queues[dev_id];
}

struct device *devmm_devid_to_device(u32 dev_id)
{
    if (dev_id >= DEVMM_MAX_DEVICE_NUM) {
        devmm_drv_err("dev_id invalid,dev_id:%d \n", dev_id);
        return NULL;
    }
    return devmm_devices[dev_id];
}

unsigned int devmm_devid_to_busnumber(u32 dev_id)
{
    if (devmm_client_instance[dev_id] != NULL)
        return devmm_client_instance[dev_id]->dev_ctrl->bus->number;
    return (unsigned int)(-1);
}

STATIC int devmm_rx_msg_process(void *msg_chan, void *data, u32 in_data_len, u32 out_data_len, u32 *real_out_len)
{
    struct devdrv_client_instance *client = (struct devdrv_client_instance *)devdrv_get_msg_chan_priv(msg_chan);
    u32 did;

    if ((data == NULL) || (real_out_len == NULL) || (client == NULL) || (client->dev_ctrl == NULL)) {
        devmm_drv_err("data(%pK) or out_len(%pK) or client(%pK) or dev_ctrl is null.\n", data, real_out_len, client);
        return -EINVAL;
    }

    if (in_data_len < sizeof(struct devmm_chan_msg_head)) {
        devmm_drv_err("invalid in_data_len(%u).\n", in_data_len);
        return -EINVAL;
    }

    devmm_svm_stat_recv_inc();
    did = client->dev_ctrl->dev_id;
    if (did >= DEVMM_MAX_DEVICE_NUM) {
        devmm_drv_err("deivice id (%d) must less than %d\n", did, DEVMM_MAX_DEVICE_NUM);
        return -EINVAL;
    }
    ((struct devmm_chan_msg_head *)data)->dev_id = did;

    if (((struct devmm_chan_msg_head *)data)->magic != DEVMM_CHANNEL_MAGIC) {
        devmm_drv_err("magic=0x%08x.\n", ((struct devmm_chan_msg_head *)data)->magic);
        return -EINVAL;
    }

    return devmm_chan_msg_dispatch(data, in_data_len, out_data_len, real_out_len);
}

#define DEVMM_NON_TRANS_MSG_DESC_SIZE 10240
struct devdrv_non_trans_msg_chan_info devmm_msg_chan_info = {
    .msg_type = devdrv_msg_client_devmm,
    .flag = 0,
    .level = DEVDRV_MSG_CHAN_LEVEL_LOW,
    .s_desc_size = DEVMM_NON_TRANS_MSG_DESC_SIZE,
    .c_desc_size = DEVMM_NON_TRANS_MSG_DESC_SIZE,
    .rx_msg_process = devmm_rx_msg_process,
};

STATIC int devmm_rx_common_msg_process(u32 dev_id, void *data, u32 in_data_len, u32 out_data_len, u32 *real_out_len)
{
    devmm_svm_stat_recv_inc();
    /* host recive mag: get devid from msg-chan-struct* */
    if ((data == NULL) || (real_out_len == NULL)) {
        devmm_drv_err("data(%pK) or out_len(%pK) is NULL.\n", data, real_out_len);
        return -EINVAL;
    }

    if (in_data_len < sizeof(struct devmm_chan_msg_head)) {
        devmm_drv_err("invalid in_data_len(%u).\n", in_data_len);
        return -EINVAL;
    }

    if (dev_id >= DEVMM_MAX_DEVICE_NUM) {
        devmm_drv_err("deivice id (%d) must less than %d\n", dev_id, DEVMM_MAX_DEVICE_NUM);
        return -EINVAL;
    }

    ((struct devmm_chan_msg_head *)data)->dev_id = dev_id;

    if (((struct devmm_chan_msg_head *)data)->magic != DEVMM_CHANNEL_MAGIC) {
        devmm_drv_err("magic=0x%08x.\n", ((struct devmm_chan_msg_head *)data)->magic);
        return -EINVAL;
    }

    return devmm_chan_msg_dispatch(data, in_data_len, out_data_len, real_out_len);
}

STATIC int devmm_alloc_msg_chan_process(u32 dev_id)
{
    if (devmm_msg_queues[dev_id] != NULL) {
        devmm_drv_err("devmm has alloced msg tran.did=%d\n", dev_id);
        return -EINVAL;
    }

    devmm_msg_queues[dev_id] = devdrv_pcimsg_alloc_non_trans_queue(dev_id, &devmm_msg_chan_info);

    if (devmm_msg_queues[dev_id] == NULL) {
        devmm_drv_err("devmm malloc msg tran fail.did=%d.\n", dev_id);
        return -EINVAL;
    }

    if (devdrv_set_msg_chan_priv(devmm_msg_queues[dev_id], (void *)devmm_client_instance[dev_id])) {
        devmm_drv_err("devmm set msg tran priv fail.did=%d.\n", dev_id);
        return -EINVAL;
    }
    devmm_drv_info("host non-tran msg chan has inited, did=%d.\n", dev_id);
    return 0;
}

int devmm_channel_set_ready(u32 dev_id)
{
    if (dev_id >= DEVMM_MAX_DEVICE_NUM) {
        devmm_drv_err("host channel set did[%d] EINVAL\n", dev_id);
        return -EINVAL;
    }
    if (devmm_client_instance[dev_id] == NULL) {
        devmm_drv_err("host channel did[%d] not inited\n", dev_id);
        return -EINVAL;
    }
    return devmm_alloc_msg_chan_process(dev_id);
}

STATIC void devmm_free_msg_chan_process(u32 did)
{
    if (devmm_msg_queues[did] != NULL) {
        devdrv_pcimsg_free_non_trans_queue(devmm_msg_queues[did]);
    }
}

STATIC void devmm_common_msg_chan_notify(u32 dev_id, int status)
{
    if (status) {
        devmm_drv_info("host common msg chan is initing dev[%d].\n", dev_id);
        return;
    }
}

STATIC void devmm_dev_ready_work(struct work_struct *work)
{
    struct devmm_init_work *p_work = container_of(work, struct devmm_init_work, init_work.work);
    unsigned int devid = p_work->devid;

    if (devid >= DEVMM_MAX_DEVICE_NUM) {
        devmm_drv_err("devmm work can not find devid[%d]\n", devid);
        return;
    }

    /* send pagesize msg to device. */
    if (devmm_host_broadcast_pginfo(devid) != 0) {
        if (devmm_rdy_work[devid].times < DEVMM_MSG_INIT_SEND_TRYTIMES) {
            devmm_rdy_work[devid].times++;
            devmm_drv_info("device[%d] is not ready,try %d.", devid, devmm_rdy_work[devid].times);
            schedule_delayed_work(&devmm_rdy_work[devid].init_work, msecs_to_jiffies(DEVMM_MSG_INIT_SEND_WAITTIME));
        } else {
            devmm_drv_err("devmm dev[%d] msg channel init timeout", devid);
        }
    } else {
        if (devmm_channel_set_ready(devid) != 0) {
            devmm_drv_info("host to device[%d] channel is not ready.", devid);
        } else {
            devmm_drv_info("devmm dev[%d] msg channel is ready", devid);
            (void)devdrv_set_module_init_finish(devid, DEVDRV_HOST_MODULE_DEVMM);
        }
    }
}

STATIC struct devdrv_common_msg_client devmm_common_msg_client = {
    .type = DEVDRV_COMMON_MSG_DEVMM,
    .common_msg_recv = devmm_rx_common_msg_process,
    .init_notify = devmm_common_msg_chan_notify,
};

STATIC int devmm_init_instance(struct devdrv_client_instance *dev_ctrl)
{
    struct device *dev = NULL;
    u32 did;
    int ret;

    if ((dev_ctrl == NULL) || (dev_ctrl->dev_ctrl == NULL)) {
        devmm_drv_err("dev_ctrl(%d) or dev_ctrl->dev_ctrl is NULL.\n", (dev_ctrl == NULL));
        return -EINVAL;
    }

    dev = dev_ctrl->dev_ctrl->dev;
    did = dev_ctrl->dev_ctrl->dev_id;

    if ((did >= DEVMM_MAX_DEVICE_NUM) || (dev == NULL)) {
        devmm_drv_err("dev id is wrong or dev is null,did=%d, dev=%pK.\n", did, dev);
        return -EINVAL;
    }
    down_write(&devmm_chan_sem[did]);
    devmm_devices[did] = dev;
    devmm_client_instance[did] = dev_ctrl;
    ret = devdrv_register_common_msg_client(&devmm_common_msg_client);
    if (ret) {
        up_write(&devmm_chan_sem[did]);
        devmm_drv_err("devdrv register common msg channel failed ret=%d\n", ret);
        return ret;
    }
    devmm_drv_info("devmm msg chan will init.did=%d.\n", did);
    devmm_rdy_work[did].devid = did;
    INIT_DELAYED_WORK(&devmm_rdy_work[did].init_work, devmm_dev_ready_work);
    schedule_delayed_work(&devmm_rdy_work[did].init_work, msecs_to_jiffies(DEVMM_MSG_INIT_SEND_WAITTIME));
    up_write(&devmm_chan_sem[did]);
    return 0;
}

STATIC int devmm_uninit_instance(struct devdrv_client_instance *dev_ctrl)
{
    u32 did;
    int ret;

    if ((dev_ctrl == NULL) || (dev_ctrl->dev_ctrl == NULL)) {
        devmm_drv_err("dev_ctrl(%d) or dev_ctrl->dev_ctrl is NULL.\n", (dev_ctrl == NULL));
        return -EINVAL;
    }

    did = dev_ctrl->dev_ctrl->dev_id;
    if (did >= DEVMM_MAX_DEVICE_NUM) {
        devmm_drv_err("dev id is wrong did=%d.\n", did);
        return -EINVAL;
    }

    devmm_drv_info("devmm msg chan will uninit.did=%d.\n", did);

    down_write(&devmm_chan_sem[did]);
    cancel_delayed_work_sync(&devmm_rdy_work[did].init_work);
    devmm_rdy_work[did].times = 0;
    ret = devdrv_unregister_common_msg_client(did, &devmm_common_msg_client);
    if (ret) {
        devmm_drv_err("dev(%u) unregister common msg client fail, ret = %d.\n", did, ret);
        up_write(&devmm_chan_sem[did]);
        return ret;
    }

    devmm_free_msg_chan_process(did);

    if (devdrv_set_msg_chan_priv(devmm_msg_queues[did], NULL)) {
        devmm_drv_err("devmm clear msg tran priv fail.did=%d.\n", did);
        up_write(&devmm_chan_sem[did]);
        return -EINVAL;
    }
    devmm_msg_queues[did] = NULL;
    devmm_devices[did] = NULL;
    devmm_client_instance[did] = NULL;
    up_write(&devmm_chan_sem[did]);
    return 0;
}

STATIC struct devdrv_client devmm_host_instance = {
    .type = DEVDRV_CLIENT_DEVMM,
    .init_instance = devmm_init_instance,
    .uninit_instance = devmm_uninit_instance,
};

int devmm_msg_chan_init(void)
{
    int i;

    for (i = 0; i < DEVMM_MAX_DEVICE_NUM; i++) {
        init_rwsem(&devmm_chan_sem[i]);
    }
    return devdrv_register_client(&devmm_host_instance);
}

void devmm_msg_chan_uninit(void)
{
    devdrv_unregister_client(&devmm_host_instance);
}

int devmm_chan_msg_send(void *msg, unsigned int len, unsigned int out_len)
{
    struct devmm_chan_msg_head *tmp_msg = (struct devmm_chan_msg_head *)msg;
    u32 did = tmp_msg->dev_id;
    int ret;

    tmp_msg->magic = DEVMM_CHANNEL_MAGIC;

    if ((did >= DEVMM_MAX_DEVICE_NUM) || (devmm_msg_queues[did] == NULL)) {
        devmm_drv_err("host send device[%d] get chan is NULL(%d) err.\n", did,
                      did >= DEVMM_MAX_DEVICE_NUM ? 1 : (devmm_msg_queues[did] == NULL));
        return -EINVAL;
    }
    devmm_svm_stat_send_inc();
    down_read(&devmm_chan_sem[did]);
    ret = devdrv_sync_msg_send(devmm_msg_queues[did], msg, len, out_len, &out_len);
    up_read(&devmm_chan_sem[did]);
    if (ret) {
        devmm_drv_err("devdrv_sync_msg_send failed, ret = %d.\n", ret);
    }

    return ret;
}

int devmm_common_msg_send(void *msg, unsigned int len, unsigned int out_len)
{
    struct devmm_chan_msg_head *tmp_msg = NULL;
    u32 did;

    if (msg == NULL) {
        devmm_drv_err("msg is NULL.\n");
        return -EINVAL;
    }

    tmp_msg = (struct devmm_chan_msg_head *)msg;
    did = tmp_msg->dev_id;
    tmp_msg->magic = DEVMM_CHANNEL_MAGIC;

    if (did >= DEVMM_MAX_DEVICE_NUM) {
        devmm_drv_err("host send device[%d] error.\n", did);
        return -EINVAL;
    }
    devmm_svm_stat_send_inc();
    return devdrv_common_msg_send(did, msg, len, out_len, &out_len, DEVDRV_COMMON_MSG_DEVMM);
}

