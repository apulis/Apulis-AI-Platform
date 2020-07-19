/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2012-2019. All rights reserved.
 * Description: This file contains api definition for memory manager driver.
 * Author: huawei
 * Create: 2019-06-11
 */

#include <linux/slab.h>
#include <linux/vmalloc.h>
#include <linux/types.h>
#include <linux/delay.h>
#include <linux/version.h>
#include <linux/mm.h>

#include "devmm_adapt.h"
#include "devmm_proc_ipc.h"
#include "devmm_channel.h"
#include "devmm_nsm.h"
#include "devdrv_manager_comm.h"
#include "devmm_common.h"

STATIC int devmm_ipc_mem_check_pid(struct ipc_mem_node *node)
{
    pid_t pid = devmm_get_current_pid();
    int i;

    for (i = 0; i < DEVMM_SECONDARY_PROCESS_NUM; i++) {
        if (pid == node->pid[i]) {
            devmm_drv_err("already opened by thread=%d. vptr=(0x%llx).\n", node->pid[i], node->vptr[i]);
            return -EFAULT;
        }
    }
    for (i = 0; i < DEVMM_SECONDARY_PROCESS_NUM; i++) {
        if (pid == node->white_list_pid[i]) {
            return 0;
        }
    }
    devmm_drv_err("this node not allowed open by this thread=%d.\n", pid);
    return -EINVAL;
}

STATIC int devmm_ipc_mem_get_idle_index(struct ipc_mem_node *node, pid_t pid, unsigned long vptr)
{
    int i, ret;

    mutex_lock(&node->node_mutex);
    ret = devmm_ipc_mem_check_pid(node);
    if (ret) {
        mutex_unlock(&node->node_mutex);
        devmm_drv_err("node check pid err. vptr=0x%lx, ret=%d.", vptr, ret);
        return DEVMM_SVM_INVALID_INDEX;
    }
    /* 0 is refed for create process */
    for (i = 1; i < DEVMM_SECONDARY_PROCESS_NUM; i++) {
        if (node->pid[i] == DEVMM_SVM_INVALID_PID) {
            node->pid[i] = pid;
            node->vptr[i] = vptr;
            mutex_unlock(&node->node_mutex);
            return i;
        }
    }
    mutex_unlock(&node->node_mutex);
    return DEVMM_SVM_INVALID_INDEX;
}

STATIC int devmm_ipc_mem_get_index(struct ipc_mem_node *node, pid_t pid, unsigned long vptr)
{
    int i;

    mutex_lock(&node->node_mutex);
    for (i = 0; i < DEVMM_SECONDARY_PROCESS_NUM; i++) {
        /* each process can open / create same node once */
        if ((node->pid[i] == pid) && ((vptr == 0) || (node->vptr[i] == vptr))) {
            mutex_unlock(&node->node_mutex);
            return i;
        }
    }
    mutex_unlock(&node->node_mutex);

    return DEVMM_SVM_INVALID_INDEX;
}

STATIC void devmm_ipc_mem_node_set_close(struct ipc_mem_node *node, u32 index)
{
    node->pid[index] = DEVMM_SVM_INVALID_PID;
    node->vptr[index] = 0;
    node->svm_process[index] = NULL;
    atomic_dec(&node->ref);
}

STATIC void devmm_ipc_mem_node_set_open(struct devmm_svm_process *svm_proc, struct ipc_mem_node *node, u32 index)
{
    atomic_inc(&node->ref);
    node->svm_process[index] = svm_proc;
}

