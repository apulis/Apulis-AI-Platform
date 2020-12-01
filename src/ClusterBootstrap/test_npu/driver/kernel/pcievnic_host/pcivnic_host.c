/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2012-2019. All rights reserved.
 * Description: Host Pci vnic
 * Author: Huawei Technologies Co., Ltd.
 * Create: 2018/8/18
 */
#include <linux/errno.h>
#include <linux/netdevice.h>
#include <linux/etherdevice.h>
#include <linux/pci.h>
#include <linux/jiffies.h>
#include <linux/io.h>
#include <linux/module.h>

#include "pcivnic_host.h"
#include "pcivnic_main.h"

#define STATIC
#define PCI_VENDOR_ID_HUAWEI 0x19e5

/*lint -e508 */
static const struct pci_device_id g_devdrv_pcivnic_tbl[] = {{ PCI_VDEVICE(HUAWEI, 0xd100), 0 },
    { PCI_VDEVICE(HUAWEI, PCI_DEVICE_CLOUD), 0 },
    { PCI_VDEVICE(HUAWEI, 0xd801), 0 },
    {}};
MODULE_DEVICE_TABLE(pci, g_devdrv_pcivnic_tbl);
/*lint +e508 */
struct pcivnic_netdev *g_vnic_dev = NULL;
unsigned char g_pcivnic_mac[ETH_ALEN];

struct pcivnic_pcidev *pcivnic_get_pcidev(void *msg_chan)
{
    return (struct pcivnic_pcidev *)devdrv_get_msg_chan_priv(msg_chan);
}

struct pcivnic_sq_desc *pcivnic_get_w_sq_desc(void *msg_chan, u32 *tail)
{
    void *sq_desc = NULL;

    sq_desc = devdrv_get_msg_chan_slave_sq_tail(msg_chan, tail);

    return (struct pcivnic_sq_desc *)sq_desc;
}

void pcivnic_set_w_sq_desc_head(void *msg_chan, u32 head)
{
    devdrv_set_msg_chan_slave_sq_head(msg_chan, head);
}

void pcivnic_copy_sq_desc_to_remote(struct pcivnic_pcidev *pcidev, struct pcivnic_sq_desc *sq_desc)
{
    void *msg_chan = pcidev->msg_chan;

    (void)sq_desc;
    devdrv_move_msg_chan_slave_sq_tail(msg_chan);

    /* shared memory without copying */
    wmb();

    /* trigger doorbell irq */
    devdrv_msg_ring_doorbell(msg_chan);
}

bool pcivnic_w_sq_full_check(void *msg_chan)
{
    return devdrv_msg_chan_slave_sq_full_check(msg_chan);
}

struct pcivnic_sq_desc *pcivnic_get_r_sq_desc(void *msg_chan, u32 *head)
{
    void *sq_desc = NULL;

    sq_desc = devdrv_get_msg_chan_host_sq_head(msg_chan, head);

    return (struct pcivnic_sq_desc *)sq_desc;
}

void pcivnic_move_r_sq_desc(void *msg_chan)
{
    devdrv_move_msg_chan_host_sq_head(msg_chan);

    return;
}

int pcivnic_dma_copy(struct pcivnic_pcidev *pcidev, u64 src, u64 dst, u32 size,
                     struct devdrv_asyn_dma_para_info *para_info)
{
    int ret;
    ret = devdrv_dma_async_copy(pcidev->dev_id, DEVDRV_DMA_DATA_COMMON, src, dst, size, DEVDRV_DMA_DEVICE_TO_HOST,
                                para_info);
    if (ret) {
        devdrv_err("dev %d dma copy failed size %x\n", pcidev->dev_id, size);
    }

    return ret;
}

struct pcivnic_cq_desc *pcivnic_get_w_cq_desc(void *msg_chan)
{
    void *cq_desc = NULL;

    cq_desc = devdrv_get_msg_chan_slave_cq_tail(msg_chan);

