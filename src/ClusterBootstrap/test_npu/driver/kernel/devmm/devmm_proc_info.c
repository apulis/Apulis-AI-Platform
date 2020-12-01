/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2012-2019. All rights reserved.
 * Description: This file contains api definition for memory manager driver.
 * Author: huawei
 * Create: 2019-06-11
 */

#include <linux/dma-mapping.h>
#include <linux/slab.h>
#include <linux/vmalloc.h>
#include <linux/types.h>
#include <linux/delay.h>
#include <linux/version.h>
#include <linux/mm.h>
#include <linux/pci.h>

#include "devmm_adapt.h"
#include "devmm_dev.h"
#include "devmm_proc_info.h"
#include "devmm_channel.h"
#include "devmm_nsm.h"
#include "devmm_msg_init.h"
#include "devdrv_interface.h"
#include "devdrv_manager_comm.h"
#include "devmm_common.h"
#include "devmm_proc_ipc.h"

#ifndef DEVMM_UT
#define PCI_VENDOR_ID_HUAWEI 0x19e5
STATIC const struct pci_device_id g_devmm_driver_tbl[] = {{ PCI_VDEVICE(HUAWEI, 0xd100), 0 },
                                                          { PCI_VDEVICE(HUAWEI, 0xa126), 0 },
                                                          { PCI_VDEVICE(HUAWEI, 0xd801), 0 },
                                                          {}};
MODULE_DEVICE_TABLE(pci, g_devmm_driver_tbl);
#endif

int devmm_dev_is_same_system(pid_t pid, unsigned int src_devid, u32 dst_devid)
{
    struct devmm_svm_process *svm_pro = NULL;

    svm_pro = devmm_get_svm_pro(pid);
    if (svm_pro == NULL) {
        devmm_drv_err("can not find process by pid (%d).\n", pid);
        return DEVMM_FALSE;
    }

    if (src_devid >= DEVMM_MAX_DEVICE_NUM || dst_devid >= DEVMM_MAX_DEVICE_NUM) {
        devmm_drv_err("invalid src_devid(%u) or dst_devid(%u).\n", src_devid, dst_devid);
        return DEVMM_FALSE;
    }

    /* DEVMM_MAX_AGENTMM_CLUSTER_NUM mean devcie did not setup */
    if ((svm_pro->deviceinfo[src_devid].cluster_id == DEVMM_MAX_AGENTMM_CLUSTER_NUM) ||
        (svm_pro->deviceinfo[dst_devid].cluster_id == DEVMM_MAX_AGENTMM_CLUSTER_NUM)) {
        return DEVMM_TRUE;
    }
    if (svm_pro->deviceinfo[src_devid].cluster_id == svm_pro->deviceinfo[dst_devid].cluster_id) {
        return DEVMM_TRUE;
    }
    return DEVMM_FALSE;
}

u32 *devmm_get_page_bitmap_with_heapidx(struct devmm_svm_process *svm_process, u32 heap_idx, u64 va)
{
    struct devmm_svm_heap *heap = NULL;
    unsigned long pfn;

    if (heap_idx >= DEVMM_MAX_HEAP_NUM) {
        devmm_drv_err("err heap_idx, heap_idx=%u.\n", heap_idx);
        return NULL;
    }
    heap = &svm_process->heaps[heap_idx];

    if (heap->heap_type == DEVMM_HEAP_IDLE) {
        devmm_drv_err("heap->heap_type is DEVMM_HEAP_IDLE. va=0x%llx, heap_idx=%u.\n", va, heap_idx);
        return NULL;
    }

    if ((va < heap->start) || (va > heap->end)) {
        devmm_drv_err("va(0x%llx) overflow, heap_idx=%u, start=0x%lx, end=0x%lx.\n", va,
                      heap_idx, heap->start, heap->end);
        return NULL;
    }

    pfn = (va - heap->start) / heap->chunk_page_size;

    return &svm_process->heaps[heap_idx].page_bitmap[pfn];
}

u32 *devmm_get_page_bitmap(struct devmm_svm_process *svm_process, u64 va)
{
    u32 heap_idx;

    if (devmm_va_is_not_svm_process_addr(svm_process, va)) {
        devmm_drv_err("p=0x%llx overflow,start=0x%lx,end=0x%lx.\n", va, svm_process->start_addr, svm_process->end_addr);
        return NULL;
    }

    heap_idx = (va - svm_process->start_addr) / devmm_svm->heap_size;

    return devmm_get_page_bitmap_with_heapidx(svm_process, heap_idx, va);
}

u32 *devmm_get_page_bitmap_by_va(u64 va)
{
    struct devmm_svm_process *svm_process = NULL;

    svm_process = devmm_get_svm_pro(devmm_get_current_pid());
    if (svm_process == NULL) {
        devmm_drv_err("can not find process by cur pid.\n");
        return NULL;
    }
    return devmm_get_page_bitmap(svm_process, va);
}

STATIC u32 *devmm_get_alloced_page_bitmap(struct devmm_svm_process *svm_process, u64 p)
{
    u32 *page_bitmap = NULL;

    page_bitmap = devmm_get_page_bitmap(svm_process, p);
    if (page_bitmap == NULL) {
        devmm_drv_err("can not find page_bitmap. va=0x%llx.\n", p);
        return page_bitmap;
    }

    if (!devmm_page_bitmap_is_page_available(page_bitmap)) {
        devmm_drv_err("va is not alloced. va=0x%llx. page_bitmap=0x%x\n", p, devmm_page_read_bitmap(page_bitmap));

        return NULL;
    }

    return page_bitmap;
}

STATIC u32 *devmm_get_fst_alloc_bitmap_by_heapidx(struct devmm_svm_process *svm_process, u32 heap_idx, u64 va)
{
    u32 *page_bitmap = NULL;

    page_bitmap = devmm_get_page_bitmap_with_heapidx(svm_process, heap_idx, va);
    if (page_bitmap == NULL) {
        devmm_drv_err("heap_idx=%u, va=0x%llx\n", heap_idx, va);
        return NULL;
    }

    if (!devmm_page_bitmap_is_page_alloced(page_bitmap) || !devmm_page_bitmap_is_first_page(page_bitmap)) {
        devmm_drv_err("va is not first page... va=0x%llx. page_bitmap=0x%x, heap_idx=%u.\n", va,
                      devmm_page_read_bitmap(page_bitmap), heap_idx);
        return NULL;
    }

    return page_bitmap;
}

STATIC u32 devmm_get_order_by_pagecount(u64 count)
{
    u64 step = 1;
    u32 order = 0;

    while (count > step) {
        step = step << 1;
        order++;
    }
    return order;
}

int devmm_check_status_va_info(struct devmm_svm_process *svm_process, u64 va, u64 count)
{
    struct devmm_svm_heap *heap = NULL;

    heap = devmm_svm_get_heap(svm_process, va);
    if (heap == NULL) {
        devmm_drv_err("heap is NULL, p=0x%llx, start_addr=0x%lx, heap_size=%llu.\n", va, svm_process->start_addr,
                      devmm_svm->heap_size);
        return -EINVAL;
    }
    return devmm_check_va_add_size_by_heap(heap, va, count);
}

STATIC u32 devmm_get_shared_page_num(pid_t pid, u32 *page_bitmap, u32 bitmap_num, u32 devid)
{
    u32 share_num = 0;

    /* order sizes mask 5 bit, page sizes max 32 */
    for (share_num = 0; share_num < DEVMM_P2P_FAULT_PAGE_MAX_NUM;) {
        /* page alloced and mapped by dev, */
        if (devmm_page_bitmap_is_page_available(&page_bitmap[share_num]) &&
            devmm_page_bitmap_is_dev_mapped(&page_bitmap[share_num]) &&
            !devmm_dev_is_same_system(pid, devid, devmm_page_bitmap_get_devid(&page_bitmap[share_num]))) {
            /* set share flag */
            devmm_page_bitmap_set_flag(&page_bitmap[share_num], DEVMM_PAGE_ADVISE_MEMORY_SHARED_MASK);
        } else {
            break;
        }
        share_num++;
        /* next page is the end of this heap */
        if (share_num >= bitmap_num) {
            break;
        }
        /*  if next page fisrt flag is seted, the page is other usr alloc */
        if (devmm_page_bitmap_is_isolation_page(&page_bitmap[share_num]) ||
            devmm_page_bitmap_is_first_page(&page_bitmap[share_num])) {
            break;
        }
    }

    return share_num;
}

STATIC int devmm_svm_check_bitmap_available(u32 *page_bitmap, size_t size, size_t page_size)
{
    int page_num, i;

    page_size = page_size ? page_size : PAGE_SIZE;
    page_num = size / page_size;
    if (size % page_size) {
        page_num++;
    }

    for (i = 0; i < page_num; i++) {
        if (!devmm_page_bitmap_is_page_available(page_bitmap + i)) {
            devmm_drv_err("bit map none alloc. va_offset=0x%lx, size=%lu.\n", i * page_size, size);
            return DEVMM_FALSE;
        }
    }
    return DEVMM_TRUE;
}

STATIC void devmm_svm_set_bitmap_mapped(u32 *page_bitmap, size_t size, size_t page_size, unsigned int devid)
{
    int page_num, i;
    u32 mapped_flag;
    page_size = page_size ? page_size : PAGE_SIZE;
    page_num = size / page_size;
    if (size % page_size) {
        page_num++;
    }
    if (devid < DEVMM_MAX_DEVICE_NUM) {
        mapped_flag = DEVMM_PAGE_DEV_MAPPED_MASK;
    } else {
        mapped_flag = DEVMM_PAGE_HOST_MAPPED_MASK;
    }
    for (i = 0; i < page_num; i++) {
        if (!devmm_page_bitmap_is_page_available(page_bitmap + i)) {
            devmm_drv_err("bit map none alloc. va_offset=0x%lx, size=%lu, devid=%u.\n", i * page_size, size, devid);
            return;
        }
        devmm_page_bitmap_set_flag(page_bitmap + i, mapped_flag);
        if (devid < DEVMM_MAX_DEVICE_NUM) {
            devmm_page_bitmap_set_devid(page_bitmap + i, devid);
        }
    }
}

STATIC void devmm_svm_set_mapped(pid_t host_pid, unsigned long va, size_t size, unsigned int devid)
{
    struct devmm_svm_process *svm_process = NULL;
    struct devmm_svm_heap *heap = NULL;
    u32 *page_bitmap = NULL;

    svm_process = devmm_get_svm_pro(host_pid);
    if (svm_process == NULL) {
        devmm_drv_err("can not find proc by pid (%d). va=0x%lx, size=%lu, devid=%u.\n", host_pid, va, size, devid);
        return;
    }
    heap = devmm_svm_get_heap(svm_process, va);
    if (heap == NULL) {
        devmm_drv_err("can not find heap, hostpid(%d). va=0x%lx, size=%lu, devid=%u.\n", host_pid, va, size, devid);
        return;
    }
    page_bitmap = devmm_get_page_bitmap_with_heapidx(svm_process, heap->heap_idx, va);
    if (page_bitmap == NULL) {
        devmm_drv_err("can not find page bitmap. va=0x%lx, size=%lu, devid=%u.\n", va, size, devid);
        return;
    }

    devmm_svm_set_bitmap_mapped(page_bitmap, size, heap->chunk_page_size, devid);
}

void devmm_svm_set_mapped_with_heap(pid_t host_pid, unsigned long va, size_t size, u32 devid,
                                    struct devmm_svm_heap *heap)
{
    struct devmm_svm_process *svm_process = NULL;
    u32 *page_bitmap = NULL;

    svm_process = devmm_get_svm_pro(host_pid);
    if (svm_process == NULL) {
        devmm_drv_err("can not find proc by hostpid (%d),"
                      "va=0x%lx, size=%lu, devid=%u.\n",
                      host_pid, va, size, devid);
        return;
    }

    page_bitmap = devmm_get_page_bitmap_with_heapidx(svm_process, heap->heap_idx, va);
    if (page_bitmap == NULL) {
        devmm_drv_err("can not find page bitmap."
                      "va=0x%lx, size=%lu, devid=%u.\n",
                      va, size, devid);
        return;
    }
    devmm_svm_set_bitmap_mapped(page_bitmap, size, heap->chunk_page_size, devid);
}

STATIC void devmm_svm_clear_bitmap_mapped(u32 *page_bitmap, size_t size, size_t page_size, u32 devid)
{
    int page_num, i;
    u32 mapped_flag;

    page_size = page_size ? page_size : PAGE_SIZE;
    page_num = size / page_size;
    if (size % page_size) {
        page_num++;
    }
    if (devid < DEVMM_MAX_DEVICE_NUM) {
        mapped_flag = DEVMM_PAGE_DEV_MAPPED_MASK;
    } else {
        mapped_flag = DEVMM_PAGE_HOST_MAPPED_MASK;
    }
    for (i = 0; i < page_num; i++) {
        if (!devmm_page_bitmap_is_page_available(page_bitmap + i)) {
            devmm_drv_warn("bit map none alloc. va_offset=0x%lx, size=%lu, devid=%u.\n", i * page_size, size, devid);
            return;
        }
        devmm_page_bitmap_clear_flag(page_bitmap + i, mapped_flag);
        if (!devmm_page_bitmap_is_dev_mapped(page_bitmap + i)) {
            devmm_page_bitmap_clear_flag(page_bitmap + i, DEVMM_PAGE_ADVISE_POPULATE_MASK);
        }
    }
}

void devmm_svm_clear_mapped_with_heap(pid_t host_pid, unsigned long va, size_t size, u32 devid,
                                      struct devmm_svm_heap *heap)
{
    struct devmm_svm_process *svm_process = NULL;
    u32 *page_bitmap = NULL;

    svm_process = devmm_get_svm_pro(host_pid);
    if (svm_process == NULL) {
        devmm_drv_err("can not find proc by pid (%d)."
                      "(0x%lx,0x%lx,%u)\n",
                      host_pid, va, size, devid);
        return;
    }

    page_bitmap = devmm_get_page_bitmap_with_heapidx(svm_process, heap->heap_idx, va);
    if (page_bitmap == NULL) {
        devmm_drv_err("can not find page bitmap.(%u,0x%lx,0x%lx,%u)\n", host_pid, va, size, devid);
        return;
    }
    devmm_svm_clear_bitmap_mapped(page_bitmap, size, heap->chunk_page_size, devid);
}

u32 devmm_svm_va_to_devid(unsigned long va)
{
    struct devmm_svm_process *svm_process = NULL;
    u32 *page_bitmap = NULL;

    svm_process = devmm_get_svm_pro(devmm_get_current_pid());
    if (svm_process == NULL) {
        devmm_drv_err("can not find process by cur pid.\n");
        return DEVMM_MAX_DEVICE_NUM;
    }
    page_bitmap = devmm_get_page_bitmap(svm_process, va);
    if (page_bitmap == NULL) {
        devmm_drv_err("can not find bitmap cur pid, va=0x%lx.\n", va);
        return DEVMM_MAX_DEVICE_NUM;
    }

    if (!devmm_page_bitmap_is_page_available(page_bitmap) || !devmm_page_bitmap_is_dev_mapped(page_bitmap)) {
        /* host fault will return DEVMM_MAX_DEVICE_NUM  */
        return DEVMM_MAX_DEVICE_NUM;
    }

    return devmm_page_bitmap_get_devid(page_bitmap);
}
EXPORT_SYMBOL(devmm_svm_va_to_devid);

void devmm_print_pre_alloced_va(struct devmm_svm_process *svm_process, u64 va)
{
    u64 max_alloc_num, pfn, i, j, page_size, fst_alloced, end_alloced;
    struct devmm_svm_heap *heap = NULL;
    u32 *page_bitmap = NULL;
    int ret;

    heap = devmm_svm_get_heap(svm_process, va);
    if ((heap == NULL) || (heap->heap_type == DEVMM_HEAP_IDLE) || (heap->page_bitmap == NULL)) {
        return;
    }

    page_size = (u64)heap->chunk_page_size;
    pfn = (va - heap->start) / page_size;
    page_bitmap = heap->page_bitmap;
    /* find fisrt alloc page */
    for (i = pfn + 1, fst_alloced = 0, end_alloced = 0; i > 0; i--) {
        if (devmm_page_bitmap_is_page_alloced(&page_bitmap[i - 1])) {
            ret = (end_alloced == 0) && !devmm_page_bitmap_is_isolation_page(&page_bitmap[i - 1]);
            if (ret) {
                end_alloced = i - 1;
            }
            if (devmm_page_bitmap_is_first_page(&page_bitmap[i - 1])) {
                fst_alloced = i - 1;
                break;
            }
        }
    }
    /* find last alloc page */
    ret = (i != 0) && ((end_alloced == pfn) || (end_alloced == 0));
    if (ret) {
        max_alloc_num = 1ull << devmm_page_bitmap_get_order(&page_bitmap[fst_alloced]);
        for (j = pfn + 1; j < max_alloc_num; j++) {
            if (devmm_page_bitmap_is_page_available(&page_bitmap[j])) {
                continue;
            }
            end_alloced = j - 1;
            break;
        }
    }

    if (i != 0) {
        if (devmm_page_bitmap_is_isolation_page(&page_bitmap[fst_alloced])) {
            /* first and last page are used by isolation */
            devmm_drv_err("va=0x%llx previous alloced start_va=0x%llx, end_va=0x%llx, max size=%lld.\n", va,
                ((fst_alloced + DEVMM_ISOLATION_PAGE_NUM) * page_size + heap->start),
                ((end_alloced + 1) * page_size + heap->start - 1),
                ((end_alloced - (fst_alloced + DEVMM_ISOLATION_PAGE_NUM) + 1) * page_size));
        } else {
            devmm_drv_err("va=0x%llx previous alloced start_va=0x%llx, end_va=0x%llx, max size=%lld.\n", va,
                (fst_alloced * page_size + heap->start), ((end_alloced + 1) * page_size + heap->start - 1),
                ((end_alloced - fst_alloced + 1) * page_size));
        }
    }
}

int devmm_svm_get_vaflgs_by_pid(pid_t pid, unsigned long va, u32 dev_id, u32 *bitmap)
{
    struct devmm_svm_process *svm_process = NULL;
    struct devmm_svm_heap *heap = NULL;
    u32 share_page_num, pfn_num;
    u32 *page_bitmap = NULL;

    svm_process = devmm_get_svm_pro(pid);
    if (svm_process == NULL) {
        devmm_drv_err("there is not this process pid(%d).va=0x%lx, devid=%u\n", pid, va, dev_id);
        return -ESRCH;
    }
    heap = devmm_svm_get_heap(svm_process, va);
    if (heap == NULL) {
        devmm_drv_err("pid=%d, heap error, va=0x%lx.\n", pid, va);
        return -EINVAL;
    }
    page_bitmap = devmm_get_page_bitmap_with_heapidx(svm_process, heap->heap_idx, va);
    if ((page_bitmap == NULL) || !devmm_page_bitmap_is_page_available(page_bitmap) ||
        devmm_page_bitmap_is_locked_host(page_bitmap) || devmm_page_bitmap_is_ipc_mem(page_bitmap)) {
        devmm_drv_err("pid =%d, va(0x%lx) is not allow fault by device.devid=%u.bitmap(0x%x)\n", pid, va, dev_id,
                      page_bitmap ? devmm_page_read_bitmap(page_bitmap) : 0);
        devmm_print_pre_alloced_va(svm_process, va);
        return -EINVAL;
    }

    *bitmap = devmm_page_read_bitmap(page_bitmap);
    devmm_page_bitmap_clear_flag(bitmap, DEVMM_PAGE_NOSYNC_FLG | DEVMM_PAGE_SYNC_SHARE_NUM_FLG);
    if (!devmm_page_bitmap_is_host_mapped(page_bitmap)) {
        pfn_num = (heap->end - va) / heap->chunk_page_size + 1;
        share_page_num = devmm_get_shared_page_num(pid, page_bitmap, pfn_num, dev_id);
        if (share_page_num == 0) {
            if (devmm_page_bitmap_is_dev_mapped(page_bitmap)) {
                devmm_drv_err("addr is mapped and not p2p pa, pid=%d,va=0x%lx.devid=%d,bitmap=0x%x\n",
                              pid, va, dev_id, devmm_page_read_bitmap(page_bitmap));
                return -EINVAL;
            }
            devmm_page_bitmap_set_flag(bitmap, DEVMM_PAGE_NOSYNC_FLG);
            devmm_svm_set_mapped_with_heap(pid, va, heap->chunk_page_size, dev_id, heap);
            /*  page_bitmap dev id bits did not set when bitmap assign */
            devmm_page_bitmap_set_devid(bitmap, dev_id);
        } else {
            devmm_page_bitmap_set_flag(bitmap, DEVMM_PAGE_SYNC_SHARE_NUM_FLG);
            /* multiplexing DEVMM_PAGE_ORDER_MASK */
            devmm_page_bitmap_set_order(bitmap, share_page_num);
        }
        devmm_drv_switch("need not sync host pagetable, pid=%d, va=0x%lx.devid=%u,flag=0x%x, share page:%d\n",
                         pid, va, dev_id, *bitmap, share_page_num);
        return 0;
    }

    /*  page_bitmap dev id bits did not set,  will seted at data copy time */
    devmm_page_bitmap_set_devid(bitmap, dev_id);

    return 0;
}

int devmm_ioctl_setup_device(struct devmm_svm_process *svm_pro, struct devmm_ioctl_arg *arg)
{
    struct devmm_mem_setup_device_para *setup_device_para = NULL;
    struct devmm_chan_setup_device *chan_setup = NULL;
    int setup_wait_time = DEVMM_SETUP_RETRY_NUM;
    int cnt_setup_wait_time;
    int devid, i;
    int ret = 0;

    setup_device_para = &arg->data.setup_device_para;
    devmm_drv_switch("enter device=%u.\n", arg->head.devid);
    devid = arg->head.devid;

    chan_setup = (struct devmm_chan_setup_device *)kzalloc(sizeof(struct devmm_chan_setup_device), GFP_KERNEL);
    if (chan_setup == NULL) {
        devmm_drv_err("struct chan_setup kmalloc fail, device=%u.\n", devid);
        return -ENOMEM;
    }

    chan_setup->head.hostpid = svm_pro->hostpid;
    chan_setup->head.dev_id = devid;
    chan_setup->head.msg_id = DEVMM_CHAN_SETUP_DEVICE_H2D;
    chan_setup->cmd = DEVMM_POLLING_CMD_CREATE;

    chan_setup->heap_cnt = 0;
    chan_setup->aicpu_flag = setup_device_para->aicpu_flag;
    for (i = 0; i < (int)DEVMM_MAX_HEAP_NUM; i++) {
        if (svm_pro->heaps[i].heap_type != DEVMM_HEAP_IDLE) {
            chan_setup->heap_type[chan_setup->heap_cnt] = svm_pro->heaps[i].heap_type;
            chan_setup->heap_idx[chan_setup->heap_cnt] = svm_pro->heaps[i].heap_idx;
            chan_setup->chunk_page_size[chan_setup->heap_cnt] = svm_pro->heaps[i].chunk_page_size;

            devmm_drv_switch("update heap info (0x%x) (%u) (%u)\n", chan_setup->heap_type[chan_setup->heap_cnt],
                             chan_setup->heap_idx[chan_setup->heap_cnt],
                             chan_setup->chunk_page_size[chan_setup->heap_cnt]);
            chan_setup->heap_cnt++;
        }
    }

    atomic_inc(&devmm_svm->setup_device_times[devid]);
    for (i = 0; i < setup_wait_time; i++) {
        chan_setup->head.dev_id = devid;
        ret = devmm_chan_msg_send(chan_setup, sizeof(struct devmm_chan_setup_device),
                                  sizeof(struct devmm_chan_setup_device));
        if (ret) {
            devmm_drv_err("setup device err "
                          "device=%u, ret =0x%x, try times=%d.\n",
                          devid, ret, i);
            ret = -EINVAL;
            goto setup_device_free_chan_setup_ptr;
        }

        if (chan_setup->devpid == DEVMM_SETUP_INVAL_PID) {
            chan_setup->cmd = DEVMM_POLLING_CMD_CREATE_WAIT;
            devmm_drv_info_if((i % DEVMM_SETUP_PRE_PRINTF_NUM) == 1, "wait did(%d) devpid(%d) for %d time...\n", devid,
                              chan_setup->devpid, i);
        } else if ((chan_setup->devpid == DEVMM_SETUP_RECREATE_PID) || (chan_setup->devpid == DEVMM_SETUP_SYNC_PID)) {
            chan_setup->cmd = DEVMM_POLLING_CMD_CREATE;
            devmm_drv_info_if((i % DEVMM_SETUP_PRE_PRINTF_NUM) == 1, "wait did(%d) devpid(%d) for create %d time...\n",
                              devid, chan_setup->devpid, i);
        } else {
            devmm_drv_info("did(%d) set ok:devpid=%d, try %d times.\n", devid, chan_setup->devpid, i);
            break;
        }
        cnt_setup_wait_time = atomic_read(&devmm_svm->setup_device_times[devid]) * DEVMM_SETUP_RETRY_NUM;
        setup_wait_time = max(setup_wait_time, cnt_setup_wait_time);
        setup_wait_time = min(setup_wait_time, DEVMM_SETUP_MAX_RETRY_NUM);
        msleep(DEVMM_SETUP_DELAYMS_PER_TURN);
    }

    if (chan_setup->devpid < 0) {
        devmm_drv_err("polling fail, devpid=%d,try times=%d\n", chan_setup->devpid, i);
        ret = -ETIME;
        goto setup_device_free_chan_setup_ptr;
    }

    svm_pro->deviceinfo[devid].devpid = chan_setup->devpid;
    svm_pro->deviceinfo[devid].cluster_id = chan_setup->cluster_id;
    svm_pro->deviceinfo[devid].devid = devid;

    devmm_drv_switch("suc devid[%d] hostpid=%d, devpid=%d, cluster_id=%d.\n", devid, svm_pro->hostpid,
                     chan_setup->devpid, chan_setup->cluster_id);

setup_device_free_chan_setup_ptr:
    atomic_dec(&devmm_svm->setup_device_times[devid]);
    kfree(chan_setup);
    chan_setup = NULL;
    return ret;
}

