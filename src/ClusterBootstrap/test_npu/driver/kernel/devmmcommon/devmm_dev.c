/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2012-2019. All rights reserved.
 * Description: This file contains api definition for memory manager driver.
 * Author: huawei
 * Create: 2017-10-15
 */

#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/errno.h>
#include <linux/fs.h>
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/memory.h>
#include <linux/module.h>
#include <linux/slab.h>
#include <linux/vmalloc.h>
#include <linux/mm.h>
#include <linux/version.h>
#include <linux/delay.h>
#include <linux/list.h>
#include <linux/pagemap.h>
#include <linux/kallsyms.h>
#include <linux/jiffies.h>
#include <linux/atomic.h>
#include <linux/nsproxy.h>
#include <linux/memcontrol.h>
#if LINUX_VERSION_CODE >= KERNEL_VERSION(4, 11, 0)
#include <linux/sched/mm.h>
#endif

#if LINUX_VERSION_CODE >= KERNEL_VERSION(4, 19, 0)
#include <linux/sched/task.h>
#endif


#include "devmm_adapt.h"
#include "devmm_dev.h"
#include "devmm_proc_info.h"
#include "devmm_channel.h"
#include "devdrv_interface.h"

typedef int (*container_virtual_to_physical_devid)(u32, u32 *);

container_virtual_to_physical_devid g_devmm_container_virtual_to_physical_devid = NULL;

STATIC enum devmm_endpoint_type devmm_get_end_type(void)
{
    return DEVMM_END_TYPE;
}

int devmm_va_is_not_svm_process_addr(const struct devmm_svm_process *svm_process, unsigned long va)
{
    if (svm_process == NULL) {
        return DEVMM_TRUE;
    }

    return ((va < svm_process->start_addr) || (va > svm_process->end_addr));
}

int devmm_va_to_pmd(const struct vm_area_struct *vma, unsigned long va, int huge_flag, pmd_t **tem_pmd)
{
    pgd_t *pgd = NULL;
#if LINUX_VERSION_CODE >= KERNEL_VERSION(4, 11, 0)
    p4d_t *p4d = NULL;
#endif
    pud_t *pud = NULL;
    pmd_t *pmd = NULL;

    if ((vma == NULL) || (vma->vm_mm == NULL)) {
        devmm_drv_err("vm_mm none(va=0x%lx,vma is NULL(%d))!\n", va, (vma == NULL));
        return -ESRCH;
    }
    /* too much log, not print */
    pgd = pgd_offset(vma->vm_mm, va);
    if (PXD_JUDGE(pgd)) {
        return -ENXIO;
    }

#if LINUX_VERSION_CODE >= KERNEL_VERSION(4, 11, 0)
    p4d = p4d_offset(pgd, va);
    if (PXD_JUDGE(p4d)) {
        return -EINVAL;
    }

    /* if kernel version is above 4.11.0,then 5 level pt arrived.
    pud_offset(pgd,va) changed to pud_offset(p4d,va) for x86
    but not changed in arm64 */
    pud = pud_offset(p4d, va);
    if (PXD_JUDGE(pud)) {
        return -ESPIPE;
    }
#else
    pud = pud_offset(pgd, va);
    if (PXD_JUDGE(pud)) {
        return -ESPIPE;
    }
#endif

    pmd = pmd_offset(pud, va);
    if (huge_flag) {
        /* huge page pmd can not judge bad flag */
        if ((pmd == NULL) || pmd_none(*(pmd_t *)pmd) || !pte_huge(*(pte_t *)pmd)) {
            return -EDOM;
        }
    } else {
        if (PXD_JUDGE(pmd)) {
            return -EDOM;
        }
    }
    *tem_pmd = pmd;
    return 0;
}

int devmm_va_to_pa(const struct vm_area_struct *vma, unsigned long va, unsigned long *pa)
{
    unsigned long aligned_va = round_down(va, PAGE_SIZE);
    pmd_t *pmd = NULL;
    pte_t *pte = NULL;
    int ret;

    ret = devmm_va_to_pmd(vma, va, 0, &pmd);
    if (ret) {
        /* too much log, not print */
        return ret;
    }
    pte = pte_offset_map(pmd, va);
    if (pte_none(*pte) || !pte_present(*pte)) {
        return -ERANGE;
    }

    *pa = PFN_PHYS(pte_pfn(*pte));
    *pa += (va - aligned_va);

    return 0;
}

/**
 * num input the size of the pa, output the real found page-block-num.
 */
int devmm_va_to_palist(const struct vm_area_struct *vma, unsigned long va, unsigned long sz, unsigned long *pa,
                       unsigned int *num)
{
    unsigned long vaddr, paddr;
    u32 pg_num = 0;
    int ret = 0;

    for (vaddr = round_down(va, PAGE_SIZE); vaddr < round_up(va + sz, PAGE_SIZE); vaddr += PAGE_SIZE) {
        if (devmm_va_to_pa(vma, vaddr, &paddr)) {
            /* too much log, not print */
            ret = -ENOENT;
            break;
        }
        if (pg_num >= *num) {
            /* va size more then array num */
            break;
        }
        pa[pg_num++] = paddr;
    }
    *num = pg_num;
    return ret;
}

void devmm_free_pages(u64 page_num, struct page **outpages, struct devmm_svm_process *svm_process)
{
    u64 i;

    for (i = 0; i < page_num; i++) {
        __free_pages(outpages[i], 0);
        outpages[i] = NULL;
        atomic64_inc(&svm_process->free_page_cnt);
        devmm_svm_stat_pg_free_inc();
    }
}

STATIC void devmm_enable_cgroup(struct mem_cgroup **memcg, pid_t pid)
{
#if LINUX_VERSION_CODE >= KERNEL_VERSION(4, 19, 0)
    struct task_struct *task = NULL;
    struct pid *pro_id = NULL;
    if (devmm_get_end_type() == DEVMM_END_DEVICE) {
        pro_id = find_get_pid(pid);
        if (pro_id == NULL) {
            return;
        }
        task = get_pid_task(pro_id, PIDTYPE_PID);
        if (task == NULL) {
            put_pid(pro_id);
            return;
        }
        put_pid(pro_id);
        *memcg = get_mem_cgroup_from_mm(task->mm);
        if (*memcg == NULL) {
            put_task_struct(task);
            return;
        }
        put_task_struct(task);
        memalloc_use_memcg(*memcg);
    }
#endif
}

STATIC void devmm_disable_cgroup(struct mem_cgroup *memcg)
{
#if LINUX_VERSION_CODE >= KERNEL_VERSION(4, 19, 0)
    if (devmm_get_end_type() == DEVMM_END_DEVICE) {
        if (memcg == NULL) {
            return;
        }
        memalloc_unuse_memcg();
        mem_cgroup_put(memcg);
    }
#endif
}