STATIC u32 devmm_ipc_mem_create_check_and_set(struct devmm_svm_process *svm_process, DVdeviceptr vptr, u64 len,
                                              u32 heap_idx, u32 page_size)
{
    u64 page_max, page_cnt;
    u32 *bitmap = NULL;
    u32 page_num = 0;

    if ((len == 0) || (page_size == 0) || (vptr % page_size != 0)) {
        devmm_drv_err("len(%llu), or vptr(0x%llx) is not aligned by page_size(%d).\n", len, vptr, page_size);
        goto devmm_ipc_create_check_out;
    }
    bitmap = devmm_get_page_bitmap_with_heapidx(svm_process, heap_idx, vptr);
    if ((bitmap == NULL) || devmm_check_status_va_info(svm_process, vptr, len)) {
        devmm_drv_err("vptr=0x%llx, heap_idx=%u\n", vptr, heap_idx);
        goto devmm_ipc_create_check_out;
    }
    page_max = devmm_get_pagecount_by_size(vptr, len, page_size);
    for (page_cnt = 0; page_cnt < page_max; page_cnt++) {
        if (devmm_page_bitmap_is_page_available(bitmap + page_cnt)) {
            if (devmm_page_bitmap_is_host_mapped(bitmap + page_cnt) ||
                devmm_page_bitmap_is_locked_host(bitmap + page_cnt) ||
                !devmm_page_bitmap_is_dev_mapped(bitmap + page_cnt)) {
                devmm_drv_err("vptr=0x%llx is locked host or hostmapped. bitmap=0x%x\n", vptr, *(bitmap + page_cnt));
                break;
            }
        } else {
            break;
        }
    }
    if (page_cnt != page_max) {
        devmm_drv_err("vptr(0x%llx) add len(%llu) out off range. Just found %llu in %llu pages!\n", vptr, len, page_cnt,
                      page_max);
        devmm_print_pre_alloced_va(svm_process, vptr);
        goto devmm_ipc_create_check_out;
    }
    page_num = page_cnt;
    for (page_cnt = 0; page_cnt < page_num; page_cnt++) {
        devmm_page_bitmap_set_flag(bitmap + page_cnt, DEVMM_PAGE_IS_IPC_MEM);
    }

devmm_ipc_create_check_out:
    return page_num;
}

STATIC int devmm_ipc_mem_set_node_to_proc(struct devmm_svm_process *svm_proc,
    struct ipc_mem_node *node, u8 idx, u64 va)
{
    struct devmm_ipc_mem_node *proc_mem_node = NULL;
    int head_idx;

    mutex_lock(&svm_proc->ipc_node.node_mutex);
    if (svm_proc->ipc_node.node_cnt >= DEVMM_MAX_IPC_MEM_NUM) {
        mutex_unlock(&svm_proc->ipc_node.node_mutex);
        devmm_drv_err("no idle ipc mem node. pid=%d,  devid=%u, len=%llu, is_huge=%d\n",
            svm_proc->hostpid, node->devid, node->len, node->is_huge);
        return -EINVAL;
    }
    proc_mem_node = kzalloc(sizeof(struct devmm_ipc_mem_node), GFP_KERNEL);
    if(proc_mem_node == NULL) {
        mutex_unlock(&svm_proc->ipc_node.node_mutex);
        devmm_drv_err("no idle ipc mem node. pid=%d,  devid=%u, len=%llu, is_huge=%d\n",
            svm_proc->hostpid, node->devid, node->len, node->is_huge);
        return -ENOMEM;
    }
    proc_mem_node->ipc_mem_node = node;
    proc_mem_node->idx = idx;
    svm_proc->ipc_node.node_cnt++;
    head_idx = (va >> DEVMM_IPC_NODE_ADDR_SHIFT) & (DEVMM_MAX_IPC_NODE_LIST_NUM - 1);
    list_add(&proc_mem_node->list, &svm_proc->ipc_node.head[head_idx]);
    mutex_unlock(&svm_proc->ipc_node.node_mutex);

    return 0;
}

STATIC int devmm_ipc_mem_remove_node_from_proc(struct devmm_svm_process *svm_proc, struct ipc_mem_node *node, u64 va)
{
    struct devmm_ipc_mem_node *proc_mem_node = NULL;
    struct list_head *pos = NULL;
    struct list_head *n = NULL;
    int head_idx;

    head_idx = (va >> DEVMM_IPC_NODE_ADDR_SHIFT) & (DEVMM_MAX_IPC_NODE_LIST_NUM - 1);
    mutex_lock(&svm_proc->ipc_node.node_mutex);
    list_for_each_safe(pos, n, &svm_proc->ipc_node.head[head_idx]) {
        proc_mem_node = list_entry(pos, struct devmm_ipc_mem_node, list);
        if(proc_mem_node->ipc_mem_node == node) {
            svm_proc->ipc_node.node_cnt--;
            list_del(&proc_mem_node->list);
            kfree(proc_mem_node);
            proc_mem_node = NULL;
            mutex_unlock(&svm_proc->ipc_node.node_mutex);
            return 0;
        }
    }
    mutex_unlock(&svm_proc->ipc_node.node_mutex);
    devmm_drv_err("has not find ipc mem node(%u). pid=%d, devid=%u, len=%llu, is_huge=%d\n",
                  node->sequence, svm_proc->hostpid, node->devid, node->len, node->is_huge);

    return -EINVAL;
}

