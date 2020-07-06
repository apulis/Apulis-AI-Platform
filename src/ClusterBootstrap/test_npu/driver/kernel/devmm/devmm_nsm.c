/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2012-2019. All rights reserved.
 * Description: This file contains api definition for memory manager driver.
 * Author: huawei
 * Create: 2019-06-11
 */
#include <linux/types.h>
#include <linux/list.h>
#include <linux/hashtable.h>
#include <linux/spinlock.h>
#include <linux/slab.h>
#include <linux/errno.h>
#include <linux/semaphore.h>
#include <securec.h>

#include "devmm_dev.h"
#include "devmm_nsm.h"
#include "devmm_proc_info.h"

static DEFINE_SPINLOCK(devmm_hash_lock);
static DEFINE_HASHTABLE(ipc_mem_hashtable, DEVMM_HASH_LIST_NUM_SHIT);

STATIC u32 devmm_mc_get_str_elfhash(const u8 *ipc_name)
{
    u32 hash = 0;
    u32 tmp_hash = 0;
    const u8 *ipc_char = ipc_name;

    while (*ipc_char) {
        hash = (hash << DEVMM_HASH_SHIT) + *ipc_char;
        tmp_hash = hash & DEVMM_HASH_MASK;

        if (!tmp_hash) {
            hash ^= tmp_hash >> DEVMM_TMP_HASH_SHIT;
            hash ^= tmp_hash;
        }
        ipc_char++;
    }
    return hash;
}

STATIC int devmm_ipc_mem_create(const char *ipc_name, u32 tag, struct ipc_mem_node **ipc_node)
{
    struct ipc_mem_node *mem_node = NULL;
    int i;
    spin_lock(&devmm_hash_lock);
    hash_for_each_possible(ipc_mem_hashtable, mem_node, link, tag) //lint !e666
    {
        if (!strcmp(mem_node->name, ipc_name)) {
            spin_unlock(&devmm_hash_lock);
            devmm_drv_err("create args error.\n");
            return -EEXIST;
        }
    }
    mem_node = kzalloc(sizeof(*mem_node), GFP_ATOMIC);
    if (mem_node == NULL) {
        spin_unlock(&devmm_hash_lock);
        devmm_drv_err("kzalloc failed.\n");
        return -ENOMEM;
    }

    for (i = 0; i < DEVMM_IPC_MEM_NAME_SIZE; i++) {
        mem_node->name[i] = ipc_name[i];
    }
    hash_add(ipc_mem_hashtable, &mem_node->link, tag); //lint !e666
    spin_unlock(&devmm_hash_lock);

    *ipc_node = (void *)mem_node;

    return 0;
}

int devmm_ipc_create(const char *ipc_name, struct ipc_mem_node **ipc_node)
{
    u32 tag = devmm_mc_get_str_elfhash((u8 *)ipc_name);
    return devmm_ipc_mem_create(ipc_name, tag, ipc_node);
}

STATIC int devmm_ipc_mem_find(const char *ipc_name, u32 tag, struct ipc_mem_node **ipc_node)
{
    struct ipc_mem_node *mem_node = NULL;
    spin_lock(&devmm_hash_lock);
    hash_for_each_possible(ipc_mem_hashtable, mem_node, link, tag) //lint !e666
    {
        if (!strcmp(mem_node->name, ipc_name)) {
            atomic_inc(&mem_node->ref);
            spin_unlock(&devmm_hash_lock);
            *ipc_node = mem_node;
            devmm_drv_switch("exit suc. find idx(%d).\n", mem_node->sequence);
            return 0;
        }
    }
    spin_unlock(&devmm_hash_lock);

    return -EINVAL;
}

int devmm_ipc_get_node(const char *ipc_name, struct ipc_mem_node **ipc_node)
{
    u32 tag = devmm_mc_get_str_elfhash((u8 *)ipc_name);
    return devmm_ipc_mem_find(ipc_name, tag, ipc_node);
}

void devmm_ipc_put_node(struct ipc_mem_node *ipc_node)
{
    spin_lock(&devmm_hash_lock);
    if (atomic_dec_return(&ipc_node->ref) == 0) {
        hash_del(&ipc_node->link);
        spin_unlock(&devmm_hash_lock);
        devmm_drv_switch("del suc. del idx(%d).\n", ipc_node->sequence);
        devmm_drv_err_if(memset_s(ipc_node->name, sizeof(ipc_node->name), 0, sizeof(ipc_node->name)),
            "memset err");
        kfree(ipc_node);
        ipc_node = NULL;
        return;
    }
    spin_unlock(&devmm_hash_lock);
}
