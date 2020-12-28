/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2012-2019. All rights reserved.
 * Description: This file contains api definition for memory manager driver.
 * Author: huawei
 * Create: 2019-06-11
 */

#include <linux/dma-mapping.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/slab.h>
#include <linux/sched.h>
#include <linux/vmalloc.h>
#include <linux/types.h>
#include <linux/delay.h>
#include <linux/version.h>
#include <linux/mm.h>
#include <linux/pci.h>

#if LINUX_VERSION_CODE >= KERNEL_VERSION(4, 11, 0)
#include <linux/sched/signal.h>
#endif
#include "devmm_adapt.h"
#include "devmm_dev.h"
#include "devmm_proc_info.h"
#include "devmm_channel.h"
#include "devdrv_interface.h"
#include "devmm_nsm.h"
#include "devmm_msg_init.h"

int devmm_set_dma_addr_to_device(struct devmm_ioctl_arg *arg)
{
    struct devmm_chan_translate_ack dma_ack = {{0}};
    int ret;

    dma_ack.head.msg_id = DEVMM_CHAN_SET_DMA_H2D_ID;
    dma_ack.head.hostpid = devmm_get_current_pid();
    dma_ack.head.dev_id = arg->head.devid;
    ret = memcpy_s(&dma_ack.dma_addr, sizeof(struct DMA_ADDR),
                   &arg->data.convrt_para.dmaAddr, sizeof(struct DMA_ADDR));
    if (ret) {
        devmm_drv_err("memcpy_s failed. ret=%d devid=%d\n", ret, dma_ack.head.dev_id);
        return ret;
    }

    ret = memset_s(&arg->data.convrt_para.dmaAddr, sizeof(struct DMA_ADDR), 0, sizeof(struct DMA_ADDR));
    if (ret) {
        devmm_drv_err("memset_s failed. ret=%d devid=%d\n", ret, dma_ack.head.dev_id);
        return ret;
    }

    down(&devmm_svm->share_memory_sem);
    ret = devmm_chan_msg_send(&dma_ack, sizeof(dma_ack), sizeof(dma_ack));
    if (ret) {
        devmm_drv_err("send msg fail. ret=%d devid=%d\n", ret, dma_ack.head.dev_id);
        up(&devmm_svm->share_memory_sem);
        return ret;
    }
    up(&devmm_svm->share_memory_sem);

    arg->data.convrt_para.dmaAddr.offsetAddr.devid = arg->data.convrt_para.virt_id;
    arg->data.convrt_para.dmaAddr.offsetAddr.offset = dma_ack.dma_addr.offsetAddr.offset;

    return 0;
}

int devmm_get_dma_addr_from_device(struct devmm_ioctl_arg *arg)
{
    struct devmm_chan_translate_ack dma_ack = {{0}};
    int ret;

    dma_ack.head.msg_id = DEVMM_CHAN_GET_DMA_H2D_ID;
    dma_ack.head.hostpid = devmm_get_current_pid();
    dma_ack.head.dev_id = arg->head.devid;
    dma_ack.dma_addr.offsetAddr.offset = arg->data.desty_para.dmaAddr.offsetAddr.offset;

    down(&devmm_svm->share_memory_sem);
    ret = devmm_chan_msg_send(&dma_ack, sizeof(dma_ack), sizeof(dma_ack));
    if (ret) {
        devmm_drv_err("send msg fail. ret=%d devid=%d\n", ret, dma_ack.head.dev_id);
        up(&devmm_svm->share_memory_sem);
        return ret;
    }
    up(&devmm_svm->share_memory_sem);

    ret = memcpy_s(&arg->data.desty_para.dmaAddr, sizeof(struct DMA_ADDR),
                   &dma_ack.dma_addr, sizeof(struct DMA_ADDR));
    if (ret) {
        devmm_drv_err("memcpy_s failed. ret=%d devid=%d\n", ret, dma_ack.head.dev_id);
        return ret;
    }

    return 0;
}