STATIC void devmm_ipc_create_node_init(struct devmm_svm_process *svm_process, struct devmm_memory_attributes *attr,
    struct devmm_mem_ipc_create_para *ipc_create_para, u64 page_num, struct ipc_mem_node *node)
{
    int i;

    init_rwsem(&node->ipc_sema);
    mutex_init(&node->node_mutex);
    atomic_set(&node->ref, 1);
    node->len = page_num * attr->page_size;
    node->is_huge = attr->is_svm_huge;
    node->devid = attr->devid;
    node->pid[0] = devmm_get_current_pid();
    node->svm_process[0] = svm_process;
    node->vptr[0] = ipc_create_para->vptr;
    node->white_list_pid[0] = devmm_get_current_pid();
    for (i = 1; i < DEVMM_SECONDARY_PROCESS_NUM; i++) {
        node->pid[i] = DEVMM_SVM_INVALID_PID;
        node->white_list_pid[i] = DEVMM_SVM_INVALID_PID;
        node->svm_process[i] = NULL;
        node->vptr[i] = 0;
    }
    node->sequence = atomic_inc_return(&devmm_svm->ipcnode_sq);
}

int devmm_ioctl_ipc_mem_create(struct devmm_svm_process *svm_process, struct devmm_ioctl_arg *arg)
{
    struct devmm_mem_ipc_create_para *ipc_create_para = &arg->data.ipc_create_para;
    struct devmm_memory_attributes attr = {0};
    struct ipc_mem_node *node = NULL;
    u64 page_num;
    int ret;

    devmm_drv_switch("enter va=0x%llx.\n", ipc_create_para->vptr);
    if (ipc_create_para->name_len < DEVMM_MAX_NAME_SIZE) {
        devmm_drv_err("name_len = %u, err.\n", ipc_create_para->name_len);
        return -EINVAL;
    }

    devmm_get_memory_attributes(svm_process, ipc_create_para->vptr, &attr);
    if (!attr.is_svm_device) {
        devmm_drv_err("mem is not device. va=0x%llx.\n", ipc_create_para->vptr);
        return -EINVAL;
    }
    page_num = devmm_ipc_mem_create_check_and_set(svm_process, ipc_create_para->vptr, ipc_create_para->len,
                                                  attr.heap_idx, attr.page_size);
    if (page_num == 0) {
        devmm_drv_err("page_num err, vptr(0x%llx) size(%ld) page_num(%llu).\n",
            ipc_create_para->vptr, ipc_create_para->len, page_num);
        return -EINVAL;
    }
    ret = devdrv_creat_ipc_name(ipc_create_para->name, DEVMM_MAX_NAME_SIZE);
    if (ret) {
        devmm_drv_err("get node name. vptr=0x%llx, ret = %d\n", ipc_create_para->vptr, ret);
        return -EINVAL;
    }
    ipc_create_para->name[DEVMM_MAX_NAME_SIZE - 1] = '\0';
    ret = devmm_ipc_create(ipc_create_para->name, &node);
    if (ret) {
        devmm_drv_err("create node err. vptr=0x%llx, ret = %d\n", ipc_create_para->vptr, ret);
        return -EBUSY;
    }

    devmm_ipc_create_node_init(svm_process, &attr, ipc_create_para, page_num, node);
    ret = devmm_ipc_mem_set_node_to_proc(svm_process, node, 0, ipc_create_para->vptr);
    if (ret) {
        devmm_drv_err("set_ipc_mem_node fail ret=%d,va=0x%llx,sequnce=%u.\n",
            ret, ipc_create_para->vptr, node->sequence);
        devmm_ipc_put_node(node);
        return -EINVAL;
    }
    devmm_drv_switch("exit suc. create idx(%d), pid=%d, vptr=0x%llx, is_huge=%d, devid=%u.\n",
        node->sequence, node->pid[0], node->vptr[0], node->is_huge, node->devid);

    return 0;
}

