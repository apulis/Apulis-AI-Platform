/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2012-2019. All rights reserved.
 * Description: This file contains api definition for memory manager driver.
 * Author: huawei
 * Create: 2017-10-15
 */
#include <linux/dma-mapping.h>
#include <linux/types.h>
#include <linux/slab.h>
#include <linux/errno.h>
#include <linux/hugetlb.h>
#include <linux/mm.h>
#include <linux/delay.h>

#include "devmm_dev.h"
#include "devmm_proc_info.h"
#include "devmm_channel.h"
#include "devdrv_interface.h"
#include "devmm_msg_init.h"
#include "devmm_common.h"

/*
 * this function is use in  loop by idx. there're 3 steps:
 * (1)init pa_idx and merg_idx
 * (2)call this function
 * (3)restore merg idx
 */
void devmm_merg_pa(unsigned long *palist, u32 pa_idx, u32 pgsz, u32 *merg_szlist, u32 *merg_idx)
{
    unsigned long *merg_palist = palist;
    u32 i = pa_idx;
    u32 j = *merg_idx;

    if ((i >= 1) && (palist[i - 1] + pgsz == palist[i])) {
        merg_szlist[j - 1] += pgsz;
    } else {
        merg_palist[j] = palist[i];
        merg_szlist[j] = pgsz;
        j++;
    }
    *merg_idx = j;
}

void devmm_merg_pa_by_num(unsigned long *pas, u32 num, u32 pgsz, u32 *merg_szlist, u32 *merg_num)
{
    u32 i, j;

    for (i = 0, j = 0; i < num; i++) {
        devmm_merg_pa(pas, i, pgsz, merg_szlist, &j);
    }
    *merg_num = j;
}

STATIC void devmm_merg_blk(struct devmm_dma_block *blks, u32 idx, u32 *merg_idx)
{
    struct devmm_dma_block *merg_blks = blks;
    u32 j = *merg_idx;
    u32 i = idx;

    if ((i >= 1) && (blks[i - 1].pa + blks[i - 1].sz == blks[i].pa)) {
        merg_blks[j - 1].sz += blks[i].sz;
    } else {
        merg_blks[j].pa = blks[i].pa;
        merg_blks[j].sz = blks[i].sz;
        j++;
    }
    *merg_idx = j;
}

STATIC void devmm_merg_phy_blk(struct devmm_chan_phy_block *blks, u32 blks_idx, u32 *merg_idx)
{
    struct devmm_chan_phy_block *merg_blks = (struct devmm_chan_phy_block *)blks;
    u32 j = *merg_idx;
    u32 i = blks_idx;

    if ((i >= 1) && (blks[i - 1].pa + blks[i - 1].sz == blks[i].pa)) {
        merg_blks[j - 1].sz += blks[i].sz;
    } else {
        merg_blks[j].pa = blks[i].pa;
        merg_blks[j].sz = blks[i].sz;
        j++;
    }
    *merg_idx = j;
}

STATIC int devmm_chan_query_vaflgs_d2h_process(void *msg, u32 *ack_len)
{
    struct devmm_chan_page_query *flg_msg = (struct devmm_chan_page_query *)msg;
    u32 devid;
    int ret;

    devmm_drv_switch("host recv query_valfg msg host-pid(%d) ,va=0x%lx.\n", flg_msg->head.hostpid, flg_msg->va);

    *ack_len = 0;
    ret = devmm_svm_get_vaflgs_by_pid(flg_msg->head.hostpid, flg_msg->va, flg_msg->head.dev_id, &flg_msg->bitmap);
    if (ret) {
        devmm_drv_err("get_flags_by_pid fail, host-pid(%d), va=0x%lx, ret=%d", flg_msg->head.hostpid, flg_msg->va, ret);
        return ret;
    }

    if (devmm_page_bitmap_is_nosync_flg(&(flg_msg->bitmap)) || devmm_page_bitmap_is_share_num_flg(&(flg_msg->bitmap))) {
        goto chan_query_vaflgs_d2h_process_out;
    }

    if (devmm_page_bitmap_is_locked_device(&flg_msg->bitmap)) {
        devid = devmm_page_bitmap_get_devid(&flg_msg->bitmap);
        if (devid != flg_msg->head.dev_id) {
            devmm_drv_err("host-pid(%d) ,vptr(0x%lx) is locked device(%u), "
                          "but attempt to access it in device(%u)\n",
                          flg_msg->head.hostpid, flg_msg->va, devid, flg_msg->head.dev_id);
            return -EINVAL;
        }
    }

chan_query_vaflgs_d2h_process_out:

    *ack_len = sizeof(struct devmm_chan_page_query);
    return 0;
}

