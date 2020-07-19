/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2012-2019. All rights reserved.
 * Description: This file contains api definition for memory manager driver.
 * Author: huawei
 * Create: 2020-03-09
 */

#ifndef __DEVMM_COMMON_H__
#define __DEVMM_COMMON_H__

#include "devmm_proc_info.h"

#ifndef __GFP_ACCOUNT

#ifdef __GFP_KMEMCG
#define __GFP_ACCOUNT __GFP_KMEMCG /* for linux version 3.10 */
#endif

#ifdef __GFP_NOACCOUNT
#define __GFP_ACCOUNT 0 /* for linux version 4.1 */
#endif

#endif

u64 devmm_get_pagecount_by_size(u64 vptr, u64 sizes, u32 page_size);
int devmm_check_va_add_size_by_heap(struct devmm_svm_heap *heap, u64 va, u64 size);
int devmm_check_input_heap_info(u32 heap_idx, u32 op, u32 heap_type, u32 page_size);
unsigned long devmm_get_addr_device_page_size(u32 hpid, u64 va);
char *devmm_zalloc(u64 size, int *vmalloc_flag);
void devmm_free(const char *ptr, int vmalloc_flag);

#endif /* __DEVMM_COMMON_H__ */
