/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2012-2019. All rights reserved.
 * Description:
 * Author: huawei
 * Create: 2019/6/18
 *
 * This program is free software; you can redistribute it and /or modify it
 * under the terms of the GNU General Public License as published by the Free
 * Software Foundation; either version 2 of the License, or (at your option)
 * any later version
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
#include <linux/version.h>
#include <linux/jiffies.h>
#include <linux/ioctl.h>
#include <linux/device.h>
#include <linux/dma-mapping.h>
#include <linux/slab.h>
#include <linux/fs.h>
#include <asm/uaccess.h>
#include <linux/wait.h>
#include <linux/sched.h>
#include <linux/delay.h>
#include <linux/uaccess.h>
#include <linux/mm.h>
#include <asm/pgtable.h>
#include <asm/ptrace.h>
#include <linux/crc32.h>
#include <linux/swap.h>
#include <linux/vmalloc.h>

#include "hdcdrv_core.h"

u32 mem_type = HDCDRV_NORMAL_MEM;
u32 mem_work_flag = 0;
u64 mem_work_cnt = 0;


struct hdcdrv_mem_pool *get_pool(int pool_type, int dev_id, u32 data_len)
{
        struct hdcdrv_dev *dev = &hdc_ctrl->devices[dev_id];
        struct hdcdrv_mem_pool *pool = NULL;

        if (data_len <= (HDCDRV_SMALL_PACKET_SEGMENT - HDCDRV_MEM_BLOCK_HEAD_SIZE)) {
                pool = &dev->small_mem_pool[pool_type];
        } else {
                pool = &dev->huge_mem_pool[pool_type];
        }

        return pool;
}

struct hdcdrv_mem_pool *get_pool_by_segment(int pool_type, int dev_id, u32 seg_len)
{
    struct hdcdrv_dev *dev = &hdc_ctrl->devices[dev_id];
    struct hdcdrv_mem_pool *pool = NULL;

    if (seg_len <= HDCDRV_SMALL_PACKET_SEGMENT) {
            pool = &dev->small_mem_pool[pool_type];
    } else {
            pool = &dev->huge_mem_pool[pool_type];
    }

    return pool;
}

void free_mem_pool_single(struct device *dev, u32 segment, struct hdcdrv_mem_block_head *buf, dma_addr_t addr)
{
    if ((buf != NULL) && (addr != 0) && (dev != NULL)) {
        dma_free_coherent(dev, segment, (void *)buf, addr);
        buf = NULL;
        addr = 0;
    }
}

int alloc_mem(int pool_type, int dev_id, int len, void **buf, dma_addr_t *addr, struct list_head *wait_head)
{
    struct hdcdrv_dev *dev = &hdc_ctrl->devices[dev_id];
    struct hdcdrv_mem_pool *pool = get_pool(pool_type, dev_id, len);
    struct hdcdrv_mem_block_head *block_head = NULL;
    int ring_id;

    if (dev->valid != HDCDRV_VALID) {
        hdcdrv_err("dev id %d is invalid\n", dev_id);
        return HDCDRV_DEVICE_NOT_READY;
    }

    spin_lock_bh(&pool->mem_lock);

    if (pool->head == pool->tail) {
        if (wait_head != NULL) {
            if (wait_head->next == NULL) {
                list_add_tail(wait_head, &pool->wait_list);
            }
        }

        spin_unlock_bh(&pool->mem_lock);
        return HDCDRV_DMA_MEM_ALLOC_FAIL;
    }

    ring_id = pool->head & pool->mask;

    if (hdcdrv_mem_block_head_check(pool->ring[ring_id].buf) != HDCDRV_OK) {
        spin_unlock_bh(&pool->mem_lock);
        hdcdrv_err("pool_type %d device %d block head check failed\n", pool_type, dev_id);
        return HDCDRV_MEM_NOT_MATCH;
    }

    block_head = HDCDRV_BLOCK_HEAD(pool->ring[ring_id].buf);
    if (block_head->ref_count != 0) {
        spin_unlock_bh(&pool->mem_lock);
        hdcdrv_mem_block_head_dump(block_head);
        hdcdrv_err("memory block ref_count error, current %x, expect 0\n", block_head->ref_count);
        return HDCDRV_MEM_NOT_MATCH;
    }
    block_head->ref_count++;

    *buf = pool->ring[ring_id].buf;
    *addr = block_head->dma_addr;

    pool->ring[ring_id].buf = NULL;

    pool->head++;
    spin_unlock_bh(&pool->mem_lock);

    return HDCDRV_OK;
}


void free_mem(void *buf)
{
    struct hdcdrv_mem_block_head *block_head = NULL;
    struct hdcdrv_mem_pool *pool = NULL;
    struct list_head *list = NULL;
    int ring_id;

    if (hdcdrv_mem_block_head_check(buf) != HDCDRV_OK) {
        hdcdrv_err("block head check failed\n");
        return;
    }

    block_head = HDCDRV_BLOCK_HEAD(buf);

    pool = get_pool(block_head->type, block_head->devid, block_head->size);
    if (pool->ring == NULL) {
        hdcdrv_warn("pool ring  has freed\n");
        return;
    }

    spin_lock_bh(&pool->mem_lock);

    if (block_head->ref_count != 1) {
        spin_unlock_bh(&pool->mem_lock);
        hdcdrv_mem_block_head_dump(block_head);
        hdcdrv_err("memory block ref_count error, current %x, expect 1\n", block_head->ref_count);
        return;
    }
    block_head->ref_count--;

    ring_id = pool->tail & pool->mask;
    pool->ring[ring_id].buf = buf;

    pool->tail++;

    if (!list_empty_careful(&pool->wait_list)) {
        list = pool->wait_list.next;
        list_del(list);
        list->next = NULL;
        list->prev = NULL;
        hdcdrv_mem_avail(block_head->type, list);
    }

    spin_unlock_bh(&pool->mem_lock);
}

int alloc_mem_pool(int pool_type, int dev_id, u32 segment, u32 num)
{
    struct hdcdrv_mem_pool *pool = get_pool_by_segment(pool_type, dev_id, segment);
    struct hdcdrv_dev *hdc_dev = &hdc_ctrl->devices[dev_id];
    dma_addr_t addr = 0;
    void *buf = NULL;
    gfp_t gfp;
    u32 i;

    pool->segment = segment;
    pool->mask = num - 1;
    pool->head = 0;
    pool->size = 0;
    pool->ring = (struct hdcdrv_mem *)kzalloc(sizeof(struct hdcdrv_mem) * num, GFP_KERNEL);
    if (pool->ring == NULL) {
        pool->tail = 0;
        return HDCDRV_DMA_MEM_ALLOC_FAIL;
    }

#ifdef HDC_ENV_DEVICE
    /* no declare __GFP_RECLAIM  (GFP_KERNEL) to forbidden blocking */
    /* The application length is less than 4M, the cma memory is
            not needed(Leave cma to others) */
    gfp = GFP_NOWAIT;
#else
    /* declare __GFP_RECLAIM (GFP_KERNEL) to allow blocking, alloc cma firstly */
    gfp = GFP_KERNEL;
#endif

    INIT_LIST_HEAD(&pool->wait_list);
    spin_lock_init(&pool->mem_lock);

    for (i = 0; i < num; i++) {
        buf = dma_alloc_coherent(hdc_dev->dev, pool->segment, &addr, gfp);
        if (unlikely(buf == NULL)) {
            hdcdrv_err("dev_id %d mem pool(%x) alloc failed!\r\n", dev_id, pool->segment);
            pool->tail = pool->size;
            return HDCDRV_DMA_MEM_ALLOC_FAIL;
        }

        hdcdrv_mem_block_head_init(buf, addr, pool_type, dev_id, segment);
        pool->ring[i].buf = HDCDRV_BLOCK_BUFFER(buf);
        pool->size++;
    }

    pool->tail = pool->size;

    hdcdrv_info("dev_id %d, segment %u, mem pool real size is %u\n", dev_id, segment, pool->size);
    return HDCDRV_OK;
}

void free_mem_pool(int pool_type, int dev_id, u32 segment)
{
    struct hdcdrv_mem_pool *pool = get_pool_by_segment(pool_type, dev_id, segment);
    struct hdcdrv_dev* hdc_dev = &hdc_ctrl->devices[dev_id];
    struct hdcdrv_mem_block_head *block_head = NULL;
    int ring_id;
    u32 i;

    if (pool->ring == NULL) {
        hdcdrv_err("dev %d pool->ring is NULL\n", dev_id);
        return;
    }

    if ((pool->tail - pool->head) != pool->size) {
        hdcdrv_info("dev %d mem pool segment %d alloc mem %d, now: head %lld tail %lld\n", dev_id, pool->segment,
            pool->size, pool->head, pool->tail);
    }

    for (i = 0; i < pool->tail - pool->head; i++) {
        ring_id = (pool->head + i) % pool->size;
        if (hdcdrv_mem_block_head_check(pool->ring[ring_id].buf) == HDCDRV_OK) {
            block_head = HDCDRV_BLOCK_HEAD(pool->ring[ring_id].buf);
            free_mem_pool_single(hdc_dev->dev, segment, block_head, HDCDRV_BLOCK_DMA_HEAD(block_head->dma_addr));
        }
        pool->ring[ring_id].buf = NULL;
    }

    kfree(pool->ring);
    pool->ring = NULL;
}

static inline u32 hdcdrv_mem_block_head_crc32(struct hdcdrv_mem_block_head *block_head)
{
    return crc32_le(~0, (unsigned char *)block_head, HDCDRV_BLOCK_CRC_LEN);
}

void hdcdrv_mem_block_head_init(void *buf, dma_addr_t addr, int pool_type, int dev_id, u32 segment)
{
    struct hdcdrv_mem_block_head *block_head = (struct hdcdrv_mem_block_head *)buf;

    block_head->magic = HDCDRV_MEM_BLOCK_MAGIC;
    block_head->devid  = dev_id;
    block_head->type = pool_type;
    block_head->size = segment - HDCDRV_MEM_BLOCK_HEAD_SIZE;
    block_head->dma_addr = HDCDRV_BLOCK_DMA_BUFFER(addr);
    block_head->head_crc = hdcdrv_mem_block_head_crc32(block_head);
    block_head->ref_count = 0;
}