STATIC void devmm_fill_page_fault_msg(unsigned int dev_id, unsigned long va, u32 adjust_order, int msg_id,
    struct devmm_chan_page_fault *fault_msg)
{
    fault_msg->head.msg_id = DEVMM_CHAN_PAGE_FAULT_H2D_ID;
    fault_msg->head.dev_id = dev_id;
    fault_msg->head.hostpid = devmm_get_current_pid();
    fault_msg->va = va;
    fault_msg->num = (1ul << adjust_order);
}

/* h2d fault, inv device pagetable: (the max dma unit size is PAGESIZE ?)
 * 1. host host query host page (pin page and map dma)
 * 2. host send page-msg to device
 * 3. device recv and prs devic pagetable
 * 4. device query device page
 * 5. device copy to host
 * 6. device inv device page,return.
 * 7. host (unpin page and unmap dma) if non full size, return to 1.
 */
int devmm_page_fault_h2d_sync(unsigned int dev_id, struct page **pages, unsigned long va, u32 adjust_order,
                              const struct devmm_svm_heap *heap)
{
    struct devmm_chan_page_fault *fault_msg = NULL;
    struct devmm_chan_phy_block *blks = NULL;
    struct device *dev = NULL;
    u32 j, idx;
    int ret;

    devmm_drv_switch("enter dev_id=%u, va=0x%lx, adjust_order=%u\n", dev_id, va, adjust_order);

    fault_msg = (struct devmm_chan_page_fault *)kzalloc(sizeof(struct devmm_chan_page_fault), GFP_KERNEL);
    if (fault_msg == NULL) {
        devmm_drv_err("kmalloc error. dev_id=%u, va=0x%lx, adjust_order=%u\n", dev_id, va, adjust_order);
        return -ENOMEM;
    }
    blks = fault_msg->blks;
    devmm_fill_page_fault_msg(dev_id, va, adjust_order, DEVMM_CHAN_PAGE_FAULT_H2D_ID, fault_msg);

    dev = devmm_devid_to_device(dev_id);
    if (dev == NULL) {
        devmm_drv_err("dev is NULL,dev_id = %d\n", dev_id);
        kfree(fault_msg);
        return -EINVAL;
    }

    for (idx = 0, j = 0; idx < fault_msg->num; idx++) {
        blks[idx].pa = page_to_phys(pages[idx]);
        blks[idx].sz = PAGE_SIZE;
        devmm_merg_phy_blk(blks, idx, &j);
    }
    fault_msg->num = j;

    for (idx = 0; idx < fault_msg->num; idx++) {
        blks[idx].pa = dma_map_page(dev, pfn_to_page(PFN_DOWN(blks[idx].pa)), 0, blks[idx].sz, DMA_BIDIRECTIONAL);
        if (dma_mapping_error(dev, blks[idx].pa)) {
            devmm_drv_err("host page fault dma map page failed. dev_id=%u, va=0x%lx, adjust_order=%u\n",
                          dev_id, va, adjust_order);
            ret = -EIO;
            goto host_page_fault_dma_free;
        }
    }

    /* sync send msg:device todo copy data process */
    ret = devmm_chan_msg_send(fault_msg, sizeof(*fault_msg), 0);
    if (ret) {
        devmm_drv_err("host page fault send msg failed, ret=%d. dev_id=%u, va=0x%lx, adjust_order=%u\n",
                      ret, dev_id, va, adjust_order);
    }

host_page_fault_dma_free:
    for (j = 0; j < idx; j++) {
        dma_unmap_page(dev, blks[j].pa, blks[j].sz, DMA_BIDIRECTIONAL);
    }
    kfree(fault_msg);
    fault_msg = NULL;
    return ret;
}

