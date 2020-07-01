/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2012-2019. All rights reserved.
 * Description:
 * Author: huawei
 * Create: 2017/8/18
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

#include <asm/errno.h>
#include <linux/kernel.h>
#include <linux/string.h>
#include <linux/types.h>
#include <linux/pci.h>
#include <linux/delay.h>
#include <linux/device.h>
#include <linux/version.h>
#include <linux/jiffies.h>
#include <linux/iommu.h>

#include "devdrv_ctrl.h"
#include "devdrv_dma.h"
#include "devdrv_pci.h"
#include "devdrv_msg.h"
#include "devdrv_util.h"
#include "resource_comm_drv.h"

struct devdrv_ctrl g_ctrls[MAX_DEV_CNT];
struct devdrv_dev_state_ctrl g_state_ctrl;
struct devdrv_client *g_clients[DEVDRV_CLIENT_TYPE_MAX] = {
    NULL,
};
struct devdrv_black_callback g_black_box = {
    .callback = NULL
};
struct devdrv_client_instance g_clients_instance[DEVDRV_CLIENT_TYPE_MAX][MAX_DEV_CNT];

struct devdrv_p2p_attr_info g_p2p_attr_info[DEVDRV_P2P_SURPORT_MAX_DEVICE][DEVDRV_P2P_SURPORT_MAX_DEVICE];

struct mutex g_devdrv_ctrl_mutex; /* Used to walk the hash */
int g_devdrv_ctrl_hot_reset_status = 0;
int g_devdrv_ctrl_hot_reset_dev_id = 0;

#ifndef readq
static inline u64 readq(void __iomem *addr)
{
    return readl(addr) + ((u64)readl(addr + 4) << 32);
}
#endif

#ifndef writeq
static inline void writeq(u64 value, volatile void *addr)
{
    *(volatile u64 *)addr = value;
}
#endif

void devdrv_dev_startup_ctrl_init(void)
{
    int i;
    g_state_ctrl.dev_num = 0;
    g_state_ctrl.first_flag = 0;
    g_state_ctrl.startup_callback = NULL;
    g_state_ctrl.state_notifier_callback = NULL;
    for (i = 0; i < MAX_DEV_CNT; i++) {
        g_state_ctrl.devid[i] = MAX_DEV_CNT;
        g_state_ctrl.hotreset_devid[i] = MAX_DEV_CNT;
    }
}

int devdrv_ctrl_init(void)
{
    u32 i, j;

    if ((memset_s(g_ctrls, sizeof(g_ctrls), 0, sizeof(g_ctrls)) != 0) ||
        (memset_s(g_clients, sizeof(g_clients), 0, sizeof(g_clients)) != 0) ||
        (memset_s(g_clients_instance, sizeof(g_clients_instance), 0, sizeof(g_clients_instance)) != 0) ||
        (memset_s(g_p2p_attr_info, sizeof(g_p2p_attr_info), 0, sizeof(g_p2p_attr_info)) != 0)) {
        devdrv_err("memset_s failed\n");
        return -EINVAL;
    }

    for (i = 0; i < DEVDRV_CLIENT_TYPE_MAX; i++) {
        for (j = 0; j < MAX_DEV_CNT; j++) {
            mutex_init(&g_clients_instance[i][j].flag_mutex);
        }
    }
    mutex_init(&g_devdrv_ctrl_mutex);
    devdrv_dev_startup_ctrl_init();

    return 0;
}

/* cloud ai server, has 2 node(4p system), host side number 0-3 --- node 0, 4-7 --- node 1
   cloud evb, has 2 node(2p system), host side number 0-1 --- node 0, 2-3 --- node 1 */
int devdrv_get_cloud_server_devid(struct devdrv_shr_para __iomem *para)
{
    int dev_id = -1;

    if ((para->slot_id >= BOARD_CLOUD_MAX_DEV_NUM) || (para->slot_id < 0)) {
        devdrv_err("slot id %d is out of range\n", para->slot_id);
        return dev_id;
    }

    dev_id = para->slot_id;

    devdrv_info("device board type %d node_id %d dev id %d use host dev id %d.\n", para->board_type, para->node_id,
                para->chip_id, dev_id);

    if (g_ctrls[dev_id].priv != NULL) {
        devdrv_err("dev_id %d is already registered \n", dev_id);
        dev_id = -1;
    } else {
        g_ctrls[dev_id].startup_flg = DEVDRV_DEV_STARTUP_PROBED;
    }
    return dev_id;
}

void devdrv_set_devctrl_startup_flag(u32 dev_id, enum devdrv_dev_startup_flag_type flag)
{
    if (dev_id >= MAX_DEV_CNT) {
        devdrv_err("Invalid dev_id:%d\n", dev_id);
        return;
    }
    g_ctrls[dev_id].startup_flg = flag;
}

int devdrv_alloc_devid_check_ctrls(struct devdrv_ctrl *ctrl_this)
{
    int i;
    int dev_id = -1;

    for (i = 0; i < MAX_DEV_CNT; i++) {
        /* bus may be already inited probe at first time, hot reset happen. */
        if ((g_ctrls[i].bus != NULL) && (g_ctrls[i].bus == ctrl_this->bus) &&
            (g_ctrls[i].startup_flg == DEVDRV_DEV_STARTUP_UNPROBED)) {
            g_ctrls[i].startup_flg = DEVDRV_DEV_STARTUP_PROBED;
            dev_id = i;
            break;
        }
    }

    return dev_id;
}

int devdrv_alloc_devid_inturn(u32 beg, u32 stride)
{
    int dev_id = -1;
    int i;

    for (i = beg; i < MAX_DEV_CNT; i = i + stride) {
        if (g_ctrls[i].startup_flg == DEVDRV_DEV_STARTUP_UNPROBED) {
            g_ctrls[i].startup_flg = DEVDRV_DEV_STARTUP_PROBED;
            dev_id = i;
            break;
        }
    }
    return dev_id;
}

#ifdef CFG_SOC_PLATFORM_CLOUD
int devdrv_alloc_devid_cloud(struct devdrv_pci_ctrl *pci_ctrl)
{
    struct devdrv_shr_para __iomem *para = NULL;
    int dev_id = -1;
    int board_type;

    para = pci_ctrl->shr_para;
    board_type = para->board_type;

    if ((board_type == BOARD_CLOUD_AI_SERVER) || (board_type == BOARD_CLOUD_EVB)) {
        dev_id = devdrv_get_cloud_server_devid(para);
    } else {
        dev_id = devdrv_alloc_devid_inturn(0, 1);
    }

    return dev_id;
}
#endif

STATIC int devdrv_get_devid_by_funcid(u32 state, int func_id_check, int func_id_this, int i, int *dev_id)
{
    if ((func_id_check == 0) || (func_id_check == 1)) {
        devdrv_info("this func %d match func %d\n", func_id_this, func_id_check);
        if (state != DEVDRV_DEV_STARTUP_UNPROBED) {
            /* bus same ,has startup, function check */
            if (func_id_check == func_id_this) {
                /* bus same, has startup, func match */
                devdrv_err("All already is wrong.\n");
                return -EINVAL;
            } else {
                /* new func */
                *dev_id = (i / DEVDRV_MAX_FUNC_NUM) * DEVDRV_MAX_FUNC_NUM + func_id_this;
                devdrv_info("new func, another dev %d new dev %d\n", i, *dev_id);
                return 1;
            }
        } else {
            /* host reset */
            if (func_id_check == func_id_this) {
                *dev_id = i;
                devdrv_info("bus match but state change, then use dev %d\n", *dev_id);
                return 1;
            } else {
                /* new func, alloc dev id, but go on to find match id. */
                *dev_id = (i / DEVDRV_MAX_FUNC_NUM) * DEVDRV_MAX_FUNC_NUM + func_id_this;
                devdrv_info("new func, another dev %d new dev %d\n", i, *dev_id);
            }
        }
    } else {
        devdrv_err("wrong func %d \n", func_id_check);
        return -EINVAL;
    }
    return 0;
}

int devdrv_alloc_devid_mdc(struct devdrv_ctrl *ctrl_this)
{
    struct devdrv_pci_ctrl *pci_ctrl = (struct devdrv_pci_ctrl *)ctrl_this->priv;
    int dev_id_check = -1;
    int dev_id = -1;
    int func_id_this = pci_ctrl->func_id;
    int func_id_check = -1;
    u32 i;
    u32 state = 0;
    int ret;

    for (i = 0; i < MAX_DEV_CNT; i++) {
        if ((g_ctrls[i].bus != NULL) && (g_ctrls[i].bus == ctrl_this->bus)) {
            /* bus is same, means another function finish or host reset. */
            func_id_check = g_ctrls[i].func_id;
            state = g_ctrls[i].startup_flg;
            devdrv_info("bus match in %d func %d state %d\n", i, func_id_check, state);
            ret = devdrv_get_devid_by_funcid(state, func_id_check, func_id_this, i, &dev_id_check);
            if (ret < 0) {
                devdrv_err("get devid fail, func check %d, func this %d\n", func_id_check, func_id_this);
                return ret;
            } else if (ret > 0) {
                break;
            }
        }
    }

    if (dev_id_check == -1) {
        devdrv_debug("no match\n");
        /* no bus match, then alloc in turn by func */
        dev_id = devdrv_alloc_devid_inturn(func_id_this, 2);
    } else {
        dev_id = dev_id_check;
        g_ctrls[dev_id].startup_flg = DEVDRV_DEV_STARTUP_PROBED;
        devdrv_info("device id %d\n", dev_id);
    }
    return dev_id;
}

STATIC int devdrv_alloc_devid(struct devdrv_ctrl *ctrl_this)
{
    struct devdrv_pci_ctrl *pci_ctrl = NULL;
    int dev_id;

    pci_ctrl = (struct devdrv_pci_ctrl *)ctrl_this->priv;

    mutex_lock(&g_devdrv_ctrl_mutex);

#ifndef CFG_SOC_PLATFORM_MDC_V51
    dev_id = devdrv_alloc_devid_check_ctrls(ctrl_this);
    /* probe init at first time */
    if (dev_id == -1) {
#ifdef CFG_SOC_PLATFORM_CLOUD
        dev_id = devdrv_alloc_devid_cloud(pci_ctrl);
#else
        dev_id = devdrv_alloc_devid_inturn(0, 1);
#endif
    }
#else
    dev_id = devdrv_alloc_devid_mdc(ctrl_this);
#endif

    mutex_unlock(&g_devdrv_ctrl_mutex);

    return dev_id;
}

/* called after probed */
struct devdrv_ctrl *devdrv_get_top_half_devctrl_by_id(u32 dev_id)
{
    u32 i;

    for (i = 0; i < MAX_DEV_CNT; i++) {
        if (((g_ctrls[i].startup_flg == DEVDRV_DEV_STARTUP_TOP_HALF_OK) ||
             (g_ctrls[i].startup_flg == DEVDRV_DEV_STARTUP_BOTTOM_HALF_OK)) &&
            (g_ctrls[i].dev_id == dev_id)) {
            return &g_ctrls[i];
        }
    }

    return NULL;
}

/* called after half probed */
struct devdrv_ctrl *devdrv_get_bottom_half_devctrl_by_id(u32 dev_id)
{
    u32 i;

    for (i = 0; i < MAX_DEV_CNT; i++) {
        if ((g_ctrls[i].startup_flg == DEVDRV_DEV_STARTUP_BOTTOM_HALF_OK) && (g_ctrls[i].dev_id == dev_id)) {
            return &g_ctrls[i];
        }
    }

    return NULL;
}

struct devdrv_ctrl *devdrv_get_devctrl_by_id(u32 i)
{
    if (i < MAX_DEV_CNT) {
        return &g_ctrls[i];
    }

    return NULL;
}

/* device boot status */
int devdrv_get_device_boot_status(u32 devid, u32 *boot_status)
{
    struct devdrv_pci_ctrl *pci_ctrl = NULL;
    struct devdrv_ctrl *ctrl = NULL;

    if (devid >= MAX_DEV_CNT) {
        devdrv_err("dev id %d is invalid\n", devid);
        return -EINVAL;
    }
    if (boot_status == NULL) {
        devdrv_err("devid %d boot_status is null\n", devid);
        return -EINVAL;
    }

    ctrl = devdrv_get_top_half_devctrl_by_id(devid);
    if ((ctrl == NULL) || (ctrl->priv == NULL)) {
        devdrv_err("get devctrl by id err:%u!\n", devid);
        return -EINVAL;
    }

    pci_ctrl = ctrl->priv;

    *boot_status = pci_ctrl->device_boot_status;
    return 0;
}
EXPORT_SYMBOL(devdrv_get_device_boot_status);

/* record probed mini */
void drvdrv_dev_startup_record(u32 dev_id)
{
    u32 find_flag = 0;
    u32 i;

    mutex_lock(&g_devdrv_ctrl_mutex);
    /* not the first time probe(hot reset) */
    for (i = 0; i < g_state_ctrl.dev_num; i++) {
        if (dev_id == g_state_ctrl.devid[i]) {
            find_flag = 1;
            break;
        }
    }
    /* the first time probe */
    if (find_flag == 0) {
        g_state_ctrl.devid[g_state_ctrl.dev_num] = dev_id;
        if (g_state_ctrl.dev_num < MAX_DEV_CNT) {
            g_state_ctrl.dev_num++;
        }
        devdrv_info("probe new dev %u, add to report,dev_num:%u.\n", dev_id, g_state_ctrl.dev_num);
    } else {
        devdrv_info("dev %u no need record, just report. dev_num:%u\n", dev_id, g_state_ctrl.dev_num);
    }

    mutex_unlock(&g_devdrv_ctrl_mutex);
}