int devmm_alloc_pages(int nid, int is_4G, u64 page_num, struct page **outpages, struct devmm_svm_process *svm_process)
{
    struct mem_cgroup *memcg = NULL;
    pid_t pid;
    int flag;
    u64 i;

    flag = devmm_get_alloc_mask();
    if (is_4G) {
        flag |= GFP_DMA32;
    }

    if (devmm_get_end_type() == DEVMM_END_HOST) {
        pid = svm_process->hostpid;
    } else {
        pid = svm_process->devpid;
    }

    devmm_drv_switch("nid=%d, is_4G=%d, page_num=%llu\n", nid, is_4G, page_num);

    devmm_enable_cgroup(&memcg, pid);
    for (i = 0; i < page_num; i++) {
        outpages[i] = alloc_pages_node(nid, flag, 0);
        if (outpages[i] == NULL) {
            outpages[i] = alloc_pages_node(devmm_get_another_nid(nid), flag, 0);
            if (outpages[i] == NULL) {
                devmm_print_phy_meminfo(nid);
                devmm_free_pages(i, outpages, svm_process);
                devmm_disable_cgroup(memcg);
                devmm_drv_err("i=%llu, page_num=%llu, pid(%d) alloc_cnt=%lld, free_cnt=%lld.\n", i, page_num, pid,
                              (long long)atomic64_read(&svm_process->alloc_page_cnt), (long long)atomic64_read(&svm_process->free_page_cnt));
                return -ENOMEM;
            }
        }
        atomic64_inc(&svm_process->alloc_page_cnt);
        devmm_svm_stat_pg_alloc_inc();
    }
    devmm_disable_cgroup(memcg);
    return 0;
}

void devmm_zap_vma_ptes(struct vm_area_struct *vma, unsigned long vaddr, unsigned long size)
{
#if LINUX_VERSION_CODE < KERNEL_VERSION(4, 18, 0)
    int ret;

    ret = zap_vma_ptes(vma, vaddr, size);
    if (ret) {
        devmm_drv_err("zap_vma_ptes fail, va=0x%lx, ret=%d,vma=%pK,vm_flags=0x%lx.\n", vaddr, ret, vma, vma->vm_flags);
    }
#else
    zap_vma_ptes(vma, vaddr, size);
#endif
}

int devmm_umap_l2buff_pages(struct vm_area_struct *vma, unsigned long vaddr, unsigned long paddr)
{
    u32 dev_chip_num, j;

    if (devmm_get_end_type() == DEVMM_END_DEVICE) {
        dev_chip_num = atomic_read(&devmm_svm->device_info.devicechipnum);
        for (j = 0; j < dev_chip_num; j++) {
            if ((paddr >= devmm_svm->local_l2buffinfo[j].l2buff) &&
                (paddr < devmm_svm->local_l2buffinfo[j].l2buff + devmm_svm->local_l2buffinfo[j].l2size)) {
                devmm_zap_vma_ptes(vma, vaddr, PAGE_SIZE);
                devmm_svm_stat_l2_unmap_inc();
                return DEVMM_TRUE;
            }
        }
    }
    return DEVMM_FALSE;
}

int devmm_unmap_p2p_pages(struct vm_area_struct *vma, unsigned long vaddr, unsigned long pa)
{
    if (devmm_get_end_type() == DEVMM_END_DEVICE) {
        if (devmm_pa_is_p2p_addr(pa)) {
            devmm_zap_vma_ptes(vma, vaddr, PAGE_SIZE);
            devmm_svm_stat_p2p_unmap_inc();
            return DEVMM_TRUE;
        }
    }
    return DEVMM_FALSE;
}

int devmm_unpin_page_and_get_count(struct page *page)
{
    int ref;

    lock_page(page);
    ref = page_count(page);
    /* pagecount:alloc + 1, remap pin + 1 , always >= 2 */
    if (ref > 1) {
        put_page(page);
    } else {
        unlock_page(page);
        /*
         * when ref = 2,will go unpin branch,and caller will free this page,see devmm_unmap_phy_pages.
         * so ref=1 donot deal, return 0.
         */
        return 0;
    }
    ref = page_count(page);
    unlock_page(page);
    return ref;
}

void devmm_put_page(struct page *tem_page, struct devmm_svm_process *svm_process)
{
    int ref;

    ref = devmm_unpin_page_and_get_count(tem_page);
    /* page ref = 1, call free page return ram to os */
    if (ref == 1) {
        /* try best to set zero */
        (void)memset_s(phys_to_virt(page_to_phys(tem_page)), PAGE_SIZE, 0, PAGE_SIZE);
        devmm_free_pages(1, &tem_page, svm_process);
        devmm_svm_stat_page_dec(PAGE_SIZE);
        devmm_svm_stat_pg_unmap_inc();
    }
}

void devmm_unmap_phy_pages(struct vm_area_struct *vma, unsigned long vaddr, unsigned long paddr,
    struct devmm_svm_process *svm_process)
{
    struct page *tem_page = pfn_to_page(PFN_DOWN(paddr));

    devmm_zap_vma_ptes(vma, vaddr, PAGE_SIZE);
    devmm_put_page(tem_page, svm_process);
}

int devmm_unmap_page_range(struct devmm_svm_process *svm_process, unsigned long vaddr, u64 num)
{
    struct vm_area_struct *vma = svm_process->vma;
    unsigned long paddr;
    u64 i, temp_addr;
    int ret;

    for (i = 0; i < num; i++) {
        temp_addr = vaddr + (i << PAGE_SHIFT);
        ret = devmm_va_to_pa(vma, temp_addr, &paddr);
        if (ret) {
            continue;
        }
        if (devmm_umap_l2buff_pages(vma, temp_addr, paddr)) {
            continue;
        }
        if (devmm_unmap_p2p_pages(vma, temp_addr, paddr)) {
            continue;
        }

        devmm_unmap_phy_pages(vma, temp_addr, paddr, svm_process);
    }

    return 0;
}

int devmm_unmap_pages(struct devmm_svm_process *svm_process, unsigned long vaddr, u32 adjust_order)
{
    u64 num = 1ull << adjust_order;

    return devmm_unmap_page_range(svm_process, vaddr, num);
}

int devmm_insert_pages_range_to_vma(struct devmm_svm_process *svm_process, unsigned long va,
    u64 page_num, struct page **inpages)
{
    struct vm_area_struct *vma = svm_process->vma;
    phys_addr_t offset;
    int ret;
    u64 i;

    vma->vm_page_prot = devmm_make_pgprot(0);
    for (i = 0; i < page_num; i++) {
        offset = (i << PAGE_SHIFT);
        ret = remap_pfn_range(vma, va + offset, page_to_pfn(inpages[i]), PAGE_SIZE, vma->vm_page_prot);
        if (ret) {
            devmm_drv_err("vm_insert_page() failed,ret=%d. va=0x%lx, i=%llu, page_num=%llu.\n",
                ret, va, i, page_num);
            /* will not return fail ,so free page here */
            devmm_free_pages(1, &inpages[i], svm_process);
        } else {
            devmm_pin_page(inpages[i]);
        }
        devmm_svm_stat_page_inc(PAGE_SIZE);
        devmm_svm_stat_pg_map_inc();
    }

    return 0;
}

int devmm_insert_pages_to_vma(struct devmm_svm_process *svm_process, unsigned long va, u32 adjust_order,
                              struct page **inpages)
{
    u64 page_num = (1ull << adjust_order);

    return devmm_insert_pages_range_to_vma(svm_process, va, page_num, inpages);
}

int devmm_insert_virt_to_vma(struct vm_area_struct *vma, unsigned long va, u32 adjust_order,
                             unsigned long *paddr)
{
    pgprot_t vm_page_prot;
    phys_addr_t offset;
    u64 i, page_num;
    int ret = 0;

    vm_page_prot = __pgprot(pgprot_val(PAGE_SHARED_EXEC));
    page_num = 1ull << adjust_order;
    for (i = 0; i < page_num; i++) {
        offset = (i << PAGE_SHIFT);
        ret = remap_pfn_range(vma, va + offset, paddr[i] >> PAGE_SHIFT, PAGE_SIZE, vm_page_prot);
        if (ret) {
            devmm_drv_err("vm_insert_page() failed,ret=%d. va=0x%lx, adjust_order=%u,vma=%pK,vm_flags=0x%lx.\n",
                          ret, va, adjust_order, vma, vma->vm_flags);
        }

        devmm_svm_stat_p2p_map_inc();
    }

    return ret;
}

