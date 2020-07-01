/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2012-2019. All rights reserved.
 * Description:
 * Author: huawei
 * Create: 2019-10-15
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

#include <linux/delay.h>
#include <linux/irq.h>
#include <linux/module.h>
#include <linux/of_address.h>
#include <linux/platform_device.h>
#include <linux/uaccess.h>
#include <linux/idr.h>
#include <linux/errno.h>
#include <linux/fs.h>
#include <linux/sched.h>
#include <linux/stat.h>
#include <linux/io.h>
#include <linux/pci.h>
#include <linux/vmalloc.h>
#include <linux/pid_namespace.h>

#include "devdrv_manager_common.h"
#include "devdrv_pm.h"
#include "devdrv_driver_pm.h"
#include "devdrv_manager.h"
#include "devdrv_manager_msg.h"
#include "devdrv_platform_resource.h"
#include "devdrv_manager_dfx.h"
#include "devdrv_driver_adapt.h"
#include "devdrv_platform_resource.h"
#include "devdrv_manager_rand.h"
#include "devdrv_black_box.h"
#include "devdrv_info.h"
#include "devdrv_pcie.h"
#include "devdrv_device_online.h"

static unsigned int devdrv_manager_major;
static struct class *devdrv_manager_class;
void *dev_manager_no_trasn_chan[DEVDRV_MAX_DAVINCI_NUM];
struct devdrv_manager_info *dev_manager_info;
STATIC struct devdrv_common_msg_client devdrv_manager_common_chan;
STATIC struct devdrv_info *devdrv_info_array[DEVDRV_MAX_DAVINCI_NUM];
STATIC atomic_t manager_context_num[MAX_DOCKER_NUM + 1] = {{0}};
STATIC u32 devdrv_manager_get_devnum(void);
STATIC void devdrv_set_devdrv_info_array(u32 dev_id, struct devdrv_info *dev_info);
STATIC struct devdrv_init_client *devdrv_manager_get_client(void);
STATIC int devdrv_manager_client_info_registered(void);
STATIC void devdrv_manager_set_no_trans_chan(u32 dev_id, void *no_trans_chan);
STATIC int devdrv_manager_ipc_notify_ioctl(struct file *filep, unsigned int cmd, unsigned long arg);

static DEFINE_SPINLOCK(devdrv_spinlock);
static DEFINE_MUTEX(devdrv_get_devnum_mutex);
static DEFINE_MUTEX(devdrv_mutex);

STATIC u32 devdrv_heart_beat_en = 1;

module_param(devdrv_heart_beat_en, uint, S_IRUGO); /*lint !e514 */

STATIC struct devdrv_client_info dev_manager_client_info = {
    .client_init = 0,
    .priv = NULL,
};

#ifndef CFG_SOC_PLATFORM_MINIV2
#define DEVDRV_MEMDUMP_ADDR 0x06E00000
#define DEVDRV_MEMDUMP_SIZE 0x00F00000
#else
#define DEVDRV_MEMDUMP_ADDR 0x0A400000
#define DEVDRV_MEMDUMP_SIZE 0x00F00000
#endif
#define MAX_BIT_NUM 32

#define DEVDRV_IRQ_NUM 80
#define DEVDRV_IRQ_NUM_PLUS 81

void devdrv_manager_set_devdrv_ops(const struct file_operations *ops);
void devdrv_manager_unset_devdrv_ops(void);
STATIC void devdrv_manager_release_one_device(struct devdrv_info *dev_info);
STATIC void devdrv_manager_process_sign_release(pid_t pid);
STATIC int devdrv_manager_create_one_device(struct devdrv_info *dev_info);
u32 devdrv_manager_get_ts_num(struct devdrv_info *dev_info);

#ifdef CONFIG_SYSFS

static u32 sysfs_devid = 0;

#define DEVDRV_ATTR_RO(_name) static struct kobj_attribute _name##_attr = __ATTR_RO(_name)

#define DEVDRV_ATTR(_name) static struct kobj_attribute _name##_attr = __ATTR(_name, 0600, _name##_show, _name##_store)

STATIC ssize_t devdrv_resources_store(struct kobject *kobj, struct kobj_attribute *attr, const char *buf, size_t count)
{
    u32 result;
    int ret;

    if (buf == NULL) {
        devdrv_drv_err("invalid args(buf is null).\n");
        return count;
    }

    ret = kstrtouint(buf, 10, &result); /* base is 10, buf is converted to decimal number */
    if (ret) {
        devdrv_drv_err("unable to transform input string into devid, input string: %s, ret(%d)", buf, ret);
        return count;
    }

    devdrv_drv_info("input devid is: %u.\n", result);

    if (result >= DEVDRV_MAX_DAVINCI_NUM) {
        devdrv_drv_warn("input devid(%u) is too big, reset into 0, input string: %s", result, buf);
        result = 0;
    }

    sysfs_devid = result;

    return count;
}

STATIC ssize_t devdrv_resources_show_refill_buf(struct devdrv_info *dev_info, char *str)
{
    struct devdrv_platform_data *pdata = dev_info->pdata;
    struct devdrv_id_data *id_data = NULL;
    int avl_len = PAGE_SIZE;
    char *refill_buf = str;
    int tlen;

    id_data = &pdata->ts_pdata[0].id_data;
    tlen = snprintf_s(str, avl_len, avl_len - 1, "device: %u -avail- ", dev_info->dev_id);
    if (tlen < 0) {
        devdrv_drv_info("device: %u -avail- snprintf_s failed.\n", dev_info->dev_id);
        return 0;
    }
    str += tlen;
    avl_len -= tlen;
    tlen = snprintf_s(str, avl_len, avl_len - 1, "stream: %u ", dev_info->cce_ctrl[0]->stream_num);
    if (tlen < 0) {
        devdrv_drv_info("snprintf_s fail, dev_id(%u), stream: %u.\n",
                        dev_info->dev_id, dev_info->cce_ctrl[0]->stream_num);
        return 0;
    }
    str += tlen;
    avl_len -= tlen;
    tlen = snprintf_s(str, avl_len, avl_len - 1, "event: %u ", dev_info->cce_ctrl[0]->event_num);
    if (tlen < 0) {
        devdrv_drv_info("snprintf_s fail, dev_id(%u), event: %u.\n",
                        dev_info->dev_id, dev_info->cce_ctrl[0]->event_num);
        return 0;
    }
    str += tlen;
    avl_len -= tlen;
    tlen = snprintf_s(str, avl_len, avl_len - 1, "sq: %u ", dev_info->cce_ctrl[0]->sq_num);
    if (tlen < 0) {
        devdrv_drv_info("snprintf_s fail, dev_id(%u) sq: %u.\n", dev_info->dev_id, dev_info->cce_ctrl[0]->sq_num);
        return 0;
    }
    str += tlen;
    avl_len -= tlen;
    tlen = snprintf_s(str, avl_len, avl_len - 1, "cq: %u ", dev_info->cce_ctrl[0]->cq_num);
    if (tlen < 0) {
        devdrv_drv_info("snprintf_s fail, dev_id(%u), cq: %u.\n", dev_info->dev_id, dev_info->cce_ctrl[0]->cq_num);
        return 0;
    }
    str += tlen;
    avl_len -= tlen;
    tlen = snprintf_s(str, avl_len, avl_len - 1, "notify: %u ", dev_info->cce_ctrl[0]->notify_id_num);
    if (tlen < 0) {
        devdrv_drv_info("snprintf_s fail, dev_id(%u) notify: %u.\n",
                        dev_info->dev_id, dev_info->cce_ctrl[0]->notify_id_num);
        return 0;
    }
    str += tlen;
    avl_len -= tlen;
    tlen = snprintf_s(str, avl_len, avl_len - 1, "model: %u ", dev_info->cce_ctrl[0]->model_id_num);
    if (tlen < 0) {
        devdrv_drv_info("snprintf_s fail, dev_id(%u) model: %u\n",
                        dev_info->dev_id, dev_info->cce_ctrl[0]->model_id_num);
        return 0;
    }
    str += tlen;
    avl_len -= tlen;
    tlen = snprintf_s(str, avl_len, avl_len - 1, "taskpoll: %u\n", dev_info->cce_ctrl[0]->task_id_num);
    if (tlen < 0) {
        devdrv_drv_info("snprintf_s fail, dev_id(%u) taskpoll: %u\n",
                        dev_info->dev_id, dev_info->cce_ctrl[0]->task_id_num);
        return 0;
    }
    str += tlen;
    avl_len -= tlen;
    tlen = snprintf_s(str, avl_len, avl_len - 1, "-resources in host side- stream: %u ", id_data->cur_stream_num);
    if (tlen < 0) {
        devdrv_drv_info("-resources in host side- stream: %u   snprintf_s failed. dev_id(%u)\n",
                        id_data->cur_stream_num, dev_info->dev_id);
        return 0;
    }
    str += tlen;
    avl_len -= tlen;
    tlen = snprintf_s(str, avl_len, avl_len - 1, "event: %u ", id_data->cur_event_num);
    if (tlen < 0) {
        devdrv_drv_info("snprintf_s fail, dev_id(%u) event: %u\n", dev_info->dev_id, id_data->cur_event_num);
        return 0;
    }
    str += tlen;
    avl_len -= tlen;
    tlen = snprintf_s(str, avl_len, avl_len - 1, "sq: %u ", id_data->cur_sq_num);
    if (tlen < 0) {
        devdrv_drv_info("snprintf_s fail. dev_id(%u) sq: %u\n", dev_info->dev_id, id_data->cur_sq_num);
        return 0;
    }
    str += tlen;
    avl_len -= tlen;
    tlen = snprintf_s(str, avl_len, avl_len - 1, "cq: %u ", id_data->cur_cq_num);
    if (tlen < 0) {
        devdrv_drv_info("snprintf_s fail. dev_id(%u) cq: %u\n", dev_info->dev_id, id_data->cur_cq_num);
        return 0;
    }
    str += tlen;
    avl_len -= tlen;
    tlen = snprintf_s(str, avl_len, avl_len - 1, "notify: %u ", id_data->cur_notify_num);
    if (tlen < 0) {
        devdrv_drv_info("snprintf_s fail. dev_id(%u) notify: %u\n", dev_info->dev_id, id_data->cur_notify_num);
        return 0;
    }
    str += tlen;
    avl_len -= tlen;
    tlen = snprintf_s(str, avl_len, avl_len - 1, "model: %u ", id_data->cur_model_num);
    if (tlen < 0) {
        devdrv_drv_info("snprintf_s fail. dev_id(%u) model: %u\n", dev_info->dev_id, id_data->cur_model_num);
        return 0;
    }
    str += tlen;
    avl_len -= tlen;
    tlen = snprintf_s(str, avl_len, avl_len - 1, "taskpoll: %u\n", id_data->cur_task_num);
    if (tlen < 0) {
        devdrv_drv_info("snprintf_s failed. dev_id(%u) taskpoll: %u\n", dev_info->dev_id, id_data->cur_task_num);
        return 0;
    }
    str += tlen;

    return str - refill_buf;
}

STATIC ssize_t devdrv_resources_show(struct kobject *kobj, struct kobj_attribute *attr, char *buf)
{
    struct devdrv_info *dev_info = NULL;
    char *str = buf;
    int ret;
    u32 tsid = 0;

    if ((buf == NULL) || (sysfs_devid >= DEVDRV_MAX_DAVINCI_NUM)) {
        devdrv_drv_err("invalid sysfs_devid(%u) or buf is NULL.\n", sysfs_devid);
        return 0;
    }

    dev_info = devdrv_manager_get_devdrv_info(sysfs_devid);
    if (dev_info == NULL) {
        devdrv_drv_info("device(%u) is not exist.\n", sysfs_devid);
        return 0;
    }

    if (dev_info->cce_ctrl[tsid] == NULL) {
        devdrv_drv_info("device(%u) is not running online mode.\n", sysfs_devid);
        return 0;
    }

    ret = devdrv_resources_show_refill_buf(dev_info, str);
    if (!ret) {
        devdrv_drv_info("devdrv_resources_show:%u sprintf_s failed, ret(%d).\n", sysfs_devid, ret);
        return 0;
    }

    return ret;
}
DEVDRV_ATTR(devdrv_resources);

static struct attribute *devdrv_manager_attrs[] = {
    &devdrv_resources_attr.attr,
    NULL,
};

static struct attribute_group devdrv_manager_attr_group = {
    .attrs = devdrv_manager_attrs,
    .name = "devdrv_manager",
};

#endif /* CONFIG_SYSFS */

int copy_from_user_safe(void *to, const void __user *from, unsigned long n)
{
    if ((to == NULL) || (from == NULL) || (n == 0)) {
        devdrv_drv_err("user pointer is NULL.\n");
        return -EINVAL;
    }

    if (copy_from_user(to, (void *)from, n))
        return -ENODEV;

    return 0;
}
EXPORT_SYMBOL(copy_from_user_safe);

int copy_to_user_safe(void __user *to, const void *from, unsigned long n)
{
    if ((to == NULL) || (from == NULL) || (n == 0)) {
        devdrv_drv_err("user pointer is NULL.\n");
        return -EINVAL;
    }

    if (copy_to_user(to, (void *)from, n))
        return -ENODEV;

    return 0;
}
EXPORT_SYMBOL(copy_to_user_safe);

STATIC void devdrv_set_devdrv_info_array(u32 dev_id, struct devdrv_info *dev_info)
{
    spin_lock(&devdrv_spinlock);
    devdrv_info_array[dev_id] = dev_info;
    spin_unlock(&devdrv_spinlock);
    return;
}

STATIC struct devdrv_info *devdrv_get_devdrv_info_array(u32 dev_id)
{
    struct devdrv_info *dev_info = NULL;

    if (dev_id >= DEVDRV_MAX_DAVINCI_NUM) {
        devdrv_drv_err("invalid dev_id(%u)\n", dev_id);
        return NULL;
    }

    spin_lock(&devdrv_spinlock);
    dev_info = devdrv_info_array[dev_id];
    spin_unlock(&devdrv_spinlock);

    return dev_info;
}

STATIC void devdrv_manager_first_get_devnum(void)
{
    int total_timeout = DEVDRV_GET_DEVNUM_STARTUP_TIMEOUT;
    int timeout = 0;
    u32 num_dev = 0;
    int stage = 0;
    int pcie_num;

    pcie_num = devdrv_get_dev_num();
    if (pcie_num <= 0) {
        devdrv_drv_err("pcie report dev_num fail\n");
        return;
    }

    devdrv_drv_info("pcie report dev_num = %d\n", pcie_num);

    do {
        /* arm centos environment, host_mnt_ns is NULL */
        if (devdrv_manager_get_host_mnt_ns() == NULL) {
            num_dev = devdrv_manager_container_get_devnum_bare();
        } else {
            num_dev = devdrv_manager_container_get_devnum(devdrv_manager_get_host_mnt_ns());
        }
        if (pcie_num == num_dev) {
            devdrv_drv_info("get devnum succ, cost time %d(s) in stage %d \n", timeout, stage);
            break;
        }

        ssleep(1);
        timeout++;
        if ((num_dev > 0) && (num_dev < DEVDRV_MAX_DAVINCI_NUM) && (stage == 0)) {
            devdrv_drv_info("cost %d(s), enter next stage, curr_num_dev = %u\n", timeout, num_dev);
            stage = 1;
            timeout = 0;
            total_timeout = DEVDRV_GET_DEVNUM_SYNC_TIMEOUT;
        }
        if (timeout > total_timeout) {
            devdrv_drv_err("first get dev_num timeout in stage %d\n", stage);
            return;
        }
    } while (1);

    return;
}

STATIC u32 devdrv_manager_get_devnum(void)
{
    STATIC u32 devdrv_first_get_devnum_en = 1;
    u32 num_dev;

    if (dev_manager_info == NULL)
        return (DEVDRV_MAX_DAVINCI_NUM + 1);

    mutex_lock(&devdrv_get_devnum_mutex);
    if (devdrv_first_get_devnum_en) {
        devdrv_manager_first_get_devnum();
        devdrv_first_get_devnum_en = 0;
    }

    num_dev = devdrv_manager_container_get_devnum(current->nsproxy->mnt_ns);

    mutex_unlock(&devdrv_get_devnum_mutex);
    return num_dev;
}

struct devdrv_info *devdrv_manager_get_devdrv_info(u32 dev_id)
{
    struct devdrv_info *dev_info = NULL;
    unsigned long flags;

    if ((dev_manager_info == NULL) || (dev_id >= DEVDRV_MAX_DAVINCI_NUM)) {
        return NULL;
    }

    spin_lock_irqsave(&dev_manager_info->spinlock, flags);
    dev_info = dev_manager_info->dev_info[dev_id];
    spin_unlock_irqrestore(&dev_manager_info->spinlock, flags);

    return dev_info;
}
EXPORT_SYMBOL(devdrv_manager_get_devdrv_info);

STATIC int devdrv_manager_set_devinfo_inc_devnum(u32 dev_id, struct devdrv_info *dev_info)
{
    unsigned long flags;

    if ((dev_id >= DEVDRV_MAX_DAVINCI_NUM) || (dev_manager_info == NULL) || (dev_info == NULL)) {
        devdrv_drv_err("invalid dev_id(%u), or dev_manager_info(%pK) or dev_info(%pK) is NULL\n", dev_id,
                       dev_manager_info, dev_info);
        return -EINVAL;
    }

    spin_lock_irqsave(&dev_manager_info->spinlock, flags);
    if (dev_manager_info->dev_info[dev_id] != NULL) {
        spin_unlock_irqrestore(&dev_manager_info->spinlock, flags);
        devdrv_drv_err("dev_info is not NULL, dev_id(%u)\n", dev_id);
        return -ENODEV;
    }

    if (dev_manager_info->num_dev >= DEVDRV_MAX_DAVINCI_NUM) {
        spin_unlock_irqrestore(&dev_manager_info->spinlock, flags);
        devdrv_drv_err("wrong device num, num_dev(%u). dev_id(%u)\n", dev_manager_info->num_dev, dev_id);
        return -EFAULT;
    }
    dev_manager_info->num_dev++;
    dev_manager_info->dev_info[dev_id] = dev_info;
    spin_unlock_irqrestore(&dev_manager_info->spinlock, flags);

    return 0;
}

STATIC int devdrv_manager_reset_devinfo_dec_devnum(u32 dev_id)
{
    unsigned long flags;

    if ((dev_id >= DEVDRV_MAX_DAVINCI_NUM) || (dev_manager_info == NULL)) {
        devdrv_drv_err("invalid dev_id(%u), or dev_manager_info is NULL\n", dev_id);
        return -EINVAL;
    }

    spin_lock_irqsave(&dev_manager_info->spinlock, flags);
    if (dev_manager_info->dev_info[dev_id] == NULL) {
        spin_unlock_irqrestore(&dev_manager_info->spinlock, flags);
        devdrv_drv_err("dev_info is not NULL, dev_id(%u)\n", dev_id);
        return -ENODEV;
    }

    if ((dev_manager_info->num_dev > DEVDRV_MAX_DAVINCI_NUM) || (dev_manager_info->num_dev == 0)) {
        spin_unlock_irqrestore(&dev_manager_info->spinlock, flags);
        devdrv_drv_err("wrong device num, num_dev(%u). dev_id(%u)\n", dev_manager_info->num_dev, dev_id);
        return -EFAULT;
    }
    dev_manager_info->dev_info[dev_id] = NULL;
    dev_manager_info->num_dev--;
    spin_unlock_irqrestore(&dev_manager_info->spinlock, flags);

    return 0;
}

