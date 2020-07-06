/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2012-2019. All rights reserved.
 * Description:
 * Author: huawei
 * Create: 2017/8/18
 */

#include <linux/sysfs.h>

#include "devdrv_pci.h"
#include "devdrv_ctrl.h"
#include "devdrv_sysfs.h"

static u32 devdrv_sysfs_get_devid_by_dev(struct device *dev)
{
    struct devdrv_ctrl *ctrl = NULL;
    u32 i;

    if (dev == NULL) {
        devdrv_err("dev is null\n");
        return MAX_DEV_CNT;
    }
    for (i = 0; i < MAX_DEV_CNT; i++) {
        ctrl = devdrv_get_devctrl_by_id(i);
        if ((ctrl != NULL) && (dev == ctrl->dev)) {
            return ctrl->dev_id;
        }
    }

    devdrv_err("find devid failed!\n");

    return MAX_DEV_CNT;
}

int devdrv_sysfs_common_msg_send(u32 devid, void *data, u32 *real_out_len)
{
    return devdrv_common_msg_send(devid, data, DEVDRV_SYSFS_MSG_IN_BYTES, sizeof(struct devdrv_sysfs_msg), real_out_len,
                                  DEVDRV_COMMON_MSG_SYSFS);
}

ssize_t devdrv_sysfs_link_info_show(struct device *dev, struct device_attribute *attr, char *buf)
{
    u32 devid;
    u32 real_out_len = 0;
    int ret;
    struct devdrv_sysfs_msg msg;

    msg.type = DEVDRV_SYSFS_LINK_INFO;

    devdrv_info("devdrv sysfs show link info\n");

    devid = devdrv_sysfs_get_devid_by_dev(dev);
    if (devid >= MAX_DEV_CNT) {
        devdrv_err("get devid faild, devid:%d", devid);
        return 0;
    }

    ret = devdrv_sysfs_common_msg_send(devid, (void *)&msg, &real_out_len);
    if (ret) {
        devdrv_err("common_msg_send ERROR\n");
        ret = snprintf_s(buf, PAGE_SIZE, PAGE_SIZE - 1, "read link_info fail, ret:%d\n", ret);
        return ret == -1 ? 0 : ret;
    }

    ret = snprintf_s(buf, PAGE_SIZE, PAGE_SIZE - 1, "link speed: %d, link width: %d, link_status: 0x%x\n",
                     msg.link_info.link_speed, msg.link_info.link_width, msg.link_info.link_status);
    return ret == -1 ? 0 : ret;
}

ssize_t devdrv_sysfs_rx_para_show(struct device *dev, struct device_attribute *attr, char *buf)
{
    u32 devid;
    u32 real_out_len = 0;
    u32 offset = 0;
    u32 i;
    int ret;
    struct devdrv_sysfs_msg msg;

    msg.type = DEVDRV_SYSFS_RX_PARA;

    devdrv_info("devdrv sysfs show rx para\n");

    devid = devdrv_sysfs_get_devid_by_dev(dev);
    if (devid >= MAX_DEV_CNT) {
        devdrv_err("get devid faild, devid:%d\n", devid);
        return 0;
    }

    devdrv_info("get devid success, devid:%d\n", devid);

    ret = devdrv_sysfs_common_msg_send(devid, (void *)&msg, &real_out_len);
    if (ret) {
        devdrv_err("devid %d common_msg_send ERROR\n", devid);
        ret = snprintf_s(buf, PAGE_SIZE, PAGE_SIZE - 1, "read rx_para fail, ret:%d\n", ret);
        if (ret != -1) {
            offset = ret;
        }
        return offset;
    }

    for (i = 0; i < DEVDRV_SYSFS_RX_LANE_MAX; i++) {
        ret = snprintf_s(buf + offset, PAGE_SIZE - offset, PAGE_SIZE - offset - 1,
                         "lane%d  att:%-10d gain:%-10d boost:%-10d tap1:%-10d tap2:%-10d\n", i,
                         msg.rx_para.lane_rx_para[i].att, msg.rx_para.lane_rx_para[i].gain,
                         msg.rx_para.lane_rx_para[i].boost, msg.rx_para.lane_rx_para[i].tap1,
                         msg.rx_para.lane_rx_para[i].tap2);
        if (ret != -1) {
            offset += ret;
        } else {
            devdrv_err("snprintf_s failed\n");
            return 0;
        }
    }

    return offset;
}

