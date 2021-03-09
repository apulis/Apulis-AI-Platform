/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2012-2019. All rights reserved.
 * Description:
 * Author: huawei
 * Create: 2019-10-15
 */
#ifndef __DEVDRV_MANAGER_COMMON_H
#define __DEVDRV_MANAGER_COMMON_H

#include <linux/list.h>
#include <linux/cdev.h>
#include <linux/mutex.h>
#include <linux/types.h>
#include <linux/uio_driver.h>
#include <linux/notifier.h>
#include <linux/radix-tree.h>

#include "devdrv_common.h"
#include "devdrv_platform_resource.h"
#include "devdrv_manager_comm.h"

#define PCI_VENDOR_ID_HUAWEI 0x19e5

/* manager */
#define DEVDRV_MANAGER_MAGIC 'M'
#define DEVDRV_MANAGER_GET_PCIINFO _IO(DEVDRV_MANAGER_MAGIC, 1)
#define DEVDRV_MANAGER_GET_DEVNUM _IO(DEVDRV_MANAGER_MAGIC, 2)
#define DEVDRV_MANAGER_GET_PLATINFO _IO(DEVDRV_MANAGER_MAGIC, 3)
#define DEVDRV_MANAGER_SVMVA_TO_DEVID _IO(DEVDRV_MANAGER_MAGIC, 4)
#define DEVDRV_MANAGER_DEVICE_STATUS _IO(DEVDRV_MANAGER_MAGIC, 7)
#define DEVDRV_MANAGER_GET_CORE_SPEC _IO(DEVDRV_MANAGER_MAGIC, 17)
#define DEVDRV_MANAGER_GET_CORE_INUSE _IO(DEVDRV_MANAGER_MAGIC, 18)
#define DEVDRV_MANAGER_GET_DEVIDS _IO(DEVDRV_MANAGER_MAGIC, 19)
#define DEVDRV_MANAGER_GET_DEVINFO _IO(DEVDRV_MANAGER_MAGIC, 20)
#define DEVDRV_MANAGER_GET_PCIE_ID_INFO _IO(DEVDRV_MANAGER_MAGIC, 21)
#define DEVDRV_MANAGER_GET_FLASH_COUNT _IO(DEVDRV_MANAGER_MAGIC, 22)
#define DEVDRV_MANAGER_GET_VOLTAGE _IO(DEVDRV_MANAGER_MAGIC, 24)
#define DEVDRV_MANAGER_GET_TEMPERATURE _IO(DEVDRV_MANAGER_MAGIC, 25)
#define DEVDRV_MANAGER_GET_AI_USE_RATE _IO(DEVDRV_MANAGER_MAGIC, 26)
#define DEVDRV_MANAGER_GET_FREQUENCY _IO(DEVDRV_MANAGER_MAGIC, 27)
#define DEVDRV_MANAGER_GET_POWER _IO(DEVDRV_MANAGER_MAGIC, 28)
#define DEVDRV_MANAGER_GET_HEALTH_CODE _IO(DEVDRV_MANAGER_MAGIC, 29)
#define DEVDRV_MANAGER_GET_ERROR_CODE _IO(DEVDRV_MANAGER_MAGIC, 30)
#define DEVDRV_MANAGER_GET_DDR_CAPACITY _IO(DEVDRV_MANAGER_MAGIC, 31)
#define DEVDRV_MANAGER_LPM3_SMOKE _IO(DEVDRV_MANAGER_MAGIC, 32)
#define DEVDRV_MANAGER_BLACK_BOX_GET_EXCEPTION _IO(DEVDRV_MANAGER_MAGIC, 33)
#define DEVDRV_MANAGER_DEVICE_MEMORY_DUMP _IO(DEVDRV_MANAGER_MAGIC, 34)
#define DEVDRV_MANAGER_DEVICE_RESET_INFORM _IO(DEVDRV_MANAGER_MAGIC, 35)
#define DEVDRV_MANAGER_GET_MODULE_STATUS _IO(DEVDRV_MANAGER_MAGIC, 36)
#define DEVDRV_MANAGER_GET_DEVICE_DMA_ADDR _IO(DEVDRV_MANAGER_MAGIC, 37)
#define DEVDRV_MANAGER_GET_INTERRUPT_INFO _IO(DEVDRV_MANAGER_MAGIC, 38)
#define DEVDRV_MANAGER_INTERRUPT_RECV_SIGNAL _IO(DEVDRV_MANAGER_MAGIC, 39)
#define DEVDRV_MANAGER_GET_MINI_BOARD_ID _IO(DEVDRV_MANAGER_MAGIC, 43)
#define DEVDRV_MANAGER_PCIE_PRE_RESET _IO(DEVDRV_MANAGER_MAGIC, 44)
#define DEVDRV_MANAGER_PCIE_RESCAN _IO(DEVDRV_MANAGER_MAGIC, 45)
#define DEVDRV_MANAGER_ALLOC_HOST_DMA_ADDR _IO(DEVDRV_MANAGER_MAGIC, 46)
#define DEVDRV_MANAGER_FREE_HOST_DMA_ADDR _IO(DEVDRV_MANAGER_MAGIC, 47)
#define DEVDRV_MANAGER_PCIE_SRAM_READ _IO(DEVDRV_MANAGER_MAGIC, 48)
#define DEVDRV_MANAGER_PCIE_SRAM_WRITE _IO(DEVDRV_MANAGER_MAGIC, 49)
#define DEVDRV_MANAGER_GET_USER_CONFIG _IO(DEVDRV_MANAGER_MAGIC, 50)
#define DEVDRV_MANAGER_GET_EMMC_VOLTAGE _IO(DEVDRV_MANAGER_MAGIC, 51)
#define DEVDRV_MANAGER_GET_DEVICE_BOOT_STATUS _IO(DEVDRV_MANAGER_MAGIC, 52)
#define DEVDRV_MANAGER_ENABLE_EFUSE_LDO _IO(DEVDRV_MANAGER_MAGIC, 53)
#define DEVDRV_MANAGER_DISABLE_EFUSE_LDO _IO(DEVDRV_MANAGER_MAGIC, 54)
#define DEVDRV_MANAGER_GET_TSENSOR _IO(DEVDRV_MANAGER_MAGIC, 55)
#define DEVDRV_MANAGER_CHECK_CANCEL_STATUS _IO(DEVDRV_MANAGER_MAGIC, 56)
#define DEVDRV_MANAGER_DFX_CMD _IO(DEVDRV_MANAGER_MAGIC, 57)
#define DEVDRV_MANAGER_CONTAINER_CMD _IO(DEVDRV_MANAGER_MAGIC, 58)
#define DEVDRV_MANAGER_GET_HOST_PHY_MACH_FLAG _IO(DEVDRV_MANAGER_MAGIC, 59)
#define DEVDRV_MANAGER_LOAD_KERNEL_LIB _IO(DEVDRV_MANAGER_MAGIC, 60)
#define DEVDRV_MANAGER_GET_KERNEL_LIB _IO(DEVDRV_MANAGER_MAGIC, 61)
#define DEVDRV_MANAGER_GET_LOCAL_DEVICEIDS _IO(DEVDRV_MANAGER_MAGIC, 62)
#define DEVDRV_MANAGER_IMU_SMOKE _IO(DEVDRV_MANAGER_MAGIC, 63)
#define DEVDRV_MANAGER_IPC_NOTIFY_CREATE _IO(DEVDRV_MANAGER_MAGIC, 65)
#define DEVDRV_MANAGER_IPC_NOTIFY_OPEN _IO(DEVDRV_MANAGER_MAGIC, 66)
#define DEVDRV_MANAGER_IPC_NOTIFY_CLOSE _IO(DEVDRV_MANAGER_MAGIC, 67)
#define DEVDRV_MANAGER_IPC_NOTIFY_DESTROY _IO(DEVDRV_MANAGER_MAGIC, 68)
#define DEVDRV_MANAGER_SET_NEW_TIME _IO(DEVDRV_MANAGER_MAGIC, 69)
#define DEVDRV_MANAGER_PCIE_DDR_READ _IO(DEVDRV_MANAGER_MAGIC, 71)
#define DEVDRV_MANAGER_GET_TRANSWAY _IO(DEVDRV_MANAGER_MAGIC, 73)
#define DEVDRV_MANAGER_GET_CPU_INFO _IO(DEVDRV_MANAGER_MAGIC, 74)
#define DEVDRV_MANAGER_SEND_TO_IMU _IO(DEVDRV_MANAGER_MAGIC, 76)
#define DEVDRV_MANAGER_RECV_FROM_IMU _IO(DEVDRV_MANAGER_MAGIC, 77)
#define DEVDRV_MANAGER_GET_HBM_CAPACITY _IO(DEVDRV_MANAGER_MAGIC, 78)
#define DEVDRV_MANAGER_GET_DDR_BW_UTILIZATION _IO(DEVDRV_MANAGER_MAGIC, 79)
#define DEVDRV_MANAGER_GET_HBM_BW_UTILIZATION _IO(DEVDRV_MANAGER_MAGIC, 80)
#define DEVDRV_MANAGER_GET_IMU_INFO _IO(DEVDRV_MANAGER_MAGIC, 81)
#define DEVDRV_MANAGER_GET_ECC_STATICS _IO(DEVDRV_MANAGER_MAGIC, 82)
#define DEVDRV_MANAGER_CONFIG_ECC_ENABLE _IO(DEVDRV_MANAGER_MAGIC, 83)
#define DEVDRV_MANAGER_GET_DDR_UTILIZATION _IO(DEVDRV_MANAGER_MAGIC, 84)
#define DEVDRV_MANAGER_GET_HBM_UTILIZATION _IO(DEVDRV_MANAGER_MAGIC, 85)
#define DEVDRV_MANAGER_GET_PMU_VOLTAGE _IO(DEVDRV_MANAGER_MAGIC, 86)
#define DEVDRV_MANAGER_GET_BOOT_DEV_ID _IO(DEVDRV_MANAGER_MAGIC, 87)
#define DEVDRV_MANAGER_CFG_DDR_STAT_INFO _IO(DEVDRV_MANAGER_MAGIC, 88)
#define DEVDRV_MANAGER_GET_PMU_DIEID _IO(DEVDRV_MANAGER_MAGIC, 89)
#define DEVDRV_MANAGER_GET_PROBE_NUM _IO(DEVDRV_MANAGER_MAGIC, 90)
#define DEVDRV_MANAGER_DEBUG_INFORM _IO(DEVDRV_MANAGER_MAGIC, 91)
#define DEVDRV_MANAGER_COMPUTE_POWER _IO(DEVDRV_MANAGER_MAGIC, 92)
#define DEVDRV_MANAGER_GET_DEVID_BY_LOCALDEVID _IO(DEVDRV_MANAGER_MAGIC, 93)
#define DEVDRV_MANAGER_SYNC_MATRIX_DAEMON_READY _IO(DEVDRV_MANAGER_MAGIC, 94)
#define DEVDRV_MANAGER_GET_CONTAINER_DEVIDS _IO(DEVDRV_MANAGER_MAGIC, 95)
#define DEVDRV_MANAGER_GET_BBOX_ERRSTR _IO(DEVDRV_MANAGER_MAGIC, 97)
#define DEVDRV_MANAGER_GET_LLC_PERF_PARA _IO(DEVDRV_MANAGER_MAGIC, 98)
#define DEVDRV_MANAGER_PCIE_IMU_DDR_READ _IO(DEVDRV_MANAGER_MAGIC, 99)
#define DEVDRV_MANAGER_GET_SLOT_ID _IO(DEVDRV_MANAGER_MAGIC, 100)
#define DEVDRV_MANAGER_PCIE_HOT_RESET _IO(DEVDRV_MANAGER_MAGIC, 101)
#define DEVDRV_MANAGER_GET_SOC_DIE_ID _IO(DEVDRV_MANAGER_MAGIC, 102)
#define DEVDRV_MANAGER_GET_CHIP_INFO _IO(DEVDRV_MANAGER_MAGIC, 103)
#define DEVDRV_MANAGER_RST_I2C_CTROLLER _IO(DEVDRV_MANAGER_MAGIC, 104)
#define DEVDRV_MANAGER_GET_XLOADER_BOOT_INFO _IO(DEVDRV_MANAGER_MAGIC, 105)
#define DEVDRV_MANAGER_GET_GPIO_STATE _IO(DEVDRV_MANAGER_MAGIC, 106)
#define DEVDRV_MANAGER_APPMON_BBOX_EXCEPTION_CMD _IO(DEVDRV_MANAGER_MAGIC, 107)
#define DEVDRV_MANAGER_GET_CONTAINER_FLAG _IO(DEVDRV_MANAGER_MAGIC, 108)
#define DEVDRV_MANAGER_IPC_NOTIFY_SET_PID _IO(DEVDRV_MANAGER_MAGIC, 109)
#define DEVDRV_MANAGER_FLASH_USER_CMD _IO(DEVDRV_MANAGER_MAGIC, 110)
#define DEVDRV_MANAGER_P2P_ATTR _IO(DEVDRV_MANAGER_MAGIC, 111)
#define DEVDRV_MANAGER_GET_PROCESS_SIGN _IO(DEVDRV_MANAGER_MAGIC, 112)
#define DEVDRV_MANAGER_GET_MASTER_DEV_IN_THE_SAME_OS _IO(DEVDRV_MANAGER_MAGIC, 113)
#define DEVDRV_MANAGER_GET_LOCAL_DEV_ID_BY_HOST_DEV_ID _IO(DEVDRV_MANAGER_MAGIC, 114)
#define DEVDRV_MANAGER_GET_FAULT_REPORT _IO(DEVDRV_MANAGER_MAGIC, 115)
#define DEVDRV_MANAGER_PASSTHRU_MCU _IO(DEVDRV_MANAGER_MAGIC, 116)