STATIC int devdrv_manager_set_devdrv_info(u32 dev_id, struct devdrv_info *dev_info)
{
    unsigned long flags;

    if ((dev_id >= DEVDRV_MAX_DAVINCI_NUM) || (dev_manager_info == NULL)) {
        devdrv_drv_err("invalid dev_id(%u), or dev_manager_info is NULL\n", dev_id);
        return -EINVAL;
    }

    spin_lock_irqsave(&dev_manager_info->spinlock, flags);
    dev_manager_info->dev_info[dev_id] = dev_info;
    spin_unlock_irqrestore(&dev_manager_info->spinlock, flags);

    return 0;
}

int devdrv_get_platformInfo(u32 *info)
{
    if (info == NULL)
        return -EINVAL;

    *info = DEVDRV_MANAGER_HOST_ENV;
    return 0;
}
EXPORT_SYMBOL(devdrv_get_platformInfo);

int devdrv_get_devnum(u32 *num_dev)
{
    u32 dev_num;

    dev_num = devdrv_manager_get_devnum();
    if ((dev_num == 0) || (dev_num > DEVDRV_MAX_DAVINCI_NUM)) {
        devdrv_drv_err("invalid device number, dev_num = %d\n", dev_num);
        return -EFAULT;
    }

    *num_dev = dev_num;
    return 0;
}
EXPORT_SYMBOL(devdrv_get_devnum);

int devdrv_get_devids(u32 *devices, u32 device_num)
{
    struct devdrv_info *dev_info = NULL;
    u32 num_dev;
    u32 i;
    u32 j = 0;

    if (devices == NULL) {
        devdrv_drv_err("invalid parameter\n");
        return -EINVAL;
    }

    num_dev = devdrv_manager_get_devnum();
    if ((num_dev > DEVDRV_MAX_DAVINCI_NUM) || (num_dev == 0) || (num_dev > device_num)) {
        devdrv_drv_err("wrong dev number, num_dev = %u max_num :%u\n", num_dev, device_num);
        return -ENODEV;
    }

    for (i = 0; i < DEVDRV_MAX_DAVINCI_NUM; i++) {
        dev_info = devdrv_manager_get_devdrv_info(i);
        if (dev_info == NULL)
            continue;
        if (j >= device_num)
            break;
        devices[j] = dev_info->dev_id;
        j++;
    }
    if (j == 0) {
        devdrv_drv_err("NO dev_info!!!\n");
        return -EFAULT;
    }

    return 0;
}
EXPORT_SYMBOL(devdrv_get_devids);

int devdrv_get_devinfo(u32 devid, struct devdrv_device_info *info)
{
    struct devdrv_platform_data *pdata = NULL;
    struct devdrv_info *dev_info = NULL;

    if (info == NULL) {
        devdrv_drv_err("invalid parameter, dev_id = %d\n", devid);
        return -EINVAL;
    }

    dev_info = devdrv_manager_get_devdrv_info(devid);
    if (dev_info == NULL) {
        devdrv_drv_err("device is not ready, devid = %d\n", devid);
        return -ENODEV;
    }
    pdata = dev_info->pdata;
    /* check if received device ready message from device side */
    if (dev_info->dev_ready == 0) {
        devdrv_drv_err("device(%u) not ready!", dev_info->dev_id);
        return -ENODEV;
    }

    info->ctrl_cpu_ip = dev_info->ctrl_cpu_ip;
    info->ctrl_cpu_id = dev_info->ctrl_cpu_id;
    info->ctrl_cpu_core_num = dev_info->ctrl_cpu_core_num;
    info->ctrl_cpu_endian_little = dev_info->ctrl_cpu_endian_little;
    info->ts_cpu_core_num = pdata->ts_pdata[0].ts_cpu_core_num;
    info->ai_cpu_core_num = dev_info->ai_cpu_core_num;
    info->ai_core_num = dev_info->ai_core_num;
    info->aicpu_occupy_bitmap = dev_info->aicpu_occupy_bitmap;
    info->env_type = dev_info->env_type;

    return 0;
}
EXPORT_SYMBOL(devdrv_get_devinfo);

int devdrv_get_core_inuse(u32 devid, struct devdrv_hardware_inuse *inuse)
{
    struct devdrv_info *dev_info = NULL;

    if (devid >= DEVDRV_MAX_DAVINCI_NUM) {
        devdrv_drv_err("invalid dev_id(%u).\n", devid);
        return -EINVAL;
    }

    if ((inuse == NULL) || (dev_manager_info == NULL) || (dev_manager_info->dev_info[devid] == NULL)) {
        devdrv_drv_err("inuse(%pK) or dev_manager_info(%pK) or dev_manager_info->dev_info[devid(%u)] is NULL.\n", inuse,
                       dev_manager_info, devid);
        return -EINVAL;
    }

    dev_info = dev_manager_info->dev_info[devid];

    if (devdrv_is_ts_work(&dev_info->ts_mng[0]) == 0) {
        devdrv_drv_err("device(%u) is not working.\n", devid);
        return -ENOMEM;
    }

    inuse->ai_core_num = dev_info->inuse.ai_core_num;
    inuse->ai_core_error_bitmap = dev_info->inuse.ai_core_error_bitmap;
    inuse->ai_cpu_num = dev_info->inuse.ai_cpu_num;
    inuse->ai_cpu_error_bitmap = dev_info->inuse.ai_cpu_error_bitmap;

    return 0;
}
EXPORT_SYMBOL(devdrv_get_core_inuse);

int devdrv_get_core_spec(u32 devid, struct devdrv_hardware_spec *spec)
{
    struct devdrv_info *dev_info = NULL;

    if (devid >= DEVDRV_MAX_DAVINCI_NUM) {
        devdrv_drv_err("invalid dev_id(%u).\n", devid);
        return -EINVAL;
    }

    if ((spec == NULL) || (dev_manager_info == NULL) || (dev_manager_info->dev_info[devid] == NULL)) {
        devdrv_drv_err("spec(%pK) or dev_manager_info(%pK) or dev_manager_info->dev_info[devid(%u)] is NULL.\n", spec,
                       dev_manager_info, devid);
        return -EINVAL;
    }

    dev_info = dev_manager_info->dev_info[devid];

    spec->ai_core_num = dev_info->ai_core_num;
    spec->first_ai_core_id = dev_info->ai_core_id;
    spec->ai_cpu_num = dev_info->ai_cpu_core_num;
    spec->first_ai_cpu_id = dev_info->ai_cpu_core_id;

    return 0;
}
EXPORT_SYMBOL(devdrv_get_core_spec);

STATIC int devdrv_manager_run_register_client(struct devdrv_info *dev_info)
{
    struct devdrv_init_client *dev_client = NULL;
    unsigned long flags;
    int ret;
    u32 tsid = 0;

    if (dev_info == NULL) {
        devdrv_drv_err("invalid args.\n");
        return -EINVAL;
    }

    dev_client = devdrv_manager_get_client();
    mutex_lock(&dev_info->lock);
    if ((dev_info->driver_flag == 0) && (devdrv_manager_client_info_registered() == 0)) {
        ret = dev_client->register_client(dev_info);
        if (ret) {
            devdrv_drv_err("device(%u) driver register failed\n", dev_info->dev_id);
            mutex_unlock(&dev_info->lock);
            return -EINVAL;
        }
        dev_info->driver_flag = 1;

        devdrv_drv_debug("*** set driver ts status: %d ***\n",
            dev_info->ts_mng[tsid].ts_work_status_shadow);

        spin_lock_irqsave(&dev_info->ts_mng[tsid].ts_spinlock, flags);
        if (dev_info->ts_mng[tsid].ts_work_status != NULL)
            *dev_info->ts_mng[tsid].ts_work_status = dev_info->ts_mng[tsid].ts_work_status_shadow;
        spin_unlock_irqrestore(&dev_info->ts_mng[tsid].ts_spinlock, flags);
    }
    mutex_unlock(&dev_info->lock);
    return 0;
}
STATIC void devdrv_manager_run_unregister_client(struct devdrv_info *dev_info)
{
    struct devdrv_init_client *dev_client = NULL;

    if ((dev_info->driver_flag == 0) || (devdrv_manager_client_info_registered() != 0))
        return;

    dev_client = devdrv_manager_get_client();
    dev_client->unregister_client(dev_info);
    dev_info->driver_flag = 0;
}

STATIC int devdrv_manager_ipc_notify_init(struct devdrv_manager_context *dev_manager_context)
{
    struct ipc_notify_info *ipc_notify_info = NULL;

    ipc_notify_info = vzalloc(sizeof(struct ipc_notify_info));
    if (ipc_notify_info == NULL)
        return -ENOMEM;
    ipc_notify_info->create_fd_num = 0;
    ipc_notify_info->open_fd_num = 0;
    INIT_LIST_HEAD(&ipc_notify_info->create_list_head);
    INIT_LIST_HEAD(&ipc_notify_info->open_list_head);
    mutex_init(&ipc_notify_info->info_mutex);

    dev_manager_context->ipc_notify_info = ipc_notify_info;

    return 0;
}

STATIC void devdrv_manager_ipc_notify_uninit(struct devdrv_manager_context *dev_manager_context)
{
    struct ipc_notify_info *ipc_notify_info = NULL;

    ipc_notify_info = dev_manager_context->ipc_notify_info;
    if (ipc_notify_info == NULL)
        return;

    mutex_destroy(&ipc_notify_info->info_mutex);
    vfree(ipc_notify_info);
    ipc_notify_info = NULL;
    dev_manager_context->ipc_notify_info = NULL;
}

STATIC int devdrv_manager_ipc_id_init(struct devdrv_manager_context *dev_manager_context)
{
    if (devdrv_manager_ipc_notify_init(dev_manager_context)) {
        devdrv_drv_err("ipc notify init failed\n");
        return -ENODEV;
    }

    return 0;
}

STATIC void devdrv_manager_ipc_id_uninit(struct devdrv_manager_context *dev_manager_context)
{
    devdrv_manager_ipc_notify_uninit(dev_manager_context);
}

STATIC struct devdrv_manager_context *devdrv_manager_context_init(void)
{
    struct devdrv_manager_context *dev_manager_context = NULL;

    dev_manager_context = vzalloc(sizeof(struct devdrv_manager_context));
    if (dev_manager_context == NULL) {
        devdrv_drv_err("kmalloc dev_manager_context failed\n");
        return NULL;
    }

    dev_manager_context->pid = current->pid;
    dev_manager_context->tgid = task_tgid_vnr(current);
    dev_manager_context->task = current;
    dev_manager_context->mnt_ns = current->nsproxy->mnt_ns;
    dev_manager_context->start_time = current->start_time;
    dev_manager_context->real_start_time = current->real_start_time;
#if LINUX_VERSION_CODE >= KERNEL_VERSION(3, 11, 0)
    dev_manager_context->pid_ns = current->nsproxy->pid_ns_for_children;
#else
    dev_manager_context->pid_ns = current->nsproxy->pid_ns;
#endif

    if (devdrv_manager_ipc_id_init(dev_manager_context)) {
        devdrv_drv_err("manager ipc id init failed\n");
        vfree(dev_manager_context);
        dev_manager_context = NULL;
        return NULL;
    }

    return dev_manager_context;
}

STATIC void devdrv_manager_context_uninit(struct devdrv_manager_context *dev_manager_context)
{
    if (dev_manager_context == NULL)
        return;
    devdrv_manager_ipc_id_uninit(dev_manager_context);
    vfree(dev_manager_context);
    dev_manager_context = NULL;
}

STATIC int devdrv_manager_open(struct inode *inode, struct file *filep)
{
    struct devdrv_manager_context *dev_manager_context = NULL;
    u32 docker_id;
    int ret;

    dev_manager_context = devdrv_manager_context_init();
    if (dev_manager_context == NULL) {
        devdrv_drv_err("context init failed\n");
        return -ENOMEM;
    }

    ret = devdrv_manager_container_table_overlap(dev_manager_context, &docker_id);
    if (ret) {
        devdrv_manager_context_uninit(dev_manager_context);
        devdrv_drv_err("update_item failed, ret(%d).\n", ret);
        return -EINVAL;
    }

    if (atomic_read(&manager_context_num[docker_id]) >= DEVDRV_MAX_PID_NUM) {
        devdrv_manager_container_table_process_release(dev_manager_context);
        devdrv_manager_context_uninit(dev_manager_context);
        devdrv_drv_err("manager_context_num has exceeded maximum(%u), docker_id(%u).\n",
            DEVDRV_MAX_PID_NUM, docker_id);
        return -EINVAL;
    }

    dev_manager_context->docker_id = docker_id;
    atomic_inc(&manager_context_num[docker_id]);
    devdrv_drv_info("current manager_context_num(%u), docker_id(%u).\n",
        atomic_read(&manager_context_num[docker_id]), docker_id);
    filep->private_data = dev_manager_context;

    return 0;
}

STATIC void devdrv_manager_context_num_dec(struct devdrv_manager_context *context)
{
    if (context == NULL)
        return;

    atomic_dec(&manager_context_num[context->docker_id]);
    devdrv_drv_info("current docker_id(%u), manager_context_num(%d).\n", context->docker_id,
        atomic_read(&manager_context_num[context->docker_id]));

    return;
}

STATIC void devdrv_manager_ipc_id_recycle_work(struct work_struct *work)
{
    struct devdrv_manager_context *context = NULL;

    context = container_of(work, struct devdrv_manager_context, work);
    devdrv_manager_ipc_notify_release_recycle(context);
    devdrv_manager_context_num_dec(context);
    devdrv_manager_context_uninit(context);
}

STATIC void devdrv_manager_resource_recycle(struct devdrv_manager_context *dev_manager_context)
{
    struct devdrv_manager_info *manager_info = devdrv_get_manager_info();
    struct ipc_notify_info *ipc_notify_info = dev_manager_context->ipc_notify_info;
    u32 ipc_notify_create_num = 0;
    u32 ipc_notify_open_num = 0;

    if (ipc_notify_info != NULL) {
        ipc_notify_open_num = ipc_notify_info->open_fd_num;
        ipc_notify_create_num = ipc_notify_info->create_fd_num;
    }

    if ((ipc_notify_open_num > 0) || (ipc_notify_create_num > 0)) {
        devdrv_drv_info("ipc resource leak, "
                        "ipc_notify_create_num = %d, "
                        "ipc_notify_open_num = %d\n",
                        ipc_notify_create_num, ipc_notify_open_num);

        INIT_WORK(&dev_manager_context->work, devdrv_manager_ipc_id_recycle_work);
        queue_work(manager_info->ipc_event_recycle, &dev_manager_context->work);
        return;
    }
    /* there is no resource leak */
    devdrv_manager_context_num_dec(dev_manager_context);
    devdrv_manager_context_uninit(dev_manager_context);
    return;
}

STATIC void devdrv_manager_process_sign_release(pid_t pid)
{
    struct devdrv_manager_info *d_info = NULL;
    struct devdrv_process_sign *d_sign = NULL;
    struct list_head *pos = NULL;
    struct list_head *n = NULL;

    d_info = devdrv_get_manager_info();
    if (d_info == NULL) {
        devdrv_drv_err("d_info is NULL.\n");
        return;
    }
    down(&d_info->devdrv_sign_list_sema);
    if (!list_empty_careful(&d_info->hostpid_list_header)) {
        list_for_each_safe(pos, n, &d_info->hostpid_list_header) {
            d_sign = list_entry(pos, struct devdrv_process_sign, list);
            if (d_sign->hostpid == pid) {
                list_del(&d_sign->list);
                d_info->devdrv_sign_count[d_sign->docker_id]--;
                kfree(d_sign);
                d_sign = NULL;
                break;
            }
        }
    }
    up(&d_info->devdrv_sign_list_sema);

    return;
}

STATIC int devdrv_manager_release(struct inode *inode, struct file *filep)
{
    struct devdrv_manager_context *dev_manager_context = NULL;

    devdrv_manager_process_sign_release(current->tgid);
    devdrv_host_black_box_close_check(current->tgid);

    dev_manager_context = filep->private_data;
    if (dev_manager_context == NULL) {
        devdrv_drv_err("context is NULL\n");
        return -ENOMEM;
    }


    devdrv_flush_p2p(dev_manager_context->pid);
    devdrv_manager_container_table_process_release(dev_manager_context);
    devdrv_container_assign_admin_end();
#if LINUX_VERSION_CODE >= KERNEL_VERSION(3, 11, 0)
    put_pid_ns(dev_manager_context->pid_ns);
#endif
    devdrv_manager_resource_recycle(dev_manager_context);
	

    return 0;
}

struct devdrv_manager_info *devdrv_get_manager_info(void)
{
    return dev_manager_info;
}
EXPORT_SYMBOL(devdrv_get_manager_info);

STATIC int devdrv_manager_get_pci_info(struct file *filep, unsigned int cmd, unsigned long arg)
{
    struct devdrv_platform_data *pdata = NULL;
    u32 dev_id = DEVDRV_MAX_DAVINCI_NUM + 1;
    struct devdrv_info *dev_info = NULL;
    struct devdrv_pci_info pci_info;
    u32 virt_id;

    if (copy_from_user_safe(&pci_info, (void *)(uintptr_t)arg, sizeof(struct devdrv_pci_info))) {
        return -EFAULT;
    }
    virt_id = pci_info.dev_id;
    if (devdrv_manager_container_logical_id_to_physical_id(virt_id, &dev_id) != 0) {
        devdrv_drv_err("can't transfor virt id %u \n", virt_id);
        return -EFAULT;
    }

    if (dev_id >= DEVDRV_MAX_DAVINCI_NUM) {
        devdrv_drv_err("invalid dev_id(%u)\n", dev_id);
        return -ENODEV;
    }
    dev_info = dev_manager_info->dev_info[dev_id];
    if ((dev_info == NULL) || (dev_info->pdata == NULL)) {
        devdrv_drv_err("dev_info[%u] is NULL\n", dev_id);
        return -EFAULT;
    }
    pdata = dev_info->pdata;
    pci_info.bus_number = pdata->pci_info.bus_number;
    pci_info.dev_number = pdata->pci_info.dev_number;
    pci_info.function_number = pdata->pci_info.function_number;

    if (copy_to_user_safe((void *)(uintptr_t)arg, &pci_info, sizeof(struct devdrv_pci_info)))
        return -EFAULT;

    return 0;
}

STATIC int devdrv_manager_svmva_to_devid(struct file *filep, unsigned int cmd, unsigned long arg)
{
    struct devdrv_svm_to_devid devdrv_svm_devid = {0};
    u32 dev_id;

    if (copy_from_user_safe(&devdrv_svm_devid, (void *)(uintptr_t)arg, sizeof(devdrv_svm_devid))) {
        devdrv_drv_err("copy from user failed.\n");
        return -EFAULT;
    }

    if ((dev_manager_info->drv_ops == NULL) || (dev_manager_info->drv_ops->svm_va_to_devid == NULL)) {
        devdrv_drv_err("dev_manager_info->drv_ops(%pK) or dev_manager_info->drv_ops->svm_va_to_devid is NULL\n",
                       dev_manager_info->drv_ops);
        return -ENOMEM;
    }

    dev_id = dev_manager_info->drv_ops->svm_va_to_devid(devdrv_svm_devid.src_addr);
    if (dev_id >= DEVDRV_MAX_DAVINCI_NUM) {
        devdrv_drv_err("invalid src dev_id(%u)\n", dev_id);
        return -ENODEV;
    }

    devdrv_svm_devid.src_devid = dev_id;
    dev_id = dev_manager_info->drv_ops->svm_va_to_devid(devdrv_svm_devid.dest_addr);
    if (dev_id >= DEVDRV_MAX_DAVINCI_NUM) {
        devdrv_drv_err("invalid dst dev_id(%u)\n", dev_id);
        return -ENODEV;
    }

    devdrv_svm_devid.dest_devid = dev_id;
    if (copy_to_user_safe((void *)(uintptr_t)arg, &devdrv_svm_devid, sizeof(struct devdrv_svm_to_devid))) {
        devdrv_drv_err("copy to user failed.\n");
        return -EFAULT;
    }
    return 0;
}