/*
report fomat(eg.8mini):
when first register,probe 5mini:
5,[0,1,2,3,4],5
then:
6,[5],1
7,[6],1
8,[7],1
after hot reset devid=2:
8,[2],1
after hot reset devid=4:
8,[4],1
*/
void drvdrv_dev_startup_report(u32 dev_id)
{
    mutex_lock(&g_devdrv_ctrl_mutex);
    if (g_state_ctrl.startup_callback != NULL) {
        /* report all probed mini when first time reset */
        if (g_state_ctrl.first_flag == 0) {
            if (g_state_ctrl.dev_num != 0) {
                (g_state_ctrl.startup_callback)(g_state_ctrl.dev_num, g_state_ctrl.devid, MAX_DEV_CNT,
                                                g_state_ctrl.dev_num);
            }
            devdrv_info("dev startup report num:%u\n", g_state_ctrl.dev_num);
            g_state_ctrl.first_flag = 1;
        } else {
            /* report one probed mini when hotreset */
            g_state_ctrl.hotreset_devid[0] = dev_id;
            (g_state_ctrl.startup_callback)(g_state_ctrl.dev_num, g_state_ctrl.hotreset_devid, MAX_DEV_CNT, 1);
            devdrv_info("dev startup report id:%u\n", dev_id);
        }
    } else {
        devdrv_info("dev startup no report id:%u\n", dev_id);
    }
    mutex_unlock(&g_devdrv_ctrl_mutex);

    return;
}

void drvdrv_dev_state_notifier(struct devdrv_pci_ctrl *pci_ctrl)
{
    u32 dev_id;

    if (pci_ctrl == NULL) {
        devdrv_info("pci_ctrl ptr is null\n");
        return;
    }

    /* when pcie ko removed, stop notify dev manager, and set callback NULL */
    if (pci_ctrl->module_exit_flag == DEVDRV_REMOVE_CALLED_BY_MODULE_EXIT) {
        devdrv_info("Do not notify dev manager driver remove\n");
        g_state_ctrl.state_notifier_callback = NULL;
        return;
    }

    dev_id = pci_ctrl->dev_id;
    if (dev_id >= MAX_DEV_CNT) {
        devdrv_info("dev_id is invalid, %u\n", dev_id);
        return;
    }

    if (g_state_ctrl.state_notifier_callback != NULL) {
        (g_state_ctrl.state_notifier_callback)(dev_id, (u32)GOING_TO_DISABLE_DEV);
        /* add 1s for bbox to dump when unbind */
        ssleep(1);
    } else {
        devdrv_info("state_notifier_callback is NULL, dev_id:%u\n", dev_id);
    }
}

/* for dev manager to register when insmod,then pcie report dev startup info to dev manager */
void drvdrv_dev_startup_register(devdrv_dev_startup_notify startup_callback)
{
    if (startup_callback == NULL) {
        devdrv_warn("startup callback is NULL\n");
        return;
    }
    devdrv_info("startup register, call startup report\n");
    g_state_ctrl.startup_callback = startup_callback;
    drvdrv_dev_startup_report(MAX_DEV_CNT);
}
EXPORT_SYMBOL(drvdrv_dev_startup_register);

/* for dev manager to register when insmod,then pcie report dev remove or other info to dev manager */
void drvdrv_dev_state_notifier_register(devdrv_dev_state_notify state_callback)
{
    if (state_callback == NULL) {
        devdrv_warn("state callback is null\n");
        return;
    }
    devdrv_info("state notifier register\n");
    g_state_ctrl.state_notifier_callback = state_callback;
}
EXPORT_SYMBOL(drvdrv_dev_state_notifier_register);

void devdrv_hb_broken_stop_msg_send(u32 devid)
{
    struct devdrv_ctrl *ctrl = NULL;
    struct devdrv_pci_ctrl *pci_ctrl = NULL;

    if (devid >= MAX_DEV_CNT) {
        devdrv_info("device id invalid: %d\n", devid);
        return;
    }

    ctrl = devdrv_get_bottom_half_devctrl_by_id(devid);
    if ((ctrl == NULL) || (ctrl->priv == NULL)) {
        devdrv_info("pci dev devid %d parameter is error\n", devid);
        return;
    }
    pci_ctrl = (struct devdrv_pci_ctrl *)ctrl->priv;

    devdrv_set_device_status(pci_ctrl, DEVDRV_DEVICE_DEAD);

    devdrv_info("heartbeat_broken stop msg end\n");
}
EXPORT_SYMBOL(devdrv_hb_broken_stop_msg_send);

void devdrv_set_device_boot_status(struct devdrv_pci_ctrl *pci_ctrl, u32 status)
{
    pci_ctrl->device_boot_status = status;
}

int devdrv_get_pcie_status_by_id(u32 dev_id)
{
    struct devdrv_ctrl *ctrl = NULL;

    ctrl = devdrv_get_bottom_half_devctrl_by_id(dev_id);
    if (ctrl == NULL) {
        devdrv_err("pcie status not startup ok, dev_id: %d\n", dev_id);
        return -EINVAL;
    }
    return 0;
}

int devdrv_pcie_reinit(u32 dev_id)
{
    struct pci_bus *bus = NULL;
    u32 ret;

    if (dev_id >= MAX_DEV_CNT) {
        devdrv_err("dev id %d is invalid\n", dev_id);
        return -EINVAL;
    }
    bus = g_ctrls[dev_id].bus;

    if (bus == NULL) {
        devdrv_err("devid %d bus does not exist\n", dev_id);
        return -EIO;
    }
    devdrv_info("call pcie_reinit, dev_id: %u\n", dev_id);

    ret = pci_rescan_bus(bus->self->bus);
    if (ret == 0) {
        devdrv_err("devid %d rescan bus falied ret = %d\n", dev_id, ret);
        return -EIO;
    }

    return 0;
}
EXPORT_SYMBOL(devdrv_pcie_reinit);

struct devdrv_dma_dev *devdrv_get_dma_dev(u32 dev_id)
{
    struct devdrv_ctrl *ctrl = devdrv_get_bottom_half_devctrl_by_id(dev_id);
    struct devdrv_pci_ctrl *pci_ctrl = NULL;

    if ((ctrl == NULL) || (ctrl->priv == NULL)) {
        devdrv_err("add_dev %d: access null pointer!\n", dev_id);
        return NULL;
    }
    pci_ctrl = ctrl->priv;

    return pci_ctrl->dma_dev;
}

u32 devdrv_get_devid_by_dev(struct devdrv_msg_dev *msg_dev)
{
    struct devdrv_pci_ctrl *pci_ctrl = NULL;
    u32 i;

    if (msg_dev == NULL) {
        devdrv_err("msg_dev is null\n");
        return MAX_DEV_CNT;
    }
    for (i = 0; i < MAX_DEV_CNT; i++) {
        pci_ctrl = (struct devdrv_pci_ctrl *)g_ctrls[i].priv;
        if (pci_ctrl == NULL) {
            continue;
        }
        if (pci_ctrl->msg_dev == msg_dev) {
            return g_ctrls[i].dev_id;
        }
    }

    devdrv_err("find devid failed!\n");

    return MAX_DEV_CNT;
}

/* Store the ctrl info, and return an id to register caller */
int devdrv_slave_dev_add(struct devdrv_ctrl *ctrl)
{
    int dev_id;

    dev_id = devdrv_alloc_devid(ctrl);
    if ((dev_id < 0) || (dev_id >= MAX_DEV_CNT)) {
        devdrv_err("dev link already full!dev_id:%d\n", dev_id);
        return -EINVAL;
    }

    g_ctrls[(u32)dev_id].dev_id = (u32)dev_id;
    g_ctrls[(u32)dev_id].priv = ctrl->priv;
    g_ctrls[(u32)dev_id].dev_type = ctrl->dev_type;
    g_ctrls[(u32)dev_id].dev = ctrl->dev;
    g_ctrls[(u32)dev_id].pdev = ctrl->pdev;
    g_ctrls[(u32)dev_id].bus = ctrl->bus;
    g_ctrls[(u32)dev_id].func_id = ctrl->func_id;

    return dev_id;
}
int devdrv_call_init_instance(u32 dev_id)
{
    struct devdrv_client_instance *instance = NULL;
    u32 ret;
    u32 i;

    for (i = 0; (i < DEVDRV_CLIENT_TYPE_MAX) && (dev_id < MAX_DEV_CNT); i++) {
        if (g_clients[i] == NULL) {
            continue;
        }

        instance = &g_clients_instance[i][dev_id];
        instance->dev_ctrl = &g_ctrls[dev_id];
        if (g_clients[i]->init_instance == NULL) {
            continue;
        }
        mutex_lock(&instance->flag_mutex);
        if (instance->flag == 0) {
            instance->flag = 1;
            mutex_unlock(&instance->flag_mutex);

            devdrv_debug("slave_dev_add,ctrl:%u,dev_id:%u,before init_instance!\n", i, dev_id);
            ret = g_clients[i]->init_instance(instance);
            devdrv_debug("slave_dev_add,after init_instance!\n");
            if (ret) {
                devdrv_info("client %u init fialed! ret = %d\n", i, ret);
            }
        } else {
            mutex_unlock(&instance->flag_mutex);
        }
    }

    return 0;
}

void devdrv_slave_dev_delete(u32 dev_id)
{
    struct devdrv_client_instance *instance = NULL;
    struct devdrv_ctrl *dev_ctrl = NULL;
    int ret;
    int dev_index = -1;
    u32 i;

    if (dev_id >= MAX_DEV_CNT) {
        devdrv_err("unregister dev: dev id %d not exist !\n", dev_id);
        return;
    }

    for (i = 0; i < MAX_DEV_CNT; i++)
        if (g_ctrls[i].dev_id == dev_id) {
            dev_ctrl = &g_ctrls[i];
            dev_index = i;
            break;
        }

    if (i == MAX_DEV_CNT) {
        devdrv_err(" find devctrl failed. dev_id %d\n", dev_id);
        return;
    }

    for (i = 0; i < DEVDRV_CLIENT_TYPE_MAX; i++) {
        if (g_clients[i] == NULL) {
            continue;
        }

        instance = &g_clients_instance[i][dev_index];
        if (g_clients[i]->uninit_instance) {
            mutex_lock(&instance->flag_mutex);
            instance->flag = 0;
            mutex_unlock(&instance->flag_mutex);
            devdrv_info("dev_id %u, client %u uninit start!\n", dev_id, i);
            ret = g_clients[i]->uninit_instance(instance);
            devdrv_info("dev_id %u, client %u uninit end!\n", dev_id, i);
            if (ret) {
                devdrv_info("dev_id %d,client %d uninit failed! ret = %d\n", dev_id, i, ret);
            }
            instance->dev_ctrl = NULL;
        }
    }

    if (dev_ctrl != NULL) {
        dev_ctrl->startup_flg = DEVDRV_DEV_STARTUP_UNPROBED;
        dev_ctrl->priv = NULL;
        dev_ctrl->ops = NULL;
        dev_ctrl->dev = NULL;
    }
}

void *devdrv_pcimsg_alloc_trans_queue(u32 dev_id, struct devdrv_trans_msg_chan_info *chan_info)
{
    struct devdrv_ctrl *ctrl = devdrv_get_bottom_half_devctrl_by_id(dev_id);
    if (ctrl == NULL) {
        devdrv_err("can't find dev %u \n", dev_id);
        return NULL;
    }

    if (chan_info == NULL) {
        devdrv_err("devid %d chan_info is null!\n", dev_id);
        return NULL;
    }

    return (void *)(ctrl->ops->alloc_trans_chan(ctrl->priv, chan_info));
}
EXPORT_SYMBOL(devdrv_pcimsg_alloc_trans_queue);

int devdrv_pcimsg_realease_trans_queue(void *msg_chan)
{
    if (msg_chan == NULL) {
        devdrv_err("msg_chan is null!\n");
        return -EINVAL;
    }

    return devdrv_free_trans_queue((struct devdrv_msg_chan *)msg_chan);
}
EXPORT_SYMBOL(devdrv_pcimsg_realease_trans_queue);

/* non-trans msg chan */
void *devdrv_pcimsg_alloc_non_trans_queue(u32 dev_id, struct devdrv_non_trans_msg_chan_info *chan_info)
{
    struct devdrv_ctrl *ctrl = devdrv_get_bottom_half_devctrl_by_id(dev_id);
    if (ctrl == NULL) {
        devdrv_err("can't find dev %u \n", dev_id);
        return NULL;
    }

    if (chan_info == NULL) {
        devdrv_err("devid %u chan_info is null!\n", dev_id);
        return NULL;
    }

    return (void *)(ctrl->ops->alloc_non_trans_chan(ctrl->priv, chan_info));
}
EXPORT_SYMBOL(devdrv_pcimsg_alloc_non_trans_queue);

int devdrv_pcimsg_free_non_trans_queue(void *msg_chan)
{
    if (msg_chan == NULL) {
        devdrv_err("msg_chan is NULL!\n");
        return -EINVAL;
    }

    return devdrv_free_non_trans_queue((struct devdrv_msg_chan *)msg_chan);
}
EXPORT_SYMBOL(devdrv_pcimsg_free_non_trans_queue);

int devdrv_register_black_callback(struct devdrv_black_callback *black_callback)
{
    if (black_callback == NULL) {
        devdrv_err("black_callback is null.\n");
        return -EINVAL;
    }

    if (black_callback->callback == NULL) {
        devdrv_err("callback is null.\n");
        return -EINVAL;
    }

    g_black_box.callback = black_callback->callback;

    return 0;
}
EXPORT_SYMBOL(devdrv_register_black_callback);

void devdrv_unregister_black_callback(struct devdrv_black_callback *black_callback)
{
    if (black_callback == NULL) {
        devdrv_err("black_callback is null.\n");
        return;
    }

    if (black_callback->callback != g_black_box.callback) {
        devdrv_err("callback is not same.\n");
        return;
    }

    g_black_box.callback = NULL;

    return;
}
EXPORT_SYMBOL(devdrv_unregister_black_callback);

