/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2012-2019. All rights reserved.
 * Description: This file contains api declaration for memory manager driver.
 * Author: huawei
 * Create: 2019-06-11
 */

#ifndef __DEVMM_CHANNEL_H__
#define __DEVMM_CHANNEL_H__
#include "devmm_dev.h"
#include "devmm_proc_info.h"

#define DEVMM_CHANNEL_MAGIC ((u32)('d' << 24 | 'e' << 16 | 'm' << 8 | 'm'))
#define DEVMM_CHANNEL_MASK 0x00000FFF

enum {
    DEVMM_CHAN_EX_PGINFO_H2D_ID,
    DEVMM_CHAN_SETUP_DEVICE_H2D,
    DEVMM_CHAN_CLOSE_DEVICE_H2D,
    DEVMM_CHAN_PAGE_QUERY_H2D_ID,
    DEVMM_CHAN_PAGE_CREATE_H2D_ID,
    DEVMM_CHAN_PAGE_CREATE_QUERY_H2D_ID,
    DEVMM_CHAN_PAGE_D2D_CREATE_H2D_ID,
    DEVMM_CHAN_FREE_PAGES_H2D_ID,
    DEVMM_CHAN_MEMSET8_H2D_ID,
    DEVMM_CHAN_MEMSET16_H2D_ID,
    DEVMM_CHAN_MEMSET32_H2D_ID,
    DEVMM_CHAN_MEMSET64_H2D_ID,
    DEVMM_CHAN_CREATE_MEMSET8_H2D_ID,
    DEVMM_CHAN_CREATE_MEMSET16_H2D_ID,
    DEVMM_CHAN_CREATE_MEMSET32_H2D_ID,
    DEVMM_CHAN_CREATE_MEMSET64_H2D_ID,
    DEVMM_CHAN_SET_DMA_H2D_ID,
    DEVMM_CHAN_GET_DMA_H2D_ID,
    DEVMM_CHAN_TRANSLATION_H2D_ID,
    DEVMM_CHAN_SMMU_QUERY_H2D_ID,
    DEVMM_CHAN_L2BUFF_H2D_ID,
    DEVMM_CHAN_QUERY_L2BUFF_SIZE_H2D_ID,
    DEVMM_CHAN_L2BUFF_DESTROY_H2D_ID,
    DEVMM_CHAN_MEM_IPC_OPEN_H2D_ID,
    DEVMM_CHAN_MEM_IPC_CLOSE_H2D_ID,
    DEVMM_CHAN_PAGE_FAULT_H2D_ID,
    DEVMM_CHAN_PAGE_FAULT_D2H_ID,
    DEVMM_CHAN_QUERY_VAFLGS_D2H_ID,
    DEVMM_CHAN_UPDATE_HEAP_H2D_ID,
    DEVMM_CHAN_QUERY_MEMINFO_H2D_ID,
    DEVMM_CHAN_PAGE_FAULT_D2D_ID,
    DEVMM_CHAN_SET_L2CACHE_READ_COUNT_ID,
    DEVMM_CHAN_PROC_ABORT_D2H_ID,
    DEVMM_CHAN_MAX_ID,
};

enum {
    DEVMM_DMA,
    DEVMM_NON_DMA,
};

#define DEVMM_DATA_SIZE 48
struct devmm_share_memory_data {
    UINT32 image_word;
    int resv;
    int host_pid;
    int data_type;
    char data[DEVMM_DATA_SIZE];
};

#define DEVMM_BUSY_MAGIC_WORD 0xAA55AA55
#define DEVMM_IDLE_MAGIC_WORD 0xBB33BB33
extern int (*devmm_channel_msg_processes[DEVMM_CHAN_MAX_ID])(void *msg, u32 *ack_len);
extern unsigned int devmm_channel_msg_len[DEVMM_CHAN_MAX_ID];

/*
 * 1M数据，查询到的页表所占用空间大小:
 * 32Bytes(msg head) + 256(num=1M/4K) * 12Bytes(PA+SZ) = 3104Bytes
 * 典型任务输入数据大小:
 * 每个核计算任务数据大小:16K~1512K.
 * 有32个核资源的话:(48M : 1512K * 32)
 */