ssize_t devdrv_sysfs_tx_para_show(struct device *dev, struct device_attribute *attr, char *buf)
{
    u32 devid;
    u32 real_out_len = 0;
    u32 offset = 0;
    u32 i;
    int ret;
    struct devdrv_sysfs_msg msg;

    msg.type = DEVDRV_SYSFS_TX_PARA;

    devdrv_info("devdrv sysfs show tx para\n");

    devid = devdrv_sysfs_get_devid_by_dev(dev);
    if (devid >= MAX_DEV_CNT) {
        devdrv_err("get devid faild, devid:%d\n", devid);
        return 0;
    }

    devdrv_info("get devid success, devid:%d\n", devid);

    ret = devdrv_sysfs_common_msg_send(devid, (void *)&msg, &real_out_len);
    if (ret) {
        devdrv_err("devid %d common_msg_send ERROR\n", devid);
        ret = snprintf_s(buf, PAGE_SIZE, PAGE_SIZE - 1, "read tx_para fail, ret:%d\n", ret);
        if (ret != -1) {
            offset = ret;
        }
        return offset;
    }

    for (i = 0; i < DEVDRV_SYSFS_TX_LANE_MAX; i++) {
        ret = snprintf_s(buf + offset, PAGE_SIZE - offset, PAGE_SIZE - offset - 1,
                         "lane%d  pre:%-10d main:%-10d post:%-10d\n", i, msg.tx_para.lane_tx_para[i].pre,
                         msg.tx_para.lane_tx_para[i].main, msg.tx_para.lane_tx_para[i].post);
        if (ret != -1) {
            offset += ret;
        } else {
            devdrv_err("devid %d snprintf_s failed\n", devid);
            return 0;
        }
    }

    return offset;
}

ssize_t devdrv_sysfs_aer_cnt_show(struct device *dev, struct device_attribute *attr, char *buf)
{
    u32 devid;
    u32 real_out_len = 0;
    int ret;
    struct devdrv_sysfs_msg msg;

    msg.type = DEVDRV_SYSFS_AER_COUNT;

    devdrv_info("devdrv sysfs show aer count\n");

    devid = devdrv_sysfs_get_devid_by_dev(dev);
    if (devid >= MAX_DEV_CNT) {
        devdrv_err("get devid faild, devid:%d", devid);
        return 0;
    }

    devdrv_info("get devid success, devid:%d", devid);

    ret = devdrv_sysfs_common_msg_send(devid, (void *)&msg, &real_out_len);
    if (ret) {
        devdrv_err("common_msg_send ERROR\n");
        ret = snprintf_s(buf, PAGE_SIZE, PAGE_SIZE - 1, "read aer count fail, ret:%d\n", ret);
        return ret == -1 ? 0 : ret;
    }

    ret = snprintf_s(buf, PAGE_SIZE, PAGE_SIZE - 1, "AER count: %d\n", msg.data[0]);
    return ret == -1 ? 0 : ret;
}

ssize_t devdrv_sysfs_aer_cnt_store(struct device *dev, struct device_attribute *attr, const char *buf, size_t count)
{
    u32 devid;
    u32 real_out_len = 0;
    int ret;
    struct devdrv_sysfs_msg msg;

    msg.type = DEVDRV_SYSFS_AER_CLEAR;

    devdrv_info("devdrv sysfs clear aer count\n");

    devid = devdrv_sysfs_get_devid_by_dev(dev);
    if (devid >= MAX_DEV_CNT) {
        devdrv_err("get devid faild, devid:%d", devid);
        return 0;
    }

    devdrv_info("get devid success, devid:%d", devid);

    if (!strcmp(buf, "clear")) {
        ret = devdrv_sysfs_common_msg_send(devid, (void *)&msg, &real_out_len);
        if (ret) {
            devdrv_err("devid %d common_msg_send ERROR\n", devid);
        }
    }

    return count;
}

ssize_t devdrv_sysfs_bdf_to_devid_show(struct device *dev, struct device_attribute *attr, char *buf)
{
    struct devdrv_ctrl *ctrl = NULL;
    struct pci_dev *pdev = NULL;
    ssize_t offset = 0;
    u8 bus_num = 0;
    u8 device_num = 0;
    u8 func_num = 0;
    int ret;
    u32 i;

    devdrv_info("devdrv sysfs show bdf and device id\n");

    for (i = 0; i < MAX_DEV_CNT; i++) {
        ctrl = devdrv_get_devctrl_by_id(i);
        if (ctrl == NULL) {
            continue;
        }
        if ((ctrl->dev_id < MAX_DEV_CNT) && (ctrl->pdev != NULL) &&
            (ctrl->startup_flg != DEVDRV_DEV_STARTUP_UNPROBED)) {
            pdev = ctrl->pdev;
            bus_num = pdev->bus->number;
            device_num = PCI_SLOT(pdev->devfn);
            func_num = PCI_FUNC(pdev->devfn);
            ret = snprintf_s(buf + offset, PAGE_SIZE - offset, PAGE_SIZE - offset - 1, "%02x:%02x.%d ---> %d\n",
                             bus_num, device_num, func_num, ctrl->dev_id);
            if (ret != -1) {
                offset += ret;
            }
        }
    }

    return offset;
}