int devmm_insert_pages_by_num(struct page_map_info *page_map_info, struct devmm_svm_process *svm_process)
{
    int ret;

    ret = devmm_alloc_pages(page_map_info->nid, page_map_info->is_4G,
        page_map_info->page_num, page_map_info->inpages, svm_process);
    if (ret) {
        devmm_drv_err("alloc pages fail. page_num=%llu, ret=%d, nid=%d\n",
                      page_map_info->page_num, ret, page_map_info->nid);
        return ret;
    }

    ret = devmm_insert_pages_range_to_vma(svm_process,
        page_map_info->va, page_map_info->page_num, page_map_info->inpages);
    if (ret) {
        devmm_drv_err("devmm_insert_pages_to_vma fail.page_num=%llu, ret=%d\n",
                      page_map_info->page_num, ret);
        devmm_free_pages(page_map_info->page_num, page_map_info->inpages, svm_process);
        return ret;
    }

    return ret;
}

int devmm_insert_pages(struct page_map_info *page_map_info, struct devmm_svm_process *svm_process)
{
    page_map_info->page_num = (1ull << page_map_info->adjust_order);
    return devmm_insert_pages_by_num(page_map_info, svm_process);
}

int devmm_unmap(struct devmm_svm_process *svm_process, unsigned long vaddr)
{
    return devmm_unmap_pages(svm_process, vaddr, 0);
}

struct page *devmm_insert(struct page_map_info *page_map_info, struct devmm_svm_process *svm_process)
{
    struct page *pg = NULL;
    int ret;
    page_map_info->inpages = &pg;
    page_map_info->adjust_order = 0;

    ret = devmm_insert_pages(page_map_info, svm_process);

    return (ret ? NULL : pg);
}

int devmm_insert_host_range(struct devmm_svm_process *svm_process, unsigned long va, unsigned long sz, u32 adjust_order)
{
    unsigned long pg_sz = (PAGE_SIZE << adjust_order);
    struct page_map_info page_map_info = {0};
    unsigned long end_vaddr, vaddr;
    struct page **pages = NULL;
    int ret = 0;
    /* to get pointer type size, need use sizeof(char *). */
    pages = kzalloc(sizeof(char *) * (1ul << adjust_order), GFP_KERNEL);
    if (pages == NULL) {
        devmm_drv_err("insert_host_range kzalloc fail,order=%u,"
                      " va=0x%lx sz=0x%lx\n",
                      adjust_order, va, sz);
        return -ENOMEM;
    }
    end_vaddr = round_up(va + sz, pg_sz);

    page_map_info.nid = NUMA_NO_NODE;
    page_map_info.page_num = 0;
    page_map_info.adjust_order = adjust_order;
    page_map_info.inpages = pages;

    for (vaddr = round_down(va, pg_sz); vaddr < end_vaddr; vaddr += pg_sz) {
        page_map_info.va = vaddr;
        ret = devmm_insert_pages(&page_map_info, svm_process);
        if (ret) {
            devmm_drv_err("insert pages fail. vaddr=0x%lx, "
                          "sz=%lu, adjust_order=%u, ret=%d\n",
                          vaddr, sz, adjust_order, ret);
            goto devmm_insert_host_range_fail;
        }
    }

    kfree(pages);
    pages = NULL;
    return 0;

devmm_insert_host_range_fail:
    while (vaddr >= round_down(va, pg_sz) + pg_sz) {
        vaddr -= pg_sz;
        if (devmm_unmap_pages(svm_process, vaddr, adjust_order)) {
            devmm_drv_err("unmap_fail,va=0x%lx,o=%u.\n", vaddr, adjust_order);
        }
    }
    kfree(pages);
    pages = NULL;
    return ret;
}

struct devmm_svm_heap *devmm_svm_get_heap(struct devmm_svm_process *svm_process, unsigned long va)
{
    struct devmm_svm_heap *heap = NULL;
    u32 heap_idx;

    if (devmm_va_is_not_svm_process_addr(svm_process, va)) {
        devmm_drv_err("va=0x%lx overflow, start=0x%lx, end=0x%lx.\n", va, svm_process->start_addr,
                      svm_process->end_addr);
        return NULL;
    }

    heap_idx = (va - svm_process->start_addr) / devmm_svm->heap_size;
    heap = &svm_process->heaps[heap_idx];
    if (heap->heap_type == DEVMM_HEAP_IDLE) {
        devmm_drv_err("heap->heap_type is idle, va=0x%lx.\n", va);
        return NULL;
    }
    return heap;
}

int devmm_svm_get_svm_proc_and_heap(u32 pid, u64 va, struct devmm_svm_process **svm_process,
    struct devmm_svm_heap **heap)
{
    *svm_process = devmm_get_svm_pro(pid);
    if (*svm_process == NULL) {
        devmm_drv_err("process is exit, va=0x%llx, hpid=%d\n", va, pid);
        return -EINVAL;
    }

    *heap = devmm_svm_get_heap(*svm_process, va);
    if (*heap == NULL) {
        devmm_drv_err("va is err, va=0x%llx, hpid=%d\n", va, pid);
        return -EINVAL;
    }
    if (((*heap)->heap_type != DEVMM_HEAP_CHUNK_PAGE) && ((*heap)->heap_type != DEVMM_HEAP_HUGE_PAGE)) {
        devmm_drv_err("heap type error, va=0x%llx, hpid=%d, type=0x%x.\n", va, pid, (*heap)->heap_type);
        return -EINVAL;
    }

    return 0;
}

#define DEVMM_DMA_WAIT_MIN_TIME 100
#define DEVMM_DMA_WAIT_MAX_TIME 200
#define DEVMM_DMA_RETRY_CNT 1000

int devmm_dma_sync_link_copy(u32 dev_id, struct devdrv_dma_node *dma_node, u32 node_cnt)
{
    int ret;
    int retry_cnt = 0;
    enum devdrv_dma_data_type type;
    int wait_type;
    u32 size = DEVMM_DMA_QUERY_WAIT_PACKET_SIZE;

RETRY:

    /* If node_cnt is 1 and data size less then DEVMM_DMA_SMALL_PACKET_SIZE, */
    /* use PCIE_MSG channel to transfer data. use WAIT_QUREY type */
    if (size < DEVMM_DMA_SMALL_PACKET_SIZE) {
        type = DEVDRV_DMA_DATA_PCIE_MSG;
        wait_type = DEVDRV_DMA_WAIT_QUREY;
    } else if (size < DEVMM_DMA_QUERY_WAIT_PACKET_SIZE) {
        type = DEVDRV_DMA_DATA_TRAFFIC;
        wait_type = DEVDRV_DMA_WAIT_QUREY;
    } else {
        type = DEVDRV_DMA_DATA_TRAFFIC;
        wait_type = DEVDRV_DMA_WAIT_INTR;
    }

    ret = devdrv_dma_sync_link_copy(dev_id, type, wait_type, dma_node, node_cnt);

    /* dma queue is full, delay resubmit */
    if ((ret == -ENOSPC) && (retry_cnt < DEVMM_DMA_RETRY_CNT)) {
        usleep_range(DEVMM_DMA_WAIT_MIN_TIME, DEVMM_DMA_WAIT_MAX_TIME);
        retry_cnt++;
        goto RETRY;
    }

    if (ret) {
        devmm_drv_err("devdrv_dma_sync_link_copy fail. dev_id=%u, node_cnt=%u, ret=%d.\n",
                dev_id, node_cnt, ret);
    }
    return ret;
}