STATIC int devdrv_manager_get_device_status(struct file *filep, unsigned int cmd, unsigned long arg)
{
    u32 para;
    int ret;

    u32 phys_id = DEVDRV_MAX_DAVINCI_NUM + 1;
    ret = copy_from_user_safe(&para, (void *)(uintptr_t)arg, sizeof(u32));
    if (ret) {
        devdrv_drv_err("copy from user failed, ret(%d).\n", ret);
        return -EINVAL;
    }
    if (para >= DEVDRV_MAX_DAVINCI_NUM) {
        devdrv_drv_err("invalid dev_id(%u).\n", para);
        return -EINVAL;
    }

    if (devdrv_manager_container_logical_id_to_physical_id(para, &phys_id) != 0) {
        devdrv_drv_err("can't transfor virt id %u.\n", para);
        return -EFAULT;
    }

    if ((dev_manager_info == NULL) || (dev_manager_info->dev_info[phys_id] == NULL))
        para = DRV_STATUS_INITING;
    else if (dev_manager_info->dev_info[phys_id]->ts_mng[0].ts_work_status_shadow == DEVDRV_TS_DOWN)
        para = DRV_STATUS_EXCEPTION;
    else if (dev_manager_info->dev_info[phys_id]->ts_mng[0].ts_work_status_shadow == DEVDRV_TS_WORK)
        para = DRV_STATUS_WORK;
    else
        para = DRV_STATUS_INITING;

    ret = copy_to_user_safe((void *)(uintptr_t)arg, &para, sizeof(u32));
    if (ret) {
        devdrv_drv_err("copy to user failed, ret(%d). dev_id(%u)\n", ret, phys_id);
        return -EINVAL;
    }
    return 0;
}

STATIC int devdrv_manager_get_core(struct file *filep, unsigned int cmd, unsigned long arg)
{
    struct devdrv_hardware_inuse inuse;
    struct devdrv_hardware_spec spec;
    int ret;
    u32 phys_id = DEVDRV_MAX_DAVINCI_NUM + 1;
    switch (cmd) {
        case DEVDRV_MANAGER_GET_CORE_SPEC:
            ret = copy_from_user_safe(&spec, (void *)((uintptr_t)arg), sizeof(struct devdrv_hardware_spec));
            if (ret) {
                devdrv_drv_err("copy_from_user_safe failed, ret(%d).\n", ret);
                return ret;
            }
            if (devdrv_manager_container_logical_id_to_physical_id(spec.devid, &phys_id) != 0) {
                devdrv_drv_err("can't transfor virt id %u \n", spec.devid);
                return -EFAULT;
            }

            ret = devdrv_get_core_spec(phys_id, &spec);
            if (ret) {
                devdrv_drv_err("devdrv_get_core_spec failed, ret(%d), dev_id(%u).\n", ret, phys_id);
                return ret;
            }
            ret = copy_to_user_safe((void *)((uintptr_t)arg), &spec, sizeof(struct devdrv_hardware_spec));
            if (ret) {
                devdrv_drv_err("copy_to_user_safe failed, ret(%d). dev_id(%u)\n", ret, phys_id);
                return ret;
            }
            break;
        case DEVDRV_MANAGER_GET_CORE_INUSE:
            ret = copy_from_user_safe(&inuse, (void *)((uintptr_t)arg), sizeof(struct devdrv_hardware_inuse));
            if (ret) {
                devdrv_drv_err("copy_from_user_safe failed, ret(%d).\n", ret);
                return ret;
            }

            if (devdrv_manager_container_logical_id_to_physical_id(inuse.devid, &phys_id) != 0) {
                devdrv_drv_err("can't transfor virt id %u \n", inuse.devid);
                return -EFAULT;
            }

            ret = devdrv_get_core_inuse(phys_id, &inuse);
            if (ret) {
                devdrv_drv_err("devdrv_get_core_inuse failed, ret(%d), dev_id(%u).\n", ret, phys_id);
                return ret;
            }
            ret = copy_to_user_safe((void *)(uintptr_t)arg, &inuse, sizeof(struct devdrv_hardware_inuse));
            if (ret) {
                devdrv_drv_err("copy_to_user_safe failed, ret(%d). dev_id(%u)\n", ret, phys_id);
                return ret;
            }
            break;
        default:
            devdrv_drv_err("invalid cmd.\n");
            return -EINVAL;
    }
    return 0;
}

STATIC int devdrv_manager_get_devids(unsigned long arg)
{
    struct devdrv_manager_hccl_devinfo hccl_devinfo = {0};
    u32 num_dev;
    int ret;

    num_dev = devdrv_manager_get_devnum();
    if ((num_dev > DEVDRV_MAX_DAVINCI_NUM) || (num_dev == 0)) {
        devdrv_drv_err("invalid num_dev(%u)\n", num_dev);
        return -EINVAL;
    }

    devdrv_drv_debug("dev_num = %u\n", num_dev);
    hccl_devinfo.num_dev = num_dev;

    ret = devdrv_manager_container_get_devlist(hccl_devinfo.devids, DEVDRV_MAX_DAVINCI_NUM, &num_dev,
                                               current->nsproxy->mnt_ns);
    if (ret) {
        devdrv_drv_err("get devlist failed, ret(%d)\n", ret);
        return -ENODEV;
    }

    if (copy_to_user_safe((void *)(uintptr_t)arg, &hccl_devinfo, sizeof(hccl_devinfo))) {
        devdrv_drv_err("copy from user failed.\n");
        return -EINVAL;
    }

    return 0;
}

STATIC int devdrv_manager_get_container_devids(unsigned long arg)
{
    struct devdrv_manager_hccl_devinfo hccl_devinfo = {0};
    int ret;

    ret = devdrv_manager_container_get_devids_list(&hccl_devinfo);
    if (ret) {
        devdrv_drv_err("get container devlist failed, ret(%d)\n", ret);
        return -ENODEV;
    }

    if (copy_to_user_safe((void *)(uintptr_t)arg, &hccl_devinfo, sizeof(hccl_devinfo))) {
        devdrv_drv_err("copy from user failed, ret(%d).\n", ret);
        return -EINVAL;
    }

    return 0;
}

STATIC int devdrv_manager_get_devinfo(unsigned long arg)
{
    struct devdrv_manager_hccl_devinfo hccl_devinfo = {0};
    struct devdrv_platform_data *pdata = NULL;
    struct devdrv_info *dev_info = NULL;
    u32 dev_id;
    u32 phys_id = DEVDRV_MAX_DAVINCI_NUM + 1;

    if (copy_from_user_safe(&hccl_devinfo, (void *)(uintptr_t)arg, sizeof(hccl_devinfo))) {
        devdrv_drv_err("copy from user failed.\n");
        return -EINVAL;
    }

    dev_id = hccl_devinfo.dev_id;
    if (dev_id >= DEVDRV_MAX_DAVINCI_NUM) {
        devdrv_drv_err("invalid dev_id(%u)\n", hccl_devinfo.dev_id);
        return -ENODEV;
    }
    if (devdrv_manager_container_logical_id_to_physical_id(dev_id, &phys_id) != 0) {
        devdrv_drv_err("can't transfor virt id %u \n", dev_id);
        return -EFAULT;
    }

    dev_info = devdrv_manager_get_devdrv_info(phys_id);
    if (dev_info == NULL) {
        devdrv_drv_err("device(%u) is not initialized\n", phys_id);
        return -ENODEV;
    }

    atomic_inc(&dev_info->occupy_ref);
    if (dev_info->status == 1) {
        atomic_dec(&dev_info->occupy_ref);
        devdrv_drv_warn("dev %d has been reset\n", dev_info->dev_id);
        return -EINVAL;
    }

    pdata = dev_info->pdata;
    hccl_devinfo.ai_core_num = dev_info->ai_core_num;
    hccl_devinfo.ai_cpu_core_num = dev_info->ai_cpu_core_num;
    hccl_devinfo.ctrl_cpu_core_num = dev_info->ctrl_cpu_core_num;

    /* 1:little endian 0:big endian */
    hccl_devinfo.ctrl_cpu_endian_little = dev_info->ctrl_cpu_endian_little;
    hccl_devinfo.ctrl_cpu_id = dev_info->ctrl_cpu_id;
    hccl_devinfo.ctrl_cpu_ip = dev_info->ctrl_cpu_ip;
    hccl_devinfo.ts_cpu_core_num = pdata->ts_pdata[0].ts_cpu_core_num;
    hccl_devinfo.env_type = dev_info->env_type;
    hccl_devinfo.ai_core_id = dev_info->ai_core_id;
    hccl_devinfo.ai_cpu_core_id = dev_info->ai_cpu_core_id;
    hccl_devinfo.ai_cpu_bitmap = dev_info->aicpu_occupy_bitmap;
    hccl_devinfo.hardware_version = dev_info->hardware_version;
    hccl_devinfo.ts_num = devdrv_manager_get_ts_num(dev_info);

    if (dev_info->drv_ops->get_dev_info(dev_info)) {
        devdrv_drv_err("device info get failed. dev_id(%u)\n", phys_id);
    }
    hccl_devinfo.ctrl_cpu_ip = dev_info->ctrl_cpu_ip;

    devdrv_drv_debug("ctrl_cpu_ip(0x%x), ts_cpu_core_num(%d), dev_id(%u)\n", dev_info->ctrl_cpu_ip,
                     hccl_devinfo.ts_cpu_core_num, phys_id);

    atomic_dec(&dev_info->occupy_ref);

    if (copy_to_user_safe((void *)(uintptr_t)arg, &hccl_devinfo, sizeof(hccl_devinfo))) {
        devdrv_drv_err("copy to user failed. dev_id(%u)\n", phys_id);
        return -EFAULT;
    }

    return 0;
}

STATIC int devdrv_manager_devinfo_ioctl(struct file *filep, unsigned int cmd, unsigned long arg)
{
    int ret;

    switch (cmd) {
        case DEVDRV_MANAGER_GET_DEVIDS:
            ret = devdrv_manager_get_devids(arg);
            break;
        case DEVDRV_MANAGER_GET_CONTAINER_DEVIDS:
            ret = devdrv_manager_get_container_devids(arg);
            break;
        case DEVDRV_MANAGER_GET_DEVINFO:
            ret = devdrv_manager_get_devinfo(arg);
            break;
        default:
            ret = -EINVAL;
            break;
    }

    return ret;
}

/* This interface does not support using in containers */
STATIC int devdrv_manager_black_box_get_exception(struct file *filep, unsigned int cmd, unsigned long arg)
{
    struct devdrv_black_box_user black_box_user = {0};
    unsigned long flags;
    int ret;

    ret = devdrv_manager_container_is_in_container();
    if (ret) {
        devdrv_drv_err("not support using in container, ret(%d).\n", ret);
        return -EINVAL;
    }

    if (dev_manager_info == NULL) {
        devdrv_drv_err("device does not exist.\n");
        return -EINVAL;
    }

    spin_lock_irqsave(&dev_manager_info->black_box.spinlock, flags);
    if (dev_manager_info->black_box.black_box_pid && (dev_manager_info->black_box.black_box_pid != current->tgid)) {
        spin_unlock_irqrestore(&dev_manager_info->black_box.spinlock, flags);
        devdrv_drv_err("already have a black box thread.\n");
        return -EINVAL;
    }
    dev_manager_info->black_box.black_box_pid = current->tgid;

    if (dev_manager_info->black_box.exception_num > 0) {
        devdrv_drv_info("black box exception_num :%d\n", dev_manager_info->black_box.exception_num);
        spin_unlock_irqrestore(&dev_manager_info->black_box.spinlock, flags);
        goto no_wait_black_box_sema;
    }
    spin_unlock_irqrestore(&dev_manager_info->black_box.spinlock, flags);

    ret = down_interruptible(&dev_manager_info->black_box.black_box_sema);
    if (ret == -EINTR) {
        devdrv_drv_info("interrupted. ret(%d)\n", ret);
        return 0;
    }
    if (ret) {
        devdrv_drv_err("down_interrupitable fail. ret(%d)\n", ret);
        return ret;
    }

no_wait_black_box_sema:
    devdrv_host_black_box_get_exception(&black_box_user);
    ret = copy_to_user_safe((void *)((uintptr_t)arg), &black_box_user, sizeof(struct devdrv_black_box_user));
    if (ret) {
        devdrv_drv_err("copy_to_user_safe fail.\n");
        return ret;
    }

    return 0;
}

STATIC int devdrv_manager_device_memory_dump(struct file *filep, unsigned int cmd, unsigned long arg)
{
    struct devdrv_black_box_user black_box_user = {0};
    struct devdrv_info *dev_info = NULL;
    dma_addr_t host_addr_dma = 0;
    void *buffer = NULL;
    int dev_id;
    u32 phy_id;
    int ret;
    ret = copy_from_user_safe(&black_box_user, (void *)((uintptr_t)arg), sizeof(struct devdrv_black_box_user));
    if (ret) {
        devdrv_drv_err("copy_from_user_safe fail, ret(%d).\n", ret);
        return ret;
    }
    phy_id = black_box_user.devid;

    if (devdrv_manager_container_logical_id_to_physical_id(phy_id, &black_box_user.devid)) {
        devdrv_drv_err("can't transfor virt id %u \n", phy_id);
        return -EINVAL;
    }

    if (black_box_user.dst_buffer == NULL) {
        devdrv_drv_err("invalid buffer. dev_id(%u)\n", black_box_user.devid);
        return -EFAULT;
    }
    if ((black_box_user.size <= 0) || (black_box_user.size > DEVDRV_MAX_MEMORY_DUMP_SIZE)) {
        devdrv_drv_err("invalid size. dev_id(%u)\n", black_box_user.devid);
        return -EINVAL;
    }

    if ((black_box_user.addr_offset >= DEVDRV_MEMDUMP_SIZE) ||
        (black_box_user.addr_offset + black_box_user.size > DEVDRV_MEMDUMP_SIZE)) {
        devdrv_drv_err("invalid phy offset addr. dev_id(%u), offset addr(%lld), size(%u)\n", black_box_user.devid,
                       black_box_user.addr_offset, black_box_user.size);
        return -EFAULT;
    }

    dev_id = devdrv_get_device_index(black_box_user.devid);
    if (dev_id < 0 || dev_id >= DEVDRV_MAX_DAVINCI_NUM) {
        devdrv_drv_err("get device(%u) index(%d) fail.\n", black_box_user.devid, dev_id);
        return -EINVAL;
    }

    black_box_user.addr_offset += (DEVDRV_MEMDUMP_ADDR + dev_id * CHIP_BASEADDR_PA_OFFSET);

    dev_info = devdrv_get_devdrv_info_array(black_box_user.devid);
    if (dev_info == NULL) {
        devdrv_drv_err("no device(%u).\n", black_box_user.devid);
        return -ENODEV;
    }

    atomic_inc(&dev_info->occupy_ref);
    if (dev_info->status == 1) {
        atomic_dec(&dev_info->occupy_ref);
        devdrv_drv_warn("dev %d has been reset\n", dev_info->dev_id);
        return -EINVAL;
    }

    buffer = dma_alloc_coherent(dev_info->dev, black_box_user.size, &host_addr_dma, GFP_KERNEL);
    if (buffer == NULL) {
        atomic_dec(&dev_info->occupy_ref);
        devdrv_drv_err("dma_alloc_coherent fail devid(%u), size(%d).\n", black_box_user.devid, black_box_user.size);
        return -ENOMEM;
    }
    devdrv_drv_debug("devid: %u, len: %d.\n", black_box_user.devid, black_box_user.size);

    ret = devdrv_dma_sync_copy(dev_info->pci_dev_id, DEVDRV_DMA_DATA_COMMON, (u64)black_box_user.addr_offset,
                               (u64)host_addr_dma, black_box_user.size, DEVDRV_DMA_DEVICE_TO_HOST);
    if (ret) {
        devdrv_drv_err("devdrv_dma_sync_copy fail, ret(%d). dev_id(%u)\n", ret, black_box_user.devid);
        ret = -1;
        goto free_alloc;
    }

    ret = copy_to_user_safe(black_box_user.dst_buffer, buffer, black_box_user.size);
    if (ret) {
        devdrv_drv_err("copy_to_user_safe fail, ret(%d). dev_id(%u)\n", ret, black_box_user.devid);
        ret = -1;
        goto free_alloc;
    }
free_alloc:
    atomic_dec(&dev_info->occupy_ref);
    dma_free_coherent(dev_info->dev, black_box_user.size, buffer, host_addr_dma);
    buffer = NULL;
    return ret;
}

STATIC int devdrv_manager_ioctl_get_devnum(struct file *filep, unsigned int cmd, unsigned long arg)
{
    u32 devnum;

    devnum = devdrv_manager_get_devnum();

    return copy_to_user_safe((void *)((uintptr_t)arg), &devnum, sizeof(u32));
}

STATIC int devdrv_manager_ioctl_get_plat_info(struct file *filep, unsigned int cmd, unsigned long arg)
{
    u32 plat_info;

    plat_info = dev_manager_info->plat_info;
    return copy_to_user_safe((void *)((uintptr_t)arg), &plat_info, sizeof(u32));
}

/* This interface does not support using in containers */
STATIC int devdrv_manager_device_reset_inform(struct file *filep, unsigned int cmd, unsigned long arg)
{
    struct timespec stamp;
    u32 devid;
    int ret;
    int virt_id;

    ret = devdrv_manager_container_is_in_container();
    if (ret) {
        devdrv_drv_err("not support using in container, ret(%d).\n", ret);
        return -EINVAL;
    }

    ret = copy_from_user_safe(&devid, (void *)((uintptr_t)arg), sizeof(u32));
    if (ret) {
        devdrv_drv_err("copy_from_user_safe failed, ret(%d).\n", ret);
        return ret;
    }

    virt_id = devid;

    if (devdrv_manager_container_logical_id_to_physical_id(virt_id, &devid)) {
        devdrv_drv_err("can't transfor virt id %u \n", virt_id);
        return -EINVAL;
    }

    stamp = current_kernel_time();

    ret = devdrv_host_black_box_add_exception(devid, DEVDRV_BB_DEVICE_RESET_INFORM, stamp, NULL);
    if (ret) {
        devdrv_drv_err("devdrv_host_black_box_add_exception failed, ret(%d). dev_id(%u)\n", ret, devid);
        return ret;
    }

    return 0;
}

