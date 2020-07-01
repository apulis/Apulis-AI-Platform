/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2012-2019. All rights reserved.
 * Description: This file contains pcivnic_main Info.
 * Author: huawei
 * Create: 2017-10-15
 */

#ifndef _PCIVNIC_MAIN_H_
#define _PCIVNIC_MAIN_H_

#include <linux/types.h>
#include <linux/device.h>
#include <linux/netdevice.h>
#include <linux/skbuff.h>
#include <linux/bitops.h>
#include <linux/interrupt.h>

#include "drv_log.h"
#include "devdrv_interface.h"
#include "pcivnic_config.h"

#ifdef VNIC_ENV_DEVICE
#include <linux/securec.h>
#else
#include "securec.h"
#endif

#define module_devdrv "pcivnic"

#define devdrv_err(fmt...) drv_err(module_devdrv, fmt)
#define devdrv_warn(fmt...) drv_warn(module_devdrv, fmt)
#define devdrv_info(fmt...) drv_info(module_devdrv, fmt)
#define devdrv_debug(fmt...) drv_debug(module_devdrv, fmt)

#define USE_DMA_ADDR

#define PCIVNIC_VALID 1
#define PCIVNIC_INVALID 0

#define PCIVNIC_DESC_QUEUE_DEPTH 128
#define PCIVNIC_NET_LINKED 1

/* The kernel will add other fields to ensure no more than 1 page */
#define PCIVNIC_MAX_PKT_SIZE 3840
#define PCIVNIC_MTU_DEFAULT 3000

#define PCIVNIC_RX_BUDGET 64
#define PCIVNIC_TX_BUDGET 64

/* vnic napi poll small weight avoid tcp backlog drop too many pkt */
#define PCIVNIC_NAPI_POLL_WEIGHT 4

#define PCIVNIC_MTU_LOW 68
#define PCIVNIC_MTU_HIGH 3900

#define PCIVNIC_NAME_SIZE 64

#define PCIVNIC_MAC_FILE "/etc/d-pcivnic.conf"
#define PCIVNIC_CONF_FILE_SIZE  4096
#define PCIVNIC_CONF_SSCANF_OK  7

#define BIT_STATUS_LINK BIT(0)
#define BIT_STATUS_TQ_FULL BIT(1)
#define BIT_STATUS_RQ_FULL BIT(2)

#define PCIVNIC_NEXT_HOP_LOCAL_NETDEV 0xd3
#define PCIVNIC_NEXT_HOP_BROADCAST 0xff
#define PCIVNIC_INIT_INSTANCE_TIMEOUT (4 * HZ)

#define PCIVNIC_MAC_0 0
#define PCIVNIC_MAC_1 1
#define PCIVNIC_MAC_2 2
#define PCIVNIC_MAC_3 3
#define PCIVNIC_MAC_4 4
#define PCIVNIC_MAC_5 5
#define PCIVNIC_MAC_VAL_0 0x00
#define PCIVNIC_MAC_VAL_1 0xe0
#define PCIVNIC_MAC_VAL_2 0xfc
#define PCIVNIC_MAC_VAL_3 0xfc
#define PCIVNIC_MAC_VAL_4 0xd3

#define PCIVNIC_LINKDOWN_NUM 100
#define PCIVNIC_TIMESTAMP_OUT 4
#define PCIVNIC_TIMEOUT_CNT 5
#define PCIVNIC_CQ_STS 0xe0
#define PCIVNIC_SLEEP_CNT 10
#define PCIVNIC_DELAYWORK_TIME 2
#define PCIVNIC_WATCHDOG_TIME 6

#define VNIC_DBG(args...) do { \
    ;                 \
} while (0)

struct pcivnic_sq_desc {
    u64 data_buf_addr;
    u32 data_len;
    u32 valid;
};
#define PCIVNIC_SQ_DESC_SIZE sizeof(struct pcivnic_sq_desc)

struct pcivnic_cq_desc {
    u32 sq_head;
    u32 status;
    u32 cq_id;
    u32 valid;
};
#define PCIVNIC_CQ_DESC_SIZE sizeof(struct pcivnic_cq_desc)

struct pcivnic_skb_addr {
    struct sk_buff *skb;
    u64 addr;
    int len;
    void *netdev;
    unsigned long tx_seq;
    unsigned long timestamp;
};

#define PCIVNIC_FLOW_CTRL_PERIOD 100 /* ms */
#define PCIVNIC_FLOW_CTRL_THRESHOLD 200 /* 2kpps */

struct pcivnic_flow_ctrl {
    unsigned long timestamp;
    u32 pkt;
    u32 threshold;
};