STATIC void devmm_svm_mmu_notifier_unreg(struct devmm_svm_process *svm_proc, int unmap_flag)
{
    /*
    * with user unmap, trace: do_munmap->devmm_notifier_start->devmm_svm_mmu_notifier_unreg
    * without user unmap, trace: exit->devmm_notifier_release->devmm_svm_mmu_notifier_unreg
    */
    spin_lock(&devmm_svm->proc_lock);
    if (unmap_flag) {
        /* after user unmap, current process cannot use svm any more */
        svm_proc->inited = DEVMM_SVM_UNMAP_FLAG;
    }
    svm_proc->tsk = NULL;
    if (svm_proc->notifier.ops != NULL) {
        svm_proc->notifier.ops = NULL;
        spin_unlock(&devmm_svm->proc_lock);
        mmu_notifier_unregister_no_release(&svm_proc->notifier, svm_proc->mm);
        devmm_notifier_release_private(svm_proc);
    } else {
        spin_unlock(&devmm_svm->proc_lock);
    }
}

#if LINUX_VERSION_CODE >= KERNEL_VERSION(4, 14, 4)
STATIC int devmm_svm_vm_split(struct vm_area_struct *vma, unsigned long va)
{
    devmm_drv_info("user improperly unmap, need to skip do_munmap.\n");
    return -1;
}
#endif

STATIC struct vm_operations_struct devmm_vm_ops_managed = {
#if LINUX_VERSION_CODE >= KERNEL_VERSION(4, 14, 4)
    .split = devmm_svm_vm_split,
#endif
};

STATIC void devmm_init_svm_process(struct devmm_svm_process *process, struct vm_area_struct *vma)
{
    u32 i;

    process->normal_exited = DEVMM_SVM_ABNORMAL_EXITED_FLAG;
    process->start_addr = DEVMM_SVM_MEM_START;
    process->end_addr = DEVMM_SVM_MEM_START + DEVMM_SVM_MEM_SIZE - 1;

    process->local_ttbrinfo.ttbr = 0;
    process->local_ttbrinfo.ssid = 0;
    process->local_ttbrinfo.tcr = 0;
    process->local_ttbrinfo.l2buffer = NULL;

    for (i = 0; i < DEVMM_MAX_DEVICE_NUM; i++) {
        process->deviceinfo[i].devpid = DEVMM_SETUP_INVAL_PID;
        process->deviceinfo[i].devid = DEVMM_MAX_DEVICE_NUM;
        process->deviceinfo[i].cluster_id = DEVMM_MAX_AGENTMM_CLUSTER_NUM;
        process->deviceinfo[i].device_ttbrinfo.ttbr = 0;
        process->deviceinfo[i].device_ttbrinfo.ssid = 0;
        process->deviceinfo[i].device_ttbrinfo.l2buffer = NULL;
    }

    for (i = 0; i < (int)DEVMM_MAX_HEAP_NUM; i++) {
        process->heaps[i].heap_idx = i;
        process->heaps[i].start = DEVMM_SVM_MEM_START + i * DEVMM_HEAP_SIZE;
        process->heaps[i].end = DEVMM_SVM_MEM_START + (i + 1) * DEVMM_HEAP_SIZE - 1;
        process->heaps[i].heap_type = DEVMM_HEAP_IDLE;
        process->heaps[i].page_bitmap = 0;
    }

    for (i = 0; i < DEVMM_SVM_MAX_AICORE_NUM; i++) {
        process->fault_err[i].fault_addr = DEVMM_INVALID_ADDR;
        process->fault_err[i].fault_cnt = 0;
    }
    process->err_print.cnt = 0;
    process->err_print.jiffies = jiffies;
    process->mm = current->mm;

    atomic64_set(&process->alloc_page_cnt, 0);
    atomic64_set(&process->free_page_cnt, 0);
    atomic64_set(&process->alloc_hugepage_cnt, 0);
    atomic64_set(&process->free_hugepage_cnt, 0);

    sema_init(&process->setup_dev.setup_sema, 0);
    init_rwsem(&process->bitmap_sem);
    sema_init(&process->ioctl_sem, 1);
    INIT_LIST_HEAD(&process->dma_list);

    for (i = 0; i < DEVMM_MAX_IPC_NODE_LIST_NUM; i++) {
        INIT_LIST_HEAD(&process->ipc_node.head[i]);
    }
    process->ipc_node.node_cnt = 0;
    mutex_init(&process->ipc_node.node_mutex);
}

STATIC int devmm_svm_mmap(struct file *file, struct vm_area_struct *vma)
{
    struct devmm_svm_process *svm_process = NULL;

    if ((vma->vm_start != DEVMM_SVM_MEM_START) || (vma->vm_end != (DEVMM_SVM_MEM_START + DEVMM_SVM_MEM_SIZE))) {
        devmm_drv_err("svm map err. vm_start=0x%lx,vm_end=0x%lx,vm_pgoff=0x%lx, vm_flags=0x%lx.\n", vma->vm_start,
                      vma->vm_end, vma->vm_pgoff, vma->vm_flags);
        return -ESRCH;
    }

    svm_process = devmm_svm_mmap_init_struct(&devmm_vm_ops_managed, vma);
    if (svm_process == NULL) {
        devmm_drv_err("svm map init_struct err.vm_start=0x%lx,vm_end=0x%lx,vm_pgoff=0x%lx, vm_flags=0x%lx.\n",
                      vma->vm_start, vma->vm_end, vma->vm_pgoff, vma->vm_flags);
        return -ESRCH;
    }
    /* init vma */
    svm_process->vma = vma;
    vma->vm_ops = &devmm_vm_ops_managed;
    vma->vm_flags |= VM_LOCKED;
    vma->vm_flags |= VM_DONTEXPAND;
    vma->vm_flags |= VM_PFNMAP;
    vma->vm_flags |= VM_WRITE;
    vma->vm_flags |= VM_DONTDUMP;

    file->private_data = kzalloc(sizeof(struct devmm_private_data), GFP_KERNEL);
    if (file->private_data == NULL) {
        devmm_drv_err("kzalloc devmm_private_data fail."
                      "vm_start=0x%lx,vm_end=0x%lx,vm_pgoff=0x%lx,vm_flags=0x%lx.\n",
                      vma->vm_start, vma->vm_end, vma->vm_pgoff, vma->vm_flags);
        return -ENOMEM;
    }

    ((struct devmm_private_data *)file->private_data)->process = svm_process;

    devmm_drv_switch("vm_start=0x%lx,vm_end=0x%lx,vm_pgoff=0x%lx,vm_flags=0x%lx\n", vma->vm_start, vma->vm_end,
                     vma->vm_pgoff, vma->vm_flags);

    devmm_init_svm_process(svm_process, vma);

    return 0;
}

STATIC int devmm_svm_release(struct inode *inode, struct file *filp)
{
    struct devmm_svm_process *svm_proc = NULL;
    if (filp->private_data == NULL) {
        return 0;
    }
    svm_proc = (struct devmm_svm_process *)(((struct devmm_private_data *)filp->private_data)->process);
    devmm_release_private_proc(svm_proc);
    devmm_release_svm_process(svm_proc);
    kfree(filp->private_data);
    filp->private_data = NULL;
    return 0;
}

