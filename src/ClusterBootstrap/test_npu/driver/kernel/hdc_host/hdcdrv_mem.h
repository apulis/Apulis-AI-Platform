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

#ifndef _HDCDRV_MEM_H_
#define _HDCDRV_MEM_H_

#define HDCDRV_MEM_CACHE_LINE   64
#define HDCDRV_MEM_BLOCK_MAGIC  0x48444342 /* HDCB */

#define HDCDRV_SYNC_CHECK       1
#define HDCDRV_SYNC_NO_CHECK    0

#define HDCDRV_BUF_LEN 256

#define HDCDRV_LIST_MEM_NUM 1024

#define HDCDRV_DMA_MEM      1
#define HDCDRV_NORMAL_MEM   0

#define HDCDRV_MAX_COST_TIME  100


struct hdcdrv_mem {
    void *buf;
};

/*
 * hdc memory pool.
 * size : memory block count
 * segment : memory block size, include the block head
 * head : Point to where the effective memory on the ring begins
 * tail : Point to the released ring position, when the memory is released, put it in this position
 */
struct hdcdrv_mem_pool {
    u32 size;
    u32 segment;
    u32 mask;
    u32 resv;
    u64 head;
    u64 tail;
    struct hdcdrv_mem *ring;
    spinlock_t mem_lock;
    struct list_head wait_list;
};

/*
 * hdc block memory head.
 * magic: magic number, used for check head
 * devid: whith device belongs to
 * type : memory type, HDCDRV_MEM_POOL_TYPE_TX or HDCDRV_MEM_POOL_TYPE_RX
 * size : memory block size, include the block head
 * dma_addr : dma address
 * head_crc : crc32 from magic to dma_addr, used for check head
 * ref_count: the block reference count
 */
struct hdcdrv_mem_block_head {
    u32 magic;
    u32 devid;
    u32 type;
    u32 size;
    dma_addr_t dma_addr;
    u32 head_crc;
    u32 ref_count;
};

/* sizeof(struct hdcdrv_mem_block_head) must less than HDCDRV_MEM_CACHE_LINE */
#define HDCDRV_MEM_BLOCK_HEAD_SIZE HDCDRV_MEM_CACHE_LINE
#define HDCDRV_BLOCK_CRC_LEN offsetof(struct hdcdrv_mem_block_head, head_crc)

#define HDCDRV_BLOCK_HEAD(buf) (struct hdcdrv_mem_block_head *)((char *)(buf) - HDCDRV_MEM_BLOCK_HEAD_SIZE)
#define HDCDRV_BLOCK_DMA_HEAD(addr) (dma_addr_t)((u64)(addr) - HDCDRV_MEM_BLOCK_HEAD_SIZE)

#define HDCDRV_BLOCK_BUFFER(head) (struct hdcdrv_mem_block_head *)((char *)(head) + HDCDRV_MEM_BLOCK_HEAD_SIZE)
#define HDCDRV_BLOCK_DMA_BUFFER(addr_head) (dma_addr_t)((u64)(addr_head) + HDCDRV_MEM_BLOCK_HEAD_SIZE)

extern int alloc_mem(int pool_type, int dev_id, int len, void **buf, dma_addr_t *addr, struct list_head *wait_head);
extern void free_mem(void *buf);
extern int alloc_mem_pool(int pool_type, int dev_id, u32 segment, u32 num);
extern void free_mem_pool(int pool_type, int dev_id, u32 segment);

extern void hdcdrv_mem_block_head_init(void *buf, dma_addr_t addr, int pool_type, int dev_id, u32 segment);
extern int hdcdrv_mem_block_head_check(void *buf);
extern int hdcdrv_init_mem_pool(u32 dev_id);
extern int hdcdrv_mem_block_capacity(void);
extern void hdcdrv_mem_block_head_dump(struct hdcdrv_mem_block_head *block_head);
extern void hdcdrv_recycle_mem_work(struct work_struct *p_work);

#endif