int devdrv_register_client(struct devdrv_client *client)
{
    u32 ret;
    u32 dev_id;
    struct devdrv_client_instance *instance = NULL;

    if (client == NULL) {
        devdrv_err("client is null.\n");
        return -EINVAL;
    }

    if (client->type >= DEVDRV_CLIENT_TYPE_MAX) {
        devdrv_err("client type %d is error.\n", client->type);
        return -EINVAL;
    }

    if (g_clients[client->type] != NULL) {
        devdrv_err("client type %d is already registered.\n", client->type);
        return -EINVAL;
    }

    for (dev_id = 0; dev_id < MAX_DEV_CNT; dev_id++) {
        if (g_ctrls[dev_id].startup_flg != DEVDRV_DEV_STARTUP_BOTTOM_HALF_OK) {
            continue;
        }
        instance = &g_clients_instance[client->type][dev_id];
        instance->dev_ctrl = &g_ctrls[dev_id];
        if (client->init_instance == NULL) {
            continue;
        }
        devdrv_info("devdrv_register_client,client type:%d,dev_id:%d,before init_instance!\n", client->type, dev_id);
        mutex_lock(&instance->flag_mutex);
        if (instance->flag == 0) {
            instance->flag = 1;
            mutex_unlock(&instance->flag_mutex);
            ret = client->init_instance(instance);
            devdrv_info("devdrv_register_client %u,after init_instance!\n", dev_id);
            if (ret) {
                mutex_lock(&instance->flag_mutex);
                instance->flag = 0;
                mutex_unlock(&instance->flag_mutex);
                devdrv_err("init instance ctrl %u failed,delete client!\n", dev_id);
                return ret;
            }
        } else {
            mutex_unlock(&instance->flag_mutex);
        }
    }

    g_clients[client->type] = client;

    return 0;
}
EXPORT_SYMBOL(devdrv_register_client);

int devdrv_unregister_client(struct devdrv_client *client)
{
    struct devdrv_ctrl *dev_ctrl = NULL;
    struct devdrv_client_instance *instance = NULL;
    int dev_id;

    if (client == NULL) {
        devdrv_err("client is NULL!\n");
        return -EINVAL;
    }

    if (client->type >= DEVDRV_CLIENT_TYPE_MAX) {
        devdrv_err("client type %d is error.\n", client->type);
        return -EINVAL;
    }

    for (dev_id = 0; dev_id < MAX_DEV_CNT; dev_id++) {
        instance = &g_clients_instance[client->type][dev_id];
        dev_ctrl = instance->dev_ctrl;
        if (dev_ctrl != NULL) {
            if (client->uninit_instance == NULL)
                continue;
            mutex_lock(&instance->flag_mutex);
            instance->flag = 0;
            mutex_unlock(&instance->flag_mutex);
            devdrv_info("dev_id %d, client->type %u uninit_instance start!\n", dev_id, client->type);
            client->uninit_instance(instance);
            devdrv_info("dev_id %d, client->type %u uninit_instance end!\n", dev_id, client->type);
            instance->dev_ctrl = NULL;
            instance->priv = NULL;
        }
    }
    g_clients[client->type] = NULL;

    return 0;
}
EXPORT_SYMBOL(devdrv_unregister_client);

u64 devdrv_get_bar45_len(struct devdrv_pci_ctrl *pci_ctrl)
{
    u64 len;

    len = pci_resource_len(pci_ctrl->pdev, PCI_BAR_MEM);

    return len;
}

int devdrv_get_ts_sq_offset(struct devdrv_pci_ctrl *pci_ctrl, u64 *offset)
{
    int ret = 0;

#ifndef CFG_SOC_PLATFORM_CLOUD
    u64 len;

    len = devdrv_get_bar45_len(pci_ctrl);
    switch (len) {
        case MEM_BAR_LEN_64M:
            *offset = DEVDRV_BAR4_RESERVE_TS_SQ_OFFSET_64M;
            break;
        case MEM_BAR_LEN_128M:
            *offset = DEVDRV_BAR4_RESERVE_TS_SQ_OFFSET_128M;
            break;
        case MEM_BAR_LEN_4G:
            *offset = DEVDRV_BAR4_RESERVE_TS_SQ_OFFSET_4G;
            break;
        case MEM_BAR_LEN_8G:
            *offset = DEVDRV_BAR4_RESERVE_TS_SQ_OFFSET_8G;
            break;
        case MEM_BAR_LEN_16G:
            *offset = DEVDRV_BAR4_RESERVE_TS_SQ_OFFSET_16G;
            break;
        default:
            devdrv_err("devid %d BAR size is 0x%llx\n", pci_ctrl->dev_id, len);
            ret = -EINVAL;
            break;
    }
#else
    *offset = DEVDRV_RESERVE_MEM_TS_SQ_OFFSET;
#endif

    return ret;
}

int devdrv_get_test_addr_offset(struct devdrv_pci_ctrl *pci_ctrl, u64 *offset)
{
    int ret = 0;

#ifndef CFG_SOC_PLATFORM_CLOUD
    u64 len;
    len = devdrv_get_bar45_len(pci_ctrl);
    switch (len) {
        case MEM_BAR_LEN_64M:
            *offset = DEVDRV_BAR4_RESERVE_TEST_OFFSET_64M;
            break;
        case MEM_BAR_LEN_128M:
            *offset = DEVDRV_BAR4_RESERVE_TEST_OFFSET_128M;
            break;
        case MEM_BAR_LEN_4G:
            *offset = DEVDRV_BAR4_RESERVE_TEST_OFFSET_4G;
            break;
        case MEM_BAR_LEN_8G:
            *offset = DEVDRV_BAR4_RESERVE_TEST_OFFSET_8G;
            break;
        case MEM_BAR_LEN_16G:
            *offset = DEVDRV_BAR4_RESERVE_TEST_OFFSET_16G;
            break;
        default:
            devdrv_err("devid %u BAR size is 0x%llx\n", pci_ctrl->dev_id, len);
            ret = -EINVAL;
            break;
    }
#else
    *offset = DEVDRV_RESERVE_MEM_TEST_OFFSET;
#endif

    return ret;
}

STATIC int devdrv_get_addr_info_para_check(u32 index, u64 *addr, size_t *size)
{
    if (index || (addr == NULL) || (size == NULL)) {
        devdrv_err("parameter is error\n");
        return -EINVAL;
    }
    return 0;
}

STATIC void devdrv_get_ts_doorbell_addr_info(struct devdrv_pci_ctrl *pci_ctrl, u64 *addr, size_t *size)
{
    *addr = pci_ctrl->io_phy_base + DEVDRV_IO_TS_DB_OFFSET;
    *size = DEVDRV_IO_TS_DB_SIZE;
}

STATIC void devdrv_get_ts_sram_addr_info(struct devdrv_pci_ctrl *pci_ctrl, u64 *addr, size_t *size)
{
    *addr = pci_ctrl->io_phy_base + DEVDRV_IO_TS_SRAM_OFFSET;
    *size = DEVDRV_IO_TS_SRAM_SIZE;
}

STATIC int devdrv_get_ts_sq_addr_info(struct devdrv_pci_ctrl *pci_ctrl, u64 *addr, size_t *size)
{
    int ret;
    u64 offset;

    ret = devdrv_get_ts_sq_offset(pci_ctrl, &offset);
    if (ret != 0) {
        devdrv_err("devid %d devdrv_get_ts_sq_offset failed ret = %d\n", pci_ctrl->dev_id, ret);
        return -EINVAL;
    }
#ifndef CFG_SOC_PLATFORM_CLOUD
    *addr = pci_ctrl->mem_phy_base + offset;
    *size = DEVDRV_BAR4_RESERVE_TS_SQ_SIZE;
#else
    *addr = pci_ctrl->rsv_mem_phy_base + offset;
    *size = DEVDRV_RESERVE_MEM_TS_SQ_SIZE;
#endif
    return 0;
}

STATIC int devdrv_get_test_addr_info(struct devdrv_pci_ctrl *pci_ctrl, u64 *addr, size_t *size)
{
    int ret;
    u64 offset;

    ret = devdrv_get_test_addr_offset(pci_ctrl, &offset);
    if (ret != 0) {
        devdrv_err("devid %d devdrv_get_test_addr_offset failed ret = %d\n", pci_ctrl->dev_id, ret);
        return -EINVAL;
    }
#ifndef CFG_SOC_PLATFORM_CLOUD
    *addr = pci_ctrl->mem_phy_base + offset;
    *size = DEVDRV_BAR4_RESERVE_TEST_SIZE;
#else
    *addr = pci_ctrl->rsv_mem_phy_base + offset;
    *size = DEVDRV_RESERVE_MEM_TEST_SIZE;
#endif
    return 0;
}

STATIC void devdrv_get_load_sram_addr_info(struct devdrv_pci_ctrl *pci_ctrl, u64 *addr, size_t *size)
{
    *addr = pci_ctrl->io_phy_base + DEVDRV_IO_LOAD_SRAM_OFFSET;
    *size = DEVDRV_IO_LOAD_SRAM_SIZE;
}

#ifdef CFG_SOC_PLATFORM_CLOUD
STATIC void devdrv_get_hwts_addr_info(struct devdrv_pci_ctrl *pci_ctrl, u64 *addr, size_t *size)
{
    *addr = pci_ctrl->io_phy_base + DEVDRV_IO_HWTS_OFFSET;
    *size = DEVDRV_IO_HWTS_SIZE;
}

STATIC void devdrv_get_imu_log_addr_info(struct devdrv_pci_ctrl *pci_ctrl, u64 *addr, size_t *size)
{
    *addr = pci_ctrl->rsv_mem_phy_base + DEVDRV_ADDR_IMU_LOG_OFFSET;
    *size = DEVDRV_ADDR_IMU_LOG_SIZE;
}
#endif

int devdrv_get_addr_info(u32 devid, enum devdrv_addr_type type, u32 index, u64 *addr, size_t *size)
{
    struct devdrv_ctrl *ctrl = NULL;
    struct devdrv_pci_ctrl *pci_ctrl = NULL;
    int ret;

    ret = devdrv_get_addr_info_para_check(index, addr, size);
    if (ret) {
        devdrv_err("devid %d parameter is error ret = %d\n", devid, ret);
        return ret;
    }

    ctrl = devdrv_get_top_half_devctrl_by_id(devid);
    if ((ctrl == NULL) || (ctrl->priv == NULL)) {
        devdrv_err("devid parameter is error,devid:%d\n", devid);
        return -EINVAL;
    }
    pci_ctrl = (struct devdrv_pci_ctrl *)ctrl->priv;

    switch (type) {
        case DEVDRV_ADDR_TS_DOORBELL:
            devdrv_get_ts_doorbell_addr_info(pci_ctrl, addr, size);
            return 0;
        case DEVDRV_ADDR_TS_SRAM:
            devdrv_get_ts_sram_addr_info(pci_ctrl, addr, size);
            return 0;
        case DEVDRV_ADDR_TS_SQ_BASE:
            ret = devdrv_get_ts_sq_addr_info(pci_ctrl, addr, size);
            return ret;
        case DEVDRV_ADDR_TEST_BASE:
            ret = devdrv_get_test_addr_info(pci_ctrl, addr, size);
            return ret;
        case DEVDRV_ADDR_LOAD_RAM:
            devdrv_get_load_sram_addr_info(pci_ctrl, addr, size);
            return 0;
#ifdef CFG_SOC_PLATFORM_CLOUD
        case DEVDRV_ADDR_HWTS:
            devdrv_get_hwts_addr_info(pci_ctrl, addr, size);
            return 0;
        case DEVDRV_ADDR_IMU_LOG_BASE:
            devdrv_get_imu_log_addr_info(pci_ctrl, addr, size);
            return 0;
#endif
        default:
            devdrv_err("devid %d type parameter is error\n", devid);
            return -EINVAL;
    }
}
EXPORT_SYMBOL(devdrv_get_addr_info);

int devdrv_write_bar45(u32 devid, enum devdrv_addr_type type, u32 index, u64 offset, u64 val)
{
    void __iomem *addr_base = NULL;
    u64 addr = 0;
    size_t size = 0;
    int ret;

    ret = devdrv_get_addr_info(devid, type, index, &addr, &size);
    if (ret != 0) {
        devdrv_info("devid %d get bar addr fail, ret = %d\n", devid, ret);
        return -EINVAL;
    }
    if ((offset > (size - sizeof(u64))) || (offset % 0x4 != 0)) {
        devdrv_info("devid %d offset is more than size or is not 4 times\n", devid);
        return -EINVAL;
    }
    addr_base = ioremap_wc(addr, size);
    if (addr_base == NULL) {
        devdrv_info("devid %d ioremap_wc failed\n", devid);
        return -EINVAL;
    }

    writeq(val, (addr_base + offset));  //lint !e144

    iounmap(addr_base);
    addr_base = NULL;

    return 0;
}
EXPORT_SYMBOL(devdrv_write_bar45);

int devdrv_read_bar45(u32 devid, enum devdrv_addr_type type, u32 index, u64 offset, u64 *val)
{
    void __iomem *addr_base = NULL;
    u64 addr = 0;
    size_t size = 0;
    int ret;

    if (val == NULL) {
        devdrv_err("devid %d val ptr is NULL\n", devid);
        return -EINVAL;
    }
    ret = devdrv_get_addr_info(devid, type, index, &addr, &size);
    if (ret != 0) {
        devdrv_info("devid %d get bar addr fail ret = %d\n", devid, ret);
        return -EINVAL;
    }

    if ((offset > (size - sizeof(u64))) || (offset % 0x4 != 0)) {
        devdrv_info("devid %d offset is more than size or is not 4 times\n", devid);
        return -EINVAL;
    }

    addr_base = ioremap_wc(addr, size);
    if (addr_base == NULL) {
        devdrv_info("devid %d ioremap_wc failed\n", devid);
        return -EINVAL;
    }

    *val = readq(addr_base + offset);
    iounmap(addr_base);
    addr_base = NULL;

    return 0;
}
EXPORT_SYMBOL(devdrv_read_bar45);

int devdrv_get_irq_no(u32 devid, u32 index, unsigned int *irq)
{
    struct devdrv_ctrl *ctrl = NULL;
    struct devdrv_pci_ctrl *pci_ctrl = NULL;

    if (irq == NULL) {
        devdrv_info("devid %d null ptr\n", devid);
        return -EINVAL;
    }

    if (index > (DEVDRV_MSI_X_MAX_VECTORS - 1)) {
        devdrv_info("devid %d index parameter is more than %d\n", devid, (DEVDRV_MSI_X_MAX_VECTORS - 1));
        return -EINVAL;
    }
    ctrl = devdrv_get_bottom_half_devctrl_by_id(devid);
    if ((ctrl == NULL) || (ctrl->priv == NULL)) {
        devdrv_info("devid %d parameter is error\n", devid);
        return -EINVAL;
    }
    pci_ctrl = (struct devdrv_pci_ctrl *)ctrl->priv;
    *irq = pci_ctrl->msix_ctrl.entries[index].vector;

    return 0;
}
EXPORT_SYMBOL(devdrv_get_irq_no);

