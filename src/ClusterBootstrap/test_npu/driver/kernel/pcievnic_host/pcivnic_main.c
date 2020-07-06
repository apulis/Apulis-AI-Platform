/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2012-2019. All rights reserved.
 * Description: This file contains pcivnic_main Info.
 * Author: huawei
 * Create: 2017-10-15
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

#include <linux/errno.h>
#include <linux/ethtool.h>
#include <linux/netdevice.h>
#include <linux/etherdevice.h>
#include <linux/jiffies.h>
#include <linux/version.h>
#include <linux/spinlock.h>
#include <linux/vmalloc.h>
#include <linux/sysfs.h>

#include "pcivnic_main.h"
#define STATIC

#define PCIVNIC_COUNT 64
#define CHAR_ARRAY_MAX_LEN 20

const char g_pcivnic_name[CHAR_ARRAY_MAX_LEN] = "pcivnic_driver";

static struct {
    const char str[ETH_GSTRING_LEN];
} pcivnic_ethtool_stats[] = {
    {"pcivnic_rx_packets"}, {"pcivnic_tx_packets"}, {"pcivnic_rx_bytes"}, {"pcivnic_tx_bytes"}, {"pcivnic_rx_errors"},
    {"pcivnic_tx_errors"},  {"pcivnic_rx_dropped"}, {"pcivnic_tx_dropped"}, {"pcivnic_multicast"},
    {"pcivnic_collisions"}, {"pcivnic_rx_length_errors"}, {"pcivnic_rx_over_errors"}, {"pcivnic_rx_crc_errors"},
    {"pcivnic_rx_frame_errors"}, {"pcivnic_rx_fifo_errors"}, {"pcivnic_rx_missed_errors"},
    {"pcivnic_tx_aborted_errors"}, {"pcivnic_tx_carrier_errors"}, {"pcivnic_tx_fifo_errors"},
    {"pcivnic_tx_heartbeat_errors"}, {"pcivnic_tx_window_errors"}, {"pcivnic_rx_compressed"}, {"pcivnic_tx_compressed"}
};

STATIC void pcivnic_net_get_drvinfo(struct net_device *net_dev, struct ethtool_drvinfo *info)
{
    char version[CHAR_ARRAY_MAX_LEN] = "pcivnic v1.0";
    char bus_info[CHAR_ARRAY_MAX_LEN] = "pcivnic";

    if (strncpy_s(info->driver, CHAR_ARRAY_MAX_LEN, g_pcivnic_name, CHAR_ARRAY_MAX_LEN - 1) == 0 &&
        strncpy_s(info->version, CHAR_ARRAY_MAX_LEN, version, CHAR_ARRAY_MAX_LEN - 1) == 0 &&
        strncpy_s(info->bus_info, CHAR_ARRAY_MAX_LEN, bus_info, CHAR_ARRAY_MAX_LEN - 1) == 0) {
        return;
    }

    devdrv_err("strncpy_s fail\n");
    return;
}

STATIC unsigned int pcivnic_net_get_link(struct net_device *net_dev)
{
    struct pcivnic_netdev *vnic_dev = netdev_priv(net_dev);
    return (vnic_dev->status & BIT_STATUS_LINK);
}

STATIC int pcivnic_get_sset_count(struct net_device *net_dev, int sset)
{
    switch (sset) {
        case ETH_SS_STATS:
            return ARRAY_SIZE(pcivnic_ethtool_stats);
        default:
            return -EOPNOTSUPP;
    }
}

STATIC void pcivnic_get_strings(struct net_device *net_dev, u32 strset, u8 *buf)
{
    if (strset == ETH_SS_TEST) {
        devdrv_warn("strset equal with ETH_SS_TEST\n");
        return;
    }
    if (memcpy_s(buf, sizeof(pcivnic_ethtool_stats), &pcivnic_ethtool_stats, sizeof(pcivnic_ethtool_stats)) != 0) {
        devdrv_err("memcpy_s fail\n");
    }
    return;
}

STATIC void pcivnic_get_ethtool_stats(struct net_device *net_dev, struct ethtool_stats *estats, u64 *stats)
{
    struct pcivnic_netdev *vnic_dev = netdev_priv(net_dev);
    int i = 0;

    devdrv_info("%s: pcivnic_get_ethtool_stats\n", vnic_dev->ndev->name);
    stats[i++] = net_dev->stats.rx_packets;
    stats[i++] = net_dev->stats.tx_packets;
    stats[i++] = net_dev->stats.rx_bytes;
    stats[i++] = net_dev->stats.tx_bytes;
    stats[i++] = net_dev->stats.rx_errors;
    stats[i++] = net_dev->stats.tx_errors;
    stats[i++] = net_dev->stats.rx_dropped;
    stats[i++] = net_dev->stats.tx_dropped;
    stats[i++] = net_dev->stats.multicast;
    stats[i++] = net_dev->stats.collisions;
    stats[i++] = net_dev->stats.rx_length_errors;
    stats[i++] = net_dev->stats.rx_over_errors;
    stats[i++] = net_dev->stats.rx_crc_errors;
    stats[i++] = net_dev->stats.rx_frame_errors;
    stats[i++] = net_dev->stats.rx_fifo_errors;
    stats[i++] = net_dev->stats.rx_missed_errors;
    stats[i++] = net_dev->stats.tx_aborted_errors;
    stats[i++] = net_dev->stats.tx_carrier_errors;
    stats[i++] = net_dev->stats.tx_fifo_errors;
    stats[i++] = net_dev->stats.tx_heartbeat_errors;
    stats[i++] = net_dev->stats.tx_window_errors;
    stats[i++] = net_dev->stats.rx_compressed;
    stats[i++] = net_dev->stats.tx_compressed;
}

STATIC void pcivnic_irqs_enable(struct pcivnic_netdev *vnic_dev)
{
    skb_queue_head_init(&vnic_dev->skbq);
    napi_enable(&vnic_dev->napi);
}

STATIC void pcivnic_irqs_disable(struct pcivnic_netdev *vnic_dev)
{
    napi_disable(&vnic_dev->napi);
    skb_queue_purge(&vnic_dev->skbq);
}

STATIC int pcivnic_net_open(struct net_device *dev)
{
    struct pcivnic_netdev *vnic_dev = netdev_priv(dev);

    netif_carrier_off(dev);
    pcivnic_irqs_enable(vnic_dev);
    netif_carrier_on(dev);

    netif_start_queue(dev);

    vnic_dev->status |= BIT_STATUS_LINK;

    devdrv_info("%s: pcivnic_net_open\n", vnic_dev->ndev->name);

    /* Notify the other side that we're open will be next iteration */
    return 0;
}

