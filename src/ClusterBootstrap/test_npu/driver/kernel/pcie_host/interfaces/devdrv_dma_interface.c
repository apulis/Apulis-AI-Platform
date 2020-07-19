/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2012-2019. All rights reserved.
 * Description: DMA interfaces
 * Author: Huawei Technologies Co., Ltd.
 * Create: 2018/8/18
 */
#include <linux/types.h>
#include <linux/wait.h>

#include "devdrv_interface.h"
#include "devdrv_dma_interface.h"
#include "devdrv_util.h"
#include "dma_drv.h"
#include "devdrv_atu.h"
#include "devdrv_dma.h"

int devdrv_dma_sync_copy_plus(u32 dev_id, enum devdrv_dma_data_type type, int instance, u64 src, u64 dst, u32 size,
                              enum devdrv_dma_direction direction)
{
    int ret;
    struct devdrv_dma_node dma_node;
    struct devdrv_dma_dev *dma_dev = NULL;

    dma_node.src_addr = src;
    dma_node.dst_addr = dst;
    dma_node.size = size;
    dma_node.direction = direction;

    ret = devdrv_dma_para_check(dev_id, type, &dma_node, 1, DEVDRV_DMA_SYNC, NULL);
    if (ret) {
        devdrv_err("devid %d devdrv_dma_sync_copy, para check failed\n", dev_id);
        return ret;
    }
    dma_dev = devdrv_get_dma_dev(dev_id);
    if (dma_dev == NULL) {
        devdrv_err("dma_dev(dev_id %u) is NULL\n", dev_id);
        return -EINVAL;
    }

    ret = devdrv_dma_copy(dma_dev, type, instance, &dma_node, 1, DEVDRV_DMA_WAIT_INTR, DEVDRV_DMA_SYNC, NULL);

    return ret;
}
EXPORT_SYMBOL(devdrv_dma_sync_copy_plus);

int devdrv_dma_sync_copy(u32 dev_id, enum devdrv_dma_data_type type, u64 src, u64 dst, u32 size,
                         enum devdrv_dma_direction direction)
{
    return devdrv_dma_sync_copy_plus(dev_id, type, DEVDRV_INVALID_INSTANCE, src, dst, size, direction);
}
EXPORT_SYMBOL(devdrv_dma_sync_copy);

int devdrv_dma_async_copy_plus(u32 dev_id, enum devdrv_dma_data_type type, int instance, u64 src, u64 dst, u32 size,
                               enum devdrv_dma_direction direction, struct devdrv_asyn_dma_para_info *para_info)
{
    int ret;
    struct devdrv_dma_node dma_node;
    struct devdrv_dma_dev *dma_dev = NULL;

    dma_node.src_addr = src;
    dma_node.dst_addr = dst;
    dma_node.size = size;
    dma_node.direction = direction;

    ret = devdrv_dma_para_check(dev_id, type, &dma_node, 1, DEVDRV_DMA_ASYNC, para_info);
    if (ret) {
        devdrv_err("devid %d devdrv_dma_async_copy, para check failed\n", dev_id);
        return ret;
    }

    dma_dev = devdrv_get_dma_dev(dev_id);
    if (dma_dev == NULL) {
        devdrv_err("dma_dev(dev_id %u) is NULL\n", dev_id);
        return -EINVAL;
    }

    ret = devdrv_dma_copy(dma_dev, type, instance, &dma_node, 1, DEVDRV_DMA_WAIT_INTR, DEVDRV_DMA_ASYNC, para_info);

    return ret;
}
EXPORT_SYMBOL(devdrv_dma_async_copy_plus);

int devdrv_dma_async_copy(u32 dev_id, enum devdrv_dma_data_type type, u64 src, u64 dst, u32 size,
                          enum devdrv_dma_direction direction, struct devdrv_asyn_dma_para_info *para_info)
{
    return devdrv_dma_async_copy_plus(dev_id, type, DEVDRV_INVALID_INSTANCE, src, dst, size, direction, para_info);
}
EXPORT_SYMBOL(devdrv_dma_async_copy);

int devdrv_dma_sync_link_copy_plus(u32 dev_id, enum devdrv_dma_data_type type, int wait_type, int instance,
                                   struct devdrv_dma_node *dma_node, u32 node_cnt)
{
    int ret;
    struct devdrv_dma_dev *dma_dev = NULL;

    ret = devdrv_dma_para_check(dev_id, type, dma_node, node_cnt, DEVDRV_DMA_SYNC, NULL);
    if (ret) {
        devdrv_err("devid %d devdrv_dma_sync_link_copy, para check failed\n", dev_id);
        return ret;
    }

    dma_dev = devdrv_get_dma_dev(dev_id);
    if (dma_dev == NULL) {
        devdrv_err("dma_dev(dev_id %u) is NULL\n", dev_id);
        return -EINVAL;
    }

    ret = devdrv_dma_copy(dma_dev, type, instance, dma_node, node_cnt, wait_type, DEVDRV_DMA_SYNC, NULL);

    return ret;
}
EXPORT_SYMBOL(devdrv_dma_sync_link_copy_plus);

int devdrv_dma_sync_link_copy(u32 dev_id, enum devdrv_dma_data_type type, int wait_type,
                              struct devdrv_dma_node *dma_node, u32 node_cnt)
{
    return devdrv_dma_sync_link_copy_plus(dev_id, type, wait_type, DEVDRV_INVALID_INSTANCE, dma_node, node_cnt);
}
EXPORT_SYMBOL(devdrv_dma_sync_link_copy);

int devdrv_dma_async_link_copy_plus(u32 dev_id, enum devdrv_dma_data_type type, int instance,
                                    struct devdrv_dma_node *dma_node, u32 node_cnt,
                                    struct devdrv_asyn_dma_para_info *para_info)
{
    int ret;
    struct devdrv_dma_dev *dma_dev = NULL;

    ret = devdrv_dma_para_check(dev_id, type, dma_node, node_cnt, DEVDRV_DMA_ASYNC, para_info);
    if (ret) {
        devdrv_err("devid %d devdrv_dma_async_link_copy, para check failed\n", dev_id);
        return ret;
    }

    dma_dev = devdrv_get_dma_dev(dev_id);
    if (dma_dev == NULL) {
        devdrv_err("dma_dev(dev_id %u) is NULL\n", dev_id);
        return -EINVAL;
    }

    ret = devdrv_dma_copy(dma_dev, type, instance, dma_node, node_cnt,
        DEVDRV_DMA_WAIT_INTR, DEVDRV_DMA_ASYNC, para_info);

    return ret;
}
EXPORT_SYMBOL(devdrv_dma_async_link_copy_plus);

int devdrv_dma_async_link_copy(u32 dev_id, enum devdrv_dma_data_type type, struct devdrv_dma_node *dma_node,
                               u32 node_cnt, struct devdrv_asyn_dma_para_info *para_info)
{
    return devdrv_dma_async_link_copy_plus(dev_id, type, DEVDRV_INVALID_INSTANCE, dma_node, node_cnt, para_info);
}
EXPORT_SYMBOL(devdrv_dma_async_link_copy);