void hdcdrv_mem_block_head_dump(struct hdcdrv_mem_block_head *block_head)
{
    hdcdrv_err("critical error! memory block head is corrupted:\n"
               "magic : %x\n"
               "devid : %x\n"
               "type : %x\n"
               "size : %x\n"
               "dma_addr : (no print)\n"
               "head_crc : %x\n"
               "ref_count : %x\n"
               "current crc : %x\n",
               block_head->magic, block_head->devid, block_head->type,
               block_head->size, block_head->head_crc, block_head->ref_count,
               hdcdrv_mem_block_head_crc32(block_head));
    dump_stack();
}

int hdcdrv_mem_block_head_check(void *buf)
{
    struct hdcdrv_mem_block_head *block_head = NULL;

    if (buf == NULL) {
        hdcdrv_err("memory block buffer is null\n");
        return HDCDRV_ERR;
    }

    block_head = HDCDRV_BLOCK_HEAD(buf);
    if (block_head == NULL) {
        hdcdrv_err("memory block buffer head is null\n");
        return HDCDRV_ERR;
    }

    if ((block_head->magic != HDCDRV_MEM_BLOCK_MAGIC) ||
        (block_head->head_crc != hdcdrv_mem_block_head_crc32(block_head))) {
        hdcdrv_mem_block_head_dump(block_head);
        hdcdrv_err("memory block head check failed\n");
        return HDCDRV_ERR;
    }

    return HDCDRV_OK;
}

int hdcdrv_mem_block_capacity(void)
{
    return hdc_ctrl->segment - HDCDRV_MEM_BLOCK_HEAD_SIZE;
}

int hdcdrv_init_mem_pool(u32 dev_id)
{
        int ret;

        ret = alloc_mem_pool(HDCDRV_MEM_POOL_TYPE_TX, dev_id,
                             HDCDRV_SMALL_PACKET_SEGMENT, HDCDRV_SMALL_PACKET_NUM);
        if (ret != HDCDRV_OK) {
                goto free_small_tx;
        }

        ret = alloc_mem_pool(HDCDRV_MEM_POOL_TYPE_RX, dev_id,
                             HDCDRV_SMALL_PACKET_SEGMENT, HDCDRV_SMALL_PACKET_NUM);
        if (ret != HDCDRV_OK) {
                goto free_small_rx;
        }

        ret = alloc_mem_pool(HDCDRV_MEM_POOL_TYPE_TX, dev_id,
                             hdc_ctrl->segment, HDCDRV_HUGE_PACKET_NUM);
        if (ret != HDCDRV_OK) {
                goto free_huge_tx;
        }

        ret = alloc_mem_pool(HDCDRV_MEM_POOL_TYPE_RX, dev_id,
                             hdc_ctrl->segment, HDCDRV_HUGE_PACKET_NUM);
        if (ret != HDCDRV_OK) {
                goto free_huge_rx;
        }

        return HDCDRV_OK;

free_huge_rx:
        free_mem_pool(HDCDRV_MEM_POOL_TYPE_RX, dev_id, hdc_ctrl->segment);
free_huge_tx:
        free_mem_pool(HDCDRV_MEM_POOL_TYPE_TX, dev_id, hdc_ctrl->segment);
free_small_rx:
        free_mem_pool(HDCDRV_MEM_POOL_TYPE_RX, dev_id, HDCDRV_SMALL_PACKET_SEGMENT);
free_small_tx:
        free_mem_pool(HDCDRV_MEM_POOL_TYPE_TX, dev_id, HDCDRV_SMALL_PACKET_SEGMENT);

        return HDCDRV_ERR;
}

int hdcdrv_va_to_pa(struct mm_struct *mm, unsigned long long va, dma_addr_t *pa)
{
#ifdef HDC_ENV_DEVICE
    pgd_t *pgd = NULL;
    pud_t *pud = NULL;
    pmd_t *pmd = NULL;
    pgd = NULL;
    pud = NULL;
    pmd = NULL;
    pgd = pgd_offset(mm, va);
    if ((pgd == NULL) || pgd_none(*pgd) || pgd_bad(*pgd)) {
        hdcdrv_err("get pgd failed, 0x%llx\n", va);
        return HDCDRV_PARA_ERR;
    }
    pud = pud_offset(pgd, va);
    if ((pud == NULL) || pud_none(*pud) || pud_bad(*pud)) {
        hdcdrv_err("get pud failed, 0x%llx\n", va);
        return HDCDRV_PARA_ERR;
    }
    pmd = pmd_offset(pud, va);
    if ((pmd == NULL) || pmd_none(*pmd) || !pte_huge(*(pte_t *)pmd)) {
        hdcdrv_err("get pmd failed, 0x%llx\n", va);
        return HDCDRV_PARA_ERR;
    }
    *pa = (dma_addr_t)PFN_PHYS(pte_pfn(*(pte_t *)pmd));
#endif
    return HDCDRV_OK;
}
struct hdcdrv_fast_node *hdcdrv_fast_node_search(struct rb_root *root, u64 hash_va, u32 *status, int *node_stamp)
{
    struct rb_node *node = NULL;
    struct hdcdrv_fast_node *fast_node = NULL;

    spin_lock_bh(&hdc_ctrl->lock);

    node = root->rb_node;
    while (node != NULL) {
        fast_node = rb_entry(node, struct hdcdrv_fast_node, node);

        if (hash_va < fast_node->hash_va) {
            node = node->rb_left;
        } else if (hash_va > fast_node->hash_va) {
            node = node->rb_right;
        } else {
            if (hdcdrv_node_is_busy(fast_node)) {
                *status = HDCDRV_NODE_BUSY;
            } else {
                hdcdrv_node_status_busy(fast_node);
                *status = HDCDRV_NODE_IDLE;
            }
            *node_stamp = fast_node->stamp;
            spin_unlock_bh(&hdc_ctrl->lock);
            return fast_node;
        }
    }

    spin_unlock_bh(&hdc_ctrl->lock);
    return NULL;
}

struct hdcdrv_fast_node *hdcdrv_fast_node_search_timeout(struct rb_root *root, u64 hash_va, int timeout)
{
    struct hdcdrv_fast_node *fast_node = NULL;
    int loop_cnt = timeout;
    u32 node_status = 0;
    int node_stamp = 0;

    do {
        fast_node = hdcdrv_fast_node_search(root, hash_va, &node_status, &node_stamp);
        if (fast_node == NULL) {
            return NULL;
        }

        if (node_status == HDCDRV_NODE_IDLE) {
            return fast_node;
        }

        /* node busy */
        if (hdcdrv_node_is_timeout(node_stamp)) {
            hdcdrv_err("fast node timeout, busy for %d ms, loop_cnt %d.\n",
                       (u32)jiffies_to_msecs(jiffies - node_stamp), loop_cnt);
            return NULL;
        }

        if (loop_cnt > 0) {
            msleep(1);
        }
    } while (loop_cnt--);

    hdcdrv_err("fast node search failed for busy, timeout %d.\n", timeout);
    return NULL;
}


int hdcdrv_fast_node_insert(struct rb_root *root, struct hdcdrv_fast_node *fast_node)
{
    struct rb_node *parent = NULL;
    struct rb_node **new_node = NULL;

    spin_lock_bh(&hdc_ctrl->lock);

    new_node = &(root->rb_node);

    /* Figure out where to put new node */
    while (*new_node) {
        struct hdcdrv_fast_node *this = rb_entry(*new_node, struct hdcdrv_fast_node, node);

        parent = *new_node;
        if (fast_node->hash_va < this->hash_va) {
            new_node = &((*new_node)->rb_left);
        } else if (fast_node->hash_va > this->hash_va) {
            new_node = &((*new_node)->rb_right);
        } else {
            spin_unlock_bh(&hdc_ctrl->lock);
            return HDCDRV_F_NODE_SEARCH_FAIL;
        }
    }

    /* Add new node and rebalance tree. */
    rb_link_node(&fast_node->node, parent, new_node);
    rb_insert_color(&fast_node->node, root);

    hdcdrv_node_status_idle(fast_node);

    spin_unlock_bh(&hdc_ctrl->lock);

#ifdef HDCDRV_DEBUG
    hdcdrv_info("mem type %d, hash_va 0x%llx, user_va 0x%llx, phy num %d, addr0 %llx, len %u\n",
                fast_node->fast_mem.mem_type, fast_node->hash_va, fast_node->fast_mem.user_va,
                fast_node->fast_mem.phy_addr_num, fast_node->fast_mem.mem[0].addr, fast_node->fast_mem.mem[0].len);
#endif
    return HDCDRV_OK;
}


void hdcdrv_fast_node_erase(spinlock_t *lock, struct rb_root *root, struct hdcdrv_fast_node *fast_node)
{
    if (lock != NULL)
        spin_lock_bh(lock);

    rb_erase(&fast_node->node, root);

    if (lock != NULL)
        spin_unlock_bh(lock);
}

void hdcdrv_fast_node_free(struct hdcdrv_fast_node *fast_node)
{
    kfree(fast_node);
    fast_node = NULL;
}

#ifdef HDC_MEM_RB_DFX
struct hdcdrv_mem_dfx_node *hdcdrv_mem_dfx_rb_search(struct rb_root *root, u64 hash_va)
{
    struct rb_node *node = NULL;
    struct hdcdrv_mem_dfx_node *mem_node = NULL;

    spin_lock_bh(&hdc_ctrl->dfx_lock);