STATIC void devmm_ipc_mem_open_set_bitmap(struct ipc_mem_node *node, u64 vptr, u32 *page_bitmap)
{
    u64 i, page_cnt;
    u32 page_size;

    page_size = node->is_huge ? devmm_svm->device_hpage_size : devmm_svm->svm_page_size;
    page_cnt = devmm_get_pagecount_by_size(vptr, node->len, page_size);
    for (i = 0; i < page_cnt; i++) {
        devmm_page_bitmap_set_flag(page_bitmap + i, DEVMM_PAGE_ADVISE_POPULATE_MASK);
        devmm_page_bitmap_set_devid(page_bitmap + i, node->devid);
        devmm_page_bitmap_set_flag(page_bitmap + i, DEVMM_PAGE_DEV_MAPPED_MASK);
        devmm_page_bitmap_set_flag(page_bitmap + i, DEVMM_PAGE_IS_IPC_MEM);
        devmm_page_bitmap_set_flag(page_bitmap + i, DEVMM_PAGE_LOCKED_DEVICE_MASK);
    }
}

STATIC void devmm_ipc_mem_close_set_bitmap(struct ipc_mem_node *node, u64 vptr, u32 *page_bitmap)
{
    u64 i, page_cnt;
    u32 page_size;

    page_size = node->is_huge ? devmm_svm->device_hpage_size : devmm_svm->svm_page_size;
    page_cnt = devmm_get_pagecount_by_size(vptr, node->len, page_size);
    for (i = 0; i < page_cnt; i++) {
        devmm_page_bitmap_clear_flag(page_bitmap + i, DEVMM_PAGE_ADVISE_POPULATE_MASK);
        devmm_page_bitmap_clear_flag(page_bitmap + i, DEVMM_PAGE_DEV_MAPPED_MASK);
        devmm_page_bitmap_clear_flag(page_bitmap + i, DEVMM_PAGE_LOCKED_DEVICE_MASK);
        devmm_page_bitmap_clear_flag(page_bitmap + i, DEVMM_PAGE_IS_IPC_MEM);
    }
}

STATIC int devmm_ipc_mem_remove_map(struct devmm_svm_process *svm_process, struct ipc_mem_node *node, int index)
{
    struct devmm_chan_mem_ipc_close ipc_remove;
    struct devmm_svm_heap *heap = NULL;
    u32 *page_bitmap = NULL;
    int ret;

    devmm_drv_switch("sq(%u) pid=%d, index=%d, va=0x%llx.\n",
        node->sequence, node->pid[index], index, node->vptr[index]);

    heap = devmm_svm_get_heap(svm_process, node->vptr[index]);
    if (heap == NULL) {
        devmm_drv_err("canot find heap, vptr=0x%llx, devid(%u), pid=%d.\n", node->vptr[index], node->devid,
                      node->pid[index]);
        return -EINVAL;
    }

    ipc_remove.head.hostpid = node->pid[index];
    ipc_remove.head.dev_id = node->devid;
    ipc_remove.head.msg_id = DEVMM_CHAN_MEM_IPC_CLOSE_H2D_ID;
    ipc_remove.vptr = node->vptr[index];
    ipc_remove.size = node->len;
    ipc_remove.ref = DEVMM_TRUE;

    page_bitmap = devmm_get_page_bitmap_with_heapidx(svm_process, heap->heap_idx, node->vptr[index]);
    if (page_bitmap == NULL) {
        devmm_drv_err("va may be free vptr=0x%llx, devid(%u), pid=%d.\n", node->vptr[index], node->devid,
                      node->pid[index]);
        return -EINVAL;
    }

    devmm_svm_free_share_page_msg(svm_process, heap, ipc_remove.vptr, ipc_remove.size, page_bitmap);

    ret = devmm_chan_msg_send(&ipc_remove, sizeof(struct devmm_chan_mem_ipc_close), 0);
    if (ret) {
        devmm_drv_err("remove ipc mem pa err ret(%d). vptr=0x%llx, devid(%u), pid=%d.\n", ret, node->vptr[index],
                      node->devid, node->pid[index]);
        return -EINVAL;
    }

    devmm_ipc_mem_close_set_bitmap(node, ipc_remove.vptr, page_bitmap);
    return 0;
}