ssize_t devdrv_sysfs_bios_version_show(struct device *dev, struct device_attribute *attr, char *buf)
{
    struct devdrv_ctrl *ctrl = NULL;
    struct devdrv_pci_ctrl *pci_ctrl = NULL;
    int i = 0;
    ssize_t offset = 0;
    u8 nve_part1;
    u8 nve_part2;
    u8 nve_part3;
    u16 nve_part4;
    u32 nve_version;
    int ret = 0;

    for (i = 0; i < MAX_DEV_CNT; i++) {
        ctrl = devdrv_get_devctrl_by_id(i);
        if (ctrl == NULL) {
            continue;
        }
        if ((ctrl->dev_id < MAX_DEV_CNT) && (ctrl->dev == dev)) {
            pci_ctrl = (struct devdrv_pci_ctrl *)ctrl->priv;
            break;
        }
    }
    if (pci_ctrl == NULL) {
        devdrv_err("get pci_ctrl failed\n");
        return 0;
    }

    if (!(pci_ctrl->device_bios_version.valid)) {
        ret = snprintf_s(buf, PAGE_SIZE, PAGE_SIZE - 1, "device_fw_unsupported\n");
        if (ret != -1) {
            offset = ret;
        }
        return offset;
    }

    nve_version = pci_ctrl->device_bios_version.nve;
    nve_part1 = (nve_version >> DEVDRV_NVE1_MOVE) & DEVDRV_NVE1_BIT;
    nve_part2 = nve_version & DEVDRV_NVE2_BIT;
    nve_part3 = (nve_version >> DEVDRV_NVE3_MOVE) & DEVDRV_NVE3_BIT;
    nve_part4 = ((nve_version >> DEVDRV_NVE4_1_MOVE) & DEVDRV_NVE4_1_BIT) |
                ((nve_version >> DEVDRV_NVE4_2_MOVE) & DEVDRV_NVE4_2_BIT);

    ret = snprintf_s(buf + offset, PAGE_SIZE - offset, PAGE_SIZE - offset - 1, "bootrom:%x.%x.%x.%x\n",
                     pci_ctrl->device_bios_version.bootrom[DEVDRV_INDEX_0],
                     pci_ctrl->device_bios_version.bootrom[DEVDRV_INDEX_1],
                     pci_ctrl->device_bios_version.bootrom[DEVDRV_INDEX_2],
                     pci_ctrl->device_bios_version.bootrom[DEVDRV_INDEX_3]);
    if (ret != -1) {
        offset += ret;
    }

    ret = snprintf_s(buf + offset, PAGE_SIZE - offset, PAGE_SIZE - offset - 1, "xloader:%x.%x.%x.%x\n",
                     pci_ctrl->device_bios_version.xloader[DEVDRV_INDEX_0],
                     pci_ctrl->device_bios_version.xloader[DEVDRV_INDEX_1],
                     pci_ctrl->device_bios_version.xloader[DEVDRV_INDEX_2],
                     pci_ctrl->device_bios_version.xloader[DEVDRV_INDEX_3]);
    if (ret != -1) {
        offset += ret;
    }

    ret = snprintf_s(buf + offset, PAGE_SIZE - offset, PAGE_SIZE - offset - 1, "nve:%d.%d.%d.%x\n", nve_part1,
                     nve_part2, nve_part3, nve_part4);
    if (ret != -1) {
        offset += ret;
    }

    return offset;
}

ssize_t devdrv_sysfs_get_dev_id(struct device *dev, struct device_attribute *attr, char *buf)
{
    u32 dev_id;
    int ret;
    ssize_t offset = 0;

    (void)attr;

    dev_id = devdrv_sysfs_get_devid_by_dev(dev);
    if (dev_id >= MAX_DEV_CNT) {
        devdrv_err("invalid dev id %d\r\n", dev_id);
        return offset;
    }

    ret = snprintf_s(buf, PAGE_SIZE, PAGE_SIZE - 1, "%d\n", dev_id);
    if (ret >= 0) {
        offset += ret;
    }

    return offset;
}

ssize_t devdrv_sysfs_get_chip_id(struct device *dev, struct device_attribute *attr, char *buf)
{
    struct devdrv_ctrl *ctrl = NULL;
    struct devdrv_pci_ctrl *pci_ctrl = NULL;
    u32 dev_id, chip_id;
    int ret;
    ssize_t offset = 0;

    (void)attr;

    dev_id = devdrv_sysfs_get_devid_by_dev(dev);
    if (dev_id >= MAX_DEV_CNT) {
        devdrv_err("invalid dev id %d\r\n", dev_id);
        return offset;
    }

    ctrl = devdrv_get_bottom_half_devctrl_by_id(dev_id);
    if (ctrl == NULL) {
        devdrv_err("dev id %d not startup\r\n", dev_id);
        return offset;
    }

    pci_ctrl = (struct devdrv_pci_ctrl *)ctrl->priv;

#ifdef CFG_SOC_PLATFORM_CLOUD
    chip_id = pci_ctrl->shr_para->chip_id;
#else
    (void)pci_ctrl;
    chip_id = 0;
#endif

    ret = snprintf_s(buf, PAGE_SIZE, PAGE_SIZE - 1, "%d\n", chip_id);
    if (ret >= 0) {
        offset += ret;
    }

    return offset;
}

ssize_t devdrv_sysfs_get_bus_name(struct device *dev, struct device_attribute *attr, char *buf)
{
    struct devdrv_ctrl *ctrl = NULL;
    struct pci_bus *bus = NULL;
    struct pci_dev *pdev = NULL;
    u32 dev_id;
    int ret;
    ssize_t offset = 0;

    (void)attr;

    dev_id = devdrv_sysfs_get_devid_by_dev(dev);
    if (dev_id >= MAX_DEV_CNT) {
        devdrv_err("invalid dev id %d\r\n", dev_id);
        return offset;
    }

    ctrl = devdrv_get_bottom_half_devctrl_by_id(dev_id);
    if (ctrl == NULL) {
        devdrv_err("dev id %d not startup\r\n", dev_id);
        return offset;
    }

    bus = ctrl->bus;
    if (bus != NULL) {
        pdev = bus->self;
        ret = snprintf_s(buf, PAGE_SIZE, PAGE_SIZE - 1, "%04x:%02x:%02x.%d\n", pci_domain_nr(bus), pdev->bus->number,
                         PCI_SLOT(pdev->devfn), PCI_FUNC(pdev->devfn));
        if (ret >= 0) {
            offset += ret;
        }
    }

    return offset;
}