STATIC int pcivnic_net_close(struct net_device *dev)
{
    struct pcivnic_netdev *vnic_dev = netdev_priv(dev);

    pcivnic_irqs_disable(vnic_dev);
    netif_carrier_off(dev);
    netif_stop_queue(dev);
    vnic_dev->status &= ~BIT_STATUS_LINK;

    devdrv_info("%s: pcivnic_net_close\n", vnic_dev->ndev->name);

    /* Notify the other side that we're closed will be next iteration */
    return 0;
}

int pcivnic_get_next_valid_pcidev(struct pcivnic_netdev *vnic_dev, int begin)
{
    struct pcivnic_pcidev *pcidev = NULL;
    int i;

    for (i = begin; i < NETDEV_PCIDEV_NUM; i++) {
        pcidev = vnic_dev->pcidev[i];
        if (pcidev == NULL) {
            continue;
        }
        if (pcidev->status & BIT_STATUS_LINK) {
            if (pcidev->msg_chan != NULL && !pcivnic_device_status_abnormal(pcidev->msg_chan)) {
                return i;
            }
        }
    }

    return -EINVAL;
}

int pcivnic_pciedev_send(struct sk_buff *skb, struct pcivnic_pcidev *pcidev, struct net_device *dev)
{
    struct pcivnic_netdev *vnic_dev = (struct pcivnic_netdev *)pcidev->netdev;
    struct pcivnic_sq_desc *sq_desc = NULL;
    u64 addr;
    u32 tail = 0;
    int ret;

    spin_lock_bh(&pcidev->lock);

    if (pcivnic_device_status_abnormal(pcidev->msg_chan)) {
        spin_unlock_bh(&pcidev->lock);
        devdrv_warn("device %d is dead\n", pcidev->dev_id);
        ret = NETDEV_TX_OK;
        goto ERROR;
    }

    /* tx queue full */
    if (unlikely(pcidev->status & BIT_STATUS_TQ_FULL)) {
        spin_unlock_bh(&pcidev->lock);
        pcidev->stat.tx_full++;
        ret = NETDEV_TX_BUSY;
        goto ERROR;
    }

#ifdef USE_DMA_ADDR
    addr = dma_map_single(pcidev->dev, skb->data, skb->len, DMA_TO_DEVICE);
    if (dma_mapping_error(pcidev->dev, addr)) {
        spin_unlock_bh(&pcidev->lock);
        devdrv_err("device %d dma map is error\n", pcidev->dev_id);
        ret = NETDEV_TX_OK;
        goto ERROR;
    }
#else
    addr = virt_to_phys(skb->data);
#endif

    /* update netdev */
    if (dev != NULL) {
        spin_lock_bh(&vnic_dev->lock);
#if LINUX_VERSION_CODE <= KERNEL_VERSION(4, 2, 0)
#ifndef RHEL_RELEASE_CODE
        dev->trans_start = jiffies;
#endif
#endif
        dev->stats.tx_packets++;
        dev->stats.tx_bytes += skb->len;
        netdev_sent_queue(dev, skb->len);
        spin_unlock_bh(&vnic_dev->lock);
    }

    sq_desc = pcivnic_get_w_sq_desc(pcidev->msg_chan, &tail);
    if (sq_desc == NULL || tail >= PCIVNIC_DESC_QUEUE_DEPTH) {
        spin_unlock_bh(&pcidev->lock);
        devdrv_err("sq_desc is NULL\n");
#ifdef USE_DMA_ADDR
        dma_unmap_single(pcidev->dev, addr, skb->len, DMA_TO_DEVICE);
#endif
        ret = NETDEV_TX_OK;
        goto ERROR;
    }

#ifdef PCIVNIC_DEBUG
    devdrv_info("dev id %d tail %d len %d\n", pcidev->dev_id, tail, skb->len);
#endif

    sq_desc->data_buf_addr = addr;
    sq_desc->data_len = skb->len;
    wmb();
    sq_desc->valid = PCIVNIC_VALID;

    /* stored locally for release after subsequent sending */
    pcidev->tx[tail].addr = addr;
    pcidev->tx[tail].skb = skb;
    pcidev->tx[tail].len = skb->len;
    pcidev->tx[tail].netdev = (void *)dev;
    if (dev != NULL) {
        spin_lock_bh(&vnic_dev->lock);
        pcidev->tx[tail].tx_seq = dev->stats.tx_packets;
        pcidev->tx[tail].timestamp = (unsigned long)(jiffies / HZ);
        spin_unlock_bh(&vnic_dev->lock);
    }

    pcidev->stat.tx_pkt++;
    pcidev->stat.tx_bytes += skb->len;

    pcivnic_copy_sq_desc_to_remote(pcidev, sq_desc);

    if (pcivnic_w_sq_full_check(pcidev->msg_chan)) {
        pcidev->status |= BIT_STATUS_TQ_FULL;
    }

    spin_unlock_bh(&pcidev->lock);

    return NETDEV_TX_OK;

ERROR:
    if (ret == NETDEV_TX_OK) {
        if (dev != NULL) {
            spin_lock_bh(&vnic_dev->lock);
            dev->stats.tx_dropped++;
            dev->stats.tx_fifo_errors++;
            spin_unlock_bh(&vnic_dev->lock);
        }
        dev_kfree_skb_any(skb);
        skb = NULL;
    } else {
        if (dev != NULL) {
            netif_stop_queue(dev);
        } else {
            dev_kfree_skb_any(skb);
            skb = NULL;
        }
    }

    return ret;
}

