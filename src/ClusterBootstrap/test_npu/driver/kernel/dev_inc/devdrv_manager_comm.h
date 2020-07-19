/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2017-2019. All rights reserved.
 * Description:
 * Author:
 * Create: 2020-1-8
 */

#ifndef __DEVDRV_MANAGER_COMM_H
#define __DEVDRV_MANAGER_COMM_H


#ifndef pid_t
typedef int pid_t;
#endif

#define MAX_SUP_MNT_NS_NUM 5 /* max admin docker num */
#define MAX_DOCKER_NUM (64U + MAX_SUP_MNT_NS_NUM) /* equal to max device num add  max admin docker num */

int devdrv_creat_ipc_name(char *ipc_name, unsigned int len);
int devdrv_check_hostpid(pid_t hostpid);
int devdrv_bind_hostpid(pid_t hostpid, const char *sign, u32 len);
int devdrv_check_sign(pid_t hostpid, const char *sign, u32 len);

extern struct devdrv_info *devdrv_manager_get_devdrv_info(u32 dev_id);

#endif /* __DEVDRV_MANAGER_COMM_H */