STATIC int devmm_set_isolation_page_bitmap(u32 *page_bitmap, u64 cnt)
{
    /* 0 and (cnt - 1) isolation_page */
    if ((devmm_page_read_bitmap(page_bitmap) != 0) || (devmm_page_read_bitmap(page_bitmap + (cnt - 1)) != 0)) {
        return -EINVAL;
    }

    devmm_page_bitmap_set_flag(page_bitmap, DEVMM_PAGE_ALLOCED_MASK);
    /* add isolation flag to frist and last page */
    devmm_page_bitmap_set_flag(page_bitmap, DEVMM_PAGE_IS_ISOLATION_MASK);
    devmm_page_bitmap_set_flag(page_bitmap + (cnt - 1), DEVMM_PAGE_IS_ISOLATION_MASK);
    devmm_page_bitmap_set_flag(page_bitmap, DEVMM_PAGE_IS_FIRST_PAGE_MASK);
    /* set order to frist page */
    devmm_page_bitmap_set_order(page_bitmap, devmm_get_order_by_pagecount(cnt));

    return 0;
}

STATIC void devmm_clear_isolation_page_bitmap(u32 *page_bitmap, u64 cnt)
{
    devmm_page_clean_bitmap(page_bitmap);
    devmm_page_clean_bitmap(page_bitmap + (cnt - 1));
}


STATIC int devmm_ioctl_alloc_host(struct devmm_svm_process *svm_process, struct devmm_ioctl_arg *arg)
{
    struct devmm_mem_alloc_host_para *alloc_para = &arg->data.alloc_para;
    struct page_map_info page_map_info = {0};
    struct devmm_svm_heap *heap = NULL;
    u64 chunk_cnt, map_cnt, i, j, vaddr;
    struct page *page = NULL;
    u32 *page_bitmap = NULL;

    devmm_drv_switch("enter p=0x%llx, size=%lu.\n", alloc_para->p, alloc_para->size);
    heap = devmm_svm_get_heap(svm_process, alloc_para->p);
    if (heap == NULL) {
        devmm_drv_err("not find heap. p=0x%llx, size=%lu.\n", alloc_para->p, alloc_para->size);
        return -EINVAL;
    }

    page_bitmap = devmm_get_page_bitmap_with_heapidx(svm_process, heap->heap_idx, alloc_para->p);
    if ((page_bitmap == NULL) ||
        (devmm_check_va_add_size_by_heap(heap, alloc_para->p, alloc_para->size))) {
        devmm_drv_err("not find bitmap. p=0x%llx, size=%lu.\n", alloc_para->p, alloc_para->size);
        return -EINVAL;
    }
    /* alloc pages with pin and set alloced bit mask. */
    chunk_cnt = devmm_get_pagecount_by_size(alloc_para->p, alloc_para->size, heap->chunk_page_size);
    map_cnt = chunk_cnt - DEVMM_ISOLATION_PAGE_NUM;
    /* add isolation flag to first and last page */
    if (devmm_set_isolation_page_bitmap(page_bitmap, chunk_cnt)) {
        devmm_drv_err("isolation already allocd.va=0x%llx,size=%lu.\n", alloc_para->p, alloc_para->size);
        return -EINVAL;
    }

    page_map_info.nid = NUMA_NO_NODE;
    page_map_info.page_num = 0;
    page_map_info.inpages = NULL;

    /* set mask form isolation to page num - isolation */
    for (i = DEVMM_ISOLATION_PAGE_NUM; i < map_cnt; i++) {
        if (devmm_page_read_bitmap(page_bitmap + i) != 0) {
            devmm_drv_err("already allocd(%llu).va=0x%llx,size=%lu.\n", i, alloc_para->p, alloc_para->size);
            goto pinned_fail_handle;
        }
        vaddr = alloc_para->p + heap->chunk_page_size * i;
        page_map_info.va = vaddr;
        page = devmm_insert(&page_map_info, svm_process);
        if (page == NULL) {
            devmm_drv_err("insert fail. va=0x%llx, idx=%lld.\n", vaddr, i);
            goto pinned_fail_handle;
        }
        devmm_page_bitmap_set_flag(page_bitmap + i, DEVMM_PAGE_ALLOCED_MASK);
        devmm_page_bitmap_set_flag(page_bitmap + i, DEVMM_PAGE_LOCKED_HOST_MASK);
    }

    devmm_svm_stat_vir_page_inc(round_up(alloc_para->size, heap->chunk_page_size));

    return 0;
    /* when fail, reclaim pages alloced, clear page_ bitmaps. */
pinned_fail_handle:
    devmm_clear_isolation_page_bitmap(page_bitmap, chunk_cnt);
    for (vaddr = alloc_para->p, j = DEVMM_ISOLATION_PAGE_NUM; j < i; j++, vaddr += heap->chunk_page_size) {
        if (devmm_page_bitmap_is_page_alloced(page_bitmap + j)) {
            devmm_drv_err_if(devmm_unmap(svm_process, vaddr), "unmap err,va=0x%llx.\n", vaddr);
        }
        devmm_page_clean_bitmap(page_bitmap + j);
    }

    return -EINVAL;
}

STATIC int devmm_ioctl_free_host(struct devmm_svm_process *svm_process, struct devmm_ioctl_arg *arg)
{
    struct devmm_mem_free_host_para *free_pinned = &arg->data.free_para;
    struct devmm_svm_heap *heap = NULL;
    u64 order, page_num, i, vaddr;
    u32 *page_bitmap = NULL;

    devmm_drv_switch("p=0x%llx.\n", free_pinned->p);
    /*
     * 1.get corresponding process stru and the 'first' bitmap.
     * if p is not align or 'first', fail.
     */
    heap = devmm_svm_get_heap(svm_process, (unsigned long)free_pinned->p);
    if (heap == NULL) {
        devmm_drv_err("can not get heap, p=0x%llx.\n", free_pinned->p);
        return -EINVAL;
    }

    if (free_pinned->p != round_down(free_pinned->p, heap->chunk_page_size)) {
        devmm_drv_err("not align va=0x%llx, page_size=%u.\n", free_pinned->p, heap->chunk_page_size);
        return -EINVAL;
    }

    if (heap->heap_type != DEVMM_HEAP_PINNED_HOST) {
        devmm_drv_err("invalid heap_type(%x), maybe use wrong api, p=0x%llx.\n",
                      heap->heap_type, free_pinned->p);
        return -EINVAL;
    }

    page_bitmap = devmm_get_fst_alloc_bitmap_by_heapidx(svm_process, heap->heap_idx, free_pinned->p);
    if (page_bitmap == NULL) {
        devmm_drv_err("can not get bitmap, p=0x%llx.\n", free_pinned->p);
        return -EINVAL;
    }

    /*
     * 2.get order by first bitmap,size=(1<<order), we do not know real size.
     * unmap all pages alloced and reset bitmap.
     */
    order = devmm_page_bitmap_get_order(page_bitmap);
    page_num = (1ul << order);
    devmm_svm_stat_vir_page_dec(page_num);

    for (i = 0; i < page_num; i++) {
        if (devmm_page_bitmap_is_page_available(page_bitmap + i)) {
            vaddr = (u64)(uintptr_t)free_pinned->p + i * (u64)PAGE_SIZE;
            devmm_drv_err_if(devmm_unmap(svm_process, (unsigned long)vaddr),
                "unmap err, va=0x%llx, idx=%lld.", vaddr, i);
        }
        devmm_page_clean_bitmap(page_bitmap + i);
    }
    return 0;
}

STATIC int devmm_ioctl_alloc(struct devmm_svm_process *svm_process, struct devmm_ioctl_arg *arg)
{
    struct devmm_mem_alloc_para *alloc_para = &arg->data.alloc_svm_para;
    struct devmm_svm_heap *heap = NULL;
    u64 chunk_cnt, alloc_cnt, i, j;
    u32 *page_bitmap = NULL;

    devmm_drv_switch("va=0x%llx, size=%lu.\n", alloc_para->p, alloc_para->size);

    heap = devmm_svm_get_heap(svm_process, (unsigned long)alloc_para->p);
    if (heap == NULL) {
        devmm_drv_err("can not find heap. va=0x%llx,size=%lu.\n", alloc_para->p, alloc_para->size);
        return -EINVAL;
    }

    page_bitmap = devmm_get_page_bitmap_with_heapidx(svm_process, heap->heap_idx, alloc_para->p);
    if ((page_bitmap == NULL) ||
        devmm_check_va_add_size_by_heap(heap, alloc_para->p, alloc_para->size)) {
        devmm_drv_err("can not find page_bitmap. va=0x%llx, size=%lu.\n", alloc_para->p, alloc_para->size);
        return -EINVAL;
    }
    chunk_cnt = devmm_get_pagecount_by_size(alloc_para->p, alloc_para->size, heap->chunk_page_size);
    alloc_cnt = chunk_cnt - DEVMM_ISOLATION_PAGE_NUM;
    /* add isolation flag to first and last page */
    if (devmm_set_isolation_page_bitmap(page_bitmap, chunk_cnt)) {
        devmm_drv_err("already allocd.va=0x%llx,size=%lu.\n", alloc_para->p, alloc_para->size);
        return -EINVAL;
    }
    for (i = DEVMM_ISOLATION_PAGE_NUM; i < alloc_cnt; i++) {
        if (devmm_page_read_bitmap(page_bitmap + i) != 0) {
            devmm_drv_err("already allocd(%llu).va=0x%llx,size=%lu.\n", i, alloc_para->p, alloc_para->size);
            goto alloc_fail_handle;
        }
        devmm_page_bitmap_set_flag(page_bitmap + i, DEVMM_PAGE_ALLOCED_MASK);
    }
    devmm_svm_stat_vir_page_inc(round_up(alloc_para->size, heap->chunk_page_size));

    return 0;

alloc_fail_handle:
    devmm_clear_isolation_page_bitmap(page_bitmap, chunk_cnt);
    for (j = DEVMM_ISOLATION_PAGE_NUM; j < i; j++) {
        devmm_page_clean_bitmap(page_bitmap + j);
    }

    return -EINVAL;
}

STATIC void devmm_get_svm_attributes(struct devmm_memory_attributes *attributes,
    struct devmm_svm_process *svm_process, DVdeviceptr addr, int heap_idx, struct devmm_svm_heap *heap)
{
    int is_alloced = DEVMM_FALSE;
    u32 *bitmap = NULL;

    attributes->is_svm = DEVMM_TRUE;

    if (heap->heap_type == DEVMM_HEAP_HUGE_PAGE) {
        attributes->is_svm_huge = DEVMM_TRUE;
    }

    bitmap = devmm_get_page_bitmap_with_heapidx(svm_process, heap_idx, addr);
    if (bitmap != NULL) {
        is_alloced = devmm_page_bitmap_is_page_alloced(bitmap);
    }
    if (is_alloced == DEVMM_FALSE) {
        attributes->is_svm = DEVMM_FALSE;
        attributes->is_svm_huge = DEVMM_FALSE;
        devmm_print_pre_alloced_va(svm_process, addr);
        return;
    }

    attributes->is_locked_host = devmm_page_bitmap_is_locked_host(bitmap);
    attributes->is_locked_device = devmm_page_bitmap_is_locked_device(bitmap);

    if (attributes->is_locked_device) {
        attributes->devid = devmm_page_bitmap_get_devid(bitmap);
    }
    attributes->is_svm_host = devmm_page_bitmap_is_host_mapped(bitmap);
    attributes->is_svm_device = devmm_page_bitmap_is_dev_mapped(bitmap);
    attributes->is_svm_non_page = (!attributes->is_svm_host && !attributes->is_svm_device);

    if (attributes->is_svm_device) {
        attributes->devid = devmm_page_bitmap_get_devid(bitmap);
    }
}

void devmm_get_memory_attributes(struct devmm_svm_process *svm_process,
    DVdeviceptr addr, struct devmm_memory_attributes *attributes)
{
    struct devmm_svm_heap *heap = NULL;
    int heap_idx, ret;

    ret = memset_s(attributes, sizeof(struct devmm_memory_attributes), 0, sizeof(struct devmm_memory_attributes));
    if (ret) {
        devmm_drv_err("memset fail, ret = %d.\n", ret);
        return;
    }

    if (devmm_va_is_not_svm_process_addr(svm_process, addr)) {
        attributes->is_local_host = DEVMM_TRUE;
        attributes->page_size = PAGE_SIZE;
        return;
    }

    heap_idx = (addr - svm_process->start_addr) / devmm_svm->heap_size;
    heap = &svm_process->heaps[heap_idx];

    attributes->heap_idx = heap_idx;
    attributes->page_size = heap->chunk_page_size;
    if ((heap->heap_type == DEVMM_HEAP_HUGE_PAGE) || (heap->heap_type == DEVMM_HEAP_CHUNK_PAGE)) {
        devmm_get_svm_attributes(attributes, svm_process, addr, heap_idx, heap);
    } else if (heap->heap_type == DEVMM_HEAP_PINNED_HOST) {
        attributes->is_host_pin = DEVMM_TRUE;
    } else {
        devmm_drv_err("heap_type=0x%x, addr=0x%llx\n", heap->heap_type, addr);
    }
}

STATIC void devmm_acquire_aligned_addr_and_cnt(DVdeviceptr address, u64 byte_count, int is_svm_huge,
                                               DVdeviceptr *aligned_down_addr, u64 *aligned_count)
{
    DVdeviceptr aligned_up_addr;
    if (is_svm_huge) {
        *aligned_down_addr = round_down(address, devmm_svm->device_hpage_size);
        aligned_up_addr = round_up(address + byte_count, devmm_svm->device_hpage_size);
    } else {
        *aligned_down_addr = round_down(address, PAGE_SIZE);
        aligned_up_addr = round_up(address + byte_count, PAGE_SIZE);
    }

    *aligned_count = aligned_up_addr - *aligned_down_addr;

    devmm_drv_switch("aligned_down_addr=0x%llx, aligned_up_addr=0x%llx, aligned_count=%llu\n", *aligned_down_addr,
                     aligned_up_addr, *aligned_count);
}

STATIC int devmm_va_to_palist_add_pgtable(struct devmm_svm_process *svm_process, unsigned long va,
    unsigned long sz, unsigned long *pa, u32 *num)
{
    unsigned long va_max_offset, va_offset, vaddr, paddr, page_sq;
    struct page_map_info page_map_info = {0};
    struct devmm_svm_heap *heap = NULL;
    struct page *pages = NULL;
    u32 *bitmap = NULL;
    u32 pg_num = 0;
    int p_flag = 1;
    int ret = 0;

    heap = devmm_svm_get_heap(svm_process, va);
    if (heap == NULL) {
        devmm_drv_err("oper may out heap size, src=0x%lx, count=%lu.\n", va, sz);
        return -EINVAL;
    }
    bitmap = devmm_get_page_bitmap_with_heapidx(svm_process, heap->heap_idx, va);
    if ((bitmap == NULL) || devmm_check_va_add_size_by_heap(heap, va, sz)) {
        devmm_drv_err("va(0x%lx) bitmap is err, sz=%lu.\n", va, sz);
        return -EINVAL;
    }
    vaddr = round_down(va, PAGE_SIZE);
    va_offset = vaddr + PAGE_SIZE; /* add PAGE_SIZE to adapt va is aligned heap page size */
    va_max_offset = round_up(va + sz, PAGE_SIZE);

    page_map_info.nid = NUMA_NO_NODE;
    page_map_info.page_num = 0;
    page_map_info.inpages = NULL;

    for (page_sq = 0; vaddr < va_max_offset; vaddr += PAGE_SIZE, va_offset += PAGE_SIZE) {
        if (round_up(va_offset, heap->chunk_page_size) > (va + page_sq * heap->chunk_page_size)) {
            if (!devmm_page_bitmap_is_page_available(bitmap + page_sq)) {
                ret = -EINVAL;
                devmm_drv_err("va(0x%lx) is not alloced,bitmap=0x%x.\n", vaddr, *(bitmap + page_sq));
                devmm_print_pre_alloced_va(svm_process, vaddr);
                break;
            }
            page_sq++;
        }
        if (devmm_va_to_pa(svm_process->vma, vaddr, &paddr)) {
            page_map_info.va = vaddr;
            page_map_info.is_4G = devmm_page_bitmap_is_advise_4g_ddr(bitmap + page_sq);
            pages = devmm_insert(&page_map_info, svm_process);
            if (pages == NULL) {
                devmm_drv_err("devmm_insert failed, va=0x%lx.\n", vaddr);
                ret = -ENOMEM;
                break;
            }
            paddr = page_to_phys(pages);
            devmm_svm_set_mapped_with_heap(svm_process->hostpid, vaddr, PAGE_SIZE, DEVMM_MAX_DEVICE_NUM, heap);
            devmm_drv_warn_if(p_flag, "hole mapped va=0x%lx to input va=0x%lx, size=%ld!\n", vaddr, va, sz);
            p_flag = 0;
        }
        if (pg_num >= *num) {
            devmm_drv_err("pa array num err, va=0x%lx, size=%ld, num=%u.\n", vaddr, sz, *num);
            return -EINVAL;
        }
        pa[pg_num++] = paddr;
    }
    *num = pg_num;
    return ret;
}

STATIC void devmm_unpin_page_by_flg(int flg, struct page *page)
{
    if (flg == DEVMM_USER_PIN_PAGES) {
        put_page(page);
    }
}

STATIC int devmm_fill_dma_blklist(u32 dev_id, struct devmm_dma_block *dma_blks, u32 blks_num,
    unsigned long *palist, u32 *szlist)
{
    struct device *dev = NULL;
    u32 i, j;

    dev = devmm_devid_to_device(dev_id);
    if (dev == NULL) {
        devmm_drv_err("dev is null.\n");
        return -EINVAL;
    }

    for (i = 0; i < blks_num; i++) {
        dma_blks[i].pa = palist[i];
        dma_blks[i].sz = szlist[i];
        dma_blks[i].page = pfn_to_page(PFN_DOWN(palist[i]));
        dma_blks[i].dma = dma_map_page(dev, dma_blks[i].page, 0, dma_blks[i].sz, DMA_BIDIRECTIONAL);
        if (dma_mapping_error(dev, dma_blks[i].dma)) {
            devmm_drv_err("dma[%d]_mapping_error:%d.\n", i, dma_mapping_error(dev, dma_blks[i].dma));
            goto devmm_fill_dma_blklist_err;
        }
    }

    return 0;
devmm_fill_dma_blklist_err:
    for (j = 0; j < i; j++) {
        dma_unmap_page(dev, dma_blks[j].dma, dma_blks[j].sz, DMA_BIDIRECTIONAL);
    }

    return -EIO;
}

STATIC int devmm_fill_pa_list(struct devmm_svm_process *svm_proc, struct devmm_page_query_arg query_arg,
    struct devmm_pa_list_info *dma_query_arg, u32 *num)
{
    u32 i, num_tmp;
    int ret;
    /*
    * query num is just counted as byte_cnt/page_size + 2, here try best to get pa,
    * devmm_make_dmanode_list will judge num is right or not with size and offset
    */
    if (dma_query_arg->pin_flg != DEVMM_USER_PIN_PAGES) {
        num_tmp = *num;
        ret = devmm_va_to_palist_add_pgtable(svm_proc, query_arg.va, query_arg.size, dma_query_arg->palist, &num_tmp);
        if (ret) {
            devmm_drv_err("va_to_palist_add_pgtable fail, (0x%lx,0x%lx,%d)\n",
                query_arg.va, query_arg.size, *num);
            return ret;
        }
    } else {
        /* get_user_pages page_count will inc, need to call put_page after all */
        ret = get_user_pages_fast(query_arg.va, *num, dma_query_arg->write, dma_query_arg->pages);
        if (ret < 0) {
            devmm_drv_err("get_user_pages fail ret=%d, (0x%lx,0x%lx,%d).\n", ret, query_arg.va, query_arg.size, *num);
            return ret;
        }
        num_tmp = (u32)ret;
        for (i = 0; i < num_tmp; i++) {
            dma_query_arg->palist[i] = page_to_phys(dma_query_arg->pages[i]);
        }
    }
    *num = num_tmp;
    return 0;
}


STATIC int devmm_va_to_dmalist(struct devmm_svm_process *svm_proc, struct devmm_page_query_arg query_arg,
    int pin_flg, int write, struct devmm_copy_side *side)
{
    size_t szlist_len, pages_len, palist_len;
    struct devmm_pa_list_info pa_list_info;
    int ret, vmalloc_flag;
    u32 num_tmp;
    u32 j;

    devmm_drv_switch("enter (%d,0x%lx,0x%lx,%d).\n", pin_flg, query_arg.va, query_arg.size, side->num);

    palist_len = (size_t)(side->num) * sizeof(unsigned long);
    pages_len = (size_t)(side->num) * sizeof(char *); /* get pointer type size, need use sizeof(char *). */
    szlist_len = (size_t)(side->num) * sizeof(u32);
    pa_list_info.palist = (unsigned long *)devmm_zalloc(palist_len + pages_len + szlist_len, &vmalloc_flag);
    if (pa_list_info.palist == NULL) {
        devmm_drv_err("copy buf malloc fail.(0x%lx,0x%lx).\n", query_arg.va, query_arg.size);
        return -ENOMEM;
    }
    pa_list_info.pages = (struct page **)(uintptr_t)((unsigned long)(uintptr_t)pa_list_info.palist + palist_len);
    pa_list_info.szlist = (u32 *)(uintptr_t)((unsigned long)(uintptr_t)pa_list_info.palist + palist_len + pages_len);
    pa_list_info.pin_flg = pin_flg;
    pa_list_info.write = write;
    num_tmp = side->num;
    ret = devmm_fill_pa_list(svm_proc, query_arg, &pa_list_info, &num_tmp);
    if (ret) {
        devmm_drv_err("va_to_palist fail, (0x%lx,0x%lx,%d)\n", query_arg.va, query_arg.size, side->num);
        goto err_free_palist;
    }
    devmm_merg_pa_by_num(pa_list_info.palist, num_tmp, side->blk_page_size, pa_list_info.szlist, &side->num);
    ret = devmm_fill_dma_blklist(query_arg.dev_id, side->blks, side->num, pa_list_info.palist, pa_list_info.szlist);
    if (ret) {
        devmm_drv_err("fill_dma_blklist fail, ret=%d, (%d,%d,0x%lx,0x%lx,(%d,%d)).\n",
                      ret, query_arg.dev_id, pin_flg, query_arg.va, query_arg.size, num_tmp, side->num);
        goto dma_mapping_error_lable;
    }
    devmm_free((const char *)pa_list_info.palist, vmalloc_flag);
    pa_list_info.palist = NULL;

    return 0;

dma_mapping_error_lable:
    for (j = 0; j < num_tmp; j++) {
        devmm_unpin_page_by_flg(pin_flg, pa_list_info.pages[j]);
    }

err_free_palist:
    devmm_free((const char *)pa_list_info.palist, vmalloc_flag);
    pa_list_info.palist = NULL;
    return ret;
}

