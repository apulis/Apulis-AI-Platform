/**
* @file devdrv_dfm.h
*
* Copyright (c) Huawei Technologies Co., Ltd. 2012-2019. All rights reserved.
* Description: This file contains api declaration for Device Driver and Device Management
* Author: huawei
* Create: 2019-10-15
*
* @brief devdrv_dfm interface
*/
#ifndef __DEVDRV_DFM_H__
#define __DEVDRV_DFM_H__

#include <linux/mntn/rdr_pub.h>

#define DFM_OK 0
#define DFM_ERROR 1

#define DFM_MODULE_ID_DRIVER 1
#define DFM_MODULE_ID_DVPP 10

#define DFM_SUBMODULE_ID_DEVMNG 0  /* 设备管理 */
#define DFM_SUBMODULE_ID_PCIE 1    /* PCIE */
#define DFM_SUBMODULE_ID_USB 2     /* USB */
#define DFM_SUBMODULE_ID_NET 3     /* 网络相关，RGMII/MAC/ETH */
#define DFM_SUBMODULE_ID_AICPU 4   /* AICPU DRV，RGMII/MAC/ETH */
#define DFM_SUBMODULE_ID_APPMOND 5 /* APPMOND */
#define DFM_SUBMODULE_ID_UPGRADE 6 /* UPGRADE */

#define DFM_SUBMODULE_ID_END 7

#define FW_SYNC_FAIL 0xA2020000

#define DVPP_JPEGD_IRQ_FAIL 0xa6140001
#define DVPP_JPEGE_IRQ_FAIL 0xa6140002
#define DVPP_VPC_IRQ_FAIL 0xa6140003
#define DVPP_PNGD_IRQ_FAIL 0xa6140004
#define DVPP_VDEC_IRQ_FAIL 0xa6140005
#define DVPP_VENC_IRQ_FAIL 0xa6140006
#define DVPP_JPEGD_PROC_FAIL 0xa2140007
#define DVPP_JPEGE_PROC_FAIL 0xa2140008
#define DVPP_VPC_PROC_FAIL 0xa2140009
#define DVPP_PNGD_PROC_FAIL 0xa214000a
#define DVPP_VDEC_PROC_FAIL 0xa214000b
#define DVPP_VENC_PROC_FAIL 0xa214000c
#define DVPP_CMD_LIST_PROC_FAIL 0xa214000d
#define DVPP_BUS_ERROR 0xa214000e

#define DFM_AGENTDRV_DMA_PROBE_FAIL 0xA8021000
#define DFM_AGENTDRV_SDIO_PROBE_FAIL 0xA8021001
#define DFM_AGENTDRV_PM_PROBE_FAIL 0xA8021002
#define DFM_AGENTDRV_LINKDOWN_FAIL 0xA8021003

#define DFM_HIGMAC_E_TX_TIMEOUT 0xA6023001
#define DFM_HIGMAC_E_RX_INCONST 0xA6023002
#define DFM_HIGMAC_E_TX_INCONST 0xA6023003
#define DFM_HIGMAC_E_TX_BD_ERROR 0xA6023004

#define AICPU_DRV_INIT_FAIL_CODE (0xA8024001)
#define AICPU_DRV_RUN_FAIL_CODE (0xA8024002)

#define DFM_EXECP_ID_USB_TRB 0xA6022001

#define DFM_AGENTDRV_DMA_FAIL 0xA8021004

#define APPMON_IDLE_RECOVERY 0x96025021
#define APPMON_IDLE_HEART_BEAT_LOST 0xA6025021
#define APPMON_IDLE_RAISE_FAIL 0xA8025022
#define APPMON_SLOGD_RECOVERY 0x96025041
#define APPMON_SLOGD_HEART_BEAT_LOST 0xA6025041
#define APPMON_SLOGD_RAISE_FAIL 0xA8025042
#define APPMON_SKLOGD_RECOVERY 0x96025061
#define APPMON_SKLOGD_HEART_BEAT_LOST 0xA6025061
#define APPMON_SKLOGD_RAISE_FAIL 0xA8025062
#define APPMON_DMP_RECOVERY 0x96025081
#define APPMON_DMP_HEART_BEAT_LOST 0xA6025081
#define APPMON_DMP_RAISE_FAIL 0xA8025082
#define APPMON_TSDAEMON_RECOVERY 0x960250A1
#define APPMON_TSDAEMON_HEART_BEAT_LOST 0xA60250A1
#define APPMON_TSDAEMON_RAISE_FAIL 0xA80250A2
#define APPMON_DEVMM_RECOVERY 0x960250C1
#define APPMON_DEVMM_HEART_BEAT_LOST 0xA60250C1
#define APPMON_DEVMM_RAISE_FAIL 0xA80250C2

