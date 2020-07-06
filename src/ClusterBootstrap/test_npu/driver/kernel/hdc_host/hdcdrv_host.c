/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2012-2019. All rights reserved.
 * Description:
 * Author: huawei
 * Create: 2019-6-18
 *
 * This program is free software; you can redistribute it and /or modify it
 * under the terms of the GNU General Public License as published by the Free
 * Software Foundation; either version 2 of the License, or (at your option)
 * any later version
 */
#include <linux/init.h>
#include <linux/module.h>
#include <linux/kallsyms.h>

#include <linux/errno.h>
#include <linux/pci.h>
#include <linux/jiffies.h>
#include <linux/delay.h>
#include <linux/nsproxy.h>

#include "hdcdrv_core.h"
#include "hdcdrv_host.h"

container_virtual_to_physical_devid hdcdrv_container_virtual_to_physical_devid = NULL;
devdrv_manager_get_run_env devdrv_manager_get_run_env_func = NULL;
hdcdrv_is_in_container hdcdrv_is_in_container_func = NULL;
get_container_id hdcdrv_get_container_id_func = NULL;


static const struct pci_device_id hdcdrv_tbl[] = {
    { PCI_VDEVICE(HUAWEI, 0xd100), 0 },
    { PCI_VDEVICE(HUAWEI, PCI_DEVICE_CLOUD), 0 },
    { PCI_VDEVICE(HUAWEI, 0xd801), 0 },
    {}};
MODULE_DEVICE_TABLE(pci, hdcdrv_tbl);
int normal_chan_cnt;
int packet_segment;
int running_env;

int hdcdrv_get_running_env(void)
{
    return running_env;
}

enum devdrv_dma_direction hdcdrv_get_dma_direction(void)
{
    return DEVDRV_DMA_DEVICE_TO_HOST;
}

int hdcdrv_set_msg_chan_priv(void *msg_chan, void *priv)
{
    return devdrv_set_msg_chan_priv(msg_chan, priv);
}

struct hdcdrv_msg_chan *hdcdrv_get_msg_chan_priv(void *msg_chan)
{
    return (struct hdcdrv_msg_chan *)devdrv_get_msg_chan_priv(msg_chan);
}

struct hdcdrv_sq_desc *hdcdrv_get_w_sq_desc(void *msg_chan, u32 *tail)
{
    return (struct hdcdrv_sq_desc *)devdrv_get_msg_chan_slave_sq_tail(msg_chan, tail);
}

void hdcdrv_set_w_sq_desc_head(void *msg_chan, u32 head)
{
    devdrv_set_msg_chan_slave_sq_head(msg_chan, head);
}

void hdcdrv_copy_sq_desc_to_remote(struct hdcdrv_msg_chan *msg_dev, struct hdcdrv_sq_desc *sq_desc,
                                   enum devdrv_dma_data_type data_type)
{
    void *msg_chan = msg_dev->chan;

    (void)sq_desc;
    (void)data_type;
    devdrv_move_msg_chan_slave_sq_tail(msg_chan);

    /* shared memory without copying */
    wmb();

    /* trigger doorbell irq */
    devdrv_msg_ring_doorbell(msg_chan);
}

bool hdcdrv_w_sq_full_check(void *msg_chan)
{
    return devdrv_msg_chan_slave_sq_full_check(msg_chan);
}

struct hdcdrv_sq_desc *hdcdrv_get_r_sq_desc(void *msg_chan, u32 *head)
{
    return (struct hdcdrv_sq_desc *)devdrv_get_msg_chan_host_sq_head(msg_chan, head);
}

void hdcdrv_move_r_sq_desc(void *msg_chan)
{
    devdrv_move_msg_chan_host_sq_head(msg_chan);
}

struct hdcdrv_cq_desc *hdcdrv_get_w_cq_desc(void *msg_chan)
{
    return (struct hdcdrv_cq_desc *)devdrv_get_msg_chan_slave_cq_tail(msg_chan);
}

void hdcdrv_copy_cq_desc_to_remote(struct hdcdrv_msg_chan *msg_dev, struct hdcdrv_cq_desc *cq_desc,
                                   enum devdrv_dma_data_type data_type)
{
    void *msg_chan = msg_dev->chan;

    (void)cq_desc;
    (void)data_type;
    devdrv_move_msg_chan_slave_cq_tail(msg_chan);

    /* shared memory without copying */
    wmb();

    /* trigger doorbell irq */
    devdrv_msg_ring_cq_doorbell(msg_chan);
}