struct devmm_chan_msg_head {
    unsigned int magic;
    unsigned int msg_id;
    unsigned int dev_id;
    pid_t hostpid;
};

struct devmm_chan_exchange_pginfo {
    struct devmm_chan_msg_head head;
    unsigned int host_page_shift;
    unsigned int host_hpage_shift;
    unsigned int device_page_shift;
    unsigned int device_hpage_shift;
};

struct devmm_chan_phy_block {
    unsigned long pa;
    unsigned int sz;
};

#define DEVMM_PAGE_NUM_PER_FAULT 514
struct devmm_chan_page_fault {
    struct devmm_chan_msg_head head;
    unsigned long va;
    u32 num;
    struct devmm_chan_phy_block blks[DEVMM_PAGE_NUM_PER_FAULT];
};
#define DEVMM_BLKNUM_ADD_NUM 2
#define DEVMM_CHUNK_PAGE_SHIFT 12
#define DEVMM_HUGE_PAGE_SHIFT 21
#define DEVMM_SIZE_TO_PAGE_NUM(size, page_size) (((size) / (page_size)) + DEVMM_BLKNUM_ADD_NUM)
#define DEVMM_SIZE_TO_PAGE_MAX_NUM(size) (((size) >> DEVMM_CHUNK_PAGE_SHIFT) + DEVMM_BLKNUM_ADD_NUM)
#define DEVMM_SIZE_TO_HUGEPAGE_MAX_NUM(size) (((size) >> DEVMM_HUGE_PAGE_SHIFT) + DEVMM_BLKNUM_ADD_NUM)
#define DEVMM_BLKNUM_TO_DMANODE_MAX_NUM(blk_num) ((blk_num) * 2 + DEVMM_BLKNUM_ADD_NUM)
#define DEVMM_VA_SIZE_TO_PAGE_NUM(va, sz, pgsz) ((round_up((va) + (sz), pgsz) - round_down(va, pgsz)) / (pgsz))
#define DEVMM_PAGE_MAX_NUM 4096
#define DEVMM_PAGE_NUM_PER_MSG 100
#define DEVMM_MEMSET_SIZE_PER_MSG (1ULL << 24)    // 16M
#define DEVMM_MEMSET8D_SIZE_PER_MSG (1ULL << 29)  // 512M
#define DEVMM_MAX_DUMP_PRE_PRINTF_NUM 4
#define DEVMM_MAX_DUMP_BUF_MEM_NUM (4 * DEVMM_MAX_DUMP_MEM_NUM + 1)

struct devmm_chan_page_query {
    struct devmm_chan_msg_head head;
    unsigned long va;
    unsigned long size;
    unsigned int bitmap;
};

struct devmm_chan_page_query_ack {
    struct devmm_chan_msg_head head;
    unsigned long va;
    unsigned long size;
    u32 bitmap;
    u32 num;
    struct devmm_chan_phy_block blks[0];
};

struct devmm_chan_translate_ack {
    struct devmm_chan_msg_head head;
    struct DMA_ADDR dma_addr;
    unsigned long va;
    unsigned long pa;
};

struct devmm_chan_free_pages {
    struct devmm_chan_msg_head head;
    int shared_flag;
    unsigned long va;
    unsigned int order;
    unsigned long long real_size;  // page aligned
};

struct devmm_chan_memset {
    struct devmm_chan_msg_head head;
    unsigned long dst;
    unsigned long long value;
    unsigned int N;
};

struct devmm_chan_smmu_query_ack {
    struct devmm_chan_msg_head head;
    unsigned long long ttbr;
    unsigned int ssid;
    unsigned long long tcr;
};

struct devmm_chan_l2buff_ack {
    struct devmm_chan_msg_head head;
    unsigned long l2buff;
    unsigned long l2buffsize;
    unsigned long long pte[DEVMM_L2BUFF_PTE_NUM];
};