#if LINUX_VERSION_CODE < KERNEL_VERSION(4, 14, 0)
ssize_t devdrv_load_file_read(struct file *file, loff_t *pos, char *addr, size_t count)
{
    char __user *buf = (char __user *)addr;
    mm_segment_t old_fs;
    ssize_t len;

    old_fs = get_fs();
    set_fs(get_ds()); /*lint !e501 */ /* kernel source */
    len = vfs_read(file, buf, count, pos);
    set_fs(old_fs);

    return len;
}
#endif

STATIC int devdrv_manager_get_container_flag(struct file *filep, unsigned int cmd, unsigned long arg)
{
    unsigned int flag;
    int ret;

    flag = (unsigned int)devdrv_manager_container_is_in_container();

    ret = copy_to_user_safe((void *)((uintptr_t)arg), &flag, sizeof(unsigned int));

    return ret;
}

STATIC int devdrv_manager_dfx_cmd(struct file *filep, unsigned int cmd, unsigned long arg)
{
    return devdrv_manager_dfx_process(arg);
}

STATIC int devdrv_manager_container_cmd(struct file *filep, unsigned int cmd, unsigned long arg)
{
    return devdrv_manager_container_process(filep, arg);
}

STATIC int devdrv_manager_get_probe_num(struct file *filep, unsigned int cmd, unsigned long arg)
{
    u32 num;
    int ret;
    unsigned long flags;

    spin_lock_irqsave(&dev_manager_info->spinlock, flags);
    num = dev_manager_info->prob_num;
    spin_unlock_irqrestore(&dev_manager_info->spinlock, flags);

    ret = copy_to_user_safe((void *)((uintptr_t)arg), &num, sizeof(u32));
    if (ret) {
        devdrv_drv_err("copy to user failed, ret(%d).\n", ret);
        return ret;
    }

    return 0;
}

STATIC int devdrv_creat_random_sign(char *random_sign, u32 len)
{
    char random[RANDOM_SIZE] = {0};
    int offset = 0;
    int ret;
    int i;

    ret = devdrv_get_random(random, RANDOM_SIZE);
    if (ret) {
        devdrv_drv_err("get random failed, ret(%d).\n", ret);
        return ret;
    }
    for (i = 0; i < RANDOM_SIZE; i++) {
        ret = snprintf_s(random_sign + offset, len - offset, len - 1 - offset, "%02x", (u8)random[i]);
        if (ret < 0) {
            (void)memset_s(random, RANDOM_SIZE, 0, RANDOM_SIZE);
            devdrv_drv_err("snprintf failed, ret(%d).\n", ret);
            return -EINVAL;
        }
        offset += ret;
    }
    random_sign[len - 1] = '\0';
    (void)memset_s(random, RANDOM_SIZE, 0, RANDOM_SIZE);

    return 0;
}

STATIC int devdrv_get_process_sign(struct devdrv_manager_info *d_info, char *sign, u32 len, u32 docker_id)
{
    struct devdrv_process_sign *d_sign = NULL;
    struct list_head *pos = NULL;
    struct list_head *n = NULL;
    int ret;

    if (!list_empty_careful(&d_info->hostpid_list_header)) {
        list_for_each_safe(pos, n, &d_info->hostpid_list_header) {
            d_sign = list_entry(pos, struct devdrv_process_sign, list);
            if (d_sign->hostpid == current->tgid) {
                ret = strcpy_s(sign, len, d_sign->sign);
                if (ret) {
                    devdrv_drv_err("strcpy_s failed, ret(%d).\n", ret);
                    return -EINVAL;
                }
                return 0;
            }
        }
    }

    if (d_info->devdrv_sign_count[docker_id] >= DEVDRV_MAX_SIGN_NUM) {
        devdrv_drv_err("list add failed, docker_id(%u).\n", docker_id);
        return -EINVAL;
    }

    d_sign = kzalloc(sizeof(struct devdrv_process_sign), GFP_KERNEL);
    if (d_sign == NULL) {
        devdrv_drv_err("kzalloc failed, docker_id(%u).\n", docker_id);
        return -ENOMEM;
    }
    d_sign->hostpid = current->tgid;
    d_sign->docker_id = docker_id;

    ret = devdrv_creat_random_sign(d_sign->sign, PROCESS_SIGN_LENGTH);
    if (ret) {
        devdrv_drv_err("get sign failed, ret(%d), docker_id(%u).\n", ret, docker_id);
        kfree(d_sign);
        d_sign = NULL;
        return ret;
    }
    ret = strcpy_s(sign, len, d_sign->sign);
    if (ret) {
        devdrv_drv_err("strcpy_s failed, ret(%d), docker_id(%u).\n", ret, docker_id);
        kfree(d_sign);
        d_sign = NULL;
        return -EINVAL;
    }
    list_add(&d_sign->list, &d_info->hostpid_list_header);
    d_info->devdrv_sign_count[d_sign->docker_id]++;

    return 0;
}

STATIC int devdrv_manager_get_process_sign(struct file *filep, unsigned int cmd, unsigned long arg)
{
    struct devdrv_manager_info *d_info = devdrv_get_manager_info();
    u32 docker_id = MAX_DOCKER_NUM;
    struct process_sign dev_sign = {0};
    int ret;

    if (d_info == NULL) {
        devdrv_drv_err("d_info is null.\n");
        return -EINVAL;
    }

    /*
     * check current environment :
     * non-container : docker_id is set to 69;
     * container : get docker_id , docker_id is 0 ~ 68.
     */
    ret = devdrv_manager_container_is_in_container();
    if (ret) {
        ret = devdrv_manager_container_get_docker_id(&docker_id);
        if (ret) {
            devdrv_drv_err("container get docker_id failed, ret(%d).\n", ret);
            return -EINVAL;
        }
    }

    down(&d_info->devdrv_sign_list_sema);
    ret = devdrv_get_process_sign(d_info, dev_sign.sign, PROCESS_SIGN_LENGTH, docker_id);
    if (ret) {
        up(&d_info->devdrv_sign_list_sema);
        devdrv_drv_err("get process_sign failed, ret(%d).\n", ret);
        return ret;
    }
    up(&d_info->devdrv_sign_list_sema);

    dev_sign.tgid = current->tgid;

    ret = copy_to_user_safe((void *)((uintptr_t)arg), &dev_sign, sizeof(struct process_sign));
    if (ret) {
        devdrv_drv_err("copy to user failed, ret(%d).\n", ret);
        return ret;
    }
    (void)memset_s(dev_sign.sign, PROCESS_SIGN_LENGTH, 0, PROCESS_SIGN_LENGTH);

    return 0;
}

STATIC int (*const devdrv_manager_ioctl_handlers[DEVDRV_MANAGER_CMD_MAX_NR])(struct file *filep, unsigned int cmd,
    unsigned long arg) = {
        [_IOC_NR(DEVDRV_MANAGER_GET_PCIINFO)] = devdrv_manager_get_pci_info,
        [_IOC_NR(DEVDRV_MANAGER_GET_DEVNUM)] = devdrv_manager_ioctl_get_devnum,
        [_IOC_NR(DEVDRV_MANAGER_GET_PLATINFO)] = devdrv_manager_ioctl_get_plat_info,
        [_IOC_NR(DEVDRV_MANAGER_SVMVA_TO_DEVID)] = devdrv_manager_svmva_to_devid,
        [_IOC_NR(DEVDRV_MANAGER_DEVICE_STATUS)] = devdrv_manager_get_device_status,
        [_IOC_NR(DEVDRV_MANAGER_GET_CORE_SPEC)] = devdrv_manager_get_core,
        [_IOC_NR(DEVDRV_MANAGER_GET_CORE_INUSE)] = devdrv_manager_get_core,
        [_IOC_NR(DEVDRV_MANAGER_GET_DEVIDS)] = devdrv_manager_devinfo_ioctl,
        [_IOC_NR(DEVDRV_MANAGER_GET_CONTAINER_DEVIDS)] = devdrv_manager_devinfo_ioctl,
        [_IOC_NR(DEVDRV_MANAGER_GET_DEVINFO)] = devdrv_manager_devinfo_ioctl,
        [_IOC_NR(DEVDRV_MANAGER_GET_DEVID_BY_LOCALDEVID)] = NULL,
        [_IOC_NR(DEVDRV_MANAGER_GET_PCIE_ID_INFO)] = NULL,
        [_IOC_NR(DEVDRV_MANAGER_GET_VOLTAGE)] = NULL,
        [_IOC_NR(DEVDRV_MANAGER_GET_TEMPERATURE)] = NULL,
        [_IOC_NR(DEVDRV_MANAGER_GET_TSENSOR)] = NULL,
        [_IOC_NR(DEVDRV_MANAGER_GET_AI_USE_RATE)] = NULL,
        [_IOC_NR(DEVDRV_MANAGER_GET_FREQUENCY)] = NULL,
        [_IOC_NR(DEVDRV_MANAGER_GET_POWER)] = NULL,
        [_IOC_NR(DEVDRV_MANAGER_GET_HEALTH_CODE)] = NULL,
        [_IOC_NR(DEVDRV_MANAGER_GET_ERROR_CODE)] = NULL,
        [_IOC_NR(DEVDRV_MANAGER_GET_DDR_CAPACITY)] = NULL,
        [_IOC_NR(DEVDRV_MANAGER_LPM3_SMOKE)] = NULL,
        [_IOC_NR(DEVDRV_MANAGER_BLACK_BOX_GET_EXCEPTION)] = devdrv_manager_black_box_get_exception,
        [_IOC_NR(DEVDRV_MANAGER_DEVICE_MEMORY_DUMP)] = devdrv_manager_device_memory_dump,
        [_IOC_NR(DEVDRV_MANAGER_DEVICE_RESET_INFORM)] = devdrv_manager_device_reset_inform,
        [_IOC_NR(DEVDRV_MANAGER_GET_MODULE_STATUS)] = NULL,
        [_IOC_NR(DEVDRV_MANAGER_GET_MINI_BOARD_ID)] = NULL,
        [_IOC_NR(DEVDRV_MANAGER_PCIE_PRE_RESET)] = devdrv_manager_pcie_pre_reset,
        [_IOC_NR(DEVDRV_MANAGER_PCIE_RESCAN)] = devdrv_manager_pcie_rescan,
        [_IOC_NR(DEVDRV_MANAGER_PCIE_HOT_RESET)] = devdrv_manager_pcie_hot_reset,
        [_IOC_NR(DEVDRV_MANAGER_P2P_ATTR)] = devdrv_manager_p2p_attr_op,
        [_IOC_NR(DEVDRV_MANAGER_ALLOC_HOST_DMA_ADDR)] = drv_dma_mmap,
        [_IOC_NR(DEVDRV_MANAGER_PCIE_SRAM_READ)] = drv_pcie_sram_read,
        [_IOC_NR(DEVDRV_MANAGER_PCIE_SRAM_WRITE)] = drv_pcie_sram_write,
        [_IOC_NR(DEVDRV_MANAGER_GET_USER_CONFIG)] = drv_get_user_config,
        [_IOC_NR(DEVDRV_MANAGER_GET_EMMC_VOLTAGE)] = NULL,
        [_IOC_NR(DEVDRV_MANAGER_GET_DEVICE_BOOT_STATUS)] = drv_get_device_boot_status,
        [_IOC_NR(DEVDRV_MANAGER_ENABLE_EFUSE_LDO)] = NULL,
        [_IOC_NR(DEVDRV_MANAGER_DISABLE_EFUSE_LDO)] = NULL,
        [_IOC_NR(DEVDRV_MANAGER_DFX_CMD)] = devdrv_manager_dfx_cmd,
        [_IOC_NR(DEVDRV_MANAGER_CONTAINER_CMD)] = devdrv_manager_container_cmd,
        [_IOC_NR(DEVDRV_MANAGER_GET_HOST_PHY_MACH_FLAG)] = devdrv_manager_get_host_phy_mach_flag,
        [_IOC_NR(DEVDRV_MANAGER_GET_LOCAL_DEVICEIDS)] = NULL,
        [_IOC_NR(DEVDRV_MANAGER_IMU_SMOKE)] = NULL,
        [_IOC_NR(DEVDRV_MANAGER_SET_NEW_TIME)] = NULL,
        [_IOC_NR(DEVDRV_MANAGER_IPC_NOTIFY_CREATE)] = devdrv_manager_ipc_notify_ioctl,
        [_IOC_NR(DEVDRV_MANAGER_IPC_NOTIFY_OPEN)] = devdrv_manager_ipc_notify_ioctl,
        [_IOC_NR(DEVDRV_MANAGER_IPC_NOTIFY_CLOSE)] = devdrv_manager_ipc_notify_ioctl,
        [_IOC_NR(DEVDRV_MANAGER_IPC_NOTIFY_DESTROY)] = devdrv_manager_ipc_notify_ioctl,
        [_IOC_NR(DEVDRV_MANAGER_IPC_NOTIFY_SET_PID)] = devdrv_manager_ipc_notify_ioctl,
        [_IOC_NR(DEVDRV_MANAGER_PCIE_DDR_READ)] = drv_pcie_bbox_ddr_read,
        [_IOC_NR(DEVDRV_MANAGER_GET_TRANSWAY)] = devdrv_manager_get_transway,
        [_IOC_NR(DEVDRV_MANAGER_GET_CPU_INFO)] = NULL,
        [_IOC_NR(DEVDRV_MANAGER_SEND_TO_IMU)] = NULL,
        [_IOC_NR(DEVDRV_MANAGER_RECV_FROM_IMU)] = NULL,
        [_IOC_NR(DEVDRV_MANAGER_GET_IMU_INFO)] = NULL,
        [_IOC_NR(DEVDRV_MANAGER_GET_ECC_STATICS)] = NULL,
        [_IOC_NR(DEVDRV_MANAGER_CONFIG_ECC_ENABLE)] = NULL,
        [_IOC_NR(DEVDRV_MANAGER_GET_PROBE_NUM)] = devdrv_manager_get_probe_num,
        [_IOC_NR(DEVDRV_MANAGER_DEBUG_INFORM)] = NULL,
        [_IOC_NR(DEVDRV_MANAGER_COMPUTE_POWER)] = NULL,
        [_IOC_NR(DEVDRV_MANAGER_SYNC_MATRIX_DAEMON_READY)] = NULL,
        [_IOC_NR(DEVDRV_MANAGER_GET_BBOX_ERRSTR)] = NULL,
        [_IOC_NR(DEVDRV_MANAGER_PCIE_IMU_DDR_READ)] = drv_pcie_bbox_imu_ddr_read,
        [_IOC_NR(DEVDRV_MANAGER_GET_SLOT_ID)] = NULL,
        [_IOC_NR(DEVDRV_MANAGER_APPMON_BBOX_EXCEPTION_CMD)] = NULL,
        [_IOC_NR(DEVDRV_MANAGER_GET_CONTAINER_FLAG)] = devdrv_manager_get_container_flag,
        [_IOC_NR(DEVDRV_MANAGER_GET_PROCESS_SIGN)] = devdrv_manager_get_process_sign,
        [_IOC_NR(DEVDRV_MANAGER_GET_MASTER_DEV_IN_THE_SAME_OS)] = devdrv_manager_get_master_device_in_the_same_os,
        [_IOC_NR(DEVDRV_MANAGER_GET_LOCAL_DEV_ID_BY_HOST_DEV_ID)] = NULL,
        [_IOC_NR(DEVDRV_MANAGER_GET_BOOT_DEV_ID)] = devdrv_manager_online_get_devids,
        [_IOC_NR(DEVDRV_MANAGER_PASSTHRU_MCU)] = NULL,
};

STATIC long devdrv_manager_ioctl(struct file *filep, unsigned int cmd, unsigned long arg)
{
    if (dev_manager_info == NULL) {
        devdrv_drv_err("invalid parameter, "
                       "dev_manager_info = %pK, "
                       "arg = 0x%lx\n",
                       dev_manager_info, arg);
        return -EINVAL;
    }

    if (_IOC_NR(cmd) >= DEVDRV_MANAGER_CMD_MAX_NR) {
        devdrv_drv_err("cmd out of range, cmd = %u\n", _IOC_NR(cmd));
        return -EINVAL;
    }

    if (devdrv_manager_ioctl_handlers[_IOC_NR(cmd)] == NULL) {
        devdrv_drv_err("invalid cmd, cmd = %u\n", _IOC_NR(cmd));
        return -EINVAL;
    }

    return devdrv_manager_ioctl_handlers[_IOC_NR(cmd)](filep, cmd, arg);
}

STATIC int devdrv_manager_ipc_notify_ioctl(struct file *filep, unsigned int cmd, unsigned long arg)
{
    struct devdrv_notify_ioctl_info notify_ioctl_info;
    struct devdrv_manager_context *context = NULL;
    int ret;

    context = filep->private_data;
    if ((context == NULL) || (context->ipc_notify_info == NULL)) {
        devdrv_drv_err("context(%pK) or context->ipc_notify_info is NULL\n", context);
        return -ENODEV;
    }

    ret = copy_from_user_safe(&notify_ioctl_info, (void *)((uintptr_t)arg),
        sizeof(struct devdrv_notify_ioctl_info));
    if (ret) {
        devdrv_drv_err("copy from user failed, ret(%d).\n", ret);
        return -EFAULT;
    }
    notify_ioctl_info.name[DEVDRV_IPC_NAME_SIZE - 1] = '\0';

    switch (cmd) {
        case DEVDRV_MANAGER_IPC_NOTIFY_CREATE:
            ret = devdrv_manager_ipc_notify_create(context, arg, &notify_ioctl_info);
            break;
        case DEVDRV_MANAGER_IPC_NOTIFY_OPEN:
            ret = devdrv_manager_ipc_notify_open(context, arg, &notify_ioctl_info);
            break;
        case DEVDRV_MANAGER_IPC_NOTIFY_CLOSE:
            ret = devdrv_manager_ipc_notify_close(context, &notify_ioctl_info);
            break;
        case DEVDRV_MANAGER_IPC_NOTIFY_DESTROY:
            ret = devdrv_manager_ipc_notify_destroy(context, &notify_ioctl_info);
            break;
        case DEVDRV_MANAGER_IPC_NOTIFY_SET_PID:
            ret = devdrv_manager_ipc_notify_set_pid(context, &notify_ioctl_info);
            break;

        default:
            devdrv_drv_err("invalid cmd, cmd = %d\n", _IOC_NR(cmd));
            ret = -EFAULT;
            break;
    };
    (void)memset_s(notify_ioctl_info.name, DEVDRV_IPC_NAME_SIZE, 0, DEVDRV_IPC_NAME_SIZE);

    return ret;
}

STATIC ssize_t devdrv_manager_read(struct file *filep, char __user *buf, size_t count, loff_t *ppos)
{
    return 0;
}

const struct file_operations devdrv_manager_file_operations = {
    .owner = THIS_MODULE,
    .read = devdrv_manager_read,
    .open = devdrv_manager_open,
    .release = devdrv_manager_release,
    .unlocked_ioctl = devdrv_manager_ioctl,
    .poll = devdrv_manager_poll,
};

