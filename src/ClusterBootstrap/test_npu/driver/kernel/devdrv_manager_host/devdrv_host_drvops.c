/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2012-2019. All rights reserved.
 * Description:
 * Author: huawei
 * Create: 2019-10-15
 */
#include <linux/slab.h>

#include "devdrv_manager_msg.h"
#include "devdrv_host_drvops.h"

#define DEVDRV_TS_DOORBELL_OFFSET1 (510 + DEVDRV_TS_DOORBELL_SQ_NUM)
#define DEVDRV_TS_DOORBELL_OFFSET2 (511 + DEVDRV_TS_DOORBELL_SQ_NUM)
#define DEVDRV_TS_DOORBELL_ONLINE_OFFSET 16

STATIC void devdrv_manager_host_irq_trigger(struct devdrv_info *dev_info, u32 tsid, int irq_num)
{
    struct devdrv_platform_data *pdata = NULL;
    static u32 read_done_value = 0;
    static u32 value = 0;
    u32 *doorbell = NULL;
    u8 *addr = NULL;

    pdata = dev_info->pdata;
    if (irq_num == DEVDRV_MAILBOX_SEND_OFFLINE_IRQ) {
        addr = pdata->ts_pdata[tsid].doorbell_vaddr;
        addr = addr + ((unsigned long)(DEVDRV_TS_DOORBELL_OFFSET1) * DEVDRV_TS_DOORBELL_STRIDE);
        doorbell = (u32 *)addr;
        *doorbell = value;  // 0x3A;
        value++;
    } else if (irq_num == DEVDRV_MAILBOX_SEND_ONLINE_IRQ) {
        addr = pdata->ts_pdata[tsid].doorbell_vaddr;
        addr = addr + ((unsigned long)(DEVDRV_TS_DOORBELL_OFFSET1 - DEVDRV_TS_DOORBELL_ONLINE_OFFSET) *
                        DEVDRV_TS_DOORBELL_STRIDE);
        doorbell = (u32 *)addr;
        *doorbell = value;  // 0x3A;
        value++;
    } else if (irq_num == DEVDRV_MAILBOX_READ_DONE_IRQ) {
        addr = pdata->ts_pdata[tsid].doorbell_vaddr;
        addr = addr + ((unsigned long)(DEVDRV_TS_DOORBELL_OFFSET2) * DEVDRV_TS_DOORBELL_STRIDE);
        doorbell = (u32 *)addr;
        *doorbell = read_done_value;  // 0x3A;
        read_done_value++;
    }
    mb();
}

STATIC void devdrv_manager_host_flush_cache(u64 base, u32 len)
{
}

STATIC int devdrv_memcpy_to_device_sq(struct devdrv_info *dev_info, u64 dst, u64 src, u32 buf_len)
{
    int ret;

    ret = devdrv_dma_sync_copy(dev_info->dev_id, DEVDRV_DMA_DATA_COMMON, src, dst, buf_len, DEVDRV_DMA_HOST_TO_DEVICE);
    if (ret) {
        devdrv_drv_err("devdrv_dma_sync_copy failed, ret(%d). dev_id(%u)\n", ret, dev_info->dev_id);
        return -ENOMEM;
    }

    return 0;
}

struct devdrv_drv_ops devdrv_host_drv_ops = {
    .alloc_mem = NULL,  // devdrv_manager_alloc_device_mem,
    .free_mem = NULL,   // devdrv_manager_free_device_mem,
    .svm_va_to_devid = NULL,
    .flush_cache = devdrv_manager_host_flush_cache,
    .irq_trigger = devdrv_manager_host_irq_trigger,
    .get_stream_sync = devdrv_manager_msg_h2d_alloc_sync_stream,
    .get_event_sync = devdrv_manager_msg_h2d_alloc_sync_event,
    .get_model_sync = devdrv_manager_msg_h2d_alloc_sync_model,
    .get_notify_sync = devdrv_manager_msg_h2d_alloc_sync_notify,
    .get_task_sync = devdrv_manager_msg_h2d_alloc_sync_task,
    .get_sq_sync = devdrv_manager_msg_h2d_alloc_sync_sq,
    .get_cq_sync = devdrv_manager_msg_h2d_alloc_sync_cq,
    .ipc_msg_send_sync = NULL,
    .ipc_msg_rx_register = NULL,
    .ipc_msg_rx_unregister = NULL,
    .ipc_msg_write = NULL,
    .ipc_msg_read = NULL,
    .ipc_msg_stop_thread = NULL,
    .get_dev_info = devdrv_manager_h2d_sync_get_devinfo,
    .memcpy_to_device_sq = devdrv_memcpy_to_device_sq,
};