STATIC netdev_tx_t pcivnic_net_xmit(struct sk_buff *skb, struct net_device *dev)
{
    struct pcivnic_netdev *vnic_dev = netdev_priv(dev);
    struct sk_buff *skb_cp = NULL;
    int ret;
    int next_hop;
    int begin = 0;

    /* bad pkt */
    if (skb->len < ETH_HLEN) {
        goto free_skb;
    }

    next_hop = pcivnic_down_get_next_hop((unsigned char *)skb_mac_header(skb));
    if (next_hop != PCIVNIC_NEXT_HOP_BROADCAST) {
        if (vnic_dev->pcidev[next_hop] == NULL) {
            devdrv_info("next_hop %d dmac: %pM no pciedev\n", next_hop, skb_mac_header(skb));
            goto free_skb;
        }

        if (vnic_dev->pcidev[next_hop]->msg_chan == NULL) {
            devdrv_info("next_hop %d dmac: %pM no msg chan\n", next_hop, skb_mac_header(skb));
            goto free_skb;
        }

        ret = pcivnic_pciedev_send(skb, vnic_dev->pcidev[next_hop], dev);
    } else {
        next_hop = pcivnic_get_next_valid_pcidev(vnic_dev, begin);
        if (next_hop < 0) {
            goto free_skb;
        }

        begin = next_hop + 1;

        /* Broadcast transmission only counts once */
        skb_cp = skb_copy(skb, GFP_ATOMIC);
        ret = pcivnic_pciedev_send(skb, vnic_dev->pcidev[next_hop], dev);

        while (skb_cp != NULL) {
            next_hop = pcivnic_get_next_valid_pcidev(vnic_dev, begin);
            if (next_hop < 0) {
                break;
            }
            begin = next_hop + 1;

            skb = skb_copy(skb_cp, GFP_ATOMIC);
            if (skb != NULL)
                (void)pcivnic_pciedev_send(skb, vnic_dev->pcidev[next_hop], NULL);
        }

        if (skb_cp != NULL) {
            dev_kfree_skb_any(skb_cp);
            skb_cp = NULL;
        }
    }

    return (netdev_tx_t)ret;

free_skb:
    spin_lock_bh(&vnic_dev->lock);
    dev->stats.tx_errors++;
    dev->stats.tx_dropped++;
    spin_unlock_bh(&vnic_dev->lock);
    dev_kfree_skb_any(skb);
    skb = NULL;
    return NETDEV_TX_OK;
}

int pcivnic_tx_finish_para_check(struct pcivnic_cq_desc *cq_desc)
{
    if ((cq_desc == NULL) || (cq_desc->valid != PCIVNIC_VALID) ||
            (cq_desc->sq_head >= PCIVNIC_DESC_QUEUE_DEPTH)) {
        return -EINVAL;
    }
    return 0;
}

void pcivnic_tx_finish_notify_task(unsigned long data)
{
    struct pcivnic_pcidev *pcidev = (struct pcivnic_pcidev *)((uintptr_t)data);
    struct pcivnic_netdev *vnic_dev = (struct pcivnic_netdev *)pcidev->netdev;
    void *msg_chan = pcidev->msg_chan;
    struct net_device *ndev = vnic_dev->ndev;
    u32 bytes_compl = 0;
    u32 pkts_compl = 0;
    struct sk_buff *skb = NULL;
    u16 tx_head;
#ifdef PCIVNIC_DEBUG
    int i = 0;
#endif
    int cnt = 0;
    int ret;

    struct pcivnic_cq_desc *cq_desc = NULL;

    do {
        cq_desc = pcivnic_get_r_cq_desc(msg_chan);
        ret = pcivnic_tx_finish_para_check(cq_desc);
        if (ret) {
            break;
        }

        /* Reach the threshold and schedule out */
        if (cnt >= PCIVNIC_TX_BUDGET) {
            tasklet_schedule(&pcidev->tx_finish_task);
            break;
        }

        tx_head = cq_desc->sq_head;

#ifdef PCIVNIC_DEBUG
        devdrv_info("dev %d pcivnic_tx_finish_notify tx_head %d\n", pcidev->dev_id, tx_head);
#endif

        skb = pcidev->tx[tx_head].skb;
        if (skb == NULL) {
            devdrv_err(
                "dev %d cq_desc tx_head = %d skb is null. cq: valid %d, status %d, sq_head %d, cq id %d tx %ld\n",
                pcidev->dev_id, tx_head, cq_desc->valid, cq_desc->status, cq_desc->sq_head,
                cq_desc->cq_id, ndev->stats.tx_packets);
#ifdef PCIVNIC_DEBUG
            for (i = 0; i < PCIVNIC_DESC_QUEUE_DEPTH; i++) {
                cq_desc = pcivnic_get_r_cq_desc(msg_chan);
                pcivnic_move_r_cq_desc(msg_chan);
                devdrv_info("cq_desc valid %d, status %d, sq_head %d, cq id %d\n",
                            cq_desc->valid, cq_desc->status, cq_desc->sq_head, cq_desc->cq_id);
            }
#endif
            goto next;
        }

        if (cq_desc->status != 0) {
            if (pcidev->tx[tx_head].netdev) {
                spin_lock_bh(&vnic_dev->lock);
                ndev->stats.tx_errors++;
                ndev->stats.tx_carrier_errors++;
                spin_unlock_bh(&vnic_dev->lock);
            }

            devdrv_err("dev %d tx failed tx_head = %d status %d.\n", pcidev->dev_id, tx_head, cq_desc->status);
        }

        if (pcidev->tx[tx_head].netdev) {
            pkts_compl++;
            bytes_compl += pcidev->tx[tx_head].len;
        }

#ifdef USE_DMA_ADDR
        dma_unmap_single(pcidev->dev, pcidev->tx[tx_head].addr, skb->len, DMA_TO_DEVICE);
#endif
        dev_consume_skb_any(skb);

    next:
        pcidev->tx[tx_head].skb = NULL;
        pcidev->tx[tx_head].addr = 0;
        pcidev->tx[tx_head].len = 0;
        pcidev->tx[tx_head].netdev = NULL;

        cq_desc->valid = PCIVNIC_INVALID;
        pcivnic_move_r_cq_desc(msg_chan);

        spin_lock_bh(&pcidev->lock);

        /* update the sq head pointer to continue send packet */
        pcivnic_set_w_sq_desc_head(msg_chan, tx_head);

        cnt++;

        if (unlikely(pcidev->status & BIT_STATUS_TQ_FULL)) {
            pcidev->status &= ~BIT_STATUS_TQ_FULL;
        }
        spin_unlock_bh(&pcidev->lock);
    } while (1);

    spin_lock_bh(&vnic_dev->lock);
    if (pkts_compl || bytes_compl) {
        netdev_completed_queue(vnic_dev->ndev, pkts_compl, bytes_compl);
    }

    if (unlikely(netif_queue_stopped(vnic_dev->ndev)) && pkts_compl) {
        netif_wake_queue(vnic_dev->ndev);
    }
    spin_unlock_bh(&vnic_dev->lock);
}

void pcivnic_tx_finish_notify(void *msg_chan)
{
    struct pcivnic_pcidev *pcidev = pcivnic_get_pcidev(msg_chan);
    if (pcidev != NULL) {
        tasklet_schedule(&pcidev->tx_finish_task);
    }
}

void pcivnic_response_cq(struct pcivnic_pcidev *pcidev, u32 sq_head, u32 status)
{
    struct pcivnic_cq_desc *cq_desc = NULL;

    /* response cq */
    cq_desc = pcivnic_get_w_cq_desc(pcidev->msg_chan);
    if (cq_desc == NULL) {
        devdrv_err("devid %d cq_desc is NULL, sq_head 0x%x, status 0x%x.\n", pcidev->dev_id, sq_head, status);
        return;
    }
    cq_desc->sq_head = sq_head;
    cq_desc->status = status;
    wmb();
    cq_desc->valid = PCIVNIC_VALID;
    pcivnic_copy_cq_desc_to_remote(pcidev, cq_desc);
}