struct hdcdrv_cq_desc *hdcdrv_get_r_cq_desc(void *msg_chan)
{
    return (struct hdcdrv_cq_desc *)devdrv_get_msg_chan_host_cq_head(msg_chan);
}

void hdcdrv_move_r_cq_desc(void *msg_chan)
{
    devdrv_move_msg_chan_host_cq_head(msg_chan);
}

long hdcdrv_ctrl_msg_send(u32 devid, void *data, u32 in_data_len, u32 out_data_len, u32 *real_out_len)
{
    return (long)devdrv_common_msg_send(devid, data, in_data_len, out_data_len, real_out_len, DEVDRV_COMMON_MSG_HDC);
}

long hdcdrv_non_trans_ctrl_msg_send(u32 devid, void *data, u32 in_data_len, u32 out_data_len, u32 *real_out_len)
{
    struct hdcdrv_dev *hdc_dev = &hdc_ctrl->devices[devid];

    return (long)devdrv_sync_msg_send(hdc_dev->ctrl_msg_chan, data, in_data_len, out_data_len, real_out_len);
}

int hdcdrv_non_trans_ctrl_msg_recv(void *msg_chan, void *data, u32 in_data_len, u32 out_data_len, u32 *real_out_len)
{
    u32 devid = devdrv_get_msg_chan_devid(msg_chan);
    return hdcdrv_ctrl_msg_recv(devid, data, in_data_len, out_data_len, real_out_len);
}

struct devdrv_common_msg_client hdcdrv_host_comm_msg_client = {
    .type = DEVDRV_COMMON_MSG_HDC,
    .common_msg_recv = hdcdrv_ctrl_msg_recv,
};

struct devdrv_non_trans_msg_chan_info hdcdrv_non_trans_msg_chan_info = {
    .msg_type = devdrv_msg_client_hdc,
    .flag = 0,
    .level = DEVDRV_MSG_CHAN_LEVEL_HIGH,
    .s_desc_size = HDCDRV_NON_TRANS_MSG_S_DESC_SIZE,
    .c_desc_size = HDCDRV_NON_TRANS_MSG_C_DESC_SIZE,
    .rx_msg_process = hdcdrv_non_trans_ctrl_msg_recv,
};

struct devdrv_trans_msg_chan_info hdcdrv_msg_chan_info = {
    .msg_type = devdrv_msg_client_hdc,
    .queue_depth = HDCDRV_DESC_QUEUE_DEPTH,
    .level = DEVDRV_MSG_CHAN_LEVEL_LOW,
    .sq_desc_size = sizeof(struct hdcdrv_sq_desc),
    .cq_desc_size = sizeof(struct hdcdrv_cq_desc),
    .rx_msg_notify = hdcdrv_rx_msg_notify,
    .tx_finish_notify = hdcdrv_tx_finish_notify,
};