STATIC void devmm_ipc_mem_destory_node(struct ipc_mem_node *node, int pid_id)
{
    int k, ret;
    /* create node va unmap by user call free so start from k=1 */
    for (k = 1; k < DEVMM_SECONDARY_PROCESS_NUM; k++) {
        ret = (node->pid[k] == DEVMM_SVM_INVALID_PID) || (node->pid[k] == pid_id) ||
              (devmm_va_is_not_svm_process_addr(node->svm_process[k], node->vptr[k]));
        if (ret) {
            continue;
        }
        devmm_drv_switch("create process(%d) crash. pid=%d, vptr=0x%llx.\n", pid_id, node->pid[k], node->vptr[k]);

        ret = devmm_ipc_mem_remove_map(node->svm_process[k], node, k);
        if (ret) {
            devmm_drv_err("remove fail, process(%d). pid=%d, vptr=0x%llx\n", pid_id, node->pid[k], node->vptr[k]);
        }
    }
}

STATIC int devmm_ipc_mem_close_get_name(struct devmm_svm_process *svm_proc, DVdeviceptr vptr, char *name,
                                        int name_len)
{
    struct devmm_ipc_mem_node *proc_mem_node = NULL;
    struct ipc_mem_node *node = NULL;
    struct list_head *pos = NULL;
    struct list_head *n = NULL;
    int head_idx, ret;

    head_idx = (vptr >> DEVMM_IPC_NODE_ADDR_SHIFT) & (DEVMM_MAX_IPC_NODE_LIST_NUM - 1);
    mutex_lock(&svm_proc->ipc_node.node_mutex);
    list_for_each_safe(pos, n, &svm_proc->ipc_node.head[head_idx]) {
        proc_mem_node = list_entry(pos, struct devmm_ipc_mem_node, list);
        node = proc_mem_node->ipc_mem_node;
        /* ptr return by malloced is sole, the name found by ptr and pid will be sole, idx=0 is created */
        if ((proc_mem_node->idx == 0) || (node->pid[proc_mem_node->idx] != svm_proc->hostpid) ||
            (node->vptr[proc_mem_node->idx] != vptr)) {
            continue;
        }
        ret = memcpy_s(name, name_len, node->name, DEVMM_IPC_MEM_NAME_SIZE);
        if (ret) {
            devmm_drv_err("memcpy_s error ret=%d.\n", ret);
            mutex_unlock(&svm_proc->ipc_node.node_mutex);
            return -EINVAL;
        }
        /* set svm proc node null, avoid close many times at the same time */
        svm_proc->ipc_node.node_cnt--;
        list_del(&proc_mem_node->list);
        kfree(proc_mem_node);
        proc_mem_node = NULL;
        mutex_unlock(&svm_proc->ipc_node.node_mutex);
        return 0;
    }
    mutex_unlock(&svm_proc->ipc_node.node_mutex);
    devmm_drv_err("can not find va=0x%llx name. check ptr is right or not.\n", vptr);
    return -EINVAL;
}

int devmm_ioctl_ipc_mem_set_pid(struct devmm_svm_process *svm_pro, struct devmm_ioctl_arg *arg)
{
    struct devmm_mem_ipc_set_pid_para *ipc_set_pid_para = &arg->data.ipc_set_pid_para;
    struct ipc_mem_node *node = NULL;
    int ret;
    u32 i, j;

    /* prevent string is not end of \0 */
    ipc_set_pid_para->name[DEVMM_MAX_NAME_SIZE - 1] = '\0';
    devmm_drv_switch("pid=%d.\n", ipc_set_pid_para->set_pid[0]);
    ret = devmm_ipc_get_node(ipc_set_pid_para->name, &node);
    if (ret || (node->pid[0] != devmm_get_current_pid())) {
        devmm_drv_err("find node err. ret=%d,or set pid not create thread.", ret);
        return -EINVAL;
    }
    mutex_lock(&node->node_mutex);
    /* find seted num */
    for (i = 0; i < DEVMM_SECONDARY_PROCESS_NUM; i++) {
        if (node->white_list_pid[i] == DEVMM_SVM_INVALID_PID) {
            break;
        }
    }
    if (((i + ipc_set_pid_para->num) > DEVMM_SECONDARY_PROCESS_NUM) ||
        (ipc_set_pid_para->num > (DEVMM_SECONDARY_PROCESS_NUM - 1))) {
        mutex_unlock(&node->node_mutex);
        devmm_ipc_put_node(node);
        devmm_drv_err("while pid num to large. seted num=%u, set num=%u", i, ipc_set_pid_para->num);
        return -EINVAL;
    }
    for (j = 0; j < ipc_set_pid_para->num; j++) {
        for (i = 0; i < DEVMM_SECONDARY_PROCESS_NUM; i++) {
            if (node->white_list_pid[i] == ipc_set_pid_para->set_pid[j]) {
                /* already setted , continue ok */
                break;
            }
            if (node->white_list_pid[i] == DEVMM_SVM_INVALID_PID) {
                node->white_list_pid[i] = ipc_set_pid_para->set_pid[j];
                break;
            }
        }
    }
    mutex_unlock(&node->node_mutex);
    devmm_ipc_put_node(node);
    devmm_drv_switch("exit, suc.num=%d, pid=%d.\n", ipc_set_pid_para->num, ipc_set_pid_para->set_pid[0]);

    return ret;
}