    node = root->rb_node;
    while (node != NULL) {
        mem_node = rb_entry(node, struct hdcdrv_mem_dfx_node, node);

        if (hash_va < mem_node->hash_va) {
            node = node->rb_left;
        } else if (hash_va > mem_node->hash_va) {
            node = node->rb_right;
        } else {
            spin_unlock_bh(&hdc_ctrl->dfx_lock);
            return mem_node;
        }
    }
    spin_unlock_bh(&hdc_ctrl->dfx_lock);
    return NULL;
}

int hdcdrv_mem_dfx_rb_insert(struct rb_root *root, struct hdcdrv_mem_dfx_node *mem_node)
{
    struct rb_node *parent = NULL;
    struct rb_node **new_node = NULL;

    spin_lock_bh(&hdc_ctrl->dfx_lock);

    new_node = &(root->rb_node);
    /* Figure out where to put new node */
    while (*new_node) {
        struct hdcdrv_mem_dfx_node *this = rb_entry(*new_node, struct hdcdrv_mem_dfx_node, node);

        parent = *new_node;
        if (mem_node->hash_va < this->hash_va) {
            new_node = &((*new_node)->rb_left);
        } else if (mem_node->hash_va > this->hash_va) {
            new_node = &((*new_node)->rb_right);
        } else {
            spin_unlock_bh(&hdc_ctrl->dfx_lock);
            return HDCDRV_ERR;
        }
    }

    /* Add new node and rebalance tree. */
    rb_link_node(&mem_node->node, parent, new_node);
    rb_insert_color(&mem_node->node, root);
    spin_unlock_bh(&hdc_ctrl->dfx_lock);
    return HDCDRV_OK;
}

int hdcdrv_dfx_node_insert_fn(struct rb_root *root, u64 hash_va, int flag)
{
    struct hdcdrv_mem_dfx_node *mem_node = NULL;
    int ret;

    mem_node = hdcdrv_mem_dfx_rb_search(root, hash_va);
    if (mem_node != NULL) {
        spin_lock_bh(&hdc_ctrl->dfx_lock);
        mem_node->mem_rbtree_state = HDCDRV_MEM_DFX_ALLOC_USED;
        mem_node->dfx_current_time = jiffies;
        spin_unlock_bh(&hdc_ctrl->dfx_lock);
        return HDCDRV_OK;
    }

    mem_node = (struct hdcdrv_mem_dfx_node *)kzalloc(sizeof(struct hdcdrv_mem_dfx_node), GFP_KERNEL | __GFP_ACCOUNT);
    if (mem_node == NULL) {
        hdcdrv_err("[DFX] m_node kzalloc failed!\n");
        return HDCDRV_ERR;
    }

    mem_node->mem_rbtree_state = HDCDRV_MEM_DFX_ALLOC_USED;
    mem_node->dfx_current_time = jiffies;
    mem_node->pid = hdcdrv_get_pid();
    mem_node->hash_va = hash_va;

    ret = hdcdrv_mem_dfx_rb_insert(root, mem_node);
    if (ret != HDCDRV_OK) {
        kfree(mem_node);
        mem_node = NULL;
        hdcdrv_info("[DFX] mem node insert failed!\n");
        return HDCDRV_ERR;
    }

    spin_lock_bh(&hdc_ctrl->dfx_lock);
    if (flag == HDCDRV_MEM_DFX_LOCAL) {
        hdc_ctrl->dfx_mem_max_count++;
        hdc_ctrl->dfx_mem_current_count++;
    } else {
        hdc_ctrl->dfx_mem_remote_max_count++;
        hdc_ctrl->dfx_mem_remote_current_count++;
    }
    spin_unlock_bh(&hdc_ctrl->dfx_lock);
    return HDCDRV_OK;
}

void hdcdrv_dfx_node_upgrade_fn(struct rb_root *root, u64 hash_va, int state)
{
    struct hdcdrv_mem_dfx_node *mem_node = NULL;
    mem_node = hdcdrv_mem_dfx_rb_search(root, hash_va);
    if (mem_node != NULL) {
        spin_lock_bh(&hdc_ctrl->dfx_lock);
        mem_node->mem_rbtree_state = state;
        mem_node->dfx_current_time = jiffies;
        spin_unlock_bh(&hdc_ctrl->dfx_lock);
    }
}

int hdcdrv_dfx_node_free_check(struct hdcdrv_mem_dfx_node *mem_node, u64 dfx_time)
{
    return ((mem_node->mem_rbtree_state != HDCDRV_MEM_DFX_ALLOC_USED) &&
                ((u64)jiffies_to_msecs(dfx_time - mem_node->dfx_current_time) > HDCDRV_MEM_DFX_DEFAULT_TIME)) ? 1 : 0;
}

void hdcdrv_dfx_node_free_fn(struct rb_root *root, int flag)
{
    struct rb_node *node = NULL;
    struct hdcdrv_mem_dfx_node *mem_node = NULL;
    u64 dfx_current_time = jiffies;

    spin_lock_bh(&hdc_ctrl->dfx_lock);
    node = rb_first(root);
    while (node != NULL) {
        mem_node = rb_entry(node, struct hdcdrv_mem_dfx_node, node);
        node = rb_next(node);

        if (hdcdrv_dfx_node_free_check(mem_node, dfx_current_time)) {
            rb_erase(&mem_node->node, root);
            kfree(mem_node);
            mem_node = NULL;

            if (flag == HDCDRV_MEM_DFX_LOCAL) {
                hdc_ctrl->dfx_mem_current_count--;
            } else {
                hdc_ctrl->dfx_mem_remote_current_count--;
            }
        }
    }
    spin_unlock_bh(&hdc_ctrl->dfx_lock);
}

void hdcdrv_dfx_node_info_fn(struct rb_root *root, u64 hash_va)
{
    struct hdcdrv_mem_dfx_node *m_node = NULL;
    m_node = hdcdrv_mem_dfx_rb_search(root, hash_va);
    if (m_node != NULL) {
        hdcdrv_err("[DFX] pid %lld, node state %d, be ctl %llu(ms) ago,"
            "max local node count is %llu, current local node count is %llu,"
            "max remote node count is %llu, current remote node count is %llu.\n",
            m_node->pid, m_node->mem_rbtree_state, (u64)jiffies_to_msecs(jiffies - m_node->dfx_current_time),
            hdc_ctrl->dfx_mem_max_count, hdc_ctrl->dfx_mem_current_count,
            hdc_ctrl->dfx_mem_remote_max_count, hdc_ctrl->dfx_mem_remote_current_count);
    }
}

void hdcdrv_dfx_node_uninit_fn(struct rb_root *root)
{
    struct rb_node *node = NULL;
    struct hdcdrv_mem_dfx_node *mem_node = NULL;

    spin_lock_bh(&hdc_ctrl->lock);

    node = rb_first(root);
    while (node != NULL) {
        mem_node = rb_entry(node, struct hdcdrv_mem_dfx_node, node);
        node = rb_next(node);

        rb_erase(&mem_node->node, root);
        kfree(mem_node);
        mem_node = NULL;
    }

    spin_unlock_bh(&hdc_ctrl->lock);
}

void hdcdrv_dfx_node_work_fn(struct work_struct *p_work)
{
    int i;
    u64 start_time;
    struct rb_root *remote_mem_rb = NULL;
    struct delayed_work *delayed_work = container_of(p_work, struct delayed_work, work);

    start_time = jiffies;

    hdcdrv_dfx_node_free(&hdc_ctrl->mem_rbtree, HDCDRV_MEM_DFX_LOCAL);

    for (i = 0; i < HDCDRV_SURPORT_MAX_DEV; i++) {
        if (hdc_ctrl->devices[i].valid == HDCDRV_VALID) {
            remote_mem_rb = &hdc_ctrl->devices[i].remote_mem_rb_mems;

            hdcdrv_dfx_node_free(remote_mem_rb, HDCDRV_MEM_DFX_REMOTE);
        }
    }

    hdcdrv_info("[DFX] rm unused dfx node, spend %llu(ms) times .\n", (u64)jiffies_to_msecs(jiffies - start_time));

    schedule_delayed_work(delayed_work, HDCDRV_MEM_DFX_DEFAULT_TIME);
}

void hdcdrv_dfx_node_init_fn(void)
{
    int i;
    struct delayed_work *mem_dfx_cycle = NULL;
    hdc_ctrl->mem_rbtree = RB_ROOT;
    for (i = 0; i < HDCDRV_SURPORT_MAX_DEV; i++) {
        hdc_ctrl->devices[i].remote_mem_rb_mems = RB_ROOT;
    }
    spin_lock_init(&hdc_ctrl->dfx_lock);
    mem_dfx_cycle = &hdc_ctrl->mem_dfx_cycle;
    INIT_DELAYED_WORK(mem_dfx_cycle, hdcdrv_dfx_node_work_fn);
    schedule_delayed_work(&hdc_ctrl->mem_dfx_cycle, HDCDRV_MEM_DFX_DEFAULT_TIME);
}
#endif

struct hdcdrv_fast_mem *hdcdrv_get_fast_mem(struct rb_root *root, int type, int len, u64 hash_va, int timeout)
{
    struct hdcdrv_fast_node *f_node = NULL;

    f_node = hdcdrv_fast_node_search_timeout(root, hash_va, timeout);
    if (f_node == NULL) {
        hdcdrv_warn("node not found, type %d, hash va 0x%llx\n", type, hash_va);
        return NULL;
    }

    if ((f_node->fast_mem.mem_type != type) && (f_node->fast_mem.mem_type != HDCDRV_FAST_MEM_TYPE_DVPP)) {
        hdcdrv_node_status_idle(f_node);
        hdcdrv_err("mem type %d not match type %d\n", f_node->fast_mem.mem_type, type);
        return NULL;
    }

    if (f_node->fast_mem.alloc_len < (u32)len) {
        hdcdrv_node_status_idle(f_node);
        hdcdrv_err("len %d > f_node->alloc_len %u\n", len, f_node->fast_mem.alloc_len);
        return NULL;
    }

    return &f_node->fast_mem;
}