/* d2h fault, inv host pagetable:
 * 1. device query device page
 * 2. device send page-msg to host
 * 3. host recv and prs devic pages
 * 4. host query host page (pin page and map dma)
 * 5. host copy to device
 * 6. host inv host page (unpin page and unmap dma), return.
 * 7. device if nonfull size return to 1.
 */
STATIC int devmm_chan_page_fault_d2h_process_dma_copy(struct devmm_chan_page_fault *fault_msg, unsigned long *pas,
                                                      u32 *szs, unsigned int num)
{
    struct devdrv_dma_node *dma_nodes = NULL;
    u32 off, max_num, i;
    int ret;

    dma_nodes = (struct devdrv_dma_node *)kzalloc(sizeof(struct devdrv_dma_node) * DEVMM_PAGE_NUM_PER_FAULT,
                                                  GFP_KERNEL);
    if (dma_nodes == NULL) {
        devmm_drv_err("kmalloc error,hostpid(%d), va=0x%lx, num=%d.\n", fault_msg->head.hostpid, fault_msg->va,
                      fault_msg->num);
        return -ENOMEM;
    }

    /* Create dma node list* */
    if (num > fault_msg->num) {
        for (i = 0, off = 0; i < num; i++) {
            dma_nodes[i].src_addr = pas[i];
            dma_nodes[i].dst_addr = fault_msg->blks[0].pa + off;
            dma_nodes[i].size = szs[i];
            dma_nodes[i].direction = DEVDRV_DMA_HOST_TO_DEVICE;
            off += dma_nodes[i].size;
        }
        max_num = num;
    } else {
        for (i = 0, off = 0; i < fault_msg->num; i++) {
            dma_nodes[i].src_addr = pas[0] + off;
            dma_nodes[i].dst_addr = fault_msg->blks[i].pa;
            dma_nodes[i].size = fault_msg->blks[i].sz;
            dma_nodes[i].direction = DEVDRV_DMA_HOST_TO_DEVICE;
            off += dma_nodes[i].size;
        }
        max_num = fault_msg->num;
    }

    ret = devmm_dma_sync_link_copy(fault_msg->head.dev_id, dma_nodes, max_num);
    if (ret) {
        devmm_drv_err("hostpid(%d), device va=0x%lx, num=%d, ret=%d.\n", fault_msg->head.hostpid, fault_msg->va,
                      fault_msg->num, ret);
    }

    kfree(dma_nodes);
    dma_nodes = NULL;

    return ret;
}

STATIC int devmm_chan_page_fault_d2h_process_copy(struct devmm_chan_page_fault *fault_msg,
                                                  struct devmm_svm_process *svm_process, struct devmm_svm_heap *heap)
{
    u32 num = DEVMM_PAGE_NUM_PER_FAULT;
    struct device *dev = NULL;
    unsigned long *pas = NULL;
    u32 *szs = NULL;
    u32 i, j, pa_len;
    int ret;