STATIC void devmm_va_to_dmalist_free(u32 dev_id, int pin_flg, struct devmm_copy_side *side)
{
    struct device *dev = devmm_devid_to_device(dev_id);
    struct devmm_dma_block *dma_blks = side->blks;
    u32 num = side->num;
    u32 i, j, blk_num;

    if (dev == NULL) {
        devmm_drv_err("dev is null, dev_id=%u, pin_flg=%d, num=%d\n", dev_id, pin_flg, num);
        return;
    }

    for (i = 0; i < num; i++) {
        dma_unmap_page(dev, dma_blks[i].dma, dma_blks[i].sz, DMA_BIDIRECTIONAL);
        blk_num = dma_blks[i].sz / side->blk_page_size;
        if (dma_blks[i].page == NULL) {
            continue;
        }
        for (j = 0; j < blk_num; j++) {
            devmm_unpin_page_by_flg(pin_flg, pfn_to_page(page_to_pfn(dma_blks[i].page) + j));
        }
    }
}

STATIC int devmm_insert_host_page_range(struct devmm_svm_process *svm_pro, DVdeviceptr aligned_addr,
                                        u64 aligned_count, struct devmm_memory_attributes *fst_attr)
{
    u32 adjust_order = 0;
    u32 *bitmap = NULL;
    u64 page_cnt, i;
    u32 page_size;
    int ret;

    if (fst_attr->is_svm_huge) {
        page_size = devmm_svm->device_hpage_size;
        adjust_order = devmm_host_hugepage_fault_adjust_order();
    } else {
        page_size = PAGE_SIZE;
    }

    ret = devmm_check_status_va_info(svm_pro, aligned_addr, aligned_count);
    if (ret) {
        devmm_drv_err("va may out heap size ret=%d, src=0x%llx, count=%llu\n", ret, aligned_addr, aligned_count);
        return ret;
    }

    page_cnt = aligned_count / page_size;
    bitmap = devmm_get_page_bitmap_with_heapidx(svm_pro, fst_attr->heap_idx, aligned_addr);
    if (bitmap == NULL) {
        devmm_drv_err("page bitmap is null, addr=0x%llx, count=%llu\n", aligned_addr, aligned_count);
        return -EINVAL;
    }
    for (i = 0; i < page_cnt; i++, bitmap++) {
        ret = !devmm_page_bitmap_is_page_available(bitmap) || devmm_page_bitmap_is_locked_device(bitmap);
        if (ret) {
            devmm_drv_err("page is not alloced or locked device, aligned_addr=0x%llx,"
                          " aligned_count=%llu, i=%llu ,bitmap=0x%x\n ",
                          aligned_addr, aligned_count, i, *bitmap);
            devmm_print_pre_alloced_va(svm_pro, (unsigned long)(aligned_addr + (DVdeviceptr)page_size * i));
            return -EINVAL;
        }

        if (!devmm_page_bitmap_is_host_mapped(bitmap)) {
            /* insert use host page size, heap adjust order, hugepage at devmm_alloc_pages will try 64k per time */
            ret = devmm_insert_host_range(svm_pro, (unsigned long)(aligned_addr + (DVdeviceptr)page_size * i),
                                          page_size, adjust_order);
            if (ret) {
                devmm_drv_err("insert host range fail. aligned_addr=0x%llx, page_size=%u",
                              aligned_addr + (DVdeviceptr)page_size * i, page_size);
                return ret;
            }

            devmm_page_bitmap_set_flag(bitmap, DEVMM_PAGE_HOST_MAPPED_MASK);
        }
    }

    return 0;
}

STATIC int devmm_memcpy_pre_process(struct devmm_svm_process *svm_process,
                                    struct devmm_ioctl_arg *arg,
                                    struct devmm_memory_attributes *src_attr,
                                    struct devmm_memory_attributes *dst_attr,
                                    enum devmm_copy_direction *dir)
{
    DVdeviceptr dst = arg->data.copy_para.dst;
    DVdeviceptr src = arg->data.copy_para.src;
    u64 byte_count = arg->data.copy_para.ByteCount;
    DVdeviceptr aligned_down_addr;
    int src_host, dst_host;
    u64 aligned_count;
    int ret;

    if (src_attr->is_svm_non_page || (byte_count == 0)) {
        devmm_drv_err("src has non page.src=0x%llx, dst=0x%llx, count=%llu.\n", src, dst, byte_count);
        *dir = DEVMM_COPY_INVILED_DIRECTION;
        return -EINVAL;
    }

    if (dst_attr->is_svm_non_page) {
        devmm_acquire_aligned_addr_and_cnt(dst, byte_count, dst_attr->is_svm_huge, &aligned_down_addr,
                                           &aligned_count);

        ret = devmm_insert_host_page_range(svm_process, aligned_down_addr, aligned_count, dst_attr);
        if (ret) {
            devmm_drv_err("insert host range fail. dst=0x%llx, count=%llu", dst, byte_count);
            *dir = DEVMM_COPY_INVILED_DIRECTION;
            return ret;
        }

        devmm_svm_set_mapped(devmm_get_current_pid(), aligned_down_addr, aligned_count, DEVMM_MAX_DEVICE_NUM);

        dst_attr->is_svm_host = DEVMM_TRUE;
        dst_attr->is_svm_non_page = DEVMM_FALSE;
    }

    src_host = src_attr->is_svm_host || src_attr->is_local_host || src_attr->is_host_pin;
    dst_host = dst_attr->is_svm_host || dst_attr->is_local_host || dst_attr->is_host_pin;

    /*
     * copy directions checking list:
     * SRC\DST     NOT-SVM        NONPG-SVM HOST-SVM DEV-SVM
     * NOT-SVM     user-cpoy-done H2H       H2H      H2D
     * NONPG-SVM   err            err       err      err
     * HOST-SVM    H2H            H2H       H2H      H2D
     * DEV-SVM     D2H            D2H       D2H      D2D
     */
    if (src_attr->is_svm_device && dst_attr->is_svm_device) {
        *dir = DEVMM_COPY_DEVICE_TO_DEVICE;
    } else if (dst_attr->is_svm_device && src_host) {
        *dir = DEVMM_COPY_HOST_TO_DEVICE;
    } else if (src_attr->is_svm_device && dst_host) {
        *dir = DEVMM_COPY_DEVICE_TO_HOST;
    } else if (src_host && dst_host) {
        *dir = DEVMM_COPY_HOST_TO_HOST;
    } else {
        *dir = DEVMM_COPY_INVILED_DIRECTION;
        devmm_drv_err("invalid copy direction, src_host(%d), dst_host(%d), src_dev(%d), dst_dev(%d)\n", src_host,
                      dst_host, src_attr->is_svm_device, dst_attr->is_svm_device);
        return -EINVAL;
    }

    return 0;
}

STATIC int devmm_is_host(struct devmm_memory_attributes *attr)
{
    return (attr->is_svm_host || attr->is_local_host || attr->is_host_pin) ? DEVMM_TRUE : DEVMM_FALSE;
}

STATIC void devmm_find_memcpy_dir(enum devmm_copy_direction *dir, struct devmm_memory_attributes *src_attr,
                                  struct devmm_memory_attributes *dst_attr)
{
    /*
     * copy directions checking list:
     * SRC\DST     NOT-SVM        NONPG-SVM HOST-SVM DEV-SVM
     * NOT-SVM     user-cpoy-done H2H       H2H      H2D
     * NONPG-SVM   err            err       err      err
     * HOST-SVM    H2H            H2H       H2H      H2D
     * DEV-SVM     D2H            D2H       D2H      D2D
     */
    if (src_attr->is_svm_device && dst_attr->is_svm_device) {
        *dir = DEVMM_COPY_DEVICE_TO_DEVICE;
    } else if (dst_attr->is_svm_device && devmm_is_host(src_attr)) {
        *dir = DEVMM_COPY_HOST_TO_DEVICE;
    } else if (src_attr->is_svm_device && devmm_is_host(dst_attr)) {
        *dir = DEVMM_COPY_DEVICE_TO_HOST;
    } else if (devmm_is_host(src_attr) && devmm_is_host(dst_attr)) {
        *dir = DEVMM_COPY_HOST_TO_HOST;
    } else {
        devmm_drv_err("Don't support this type, config: src = %d, dst = %d.\n", dst_attr->is_svm_host,
                      dst_attr->is_svm_host);
        *dir = DEVMM_COPY_INVILED_DIRECTION;
    }
}

STATIC int devmm_fill_dmanode(struct devmm_dma_block *blks, struct devmm_dma_block *dst_blks,
                              struct devdrv_dma_node *dma_node, struct devmm_copy_res *buf)
{
    int copy_direction = buf->copy_direction;
    u32 src_dev_id = buf->dev_id;
    u32 dst_dev_id = buf->dst_dev_id;
    int ret = 0;

    if (copy_direction == DEVMM_COPY_DEVICE_TO_DEVICE) {
        dma_node->src_addr = blks->pa;
        ret = devdrv_devmem_addr_d2h(dst_dev_id, dst_blks->pa, &dma_node->dst_addr);
        if (ret) {
            devmm_drv_err("pcie fill pa-to-barpa(dev_id=%d,copy_direction=%d) fail, ret=%d.\n",
                          dst_dev_id, copy_direction, ret);
            return ret;
        }
        ret = devdrv_devmem_addr_bar_to_dma(src_dev_id, dst_dev_id, dma_node->dst_addr, &dma_node->dst_addr);
        if (ret) {
            devmm_drv_err("pcie fill bar2dma(dev_id=(%d,%d),bar=0x%llx) fail, ret=%d.\n", src_dev_id, dst_dev_id,
                          dma_node->dst_addr, ret);
            return ret;
        }
        dma_node->direction = DEVDRV_DMA_DEVICE_TO_HOST;
    } else if (copy_direction == DEVMM_COPY_DEVICE_TO_HOST) {
        dma_node->src_addr = blks->pa;
        dma_node->dst_addr = dst_blks->dma;
        dma_node->direction = DEVDRV_DMA_DEVICE_TO_HOST;
    } else if (copy_direction == DEVMM_COPY_HOST_TO_DEVICE) {
        dma_node->src_addr = blks->dma;
        dma_node->dst_addr = dst_blks->pa;
        dma_node->direction = DEVDRV_DMA_HOST_TO_DEVICE;
    } else {
        devmm_drv_err("the copy direction not support, copy_direction = %d.\n", copy_direction);
        return -EINVAL;
    }

    return 0;
}

STATIC int devmm_make_dmanode_list(DVdeviceptr src, DVdeviceptr dst, size_t count, struct devmm_copy_res *buf)
{
    size_t sz_src, sz_dst, off_src, off_dst, end_blk_src, end_blk_dst, total_len;
    u32 idx_src, idx_dst, idx_dma;
    int ret = 0;

    off_src = src & (buf->from.blk_page_size - 1);
    off_dst = dst & (buf->to.blk_page_size - 1);

    for (total_len = 0, idx_src = 0, idx_dst = 0, idx_dma = 0;
        (total_len < count) && (idx_dst < buf->to.num) && (idx_src < buf->from.num);) {
        sz_src = buf->from.blks[idx_src].sz;
        sz_dst = buf->to.blks[idx_dst].sz;
        end_blk_src = min(sz_src - off_src, count - total_len);
        end_blk_dst = min(sz_dst - off_dst, count - total_len);

        ret = devmm_fill_dmanode(&buf->from.blks[idx_src], &buf->to.blks[idx_dst], &buf->dma_node[idx_dma], buf);
        if (ret) {
            break;
        }
        buf->dma_node[idx_dma].src_addr += off_src;
        buf->dma_node[idx_dma].dst_addr += off_dst;

        if (end_blk_src < end_blk_dst) {
            buf->dma_node[idx_dma].size = end_blk_src;
            idx_src++;
            off_src = 0;
            off_dst += end_blk_src;
        } else if (end_blk_src > end_blk_dst) {
            buf->dma_node[idx_dma].size = end_blk_dst;
            idx_dst++;
            off_dst = 0;
            off_src += end_blk_dst;
        } else {
            buf->dma_node[idx_dma].size = end_blk_dst;
            idx_dst++;
            off_dst = 0;
            idx_src++;
            off_src = 0;
        }
        total_len += buf->dma_node[idx_dma].size;
        idx_dma++;
    }
    buf->dma_node_num = idx_dma;

    if (total_len != count) {
        devmm_drv_err("make dma node-size check fail.(total_len=%lu, count=%lu, did=%d, dst_did=%d, "
            "src=0x%llx, dst=0x%llx)(src(%u,%u),dst(%u,%u)).please check addr size.\n", total_len, count,
            buf->dev_id, buf->dst_dev_id, src, dst, idx_src, buf->from.num, idx_dst, buf->to.num);
            ret = -EINVAL;
    }
    devmm_drv_switch("exit make dma nodes(0x%llx,0x%llx,0x%lx,0x%lx,%u,%u,%u,%u,%u,%u).\n", src, dst,
        total_len, count, idx_src, buf->from.num, idx_dst, buf->to.num, idx_dma, buf->dma_node_num);

    return ret;
}

STATIC void devmm_get_device_cpy_blk_num(int is_svm_huge, u64 byte_count, struct devmm_copy_side *side)
{
    if (is_svm_huge) {
        side->num = DEVMM_SIZE_TO_HUGEPAGE_MAX_NUM(byte_count);
        side->blk_page_size = devmm_svm->device_hpage_size;
    } else {
        side->num = DEVMM_SIZE_TO_PAGE_MAX_NUM(byte_count);
        side->blk_page_size = devmm_svm->device_page_size;
    }
}

STATIC struct devmm_copy_res *devmm_alloc_copy_res(u64 byte_count, u32 page_size)
{
    size_t buf_head_len, buf_blks_len, buf_dma_node_len, buf_total_len;
    struct devmm_copy_res *buf = NULL;
    u32 buf_blk_num;

    page_size = page_size ? page_size : PAGE_SIZE;
    buf_blk_num = DEVMM_SIZE_TO_PAGE_NUM(byte_count, page_size);
    buf_head_len = sizeof(struct devmm_copy_res);
    buf_blks_len = sizeof(struct devmm_dma_block) * buf_blk_num;
    buf_dma_node_len = sizeof(struct devdrv_dma_node) * (unsigned)DEVMM_BLKNUM_TO_DMANODE_MAX_NUM(buf_blk_num);

    buf = (struct devmm_copy_res *)kzalloc(buf_head_len, GFP_KERNEL);
    if (buf == NULL) {
        goto devmm_copy_res_retry;
    }
    buf->from.blks = (struct devmm_dma_block *)kmalloc(buf_blks_len, GFP_KERNEL);
    if (buf->from.blks == NULL) {
        goto devmm_copy_res_free_buf;
    }
    buf->to.blks = (struct devmm_dma_block *)kmalloc(buf_blks_len, GFP_KERNEL);
    if (buf->to.blks == NULL) {
        goto devmm_copy_res_free_from_blks;
    }
    buf->dma_node = (struct devdrv_dma_node *)kmalloc(buf_dma_node_len, GFP_KERNEL);
    if (buf->dma_node == NULL) {
        goto devmm_copy_res_free_to_blks;
    }
    buf->vmalloc_flag = DEVMM_FALSE;

    return buf;

devmm_copy_res_free_to_blks:
    kfree(buf->to.blks);
    buf->to.blks = NULL;
devmm_copy_res_free_from_blks:
    kfree(buf->from.blks);
    buf->from.blks = NULL;
devmm_copy_res_free_buf:
    kfree(buf);
    buf = NULL;

devmm_copy_res_retry:
    /* kmalloc may fail so use vmalloc try */
    buf_total_len = buf_head_len + buf_blks_len + buf_blks_len + buf_dma_node_len;
    buf = (struct devmm_copy_res *)vzalloc(buf_total_len);
    if (buf == NULL) {
        devmm_drv_err("vmalloc buf failed %lu.\n", buf_total_len);
        return NULL;
    }
    buf->from.blks = (struct devmm_dma_block *)(uintptr_t)((u64)(uintptr_t)buf + buf_head_len);
    buf->to.blks = (struct devmm_dma_block *)(uintptr_t)((u64)(uintptr_t)buf->from.blks + buf_blks_len);
    buf->dma_node = (struct devdrv_dma_node *)(uintptr_t)((u64)(uintptr_t)buf->to.blks + buf_blks_len);
    buf->vmalloc_flag = DEVMM_TRUE;

    return buf;
}

STATIC void devmm_free_copy_mem(struct devmm_copy_res *buf)
{
    if (buf == NULL) {
        return ;
    }
    if (buf->vmalloc_flag == DEVMM_FALSE) {
        kfree(buf->from.blks);
        buf->from.blks = NULL;
        kfree(buf->to.blks);
        buf->to.blks = NULL;
        kfree(buf->dma_node);
        buf->dma_node = NULL;
        kfree(buf);
        buf = NULL;
    } else {
        vfree(buf);
        buf = NULL;
    }
}

STATIC int devmm_ioctl_memcpy_process(struct devmm_svm_process *svm_process,
                                      struct devmm_ioctl_arg *arg,
                                      struct devmm_memory_attributes *src_attr,
                                      struct devmm_memory_attributes *dst_attr,
                                      struct devmm_copy_res *buf);

STATIC int devmm_ioctl_memcpy_process_frame(struct devmm_svm_process *svm_pro, struct devmm_ioctl_arg *arg,
    struct devmm_memory_attributes *src_attr, struct devmm_memory_attributes *dst_attr)
{
    u32 page_size = min((u32)PAGE_SIZE, devmm_svm->device_page_size);
    DVdeviceptr dst = arg->data.copy_para.dst;
    DVdeviceptr src = arg->data.copy_para.src;
    size_t count = arg->data.copy_para.ByteCount;
    size_t byte_offset, def_offset, max_per_size;
    struct devmm_copy_res *buf = NULL;
    int ret = 0;

    if (arg->data.copy_para.direction == DEVMM_COPY_DEVICE_TO_DEVICE) {
        if (src_attr->devid == dst_attr->devid) {
            devmm_drv_err("not support. src devid(%u) eq dst devid(%u), src=0x%llx, dst=0x%llx, count=%lu\n",
                src_attr->devid, dst_attr->devid, src, dst, count);
            return -EINVAL;
        }
        if (src_attr->is_svm_huge && dst_attr->is_svm_huge) {
            page_size = devmm_svm->device_hpage_size;
        }
    }
    byte_offset = 0;
    def_offset = DEVMM_SYNC_COPY_NUM * page_size;
    max_per_size = (def_offset > count) ? count : def_offset;
    buf = devmm_alloc_copy_res(max_per_size, page_size);
    if (buf == NULL) {
        devmm_drv_err("copy buf kmalloc fail, byte_count=%lu\n", count);
        return -ENOMEM;
    }
    while (byte_offset < count) {
        arg->data.copy_para.ByteCount = (byte_offset + def_offset > count) ?
            (count - byte_offset) : def_offset;
        arg->data.copy_para.src = src;
        arg->data.copy_para.dst = dst;

        ret = devmm_ioctl_memcpy_process(svm_pro, arg, src_attr, dst_attr, buf);
        if (ret) {
            devmm_drv_err("memcpy ret(%d), src=0x%llx, dst=0x%llx, count=%lu\n", ret, src, dst,
                          arg->data.copy_para.ByteCount);
            devmm_free_copy_mem(buf);
            return ret;
        }

        byte_offset += arg->data.copy_para.ByteCount;
        src += arg->data.copy_para.ByteCount;
        dst += arg->data.copy_para.ByteCount;
    }
    devmm_free_copy_mem(buf);
    devmm_svm_stat_copy_inc(arg->data.copy_para.direction, count);

    return ret;
}

STATIC int devmm_ioctl_memcpy_h2d_pre_process(struct devmm_svm_process *svm_pro,
                                              struct devmm_ioctl_arg *arg,
                                              struct devmm_memory_attributes *src_attr,
                                              struct devmm_memory_attributes *dst_attr)
{
    DVdeviceptr dst = arg->data.copy_para.dst;
    DVdeviceptr src = arg->data.copy_para.src;
    u64 byte_count = arg->data.copy_para.ByteCount;

    if (src_attr->is_svm_non_page || src_attr->is_svm_device || dst_attr->is_local_host || dst_attr->is_host_pin ||
        dst_attr->is_svm_non_page || (byte_count == 0)) {
        devmm_drv_err("h2d memcpy, but src is_svm_non_page=%d,"
                      "src is_svm_device=%d, dst is_local_host=%d, "
                      "dst is_host_pin=%d, dst is_svm_non_page=%d, src=0x%llx, dst=0x%llx, count=%llu.\n",
                      src_attr->is_svm_non_page, src_attr->is_svm_device, dst_attr->is_local_host,
                      dst_attr->is_host_pin, dst_attr->is_svm_non_page, src, dst, byte_count);

        return -EINVAL;
    }

    return 0;
}

STATIC int devmm_make_raw_dmanode_list_h2d(struct devmm_mem_copy_convrt_para arg,
                                           struct devmm_svm_process *svm_process,
                                           struct devmm_copy_res *buf,
                                           struct devmm_memory_attributes *src_attr,
                                           struct devmm_memory_attributes *dst_attr)
{
    struct devmm_page_query_arg query_arg;
    int ret;

    buf->dst_dev_id = dst_attr->devid;
    buf->dev_id = dst_attr->devid; /* save for dma api input arg, to use device dma */
    devmm_get_device_cpy_blk_num(dst_attr->is_svm_huge, arg.count, &buf->to);

    query_arg.hostpid = svm_process->hostpid;
    query_arg.dev_id = buf->dst_dev_id;
    query_arg.va = arg.dst;
    query_arg.size = arg.count;
    query_arg.offset = arg.dst & (buf->to.blk_page_size - 1);
    query_arg.page_size = dst_attr->page_size;
    ret = devmm_query_page_by_msg(query_arg, DEVMM_CHAN_PAGE_QUERY_H2D_ID, buf->to.blks, &buf->to.num);
    if (ret || !buf->to.num) {
        ret = ret ? ret : -EINVAL;
        devmm_drv_err("h2d cp query fail, num=%d,ret=%d\n", buf->to.num, ret);
        return ret;
    }

    buf->pin_flg = src_attr->is_local_host ? DEVMM_USER_PIN_PAGES : DEVMM_UNPIN_PAGES;
    buf->from.num = DEVMM_SIZE_TO_PAGE_MAX_NUM(arg.count);
    buf->from.blk_page_size = (1ULL << PAGE_SHIFT);
    query_arg.va = arg.src;
    ret = devmm_va_to_dmalist(svm_process, query_arg, buf->pin_flg, 0, &buf->from);
    if (ret) {
        devmm_drv_err("h2d cp dmalist fail, num=%d,ret=%d\n", buf->from.num, ret);
    }
    return ret;
}