STATIC int devmm_ipc_mem_remove(struct devmm_svm_process *svm_process, pid_t pid, const char *name, u64 vptr)
{
    struct ipc_mem_node *node = NULL;
    int index, ret;

    ret = devmm_ipc_get_node(name, &node);
    if (ret) {
        devmm_drv_err("find node err. va=0x%llx.\n", vptr);
        return -EINVAL;
    }

    index = devmm_ipc_mem_get_index(node, pid, vptr);
    devmm_drv_switch("va=0x%llx, pid=%d, sequence=%u, index=%d.\n", vptr, pid, node->sequence, index);
    if (index == DEVMM_SVM_INVALID_INDEX) {
        devmm_drv_err("find index node err. va=0x%llx, sequence=%u.\n", vptr, node->sequence);
        devmm_ipc_put_node(node);
        return -EINVAL;
    }
    if (index == 0) {
        /* here will unmap all thread va open by this node, avoids act with close or open at the same time */
        down_write(&node->ipc_sema);
        devmm_ipc_mem_destory_node(node, pid);
        devmm_ipc_mem_node_set_close(node, index);
        up_write(&node->ipc_sema);
    } else {
        /* avoid act with destory */
        down_read(&node->ipc_sema);
        ret = devmm_ipc_mem_remove_map(svm_process, node, index);
        devmm_ipc_mem_node_set_close(node, index);
        up_read(&node->ipc_sema);
        if (ret) {
            devmm_drv_err("remove ipc mem pa err ret(%d). vptr=0x%llx, devid(%u), pid=%d.\n", ret, node->vptr[index],
                          node->devid, node->pid[index]);
        }
    }
    devmm_ipc_put_node(node);

    return ret;
}

int devmm_ioctl_ipc_mem_destroy(struct devmm_svm_process *svm_proc, struct devmm_ioctl_arg *arg)
{
    struct devmm_mem_ipc_destroy_para *ipc_destroy_para = &arg->data.ipc_destroy_para;
    struct ipc_mem_node *node = NULL;
    int ret;

    /* prevent string is not end of \0 */
    ipc_destroy_para->name[DEVMM_MAX_NAME_SIZE - 1] = '\0';
    ret = devmm_ipc_get_node(ipc_destroy_para->name, &node);
    if (ret) {
        devmm_drv_err("find node err.\n");
        return -EINVAL;
    }
    if (node->pid[0] != svm_proc->hostpid) {
        devmm_drv_err("not created by this thread.\n");
        return -EINVAL;
    }
    ret = devmm_ipc_mem_remove_node_from_proc(svm_proc, node, node->vptr[0]);
    if (ret) {
        devmm_drv_err("node(%u) already removed from this thread.\n", node->sequence);
        return -EINVAL;
    }
    /* here will unmap all thread va open by this node, avoids act with close or open at the same time */
    down_write(&node->ipc_sema);
    devmm_ipc_mem_destory_node(node, svm_proc->hostpid);
    devmm_ipc_mem_node_set_close(node, 0);
    up_write(&node->ipc_sema);
    devmm_ipc_put_node(node);

    return ret;
}

STATIC int devmm_chan_ipc_mem_open_msg_send(struct devmm_svm_process *svm_process, struct ipc_mem_node *node,
    struct devmm_mem_ipc_open_para *ipc_open_para)
{
    struct devmm_chan_mem_ipc_open ipc_open;
    int ret;

