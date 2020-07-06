/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2012-2019. All rights reserved.
 * Description: This file contains api declaration for memory manager driver.
 * Author: huawei
 * Create: 2019-06-11
 */

#ifndef __DEVMM_MSG_INIT_H__
#define __DEVMM_MSG_INIT_H__

struct devmm_init_work {
    int devid;
    int times;
    struct delayed_work init_work;
};

#define DEVMM_MSG_INIT_SEND_TRYTIMES 1000
#define DEVMM_MSG_INIT_SEND_WAITTIME 1000

unsigned int devmm_devid_to_busnumber(u32 dev_id);
struct device *devmm_devid_to_device(u32 dev_id);
int devmm_channel_set_ready(u32 dev_id);

#endif /* __DEVMM_MSG_INIT_H__ */