/* napi receive polling callback function */
static int pcivnic_napi(struct napi_struct *napi, int budget)
{
    struct pcivnic_netdev *vnic_dev = container_of(napi, struct pcivnic_netdev, napi);
    int work_done = 0;
    struct sk_buff *skb = NULL;

    while (1) {
        if (work_done >= budget) {
            break;
        }

        skb = skb_dequeue(&vnic_dev->skbq);
        if (unlikely(skb == NULL)) {
            break;
        }

        napi_gro_receive(&vnic_dev->napi, skb);
        work_done++;
    }

    if (work_done < budget) {
        napi_complete(napi);
    }

    return work_done;
}

void pcivnic_rx_packet(struct sk_buff *skb, struct pcivnic_netdev *vnic_dev)
{
    struct net_device *ndev = vnic_dev->ndev;

    spin_lock_bh(&vnic_dev->lock);

    if ((vnic_dev->status & BIT_STATUS_LINK) == 0) {
        ndev->stats.rx_dropped++;
        spin_unlock_bh(&vnic_dev->lock);
        if (ndev->stats.rx_dropped % PCIVNIC_LINKDOWN_NUM == 0) {
            devdrv_info("link down drop packet len %d.\n", skb->len);
        }
        dev_consume_skb_any(skb);
        return;
    }

    /* update stats */
    ndev->stats.rx_packets++;
    ndev->stats.rx_bytes += skb->len;

    spin_unlock_bh(&vnic_dev->lock);

    /* set skb */
    skb->protocol = eth_type_trans(skb, ndev);
    skb->ip_summed = CHECKSUM_NONE;
    skb->dev = ndev;

    /* submit upper level process */
    rmb();

    skb_queue_tail(&vnic_dev->skbq, skb);

    if (napi_schedule_prep(&vnic_dev->napi)) {
        __napi_schedule(&vnic_dev->napi);
    }
#ifdef RHEL_RELEASE_CODE
#if (RHEL_RELEASE_VERSION(7, 5) > RHEL_RELEASE_CODE)
    ndev->last_rx = jiffies;
#endif
#else
#if LINUX_VERSION_CODE <= KERNEL_VERSION(4, 2, 0)
    ndev->last_rx = jiffies;
#endif
#endif
}

int pcivnic_forward_flow_ctrl(struct pcivnic_netdev *vnic_dev, int forward_dev, int next_hop)
{
    struct pcivnic_flow_ctrl *flow_ctrl = NULL;
    struct pcivnic_fwd_stat *fwd_stat = NULL;
    unsigned long timestamp;

    fwd_stat = &vnic_dev->pcidev[forward_dev]->fwd_stat[next_hop];

    fwd_stat->fwd_all++;

    if (pcivnic_is_p2p_enabled(forward_dev, next_hop) == false) {
        fwd_stat->disable_drop++;
        return PCIVNIC_VALID;
    }

    flow_ctrl = &vnic_dev->pcidev[forward_dev]->flow_ctrl[next_hop];

    flow_ctrl->pkt++;

    /* Threshold not reached */
    if (flow_ctrl->pkt <= flow_ctrl->threshold) {
        fwd_stat->fwd_success++;
        return PCIVNIC_INVALID;
    }

    /* Threshold reached */
    timestamp = (unsigned long)jiffies;
    if (jiffies_to_msecs(timestamp - flow_ctrl->timestamp) < PCIVNIC_FLOW_CTRL_PERIOD) {
        fwd_stat->flow_ctrl_drop++;
        return PCIVNIC_VALID;
    }

    /* new period */
    flow_ctrl->timestamp = timestamp;
    flow_ctrl->pkt = 0;

    fwd_stat->fwd_success++;
    return PCIVNIC_INVALID;
}

void pcivnic_forward_packet(struct sk_buff *skb, struct pcivnic_netdev *vnic_dev, int forward_dev)
{
    struct sk_buff *skb_cp = NULL;
    int next_hop;
    int begin = 0;

    next_hop = pcivnic_up_get_next_hop((unsigned char *)skb->data);
    if (next_hop == PCIVNIC_NEXT_HOP_LOCAL_NETDEV) {
        pcivnic_rx_packet(skb, vnic_dev);
    } else if (next_hop != PCIVNIC_NEXT_HOP_BROADCAST) {
        if ((forward_dev == next_hop) || (vnic_dev->pcidev[next_hop] == NULL)) {
            dev_consume_skb_any(skb);
        } else {
            if (pcivnic_forward_flow_ctrl(vnic_dev, forward_dev, next_hop) == PCIVNIC_VALID) {
                dev_consume_skb_any(skb);
                return;
            }

            (void)pcivnic_pciedev_send(skb, vnic_dev->pcidev[next_hop], NULL);
        }
    } else {
        while (1) {
            next_hop = pcivnic_get_next_valid_pcidev(vnic_dev, begin);
            if (next_hop < 0) {
                break;
            }
            begin = next_hop + 1;
            if (forward_dev == next_hop) {
                continue;
            }

            if (pcivnic_forward_flow_ctrl(vnic_dev, forward_dev, next_hop) == PCIVNIC_VALID) {
                continue;
            }

            skb_cp = skb_copy(skb, GFP_ATOMIC);
            if (skb_cp != NULL) {
                (void)pcivnic_pciedev_send(skb_cp, vnic_dev->pcidev[next_hop], NULL);
            }
        }

        pcivnic_rx_packet(skb, vnic_dev);
    }
}

void pcivnic_rx_msg_callback(void *data, u32 trans_id, u32 status)
{
    struct pcivnic_pcidev *pcidev = (struct pcivnic_pcidev *)data;
    struct pcivnic_netdev *vnic_dev = (struct pcivnic_netdev *)pcidev->netdev;
    struct sk_buff *skb = NULL;

    if (trans_id >= PCIVNIC_DESC_QUEUE_DEPTH) {
        devdrv_err("dev %d pcivnic rx callback trans id %d error.\n", pcidev->dev_id, trans_id);
        return;
    }

#ifdef PCIVNIC_DEBUG
    devdrv_info("dev id %d trans_id %d, status %d\n", pcidev->dev_id, trans_id, status);
#endif

    skb = pcidev->rx[trans_id].skb;
    if (skb == NULL) {
        devdrv_err("dev %d pcivnic rx head %d skb is null.\n", pcidev->dev_id, trans_id);
        return;
    }

#ifdef USE_DMA_ADDR
    dma_unmap_single(pcidev->dev, pcidev->rx[trans_id].addr, PCIVNIC_MAX_PKT_SIZE, DMA_FROM_DEVICE);
#endif

    pcidev->stat.rx_pkt++;
    pcidev->stat.rx_bytes += pcidev->rx[trans_id].len;

    skb_put(skb, pcidev->rx[trans_id].len);
    if (status != 0) {
        devdrv_err("dev %d pcivnic rx head %d status %d error.\n", pcidev->dev_id, trans_id, status);
        dev_consume_skb_any(skb);
        pcidev->stat.rx_dma_err++;
    } else {
        pcivnic_forward_packet(skb, vnic_dev, pcidev->dev_id);
        pcivnic_response_cq(pcidev, trans_id, status);
    }

    pcidev->rx[trans_id].skb = NULL;
    pcidev->rx[trans_id].addr = 0;
    pcidev->rx[trans_id].len = 0;
}