STATIC int hdcdrv_init_msg_chan(u32 dev_id)
{
    int i = 0;
    int j = 0;
    void *msg_chan = NULL;
    struct hdcdrv_dev *hdc_dev = NULL;
    struct hdcdrv_ctrl_msg msg;
    u32 len = 0;
    int msg_chan_cnt = devdrv_get_hdc_surport_msg_chan_cnt(dev_id);
    char *msgChanTmp[HDCDRV_SURPORT_MAX_DEV_MSG_CHAN] = {NULL};
    long ret;

    if ((msg_chan_cnt > HDCDRV_SURPORT_MAX_DEV_MSG_CHAN) || (msg_chan_cnt <= 0)) {
        hdcdrv_err("msg_chan_cnt is too large %d.\n", msg_chan_cnt);
        return HDCDRV_ERR;
    }

    hdcdrv_set_device_para(dev_id, normal_chan_cnt);
    msg.type = HDCDRV_CTRL_MSG_TYPE_CHAN_SET;
    msg.chan_set_msg.normal_chan_num = normal_chan_cnt;
    msg.error_code = HDCDRV_OK;

    ret = hdcdrv_ctrl_msg_send(dev_id, (void *)&msg, sizeof(msg), sizeof(msg), &len);
    if ((ret != HDCDRV_OK) || (len != sizeof(msg)) || (msg.error_code != HDCDRV_OK)) {
        hdcdrv_err("dev_id %d chan set msg send failed.\n", dev_id);
        return HDCDRV_ERR;
    }

    for (i = 0; i < msg_chan_cnt; i++) {
#ifdef CFG_SOC_PLATFORM_CLOUD
        if (i < normal_chan_cnt) {
            hdcdrv_msg_chan_info.level = DEVDRV_MSG_CHAN_LEVEL_LOW;
            if (hdcdrv_get_service_level(i) == HDCDRV_SERVICE_HIGH_LEVEL)
                hdcdrv_msg_chan_info.level = DEVDRV_MSG_CHAN_LEVEL_HIGH;
        } else {
            hdcdrv_msg_chan_info.level = DEVDRV_MSG_CHAN_LEVEL_HIGH;
        }
#endif
        msg_chan = devdrv_pcimsg_alloc_trans_queue(dev_id, &hdcdrv_msg_chan_info);
        if (msg_chan == NULL) {
            hdcdrv_err("dev_id %d alloc msg chan failed.\n", dev_id);
            goto alloc_err;
        }

        msgChanTmp[i] = msg_chan;

        if (hdcdrv_add_msg_chan_to_dev(dev_id, msg_chan)) {
            i = i + 1;
            hdcdrv_err("dev_id %d add msg chan to dev failed.\n", dev_id);
            goto alloc_err;
        }
    }

    msg_chan = devdrv_pcimsg_alloc_non_trans_queue(dev_id, &hdcdrv_non_trans_msg_chan_info);
    if (msg_chan == NULL) {
        hdcdrv_err("devid %d ctrl msg chan alloc failed\n", dev_id);
        goto alloc_err;
    }

    if (hdcdrv_add_ctrl_msg_chan_to_dev(dev_id, msg_chan) != HDCDRV_OK) {
        devdrv_pcimsg_free_non_trans_queue(msg_chan);
        hdcdrv_err("devid %d ctrl msg chan add failed\n", dev_id);
        goto alloc_err;
    }

    return HDCDRV_OK;

alloc_err:
    hdc_dev = &hdc_ctrl->devices[dev_id];
    hdcdrv_free_msg_chan(hdc_dev);
    for (j = 0; j < i; j++) {
        devdrv_pcimsg_realease_trans_queue(msgChanTmp[j]);
    }
    return HDCDRV_ERR;
}

STATIC void hdcdrv_uninit_msg_chan(struct hdcdrv_dev *hdc_dev)
{
    int i;

    if (hdc_dev->ctrl_msg_chan != NULL) {
        devdrv_pcimsg_free_non_trans_queue(hdc_dev->ctrl_msg_chan);
        hdc_dev->ctrl_msg_chan = NULL;
    }

    for (i = 0; i < hdc_dev->msg_chan_cnt; i++) {
        devdrv_set_msg_chan_priv(hdc_dev->msg_chan[i]->chan, NULL);
        devdrv_pcimsg_realease_trans_queue(hdc_dev->msg_chan[i]->chan);
    }
}

void hdcdrv_init_msg_chan_work(struct work_struct *p_work)
{
    struct hdcdrv_dev *hdc_dev = container_of(p_work, struct hdcdrv_dev, init.work);
    struct hdcdrv_ctrl_msg msg;
    u32 len = 0;
    long ret;

    /* wait for device init */
    msg.type = HDCDRV_CTRL_MSG_TYPE_SYNC;
    msg.sync_msg.segment = packet_segment;
    msg.sync_msg.peer_dev_id = hdc_dev->dev_id;
    msg.error_code = HDCDRV_OK;

    ret = hdcdrv_ctrl_msg_send(hdc_dev->dev_id, (void *)&msg, sizeof(msg), sizeof(msg), &len);
    if ((ret != HDCDRV_OK) || (len != sizeof(msg)) || (msg.error_code != HDCDRV_OK)) {
        hdcdrv_info("dev id %d, sync failed(ret %ld, err code %d, msg len %d(%ld)), wait for device startup.\n",
                    hdc_dev->dev_id, ret, msg.error_code, len, sizeof(msg));
        schedule_delayed_work(&hdc_dev->init, 1 * HZ);
        return;
    }
    hdcdrv_set_peer_dev_id(hdc_dev->dev_id, msg.sync_msg.peer_dev_id);

    /* init msg channel */
    if (hdcdrv_init_msg_chan(hdc_dev->dev_id)) {
        hdcdrv_err("dev id %d, init msg chan failed!\n", hdc_dev->dev_id);
        return;
    }

    ret = devdrv_register_common_msg_client(&hdcdrv_host_comm_msg_client);
    if (ret) {
        hdcdrv_err("call devdrv_register_common_msg_client failed");
        hdcdrv_uninit_msg_chan(hdc_dev);
        return;
    }

    if (hdc_dev->msg_chan_cnt > 0) {
        hdcdrv_set_device_status(hdc_dev->dev_id, HDCDRV_VALID);
    }

    ret = devdrv_set_module_init_finish(hdc_dev->dev_id, DEVDRV_HOST_MODULE_HDC);
    if (ret != HDCDRV_OK) {
        hdcdrv_warn("dev %d set module init finish failed\n", hdc_dev->dev_id);
    }
    up(&hdc_dev->hdc_instance_sem);
    hdcdrv_info("dev id %d, peer dev id %d dev enable work, msg chan count %d.\n",
        hdc_dev->dev_id, hdc_dev->peer_dev_id, hdc_dev->msg_chan_cnt);
}

