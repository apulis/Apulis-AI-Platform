/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2012-2019. All rights reserved.
 * Description:
 * Author: huawei
 * Create: 2019-10-15
 */
#ifndef __DEVDRV_MANAGER_H
#define __DEVDRV_MANAGER_H

#include "devdrv_manager_common.h"
#include "devdrv_interface.h"

void *devdrv_manager_get_no_trans_chan(u32 dev_id);

struct devdrv_exception {
    struct timespec stamp;
    struct list_head list;
    u32 code;
    u32 devid;

    /* private data for some exception code */
    void *data;
};
struct devdrv_process_sign {
    pid_t hostpid;
    u32 docker_id;
    char sign[PROCESS_SIGN_LENGTH];
    struct list_head list;
};
#ifdef CFG_SOC_PLATFORM_MINIV2
#define DEVDRV_GET_DEVNUM_STARTUP_TIMEOUT 300000
#define DEVDRV_GET_DEVNUM_SYNC_TIMEOUT 1000
#else
#define DEVDRV_GET_DEVNUM_STARTUP_TIMEOUT 300
#define DEVDRV_GET_DEVNUM_SYNC_TIMEOUT 60
#endif

#define DEVDRV_IPC_EVENT_DEFAULT 0
#define DEVDRV_IPC_EVENT_CREATED (1 << 1)
#define DEVDRV_IPC_EVENT_RECORDED (1 << 2)
#define DEVDRV_IPC_EVENT_RECORD_COMPLETED (1 << 3)
#define DEVDRV_IPC_EVENT_DESTROYED (1 << 4)

#define DEVDRV_IPC_NODE_DEFAULT 0
#define DEVDRV_IPC_NODE_OPENED (1 << 3)
#define DEVDRV_IPC_NODE_CLOSED (1 << 4)

#define DEVDRV_INIT_INSTANCE_TIMEOUT (4 * HZ)

struct ipc_notify_info {
    u32 open_fd_num;
    u32 create_fd_num;

    /* created node list head */
    struct list_head create_list_head;
    struct list_head open_list_head;

    struct mutex info_mutex;
};

struct devdrv_manager_context {
    pid_t pid;
    pid_t tgid;
    u32 docker_id;
    struct mnt_namespace *mnt_ns;
    struct pid_namespace *pid_ns;
#if LINUX_VERSION_CODE >= KERNEL_VERSION(4, 1, 0)
    u64 start_time;
    u64 real_start_time;
#else
#ifdef DEVDRV_MANAGER_HOST_UT_TEST
    u64 start_time;
    u64 real_start_time;
#else
    struct timespec start_time;
    struct timespec real_start_time;
#endif
#endif
    struct task_struct *task;
    struct work_struct work;
    struct ipc_notify_info *ipc_notify_info;
};

#define DEVDRV_BB_DEVICE_LOAD_TIMEOUT 0x68020001
#define DEVDRV_BB_DEVICE_HEAT_BEAT_LOST 0x68020002
#define DEVDRV_BB_DEVICE_RESET_INFORM 0x68020003

int devdrv_manager_ipc_notify_create(struct devdrv_manager_context *context, unsigned long arg,
                                     struct devdrv_notify_ioctl_info *notify_ioctl_info);
int devdrv_manager_ipc_notify_open(struct devdrv_manager_context *context, unsigned long arg,
                                   struct devdrv_notify_ioctl_info *notify_ioctl_info);
int devdrv_manager_ipc_notify_close(struct devdrv_manager_context *context,
                                    struct devdrv_notify_ioctl_info *notify_ioctl_info);
int devdrv_manager_ipc_notify_destroy(struct devdrv_manager_context *context,
                                      struct devdrv_notify_ioctl_info *notify_ioctl_info);
int devdrv_manager_ipc_notify_set_pid(struct devdrv_manager_context *context,
                                      struct devdrv_notify_ioctl_info *notify_ioctl_info);
void devdrv_manager_ipc_notify_release_recycle(struct devdrv_manager_context *context);
struct devdrv_manager_info *devdrv_get_manager_info(void);
u32 devdrv_manager_get_ts_num(struct devdrv_info *dev_info);
int devdrv_manager_send_msg(struct devdrv_info *dev_info, struct devdrv_manager_msg_info *dev_manager_msg_info,
    int *out_len);
extern struct devdrv_drv_ops devdrv_host_drv_ops;
extern struct task_struct init_task;
extern int devdrv_get_pcie_id_info(u32 devid, struct devdrv_pcie_id_info *pcie_id_info);

#if LINUX_VERSION_CODE < KERNEL_VERSION(4, 14, 0)
ssize_t devdrv_load_file_read(struct file *file, loff_t *pos, char *addr, size_t count);
#endif
#endif /* __DEVDRV_MANAGER_H */