ssize_t devdrv_sysfs_set_hotreset_flag(struct device *dev, struct device_attribute *attr, const char *buf, size_t count)
{
    struct devdrv_ctrl *ctrl = NULL;
    struct devdrv_pci_ctrl *pci_ctrl = NULL;
    u32 dev_id;
    ssize_t result;
    unsigned long val = 0;

    (void)attr;
    result = kstrtoul(buf, 0, &val);
    if (result < 0) {
        devdrv_err("kstrtoul failed result = %ld", result);
        return result;
    }
    dev_id = devdrv_sysfs_get_devid_by_dev(dev);
    if (dev_id >= MAX_DEV_CNT) {
        devdrv_err("invalid dev id %d\r\n", dev_id);
        return result;
    }

    ctrl = devdrv_get_bottom_half_devctrl_by_id(dev_id);
    if (ctrl == NULL) {
        devdrv_err("dev id %d not startup\r\n", dev_id);
        return result;
    }

    pci_ctrl = (struct devdrv_pci_ctrl *)ctrl->priv;

    if (val == 1) {
#ifdef CFG_SOC_PLATFORM_CLOUD
        if (pci_ctrl != NULL) {
            if (pci_ctrl->shr_para != NULL) {
                pci_ctrl->shr_para->hot_reset_pcie_flag = DEVDRV_PCIE_HOT_RESET_FLAG;
                devdrv_info("devid %d write hotreset flag\n", dev_id);
            }
        }
#endif
    }

    return count;
}
ssize_t devdrv_sysfs_get_common_msg(struct device *dev, struct device_attribute *attr, char *buf)
{
    struct devdrv_ctrl *ctrl = NULL;
    struct devdrv_pci_ctrl *pci_ctrl = NULL;
    u32 dev_id;
    int ret;
    ssize_t offset = 0;
    struct devdrv_sysfs_msg *msg = NULL;
    u32 real_out_len = 0;
    int i;
    u64 tx_total_cnt;
    u64 tx_success_cnt;
    u64 tx_failed_cnt1;
    u64 tx_failed_cnt2;
    u64 tx_failed_cnt3;
    u64 tx_failed_cnt4;
    u64 tx_failed_cnt5;
    u64 rx_total_cnt;
    u64 rx_success_cnt;
    u64 rx_work_max_time;
    u64 rx_work_delay_cnt;
    char *common_str[DEVDRV_COMMON_MSG_TYPE_MAX] = {DEVDRV_PCIVNIC, DEVDRV_SMMU, DEVDRV_DEVMM, " ", DEVDRV_PROFILE,
                                                    DEVDRV_DEVMAN, DEVDRV_HDC, DEVDRV_SYSFS};

    (void)attr;

    dev_id = devdrv_sysfs_get_devid_by_dev(dev);
    if (dev_id >= MAX_DEV_CNT) {
        devdrv_err("invalid dev id %d\r\n", dev_id);
        return offset;
    }

    ctrl = devdrv_get_bottom_half_devctrl_by_id(dev_id);
    if (ctrl == NULL) {
        devdrv_err("dev id %d not startup\r\n", dev_id);
        return offset;
    }

    pci_ctrl = (struct devdrv_pci_ctrl *)ctrl->priv;

    msg = (struct devdrv_sysfs_msg *)kzalloc(sizeof(struct devdrv_sysfs_msg), GFP_KERNEL);
    if (msg == NULL) {
            devdrv_err("malloc fail\n");
            return 0;
    }

    msg->type = DEVDRV_SYSFS_COMMON_MSG;

    ret = devdrv_sysfs_common_msg_send(dev_id, (void *)msg, &real_out_len);
    if (ret) {
        kfree(msg);
        msg = NULL;
        devdrv_err("common_msg_send ERROR\n");
        ret = snprintf_s(buf, PAGE_SIZE, PAGE_SIZE - 1, "read common msg fail, ret:%d\n", ret);
        return ret == -1 ? 0 : ret;
    }

    ret = snprintf_s(buf + offset, PAGE_SIZE - offset, PAGE_SIZE - offset - 1,
                     "\nhost chan|    tx_total    |    tx_success  |  inval |  nodev |"
                     " enosys |timedout| default|"
                     "  rx_total_cnt  | rx_success_cnt | cost_time|work_delay\n");
    if (ret != -1) {
        offset += ret;
    }
    for (i = 0; i < DEVDRV_COMMON_MSG_TYPE_MAX; i++) {
        if (i == DEVDRV_COMMON_MSG_PROFILE - 1) {
            continue;
        }

        tx_total_cnt = pci_ctrl->msg_dev->common_msg.com_msg_stat[i].tx_total_cnt;
        tx_success_cnt = pci_ctrl->msg_dev->common_msg.com_msg_stat[i].tx_success_cnt;
        tx_failed_cnt1 = pci_ctrl->msg_dev->common_msg.com_msg_stat[i].tx_einval_err;
        tx_failed_cnt2 = pci_ctrl->msg_dev->common_msg.com_msg_stat[i].tx_enodev_err;
        tx_failed_cnt3 = pci_ctrl->msg_dev->common_msg.com_msg_stat[i].tx_enosys_err;
        tx_failed_cnt4 = pci_ctrl->msg_dev->common_msg.com_msg_stat[i].tx_etimedout_err;
        tx_failed_cnt5 = pci_ctrl->msg_dev->common_msg.com_msg_stat[i].tx_default_err;
        rx_total_cnt = pci_ctrl->msg_dev->common_msg.com_msg_stat[i].rx_total_cnt;
        rx_success_cnt = pci_ctrl->msg_dev->common_msg.com_msg_stat[i].rx_success_cnt;
        rx_work_max_time = pci_ctrl->msg_dev->common_msg.com_msg_stat[i].rx_work_max_time;
        rx_work_delay_cnt = pci_ctrl->msg_dev->common_msg.com_msg_stat[i].rx_work_delay_cnt;

        ret = snprintf_s(buf + offset, PAGE_SIZE - offset, PAGE_SIZE - offset - 1, "  %s|%-16llu|%-16llu|%-8llu|"
                         "%-8llu|%-8llu|%-8llu|%-8llu|%-16llu|%-16llu|%-10llu|%-10llu\n",
                         common_str[i], tx_total_cnt, tx_success_cnt, tx_failed_cnt1, tx_failed_cnt2,
                         tx_failed_cnt3, tx_failed_cnt4, tx_failed_cnt5, rx_total_cnt, rx_success_cnt,
                         rx_work_max_time, rx_work_delay_cnt);
        if (ret != -1) {
            offset += ret;
        }
    }

    ret = snprintf_s(buf + offset, PAGE_SIZE - offset, PAGE_SIZE - offset - 1,
                     "\ndev  chan|    tx_total    |    tx_success  |  inval |  nodev |"
                     " enosys |timedout| default|"
                     "  rx_total_cnt  | rx_success_cnt | cost_time|work_delay\n");
    if (ret != -1) {
        offset += ret;
    }
    for (i = 0; i < DEVDRV_COMMON_MSG_TYPE_MAX; i++) {
        if (i == DEVDRV_COMMON_MSG_PROFILE - 1) {
            continue;
        }

        tx_total_cnt = msg->common_stat[i].tx_total_cnt;
        tx_success_cnt = msg->common_stat[i].tx_success_cnt;
        tx_failed_cnt1 = msg->common_stat[i].tx_einval_err;
        tx_failed_cnt2 = msg->common_stat[i].tx_enodev_err;
        tx_failed_cnt3 = msg->common_stat[i].tx_enosys_err;
        tx_failed_cnt4 = msg->common_stat[i].tx_etimedout_err;
        tx_failed_cnt5 = msg->common_stat[i].tx_default_err;
        rx_total_cnt = msg->common_stat[i].rx_total_cnt;
        rx_success_cnt = msg->common_stat[i].rx_success_cnt;
        rx_work_max_time = msg->common_stat[i].rx_work_max_time;
        rx_work_delay_cnt = msg->common_stat[i].rx_work_delay_cnt;

        ret = snprintf_s(buf + offset, PAGE_SIZE - offset, PAGE_SIZE - offset - 1, "  %s|%-16llu|%-16llu|%-8llu|"
                         "%-8llu|%-8llu|%-8llu|%-8llu|%-16llu|%-16llu|%-10llu|%-10llu\n",
                         common_str[i], tx_total_cnt, tx_success_cnt, tx_failed_cnt1, tx_failed_cnt2,
                         tx_failed_cnt3, tx_failed_cnt4, tx_failed_cnt5, rx_total_cnt, rx_success_cnt,
                         rx_work_max_time, rx_work_delay_cnt);
        if (ret != -1) {
            offset += ret;
        }
    }

    kfree(msg);
    msg = NULL;
    return offset;
}