void pcivnic_rx_msg_notify_task(unsigned long data)
{
    struct pcivnic_pcidev *pcidev = (struct pcivnic_pcidev *)((uintptr_t)data);
    void *msg_chan = pcidev->msg_chan;
    struct sk_buff *skb = NULL;
    struct pcivnic_sq_desc *sq_desc = NULL;
    u64 addr;
    struct devdrv_asyn_dma_para_info dma_para_info;
    u32 head;
    int cnt = 0;
    int ret = 0;

    dma_para_info.interrupt_and_attr_flag = DEVDRV_LOCAL_IRQ_FLAG;
    dma_para_info.priv = (void *)pcidev;
    dma_para_info.finish_notify = pcivnic_rx_msg_callback;

    do {
        sq_desc = pcivnic_get_r_sq_desc(msg_chan, &head);
        if ((sq_desc == NULL) || (sq_desc->valid != PCIVNIC_VALID) ||
            (head >= PCIVNIC_DESC_QUEUE_DEPTH)) {
            break;
        }

        /* Reach the threshold and schedule out */
        if (cnt >= PCIVNIC_RX_BUDGET) {
            tasklet_schedule(&pcidev->rx_notify_task);
            break;
        }

#ifdef PCIVNIC_DEBUG
        devdrv_info("pcivnic_rx_msg_notify head %d\n", head);
#endif

        dma_para_info.trans_id = head;

        /* the receiving end cannot apply for memory, too many receive
         * buffers, and waits for the upper layer to receive packets.
         * Block live send.
         */
        skb = dev_alloc_skb(PCIVNIC_MAX_PKT_SIZE);
        if (unlikely(skb == NULL)) {
            devdrv_err("dev %d rx alloc skb failed!\n", pcidev->dev_id);
            tasklet_schedule(&pcidev->rx_notify_task);
            break;
        }

#ifdef USE_DMA_ADDR
        addr = dma_map_single(pcidev->dev, skb->data, PCIVNIC_MAX_PKT_SIZE, DMA_FROM_DEVICE);
        if (dma_mapping_error(pcidev->dev, addr)) {
            dev_kfree_skb_any(skb);
            devdrv_err("dev %d dma mapping error!\n", pcidev->dev_id);
            tasklet_schedule(&pcidev->rx_notify_task);
            break;
        }
#else
        addr = virt_to_phys(skb->data);
#endif

        pcidev->rx[head].skb = skb;
        pcidev->rx[head].addr = addr;
        pcidev->rx[head].len = sq_desc->data_len;

        /* copy the packet */
        ret = pcivnic_dma_copy(pcidev, sq_desc->data_buf_addr, addr, sq_desc->data_len, &dma_para_info);
        if (ret) {
            devdrv_warn("devid %d pcivnic_dma_copy is fail\n", pcidev->dev_id);
            pcidev->stat.rx_dma_fail++;
        }
        sq_desc->valid = PCIVNIC_INVALID;

        pcivnic_move_r_sq_desc(msg_chan);
        cnt++;
    } while (1);
}

void pcivnic_rx_msg_notify(void *msg_chan)
{
    struct pcivnic_pcidev *pcidev = pcivnic_get_pcidev(msg_chan);
    if (pcidev != NULL) {
        tasklet_schedule(&pcidev->rx_notify_task);
    }
}

unsigned long pcivnic_findout_smallest_tx_seq(struct pcivnic_netdev *vnic_dev, int *dev_id, u16 *tx_head)
{
    struct pcivnic_pcidev *pcidev = NULL;
    int i, j;
    unsigned long tx_seq = (unsigned long)-1;

    /* Search for the pkt with the smallest tx seq */
    for (i = 0; i < NETDEV_PCIDEV_NUM; i++) {
        pcidev = vnic_dev->pcidev[i];
        if (pcidev == NULL) {
            continue;
        }

        spin_lock_bh(&pcidev->lock);
        for (j = 0; j < (int)(pcidev->queue_depth); j++) {
            if (pcidev->tx[j].netdev == NULL) {
                continue;
            }
            if (pcidev->tx[j].tx_seq < tx_seq) {
                tx_seq = pcidev->tx[j].tx_seq;
                *dev_id = i;
                *tx_head = j;
            }
        }
        spin_unlock_bh(&pcidev->lock);
    }

    return tx_seq;
}

void pcivnic_net_timeout_work(struct work_struct *p_work)
{
    struct delayed_work *delayed_work = container_of(p_work, struct delayed_work, work);
    struct pcivnic_netdev *vnic_dev = container_of(delayed_work, struct pcivnic_netdev, timeout);
    struct net_device *dev = vnic_dev->ndev;
    struct pcivnic_pcidev *pcidev = NULL;
    struct pcivnic_cq_desc *cq_desc = NULL;
    u16 tx_head = 0;
    int dev_id = 0;
    unsigned long tx_seq;
    unsigned long cur_timestamp = (unsigned long)(jiffies / HZ);
    int timeout_num = 0;

find_next:

    /* Search for the pkt with the smallest tx seq */
    tx_seq = pcivnic_findout_smallest_tx_seq(vnic_dev, &dev_id, &tx_head);

    /* find out */
    if (tx_seq != (unsigned long)-1) {
        pcidev = vnic_dev->pcidev[dev_id];

        if (cur_timestamp - pcidev->tx[tx_head].timestamp > PCIVNIC_TIMESTAMP_OUT) {
            /* set the head cq desc status valid */
            cq_desc = pcivnic_get_r_cq_desc(pcidev->msg_chan);
            if (cq_desc != NULL) {
                devdrv_info(
                    "dev %d cq_desc cq id %d valid %d tx_head %d tx_seq %ld cur_timestamp %lu timestamp %lu\n",
                    dev_id, cq_desc->cq_id, cq_desc->valid, tx_head, tx_seq, cur_timestamp,
                    pcidev->tx[tx_head].timestamp);

                cq_desc->sq_head = cq_desc->cq_id;
                cq_desc->status = PCIVNIC_CQ_STS;
                cq_desc->valid = PCIVNIC_VALID;
            }
            pcidev->timeout_cnt++;

            /* multi pcie dev set linkdown  */
            if ((pcidev->timeout_cnt > PCIVNIC_TIMEOUT_CNT) && (vnic_dev->pciedev_num > 1)) {
                pcidev->status &= ~BIT_STATUS_LINK;
            }
            timeout_num++;

            /* wakeup tx finish  */
            tasklet_schedule(&pcidev->tx_finish_task);
            msleep(PCIVNIC_SLEEP_CNT);
            goto find_next;
        }
    }
    devdrv_info("net dev %s watchdog timeout num %d, cur_timestamp %lu, tx finish!\n", dev->name, timeout_num,
                cur_timestamp);
}