STATIC int devdrv_manager_get_dev_resource(struct devdrv_info *dev_info)
{
    struct devdrv_platform_data *pdata = NULL;
    struct devdrv_pci_dev_info dev_pci_info;
    u32 dev_id = dev_info->pci_dev_id;
    u32 irq;
    u32 i;
    u32 tsid = 0;

    pdata = dev_info->pdata;
    if (devdrv_get_pci_dev_info(dev_id, &dev_pci_info)) {
        devdrv_drv_err("devdrv_get_pci_dev_info failed. dev_id(%u)\n", dev_id);
        return -EBUSY;
    }
    pdata->pci_info.bus_number = dev_pci_info.bus_no;
    pdata->pci_info.dev_number = dev_pci_info.device_no;
    pdata->pci_info.function_number = dev_pci_info.function_no;

    for (i = 0; i < DEVDRV_CQ_UPDATE_IRQ_SUM; i++) {
        if (devdrv_get_irq_no(dev_id, i + DEVDRV_IRQ_NUM, &irq)) {
            devdrv_drv_err("get irq failed, index = %d. dev_id(%u)\n", i + DEVDRV_IRQ_NUM, dev_id);
            return -EBUSY;
        }
        pdata->ts_pdata[tsid].irq_cq_update[i] = irq;
    }
    if (devdrv_get_irq_no(dev_id, i + DEVDRV_IRQ_NUM, &irq)) {
        devdrv_drv_err("get irq failed, index = %d. dev_id(%u)\n", i + DEVDRV_IRQ_NUM, dev_id);
        return -EBUSY;
    }
    pdata->ts_pdata[tsid].irq_functional_cq = irq;

    if (devdrv_get_irq_no(dev_id, i + DEVDRV_IRQ_NUM_PLUS, &irq)) {
        devdrv_drv_err("get irq failed, index = %d. dev_id(%u)\n", i + DEVDRV_IRQ_NUM_PLUS, dev_id);
        return -EBUSY;
    }
    pdata->ts_pdata[tsid].irq_mailbox_ack = irq;

    if (devdrv_get_addr_info(dev_id, DEVDRV_ADDR_TS_DOORBELL, 0, &pdata->ts_pdata[tsid].doorbell_paddr,
                             (size_t *)&pdata->ts_pdata[tsid].doorbell_size)) {
        devdrv_drv_err("get doorbell addr falied. dev_id(%u)\n", dev_id);
        return -EBUSY;
    }

    if (devdrv_get_addr_info(dev_id, DEVDRV_ADDR_TS_SRAM, 0, &pdata->ts_pdata[tsid].sram_paddr,
                             (size_t *)&pdata->ts_pdata[tsid].sram_size)) {
        devdrv_drv_err("get mailbox sram addr falied. dev_id(%u)\n", dev_id);
        return -EBUSY;
    }
    if (devdrv_get_addr_info(dev_id, DEVDRV_ADDR_TS_SQ_BASE, 0, &pdata->ts_pdata[tsid].ts_sq_static_addr,
                             (size_t *)&pdata->ts_pdata[tsid].ts_sq_static_size)) {
        devdrv_drv_err("get mailbox sram addr falied. dev_id(%u)\n", dev_id);
        return -EBUSY;
    }

    pdata->ts_pdata[tsid].doorbell_vaddr = devm_ioremap(dev_info->dev, pdata->ts_pdata[tsid].doorbell_paddr,
                                                        pdata->ts_pdata[tsid].doorbell_size);
    if (pdata->ts_pdata[tsid].doorbell_vaddr == NULL)
        return -ENOMEM;

    pdata->ts_pdata[tsid].ts_mbox_send_paddr = pdata->ts_pdata[tsid].sram_paddr;
    pdata->ts_pdata[tsid].ts_mbox_send_size = DEVDRV_MAILBOX_PAYLOAD_LENGTH;

    pdata->ts_pdata[tsid].ts_mbox_send_vaddr = devm_ioremap(dev_info->dev, pdata->ts_pdata[tsid].ts_mbox_send_paddr,
                                                            pdata->ts_pdata[tsid].ts_mbox_send_size);
    if (pdata->ts_pdata[tsid].ts_mbox_send_vaddr == NULL) {
        devdrv_drv_err("ts mbox send ioremap failed\n");
        devm_iounmap(dev_info->dev, pdata->ts_pdata[tsid].doorbell_vaddr);
        return -ENOMEM;
    }

    pdata->ts_pdata[tsid].ts_mbox_rcv_paddr = pdata->ts_pdata[tsid].sram_paddr + DEVDRV_MAILBOX_PAYLOAD_LENGTH;
    pdata->ts_pdata[tsid].ts_mbox_rcv_size = DEVDRV_MAILBOX_PAYLOAD_LENGTH;
    pdata->ts_pdata[tsid].ts_mbox_rcv_vaddr = devm_ioremap(dev_info->dev, pdata->ts_pdata[tsid].ts_mbox_rcv_paddr,
                                                           pdata->ts_pdata[tsid].ts_mbox_rcv_size);
    if (pdata->ts_pdata[tsid].ts_mbox_rcv_vaddr == NULL) {
        devdrv_drv_err("ts mbox send ioremap failed\n");
        devm_iounmap(dev_info->dev, pdata->ts_pdata[tsid].doorbell_vaddr);
        devm_iounmap(dev_info->dev, pdata->ts_pdata[tsid].ts_mbox_send_vaddr);
        return -ENOMEM;
    }

    devdrv_drv_debug("device(%u) ts doorbell addr.\n", dev_id);
    devdrv_drv_debug("ts_sq_static_addr, size: %d.\n", (int)pdata->ts_pdata[tsid].ts_sq_static_size);

    return 0;
}

int devdrv_manager_send_msg(struct devdrv_info *dev_info, struct devdrv_manager_msg_info *dev_manager_msg_info,
                            int *out_len)
{
    int ret;

    ret = devdrv_common_msg_send(dev_info->pci_dev_id, dev_manager_msg_info, sizeof(struct devdrv_manager_msg_info),
                                 sizeof(struct devdrv_manager_msg_info), (u32 *)out_len,
                                 DEVDRV_COMMON_MSG_DEVDRV_MANAGER);

    return ret;
}

STATIC int devdrv_manager_send_devid(struct devdrv_info *dev_info)
{
    struct devdrv_manager_msg_info dev_manager_msg_info;
    u32 out_len;
    int ret;
    u32 i;

    dev_manager_msg_info.header.msg_id = DEVDRV_MANAGER_CHAN_H2D_SEND_DEVID;
    dev_manager_msg_info.header.valid = DEVDRV_MANAGER_MSG_VALID;

    /* give a random value for checking result later */
    dev_manager_msg_info.header.result = (u16)DEVDRV_MANAGER_MSG_INVALID_RESULT;

    /* inform corresponding devid to device side */
    dev_manager_msg_info.header.dev_id = dev_info->dev_id;

    for (i = 0; i < DEVDRV_MANAGER_INFO_PAYLOAD_LEN; i++)
        dev_manager_msg_info.payload[i] = 0;

    ret = devdrv_common_msg_send(dev_info->pci_dev_id, &dev_manager_msg_info, sizeof(dev_manager_msg_info),
                                 sizeof(dev_manager_msg_info), &out_len, DEVDRV_COMMON_MSG_DEVDRV_MANAGER);
    if (ret || (dev_manager_msg_info.header.result != 0)) {
        devdrv_drv_err("send dev_id(%u) to device(%u) failed, ret(%d)\n", dev_info->dev_id, dev_info->pci_dev_id, ret);
        return -EFAULT;
    }

    return 0;
}

u32 devdrv_manager_get_ts_num(struct devdrv_info *dev_info)
{
    if (dev_info == NULL) {
        devdrv_drv_err("invalid input handler.\n");
        return (u32)-1;
    }
    if (dev_info->pdata == NULL) {
        devdrv_drv_err("invalid input handler.\n");
        return (u32)-1;
    }

    if (dev_info->pdata->ts_num > DEVDRV_MAX_TS_NUM) {
        devdrv_drv_err("ts_num(%u).\n", dev_info->pdata->ts_num);
        return (u32)-1;
    }
    dev_info->pdata->ts_num = 1;

    return dev_info->pdata->ts_num;
}
EXPORT_SYMBOL(devdrv_manager_get_ts_num);

STATIC void devdrv_manager_get_max_id_num(struct devdrv_info *dev_info)
{
    struct devdrv_platform_data *pdata = dev_info->pdata;
    struct devdrv_id_data *id_data = &pdata->ts_pdata[0].id_data;
    if (devdrv_manager_client_info_registered() != 0) {
        /* devdrv_driver is not registered,
            no resource ids assigned by default */
        id_data->min_stream_id = 0;
        id_data->max_stream_id = 0;
        id_data->min_event_id = 0;
        id_data->max_event_id = 0;
        id_data->min_sq_id = 0;
        id_data->max_sq_id = 0;
        id_data->min_cq_id = 0;
        id_data->max_cq_id = 0;
        id_data->min_model_id = 0;
        id_data->max_model_id = 0;
        id_data->min_notify_id = 0;
        id_data->max_notify_id = 0;
        id_data->min_task_id = 0;
        id_data->max_task_id = 0;
    }

    id_data->cur_cq_num = id_data->max_cq_id - id_data->min_cq_id;
    id_data->cur_sq_num = id_data->max_sq_id - id_data->min_sq_id;
    id_data->cur_stream_num = id_data->max_stream_id - id_data->min_stream_id;
    id_data->cur_event_num = id_data->max_event_id - id_data->min_event_id;
    id_data->cur_model_num = id_data->max_model_id - id_data->min_model_id;
    id_data->cur_notify_num = id_data->max_notify_id - id_data->min_notify_id;
    id_data->cur_task_num = id_data->max_task_id - id_data->min_task_id;
}

int devdrv_manager_register(struct devdrv_info *dev_info)
{
    u32 tsid = 0;
    int ret;

    if (dev_info == NULL) {
        devdrv_drv_err("devdrv manager has not initialized\n");
        return -EINVAL;
    }

    if (devdrv_manager_get_devdrv_info(dev_info->dev_id) != NULL) {
        devdrv_drv_err("device(%u) has already registered\n", dev_info->dev_id);
        return -ENODEV;
    }

    if (devdrv_manager_get_dev_resource(dev_info)) {
        devdrv_drv_err("devdrv manager get device(%u) resource failed\n", dev_info->dev_id);
        return -EINVAL;
    }
    dev_info->drv_ops = &devdrv_host_drv_ops;

    if (devdrv_manager_set_devinfo_inc_devnum(dev_info->dev_id, dev_info)) {
        devdrv_drv_err("devdrv_manager_set_devinfo_inc_devnum failed\n");
        goto devinfo_iounmap;
    }

    devdrv_manager_get_max_id_num(dev_info);

    ret = devdrv_manager_run_register_client(dev_info);
    if (ret) {
        devdrv_drv_err("driver register device(%u) failed.\n", dev_info->dev_id);
        goto devinfo_dec_devnum;
    }
    if (devdrv_manager_send_devid(dev_info)) {
        devdrv_drv_err("send devid to device(%u) failed.\n", dev_info->dev_id);
        goto devinfo_unregister_client;
    }

    (void)devdrv_set_module_init_finish(dev_info->dev_id, DEVDRV_HOST_MODULE_DEVMNG);

    return 0;

devinfo_unregister_client:
    devdrv_manager_run_unregister_client(dev_info);
devinfo_dec_devnum:
    (void)devdrv_manager_reset_devinfo_dec_devnum(dev_info->dev_id);
devinfo_iounmap:
    devm_iounmap(dev_info->dev, dev_info->pdata->ts_pdata[tsid].doorbell_vaddr);
    dev_info->pdata->ts_pdata[tsid].doorbell_vaddr = NULL;
    devm_iounmap(dev_info->dev, dev_info->pdata->ts_pdata[tsid].ts_mbox_send_vaddr);
    dev_info->pdata->ts_pdata[tsid].ts_mbox_send_vaddr = NULL;
    devm_iounmap(dev_info->dev, dev_info->pdata->ts_pdata[tsid].ts_mbox_rcv_vaddr);
    dev_info->pdata->ts_pdata[tsid].ts_mbox_rcv_vaddr = NULL;

    return -EINVAL;
}
EXPORT_SYMBOL(devdrv_manager_register);

void devdrv_manager_unregister(struct devdrv_info *dev_info)
{
    u32 tsid = 0;

    devdrv_drv_debug("devdrv_manager_unregister started.\n");

    if ((dev_info == NULL) || (dev_manager_info == NULL) || (dev_info->dev_id >= DEVDRV_MAX_DAVINCI_NUM)) {
        devdrv_drv_err("dev_info(%pK) or dev_manager_info(%pK) is NULL, dev_id(%u)\n", dev_info, dev_manager_info,
                       (dev_info == NULL) ? DEVDRV_MAX_DAVINCI_NUM : dev_info->dev_id);
        return;
    }

    if (dev_manager_info->dev_info[dev_info->dev_id] == NULL) {
        devdrv_drv_err("device(%u) is not initialized\n", dev_info->dev_id);
        return;
    }
    devdrv_manager_run_unregister_client(dev_info);
    devm_iounmap(dev_info->dev, dev_info->pdata->ts_pdata[tsid].doorbell_vaddr);
    dev_info->pdata->ts_pdata[tsid].doorbell_vaddr = NULL;
    devm_iounmap(dev_info->dev, dev_info->pdata->ts_pdata[tsid].ts_mbox_send_vaddr);
    dev_info->pdata->ts_pdata[tsid].ts_mbox_send_vaddr = NULL;
    devm_iounmap(dev_info->dev, dev_info->pdata->ts_pdata[tsid].ts_mbox_rcv_vaddr);
    dev_info->pdata->ts_pdata[tsid].ts_mbox_rcv_vaddr = NULL;

    if (devdrv_manager_reset_devinfo_dec_devnum(dev_info->dev_id)) {
        devdrv_drv_err("devdrv_manager_unregister device(%u) fail !!!!!!!\n", dev_info->dev_id);
        return;
    }

    devdrv_drv_debug("devdrv_manager_unregister device(%u) finished, "
                     "dev_manager_info->num_dev = %d\n",
                     dev_info->dev_id, dev_manager_info->num_dev);
}
EXPORT_SYMBOL(devdrv_manager_unregister);

STATIC char *devdrv_manager_devnode(struct device *dev, umode_t *mode)
{
    if (mode != NULL)
        *mode = 0660; /* mode is set to 0660 */
    return NULL;
}

STATIC int devdrv_manager_device_ready_info(struct devdrv_info *dev_info, struct devdrv_device_info *drv_info)
{
    struct devdrv_platform_data *pdata = dev_info->pdata;
    struct devdrv_id_data *id_data = NULL;
    u32 tsid = 0;
    u32 i;

    id_data = &pdata->ts_pdata[tsid].id_data;
    spin_lock_bh(&dev_info->spinlock);
    dev_info->ai_core_num = drv_info->ai_core_num;
    dev_info->ai_cpu_core_num = drv_info->ai_cpu_core_num;
    dev_info->ctrl_cpu_core_num = drv_info->ctrl_cpu_core_num;
    dev_info->ctrl_cpu_endian_little = drv_info->ctrl_cpu_endian_little;
    dev_info->ctrl_cpu_id = drv_info->ctrl_cpu_id;
    dev_info->ctrl_cpu_ip = drv_info->ctrl_cpu_ip;
    pdata->ts_pdata[tsid].ts_cpu_core_num = drv_info->ts_cpu_core_num;
    dev_info->env_type = drv_info->env_type;
    dev_info->ai_core_id = drv_info->ai_core_id;
    dev_info->ai_cpu_core_id = drv_info->ai_cpu_core_id;

    dev_info->inuse.ai_core_num = drv_info->ai_core_ready_num;
    dev_info->inuse.ai_core_error_bitmap = drv_info->ai_core_broken_map;
    dev_info->inuse.ai_cpu_num = drv_info->ai_cpu_ready_num;
    dev_info->inuse.ai_cpu_error_bitmap = drv_info->ai_cpu_broken_map;
    dev_info->ai_subsys_ip_broken_map = drv_info->ai_subsys_ip_map;
    dev_info->hardware_version = drv_info->hardware_version;

    id_data->min_sq_id = drv_info->min_sq_id;
    id_data->max_sq_id = drv_info->max_sq_id;
    id_data->min_cq_id = drv_info->min_cq_id;
    id_data->max_cq_id = drv_info->max_cq_id;
    id_data->min_stream_id = drv_info->min_stream_id;
    id_data->max_stream_id = drv_info->max_stream_id;
    id_data->min_event_id = drv_info->min_event_id;
    id_data->max_event_id = drv_info->max_event_id;
    id_data->min_model_id = drv_info->min_model_id;
    id_data->max_model_id = drv_info->max_model_id;
    id_data->min_notify_id = drv_info->min_notify_id;
    id_data->max_notify_id = drv_info->max_notify_id;
    id_data->min_task_id = drv_info->min_task_id;
    id_data->max_task_id = drv_info->max_task_id;
    spin_unlock_bh(&dev_info->spinlock);

    devdrv_drv_info("device: %u"
                    "ai cpu num: %d, "
                    "ai cpu broken bitmap: 0x%x, "
                    "ai core num: %d,"
                    "ai core broken bitmap: 0x%x, "
                    "ai subsys broken map: 0x%x.\n",
                    dev_info->dev_id, drv_info->ai_cpu_ready_num, drv_info->ai_cpu_broken_map,
                    drv_info->ai_core_ready_num, drv_info->ai_core_broken_map, drv_info->ai_subsys_ip_map);

    if ((dev_info->ai_cpu_core_num > MAX_BIT_NUM) || (dev_info->ai_core_num > MAX_BIT_NUM)) {
        devdrv_drv_err("invalid num : aicpu %u aicore  %u, \n", dev_info->ai_cpu_core_num, dev_info->ai_core_num);
        return -ENODEV;
    }
    if (dev_info->inuse.ai_cpu_error_bitmap) {
        for (i = 0; i < dev_info->ai_cpu_core_num; i++) {
            if (dev_info->inuse.ai_cpu_error_bitmap & (0x01U << i))
                devdrv_drv_err("ai cpu: %d init failed.\n", i);
        }
    }

    if (dev_info->inuse.ai_core_error_bitmap) {
        for (i = 0; i < dev_info->ai_core_num; i++) {
            if (dev_info->inuse.ai_core_error_bitmap & (0x01U << i))
                devdrv_drv_err("ai core: %d init failed.\n", i);
        }
    }

    devdrv_drv_debug("received ready message from pcie device(%u)\n", dev_info->dev_id);
    devdrv_drv_debug(" ai_core_num = %d, ai_cpu_core_num = %d, "
                     "ctrl_cpu_core_num = %d, ctrl_cpu_endian_little = %d, "
                     "ctrl_cpu_id = %d, ctrl_cpu_ip = %d, "
                     "ts_cpu_core_num = %d, "
                     "ai_core_id = %d, "
                     "ai_cpu_core_id = %d\n",
                     dev_info->ai_core_num,
                     dev_info->ai_cpu_core_num,
                     dev_info->ctrl_cpu_core_num,
                     dev_info->ctrl_cpu_endian_little,
                     dev_info->ctrl_cpu_id,
                     dev_info->ctrl_cpu_ip,
                     pdata->ts_pdata[tsid].ts_cpu_core_num,
                     dev_info->ai_core_id,
                     dev_info->ai_cpu_core_id);
    devdrv_drv_debug("env_type = %d\n", dev_info->env_type);

    return 0;
}

STATIC u32 devdrv_get_ts_num(void)
{
    u32 tsid;

#ifndef CFG_SOC_PLATFORM_MINIV2
    tsid = DEVDRV_MAX_TS_NUM;
#else
    tsid = 1;  // tsid should be transfered from dev side later
#endif /* CFG_SOC_PLATFORM_MINIV2 */
    return tsid;
}