STATIC int devmm_ioctl_memcpy_h2d(struct devmm_svm_process *svm_pro, struct devmm_ioctl_arg *arg)
{
    DVdeviceptr dst = arg->data.copy_para.dst;
    DVdeviceptr src = arg->data.copy_para.src;
    struct devmm_memory_attributes src_attr;
    struct devmm_memory_attributes dst_attr;
    int ret;

    devmm_get_memory_attributes(svm_pro, src, &src_attr);
    devmm_get_memory_attributes(svm_pro, dst, &dst_attr);

    arg->data.copy_para.direction = DEVMM_COPY_HOST_TO_DEVICE;

    ret = devmm_ioctl_memcpy_h2d_pre_process(svm_pro, arg, &src_attr, &dst_attr);
    if (ret) {
        devmm_drv_err("devmm_ioctl_memcpy_h2d_pre_process failed.\n");
        return ret;
    }
    arg->data.copy_para.direction = DEVMM_COPY_HOST_TO_DEVICE;
    return devmm_ioctl_memcpy_process_frame(svm_pro, arg, &src_attr, &dst_attr);
}

STATIC int devmm_make_raw_dmanode_list_d2h(struct devmm_mem_copy_convrt_para arg,
                                           struct devmm_svm_process *svm_process,
                                           struct devmm_copy_res *buf,
                                           struct devmm_memory_attributes *src_attr,
                                           struct devmm_memory_attributes *dst_attr)
{
    struct devmm_page_query_arg query_arg;
    int ret;

    buf->dev_id = src_attr->devid;
    devmm_get_device_cpy_blk_num(src_attr->is_svm_huge, arg.count, &buf->from);

    query_arg.hostpid = svm_process->hostpid;
    query_arg.dev_id = buf->dev_id;
    query_arg.va = arg.src;
    query_arg.size = arg.count;
    query_arg.offset = arg.src & (buf->from.blk_page_size - 1);
    query_arg.page_size = src_attr->page_size;
    ret = devmm_query_page_by_msg(query_arg, DEVMM_CHAN_PAGE_QUERY_H2D_ID, buf->from.blks, &buf->from.num);
    if (ret || !buf->from.num) {
        ret = ret ? ret : -EINVAL;
        devmm_drv_err("d2h cp query fail, num=%d,ret=%d,"
                      " src=0x%llx, dst=0x%llx, ByteCount=%lu\n",
                      buf->from.num, ret, arg.src, arg.dst, arg.count);
        return ret;
    }

    buf->pin_flg = dst_attr->is_local_host ? DEVMM_USER_PIN_PAGES : DEVMM_UNPIN_PAGES;
    buf->to.num = DEVMM_SIZE_TO_PAGE_MAX_NUM(arg.count);
    buf->to.blk_page_size = (1ULL << PAGE_SHIFT);
    query_arg.va = arg.dst;
    ret = devmm_va_to_dmalist(svm_process, query_arg, buf->pin_flg, 1, &buf->to);
    if (ret)
        devmm_drv_err("d2h cp dmalist fail, dst_num=%d,ret=%d, "
                      "src=0x%llx, dst=0x%llx, ByteCount=%lu\n",
                      buf->to.num, ret, arg.src, arg.dst, arg.count);

    return ret;
}

STATIC int devmm_ioctl_memcpy_d2h_pre_process(struct devmm_svm_process *svm_pro,
                                              struct devmm_ioctl_arg *arg,
                                              struct devmm_memory_attributes *src_attr,
                                              struct devmm_memory_attributes *dst_attr)
{
    DVdeviceptr dst = arg->data.copy_para.dst;
    DVdeviceptr src = arg->data.copy_para.src;
    u64 byte_count = arg->data.copy_para.ByteCount;

    DVdeviceptr aligned_down_addr;
    u64 aligned_count;
    int ret;

    if (!src_attr->is_svm_device || dst_attr->is_svm_device || (byte_count == 0)) {
        devmm_drv_err("dir err.src=0x%llx.dst=0x%llx, count=%llu, "
                      "src is_svm_device=%d, dst is_svm_device=%d\n",
                      src, dst, byte_count, src_attr->is_svm_device, dst_attr->is_svm_device);
        return -EINVAL;
    }

    if (dst_attr->is_svm_non_page) {
        devmm_acquire_aligned_addr_and_cnt(dst, byte_count, dst_attr->is_svm_huge, &aligned_down_addr,
                                           &aligned_count);

        ret = devmm_insert_host_page_range(svm_pro, aligned_down_addr, aligned_count, dst_attr);
        if (ret) {
            devmm_drv_err("insert host range fail. src=0x%llx, dst=0x%llx, count=%llu", src, dst, byte_count);
            return ret;
        }

        devmm_svm_stat_copy_inc(DEVMM_COPY_DEVICE_TO_HOST, byte_count);

        devmm_svm_set_mapped(devmm_get_current_pid(), aligned_down_addr, aligned_count, DEVMM_MAX_DEVICE_NUM);

        dst_attr->is_svm_host = DEVMM_TRUE;
        dst_attr->is_svm_non_page = DEVMM_FALSE;
    }

    return 0;
}

STATIC int devmm_ioctl_memcpy_d2h(struct devmm_svm_process *svm_pro, struct devmm_ioctl_arg *arg)
{
    DVdeviceptr dst = arg->data.copy_para.dst;
    DVdeviceptr src = arg->data.copy_para.src;
    struct devmm_memory_attributes src_attr;
    struct devmm_memory_attributes dst_attr;
    int ret;

    devmm_get_memory_attributes(svm_pro, src, &src_attr);
    devmm_get_memory_attributes(svm_pro, dst, &dst_attr);

    devmm_drv_switch("enter (dst=0x%llx,src=0x%llx).\n", dst, src);
    arg->data.copy_para.direction = DEVMM_COPY_DEVICE_TO_HOST;

    ret = devmm_ioctl_memcpy_d2h_pre_process(svm_pro, arg, &src_attr, &dst_attr);
    if (ret) {
        devmm_drv_err("d2h_pre_process fail. ret=%d, src=0x%llx, dst=0x%llx\n",
            ret, src, dst);
        return ret;
    }

    return devmm_ioctl_memcpy_process_frame(svm_pro, arg, &src_attr, &dst_attr);
}

STATIC int devmm_make_raw_dmanode_list_d2d(struct devmm_mem_copy_convrt_para arg,
                                           struct devmm_svm_process *svm_process,
                                           struct devmm_copy_res *buf,
                                           struct devmm_memory_attributes *src_attr,
                                           struct devmm_memory_attributes *dst_attr)
{
    struct devmm_page_query_arg query_arg;
    int ret;

    buf->dev_id = src_attr->devid;
    devmm_get_device_cpy_blk_num(src_attr->is_svm_huge, arg.count, &buf->from);
    query_arg.hostpid = svm_process->hostpid;
    query_arg.dev_id = buf->dev_id;
    query_arg.va = arg.src;
    query_arg.size = arg.count;
    query_arg.offset = arg.src & (buf->from.blk_page_size - 1);
    query_arg.page_size = src_attr->page_size;
    ret = devmm_query_page_by_msg(query_arg, DEVMM_CHAN_PAGE_QUERY_H2D_ID, buf->from.blks, &buf->from.num);
    if (ret || !buf->from.num) {
        ret = ret ? ret : -EINVAL;
        devmm_drv_err("d2d cp query fail, num=%d,ret=%d, src=0x%llx, dst=0x%llx, count=%lu\n",
                      buf->from.num, ret, arg.src, arg.dst, arg.count);
        return ret;
    }
    buf->dst_dev_id = dst_attr->devid;
    devmm_get_device_cpy_blk_num(dst_attr->is_svm_huge, arg.count, &buf->to);
    query_arg.dev_id = buf->dst_dev_id;
    query_arg.va = arg.dst;
    query_arg.offset = arg.dst & (buf->to.blk_page_size - 1);
    query_arg.page_size = dst_attr->page_size;
    ret = devmm_query_page_by_msg(query_arg, DEVMM_CHAN_PAGE_QUERY_H2D_ID, buf->to.blks, &buf->to.num);
    if (ret || !buf->to.num) {
        devmm_drv_err("d2d cp query fail, num=%d,ret=%d, src=0x%llx, dst=0x%llx, count=%lu\n",
                      buf->to.num, ret, arg.src, arg.dst, arg.count);
        return ret;
    }
    devmm_drv_switch("(dev_id=%u,dst_dev_id=%u, src=0x%llx, dst=0x%llx, count=%lu).\n",
                     buf->dev_id, buf->dst_dev_id, arg.src, arg.dst, arg.count);
    return ret;
}

STATIC int devmm_ioctl_memcpy_d2d_pre_process(struct devmm_svm_process *svm_pro,
                                              struct devmm_ioctl_arg *arg,
                                              struct devmm_memory_attributes *src_attr,
                                              struct devmm_memory_attributes *dst_attr)
{
    DVdeviceptr dst = arg->data.copy_para.dst;
    DVdeviceptr src = arg->data.copy_para.src;
    u64 byte_count = arg->data.copy_para.ByteCount;

    if (!src_attr->is_svm_device || (byte_count == 0) || !dst_attr->is_svm_device) {
        devmm_drv_err("memcpy_d2d err. src=0x%llx, dst=0x%llx, count=%llu, src(%d) and dst(%d) not svm.\n",
            src, dst, byte_count, src_attr->is_svm_device, dst_attr->is_svm_device);
        return -EINVAL;
    }

    return 0;
}

STATIC int devmm_ioctl_memcpy_peer(struct devmm_svm_process *svm_pro, struct devmm_ioctl_arg *arg)
{
    DVdeviceptr dst = arg->data.copy_para.dst;
    DVdeviceptr src = arg->data.copy_para.src;
    struct devmm_memory_attributes src_attr;
    struct devmm_memory_attributes dst_attr;
    int ret;

    devmm_get_memory_attributes(svm_pro, src, &src_attr);
    devmm_get_memory_attributes(svm_pro, dst, &dst_attr);

    arg->data.copy_para.direction = DEVMM_COPY_DEVICE_TO_DEVICE;

    ret = devmm_ioctl_memcpy_d2d_pre_process(svm_pro, arg, &src_attr, &dst_attr);
    if (ret) {
        arg->data.copy_para.direction = DEVMM_COPY_INVILED_DIRECTION;
        devmm_drv_err("d2d degenerate d2h fail.src=0x%llx,dst=0x%llx, ret=%d.\n",
                      src, dst, ret);
        return ret;
    }

    ret = devmm_ioctl_memcpy_process_frame(svm_pro, arg, &src_attr, &dst_attr);
    if (ret) {
        devmm_drv_err("memcpy_d2d_process fail, ret=%d, src=0x%llx, dst=0x%llx.\n",
                      ret, src, dst);
    }

    return ret;
}

STATIC int devmm_make_raw_dmanode_list(struct devmm_mem_copy_convrt_para para,
                                       struct devmm_svm_process *svm_process,
                                       struct devmm_copy_res *buf,
                                       struct devmm_memory_attributes *src_attr,
                                       struct devmm_memory_attributes *dst_attr)
{
    devmm_make_raw_dmanode_list_fun make_raw_dmanode_list[] = {
        NULL,
        devmm_make_raw_dmanode_list_h2d,
        devmm_make_raw_dmanode_list_d2h,
        devmm_make_raw_dmanode_list_d2d,
        NULL};
    int ret;

    if ((buf->copy_direction == DEVMM_COPY_HOST_TO_HOST) ||
        (buf->copy_direction >= DEVMM_COPY_INVILED_DIRECTION)) {
        devmm_drv_err("dir err(%d). src=0x%llx, dst=0x%llx, count=%lu.\n",
                      buf->copy_direction, para.src, para.dst, para.count);
        return -EINVAL;
    }
    ret = make_raw_dmanode_list[buf->copy_direction](para, svm_process, buf, src_attr, dst_attr);
    if (ret) {
        devmm_drv_err("raw_dmanode_list fail ret=%d, src=0x%llx, dst=0x%llx, count=%lu\n",
                      ret, para.src, para.dst, para.count);
    }
    return ret;
}

STATIC void devmm_free_raw_dmanode_list(struct devmm_copy_res *buf)
{
    /* D2D donot dma map free* */
    if (buf->copy_direction == DEVMM_COPY_DEVICE_TO_HOST) { /* D2H */
        devmm_va_to_dmalist_free(buf->dev_id, buf->pin_flg, &buf->to);
    }
    if (buf->copy_direction == DEVMM_COPY_HOST_TO_DEVICE) { /* H2D */
        devmm_va_to_dmalist_free(buf->dst_dev_id, buf->pin_flg, &buf->from);
    }
}

STATIC int devmm_ioctl_memcpy_process(struct devmm_svm_process *svm_process,
                                      struct devmm_ioctl_arg *arg,
                                      struct devmm_memory_attributes *src_attr,
                                      struct devmm_memory_attributes *dst_attr,
                                      struct devmm_copy_res *buf)
{
    struct devmm_mem_copy_convrt_para para;
    int ret;

    para.dst = arg->data.copy_para.dst;
    para.src = arg->data.copy_para.src;
    para.count = arg->data.copy_para.ByteCount;

    buf->copy_direction = arg->data.copy_para.direction;
    ret = devmm_make_raw_dmanode_list(para, svm_process, buf, src_attr, dst_attr);
    if (ret) {
        devmm_drv_err("raw_dmanode fail ret=%d, src=0x%llx, dst=0x%llx, count=%lu.\n",
                      ret, para.src, para.dst, para.count);
        return ret;
    }

    ret = devmm_make_dmanode_list(para.src, para.dst, para.count, buf);
    if (ret) {
        devmm_drv_err("cp make_dmanode_list, num=%d,ret=%d, src=0x%llx, dst=0x%llx, count=%lu.\n",
                      buf->to.num, ret, para.src, para.dst, para.count);
        goto dma_node_unpin_flag;
    }
    /* d2d use source device dma. h2d&d2h use device dma */
    ret = devmm_dma_sync_link_copy(buf->dev_id, buf->dma_node, buf->dma_node_num);
    if (ret) {
        devmm_drv_err("dma_sync_copy fail(%d). src=0x%llx, dst=0x%llx, count=%lu.\n",
                      ret, para.src, para.dst, para.count);
    }

dma_node_unpin_flag:
    devmm_free_raw_dmanode_list(buf);

    return ret;
}

STATIC int devmm_ioctl_memcpy_proc(struct devmm_svm_process *svm_pro, struct devmm_ioctl_arg *arg)
{
    DVdeviceptr dst = arg->data.copy_para.dst;
    DVdeviceptr src = arg->data.copy_para.src;
    enum devmm_copy_direction dir = DEVMM_COPY_INVILED_DIRECTION;
    struct devmm_memory_attributes src_attr;
    struct devmm_memory_attributes dst_attr;
    int ret;

    devmm_get_memory_attributes(svm_pro, src, &src_attr);
    devmm_get_memory_attributes(svm_pro, dst, &dst_attr);

    devmm_drv_switch("enter devmm_ioctl_memcpy(dst=0x%llx,src=0x%llx,ByteCount=0x%lx).\n",
        arg->data.copy_para.src, arg->data.copy_para.dst, arg->data.copy_para.ByteCount);

    ret = devmm_memcpy_pre_process(svm_pro, arg, &src_attr, &dst_attr, &dir);
    if (ret) {
        devmm_drv_err("memcpu pre process fail, src=0x%llx, dst=0x%llx, count=0x%lx\n",
            arg->data.copy_para.src, arg->data.copy_para.dst, arg->data.copy_para.ByteCount);
        arg->data.copy_para.direction = DEVMM_COPY_INVILED_DIRECTION;
        return ret;
    }

    arg->data.copy_para.direction = dir;

    if ((dir > DEVMM_COPY_HOST_TO_HOST) && (dir < DEVMM_COPY_INVILED_DIRECTION)) {
        return devmm_ioctl_memcpy_process_frame(svm_pro, arg, &src_attr, &dst_attr);
    } else if (dir == DEVMM_COPY_HOST_TO_HOST) {
        devmm_svm_stat_copy_inc(DEVMM_COPY_HOST_TO_HOST, arg->data.copy_para.ByteCount);
    } else {
        devmm_drv_err("dir err(%d). src=0x%llx, dst=0x%llx.\n", dir, dst, src);
        return -EINVAL;
    }

    return 0;
}

STATIC int devmm_ioctl_convert_addr_pre_process(struct devmm_svm_process *svm_process,
                                                struct devmm_ioctl_arg *arg,
                                                struct devmm_memory_attributes *src_attr,
                                                struct devmm_memory_attributes *dst_attr)
{
    DVdeviceptr src = arg->data.convrt_para.pSrc;
    DVdeviceptr dst = arg->data.convrt_para.pDst;
    u64 byte_count = arg->data.convrt_para.len;
    DVdeviceptr aligned_down_addr;
    u64 aligned_count;
    int ret;

    if (src_attr->is_svm_non_page || (!src_attr->is_svm_device && !dst_attr->is_svm_device)) {
        devmm_drv_err("ioctl_convert_addr error. src=0x%llx, dst=0x%llx, count=%llu. "
                      "src is_svm_non_page(%d), src is_svm_device(%d), dst is_svm_device(%d)\n",
                      src, dst, byte_count, src_attr->is_svm_non_page, src_attr->is_svm_device,
                      dst_attr->is_svm_device);
        return -EINVAL;
    }

    if (dst_attr->is_svm_non_page) {
        devmm_acquire_aligned_addr_and_cnt(dst, byte_count, dst_attr->is_svm_huge, &aligned_down_addr,
                                           &aligned_count);

        ret = devmm_insert_host_page_range(svm_process, aligned_down_addr, aligned_count, dst_attr);
        if (ret) {
            devmm_drv_err("insert host range fail. src=0x%llx, dst=0x%llx, count=%llu", src, dst, byte_count);
            return ret;
        }

        devmm_svm_set_mapped(devmm_get_current_pid(), aligned_down_addr, aligned_count, DEVMM_MAX_DEVICE_NUM);

        dst_attr->is_svm_host = 1;
        dst_attr->is_svm_non_page = 0;
    }

    return 0;
}

STATIC int devmm_make_convert_para(struct devmm_ioctl_arg *arg, struct devmm_copy_res *buf, u32 dev_id)
{
    struct devdrv_dma_prepare *dma_prepare = NULL;

    if (buf->dma_node_num == 1) {
        arg->data.convrt_para.dmaAddr.phyAddr.src = (void *)(uintptr_t)buf->dma_node[0].src_addr;
        arg->data.convrt_para.dmaAddr.phyAddr.dst = (void *)(uintptr_t)buf->dma_node[0].dst_addr;
        arg->data.convrt_para.dmaAddr.phyAddr.len = buf->dma_node[0].size;
        arg->data.convrt_para.dmaAddr.phyAddr.flag = 0;
    } else {
        dma_prepare = devdrv_dma_link_prepare(dev_id, DEVDRV_DMA_DATA_TRAFFIC, buf->dma_node, buf->dma_node_num);
        if (dma_prepare == NULL) {
            devmm_drv_err("dev_id=%u, dma_node_num=%u\n", dev_id, buf->dma_node_num);
            return -EINVAL;
        }

        arg->data.convrt_para.dmaAddr.phyAddr.src = (void *)(uintptr_t)dma_prepare->sq_dma_addr;
        arg->data.convrt_para.dmaAddr.phyAddr.dst = (void *)(uintptr_t)dma_prepare->cq_dma_addr;

        arg->data.convrt_para.dmaAddr.phyAddr.len = buf->dma_node_num;
        arg->data.convrt_para.dmaAddr.phyAddr.flag = 1;
        buf->dma_prepare = dma_prepare;
    }

    return 0;
}

STATIC int devmm_convert_addr_process(struct devmm_svm_process *svm_process, struct devmm_ioctl_arg *arg,
    struct devmm_memory_attributes *src_attr, struct devmm_memory_attributes *dst_attr)
{
    u32 page_size = min((u32)PAGE_SIZE, devmm_svm->device_page_size);
    struct devmm_mem_copy_convrt_para para;
    struct devmm_copy_res *buf = NULL;
    int ret;

    para.dst = arg->data.convrt_para.pDst;
    para.src = arg->data.convrt_para.pSrc;
    para.count = (size_t)arg->data.convrt_para.len;
    if (arg->data.convrt_para.direction == DEVMM_COPY_DEVICE_TO_DEVICE) {
        if (src_attr->is_svm_huge && dst_attr->is_svm_huge) {
            page_size = devmm_svm->device_hpage_size;
        }
    }

    buf = devmm_alloc_copy_res(para.count, page_size);
    if (buf == NULL) {
        devmm_drv_err("copy buf kmalloc fail., src=0x%llx, dst=0x%llx, byte_count=%lu\n",
                      para.src, para.dst, para.count);
        return -ENOMEM;
    }

    arg->data.convrt_para.dmaAddr.phyAddr.priv = (void *)buf;
    buf->copy_direction = arg->data.convrt_para.direction;
    ret = devmm_make_raw_dmanode_list(para, svm_process, buf, src_attr, dst_attr);
    if (ret) {
        devmm_drv_err("raw_dmanode_list fail ret=%d, src=0x%llx, dst=0x%llx, count=%lu\n",
                      ret, para.src, para.dst, para.count);
        goto convert_free_flag;
    }

    ret = devmm_make_dmanode_list(para.src, para.dst, para.count, buf);
    if (ret) {
        devmm_drv_err("devmm_make_dmanode_list fail(%d). src=0x%llx, dst=0x%llx, count=%lu\n",
                      ret, para.src, para.dst, para.count);
        goto convert_dmalist_free;
    }
    /* d2d use source device dma. h2d&d2h use device dma */
    ret = devmm_make_convert_para(arg, buf, buf->dev_id);
    if (ret) {
        devmm_drv_err("make_convert_para error, ret=%d, src=0x%llx, dst=0x%llx, count=%lu\n",
                      ret, para.src, para.dst, para.count);
        goto convert_dmalist_free;
    }
    devmm_svm_stat_copy_inc(arg->data.convrt_para.direction, para.count);

    return 0;

convert_dmalist_free:
    devmm_free_raw_dmanode_list(buf);
convert_free_flag:
    devmm_free_copy_mem(buf);
    return ret;
}

STATIC int devmm_destroy_dma_addr(struct DMA_ADDR dma_addr)
{
    struct devmm_copy_res *buf = dma_addr.phyAddr.priv;
    struct devdrv_dma_prepare *dma_prepare = (buf ? buf->dma_prepare : NULL);
    int ret = 0;

    if (dma_addr.phyAddr.flag) {
        ret = devdrv_dma_link_free(dma_prepare);
        if (ret) {
            /* not need return, will free buf */
            devmm_drv_err("dma_link_free failed(%d).\n", ret);
        }
    }

    if (buf != NULL) {
        devmm_free_raw_dmanode_list(buf);
        devmm_free_copy_mem(buf);
    }

    return ret;
}

STATIC int devmm_add_dma_addr_to_list(struct devmm_svm_process *svm_pro, struct devmm_ioctl_arg *arg)
{
    struct devmm_dma_node *dma_node = NULL;
    int ret;

    dma_node = kzalloc(sizeof(struct devmm_dma_node), GFP_KERNEL);
    if (dma_node == NULL) {
        devmm_drv_err("kzalloc failed.\n");
        return -ENOMEM;
    }

    ret = memcpy_s(&dma_node->dma_addr, sizeof(struct DMA_ADDR),
                   &arg->data.convrt_para.dmaAddr, sizeof(struct DMA_ADDR));
    if (ret) {
        devmm_drv_err("memcpy_s failed, ret = %d.\n", ret);
        kfree(dma_node);
        dma_node = NULL;
        return -ENOMEM;
    }

    ret = devmm_set_dma_addr_to_device(arg);
    if (ret) {
        devmm_drv_err("set dma_addr to device failed, ret = %d.\n", ret);
        kfree(dma_node);
        dma_node = NULL;
        return ret;
    }
    list_add_tail(&dma_node->dma_node_list, &svm_pro->dma_list);

    return 0;
}