STATIC int devmm_container_vir_to_phs_devid(u32 virtual_devid, u32 *physical_devid)
{
    if (g_devmm_container_virtual_to_physical_devid == NULL) {
        g_devmm_container_virtual_to_physical_devid = (container_virtual_to_physical_devid)(
            uintptr_t)kallsyms_lookup_name("devdrv_manager_container_logical_id_to_physical_id");
    }

    if (g_devmm_container_virtual_to_physical_devid != NULL) {
        return g_devmm_container_virtual_to_physical_devid(virtual_devid, physical_devid);
    } else {
        return -EFAULT;
    }
}

STATIC int devmm_convert_id_from_vir_to_phy(struct devmm_ioctl_arg *buffer)
{
    u32 *devid = NULL;
    int ret = -EINVAL;

    devid = &buffer->head.devid;
    if (*devid < DEVMM_MAX_DEVICE_NUM) {
        ret = devmm_container_vir_to_phs_devid(*devid, devid);
        if (ret) {
            devmm_drv_err("convert devid failed, ret = %d.\n", ret);
            return ret;
        }
    }

    return ret;
}

void devmm_svm_ioctl_lock(struct devmm_svm_process *svm_proc)
{
    if (devmm_get_end_type() == DEVMM_END_DEVICE) {
        return;
    }
    if (svm_proc == NULL) {
        return;
    }
    down(&svm_proc->ioctl_sem);
}

void devmm_svm_ioctl_unlock(struct devmm_svm_process *svm_proc)
{
    if (devmm_get_end_type() == DEVMM_END_DEVICE) {
        return;
    }
    if (svm_proc == NULL) {
        return;
    }
    up(&svm_proc->ioctl_sem);
}

STATIC int devmm_ioctl_get_svm_pro(struct file *file, u32 cmd, struct devmm_svm_process **svm_proc)
{
    if (_IOC_NR(cmd) < DEVMM_SVM_CMD_USE_PRIVATE_MAX_CMD) {
        if ((file->private_data == NULL) || (((struct devmm_private_data *)file->private_data)->process == NULL)) {
            devmm_drv_err("cmd(0x%x), svm not inited. private_data is null(%d), or svm proc is null.\n",
                cmd, (file->private_data == NULL));
            return -EINVAL;
        }
        *svm_proc = ((struct devmm_private_data *)file->private_data)->process;
        if (devmm_get_end_type() == DEVMM_END_HOST && _IOC_NR(cmd) != DEVMM_SVM_INIT_PROCESS_CMD &&
            (*svm_proc)->inited != DEVMM_SVM_INITED_FLAG) {
            devmm_drv_err("invalid svm_proc states(%u).\n", (*svm_proc)->inited);
            return -EINVAL;
        }
        return 0;
    }
    /* part of cmd do not need svm_pro */
    return 0;
}

extern int (*const devmm_ioctl_handlers[DEVMM_SVM_CMD_MAX_CMD])(struct devmm_svm_process *, struct devmm_ioctl_arg *);
STATIC int devmm_dispatch_ioctl(struct file *file, u32 cmd, void __user *arg)
{
    struct devmm_svm_process *svm_proc = NULL;
    struct devmm_ioctl_arg buffer = {{0}};
    int ret = 0;

    ret = (_IOC_TYPE(cmd) != DEVMM_SVM_MAGIC) || (_IOC_NR(cmd) >= DEVMM_SVM_CMD_MAX_CMD) ||
          (devmm_ioctl_handlers[_IOC_NR(cmd)] == NULL);
    if (ret) {
        devmm_drv_err("cmd(0x%x) not support\n", cmd);
        return -ENOTTY;
    }

    ret = devmm_ioctl_get_svm_pro(file, cmd, &svm_proc);
    if (ret) {
        devmm_drv_err("get svm_pro failed.\n");
        return ret;
    }

    if (arg == NULL) {
        devmm_drv_err("arg is NULL fail cmd(0x%x)\n", cmd);
        return -EINVAL;
    }

    if (_IOC_DIR(cmd) & _IOC_WRITE) {
        if (copy_from_user(&buffer, arg, sizeof(struct devmm_ioctl_arg))) {
            devmm_drv_err("copy_from_user fail cmd(0x%x)\n", cmd);
            return -EFAULT;
        }
        ret = devmm_convert_id_from_vir_to_phy(&buffer);
        if (ret < 0) {
            devmm_drv_err("convert virtual id to physical id failed, cmd(0x%x), ret(%d)\n", cmd, ret);
            goto clear_info;
        }
    }

    devmm_svm_ioctl_lock(svm_proc);
    ret = devmm_ioctl_handlers[_IOC_NR(cmd)](svm_proc, &buffer);
    devmm_svm_ioctl_unlock(svm_proc);
    if (ret < 0) {
        /* cmd fun will print detailed err */
        goto clear_info;
    }

    if (_IOC_DIR(cmd) & _IOC_READ) {
        if (copy_to_user(arg, &buffer, sizeof(struct devmm_ioctl_arg))) {
            devmm_drv_err("copy_to_user fail cmd(0x%x)\n", cmd);
            ret = -EINVAL;
        }
    }
clear_info:
    devmm_drv_err_if(memset_s(&buffer, sizeof(struct devmm_ioctl_arg), 0, sizeof(struct devmm_ioctl_arg)), "set err");
    return ret;
}

STATIC long devmm_svm_ioctl(struct file *file, u32 cmd, unsigned long arg)
{
    if (file == NULL) {
        devmm_drv_err("file is null, check svm init, cmd(0x%x)\n", cmd);
        return -EINVAL;
    }
    return devmm_dispatch_ioctl(file, cmd, (void __user *)(uintptr_t)arg);
}

STATIC struct file_operations devmm_svm_fops = {
    .owner = THIS_MODULE,
    .release = devmm_svm_release,
    .mmap = devmm_svm_mmap,
    .unlocked_ioctl = devmm_svm_ioctl,
};

void devmm_chan_set_host_device_page_size(void)
{
    devmm_svm->host_page_size = devmm_svm_pageshift2pagesize(devmm_svm->host_page_shift);
    devmm_svm->host_hpage_size = devmm_svm_pageshift2pagesize(devmm_svm->host_hpage_shift);

    devmm_svm->device_page_size = devmm_svm_pageshift2pagesize(devmm_svm->device_page_shift);
    devmm_svm->device_hpage_size = devmm_svm_pageshift2pagesize(devmm_svm->device_hpage_shift);

    devmm_svm->svm_page_size = max(devmm_svm->host_page_size, devmm_svm->device_page_size);
    devmm_svm->svm_page_shift = max(devmm_svm->host_page_shift, devmm_svm->device_page_shift);

    /* device huge page size is 2M ,host do not has huge page,just 4k/16k/64k */
    devmm_svm->host_page2device_hugepage_order = devmm_svm->device_hpage_shift - devmm_svm->host_page_shift;
    /* device page size is 4k ,host will be 4k/16k/64k */
    devmm_svm->host_page2device_page_order = devmm_svm->host_page_shift - devmm_svm->device_page_shift;

    devmm_drv_info("host_page_shift:%u,host_hpage_shift:%u,device_page_shift:%u,device_hpage_shift:%u,"
                   "h2dh_adjustorder=%u, h2d_adjustorder=%u.\n",
                   devmm_svm->host_page_shift, devmm_svm->host_hpage_shift, devmm_svm->device_page_shift,
                   devmm_svm->device_hpage_shift, devmm_svm->host_page2device_hugepage_order,
                   devmm_svm->host_page2device_page_order);

    devmm_drv_info("host_page_size:%u,host_hpage_size:%u,"
                   "device_page_size:%u, device_hpage_size:%u, svm_page_size:%u.\n",
                   devmm_svm->host_page_size, devmm_svm->host_hpage_size, devmm_svm->device_page_size,
                   devmm_svm->device_hpage_size, devmm_svm->svm_page_size);
}