int devdrv_sysfs_fill_buf(struct devdrv_msg_dev *msg_dev, struct devdrv_sysfs_msg *msg, char *buf)
{
    int ret;
    u32 i;
    u64 tx_total_cnt;
    u64 tx_success_cnt;
    u64 tx_failed_cnt1;
    u64 tx_failed_cnt2;
    u64 tx_failed_cnt3;
    u64 tx_failed_cnt4;
    u64 tx_failed_cnt5;
    u64 tx_failed_cnt6;
    u64 rx_total_cnt;
    u64 rx_success_cnt;
    u64 rx_para_err;
    u64 rx_work_max_time;
    u64 rx_work_delay_cnt;
    u32 msg_type;
    ssize_t offset = 0;
    struct devdrv_msg_chan *chan = NULL;
    char *non_str[devdrv_msg_client_max] = {DEVDRV_PCIVNIC, DEVDRV_SMMU, DEVDRV_DEVMM, DEVDRV_COMMON,
                                            DEVDRV_DEVMAN, DEVDRV_HDC, DEVDRV_RESERVE};

    ret = snprintf_s(buf + offset, PAGE_SIZE - offset, PAGE_SIZE - offset - 1,
                     "\nhost type|    tx_total    |   tx_success   |  len_err | stat_abno|"
                     "irqtimeout|  timeout | proc_err | inva_para|"
                     "   rx_total   |   rx_success   | para_err | cost_time|work_delay\n");
    if (ret != -1) {
        offset += ret;
    }
    for (i = 0; i < msg_dev->chan_cnt; i++) {
        chan = &(msg_dev->msg_chan[i]);

        if ((chan->status == AGENTDRV_ENABLE) && (chan->queue_type == NON_TRANSPARENT_MSG_QUEUE)) {
            tx_total_cnt = chan->chan_stat.tx_total_cnt;
            tx_success_cnt = chan->chan_stat.tx_success_cnt;
            tx_failed_cnt1 = chan->chan_stat.tx_len_check_err;
            tx_failed_cnt2 = chan->chan_stat.tx_status_abnormal_err;
            tx_failed_cnt3 = chan->chan_stat.tx_irq_timeout_err;
            tx_failed_cnt4 = chan->chan_stat.tx_timeout_err;
            tx_failed_cnt5 = chan->chan_stat.tx_process_err;
            tx_failed_cnt6 = chan->chan_stat.tx_invalid_para_err;
            rx_total_cnt = chan->chan_stat.rx_total_cnt;
            rx_success_cnt = chan->chan_stat.rx_success_cnt;
            rx_para_err = chan->chan_stat.rx_para_err;
            rx_work_max_time = chan->chan_stat.rx_work_max_time;
            rx_work_delay_cnt = chan->chan_stat.rx_work_delay_cnt;

            ret = snprintf_s(buf + offset, PAGE_SIZE - offset, PAGE_SIZE - offset - 1,
                             "  %s|%-16llu|%-16llu|%-10llu|%-10llu|%-10llu|%-10llu|%-10llu|%-10llu|%-16llu|"
                             "%-16llu|%-10llu|%-10llu|%-10llu\n",
                             non_str[chan->msg_type], tx_total_cnt, tx_success_cnt, tx_failed_cnt1, tx_failed_cnt2,
                             tx_failed_cnt3, tx_failed_cnt4, tx_failed_cnt5, tx_failed_cnt6, rx_total_cnt,
                             rx_success_cnt, rx_para_err, rx_work_max_time, rx_work_delay_cnt);
            if (ret != -1) {
                offset += ret;
            }
        }
    }

    ret = snprintf_s(buf + offset, PAGE_SIZE - offset, PAGE_SIZE - offset - 1,
                     "\ndev  type|    tx_total    |   tx_success   |  len_err | dma_copy |          |"
                     "  timeout | proc_err | inva_para|   rx_total   |   rx_success   | para_err |"
                     " cost_time|work_delay\n");
    if (ret != -1) {
        offset += ret;
    }
    for (i = 0; i < devdrv_msg_client_max; i++) {
        if ((msg->non_trans_stat[i].msg_type == 0) || (msg->non_trans_stat[i].msg_type >= devdrv_msg_client_max)) {
            continue;
        }

        msg_type = msg->non_trans_stat[i].msg_type;
        tx_total_cnt = msg->non_trans_stat[i].tx_total_cnt;
        tx_success_cnt = msg->non_trans_stat[i].tx_success_cnt;
        tx_failed_cnt1 = msg->non_trans_stat[i].tx_len_check_err;
        tx_failed_cnt2 = msg->non_trans_stat[i].tx_dma_copy_err;
        tx_failed_cnt3 = msg->non_trans_stat[i].tx_timeout_err;
        tx_failed_cnt4 = msg->non_trans_stat[i].tx_process_err;
        tx_failed_cnt5 = msg->non_trans_stat[i].tx_invalid_para_err;
        rx_total_cnt = msg->non_trans_stat[i].rx_total_cnt;
        rx_success_cnt = msg->non_trans_stat[i].rx_success_cnt;
        rx_para_err = msg->non_trans_stat[i].rx_para_err;
        rx_work_max_time = msg->non_trans_stat[i].rx_work_max_time;
        rx_work_delay_cnt = msg->non_trans_stat[i].rx_work_delay_cnt;

        ret = snprintf_s(buf + offset, PAGE_SIZE - offset, PAGE_SIZE - offset - 1,
                         "  %s|%-16llu|%-16llu|%-10llu|%-10llu|%-10llu|%-10llu|%-10llu|%-10llu|%-16llu|"
                         "%-16llu|%-10llu|%-10llu|%-10llu\n", non_str[msg_type], tx_total_cnt, tx_success_cnt,
                         tx_failed_cnt1, tx_failed_cnt2, 0, tx_failed_cnt3, tx_failed_cnt4, tx_failed_cnt5,
                         rx_total_cnt, rx_success_cnt, rx_para_err, rx_work_max_time, rx_work_delay_cnt);
        if (ret != -1) {
            offset += ret;
        }
    }

    return offset;
}
ssize_t devdrv_sysfs_get_non_trans_msg(struct device *dev, struct device_attribute *attr, char *buf)
{
    struct devdrv_ctrl *ctrl = NULL;
    struct devdrv_pci_ctrl *pci_ctrl = NULL;
    u32 dev_id;
    int ret;
    ssize_t offset = 0;
    struct devdrv_sysfs_msg *msg = NULL;
    struct devdrv_msg_dev *msg_dev = NULL;
    u32 real_out_len = 0;

    (void)attr;

    dev_id = devdrv_sysfs_get_devid_by_dev(dev);
    if (dev_id >= MAX_DEV_CNT) {
        devdrv_err("invalid dev id %d\r\n", dev_id);
        return offset;
    }

    ctrl = devdrv_get_bottom_half_devctrl_by_id(dev_id);
    if (ctrl == NULL) {
        devdrv_err("dev id %d not startup\r\n", dev_id);
        return offset;
    }

    pci_ctrl = (struct devdrv_pci_ctrl *)ctrl->priv;
    msg_dev = pci_ctrl->msg_dev;

    msg = (struct devdrv_sysfs_msg *)kzalloc(sizeof(struct devdrv_sysfs_msg), GFP_KERNEL);
    if (msg == NULL) {
            devdrv_err("malloc fail\n");
            return 0;
    }

    msg->type = DEVDRV_SYSFS_NON_TRANS_MSG;

    ret = devdrv_sysfs_common_msg_send(dev_id, (void *)msg, &real_out_len);
    if (ret) {
        kfree(msg);
        msg = NULL;
        devdrv_err("common_msg_send ERROR\n");
        ret = snprintf_s(buf, PAGE_SIZE, PAGE_SIZE - 1, "read non trans fail, ret:%d\n", ret);
        return ret == -1 ? 0 : ret;
    }

    offset = devdrv_sysfs_fill_buf(msg_dev, msg, buf);

    kfree(msg);
    msg = NULL;
    return offset;
}