#define DEVDRV_MANAGER_CMD_MAX_NR 117

#define DEVDRV_CONTIANER_NUM_OF_LONG ((DEVDRV_MAX_DAVINCI_NUM - 1) / 64 + 1)

#define DEVDRV_MANAGER_IPC_NOTIFY_CMD_NUM \
    _IOC_NR(DEVDRV_MANAGER_IPC_NOTIFY_DESTROY) - _IOC_NR(DEVDRV_MANAGER_IPC_NOTIFY_CREATE)

#define DEVICE_INFO_RESERVE 3
struct devdrv_device_info {
    u8 env_type;

    u8 ai_cpu_ready_num;
    u8 ai_cpu_broken_map;
    u8 ai_core_ready_num;
    u8 ai_core_broken_map;
    u8 ai_subsys_ip_map;

    u32 ctrl_cpu_ip;
    u32 ctrl_cpu_id;
    u32 ctrl_cpu_core_num;
    u32 ctrl_cpu_endian_little;
    u32 ts_cpu_core_num;
    u32 ai_cpu_core_num;
    u32 ai_core_num;
    u32 ai_cpu_core_id;
    u32 ai_core_id;
    u32 aicpu_occupy_bitmap;
    u32 hardware_version;

    u32 ts_load_fail;

    u32 min_sq_id;
    u32 max_sq_id;
    u32 min_cq_id;
    u32 max_cq_id;
    u32 min_stream_id;
    u32 max_stream_id;
    u32 min_event_id;
    u32 max_event_id;
    u32 min_model_id;
    u32 max_model_id;
    u32 min_notify_id;
    u32 max_notify_id;
    u32 min_task_id;
    u32 max_task_id;