    pa_len = sizeof(unsigned long) * DEVMM_PAGE_NUM_PER_FAULT + sizeof(u32) * DEVMM_PAGE_NUM_PER_FAULT;
    pas = (unsigned long *)kzalloc(pa_len, GFP_KERNEL);
    if (pas == NULL) {
        devmm_drv_err("kzalloc pas failed, va=0x%lx, num=%d.\n", fault_msg->va, fault_msg->num);
        return -ENOMEM;
    }
    szs = (u32 *)(uintptr_t)((unsigned long)(uintptr_t)pas + (u32)(sizeof(unsigned long) * num));
    ret = devmm_va_to_palist(svm_process->vma, fault_msg->va, heap->chunk_page_size, pas, &num);
    if (ret) {
        devmm_drv_err("host-pid(%d),ret=%d num=%d. va=0x%lx\n", fault_msg->head.hostpid, ret, num, fault_msg->va);
        goto page_fault_d2h_copy_free_pas;
    }
    dev = devmm_devid_to_device(fault_msg->head.dev_id);
    if (dev == NULL) {
        devmm_drv_err("dev is NULL, dev_id(%d).\n", fault_msg->head.dev_id);
        ret = -EINVAL;
        goto page_fault_d2h_copy_free_pas;
    }

    devmm_merg_pa_by_num(pas, num, PAGE_SIZE, szs, &num);
    for (i = 0; i < num; i++) {
        pas[i] = dma_map_page(dev, pfn_to_page(PFN_DOWN(pas[i])), 0, szs[i], DMA_BIDIRECTIONAL);
        ret = dma_mapping_error(dev, pas[i]);
        if (ret) {
            devmm_drv_err("host proc dma map page fail. host-pid(%d), va=0x%lx, num=%d, ret=%d.\n",
                          fault_msg->head.hostpid, fault_msg->va, fault_msg->num, ret);
            goto page_fault_d2h_copy_dma_free;
        }
    }
    ret = devmm_chan_page_fault_d2h_process_dma_copy(fault_msg, pas, szs, num);
    if (ret) {
        devmm_drv_err("dma fail.host-pid(%d), va=0x%lx, num=%d, num fault=%d, ret=%d.\n", fault_msg->head.hostpid,
                      fault_msg->va, num, fault_msg->num, ret);
    }

page_fault_d2h_copy_dma_free:
    for (j = 0; j < i; j++) {
        dma_unmap_page(dev, pas[j], szs[j], DMA_BIDIRECTIONAL);
    }
page_fault_d2h_copy_free_pas:
    kfree(pas);
    pas = NULL;

    return ret;
}

STATIC int devmm_chan_page_fault_d2h_process(void *msg, u32 *ack_len)
{
    struct devmm_chan_page_fault *fault_msg = (struct devmm_chan_page_fault *)msg;
    struct devmm_svm_process *svm_process = NULL;
    struct devmm_svm_heap *heap = NULL;
    u32 adjust_order;
    int ret;

    devmm_drv_switch("enter host recv page_fault host-pid(%d), va=0x%lx, num=%d.\n", fault_msg->head.hostpid,
                     fault_msg->va, fault_msg->num);
    *ack_len = 0;
    ret = devmm_svm_get_svm_proc_and_heap(fault_msg->head.hostpid, fault_msg->va, &svm_process, &heap);
    if (ret) {
        devmm_drv_err("proc exit or heap error, hostpid(%d), va:0x%lx, num=%u.\n",
                      fault_msg->head.hostpid, fault_msg->va, fault_msg->num);
        return -EINVAL;
    }
    adjust_order = devmm_get_adjust_order_by_heap(heap);

    devmm_svm_set_mapped_with_heap(fault_msg->head.hostpid, fault_msg->va, heap->chunk_page_size,
                                   fault_msg->head.dev_id, heap);

    ret = devmm_chan_page_fault_d2h_process_copy(fault_msg, svm_process, heap);
    if (ret) {
        devmm_drv_err("host-pid(%d), va=0x%lx, num fault=%d, ret=%d, adjust_order=%u.\n", fault_msg->head.hostpid,
                      fault_msg->va, fault_msg->num, ret, adjust_order);
        return ret;
    }

    ret = devmm_unmap_pages(svm_process, fault_msg->va, adjust_order);
    if (ret) {
        devmm_drv_err("host-pid(%d), va=0x%lx, num fault=%d, ret=%d, adjust_order=%u.\n", fault_msg->head.hostpid,
                      fault_msg->va, fault_msg->num, ret, adjust_order);
        return ret;
    }

    devmm_svm_clear_mapped_with_heap(fault_msg->head.hostpid, fault_msg->va, heap->chunk_page_size,
                                     DEVMM_MAX_DEVICE_NUM, heap);
    devmm_drv_switch("exit host-pid(%d), va=0x%lx, num=%d, ret=%d.\n", fault_msg->head.hostpid, fault_msg->va,
                     fault_msg->num, ret);

    return 0;
}