unsigned int hdcdrv_fast_get_alloc_pages_segment(unsigned int len, unsigned int *max_len_bit)
{
    unsigned int j;
    unsigned int segment = 0;

    /* max segment is 4M */
    if (len >> *max_len_bit) {
        return (unsigned int)(0x1 << *max_len_bit);
    }

    for (j = *max_len_bit - 1; j >= HDCDRV_MEM_MIN_PAGE_LEN_BIT; j--) {
        if (len & (0x1 << j)) {
            *max_len_bit = j;
            segment = 0x1 << j;
            break;
        }
    }

    return segment;
}

int hdcdrv_dma_map(struct hdcdrv_fast_mem *f_mem, int devid)
{
    int i, j, ret;
    struct hdcdrv_dev *hdc_dev = NULL;
    u64 offset = 0;
    /* devid has checked outside */
    if (hdc_ctrl->devices[devid].valid != HDCDRV_VALID) {
        hdcdrv_err("device %d is not ready, map failed.\n", devid);
        return HDCDRV_DEVICE_NOT_READY;
    }

    if (f_mem->dma_map) {
        ret = ((f_mem->devid == devid) ? HDCDRV_OK : HDCDRV_PARA_ERR);
        hdcdrv_info("hdcdrv dma has already map dev %d, now dev %d\n", f_mem->devid, devid);
        return ret;
    }

    hdc_dev = &hdc_ctrl->devices[devid];

    for (i = 0; i < f_mem->phy_addr_num; i++) {
        if (f_mem->page_type == HDCDRV_PAGE_TYPE_HUGE) {
            f_mem->mem[i].len = HPAGE_SIZE;
            if (hdcdrv_va_to_pa(current->mm, f_mem->user_va + offset, &f_mem->mem[i].addr)) {
                return HDCDRV_DMA_MPA_FAIL;
            }
            offset += HPAGE_SIZE;
        } else {
            f_mem->mem[i].addr = (u64)dma_map_page(hdc_dev->dev, f_mem->mem[i].page, 0, f_mem->mem[i].len,
                                                   DMA_BIDIRECTIONAL);
            if (dma_mapping_error(hdc_dev->dev, f_mem->mem[i].addr)) {
                hdcdrv_err("dma map error\n");
                goto dma_unmap;
            }
        }
    }

    f_mem->devid = devid;

    ret = hdcdrv_send_mem_info(f_mem, devid, HDCDRV_ADD_FLAG);
    if (ret != HDCDRV_OK) {
        hdcdrv_err("hdcdrv_send_mem_info failed, ret = %d\n", ret);
        goto dma_unmap;
    }

    f_mem->dma_map = 1;
    return ret;

dma_unmap:
    if (f_mem->page_type != HDCDRV_PAGE_TYPE_HUGE) {
        for (j = 0; j < i; j++) {
            dma_unmap_page(hdc_dev->dev, f_mem->mem[j].addr, f_mem->mem[j].len, DMA_BIDIRECTIONAL);
            f_mem->mem[j].addr = 0;
        }
    }
    return HDCDRV_DMA_MPA_FAIL;
}

bool hdcdrv_mem_is_notify(struct hdcdrv_fast_mem *f_mem)
{
    if ((f_mem->mem_type == HDCDRV_FAST_MEM_TYPE_RX_DATA) || (f_mem->mem_type == HDCDRV_FAST_MEM_TYPE_RX_CTRL)) {
        return false;
    }

    return true;
}

void hdcdrv_mem_stat_info_show(void)
{
    u64 alloc_len;
    u64 alloc_normal_len;
    u64 alloc_dma_len;
    u64 alloc_cnt;
    u64 free_len;
    u64 free_cnt;

    alloc_cnt = hdc_ctrl->mem_dfx_stat.alloc_cnt;
    alloc_len = hdc_ctrl->mem_dfx_stat.alloc_size;
    alloc_normal_len = hdc_ctrl->mem_dfx_stat.alloc_normal_size;
    alloc_dma_len = hdc_ctrl->mem_dfx_stat.alloc_dma_size;
    free_cnt = hdc_ctrl->mem_dfx_stat.free_cnt;
    free_len = hdc_ctrl->mem_dfx_stat.free_size;

    hdcdrv_info(
        "hdc mem stat info : alloc_cnt %llu, alloc_size 0x%llx, alloc_normal_len 0x%llx, alloc_dma_len 0x%llx,"
        " free_cnt %llu, free_size 0x%llx\n",
        alloc_cnt, alloc_len, alloc_normal_len, alloc_dma_len, free_cnt, free_len);
}

int hdcdrv_dma_unmap(struct hdcdrv_fast_mem *f_mem, int devid, int sync)
{
    struct hdcdrv_dev *hdc_dev = NULL;
    int ret = HDCDRV_OK;
    int i;

    if (!f_mem->dma_map) {
        hdcdrv_info("Dma mem has not map\n");
        return HDCDRV_OK;
    }

    if (hdcdrv_mem_is_notify(f_mem)) {
        ret = hdcdrv_send_mem_info(f_mem, devid, HDCDRV_DEL_FLAG);
        if (ret != HDCDRV_OK) {
            hdcdrv_err("send del ctrl msg failed, ret %d\n", ret);
            /* ignore the sync msg */
            if (sync == HDCDRV_SYNC_CHECK) {
                return ret;
            }
        }
    }

    hdc_dev = &hdc_ctrl->devices[devid];

    if (f_mem->page_type != HDCDRV_PAGE_TYPE_HUGE) {
        for (i = 0; i < f_mem->phy_addr_num; i++) {
            dma_unmap_page(hdc_dev->dev, f_mem->mem[i].addr,
                           f_mem->mem[i].len, DMA_BIDIRECTIONAL);
        }
    }

    f_mem->dma_map = 0;
    f_mem->devid = 0;

    return HDCDRV_OK;
}

void hdcdrv_fast_mem_continuity_check(u32 alloc_len, u32 addr_num, const int segment_mem_num[], u32 segment_num)
{
    int score, ret, offset;
    u32 i, expect_num, segment;
    char buf[HDCDRV_BUF_LEN];

    expect_num = 0;
    for (i = HDCDRV_MEM_MIN_PAGE_LEN_BIT; ; i++) {
        segment = (0x1 << i);
        if (alloc_len < segment) {
            break;
        }

        if (alloc_len & segment) {
            /* 256kb is the critical point of performance */
            if (i > HDCDRV_MEM_MIN_LEN_BIT) {
                expect_num += 0x1 << (i - HDCDRV_MEM_MIN_LEN_BIT);
            } else {
                expect_num++;
            }
        }
    }

    /* Percentage, more than 100 points is excellence */
    score = 0;
    if (addr_num != 0) {
        score = (int)(expect_num * HDCDRV_MEM_SCORE_SCALE / addr_num);
    }

    offset = 0;
    ret = snprintf_s(buf, HDCDRV_BUF_LEN, HDCDRV_BUF_LEN - 1, "order:num");
    if (ret >= 0) {
        offset += ret;
    }

    for (i = 0; i < segment_num; i++) {
        if (segment_mem_num[i] > 0) {
            ret = snprintf_s(buf + offset, HDCDRV_BUF_LEN - offset, HDCDRV_BUF_LEN - offset - 1,
                ",%d:%d", i, segment_mem_num[i]);
            if (ret >= 0) {
                offset += ret;
            }
        }
    }

    if (score < HDCDRV_MEM_SCORE_SCALE) {
        hdcdrv_warn("alloc len 0x%x addr num expect %u actual %u score %d, addr info: %s\r\n",
            alloc_len, expect_num, addr_num, score, buf);
    }
}

static inline void hdcdrv_fast_init_segment_mem_num(int segment_mem_num[], u32 segment_num)
{
    u32 i;
    for (i = 0; i < segment_num; i++) {
        segment_mem_num[i] = 0;
    }
}

static inline void hdcdrv_fast_inc_segment_mem_num(int segment_mem_num[], u32 segment_num, u32 power)
{
    if (power < segment_num) {
        segment_mem_num[power]++;
    }
}

void hdcdrv_fill_fast_mem_info(struct hdcdrv_fast_mem *f_mem, u64 va, u32 len, u32 type)
{
    f_mem->user_va = va;
    f_mem->hash_va = hdcdrv_get_hash(va, (u64)hdcdrv_get_pid());
    f_mem->alloc_len = len;
    f_mem->mem_type = type;
    f_mem->free_flag = HDCDRV_TRUE_FLAG;
}

int hdcdrv_fast_alloc_huge_page_mem(struct hdcdrv_fast_mem *f_mem, u64 va, u32 len, u32 type, u32 devid)
{
    hdcdrv_fill_fast_mem_info(f_mem, va, len, type);
    f_mem->phy_addr_num = len >> HDCDRV_MEM_MIN_HUGE_PAGE_LEN_BIT;
    f_mem->mem = (struct hdcdrv_mem_f *)kmalloc((u64)(unsigned int)f_mem->phy_addr_num *
                                                sizeof(struct hdcdrv_mem_f), GFP_KERNEL | __GFP_ACCOUNT);
    if (f_mem->mem == NULL) {
        hdcdrv_err("kmalloc error\n");
        return HDCDRV_MEM_ALLOC_FAIL;
    }

    return HDCDRV_OK;
}

void hdcdrv_fast_set_normal_mem_dfx_stat(struct hdcdrv_mem_f *mem, int phy_num, u32 len)
{
    int i;

    spin_lock_bh(&hdc_ctrl->mem_dfx_stat.lock);
    hdc_ctrl->mem_dfx_stat.alloc_cnt++;
    hdc_ctrl->mem_dfx_stat.alloc_size += len;
    for (i = 0; i < phy_num; i++) {
        if (mem[i].type == HDCDRV_NORMAL_MEM) {
            hdc_ctrl->mem_dfx_stat.alloc_normal_size += mem[i].len;
        } else {
            hdc_ctrl->mem_dfx_stat.alloc_dma_size += mem[i].len;
        }
    }
    spin_unlock_bh(&hdc_ctrl->mem_dfx_stat.lock);
}

