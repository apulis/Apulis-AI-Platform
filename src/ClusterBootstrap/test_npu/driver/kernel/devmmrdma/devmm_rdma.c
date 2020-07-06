/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2012-2019. All rights reserved.
 * Description: This file contains api definition for memory manager driver.
 * Author: huawei
 * Create: 2017-10-15
 */

#include <linux/dma-mapping.h>
#include <linux/module.h>
#include <linux/slab.h>
#include <linux/sched.h>
#include <linux/list.h>
#include <linux/errno.h>
#include <linux/export.h>
#include <linux/scatterlist.h>
#include <linux/vmalloc.h>
#include <linux/types.h>

#include "devmm_dev.h"
#include "devmm_common.h"
#include "devmm_proc_rdma.h"
#include "devmm_rdma.h"

STATIC struct rw_semaphore rdma_context_sem;

STATIC int devmm_mem_acquire(unsigned long addr, size_t size, struct peer_memory_data *memory_data,
    char *peer_mem_name, struct devmm_mem_context **client_context)
{
    u64 va_aligned_start, va_aligned_end, page_size;
    struct devmm_mem_context *mm_context = NULL;
    u32 page_num;

    devmm_rdma_switch("enter addr=0x%lx, size=%lu.\n", addr, size);

    if ((addr < DEVMM_SVM_MEM_START) || (addr >= (DEVMM_SVM_MEM_START + DEVMM_SVM_MEM_SIZE)) ||
        (size > DEVMM_SVM_MEM_SIZE) || ((addr + size) > (DEVMM_SVM_MEM_START + DEVMM_SVM_MEM_SIZE))) {
        /* can not print log */
        return DEVMM_FALSE;
    }

    if ((memory_data == NULL) || (client_context == NULL)) {
        devmm_rdma_err("input err check private_data, client_context.\n");
        return DEVMM_FALSE;
    }

    if (devmm_mem_acquire_proc(addr, size, memory_data->host_pid, memory_data->pid_sign,
        PROCESS_SIGN_LENGTH) != DEVMM_TRUE) {
        /* Error case handled as not svm */
        devmm_rdma_err("mem_acquire_proc err.pid=%u, add=0x%lx, size=%lu\n",
            memory_data->host_pid, addr, size);
        return DEVMM_FALSE;
    }

    page_size = devmm_get_addr_device_page_size(memory_data->host_pid, addr);
    if (page_size == 0) {
        devmm_rdma_err("pid=%u, add=0x%lx, size=%lu err.\n", memory_data->host_pid, addr, size);
        return DEVMM_FALSE;
    }

    va_aligned_start = round_down(addr, page_size);
    va_aligned_end = round_up((addr + size), page_size);
    page_num = (va_aligned_end - va_aligned_start) / page_size;
    mm_context = (struct devmm_mem_context *)vzalloc((sizeof(struct devmm_mem_context)) +
        page_num * sizeof(struct devmm_dma_block));
    if (mm_context == NULL) {
        /* Error case handled as not svm */
        devmm_rdma_err("kalloc err.pid=%u, add=0x%lx, page_num=%d\n", memory_data->host_pid, addr, page_num);
        return DEVMM_FALSE;
    }
    mm_context->inited_flag = DEVMM_RMDA_INITED_FLAG;
    mm_context->sg_head = NULL;
    mm_context->is_callback = 0;
    mm_context->host_pid = memory_data->host_pid;
    mm_context->va = addr;
    mm_context->len = size;
    mm_context->va_aligned_start = va_aligned_start;
    mm_context->va_aligned_end = va_aligned_end;
    mm_context->blk_num = page_num;
    mutex_init(&mm_context->context_mutex);
    /* 1 means svm */
    *client_context = mm_context;

    return DEVMM_TRUE;
}

STATIC int devmm_mem_get_pages(unsigned long addr, size_t size, int write, int force, struct sg_table *sg_head,
    struct devmm_mem_context *mm_context, u64 core_context)
{
    u64 va_aligned_start, aligned_size, page_size;
    int ret;