STATIC int devmm_ioctl_convert_addr(struct devmm_svm_process *svm_pro, struct devmm_ioctl_arg *arg)
{
    DVdeviceptr src = arg->data.convrt_para.pSrc;
    DVdeviceptr dst = arg->data.convrt_para.pDst;
    size_t byte_count = arg->data.convrt_para.len;
    struct devmm_memory_attributes src_attr;
    struct devmm_memory_attributes dst_attr;
    enum devmm_copy_direction dir;
    int ret;

    if (byte_count == 0 || byte_count > DEVMM_CONVERT_MAX_SIZE) {
        devmm_drv_err("invalid count.(dst=0x%llx,src=0x%llx,count=0x%lx).\n", dst, src, byte_count);
        return -EINVAL;
    }

    devmm_get_memory_attributes(svm_pro, src, &src_attr);
    devmm_get_memory_attributes(svm_pro, dst, &dst_attr);
    ret = devmm_ioctl_convert_addr_pre_process(svm_pro, arg, &src_attr, &dst_attr);
    if (ret) {
        devmm_drv_err("convert_addr_pre_process fail, ret=%d, src=0x%llx, dst=0x%llx, count=%lu\n",
                      ret, src, dst, byte_count);
        return ret;
    }

    devmm_find_memcpy_dir(&dir, &src_attr, &dst_attr);
    if ((dir <= DEVMM_COPY_HOST_TO_HOST) || (dir >= DEVMM_COPY_INVILED_DIRECTION)) {
        devmm_drv_err("dir err=%d, (dst=0x%llx,src=0x%llx,count=%lu)\n", dir, dst, src, byte_count);
        return -EINVAL;
    }
    arg->data.convrt_para.direction = dir;
    ret = devmm_convert_addr_process(svm_pro, arg, &src_attr, &dst_attr);
    if (ret) {
        devmm_drv_err("convert addr failed. dir=%d ret=%d, devid=%d, (dst=0x%llx,src=0x%llx,count=%lu)\n",
            dir, ret, arg->head.devid, dst, src, byte_count);
        return ret;
    }

    ret = devmm_add_dma_addr_to_list(svm_pro, arg);
    if (ret) {
        devmm_drv_err("save dma_addr failed, ret = %d.(dst=0x%llx,src=0x%llx,count=%lu).\n",
            ret, dst, src, byte_count);
        (void)devmm_destroy_dma_addr(arg->data.convrt_para.dmaAddr);
        return ret;
    }

    return 0;
}

STATIC int devmm_ioctl_destroy_addr(struct devmm_svm_process *svm_pro, struct devmm_ioctl_arg *arg)
{
    struct devmm_dma_node *dma_node = NULL;
    struct list_head *pos = NULL;
    struct list_head *n = NULL;
    int ret;

    ret = devmm_get_dma_addr_from_device(arg);
    if (ret) {
        devmm_drv_err("get dma addr from device failed. ret=%d devid=%d\n", ret, arg->head.devid);
        return ret;
    }

    list_for_each_safe(pos, n, &svm_pro->dma_list)
    {
        dma_node = list_entry(pos, struct devmm_dma_node, dma_node_list);
        if (dma_node->dma_addr.phyAddr.priv == arg->data.desty_para.dmaAddr.phyAddr.priv) {
            list_del(&dma_node->dma_node_list);
            kfree(dma_node);
            dma_node = NULL;
            return devmm_destroy_dma_addr(arg->data.desty_para.dmaAddr);
        }
    }

    devmm_drv_err("invalid dma addr.\n");
    return -EINVAL;
}

STATIC int devmm_raw_dmanode_list_prefetch(struct devmm_mem_aligned aligned,
    struct devmm_svm_process *svm_process, struct devmm_copy_res *buf,
    u32 dev_id, struct devmm_memory_attributes *attr)
{
    struct devmm_page_query_arg query_arg;
    u32 *page_bitmap = NULL;
    int ret, i, page_num;

    query_arg.hostpid = svm_process->hostpid;
    query_arg.dev_id = dev_id;
    query_arg.va = aligned.aligned_down_addr;
    query_arg.size = aligned.aligned_count;
    devmm_get_device_cpy_blk_num(attr->is_svm_huge, query_arg.size, &buf->to);

    page_bitmap = devmm_get_alloced_page_bitmap(svm_process, query_arg.va);
    if (page_bitmap == NULL) {
        devmm_drv_err("va=0x%lx, devid=%u, can not find bitmap.\n", query_arg.va, dev_id);
        return -EINVAL;
    }
    page_num = devmm_get_pagecount_by_size(query_arg.va, query_arg.size, attr->page_size);
    for (i = 0; i < page_num; i++) {
        devmm_page_bitmap_set_flag(page_bitmap + i, DEVMM_PAGE_ADVISE_POPULATE_MASK);
        devmm_page_bitmap_set_flag(page_bitmap + i, DEVMM_PAGE_DEV_MAPPED_MASK);
        devmm_page_bitmap_set_devid(page_bitmap + i, dev_id);
    }

    query_arg.offset = query_arg.va & (buf->to.blk_page_size - 1);
    query_arg.page_size = attr->page_size;
    ret = devmm_query_page_by_msg(query_arg, DEVMM_CHAN_PAGE_CREATE_QUERY_H2D_ID, buf->to.blks, &buf->to.num);
    if (ret || !buf->to.num) {
        devmm_drv_err("devmm_query_page_by_msg err. ret=%d, num=%u, devPtr=0x%lx, devid=%u\n",
                      ret, buf->to.num, query_arg.va, dev_id);
        /* heap page size make not real page size, buf->to.num may more than page_num, here do not clear bitmap */
        return (ret ? ret : -EINVAL);
    }

    buf->from.num = DEVMM_SIZE_TO_PAGE_MAX_NUM(query_arg.size);
    buf->from.blk_page_size = (1ULL << PAGE_SHIFT);

    ret = devmm_va_to_dmalist(svm_process, query_arg, DEVMM_UNPIN_PAGES, 0, &buf->from);
    if (ret || !buf->from.num) {
        devmm_drv_err("devmm_va_to_dmalist err. va=0x%lx, num=%u, ret=%d\n", query_arg.va, buf->from.num, ret);
        ret = -EINVAL;
    }

    return ret;
}

STATIC void devmm_clear_hostmapped_prefetch(struct devmm_svm_process *svm_pro, DVdeviceptr devPtr, size_t count,
                                            u32 dev_id, struct devmm_memory_attributes *attr)
{
    u32 adjust_order, page_size, num, i;
    u32 *page_bitmap = NULL;
    int ret;

    page_bitmap = devmm_get_alloced_page_bitmap(svm_pro, devPtr);
    if (page_bitmap == NULL) {
        devmm_drv_err("devPtr=0x%llx, count=%ld, devid=%u, can not find bitmap.\n",
                      devPtr, count, dev_id);
        return;
    }

    if (attr->is_svm_huge) {
        adjust_order = devmm_host_hugepage_fault_adjust_order();
        page_size = devmm_svm->device_hpage_size;
    } else {
        adjust_order = 0;
        page_size = PAGE_SIZE;
    }
    num = round_up(count, page_size) / page_size;
    for (i = 0; i < num; i++) {
        if (devmm_page_bitmap_is_host_mapped(page_bitmap + i)) {
            ret = devmm_unmap_pages(svm_pro, devPtr + i * (unsigned long)page_size, adjust_order);
            if (ret) {
                devmm_drv_err("unmap_host_range fail, devPtr=0x%llx"
                              "chunk_page_size=%u, adjust_order=%u, devid=%u\n",
                              devPtr, page_size, adjust_order, dev_id);
            }
            devmm_page_bitmap_clear_flag(page_bitmap + i, DEVMM_PAGE_HOST_MAPPED_MASK);
        }
    }
}

STATIC int devmm_prefetch_to_device(struct devmm_svm_process *svm_pro, DVdeviceptr dev_ptr,
    u64 byte_count, u32 dev_id, struct devmm_memory_attributes *attr)
{
    struct devmm_copy_res *buf = NULL;
    struct devmm_mem_aligned aligned;
    DVdeviceptr aligned_down_addr;
    u64 aligned_count;
    int ret;

    devmm_acquire_aligned_addr_and_cnt(dev_ptr, byte_count, attr->is_svm_huge, &aligned_down_addr, &aligned_count);
    /* page size use device and host min to get max buf size */
    buf = devmm_alloc_copy_res(aligned_count, min((u32)PAGE_SIZE, devmm_svm->device_page_size));
    if (buf == NULL) {
        devmm_drv_err("copy buf kzalloc fail."
                      "ptr=0x%llx, count=%llu, aligned_down_addr=0x%llx, aligned_count=%llu\n",
                      dev_ptr, byte_count, aligned_down_addr, aligned_count);
        return -ENOMEM;
    }

    aligned.aligned_down_addr = aligned_down_addr;
    aligned.aligned_count = aligned_count;
    ret = devmm_raw_dmanode_list_prefetch(aligned, svm_pro, buf, dev_id, attr);
    if (ret) {
        devmm_drv_err("raw_dmanode_list_prefetch fail(ret=%d). ptr=0x%llx, count=%llu,"
                      " aligned_down_addr=0x%llx, aligned_count=%llu, dev_id=%u\n",
                      ret, dev_ptr, byte_count, aligned_down_addr, aligned_count, dev_id);
        goto prefetch_free_buf_flag;
    }

    buf->copy_direction = DEVMM_COPY_HOST_TO_DEVICE;
    ret = devmm_make_dmanode_list(aligned_down_addr, aligned_down_addr, aligned_count, buf);
    if (ret) {
        devmm_drv_err("d2h-cp devmm_make_dmanode_list fail(%d). ptr=0x%llx, count=%llu,"
                      " aligned_down_addr=0x%llx, aligned_count=%llu, dev_id=%u\n",
                      ret, dev_ptr, byte_count, aligned_down_addr, aligned_count, dev_id);
        goto prefetch_unpin_flag;
    }

    ret = devmm_dma_sync_link_copy(dev_id, buf->dma_node, buf->dma_node_num);
    if (ret) {
        devmm_drv_err("devdrv_dma_sync_copy fail(%d). dma_node_num=%u, ptr=0x%llx, "
                      "count=%llu, aligned_down_addr=0x%llx, aligned_count=%llu, dev_id=%u\n",
                      ret, buf->dma_node_num, dev_ptr, byte_count, aligned_down_addr, aligned_count, dev_id);
        goto prefetch_unpin_flag;
    }
    devmm_clear_hostmapped_prefetch(svm_pro, aligned_down_addr, aligned_count, dev_id, attr);

prefetch_unpin_flag:
    devmm_va_to_dmalist_free(dev_id, DEVMM_UNPIN_PAGES, &buf->from);
prefetch_free_buf_flag:
    devmm_free_copy_mem(buf);

    return ret;
}

STATIC int devmm_prefetch_to_device_proc(struct devmm_svm_process *svm_pro,
    u64 dev_ptr, u64 byte_count, u32 dev_id)
{
    struct devmm_memory_attributes attr;
    u64 byte_offset, cnt, per_max_cnt;
    u64 ptr = dev_ptr;
    int ret = -EINVAL;

    devmm_get_memory_attributes(svm_pro, dev_ptr, &attr);
    per_max_cnt = DEVMM_SYNC_COPY_NUM * PAGE_SIZE;
    for (byte_offset = 0; byte_offset < byte_count; byte_offset += cnt, ptr += cnt) {
        cnt = min((byte_count - byte_offset), per_max_cnt);
        ret = devmm_prefetch_to_device(svm_pro, ptr, cnt, dev_id, &attr);
        if (ret) {
            devmm_drv_err("prefetch ret(%d), ptr=0x%llx, count=%llu\n", ret, ptr, cnt);
            return ret;
        }
        devmm_drv_switch("prefetch ret(%d), ptr=0x%llx, count=%llu\n", ret, ptr, cnt);
    }

    return ret;
}

STATIC int devmm_prefetch_to_device_frame(struct devmm_svm_process *svm_pro,
    struct devmm_page_query_arg query_arg, u32 *page_bitmap, u64 *num)
{
    u64 i, last_prefetch, page_cnt, prefetch_cnt, ptr;
    int ret;

    page_cnt = *num;
    /* va not continued in host */
    for (last_prefetch = 0, i = 0; i < page_cnt; i++) {
        if (devmm_page_bitmap_is_host_mapped(page_bitmap + i)) {
            continue;
        } else if (!devmm_page_bitmap_is_dev_mapped(page_bitmap + i)) {
            /* host and devcie not mapped, goto advise, return i for pretched num */
            break;
        }
        prefetch_cnt = i - last_prefetch;
        if (prefetch_cnt == 0) {
            last_prefetch = i + 1;
            continue;
        }
        prefetch_cnt *= query_arg.page_size;
        ptr = query_arg.va + last_prefetch * query_arg.page_size;
        ret = devmm_prefetch_to_device_proc(svm_pro, ptr, prefetch_cnt, query_arg.dev_id);
        if (ret) {
            devmm_drv_err("prefetch ret(%d), ptr=0x%llx, count=%llu.\n", ret, ptr, prefetch_cnt);
            return ret;
        }
        last_prefetch = i + 1;
    }
    /* last take */
    prefetch_cnt = i - last_prefetch;
    if (prefetch_cnt == 0) {
        *num = i;
        return 0;
    }
    prefetch_cnt *= query_arg.page_size;
    ptr = query_arg.va + last_prefetch * query_arg.page_size;
    ret = devmm_prefetch_to_device_proc(svm_pro, ptr, prefetch_cnt, query_arg.dev_id);
    if (ret) {
        devmm_drv_err("prefetch ret(%d), ptr=0x%llx, count=%llu.\n", ret, ptr, prefetch_cnt);
        return ret;
    }
    *num = i;
    devmm_drv_switch("prefetch num(%llu),ptr=0x%lx,count=(%lu,%llu).\n", i, query_arg.va, query_arg.size, prefetch_cnt);
    return 0;
}

STATIC int devmm_populate_to_device_frame(struct devmm_svm_process *svm_pro,
    struct devmm_page_query_arg query_arg, u32 *page_bitmap, u64 *num)
{
    u32 page_cnt, i, populate_num;
    int ret;

    page_cnt = *num;
    for (i = 0, populate_num = 0; i < page_cnt; i++) {
        if (devmm_page_bitmap_is_host_mapped(page_bitmap + i) ||
            devmm_page_bitmap_is_dev_mapped(page_bitmap + i)) {
            /* is host or device maped goto prefetch */
            break;
        }
        devmm_page_bitmap_set_devid(page_bitmap + i, query_arg.dev_id);
        devmm_page_bitmap_set_flag(page_bitmap + i, DEVMM_PAGE_DEV_MAPPED_MASK);
        devmm_page_bitmap_set_flag(page_bitmap + i, DEVMM_PAGE_ADVISE_POPULATE_MASK);
        populate_num++;
    }
    page_cnt = populate_num;
    query_arg.size = query_arg.page_size * (unsigned long)page_cnt;  /* just create size of page cnt */
    /* populate_num will not eq 0 */
    ret = devmm_query_page_by_msg(query_arg, DEVMM_CHAN_PAGE_CREATE_H2D_ID, NULL, &page_cnt);
    if (ret || (page_cnt == 0)) {
        devmm_drv_err("query_page err. ret=%d, va=0x%lx, count=%lu, page_cnt=(%u, %llu).\n",
                      ret, query_arg.va, query_arg.size, page_cnt, *num);
        for (i = page_cnt; i < populate_num; i++) {
            devmm_page_bitmap_clear_flag(page_bitmap + i, DEVMM_PAGE_ADVISE_POPULATE_MASK);
            devmm_page_bitmap_clear_flag(page_bitmap + i, DEVMM_PAGE_DEV_MAPPED_MASK);
        }
        return (ret ? ret : -EINVAL);
    }
    *num = (u64)page_cnt;
    devmm_drv_switch("populated num(%u), ptr=0x%lx, count=(%lu).\n", page_cnt, query_arg.va, query_arg.size);
    return 0;
}

STATIC int devmm_advise_populate_arg_check(struct devmm_svm_heap *heap,
    u32 *page_bitmap, struct devmm_ioctl_arg *arg, u64 page_cnt)
{
    u64 i;

    if ((arg->data.advise_para.devPtr & (heap->chunk_page_size - 1)) != 0) {
        devmm_drv_err("devmm devPtr not align err. va=0x%llx, device=%u\n",
                      arg->data.advise_para.devPtr, arg->head.devid);
        return -EINVAL;
    }
    /* if locked device, device id must eq device get form bitmap */
    if (devmm_page_bitmap_is_locked_device(page_bitmap) &&
        (devmm_page_bitmap_get_devid(page_bitmap) != arg->head.devid)) {
        devmm_drv_err("devid err. va=0x%llx, locked did=%u, advise did=%u.\n",
            arg->data.advise_para.devPtr, devmm_page_bitmap_get_devid(page_bitmap), arg->head.devid);
        return -EINVAL;
    }
    for (i = 0; i < page_cnt; i++) {
        if (devmm_page_bitmap_is_dev_mapped(page_bitmap + i) &&
            (devmm_page_bitmap_get_devid(page_bitmap + i) != arg->head.devid)) {
            /* mapped by device, but deivce id is not same */
            devmm_drv_err("mapped by device, but advise devid(%u) err. va=0x%llx, num=%llu.\n",
                arg->head.devid, arg->data.advise_para.devPtr, i);
            return -EINVAL;
        }
    }
    return 0;
}

STATIC int devmm_advise_populate_process(struct devmm_svm_process *svm_pro, struct devmm_svm_heap *heap,
    u32 *page_bitmap, struct devmm_ioctl_arg *arg, u64 page_cnt)
{
    struct devmm_page_query_arg query_arg;
    u64 i, num;
    int ret;

    if (devmm_advise_populate_arg_check(heap, page_bitmap, arg, page_cnt)) {
        devmm_drv_err("devmm populate_arg_check err. va=0x%llx, device=%u\n",
                      arg->data.advise_para.devPtr, arg->head.devid);
        return -EINVAL;
    }

    query_arg.hostpid = svm_pro->hostpid;
    query_arg.dev_id = arg->head.devid;
    query_arg.page_size = heap->chunk_page_size;
    query_arg.offset = 0;
    for (i = 0; i < page_cnt; i += num) {
        num = page_cnt - i;
        query_arg.va = arg->data.advise_para.devPtr + i * heap->chunk_page_size;
        query_arg.size = arg->data.advise_para.count - i * heap->chunk_page_size;
        if (devmm_page_bitmap_is_host_mapped(page_bitmap + i)) {
            /* mapped by host prefetch data to device */
            ret = devmm_prefetch_to_device_frame(svm_pro, query_arg, (page_bitmap + i), &num);
        } else if (!devmm_page_bitmap_is_dev_mapped(page_bitmap + i)) {
            /* not maped by host and device populate page on device */
            ret = devmm_populate_to_device_frame(svm_pro, query_arg, (page_bitmap + i), &num);
        } else {
            /* mapped by device, next page */
            ret = 0;
            num = 1;
        }
        if (ret || num == 0) {
            devmm_drv_err("devmm populate err. va=0x%lx, count=%lu, device=%u, num=%llu.\n",
                          query_arg.va, query_arg.size, arg->head.devid, num);
            return ret;
        }
        devmm_drv_switch("devmm populated. va=0x%lx, count=%lu, device=%u, num=%llu, %llu.\n",
                         query_arg.va, query_arg.size, arg->head.devid, num, i);
    }

    return 0;
}

STATIC int devmm_advise_d2d_populate_process(struct devmm_svm_process *svm_pro, struct devmm_svm_heap *heap,
    u32 *page_bitmap, struct devmm_ioctl_arg *arg, u64 paget_cnt)
{
    struct devmm_page_query_arg query_arg;
    u32 num;
    int ret;
    u64 i;

    num = (u32)paget_cnt;
    query_arg.hostpid = svm_pro->hostpid;
    query_arg.dev_id = arg->head.devid;
    query_arg.va = arg->data.advise_para.devPtr;
    query_arg.size = arg->data.advise_para.count;
    query_arg.offset = (u64)(arg->data.advise_para.devPtr & (heap->chunk_page_size - 1));
    query_arg.page_size = heap->chunk_page_size;
    for (i = 0; i < paget_cnt; i++) {
        devmm_page_bitmap_set_flag(page_bitmap + i, DEVMM_PAGE_ADVISE_MEMORY_SHARED_MASK);
    }
    ret = devmm_query_page_by_msg(query_arg, DEVMM_CHAN_PAGE_D2D_CREATE_H2D_ID, NULL, &num);
    if (ret || (num == 0)) {
        devmm_drv_err("advise_page err=%d.num=%u,va=0x%llx,count=%lu,page_cnt=%llu,page_size=%u.\n", ret, num,
            arg->data.advise_para.devPtr, arg->data.advise_para.count, paget_cnt, heap->chunk_page_size);
        for (i = num; i < paget_cnt; i++) {
            devmm_page_bitmap_clear_flag(page_bitmap + i, DEVMM_PAGE_ADVISE_MEMORY_SHARED_MASK);
        }

        return (ret ? ret : -EINVAL);
    }

    return 0;
}

STATIC int devmm_ioctl_advise_populate(struct devmm_svm_process *svm_pro, struct devmm_svm_heap *heap,
    u32 *page_bitmap, struct devmm_ioctl_arg *arg, u64 page_cnt)
{
    struct devmm_mem_advise_para *advise_para = &arg->data.advise_para;
    u32 devid_from_bitmap = devmm_page_bitmap_get_devid(page_bitmap);
    u32 is_dev_mapped = devmm_page_bitmap_is_dev_mapped(page_bitmap);
    int ret = 0;

    if (advise_para->advise & DV_ADVISE_POPULATE) {
        if (is_dev_mapped &&
            !devmm_dev_is_same_system(devmm_get_current_pid(), arg->head.devid, devid_from_bitmap)) {
            /* mapped and advise to another system device */
            ret = devmm_advise_d2d_populate_process(svm_pro, heap, page_bitmap, arg, page_cnt);
        } else {
            /* device not mapped or device mapped and advise to the same device */
            ret = devmm_advise_populate_process(svm_pro, heap, page_bitmap, arg, page_cnt);
        }
    }
    devmm_drv_switch("advise suc, devPtr=0x%llx, count=%lu, advise=0x%x, device=%u.\n",
                     advise_para->devPtr, advise_para->count, advise_para->advise, arg->head.devid);

    return ret;
}

STATIC int devmm_advise_check_and_set_bitmap(struct devmm_mem_advise_para *advise_para, u32 *page_bitmap, u64 chunk_cnt)
{
    u64 i;

    for (i = 0; i < chunk_cnt; i++) {
        if (!devmm_page_bitmap_is_page_available(page_bitmap + i)) {
            devmm_drv_err("advise_populate err not alloc , devPtr=0x%llx, count=%lu.\n",
                          advise_para->devPtr, advise_para->count);
            return -EINVAL;
        }
    }

    for (i = 0; i < chunk_cnt; i++) {
        /* if aready populate donot change mem attribute */
        if (devmm_page_bitmap_is_advise_populate(page_bitmap + i)) {
            continue;
        }
        if (advise_para->advise & DV_ADVISE_DDR) {
            /* ddr and hbm is mutual exclusion */
            devmm_page_bitmap_set_flag(page_bitmap + i, DEVMM_PAGE_ADVISE_DDR_MASK);
            devmm_page_bitmap_clear_flag(page_bitmap + i, DEVMM_PAGE_ADVISE_P2P_HBM_MASK);
        }
        if (advise_para->advise & DV_ADVISE_4G_DDR) {
            devmm_page_bitmap_set_flag(page_bitmap + i, DEVMM_PAGE_4G_MASK);
        }
        if (advise_para->advise & DV_ADVISE_P2P_HBM) {
            /* ddr and hbm is mutual exclusion */
            devmm_page_bitmap_set_flag(page_bitmap + i, DEVMM_PAGE_ADVISE_P2P_HBM_MASK);
            devmm_page_bitmap_clear_flag(page_bitmap + i, DEVMM_PAGE_ADVISE_DDR_MASK);
        }
        if (advise_para->advise & DV_ADVISE_P2P_DDR) {
            /* p2p ddr and hbm is mutual exclusion */
            devmm_page_bitmap_set_flag(page_bitmap + i, DEVMM_PAGE_ADVISE_P2P_DDR_MASK);
            devmm_page_bitmap_clear_flag(page_bitmap + i, DEVMM_PAGE_ADVISE_P2P_HBM_MASK);
        }
    }

    return 0;
}