STATIC void pcivnic_net_timeout(struct net_device *dev)
{
    struct pcivnic_netdev *vnic_dev = netdev_priv(dev);
    struct pcivnic_pcidev *pcidev = NULL;
    struct pcivnic_cq_desc *cq_desc = NULL;
    u16 tx_head = 0;
    int dev_id = 0;
    unsigned long tx_seq;
    unsigned long cur_timestamp = (unsigned long)(jiffies / HZ);

    tx_seq = pcivnic_findout_smallest_tx_seq(vnic_dev, &dev_id, &tx_head);
    /* find out */
    if (tx_seq != (unsigned long)-1) {
        pcidev = vnic_dev->pcidev[dev_id];
        if (cur_timestamp - pcidev->tx[tx_head].timestamp > PCIVNIC_TIMESTAMP_OUT) {
            cq_desc = pcivnic_get_r_cq_desc(pcidev->msg_chan);
            if (cq_desc != NULL) {
                devdrv_info("dev %d cq id %d valid %d tx_head %d tx_seq %ld cur_timestamp %lu timestamp %lu\n", dev_id,
                            cq_desc->cq_id, cq_desc->valid, tx_head, tx_seq, cur_timestamp,
                            pcidev->tx[tx_head].timestamp);
            }
            /* wakeup tx finish  */
            tasklet_schedule(&pcidev->tx_finish_task);
        }
    }
    devdrv_info("net dev %s tx_seq %ld cur_timestamp %lu\n", dev->name, tx_seq, cur_timestamp);

    /* perhaps tx finish tasklet is block, resycle the pkts later */
    schedule_delayed_work(&vnic_dev->timeout, PCIVNIC_DELAYWORK_TIME * HZ);
}

STATIC int pcivnic_net_change_mtu(struct net_device *dev, int new_mtu)
{
    if ((new_mtu < PCIVNIC_MTU_LOW) || (new_mtu > PCIVNIC_MTU_HIGH)) {
        devdrv_err("mtu value is invalid!\n");
        return -EINVAL;
    }
    dev->mtu = new_mtu;

    return 0;
}

STATIC struct net_device_stats *pcivnic_net_get_stats(struct net_device *dev)
{
    return &dev->stats;
}

struct ethtool_ops g_pcivnic_ethtools_ops = {
    .get_drvinfo = pcivnic_net_get_drvinfo,
    .get_link = pcivnic_net_get_link,
    .get_strings = pcivnic_get_strings,
    .get_sset_count = pcivnic_get_sset_count,
    .get_ethtool_stats = pcivnic_get_ethtool_stats,
};

struct net_device_ops g_pcivnic_netdev_ops = {
    .ndo_open = pcivnic_net_open,
    .ndo_stop = pcivnic_net_close,
    .ndo_start_xmit = pcivnic_net_xmit,
#ifndef RHEL_RELEASE_CODE
    .ndo_change_mtu = pcivnic_net_change_mtu,
#endif
    .ndo_tx_timeout = pcivnic_net_timeout,
    .ndo_get_stats = pcivnic_net_get_stats,
};

STATIC ssize_t pcivnic_read_file(struct file *file, loff_t *pos, char *addr, size_t count)
{
    ssize_t len;

#if LINUX_VERSION_CODE >= KERNEL_VERSION(4, 14, 0)
    len = kernel_read(file, addr, count, pos);
#else

    char __user *buf = (char __user *)addr;
    mm_segment_t old_fs;

    old_fs = get_fs();
    set_fs(get_ds()); /*lint !e501 */ /* kernel source */
    len = vfs_read(file, buf, count, pos);
    set_fs(old_fs);
#endif

    return len;
}

char *pcivnic_skip_blank_space(const char *ptr)
{
    while ((*ptr == ' ') || (*ptr == '\t') || (*ptr == '\n')) {
        ptr++;
    }

    return (char *)ptr;
}

char *pcivnic_skip_line(const char *ptr)
{
    while ((*ptr != '\n') && (*ptr != '\0')) {
        ptr++;
    }

    if (*ptr == '\n') {
        ptr++;
    }

    return pcivnic_skip_blank_space(ptr);
}

char *pcivnic_skip_notes(const char *ptr)
{
    /* skip blank space  */
    ptr = pcivnic_skip_blank_space(ptr);

    /* skip notes with # */
    while (*ptr == '#') {
        ptr = pcivnic_skip_line(ptr);
    }

    return (char *)ptr;
}

int pcivnic_is_valid_mac(unsigned char *mac, int len)
{
    (void)len;

    return is_valid_ether_addr(mac);
}

