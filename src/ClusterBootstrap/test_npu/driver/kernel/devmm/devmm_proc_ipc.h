/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2012-2019. All rights reserved.
 * Description: This file contains api definition for memory manager driver.
 * Author: huawei
 * Create: 2019-06-11
 */

#ifndef __DEVMM_PROC_IPC_H
#define __DEVMM_PROC_IPC_H
#include "devmm_proc_info.h"

void devmm_destroy_ipc_mem_node_by_proc(struct devmm_svm_process *svm_pro);
int devmm_ioctl_ipc_mem_query(struct devmm_svm_process *svm_pro, struct devmm_ioctl_arg *arg);
int devmm_ioctl_ipc_mem_close(struct devmm_svm_process *svm_pro, struct devmm_ioctl_arg *arg);
int devmm_ioctl_ipc_mem_open(struct devmm_svm_process *svm_process, struct devmm_ioctl_arg *arg);
int devmm_ioctl_ipc_mem_destroy(struct devmm_svm_process *svm_proc, struct devmm_ioctl_arg *arg);
int devmm_ioctl_ipc_mem_set_pid(struct devmm_svm_process *svm_pro, struct devmm_ioctl_arg *arg);
int devmm_ioctl_ipc_mem_create(struct devmm_svm_process *svm_process, struct devmm_ioctl_arg *arg);

#endif /* __DEVMM_PROC_IPC_H */