STATIC int devmm_ioctl_advise(struct devmm_svm_process *svm_pro, struct devmm_ioctl_arg *arg)
{
    struct devmm_mem_advise_para *advise_para = &arg->data.advise_para;
    struct devmm_svm_heap *heap = NULL;
    u64 byte_count, chunk_cnt, dev_ptr;
    u32 *page_bitmap = NULL;
    int ret;

    dev_ptr = advise_para->devPtr;
    byte_count = advise_para->count;
    heap = devmm_svm_get_heap(svm_pro, dev_ptr);
    if (heap == NULL) {
        devmm_drv_err("heap is null, devPtr=0x%llx, device=%u.\n", dev_ptr, arg->head.devid);
        return -EINVAL;
    }

    page_bitmap = devmm_get_page_bitmap_with_heapidx(svm_pro, heap->heap_idx, dev_ptr);
    if ((page_bitmap == NULL) || devmm_check_va_add_size_by_heap(heap, dev_ptr, byte_count)) {
        devmm_drv_err("page_bitmap is NULL, devPtr=0x%llx, count=%llu.\n", dev_ptr, byte_count);
        return -EINVAL;
    }

    if (devmm_page_bitmap_is_locked_host(page_bitmap)) {
        devmm_drv_err("devPtr=0x%llx, count=%llu, advise=%d, device=%u, page_bitmap=0x%x"
                      " locked host, but attempt to advise to device...\n",
                      dev_ptr, byte_count, advise_para->advise, arg->head.devid, devmm_page_read_bitmap(page_bitmap));
        return -EINVAL;
    }

    chunk_cnt = devmm_get_pagecount_by_size(dev_ptr, byte_count, heap->chunk_page_size);
    ret = devmm_advise_check_and_set_bitmap(advise_para, page_bitmap, chunk_cnt);
    devmm_drv_switch("bitmap devPtr=0x%llx, count=%llu, advise=0x%x, device=%u, page_bitmap=0x%x\n", dev_ptr,
        byte_count, advise_para->advise, arg->head.devid, devmm_page_read_bitmap(page_bitmap));
    if (ret) {
        devmm_drv_err("bitmap check err devPtr=0x%llx, count=%llu, advise=0x%x, device=%u, page_bitmap=0x%x\n", dev_ptr,
                      byte_count, advise_para->advise, arg->head.devid, devmm_page_read_bitmap(page_bitmap));
        devmm_print_pre_alloced_va(svm_pro, dev_ptr);
        return ret;
    }

    ret = devmm_ioctl_advise_populate(svm_pro, heap, page_bitmap, arg, chunk_cnt);
    if (ret) {
        devmm_drv_err("advise_populate err, devPtr=0x%llx, count=%llu, advise=0x%x, device=%u.\n", dev_ptr, byte_count,
                      advise_para->advise, arg->head.devid);
        return ret;
    }
    return 0;
}

STATIC int devmm_ioctl_prefetch(struct devmm_svm_process *svm_pro, struct devmm_ioctl_arg *arg)
{
    struct devmm_mem_advise_para *prefetch_para = &arg->data.prefetch_para;
    struct devmm_svm_heap *heap = NULL;
    u64 count, dev_ptr, page_cnt, i;
    u32 *page_bitmap = NULL;
    int ret;

    dev_ptr = prefetch_para->devPtr;
    count = (u64)prefetch_para->count;

    devmm_drv_switch("prefetch devPtr=0x%llx, count=%llu, device=%u", dev_ptr, count, arg->head.devid);
    heap = devmm_svm_get_heap(svm_pro, dev_ptr);
    if (heap == NULL) {
        devmm_drv_err("heap is null, devPtr=0x%llx, cnt=%llu.\n", dev_ptr, count);
        return -EINVAL;
    }
    page_bitmap = devmm_get_page_bitmap_with_heapidx(svm_pro, heap->heap_idx, dev_ptr);
    if ((page_bitmap == NULL) || devmm_check_va_add_size_by_heap(heap, dev_ptr, count)) {
        devmm_drv_err("page_bitmap is NULL, devPtr=0x%llx, count=%llu.\n", dev_ptr, count);
        return -EINVAL;
    }
    /* locked host,lock cmd lock all alloc page ,so just judge frist page */
    if (devmm_page_bitmap_is_locked_host(page_bitmap)) {
        devmm_drv_err("devPtr=0x%llx, count=%llu, device=%u, page_bitmap=0x%x"
                      " locked host, but attempt to prefetch to device...\n",
                      dev_ptr, count, arg->head.devid, devmm_page_read_bitmap(page_bitmap));
        return -EINVAL;
    }
    page_cnt = devmm_get_pagecount_by_size(dev_ptr, count, heap->chunk_page_size);
    for (i = 0; i < page_cnt; i++) {
        if (!devmm_page_bitmap_is_page_available(page_bitmap + i)) {
            devmm_drv_err("prefetch err. not alloc , devPtr=0x%llx, %llu, count=%llu.\n",
                          dev_ptr, i, count);
            return -EINVAL;
        }
    }
    ret = devmm_advise_populate_process(svm_pro, heap, page_bitmap, arg, page_cnt);
    if (ret) {
        devmm_drv_err("prefetch fail. ptr=0x%llx, count=%llu, device=%u.\n", dev_ptr, count, arg->head.devid);
        devmm_print_pre_alloced_va(svm_pro, dev_ptr);
        return -EINVAL;
    }

    return 0;
}

STATIC int devmm_memset_fill_pages_process(struct devmm_svm_process *svm_pro,
    struct devmm_mem_memset_para *memset_para, size_t byte_count,
    struct devmm_memory_attributes *fst_attr)
{
    DVdeviceptr aligned_down_addr;
    u64 aligned_count;
    int ret;

    devmm_drv_switch("enter dst=0x%llx, count=%lu, is_svm_huge=%d\n", memset_para->dst, byte_count,
                     fst_attr->is_svm_huge);

    devmm_acquire_aligned_addr_and_cnt(memset_para->dst, byte_count, fst_attr->is_svm_huge, &aligned_down_addr,
                                       &aligned_count);

    devmm_drv_switch("aligned_down_va=0x%llx,  aligned_count=%llu, dst=0x%llx, count=%lu, is_svm_huge=%d\n",
                     aligned_down_addr, aligned_count, memset_para->dst, byte_count, fst_attr->is_svm_huge);

    ret = devmm_insert_host_page_range(svm_pro, aligned_down_addr, aligned_count, fst_attr);
    if (ret) {
        devmm_drv_err("insert host range fail. dst=0x%llx, count=%lu, ret=%d\n", memset_para->dst, byte_count, ret);
        return ret;
    }

    devmm_svm_set_mapped(devmm_get_current_pid(), aligned_down_addr, aligned_count, DEVMM_MAX_DEVICE_NUM);

    memset_para->hostmapped = 1;
    return 0;
}

STATIC int devmm_ioctl_memset_device_check(struct devmm_svm_process *svm_process, u64 va, size_t byte_count)
{
    struct devmm_svm_heap *heap = NULL;
    u32 *page_bitmap = NULL;

    heap = devmm_svm_get_heap(svm_process, va);
    if (heap == NULL) {
        devmm_drv_err("can not find heap,va=0x%llx, size=%lu.\n", va, byte_count);
        return -EINVAL;
    }

    page_bitmap = devmm_get_page_bitmap_with_heapidx(svm_process, heap->heap_idx, va);
    if ((page_bitmap == NULL) ||
        devmm_check_va_add_size_by_heap(heap, va, byte_count)) {
        devmm_drv_err("dev bitmap is null, dst=0x%llx, count=%lu\n", va, byte_count);
        return -EINVAL;
    }

    if (devmm_svm_check_bitmap_available(page_bitmap, byte_count, heap->chunk_page_size) != DEVMM_TRUE) {
        devmm_drv_err("va is not alloc, dst=0x%llx, count=%lu\n", va, byte_count);
        devmm_print_pre_alloced_va(svm_process, va);
        return -EINVAL;
    }

    return 0;
}


STATIC int devmm_ioctl_memset_device_process(struct devmm_svm_process *svm_process,
                                             struct devmm_mem_memset_para *memset_para,
                                             struct devmm_memory_attributes *attr, size_t byte_count, int unit_shift)
{
    const u32 msg_ids[DEVMM_CHAN_MEMSET_MAX] = {
        DEVMM_CHAN_MEMSET8_H2D_ID,
        DEVMM_CHAN_MEMSET16_H2D_ID,
        DEVMM_CHAN_MEMSET32_H2D_ID,
        DEVMM_CHAN_MEMSET64_H2D_ID
    };
    struct devmm_chan_memset chan_memset;
    size_t add_count, per_cnt;
    int ret;

    chan_memset.head.hostpid = svm_process->hostpid;

    if (attr->is_svm_non_page) {
        devmm_drv_err("addr is not maped by device va=0x%llx, size=%lu.\n", memset_para->dst, byte_count);
        return -EINVAL;
    }

    chan_memset.head.msg_id = msg_ids[unit_shift];
    chan_memset.value = memset_para->value;

    if (devmm_ioctl_memset_device_check(svm_process, memset_para->dst, byte_count)) {
        devmm_drv_err("va and size check err, dst=0x%llx, N=%lu\n", memset_para->dst, memset_para->N);
        return -EINVAL;
    }

    per_cnt = (unit_shift == DEVMM_CHAN_MEMSET8) ? DEVMM_MEMSET8D_SIZE_PER_MSG : DEVMM_MEMSET_SIZE_PER_MSG;
    for (add_count = 0; add_count < byte_count; add_count += per_cnt) {
        chan_memset.head.dev_id = attr->devid;
        chan_memset.dst = memset_para->dst + add_count;
        chan_memset.N = (size_t)min(byte_count - add_count, per_cnt) >> (unsigned)unit_shift;
        ret = devmm_chan_msg_send(&chan_memset, sizeof(struct devmm_chan_memset), 0);
        if (ret) {
            devmm_drv_err("dev memset err. ret=%d, dst=0x%llx, value=0x%llx, N=%lu\n", ret, memset_para->dst,
                          memset_para->value, memset_para->N);
            return -EINVAL;
        }
    }

    return 0;
}

STATIC int devmm_ioctl_memset_process(struct devmm_svm_process *svm_pro,
    struct devmm_ioctl_arg *arg, int unit_shift)
{
    struct devmm_mem_memset_para *memset_para = NULL;
    struct devmm_memory_attributes attr;
    size_t byte_count;

    memset_para = &arg->data.memset_para;
    devmm_drv_switch("dst=0x%llx, value=0x%llx, N=%lu.", memset_para->dst, memset_para->value, memset_para->N);

    devmm_get_memory_attributes(svm_pro, memset_para->dst, &attr);

    byte_count = memset_para->N << (unsigned)unit_shift;

    if ((attr.is_svm_non_page && !attr.is_locked_device) || attr.is_svm_host) {
        return devmm_memset_fill_pages_process(svm_pro, memset_para, byte_count, &attr);
    } else if ((attr.is_svm_non_page && attr.is_locked_device) || attr.is_svm_device) {
        return devmm_ioctl_memset_device_process(svm_pro, memset_para, &attr, byte_count, unit_shift);
    } else {
        devmm_drv_err("is_local_host=%d, is_host_pin=%d, is_svm=%d, devid=%u, dst=0x%llx, value=0x%llx, N=%lu\n",
                      attr.is_local_host, attr.is_host_pin, attr.is_svm, attr.devid, memset_para->dst,
                      memset_para->value, memset_para->N);
        return -EINVAL;
    }
}

STATIC int devmm_ioctl_memset8(struct devmm_svm_process *svm_pro, struct devmm_ioctl_arg *arg)
{
    return devmm_ioctl_memset_process(svm_pro, arg, DEVMM_CHAN_MEMSET8);
}

STATIC int devmm_ioctl_memset16(struct devmm_svm_process *svm_pro, struct devmm_ioctl_arg *arg)
{
    return devmm_ioctl_memset_process(svm_pro, arg, DEVMM_CHAN_MEMSET16);
}

STATIC int devmm_ioctl_memset32(struct devmm_svm_process *svm_pro, struct devmm_ioctl_arg *arg)
{
    return devmm_ioctl_memset_process(svm_pro, arg, DEVMM_CHAN_MEMSET32);
}

STATIC int devmm_ioctl_memset64(struct devmm_svm_process *svm_pro, struct devmm_ioctl_arg *arg)
{
    return devmm_ioctl_memset_process(svm_pro, arg, DEVMM_CHAN_MEMSET64);
}

STATIC int devmm_ioctl_translate(struct devmm_svm_process *svm_pro, struct devmm_ioctl_arg *arg)
{
    unsigned long vaddr = (unsigned long)arg->data.translate_para.vptr;
    struct devmm_chan_translate_ack tran_ack = {{0}};
    struct devmm_memory_attributes attr;
    int ret;

    devmm_get_memory_attributes(svm_pro, vaddr, &attr);

    if (attr.is_svm_non_page || !attr.is_svm_device) {
        devmm_drv_err("vaddr=0x%lx is svm non page or is not svm device(%d).\n", vaddr, attr.is_svm_device);
        return -EINVAL;
    }

    tran_ack.head.msg_id = DEVMM_CHAN_TRANSLATION_H2D_ID;
    tran_ack.head.dev_id = attr.devid;
    tran_ack.head.hostpid = devmm_get_current_pid();
    tran_ack.va = vaddr;

    down(&devmm_svm->share_memory_sem);
    ret = devmm_chan_msg_send(&tran_ack, sizeof(tran_ack), sizeof(tran_ack));
    if (ret) {
        devmm_drv_err("translate send msg fail. vaddr=0x%lx ret=%d devid=%d\n", vaddr, ret, tran_ack.head.dev_id);
        up(&devmm_svm->share_memory_sem);
        return ret;
    }
    up(&devmm_svm->share_memory_sem);

    arg->data.translate_para.pptr = tran_ack.pa;
    return 0;
}

STATIC int devmm_ioctl_smmu_query(struct devmm_svm_process *svm_pro, struct devmm_ioctl_arg *arg)
{
    struct devmm_mem_smmu_query_para *smmu_query_para = NULL;
    struct devmm_chan_smmu_query_ack chan_smmu_query;
    u32 devid = arg->head.devid;
    int ret;

    smmu_query_para = &arg->data.smmu_para;
    devmm_drv_switch("device=%u.\n", devid);

    if (devid >= DEVMM_MAX_DEVICE_NUM) {
        devmm_drv_err("dev_id(%d) invalid.\n", devid);
        return -EINVAL;
    }

    chan_smmu_query.head.hostpid = svm_pro->hostpid;
    chan_smmu_query.head.dev_id = devid;
    chan_smmu_query.head.msg_id = DEVMM_CHAN_SMMU_QUERY_H2D_ID;
    chan_smmu_query.ssid = 0;
    chan_smmu_query.ttbr = 0;
    chan_smmu_query.tcr = 0;

    ret = devmm_chan_msg_send(&chan_smmu_query, sizeof(struct devmm_chan_smmu_query_ack),
                              sizeof(struct devmm_chan_smmu_query_ack));
    if (ret) {
        devmm_drv_err("mmu query err. ret=%d\n", ret);
        return -EINVAL;
    }

    smmu_query_para->ssid = chan_smmu_query.ssid;
    svm_pro->deviceinfo[devid].device_ttbrinfo.ssid = chan_smmu_query.ssid;
    devmm_drv_switch("host ttbr=0x%llx, ssid=0x%x.tcr=0x%llx\n", chan_smmu_query.ttbr, chan_smmu_query.ssid,
                     chan_smmu_query.tcr);
    return 0;
}

STATIC int devmm_ioctl_query_l2buffsize(struct devmm_svm_process *svm_process, struct devmm_ioctl_arg *arg)
{
    struct devmm_mem_l2buff_para *l2buff_para = &arg->data.l2buff_para;
    struct devmm_chan_l2buff_ack chan_l2buff;
    u32 devid;
    int ret;

    if (arg->head.devid >= DEVMM_MAX_DEVICE_NUM) {
        devmm_drv_err("dev_id(%d) invalid.\n", arg->head.devid);
        return -EINVAL;
    }

    devid = arg->head.devid;
    if (devmm_svm->device_l2buffinfo[devid].l2size > 0) {
        l2buff_para->l2buffer_size = (size_t)devmm_svm->device_l2buffinfo[devid].l2size;
        return 0;
    }

    chan_l2buff.head.hostpid = svm_process->hostpid;
    chan_l2buff.head.dev_id = devid;
    chan_l2buff.head.msg_id = DEVMM_CHAN_QUERY_L2BUFF_SIZE_H2D_ID;

    ret = devmm_chan_msg_send(&chan_l2buff, sizeof(struct devmm_chan_l2buff_ack), sizeof(struct devmm_chan_l2buff_ack));
    if (ret) {
        devmm_drv_err("query l2buff err, ret=%d, device id=%u.\n", ret, devid);
        return -EINVAL;
    }

    l2buff_para->l2buffer_size = chan_l2buff.l2buffsize;
    devmm_svm->device_l2buffinfo[devid].l2buff = (phys_addr_t)chan_l2buff.l2buff;
    devmm_svm->device_l2buffinfo[devid].l2size = (size_t)chan_l2buff.l2buffsize;

    devmm_drv_switch("ok. devid=%u, l2buffsize=%lu \n", devid, chan_l2buff.l2buffsize);
    return 0;
}

STATIC int devmm_send_alloc_l2buff_msg(struct devmm_svm_process *svm_process, u32 devid, u64 l2_addr, u64 *pte)
{
    struct devmm_chan_l2buff_ack chan_l2buff;
    u64 i, msg_len;
    int ret;

    chan_l2buff.head.hostpid = svm_process->hostpid;
    chan_l2buff.head.dev_id = devid;
    chan_l2buff.head.msg_id = DEVMM_CHAN_L2BUFF_H2D_ID;
    chan_l2buff.l2buff = (unsigned long)l2_addr;
    msg_len = sizeof(struct devmm_chan_l2buff_ack);
    ret = devmm_chan_msg_send(&chan_l2buff, msg_len, msg_len);
    if (ret) {
        devmm_drv_err("l2buff query err ret=%d. va=0x%llx, device=%u\n", ret, l2_addr, devid);
        return -EINVAL;
    }

    for (i = 0; i < DEVMM_L2BUFF_PTE_NUM; i++) {
        pte[i] = chan_l2buff.pte[i];
    }
    return 0;
}

STATIC int devmm_ioctl_l2buff(struct devmm_svm_process *svm_process, struct devmm_ioctl_arg *arg)
{
    struct devmm_mem_l2buff_para *l2buff_para = &arg->data.l2buff_para;
    u64 i, j, page_cnt, l2size, l2_addr;
    struct devmm_svm_heap *heap = NULL;
    u32 *page_bitmap = NULL;
    u32 devid;
    int ret;

    devmm_drv_switch("enter va=0x%llx,device=%u.\n", l2buff_para->p, arg->head.devid);

    devid = arg->head.devid;
    l2_addr = l2buff_para->p;
    /* 1.get corresponding process stru and the 'first' bitmap. */
    heap = devmm_svm_get_heap(svm_process, (unsigned long)l2_addr);
    if (heap == NULL) {
        devmm_drv_err("can not find heap.va=0x%llx, device=%u.\n", l2_addr, devid);
        return -EINVAL;
    }

    /* 2. alloc pages with pin and set alloced and first page bit mask. */
    l2size = devmm_svm->device_l2buffinfo[devid].l2size;
    page_bitmap = devmm_get_page_bitmap_with_heapidx(svm_process, heap->heap_idx, l2_addr);
    if ((page_bitmap == NULL) || devmm_check_va_add_size_by_heap(heap, l2_addr, l2size)) {
        devmm_drv_err("can not find page_bitmap.va=0x%llx, device=%u, size=0x%llx.\n", l2_addr, devid, l2size);
        return -EINVAL;
    }
    page_cnt = devmm_get_pagecount_by_size(l2_addr, l2size, heap->chunk_page_size);
    for (i = 0; i < page_cnt; i++) {
        if (devmm_page_read_bitmap(page_bitmap + i) != 0) {
            devmm_drv_err("already allocd(%llu).va=0x%llx,size=%llu.\n", i, l2_addr, l2size);
            goto l2_fail_handle;
        }
        devmm_page_bitmap_set_flag(page_bitmap + i, DEVMM_PAGE_ALLOCED_MASK);
        devmm_page_bitmap_set_flag(page_bitmap + i, DEVMM_PAGE_DEV_MAPPED_MASK);
        devmm_page_bitmap_set_flag(page_bitmap + i, DEVMM_PAGE_LOCKED_DEVICE_MASK);
        devmm_page_bitmap_set_devid(page_bitmap + i, devid);
    }
    /* set first page bit mask. */
    devmm_page_bitmap_set_flag(page_bitmap, DEVMM_PAGE_IS_FIRST_PAGE_MASK);
    devmm_page_bitmap_set_order(page_bitmap, devmm_get_order_by_pagecount(page_cnt));

    ret = devmm_send_alloc_l2buff_msg(svm_process, devid, l2_addr, l2buff_para->pte);
    if (ret) {
        devmm_drv_err("l2buff query err ret=%d. va=0x%llx, device=%u\n", ret, l2_addr, devid);
        goto l2_fail_handle;
    }

    return 0;

l2_fail_handle:
    for (j = 0; j < i; j++) {
        devmm_page_clean_bitmap(page_bitmap + j);
    }

    return -EINVAL;
}

STATIC int devmm_ioctl_l2buff_destroy(struct devmm_svm_process *svm_process, struct devmm_ioctl_arg *arg)
{
    struct devmm_mem_l2buff_destroy_para *l2_buffer_destroy = &arg->data.l2buff_destroy_para;
    struct devmm_chan_l2buff_ack l2buff_destroy;
    struct devmm_svm_heap *heap = NULL;
    u64 order, i, page_cnt, l2_addr;
    u32 *page_bitmap = NULL;
    u32 temp_did;
    int ret;

    devmm_drv_switch("va=0x%llx, device=%u.\n", l2_buffer_destroy->p, arg->head.devid);
    /*
     * 1.get corresponding process stru and the 'first' bitmap.
     * if p is not align or 'first', fail.
     */
    l2_addr = l2_buffer_destroy->p;
    heap = devmm_svm_get_heap(svm_process, l2_addr);
    if (heap == NULL) {
        devmm_drv_err("can not find heap.va=0x%llx, dev=%u.\n", l2_addr, arg->head.devid);
        return -EINVAL;
    }

    if (l2_addr != round_down(l2_addr, heap->chunk_page_size)) {
        devmm_drv_err("not align va=0x%llx, page_size=%u.\n", l2_addr, heap->chunk_page_size);
        return -EINVAL;
    }

    page_bitmap = devmm_get_fst_alloc_bitmap_by_heapidx(svm_process, heap->heap_idx, l2_addr);
    if (page_bitmap == NULL) {
        devmm_drv_err("page_bitmap is null. device=%u.\n", arg->head.devid);
        return -EINVAL;
    }
    temp_did = devmm_page_bitmap_get_devid(page_bitmap);
    if (temp_did != arg->head.devid) {
        devmm_drv_err("va=0x%llx, devid(%u) not correspond. device=%u.\n", l2_addr, temp_did, arg->head.devid);
    }

    l2buff_destroy.head.hostpid = svm_process->hostpid;
    l2buff_destroy.head.dev_id = temp_did;
    l2buff_destroy.head.msg_id = DEVMM_CHAN_L2BUFF_DESTROY_H2D_ID;
    l2buff_destroy.l2buff = l2_addr;
    ret = devmm_chan_msg_send(&l2buff_destroy, sizeof(struct devmm_chan_l2buff_ack),
                              sizeof(struct devmm_chan_l2buff_ack));
    if (ret) {
        devmm_drv_err("l2buff destory err ret=%d. va=0x%llx, device=%u\n", ret, l2_addr, temp_did);
        return -EINVAL;
    }
    /* 2.get order by first bitmap,size=(1<<order). */
    order = devmm_page_bitmap_get_order(page_bitmap);
    page_cnt = 1ul << order;
    for (i = 0; i < page_cnt; i++) {
        devmm_page_clean_bitmap(page_bitmap + i);
    }
    devmm_drv_switch("suc. va=0x%llx, order=%llu, device=%u.\n", l2_addr, order, temp_did);
    return 0;
}