STATIC void devmm_svm_bitmap_readlock(struct devmm_svm_process *svm_proc)
{
    if (devmm_get_end_type() == DEVMM_END_DEVICE) {
        return;
    }
    if (svm_proc == NULL) {
        return;
    }
    down_read(&svm_proc->bitmap_sem);
}

STATIC void devmm_svm_bitmap_readunlock(struct devmm_svm_process *svm_proc)
{
    if (devmm_get_end_type() == DEVMM_END_DEVICE) {
        return;
    }
    if (svm_proc == NULL) {
        return;
    }
    up_read(&svm_proc->bitmap_sem);
}

int devmm_chan_msg_dispatch(void *msg, u32 in_data_len, u32 out_data_len, u32 *ack_len)
{
    struct devmm_chan_page_query_ack *ack_msg = (struct devmm_chan_page_query_ack *)msg;
    u32 data_len = max(in_data_len, out_data_len);
    u32 host_id, msg_id, proc_len;
    int ret;

    msg_id = ((struct devmm_chan_msg_head *)msg)->msg_id;
    if ((msg_id >= DEVMM_CHAN_MAX_ID) || (devmm_channel_msg_processes[msg_id] == NULL)) {
        devmm_drv_err("invalid msg_id(%u) or none process func.\n", msg_id);
        return -EINVAL;
    }
    proc_len = devmm_channel_msg_len[msg_id];
    /* msg_id will process special struct : dynamic size variable */
    if (DEVMM_CHAN_PAGE_QUERY_H2D_ID <= msg_id && msg_id <= DEVMM_CHAN_PAGE_D2D_CREATE_H2D_ID) {
        if (ack_msg->num > DEVMM_PAGE_NUM_PER_MSG) {
            devmm_drv_err("invalid dynamic num(%u).\n", ack_msg->num);
            return -EINVAL;
        }
        proc_len += ack_msg->num * sizeof(struct devmm_chan_phy_block);
    }

    if (data_len < proc_len) {
        devmm_drv_err("invalid proc_len(%u). in_len(%u), out_len(%u).\n", proc_len, in_data_len, out_data_len);
        return -EINVAL;
    }

    host_id = ((struct devmm_chan_msg_head *)msg)->hostpid;
    devmm_svm_bitmap_readlock(devmm_get_svm_pro(host_id));
    ret = devmm_channel_msg_processes[msg_id](msg, ack_len);
    devmm_svm_bitmap_readunlock(devmm_get_svm_pro(host_id));
    return ret;
}

/*
 * devmm svm module init
 */
struct devmm_svm_dev *devmm_svm = NULL;

STATIC int devmm_init_devmm_struct(void)
{
    int pro_idx, i;

    devmm_svm = (struct devmm_svm_dev *)vzalloc(sizeof(struct devmm_svm_dev));
    if (devmm_svm == NULL) {
        devmm_drv_err("vzalloc fail.\n");
        return -ENOMEM;
    }

    devmm_svm->dev_no = 0;
    devmm_svm->host_page_shift = 0;
    devmm_svm->heap_size = DEVMM_HEAP_SIZE;
    devmm_svm->device_page_shift = 0;
    devmm_svm->order = 0;
    devmm_svm->svm_pro_cnt = 0;
    devmm_svm->svm_page_size = PAGE_SIZE;
    spin_lock_init(&devmm_svm->proc_lock);
    sema_init(&devmm_svm->share_memory_sem, 1);
    for (pro_idx = 0; pro_idx < DEVMM_SVM_MAX_PROCESS_NUM; pro_idx++) {
        devmm_svm->svm_pro[pro_idx].hostpid = DEVMM_SVM_INVALID_PID;
        devmm_svm->svm_pro[pro_idx].devpid = DEVMM_SVM_INVALID_PID;
        devmm_svm->svm_pro[pro_idx].doc_idx = DEVMM_SVM_INVALID_INDEX;
    }
    for (i = 0; i < DEVMM_MAX_DEVICE_NUM; i++) {
        atomic_set(&devmm_svm->device_l2buff_load_program[i], 1);
        atomic_set(&devmm_svm->setup_device_times[i], 0);
    }
    devmm_init_dev_private(devmm_svm, &devmm_svm_fops);
    return 0;
}

STATIC void devmm_unint_devmm_struct(void)
{
    vfree(devmm_svm);
    devmm_svm = NULL;
}

#if LINUX_VERSION_CODE >= KERNEL_VERSION(4, 19, 0)
STATIC int devmm_notifier_start(struct mmu_notifier *mn, struct mm_struct *mm, unsigned long start, unsigned long end,
    bool blockable)
{
    struct devmm_svm_process *svm_proc = container_of(mn, struct devmm_svm_process, notifier);

    if (start == DEVMM_SVM_MEM_START && end == (DEVMM_SVM_MEM_START + DEVMM_SVM_MEM_SIZE)) {
        if (blockable == 0) {
            /*
            * If blockable argument is set to false then the callback cannot
            * sleep and has to return with -EAGAIN. 0 should be returned
            * otherwise
            */
            return -EAGAIN;
        }
        devmm_drv_info("user unmap, need to release all resources.\n");
        devmm_svm_ioctl_lock(svm_proc);
        devmm_svm_mmu_notifier_unreg(svm_proc, 1);
        devmm_svm_ioctl_unlock(svm_proc);
    }
    return 0;
}
#endif

STATIC void devmm_notifier_release(struct mmu_notifier *mn, struct mm_struct *mm)
{
    struct devmm_svm_process *svm_proc = container_of(mn, struct devmm_svm_process, notifier);

    devmm_svm_mmu_notifier_unreg(svm_proc, 0);
}

struct mmu_notifier_ops devmm_process_mmu_notifier = {
#if LINUX_VERSION_CODE >= KERNEL_VERSION(4, 19, 0)
    .invalidate_range_start = devmm_notifier_start,
#endif
    .release = devmm_notifier_release,
};

void devmm_release_svm_process(struct devmm_svm_process *svm_process)
{
    spin_lock(&devmm_svm->proc_lock);
    svm_process->inited = DEVMM_SVM_UNINITED_FLAG;
    svm_process->hostpid = DEVMM_SVM_INVALID_PID;
    svm_process->devpid = DEVMM_SVM_INVALID_PID;
    svm_process->setup_dev.dev_daemon_inited = DEVMM_SVM_UNINITED_FLAG;
    svm_process->setup_dev.dev_setup_map = 0;
    svm_process->setup_dev.dev_setuped_map = 0;
    devmm_svm->svm_pro_cnt--;
    if (svm_process->doc_idx >= 0 && svm_process->doc_idx < DEVMM_MAX_DOC_NUM) {
        devmm_svm->svm_doc[svm_process->doc_idx].svm_doc_pro_cnt--;
        if (devmm_svm->svm_doc[svm_process->doc_idx].svm_doc_pro_cnt == 0) {
            devmm_svm->svm_doc[svm_process->doc_idx].svm_doc_flag = 0;
            devmm_svm->svm_doc[svm_process->doc_idx].doc_mnt_namespace = 0;
        }
    }
    svm_process->doc_idx = DEVMM_SVM_INVALID_INDEX;
    spin_unlock(&devmm_svm->proc_lock);
}

