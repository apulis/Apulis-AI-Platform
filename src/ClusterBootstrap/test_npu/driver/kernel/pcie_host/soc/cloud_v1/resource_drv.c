/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2012-2019. All rights reserved.
 * Description:
 * Author: Huawei Technologies Co., Ltd.
 * Create: 2018/8/18
 */
#include "devdrv_msg_def.h"
#include "dma_drv.h"
#include "resource_drv.h"

void devdrv_dma_res_chan(u32 func_id, u32 dma_irq_base, u32 dma_irq_num, struct devdrv_res_dma_chan_info *out)
{
    out->chan_num = DMA_CHAN_REMOTE_USED_NUM;
    out->chan_begin = DMA_CHAN_REMOTE_USED_START_INDEX;
    out->done_irq_base = dma_irq_base;
    out->err_irq_base = dma_irq_base + dma_irq_num / DEVDRV_EACH_DMA_IRQ_NUM;
    /* host always buid err. */
    out->err_flag = 1;
}