    u32 res[DEVICE_INFO_RESERVE];
};

struct devdrv_black_box {
    struct semaphore black_box_sema;
    spinlock_t spinlock;
    struct list_head exception_list;
    u32 exception_num;
    pid_t black_box_pid;
    u8 thread_should_stop;
};

/* only use in device */
struct devdrv_lib_serve_master {
    struct radix_tree_root lib_tree;  // host_pid is tag, client context is item
    struct mutex lock;                // lock when initing
    u32 status;                       // flag whether lib_tree is inited
};

struct devdrv_manager_info {
    /* number of devices probed by pcie */
    u32 prob_num;

    u32 num_dev;

    u32 plat_info;                           /* 0:device side, 1: host side */
    u32 dev_id_flag[DEVDRV_MAX_DAVINCI_NUM]; /* get devce id from host */
    u32 dev_id[DEVDRV_MAX_DAVINCI_NUM];      /* device id assigned by host device driver */
    struct cdev cdev;

    struct device *dev; /* device manager dev */
    spinlock_t spinlock;
    struct workqueue_struct *dev_rdy_work;

    int msg_chan_rdy[MAX_CHIP_NUM]; /* wait for msg channel ready */
    wait_queue_head_t msg_chan_wait[MAX_CHIP_NUM];
    struct devdrv_info *dev_info[DEVDRV_MAX_DAVINCI_NUM];
    struct list_head pm_list_header; /* for power manager */
    spinlock_t pm_list_lock;         /* for power manager */