int devdrv_get_pci_dev_info(u32 devid, struct devdrv_pci_dev_info *dev_info)
{
    struct devdrv_ctrl *ctrl = NULL;
    struct devdrv_pci_ctrl *pci_ctrl = NULL;

    if (dev_info == NULL) {
        devdrv_info("devid %d null ptr\n", devid);
        return -EINVAL;
    }

    ctrl = devdrv_get_top_half_devctrl_by_id(devid);
    if ((ctrl == NULL) || (ctrl->priv == NULL)) {
        devdrv_info("pci dev devid %d parameter is error\n", devid);
        return -EINVAL;
    }
    pci_ctrl = (struct devdrv_pci_ctrl *)ctrl->priv;
    dev_info->bus_no = (u8)pci_ctrl->pdev->bus->number;
    dev_info->device_no = (u8)((pci_ctrl->pdev->devfn >> DEVDRV_DEVFN_BIT) & DEVDRV_DEVFN_DEV_VAL);
    dev_info->function_no = (u8)(pci_ctrl->pdev->devfn & DEVDRV_DEVFN_FN_VAL);

    return 0;
}
EXPORT_SYMBOL(devdrv_get_pci_dev_info);

int devdrv_get_pcie_id_info(u32 devid, struct devdrv_pcie_id_info *dev_info)
{
    struct devdrv_ctrl *ctrl = NULL;
    struct devdrv_pci_ctrl *pci_ctrl = NULL;

    if (dev_info == NULL) {
        devdrv_info("devid %d null ptr\n", devid);
        return -EINVAL;
    }

    ctrl = devdrv_get_top_half_devctrl_by_id(devid);
    if ((ctrl == NULL) || (ctrl->priv == NULL)) {
        devdrv_info("pcie devid %d parameter is error\n", devid);
        return -EINVAL;
    }
    pci_ctrl = (struct devdrv_pci_ctrl *)ctrl->priv;
    dev_info->venderid = pci_ctrl->pdev->vendor;
    dev_info->subvenderid = pci_ctrl->pdev->subsystem_vendor;
    dev_info->deviceid = pci_ctrl->pdev->device;
    dev_info->subdeviceid = pci_ctrl->pdev->subsystem_device;
    dev_info->bus = pci_ctrl->pdev->bus->number;
    dev_info->device = (pci_ctrl->pdev->devfn >> DEVDRV_DEVFN_BIT) & DEVDRV_DEVFN_DEV_VAL;
    dev_info->fn = (pci_ctrl->pdev->devfn) & DEVDRV_DEVFN_FN_VAL;

    return 0;
}
EXPORT_SYMBOL(devdrv_get_pcie_id_info);

int devdrv_get_atu_info(u32 devid, int atu_type, struct devdrv_iob_atu **atu, u64 *host_phy_base)
{
    struct devdrv_pci_ctrl *pci_ctrl = NULL;
    struct devdrv_ctrl *ctrl = NULL;

    if (devid >= MAX_DEV_CNT) {
        devdrv_err("dev %d is error.\n", devid);
        return -EINVAL;
    }

    ctrl = devdrv_get_bottom_half_devctrl_by_id(devid);
    if ((ctrl == NULL) || (ctrl->priv == NULL)) {
        devdrv_err("get devctrl by id failed, dev_id: %d\n", devid);
        return -EINVAL;
    }
    pci_ctrl = (struct devdrv_pci_ctrl *)ctrl->priv;

    switch (atu_type) {
        case ATU_TYPE_RX_MEM:
            *atu = pci_ctrl->mem_rx_atu;
            *host_phy_base = pci_ctrl->mem_phy_base;
            break;
#ifdef DRV_CLOUD
        case ATU_TYPE_RX_IO:
            *atu = pci_ctrl->io_rx_atu;
            *host_phy_base = pci_ctrl->io_phy_base;
            break;
#endif
        default:
            devdrv_err("dev %d atu_type %d not surport.\n", devid, atu_type);
            return -EINVAL;
    }

    return 0;
}

STATIC int devdrv_dma_link_prepare_para_check(u32 devid, struct devdrv_dma_node *dma_node, u32 node_cnt)
{
    if (devid >= MAX_DEV_CNT) {
        devdrv_err("invalid dev id %d, id overflow!\n", devid);
        return -EINVAL;
    }
    if (dma_node == NULL) {
        devdrv_err("devid %d dma_node is null\n", devid);
        return -EINVAL;
    }
    if (node_cnt == 0) {
        devdrv_err("devid %d sq_dma_addr is 0\n", devid);
        return -EINVAL;
    }
    return 0;
}

STATIC int devdrv_dma_link_alloc_sq_cq_addr(struct devdrv_dma_prepare *dma_prepare, struct device *dev, u32 node_cnt)
{
    u64 size;

    size = ((u64)node_cnt + DEVDRV_RESERVE_NUM) * sizeof(struct devdrv_dma_sq_node);
    dma_prepare->sq_size = size;
    dma_prepare->sq_base = dma_zalloc_coherent(dev, dma_prepare->sq_size, &dma_prepare->sq_dma_addr, GFP_KERNEL);
    if (dma_prepare->sq_base == NULL) {
        devdrv_err("devid %d dma sq alloc fail\n", dma_prepare->devid);
        return -ENOMEM;
    }

    size = ((u64)node_cnt + DEVDRV_RESERVE_NUM) * sizeof(struct devdrv_dma_cq_node);

    dma_prepare->cq_size = size;
    dma_prepare->cq_base = dma_zalloc_coherent(dev, dma_prepare->cq_size, &dma_prepare->cq_dma_addr, GFP_KERNEL);
    if (dma_prepare->cq_base == NULL) {
        devdrv_err("devid %d dma cq alloc fail\n", dma_prepare->devid);
        dma_free_coherent(dev, dma_prepare->sq_size, dma_prepare->sq_base, dma_prepare->sq_dma_addr);
        dma_prepare->sq_base = NULL;
        return -ENOMEM;
    }
    return 0;
}

STATIC void devdrv_dma_link_free_sq_cq_addr(struct devdrv_dma_prepare *dma_prepare, struct device *dev)
{
    if (dma_prepare->sq_base != NULL) {
        dma_free_coherent(dev, dma_prepare->sq_size, dma_prepare->sq_base, dma_prepare->sq_dma_addr);
        dma_prepare->sq_base = NULL;
    }
    if (dma_prepare->cq_base != NULL) {
        dma_free_coherent(dev, dma_prepare->cq_size, dma_prepare->cq_base, dma_prepare->cq_dma_addr);
        dma_prepare->cq_base = NULL;
    }
}

STATIC int devdrv_dma_link_prepare_fill_sq_node(struct devdrv_dma_prepare *dma_prepare,
    struct devdrv_dma_node *dma_node, u32 node_cnt)
{
    struct devdrv_dma_sq_node *current_sq_node = NULL;
    u32 i;

    for (i = 0; i < node_cnt; i++) {
        current_sq_node = (struct devdrv_dma_sq_node *)(dma_prepare->sq_base) + i;
        current_sq_node->src_addr_l = dma_node[i].src_addr & 0xFFFFFFFF;
        current_sq_node->src_addr_h = ((u64)dma_node[i].src_addr) >> DEVDRV_MOVE_BIT_32;
        current_sq_node->dst_addr_l = dma_node[i].dst_addr & 0xFFFFFFFF;
        current_sq_node->dst_addr_h = ((u64)dma_node[i].dst_addr) >> DEVDRV_MOVE_BIT_32;
        current_sq_node->length = dma_node[i].size;

        if (dma_node[i].direction == DEVDRV_DMA_DEVICE_TO_HOST) {
            current_sq_node->opcode = DEVDRV_DMA_WRITE;
        } else if (dma_node[i].direction == DEVDRV_DMA_HOST_TO_DEVICE) {
            current_sq_node->opcode = DEVDRV_DMA_READ;
        } else {
            devdrv_err("devid %d DMA direction para is invalid\n", dma_prepare->devid);
            return -EINVAL;
        }
        current_sq_node->pf = DEVDRV_PF_NUM;
        if (i == node_cnt - 1) {
            current_sq_node->ldie = 1;
        }
        /* enable RO */
        current_sq_node->attr = DEVDRV_DMA_SQ_ATTR_RO;
    }
    return 0;
}

/* async DMA link prepare */
struct devdrv_dma_prepare *devdrv_dma_link_prepare(u32 devid, enum devdrv_dma_data_type type,
                                                   struct devdrv_dma_node *dma_node, u32 node_cnt)
{
    struct devdrv_dma_prepare *dma_prepare = NULL;
    struct devdrv_ctrl *drv_ctrl = NULL;
    struct device *dev = NULL;
    int ret;

    ret = devdrv_dma_link_prepare_para_check(devid, dma_node, node_cnt);
    if (ret) {
        devdrv_err("devdrv_dma_link_prepare devid %d para check fail\n", devid);
        return NULL;
    }

    drv_ctrl = devdrv_get_bottom_half_devctrl_by_id(devid);
    if (drv_ctrl == NULL) {
        devdrv_err("devid %d drv_ctrl is null\n", devid);
        return NULL;
    }
    dev = drv_ctrl->dev;

    dma_prepare = kzalloc(sizeof(struct devdrv_dma_prepare), GFP_KERNEL);
    if (dma_prepare == NULL) {
        devdrv_err("devid %d dma_prepare alloc fail\n", devid);
        return NULL;
    }

    dma_prepare->devid = devid;

    ret = devdrv_dma_link_alloc_sq_cq_addr(dma_prepare, dev, node_cnt);
    if (ret) {
        devdrv_err("devid %d alloc sq cq addr failed\n", devid);
        goto free_prepare;
    }

    ret = devdrv_dma_link_prepare_fill_sq_node(dma_prepare, dma_node, node_cnt);
    if (ret) {
        devdrv_err("devid %d fill sq node failed\n", devid);
        goto free_sq_cq;
    }
    return dma_prepare;

free_sq_cq:
    devdrv_dma_link_free_sq_cq_addr(dma_prepare, dev);
free_prepare:
    kfree(dma_prepare);
    dma_prepare = NULL;
    return NULL;
}
EXPORT_SYMBOL(devdrv_dma_link_prepare);

/* async DMA link release */
int devdrv_dma_link_free(struct devdrv_dma_prepare *dma_prepare)
{
    struct devdrv_ctrl *drv_ctrl = NULL;
    struct device *dev = NULL;

    if (dma_prepare == NULL) {
        devdrv_err("dma_prepare is null\n");
        return -EINVAL;
    }

    drv_ctrl = devdrv_get_bottom_half_devctrl_by_id(dma_prepare->devid);
    if (drv_ctrl == NULL) {
        devdrv_err("drv_ctrl is null\n");
        return -EINVAL;
    }

    dev = drv_ctrl->dev;
    if (dma_prepare->cq_base != NULL) {
        dma_free_coherent(dev, dma_prepare->cq_size, dma_prepare->cq_base, dma_prepare->cq_dma_addr);
        dma_prepare->cq_base = NULL;
    }
    if (dma_prepare->sq_base != NULL) {
        dma_free_coherent(dev, dma_prepare->sq_size, dma_prepare->sq_base, dma_prepare->sq_dma_addr);
        dma_prepare->sq_base = NULL;
    }
    kfree(dma_prepare);
    dma_prepare = NULL;
    return 0;
}
EXPORT_SYMBOL(devdrv_dma_link_free);

static const struct devdrv_ops g_ops = {
    .alloc_trans_chan = devdrv_alloc_trans_queue,
    .realease_trans_chan = devdrv_free_trans_queue,
    .alloc_non_trans_chan = devdrv_alloc_non_trans_queue,
    .release_non_trans_chan = devdrv_free_non_trans_queue
};

int devdrv_register_pci_devctrl(struct devdrv_pci_ctrl *pci_ctrl)
{
    struct devdrv_ctrl pci_dev_ctrl;
    int dev_id;

    pci_dev_ctrl.priv = pci_ctrl;
    pci_dev_ctrl.dev_type = DEV_TYPE_PCI;
    pci_dev_ctrl.dev = &pci_ctrl->pdev->dev;
    pci_dev_ctrl.pdev = pci_ctrl->pdev;
    pci_dev_ctrl.bus = pci_ctrl->pdev->bus;
    pci_dev_ctrl.func_id = pci_ctrl->func_id;
    dev_id = devdrv_slave_dev_add(&pci_dev_ctrl);
    devdrv_info("dev_id:%d, bus:%pK\n", dev_id, pci_dev_ctrl.bus);

    if (dev_id < 0) {
        devdrv_err("pci device %d register fail\n", dev_id);
        return -ENOSPC;
    } else {
        pci_ctrl->dev_id = (u32)dev_id;
        return 0;
    }
}

void devdrv_register_half_devctrl(struct devdrv_pci_ctrl *pci_ctrl)
{
    u32 dev_id;

    dev_id = pci_ctrl->dev_id;
    g_ctrls[dev_id].ops = &g_ops;
    g_ctrls[dev_id].startup_flg = DEVDRV_DEV_STARTUP_BOTTOM_HALF_OK;
}

int devdrv_instance_callback_after_probe(u32 dev_id)
{
    int ret;

    mutex_lock(&g_devdrv_ctrl_mutex);
    ret = devdrv_call_init_instance(dev_id);
    if (ret != 0) {
        mutex_unlock(&g_devdrv_ctrl_mutex);
        devdrv_err("dev_id %d pci_probe register fail ret = %d\n", dev_id, ret);
        return ret;
    }
    mutex_unlock(&g_devdrv_ctrl_mutex);
    return ret;
}

