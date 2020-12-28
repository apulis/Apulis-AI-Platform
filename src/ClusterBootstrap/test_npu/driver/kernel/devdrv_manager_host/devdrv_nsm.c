/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2012-2019. All rights reserved.
 * Description:
 * Author: huawei
 * Create: 2019-10-15
 */
#include <linux/init.h>
#include <linux/module.h>
#include <linux/types.h>
#include <linux/list.h>
#include <linux/hashtable.h>
#include <linux/spinlock.h>
#include <linux/slab.h>
#include <linux/errno.h>
#include <linux/platform_device.h>
#include <linux/vmalloc.h>

#include "devdrv_common.h"
#include "devdrv_nsm.h"
#include "devdrv_driver_adapt.h"
static DEFINE_SPINLOCK(devdrv_hash_notify_lock);

static DEFINE_HASHTABLE(ipc_notify_hashtable, 4); /* hash shifts 4 bits */

STATIC int devdrv_ipc_notify_create(const char *ipc_name, u32 tag, void **ipc_node);
STATIC int devdrv_ipc_notify_find(const char *ipc_name, u32 tag, void **ipc_node);
STATIC int devdrv_ipc_notify_del(const char *ipc_name, u32 tag);

int (*const ipc_create_handler[DEVDRV_IPC_TYPE_MAX])(const char *ipc_name, u32 tag, void **ipc_node) = {
    [DEVDRV_IPC_NOTIFY] = devdrv_ipc_notify_create,
};

int (*const ipc_find_handler[DEVDRV_IPC_TYPE_MAX])(const char *ipc_name, u32 tag, void **ipc_node) = {
    [DEVDRV_IPC_NOTIFY] = devdrv_ipc_notify_find,
};

int (*const ipc_del_handler[DEVDRV_IPC_TYPE_MAX])(const char *ipc_name, u32 tag) = {
    [DEVDRV_IPC_NOTIFY] = devdrv_ipc_notify_del,
};
STATIC u32 ipc_name_ref = 0;

int devdrv_creat_ipc_name(char *ipc_name, unsigned int len)
{
    char random[RANDOM_LENGTH] = {0};
    int offset;
    pid_t tgid;
    int ret;
    int i;

    if ((ipc_name == NULL) || (len < DEVDRV_IPC_NAME_SIZE)) {
        devdrv_drv_err("ipc is null(%d) or invalid len(%u).\n", (ipc_name == NULL), len);
        return -EINVAL;
    }
    ret = devdrv_get_random(random, RANDOM_LENGTH);
    if (ret) {
        devdrv_drv_err("get random failed, ret(%d).\n", ret);
        return -EINVAL;
    }

    tgid = current->tgid;
    offset = snprintf_s(ipc_name, DEVDRV_IPC_NAME_SIZE, DEVDRV_IPC_NAME_SIZE - 1, "%08x%08x", tgid, ipc_name_ref);
    if (offset < 0) {
        (void)memset_s(random, RANDOM_LENGTH, 0, RANDOM_LENGTH);
        devdrv_drv_err("snprintf failed, ret(%d).\n", ret);
        return -EINVAL;
    }
    for (i = 0; i < RANDOM_LENGTH; i++) {
        ret = snprintf_s(ipc_name + offset, DEVDRV_IPC_NAME_SIZE - offset, DEVDRV_IPC_NAME_SIZE - 1 - offset,
            "%02x", (u8)random[i]);
        if (ret < 0) {
            (void)memset_s(random, RANDOM_LENGTH, 0, RANDOM_LENGTH);
            devdrv_drv_err("snprintf failed, ret(%d).\n", ret);
            return -EINVAL;
        }
        offset += ret;
    }
    ipc_name[DEVDRV_IPC_NAME_SIZE - 1] = '\0';
    atomic_inc((atomic_t *)&ipc_name_ref);
    (void)memset_s(random, RANDOM_LENGTH, 0, RANDOM_LENGTH);
    return 0;
}

EXPORT_SYMBOL(devdrv_creat_ipc_name);

u32 devdrv_mc_get_str_elfhash(const char *ipc_name, size_t len)
{
    u32 hash = 0;
    u32 tmp_hash = 0;
    u32 tmp_len = 0;
    const u8 *ipc_char = (u8 *)ipc_name;

    while ((*ipc_char != 0) && (tmp_len < len)) {
        hash = (hash << 4) + *ipc_char; /* hash shifts left 4 bits */
        tmp_hash = hash & 0xF0000000;

        if (!tmp_hash) {
            hash ^= tmp_hash >> 24; /* tmp_hash shifts right 24 bits */
            hash ^= tmp_hash;
        }
        ipc_char++;
        tmp_len += sizeof(u8);
    }

    return hash;
}