ssize_t devdrv_sysfs_sync_dma_info_show(struct device *dev, struct device_attribute *attr, char *buf)
{
        u32 dev_id;
        u32 real_out_len = 0;
        int ret, i;
        struct devdrv_sysfs_msg *msg = NULL;
        ssize_t offset = 0;
        struct devdrv_ctrl *ctrl = NULL;
        struct devdrv_pci_ctrl *pci_ctrl = NULL;
        struct devdrv_msg_dev *msg_dev = NULL;

        dev_id = devdrv_sysfs_get_devid_by_dev(dev);
        if (dev_id >= MAX_DEV_CNT) {
            devdrv_err("invalid dev id %d\r\n", dev_id);
            return offset;
        }

        ctrl = devdrv_get_bottom_half_devctrl_by_id(dev_id);
        if (ctrl == NULL) {
            devdrv_err("dev id %d not startup\r\n", dev_id);
            return offset;
        }

        pci_ctrl = (struct devdrv_pci_ctrl *)ctrl->priv;

        msg_dev = pci_ctrl->msg_dev;

        devdrv_info("devdrv sysfs show common msg info\n");

        msg = (struct devdrv_sysfs_msg *)kzalloc(sizeof(struct devdrv_sysfs_msg), GFP_KERNEL);
        if (msg == NULL) {
                devdrv_err("malloc fail\n");
                return 0;
        }

        msg->type = DEVDRV_SYSFS_SYNC_DMA_INFO;

        devdrv_info("send common msg\n");

        ret = devdrv_sysfs_common_msg_send(dev_id, (void *)msg, &real_out_len);
        if (ret) {
                devdrv_err("common_msg_send ERROR\n");
                ret = snprintf_s(buf, PAGE_SIZE, PAGE_SIZE - 1,
                                 "read aer count fail, ret:%d\n", ret);
                kfree(msg);
                msg = NULL;
                return (ret == -1) ? 0 : ret;
        }

        devdrv_info("send common msg success\n");

        ret = snprintf_s(buf + offset, PAGE_SIZE - offset, PAGE_SIZE - offset - 1,
                         "\n chan|sync_submit|asy_sbumit|sml_submit|total_sync|trig_remot|trg_local_128|done_int  |"
                         "re_schedule|done_work |sync_proc |async_proc|err_int|err_work|max_task_op|sq_idle_cnt\n");
        if (ret != -1) {
                offset += ret;
        }

        for (i = 0; i < DEVDRV_SYSFS_DMA_CHAN_NUM; i++) {
                ret = snprintf_s(buf + offset, PAGE_SIZE - offset, PAGE_SIZE - offset - 1,
                                 " %-4u|%-11llu|%-10llu|%-10llu|%-10llu|%-10llu|%-13llu|%-10llu|%-11llu|%-10llu|"
                                 "%-10llu|%-10llu|%-7llu|%-8llu|%-11llu|%-11llu\n",
                                 i,
                                 msg->sync_dma_stat[i].sync_submit_cnt,
                                 msg->sync_dma_stat[i].async_submit_cnt,
                                 msg->sync_dma_stat[i].sml_submit_cnt,
                                 msg->sync_dma_stat[i].sync_submit_cnt + msg->sync_dma_stat[i].sml_submit_cnt,
                                 msg->sync_dma_stat[i].trigger_remot_int_cnt,
                                 msg->sync_dma_stat[i].trigger_local_128,
                                 msg->sync_dma_stat[i].done_int_cnt,
                                 msg->sync_dma_stat[i].re_schedule_cnt,
                                 msg->sync_dma_stat[i].done_tasklet_in_cnt,
                                 msg->sync_dma_stat[i].sync_sem_up_cnt,
                                 msg->sync_dma_stat[i].async_proc_cnt,
                                 msg->sync_dma_stat[i].err_int_cnt,
                                 msg->sync_dma_stat[i].err_work_cnt,
                                 msg->sync_dma_stat[i].max_task_op_time,
                                 msg->sync_dma_stat[i].sq_idle_bd_cnt);
            if (ret != -1) {
                offset += ret;
            }
        }

        kfree(msg);
        msg = NULL;

        return offset;
}