STATIC int devmm_get_pro_idx_by_pid(pid_t pid, u32 inited, enum devmm_endpoint_type endtype)
{
    int pro_idx;

    for (pro_idx = 0; pro_idx < DEVMM_SVM_MAX_PROCESS_NUM; pro_idx++) {
        if (devmm_svm->svm_pro[pro_idx].inited != inited) {
            continue;
        }
        if (endtype == DEVMM_END_HOST) {
            if (devmm_svm->svm_pro[pro_idx].hostpid == pid) {
                break;
            }
        } else if (endtype == DEVMM_END_DEVICE) {
            if (devmm_svm->svm_pro[pro_idx].devpid == pid) {
                break;
            }
        } else {
            break;
        }
    }
    return pro_idx;
}

struct devmm_svm_process *devmm_get_pro_by_pid(pid_t pid, u32 inited, enum devmm_endpoint_type endtype)
{
    int pro_idx;

    spin_lock(&devmm_svm->proc_lock);
    pro_idx = devmm_get_pro_idx_by_pid(pid, inited, endtype);
    spin_unlock(&devmm_svm->proc_lock);

    if (pro_idx == DEVMM_SVM_MAX_PROCESS_NUM) {
        devmm_drv_switch("can not find process by pid (%d).\n", pid);
        return NULL;
    }

    return &devmm_svm->svm_pro[pro_idx];
}

/*
 * Description:
 * get svm pro by hostpid
 *
 * Params:
 * @pid: the host pid
 * Return:
 * @svm_process:svm_pro
 */
struct devmm_svm_process *devmm_get_svm_pro(pid_t pid)
{
    return devmm_get_pro_by_pid(pid, DEVMM_SVM_INITED_FLAG, DEVMM_END_HOST);
}

struct devmm_svm_process *devmm_get_svm_pro_by_dev_vma(struct vm_area_struct *vma)
{
    int pro_idx;

    spin_lock(&devmm_svm->proc_lock);
    for (pro_idx = 0; pro_idx < DEVMM_SVM_MAX_PROCESS_NUM; pro_idx++) {
        if ((devmm_svm->svm_pro[pro_idx].inited == DEVMM_SVM_INITED_FLAG) ||
            (devmm_svm->svm_pro[pro_idx].inited == DEVMM_SVM_INITING_FLAG)) {
            if (devmm_svm->svm_pro[pro_idx].vma == vma) {
                break;
            }
        }
    }
    spin_unlock(&devmm_svm->proc_lock);

    if (pro_idx == DEVMM_SVM_MAX_PROCESS_NUM) {
        devmm_drv_switch("can not find process by vma (%pK).\n", vma);
        return NULL;
    }

    return &devmm_svm->svm_pro[pro_idx];
}

STATIC int devmm_get_doc_idx(int *idx)
{
    uintptr_t mnt_namespace = (uintptr_t)DEVMM_CENTOR_MNS_DATA;
    int doc_idx;

    spin_lock(&devmm_svm->proc_lock);
    if (current->nsproxy->mnt_ns != NULL) {
        mnt_namespace = (uintptr_t)current->nsproxy->mnt_ns;
    }

    for (doc_idx = 0; doc_idx < DEVMM_MAX_DOC_NUM; doc_idx++) {
        if (devmm_svm->svm_doc[doc_idx].doc_mnt_namespace == mnt_namespace) {
            if (devmm_svm->svm_doc[doc_idx].svm_doc_pro_cnt < DEVMM_MAX_DOC_PROCESS_NUM) {
                devmm_svm->svm_doc[doc_idx].svm_doc_pro_cnt++;
                *idx = doc_idx;
                spin_unlock(&devmm_svm->proc_lock);
                return 0;
            }
            *idx = DEVMM_SVM_INVALID_INDEX;
            spin_unlock(&devmm_svm->proc_lock);
            return -EIO;
        }
    }

    for (doc_idx = 0; doc_idx < DEVMM_MAX_DOC_NUM; doc_idx++) {
        if (devmm_svm->svm_doc[doc_idx].svm_doc_flag != DEVMM_SVM_DOC_VALID_FLAG) {
            devmm_svm->svm_doc[doc_idx].doc_mnt_namespace = mnt_namespace;
            devmm_svm->svm_doc[doc_idx].svm_doc_flag = DEVMM_SVM_DOC_VALID_FLAG;
            devmm_svm->svm_doc[doc_idx].svm_doc_pro_cnt++;
            *idx = doc_idx;
            spin_unlock(&devmm_svm->proc_lock);
            return 0;
        }
    }
    *idx = DEVMM_SVM_INVALID_INDEX;
    spin_unlock(&devmm_svm->proc_lock);
    return -ENXIO;
}

struct devmm_svm_process *devmm_get_idle_process_set_init(pid_t host_pid, pid_t dev_pid, u32 inited)
{
    int doc_idx = DEVMM_SVM_INVALID_INDEX;
    int pro_idx, i;
    int ret = 0;

    for (i = 0; i < DEVMM_SETUP_RETRY_NUM; i++) {
        ret = devmm_get_doc_idx(&doc_idx);
        if (ret == 0) {
            break;
        }
        if (devmm_get_end_type() == DEVMM_END_HOST) {
            msleep(DEVMM_GET_IDLE_TIMEOUT);
        }
    }
    if (ret < 0) {
        devmm_drv_err("can not find idle docker, pid(%d,%d).\n", host_pid, dev_pid);
        return NULL;
    }

    spin_lock(&devmm_svm->proc_lock);
    pro_idx = devmm_get_pro_idx_by_pid(DEVMM_SVM_INVALID_PID, DEVMM_SVM_UNINITED_FLAG, DEVMM_END_NUM);
    if (pro_idx == DEVMM_SVM_MAX_PROCESS_NUM) {
        spin_unlock(&devmm_svm->proc_lock);
        devmm_drv_err("can not find idle process,pid(%d,%d).\n", host_pid, dev_pid);
        return NULL;
    }

    devmm_svm->svm_pro[pro_idx].inited = inited;
    devmm_svm->svm_pro[pro_idx].hostpid = host_pid;
    devmm_svm->svm_pro[pro_idx].devpid = dev_pid;
    devmm_svm->svm_pro[pro_idx].doc_idx = doc_idx;
    devmm_svm->svm_pro_cnt++;
    spin_unlock(&devmm_svm->proc_lock);

    return &devmm_svm->svm_pro[pro_idx];
}

struct devmm_svm_process *devmm_get_pre_initing_pro(pid_t dev_pid, pid_t set_dev_pid, u32 inited, u32 initing)
{
    int pro_idx;

    spin_lock(&devmm_svm->proc_lock);
    pro_idx = devmm_get_pro_idx_by_pid(dev_pid, inited, DEVMM_END_DEVICE);
    if (pro_idx == DEVMM_SVM_MAX_PROCESS_NUM) {
        spin_unlock(&devmm_svm->proc_lock);
        devmm_drv_switch("can not find pre initing process.\n");
        return NULL;
    }
    devmm_svm->svm_pro[pro_idx].inited = initing;
    if (set_dev_pid != DEVMM_SVM_INVALID_PID) {
        devmm_svm->svm_pro[pro_idx].devpid = set_dev_pid;
        devmm_svm->svm_pro[pro_idx].tsk = current;
    }
    spin_unlock(&devmm_svm->proc_lock);
    return &devmm_svm->svm_pro[pro_idx];
}