STATIC int devdrv_manager_device_ready(void *msg, u32 *ack_len)
{
    struct devdrv_manager_msg_info *dev_manager_msg_info = NULL;
    struct devdrv_device_info *drv_info = NULL;
    struct devdrv_platform_data *pdata = NULL;
    struct devdrv_info *dev_info = NULL;
    u32 dev_id;
    int ret;

    devdrv_drv_info("*** receive device ready info ***\n");

    dev_manager_msg_info = (struct devdrv_manager_msg_info *)msg;
    dev_id = dev_manager_msg_info->header.dev_id;

    if (dev_manager_msg_info->header.valid != DEVDRV_MANAGER_MSG_VALID) {
        devdrv_drv_err("invalid message from host\n");
        return -EINVAL;
    }

    if (dev_id >= DEVDRV_MAX_DAVINCI_NUM) {
        devdrv_drv_err("invalid dev_id(%u)\n", dev_id);
        return -ENODEV;
    }

    /* dev send message with pcie device id, get dev_info from devdrv_info_array */
    dev_info = devdrv_get_devdrv_info_array(dev_id);
    if (dev_info == NULL) {
        devdrv_drv_err("device(%u) is not ready.\n", dev_id);
        return -EINVAL;
    }

    if (dev_info->dev_ready != 0) {
        devdrv_drv_info("device(%u) already informed\n", dev_id);
        return -ENODEV;
    }

    dev_info->dev_ready = DEVDRV_DEV_READY_EXIST;

    pdata = dev_info->pdata;
    if (pdata == NULL) {
        devdrv_drv_err("pata is NULL\n");
        return -ENOMEM;
    }
    pdata->dev_id = dev_info->dev_id;
    pdata->ts_num = devdrv_get_ts_num();

    drv_info = (struct devdrv_device_info *)dev_manager_msg_info->payload;
    if (drv_info->ts_load_fail == 0) {
        devdrv_drv_info("*** ts fw load succeed, set device(%u) ready work ***\n", dev_id);
        devdrv_set_ts_work(&dev_info->ts_mng[0]);
        dev_info->dev_ready = DEVDRV_DEV_READY_WORK;
    }

    ret = devdrv_manager_device_ready_info(dev_info, drv_info);
    if (ret) {
        devdrv_drv_err("device(%u) ready info failed\n", dev_id);
        return -EINVAL;
    }

    *ack_len = sizeof(*dev_manager_msg_info);

    if (devdrv_manager_client_info_registered() == 0)
        /* dev driver registered, send correct result to device */
        dev_manager_msg_info->header.result = 0;
    else
        /* dev driver doesn't registered, send wrong result to device */
        dev_manager_msg_info->header.result = (u16)DEVDRV_MANAGER_MSG_INVALID_RESULT;

    if (dev_manager_info != NULL) {
        devdrv_drv_info("device(%u) ready\n", dev_id);
        queue_work(dev_manager_info->dev_rdy_work, &dev_info->work);
    }

    return 0;
}

STATIC int devdrv_manager_device_ready_and_ts_work(void *msg, u32 *ack_len)
{
    struct devdrv_manager_msg_info *dev_manager_msg_info = NULL;
    struct devdrv_info *dev_info = NULL;
    u32 dev_id;

    devdrv_drv_debug("*** receive device ready work info ***\n");

    dev_manager_msg_info = (struct devdrv_manager_msg_info *)msg;
    dev_id = dev_manager_msg_info->header.dev_id;

    if (dev_manager_msg_info->header.valid != DEVDRV_MANAGER_MSG_VALID) {
        devdrv_drv_err("invalid message from host\n");
        return -EINVAL;
    }

    if (dev_id >= DEVDRV_MAX_DAVINCI_NUM) {
        devdrv_drv_err("invalid dev_id(%u)\n", dev_id);
        return -ENODEV;
    }

    /* dev send message with pcie device id, get dev_info from devdrv_info_array */
    dev_info = devdrv_get_devdrv_info_array(dev_id);
    if (dev_info == NULL) {
        devdrv_drv_warn("device(%u) is not ready.\n", dev_id);
        return -EINVAL;
    }

    if (dev_info->dev_ready != DEVDRV_DEV_READY_EXIST) {
        devdrv_drv_info("device(%u) info is not inited.\n", dev_id);
        return -EINVAL;
    }
    dev_info->dev_ready = DEVDRV_DEV_READY_WORK;

    devdrv_set_ts_work(&dev_info->ts_mng[0]);

    *ack_len = sizeof(*dev_manager_msg_info);
    dev_manager_msg_info->header.result = 0;

    devdrv_drv_debug("*** set status work ***\n");

    return 0;
}

STATIC int devdrv_manager_device_inform_handler(void *msg, u32 *ack_len, enum devdrv_ts_status status)
{
    struct devdrv_manager_msg_info *dev_manager_msg_info = NULL;
    struct devdrv_info *dev_info = NULL;
    u32 dev_id;
    int ret;

    dev_manager_msg_info = (struct devdrv_manager_msg_info *)msg;
    if (dev_manager_msg_info->header.valid != DEVDRV_MANAGER_MSG_VALID) {
        devdrv_drv_err("invalid message from host\n");
        return -EINVAL;
    }

    dev_id = dev_manager_msg_info->header.dev_id;
    if (dev_id >= DEVDRV_MAX_DAVINCI_NUM) {
        devdrv_drv_err("invalid dev_id(%u)\n", dev_id);
        return -ENODEV;
    }

    dev_info = devdrv_get_devdrv_info_array(dev_id);
    if (dev_info == NULL) {
        devdrv_drv_err("device(%u) is not ready.\n", dev_id);
        return -EINVAL;
    }

    switch (status) {
        case DEVDRV_TS_DOWN:
            devdrv_drv_err("receive ts exception message from device(%u).\n", dev_id);
            devdrv_host_manager_device_exception(dev_info);
            ret = 0;
            break;
        case DEVDRV_TS_WORK:
            devdrv_drv_err("receive ts resume message from device(%u).\n", dev_id);
            ret = devdrv_host_manager_device_resume(dev_info);
            break;
        case DEVDRV_TS_FAIL_TO_SUSPEND:
            devdrv_drv_err("receive ts fail to suspend message from device(%u).\n", dev_id);
            ret = devdrv_host_manager_device_resume(dev_info);
            break;
        case DEVDRV_TS_SLEEP:
            devdrv_drv_err("receive ts suspend message from device(%u).\n", dev_id);
            ret = devdrv_host_manager_device_suspend(dev_info);
            break;
        default:
            devdrv_drv_err("invalid input ts status. dev_id(%u)\n", dev_id);
            return -EINVAL;
    }

    *ack_len = sizeof(*dev_manager_msg_info);
    /* return result */
    dev_manager_msg_info->header.result = ret;
    return 0;
}

STATIC int devdrv_manager_device_down(void *msg, u32 *ack_len)
{
    return devdrv_manager_device_inform_handler(msg, ack_len, DEVDRV_TS_DOWN);
}

STATIC int devdrv_manager_device_suspend(void *msg, u32 *ack_len)
{
    return devdrv_manager_device_inform_handler(msg, ack_len, DEVDRV_TS_SLEEP);
}

STATIC int devdrv_manager_device_resume(void *msg, u32 *ack_len)
{
    return devdrv_manager_device_inform_handler(msg, ack_len, DEVDRV_TS_WORK);
}

STATIC int devdrv_manager_device_fail_to_suspend(void *msg, u32 *ack_len)
{
    return devdrv_manager_device_inform_handler(msg, ack_len, DEVDRV_TS_FAIL_TO_SUSPEND);
}
STATIC int devdrv_manager_check_process_sign(void *msg, u32 *ack_len)
{
    struct devdrv_manager_msg_info *dev_manager_msg_info = NULL;
    int ret = DEVDRV_MANAGER_MSG_INVALID_RESULT;
    struct devdrv_manager_info *d_info = NULL;
    struct devdrv_process_sign *d_sign = NULL;
    struct process_sign *process_sign = NULL;
    struct list_head *pos = NULL;
    struct list_head *n = NULL;
    u32 dev_id;

    dev_manager_msg_info = (struct devdrv_manager_msg_info *)msg;
    if (dev_manager_msg_info->header.valid != DEVDRV_MANAGER_MSG_VALID) {
        devdrv_drv_err("invalid message from host\n");
        return -EINVAL;
    }

    dev_id = dev_manager_msg_info->header.dev_id;
    if ((dev_id >= DEVDRV_MAX_DAVINCI_NUM)) {
        devdrv_drv_err("invalid dev_id(%u).\n", dev_id);
        return -EINVAL;
    }
    d_info = devdrv_get_manager_info();
    if (d_info == NULL) {
        devdrv_drv_err("d_info is null.\n");
        return -EINVAL;
    }
    process_sign = (struct process_sign *)dev_manager_msg_info->payload;
    down(&d_info->devdrv_sign_list_sema);
    if (list_empty_careful(&d_info->hostpid_list_header)) {
        devdrv_drv_err("list is empty.\n");
        goto out;
    }
    list_for_each_safe(pos, n, &d_info->hostpid_list_header) {
        d_sign = list_entry(pos, struct devdrv_process_sign, list);
        if (d_sign->hostpid == process_sign->tgid) {
            ret = strcmp(d_sign->sign, process_sign->sign);
            if (ret) {
                devdrv_drv_err("sign is error, ret(%d).\n", ret);
            }
            goto out;
        }
    }
    devdrv_drv_err("hostpid(%d) is error.\n", process_sign->tgid);

out:
    up(&d_info->devdrv_sign_list_sema);
    *ack_len = sizeof(*dev_manager_msg_info);
    dev_manager_msg_info->header.result = ret;
    return ret;
}

STATIC int devdrv_manager_get_pcie_id(void *msg, u32 *ack_len)
{
    struct devdrv_manager_msg_info *dev_manager_msg_info = NULL;
    struct devdrv_pcie_id_info pcie_id_info = {0};
    struct devdrv_pcie_id_info *host_pcie_id_info = NULL;
    struct devdrv_info *dev_info = NULL;
    u32 dev_id;
    int ret;

    dev_manager_msg_info = (struct devdrv_manager_msg_info *)msg;
    if (dev_manager_msg_info->header.valid != DEVDRV_MANAGER_MSG_VALID) {
        devdrv_drv_err("invalid message from host\n");
        return -EINVAL;
    }

    dev_id = dev_manager_msg_info->header.dev_id;
    if ((dev_id >= DEVDRV_MAX_DAVINCI_NUM) || (devdrv_info_array[dev_id] == NULL)) {
        devdrv_drv_err("invalid dev_id(%u), or devdrv_info_array[dev_id] is NULL\n", dev_id);
        return -EINVAL;
    }

    dev_info = devdrv_info_array[dev_id];

    ret = devdrv_get_pcie_id_info(dev_info->pci_dev_id, &pcie_id_info);
    if (ret) {
        devdrv_drv_err("devdrv_manager_get_pcie_id_info failed. dev_id(%u)\n", dev_id);
        goto out;
    }

    host_pcie_id_info = (struct devdrv_pcie_id_info *)dev_manager_msg_info->payload;

    host_pcie_id_info->venderid = pcie_id_info.venderid;
    host_pcie_id_info->subvenderid = pcie_id_info.subvenderid;
    host_pcie_id_info->deviceid = pcie_id_info.deviceid;
    host_pcie_id_info->subdeviceid = pcie_id_info.subdeviceid;
    host_pcie_id_info->bus = pcie_id_info.bus;
    host_pcie_id_info->device = pcie_id_info.device;
    host_pcie_id_info->fn = pcie_id_info.fn;

out:
    *ack_len = sizeof(*dev_manager_msg_info);
    /* return result */
    dev_manager_msg_info->header.result = ret;
    return 0;
}

STATIC int devdrv_manager_d2h_sync_matrix_ready(void *msg, u32 *ack_len)
{
    struct devdrv_manager_msg_info *dev_manager_msg_info = NULL;
    u32 dev_id;
    int ret = 0;

    dev_manager_msg_info = (struct devdrv_manager_msg_info *)msg;
    if (dev_manager_msg_info->header.valid != DEVDRV_MANAGER_MSG_VALID) {
        devdrv_drv_err("invalid message from host\n");
        return -EINVAL;
    }

    dev_id = dev_manager_msg_info->header.dev_id;
    if ((dev_id >= DEVDRV_MAX_DAVINCI_NUM) || (devdrv_info_array[dev_id] == NULL)) {
        devdrv_drv_err("invalid dev_id(%u), or devdrv_info_array[dev_id] is NULL\n", dev_id);
        return -EINVAL;
    }

    *ack_len = sizeof(*dev_manager_msg_info);
    dev_manager_msg_info->header.result = ret;
    return 0;
}

STATIC int (*devdrv_manager_chan_msg_processes[])(void *msg, u32 *ack_len) = {
    [DEVDRV_MANAGER_CHAN_D2H_DEVICE_READY] = devdrv_manager_device_ready,
    [DEVDRV_MANAGER_CHAN_D2H_DEVICE_READY_AND_TS_WORK] = devdrv_manager_device_ready_and_ts_work,
    [DEVDRV_MANAGER_CHAN_D2H_DOWN] = devdrv_manager_device_down,
    [DEVDRV_MANAGER_CHAN_D2H_SUSNPEND] = devdrv_manager_device_suspend,
    [DEVDRV_MANAGER_CHAN_D2H_RESUME] = devdrv_manager_device_resume,
    [DEVDRV_MANAGER_CHAN_D2H_FAIL_TO_SUSPEND] = devdrv_manager_device_fail_to_suspend,
    [DEVDRV_MANAGER_CHAN_D2H_CORE_INFO] = NULL,
    [DEVDRV_MANAGER_CHAN_D2H_GET_PCIE_ID_INFO] = devdrv_manager_get_pcie_id,
    /* [DEVDRV_MANAGER_CHAN_H2D_HEART_BEAT] = NULL, */
    [DEVDRV_MANAGER_CHAN_D2H_SYNC_GET_STREAM] = devdrv_manager_d2h_sync_get_stream,
    [DEVDRV_MANAGER_CHAN_D2H_SYNC_GET_EVENT] = devdrv_manager_d2h_sync_get_event,
    [DEVDRV_MANAGER_CHAN_D2H_SYNC_GET_SQ] = devdrv_manager_d2h_sync_get_sq,
    [DEVDRV_MANAGER_CHAN_D2H_SYNC_GET_CQ] = devdrv_manager_d2h_sync_get_cq,
    [DEVDRV_MANAGER_CHAN_D2H_SYNC_GET_MODEL] = devdrv_manager_d2h_sync_get_model,
    [DEVDRV_MANAGER_CHAN_D2H_SYNC_GET_NOTIFY] = devdrv_manager_d2h_sync_get_notify,
    [DEVDRV_MANAGER_CHAN_D2H_LOAD_KERNEL_RESULT] = NULL,
    [DEVDRV_MANAGER_CHAN_D2H_SYNC_GET_TASK] = devdrv_manager_d2h_sync_get_task,
    [DEVDRV_MANAGER_CHAN_D2H_SYNC_MATRIX_READY] = devdrv_manager_d2h_sync_matrix_ready,
    [DEVDRV_MANAGER_CHAN_D2H_CHECK_PROCESS_SIGN] = devdrv_manager_check_process_sign,
    [DEVDRV_MANAGER_CHAN_MAX_ID] = NULL,
};

STATIC int devdrv_chan_msg_dispatch(void *data, u32 *real_out_len)
{
    u32 msg_id = ((struct devdrv_manager_msg_head *)data)->msg_id;

    return devdrv_manager_chan_msg_processes[msg_id](data, real_out_len);
}

STATIC int devdrv_manager_rx_common_msg_process(u32 dev_id, void *data, u32 in_data_len, u32 out_data_len,
                                                u32 *real_out_len)
{
    struct devdrv_manager_msg_info *dev_manager_msg_info = NULL;
    u32 msg_id;

    if ((dev_id >= DEVDRV_MAX_DAVINCI_NUM) || (data == NULL) || (real_out_len == NULL) ||
        (in_data_len < sizeof(struct devdrv_manager_msg_info))) {
        devdrv_drv_err("date(%pK) or real_out_len(%pK) is NULL, devid(%u), in_data_len(%u)\n",
            data, real_out_len, dev_id, in_data_len);
        return -EINVAL;
    }

    msg_id = ((struct devdrv_manager_msg_head *)data)->msg_id;

    if (msg_id >= DEVDRV_MANAGER_CHAN_MAX_ID) {
        devdrv_drv_err("invalid msg_id(%u)\n", msg_id);
        return -EINVAL;
    }
    if (devdrv_manager_chan_msg_processes[msg_id] == NULL) {
        devdrv_drv_err("devdrv_manager_chan_msg_processes[%u] is NULL\n", msg_id);
        return -EINVAL;
    }

    dev_manager_msg_info = (struct devdrv_manager_msg_info *)data;
    dev_manager_msg_info->header.dev_id = dev_id;
    return devdrv_chan_msg_dispatch(data, real_out_len);
}

STATIC int devdrv_manager_rx_msg_process(void *msg_chan, void *data, u32 in_data_len, u32 out_data_len,
                                         u32 *real_out_len)
{
    struct devdrv_manager_msg_info *dev_manager_msg_info = NULL;
    struct devdrv_info *dev_info = NULL;
    u32 msg_id;
    int dev_id;

    if ((msg_chan == NULL) || (data == NULL) || (real_out_len == NULL) ||
        (in_data_len < sizeof(struct devdrv_manager_msg_info))) {
        devdrv_drv_err("msg_chan(%pK) or data(%pK) or real_out_len(%pK) is NULL, in_date_len(%u)\n",
            msg_chan, data, real_out_len, in_data_len);
        return -EINVAL;
    }
    msg_id = ((struct devdrv_manager_msg_head *)data)->msg_id;

    if ((msg_id >= DEVDRV_MANAGER_CHAN_MAX_ID) || (devdrv_manager_chan_msg_processes[msg_id] == NULL)) {
        devdrv_drv_err("invalid msg_id(%u) or devdrv_manager_chan_msg_processes[msg_id] is NULL\n", msg_id);
        return -EINVAL;
    }

    dev_info = (struct devdrv_info *)devdrv_get_msg_chan_priv(msg_chan);
    dev_manager_msg_info = (struct devdrv_manager_msg_info *)data;

    /* get dev_id by msg_chan */
    if ((dev_id = devdrv_get_msg_chan_devid(msg_chan)) < 0) {
        devdrv_drv_err("msg_chan to devid failed\n");
        return -EINVAL;
    }

    if (dev_id >= DEVDRV_MAX_DAVINCI_NUM) {
        devdrv_drv_err("invalid dev_id(%d).\r\n", dev_id);
        return -EINVAL;
    }
    dev_manager_msg_info->header.dev_id = dev_id;

    return devdrv_chan_msg_dispatch(data, real_out_len);
}

STATIC void devdrv_manager_msg_chan_notify(u32 dev_id, int status)
{
}

#define DEV_MNG_NON_TRANS_MSG_DESC_SIZE 1024
struct devdrv_non_trans_msg_chan_info dev_manager_msg_chan_info = {
    .msg_type = devdrv_msg_client_devmanager,
    .flag = 0,
    .level = DEVDRV_MSG_CHAN_LEVEL_LOW,
    .s_desc_size = DEV_MNG_NON_TRANS_MSG_DESC_SIZE,
    .c_desc_size = DEV_MNG_NON_TRANS_MSG_DESC_SIZE,
    .rx_msg_process = devdrv_manager_rx_msg_process,
};

