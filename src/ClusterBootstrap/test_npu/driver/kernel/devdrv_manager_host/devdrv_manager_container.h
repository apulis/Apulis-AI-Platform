/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2012-2019. All rights reserved.
 * Description:
 * Author: huawei
 * Create: 2019-10-15
 */
#ifndef DEVDRV_MANAGER_CONTAINER_H
#define DEVDRV_MANAGER_CONTAINER_H

#include <linux/mutex.h>
#include <linux/nsproxy.h>

#include "devdrv_manager_common.h"
#include "devdrv_user_common.h"
#include "devdrv_manager.h"

struct devdrv_manager_container_para {
    pid_t tgid;

#if LINUX_VERSION_CODE >= KERNEL_VERSION(4, 1, 0)
    u64 start_time;
    u64 real_start_time;
#else
    struct timespec start_time;
    struct timespec real_start_time;
#endif
    u32 dev_num;
    u32 dev_id[DEVDRV_MAX_DAVINCI_NUM]; /* device list read from dev directory */
    struct mnt_namespace *mnt_ns;
    struct pid_namespace *pid_ns;
};
#define DEVDRV_MAX_PID_NUM 128 /* container start with --pid : max pidlist num */
extern struct devdrv_manager_info *dev_manager_info;
struct mnt_namespace *devdrv_manager_get_host_mnt_ns(void);
int devdrv_manager_container_table_init(struct devdrv_manager_info *manager_info, struct task_struct *tsk);
void devdrv_manager_container_table_exit(struct devdrv_manager_info *manager_info);
int devdrv_manager_container_table_overlap(struct devdrv_manager_context *dev_manager_context, u32 *docker_id);
void devdrv_manager_container_table_process_release(struct devdrv_manager_context *dev_manager_context);
int devdrv_manager_container_table_devlist_add_ns(struct devdrv_manager_info *manager_info, u32 *physical_devlist,
                                                  u32 physical_dev_num);
int devdrv_manager_container_table_devlist_del_ns(struct devdrv_manager_info *manager_info, u32 *physical_devlist,
                                                  u32 physical_dev_num);
int devdrv_manager_container_get_bare_pid(struct devdrv_manager_context *dev_manager_context,
                                          struct devdrv_container_para *cmd);
int devdrv_manager_container_get_bare_tgid(struct devdrv_manager_context *dev_manager_context,
                                           struct devdrv_container_para *cmd);
u32 devdrv_manager_container_get_devnum(struct mnt_namespace *mnt_ns);
int devdrv_manager_container_get_devlist(u32 *devlist, u32 devlist_len, u32 *devnum, struct mnt_namespace *mnt_ns);
int devdrv_manager_container_get_devids_list(struct devdrv_manager_hccl_devinfo *hccl_devinfo);
int devdrv_manager_container_logical_id_to_physical_id(u32 logical_dev_id, u32 *physical_dev_id);
u32 devdrv_manager_container_get_devnum_bare(void);
int devdrv_manager_container_get_docker_id(u32 *docker_id);
int devdrv_manager_container_process(struct file *filep, unsigned long arg);
void devdrv_container_assign_admin_end(void);
int devdrv_manager_container_is_in_container(void);

#endif