    ipc_open.head.hostpid = svm_process->hostpid;
    ipc_open.head.dev_id = node->devid;
    ipc_open.head.msg_id = DEVMM_CHAN_MEM_IPC_OPEN_H2D_ID;
    ipc_open.spid = node->pid[0];
    ipc_open.src_vptr = node->vptr[0];
    ipc_open.vptr = ipc_open_para->vptr;
    ipc_open.size = node->len;
    ret = devmm_chan_msg_send(&ipc_open, sizeof(struct devmm_chan_mem_ipc_open), 0);
    if (ret) {
        devmm_drv_err("ipc open err ret(%d). vptr=0x%llx, src_vptr=0x%llx, src_pid=%u.\n", ret, ipc_open_para->vptr,
                      node->vptr[0], node->pid[0]);
    }

    return ret;
}

int devmm_ioctl_ipc_mem_open(struct devmm_svm_process *svm_process, struct devmm_ioctl_arg *arg)
{
    struct devmm_mem_ipc_open_para *ipc_open_para = &arg->data.ipc_open_para;
    struct ipc_mem_node *node = NULL;
    u32 *page_bitmap = NULL;
    int dst_index, ret;

    /* prevent string is not end of \0 */
    ipc_open_para->name[DEVMM_MAX_NAME_SIZE - 1] = '\0';
    ret = devmm_ipc_get_node(ipc_open_para->name, &node);
    if (ret) {
        devmm_drv_err("find node err. vptr=0x%llx.", ipc_open_para->vptr);
        return -EINVAL;
    }

    dst_index = devmm_ipc_mem_get_idle_index(node, svm_process->hostpid, ipc_open_para->vptr);
    if (dst_index < 0) {
        devmm_drv_err("find node index err. vptr=0x%llx, dst_index=%d.", ipc_open_para->vptr, dst_index);
        goto devmm_ioctl_mem_ipc_open_fail_put_node;
    }
    page_bitmap = devmm_get_page_bitmap(svm_process, ipc_open_para->vptr);
    if ((page_bitmap == NULL) || devmm_check_status_va_info(svm_process, ipc_open_para->vptr, node->len)) {
        devmm_drv_err("dst va err, vptr=0x%llx.\n", ipc_open_para->vptr);
        goto devmm_ioctl_mem_ipc_open_fail_put_node;
    }
    ret = devmm_ipc_mem_set_node_to_proc(svm_process, node, dst_index, ipc_open_para->vptr);
    if (ret) {
        devmm_drv_err("set_share_mem_node fail ret=%d,va=0x%llx.\n", ret, ipc_open_para->vptr);
        goto devmm_ioctl_mem_ipc_open_fail_put_node;
    }

    devmm_ipc_mem_open_set_bitmap(node, ipc_open_para->vptr, page_bitmap);
    /* avoid act with destory */
    down_read(&node->ipc_sema);
    devmm_ipc_mem_node_set_open(svm_process, node, dst_index);
    ret = devmm_chan_ipc_mem_open_msg_send(svm_process, node, ipc_open_para);
    up_read(&node->ipc_sema);
    if (ret) {
        devmm_drv_err("ipc open msg err ret(%d). vptr=0x%llx, src_vptr=0x%llx, src_pid=%u.\n",
            ret, ipc_open_para->vptr, node->vptr[0], node->pid[0]);
        goto devmm_ioctl_mem_ipc_open_msg_send_fail;
    }

    devmm_ipc_put_node(node);
    return 0;

devmm_ioctl_mem_ipc_open_msg_send_fail:
    devmm_ipc_mem_close_set_bitmap(node, ipc_open_para->vptr, page_bitmap);
    devmm_ipc_mem_node_set_close(node, dst_index);
    (void)devmm_ipc_mem_remove_node_from_proc(svm_process, node, ipc_open_para->vptr);
devmm_ioctl_mem_ipc_open_fail_put_node:
    devmm_ipc_put_node(node);

    return ret ? ret : -EINVAL;
}