/* * when device process abnormal exit, this will be called. */
STATIC int devmm_chan_proc_abort_d2h_process(void *msg, u32 *ack_len)
{
    struct devmm_chan_proc_abort *proc_abort_msg = (struct devmm_chan_proc_abort *)msg;
    struct devmm_svm_process *svm_pro = devmm_get_svm_pro(proc_abort_msg->head.hostpid);
    devmm_report_handle_t report;
    int ret, i;

    devmm_lock_report_handle();
    report = devmm_get_report_handle();
    *ack_len = 0;
    if (report == NULL) {
        devmm_unlock_report_handle();
        devmm_drv_info("report(dev%d,hpid%d,status%d) handle has not registered.\n", proc_abort_msg->head.dev_id,
                       proc_abort_msg->head.hostpid, proc_abort_msg->status);
        /* normal status devmm test return ok */
        return 0;
    }
    if (svm_pro == NULL) {
        devmm_unlock_report_handle();
        devmm_drv_info("svm_pro is null(dev%d,hpid%d,status%d).\n", proc_abort_msg->head.dev_id,
                       proc_abort_msg->head.hostpid, proc_abort_msg->status);
        /* normal status app exited return ok */
        return 0;
    }
    for (i = 0; i < DEVMM_MAX_DEVICE_NUM; i++) {
        if (svm_pro->deviceinfo[i].devpid <= 0) {
            continue;
        }
        ret = report(proc_abort_msg->head.hostpid, i, proc_abort_msg->status);
        devmm_drv_info("device%d report process exit event to devmng hpid=%d,status=%d,ret=%d.\n",
                       i, proc_abort_msg->head.hostpid, proc_abort_msg->status, ret);
    }
    devmm_unlock_report_handle();
    return 0;
}

int (*devmm_channel_msg_processes[DEVMM_CHAN_MAX_ID])(void *msg, u32 *ack_len) = {
    [DEVMM_CHAN_PAGE_FAULT_D2H_ID] = devmm_chan_page_fault_d2h_process,
    [DEVMM_CHAN_QUERY_VAFLGS_D2H_ID] = devmm_chan_query_vaflgs_d2h_process,
    [DEVMM_CHAN_PROC_ABORT_D2H_ID] = devmm_chan_proc_abort_d2h_process,
};

/* each msg_id process func corresponds to special struct type, it will check at devmm_chan_msg_dispatch */
unsigned int devmm_channel_msg_len[DEVMM_CHAN_MAX_ID] = {
    [DEVMM_CHAN_PAGE_FAULT_D2H_ID] = sizeof(struct devmm_chan_page_fault),
    [DEVMM_CHAN_QUERY_VAFLGS_D2H_ID] = sizeof(struct devmm_chan_page_query),
    [DEVMM_CHAN_PROC_ABORT_D2H_ID] = sizeof(struct devmm_chan_proc_abort),
};