void hdcdrv_fast_free_pages(struct hdcdrv_mem_f *mem, int phy_addr_num)
{
    int i;

    for (i = 0; i < phy_addr_num; i++) {
        if (mem[i].page != NULL) {
            __free_pages(mem[i].page, mem[i].power);
            mem[i].buf = NULL;
            mem[i].page = NULL;
            mem[i].len = 0;
            mem[i].power = 0;
        }
    }
}
gfp_t hdcdrv_get_mem_work_mask(u32 type)
{
    gfp_t gfp_mask;

    if (type == HDCDRV_DMA_MEM) {
        gfp_mask = GFP_KERNEL | __GFP_NOWARN;
    } else {
        gfp_mask = GFP_KERNEL | __GFP_NOWARN | __GFP_DMA32;
    }

    return gfp_mask;
}

void hdcdrv_recycle_mem_work(struct work_struct *p_work)
{
    int i;
    u32 stamp;
    u32 cost_time;
    struct hdcdrv_mem_f *mem =  NULL;
    gfp_t gfp_mask;
    u64 work_cnt = mem_work_cnt;

    stamp = jiffies;
    gfp_mask = hdcdrv_get_mem_work_mask(mem_type);
    mem = (struct hdcdrv_mem_f *)kmalloc(HDCDRV_LIST_MEM_NUM * sizeof(struct hdcdrv_mem_f), gfp_mask);
    if (mem == NULL) {
        hdcdrv_warn("kmalloc error mem_type %u work_cnt %llu\n", mem_type, work_cnt);
        return;
    }

    for (i = 0; i < HDCDRV_LIST_MEM_NUM; i++) {
        if (mem_work_flag != 0) {
            hdcdrv_info("work exit, mem_type %d, i %i work_cnt %lld\n", mem_type, i, work_cnt);
            mem_work_flag = 0;
            goto out;
        }
        mem[i].page = alloc_pages(gfp_mask, HDCDRV_MEM_ORDER_1MB);
        if (mem[i].page == NULL) {
            hdcdrv_warn("alloc_pages failed i %d gfp_mask 0x%x, mem_type %d work_cnt %lld\n", i, gfp_mask, mem_type,
                work_cnt);
            goto out;
        }
        mem[i].power = HDCDRV_MEM_ORDER_1MB;
    }

    cost_time = jiffies_to_msecs(jiffies - stamp);
    hdcdrv_info("mem work sucess cost_time %d mask 0x%x, mem_type %d work_cnt %lld\n", cost_time, gfp_mask, mem_type,
        work_cnt);

out:
    hdcdrv_fast_free_pages(mem, i);
    kfree(mem);
    mem = NULL;
    cost_time = jiffies_to_msecs(jiffies - stamp);
    hdcdrv_info("mem work cost_time %d i %d mem_type %d work_cnt %lld\n", cost_time, i, mem_type, work_cnt);
    hdcdrv_mem_stat_info_show();
}

void hdcdrv_alloc_pages_switch(u32 *max_len_bit)
{
    mem_work_flag = 1;
    cancel_delayed_work_sync(&hdc_ctrl->recycle_mem);
    mem_work_flag = 0;
    schedule_delayed_work(&hdc_ctrl->recycle_mem, 0);
    *max_len_bit = HDCDRV_MEM_MAX_LEN_BIT;
    mem_type = ((mem_type == HDCDRV_DMA_MEM) ?  HDCDRV_NORMAL_MEM : HDCDRV_DMA_MEM);
    mem_work_cnt++;
    hdcdrv_info("schedule mem work mem_type %u cnt %llu.\n", mem_type, mem_work_cnt);
}

gfp_t hdcdrv_get_mask(u32 type, u32 max_len_bit)
{
    gfp_t gfp_mask;

    if (type == HDCDRV_DMA_MEM) {
        gfp_mask = GFP_NOWAIT | __GFP_NOWARN | __GFP_DMA32 | __GFP_ACCOUNT;
    } else {
        gfp_mask = GFP_NOWAIT | __GFP_NOWARN | __GFP_ACCOUNT;
    }

    if (max_len_bit <= HDCDRV_MEM_64KB_LEN_BIT) {
        gfp_mask = GFP_KERNEL | __GFP_NOWARN | __GFP_ACCOUNT;
    }

    return gfp_mask;
}

int hdcdrv_fast_alloc_pages(struct hdcdrv_mem_f *mem, u64 va, u32 len, u32 type, int *phy_addr_num)
{
    int segment_mem_num[HDCDRV_MEM_ORDER_NUM] = {0};
    int i = 0;
    u32 segment, power;
    u32 max_len_bit = HDCDRV_MEM_MAX_LEN_BIT;
    u32 alloc_len = len;
    gfp_t gfp_mask;
    u32 switch_time = 0;

    hdcdrv_fast_init_segment_mem_num(segment_mem_num, HDCDRV_MEM_ORDER_NUM);

    while (alloc_len > 0) {
        segment = hdcdrv_fast_get_alloc_pages_segment(alloc_len, &max_len_bit);
        power = get_order(segment);

        gfp_mask = hdcdrv_get_mask(mem_type, max_len_bit);

        mem[i].page = alloc_pages(gfp_mask, power);
        if (mem[i].page == NULL) {
            max_len_bit -= 1;
            hdcdrv_info("total len 0x%x alloc_len 0x%x max_len_bit %u, mem_work_cnt %llu, mem_type %u gfp_mask 0x%x"
                        " alloc_cnt %llu, alloc_size %llu retry smaller.\n", len, alloc_len, max_len_bit, mem_work_cnt,
                        mem_type, gfp_mask, hdc_ctrl->mem_dfx_stat.alloc_cnt, hdc_ctrl->mem_dfx_stat.alloc_size);
            if ((max_len_bit == HDCDRV_MEM_64KB_LEN_BIT) && (switch_time == 0)) {
                hdcdrv_alloc_pages_switch(&max_len_bit);
                switch_time++;
            } else if ((max_len_bit < HDCDRV_MEM_MIN_PAGE_LEN_BIT)) {
                *phy_addr_num = i;
                hdcdrv_fast_free_pages(mem, i);
                return HDCDRV_DMA_MEM_ALLOC_FAIL;
            }
            continue;
        }

        mem[i].len = segment;
        mem[i].power = power;
        mem[i].type = mem_type;
        hdcdrv_fast_inc_segment_mem_num(segment_mem_num, HDCDRV_MEM_ORDER_NUM, power);

#ifdef HDCDRV_DEBUG
        hdcdrv_info("malloc 0x%x, index %d segment 0x%x, max_len_bit %d\n", len, i, segment, max_len_bit);
#endif
        i++;
        if (segment > 0) {
            alloc_len -= segment;
        }
    }

    *phy_addr_num = i;

    hdcdrv_fast_mem_continuity_check(len, i, segment_mem_num, HDCDRV_MEM_ORDER_NUM);
    return HDCDRV_OK;
}

void *hdcdrv_kvmalloc(size_t size)
{
    void *addr = NULL;

    if (size == 0) {
        return NULL;
    }

    addr = kmalloc(size, GFP_NOWAIT | __GFP_NOWARN | __GFP_ACCOUNT);
    if (addr == NULL) {
        addr = __vmalloc(size, GFP_KERNEL | __GFP_ACCOUNT, PAGE_KERNEL);
        if (addr == NULL) {
            return NULL;
        }
    }

    return addr;
}

void hdcdrv_kvfree(const void *addr)
{
    if (is_vmalloc_addr(addr))
        vfree(addr);
    else
        kfree(addr);
}


int hdcdrv_fast_alloc_normal_page_mem(struct hdcdrv_fast_mem *f_mem, u64 va, u32 len, u32 type,
    u32 devid)
{
    int i, ret;
    struct hdcdrv_mem_f *mem = NULL;
    u32 stamp;
    u32 cost_time;
    u64 size;

    size = (u64)(len >> HDCDRV_MEM_MIN_PAGE_LEN_BIT) * sizeof(struct hdcdrv_mem_f);
    mem = (struct hdcdrv_mem_f *)hdcdrv_kvmalloc(size);
    if (mem == NULL) {
        hdcdrv_err("kvmalloc error, len %u size %lld\n", len, size);
        return HDCDRV_MEM_ALLOC_FAIL;
    }

    stamp = jiffies;

    ret = hdcdrv_fast_alloc_pages(mem, va, len, type, &f_mem->phy_addr_num);
    if (ret != HDCDRV_OK) {
        hdcdrv_err("dev %u alloc pages faile, addr bum %d\n", devid, f_mem->phy_addr_num);
        goto fail;
    }

    cost_time = jiffies_to_msecs(jiffies - stamp);
    if (cost_time > HDCDRV_MAX_COST_TIME) {
        hdcdrv_err("devid %d type %d, alloc pages more then 1s va 0x%llx len 0x%x cost_time %dms\n", devid, type, va,
            len, cost_time);
    }

    if (f_mem->phy_addr_num > HDCDRV_MEM_MAX_PHY_NUM) {
        hdcdrv_err("dev %d phy_addr_num %d is large than %d\n", devid, f_mem->phy_addr_num, HDCDRV_MEM_MAX_PHY_NUM);
        goto fail;
    }

    size = (u64)(unsigned int)f_mem->phy_addr_num * sizeof(struct hdcdrv_mem_f);

    f_mem->mem = (struct hdcdrv_mem_f *)hdcdrv_kvmalloc(size);
    if (f_mem->mem == NULL) {
        hdcdrv_err("kvmalloc error, phy_addr_num %d size %lld\n", f_mem->phy_addr_num, size);
        goto fail;
    }

    for (i = 0; i < f_mem->phy_addr_num; i++) {
        f_mem->mem[i] = mem[i];
    }

    hdcdrv_fill_fast_mem_info(f_mem, va, len, type);

    hdcdrv_fast_set_normal_mem_dfx_stat(mem, f_mem->phy_addr_num, len);

    hdcdrv_kvfree(mem);
    mem = NULL;

    return HDCDRV_OK;

fail:
    hdcdrv_fast_free_pages(mem, f_mem->phy_addr_num);
    hdcdrv_kvfree(mem);
    mem = NULL;
    hdcdrv_err("dev %u total len 0x%x alloc failed.\n", devid, len);
    hdcdrv_mem_stat_info_show();
    return HDCDRV_DMA_MEM_ALLOC_FAIL;
}