STATIC void devdrv_manager_dev_ready_work(struct work_struct *work)
{
    struct devdrv_info *dev_info = NULL;
    void *no_trans_chan = NULL;
    u32 dev_id;

    dev_info = container_of(work, struct devdrv_info, work);
    dev_id = dev_info->pci_dev_id;
    if (dev_id >= DEVDRV_MAX_DAVINCI_NUM) {
        devdrv_drv_err("invalid dev_id(%u)\n", dev_id);
        return;
    }

    no_trans_chan = devdrv_pcimsg_alloc_non_trans_queue(dev_id, &dev_manager_msg_chan_info);
    if (no_trans_chan == NULL) {
        devdrv_drv_err("device(%u) no_trans_chan alloc failed.\n", dev_id);
        return;
    }

    devdrv_manager_set_no_trans_chan(dev_id, no_trans_chan);
    devdrv_set_msg_chan_priv(no_trans_chan, (void *)dev_info);

    if (devdrv_manager_register(dev_info)) {
        devdrv_drv_err("devdrv_manager_register failed. dev_id(%u)\n", dev_id);
        return;
    }

    if (devdrv_heart_beat_init(dev_info)) {
        devdrv_drv_err("devdrv_heart_beat_init failed, device(%u) run without heart beat.\n", dev_id);
    } else {
        devdrv_drv_info("devdrv_heart_beat_init succ, device(%u) run with heart beat.\n", dev_id);
    }
    up(&dev_info->no_trans_chan_wait_sema);
    atomic_set(&dev_info->occupy_ref, 0);
    dev_info->status = 0;
}

STATIC void devdrv_manager_clean_client_info(void)
{
    mutex_lock(&devdrv_mutex);
    dev_manager_client_info.client_init = 0;
    dev_manager_client_info.dev_client.register_client = NULL;
    dev_manager_client_info.dev_client.unregister_client = NULL;
    mutex_unlock(&devdrv_mutex);
}

STATIC void devdrv_manager_init_client_info(struct devdrv_init_client *dev_client)
{
    mutex_lock(&devdrv_mutex);
    dev_manager_client_info.client_init = 1;
    dev_manager_client_info.dev_client.register_client = dev_client->register_client;
    dev_manager_client_info.dev_client.unregister_client = dev_client->unregister_client;
    mutex_unlock(&devdrv_mutex);
}

STATIC int devdrv_manager_client_info_registered(void)
{
    mutex_lock(&devdrv_mutex);
    if (dev_manager_client_info.client_init == 0) {
        mutex_unlock(&devdrv_mutex);
        return -EINVAL;
    }
    mutex_unlock(&devdrv_mutex);
    return 0;
}

STATIC struct devdrv_init_client *devdrv_manager_get_client(void)
{
    struct devdrv_init_client *dev_client = NULL;

    mutex_lock(&devdrv_mutex);
    dev_client = &dev_manager_client_info.dev_client;
    mutex_unlock(&devdrv_mutex);

    return dev_client;
}

int devdrv_manager_register_client(struct devdrv_init_client *dev_client, const struct file_operations *ops)
{
    struct devdrv_info *dev_info = NULL;
    u32 num_dev;
    int ret;
    u32 i;

    if ((dev_client == NULL) || (dev_client->register_client == NULL) || (dev_client->unregister_client == NULL) ||
        (ops == NULL)) {
        devdrv_drv_err("dev_client(%pK) or ops(%pK) or dev_client->register_client(%pK) or "
                       "dev_client->unregister_client is NULL\n",
                       dev_client, ops, (dev_client == NULL) ? NULL : dev_client->register_client);
        return -EINVAL;
    }

    devdrv_manager_init_client_info(dev_client);
    devdrv_manager_set_devdrv_ops(ops);

    num_dev = devdrv_manager_get_devnum();
    if ((num_dev == 0) || (num_dev > DEVDRV_MAX_DAVINCI_NUM)) {
        devdrv_drv_info("no device yet, if you can search <get devnum succ> on the previous print, it's OK. "
            "num_dev(%u)\n", num_dev);
        return -ENODEV;
    }

    for (i = 0; i < DEVDRV_MAX_DAVINCI_NUM; i++) {
        dev_info = devdrv_manager_get_devdrv_info(i);
        if (dev_info == NULL) {
            devdrv_drv_info("dev_info null i(%d)\n", i);
            continue;
        }

        ret = devdrv_manager_run_register_client(dev_info);
        if (ret)
            devdrv_drv_err("call register client failed, i(%d), dev_id(%u), ret(%d)\n", i, dev_info->dev_id, ret);
    }

    return 0;
}
EXPORT_SYMBOL(devdrv_manager_register_client);

int devdrv_manager_unregister_client(struct devdrv_init_client *dev_client)
{
    struct devdrv_info *dev_info = NULL;
    u32 num_dev;
    u32 i;

    if ((dev_client == NULL) || (dev_client->register_client == NULL) || (dev_client->unregister_client == NULL)) {
        devdrv_drv_err("dev_client(%pK) or dev_client->register_client(%pK) or dev_client->unregister_client is NULL\n",
                       dev_client, (dev_client == NULL) ? NULL : dev_client->register_client);
        return -EINVAL;
    }
    num_dev = devdrv_manager_get_devnum();
    if ((num_dev == 0) || (num_dev > DEVDRV_MAX_DAVINCI_NUM)) {
        devdrv_drv_err("wrong device num, num_dev(%u)\n", num_dev);
        return -EFAULT;
    }

    for (i = 0; i < DEVDRV_MAX_DAVINCI_NUM; i++) {
        dev_info = devdrv_manager_get_devdrv_info(i);
        if (dev_info == NULL) {
            continue;
        }
        devdrv_drv_info("dev_info unregister i(%d), dev_id(%u)\n", i, dev_info->dev_id);
        devdrv_manager_run_unregister_client(dev_info);
    }
    devdrv_manager_clean_client_info();
    devdrv_manager_unset_devdrv_ops();

    return 0;
}
EXPORT_SYMBOL(devdrv_manager_unregister_client);

STATIC inline void devdrv_manager_common_chan_init(void)
{
    /* this function will be called at module_init, doesn't need lock */
    devdrv_manager_common_chan.type = DEVDRV_COMMON_MSG_DEVDRV_MANAGER;
    devdrv_manager_common_chan.common_msg_recv = devdrv_manager_rx_common_msg_process;
    devdrv_manager_common_chan.init_notify = devdrv_manager_msg_chan_notify;
}

STATIC inline void devdrv_manager_common_chan_uninit(void)
{
    /* this function will be called at module_init, doesn't need lock */
    devdrv_manager_common_chan.type = DEVDRV_COMMON_MSG_TYPE_MAX;
    devdrv_manager_common_chan.common_msg_recv = NULL;
    devdrv_manager_common_chan.init_notify = NULL;
}

STATIC inline struct devdrv_common_msg_client *devdrv_manager_get_common_chan(u32 dev_id)
{
    return &devdrv_manager_common_chan;
}

void *devdrv_manager_get_no_trans_chan(u32 dev_id)
{
    void *no_trans_chan = NULL;

    /* dev_manager_no_trasn_chan doesn't need lock */
    if (dev_id >= DEVDRV_MAX_DAVINCI_NUM) {
        devdrv_drv_err("get no trans chan failed, dev_id(%u)\n", dev_id);
        return NULL;
    }
    no_trans_chan = dev_manager_no_trasn_chan[dev_id];
    return no_trans_chan;
}

STATIC void devdrv_manager_set_no_trans_chan(u32 dev_id, void *no_trans_chan)
{
    if (dev_id >= DEVDRV_MAX_DAVINCI_NUM) {
        devdrv_drv_err("set no trans chan failed, dev_id(%u)\n", dev_id);
        return;
    }
    dev_manager_no_trasn_chan[dev_id] = no_trans_chan;
}

#if LINUX_VERSION_CODE >= KERNEL_VERSION(4, 15, 0)
struct devdrv_check_start_s {
    u32 dev_id;
    struct timer_list check_timer;
};

STATIC struct devdrv_check_start_s devdrv_check_start[DEVDRV_MAX_DAVINCI_NUM];