int devmm_host_broadcast_pginfo(unsigned int did)
{
    struct devmm_chan_exchange_pginfo info = {{0}};
    int ret;

    devmm_svm_set_host_pgsf(PAGE_SHIFT);
    devmm_svm_set_host_hpgsf(HPAGE_SHIFT);

    info.head.msg_id = DEVMM_CHAN_EX_PGINFO_H2D_ID;
    info.host_page_shift = PAGE_SHIFT;
    info.host_hpage_shift = HPAGE_SHIFT;
    info.head.dev_id = did;

    ret = devmm_common_msg_send(&info, sizeof(info), sizeof(info));
    if (ret == 0) {
        devmm_svm_set_device_pgsf(info.device_page_shift);
        devmm_svm_set_device_hpgsf(info.device_hpage_shift);
        if (!devmm_svm_get_device_pgsf()) {
            devmm_drv_err("sync device page info timeout.\n");
            return -EINVAL;
        }
        devmm_chan_set_host_device_page_size();
    }
    return ret;
}

void devmm_notify_device_close_process(struct devmm_svm_process *svm_pro, u32 devid)
{
    struct devmm_chan_close_device chan_close = {{0}};
    int ret, devpid;
    int i = 0;

    devpid = svm_pro->deviceinfo[devid].devpid;
    chan_close.head.hostpid = svm_pro->hostpid;
    chan_close.head.msg_id = DEVMM_CHAN_CLOSE_DEVICE_H2D;
    chan_close.cmd = DEVMM_POLLING_CMD_DESTROY;
    do {
        chan_close.head.dev_id = devid;
        chan_close.devpid = devpid;
        ret = devmm_chan_msg_send(&chan_close, sizeof(struct devmm_chan_close_device),
                                  sizeof(struct devmm_chan_close_device));
        if (ret) {
            /* device thread may already quited */
            devmm_drv_info("device process closed, i=%d, ret=%d, device=%u, devpid=%d.\n", i, ret, devid, devpid);
            return;
        }
        if (chan_close.devpid != DEVMM_SVM_INVALID_PID) {
            msleep(DEVMM_QUERY_DEVICE_EXIT_DELAY);
        }
    } while ((chan_close.devpid != DEVMM_SVM_INVALID_PID) && (++i < DEVMM_QUERY_DEVICE_EXIT_MAXTIME));
    devmm_drv_switch("device process exited, i=%d, ret=%d, device=%u, devpid=%d.\n", i, ret, devid, devpid);
}

int devmm_query_page_by_msg(struct devmm_page_query_arg query_arg, unsigned int msg_id,
    struct devmm_dma_block *blks, u32 *num)
{
    unsigned long total_size, ack_msg_len, aligned_va, aligned_size;
    struct devmm_chan_page_query_ack *page_query = NULL;
    u32 total_num, merg_num, num_pre_msg, i;
    u32 *page_bitmap = NULL;
    int ret = 0;

    devmm_drv_switch("enter (0x%lx,0x%lx,%d,%d,%d).\n", query_arg.va, query_arg.size, msg_id, query_arg.dev_id, *num);

    num_pre_msg = *num;
    num_pre_msg = min(num_pre_msg, (u32)DEVMM_PAGE_NUM_PER_MSG);
    if (num_pre_msg == 0) {
        devmm_drv_err("invalid query num=%u.\n", num_pre_msg);
        return -EINVAL;
    }
    ack_msg_len = sizeof(struct devmm_chan_page_query_ack) +
        sizeof(struct devmm_chan_phy_block) * (unsigned long)num_pre_msg;
    page_query = (struct devmm_chan_page_query_ack *)kzalloc(ack_msg_len, GFP_KERNEL);
    if (page_query == NULL) {
        devmm_drv_err("page query kmalloc fail. (0x%lx,0x%lx,%d,%d,%d)\n",
                      query_arg.va, query_arg.size, msg_id, query_arg.dev_id, *num);
        return -ENOMEM;
    }