    struct list_head hostpid_list_header;   /* for hostpid check  */
    struct semaphore devdrv_sign_list_sema; /* for hostpid check */
    u32 devdrv_sign_count[MAX_DOCKER_NUM + 1]; /* for hostpid check , 0~68 : container, 69 :non-container */

    struct list_head msg_pm_list_header; /* for logdrv and other module register on lowpower case */
    spinlock_t msg_pm_list_lock;         /* for logdrv and other module register on lowpower case */

    struct notifier_block ipc_monitor;
    struct notifier_block m3_ipc_monitor;
    /* for heart beat between TS and driver
     * DEVICE manager use this workqueue to
     * start a exception process.
     */
    struct workqueue_struct *heart_beat_wq;
    struct devdrv_black_box black_box;
    struct workqueue_struct *ipc_event_recycle;
    u32 host_type;

    struct devdrv_drv_ops *drv_ops;

    struct devdrv_lib_serve_master lib_serve;

    struct devdrv_container_table *container_table;
};

#define DEVDRV_HEART_BEAT_SQ_CMD 0xAABBCCDD
#define DEVDRV_HEART_BEAT_CYCLE 5                               /* second */
#define DEVDRV_HEART_BEAT_TIMEOUT (DEVDRV_HEART_BEAT_CYCLE * 2) /* second */
#define DEVDRV_HEART_BEAT_THRESHOLD 3
#define DEVDRV_HEART_BEAT_MAX_QUEUE 20