int devdrv_ipc_notify_create(const char *ipc_name, u32 tag, void **ipc_node)
{
    struct ipc_notify_node *notify_node = NULL;
    int i;

    spin_lock(&devdrv_hash_notify_lock);
    /*lint -e666 */
    hash_for_each_possible(ipc_notify_hashtable, notify_node, link, tag)
        if (!strcmp(notify_node->name, ipc_name)) {
        spin_unlock(&devdrv_hash_notify_lock);
        return -EINVAL;
    }
    notify_node = kzalloc(sizeof(*notify_node), GFP_ATOMIC);
    if (notify_node == NULL) {
        spin_unlock(&devdrv_hash_notify_lock);
        return -ENOMEM;
    }

    for (i = 0; i < DEVDRV_IPC_NAME_SIZE; i++) {
        notify_node->name[i] = ipc_name[i];
    }

    for (i = 0; i < DEVDRV_PID_MAX_NUM; i++) {
        notify_node->pid[i] = 0;
    }

    hash_add(ipc_notify_hashtable, &notify_node->link, tag);
    /*lint +e666 */
    spin_unlock(&devdrv_hash_notify_lock);

    *ipc_node = (void *)notify_node;

    return 0;
}

int devdrv_ipc_create(char *ipc_name, void **ipc_node, enum devdrv_ipc_type ipc_type)
{
    u32 tag;
    int ret;

    if ((ipc_name == NULL) || (ipc_node == NULL)) {
        return -EINVAL;
    }
    if ((ipc_type >= DEVDRV_IPC_TYPE_MAX) || (ipc_create_handler[ipc_type] == NULL)) {
        devdrv_drv_err("devdrv_ipc_type = %d\n", ipc_type);
        return -ENODEV;
    }

    ret = devdrv_creat_ipc_name(ipc_name, DEVDRV_IPC_NAME_SIZE);
    if (ret) {
        devdrv_drv_err("ipc name creat failed, ret(%d).\n", ret);
        return -EINVAL;
    }
    tag = devdrv_mc_get_str_elfhash(ipc_name, strlen(ipc_name) + 1);
    ret = ipc_create_handler[ipc_type](ipc_name, tag, ipc_node);

    return ret;
}

STATIC int devdrv_ipc_notify_find(const char *ipc_name, u32 tag, void **ipc_node)
{
    struct ipc_notify_node *notify_node = NULL;

    spin_lock(&devdrv_hash_notify_lock);
    /*lint -e666 */
    hash_for_each_possible(ipc_notify_hashtable, notify_node, link, tag)
        if (!strcmp(notify_node->name, ipc_name)) {
        spin_unlock(&devdrv_hash_notify_lock);
        *ipc_node = notify_node;
        return 0;
    }
    /*lint +e666 */
    spin_unlock(&devdrv_hash_notify_lock);

    return -EINVAL;
}

int devdrv_ipc_find(const char *ipc_name, void **ipc_node, enum devdrv_ipc_type ipc_type)
{
    u32 tag;
    int ret;

    if ((ipc_name == NULL) || (ipc_node == NULL)) {
        return -EINVAL;
    }
    if ((ipc_type >= DEVDRV_IPC_TYPE_MAX) || (ipc_find_handler[ipc_type] == NULL)) {
        devdrv_drv_err("invalid parameter, ipc_type = %d\n", ipc_type);
        return -ENODEV;
    }

    tag = devdrv_mc_get_str_elfhash(ipc_name, strlen(ipc_name) + 1);
    ret = ipc_find_handler[ipc_type](ipc_name, tag, ipc_node);

    return ret;
}

STATIC int devdrv_ipc_notify_del(const char *ipc_name, u32 tag)
{
    struct ipc_notify_node *notify_node = NULL;

    spin_lock(&devdrv_hash_notify_lock);
    /*lint -e666 */
    hash_for_each_possible(ipc_notify_hashtable, notify_node, link, tag)
        if (!strcmp(notify_node->name, ipc_name)) {
        hash_del(&notify_node->link);
        spin_unlock(&devdrv_hash_notify_lock);
        goto node_free;
    }
    /*lint +e666 */
    spin_unlock(&devdrv_hash_notify_lock);

    return -EINVAL;

node_free:
    (void)memset_s(notify_node->name, DEVDRV_IPC_NAME_SIZE, 0, DEVDRV_IPC_NAME_SIZE);
    kfree(notify_node);
    notify_node = NULL;
    return 0;
}

int devdrv_ipc_del(const char *ipc_name, u32 max_len, enum devdrv_ipc_type ipc_type)
{
    u32 tag;
    int ret;

    if (ipc_name == NULL)
        return -EINVAL;
    if ((ipc_type >= DEVDRV_IPC_TYPE_MAX) || (ipc_del_handler[ipc_type] == NULL) || (strlen(ipc_name) >= max_len)) {
        devdrv_drv_err("invalid parameter, devdrv_ipc_type = %d\n", ipc_type);
        return -ENODEV;
    }

    tag = devdrv_mc_get_str_elfhash(ipc_name, strlen(ipc_name) + 1);
    ret = ipc_del_handler[ipc_type](ipc_name, tag);

    return ret;
}