    down_read(&rdma_context_sem);
    if ((mm_context == NULL) || (mm_context->inited_flag != DEVMM_RMDA_INITED_FLAG)) {
        up_read(&rdma_context_sem);
        devmm_rdma_err("mm_context(%pK) is null or hasnot inited(%u).\n", mm_context,
                       (mm_context != NULL ? mm_context->inited_flag : 0));
        return -EINVAL;
    }
    if ((addr < mm_context->va) || (addr >= (mm_context->va + mm_context->len)) ||
        (size > mm_context->len) || ((addr + size) > (mm_context->va + mm_context->len))) {
        up_read(&rdma_context_sem);
        devmm_rdma_err("va(0x%lx,%lu) err. acquired range(0x%llx,%llu).\n",
            addr, size, mm_context->va, mm_context->len);
        return -EINVAL;
    }
    page_size = devmm_get_addr_device_page_size(mm_context->host_pid, addr);
    if (page_size == 0) {
        devmm_rdma_err("hostpid=%u, va=0x%lx, page_size is zero err", mm_context->host_pid, addr);
        up_read(&rdma_context_sem);
        return -EINVAL;
    }
    va_aligned_start = round_down(addr, page_size);
    aligned_size = round_up((addr + size), page_size) - va_aligned_start;
    mutex_lock(&mm_context->context_mutex);
    if (mm_context->get_flag == 1) {
        devmm_rdma_err("hostpid=%u, va=0x%lx, already geted pages", mm_context->host_pid, addr);
        mutex_unlock(&mm_context->context_mutex);
        up_read(&rdma_context_sem);
        return -EINVAL;
    }

    ret = devmm_mem_get_pages_proc(va_aligned_start, aligned_size, mm_context->host_pid,
        mm_context->blks, mm_context->blk_num);
    if (ret == 0) {
        mm_context->get_flag = 1;
    }

    mutex_unlock(&mm_context->context_mutex);
    up_read(&rdma_context_sem);
    return ret;
}

STATIC int devmm_dma_map(struct sg_table *sg_head, struct devmm_mem_context *mm_context,
    struct device *dma_device, int dmasync, int *nmap)
{
    struct scatterlist *sg = NULL;
    u64 page_size;
    u32 i = 0;
    int ret;;

    devmm_rdma_switch("enter...\n");

    down_read(&rdma_context_sem);
    if ((mm_context == NULL) || (mm_context->inited_flag != DEVMM_RMDA_INITED_FLAG) ||
        (sg_head == NULL) || (nmap == NULL)) {
        devmm_rdma_err("mm_context(%d) is not inited(%u),sg_head(%d),nmap(%d).\n",
            (mm_context == NULL), (mm_context != NULL ? mm_context->inited_flag : 0),
            (sg_head == NULL), (nmap == NULL));
        up_read(&rdma_context_sem);
        return -EINVAL;
    }
    mutex_lock(&mm_context->context_mutex);
    if (mm_context->sg_head != NULL) {
        devmm_rdma_err("sg_allocated, pid=%u, va=0x%llx.\n", mm_context->host_pid, mm_context->va);
        ret = -EINVAL;
        goto devmm_rdma_map_exit;
    }
    ret = sg_alloc_table(sg_head, mm_context->blk_num, GFP_KERNEL);
    if (ret) {
        devmm_rdma_err("alloc err, num=%u, pid=%u.\n", mm_context->blk_num, mm_context->host_pid);
        goto devmm_rdma_map_exit;
    }

    page_size = devmm_get_addr_device_page_size(mm_context->host_pid, mm_context->va);
    for_each_sg(sg_head->sgl, sg, mm_context->blk_num, i)
    {
        sg_set_page(sg, NULL, page_size, 0);
        sg->dma_address = mm_context->blks[i].dma;
        sg->dma_length = mm_context->blks[i].sz;
    }

    *nmap = mm_context->blk_num;
    mm_context->sg_head = sg_head;
    devmm_rdma_switch("exit. suc\n");

devmm_rdma_map_exit:
    mutex_unlock(&mm_context->context_mutex);
    up_read(&rdma_context_sem);

    return ret;
}

STATIC int devmm_dma_unmap(struct sg_table *sg_head, struct devmm_mem_context *mm_context,
    struct device *dma_device)
{
    devmm_rdma_switch("enter...\n");

    down_read(&rdma_context_sem);
    if ((mm_context == NULL)  || (sg_head == NULL) || (mm_context->inited_flag != DEVMM_RMDA_INITED_FLAG)) {
        devmm_rdma_err("mm_context(%d) is not inited(%u),sg_head null(%d).\n",
            (mm_context == NULL), (mm_context != NULL ? mm_context->inited_flag : 0), (sg_head == NULL));
        up_read(&rdma_context_sem);
        return -EINVAL;
    }
    mutex_lock(&mm_context->context_mutex);
    if (sg_head != mm_context->sg_head) {
        mutex_unlock(&mm_context->context_mutex);
        devmm_rdma_err("sg_head not eq map.\n");
        up_read(&rdma_context_sem);
        return -EINVAL;
    }

    sg_free_table(sg_head);
    mm_context->sg_head = NULL;
    devmm_rdma_switch("exit. suc\n");
    mutex_unlock(&mm_context->context_mutex);
    up_read(&rdma_context_sem);

