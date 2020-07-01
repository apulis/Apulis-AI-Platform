/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2012-2019. All rights reserved.
 * Description: This file contains api definition for memory manager driver.
 * Author: huawei
 * Create: 2019-06-11
 */

#ifndef __DEVMM_PROC_RMDA_H__
#define __DEVMM_PROC_RMDA_H__

#include "devmm_proc_info.h"

int devmm_mem_acquire_proc(u64 addr, u64 size, u32 hostpid, const char *pid_sign, u32 sign_len);
int devmm_mem_get_pages_proc(u64 addr, u64 size, u32 hostpid,
    struct devmm_dma_block *blks, u32 blks_num);
void devmm_mem_put_pages_proc(u64 va, u32 hostpid, struct devmm_dma_block *blks, u32 blk_num);

#endif /* __DEVMM_PROC_RMDA_H__ */
