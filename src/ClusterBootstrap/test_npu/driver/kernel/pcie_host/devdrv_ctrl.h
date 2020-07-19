/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2012-2019. All rights reserved.
 * Description:
 * Author: huawei
 * Create: 2017/8/18
 */

#ifndef _DEVDRV_CTRL_H_
#define _DEVDRV_CTRL_H_

#include "devdrv_interface.h"
#include "devdrv_msg.h"
#include "devdrv_pci.h"

#define DEVDRV_MAX_DELAY_COUNT 20
#define DEVDRV_HOT_RESET_DELAY 3
#define DEVDRV_HOT_RESET_AFTER_DELAY 10
#define DEVDRV_HOT_RESAN_AFTER_DELAY 50
#define MAX_OS_EACH_AISERVER 2
#define MAX_DEV_CNT 64
#define MEM_BAR_LEN_64M 0x4000000
#define MEM_BAR_LEN_128M 0x8000000
#define MEM_BAR_LEN_4G 0x100000000
#define MEM_BAR_LEN_8G 0x200000000
#define MEM_BAR_LEN_16G 0x400000000
#define DEVDRV_PCIE_HOT_RESET_FLAG 0x5b5bb5b5
#define DEVDRV_LOAD_FILE_DELAY 10000
#define DEVDRV_DEVFN_BIT 3
#define DEVDRV_DEVFN_DEV_VAL 0x1F
#define DEVDRV_DEVFN_FN_VAL 0x7
#define DEVDRV_RESERVE_NUM 2
#define DEVDRV_MOVE_BIT_32 32
#define DEVDRV_MAX_FUNC_NUM 2

struct devdrv_ops {
    /* trans msg */
    struct devdrv_msg_chan *(*alloc_trans_chan)(void *priv, struct devdrv_trans_msg_chan_info *chan_info);
    int (*realease_trans_chan)(struct devdrv_msg_chan *msg_chan);

    /* non-trans msg */
    struct devdrv_msg_chan *(*alloc_non_trans_chan)(void *priv, struct devdrv_non_trans_msg_chan_info *chan_info);
    int (*release_non_trans_chan)(struct devdrv_msg_chan *msg_chan);
};

/* record startup mini and report to dev manager */
struct devdrv_dev_state_ctrl {
    /* 所有mini个数 */
    u32 dev_num;
    /* 初次上报标记 */
    u32 first_flag;
    u32 devid[MAX_DEV_CNT];
    u32 hotreset_devid[MAX_DEV_CNT];
    devdrv_dev_startup_notify startup_callback;
    devdrv_dev_state_notify state_notifier_callback;
};

#define DEVDRV_P2P_MAX_PROC_NUM 32

struct devdrv_p2p_attr_info {
    int ref;
    int proc_ref[DEVDRV_P2P_MAX_PROC_NUM];
    int pid[DEVDRV_P2P_MAX_PROC_NUM];
};

void devdrv_slave_dev_delete(u32 dev_id);

u32 devdrv_get_devid_by_dev(struct devdrv_msg_dev *msg_dev);

int devdrv_register_pci_devctrl(struct devdrv_pci_ctrl *pci_ctrl);
void devdrv_set_device_boot_status(struct devdrv_pci_ctrl *pci_ctrl, u32 status);

void devdrv_set_host_phy_mach_flag(struct devdrv_pci_ctrl *pci_ctrl, u32 host_flag);
int devdrv_instance_callback_after_probe(u32 dev_id);
int devdrv_dev_online(struct devdrv_pci_ctrl *pci_ctrl);
void devdrv_dev_offline(struct devdrv_pci_ctrl *pci_ctrl);
int devdrv_ctrl_init(void);
int devdrv_init_pci_devctrl(struct devdrv_pci_ctrl *pci_ctrl);
struct devdrv_ctrl *devdrv_get_top_half_devctrl_by_id(u32 dev_id);
struct devdrv_ctrl *devdrv_get_bottom_half_devctrl_by_id(u32 dev_id);
struct devdrv_ctrl *devdrv_get_devctrl_by_id(u32 i);
void devdrv_init_dev_startup_ctrl(void);
void drvdrv_dev_startup_record(u32 dev_id);
void drvdrv_dev_startup_report(u32 dev_id);
void drvdrv_dev_state_notifier(struct devdrv_pci_ctrl *pci_ctrl);
void devdrv_set_devctrl_startup_flag(u32 dev_id, enum devdrv_dev_startup_flag_type flag);
void devdrv_register_half_devctrl(struct devdrv_pci_ctrl *pci_ctrl);

struct pci_dev *devdrv_get_device_pf(struct pci_dev *pdev, unsigned int pf_num);
void devdrv_set_startup_status(struct devdrv_pci_ctrl *pci_ctrl, int status);
void devdrv_probe_wait(int devid);

#endif
