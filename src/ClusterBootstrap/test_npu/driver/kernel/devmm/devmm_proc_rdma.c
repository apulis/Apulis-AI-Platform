/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2012-2019. All rights reserved.
 * Description: This file contains api definition for memory manager driver.
 * Author: huawei
 * Create: 2017-10-15
 */
#include <linux/types.h>

#include "devmm_dev.h"
#include "devmm_common.h"
#include "devmm_proc_info.h"
#include "devmm_channel.h"
#include "devmm_proc_rdma.h"
#include "devdrv_interface.h"

/* acquire return code: 1 svm, 0 - not svm */
int devmm_mem_acquire_proc(u64 addr, u64 size, u32 hostpid, const char *pid_sign, u32 sign_len)
{
    struct devmm_svm_process *svm_process = NULL;
    struct devmm_svm_heap *heap = NULL;
    u32 *page_bitmap = NULL;
    int ret;

    devmm_drv_switch("enter addr=0x%llx, size=%llu.\n", addr, size);

    ret = devmm_svm_get_svm_proc_and_heap(hostpid, addr, &svm_process, &heap);
    if (ret) {
        devmm_drv_err("proc exit or heap error, hostpid(%d), va:0x%llx.\n",
                      hostpid, addr);
        return DEVMM_FALSE;
    }

    page_bitmap = devmm_get_page_bitmap_with_heapidx(svm_process, heap->heap_idx, addr);
    if ((page_bitmap == NULL) || devmm_check_va_add_size_by_heap(heap, addr, size)) {
        devmm_drv_err("bitmap is err. va=0x%llx, sz=%llu.\n", addr, size);
        return DEVMM_FALSE;
    }
    if (devmm_page_bitmap_is_page_available(page_bitmap) &&
        devmm_page_bitmap_is_dev_mapped(page_bitmap)) {
        return DEVMM_TRUE;
    }

    return DEVMM_FALSE;
}
EXPORT_SYMBOL(devmm_mem_acquire_proc);

int devmm_mem_get_pages_proc(u64 aligned_va, u64 aligned_size, u32 hostpid,
    struct devmm_dma_block *blks, u32 blk_num)
{
    struct devmm_svm_process *svm_process = NULL;
    struct devmm_page_query_arg query_arg;
    struct devmm_svm_heap *heap = NULL;
    u32 devid, i, page_num, page_size;
    u32 *page_bitmap = NULL;
    int ret;

    if (blks == NULL) {
        devmm_drv_err("blk err,pid=%u,addr=0x%llx,size=0x%llx.\n", hostpid, aligned_va, aligned_size);
        return -EINVAL;
    }

    ret = devmm_svm_get_svm_proc_and_heap(hostpid, aligned_va, &svm_process, &heap);
    if (ret) {
        devmm_drv_err("proc exit or heap error, hostpid(%u), va:0x%llx.\n",
                      hostpid, aligned_va);
        return -EINVAL;
    }

    page_bitmap = devmm_get_page_bitmap_with_heapidx(svm_process, heap->heap_idx, aligned_va);
    if ((page_bitmap == NULL) || devmm_check_va_add_size_by_heap(heap, aligned_va, aligned_size)) {
        devmm_drv_err("bitmap is null.pid=%u,va=0x%llx,sz=%llx.\n", hostpid, aligned_va, aligned_size);
        return -EINVAL;
    }
    page_size = devmm_get_addr_device_page_size(hostpid, aligned_va);
    if (page_size == 0) {
        devmm_drv_err("hostpid=%u, va=0x%llx, page_size is zero err", hostpid, aligned_va);
        return -EINVAL;
    }
    page_num = aligned_size / page_size;
    if ((page_num == 0) || (page_num > blk_num)) {
        devmm_drv_err("pid=%u,va=0x%llx,blk_num err(%u, %u).\n", hostpid, aligned_va, page_num, blk_num);
        return -EINVAL;
    }

    devid = devmm_page_bitmap_get_devid(page_bitmap);
    query_arg.hostpid = hostpid;
    query_arg.dev_id = devid;
    query_arg.va = aligned_va;
    query_arg.size = aligned_size;
    query_arg.offset = 0;
    query_arg.page_size = page_size;
    if (devmm_query_page_by_msg(query_arg, DEVMM_CHAN_PAGE_QUERY_H2D_ID, blks, &page_num)) {
        devmm_drv_err("query page err.pid=%u,did=%u,va=0x%llx,num=%u.\n", hostpid, devid, aligned_va, page_num);
        return -ENOMEM;
    }

    /* device has not config iommu, dma addr = pa */
    for (i = 0; i < page_num; i++) {
        if (devdrv_devmem_addr_d2h(devid, (phys_addr_t)blks[i].pa, (phys_addr_t *)&blks[i].dma)) {
            devmm_drv_err("addr d2h, pid=%u, devid=%u, va=0x%llx, num=%u.\n", hostpid, devid, aligned_va, i);
            return -ENOMEM;
        }
    }

    return 0;
}
EXPORT_SYMBOL(devmm_mem_get_pages_proc);

void devmm_mem_put_pages_proc(u64 va, u32 hostpid, struct devmm_dma_block *blks, u32 blk_num)
{

}
EXPORT_SYMBOL(devmm_mem_put_pages_proc);