int hdcdrv_fast_alloc_phy_mem(struct hdcdrv_fast_mem *f_mem, u64 va, u32 len, u32 type, u32 devid)
{
    if (f_mem->page_type == HDCDRV_PAGE_TYPE_HUGE) {
        return hdcdrv_fast_alloc_huge_page_mem(f_mem, va, len, type, devid);
    } else {
        return hdcdrv_fast_alloc_normal_page_mem(f_mem, va, len, type, devid);
    }
}

void hdcdrv_fast_free_phy_mem(struct hdcdrv_fast_mem *f_mem)
{
    int len;

    len = f_mem->alloc_len;
    if (f_mem->free_flag & HDCDRV_TRUE_FLAG) {
        if (f_mem->page_type != HDCDRV_PAGE_TYPE_HUGE) {
            hdcdrv_fast_free_pages(f_mem->mem, f_mem->phy_addr_num);
        }

        hdcdrv_kvfree(f_mem->mem);
        f_mem->mem = NULL;
    }

    f_mem->phy_addr_num = 0;
    f_mem->user_va = 0;
    f_mem->hash_va = 0;
    f_mem->mem_type = HDCDRV_FAST_MEM_TYPE_MAX;

    spin_lock_bh(&hdc_ctrl->mem_dfx_stat.lock);
    hdc_ctrl->mem_dfx_stat.free_cnt++;
    hdc_ctrl->mem_dfx_stat.free_size += len;
    spin_unlock_bh(&hdc_ctrl->mem_dfx_stat.lock);

    return;
}

int hdcdrv_check_va_pte(struct mm_struct *mm, pmd_t *pmd, unsigned long addr, unsigned long end)
{
    pte_t *pte = NULL;
    int pte_id = 0;

    pte = pte_offset_kernel(pmd, addr);

    do {
        if (!pte_none(*pte)) {
            hdcdrv_err("pte(%d) addr %lx valid\n", pte_id, addr);
            return HDCDRV_PARA_ERR;
        }
        pte_id++;
        pte++;
        addr += PAGE_SIZE;
    } while (addr != end);

    return HDCDRV_OK;
}

int hdcdrv_check_va_pmd(struct mm_struct *mm, pud_t *pud, unsigned long addr, unsigned long end)
{
    pmd_t *pmd = NULL;
    unsigned long next;
    int ret;

    pmd = pmd_offset(pud, addr);

    do {
        next = pmd_addr_end(addr, end);

        if (!pmd_none(*pmd)) {
            ret = hdcdrv_check_va_pte(mm, pmd, addr, next);
            if (ret != HDCDRV_OK) {
                return ret;
            }
        }
        pmd++;
        addr = next;
    } while (addr != end);

    return HDCDRV_OK;
}

#if LINUX_VERSION_CODE >= KERNEL_VERSION(4, 11, 0)
int hdcdrv_check_va_pud(struct mm_struct *mm, p4d_t *p4d, unsigned long addr, unsigned long end)
#else
int hdcdrv_check_va_pud(struct mm_struct *mm, pgd_t *pgd, unsigned long addr, unsigned long end)
#endif
{
    pud_t *pud = NULL;
    unsigned long next;
    int ret;

#if LINUX_VERSION_CODE >= KERNEL_VERSION(4, 11, 0)
    pud = pud_offset(p4d, addr);
#else
    pud = pud_offset(pgd, addr);
#endif

    do {
        next = pud_addr_end(addr, end);
        if (!pud_none(*pud)) {
            ret = hdcdrv_check_va_pmd(mm, pud, addr, next);
            if (ret != HDCDRV_OK) {
                return ret;
            }
        }
        pud++;
        addr = next;
    } while (addr != end);

    return HDCDRV_OK;
}

int hdcdrv_check_va_p4d(struct mm_struct *mm, pgd_t *pgd, unsigned long addr, unsigned long end)
{
#if LINUX_VERSION_CODE >= KERNEL_VERSION(4, 11, 0)
    p4d_t *p4d = NULL;
    unsigned long next;
    int ret;

    p4d = p4d_offset(pgd, addr);

    do {
        next = p4d_addr_end(addr, end);

        if (!p4d_none(*p4d)) {
            ret = hdcdrv_check_va_pud(mm, p4d, addr, next);
            if (ret != HDCDRV_OK) {
                return ret;
            }
        }
        p4d++;
        addr = next;
    } while (addr != end);

    return HDCDRV_OK;
#else
    return hdcdrv_check_va_pud(mm, pgd, addr, end);
#endif
}

int hdcdrv_check_va(struct vm_area_struct *vma, struct hdcdrv_fast_mem *f_mem)
{
    unsigned long size = f_mem->alloc_len;
    unsigned long addr = f_mem->user_va;
    unsigned long next;
    unsigned long end = addr + PAGE_ALIGN(size);
    struct mm_struct *mm = vma->vm_mm;
    pgd_t *pgd = NULL;
    int ret;

    if (vma->vm_flags & VM_HUGETLB) {
        hdcdrv_err("addr %llx hute table\n", f_mem->user_va);
        return HDCDRV_PARA_ERR;
    }

    if (f_mem->user_va & (PAGE_SIZE - 1)) {
        hdcdrv_err("addr %llx invalid\n", f_mem->user_va);
        return HDCDRV_PARA_ERR;
    }

    if ((f_mem->user_va < vma->vm_start) || (f_mem->user_va + f_mem->alloc_len > vma->vm_end)) {
        hdcdrv_err("vma user addr %llx, len %x error.\n", f_mem->user_va, f_mem->alloc_len);
        return HDCDRV_PARA_ERR;
    }

    if (mm->pgd == NULL) {
        return HDCDRV_OK;
    }

    pgd = pgd_offset(mm, addr);

    do {
        next = pgd_addr_end(addr, end);

        if (!pgd_none(*pgd)) {
            ret = hdcdrv_check_va_p4d(mm, pgd, addr, next);
            if (ret != HDCDRV_OK) {
                return ret;
            }
        }
        pgd++;
        addr = next;
    } while (addr != end);

    return HDCDRV_OK;
}

void hdcdrv_zap_vma_ptes(struct hdcdrv_fast_mem *f_mem, struct vm_area_struct *vma, int phy_addr_num)
{
    int i;
    u32 len;
    u32 offset = 0;

    for (i = 0; i < phy_addr_num; i++) {
        len = f_mem->mem[i].len;
#if LINUX_VERSION_CODE >= KERNEL_VERSION(4, 18, 0)
        zap_vma_ptes(vma, f_mem->user_va + offset, f_mem->mem[i].len);
#else
        if (zap_vma_ptes(vma, f_mem->user_va + offset, f_mem->mem[i].len) != 0) {
            hdcdrv_err("va 0x%llx zap_vma_ptes failed.\n", f_mem->user_va + offset);
        }
#endif
        offset += len;
    }
}

int hdcdrv_remap_va(struct hdcdrv_fast_mem *f_mem)
{
    int i, ret;
    unsigned int len;
    unsigned int offset = 0;
    struct vm_area_struct *vma = NULL;

    if (f_mem->page_type == HDCDRV_PAGE_TYPE_HUGE) {
        return HDCDRV_OK;
    }

    down_read(&current->mm->mmap_sem);

    vma = find_vma(current->mm, f_mem->user_va);
    if (vma == NULL) {
        up_read(&current->mm->mmap_sem);
        hdcdrv_err("devid %d find vma fail va 0x%llx.\n", f_mem->devid, f_mem->user_va);
        return HDCDRV_FIND_VMA_FAIL;
    }

    ret = hdcdrv_check_va(vma, f_mem);
    if (ret != HDCDRV_OK) {
        up_read(&current->mm->mmap_sem);
        return ret;
    }

    if (hdcdrv_get_running_env() == HDCDRV_RUNNING_ENV_ARM_3559) {
        vma->vm_flags |= VM_IO | VM_SHARED;
        /*lint -e446 */
        vma->vm_page_prot = pgprot_writecombine(vma->vm_page_prot);
        /*lint +e446 */
    }

    for (i = 0; i < f_mem->phy_addr_num; i++) {
        len = f_mem->mem[i].len;
        if (len > 0) {
            /*lint -e648 */
            ret = remap_pfn_range(vma, f_mem->user_va + offset, page_to_pfn(f_mem->mem[i].page), len,
                                  (vma->vm_page_prot));
            /*lint +e648 */
        }
        offset += len;
        if (ret) {
            break;
        }
    }
    if (i == f_mem->phy_addr_num) {
        up_read(&current->mm->mmap_sem);
        return HDCDRV_OK;
    }

    hdcdrv_zap_vma_ptes(f_mem, vma, i);

    up_read(&current->mm->mmap_sem);

    hdcdrv_err("dev %d vma start %lx, end %lx, addr %llx, len %x remap va failed.\n", f_mem->devid, vma->vm_start,
        vma->vm_end, f_mem->user_va, f_mem->alloc_len);

    return HDCDRV_DMA_MPA_FAIL;
}

int hdcdrv_unmap_va(struct hdcdrv_fast_mem *f_mem)
{
    struct vm_area_struct *vma = NULL;

    if (f_mem->page_type == HDCDRV_PAGE_TYPE_HUGE) {
        return HDCDRV_OK;
    }

    down_read(&current->mm->mmap_sem);

    vma = find_vma(current->mm, f_mem->user_va);
    if (vma == NULL) {
        hdcdrv_err("devid %d find vma fail va 0x%llx.\n", f_mem->devid, f_mem->user_va);
        up_read(&current->mm->mmap_sem);
        return HDCDRV_FIND_VMA_FAIL;
    }

    hdcdrv_zap_vma_ptes(f_mem, vma, f_mem->phy_addr_num);

    up_read(&current->mm->mmap_sem);

    return HDCDRV_OK;
}