static DEVICE_ATTR(devdrv_sysfs_link_info, S_IRUSR | S_IRGRP, devdrv_sysfs_link_info_show, NULL);
static DEVICE_ATTR(devdrv_sysfs_rx_para_info, S_IRUSR | S_IRGRP, devdrv_sysfs_rx_para_show, NULL);
static DEVICE_ATTR(devdrv_sysfs_tx_para_info, S_IRUSR | S_IRGRP, devdrv_sysfs_tx_para_show, NULL);
static DEVICE_ATTR(devdrv_sysfs_aer_cnt_info, S_IRUSR | S_IRGRP | S_IWUSR | S_IWGRP, devdrv_sysfs_aer_cnt_show,
                   devdrv_sysfs_aer_cnt_store);
static DEVICE_ATTR(devdrv_sysfs_bdf_to_devid, S_IRUSR | S_IRGRP, devdrv_sysfs_bdf_to_devid_show, NULL);
static DEVICE_ATTR(devdrv_sysfs_bios_version, S_IRUSR | S_IRGRP, devdrv_sysfs_bios_version_show, NULL);

static DEVICE_ATTR(dev_id, S_IRUSR | S_IRGRP, devdrv_sysfs_get_dev_id, NULL);
static DEVICE_ATTR(chip_id, S_IRUSR | S_IRGRP, devdrv_sysfs_get_chip_id, NULL);
static DEVICE_ATTR(bus_name, S_IRUSR | S_IRGRP, devdrv_sysfs_get_bus_name, NULL);
static DEVICE_ATTR(hotreset_flag, S_IWUSR | S_IWGRP, NULL, devdrv_sysfs_set_hotreset_flag);
static DEVICE_ATTR(common_msg, S_IRUSR | S_IRGRP, devdrv_sysfs_get_common_msg, NULL);
static DEVICE_ATTR(non_trans_msg, S_IRUSR | S_IRGRP, devdrv_sysfs_get_non_trans_msg, NULL);
static DEVICE_ATTR(dma_info, S_IRUGO, devdrv_sysfs_sync_dma_info_show, NULL);