#define DFM_MAX_DAVINCI_NUM 4


// dvpp recover excep id

#define CLR_EXCEP_TYPE 0xCFFFFFFF
#define SET_RECOVER_TYPE 0x10000000


#define DVPP_JPEGD_PROC_RECOVER_ID ((DVPP_JPEGD_PROC_FAIL & CLR_EXCEP_TYPE) | SET_RECOVER_TYPE)
#define DVPP_JPEGE_PROC_RECOVER_ID ((DVPP_JPEGE_PROC_FAIL & CLR_EXCEP_TYPE) | SET_RECOVER_TYPE)
#define DVPP_VPC_PROC_RECOVER_ID ((DVPP_VPC_PROC_FAIL & CLR_EXCEP_TYPE) | SET_RECOVER_TYPE)
#define DVPP_PNGD_PROC_RECOVER_ID ((DVPP_PNGD_PROC_FAIL & CLR_EXCEP_TYPE) | SET_RECOVER_TYPE)
#define DVPP_VDEC_PROC_RECOVER_ID ((DVPP_VDEC_PROC_FAIL & CLR_EXCEP_TYPE) | SET_RECOVER_TYPE)
#define DVPP_VENC_PROC_RECOVER_ID ((DVPP_VENC_PROC_FAIL & CLR_EXCEP_TYPE) | SET_RECOVER_TYPE)
#define DVPP_CMD_LIST_PROC_RECOVER_ID ((DVPP_CMD_LIST_PROC_FAIL & CLR_EXCEP_TYPE) | SET_RECOVER_TYPE)

typedef void (*dfm_dump)(u32 dev_id, u32 excep_id, u32 etype, u32 module_id, char **black_box_info);
typedef void (*dfm_dump_done)(u32 dev_id, u32 excep_id, u32 etype, u32 module_id);

int dfm_stub_print(u32 dev_id, u32 module_id, u32 type);
int dfm_write_black_box(u32 dev_id, u32 module_id, u32 sub_module_id, const char *black_box_info);
void dfm_system_error_report(u32 dev_id, u32 excep_id, u32 arg);

struct dfm_module_register {
    unsigned int module_id;     /* 模块id */
    unsigned int sub_module_id; /* 子模块id */
    dfm_dump ops_dump;          /* dump函数指针 */
};

struct dfm_exception_info {
    unsigned int excep_id;         /* 异常id */
    unsigned int module_id;        /* 模块id */
    unsigned int exce_type;        /* 异常类型 */
    unsigned int process_priority; /* 异常级别 RDR_ERR/RDR_WARN/RDR_OTHER */
    unsigned int reboot_priority;  /* 重启优先级 RDR_REBOOT_NOW/RDR_REBOOT_WAIT/RDR_REBOOT_NO */
    char *module_name;             /* 模块名 */
    char *desc;                    /* 异常描述 */
};

int dfm_register_module(struct dfm_module_register *dfm_module_info);
int dfm_unregister_module(unsigned int module_id, unsigned int sub_module_id);

void dfm_reset(unsigned int excep_id, unsigned char etype, unsigned char module_id);

int dfm_init(void);
void dfm_exit(void);

#define dfm_printk(level, module, fmt, ...) \
    printk(level " [%s] [%s %d] " fmt, module, __func__, __LINE__, ##__VA_ARGS__)

#define module_dfm "dfm"

#define dfm_err(fmt...) dfm_printk("err", module_dfm, fmt)
#define dfm_warn(fmt...) dfm_printk("warn", module_dfm, fmt)
#define dfm_inf(fmt...) dfm_printk("info", module_dfm, fmt)
#define dfm_debug(fmt...) dfm_printk("debug", module_dfm, fmt)

#endif