#define DEVDRV_MATEBOOK_WINDOWS_HOST 0x12121212
#define DEVDRV_LINUX_LINUX_HOST 0x23232323
#define DEVDRV_WILL_HOST_REBOOT 0x26262626

#define DEVDRV_TIME_NEED_UPDATE 1
#define DEVDRV_TIME_UPDATE_DONE 0
#define DEVDRV_TIME_UPDATE_THRESHOLD 6

#define IS 1

struct devdrv_heart_beat_sq {
    u32 number;              /* increment counter */
    u32 cmd;                 /* always 0xAABBCCDD */
    struct timespec64 stamp; /* system time */
    u32 devid;
    u32 reserved;           /* used for judge different host-type */
    struct timespec64 wall; /* wall time */
    u64 cntpct;             /* ccpu sys counter */
    u32 time_update;        /* if time_update = 1, you need send time to device to update */
};

struct devdrv_heart_beat_cq {
    u32 number;                /* increment counter */
    u32 cmd;                   /* always 0xAABBCCDD */
    u32 syspcie_sysdma_status; /* upper 16 bit: syspcie, lower 16 bit: sysdma */
    u32 aicpu_heart_beat_exception;
    u32 aicore_bitmap; /* every bit identify one aicore, bit0 for core0, value 0 is ok */
    u32 ts_status;     /* ts working status, 0 is ok */

    u32 report_type; /* 0: heart beat report, 1: exception report */
    u32 exception_code;
    struct timespec64 exception_time;
};

struct devdrv_heart_beat_node {
    struct devdrv_heart_beat_sq *sq;
    struct list_head list;
    /* HOST manager use this to
     * add heart beat work into workqueue
     * DEVICE manager not use
     */
    struct work_struct work;
    volatile u32 useless; /* flag this node is valid or invalid */
};

#define DEVDRV_AI_SUBSYS_INIT_CHECK_SRAM_OFFSET 0
#define DEVDRV_AI_SUBSYS_INIT_CHECK_SDMA_OFFSET 1
#define DEVDRV_AI_SUBSYS_INIT_CHECK_BS_OFFSET 2
#define DEVDRV_AI_SUBSYS_INIT_CHECK_L2_BUF0_OFFSET 3
#define DEVDRV_AI_SUBSYS_INIT_CHECK_L2_BUF1_OFFSET 4

#define DEVDRV_AI_SUBSYS_SDMA_WORKING_STATUS_OFFSET 5
#define DEVDRV_AI_SUBSYS_SPCIE_WORKING_STATUS_OFFSET 6
#define DEVDRV_AI_SUBSYS_INIT_CHECK_AI_CORE_OFFSET 7
#define DEVDRV_AI_SUBSYS_INIT_CHECK_HWTS_OFFSET 8
#define DEVDRV_AI_SUBSYS_INIT_CHECK_DOORBELL_OFFSET 9

#define DEVDRV_MANAGER_MATRIX_INVALID 0
#define DEVDRV_MANAGER_MATRIX_VALID 1