static struct attribute *g_devdrv_sysfs_attrs[] = {
    &dev_attr_devdrv_sysfs_link_info.attr,
    &dev_attr_devdrv_sysfs_rx_para_info.attr,
    &dev_attr_devdrv_sysfs_tx_para_info.attr,
    &dev_attr_devdrv_sysfs_aer_cnt_info.attr,
    &dev_attr_devdrv_sysfs_bdf_to_devid.attr,
    &dev_attr_devdrv_sysfs_bios_version.attr,
    &dev_attr_dev_id.attr,
    &dev_attr_chip_id.attr,
    &dev_attr_bus_name.attr,
    &dev_attr_hotreset_flag.attr,
    NULL,
};

static const struct attribute_group g_devdrv_sysfs_group = {
    .attrs = g_devdrv_sysfs_attrs,
};

static struct attribute *g_devdrv_sysfs_msg_attrs[] = {
    &dev_attr_common_msg.attr,
    &dev_attr_non_trans_msg.attr,
    &dev_attr_dma_info.attr,
    NULL,
};

static const struct attribute_group g_devdrv_sysfs_msg_group = {
    .attrs = g_devdrv_sysfs_msg_attrs,
    .name = "msg",
};


int devdrv_sysfs_init(struct devdrv_pci_ctrl *pci_ctrl)
{
    int ret;

    ret =  sysfs_create_group(&pci_ctrl->pdev->dev.kobj, &g_devdrv_sysfs_group);
    if (ret) {
        devdrv_err("sysfs create group failed ! ret %d\n", ret);
        return -1;
    }

    ret =  sysfs_create_group(&pci_ctrl->pdev->dev.kobj, &g_devdrv_sysfs_msg_group);
    if (ret) {
        devdrv_err("sysfs create msg group failed ! ret %d\n", ret);
        return -1;
    }

    return 0;
}

void devdrv_sysfs_exit(struct devdrv_pci_ctrl *pci_ctrl)
{
    sysfs_remove_group(&pci_ctrl->pdev->dev.kobj, &g_devdrv_sysfs_group);
    sysfs_remove_group(&pci_ctrl->pdev->dev.kobj, &g_devdrv_sysfs_msg_group);
}