struct pcivnic_dev_stat {
    u64 tx_full;
    u64 rx_dma_err;
    u64 rx_dma_fail;
    u64 tx_pkt;
    u64 tx_bytes;
    u64 rx_pkt;
    u64 rx_bytes;
};

struct pcivnic_fwd_stat {
    u64 fwd_all;
    u64 fwd_success;
    u64 disable_drop;
    u64 flow_ctrl_drop;
};

struct pcivnic_pcidev {
    struct device *dev;
    u32 queue_depth;
    u32 dev_id;
    u32 status;
    u32 timeout_cnt;
    struct pcivnic_dev_stat stat;
    struct pcivnic_fwd_stat fwd_stat[NETDEV_PCIDEV_NUM];
    struct pcivnic_flow_ctrl flow_ctrl[NETDEV_PCIDEV_NUM];
    void *msg_chan;
    void *priv;
    void *netdev;
    spinlock_t lock;
    struct tasklet_struct tx_finish_task;
    struct tasklet_struct rx_notify_task;
    struct pcivnic_skb_addr tx[PCIVNIC_DESC_QUEUE_DEPTH];
    struct pcivnic_skb_addr rx[PCIVNIC_DESC_QUEUE_DEPTH];
    struct delayed_work msg_init;
    struct semaphore vnic_instance_sem;
};

struct pcivnic_netdev {
    struct net_device *ndev;
    struct napi_struct napi;
    u32 ndev_register;
    u32 status;
    u32 pciedev_num;
    spinlock_t lock;
    spinlock_t rx_lock;
    struct delayed_work timeout;
    struct pcivnic_pcidev *pcidev[NETDEV_PCIDEV_NUM];
    struct sk_buff_head skbq;
};

struct pcivnic_ctrl_msg {
    unsigned char mac[ETH_ALEN];
};

extern struct pcivnic_pcidev *pcivnic_get_pcidev(void *msg_chan);
extern struct pcivnic_sq_desc *pcivnic_get_w_sq_desc(void *msg_chan, u32 *tail);
extern void pcivnic_set_w_sq_desc_head(void *msg_chan, u32 head);
extern void pcivnic_copy_sq_desc_to_remote(struct pcivnic_pcidev *pcidev, struct pcivnic_sq_desc *sq_desc);
extern bool pcivnic_w_sq_full_check(void *msg_chan);
extern struct pcivnic_sq_desc *pcivnic_get_r_sq_desc(void *msg_chan, u32 *head);
extern void pcivnic_move_r_sq_desc(void *msg_chan);
extern int pcivnic_dma_copy(struct pcivnic_pcidev *pcidev, u64 src, u64 dst, u32 size,
                            struct devdrv_asyn_dma_para_info *para_info);
extern struct pcivnic_cq_desc *pcivnic_get_w_cq_desc(void *msg_chan);
extern void pcivnic_copy_cq_desc_to_remote(struct pcivnic_pcidev *pcidev, struct pcivnic_cq_desc *cq_desc);
extern struct pcivnic_cq_desc *pcivnic_get_r_cq_desc(void *msg_chan);
extern void pcivnic_move_r_cq_desc(void *msg_chan);

extern struct pcivnic_pcidev *pcivnic_get_pciedev(struct device *dev);

extern bool pcivnic_is_p2p_enabled(u32 dev_id, u32 peer_dev_id);

extern int pcivnic_up_get_next_hop(unsigned char *dmac);
extern int pcivnic_down_get_next_hop(unsigned char *dmac);

extern void pcivnic_get_mac(unsigned char last_byte, unsigned char *mac);
extern void pcivnic_set_netdev_mac(struct pcivnic_netdev *vnic_dev, unsigned char *mac);

extern void pcivnic_rx_msg_notify(void *msg_chan);
extern void pcivnic_tx_finish_notify(void *msg_chan);
extern struct pcivnic_pcidev *pcivnic_add_dev(struct pcivnic_netdev *vnic_dev, struct device *dev, u32 queue_depth,
                                              int net_dev_id);
extern void pcivnic_del_dev(struct pcivnic_netdev *vnic_dev, int dev_id);
extern struct pcivnic_netdev *pcivnic_alloc_netdev(const char *ndev_name, int ndev_name_len);
extern void pcivnic_free_netdev(struct pcivnic_netdev *vnic_dev);
extern int pcivnic_register_netdev(struct pcivnic_netdev *vnic_dev);

extern void pcivnic_init_msgchan_cq_desc(void *msg_chan);

extern int pcivnic_device_status_abnormal(void *msg_chan);

#endif  // _DEVDRV_MAIN_H_