int hdcdrv_get_session_run_env(void)
{
    int ret;
    int run_env = HDCDRV_SESSION_RUN_ENV_UNKNOW;

    if (devdrv_manager_get_run_env_func == NULL) {
        devdrv_manager_get_run_env_func =
            (devdrv_manager_get_run_env)(uintptr_t)kallsyms_lookup_name("devdrv_manager_container_is_host_system");
    }

    if (devdrv_manager_get_run_env_func != NULL) {
        ret = devdrv_manager_get_run_env_func(current->nsproxy->mnt_ns);
        if (ret == 1) {
            run_env = HDCDRV_SESSION_RUN_ENV_HOST;
        } else {
            run_env = HDCDRV_SESSION_RUN_ENV_CONTAINER;
        }
    }

    return run_env;
}

STATIC int hdcdrv_container_vir_to_phs_devid(u32 virtual_devid, u32 *physical_devid)
{
    if (hdcdrv_container_virtual_to_physical_devid == NULL) {
        hdcdrv_container_virtual_to_physical_devid =
            (container_virtual_to_physical_devid)(uintptr_t)\
            kallsyms_lookup_name("devdrv_manager_container_logical_id_to_physical_id");
    }

    if (hdcdrv_container_virtual_to_physical_devid != NULL) {
        return hdcdrv_container_virtual_to_physical_devid(virtual_devid, physical_devid);
    }

    *physical_devid = virtual_devid;
    return HDCDRV_OK;
}

STATIC int hdcdrv_manager_container_host_init(void)
{
    hdcdrv_container_virtual_to_physical_devid =
        (container_virtual_to_physical_devid)(uintptr_t)\
        kallsyms_lookup_name("devdrv_manager_container_logical_id_to_physical_id");
    if (hdcdrv_container_virtual_to_physical_devid == NULL) {
        hdcdrv_info("function : devdrv_manager_container_logical_id_to_physical_id not found\n");
        return HDCDRV_ERR;
    }
    return HDCDRV_OK;
}

int hdcdrv_check_in_container(void)
{
        if (hdcdrv_is_in_container_func == NULL) {
                hdcdrv_is_in_container_func =
                        (hdcdrv_is_in_container)(uintptr_t)\
                        kallsyms_lookup_name("devdrv_manager_container_is_in_container");
        }

        if (hdcdrv_is_in_container_func != NULL) {
                return hdcdrv_is_in_container_func();
        }

        return HDCDRV_ERR;
}

u32 hdcdrv_get_container_id(void)
{
    u32 docker_id = HDCDRV_PHY_HOST_ID;
    int ret;

    if (hdcdrv_check_in_container() != 1) {
        return docker_id;
    }

    if (hdcdrv_get_container_id_func == NULL) {
        hdcdrv_get_container_id_func = (get_container_id)(uintptr_t)\
            kallsyms_lookup_name("devdrv_manager_container_get_docker_id");
    }

    if (hdcdrv_get_container_id_func != NULL) {
        ret = hdcdrv_get_container_id_func(&docker_id);
        if (ret) {
            docker_id = HDCDRV_DOCKER_MAX_NUM;
        }
    }

    return docker_id;
}