struct devdrv_ts_ai_ready_info {
    u32 ai_cpu_ready_num;
    u32 ai_cpu_broken_map;
    u32 ai_core_ready_num;
    u32 ai_core_broken_map;
    u32 ai_subsys_ip_map;
    u32 res;
};

enum {
    DEVDRV_MANAGER_CHAN_H2D_SEND_DEVID,
    DEVDRV_MANAGER_CHAN_D2H_DEVICE_READY,
    DEVDRV_MANAGER_CHAN_D2H_DEVICE_READY_AND_TS_WORK,
    DEVDRV_MANAGER_CHAN_D2H_DOWN,
    DEVDRV_MANAGER_CHAN_D2H_SUSNPEND,
    DEVDRV_MANAGER_CHAN_D2H_RESUME,
    DEVDRV_MANAGER_CHAN_D2H_FAIL_TO_SUSPEND,
    DEVDRV_MANAGER_CHAN_D2H_CORE_INFO,
    DEVDRV_MANAGER_CHAN_H2D_HEART_BEAT,
    DEVDRV_MANAGER_CHAN_D2H_GET_PCIE_ID_INFO,
    DEVDRV_MANAGER_CHAN_H2D_SYNC_GET_STREAM,
    DEVDRV_MANAGER_CHAN_D2H_SYNC_GET_STREAM,
    DEVDRV_MANAGER_CHAN_H2D_SYNC_GET_EVENT,
    DEVDRV_MANAGER_CHAN_D2H_SYNC_GET_EVENT,
    DEVDRV_MANAGER_CHAN_H2D_SYNC_GET_SQ,
    DEVDRV_MANAGER_CHAN_D2H_SYNC_GET_SQ,
    DEVDRV_MANAGER_CHAN_H2D_SYNC_GET_CQ,
    DEVDRV_MANAGER_CHAN_D2H_SYNC_GET_CQ,
    DEVDRV_MANAGER_CHAN_H2D_SYNC_GET_MODEL,
    DEVDRV_MANAGER_CHAN_D2H_SYNC_GET_MODEL,
    DEVDRV_MANAGER_CHAN_H2D_SYNC_GET_NOTIFY,
    DEVDRV_MANAGER_CHAN_D2H_SYNC_GET_NOTIFY,
    DEVDRV_MANAGER_CHAN_H2D_SYNC_GET_DEVINFO,
    DEVDRV_MANAGER_CHAN_H2D_GET_TASK_STATUS,
    DEVDRV_MANAGER_CHAN_H2D_LOAD_KERNEL,
    DEVDRV_MANAGER_CHAN_D2H_LOAD_KERNEL_RESULT,
    DEVDRV_MANAGER_CHAN_H2D_SYNC_GET_TASK,
    DEVDRV_MANAGER_CHAN_D2H_SYNC_GET_TASK,
    DEVDRV_MANAGER_CHAN_H2D_SYNC_LOW_POWER,
    DEVDRV_MANAGER_CHAN_D2H_SYNC_MATRIX_READY,
    DEVDRV_MANAGER_CHAN_D2H_CHECK_PROCESS_SIGN,
    DEVDRV_MANAGER_CHAN_MAX_ID,
};

#define DEVDRV_MANAGER_MSG_VALID 0x5A5A
#define DEVDRV_MANAGER_MSG_INVALID_RESULT 0x1A
#define DEVDRV_MANAGER_MSG_H2D_MAGIC 0x5A5A
#define DEVDRV_MANAGER_MSG_D2H_MAGIC 0xA5A5

#define DEVDRV_MANAGER_MSG_GET_ID_NUM 16

#ifdef CFG_SOC_PLATFORM_CLOUD
#define DEVDRV_LOAD_KERNEL_TIMEOUT 120000
#else
#define DEVDRV_LOAD_KERNEL_TIMEOUT 15000
#endif

enum {
    MSG_ID_TYPE_STREAM = 0x50,
    MSG_ID_TYPE_EVENT,
    MSG_ID_TYPE_SQ,
    MSG_ID_TYPE_CQ,
    MSG_ID_TYPE_MODEL,
    MSG_ID_TYPE_NOTIFY,
    MSG_ID_TYPE_TASK,
};