void pcivnic_get_mac(unsigned char last_byte, unsigned char *mac)
{
    unsigned int tmpmac[ETH_ALEN];
    struct file *file = NULL;
    char *tmpbuf = NULL;
    char *pmac = NULL;
    loff_t offset = 0;
    int tmp_id = -1;
    int len;
    const int dev_id = 0;
    int ret = 0;

    file = filp_open(PCIVNIC_MAC_FILE, O_RDONLY, 0);
    if (IS_ERR(file)) {
        devdrv_warn("pcivnic mac config file(%s) not exited\n", PCIVNIC_MAC_FILE);
        goto _random_mac;
    }

    tmpbuf = (char *)kzalloc(PCIVNIC_CONF_FILE_SIZE, GFP_KERNEL);
    if (tmpbuf == NULL) {
        devdrv_err("pcivnic mac buffer malloc failed, size %d\n", PCIVNIC_CONF_FILE_SIZE);
        goto _close_file;
    }

    len = pcivnic_read_file(file, &offset, tmpbuf, PCIVNIC_CONF_FILE_SIZE - 1);
    if (len <= 0) {
        devdrv_warn("read file(%s) failed\n", PCIVNIC_MAC_FILE);
        goto _free_buf;
    }

    tmpbuf[len] = '\0';
    pmac = &tmpbuf[0];
    pmac = pcivnic_skip_notes(pmac);

    /* find the mac address by device id */
    while (*pmac != '\0') {
        tmp_id = -1;
        ret = sscanf_s(pmac, "%d %x:%x:%x:%x:%x:%x", &tmp_id, &tmpmac[PCIVNIC_MAC_0], &tmpmac[PCIVNIC_MAC_1],
                       &tmpmac[PCIVNIC_MAC_2], &tmpmac[PCIVNIC_MAC_3], &tmpmac[PCIVNIC_MAC_4], &tmpmac[PCIVNIC_MAC_5]);
        if (ret != (ETH_ALEN + 1)) {
                devdrv_warn("sscanf_s failed! ret = %d\n", ret);
                goto _free_buf;
        }

        if ((ret == PCIVNIC_CONF_SSCANF_OK) && (tmp_id == dev_id)) {
            break;
        }

        pmac = pcivnic_skip_line(pmac);
    }

    if (tmp_id != dev_id) {
        devdrv_warn("can't find device %d fixed mac address\n", dev_id);
        goto _free_buf;
    }

    mac[PCIVNIC_MAC_0] = tmpmac[PCIVNIC_MAC_0] & 0xFF;
    mac[PCIVNIC_MAC_1] = tmpmac[PCIVNIC_MAC_1] & 0xFF;
    mac[PCIVNIC_MAC_2] = tmpmac[PCIVNIC_MAC_2] & 0xFF;
    mac[PCIVNIC_MAC_3] = tmpmac[PCIVNIC_MAC_3] & 0xFF;
    mac[PCIVNIC_MAC_4] = tmpmac[PCIVNIC_MAC_4] & 0xFF;
    mac[PCIVNIC_MAC_5] = last_byte;

    if (!pcivnic_is_valid_mac(mac, ETH_ALEN)) {
        devdrv_warn("The MAC is invalid form file %s\n", PCIVNIC_MAC_FILE);
        goto _free_buf;
    }

    kfree(tmpbuf);
    tmpbuf = NULL;
    filp_close(file, NULL);
    file = NULL;

    devdrv_info("pcivnic device %d, MAC : %pM\n", dev_id, mac);
    return;

_free_buf:
    kfree(tmpbuf);
    tmpbuf = NULL;
_close_file:
    filp_close(file, NULL);
    file = NULL;
_random_mac:
    mac[PCIVNIC_MAC_0] = PCIVNIC_MAC_VAL_0;
    mac[PCIVNIC_MAC_1] = PCIVNIC_MAC_VAL_1;
    mac[PCIVNIC_MAC_2] = PCIVNIC_MAC_VAL_2;
    mac[PCIVNIC_MAC_3] = PCIVNIC_MAC_VAL_3;
    mac[PCIVNIC_MAC_4] = PCIVNIC_MAC_VAL_4;
    mac[PCIVNIC_MAC_5] = last_byte;
    devdrv_info("pcivnic using default MAC : %pM\n", mac);

    return;
}

void pcivnic_set_netdev_mac(struct pcivnic_netdev *vnic_dev, unsigned char *mac)
{
    ether_addr_copy(vnic_dev->ndev->dev_addr, mac);
}

int pcivnic_register_netdev(struct pcivnic_netdev *vnic_dev)
{
    int ret;

    ret = register_netdev(vnic_dev->ndev);
    if (ret) {
        devdrv_err("%s, dma register_ndev failed!\n", vnic_dev->ndev->name);
        return ret;
    }

    vnic_dev->ndev_register = PCIVNIC_VALID;
    return ret;
}

void pcivnic_init_msgchan_cq_desc(void *msg_chan)
{
    struct pcivnic_cq_desc *cq_desc = NULL;
    struct pcivnic_cq_desc *w_cq_desc = NULL;
    int i;

    cq_desc = pcivnic_get_r_cq_desc(msg_chan);
    w_cq_desc = pcivnic_get_w_cq_desc(msg_chan);
    if ((cq_desc == NULL) || (w_cq_desc == NULL)) {
        devdrv_err("cq_desc is NULL\n");
        return;
    }
    for (i = 0; i < PCIVNIC_DESC_QUEUE_DEPTH; i++) {
        cq_desc->cq_id = i;
        cq_desc++;
        w_cq_desc->cq_id = i;
        w_cq_desc++;
    }
}

ssize_t pcivnic_get_dev_stat(struct device *dev, struct device_attribute *attr, char *buf)
{
    struct pcivnic_pcidev *pcidev = NULL;
    struct pcivnic_fwd_stat *fwd_stat = NULL;
    ssize_t offset = 0;
    int ret;
    int i;
    u64 num;

    (void)attr;

    pcidev = pcivnic_get_pciedev(dev);
    if (pcidev == NULL) {
        devdrv_err("not find pcidev\r\n");
        return offset;
    }

    ret = snprintf_s(buf, PAGE_SIZE, PAGE_SIZE - 1, "dev_id: %d\n", pcidev->dev_id);
    if (ret >= 0) {
        offset += ret;
    }

    ret = snprintf_s(buf + offset, PAGE_SIZE - offset, PAGE_SIZE - offset - 1,
        "    tx_full: %llu\n    rx_dma_err: %llu\n    rx_dma_fail: %llu\n",
        pcidev->stat.tx_full, pcidev->stat.rx_dma_err, pcidev->stat.rx_dma_fail);
    if (ret >= 0) {
        offset += ret;
    }

    ret = snprintf_s(buf + offset, PAGE_SIZE - offset, PAGE_SIZE - offset - 1,
                     "    tx_pkt: %llu\n    tx_bytes: %llu\n    rx_pkt: %llu\n    rx_bytes: %llu\n",
                     pcidev->stat.tx_pkt, pcidev->stat.tx_bytes, pcidev->stat.rx_pkt, pcidev->stat.rx_bytes);
    if (ret >= 0) {
        offset += ret;
    }

    ret = snprintf_s(buf + offset, PAGE_SIZE - offset, PAGE_SIZE - offset - 1,
        "\n     dev|        fwd_all       |      fwd_success     |     disable_drop     |    flow_ctrl_drop    \n");
    if (ret >= 0) {
        offset += ret;
    }

    for (i = 0; i < NETDEV_PCIDEV_NUM; i++) {
        fwd_stat = &pcidev->fwd_stat[i];
        num = fwd_stat->fwd_all + fwd_stat->fwd_success + fwd_stat->disable_drop + fwd_stat->flow_ctrl_drop;
        if (num > 0) {
            ret = snprintf_s(buf + offset, PAGE_SIZE - offset, PAGE_SIZE - offset - 1,
                             "     %-3d|%-22llu|%-22llu|%-22llu|%-22llu\n",
                             i, fwd_stat->fwd_all, fwd_stat->fwd_success,
                             fwd_stat->disable_drop, fwd_stat->flow_ctrl_drop);
            if (ret >= 0) {
                offset += ret;
            }
        }
    }

    return offset;
}