int devdrv_dev_online(struct devdrv_pci_ctrl *pci_ctrl)
{
    struct devdrv_pci_ctrl *ctrl = NULL;
    int i;

    devdrv_info("dev_id %d online chip_type %d\n", pci_ctrl->dev_id, pci_ctrl->shr_para->chip_type);
    mutex_lock(&g_devdrv_ctrl_mutex);
    devdrv_debug("chip_type %d\n", pci_ctrl->shr_para->chip_type);

    if (pci_ctrl->shr_para->chip_type == HISI_MINI) {
        mutex_unlock(&g_devdrv_ctrl_mutex);
        return 0;
    }

    /* config tx atu & p2p msg base */
    for (i = 0; i < DEVDRV_P2P_SURPORT_MAX_DEVICE; i++) {
        if (g_ctrls[i].startup_flg != DEVDRV_DEV_STARTUP_BOTTOM_HALF_OK)
            continue;

        ctrl = (struct devdrv_pci_ctrl *)g_ctrls[i].priv;

        if (pci_ctrl->dev_id == ctrl->dev_id)
            continue;

        /* notice device */
        (void)devdrv_notify_dev_online(ctrl->msg_dev, pci_ctrl->dev_id, DEVDRV_DEV_ONLINE);
    }

    (void)devdrv_notify_dev_online(pci_ctrl->msg_dev, pci_ctrl->dev_id, DEVDRV_DEV_ONLINE);

    mutex_unlock(&g_devdrv_ctrl_mutex);
    return 0;
}

void devdrv_dev_offline(struct devdrv_pci_ctrl *pci_ctrl)
{
    struct devdrv_ctrl pci_dev_ctrl;
    struct devdrv_pci_ctrl *ctrl = NULL;
    int i;

    devdrv_info("dev_id %d offline chip_type %d\n", pci_ctrl->dev_id, pci_ctrl->shr_para->chip_type);

    mutex_lock(&g_devdrv_ctrl_mutex);

    /* register this pci device to devdrv_ctrls */
    pci_dev_ctrl.dev_id = pci_ctrl->dev_id;

    /* notice device offline */
    if (pci_ctrl->load_status_flag == DEVDRV_LOAD_HALF_PROBE_STATUS) {
        (void)devdrv_notify_dev_online(pci_ctrl->msg_dev, pci_ctrl->dev_id, DEVDRV_DEV_OFFLINE);
    }
    devdrv_info("dev_id %d offline notice finish\n", pci_ctrl->dev_id);

    if (pci_ctrl->shr_para->chip_type == HISI_MINI) {
        mutex_unlock(&g_devdrv_ctrl_mutex);
        return;
    }

    /* config tx atu & p2p msg base */
    for (i = 0; i < DEVDRV_P2P_SURPORT_MAX_DEVICE; i++) {
        if (g_ctrls[i].startup_flg != DEVDRV_DEV_STARTUP_BOTTOM_HALF_OK)
            continue;

        ctrl = (struct devdrv_pci_ctrl *)g_ctrls[i].priv;

        if (pci_ctrl->dev_id == ctrl->dev_id)
            continue;

        /* notice device */
        (void)devdrv_notify_dev_online(ctrl->msg_dev, pci_ctrl->dev_id, DEVDRV_DEV_OFFLINE);
    }

    mutex_unlock(&g_devdrv_ctrl_mutex);

    return;
}

int devdrv_iommu_map_addr(struct devdrv_pci_ctrl *pci_ctrl, u64 addr, u64 size, u64 *dma_addr)
{
#if LINUX_VERSION_CODE >= KERNEL_VERSION(4, 9, 0)
    struct iommu_domain *domain = NULL;
    dma_addr_t p_dma_addr;
    domain = iommu_get_domain_for_dev(&pci_ctrl->pdev->dev);
    if (domain != NULL) {
        p_dma_addr = dma_map_resource(&pci_ctrl->pdev->dev, addr, size, DMA_BIDIRECTIONAL, 0);
        if (dma_mapping_error(&pci_ctrl->pdev->dev, (u64)dma_addr)) {
            devdrv_err("devid %d dma_map_resource fail\n", pci_ctrl->dev_id);
            return -ENOMEM;
        }
        *dma_addr = p_dma_addr;
        devdrv_info("devid %d iommu_map success\n", pci_ctrl->dev_id);
    } else {
        *dma_addr = addr;
    }
#else
    *dma_addr = addr;
#endif

    return 0;
}

void devdrv_iommu_unmap_addr(struct devdrv_pci_ctrl *pci_ctrl, u64 dma_addr, u64 size)
{
#if LINUX_VERSION_CODE >= KERNEL_VERSION(4, 9, 0)
    struct iommu_domain *domain = NULL;

    domain = iommu_get_domain_for_dev(&pci_ctrl->pdev->dev);
    if ((domain != NULL) && (dma_addr != 0)) {
        dma_unmap_resource(&pci_ctrl->pdev->dev, dma_addr, size, DMA_BIDIRECTIONAL, 0);
        devdrv_info("devid %d iommu_unmap success\n", pci_ctrl->dev_id);
    }
#endif
    return;
}

int devdrv_add_p2p_msg_chan(struct devdrv_pci_ctrl *pci_ctrl_src, struct devdrv_pci_ctrl *pci_ctrl_dst)
{
    struct devdrv_msg_dev *msg_dev = pci_ctrl_src->msg_dev;
    struct devdrv_p2p_msg_chan_cfg_cmd cmd_data;
    int ret;
    u32 dst_dev = pci_ctrl_dst->dev_id;
    u64 dma_addr;
    u64 size = pci_ctrl_dst->rsv_mem_phy_size;

    ret = devdrv_iommu_map_addr(pci_ctrl_src, (u64)pci_ctrl_dst->rsv_mem_phy_base, size, &dma_addr);
    if (ret) {
        devdrv_err("dev %d, iommu map rsv mem failed\n", pci_ctrl_src->dev_id);
        return ret;
    }
    pci_ctrl_src->shr_para->p2p_msg_base_addr[dst_dev] = dma_addr + DEVDRV_RESERVE_MEM_MSG_OFFSET;
    pci_ctrl_src->shr_para->p2p_db_base_addr[dst_dev] = dma_addr + DEVDRV_RESERVE_MEM_DB_BASE;

    cmd_data.op = DEVDRV_OP_ADD;
    cmd_data.devid = dst_dev;

    ret = devdrv_admin_msg_chan_send(msg_dev, DEVDRV_CFG_P2P_MSG_CHAN, &cmd_data, sizeof(cmd_data), NULL, 0);
    if (ret) {
        devdrv_err("dev %d, p2p mem cfg fail. dst dev %d ret %d\n", pci_ctrl_src->dev_id, dst_dev, ret);
        devdrv_iommu_unmap_addr(pci_ctrl_src, dma_addr, size);
        return ret;
    }

    pci_ctrl_src->target_bar[dst_dev].rsv_mem_dma_addr = dma_addr;
    pci_ctrl_src->target_bar[dst_dev].rsv_mem_phy_size = size;

    return ret;
}

void devdrv_del_p2p_msg_chan(struct devdrv_pci_ctrl *pci_ctrl_src, u32 dst_dev)
{
    struct devdrv_msg_dev *msg_dev = pci_ctrl_src->msg_dev;
    struct devdrv_p2p_msg_chan_cfg_cmd cmd_data;
    int ret;
    u64 dma_addr;
    u64 size;

    cmd_data.op = DEVDRV_OP_DEL;
    cmd_data.devid = dst_dev;

    ret = devdrv_admin_msg_chan_send(msg_dev, DEVDRV_CFG_P2P_MSG_CHAN, &cmd_data, sizeof(cmd_data), NULL, 0);
    if (ret) {
        devdrv_err("dev %d, p2p mem cfg fail. dst dev %d ret %d\n", pci_ctrl_src->dev_id, dst_dev, ret);
    }

    size = pci_ctrl_src->target_bar[dst_dev].rsv_mem_phy_size;
    dma_addr = pci_ctrl_src->target_bar[dst_dev].rsv_mem_dma_addr;
    pci_ctrl_src->target_bar[dst_dev].rsv_mem_phy_size = 0;
    pci_ctrl_src->target_bar[dst_dev].rsv_mem_dma_addr = 0;

    devdrv_iommu_unmap_addr(pci_ctrl_src, dma_addr, size);
}

int devdrv_add_p2p_tx_atu(struct devdrv_pci_ctrl *pci_ctrl_src, struct devdrv_pci_ctrl *pci_ctrl_dst)
{
    struct devdrv_msg_dev *msg_dev = pci_ctrl_src->msg_dev;
    struct devdrv_tx_atu_cfg_cmd cmd_data;
    int ret;
    u32 dst_dev = pci_ctrl_dst->dev_id;
    u64 dma_addr;
    u64 size;

    /* mem bar */
    size = pci_ctrl_dst->mem_phy_size;
    ret = devdrv_iommu_map_addr(pci_ctrl_src, (u64)pci_ctrl_dst->mem_phy_base, size, &dma_addr);
    if (ret) {
        devdrv_err("dev %d, iommu map tx mem failed\n", pci_ctrl_src->dev_id);
        return ret;
    }

    cmd_data.op = DEVDRV_OP_ADD;
    cmd_data.devid = dst_dev;
    cmd_data.atu_type = ATU_TYPE_TX_MEM;
    cmd_data.phy_addr = (u64)pci_ctrl_dst->mem_phy_base;
    cmd_data.target_addr = dma_addr;
    cmd_data.target_size = size;
    ret = devdrv_admin_msg_chan_send(msg_dev, DEVDRV_CFG_TX_ATU, &cmd_data, sizeof(cmd_data), NULL, 0);
    if (ret) {
        devdrv_err("dev %d, p2p mem cfg fail. dst dev %d ret %d\n", pci_ctrl_src->dev_id, dst_dev, ret);
        goto MEM_MSG_SEND;
    }

    pci_ctrl_src->target_bar[dst_dev].mem_dma_addr = dma_addr;
    pci_ctrl_src->target_bar[dst_dev].mem_phy_size = size;

    /* io bar */
    size = pci_ctrl_dst->io_phy_size;
    ret = devdrv_iommu_map_addr(pci_ctrl_src, (u64)pci_ctrl_dst->io_phy_base, size, &dma_addr);
    if (ret) {
        devdrv_err("dev %d, iommu map tx io failed\n", pci_ctrl_src->dev_id);
        goto IO_IOMMU_FAIL;
    }

    cmd_data.atu_type = ATU_TYPE_TX_IO;
    cmd_data.target_addr = dma_addr;
    cmd_data.target_size = size;
    ret = devdrv_admin_msg_chan_send(msg_dev, DEVDRV_CFG_TX_ATU, &cmd_data, sizeof(cmd_data), NULL, 0);
    if (ret) {
        devdrv_err("dev %d, p2p io cfg fail. dst dev %d ret %d\n", pci_ctrl_src->dev_id, dst_dev, ret);
        goto IO_MSG_SEND;
    }

    pci_ctrl_src->target_bar[dst_dev].io_dma_addr = dma_addr;
    pci_ctrl_src->target_bar[dst_dev].io_phy_size = size;

    return 0;

IO_MSG_SEND:
    devdrv_iommu_unmap_addr(pci_ctrl_src, dma_addr, size);

IO_IOMMU_FAIL:
    cmd_data.op = DEVDRV_OP_DEL;
    (void)devdrv_admin_msg_chan_send(msg_dev, DEVDRV_CFG_TX_ATU, &cmd_data, sizeof(cmd_data), NULL, 0);

    dma_addr = pci_ctrl_src->target_bar[dst_dev].mem_dma_addr;
    size = pci_ctrl_src->target_bar[dst_dev].mem_phy_size;

MEM_MSG_SEND:
    devdrv_iommu_unmap_addr(pci_ctrl_src, dma_addr, size);

    return ret;
}

void devdrv_del_p2p_tx_atu(struct devdrv_pci_ctrl *pci_ctrl_src, u32 dst_dev)
{
    struct devdrv_msg_dev *msg_dev = pci_ctrl_src->msg_dev;
    struct devdrv_tx_atu_cfg_cmd cmd_data;
    int ret;
    u64 dma_addr;
    u64 size;

    /* mem bar */
    cmd_data.op = DEVDRV_OP_DEL;
    cmd_data.devid = dst_dev;
    cmd_data.atu_type = ATU_TYPE_TX_MEM;
    cmd_data.phy_addr = 0;
    cmd_data.target_addr = 0;
    cmd_data.target_size = 0;
    ret = devdrv_admin_msg_chan_send(msg_dev, DEVDRV_CFG_TX_ATU, &cmd_data, sizeof(cmd_data), NULL, 0);
    if (ret) {
        devdrv_err("dev %d, p2p mem cfg fail. dst dev %d ret %d\n", pci_ctrl_src->dev_id, dst_dev, ret);
    }

    dma_addr = pci_ctrl_src->target_bar[dst_dev].mem_dma_addr;
    size = pci_ctrl_src->target_bar[dst_dev].mem_phy_size;
    pci_ctrl_src->target_bar[dst_dev].mem_dma_addr = 0;
    pci_ctrl_src->target_bar[dst_dev].mem_phy_size = 0;

    devdrv_iommu_unmap_addr(pci_ctrl_src, dma_addr, size);

    /* io bar */
    cmd_data.atu_type = ATU_TYPE_TX_IO;

    ret = devdrv_admin_msg_chan_send(msg_dev, DEVDRV_CFG_TX_ATU, &cmd_data, sizeof(cmd_data), NULL, 0);
    if (ret) {
        devdrv_err("dev %d, p2p io cfg fail. dst dev %d ret %d\n", pci_ctrl_src->dev_id, dst_dev, ret);
    }

    dma_addr = pci_ctrl_src->target_bar[dst_dev].io_dma_addr;
    size = pci_ctrl_src->target_bar[dst_dev].io_phy_size;
    pci_ctrl_src->target_bar[dst_dev].io_dma_addr = 0;
    pci_ctrl_src->target_bar[dst_dev].io_phy_size = 0;

    devdrv_iommu_unmap_addr(pci_ctrl_src, dma_addr, size);
}