STATIC int devmm_device_alive(struct devmm_svm_process *svm_pro, u32 devid)
{
    if (devid >= DEVMM_MAX_DEVICE_NUM) {
        devmm_drv_err("dev_id(%d) invalid.\n", devid);
        return -1;
    }

    if (svm_pro->deviceinfo[devid].devpid > 0) {
        return 0;
    } else {
        return -1;
    }
}

STATIC void devmm_send_free_msg_to_p2p_device(struct devmm_svm_process *svm_pro,
    struct devmm_chan_free_pages *free_info, struct devmm_chan_free_pages free_pgs)
{
    unsigned int last_did = free_info->head.dev_id;
    unsigned int i;
    int ret;

    for (i = 0; i < DEVMM_MAX_DEVICE_NUM; i++) {
        if (devmm_device_alive(svm_pro, i) != 0) {
            continue;
        }
        if (devmm_dev_is_same_system(svm_pro->hostpid, free_info->head.dev_id, i) ||
            devmm_dev_is_same_system(svm_pro->hostpid, last_did, i)) {
            continue;
        }
        last_did = i;
        free_pgs.head.dev_id = i;

        ret = devmm_chan_msg_send(&free_pgs, sizeof(struct devmm_chan_free_pages), 0);
        if (ret) {
            devmm_drv_err("free share page err, devid=%d, "
                          "ret=%d, va=0x%lx, order=%u, realsize=%llu.\n",
                          free_info->head.dev_id, ret, free_info->va, free_info->order,
                          free_info->real_size);
            continue;
        }
    }
}

void devmm_chan_send_msg_free_pages(struct devmm_chan_free_pages *free_info, struct devmm_svm_heap *heap,
                                    struct devmm_svm_process *svm_pro, u32 free_self)
{
    struct devmm_chan_free_pages free_pgs = {{0}};
    int ret, shared_flag;
    unsigned long sect_addr;
    long long sect_offset;
    long long left_size;

    free_pgs.head.hostpid = free_info->head.hostpid;
    free_pgs.head.msg_id = free_info->head.msg_id;
    sect_offset = (long long)(1ul << DEVMM_DEFAULT_SECTION_ORDER) * (int)heap->chunk_page_size;

    devmm_drv_switch("enter,dev_id[%u],  va=0x%lx, order=%u, real_size=%llu\n", free_info->head.dev_id,
        free_info->va, free_info->order, free_info->real_size);

    sect_addr = free_info->va;
    left_size = free_info->real_size;
    shared_flag = free_info->shared_flag;
    while (left_size > 0) {
        free_pgs.va = sect_addr;
        free_pgs.order = DEVMM_DEFAULT_SECTION_ORDER;
        free_pgs.shared_flag = shared_flag;
        if (left_size <= sect_offset) {
            free_pgs.real_size = left_size;
        } else {
            free_pgs.real_size = sect_offset;
        }
        sect_addr += sect_offset;
        left_size -= sect_offset;

        if (shared_flag) {
            devmm_send_free_msg_to_p2p_device(svm_pro, free_info, free_pgs);
        }
        if (free_self == 0) {
            continue;
        }
        free_pgs.head.dev_id = free_info->head.dev_id;
        ret = devmm_chan_msg_send(&free_pgs, sizeof(struct devmm_chan_free_pages), 0);
        if (ret) {
            devmm_drv_err("free page err, devid=%d, "
                          "ret=%d, va=0x%lx, order=%u, realsize=%llu.\n",
                          free_info->head.dev_id, ret, free_info->va, free_info->order,
                          free_info->real_size);
            return;
        }
    }
}

void devmm_page_free_host(struct devmm_svm_process *svm_process, u32 *page_bitmap, unsigned long va, u32 adjust_order)
{
    if (devmm_page_bitmap_is_host_mapped(page_bitmap)) {
        (void)devmm_unmap_pages(svm_process, va, adjust_order);
    }
}

u32 devmm_get_adjust_order_by_heap(struct devmm_svm_heap *heap)
{
    u32 adjust_order = (u32)-1;

    if (heap->heap_type == DEVMM_HEAP_CHUNK_PAGE)
        adjust_order = 0;
    else if (heap->heap_type == DEVMM_HEAP_HUGE_PAGE)
        adjust_order = devmm_svm->host_page2device_hugepage_order;
    else
        devmm_drv_err("heap_type error, heap_type=0x%x\n", heap->heap_type);

    return adjust_order;
}

STATIC void devmm_set_page_free_info(struct devmm_svm_process *svm_pro, struct devmm_svm_heap *heap, unsigned long va,
                                     u32 *page_bitmap, struct devmm_chan_free_pages *free_info)
{
    DVdevice devid = DEVMM_MAX_DEVICE_NUM;
    u32 last_devshareed_index = 0;
    u32 last_devmapped_index = 0;
    int devmapped_flag = 0;
    u32 page_num, order, j;
    u32 shared_flag = 0;
    u32 adjust_order;

    free_info->real_size = 0;
    if (!devmm_page_bitmap_is_first_page(page_bitmap)) {
        return;
    }
    order = devmm_page_bitmap_get_order(page_bitmap);
    page_num = 1ul << order;
    devmm_svm_stat_vir_page_dec(page_num);
    devmm_drv_switch("destroy va=0x%lx, num=%d.\n", va, page_num);

    adjust_order = devmm_get_adjust_order_by_heap(heap);
    if (adjust_order == (u32)-1) {
        devmm_drv_err("get adjust order fail.\n");
        return;
    }

    for (j = 0; j < page_num; j++) {
        if (devmm_page_bitmap_is_isolation_page(page_bitmap + j)) {
            devmm_page_clean_bitmap(page_bitmap + j);
            continue;
        }

        devmm_page_free_host(svm_pro, (page_bitmap + j), va + (unsigned)j * (unsigned long)heap->chunk_page_size,
                             adjust_order);

        if (devmm_page_bitmap_is_dev_mapped(page_bitmap + j)) {
            if (devmapped_flag == 0) {
                devmapped_flag = DEVMM_TRUE;
                devid = devmm_page_bitmap_get_devid(page_bitmap + j);
            }

            last_devmapped_index = j;
        }

        if (devmm_page_bitmap_advise_memory_shared(page_bitmap + j)) {
            shared_flag = DEVMM_TRUE;
            last_devshareed_index = j;
        }
        devmm_page_clean_bitmap(page_bitmap + j);
    }
    /* mapped and shared flag to prevented set shared but umap pa */
    if (devmapped_flag || shared_flag) {
        free_info->va = round_down(va, heap->chunk_page_size);
        free_info->order = order;
        free_info->real_size = max((last_devshareed_index + 1), (last_devmapped_index + 1));
        free_info->real_size *= heap->chunk_page_size;
        free_info->shared_flag = shared_flag;
        free_info->head.dev_id = devid;
        free_info->head.hostpid = svm_pro->hostpid;
        free_info->head.msg_id = DEVMM_CHAN_FREE_PAGES_H2D_ID;
    }
    return;
}

int devmm_ioctl_free_pages(struct devmm_svm_process *svm_pro, struct devmm_ioctl_arg *arg)
{
    struct devmm_free_pages_para *free_pages_para = NULL;
    struct devmm_chan_free_pages free_info = {{0}};
    struct devmm_svm_heap *heap = NULL;
    u32 *page_bitmap = NULL;

    free_pages_para = &arg->data.free_pages_para;

    heap = devmm_svm_get_heap(svm_pro, free_pages_para->va);
    if (heap == NULL) {
        devmm_drv_err("check heap error.va=0x%lx.\n", free_pages_para->va);
        return -EINVAL;
    }

    if (free_pages_para->va != round_down(free_pages_para->va, heap->chunk_page_size)) {
        devmm_drv_err("not align va=0x%lx, page_size=%u.\n", free_pages_para->va, heap->chunk_page_size);
        return -EINVAL;
    }

    if (heap->heap_type != DEVMM_HEAP_HUGE_PAGE && heap->heap_type != DEVMM_HEAP_CHUNK_PAGE) {
        devmm_drv_err("invalid heap_type(%x), maybe use wrong api, va=0x%lx.\n",
                      heap->heap_type, free_pages_para->va);
        return -EINVAL;
    }

    page_bitmap = devmm_get_fst_alloc_bitmap_by_heapidx(svm_pro, heap->heap_idx, free_pages_para->va);
    if (page_bitmap == NULL) {
        devmm_drv_err("get bitmap error.va=0x%lx.\n", free_pages_para->va);
        return -EINVAL;
    }
    devmm_set_page_free_info(svm_pro, heap, free_pages_para->va, page_bitmap, &free_info);

    if (free_info.real_size) {
        devmm_chan_send_msg_free_pages(&free_info, heap, svm_pro,
                                       (free_info.head.dev_id < DEVMM_MAX_DEVICE_NUM));
    }

    return 0;
}

void devmm_destory_svm_heap_bitmap(struct devmm_svm_process *svm_pro, struct devmm_svm_heap *heap, int delete_self)
{
    struct devmm_chan_free_pages free_info = {{0}};
    u32 page_cnt, order, i;
    unsigned long heap_start_addr;
    u32 *page_bitmap = NULL;

    /* eg 8192=16G/2M,when adjust order is 9 */
    page_cnt = devmm_svm->heap_size / heap->chunk_page_size;
    /* eg 9=log(2m/4k): when device and host adjust page-size = 2m */
    devmm_drv_switch("page_cnt=%u\n", page_cnt);
    for (i = 0; i < page_cnt; i++) {
        page_bitmap = heap->page_bitmap + i;

        if (!devmm_page_bitmap_is_page_alloced(page_bitmap) || !devmm_page_bitmap_is_first_page(page_bitmap)) {
            continue;
        }

        heap_start_addr = heap->start + (unsigned long)heap->chunk_page_size * i;

        devmm_set_page_free_info(svm_pro, heap, heap_start_addr, page_bitmap, &free_info);

        if (free_info.real_size) {
            devmm_chan_send_msg_free_pages(&free_info, heap, svm_pro,
                                           (free_info.head.dev_id < DEVMM_MAX_DEVICE_NUM) && delete_self);
        }
        /* eg 3: 2M * 2^3 = 16M, when alloc-size = 12M */
        order = devmm_page_bitmap_get_order(page_bitmap);
        i += ((int)(1ul << order) - 1);
    }

    devmm_drv_switch("page_cnt=%u\n", page_cnt);
}

/*
 * Description:
 * handle of new heap/del heap of host pin memory
 * or svm memory
 * Params:
 * @data:
 * @arg: arg of user state
 * Return:
 * @ret:ok or fail
 */
STATIC int devmm_alloc_new_heap_pagebitmap(struct devmm_svm_process *svm_pro, struct devmm_svm_heap *heap)
{
    unsigned long page_cnt;

    page_cnt = devmm_svm->heap_size / heap->chunk_page_size;

    heap->page_bitmap = (u32 *)vzalloc(page_cnt * sizeof(u32));
    if (heap->page_bitmap == NULL) {
        devmm_drv_err("vmalloc page_bitmap fail. page_cnt=%lu\n", page_cnt);
        return -ENOMEM;
    }

    return 0;
}

STATIC void devmm_svm_bitmap_writelock(struct devmm_svm_process *svm_proc)
{
    if (svm_proc == NULL) {
        return;
    }
    down_write(&svm_proc->bitmap_sem);
}

STATIC void devmm_svm_bitmap_writeunlock(struct devmm_svm_process *svm_proc)
{
    if (svm_proc == NULL) {
        return;
    }
    up_write(&svm_proc->bitmap_sem);
}

STATIC void devmm_free_heap_pagebitmap(struct devmm_svm_process *svm_pro, struct devmm_svm_heap *heap)
{
    devmm_svm_bitmap_writelock(svm_pro);
    heap->heap_type = DEVMM_HEAP_IDLE;
    heap->chunk_page_size = 0;
    vfree(heap->page_bitmap);
    heap->page_bitmap = NULL;
    devmm_svm_bitmap_writeunlock(svm_pro);
}

STATIC void devmm_destory_all_heap_by_proc(struct devmm_svm_process *svm_pro)
{
    struct devmm_svm_heap *heap = NULL;
    u32 heap_idx;

    for (heap_idx = 0; heap_idx < DEVMM_MAX_HEAP_NUM; heap_idx++) {
        heap = &svm_pro->heaps[heap_idx];
        if (heap->heap_type == DEVMM_HEAP_IDLE) {
            continue;
        }
        if ((heap->heap_type == DEVMM_HEAP_HUGE_PAGE) || (heap->heap_type == DEVMM_HEAP_CHUNK_PAGE)) {
            /* just clear p2p shared memory */
            devmm_destory_svm_heap_bitmap(svm_pro, heap, DEVMM_FALSE);
        }
        /* host memory is freed by slef mem destory api */
        devmm_free_heap_pagebitmap(svm_pro, heap);
    }
}

STATIC void devmm_destroy_all_convert_dma_addr(struct devmm_svm_process *svm_pro)
{
    struct devmm_dma_node *dma_node = NULL;
    struct list_head *pos = NULL;
    struct list_head *n = NULL;

    devmm_drv_switch("process exited, begin release dma addr.\n");
    list_for_each_safe(pos, n, &svm_pro->dma_list)
    {
        dma_node = list_entry(pos, struct devmm_dma_node, dma_node_list);
        (void)devmm_destroy_dma_addr(dma_node->dma_addr);
        list_del(&dma_node->dma_node_list);
        kfree(dma_node);
        dma_node = NULL;
    }
    devmm_drv_switch("process exited, release dma addr end.\n");

    return;
}

STATIC void devmm_notify_deviceprocess(struct devmm_svm_process *svm_pro)
{
    int devid;

    for (devid = 0; devid < DEVMM_MAX_DEVICE_NUM; devid++) {
        if (svm_pro->deviceinfo[devid].devpid <= 0) {
            continue;
        }
        devmm_notify_device_close_process(svm_pro, devid);
    }
}

STATIC void devmm_destory_heap_mem(struct devmm_svm_process *svm_pro, struct devmm_svm_heap *heap)
{
    u64 page_cnt = devmm_svm->heap_size / heap->chunk_page_size;
    u32 *page_bitmap = heap->page_bitmap;
    u32 adjust_order;
    u64 i;

    if ((heap->heap_type == DEVMM_HEAP_HUGE_PAGE) || (heap->heap_type == DEVMM_HEAP_CHUNK_PAGE)) {
        adjust_order = devmm_get_adjust_order_by_heap(heap);
        for (i = 0; i < page_cnt; i++) {
            if (devmm_page_bitmap_is_page_alloced(page_bitmap + i) &&
                devmm_page_bitmap_is_host_mapped(page_bitmap + i)) {
                (void)devmm_unmap_pages(svm_pro,
                    heap->start + i * (unsigned long)heap->chunk_page_size, adjust_order);
                devmm_page_bitmap_clear_flag((page_bitmap + i), DEVMM_PAGE_HOST_MAPPED_MASK);
            }
        }
    } else if (heap->heap_type == DEVMM_HEAP_PINNED_HOST) {
        for (i = 0; i < page_cnt; i++) {
            if (devmm_page_bitmap_is_page_alloced(page_bitmap + i)) {
                (void)devmm_unmap(svm_pro,
                    heap->start + (unsigned long)heap->chunk_page_size * (unsigned)i);
            }
            devmm_page_clean_bitmap(page_bitmap + i);
        }
    }
}

STATIC void devmm_destory_all_self_mem_by_proc(struct devmm_svm_process *svm_pro)
{
    struct devmm_svm_heap *heap = NULL;
    u32 heap_idx;

    devmm_drv_info("destory self_mem hostpid(%d).\n", svm_pro->hostpid);
    for (heap_idx = 0; heap_idx < DEVMM_MAX_HEAP_NUM; heap_idx++) {
        heap = &svm_pro->heaps[heap_idx];

        if (heap->heap_type == DEVMM_HEAP_IDLE) {
            continue;
        }
        devmm_destory_heap_mem(svm_pro, heap);
    }
    devmm_drv_info("page statistics: alloc_page_cnt=%lld, free_page_cnt=%lld,"
                   "alloc_hugepage_cnt=%lld, free_huge_page_cnt=%lld.\n",
                   (long long)atomic64_read(&svm_pro->alloc_page_cnt), (long long)atomic64_read(&svm_pro->free_page_cnt),
                   (long long)atomic64_read(&svm_pro->alloc_hugepage_cnt), (long long)atomic64_read(&svm_pro->free_hugepage_cnt));
}

void devmm_notifier_release_private(struct devmm_svm_process *svm_pro)
{
    devmm_destory_all_self_mem_by_proc(svm_pro);
}

STATIC void devmm_update_heap_broadcast(struct devmm_svm_process *svm_pro,
                                        struct devmm_chan_update_heap *update_heap)
{
    int ret, i;

    update_heap->head.hostpid = svm_pro->hostpid;
    update_heap->head.msg_id = DEVMM_CHAN_UPDATE_HEAP_H2D_ID;
    for (i = 0; i < DEVMM_MAX_DEVICE_NUM; i++) {
        if (devmm_device_alive(svm_pro, i) != 0) {
            continue;
        }
        update_heap->head.dev_id = i;
        ret = devmm_chan_msg_send(update_heap, sizeof(struct devmm_chan_update_heap), 0);
        if (ret) {
            devmm_drv_err("update heap err, devid=%d, ret=%d\n", i, ret);
        }
    }
}

STATIC int devmm_ioctl_update_heap(struct devmm_svm_process *svm_pro, struct devmm_ioctl_arg *arg)
{
    struct devmm_update_heap_para *cmd = &arg->data.update_heap_para;
    struct devmm_chan_update_heap update_heap;
    struct devmm_svm_heap *heap = NULL;

    devmm_drv_switch("op=0x%x,type=0x%x,page_size=0x%x,idx=%u.\n", cmd->op, cmd->heap_type,
        cmd->page_size, cmd->heap_idx);
    if (devmm_check_input_heap_info(cmd->heap_idx, cmd->op, cmd->heap_type, cmd->page_size) == DEVMM_FALSE) {
        devmm_drv_err("input err. op=0x%x, heap_type=0x%x, page_size=0x%x, heap_idx=%u.\n",
                      cmd->op, cmd->heap_type, cmd->page_size, cmd->heap_idx);
        return -EINVAL;
    }
    heap = &svm_pro->heaps[cmd->heap_idx];
    update_heap.op = cmd->op;
    update_heap.heap_idx = cmd->heap_idx;
    if (cmd->op == DEVMM_HEAP_ENABLE) {
        if (heap->heap_type != DEVMM_HEAP_IDLE) {
            devmm_drv_err("enable heap is enabled.op=0x%x,heap_type=0x%x,page_size=0x%x,heap_idx=%u.\n",
                          cmd->op, cmd->heap_type, cmd->page_size, cmd->heap_idx);
            return -EINVAL;
        }
        heap->heap_type = cmd->heap_type;
        heap->chunk_page_size = cmd->page_size;
        if (devmm_alloc_new_heap_pagebitmap(svm_pro, heap) != 0) {
            heap->heap_type = DEVMM_HEAP_IDLE;
            return -ENOMEM;
        }
        update_heap.heap_type = heap->heap_type;
        update_heap.page_size = heap->chunk_page_size;
        devmm_update_heap_broadcast(svm_pro, &update_heap);
    } else {
        if (heap->heap_type == DEVMM_HEAP_IDLE) {
            devmm_drv_err("disable heap is disabled.op=0x%x,heap_type=0x%x,page_size=0x%x,heap_idx=%u.\n",
                          cmd->op, cmd->heap_type, cmd->page_size, cmd->heap_idx);
            return -EINVAL;
        }
        if ((heap->heap_type == DEVMM_HEAP_HUGE_PAGE) || (heap->heap_type == DEVMM_HEAP_CHUNK_PAGE)) {
            devmm_destory_svm_heap_bitmap(svm_pro, heap, DEVMM_TRUE);
        } else if (heap->heap_type == DEVMM_HEAP_PINNED_HOST) {
            devmm_destory_heap_mem(svm_pro, heap);
        }
        update_heap.heap_type = DEVMM_HEAP_IDLE;
        devmm_update_heap_broadcast(svm_pro, &update_heap);
        devmm_free_heap_pagebitmap(svm_pro, heap);
    }

    return 0;
}

STATIC int devmm_ioctl_init_process(struct devmm_svm_process *svm_process, struct devmm_ioctl_arg *arg)
{
    int err;

    if (devmm_get_current_pid() != svm_process->hostpid) {
        devmm_drv_warn("svm_process is already inited. svm pid=%d\n", svm_process->hostpid);
        return 0;
    }

    arg->data.init_process_para.svm_page_size = devmm_svm->svm_page_size;
    arg->data.init_process_para.local_page_size = PAGE_SIZE;
    arg->data.init_process_para.huge_page_size = devmm_svm->device_hpage_size;

    devmm_drv_switch("svm_page_size=%u, local_page_size=%lu, huge_page_size=%u\n",
                     arg->data.init_process_para.svm_page_size, (unsigned long)PAGE_SIZE, devmm_svm->device_hpage_size);
    spin_lock(&devmm_svm->proc_lock);
    if (svm_process->inited != DEVMM_SVM_INITING_FLAG) {
        spin_unlock(&devmm_svm->proc_lock);
        devmm_drv_warn("process may exited!! please check. hostpid=%d, devpid=%d.\n", svm_process->hostpid,
                       svm_process->devpid);
        return -EINVAL;
    }
    svm_process->inited = DEVMM_SVM_INITED_FLAG;
    svm_process->notifier.ops = &devmm_process_mmu_notifier;
    spin_unlock(&devmm_svm->proc_lock);
    sema_init(&svm_process->fault_sem, 1);
    err = mmu_notifier_register(&svm_process->notifier, svm_process->mm);
    if (err) {
        devmm_drv_err("mmu_notifier_register fail err=%d mm=%pK.\n", err, svm_process->mm);
        return err;
    }
    devmm_drv_switch("register mmu release-notify success.\n");
    return 0;
}

int devmm_ioctl_get_va_status_info(struct devmm_svm_process *svm_process, struct devmm_ioctl_arg *arg)
{
    struct devmm_status_va_info_para *status_va_info_para = NULL;
    struct devmm_svm_heap *heap = NULL;
    u32 *page_bitmap = NULL;

    status_va_info_para = &arg->data.status_va_info_para;

    devmm_drv_switch("va=0x%llx.\n", status_va_info_para->va);
    heap = devmm_svm_get_heap(svm_process, (unsigned long)status_va_info_para->va);
    if (heap == NULL) {
        devmm_drv_err("heap is null, va=0x%llx.\n", status_va_info_para->va);
        return -EINVAL;
    }

    page_bitmap = devmm_get_page_bitmap_with_heapidx(svm_process, heap->heap_idx,
                                                     status_va_info_para->va);
    if (page_bitmap == NULL) {
        devmm_drv_err("can not find page bitmap by va=0x%llx.\n", status_va_info_para->va);
        return -EINVAL;
    }
    status_va_info_para->page_status = devmm_page_read_bitmap(page_bitmap);
    devmm_drv_switch("page_status %u, va=0x%llx.\n", status_va_info_para->page_status, status_va_info_para->va);
    if (heap->heap_type == DEVMM_HEAP_HUGE_PAGE) {
        devmm_page_bitmap_set_flag(&status_va_info_para->page_status, DEVMM_PAGE_ADVISE_HUGE_PAGE_MASK);
    }
    return 0;
}