struct devmm_chan_mem_ipc_open {
    struct devmm_chan_msg_head head;
    pid_t spid;
    DVdeviceptr src_vptr;
    DVdeviceptr vptr;
    size_t size;
    u32 heap_type;
    u32 chunk_page_size;
};

struct devmm_chan_mem_ipc_close {
    struct devmm_chan_msg_head head;
    DVdeviceptr vptr;
    size_t size;
    int ref;
    u32 heap_type;
    u32 chunk_page_size;
};

struct devmm_chan_setup_device {
    struct devmm_chan_msg_head head;
    u32 cmd;
    int devpid;
    int cluster_id;
    int aicpu_flag;

    int heap_cnt;
    u32 heap_type[DEVMM_MAX_HEAP_NUM];
    u32 heap_idx[DEVMM_MAX_HEAP_NUM];
    u32 chunk_page_size[DEVMM_MAX_HEAP_NUM];
};

struct devmm_chan_device_meminfo {
    struct devmm_chan_msg_head head;
    u32 free_num;
    u32 total_num;
    u32 shared_num;
    u32 huge_free_num;
    u32 huge_total_num;
    u32 huge_rsvd_num;
};

struct devmm_chan_close_device {
    struct devmm_chan_msg_head head;
    u32 cmd;
    int devpid;
};

struct devmm_chan_update_heap {
    struct devmm_chan_msg_head head;
    u32 op;
    u32 heap_type;
    u32 heap_idx;
    u32 page_size;
};

struct devmm_chan_set_l2cache_rc {
    struct devmm_chan_msg_head head;
    unsigned long vptr;
    unsigned long size;
    unsigned int rc;
};

struct devmm_chan_proc_abort {
    struct devmm_chan_msg_head head;
    unsigned int status;
};

/*
 * p2p 传输1K数据，查询到的页表所占用空间大小:
 * 32Bytes(msg head) + 100(num) * 8+20Bytes(PA+PAsize + PAnum) = 852Bytes
 * 典型任务输入数据大小:
 * 每个核计算任务数据大小:16K~1512K.
 */
#define DEVMM_P2P_PAGE_MAX_NUM_QUERY_MSG 100

struct devmm_p2p_phy_block {
    unsigned long pa;
};
struct devmm_p2p_page_query_ack {
    struct devmm_chan_msg_head head;
    unsigned long va;
    u32 page_num;
    struct devmm_p2p_phy_block blks[DEVMM_P2P_PAGE_MAX_NUM_QUERY_MSG];
};

struct devmm_p2p_pages_pin {
    struct devmm_chan_msg_head head;
    u32 va_cnt;
    unsigned long va;
    unsigned int order;
};

int devmm_chan_msg_dispatch(void *msg, u32 in_data_len, u32 out_data_len, u32 *ack_len);
int devmm_chan_msg_send(void *msg, unsigned int len, unsigned int out_len);
int devmm_common_msg_send(void *msg, unsigned int len, unsigned int out_len);

int devmm_msg_chan_init(void);
void devmm_msg_chan_uninit(void);
int devmm_host_broadcast_pginfo(unsigned int did);
void *devmm_devid_to_msg_chan(u32 dev_id);
void devmm_notify_device_close_process(struct devmm_svm_process *svm_pro, u32 devid);
int devmm_chan_l2buff_h2d_process(void *msg, u32 *ack_len);
void devmm_chan_send_msg_free_pages(struct devmm_chan_free_pages *free_info, struct devmm_svm_heap *heap,
                                    struct devmm_svm_process *svm_pro, u32 free_self);
void devmm_chan_set_host_device_page_size(void);
void devmm_merg_pa_by_num(unsigned long *pas, u32 num, u32 pgsz, u32 *merg_szlist, u32 *merg_num);
void devmm_svm_free_share_page_msg(struct devmm_svm_process *svm_process, struct devmm_svm_heap *heap,
                                   unsigned long start, u64 real_size, u32 *page_bitmap);

#endif /* __DEVMM_CHANNEL_H__ */