u64 hdcdrv_get_hash(u64 user_va, u64 pid)
{
    /*
     * 1. virtual address max 48 bits
     * 2. virtual address aligned by 4k at last, so there are 28 (16+12) bits to store pid
     * 3. 64 bits linux system pid max support 4194304 (23 bits)
     */
    return (((user_va & HDCDRV_RBTREE_ADDR_MASK) << HDCDRV_RBTREE_PID) & HDCDRV_RBTREE_PID_MASK) | pid;
}

void hdcdrv_fast_mem_uninit(struct rb_root *root, int reset)
{
    struct rb_node *node = NULL;
    struct hdcdrv_fast_node *fast_node = NULL;
    int ret = 0;

    spin_lock_bh(&hdc_ctrl->lock);

    node = rb_first(root);
    while (node != NULL) {
        fast_node = rb_entry(node, struct hdcdrv_fast_node, node);
        node = rb_next(node);

        if ((fast_node->pid == hdcdrv_get_pid()) || (reset == HDCDRV_TRUE_FLAG)) {
#ifdef HDCDRV_DEBUG
            hdcdrv_info("hdcdrv_fast_mem_uninit hash_va 0x%llx, pid %lld\n", fast_node->hash_va, fast_node->pid);
#endif
            ret = hdcdrv_dma_unmap(&fast_node->fast_mem, fast_node->fast_mem.devid, HDCDRV_SYNC_NO_CHECK);
            if (ret != HDCDRV_OK) {
                hdcdrv_err("dev %d dma unmap failed, pid %lld\n", fast_node->fast_mem.devid, fast_node->pid);
            }
            hdcdrv_fast_free_phy_mem(&fast_node->fast_mem);
            hdcdrv_fast_node_erase(NULL, root, fast_node);
            hdcdrv_fast_node_free(fast_node);
        }
    }

    spin_unlock_bh(&hdc_ctrl->lock);
}

void hdcdrv_fast_mem_free_abnormal(struct hdcdrv_fast_node *f_node)
{
    struct hdcdrv_fast_node *fast_node = NULL;
    int ret;

    fast_node = hdcdrv_fast_node_search_timeout(&hdc_ctrl->rbtree, f_node->hash_va, HDCDRV_NODE_WAIT_TIME_MAX);
    if (fast_node == NULL) {
        hdcdrv_err("fast node search failed when release, pid %llx\n", f_node->pid);
        return;
    }

    ret = hdcdrv_dma_unmap(&fast_node->fast_mem, fast_node->fast_mem.devid, HDCDRV_SYNC_NO_CHECK);
    if (ret != HDCDRV_OK) {
        hdcdrv_err("dev %d dma unmap failed when release, pid %llx\n", fast_node->fast_mem.devid, fast_node->pid);
    }

    fast_node->fast_mem.alloc_len = 0;
    hdcdrv_fast_free_phy_mem(&fast_node->fast_mem);

    hdcdrv_dfx_node_upgrade(&hdc_ctrl->mem_rbtree, fast_node->hash_va, HDCDRV_MEM_DFX_FREE_RELEASE);
    hdcdrv_fast_node_erase(&hdc_ctrl->lock, &hdc_ctrl->rbtree, fast_node);
    hdcdrv_node_status_idle(fast_node);
    hdcdrv_fast_node_free(fast_node);
}

long hdcdrv_fast_alloc_addr_check(struct hdcdrv_cmd_alloc_mem *cmd)
{
    if (((cmd->type == HDCDRV_FAST_MEM_TYPE_TX_DATA) || (cmd->type == HDCDRV_FAST_MEM_TYPE_RX_DATA)) &&
        (cmd->len > HDCDRV_MEM_MAX_LEN)) {
        hdcdrv_err("check err:cmd type is %d, cmd len is 0x%x\n", cmd->type, cmd->len);
        return HDCDRV_PARA_ERR;
    }

    if (((cmd->type == HDCDRV_FAST_MEM_TYPE_TX_CTRL) || (cmd->type == HDCDRV_FAST_MEM_TYPE_RX_CTRL)) &&
        (cmd->len > HDCDRV_CTRL_MEM_MAX_LEN)) {
        hdcdrv_err("check err:cmd type is %d, cmd len is 0x%x\n", cmd->type, cmd->len);
        return HDCDRV_PARA_ERR;
    }

    if ((cmd->page_type == HDCDRV_PAGE_TYPE_HUGE) &&
        ((cmd->va != round_down(cmd->va, HPAGE_SIZE)) || (cmd->len % HPAGE_SIZE != 0))) {
        hdcdrv_err("check err:page type is %d, cmd len is 0x%x, HPAGE_SIZE is %ld\n", cmd->page_type, cmd->len,
                   HPAGE_SIZE);
        return HDCDRV_PARA_ERR;
    }

    if (cmd->len < PAGE_SIZE) {
        hdcdrv_info("check:cmd len 0x%x is smaller than PAGE_SIZE %ld\n", cmd->len, PAGE_SIZE);
        cmd->len = PAGE_SIZE;
    }

    if ((cmd->len % PAGE_SIZE) != 0) {
        hdcdrv_err("check err:cmd len is 0x%x\n", cmd->len);
        return HDCDRV_PARA_ERR;
    }

    return HDCDRV_OK;
}

int hdcdrv_alloc_mem_param_check(int map, int devid, unsigned int type, unsigned int len)
{
    if (map && ((devid >= HDCDRV_SURPORT_MAX_DEV) || (devid < 0))) {
        hdcdrv_err("devid %d is err\n", devid);
        return HDCDRV_PARA_ERR;
    }

    if ((type >= HDCDRV_FAST_MEM_TYPE_MAX) || (len == 0)) {
        hdcdrv_err("type %u or len %u is err\n", type, len);
        return HDCDRV_PARA_ERR;
    }

    return HDCDRV_OK;
}

long hdcdrv_fast_alloc_mem(struct hdcdrv_ctx *ctx, struct hdcdrv_cmd_alloc_mem *cmd)
{
    struct hdcdrv_fast_node *f_node = NULL;
    long ret;
    long res;

    if (ctx == NULL) {
        hdcdrv_err("devid %d type %u, fast mem ctx is null\n", cmd->devid, cmd->type);
        return HDCDRV_PARA_ERR;
    }

    ret = hdcdrv_alloc_mem_param_check(cmd->map, cmd->devid, cmd->type, cmd->len);
    if (ret != HDCDRV_OK) {
        hdcdrv_err("devid %d type %u or len %u error.\n", cmd->devid, cmd->type, cmd->len);
        return HDCDRV_PARA_ERR;
    }

    if (hdcdrv_fast_alloc_addr_check(cmd)) {
        hdcdrv_err("devid %d addr %llx len 0x%x, type %u, data max len 0x%x. ctrl max len 0x%x, page_type %u\n",
                   cmd->devid, cmd->va, cmd->len, cmd->type, HDCDRV_MEM_MAX_LEN, HDCDRV_CTRL_MEM_MAX_LEN,
                   cmd->page_type);
        return HDCDRV_PARA_ERR;
    }

    f_node = (struct hdcdrv_fast_node *)kzalloc(sizeof(struct hdcdrv_fast_node), GFP_KERNEL | __GFP_ACCOUNT);
    if (f_node == NULL) {
        hdcdrv_err("dev %d f_node kzalloc failed!\n", cmd->devid);
        return HDCDRV_MEM_ALLOC_FAIL;
    }

    f_node->pid = hdcdrv_get_pid();
    f_node->fast_mem.page_type = cmd->page_type;

    ret = hdcdrv_fast_alloc_phy_mem(&f_node->fast_mem, cmd->va, cmd->len, cmd->type, cmd->devid);
    if (ret != HDCDRV_OK) {
        hdcdrv_err("dev %d alloc phy mem failed!\n", cmd->devid);
        goto fast_alloc_fail;
    }

    if (cmd->map) {
        ret = hdcdrv_dma_map(&f_node->fast_mem, cmd->devid);
        if (ret != HDCDRV_OK) {
            hdcdrv_err("dev %d dma map failed!\n", cmd->devid);
            goto dma_map_fail;
        }
    }

    ret = (long)hdcdrv_remap_va(&f_node->fast_mem);
    if (ret != HDCDRV_OK) {
        hdcdrv_err("remap va failed!\n");
        goto map_va_fail;
    }

    f_node->hash_va = f_node->fast_mem.hash_va;
    ret = hdcdrv_fast_node_insert(&hdc_ctrl->rbtree, f_node);
    if (ret != HDCDRV_OK) {
        hdcdrv_info("dev %d fast node insert failed!\n", cmd->devid);
        hdcdrv_dfx_node_info(&hdc_ctrl->mem_rbtree, f_node->hash_va);
        goto node_insert_fail;
    }

    ret = hdcdrv_bind_mem_ctx(ctx, f_node);
    if (ret != HDCDRV_OK) {
        hdcdrv_info("dev %d fast memory bind ctx failed!\n", cmd->devid);
        goto bind_ctx_fail;
    }

    hdcdrv_dfx_node_insert(&hdc_ctrl->mem_rbtree, f_node->hash_va, HDCDRV_MEM_DFX_LOCAL);

    return HDCDRV_OK;

bind_ctx_fail:
    hdcdrv_fast_node_erase(&hdc_ctrl->lock, &hdc_ctrl->rbtree, f_node);
node_insert_fail:
    res = hdcdrv_unmap_va(&f_node->fast_mem);
    if (res != HDCDRV_OK) {
        hdcdrv_err("dev %d unmap va failed!\n", cmd->devid);
    }
map_va_fail:
    if (cmd->map) {
        res = hdcdrv_dma_unmap(&f_node->fast_mem, cmd->devid, HDCDRV_SYNC_NO_CHECK);
        if (res != HDCDRV_OK) {
            hdcdrv_err("dev %d unmap va failed!\n", cmd->devid);
        }
    }
dma_map_fail:
    hdcdrv_fast_free_phy_mem(&f_node->fast_mem);
fast_alloc_fail:
    kfree(f_node);
    f_node = NULL;

    return ret;
}

