/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2012-2019. All rights reserved.
 * Description: Host Msg
 * Author: Huawei Technologies Co., Ltd.
 * Create: 2018/8/18
 */
#include "apb_comm_drv.h"
#include "resource_comm_drv.h"

u32 g_res_func_total_pre_dev = 1;

u32 agentdrv_res_get_func_total(void)
{
    return g_res_func_total_pre_dev;
}

void agentdrv_res_set_func_total(u32 totl_num)
{
    g_res_func_total_pre_dev = totl_num;
}

void agentdrv_res_init_func_total_num(void)
{
    u32 func_totl_num;

    func_totl_num = agentdrv_soc_get_func_total();
    agentdrv_res_set_func_total(func_totl_num);
}