enum devdrv_board_type {
    DEVDRV_BOARD_PCIE_MINI,
    DEVDRV_BOARD_PCIE_CLOUD,
    DEVDRV_BOARD_AISERVER,
    DEVDRV_BOARD_FPGA,
    DEVDRV_BOARD_OTHERS,
};

enum {
    MSG_CMD_TYPE_SYNC = 0x60,
    MSG_CMD_TYPE_ASYNC,
};

struct devdrv_manager_power_state {
    u16 IsLowPowerState; /* Indicate if it is low power state. 0 is false 1 is Yes */
};

struct devdrv_manager_msg_head {
    u32 dev_id;
    u32 msg_id;
    u16 valid;  /* validity judgement, 0x5A5A is valide */
    u16 result; /* process result from rp, zero for succ, non zero for fail */
    u32 tsid;
};

#define DEVDRV_MANAGER_INFO_LEN 128
#define DEVDRV_MANAGER_INFO_PAYLOAD_LEN (DEVDRV_MANAGER_INFO_LEN - sizeof(struct devdrv_manager_msg_head))

struct devdrv_manager_msg_info {
    struct devdrv_manager_msg_head header;
    u8 payload[DEVDRV_MANAGER_INFO_PAYLOAD_LEN];
};

#define DEVDRV_CONTAINER_MSG_PAYLOAD_LENTH (DEVDRV_MANAGER_INFO_PAYLOAD_LEN - sizeof(u32) * 2)
#define DEVDRV_MINI_CONTAINER_TFLOP_NUM (DEVDRV_MINI_TOTAL_TFLOP / DEVDRV_MINI_FP16_UNIT)

/* the length of struct devdrv_container_msg must not longger than DEVDRV_MANAGER_INFO_PAYLOAD_LEN */
struct devdrv_container_msg {
    u32 devid;
    u32 cmd;
    u8 payload[DEVDRV_CONTAINER_MSG_PAYLOAD_LENTH];
};

struct devdrv_load_kernel_msg {
    struct devdrv_manager_msg_head header;
    struct devdrv_load_kernel kernel_info;
};

#define DEVDRV_LOAD_KERNEL_SUCC 0
#define DEVDRV_LOAD_KERNEL_FAIL 1

struct devdrv_load_kernel_result {
    u32 state;
    int host_pid;
};

struct devdrv_load_kernel_result_msg {
    struct devdrv_manager_msg_head header;
    struct devdrv_load_kernel_result result;
};

enum devdrv_lib_status {
    DEVDRV_LIB_ORIGIN,
    DEVDRV_LIB_READY,
    DEVDRV_LIB_WAIT,
    DEVDRV_LIB_EXIT,
    DEVDRV_LIB_MAX_STATUS,
};

struct devdrv_lib_serve_client {
    pid_t host_pid;
    pid_t device_pid;
    u32 status;
    u32 proc_state;
    wait_queue_head_t wait;
    struct list_head lib_list;  // link all library info from host
    spinlock_t spinlock;        // lock when proc lib_list
};

struct devdrv_lib_info {
    struct devdrv_load_kernel kernel_info;
    struct list_head list;
};

#define PROCESS_SIGN_LENGTH  49
#define PROCESS_RESV_LENGTH  4
#define RANDOM_SIZE          24
#define DEVDRV_MAX_SIGN_NUM  1000

struct process_sign {
    pid_t tgid;
    char sign[PROCESS_SIGN_LENGTH];
    char resv[PROCESS_RESV_LENGTH];
};

struct devdrv_manager_info *devdrv_get_manager_info(void);
void devdrv_manager_unregister(struct devdrv_info *dev_info);
int devdrv_get_platformInfo(u32 *info);
int devdrv_get_devnum(unsigned int *num_dev);
u32 devdrv_manager_get_devid(u32 local_devid);
int devdrv_get_devinfo(unsigned int devid, struct devdrv_device_info *info);
int devdrv_get_core_inuse(u32 devid, struct devdrv_hardware_inuse *inuse);
int devdrv_get_core_spec(u32 devid, struct devdrv_hardware_spec *spec);
int devdrv_manager_register_client(struct devdrv_init_client *dev_client, const struct file_operations *ops);
int devdrv_manager_unregister_client(struct devdrv_init_client *dev_client);
struct devdrv_info *devdrv_manager_get_devdrv_info(u32 dev_id);

#endif /* __DEVDRV_MANAGER_COMMON_H */