STATIC void devdrv_check_start_event(struct timer_list *t)
{
    struct devdrv_info *dev_info = NULL;
    struct timespec stamp;
    struct devdrv_check_start_s *devdrv_start_check = from_timer(devdrv_start_check, t, check_timer);
    u32 dev_id;

    dev_id = devdrv_start_check->dev_id;
#else  /* LINUX_VERSION_CODE >= KERNEL_VERSION(4,15,0) */

STATIC struct timer_list devdrv_check_start[DEVDRV_MAX_DAVINCI_NUM];

STATIC void devdrv_check_start_event(unsigned long data)
{
    struct devdrv_info *dev_info = NULL;
    struct timespec stamp;
    u32 dev_id = (u32)data;
#endif /* LINUX_VERSION_CODE >= KERNEL_VERSION(4,15,0) */

    devdrv_drv_debug("*** time event for checking whether device is started or not ***\n");

    stamp = current_kernel_time();

    dev_info = devdrv_get_devdrv_info_array(dev_id);
    ;
    if (dev_info == NULL) {
        devdrv_drv_err("device(%u) is not ready.\n", dev_id);
        return;
    }

    if (dev_info->dev_ready < DEVDRV_DEV_READY_EXIST) {
        devdrv_drv_err("device(%u) is not ready, "
                       "dev_ready = %d, dev_id = %u\n",
                       dev_id, dev_info->dev_ready, dev_info->dev_id);
        (void)devdrv_host_black_box_add_exception(dev_info->dev_id, DEVDRV_BB_DEVICE_LOAD_TIMEOUT, stamp, NULL);
        devdrv_set_ts_down(&dev_info->ts_mng[0]);

        return;
    }

    devdrv_drv_debug("*** device(%u) is started and working ***\n", dev_id);
}

STATIC int devdrv_manager_host_reboot_inform(u32 devid)
{
    struct devdrv_manager_msg_info dev_manager_msg_info;
    struct devdrv_heart_beat_node *beat_node = NULL;
    struct devdrv_heart_beat_sq *sq = NULL;
    struct devdrv_heart_beat *hb = NULL;
    struct devdrv_info *info = NULL;
    int out_len;
    int ret;

    info = devdrv_get_devdrv_info_array(devid);
    if (info == NULL) {
        devdrv_drv_warn("dev(%u) info is not ready.\n", devid);
        return -EFAULT;
    }

    hb = &info->heart_beat;

    sq = kzalloc(sizeof(struct devdrv_heart_beat_sq), GFP_ATOMIC);
    if (sq == NULL) {
        devdrv_drv_err("kmalloc sq failed. dev_id(%u)\n", devid);
        return -ENOMEM;
    }
    beat_node = kzalloc(sizeof(struct devdrv_heart_beat_node), GFP_ATOMIC);
    if (beat_node == NULL) {
        kfree(sq);
        sq = NULL;
        devdrv_drv_err("kmalloc beat_node failed. dev_id(%u)\n", devid);
        return -ENOMEM;
    }

    sq->number = hb->cmd_inc_counter;
    sq->devid = info->dev_id;
    sq->cmd = DEVDRV_HEART_BEAT_SQ_CMD;
    sq->reserved = DEVDRV_WILL_HOST_REBOOT;

    beat_node->sq = sq;
    beat_node->useless = 0;

    dev_manager_msg_info.header.msg_id = DEVDRV_MANAGER_CHAN_H2D_HEART_BEAT;
    dev_manager_msg_info.header.valid = DEVDRV_MANAGER_MSG_VALID;
    /* give a random value for checking result later */
    dev_manager_msg_info.header.result = (u16)0x1A;
    /* inform corresponding devid to device side */
    dev_manager_msg_info.header.dev_id = info->dev_id;

    ret = memcpy_s(dev_manager_msg_info.payload, sizeof(dev_manager_msg_info.payload), sq,
                   sizeof(struct devdrv_heart_beat_sq));
    if (ret) {
        devdrv_drv_err("copy from sq failed. dev_id(%u)\n", devid);
        goto out;
    }

    ret = devdrv_manager_send_msg(info, &dev_manager_msg_info, &out_len);
    if (ret || (dev_manager_msg_info.header.result != 0)) {
        devdrv_drv_err("devdrv_manager_send_msg failed, "
                       "ret(%d), dev_id = %u. result=%u\n",
                       ret, info->dev_id, dev_manager_msg_info.header.result);
    }
out:
    kfree(sq);
    sq = NULL;
    kfree(beat_node);
    beat_node = NULL;
    return ret;
}

STATIC int devdrv_manager_dev_state_notify(u32 devid, u32 state)
{
    struct devdrv_black_box_state_info *bbox_state_info = NULL;
    struct timespec tstemp = {0};
    int ret;

    if ((dev_manager_info == NULL) || ((enum devdrv_device_state)state >= STATE_TO_MAX) ||
        (devid >= DEVDRV_MAX_DAVINCI_NUM)) {
        devdrv_drv_err("state notify para is invalid,"
                       "with dev_manager_info(%pK),"
                       "state(%d), devid(%u).\n",
                       dev_manager_info, (u32)state, devid);
        return -ENODEV;
    }

    bbox_state_info = kzalloc(sizeof(struct devdrv_black_box_state_info), GFP_ATOMIC);
    if (bbox_state_info == NULL) {
        devdrv_drv_err("malloc state_info failed. dev_id(%u)\n", devid);
        return -ENOMEM;
    }

    bbox_state_info->devId = devid;
    bbox_state_info->state = state;

    devdrv_drv_info("dev state notified with devid(%u), state(%d)", devid, state);

    ret = devdrv_host_black_box_add_exception(0, DEVDRV_BB_DEVICE_STATE_INFORM, tstemp, (void *)bbox_state_info);
    if (ret) {
        kfree(bbox_state_info);
        bbox_state_info = NULL;
        devdrv_drv_err("devdrv_host_black_box_add_exception failed. dev_id(%u)\n", devid);
        return -ENODEV;
    }

    /* send device info for ignore heartbeat monitor */
    if (state == STATE_TO_DISABLE_DEV) {
        ret = devdrv_manager_host_reboot_inform(devid);
        if (ret) {
            devdrv_drv_err("inform failed. dev_id(%u)\n", devid);
            return -ENODEV;
        }

        devdrv_drv_info("host reboot info inform send. dev_id(%u)\n", devid);
    }

    return 0;
}
STATIC int devdrv_manager_dev_startup_notify(u32 prob_num, const u32 devids[], u32 array_len, u32 devnum)
{
    struct devdrv_black_box_devids *bbox_devids = NULL;
    u32 dev_num = devnum;
    struct timespec tstemp = {0};
    unsigned long flags;
    int ret;
    u32 i;

    (void)array_len;
    if ((dev_manager_info == NULL) || (devnum > DEVDRV_MAX_DAVINCI_NUM) || (devids == NULL)) {
        devdrv_drv_err("dev manager info is not initialized\n");
        return -ENODEV;
    }
    spin_lock_irqsave(&dev_manager_info->spinlock, flags);
    dev_manager_info->prob_num = prob_num;
    spin_unlock_irqrestore(&dev_manager_info->spinlock, flags);

    bbox_devids = kzalloc(sizeof(struct devdrv_black_box_devids), GFP_ATOMIC);
    if (bbox_devids == NULL) {
        devdrv_drv_err("malloc devids failed\n");
        return -ENOMEM;
    }

    bbox_devids->dev_num = dev_num;
    for (i = 0; i < dev_num; i++)
        bbox_devids->devids[i] = devids[i];

    ret = devdrv_host_black_box_add_exception(0, DEVDRV_BB_DEVICE_ID_INFORM, tstemp, (void *)bbox_devids);
    if (ret) {
        kfree(bbox_devids);
        bbox_devids = NULL;
        devdrv_drv_err("devdrv_host_black_box_add_exception failed, ret(%d).\n", ret);
        return -ENODEV;
    }

    return 0;
}

STATIC int devdrv_manager_init_instance(struct devdrv_client_instance *dev_ctrl)
{
    struct devdrv_common_msg_client *devdrv_commn_chan = NULL;
    struct devdrv_platform_data *pdata = NULL;
    struct devdrv_info *dev_info = NULL;
    u32 dev_id;
    int ret;
    u32 init_flag = 1;

    dev_id = dev_ctrl->dev_ctrl->dev_id;
    if (dev_id >= DEVDRV_MAX_DAVINCI_NUM) {
        devdrv_drv_err("invalid dev_id(%u).\n", dev_id);
        return -EINVAL;
    }

    dev_info = devdrv_get_devdrv_info_array(dev_id);
    if (dev_info != NULL) {
        init_flag = 0;
        devdrv_drv_info("dev_id(%u) repeat init instance.\n", dev_id);
    }

    if (init_flag == 1) {
        dev_info = kzalloc(sizeof(*dev_info), GFP_KERNEL);
        if (dev_info == NULL) {
            devdrv_drv_err("kzalloc dev_info failed. dev_id(%u)\n", dev_id);
            return -ENOMEM;
        }

        mutex_init(&dev_info->lock);

        /* set config */
        dev_info->host_config.heart_beat_en = devdrv_heart_beat_en;

        dev_info->dev_id = dev_id;
        dev_info->pci_dev_id = dev_id;
        dev_info->heart_beat.sq = DEVDRV_MAX_FUNCTIONAL_SQ_NUM;
        dev_info->heart_beat.cq = DEVDRV_MAX_FUNCTIONAL_CQ_NUM;

        spin_lock_init(&dev_info->ts_mng[0].ts_spinlock);

        dev_info->cce_ops.cce_dev = NULL;
    }
    /* for pclint warning */
    if (dev_info == NULL) {
        devdrv_drv_err("dev_info is null, dev_id(%u)\n", dev_id);
        return -ENOMEM;
    }

    dev_info->dev_ready = 0;
    dev_info->cce_ctrl[0] = NULL;
    dev_info->ts_mng[0].ts_work_status = NULL;
    dev_info->driver_flag = 0;
    dev_info->dev = dev_ctrl->dev_ctrl->dev;

    devdrv_set_ts_initing(&dev_info->ts_mng[0]);
    devdrv_drv_debug("*** set status initing ***\n");

    if (init_flag == 1) {
        pdata = kzalloc(sizeof(struct devdrv_platform_data), GFP_KERNEL);
        if (pdata == NULL) {
            kfree(dev_info);
            dev_info = NULL;
            return -ENOMEM;
        }

        /* host has one ts in any scenes */
        pdata->ts_num = 1;

        ret = devdrv_manager_create_one_device(dev_info);
        if (ret) {
            kfree(pdata);
            pdata = NULL;
            kfree(dev_info);
            dev_info = NULL;
            devdrv_drv_err("devdrv_manager_create_one_device failed, ret(%d). dev_id(%u)\n", ret, dev_id);
            return -ENOMEM;
        }
    } else {
        if (dev_info != NULL)
            pdata = dev_info->pdata;
    }

    devdrv_commn_chan = devdrv_manager_get_common_chan(dev_id);
    if (devdrv_commn_chan->init_notify == NULL) {
        devdrv_manager_release_one_device(dev_info);
        devdrv_drv_err("common chan get failed. dev_id(%u)\n", dev_id);
        kfree(pdata);
        pdata = NULL;
        kfree(dev_info);
        dev_info = NULL;
        return -ENODEV;
    }

    ret = devdrv_register_common_msg_client(devdrv_commn_chan);
    if (ret) {
        devdrv_manager_release_one_device(dev_info);
        devdrv_drv_err("devdrv register common msg channel failed, ret(%d). dev_id(%u)\n", ret, dev_id);
        kfree(pdata);
        pdata = NULL;
        kfree(dev_info);
        dev_info = NULL;
        return -EBUSY;
    }

    dev_ctrl->priv = dev_info;

    if ((init_flag == 1) && (dev_info != NULL)) {
        mutex_init(&dev_info->container.lock);

        devdrv_set_devdrv_info_array(dev_id, dev_info);

        dev_info->plat_type = (u8)DEVDRV_MANAGER_HOST_ENV;
        dev_info->pdata = pdata;

        spin_lock_init(&dev_info->spinlock);
        sema_init(&dev_info->no_trans_chan_wait_sema, 0);
    }
    INIT_WORK(&dev_info->work, devdrv_manager_dev_ready_work);

    /* init a timer for check whether device manager is ready */
#if LINUX_VERSION_CODE >= KERNEL_VERSION(4, 15, 0)
    devdrv_check_start[dev_id].dev_id = dev_id;
    timer_setup(&devdrv_check_start[dev_id].check_timer, devdrv_check_start_event, 0);
    devdrv_check_start[dev_id].check_timer.expires = jiffies + LOAD_DEVICE_TIME * HZ;
    add_timer(&devdrv_check_start[dev_id].check_timer);
#else
    setup_timer(&devdrv_check_start[dev_id], devdrv_check_start_event, (unsigned long)dev_id);
    devdrv_check_start[dev_id].expires = jiffies + LOAD_DEVICE_TIME * HZ;
    add_timer(&devdrv_check_start[dev_id]);
#endif
    /* device online inform user */
    devdrv_manager_online_devid_update(dev_id);

    return 0;
}

STATIC int devdrv_manager_uninit_instance(struct devdrv_client_instance *dev_ctrl)
{
    struct devdrv_common_msg_client *devdrv_commn_chan = NULL;
    struct devdrv_info *dev_info = NULL;
    struct devdrv_cce_ctrl *cce_ctrl = NULL;
    void *no_trans_chan = NULL;
    u32 dev_id;
    int ret;
    u32 tsid;
    u32 ts_num;
    u32 retry_cnt = 0;

    dev_info = (struct devdrv_info *)dev_ctrl->priv;
    dev_id = dev_ctrl->dev_ctrl->dev_id;
    if ((dev_id >= DEVDRV_MAX_DAVINCI_NUM) || (dev_info == NULL)) {
        devdrv_drv_err("invalid dev_id(%u) or dev_info(%pK) is NULL.\n", dev_id, dev_info);
        return -EINVAL;
    }

    dev_info->status = 1;

    ret = down_timeout(&dev_info->no_trans_chan_wait_sema, DEVDRV_INIT_INSTANCE_TIMEOUT);
    if (ret) {
        devdrv_drv_warn("devid %d get sema from init instance timeout, ret:%d\n", dev_id, ret);
    }

    while (retry_cnt < WAIT_PROCESS_EXIT_TIME) {
        if (atomic_read(&dev_info->occupy_ref) == 0) {
            break;
        }
        retry_cnt++;
        msleep(1);
    }
    devdrv_drv_info("dev_id(%u) wait ioctrl retry_cnt %d.\n", dev_id, retry_cnt);

    ts_num = dev_info->ts_num;
    for (tsid = 0; tsid < ts_num; tsid++) {
        cce_ctrl = dev_info->cce_ctrl[tsid];
        if (cce_ctrl != NULL) {
            if (cce_ctrl->wq != NULL) {
                destroy_workqueue(cce_ctrl->wq);
                cce_ctrl->wq = NULL;
            }
        }
    }
    dev_ctrl->priv = NULL;

#if LINUX_VERSION_CODE >= KERNEL_VERSION(4, 15, 0)
    del_timer_sync(&devdrv_check_start[dev_id].check_timer);
#else
    del_timer_sync(&devdrv_check_start[dev_id]);
#endif

    devdrv_heart_beat_exit(dev_info);

    /* uninit common channel */
    devdrv_commn_chan = devdrv_manager_get_common_chan(dev_id);
    devdrv_unregister_common_msg_client(dev_id, devdrv_commn_chan);

    /* unnit non transparent channel */
    no_trans_chan = devdrv_manager_get_no_trans_chan(dev_id);
    if (no_trans_chan != NULL) {
        devdrv_set_msg_chan_priv(no_trans_chan, NULL);
        devdrv_pcimsg_free_non_trans_queue(no_trans_chan);
        no_trans_chan = NULL;
    }

    devdrv_manager_unregister(dev_info);
    return 0;
}

void devdrv_manager_uninit_devinfo(void)
{
    struct devdrv_info *dev_info = NULL;
    u32 i;

    for (i = 0; i < DEVDRV_MAX_DAVINCI_NUM; i++) {
        dev_info = devdrv_get_devdrv_info_array(i);
        if (dev_info == NULL) {
            continue;
        }

        devdrv_manager_release_one_device(dev_info);

        devdrv_set_devdrv_info_array(i, NULL);

        kfree(dev_info->pdata);
        dev_info->pdata = NULL;
        kfree(dev_info);
        dev_info = NULL;
    }
}

struct devdrv_client devdrv_manager_host_instance = {
    .type = DEVDRV_CLIENT_DEVMANAGER,
    .init_instance = devdrv_manager_init_instance,
    .uninit_instance = devdrv_manager_uninit_instance,
};

static const struct pci_device_id devdrv_driver_tbl[] = {{ PCI_VDEVICE(HUAWEI, 0xd100), 0 },
                                                         { PCI_VDEVICE(HUAWEI, PCI_DEVICE_CLOUD), 0 },
                                                         { PCI_VDEVICE(HUAWEI, 0xd801), 0 },
                                                         {}};
MODULE_DEVICE_TABLE(pci, devdrv_driver_tbl);

STATIC struct file_operations devdrv_fops_real = {
    .owner = THIS_MODULE,
    .open = NULL,
    .release = NULL,
    .unlocked_ioctl = NULL,
    .mmap = NULL,
};

void devdrv_manager_set_devdrv_ops(const struct file_operations *ops)
{
    devdrv_fops_real.open = ops->open;
    devdrv_fops_real.release = ops->release;
    devdrv_fops_real.unlocked_ioctl = ops->unlocked_ioctl;
    devdrv_fops_real.mmap = ops->mmap;
}

void devdrv_manager_unset_devdrv_ops(void)
{
    devdrv_fops_real.open = NULL;
    devdrv_fops_real.release = NULL;
    devdrv_fops_real.unlocked_ioctl = NULL;
    devdrv_fops_real.mmap = NULL;
}

STATIC long devdrv_ioctl_fake(struct file *filep, unsigned int cmd, unsigned long arg)
{
    if (devdrv_fops_real.unlocked_ioctl != NULL) {
        return devdrv_fops_real.unlocked_ioctl(filep, cmd, arg);
    }

    return 0;
}

STATIC int devdrv_open_fake(struct inode *inode, struct file *filep)
{
    if (devdrv_fops_real.open != NULL) {
        return devdrv_fops_real.open(inode, filep);
    }

    return -EINVAL;
}

STATIC int devdrv_release_fake(struct inode *inode, struct file *filep)
{
    if (devdrv_fops_real.release != NULL) {
        return devdrv_fops_real.release(inode, filep);
    }

    return 0;
}

STATIC int devdrv_mmap_fake(struct file *filep, struct vm_area_struct *vma)
{
    if (devdrv_fops_real.mmap != NULL) {
        return devdrv_fops_real.mmap(filep, vma);
    }

    return 0;
}

static unsigned int devdrv_major;
static struct class *devdrv_class;
static const struct file_operations devdrv_fops_fake = {
    .owner = THIS_MODULE,
    .open = devdrv_open_fake,
    .release = devdrv_release_fake,
    .unlocked_ioctl = devdrv_ioctl_fake,
    .mmap = devdrv_mmap_fake,
};

STATIC char *devdrv_drv_devnode(struct device *dev, umode_t *mode)
{
    if (mode != NULL)
        *mode = 0660; /* mode is set to 0660 */
    return NULL;
}

STATIC int devdrv_manager_create_driver_device(void)
{
    dev_t devdrv_dev;
    int ret;

    ret = alloc_chrdev_region(&devdrv_dev, 0, DEVDRV_MAX_DAVINCI_NUM, "devdrv-cdev");
    if (ret) {
        devdrv_drv_err("stream alloc_chrdev_region failed, ret(%d).\n", ret);
        return ret;
    }

    devdrv_major = MAJOR(devdrv_dev);
    devdrv_class = class_create(THIS_MODULE, "devdrv-class");
    if (IS_ERR(devdrv_class)) {
        unregister_chrdev_region(devdrv_dev, DEVDRV_MAX_DAVINCI_NUM);
        devdrv_drv_err("class_create failed.\n");
        return -EINVAL;
    }
    devdrv_class->devnode = devdrv_drv_devnode;

    return 0;
}

STATIC void devdrv_manager_release_driver_device(void)
{
    class_destroy(devdrv_class);
    unregister_chrdev_region(MKDEV(devdrv_major, 0), DEVDRV_MAX_DAVINCI_NUM);
}

STATIC int devdrv_manager_create_one_device(struct devdrv_info *dev_info)
{
    struct device *i_device = NULL;
    dev_t devno;
    int ret;

    devno = MKDEV(devdrv_major, dev_info->dev_id);

    dev_info->cce_ops.devdrv_cdev.owner = THIS_MODULE;
    cdev_init(&dev_info->cce_ops.devdrv_cdev, &devdrv_fops_fake);
    ret = cdev_add(&dev_info->cce_ops.devdrv_cdev, devno, DEVDRV_MAX_DAVINCI_NUM);
    if (ret) {
        devdrv_drv_err("cdev_add failed, ret(%d). dev_id(%u)\n", ret, dev_info->dev_id);
        return ret;
    }

    i_device = device_create(devdrv_class, NULL, devno, NULL, "davinci%d", dev_info->dev_id);
    if (IS_ERR(i_device)) {
        cdev_del(&dev_info->cce_ops.devdrv_cdev);
        devdrv_drv_err("device_create failed. dev_id(%u)\n", dev_info->dev_id);
        return -ENODEV;
    }
    ret = devdrv_manager_container_table_devlist_add_ns(devdrv_get_manager_info(), &dev_info->dev_id, 1);
    if (ret) {
        device_destroy(devdrv_class, devno);
        cdev_del(&dev_info->cce_ops.devdrv_cdev);
        devdrv_drv_err("add to list failed. dev_id(%u)\n", dev_info->dev_id);
        return -ENODEV;
    }

    dev_info->cce_ops.cce_dev = i_device;

    return 0;
}

STATIC void devdrv_manager_release_one_device(struct devdrv_info *dev_info)
{
    dev_t devno;

    devno = MKDEV(devdrv_major, dev_info->dev_id);

    if (dev_info->cce_ops.cce_dev != NULL) {
        device_destroy(devdrv_class, devno);
        cdev_del(&dev_info->cce_ops.devdrv_cdev);
        dev_info->cce_ops.cce_dev = NULL;
    }
}

STATIC int __init devdrv_manager_init(void)
{
    struct device *i_device = NULL;
    dev_t devdrv_dev;
    dev_t devno;
    int ret = 0;
    int i;

    devdrv_drv_info("para: \n"
                    "devdrv_heart_beat_en: %d.\n",
                    devdrv_heart_beat_en);

    ret = devdrv_manager_create_driver_device();
    if (ret) {
        devdrv_drv_err("devdrv_manager_create_driver_device return error: %d, "
                       "unable to create davinci device class.\n",
                       ret);
        return ret;
    }

    dev_manager_info = kzalloc(sizeof(*dev_manager_info), GFP_KERNEL);
    if (dev_manager_info == NULL) {
        devdrv_manager_release_driver_device();
        devdrv_drv_err("kzalloc return NULL, failed to alloc mem for manager struct.\n");
        return -ENOMEM;
    }

    spin_lock_init(&dev_manager_info->pm_list_lock);
    INIT_LIST_HEAD(&dev_manager_info->pm_list_header);

    INIT_LIST_HEAD(&dev_manager_info->hostpid_list_header);
    sema_init(&dev_manager_info->devdrv_sign_list_sema, 1);
    (void)memset_s(dev_manager_info->devdrv_sign_count, MAX_DOCKER_NUM + 1, 0, MAX_DOCKER_NUM + 1);

    devdrv_host_black_box_init();

    ret = alloc_chrdev_region(&devdrv_dev, 0, DEVDRV_MAX_DAVINCI_NUM, "devdrv_manager");
    if (ret) {
        devdrv_drv_err("alloc_chrdev_region failed, ret(%d).\n", ret);
        goto alloc_chrdev_fail;
    }

    dev_manager_info->cdev.owner = THIS_MODULE;
    devdrv_manager_major = MAJOR(devdrv_dev);
    devno = MKDEV(devdrv_manager_major, 0);

    devdrv_manager_class = class_create(THIS_MODULE, "devdrv_manager");
    if (IS_ERR(devdrv_manager_class)) {
        devdrv_drv_err("class_create failed.\n");
        ret = -EINVAL;
        goto class_fail;
    }
    devdrv_manager_class->devnode = devdrv_manager_devnode;

    cdev_init(&dev_manager_info->cdev, &devdrv_manager_file_operations);
    ret = cdev_add(&dev_manager_info->cdev, devno, DEVDRV_MAX_DAVINCI_NUM);
    if (ret) {
        devdrv_drv_err("cdev_add failed, ret(%d).\n", ret);
        goto cdev_setup_fail;
    }
    i_device = device_create(devdrv_manager_class, NULL, devno, NULL, "davinci_manager");
    if (i_device == NULL) {
        devdrv_drv_err("device_create failed.\n");
        ret = -ENODEV;
        goto device_create_fail;
    }
    dev_manager_info->prob_num = 0;
    dev_manager_info->dev = i_device;
    dev_manager_info->num_dev = 0;
    dev_manager_info->host_type = devdrv_get_product_type();
    dev_manager_info->drv_ops = &devdrv_host_drv_ops;

#ifdef CONFIG_SYSFS
    ret = sysfs_create_group(&i_device->kobj, &devdrv_manager_attr_group);
    if (ret) {
        devdrv_drv_err("sysfs create failed, ret(%d)\n", ret);
        goto sysfs_create_group_failed;
    }
#endif /* CONFIG_SYSFS */

    spin_lock_init(&dev_manager_info->spinlock);

    for (i = 0; i < DEVDRV_MAX_DAVINCI_NUM; i++) {
        devdrv_manager_set_devdrv_info(i, NULL);
        devdrv_manager_set_no_trans_chan(i, NULL);
        devdrv_set_devdrv_info_array(i, NULL);
    }
    dev_manager_info->plat_info = DEVDRV_MANAGER_HOST_ENV;
    dev_manager_info->dev_rdy_work = create_singlethread_workqueue("dev_manager_workqueue");
    if (dev_manager_info->dev_rdy_work == NULL) {
        devdrv_drv_err("create workqueue failed\n");
        ret = -ENODEV;
        goto devdrv_manager_workqueue_create_failed;
    }
    dev_manager_info->ipc_event_recycle = create_workqueue("dev_manager_ipc_event");
    if (dev_manager_info->ipc_event_recycle == NULL) {
        devdrv_drv_err("create workqueue failed\n");
        ret = -ENODEV;
        goto devdrv_manager_ipcevent_workqueue_create_failed;
    }
    ret = devdrv_manager_container_table_init(dev_manager_info, &init_task);
    if (ret) {
        devdrv_drv_err("container table init failed, ret(%d)\n", ret);
        ret = -ENODEV;
        goto devdrv_manager_container_table_init_failed;
    }
    devdrv_manager_common_chan_init();
    if (devdrv_register_client(&devdrv_manager_host_instance)) {
        devdrv_drv_err("devdrv register client failed\n");
        ret = -ENODEV;
        goto devdrv_register_client_failed;
    }
    ret = devdrv_manager_online_kfifo_alloc();
    if (ret) {
        devdrv_drv_err("kfifo_alloc failed, ret(%d)\n", ret);
        ret = -ENOMEM;
        goto devdrv_register_client_failed;
    }

    drvdrv_dev_startup_register(devdrv_manager_dev_startup_notify);
    drvdrv_dev_state_notifier_register(devdrv_manager_dev_state_notify);
    return 0;

devdrv_register_client_failed:
    devdrv_manager_container_table_exit(dev_manager_info);
devdrv_manager_container_table_init_failed:
    destroy_workqueue(dev_manager_info->ipc_event_recycle);
devdrv_manager_ipcevent_workqueue_create_failed:
    destroy_workqueue(dev_manager_info->dev_rdy_work);
devdrv_manager_workqueue_create_failed:
#ifdef CONFIG_SYSFS
    sysfs_remove_group(&i_device->kobj, &devdrv_manager_attr_group);
sysfs_create_group_failed:
#endif /* CONFIG_SYSFS */
    device_destroy(devdrv_manager_class, devno);
device_create_fail:
    cdev_del(&dev_manager_info->cdev);
cdev_setup_fail:
    class_destroy(devdrv_manager_class);
class_fail:
    unregister_chrdev_region(devdrv_dev, DEVDRV_MAX_DAVINCI_NUM);
alloc_chrdev_fail:
    kfree(dev_manager_info);
    dev_manager_info = NULL;
    devdrv_manager_release_driver_device();
    return ret;
}
module_init(devdrv_manager_init);

STATIC void __exit devdrv_manager_exit(void)
{
    struct list_head *pos = NULL;
    struct list_head *n = NULL;
    struct devdrv_pm *pm = NULL;

#ifdef CONFIG_SYSFS
    sysfs_remove_group(&dev_manager_info->dev->kobj, &devdrv_manager_attr_group);
#endif /* CONFIG_SYSFS */

    devdrv_host_black_box_exit();

    if (!list_empty_careful(&dev_manager_info->pm_list_header)) {
        list_for_each_safe(pos, n, &dev_manager_info->pm_list_header)
        {
            pm = list_entry(pos, struct devdrv_pm, list);
            list_del(&pm->list);
            kfree(pm);
            pm = NULL;
        }
    }

    devdrv_unregister_client(&devdrv_manager_host_instance);
    devdrv_manager_common_chan_uninit();

    devdrv_manager_uninit_devinfo();

    device_destroy(devdrv_manager_class, MKDEV(devdrv_manager_major, 0));
    cdev_del(&dev_manager_info->cdev);
    class_destroy(devdrv_manager_class);
    destroy_workqueue(dev_manager_info->dev_rdy_work);
    destroy_workqueue(dev_manager_info->ipc_event_recycle);
    unregister_chrdev_region(MKDEV(devdrv_manager_major, 0), DEVDRV_MAX_DAVINCI_NUM);
    devdrv_manager_container_table_exit(dev_manager_info);
    kfree(dev_manager_info);
    dev_manager_info = NULL;
    devdrv_manager_online_kfifo_free();

    devdrv_manager_release_driver_device();
}
module_exit(devdrv_manager_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Huawei Tech. Co., Ltd.");
MODULE_DESCRIPTION("DAVINCI driver");