int devmm_ioctl_query_device_mem_info(struct devmm_svm_process *svm_process, struct devmm_ioctl_arg *arg)
{
    struct devmm_query_device_mem_usedinfo *query_device_mem_usedinfo = NULL;
    struct devmm_chan_device_meminfo device_meminfo;
    int ret;

    query_device_mem_usedinfo = &arg->data.query_device_mem_usedinfo_para;

    devmm_drv_switch("enter device=%u.\n", arg->head.devid);

    device_meminfo.head.hostpid = svm_process->hostpid;
    device_meminfo.head.dev_id = arg->head.devid;
    device_meminfo.head.msg_id = DEVMM_CHAN_QUERY_MEMINFO_H2D_ID;

    ret = devmm_chan_msg_send(&device_meminfo, sizeof(struct devmm_chan_device_meminfo),
                              sizeof(struct devmm_chan_device_meminfo));
    if (ret) {
        devmm_drv_err("query mem usedinfo err device=%u, ret(%d).\n", device_meminfo.head.dev_id, ret);
        return ret;
    }

    query_device_mem_usedinfo->free_num = device_meminfo.free_num;
    query_device_mem_usedinfo->total_num = device_meminfo.total_num;
    query_device_mem_usedinfo->shared_num = device_meminfo.shared_num;
    query_device_mem_usedinfo->huge_free_num = device_meminfo.huge_free_num;
    query_device_mem_usedinfo->huge_total_num = device_meminfo.huge_total_num;
    query_device_mem_usedinfo->huge_rsvd_num = device_meminfo.huge_rsvd_num;

    devmm_drv_switch("exit device=%u, free=%u, total=%u, shared=%u.\n"
                     "huge free=%u, total=%u, rsvd=%u.\n",
                     arg->head.devid, device_meminfo.free_num, device_meminfo.total_num, device_meminfo.shared_num,
                     device_meminfo.huge_free_num, device_meminfo.huge_total_num, device_meminfo.huge_rsvd_num);
    return ret;
}

STATIC int devmm_lock_cmd_process(struct devmm_lock_cmd *lock_cmd, u32 *page_bitmap, u32 dev_id)
{
    u32 mapped_devid = devmm_page_bitmap_get_devid(page_bitmap);

    if (lock_cmd->lockType == DV_LOCK_HOST) {
        if (devmm_page_bitmap_is_dev_mapped(page_bitmap)) {
            devmm_drv_err("try to lock host, but page is mapped device(%u).\n", dev_id);
            return -EINVAL;
        }
        devmm_page_bitmap_set_flag(page_bitmap, DEVMM_PAGE_LOCKED_HOST_MASK);
    } else if (lock_cmd->lockType == DV_LOCK_DEVICE) {
        if (devmm_page_bitmap_is_host_mapped(page_bitmap)) {
            devmm_drv_err("try to lock device(%u), but page is mapped host.\n", dev_id);
            return -EINVAL;
        }
        if (devmm_page_bitmap_is_dev_mapped(page_bitmap) && (dev_id != mapped_devid)) {
            devmm_drv_err("try to lock device(%u), but page is mapped device(%u).\n", dev_id, mapped_devid);
            return -EINVAL;
        }

        devmm_page_bitmap_set_flag(page_bitmap, DEVMM_PAGE_LOCKED_DEVICE_MASK);
        devmm_page_bitmap_set_devid(page_bitmap, dev_id);
    } else if (lock_cmd->lockType == DV_UNLOCK) {
        devmm_page_bitmap_clear_flag(page_bitmap, DEVMM_PAGE_LOCKED_HOST_MASK);
        devmm_page_bitmap_clear_flag(page_bitmap, DEVMM_PAGE_LOCKED_DEVICE_MASK);
    } else {
        devmm_drv_err("invalid lockType=0x%x, device=%u.\n", lock_cmd->lockType, dev_id);
        return -EINVAL;
    }

    return 0;
}

int devmm_ioctl_lock_cmd_handle(struct devmm_svm_process *svm_process, struct devmm_ioctl_arg *arg)
{
    struct devmm_lock_cmd *lock_cmd = NULL;
    struct devmm_svm_heap *heap = NULL;
    u32 *tmp_page_bitmap = NULL;
    u32 *page_bitmap = NULL;
    int num, i, ret;
    u32 order;

    lock_cmd = &arg->data.lock_cmd_para;

    heap = devmm_svm_get_heap(svm_process, lock_cmd->devPtr);
    if (heap == NULL) {
        devmm_drv_err("cannot find heap, device=%u.\n", arg->head.devid);
        return -EINVAL;
    }
    page_bitmap = devmm_get_fst_alloc_bitmap_by_heapidx(svm_process, heap->heap_idx,
        (lock_cmd->devPtr - (DVdeviceptr)heap->chunk_page_size * DEVMM_ISOLATION_PAGE_NUM));
    if (page_bitmap == NULL) {
        devmm_drv_err("page_bitmap is null, device=%u.\n", arg->head.devid);
        return -EINVAL;
    }

    order = devmm_page_bitmap_get_order(page_bitmap);
    num = (int)(1ul << order);
    /* first page is isolation page */
    for (i = 0; i < num; i++) {
        tmp_page_bitmap = page_bitmap + i;
        if (!devmm_page_bitmap_is_page_alloced(tmp_page_bitmap)) {
            break;
        }
        ret = devmm_lock_cmd_process(lock_cmd, tmp_page_bitmap, arg->head.devid);
        if (ret) {
            devmm_drv_err("lock process failed, ret=%d, device=%u, i=%d.\n", ret, arg->head.devid, i);
            return ret;
        }
    }

    return 0;
}

STATIC int devmm_ioctl_program_load_handle(struct devmm_svm_process *svm_pro, struct devmm_ioctl_arg *arg)
{
    u32 devid = arg->head.devid;
    if (devid >= DEVMM_MAX_DEVICE_NUM) {
        devmm_drv_err("devid =%d is invalid \n", devid);
        return EINVAL;
    }
    // atomic_t initial value = 1
    if (atomic_dec_and_test(&devmm_svm->device_l2buff_load_program[devid])) {
        arg->data.program_load_cmd.is_loaded = 0;
    } else {
        atomic_inc(&devmm_svm->device_l2buff_load_program[devid]);
        arg->data.program_load_cmd.is_loaded = 1;
    }
    return 0;
}

int devmm_ioctl_set_l2cache_read_count(struct devmm_svm_process *svm_process, struct devmm_ioctl_arg *arg)
{
    struct devmm_chan_set_l2cache_rc set_l2cache_rc = {{0}};
    struct devmm_set_read_count *set_rc_para = &arg->data.set_read_count;
    u32 *page_bitmap = NULL;
    int ret;

    page_bitmap = devmm_get_page_bitmap(svm_process, set_rc_para->addr);
    if ((page_bitmap == NULL) ||
        devmm_check_status_va_info(svm_process, set_rc_para->addr, set_rc_para->size)) {
        devmm_drv_err("can not find bitmap cur pid (%d), va=0x%llx.\n",
            svm_process->hostpid, set_rc_para->addr);
        return -EINVAL;
    }

    if (!devmm_page_bitmap_is_page_alloced(page_bitmap) || !devmm_page_bitmap_is_dev_mapped(page_bitmap)) {
        devmm_drv_err("addr didnot alloced or mapped, va=0x%llx, page_bitmap=0x%x.\n",
                      set_rc_para->addr, devmm_page_read_bitmap(page_bitmap));

        return -EINVAL;
    }

    set_l2cache_rc.head.dev_id = devmm_page_bitmap_get_devid(page_bitmap);
    set_l2cache_rc.head.msg_id = DEVMM_CHAN_SET_L2CACHE_READ_COUNT_ID;
    set_l2cache_rc.head.hostpid = svm_process->hostpid;

    set_l2cache_rc.vptr = (unsigned long)set_rc_para->addr;
    set_l2cache_rc.size = set_rc_para->size;
    set_l2cache_rc.rc = set_rc_para->rc;

    ret = devmm_chan_msg_send(&set_l2cache_rc, sizeof(struct devmm_chan_set_l2cache_rc), 0);
    if (ret) {
        devmm_drv_err("l2 cache rc set err devid[%d] ret(%d).addr=0x%lx, size=%ld, rc=%d.\n",
            set_l2cache_rc.head.dev_id, ret, set_l2cache_rc.vptr, set_l2cache_rc.size, set_l2cache_rc.rc);
    }
    return ret;
}

int (*const devmm_ioctl_handlers[DEVMM_SVM_CMD_MAX_CMD])(struct devmm_svm_process *, struct devmm_ioctl_arg *) = {
    [DEVMM_SVM_ALLOC_HOST_CMD] = devmm_ioctl_alloc_host,
    [DEVMM_SVM_FREE_HOST_CMD] = devmm_ioctl_free_host,
    [DEVMM_SVM_ALLOC_CMD] = devmm_ioctl_alloc,
    [DEVMM_SVM_MEMCPY_CMD] = devmm_ioctl_memcpy_proc,
    [DEVMM_SVM_MEMCPY_H2D_CMD] = devmm_ioctl_memcpy_h2d,
    [DEVMM_SVM_MEMCPY_D2H_CMD] = devmm_ioctl_memcpy_d2h,
    [DEVMM_SVM_MEMCPY_PEER_CMD] = devmm_ioctl_memcpy_peer,
    [DEVMM_SVM_CONVERT_ADDR_CMD] = devmm_ioctl_convert_addr,
    [DEVMM_SVM_DESTROY_ADDR_CMD] = devmm_ioctl_destroy_addr,
    [DEVMM_SVM_ADVISE_CMD] = devmm_ioctl_advise,
    [DEVMM_SVM_PREFETCH_CMD] = devmm_ioctl_prefetch,
    [DEVMM_SVM_MEMSET8_CMD] = devmm_ioctl_memset8,
    [DEVMM_SVM_MEMSET16_CMD] = devmm_ioctl_memset16,
    [DEVMM_SVM_MEMSET32_CMD] = devmm_ioctl_memset32,
    [DEVMM_SVM_MEMSET64_CMD] = devmm_ioctl_memset64,
    [DEVMM_SVM_TRANSLATE_CMD] = devmm_ioctl_translate,
    [DEVMM_SVM_SMMU_QUERY_CMD] = devmm_ioctl_smmu_query,
    [DEVMM_SVM_L2BUFF_CMD] = devmm_ioctl_l2buff,
    [DEVMM_SVM_L2BUFF_DESTROY_CMD] = devmm_ioctl_l2buff_destroy,
    [DEVMM_SVM_IPC_MEM_OPEN_CMD] = devmm_ioctl_ipc_mem_open,
    [DEVMM_SVM_IPC_MEM_CLOSE_CMD] = devmm_ioctl_ipc_mem_close,
    [DEVMM_SVM_SETUP_DEVICE_CMD] = devmm_ioctl_setup_device,
    [DEVMM_SVM_IPC_MEM_CREATE_CMD] = devmm_ioctl_ipc_mem_create,
    [DEVMM_SVM_IPC_MEM_SET_PID_CMD] = devmm_ioctl_ipc_mem_set_pid,
    [DEVMM_SVM_IPC_MEM_DESTROY_CMD] = devmm_ioctl_ipc_mem_destroy,
    [DEVMM_SVM_IPC_MEM_QUERY_CMD] = devmm_ioctl_ipc_mem_query,
    [DEVMM_SVM_QUERY_L2BUFFSIZE_CMD] = devmm_ioctl_query_l2buffsize,
    [DEVMM_SVM_INIT_PROCESS_CMD] = devmm_ioctl_init_process,
    [DEVMM_SVM_UPDATE_HEAP_CMD] = devmm_ioctl_update_heap,
    [DEVMM_SVM_FREE_PAGES_CMD] = devmm_ioctl_free_pages,
    [DEVMM_SVM_LOCK_CMD] = devmm_ioctl_lock_cmd_handle,
    [DEVMM_SVM_SET_READ_COUNT_CMD] = devmm_ioctl_set_l2cache_read_count,
    [DEVMM_SVM_QUERY_MEM_USEDINFO_CMD] = devmm_ioctl_query_device_mem_info,
    [DEVMM_SVM_DBG_VA_STATUS_CMD] = devmm_ioctl_get_va_status_info,
    [DEVMM_SVM_CMD_USE_PRIVATE_MAX_CMD] = NULL, /* above this svm process must inited */
    [DEVMM_SVM_PROGRAM_LOAD_CMD] = devmm_ioctl_program_load_handle,
};

/*
 * vm do fault: host process and device process
 */
STATIC int devmm_svm_vm_fault_host(struct vm_area_struct *vma, struct vm_fault *vmf)
{
    struct devmm_svm_process *svm_process = devmm_get_svm_pro(devmm_get_current_pid());
    u64 start = vma->vm_start + (vmf->pgoff << PAGE_SHIFT);
    struct devmm_svm_heap *heap = NULL;
    unsigned int adjust_order, dev_id;
    struct page **pages = NULL;
    u32 *page_bitmap = NULL;
    u64 page_num;
    int ret;

    devmm_svm_stat_fault_inc();
    devmm_drv_switch("host enter- vm fault, start=0x%llx\n", start);
    devmm_svm_ioctl_lock(svm_process);
    if (svm_process == NULL) {
        devmm_drv_err("can not find process by cur pid.\n");
        ret = DEVMM_FAULT_ERROR;
        goto devmm_svm_vm_fault_host_out;
    }

    heap = devmm_svm_get_heap(svm_process, start);
    if (heap == NULL) {
        devmm_drv_err("incorrect address(0x%llx).\n", start);
        ret = DEVMM_FAULT_ERROR;
        goto devmm_svm_vm_fault_host_out;
    }
    if (heap->heap_type == DEVMM_HEAP_CHUNK_PAGE) {
        adjust_order = 0;
    } else if (heap->heap_type == DEVMM_HEAP_HUGE_PAGE) {
        adjust_order = devmm_host_hugepage_fault_adjust_order();
    } else {
        devmm_drv_err("heap_type=0x%x error. start=0x%llx\n", heap->heap_type, start);
        ret = DEVMM_FAULT_ERROR;
        goto devmm_svm_vm_fault_host_out;
    }

    start = round_down(start, heap->chunk_page_size);
    page_bitmap = devmm_get_page_bitmap_with_heapidx(svm_process, heap->heap_idx, start);

    ret = (page_bitmap == NULL) || !devmm_page_bitmap_is_page_available(page_bitmap) ||
          devmm_page_bitmap_is_locked_device(page_bitmap);
    if (ret) {
        devmm_drv_err("page is not alloced or locked device "
                      "start=0x%llx, heap_idx=%u\n",
                      start, heap->heap_idx);
        devmm_print_pre_alloced_va(svm_process, start);
        ret = DEVMM_FAULT_ERROR;
        goto devmm_svm_vm_fault_host_out;
    }

    /* to get pointer type size, need use sizeof(char *). */
    page_num = 1ull << adjust_order;
    pages = kzalloc(sizeof(char *) * page_num, GFP_KERNEL);
    if (pages == NULL) {
        devmm_drv_err("kmalloc fail, adjust_order=%u, start=0x%llx\n", adjust_order, start);
        ret = DEVMM_FAULT_ERROR;
        goto devmm_svm_vm_fault_host_out;
    }

    ret = devmm_alloc_pages(NUMA_NO_NODE, 0, page_num, pages, svm_process);
    if (ret) {
        devmm_drv_err("devmm_alloc_pages err, ret=%d, start=0x%llx, adjust_order=%u.\n",
                      ret, start, adjust_order);
        ret = DEVMM_FAULT_ERROR;
        goto devmm_svm_vm_fault_host_free_page;
    }

    dev_id = devmm_svm_va_to_devid(start);
    if (dev_id < DEVMM_MAX_DEVICE_NUM) {
        ret = devmm_page_fault_h2d_sync(dev_id, pages, start, adjust_order, heap);
        if (ret) {
            devmm_drv_err("devmm_page_fault_h2d_sync err, ret=%d, dev_id=%u\n", ret, dev_id);
            ret = DEVMM_FAULT_ERROR;
            devmm_free_pages(page_num, pages, svm_process);
            goto devmm_svm_vm_fault_host_free_page;
        }

        devmm_svm_free_share_page_msg(svm_process, heap, start, heap->chunk_page_size, page_bitmap);
        devmm_svm_clear_mapped_with_heap(svm_process->hostpid, start, heap->chunk_page_size, dev_id, heap);
    }
    ret = devmm_insert_pages_to_vma(svm_process, start, adjust_order, pages);
    if (ret) {
        devmm_drv_err("insert pages vma err, ret=%d, start=0x%llx, adjust_order=%u\n",
                      ret, start, adjust_order);
        ret = DEVMM_FAULT_ERROR;
        devmm_free_pages(page_num, pages, svm_process);
        goto devmm_svm_vm_fault_host_free_page;
    }
    devmm_svm_set_mapped_with_heap(svm_process->hostpid, start, heap->chunk_page_size, DEVMM_MAX_DEVICE_NUM, heap);

    ret = DEVMM_FAULT_OK;

devmm_svm_vm_fault_host_free_page:
    kfree(pages);
    pages = NULL;

devmm_svm_vm_fault_host_out:
    devmm_drv_switch("exit- vm fault, ret=%d\n", ret);
/*
 * linux kernel < 3.11 need send SIGSEGV,
 * euler LINUX_VERSION_CODE 3.10 defined VM_FAULT_SIGSEGV
 */
#ifndef VM_FAULT_SIGSEGV
    if (ret != DEVMM_FAULT_OK) {
        force_sig(SIGSEGV, current);
    }
#endif
    devmm_svm_ioctl_unlock(svm_process);
    return ret;
}

#if LINUX_VERSION_CODE >= KERNEL_VERSION(4, 11, 0)
STATIC int devmm_svm_vmf_fault_host(struct vm_fault *vmf)
{
    return devmm_svm_vm_fault_host(vmf->vma, vmf);
}
#endif

struct devmm_svm_process *devmm_svm_mmap_init_struct(struct vm_operations_struct *ops_managed,
                                                     struct vm_area_struct *vma)
{
    struct devmm_svm_process *svm_proc = NULL;
    if (devmm_get_pro_by_pid(devmm_get_current_pid(), DEVMM_SVM_INITED_FLAG, DEVMM_END_HOST) != NULL) {
        devmm_drv_err("svm process remap err.\n");
        return NULL;
    }

#if LINUX_VERSION_CODE >= KERNEL_VERSION(4, 11, 0)
    ops_managed->fault = devmm_svm_vmf_fault_host;
#else
    ops_managed->fault = devmm_svm_vm_fault_host;
#endif
    svm_proc = devmm_get_idle_process_set_init(devmm_get_current_pid(), DEVMM_SVM_INVALID_PID,
                                               DEVMM_SVM_INITING_FLAG);
    return svm_proc;
}

void devmm_print_phy_meminfo(int nid)
{
    struct sysinfo chuck_info = {0};

    si_meminfo(&chuck_info);

    devmm_drv_info("phy mem(%d) chunk page num total=%ld,free=%ld,shared=%ld\r\n", nid, chuck_info.totalram,
                   chuck_info.freeram, chuck_info.sharedram);
}

pgprot_t devmm_make_pgprot(unsigned int flg)
{
    return PAGE_SHARED_EXEC;
}

int svm_get_pasid(pid_t vpid, int dev_id __maybe_unused)
{
    struct devmm_chan_smmu_query_ack chan_smmu_query;
    struct devmm_svm_process *svm_pro = NULL;
    int ret;

    svm_pro = devmm_get_svm_pro(vpid);
    if (svm_pro == NULL) {
        devmm_drv_err("can not find process by cur pid.\n");
        return -1;
    }

    if ((dev_id < 0) || (dev_id >= DEVMM_MAX_DEVICE_NUM)) {
        devmm_drv_err("dev_id(%d) invalid.\n", dev_id);
        return -1;
    }

    chan_smmu_query.head.hostpid = svm_pro->hostpid;
    chan_smmu_query.head.dev_id = dev_id;
    chan_smmu_query.head.msg_id = DEVMM_CHAN_SMMU_QUERY_H2D_ID;
    chan_smmu_query.ssid = 0;
    chan_smmu_query.ttbr = 0;
    chan_smmu_query.tcr = 0;

    ret = devmm_chan_msg_send(&chan_smmu_query, sizeof(struct devmm_chan_smmu_query_ack),
                              sizeof(struct devmm_chan_smmu_query_ack));
    if (ret) {
        devmm_drv_err("get pasid err.\n");
        return -1;
    }

    svm_pro->deviceinfo[dev_id].device_ttbrinfo.ssid = chan_smmu_query.ssid;

    devmm_drv_switch("host ttbr=0x%llx, ssid=0x%x.tcr=0x%llx\n", chan_smmu_query.ttbr, chan_smmu_query.ssid,
                    chan_smmu_query.tcr);
    return chan_smmu_query.ssid;
}
EXPORT_SYMBOL(svm_get_pasid);

void devmm_get_host_chip_num(struct devmm_svm_dev *dev)
{
    int phys[DEVMM_MAX_HOST_CHIP_NUM] = { -1 };
    int tmp_physid;
    int cpu_id = 0;
    int i;

    dev->hostchipnum = 0;
    for_each_present_cpu(cpu_id)
    {
        if (cpu_id >= NR_CPUS) {
            devmm_drv_err("cpu_id(%d) is overflow,"
                          " NR_CPUS(%d)\n",
                          cpu_id, NR_CPUS);
            continue;
        }

        tmp_physid = topology_physical_package_id(cpu_id);
        if (tmp_physid < 0) {
            devmm_drv_err("tmp_physid(%d) is overflow\n", tmp_physid);
            continue;
        }

        for (i = 0; (i < dev->hostchipnum) && (i < DEVMM_MAX_HOST_CHIP_NUM); i++) {
            if (tmp_physid == phys[i]) {
                break;
            }
        }

        if ((i == dev->hostchipnum) && (dev->hostchipnum < DEVMM_MAX_HOST_CHIP_NUM)) {
            phys[dev->hostchipnum] = tmp_physid;
            dev->hostchipnum++;
        }
    }

    if (dev->hostchipnum == 0) {
        devmm_drv_err("socket(cluster) num is 0\n");
        dev->hostchipnum = 1;
    }
    devmm_drv_info("socket(cluster) num is %d\n", dev->hostchipnum);
}

void devmm_lock_report_handle(void)
{
    down_read(&devmm_svm->report_abort.reprot_sem);
}

void devmm_unlock_report_handle(void)
{
    up_read(&devmm_svm->report_abort.reprot_sem);
}

void devmm_set_report_handle(devmm_report_handle_t p)
{
    down_write(&devmm_svm->report_abort.reprot_sem);
    devmm_svm->report_abort.report_proc_abort = p;
    up_write(&devmm_svm->report_abort.reprot_sem);
}
EXPORT_SYMBOL(devmm_set_report_handle);

devmm_report_handle_t devmm_get_report_handle(void)
{
    return devmm_svm->report_abort.report_proc_abort;
}
EXPORT_SYMBOL(devmm_get_report_handle);

void devmm_init_dev_private(struct devmm_svm_dev *dev, struct file_operations *svm_fops)
{
    devmm_get_host_chip_num(dev);
    init_rwsem(&dev->report_abort.reprot_sem);
    devmm_set_report_handle(NULL);
}

void devmm_release_private_proc(struct devmm_svm_process *svm_proc)
{
    devmm_notify_deviceprocess(svm_proc);
    devmm_destroy_all_convert_dma_addr(svm_proc);
    devmm_destroy_ipc_mem_node_by_proc(svm_proc);
    devmm_destory_all_heap_by_proc(svm_proc);
}

void devmm_uninit_dev_private(struct devmm_svm_dev *dev)
{
}

int devmm_get_another_nid(unsigned int nid)
{
    /* host numa id always 0 */
    return (int)NUMA_NO_NODE;
}

int devmm_get_alloc_mask(void)
{
    return (int)(GFP_KERNEL | __GFP_NORETRY | __GFP_ZERO | __GFP_NOWARN | __GFP_ACCOUNT);
}

int devmm_pa_is_p2p_addr(unsigned long pa)
{
    return 0;
}
