/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2019-2020. All rights reserved.
 * Description: virtmng interfaces
 * Author: huawei
 * Create: 2019-12-01
 */
#ifndef VIRTMNG_INTERFACE_H
#define VIRTMNG_INTERFACE_H
#include <linux/types.h>
#include <linux/mutex.h>

#define VMNG_PDEV_MAX 16
#define VMNG_VDEV_MAX_PER_PDEV 16

/* Public Define */
enum vmng_client_type {
    VMNG_CLIENT_TYPE_TEST = 0,
    VMNG_CLIENT_TYPE_HDC,
    VMNG_CLIENT_TYPE_DEVMM,
    VMNG_CLIENT_TYPE_DEVMNG,
    VMNG_CLIENT_TYPE_DEVDRV,
    VMNG_CLIENT_TYPE_MAX
};

enum vmng_startup_flag_type {
    VMNG_STARTUP_UNPROBED = 0,
    VMNG_STARTUP_PROBED,
    VMNG_STARTUP_TOP_HALF_OK,
    VMNG_STARTUP_BOTTOM_HALF_OK
};

/* VPC */
enum vmng_vpc_type {
    VMNG_VPC_TYPE_TEST = 0,
    VMNG_VPC_TYPE_HDC,
    VMNG_VPC_TYPE_DEVMEM,
    VMNG_VPC_TYPE_DEVMNG,
    VMNG_VPC_TYPE_DEVDRV,
    VMNG_VPC_TYPE_MAX,
};

struct vmng_tx_msg_proc_info {
    void *data;
    u32 in_data_len;
    u32 out_data_len;
    u32 real_out_len;
};

struct vmng_rx_msg_proc_info {
    void *data;
    u32 in_data_len;
    u32 out_data_len;
    u32 *real_out_len;
};

struct vmng_vpc_client {
    enum vmng_vpc_type vpc_type;
    int (*init)(void);
    int (*msg_recv)(u32 dev_id, u32 fid, struct vmng_rx_msg_proc_info *proc_info);
};

int vmngh_vpc_register_client(struct vmng_vpc_client *vpc_client);
void vmngh_vpc_unregister_client(struct vmng_vpc_client *vpc_client);
int vmngh_vpc_msg_send(u32 dev_id, u32 fid, enum vmng_vpc_type vpc_type, struct vmng_tx_msg_proc_info *tx_info);

int vmnga_vpc_register_client(struct vmng_vpc_client *vpc_client);
void vmnga_vpc_unregister_client(struct vmng_vpc_client *vpc_client);
int vmnga_vpc_msg_send(u32 dev_id, enum vmng_vpc_type vpc_type, struct vmng_tx_msg_proc_info *tx_info);

/* Common msg */
enum vmng_msg_common_type {
    VMNG_MSG_COMMON_TYPE_EXTENSION = 0x0,
    VMNG_MSG_COMMON_TYPE_TEST,
    VMNG_MSG_COMMON_TYPE_HDC,
    VMNG_MSG_COMMON_TYPE_DEVMM,
    VMNG_MSG_COMMON_TYPE_DEVMNG,
    VMNG_MSG_COMMON_TYPE_DEVDRV,
    VMNG_MSG_COMMON_TYPE_MAX
};

struct vmng_common_msg_client {
    enum vmng_msg_common_type type;
    void (*init)(u32 dev_id, u32 fid, int status);
    int (*common_msg_recv)(u32 dev_id, u32 fid, struct vmng_rx_msg_proc_info *proc_info);
};

/* HOST */
struct vmngh_ctrl {
    void *vd_dev;
    void *vdavinci;
    enum vmng_startup_flag_type startup_flag;
    u32 dev_id;
    u32 fid;
    u32 dtype;
};

struct vmngh_client_instance {
    void *priv;
    struct vmngh_ctrl *dev_ctrl;
    struct mutex flag_mutex;
    enum vmng_client_type type;
    u32 flag;
};

struct vmngh_client {
    enum vmng_client_type type;
    int (*init_instance)(struct vmngh_client_instance *instance);
    int (*uninit_instance)(struct vmngh_client_instance *instance);
    int (*suspend)(struct vmngh_client_instance *instance);
};
int vmngh_register_client(struct vmngh_client *client);
int vmngh_unregister_client(struct vmngh_client *client);
int vmngh_common_msg_send(u32 dev_id, u32 fid, enum vmng_msg_common_type cmn_type,
    struct vmng_tx_msg_proc_info *tx_info);
int vmngh_register_common_msg_client(struct vmng_common_msg_client *msg_client);
int vmngh_unregister_common_msg_client(struct vmng_common_msg_client *msg_client);
int vmnga_common_msg_send(u32 dev_id, enum vmng_msg_common_type cmn_type, struct vmng_tx_msg_proc_info *tx_info);

/* Agent */
struct vmnga_ctrl {
    struct device *dev;
    struct pci_bus *bus;
    struct pci_dev *pdev;
    void *unit;
    enum vmng_startup_flag_type startup_flag;
    u32 dev_id;
    u32 dtype;
};

struct vmnga_client_instance {
    void *priv;
    struct vmnga_ctrl *dev_ctrl;
    struct mutex flag_mutex;
    enum vmng_client_type type;
    u32 flag;
};

struct vmnga_client {
    enum vmng_client_type type;
    int (*init_instance)(struct vmnga_client_instance *instance);
    int (*uninit_instance)(struct vmnga_client_instance *instance);
    int (*suspend)(struct vmnga_client_instance *instance);
};
int vmnga_register_client(struct vmnga_client *client);
int vmnga_unregister_client(struct vmnga_client *client);

int vmnga_register_common_msg_client(struct vmng_common_msg_client *msg_client);
int vmnga_unregister_common_msg_client(struct vmng_common_msg_client *msg_client);

/* dma code */
#include "devdrv_interface.h"
struct vmng_para {
    u32 devid;
    u32 fid;
    void *para1;
    void *para2;
    void *para3;
    void *para4;
};
int vmnga_dma_copy_sync_link(u32 dev_id, enum devdrv_dma_data_type type, struct devdrv_dma_node *dma_node,
    u32 node_cnt);

#endif