    return (struct pcivnic_cq_desc *)cq_desc;
}

void pcivnic_copy_cq_desc_to_remote(struct pcivnic_pcidev *pcidev, struct pcivnic_cq_desc *cq_desc)
{
    void *msg_chan = pcidev->msg_chan;

    (void)cq_desc;
    devdrv_move_msg_chan_slave_cq_tail(msg_chan);

    /* shared memory without copying */
    wmb();

    /* trigger doorbell irq */
    devdrv_msg_ring_cq_doorbell(msg_chan);
}

struct pcivnic_cq_desc *pcivnic_get_r_cq_desc(void *msg_chan)
{
    void *cq_desc = NULL;

    cq_desc = devdrv_get_msg_chan_host_cq_head(msg_chan);

    return (struct pcivnic_cq_desc *)cq_desc;
}

void pcivnic_move_r_cq_desc(void *msg_chan)
{
    devdrv_move_msg_chan_host_cq_head(msg_chan);
}

struct pcivnic_pcidev *pcivnic_get_pciedev(struct device *dev)
{
    struct pcivnic_pcidev *pcidev = NULL;
    u32 dev_id;

    if (g_vnic_dev != NULL) {
        for (dev_id = 0; dev_id < g_vnic_dev->pciedev_num; dev_id++) {
            pcidev = g_vnic_dev->pcidev[dev_id];
            if (pcidev == NULL) {
                continue;
            }
            if (pcidev->dev == dev) {
                break;
            }
            pcidev = NULL;
        }
    }

    return pcidev;
}

bool pcivnic_is_p2p_enabled(u32 dev_id, u32 peer_dev_id)
{
    return devdrv_is_p2p_enabled(dev_id, peer_dev_id);
}

int pcivnic_up_get_next_hop(unsigned char *dmac)
{
    if ((*(u32 *)g_pcivnic_mac == *(u32 *)dmac) && (g_pcivnic_mac[PCIVNIC_MAC_4] == dmac[PCIVNIC_MAC_4])) {
        if (dmac[PCIVNIC_MAC_5] == HOST_MAC_LAST_BYTE) {
            return PCIVNIC_NEXT_HOP_LOCAL_NETDEV;
        } else if (dmac[PCIVNIC_MAC_5] < NETDEV_PCIDEV_NUM) {
            return dmac[PCIVNIC_MAC_5];
        }
    }

    return PCIVNIC_NEXT_HOP_BROADCAST;
}

int pcivnic_down_get_next_hop(unsigned char *dmac)
{
    if ((*(u32 *)g_pcivnic_mac == *(u32 *)dmac) && (g_pcivnic_mac[PCIVNIC_MAC_4] == dmac[PCIVNIC_MAC_4]))
        if (dmac[PCIVNIC_MAC_5] < NETDEV_PCIDEV_NUM)
            return dmac[PCIVNIC_MAC_5];

    return PCIVNIC_NEXT_HOP_BROADCAST;
}

struct devdrv_trans_msg_chan_info g_msg_chan_info = {
    .msg_type = devdrv_msg_client_pcivnic,
    .queue_depth = PCIVNIC_DESC_QUEUE_DEPTH,
    .level = DEVDRV_MSG_CHAN_LEVEL_LOW,
    .cq_desc_size = sizeof(struct pcivnic_cq_desc),
    .sq_desc_size = sizeof(struct pcivnic_sq_desc),
    .rx_msg_notify = pcivnic_rx_msg_notify,
    .tx_finish_notify = pcivnic_tx_finish_notify,
};

STATIC int pcivnic_init_msg_chan(struct pcivnic_pcidev *pcidev)
{
    u32 devid = pcidev->dev_id;
    void *msg_chan = NULL;

    msg_chan = devdrv_pcimsg_alloc_trans_queue(devid, &g_msg_chan_info);
    if (msg_chan == NULL) {
        devdrv_info("dev %d init host msg chan failed!\n", devid);
        return -EINVAL;
    }

    pcivnic_init_msgchan_cq_desc(msg_chan);

    pcidev->msg_chan = msg_chan;

    devdrv_set_msg_chan_priv(msg_chan, (void *)pcidev);

    return 0;
}