static DEVICE_ATTR(stat, S_IRUSR | S_IRGRP, pcivnic_get_dev_stat, NULL);

static struct attribute *g_pcivnic_sysfs_attrs[] = {
    &dev_attr_stat.attr,
    NULL,
};

static const struct attribute_group g_pcivnic_sysfs_group = {
    .attrs = g_pcivnic_sysfs_attrs,
    .name = "vnic",
};

struct pcivnic_pcidev *pcivnic_add_dev(struct pcivnic_netdev *vnic_dev, struct device *dev, u32 queue_depth,
                                       int net_dev_id)
{
    struct pcivnic_pcidev *pcidev = NULL;
    u32 i;

    pcidev = (struct pcivnic_pcidev *)vzalloc(sizeof(struct pcivnic_pcidev));
    if (pcidev == NULL) {
        devdrv_err("dev id %d alloc err.", net_dev_id);
        return NULL;
    }

    pcidev->dev = dev;
    pcidev->netdev = (void *)vnic_dev;

    /* init skb queue */
    pcidev->queue_depth = queue_depth;

    for (i = 0; i < queue_depth; i++) {
        pcidev->rx[i].skb = NULL;
        pcidev->tx[i].skb = NULL;
    }

    for (i = 0; i < NETDEV_PCIDEV_NUM; i++) {
        pcidev->flow_ctrl[i].threshold = PCIVNIC_FLOW_CTRL_THRESHOLD;
        pcidev->flow_ctrl[i].timestamp = (unsigned long)jiffies;
    }

    spin_lock_init(&pcidev->lock);

    tasklet_init(&pcidev->tx_finish_task, pcivnic_tx_finish_notify_task, (uintptr_t)pcidev);
    tasklet_init(&pcidev->rx_notify_task, pcivnic_rx_msg_notify_task, (uintptr_t)pcidev);

    vnic_dev->pcidev[net_dev_id] = pcidev;
    vnic_dev->pciedev_num++;

    if (sysfs_create_group(&dev->kobj, &g_pcivnic_sysfs_group) != 0) {
        devdrv_warn("net_dev_id %d sysfs_create_group failed!\n", net_dev_id);
    }

    devdrv_info("net_dev_id %d add dev!\n", net_dev_id);

    return pcidev;
}

void pcivnic_del_dev(struct pcivnic_netdev *vnic_dev, int dev_id)
{
    struct pcivnic_pcidev *pcidev = NULL;
    u32 i;

    pcidev = vnic_dev->pcidev[dev_id];
    vnic_dev->pcidev[dev_id] = NULL;
    vnic_dev->pciedev_num--;

    sysfs_remove_group(&pcidev->dev->kobj, &g_pcivnic_sysfs_group);

    tasklet_kill(&pcidev->tx_finish_task);
    tasklet_kill(&pcidev->rx_notify_task);

    for (i = 0; i < pcidev->queue_depth; i++) {
        if (pcidev->tx[i].skb) {
#ifdef USE_DMA_ADDR
            dma_unmap_single(pcidev->dev, pcidev->tx[i].addr, pcidev->tx[i].skb->len, DMA_TO_DEVICE);
#endif
            dev_consume_skb_any(pcidev->tx[i].skb);
        }

        if (pcidev->rx[i].skb) {
#ifdef USE_DMA_ADDR
            dma_unmap_single(pcidev->dev, pcidev->rx[i].addr, PCIVNIC_MAX_PKT_SIZE, DMA_FROM_DEVICE);
#endif
            dev_consume_skb_any(pcidev->rx[i].skb);
        }
    }

    msleep(PCIVNIC_SLEEP_CNT);

    vfree(pcidev);
    pcidev = NULL;
}

struct pcivnic_netdev *pcivnic_alloc_netdev(const char *ndev_name, int ndev_name_len)
{
    struct pcivnic_netdev *vnic_dev = NULL;
    struct net_device *ndev = NULL;
    u32 i;

    (void)ndev_name_len;
    /* init netdev */
#if LINUX_VERSION_CODE >= KERNEL_VERSION(3, 17, 0)
    ndev = alloc_netdev(sizeof(struct pcivnic_netdev), ndev_name, NET_NAME_USER, ether_setup);
#else
    ndev = alloc_netdev(sizeof(struct pcivnic_netdev), ndev_name, ether_setup);
#endif
    if (ndev == NULL) {
        devdrv_err("alloc netdev fail\n");
        return NULL;
    }

    vnic_dev = netdev_priv(ndev);
    vnic_dev->ndev_register = PCIVNIC_INVALID;
    vnic_dev->ndev = ndev;

    ndev->netdev_ops = &g_pcivnic_netdev_ops;
    ndev->ethtool_ops = &g_pcivnic_ethtools_ops;

    ndev->watchdog_timeo = PCIVNIC_WATCHDOG_TIME * HZ;
    ndev->hw_features |= NETIF_F_GRO;
    ndev->features |= NETIF_F_HIGHDMA | NETIF_F_GSO;

    vnic_dev->status &= ~BIT_STATUS_LINK;
    vnic_dev->status &= ~BIT_STATUS_TQ_FULL;
    vnic_dev->status &= ~BIT_STATUS_RQ_FULL;

    spin_lock_init(&vnic_dev->lock);
    spin_lock_init(&vnic_dev->rx_lock);

    vnic_dev->pciedev_num = 0;

    INIT_DELAYED_WORK(&vnic_dev->timeout, pcivnic_net_timeout_work);

    for (i = 0; i < NETDEV_PCIDEV_NUM; i++) {
        vnic_dev->pcidev[i] = NULL;
    }

    /* init napi */
    netif_napi_add(vnic_dev->ndev, &vnic_dev->napi, pcivnic_napi, PCIVNIC_NAPI_POLL_WEIGHT);

    return vnic_dev;
}

void pcivnic_free_netdev(struct pcivnic_netdev *vnic_dev)
{
    netif_napi_del(&vnic_dev->napi);

    if (vnic_dev->ndev_register == PCIVNIC_VALID) {
        unregister_netdev(vnic_dev->ndev);
    }

    cancel_delayed_work_sync(&vnic_dev->timeout);

    free_netdev(vnic_dev->ndev);
}