int devmm_ioctl_ipc_mem_close(struct devmm_svm_process *svm_pro, struct devmm_ioctl_arg *arg)
{
    struct devmm_mem_ipc_close_para *ipc_close_para = &arg->data.ipc_close_para;
    char name[DEVMM_MAX_NAME_SIZE] = {0};
    int ret;

    devmm_drv_switch("enter vptr=0x%llx.\n", ipc_close_para->vptr);
    ret = devmm_ipc_mem_close_get_name(svm_pro, ipc_close_para->vptr, name, DEVMM_MAX_NAME_SIZE);
    if (ret) {
        devmm_drv_err("cannot find close node ret=%d, va=0x%llx.\n", ret, ipc_close_para->vptr);
        /* is already close or is create ptr return fail */
        (void)memset_s(name, DEVMM_MAX_NAME_SIZE, 0, DEVMM_MAX_NAME_SIZE);
        return -EINVAL;
    }
    /* prevent string is not end of \0 */
    name[DEVMM_MAX_NAME_SIZE - 1] = '\0';
    ret = devmm_ipc_mem_remove(svm_pro, devmm_get_current_pid(), name, ipc_close_para->vptr);
    (void)memset_s(name, DEVMM_MAX_NAME_SIZE, 0, DEVMM_MAX_NAME_SIZE);
    if (ret) {
        devmm_drv_err("close share mem fail ret=%d,va=0x%llx.\n", ret, ipc_close_para->vptr);
        return ret;
    }

    return 0;
}

int devmm_ioctl_ipc_mem_query(struct devmm_svm_process *svm_pro, struct devmm_ioctl_arg *arg)
{
    struct devmm_mem_query_size_para *query_size_para = &arg->data.query_size_para;
    struct devmm_ioctl_arg ioctl_arg;
    struct ipc_mem_node *node = NULL;
    int dst_index;
    u32 sequence;
    int ret;

    /* prevent string is not end of \0 */
    query_size_para->name[DEVMM_MAX_NAME_SIZE - 1] = '\0';

    ret = devmm_ipc_get_node(query_size_para->name, &node);
    if (ret) {
        devmm_drv_err("find name node err.\n");
        return -EINVAL;
    }
    dst_index = devmm_ipc_mem_get_index(node, devmm_get_current_pid(), 0);
    if (dst_index >= 0) {
        devmm_drv_err("this node already opened by current thread! sequence=%u, va=0x%llx.\n",
            node->sequence, node->vptr[dst_index]);
        devmm_ipc_put_node(node);
        return -EINVAL;
    }
    query_size_para->len = node->len;
    query_size_para->is_huge = node->is_huge;
    ioctl_arg.head.devid = node->devid;
    sequence = node->sequence;
    devmm_ipc_put_node(node);
    ioctl_arg.data.setup_device_para.aicpu_flag = 0;
    ret = devmm_ioctl_setup_device(svm_pro, &ioctl_arg);
    if (ret) {
        devmm_drv_err("setup device(%u) failed, sequence=%u, ret=%d.\n", ioctl_arg.head.devid, sequence, ret);
        return -EINVAL;
    }
    devmm_drv_switch("exit suc.sequence=%u, len=%lu, is_huge=%d, did=%d.\n",
        sequence, query_size_para->len, query_size_para->is_huge, ioctl_arg.head.devid);

    return 0;
}

void devmm_destroy_ipc_mem_node_by_proc(struct devmm_svm_process *svm_proc)
{
    struct devmm_ipc_mem_node *proc_mem_node = NULL;
    struct ipc_mem_node *node = NULL;
    struct list_head *pos = NULL;
    struct list_head *n = NULL;
    u32 sequence;
    int i, ret;

    mutex_lock(&svm_proc->ipc_node.node_mutex);
    for (i = 0; i < DEVMM_MAX_IPC_NODE_LIST_NUM; i++) {
        list_for_each_safe(pos, n, &svm_proc->ipc_node.head[i]) {
            proc_mem_node = list_entry(pos, struct devmm_ipc_mem_node, list);
            node = proc_mem_node->ipc_mem_node;
            /* just remove node open by this thread */
            if (node->pid[proc_mem_node->idx] != svm_proc->hostpid) {
                continue;
            }
            sequence = node->sequence;
            ret = devmm_ipc_mem_remove(svm_proc, svm_proc->hostpid,
                node->name, node->vptr[proc_mem_node->idx]);
            if (ret) {
                devmm_drv_warn("remove ipc mem local fail, sequence=%d.\n", sequence);
            }
            /* set svm proc node null, avoid close many times at the same time */
            svm_proc->ipc_node.node_cnt--;
            list_del(&proc_mem_node->list);
            kfree(proc_mem_node);
            proc_mem_node = NULL;
        }
    }
    mutex_unlock(&svm_proc->ipc_node.node_mutex);
}
