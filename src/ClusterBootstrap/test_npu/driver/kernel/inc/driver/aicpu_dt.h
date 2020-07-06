/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2012-2019. All rights reserved.
 * Description:
 * Author: huawei 
 * Create: 2019-10-15
 */
#ifndef __AICPU_DT_H__
#define __AICPU_DT_H__

#include "aicpu.h"

#define DT_NAME_MAX_SIZE 128
#define DT_PLAT_DEV_MAX_NUM 12
struct dt_machine_desc {
    unsigned long long magic;   /* machine.magic, same with devdrv.falg */
    unsigned long long dev_num; /* devGrp's dev num */
    unsigned long long chip_id;
    unsigned long long chip_type;
    unsigned long long chip_version;
    char name[DT_NAME_MAX_SIZE]; /* machine.name */
};

union dt_machine_desc_un {
    struct dt_machine_desc machine_desc;
    char dt_machine_desc_array[MACHINE_DESC_SIZE];
};

struct dt_dev_drv_desc {
    struct aicpu_system_config system_config;
};

union dt_dev_drv_desc_un {
    struct dt_dev_drv_desc dev_drv_desc;
    char dt_dev_drv_desc_array[DEV_DRV_DESC_SIZE];
};

// //////////////// types.h //////////////////////////
#define aicpu_phys_addr_t unsigned long long
typedef aicpu_phys_addr_t aicpu_resource_size_t;

// //////////////// ioport.h //////////////////////////
/*
 * Resources are tree-like, allowing
 * nesting etc..
 */
struct aicpu_resource {
    aicpu_resource_size_t start;
    aicpu_resource_size_t end;
    unsigned long flags;
    unsigned long desc;
};

#define DT_DEV_NAME_MAX_SIZE 32
#define DT_DEV_RES_MAX_NUM 3
#define DT_DEV_DATA_MAX_NUM 128
struct aicpu_platform_device_info {
    unsigned long long num_res;
    unsigned long long data_size;
    char name[DT_DEV_NAME_MAX_SIZE];
    struct aicpu_resource res[DT_DEV_RES_MAX_NUM];
    char data[DT_DEV_DATA_MAX_NUM];
};

struct dt_platform_device_info_desc {
    struct aicpu_platform_device_info plat_dev_info_grp[DT_PLAT_DEV_MAX_NUM];
};

union dt_platform_device_info_desc_un {
    struct dt_platform_device_info_desc platform_device_info_desc;
    char dt_platform_device_info_desc_array[DEV_GRP_DESC_SIZE];
};

struct dt_desc {
    union dt_dev_drv_desc_un dev_drv_desc_un;
    union dt_machine_desc_un machine_desc_un;
    union dt_platform_device_info_desc_un platform_device_info_desc_un;
};

// /////////////////////////// arch_timer ////////////////////////
#define TIMER_TASK 0
#define TIMER_PMU 1
#define TIMER_MAX_NUM 2

// /////////////////////////// arch_pmu ////////////////////////
#define PMU_IRQ_INDEX 0
#define PMU_IRQ_NUM 1

// /////////////////////////// gic_common ////////////////////////
#define GIC_MEM_SEG_INDEX 0
#define GIC_MEM_SEG_NUM 1

typedef enum {
    AICPU_TS_CTRL_INTR_SPI = 0,
    AICPU_TS_CTRL_INTR_LPI = 1,
    AICPU_TS_CTRL_INTR_INVALID = 0xFF,
} AICPU_TS_CTRL_INTR_TYPE;

struct aicpu_ts_ctrl_intr_lpi {
    unsigned int device_id;
    unsigned int event_id;
    unsigned long long its_pa_base;
};

struct aicpu_ts_ctrl_intr_spi {
    unsigned int ts_int_start_id;
    unsigned int ctrl_cpu_int_start_id;
};

union aicpu_ts_ctrl_intr_desc {
    struct aicpu_ts_ctrl_intr_lpi lpi;
    struct aicpu_ts_ctrl_intr_spi spi;
};

struct aicpu_ts_ctrl_intr {
    unsigned int intr_type;
    union aicpu_ts_ctrl_intr_desc intr_desc;
};

// /////////////////////////// ipc_mailbox ////////////////////////
#define IPC_IRQ_INDEX 0
#define IPC_MB_IRQ_INDEX 1
#define IPC_IRQ_NUM 2

#define IPC_MEM_SEG_INDEX 0
#define IPC_MEM_SEG_NUM 1

struct ipc_cpu_id {
    int ipc_cpu_id_ts;
    int ipc_cpu_id_acpu;
    int ipc_cpu_id_aicpu0;
    int ipc_aicpu_cpu_id_base;
};

// /////////////////////////// power_manager ////////////////////////
#define SOC_CPU_SUBCTRL_MEM_SEG_INDEX 0
#define SOC_CPU_SUBCTRL_MEM_SEG_NUM 1

struct sr_param {
    unsigned char wfe_wfi_retation;
    unsigned char l3_retation;
};

#define DT_INVALID_IRQ_ID (0xffffffffULL)
#define DT_INVALID_PA_BASE (0xffffffffffffffffULL)

#define DT_INVALID_ZERO_CASE (0x00ULL)

#endif
