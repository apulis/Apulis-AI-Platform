/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2012-2019. All rights reserved.
 * Description: This file contains api definition for memory manager driver.
 * Author: huawei
 * Create: 2020-03-09
 */
#include <linux/slab.h>
#include <linux/vmalloc.h>
#include "devmm_common.h"

u64 devmm_get_pagecount_by_size(u64 vptr, u64 sizes, u32 page_size)
{
    u64 chunk_cnt = 0;
    u64 tem_byte;

    if ((page_size == 0) || (sizes > DEVMM_HEAP_SIZE)) {
        devmm_drv_err("chunk_page_size = %u, byte_count=%llu is err.", page_size, sizes);
        return chunk_cnt;
    }
    tem_byte = ((vptr & (page_size - 1)) + sizes);
    chunk_cnt = tem_byte / page_size;
    if (tem_byte & (page_size - 1)) {
        chunk_cnt++;
    }
    return chunk_cnt;
}

int devmm_check_va_add_size_by_heap(struct devmm_svm_heap *heap, u64 va, u64 size)
{
    u64 heap_total_cnt, heap_used_count, heap_free_count, va_need_count;

    heap_used_count = (va - heap->start) / heap->chunk_page_size;
    heap_total_cnt = devmm_svm->heap_size / heap->chunk_page_size;
    heap_free_count = heap_total_cnt - heap_used_count;
    va_need_count = devmm_get_pagecount_by_size(va, size, heap->chunk_page_size);
    if ((va_need_count == 0) || (heap_free_count < va_need_count)) {
        devmm_drv_err("va_need_count is zero, or heap_free_count(%llu) < va_need_count(%llu),"
                      "va=0x%llx, size=%llu, heap_start_va=0x%lx, page_size=%u\n",
                      heap_free_count, va_need_count, va, size, heap->start, heap->chunk_page_size);
        return -EINVAL;
    }

    return 0;
}

int devmm_check_input_heap_info(u32 heap_idx, u32 op, u32 heap_type, u32 page_size)
{
    if (heap_idx >= DEVMM_MAX_HEAP_NUM) {
        return DEVMM_FALSE;
    }
    /* heap type and page size must match */
    if (op == DEVMM_HEAP_ENABLE) {
        if ((heap_type == DEVMM_HEAP_HUGE_PAGE) && (page_size == devmm_svm->device_hpage_size)) {
            return DEVMM_TRUE;
        } else if ((heap_type == DEVMM_HEAP_CHUNK_PAGE) && (page_size == devmm_svm->svm_page_size)) {
            return DEVMM_TRUE;
        } else if ((heap_type == DEVMM_HEAP_PINNED_HOST) && (page_size == devmm_svm->host_page_size)) {
            return DEVMM_TRUE;
        }
    } else if (op == DEVMM_HEAP_DISABLE) {
        return DEVMM_TRUE;
    }

    return DEVMM_FALSE;
}

/* used with the devmm_free, function as alloc */
char *devmm_zalloc(u64 size, int *vmalloc_flag)
{
    char *ptr = (void *)kzalloc(size, GFP_KERNEL);
    if (ptr != NULL) {
        *vmalloc_flag = DEVMM_FALSE;
        return ptr;
    }

    ptr = vzalloc(size);
    if (ptr == NULL) {
        return NULL;
    }
    *vmalloc_flag = DEVMM_TRUE;
    return ptr;
}

/* used with the devmm_zalloc, function as free */
void devmm_free(const char *ptr, int vmalloc_flag)
{
    if (vmalloc_flag == DEVMM_TRUE) {
        vfree(ptr);
    } else {
        kfree(ptr);
    }
}

unsigned long devmm_get_addr_device_page_size(u32 hpid, u64 va)
{
    struct devmm_svm_process *svm_process = NULL;
    struct devmm_svm_heap *heap = NULL;

    devmm_drv_switch("enter,addr=0x%llx.\n", va);

    svm_process = devmm_get_svm_pro(hpid);
    if (svm_process == NULL) {
        devmm_drv_err("va=0x%llx dev_pid=%d, get proc fail.\n", va, hpid);
        return 0;
    }
    heap = devmm_svm_get_heap(svm_process, va);
    if (heap == NULL) {
        devmm_drv_err("va=0x%llx dev_pid=%d, get heap fail.\n", va, hpid);
        return 0;
    }
    return (heap->heap_type == DEVMM_HEAP_HUGE_PAGE) ?
        devmm_svm->device_hpage_size : devmm_svm->device_page_size;
}
EXPORT_SYMBOL(devmm_get_addr_device_page_size);