int devdrv_p2p_cfg_enable(u32 dev_id, u32 peer_dev_id)
{
    struct devdrv_pci_ctrl *pci_ctrl = g_ctrls[dev_id].priv;
    struct devdrv_pci_ctrl *pci_ctrl_peer = g_ctrls[peer_dev_id].priv;
    int ret;

    ret = devdrv_add_p2p_msg_chan(pci_ctrl, pci_ctrl_peer);
    if (ret) {
        return ret;
    }

    ret = devdrv_add_p2p_tx_atu(pci_ctrl, pci_ctrl_peer);
    if (ret) {
        devdrv_del_p2p_msg_chan(pci_ctrl, peer_dev_id);
        return ret;
    }

    ret = devdrv_add_p2p_msg_chan(pci_ctrl_peer, pci_ctrl);
    if (ret) {
        devdrv_del_p2p_tx_atu(pci_ctrl, peer_dev_id);
        devdrv_del_p2p_msg_chan(pci_ctrl, peer_dev_id);
        return ret;
    }

    ret = devdrv_add_p2p_tx_atu(pci_ctrl_peer, pci_ctrl);
    if (ret) {
        devdrv_del_p2p_msg_chan(pci_ctrl_peer, dev_id);
        devdrv_del_p2p_tx_atu(pci_ctrl, peer_dev_id);
        devdrv_del_p2p_msg_chan(pci_ctrl, peer_dev_id);
        return ret;
    }

    return ret;
}

void devdrv_p2p_cfg_disable(u32 dev_id, u32 peer_dev_id)
{
    struct devdrv_ctrl *ctrl = NULL;
    struct devdrv_pci_ctrl *pci_ctrl = NULL;
    struct devdrv_pci_ctrl *pci_ctrl_peer = NULL;

    /* Should support p2p cfg disable after a device hangs */
    ctrl = devdrv_get_bottom_half_devctrl_by_id(dev_id);
    if ((ctrl != NULL) && (ctrl->priv != NULL)) {
        pci_ctrl = ctrl->priv;
        devdrv_del_p2p_tx_atu(pci_ctrl, peer_dev_id);
        devdrv_del_p2p_msg_chan(pci_ctrl, peer_dev_id);
    }

    ctrl = devdrv_get_bottom_half_devctrl_by_id(peer_dev_id);
    if ((ctrl != NULL) && (ctrl->priv != NULL)) {
        pci_ctrl_peer = ctrl->priv;
        devdrv_del_p2p_tx_atu(pci_ctrl_peer, dev_id);
        devdrv_del_p2p_msg_chan(pci_ctrl_peer, dev_id);
    }
}

int devdrv_p2p_para_check(int pid, u32 dev_id, u32 peer_dev_id)
{
    u32 func_num;

    if ((dev_id >= DEVDRV_P2P_SURPORT_MAX_DEVICE) || (peer_dev_id >= DEVDRV_P2P_SURPORT_MAX_DEVICE)) {
        devdrv_err("pid %d dev %d peer_dev_id %d not surport p2p\n", pid, dev_id, peer_dev_id);
        return -EINVAL;
    }

    func_num = agentdrv_res_get_func_total();
    if (func_num == 0) {
        devdrv_err("func_num error:%u\n", func_num);
        return -EINVAL;
    }

#ifdef CFG_SOC_PLATFORM_MDC_V51
    if ((dev_id / func_num) == (peer_dev_id / func_num)) {
        devdrv_err("pid %d dev %d peer_dev_id %d is on the same chip\n", pid, dev_id, peer_dev_id);
        return -EINVAL;
    }
#endif

    if (dev_id == peer_dev_id) {
        devdrv_err("pid %d dev %d peer_dev_id %d is same\n", pid, dev_id, peer_dev_id);
        return -EINVAL;
    }

    return 0;
}

int devdrv_p2p_dev_valid_check(int pid, u32 dev_id, u32 peer_dev_id)
{
    struct devdrv_ctrl *ctrl = NULL;
    struct devdrv_pci_ctrl *pci_ctrl = NULL;

    ctrl = devdrv_get_bottom_half_devctrl_by_id(dev_id);
    if ((ctrl == NULL) || (ctrl->priv == NULL)) {
        devdrv_err("pid %d dev %d not startup peer_dev_id %d\n", pid, dev_id, peer_dev_id);
        return -ENXIO;
    }

    pci_ctrl = ctrl->priv;
    if (pci_ctrl->shr_para->chip_type == HISI_MINI) {
        return -EINVAL;
    }

    ctrl = devdrv_get_bottom_half_devctrl_by_id(peer_dev_id);
    if ((ctrl == NULL) || (ctrl->priv == NULL)) {
        devdrv_err("pid %d dev %d not startup peer_dev_id %d\n", pid, dev_id, peer_dev_id);
        return -ENXIO;
    }

    pci_ctrl = ctrl->priv;
    if (pci_ctrl->shr_para->chip_type == HISI_MINI) {
        return -EINVAL;
    }

    return 0;
}

struct devdrv_p2p_attr_info *devdrv_get_p2p_attr(u32 dev_id, u32 peer_dev_id)
{
    return &g_p2p_attr_info[dev_id][peer_dev_id];
}

int devdrv_get_p2p_attr_proc_id(struct devdrv_p2p_attr_info *p2p_attr, int pid)
{
    int index, i;
    index = -1;

    for (i = 0; i < DEVDRV_P2P_MAX_PROC_NUM; i++) {
        if ((p2p_attr->proc_ref[i] > 0) && (p2p_attr->pid[i] == pid)) {
            index = i;
            break;
        }
    }

    return index;
}

int devdrv_get_idle_p2p_attr_proc_id(struct devdrv_p2p_attr_info *p2p_attr)
{
    int index, i;
    index = -1;

    for (i = 0; i < DEVDRV_P2P_MAX_PROC_NUM; i++) {
        if (p2p_attr->proc_ref[i] == 0) {
            index = i;
            break;
        }
    }

    return index;
}

int devdrv_enable_p2p(int pid, u32 dev_id, u32 peer_dev_id)
{
    struct devdrv_p2p_attr_info *p2p_attr = NULL;
    struct devdrv_p2p_attr_info *p2p_peer_attr = NULL;
    int ret, index;

    ret = devdrv_p2p_para_check(pid, dev_id, peer_dev_id);
    if (ret != 0) {
        devdrv_err("pid %d dev %d peer_dev_id %d check failed\n", pid, dev_id, peer_dev_id);
        return ret;
    }

    ret = devdrv_p2p_dev_valid_check(pid, dev_id, peer_dev_id);
    if (ret != 0) {
        devdrv_err("pid %d dev %d peer_dev_id %d dev valid check failed\n", pid, dev_id, peer_dev_id);
        return ret;
    }

    p2p_attr = devdrv_get_p2p_attr(dev_id, peer_dev_id);

    mutex_lock(&g_devdrv_ctrl_mutex);

    index = devdrv_get_p2p_attr_proc_id(p2p_attr, pid);
    /* The process is configured for the first time */
    if (index < 0) {
        index = devdrv_get_idle_p2p_attr_proc_id(p2p_attr);
        if (index < 0) {
            devdrv_err("pid %d dev %d peer_dev_id %d used up\n", pid, dev_id, peer_dev_id);
            ret = -ENOMEM;
            goto OUT;
        }
        p2p_peer_attr = devdrv_get_p2p_attr(peer_dev_id, dev_id);
        /* First configuration, and peer allready configuration */
        if ((p2p_attr->ref == 0) && (p2p_peer_attr->ref > 0)) {
            /* Both directions are configured to take effect */
            ret = devdrv_p2p_cfg_enable(dev_id, peer_dev_id);
            if (ret != 0) {
                devdrv_err("pid %d dev %d peer_dev_id %d cfg failed\n", pid, dev_id, peer_dev_id);
                goto OUT;
            }

            devdrv_event("pid %d dev %d peer_dev_id %d enable p2p success\n", pid, dev_id, peer_dev_id);
        }
        p2p_attr->pid[index] = pid;
    }

    p2p_attr->proc_ref[index]++;
    p2p_attr->ref++;

    devdrv_info("dev %d peer_dev_id %d pid %d ref %d total ref %d\n",
        dev_id, peer_dev_id, pid, p2p_attr->proc_ref[index], p2p_attr->ref);

OUT:
    mutex_unlock(&g_devdrv_ctrl_mutex);

    return ret;
}
EXPORT_SYMBOL(devdrv_enable_p2p);

int devdrv_disable_p2p(int pid, u32 dev_id, u32 peer_dev_id)
{
    struct devdrv_p2p_attr_info *p2p_attr = NULL;
    struct devdrv_p2p_attr_info *p2p_peer_attr = NULL;
    int ret, index;

    ret = devdrv_p2p_para_check(pid, dev_id, peer_dev_id);
    if (ret != 0) {
        devdrv_err("pid %d dev %d peer_dev_id %d check failed\n", pid, dev_id, peer_dev_id);
        return ret;
    }

    p2p_attr = devdrv_get_p2p_attr(dev_id, peer_dev_id);

    mutex_lock(&g_devdrv_ctrl_mutex);

    index = devdrv_get_p2p_attr_proc_id(p2p_attr, pid);
    /* The process has not been configured with p2p */
    if (index < 0) {
        ret = -ESRCH;
        goto OUT;
    }

    if ((p2p_attr->proc_ref[index] <= 0) || (p2p_attr->ref <= 0)) {
        devdrv_err("pid %d dev %d peer_dev_id %d ref error, proc ref %d total ref %d\n",
            pid, dev_id, peer_dev_id, p2p_attr->proc_ref[index], p2p_attr->ref);
        ret = -ESRCH;
        goto OUT;
    }

    p2p_attr->proc_ref[index]--;
    p2p_attr->ref--;

    if (p2p_attr->proc_ref[index] == 0) {
        p2p_attr->pid[index] = 0;
    }

    /* All configurations are canceled, delete tx atu */
    if (p2p_attr->ref == 0) {
        /* peer not yet cancel configuration */
        p2p_peer_attr = devdrv_get_p2p_attr(peer_dev_id, dev_id);
        if (p2p_peer_attr->ref > 0) {
            /* Cancel failed, can't do anything anymore */
            devdrv_p2p_cfg_disable(dev_id, peer_dev_id);

            devdrv_event("pid %d dev %d peer_dev_id %d disable p2p success\n", pid, dev_id, peer_dev_id);
        }
    }

    devdrv_info("dev %d peer_dev_id %d pid %d ref %d total ref %d\n",
        dev_id, peer_dev_id, pid, p2p_attr->proc_ref[index], p2p_attr->ref);

OUT:
    mutex_unlock(&g_devdrv_ctrl_mutex);

    return ret;
}
EXPORT_SYMBOL(devdrv_disable_p2p);

bool devdrv_is_p2p_enabled(u32 dev_id, u32 peer_dev_id)
{
    struct devdrv_p2p_attr_info *p2p_attr = NULL;
    struct devdrv_p2p_attr_info *p2p_peer_attr = NULL;
    int ret;

    ret = devdrv_p2p_para_check(0, dev_id, peer_dev_id);
    if (ret != 0) {
        devdrv_err("dev %d peer_dev_id %d check failed\n", dev_id, peer_dev_id);
        return false;
    }

    p2p_attr = devdrv_get_p2p_attr(dev_id, peer_dev_id);
    p2p_peer_attr = devdrv_get_p2p_attr(peer_dev_id, dev_id);

    return ((p2p_attr->ref > 0) && (p2p_peer_attr->ref > 0));
}
EXPORT_SYMBOL(devdrv_is_p2p_enabled);

/* Restore the configuration of the process when the process exits */
void devdrv_flush_p2p(int pid)
{
    struct devdrv_p2p_attr_info *p2p_attr = NULL;
    int i, j, num, index;

    for (i = 0; i < DEVDRV_P2P_SURPORT_MAX_DEVICE; i++) {
        for (j = 0; j < DEVDRV_P2P_SURPORT_MAX_DEVICE; j++) {
            if (i == j) {
                continue;
            }

            /* get pid attr */
            p2p_attr = devdrv_get_p2p_attr(i, j);
            index = devdrv_get_p2p_attr_proc_id(p2p_attr, pid);
            if (index < 0) {
                continue;
            }
            /* pid has config p2p */
            num = 0;
            while (devdrv_disable_p2p(pid, i, j) == 0) {
                num++;
            }

            if (num > 0) {
                devdrv_event("dev %d peer_dev_id %d pid %d recyle %d ref\n", i, j, pid, num);
            }
        }
    }
}
EXPORT_SYMBOL(devdrv_flush_p2p);

int devdrv_devmem_addr_bar_to_dma(u32 devid, u32 dst_devid, phys_addr_t host_bar_addr, dma_addr_t *dma_addr)
{
    int ret;
    struct devdrv_pci_ctrl *pci_ctrl = NULL;
    struct devdrv_pci_ctrl *pci_ctrl_dst = NULL;

    if (dma_addr == NULL) {
        devdrv_err("dev %d peer_dev_id %d dma_addr is null\n", devid, dst_devid);
        return -EINVAL;
    }

    ret = devdrv_p2p_para_check(0, devid, dst_devid);
    if (ret != 0) {
        devdrv_err("dev %d peer_dev_id %d check failed\n", devid, dst_devid);
        return ret;
    }

    ret = devdrv_p2p_dev_valid_check(0, devid, dst_devid);
    if (ret != 0) {
        devdrv_err("dev %d peer_dev_id %d dev valid check failed\n", devid, dst_devid);
        return ret;
    }

    pci_ctrl = g_ctrls[devid].priv;
    pci_ctrl_dst = g_ctrls[dst_devid].priv;
    if ((pci_ctrl_dst->mem_phy_base <= host_bar_addr) &&
        ((pci_ctrl_dst->mem_phy_base + pci_ctrl_dst->mem_phy_size) > host_bar_addr)) {
        /* iommu has maped */
        if (pci_ctrl->target_bar[dst_devid].mem_phy_size == pci_ctrl_dst->mem_phy_size) {
            *dma_addr = host_bar_addr - pci_ctrl_dst->mem_phy_base + pci_ctrl->target_bar[dst_devid].mem_dma_addr;
            return 0;
        }
    }

    return -EINVAL;
}
EXPORT_SYMBOL(devdrv_devmem_addr_bar_to_dma);

