/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2012-2019. All rights reserved.
 * Description:
 * Author: huawei
 * Create: 2019-12-20
 */
#include <linux/fs.h>
#include <linux/jiffies.h>

#include "devdrv_manager.h"
#include "devdrv_manager_rand.h"

int devdrv_get_random(char *random, u32 len)
{
    struct file *file = NULL;
    unsigned long time;
    u32 tmp_len = len;
    u32 out_size = 0;
    loff_t pos;
    int ret;

    if (random == NULL || len <= 0) {
        devdrv_drv_err("random is null or invalid len(%u).\n", len);
        return -EINVAL;
    }

    file = filp_open("/dev/random", O_RDONLY, 0);
    if (IS_ERR_OR_NULL(file)) {
        devdrv_drv_err("open file:/dev/random failed, file(%pK), errcode(%ld).\n", file, PTR_ERR(file));
        return -EINVAL;
    }

    time = jiffies;
    do {
        pos = 0;
        tmp_len = len - out_size;

#if LINUX_VERSION_CODE >= KERNEL_VERSION(4, 14, 0)
        ret = kernel_read(file, random + out_size, (ssize_t)tmp_len, &pos);
#else
        ret = devdrv_load_file_read(file, &pos, random + out_size, (ssize_t)tmp_len);
#endif
        if (ret == -ERESTARTSYS || ret == -EAGAIN) {
            devdrv_drv_warn("kernel read continue, ret(%d).\n", ret);
            continue;
        }
        if (ret < 0) {
            devdrv_drv_err("kernel read failed, ret(%d).\n", ret);
            filp_close(file, NULL);
            file = NULL;
            return -EINVAL;
        }
        out_size += ret;
    } while (out_size < len);

    if (time_is_before_jiffies(time + HZ / 10)) {
        devdrv_drv_warn("spend time : %u ms.\n", jiffies_to_msecs(jiffies - time));
    }

    filp_close(file, NULL);
    file = NULL;

    return 0;
}
EXPORT_SYMBOL(devdrv_get_random);