STATIC int devmm_process_pid_check(enum devmm_endpoint_type endtype, int pro_idx, pid_t pid)
{
    if (endtype == DEVMM_END_HOST) {
        if (devmm_svm->svm_pro[pro_idx].hostpid == pid) {
            return DEVMM_TRUE;
        }
    } else if (endtype == DEVMM_END_DEVICE) {
        if (devmm_svm->svm_pro[pro_idx].devpid == pid) {
            return DEVMM_TRUE;
        }
    }
    return DEVMM_FALSE;
}

struct devmm_svm_process *devmm_get_initing_pro_by_pid(pid_t pid, enum devmm_endpoint_type endtype)
{
    int pro_idx;
    int ret;

    spin_lock(&devmm_svm->proc_lock);
    for (pro_idx = 0; pro_idx < DEVMM_SVM_MAX_PROCESS_NUM; pro_idx++) {
        ret = (devmm_svm->svm_pro[pro_idx].inited == DEVMM_SVM_INITING_FLAG) ||
              (devmm_svm->svm_pro[pro_idx].inited == DEVMM_SVM_PRE_INITING_FLAG) ||
              (devmm_svm->svm_pro[pro_idx].inited == DEVMM_SVM_POLLING_FLAG) ||
              (devmm_svm->svm_pro[pro_idx].inited == DEVMM_SVM_POLLED_FLAG);
        if (ret) {
            if (devmm_process_pid_check(endtype, pro_idx, pid) == DEVMM_TRUE) {
                break;
            }
        }
    }
    spin_unlock(&devmm_svm->proc_lock);
    if (pro_idx == DEVMM_SVM_MAX_PROCESS_NUM) {
        devmm_drv_switch("can not find pre initing process[%d].\n", pid);
        return NULL;
    }
    return &devmm_svm->svm_pro[pro_idx];
}

u32 devmm_get_proc_printf_flag(void)
{
    struct devmm_svm_process *svm_proc = NULL;
    unsigned long temp_jiffies;
    u32 flag = DEVMM_FALSE;

    if (devmm_get_end_type() == DEVMM_END_DEVICE) {
        /* device end, always printf. */
        return DEVMM_TRUE;
    }
    if (devmm_svm == NULL) {
        /* not init, always printf. */
        return DEVMM_TRUE;
    }
    svm_proc = devmm_get_svm_pro(devmm_get_current_pid());
    if (svm_proc == NULL) {
        /* not find proc, always printf. */
        return DEVMM_TRUE;
    }

    /* print num greater than MAX_PRINT_NUM per period, stop print */
    if (svm_proc->err_print.cnt < DEVMM_SVM_PRO_MAX_PRINT_NUM) {
        svm_proc->err_print.cnt++;
        flag = DEVMM_TRUE;
        /* jiffies inited by process init, first time err may print 2*MAX_PRINT_NUM times */
        temp_jiffies = svm_proc->err_print.jiffies + DEVMM_SVM_PRO_ERR_PRINT_PERIOD;
        if ((svm_proc->err_print.cnt >= DEVMM_SVM_PRO_MAX_PRINT_NUM) && (time_is_before_jiffies(temp_jiffies))) {
            svm_proc->err_print.jiffies = jiffies;
            svm_proc->err_print.cnt = 0;
        }
    }
    return flag;
}

/* create char-dev node* */
static struct class *devmm_cdev_class = NULL;

/* set device cdev node 0640 / set host cdev node 0660 */
STATIC char *devmm_devnode(struct device *d, umode_t *mode)
{
    if (mode != NULL) {
        *mode = DEVMM_DEV_DEVNODE_MODE;
    }
    return NULL;
}

STATIC int devmm_svm_dev_init(void)
{
    struct device *class_cdev = NULL;
    dev_t devno;
    u32 major;
    int ret;

    ret = alloc_chrdev_region(&devmm_svm->dev_no, 0, 1, DEVMM_SVM_DEV_NAME);
    if (ret) {
        devmm_drv_err("alloc_chrdev_region error.\n");
        return ret;
    }

    /* init and add char device */
    major = MAJOR(devmm_svm->dev_no);
    devno = MKDEV(major, 0);
    cdev_init(&devmm_svm->char_dev, &devmm_svm_fops);
    devmm_svm->char_dev.owner = THIS_MODULE;
    ret = cdev_add(&devmm_svm->char_dev, devno, 1);
    if (ret) {
        devmm_drv_err("cdev_add error. major=%u, devno=%d.\n", major, (int)devno);
        goto cdev_add_fail;
    }

    /* after the process followed, mmapi node in /dev setups automatically */
    devmm_cdev_class = class_create(THIS_MODULE, DEVMM_SVM_DEV_NAME);
    if (IS_ERR_OR_NULL(devmm_cdev_class)) {
        devmm_drv_err("class_create error. major=%u, devno=%d.\n", major, (int)devno);
        ret = -EBUSY;
        goto class_create_fail;
    }
    devmm_cdev_class->devnode = devmm_devnode;

    class_cdev = device_create(devmm_cdev_class, NULL, devno, NULL, DEVMM_SVM_DEV_NAME);
    if (IS_ERR_OR_NULL(class_cdev)) {
        devmm_drv_err("device_create error.\n");
        ret = -ENOTBLK;
        goto device_create_fail;
    }
    devmm_svm->dev = class_cdev;

    return 0;

device_create_fail:
    class_destroy(devmm_cdev_class);
class_create_fail:
    cdev_del(&devmm_svm->char_dev);
cdev_add_fail:
    unregister_chrdev_region(devmm_svm->dev_no, 1);

    return ret;
}

STATIC void devmm_svm_dev_destory(void)
{
    u32 major = MAJOR(devmm_svm->dev_no);
    dev_t devno = MKDEV(major, 0);

    device_destroy(devmm_cdev_class, devno);
    class_destroy(devmm_cdev_class);
    cdev_del(&devmm_svm->char_dev);
    unregister_chrdev_region(devno, 1);
}


STATIC int devmm_svm_init(void)
{
    int ret;

    devmm_drv_info("enter devmm mod init.\n");

    ret = devmm_init_devmm_struct();
    if (ret) {
        devmm_drv_err("devmm_init_devmm_struct fail.\n");
        return -ENOMEM;
    }

    ret = devmm_svm_dev_init();
    if (ret) {
        devmm_drv_err("alloc_chrdev_region error.\n");
        goto register_dev_fail;
    }

    ret = devmm_msg_chan_init();
    if (ret) {
        devmm_drv_err("register msg client error, ret=%d.\n", ret);
        goto register_client_fail;
    }

    devmm_drv_info("leave devmm mod init.\n");
    return 0;

register_client_fail:
    devmm_svm_dev_destory();
register_dev_fail:
    devmm_unint_devmm_struct();

    return ret;
}

STATIC void devmm_svm_exit(void)
{
    devmm_msg_chan_uninit();
    devmm_uninit_dev_private(devmm_svm);
    devmm_svm_dev_destory();
    devmm_unint_devmm_struct();
}

module_init(devmm_svm_init);
module_exit(devmm_svm_exit);
MODULE_AUTHOR("Huawei Tech. Co., Ltd.");
MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("devmm shared memory manager driver");
