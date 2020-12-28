/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2012-2019. All rights reserved.
 * Description:
 * Author: huawei
 * Create: 2019-10-15
 */
#ifndef __DEVDRV_NSM_H
#define __DEVDRV_NSM_H

#define RANDOM_LENGTH          24

struct ipc_notify_node {
    u32 dev_id;
    u32 notify_id;

    u32 tag;
    char name[DEVDRV_IPC_NAME_SIZE];
    pid_t pid[DEVDRV_PID_MAX_NUM];

    atomic_t ref;

    struct hlist_node link;

    struct list_head create_list_node;
    struct list_head open_list_node;
    spinlock_t spinlock;
};

enum devdrv_ipc_type {
    DEVDRV_IPC_NOTIFY = 0,
    DEVDRV_IPC_TYPE_MAX,
};

enum devdrv_ipc_ref_status {
    DEVDRV_IPC_REF_INIT = 1,
    DEVDRV_IPC_REF_MAX
};

int devdrv_ipc_create(char *ipc_name, void **ipc_node, enum devdrv_ipc_type);
int devdrv_ipc_del(const char *ipc_name, u32 max_len, enum devdrv_ipc_type);
int devdrv_ipc_find(const char *ipc_name, void **ipc_node, enum devdrv_ipc_type);
int devdrv_get_random(char *sign, u32 len);

#endif /* __DEVDRV_NSM_H */
