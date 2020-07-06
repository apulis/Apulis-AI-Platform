/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2012-2019. All rights reserved.
 * Description: This file contains api declaration for memory manager driver.
 * Author: huawei
 * Create: 2019-06-11
 */

#ifndef __DEVMM_RDMA_H__
#define __DEVMM_RDMA_H__

#define IB_PEER_MEMORY_NAME_MAX 64
#define IB_PEER_MEMORY_VER_MAX 16

#define DEVMM_RMDA_INITED_FLAG 0xF1234567UL

#define module_devmm_rdma "devmm"
#define devmm_rdma_err(fmt, ...) \
    drv_err(module_devmm_rdma, "<%s:%d,%d> " fmt, current->comm, current->tgid, current->pid, ##__VA_ARGS__)
#define devmm_rdma_warn(fmt, ...) \
    drv_warn(module_devmm_rdma, "<%s:%d,%d> " fmt, current->comm, current->tgid, current->pid, ##__VA_ARGS__)
#define devmm_rdma_info(fmt, ...) \
    drv_info(module_devmm_rdma, "<%s:%d,%d> " fmt, current->comm, current->tgid, current->pid, ##__VA_ARGS__)
#define devmm_rdma_switch(fmt, ...) (void)0

#define PROCESS_SIGN_LENGTH 49
struct peer_memory_data {
    u32 host_pid;
    char pid_sign[PROCESS_SIGN_LENGTH];
};

struct devmm_mem_context {
    struct mutex context_mutex;
    struct sg_table *sg_head;
    u32 inited_flag;
    u32 get_flag;
    int is_callback;
    u32 host_pid;

    u64 va;
    u64 len;
    u64 va_aligned_start;
    u64 va_aligned_end;
    u32 blk_num;
    struct devmm_dma_block blks[0];
};

struct peer_memory_client {
    char name[IB_PEER_MEMORY_NAME_MAX];
    char version[IB_PEER_MEMORY_VER_MAX];
    int (*acquire)(unsigned long addr, size_t size, struct peer_memory_data *memory_data, char *peer_mem_name,
                   struct devmm_mem_context **client_context);
    int (*get_pages)(unsigned long addr, size_t size, int write, int force, struct sg_table *sg_head,
                     struct devmm_mem_context *client_context, u64 core_context);
    void (*put_pages)(struct sg_table *sg_head, struct devmm_mem_context *client_context);
    int (*dma_map)(struct sg_table *sg_head, struct devmm_mem_context *client_context,
        struct device *dma_device, int dmasync, int *nmap);
    int (*dma_unmap)(struct sg_table *sg_head, struct devmm_mem_context *client_context,
        struct device *dma_device);
    unsigned long (*get_page_size)(struct devmm_mem_context *client_context);
    void (*release)(struct devmm_mem_context *client_context);
    void *(*get_context_private_data)(u64 peer_id);
    void (*put_context_private_data)(struct devmm_mem_context *context);
};

typedef int (*invalidate_peer_memory)(int *reg_handle, u64 core_context);

void *ib_register_peer_memory_client(const struct peer_memory_client *peer_client,
                                     invalidate_peer_memory *invalidate_callback);
void ib_unregister_peer_memory_client(int *reg_handle);

#endif /* __DEVMM_RDMA_H__ */