STATIC int hdcdrv_init_instance(struct devdrv_client_instance *client_instance)
{
    struct device *dev = NULL;
    int dev_id;
    struct hdcdrv_dev *hdc_dev = NULL;

    if (client_instance == NULL) {
        hdcdrv_err("init client_instance is NULL\n");
        return HDCDRV_ERR;
    }

    dev = client_instance->dev_ctrl->dev;
    dev_id = (int)client_instance->dev_ctrl->dev_id;

    hdc_dev = hdcdrv_add_dev(dev, dev_id);
    if (hdc_dev == NULL) {
        hdcdrv_err("%s add failed", dev_driver_string(dev));
        return HDCDRV_ERR;
    }

    client_instance->priv = (void *)hdc_dev;
    sema_init(&hdc_dev->hdc_instance_sem, 0);
    INIT_DELAYED_WORK(&hdc_dev->init, hdcdrv_init_msg_chan_work);

    schedule_delayed_work(&hdc_dev->init, 0);

    return HDCDRV_OK;
}

STATIC int hdcdrv_uninit_instance(struct devdrv_client_instance *client_instance)
{
    struct hdcdrv_dev *hdc_dev = NULL;
    struct hdcdrv_ctrl_msg msg;
    u32 len = 0;
    long ret;

    if (client_instance == NULL) {
        hdcdrv_err("uninit client_instance is NULL\n");
        return HDCDRV_ERR;
    }

    hdc_dev = (struct hdcdrv_dev *)client_instance->priv;

    if (hdc_dev == NULL) {
        return HDCDRV_ERR;
    }
    ret = down_timeout(&hdc_dev->hdc_instance_sem, HDCDRV_INIT_INSTANCE_TIMEOUT);
    if (ret) {
        hdcdrv_warn("hdc devid %d instance timeout ret = %ld\n", hdc_dev->dev_id, ret);
    }
    msg.type = HDCDRV_CTRL_MSG_TYPE_RESET;
    msg.error_code = HDCDRV_OK;

    ret = hdcdrv_ctrl_msg_send(hdc_dev->dev_id, (void *)&msg, sizeof(msg), sizeof(msg), &len);
    if ((ret != HDCDRV_OK) || (len != sizeof(msg)) || (msg.error_code != HDCDRV_OK)) {
        hdcdrv_info("%s, reset failed.\n", dev_driver_string(hdc_dev->dev));
    }

    cancel_delayed_work_sync(&hdc_dev->init);
    devdrv_unregister_common_msg_client(hdc_dev->dev_id, &hdcdrv_host_comm_msg_client);

    hdcdrv_reset_dev(hdc_dev);
    msleep(500);

    hdcdrv_stop_work(hdc_dev);
    hdcdrv_uninit_msg_chan(hdc_dev);
    hdcdrv_remove_dev(hdc_dev);

    return HDCDRV_OK;
}

static struct devdrv_client hdcdrv_host_instance = {
    .type = DEVDRV_CLIENT_HDC,
    .init_instance = hdcdrv_init_instance,
    .uninit_instance = hdcdrv_uninit_instance,
};

int __init hdcdrv_init_module(void)
{
    int ret;

    if (devdrv_get_product_type() == HOST_TYPE_NORMAL) {
        normal_chan_cnt = HDCDRV_SURPORT_MAX_DEV_NORMAL_MSG_CHAN;
        packet_segment = HDCDRV_HUGE_PACKET_SEGMENT;
        running_env = HDCDRV_RUNNING_ENV_X86_NORMAL;
    } else {
        normal_chan_cnt = HDCDRV_NORMAL_MSG_CHAN_CNT;
        packet_segment = HDCDRV_PACKET_SEGMENT;
        running_env = HDCDRV_RUNNING_ENV_ARM_3559;
    }

    ret = hdcdrv_init();
    if (ret) {
        hdcdrv_err("call hdcdrv_init failed.\n");
        return ret;
    }

    hdcdrv_set_segment(packet_segment);

    ret = devdrv_register_client(&hdcdrv_host_instance);
    if (ret) {
        hdcdrv_uninit();
        hdcdrv_err("call devdrv_register_client failed.\n");
        return ret;
    }

    ret = hdcdrv_manager_container_host_init();
    if (ret != HDCDRV_OK) {
        hdcdrv_warn("call hdcdrv_manager_container_host_init failed.\n");
    }

    return HDCDRV_OK;
}

void __exit hdcdrv_exit_module(void)
{
    devdrv_unregister_common_msg_client(0, &hdcdrv_host_comm_msg_client);
    devdrv_unregister_client(&hdcdrv_host_instance);
    hdcdrv_uninit();
}

module_init(hdcdrv_init_module);
module_exit(hdcdrv_exit_module);

MODULE_AUTHOR("Huawei Tech. Co., Ltd.");
MODULE_DESCRIPTION("hdcdrv host driver");
MODULE_LICENSE("GPL");