long hdcdrv_fast_free_mem(struct hdcdrv_cmd_free_mem *cmd)
{
    struct hdcdrv_fast_node *f_node = NULL;
    u64 pid, hash_va;
    int ret;

    if ((cmd->type >= HDCDRV_FAST_MEM_TYPE_MAX)) {
        hdcdrv_err("type %d or pa 0x%llx error.\n", cmd->type, cmd->va);
        return HDCDRV_PARA_ERR;
    }

    pid = (u64)hdcdrv_get_pid();
    hash_va = hdcdrv_get_hash(cmd->va, pid);

    f_node = hdcdrv_fast_node_search_timeout(&hdc_ctrl->rbtree, hash_va, HDCDRV_NODE_WAIT_TIME_MAX);
    if (f_node == NULL) {
        hdcdrv_dfx_node_info(&hdc_ctrl->mem_rbtree, hash_va);
        hdcdrv_err("fast node search va 0x%llx fail.\n", cmd->va);
        return HDCDRV_F_NODE_SEARCH_FAIL;
    }

    if (cmd->type != f_node->fast_mem.mem_type) {
        hdcdrv_err("fast_mem.devid %d,cmd->type %u,fast_mem.mem_type %d\n", f_node->fast_mem.devid, cmd->type,
                   f_node->fast_mem.mem_type);
        hdcdrv_node_status_idle(f_node);
        return HDCDRV_PARA_ERR;
    }

    ret = hdcdrv_dma_unmap(&f_node->fast_mem, f_node->fast_mem.devid, HDCDRV_SYNC_CHECK);
    if (ret != HDCDRV_OK) {
        hdcdrv_err("type %d dma unmap failed.\n", cmd->type);
        hdcdrv_node_status_idle(f_node);
        return ret;
    }

    ret = hdcdrv_unmap_va(&f_node->fast_mem);
    if (ret != HDCDRV_OK) {
        hdcdrv_err("type %d unmap va failed.\n", cmd->type);
    }

    cmd->len = f_node->fast_mem.alloc_len;
    cmd->page_type = f_node->fast_mem.page_type;

    f_node->fast_mem.alloc_len = 0;
    hdcdrv_fast_free_phy_mem(&f_node->fast_mem);

    hdcdrv_unbind_mem_ctx(f_node);

    hdcdrv_dfx_node_upgrade(&hdc_ctrl->mem_rbtree, f_node->hash_va, HDCDRV_MEM_DFX_FREE_USER);

    hdcdrv_fast_node_erase(&hdc_ctrl->lock, &hdc_ctrl->rbtree, f_node);
    hdcdrv_node_status_idle(f_node);
    hdcdrv_fast_node_free(f_node);

    return HDCDRV_OK;
}

long hdcdrv_fast_dma_map(struct hdcdrv_cmd_dma_map *cmd)
{
    int ret;
    struct hdcdrv_fast_node *f_node = NULL;
    u64 hash_va;
    u64 pid;

    if ((cmd->devid >= HDCDRV_SURPORT_MAX_DEV) || (cmd->devid < 0)) {
        return HDCDRV_PARA_ERR;
    }

    if ((cmd->type >= HDCDRV_FAST_MEM_TYPE_MAX)) {
        hdcdrv_err("devid %d type %u or va 0x%llx error.\n", cmd->devid, cmd->type, cmd->va);
        return HDCDRV_PARA_ERR;
    }

#ifdef HDCDRV_DEBUG
    hdcdrv_info("type %u va 0x%llx devid %d dma map.\n", cmd->type, cmd->va, cmd->devid);
#endif

    pid = (u64)hdcdrv_get_pid();
    hash_va = hdcdrv_get_hash(cmd->va, pid);
    f_node = hdcdrv_fast_node_search_timeout(&hdc_ctrl->rbtree, hash_va, HDCDRV_NODE_WAIT_TIME_MAX);
    if (f_node == NULL) {
        hdcdrv_dfx_node_info(&hdc_ctrl->mem_rbtree, hash_va);
        hdcdrv_err("fast node search va 0x%llx fail.\n", cmd->va);
        return HDCDRV_F_NODE_SEARCH_FAIL;
    }

    if (cmd->type != f_node->fast_mem.mem_type) {
        hdcdrv_node_status_idle(f_node);
        hdcdrv_err("type %u, fast_mem.mem_type %d not match\n", cmd->type, f_node->fast_mem.mem_type);
        return HDCDRV_PARA_ERR;
    }

    ret = hdcdrv_dma_map(&f_node->fast_mem, cmd->devid);
    if (ret != HDCDRV_OK) {
        hdcdrv_node_status_idle(f_node);
        hdcdrv_info("dev %d dma map failed!\n", f_node->fast_mem.devid);
        return ret;
    }
#ifdef HDCDRV_DEBUG
    hdcdrv_info("va 0x%llx, len %d, pid %lld, hash_va0x%llx\n", cmd->va,
                f_node->fast_mem.mem[0].len, pid, hash_va);
#endif

    hdcdrv_node_status_idle(f_node);

    return HDCDRV_OK;
}

long hdcdrv_fast_dma_unmap(struct hdcdrv_cmd_dma_unmap *cmd)
{
    u64 pid, hash_va;
    struct hdcdrv_fast_node *f_node = NULL;
    int ret;

    if ((cmd->type >= HDCDRV_FAST_MEM_TYPE_MAX)) {
        hdcdrv_err("type %d or va 0x%llx error.\n", cmd->type, cmd->va);
        return HDCDRV_PARA_ERR;
    }

#ifdef HDCDRV_DEBUG
    hdcdrv_info("type %d va 0x%llx dma unmap.\n", cmd->type, cmd->va);
#endif

    pid = (u64)hdcdrv_get_pid();
    hash_va = hdcdrv_get_hash(cmd->va, pid);
    f_node = hdcdrv_fast_node_search_timeout(&hdc_ctrl->rbtree, hash_va, HDCDRV_NODE_WAIT_TIME_MAX);
    if (f_node == NULL) {
        hdcdrv_dfx_node_info(&hdc_ctrl->mem_rbtree, hash_va);
        hdcdrv_err("fast node search va 0x%llx fail.\n", cmd->va);
        return HDCDRV_F_NODE_SEARCH_FAIL;
    }

    if (cmd->type != f_node->fast_mem.mem_type) {
        hdcdrv_node_status_idle(f_node);
        hdcdrv_err("type %d, fast_mem.mem_type %d not match\n", cmd->type, f_node->fast_mem.mem_type);
        return HDCDRV_PARA_ERR;
    }

    ret = hdcdrv_dma_unmap(&f_node->fast_mem, f_node->fast_mem.devid, HDCDRV_SYNC_CHECK);
    if (ret != HDCDRV_OK) {
        hdcdrv_node_status_idle(f_node);
        hdcdrv_err("type %d, fast_mem unmap failed\n", cmd->type);
        return ret;
    }

    hdcdrv_node_status_idle(f_node);

    return HDCDRV_OK;
}

long hdcdrv_fast_dma_remap(struct hdcdrv_cmd_dma_remap *cmd)
{
    int ret;
    u64 pid, hash_va;
    struct hdcdrv_fast_node *f_node = NULL;
    int devid;

    if ((cmd->devid >= HDCDRV_SURPORT_MAX_DEV) || (cmd->devid < 0)) {
        return HDCDRV_PARA_ERR;
    }

    if ((cmd->type >= HDCDRV_FAST_MEM_TYPE_MAX)) {
        hdcdrv_err("devid %d type %u or pa 0x%llx error.\n", cmd->devid, cmd->type, cmd->va);
        return HDCDRV_PARA_ERR;
    }

#ifdef HDCDRV_DEBUG
    hdcdrv_info("type %u va 0x%llx devid %d dma map.\n", cmd->type, cmd->va, cmd->devid);
#endif

    pid = (u64)hdcdrv_get_pid();
    hash_va = hdcdrv_get_hash(cmd->va, pid);
    f_node = hdcdrv_fast_node_search_timeout(&hdc_ctrl->rbtree, hash_va, HDCDRV_NODE_WAIT_TIME_MAX);
    if (f_node == NULL) {
        hdcdrv_dfx_node_info(&hdc_ctrl->mem_rbtree, hash_va);
        hdcdrv_err("fast node search va 0x%llx fail.\n", cmd->va);
        return HDCDRV_F_NODE_SEARCH_FAIL;
    }

    if (cmd->type != f_node->fast_mem.mem_type) {
        hdcdrv_node_status_idle(f_node);
        hdcdrv_err("type %u, fast_mem.mem_type %d not match\n", cmd->type, f_node->fast_mem.mem_type);
        return HDCDRV_PARA_ERR;
    }

    devid = f_node->fast_mem.devid;
    ret = hdcdrv_dma_unmap(&f_node->fast_mem, f_node->fast_mem.devid, HDCDRV_SYNC_CHECK);
    if (ret != HDCDRV_OK) {
        hdcdrv_node_status_idle(f_node);
        hdcdrv_err("dma unmap failed! device id %d\n", f_node->fast_mem.devid);
        return ret;
    }

    ret = hdcdrv_dma_map(&f_node->fast_mem, cmd->devid);
    if (ret != HDCDRV_OK) {
        ret = hdcdrv_dma_map(&f_node->fast_mem, devid);
        if (ret != HDCDRV_OK) {
            hdcdrv_err("dma resume map failed! device id %d\n", devid);
        }
        hdcdrv_node_status_idle(f_node);
        hdcdrv_err("dma map failed! device id %d\n", cmd->devid);
        return ret;
    }

    hdcdrv_node_status_idle(f_node);

    return HDCDRV_OK;
}