STATIC void pcivnic_uninit_msg_chan(struct pcivnic_pcidev *pcidev)
{
    if (!pcidev->msg_chan) {
        return;
    }

    devdrv_set_msg_chan_priv(pcidev->msg_chan, NULL);
    devdrv_pcimsg_realease_trans_queue(pcidev->msg_chan);
    pcidev->msg_chan = NULL;
}

void pcivnic_init_msg_instance(struct work_struct *p_work)
{
    struct delayed_work *delayed_work = container_of(p_work, struct delayed_work, work);
    struct pcivnic_pcidev *pcidev = container_of(delayed_work, struct pcivnic_pcidev, msg_init);
    struct pcivnic_netdev *vnic_dev = (struct pcivnic_netdev *)pcidev->netdev;
    struct pcivnic_ctrl_msg msg;
    u32 len = 0;

    ether_addr_copy(msg.mac, g_pcivnic_mac);
    if (devdrv_common_msg_send(pcidev->dev_id, (void *)&msg, sizeof(msg), sizeof(msg), &len,
                               DEVDRV_COMMON_MSG_PCIVNIC)) {
        devdrv_info("dev %d sync failed, wait for device startup.\n", pcidev->dev_id);
        schedule_delayed_work(&pcidev->msg_init, 1 * HZ);
        return;
    }

    /* init msg channel from DMA */
    if (pcivnic_init_msg_chan(pcidev)) {
        devdrv_err("dev %d init msg chan for netdev failed!\n", pcidev->dev_id);
        return;
    }

    pcidev->status = BIT_STATUS_LINK;

    (void)devdrv_set_module_init_finish(pcidev->dev_id, DEVDRV_HOST_MODULE_PCIVNIC);
    up(&pcidev->vnic_instance_sem);
    /* !!!!! CI will judge this log to determine the device startup status,
     * do not change it. !!!!!
     */
    devdrv_info("dev id %d: %s, alloc new pcivnic device <%s>  mac: %pM, (locked and finish)\n", pcidev->dev_id,
                dev_driver_string(pcidev->dev), vnic_dev->ndev->name, vnic_dev->ndev->dev_addr);

    return;
}

int pcivnic_ctrl_msg_recv(u32 devid, void *data, u32 in_data_len, u32 out_data_len, u32 *real_out_len)
{
    *real_out_len = 0;

    return 0;
}

struct devdrv_common_msg_client g_pcivnic_host_comm_msg_client = {
    .type = DEVDRV_COMMON_MSG_PCIVNIC,
    .common_msg_recv = pcivnic_ctrl_msg_recv,
};

STATIC int pcivnic_init_instance(struct devdrv_client_instance *client_instance)
{
    struct device *dev = client_instance->dev_ctrl->dev;
    struct pcivnic_netdev *vnic_dev = g_vnic_dev;
    struct pcivnic_pcidev *pcidev = NULL;
    int dev_id = client_instance->dev_ctrl->dev_id;
    int ret;

    pcidev = pcivnic_add_dev(vnic_dev, dev, PCIVNIC_DESC_QUEUE_DEPTH, dev_id);
    if (pcidev == NULL) {
        devdrv_err("dev_id %d add pcidev failed!\n", dev_id);
        return -ENOMEM;
    }

    ret = devdrv_register_common_msg_client(&g_pcivnic_host_comm_msg_client);
    if (ret) {
        pcivnic_del_dev(vnic_dev, dev_id);
        devdrv_err("dev_id %d register_common_msg failed ret %d", dev_id, ret);
        return ret;
    }

    pcidev->dev_id = dev_id;
    pcidev->priv = (void *)client_instance;
    client_instance->priv = (void *)pcidev;
    sema_init(&pcidev->vnic_instance_sem, 0);
    INIT_DELAYED_WORK(&pcidev->msg_init, pcivnic_init_msg_instance);

    schedule_delayed_work(&pcidev->msg_init, 0);

    return 0;
}

