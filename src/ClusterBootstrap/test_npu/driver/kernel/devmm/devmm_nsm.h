/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2012-2019. All rights reserved.
 * Description: This file contains api definition for memory manager driver.
 * Author: huawei
 * Create: 2019-06-11
 */

#ifndef __DEVMM_NSM_H
#define __DEVMM_NSM_H

#define DEVMM_IPC_MEM_NAME_SIZE (65)
#define DEVMM_HASH_SHIT 4
#define DEVMM_TMP_HASH_SHIT 24
#define DEVMM_HASH_MASK 0xF0000000
#define DEVMM_SECONDARY_PROCESS_NUM (32)
#define DEVMM_HASH_LIST_NUM_SHIT 7  /* 128 1<<7 */

struct ipc_mem_node {
    char name[DEVMM_IPC_MEM_NAME_SIZE];
    struct hlist_node link;
    struct rw_semaphore ipc_sema;  /* pa unmap or pa remap */
    struct mutex node_mutex;       /* node open and close assigning */
    pid_t pid[DEVMM_SECONDARY_PROCESS_NUM];
    void *svm_process[DEVMM_SECONDARY_PROCESS_NUM];
    u64 vptr[DEVMM_SECONDARY_PROCESS_NUM];
    pid_t white_list_pid[DEVMM_SECONDARY_PROCESS_NUM];
    u64 len;
    int is_huge;
    u32 devid;
    u32 sequence;  /* use to printf and get node info form record */
    /*
    * ref create set to 1, devmm_ipc_get_node + 1, must call devmm_ipc_put_node to -1
    * ipc open + 1, must call close -1, create node call destory -1
    * when ref = zero del node
    */
    atomic_t ref;
};

int devmm_ipc_create(const char *ipc_name, struct ipc_mem_node **ipc_node);
int devmm_ipc_get_node(const char *ipc_name, struct ipc_mem_node **ipc_node);
void devmm_ipc_del(const char *ipc_name);
void devmm_ipc_put_node(struct ipc_mem_node *ipc_node);

#endif /* __DEVMM_NSM_H */