    return 0;
}

STATIC void devmm_mem_put_pages(struct sg_table *sg_head, struct devmm_mem_context *mm_context)
{
    devmm_rdma_switch("enter...\n");
    down_read(&rdma_context_sem);
    if ((mm_context == NULL) || (mm_context->inited_flag != DEVMM_RMDA_INITED_FLAG)) {
        devmm_rdma_err("mm_context(%d) is not inited(%u).\n",
            (mm_context == NULL), (mm_context != NULL ? mm_context->inited_flag : 0));
        up_read(&rdma_context_sem);
        return;
    }
    mutex_lock(&mm_context->context_mutex);
    if (mm_context->get_flag == 1) {
        mm_context->get_flag = 0;
        devmm_mem_put_pages_proc(mm_context->va, mm_context->host_pid, mm_context->blks, mm_context->blk_num);
    }

    mutex_unlock(&mm_context->context_mutex);
    up_read(&rdma_context_sem);
}

STATIC void devmm_mem_release(struct devmm_mem_context *mm_context)
{
    devmm_rdma_switch("enter...\n");
    down_write(&rdma_context_sem);
    if ((mm_context != NULL) && (mm_context->inited_flag == DEVMM_RMDA_INITED_FLAG)) {
        mm_context->inited_flag = 0;
        if (mm_context->sg_head != NULL) {
            sg_free_table(mm_context->sg_head);
            mm_context->sg_head = NULL;
        }
        if (mm_context->get_flag == 1) {
            mm_context->get_flag = 0;
            devmm_mem_put_pages_proc(mm_context->va, mm_context->host_pid, mm_context->blks, mm_context->blk_num);
        }
        vfree(mm_context);
        mm_context = NULL;
    }
    devmm_rdma_switch("exit.\n");
    up_write(&rdma_context_sem);
}

STATIC unsigned long devmm_mem_get_page_size(struct devmm_mem_context *mm_context)
{
    unsigned long page_size;

    down_read(&rdma_context_sem);
    if ((mm_context == NULL) || (mm_context->inited_flag != DEVMM_RMDA_INITED_FLAG)) {
        up_read(&rdma_context_sem);
        devmm_rdma_err("context is NULL.\n");
        return 0;
    }
    page_size = devmm_get_addr_device_page_size(mm_context->host_pid, mm_context->va);
    up_read(&rdma_context_sem);

    return page_size;
}

invalidate_peer_memory mem_invalidate_callback;

STATIC struct peer_memory_client devmm_mem_client = {
    .acquire = devmm_mem_acquire,
    .get_pages = devmm_mem_get_pages,
    .dma_map = devmm_dma_map,
    .dma_unmap = devmm_dma_unmap,
    .put_pages = devmm_mem_put_pages,
    .get_page_size = devmm_mem_get_page_size,
    .release = devmm_mem_release,
};

int *reg_handle;
#define DRV_NAME "DEVMM RDMA"
#define DRV_VERSION "1.0"

STATIC int __init devmm_mem_client_init(void)
{
    u32 len, i;

    devmm_rdma_info("enter devmm rdma mod init.\n");

    len = strnlen(DRV_NAME, IB_PEER_MEMORY_NAME_MAX - 1);
    /* replace sec-cp-str */
    for (i = 0; i < len; i++) {
        devmm_mem_client.name[i] = DRV_NAME[i];
    }
    devmm_mem_client.name[i] = 0;

    len = strnlen(DRV_VERSION, IB_PEER_MEMORY_VER_MAX - 1);
    /* replace sec-cp-str */
    for (i = 0; i < len; i++) {
        devmm_mem_client.version[i] = DRV_VERSION[i];
    }
    devmm_mem_client.version[i] = 0;

    reg_handle = ib_register_peer_memory_client(&devmm_mem_client, &mem_invalidate_callback);

    if (reg_handle == NULL) {
        devmm_rdma_err("ib_register_peer_memory_client fail. reg_handle=%pK\n", reg_handle);
        return -EINVAL;
    }
    init_rwsem(&rdma_context_sem);
    devmm_rdma_info("devmm rdma mod init success.\n");

    return 0;
}

STATIC void __exit devmm_mem_client_cleanup(void)
{
    devmm_rdma_info("devmm rdma rmmod.\n");
    ib_unregister_peer_memory_client(reg_handle);
}

module_init(devmm_mem_client_init);
module_exit(devmm_mem_client_cleanup);
MODULE_AUTHOR("Huawei Tech. Co., Ltd.");
MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("devmm rdma driver");