STATIC int pcivnic_uninit_instance(struct devdrv_client_instance *client_instance)
{
    struct pcivnic_pcidev *pcidev = client_instance->priv;
    int ret;

    if (pcidev != NULL) {
        ret = down_timeout(&pcidev->vnic_instance_sem, PCIVNIC_INIT_INSTANCE_TIMEOUT);
        if (ret) {
            devdrv_warn("pcivnic devid %d uninstance timeout ret = %d\n", pcidev->dev_id, ret);
        }
        devdrv_info("devid %d pcivnic_uninit_instance\n", pcidev->dev_id);
        (void)devdrv_unregister_common_msg_client(pcidev->dev_id, &g_pcivnic_host_comm_msg_client);
        cancel_delayed_work_sync(&pcidev->msg_init);
        pcivnic_uninit_msg_chan(pcidev);
        if (g_vnic_dev != NULL) {
            pcivnic_del_dev(g_vnic_dev, pcidev->dev_id);
        }
    }

    return 0;
}

int pcivnic_device_status_abnormal(void *msg_chan)
{
    int ret;
    ret = devdrv_device_status_abnormal_check(msg_chan);

    return ret;
}

static struct devdrv_client g_pcivnic_host_instance = {
    .type = DEVDRV_CLIENT_PCIVNIC,
    .init_instance = pcivnic_init_instance,
    .uninit_instance = pcivnic_uninit_instance,
};

int __init pcivnic_init_module(void)
{
    struct pcivnic_netdev *vnic_dev = NULL;
    char *ndev_name = "endvnic";
    int ret;

    devdrv_info("pcivnic_init_module start\n");
    vnic_dev = pcivnic_alloc_netdev(ndev_name, PCIVNIC_NAME_SIZE);
    if (vnic_dev == NULL) {
        devdrv_err("alloc netdev failed!\n");
        return -ENOMEM;
    }
    pcivnic_get_mac(HOST_MAC_LAST_BYTE, g_pcivnic_mac);
    pcivnic_set_netdev_mac(vnic_dev, g_pcivnic_mac);

    ret = pcivnic_register_netdev(vnic_dev);
    if (ret) {
        pcivnic_free_netdev(vnic_dev);
        devdrv_err("register netdev fail ret = %d\n", ret);
        return ret;
    }
    g_vnic_dev = vnic_dev;

    ret = devdrv_register_client(&g_pcivnic_host_instance);
    if (ret) {
        devdrv_err("devdrv_register_client fail ret = %d\n", ret);
        pcivnic_free_netdev(vnic_dev);
        return ret;
    }

    return 0;
}

void __exit pcivnic_exit_module(void)
{
    int ret = 0;
    struct pcivnic_netdev *vnic_dev = g_vnic_dev;

    ret = devdrv_unregister_client(&g_pcivnic_host_instance);
    if (ret) {
        devdrv_err("devdrv_unregister_client is fail ret = %d\n", ret);
        return;
    }

    if (vnic_dev != NULL) {
        devdrv_info("destroy pcivnic device <%s>  mac: %pM\n", vnic_dev->ndev->name, vnic_dev->ndev->dev_addr);
        pcivnic_free_netdev(vnic_dev);
    }
    devdrv_info("pcivnic_exit_module success\n");
}

module_init(pcivnic_init_module);
module_exit(pcivnic_exit_module);

MODULE_AUTHOR("Huawei Tech. Co., Ltd.");
MODULE_DESCRIPTION("pcivnic host driver");
MODULE_LICENSE("GPL");
