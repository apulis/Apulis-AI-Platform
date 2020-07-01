/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2012-2019. All rights reserved.
 * Description:
 * Author: Huawei Technologies Co., Ltd.
 * Create: 2018/8/18
 */
#ifndef __RESOURCE_COMM_DRV_H__
#define __RESOURCE_COMM_DRV_H__

#include <asm/io.h>

#define DEVDRV_EACH_DMA_IRQ_NUM 2

struct devdrv_res_dma_chan_info {
    u32 chan_begin;
    u32 chan_num;
    u32 done_irq_base;
    u32 err_irq_base;
    u32 err_flag;
};

struct agent_res_sdi_addr {
    u64 msg_size;
    phys_addr_t msg_base;
    u64 db_size;
    phys_addr_t db_base;
    u64 shr_size;
    phys_addr_t shr_base;
};

void agentdrv_res_init_func_total_num(void);
u32 agentdrv_res_get_func_total(void);
void agentdrv_res_set_func_total(u32 totl_num);
void agentdrv_dma_res_chan(u32 func_id, u32 dma_irq_base, struct devdrv_res_dma_chan_info *out);
void devdrv_dma_res_chan(u32 func_id, u32 dma_irq_base, u32 dma_irq_num, struct devdrv_res_dma_chan_info *out);
void agentdrv_res_sdi_addr(u32 dev_id, u32 func_id, struct agent_res_sdi_addr *sdi_addr);

#endif /* __RESOURCE_COMM_DRV_H__ */