    /* query dst addr list* */
    total_num = 0;
    merg_num = 0;
    total_size = 0;
    page_bitmap = devmm_get_page_bitmap_by_va(query_arg.va);
    page_query->bitmap = page_bitmap ? devmm_page_read_bitmap(page_bitmap) : 0;
    page_query->head.hostpid = query_arg.hostpid;
    page_query->head.msg_id = msg_id;
    aligned_va = round_down(query_arg.va, query_arg.page_size);
    aligned_size = round_up((query_arg.size + query_arg.offset), query_arg.page_size);
    do {
        page_query->head.dev_id = query_arg.dev_id;
        page_query->va = aligned_va + total_size;
        page_query->size = aligned_size - total_size;
        page_query->num = min(num_pre_msg, (*num - total_num));
        ret = devmm_chan_msg_send(page_query, sizeof(struct devmm_chan_page_query_ack), ack_msg_len);
        if (ret) {
            devmm_drv_err("page query fail, ret=%d,ack_len=%lu, (0x%lx,%lu,%u,%u,%u,%u)\n",
                ret, ack_msg_len, page_query->va, page_query->size, msg_id, query_arg.dev_id, *num, total_num);
            goto query_page_bymsg_free;
        }

        /* check rest pages* */
        if ((page_query->num <= 0) || (page_query->num > num_pre_msg) || ((total_num + page_query->num) > *num)) {
            /* over max size */
            ret = -E2BIG;
            devmm_drv_err("page query pa num(%u) null or too long(%u). (0x%lx,%lu,%u,%u,%u,%u)\n",
                          page_query->num, num_pre_msg, page_query->va, page_query->size,
                          msg_id, query_arg.dev_id, *num, total_num);
            if (devmm_get_svm_pro(query_arg.hostpid) != NULL) {
                devmm_print_pre_alloced_va(devmm_get_svm_pro(query_arg.hostpid), query_arg.va);
            }
            goto query_page_bymsg_free;
        }

        for (i = 0; i < page_query->num; i++) {
            total_size += page_query->blks[i].sz;
            /* create do not need save pa to blks... */
            if (blks != NULL) {
                blks[total_num].pa = page_query->blks[i].pa;
                blks[total_num].sz = page_query->blks[i].sz;
                devmm_merg_blk(blks, total_num, &merg_num);
            }

            total_num++;
        }

        devmm_drv_switch("num=%d, total_size=%lu, total_num=%u, ack_msg_len=%lu, (0x%lx,%lu,%u,%u,%u)\n",
                         page_query->num, total_size, total_num, ack_msg_len, query_arg.va,
                         query_arg.size, msg_id, query_arg.dev_id, *num);
    } while (total_size < aligned_size);
    *num = (blks != NULL) ? merg_num : total_num;

query_page_bymsg_free:

    kfree(page_query);
    page_query = NULL;
    return ret;
}

void devmm_svm_free_share_page_msg(struct devmm_svm_process *svm_process, struct devmm_svm_heap *heap,
                                   unsigned long start, u64 real_size, u32 *page_bitmap)
{
    struct devmm_chan_free_pages free_info;
    int share_flag = 0;
    u64 tem_cnt, i;

    tem_cnt = devmm_get_pagecount_by_size(start, real_size, heap->chunk_page_size);
    for (i = 0; i < tem_cnt; i++) {
        if (devmm_page_bitmap_advise_memory_shared(page_bitmap + i)) {
            share_flag = 1;
            break;
        }
    }

    if (share_flag) {
        free_info.va = start;
        free_info.order = (u32)get_order(real_size);
        free_info.real_size = real_size;
        free_info.shared_flag = 1;
        free_info.head.hostpid = svm_process->hostpid;
        free_info.head.dev_id = devmm_page_bitmap_get_devid(page_bitmap);
        free_info.head.msg_id = DEVMM_CHAN_FREE_PAGES_H2D_ID;

        devmm_chan_send_msg_free_pages(&free_info, heap, svm_process, DEVMM_FALSE);

        for (i = 0; i < tem_cnt; i++) {
            devmm_page_bitmap_clear_flag(page_bitmap + i, DEVMM_PAGE_ADVISE_MEMORY_SHARED_MASK);
        }
    }
}