int devdrv_pcie_prereset(u32 dev_id)
{
    struct devdrv_pci_ctrl *pci_ctrl = NULL;
    struct devdrv_ctrl *dev_ctrl = NULL;

    if (dev_id >= MAX_DEV_CNT) {
        devdrv_err(" dev id %u not exist !\n", dev_id);
        return -EINVAL;
    }

    dev_ctrl = devdrv_get_bottom_half_devctrl_by_id(dev_id);
    if ((dev_ctrl == NULL) || (dev_ctrl->priv == NULL)) {
        devdrv_err("get devctrl by id failed, dev_id: %u\n", dev_id);
        return -EINVAL;
    }
    devdrv_info("call pcie_prereset, dev_id: %u\n", dev_id);
    pci_ctrl = (struct devdrv_pci_ctrl *)dev_ctrl->priv;
    pci_stop_and_remove_bus_device(pci_ctrl->pdev);

    return 0;
}
EXPORT_SYMBOL(devdrv_pcie_prereset);

int devdrv_get_host_phy_mach_flag(u32 devid, u32 *host_flag)
{
    struct devdrv_pci_ctrl *pci_ctrl = NULL;
    struct devdrv_ctrl *ctrl = NULL;

    if (devid >= MAX_DEV_CNT) {
        devdrv_err("dev id %d is invalid\n", devid);
        return -EINVAL;
    }
    if (host_flag == NULL) {
        devdrv_err("devid %d host_flag is null\n", devid);
        return -EINVAL;
    }
    ctrl = devdrv_get_bottom_half_devctrl_by_id(devid);
    if ((ctrl == NULL) || (ctrl->priv == NULL)) {
        devdrv_err("get devctrl by id err:%d!\n", devid);
        return -EINVAL;
    }

    pci_ctrl = ctrl->priv;

    *host_flag = readl(pci_ctrl->mem_base + DEVDRV_HOST_PHY_MACH_FLAG_OFFSET);

    return 0;
}
EXPORT_SYMBOL(devdrv_get_host_phy_mach_flag);

/* set host flag */
void devdrv_set_host_phy_mach_flag(struct devdrv_pci_ctrl *pci_ctrl, u32 host_flag)
{
    writel(host_flag, (pci_ctrl->mem_base + DEVDRV_HOST_PHY_MACH_FLAG_OFFSET));
}

/* one device only surport a txatu item, repeat configuration will overwrite previous
   devid: host devid, size <= 128MB */
int devdrv_device_txatu_config(u32 devid, dma_addr_t host_dma_addr, u32 size)
{
    struct devdrv_ctrl *ctrl = devdrv_get_bottom_half_devctrl_by_id(devid);
    struct devdrv_pci_ctrl *pci_ctrl = NULL;
    struct devdrv_msg_dev *msg_dev = NULL;
    struct devdrv_tx_atu_cfg_cmd cmd_data;
    int ret;

    if ((ctrl == NULL) || (ctrl->priv == NULL)) {
        devdrv_err("devid %d get ctrl failed!\n", devid);
        return -EINVAL;
    }

    pci_ctrl = (struct devdrv_pci_ctrl *)ctrl->priv;

    if (pci_ctrl->shr_para->chip_type == HISI_MINI) {
        return 0;
    }

    if ((size == 0) || ((size % ATU_SIZE_ALIGN) != 0)) {
        devdrv_err("devid %d size 0x%x is illegal\n", devid, size);
        return -EINVAL;
    }

    msg_dev = pci_ctrl->msg_dev;
    if (pci_ctrl->host_mem_phy_size != 0) {
        pci_ctrl->host_mem_phy_size = 0;
    }
    cmd_data.op = DEVDRV_OP_ADD;
    cmd_data.devid = (u32)-1;
    cmd_data.atu_type = ATU_TYPE_TX_HOST;
    cmd_data.phy_addr = (u64)host_dma_addr;
    cmd_data.target_addr = (u64)host_dma_addr;
    cmd_data.target_size = size;

    ret = devdrv_admin_msg_chan_send(msg_dev, DEVDRV_CFG_TX_ATU, &cmd_data, sizeof(cmd_data), NULL, 0);
    if (ret) {
        devdrv_err("dev %d, p2p host mem cfg fail ret %d.\n", devid, ret);
        return ret;
    }

    pci_ctrl->host_mem_dma_addr = host_dma_addr;
    pci_ctrl->host_mem_phy_size = size;

    return ret;
}
EXPORT_SYMBOL(devdrv_device_txatu_config);
/* This function called by host to get device index with host device id
 * on cloud,  an smp system has 4 chips, so the device index is 0 ~ 3
 * on mini, device index is 0
 */
int devdrv_get_device_index(u32 host_dev_id)
{
#ifdef CFG_SOC_PLATFORM_CLOUD
    struct devdrv_shr_para __iomem *para = NULL;
    struct devdrv_pci_ctrl *pci_ctrl = NULL;
    struct devdrv_ctrl *ctrl = NULL;
    int board_type;
#endif
    int index;

#ifndef CFG_SOC_PLATFORM_CLOUD
    index = 0;
#else
    ctrl = devdrv_get_bottom_half_devctrl_by_id(host_dev_id);
    if ((ctrl == NULL) || (ctrl->priv == NULL)) {
        devdrv_err("get struct devdrv_ctrl error : access null pointer! host_dev_id = %d\n", host_dev_id);
        return -EINVAL;
    }

    pci_ctrl = ctrl->priv;
    para = pci_ctrl->shr_para;
    board_type = para->board_type;
    if ((board_type == BOARD_CLOUD_AI_SERVER) || (board_type == BOARD_CLOUD_EVB)) {
        index = para->chip_id;
    } else {
        index = 0;
    }
#endif
    return index;
}
EXPORT_SYMBOL(devdrv_get_device_index);

void devdrv_set_device_boot_finish(struct devdrv_pci_ctrl *pci_ctrl)
{
    struct devdrv_startup_status *startup_status = &(pci_ctrl->startup_status);

    if ((startup_status->status == DEVDRV_STARTUP_STATUS_FINISH) &&
        ((startup_status->module_bit_map & DEVDRV_HOST_MODULE_MASK) == DEVDRV_HOST_MODULE_MASK)) {
        /* set device boot status:boot finish */
        devdrv_set_device_boot_status(pci_ctrl, DSMI_BOOT_STATUS_FINISH);
        devdrv_info("dev id %d boot finish\r\n", pci_ctrl->dev_id);
    }
}

void devdrv_set_startup_status(struct devdrv_pci_ctrl *pci_ctrl, int status)
{
    struct devdrv_startup_status *startup_status = &(pci_ctrl->startup_status);

    if (status == DEVDRV_STARTUP_STATUS_INIT) {
        startup_status->module_bit_map = 0;
        devdrv_info("dev id %d startup status init jiffies %ld \r\n", pci_ctrl->dev_id, jiffies);
    } else {
        devdrv_info("dev id %d pre status %d, current %d, use time %d(ms) \r\n", pci_ctrl->dev_id,
                    startup_status->status, status, jiffies_to_msecs(jiffies - startup_status->timestamp));
    }

    startup_status->status = status;
    startup_status->timestamp = jiffies;

    devdrv_set_device_boot_finish(pci_ctrl);
}

int devdrv_set_module_init_finish(int dev_id, int module)
{
    struct devdrv_pci_ctrl *pci_ctrl = NULL;
    struct devdrv_ctrl *ctrl = NULL;
    struct devdrv_startup_status *startup_status = NULL;

    ctrl = devdrv_get_bottom_half_devctrl_by_id(dev_id);
    if ((ctrl == NULL) || (ctrl->priv == NULL)) {
        devdrv_err("get ctrl error dev_id = %d \r\n", dev_id);
        return -EINVAL;
    }

    pci_ctrl = ctrl->priv;
    startup_status = &(pci_ctrl->startup_status);

    mutex_lock(&g_devdrv_ctrl_mutex);

    startup_status->module_bit_map |= (0x1UL << (u32)module);

    devdrv_set_device_boot_finish(pci_ctrl);

    mutex_unlock(&g_devdrv_ctrl_mutex);

    devdrv_info("dev_id %d module %d init finish\r\n", dev_id, module);

    return 0;
}
EXPORT_SYMBOL(devdrv_set_module_init_finish);

bool devdrv_can_hotreset(struct devdrv_startup_status *startup_status)
{
    bool ret = false;

    mutex_lock(&g_devdrv_ctrl_mutex);

    if (startup_status->status == DEVDRV_STARTUP_STATUS_FINISH) {
        if ((startup_status->module_bit_map & DEVDRV_HOST_MODULE_MASK) == DEVDRV_HOST_MODULE_MASK) {
            /* all module init finish */
            ret = true;
        } else if (jiffies_to_msecs(jiffies - startup_status->timestamp) > DEVDRV_MODULE_FINISH_TIMEOUT) {
            /* module init timeout */
            ret = true;
        }
    } else if (startup_status->status == DEVDRV_STARTUP_STATUS_TIMEOUT) {
        /* load timeout */
        ret = true;
    } else {
        if (jiffies_to_msecs(jiffies - startup_status->timestamp) > DEVDRV_MODULE_INIT_TIMEOUT) {
            /* slave chip no load */
            ret = true;
        }
    }

    mutex_unlock(&g_devdrv_ctrl_mutex);

    devdrv_info("status %d, module_bit_map 0x%x, timestamp %ld cur %ld ret %d \r\n", startup_status->status,
                startup_status->module_bit_map, startup_status->timestamp, jiffies, ret);

    return ret;
}

struct pci_dev *devdrv_get_device_pf(struct pci_dev *pdev, unsigned int pf_num)
{
    unsigned int devfn;
    int domain_nr;
    struct pci_dev *dev = NULL;

    devfn = PCI_DEVFN(PCI_SLOT(pdev->devfn), pf_num);
    domain_nr = pci_domain_nr(pdev->bus);
    dev = pci_get_domain_bus_and_slot(domain_nr, pdev->bus->number, devfn);

    return dev;
}

void devdrv_device_unbind_driver(struct pci_dev *pdev_b)
{
    struct pci_dev *pdev = NULL;

#ifdef CFG_SOC_PLATFORM_CLOUD
    /* network pf0 */
    pdev = devdrv_get_device_pf(pdev_b, NETWORK_PF_0);
    if (pdev != NULL) {
        devdrv_info("pcie hotreset pci dev(%02x:%02x.%d), unbind driver\n", pdev->bus->number, PCI_SLOT(pdev->devfn),
                    PCI_FUNC(pdev->devfn));
        device_release_driver(&pdev->dev);
    }

    /* network pf1 */
    pdev = devdrv_get_device_pf(pdev_b, NETWORK_PF_1);
    if (pdev != NULL) {
        devdrv_info("pcie hotreset pci dev(%02x:%02x.%d), unbind driver\n", pdev->bus->number, PCI_SLOT(pdev->devfn),
                    PCI_FUNC(pdev->devfn));
        device_release_driver(&pdev->dev);
    }
#endif
    /* davinci pf */
    pdev = pdev_b;
    devdrv_info("pcie hotreset pci dev(%02x:%02x.%d), unbind driver\n", pdev->bus->number, PCI_SLOT(pdev->devfn),
                PCI_FUNC(pdev->devfn));
    device_release_driver(&pdev->dev);
}

void devdrv_device_bus_rescan(struct pci_dev *pdev)
{
    int num;

    num = pci_rescan_bus(pdev->bus);
    devdrv_err("pcie hotreset pci bus dev(%02x:%02x.%d), rescan device device num %d\n", pdev->bus->number,
               PCI_SLOT(pdev->devfn), PCI_FUNC(pdev->devfn), num);
}

void devdrv_device_bus_remove(struct pci_dev *pdev_r)
{
    struct pci_dev *pdev = NULL;

#ifdef CFG_SOC_PLATFORM_CLOUD

    /* network pf0 */
    pdev = devdrv_get_device_pf(pdev_r, NETWORK_PF_0);
    if (pdev != NULL) {
        devdrv_info("pcie hotreset pci dev(%02x:%02x.%d), bus remove\n", pdev->bus->number, PCI_SLOT(pdev->devfn),
                    PCI_FUNC(pdev->devfn));
        pci_stop_and_remove_bus_device(pdev);
    }

    /* network pf1 */
    pdev = devdrv_get_device_pf(pdev_r, NETWORK_PF_1);
    if (pdev != NULL) {
        devdrv_info("pcie hotreset pci dev(%02x:%02x.%d), bus remove\n", pdev->bus->number, PCI_SLOT(pdev->devfn),
                    PCI_FUNC(pdev->devfn));
        pci_stop_and_remove_bus_device(pdev);
    }
#endif
    /* davinci pf */
    pdev = pdev_r;
    devdrv_info("pcie hotreset pci dev(%02x:%02x.%d), bus remove\n", pdev->bus->number, PCI_SLOT(pdev->devfn),
                PCI_FUNC(pdev->devfn));

    pci_stop_and_remove_bus_device(pdev);
}

void devdrv_device_bridge_bus_reset(struct pci_bus *bus)
{
    struct pci_dev *pdev = bus->self;
#if LINUX_VERSION_CODE >= KERNEL_VERSION(4, 19, 0)
    int ret;
#endif

    devdrv_info("pcie hotreset parent pci dev(%02x:%02x.%d), reset bridge secondary bus\n", pdev->bus->number,
                PCI_SLOT(pdev->devfn), PCI_FUNC(pdev->devfn));

#if LINUX_VERSION_CODE >= KERNEL_VERSION(4, 19, 0)
    ret = pci_bridge_secondary_bus_reset(pdev);
    devdrv_info("pcie hotreset parent pci dev(%02x:%02x.%d), ret %d\n", pdev->bus->number, PCI_SLOT(pdev->devfn),
                PCI_FUNC(pdev->devfn), ret);
#else
    pci_reset_bridge_secondary_bus(pdev);
#endif
}

void devdrv_hotreset_set_timestamp(struct devdrv_ctrl *dev_ctrl)
{
    dev_ctrl->timestamp = (unsigned long)jiffies;
}

unsigned long devdrv_hotreset_get_timetamp(struct devdrv_ctrl *dev_ctrl)
{
    return dev_ctrl->timestamp;
}

void devdrv_probe_wait(int devid)
{
    struct devdrv_ctrl *dev_ctrl = &g_ctrls[devid];
    unsigned long timestamp;
    unsigned int timedif;

    timestamp = devdrv_hotreset_get_timetamp(dev_ctrl);
    if (timestamp == 0) {
        return;
    }
check:
    timedif = jiffies_to_msecs(jiffies - timestamp);
    if (timedif < DEVDRV_LOAD_FILE_DELAY) {
        msleep(1);
        goto check;
    }
    return;
}

bool devdrv_pre_hotreset_check(int dev_num)
{
    struct devdrv_ctrl *dev_ctrl = NULL;
    struct devdrv_pci_ctrl *pci_ctrl = NULL;
    struct devdrv_startup_status *startup_status = NULL;
    int i, ready_dev_num;
    int ret = false;

    ready_dev_num = 0;

    for (i = 0; i < BOARD_CLOUD_AI_SERVER_DEV_NUM * MAX_OS_EACH_AISERVER; i++) {
        dev_ctrl = &g_ctrls[i];

        pci_ctrl = (struct devdrv_pci_ctrl *)dev_ctrl->priv;
        if (pci_ctrl != NULL) {
            startup_status = &(pci_ctrl->startup_status);
            if (devdrv_can_hotreset(startup_status)) {
                ready_dev_num++;
            } else {
                devdrv_info("dev id %d cannot hotreset. it's status %d\r\n", dev_ctrl->dev_id, startup_status->status);
            }
        }
    }

    devdrv_info("dev num %d ready_dev_num %d\r\n", dev_num, ready_dev_num);

    if (ready_dev_num == dev_num) {
        ret = true;
    }

    return ret;
}

void devdrv_before_hot_reset(void)
{
    struct devdrv_ctrl *dev_ctrl = NULL;
    struct devdrv_pci_ctrl *pci_ctrl = NULL;
    int i;

    for (i = 0; i < BOARD_CLOUD_AI_SERVER_DEV_NUM * MAX_OS_EACH_AISERVER; i++) {
        dev_ctrl = &g_ctrls[i];

        if (dev_ctrl->startup_flg != DEVDRV_DEV_STARTUP_UNPROBED) {
            pci_ctrl = (struct devdrv_pci_ctrl *)dev_ctrl->priv;
            if (pci_ctrl->shr_para->chip_id > 0) {
                devdrv_info("pcie hotreset dev %d before hot reset remove\r\n", dev_ctrl->dev_id);
                dev_ctrl->master_flag = 0;
                devdrv_device_bus_remove(dev_ctrl->pdev);
                msleep(1); /* free cpu */
            }
        }
    }

    for (i = 0; i < BOARD_CLOUD_AI_SERVER_DEV_NUM * MAX_OS_EACH_AISERVER; i++) {
        dev_ctrl = &g_ctrls[i];

        if (dev_ctrl->startup_flg != DEVDRV_DEV_STARTUP_UNPROBED) {
            pci_ctrl = (struct devdrv_pci_ctrl *)dev_ctrl->priv;
            if (pci_ctrl->shr_para->chip_id == 0) {
                devdrv_info("pcie hotreset dev %d before hot reset unbind\r\n", dev_ctrl->dev_id);
                dev_ctrl->master_flag = 1;
                pci_ctrl->shr_para->hot_reset_pcie_flag = DEVDRV_PCIE_HOT_RESET_FLAG;
                devdrv_device_unbind_driver(dev_ctrl->pdev);
                msleep(1); /* free cpu */
            }
        }
    }
}

void devdrv_do_hot_reset(void)
{
    struct devdrv_ctrl *dev_ctrl = NULL;
    int i;

    for (i = 0; i < BOARD_CLOUD_AI_SERVER_DEV_NUM * MAX_OS_EACH_AISERVER; i++) {
        dev_ctrl = &g_ctrls[i];

        if (dev_ctrl->master_flag == 1) {
            if (dev_ctrl->bus != NULL) {
                devdrv_info("pcie hotreset dev %d do hot reset\r\n", dev_ctrl->dev_id);
                devdrv_device_bridge_bus_reset(dev_ctrl->bus);
            }
        }
    }
}

void devdrv_after_hot_reset(int num)
{
    struct devdrv_ctrl *dev_ctrl = NULL;
    int delay_count = 0;
    int i;
    int num_after;

    for (i = 0; i < BOARD_CLOUD_AI_SERVER_DEV_NUM * MAX_OS_EACH_AISERVER; i++) {
        dev_ctrl = &g_ctrls[i];

        if (dev_ctrl->master_flag == 1) {
            dev_ctrl->master_flag = 0;
            if (dev_ctrl->pdev != NULL) {
                devdrv_info("pcie hotreset dev %d remove\r\n", dev_ctrl->dev_id);
                devdrv_device_bus_remove(dev_ctrl->pdev);
                msleep(1); /* free cpu */
            }
        }
    }

    for (i = 0; i < BOARD_CLOUD_AI_SERVER_DEV_NUM * MAX_OS_EACH_AISERVER; i++) {
        dev_ctrl = &g_ctrls[i];
        devdrv_hotreset_set_timestamp(dev_ctrl);
    }
    /* make sure rescan device num equal with num before hot reset */
    do {
        for (i = 0; i < BOARD_CLOUD_AI_SERVER_DEV_NUM * MAX_OS_EACH_AISERVER; i++) {
            dev_ctrl = &g_ctrls[i];
            devdrv_hotreset_set_timestamp(dev_ctrl);
            if (dev_ctrl->bus != NULL) {
                devdrv_info("pcie hotreset dev %d after hot reset\r\n", dev_ctrl->dev_id);
                devdrv_device_bus_rescan(dev_ctrl->bus->self);
            }
        }
        devdrv_init_dev_num();
        num_after = devdrv_get_dev_num();
        if (num == num_after) {
            break;
        }
        delay_count++;
        ssleep(DEVDRV_HOT_RESET_DELAY);
    } while (delay_count < DEVDRV_MAX_DELAY_COUNT);

    devdrv_info("rescan cost %d time, num_before %d, num_after %d\n", delay_count, num, num_after);
}

bool devdrv_is_amp_system(u32 dev_id, int node_id, int chip_id)
{
    struct devdrv_ctrl *ctrl = NULL;
    struct devdrv_pci_ctrl *pci_ctrl = NULL;
    struct devdrv_shr_para __iomem *shr_para = NULL;
    int i;

    if (chip_id != 0) {
        return false;
    }

    for (i = 0; i < MAX_DEV_CNT; i++) {
        if (dev_id == i) {
            continue;
        }
        ctrl = devdrv_get_bottom_half_devctrl_by_id(i);
        if ((ctrl == NULL) || (ctrl->priv == NULL)) {
            continue;
        }
        pci_ctrl = (struct devdrv_pci_ctrl *)ctrl->priv;
        shr_para = pci_ctrl->shr_para;
        if ((shr_para->node_id == node_id) && (shr_para->chip_id != 0)) {
            return false;
        }
    }

    return true;
}

int devdrv_hot_reset_single_device(u32 dev_id)
{
    struct devdrv_ctrl *dev_ctrl = NULL;
    struct devdrv_pci_ctrl *pci_ctrl = NULL;
    struct devdrv_startup_status *startup_status = NULL;
    int delay_count = 0;

    dev_ctrl = &g_ctrls[dev_id];
    pci_ctrl = (struct devdrv_pci_ctrl *)dev_ctrl->priv;
    if (pci_ctrl == NULL) {
        devdrv_info("dev id %d is not init\r\n", dev_id);
        return -EINVAL;
    }

    startup_status = &(pci_ctrl->startup_status);
    if (devdrv_can_hotreset(startup_status) == false) {
        devdrv_info("dev id %d cannot hotreset. it's status %d\r\n", dev_id, startup_status->status);
        return -EAGAIN;
    }

    if (devdrv_is_amp_system(dev_id, pci_ctrl->shr_para->node_id, pci_ctrl->shr_para->chip_id) == false) {
        devdrv_info("dev id %d is not amp system\r\n", dev_id);
        return -EINVAL;
    }

    /* before hotreset: unbind driver */
    pci_ctrl->shr_para->hot_reset_pcie_flag = DEVDRV_PCIE_HOT_RESET_FLAG;
    devdrv_device_unbind_driver(dev_ctrl->pdev);

    /* do hotreset */
    devdrv_info("pcie hotreset dev %d do hot reset\r\n", dev_id);
    devdrv_device_bridge_bus_reset(dev_ctrl->bus);

    ssleep(3);

    /* after hotreset */
    devdrv_device_bus_remove(dev_ctrl->pdev);

    if (dev_ctrl->priv != NULL) {
        devdrv_info("pcie hotreset dev %d after bus remove\r\n", dev_id);
    }

    do {
        devdrv_hotreset_set_timestamp(dev_ctrl);
        devdrv_device_bus_rescan(dev_ctrl->bus->self);
        if (dev_ctrl->priv != NULL) {
            break;
        }
        ssleep(3);
    } while (delay_count < DEVDRV_MAX_DELAY_COUNT);

    return 0;
}

int devdrv_hot_reset_all_device(void)
{
    int num_before;

    num_before = devdrv_get_dev_num();
    if (num_before <= 0) {
        devdrv_err("pcie hotreset all device have %d device\n", num_before);
        return -EAGAIN;
    }

#ifdef CFG_SOC_PLATFORM_CLOUD
    if (devdrv_pre_hotreset_check(num_before) == false) {
        devdrv_err("can not hotreset device num %d\r\n", num_before);
        return -EAGAIN;
    }

    devdrv_before_hot_reset();

    devdrv_do_hot_reset();

    ssleep(DEVDRV_HOT_RESET_DELAY);

    devdrv_after_hot_reset(num_before);
#endif

    return 0;
}

int devdrv_hot_reset_device(u32 dev_id)
{
    int ret = 0;

    mutex_lock(&g_devdrv_ctrl_mutex);

    if (g_devdrv_ctrl_hot_reset_status == 1) {
        mutex_unlock(&g_devdrv_ctrl_mutex);
        devdrv_err("dev %d pcie hotreset dev %d on process\r\n", dev_id, g_devdrv_ctrl_hot_reset_dev_id);
        return -EAGAIN;
    }

    g_devdrv_ctrl_hot_reset_status = 1;
    g_devdrv_ctrl_hot_reset_dev_id = dev_id;

    mutex_unlock(&g_devdrv_ctrl_mutex);

    if (dev_id == 0xff) {
        ret = devdrv_hot_reset_all_device();
    } else if (dev_id < MAX_DEV_CNT) {
        ret = devdrv_hot_reset_single_device(dev_id);
    } else {
        devdrv_err("pcie hotreset dev %d err\r\n", dev_id);
        ret = -EINVAL;
    }

    if (ret == 0) {
        devdrv_info("pcie hotreset dev %d hot reset success\r\n", dev_id);
    }

    mutex_lock(&g_devdrv_ctrl_mutex);
    g_devdrv_ctrl_hot_reset_status = 0;
    mutex_unlock(&g_devdrv_ctrl_mutex);

    return ret;
}
EXPORT_SYMBOL(devdrv_hot_reset_device);

int devdrv_get_master_devid_in_the_same_os(u32 dev_id, u32 *master_dev_id)
{
    struct devdrv_ctrl *ctrl = NULL;
    struct devdrv_ctrl *peer_ctrl = NULL;
    struct devdrv_pci_ctrl *pci_ctrl = NULL;
    struct devdrv_pci_ctrl *peer_pci_ctrl = NULL;
    struct devdrv_shr_para __iomem *shr_para = NULL;
    struct devdrv_shr_para __iomem *peer_shr_para = NULL;
    int i;

    if ((dev_id >= MAX_DEV_CNT) || (master_dev_id == NULL)) {
        devdrv_err("device id invalid: %d or master is null\n", dev_id);
        return -EINVAL;
    }

    ctrl = devdrv_get_bottom_half_devctrl_by_id(dev_id);
    if ((ctrl == NULL) || (ctrl->priv == NULL)) {
        devdrv_err("pci dev devid %d parameter is error\n", dev_id);
        return -EINVAL;
    }
    pci_ctrl = (struct devdrv_pci_ctrl *)ctrl->priv;
    shr_para = pci_ctrl->shr_para;
    if ((shr_para->chip_type == HISI_MINI) || (shr_para->chip_id == 0)) {
        *master_dev_id = dev_id;
        return 0;
    }

    for (i = 0; i < MAX_DEV_CNT; i++) {
        if (dev_id == i) {
            continue;
        }
        peer_ctrl = devdrv_get_bottom_half_devctrl_by_id(i);
        if ((peer_ctrl == NULL) || (peer_ctrl->priv == NULL)) {
            continue;
        }
        peer_pci_ctrl = (struct devdrv_pci_ctrl *)peer_ctrl->priv;
        peer_shr_para = peer_pci_ctrl->shr_para;
        if ((shr_para->node_id == peer_shr_para->node_id) && (peer_shr_para->chip_id == 0)) {
            *master_dev_id = i;
            return 0;
        }
    }
    devdrv_err("devid %d no find master devid\n", dev_id);
    return -EINVAL;
}
EXPORT_SYMBOL(devdrv_get_master_devid_in_the_same_os);

void *devdrv_get_devdrv_priv(struct pci_dev *pdev)
{
    struct devdrv_pci_ctrl *pci_ctrl = NULL;

    if (pdev == NULL) {
        devdrv_err("pdev is NULL\n");
        return NULL;
    }
    pci_ctrl = (struct devdrv_pci_ctrl *)pci_get_drvdata(pdev);
    if (pci_ctrl == NULL){
        devdrv_err("pci_ctrl is NULL\n");
        return NULL;
    }
    return (void *)&(pci_ctrl->vdavinci_priv);
}
EXPORT_SYMBOL(devdrv_get_devdrv_priv);

int devdrv_get_dev_id_by_pdev(struct pci_dev *pdev)
{
    struct devdrv_pci_ctrl *pci_ctrl = NULL;

    if (pdev == NULL) {
        devdrv_err("pdev is NULL\n");
        return -EINVAL;
    }
    pci_ctrl = (struct devdrv_pci_ctrl *)pci_get_drvdata(pdev);
    if (pci_ctrl == NULL){
        devdrv_err("pci_ctrl is NULL\n");
        return -EINVAL;
    }
    return (int)pci_ctrl->dev_id;
}
EXPORT_SYMBOL(devdrv_get_dev_id_by_pdev);
