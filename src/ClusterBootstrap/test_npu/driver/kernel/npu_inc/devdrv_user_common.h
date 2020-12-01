/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2012-2019. All rights reserved.
 * Description: This file contains devdrv_user_common Info.
 * Author: huawei
 * Create: 2017-10-15
 */

#ifndef __DEVDRV_USER_COMMON_H
#define __DEVDRV_USER_COMMON_H

#ifndef pid_t
typedef int pid_t;
#endif

#define DEVDRV_MAX_STREAM_ID (1024)
#define DEVDRV_MAX_EVENT_ID  (1024)
#define DEVDRV_MAX_MODEL_ID  (1024)

#ifdef CFG_SOC_PLATFORM_MINI
#define DEVDRV_MAX_NOTIFY_ID (1024)
#define DEVDRV_MAX_TASK_ID   15000
#else
#define DEVDRV_MAX_NOTIFY_ID (1024)
#define DEVDRV_MAX_TASK_ID   60000
#endif /* CFG_SOC_PLATFORM_MINI */

#define MAX_DAVINCI_NUM_OF_ONE_CHIP     (4)

#define DEVDRV_SHM_MAPS_SIZE            (4 * 1024 * 1024)

#define DEVDRV_SRAM_TS_SHM_SIZE         (0x1000)

#define DEVDRV_SQ_SLOT_SIZE             (64)
#define DEVDRV_CQ_SLOT_SIZE             (12)
#define DEVDRV_MAX_CQ_SLOT_SIZE         (128)

#define DEVDRV_CACHELINE_OFFSET         (6)
#define DEVDRV_CACHELINE_SIZE           (64)
#define DEVDRV_CACHELINE_MASK           (DEVDRV_CACHELINE_SIZE - 1)

#define DEVDRV_IMU_CMD_LEN              (32)
#define DEVDRV_MAX_CQ_DEPTH             (1024)

#define BIT_NUM_U64                     (64)
#define BITMAP_MAX                      ((DEVDRV_MAX_STREAM_ID - 1) / BIT_NUM_U64 + 1)

#define DEVDRV_MAX_SQ_DEPTH             (1024)
#define DEVDRV_MAX_DAVINCI_NUM          (64)
#define DEVDRV_MAX_SQ_NUM               (512)
#define DEVDRV_MAX_CQ_NUM               (352)

#define DEVDRV_FUNCTIONAL_SQ_FIRST_INDEX    (496)
#define DEVDRV_FUNCTIONAL_CQ_FIRST_INDEX    (500)
#define DEDVRV_DEV_PROCESS_HANG             (0x0F000FFF)

/* Lite : Mini */
#define DEVDRV_MAX_FUNCTIONAL_SQ_NUM        (4)
#define DEVDRV_MAX_FUNCTIONAL_CQ_NUM        (10)

#define DEVDRV_MAILBOX_MAX_FEEDBACK         (16)
#define DEVDRV_MAILBOX_STOP_THREAD          (0x0FFFFFFF)
#define DEVDRV_BUS_DOWN                     (0x0F00FFFF)

#define DEVDRV_IPC_NAME_SIZE               (65)
#define DEVDRV_HCCL_MAX_NODE_NUM            (128)
#define DEVDRV_PID_MAX_NUM                  (16)

#define DEVDRV_MAX_INTERRUPT_NUM            (32)
#define DEVDRV_MAX_MEMORY_DUMP_SIZE         (4 * 1024 * 1024)

#define DEVDRV_USER_CONFIG_NAME_LEN         (32)
#define DEVDRV_USER_CONFIG_NAME_MAX         (32)
#define DEVDRV_USER_CONFIG_VALUE_LEN        (128)

#define DEVDRV_BB_DEVICE_ID_INFORM          (0x66020004)
#define DEVDRV_BB_DEVICE_STATE_INFORM       (0x66020008)

#ifdef CFG_SOC_PLATFORM_MINIV2
#define CHIP_BASEADDR_PA_OFFSET             (0x8000000000ULL)
#else
#define CHIP_BASEADDR_PA_OFFSET             (0x200000000000ULL)
#endif /* CFG_SOC_PLATFORM_MINIV2 */

enum devdrv_stream_status {
    DEVDRV_STREAM_NORMAL = 0,
    DEVDRV_STREAM_EXCEPTION,
    DEVDRV_MAX_STREAM_STATUS,
};

#if defined(CFG_SOC_PLATFORM_MINIV2)
#define DEVDRV_MAX_TS_NUM                   (2)
#else
#define DEVDRV_MAX_TS_NUM                   (1)
#endif

enum devdrv_ts_status {
    DEVDRV_TS_WORK = 0x0,
    DEVDRV_TS_SLEEP,
    DEVDRV_TS_DOWN,
    DEVDRV_TS_INITING,
    DEVDRV_TS_BOOTING,
    DEVDRV_TS_FAIL_TO_SUSPEND,
    DEVDRV_TS_MAX_STATUS,
};

struct devdrv_ts_sq_info {
    u32 head;
    u32 tail;
    u32 credit;
    u32 index;

    int uio_num;
    int uio_fd;
    int uio_map;
    u8 *uio_addr;
    int uio_size;

    u32 stream_num;
    u64 send_count;

    void *sq_sub;
};

struct devdrv_ts_cq_info {
    u32 head;
    u32 tail;
    volatile u32 count_report;
    u32 index;
    u32 phase;
    u32 int_flag;

    int uio_num;
    int uio_fd;
    int uio_map;
    u8 *uio_addr;
    int uio_size;

    u32 stream_num;
    u64 receive_count;

    void *cq_sub;

    u8 slot_size;
};

struct devdrv_stream_info {
    int id;
    u32 devid;
    u32 cq_index;
    u32 sq_index;
    void *stream_sub;
    int pid;
    u32 strategy;
};

#define DEVDRV_MAILBOX_PAYLOAD 64

struct devdrv_mailbox_user_message {
    u8 message_payload[DEVDRV_MAILBOX_PAYLOAD];
    int message_length;
    int feedback_num;
    u8 *feedback_buffer; /*
                          * if a sync message need feedback, must alloc buffer for feedback data.
                          * if a async message need feedback, set this to null,
                          * because driver will send a callback parameter to callback func,
                          * app has no need to free callback parameter in callback func.
                          */
    int sync_type;
    int cmd_type;
    int message_index;
    int message_pid;
    void (*callback)(void *data);
};

struct devdrv_mailbox_feedback {
    void (*callback)(void *data);
    u8 *buffer;
    int feedback_num;
    int process_result;
};

struct devdrv_user_parameter {
    u32 devid;
    u32 cq_slot_size;
    u16 disable_wakelock;
};

struct devdrv_pci_info {
    u32 dev_id;
    u8 bus_number;
    u8 dev_number;
    u8 function_number;
};

struct devdrv_svm_to_devid {
    u32 src_devid;
    u32 dest_devid;
    unsigned long src_addr;
    unsigned long dest_addr;
};

struct devdrv_channel_info_devid {
    char name[DEVDRV_IPC_NAME_SIZE];
    u32 handle;

    u32 event_id;
    u32 src_devid;
    u32 dest_devid;

    /* for ipc event query */
    u32 status;
    u64 timestamp;
};

struct devdrv_trans_info {
    u32 src_devid;
    u32 dest_devid;
    u8 ways;
};

struct devdrv_invaild_tlb_info {
    unsigned long vaddr;
    unsigned long size;
};

struct devdrv_stream_ioctl_info {
    u32 dev_id;
    int stream;
    u32 status;
};

enum devdrv_stream_strategy {
    STREAM_STRATEGY_NORMAL = 0,
    STREAM_STRATEGY_SINK = 1,
    STREAM_STRATEGY_MAX,
};

struct devdrv_stream_para {
    int stream_id;
    u32 strategy;
};

struct devdrv_event_para {
    int event_id;
};

struct devdrv_notify_para {
    u32 dev_id;
    int notify_id;
    u64 dev_addr;
    u64 host_addr;
};

struct devdrv_model_para {
    int model_id;
};

struct devdrv_taskpool_para {
    int taskpool_id;
};

struct devdrv_report_para {
    int timeout;
};

struct devdrv_ioctl_arg {
    u32 tsid;
    union {
        struct devdrv_stream_para stream_para;
        struct devdrv_event_para event_para;
        struct devdrv_notify_para notify_para;
        struct devdrv_model_para model_para;
        struct devdrv_taskpool_para taskpool_para;
        struct devdrv_report_para report_para;
    };
};

struct devdrv_notify_ioctl_info {
    u32 dev_id;
    u32 tsid;
    u32 notify_id;
    u64 dev_addr;
    u64 host_addr;
    char name[DEVDRV_IPC_NAME_SIZE];
    pid_t pid[DEVDRV_PID_MAX_NUM];
};

struct devdrv_hardware_spec {
    u32 devid;
    u32 ai_core_num;
    u32 first_ai_core_id;
    u32 ai_cpu_num;
    u32 first_ai_cpu_id;
};

struct devdrv_hardware_inuse {
    u32 devid;
    u32 ai_core_num;
    u32 ai_core_error_bitmap;
    u32 ai_cpu_num;
    u32 ai_cpu_error_bitmap;
};

struct devdrv_manager_hccl_devinfo {
    u8 env_type;
    u32 dev_id;
    u32 ctrl_cpu_ip;
    u32 ctrl_cpu_id;
    u32 ctrl_cpu_core_num;
    u32 ctrl_cpu_endian_little;
    u32 ts_cpu_core_num;
    u32 ai_cpu_core_num;
    u32 ai_core_num;
    u32 ai_cpu_bitmap;
    u32 ai_core_id;
    u32 ai_cpu_core_id;
    u32 hardware_version; /* mini, cloud, lite, etc. */

    u32 num_dev;
    u32 devids[DEVDRV_MAX_DAVINCI_NUM];
    u32 ts_num;
};

struct devdrv_sysrdy_info {
    u32 probe_dev_num;
    u32 rdy_dev_num;
};

enum devdrv_arch_type {
    ARCH_BEGIN = 0,
    ARCH_V100 = ARCH_BEGIN,
    ARCH_V200,
    ARCH_END,
};

enum devdrv_chip_type {
    CHIP_BEGIN = 0,
    CHIP_MINI = CHIP_BEGIN,
    CHIP_CLOUD,
    CHIP_LITE_PHOENIX,
    CHIP_LITE_ORLANDO,
    CHIP_TINY_PHOENIX,
    CHIP_END,
};

enum devdrv_version {
    VER_BEGIN = 0,
    VER_NA = VER_BEGIN,
    VER_ES,
    VER_CS,
    VER_END,
};

#define PLAT_COMBINE(arch, chip, ver) ((arch << 16) | (chip << 8) | (ver))
#define PLAT_GET_ARCH(type) ((type >> 16) & 0xffff)
#define PLAT_GET_CHIP(type) ((type >> 8) & 0xff)
#define PLAT_GET_VER(type) (type & 0xff)

enum devdrv_hardware_version {
    DEVDRV_PLATFORM_MINI_V1 = PLAT_COMBINE(ARCH_V100, CHIP_MINI, VER_NA),
    DEVDRV_PLATFORM_CLOUD_V1 = PLAT_COMBINE(ARCH_V100, CHIP_CLOUD, VER_NA),
    DEVDRV_PLATFORM_LITE_PHOENIX_ES = PLAT_COMBINE(ARCH_V100, CHIP_LITE_PHOENIX, VER_ES),
    DEVDRV_PLATFORM_LITE_PHOENIX_CS = PLAT_COMBINE(ARCH_V100, CHIP_LITE_PHOENIX, VER_CS),
    DEVDRV_PLATFORM_LITE_ORLANDO = PLAT_COMBINE(ARCH_V100, CHIP_LITE_ORLANDO, VER_NA),
    DEVDRV_PLATFORM_TINY_PHOENIX_ES = PLAT_COMBINE(ARCH_V100, CHIP_TINY_PHOENIX, VER_ES),
    DEVDRV_PLATFORM_TINY_PHOENIX_CS = PLAT_COMBINE(ARCH_V100, CHIP_TINY_PHOENIX, VER_CS),
    DEVDRV_PLATFORM_END,
};

struct devdrv_occupy_stream_id {
    u32 tsid;
    u16 count;
    u16 id[DEVDRV_MAX_STREAM_ID];
};

struct devdrv_black_box_devids {
    u32 dev_num;
    u32 devids[DEVDRV_MAX_DAVINCI_NUM];
};

enum devdrv_device_state {
    STATE_TO_SO = 0,
    STATE_TO_SUSPEND,
    STATE_TO_S3,
    STATE_TO_S4,
    STATE_TO_D0,
    STATE_TO_D3,
    STATE_TO_DISABLE_DEV,
    STATE_TO_ENABLE_DEV,
    STATE_TO_MAX,
};

struct devdrv_black_box_state_info {
    u32 state;
    u32 devId;
};

struct devdrv_black_box_user {
    u32 devid;
    u32 size;
    u64 addr_offset;
    void *dst_buffer;
    u32 thread_should_stop;
    u32 exception_code;
    u64 tv_sec;
    u64 tv_nsec;

    union {
        struct devdrv_black_box_devids bbox_devids;
        struct devdrv_black_box_state_info bbox_state;
    } priv_data;
};

struct devdrv_module_status {
    u32 devid;
    u32 ai_core_error_bitmap;
    u8 lpm3_start_fail;
    u8 lpm3_lost_heart_beat;
    u8 ts_start_fail;
    u8 ts_lost_heart_beat;
    u8 ts_sram_broken;
    u8 ts_sdma_broken;
    u8 ts_bs_broken;
    u8 ts_l2_buf0_broken;
    u8 ts_l2_buf1_broken;
    u8 ts_spcie_broken;
    u8 ts_ai_core_broken;
    u8 ts_hwts_broken;
    u8 ts_doorbell_broken;
};

struct devdrv_pcie_pre_reset {
    u32 dev_id;
};
struct devdrv_pcie_rescan {
    u32 dev_id;
};
struct devdrv_pcie_hot_reset {
    u32 dev_id;
};

enum devdrv_p2p_attr_op {
    DEVDRV_P2P_ADD = 0,
    DEVDRV_P2P_DEL,
    DEVDRV_P2P_QUERY,
    DEVDRV_P2P_MAX
};

struct devdrv_p2p_attr {
    u32 op;
    u32 dev_id;
    u32 peer_dev_id;
};

struct devdrv_alloc_host_dma_addr_para {
    unsigned int devId;
    unsigned int size;
    unsigned long long phyAddr;
    unsigned long long virAddr;
};
struct devdrv_free_host_dma_addr_para {
    unsigned int devId;
    unsigned int size;
    unsigned long long phyAddr;
    unsigned long long virAddr;
};

#define DEVDRV_VALUE_SIZE 512

struct devdrv_pcie_sram_read_para {
    unsigned int devId;
    unsigned int offset;
    unsigned char value[DEVDRV_VALUE_SIZE];
    unsigned int len;
};
struct devdrv_pcie_sram_write_para {
    unsigned int devId;
    unsigned int offset;
    unsigned char value[DEVDRV_VALUE_SIZE];
    unsigned int len;
};

struct devdrv_pcie_ddr_read_para {
    unsigned int devId;
    unsigned int offset;
    unsigned char value[DEVDRV_VALUE_SIZE];
    unsigned int len;
};

struct devdrv_pcie_ddr_write_para {
    unsigned int devId;
    unsigned int offset;
    unsigned char value[DEVDRV_VALUE_SIZE];
    unsigned int len;
};

struct devdrv_get_user_config_para {
    char config_name[DEVDRV_USER_CONFIG_NAME_LEN];
    char config_value[DEVDRV_USER_CONFIG_VALUE_LEN];
    u32 config_value_len;
};
struct devdrv_get_device_boot_status_para {
    unsigned int devId;
    u32 boot_status;
};
struct devdrv_get_host_phy_mach_flag_para {
    unsigned int devId;
    unsigned int host_flag;
};
struct devdrv_pcie_imu_ddr_read_para {
    unsigned int devId;
    unsigned int offset;
    unsigned char value[DEVDRV_VALUE_SIZE];
    unsigned int len;
};

struct devdrv_get_device_os_para {
    unsigned int dev_id;
    unsigned int master_dev_id;
};

struct devdrv_get_local_devid_para {
    u32 host_dev_id;
    u32 local_dev_id;
};

#define DEVDRV_SQ_INFO_OCCUPY_SIZE \
    (sizeof(struct devdrv_ts_sq_info) * DEVDRV_MAX_SQ_NUM)
#define DEVDRV_CQ_INFO_OCCUPY_SIZE \
    (sizeof(struct devdrv_ts_cq_info) * DEVDRV_MAX_CQ_NUM)
#define DEVDRV_STREAM_INFO_OCCUPY_SIZE \
    (sizeof(struct devdrv_stream_info) * DEVDRV_MAX_STREAM_ID)

#define DEVDRV_MAX_INFO_SIZE             \
    (DEVDRV_SQ_INFO_OCCUPY_SIZE +        \
        DEVDRV_CQ_INFO_OCCUPY_SIZE +     \
        DEVDRV_STREAM_INFO_OCCUPY_SIZE + \
        sizeof(u32))

#define DEVDRV_MAX_INFO_ORDER (get_order(DEVDRV_MAX_INFO_SIZE))

#define PMU_EMMC_VCC_CHANNEL  (7)
#define PMU_EMMC_VCCQ_CHANNEL (14)
#define ADCIN7_SLOT0          (7)
#define ADCIN8_SLOT1          (8)

struct devdrv_emmc_voltage_para {
    int emmc_vcc;   // should be 2950 mv
    int emmc_vccq;  // should be 1800 mv
};

#define DMANAGE_ERROR_ARRAY_NUM (128)
struct devdrv_error_code_para {
    int error_code_count;
    unsigned int error_code[DMANAGE_ERROR_ARRAY_NUM];
    unsigned int dev_id;
};

#define RESULT_SIZE 4

struct tsensor_ioctl_arg {
    u32 dev_id;
    u32 coreid;
    u32 result_size;
    u32 result[RESULT_SIZE];
};

/*
 * add necessary dfx function if you need
 */
enum devdrv_dfx_cmd {
    DEVDRV_DFX_QUERY_STATUS,
    DEVDRV_DFX_GET_STATUS,
    DEVDRV_DFX_MAX_CMD,
};

enum devdrv_dfx_sec_cmd {
    DEVMNG_DFX_TS_WORKING = 0,
    DEVMNG_DFX_HEART_BEAT,
    DEVMNG_DFX_CQSQ,
    DEVMNG_DFX_ALL_STATUS,
    DEVMNG_DFX_MAX_DFX_CMD,
};

struct devdrv_cq_dfx_info {
    u32 flag;
    u32 a;
    u32 b;
    u32 c;
    u32 a1;
    u32 b1;
    u32 c1;
};

/*
 * DEVDRV_DFX_QUERY_STATUS
 * add necessary value info if you need, remember add both user code and kernel code
 */
struct devdrv_status_info {
    u16 sq_head[DEVDRV_MAX_SQ_NUM];
    u16 sq_tail[DEVDRV_MAX_SQ_NUM];
    u16 cq_head[DEVDRV_MAX_CQ_NUM];
    u16 cq_tail[DEVDRV_MAX_CQ_NUM];
    u16 func_sq_head[DEVDRV_MAX_FUNCTIONAL_SQ_NUM];
    u16 func_sq_tail[DEVDRV_MAX_FUNCTIONAL_SQ_NUM];
    u16 func_cq_head[DEVDRV_MAX_FUNCTIONAL_CQ_NUM];
    u16 func_cq_tail[DEVDRV_MAX_FUNCTIONAL_CQ_NUM];
    u64 sq_addr[DEVDRV_MAX_SQ_NUM];
    u64 cq_virt_addr[DEVDRV_MAX_CQ_NUM];
    u64 cq_addr[DEVDRV_MAX_CQ_NUM];
    u64 func_sq_addr[DEVDRV_MAX_FUNCTIONAL_SQ_NUM];
    u64 func_cq_addr[DEVDRV_MAX_FUNCTIONAL_CQ_NUM];
    u16 stream_sq[DEVDRV_MAX_STREAM_ID];
    u16 stream_cq[DEVDRV_MAX_STREAM_ID];
    u32 ts_beat_count;
    u32 m3_beat_count;
    u32 ts_status;
    u8 ts_beat_en;
    u8 m3_beat_en;
    u8 cq_phase[DEVDRV_MAX_CQ_NUM];
    u8 func_cq_phase[DEVDRV_MAX_FUNCTIONAL_CQ_NUM];
};

/* ioctl parameter */
struct devdrv_dfx_para {
    u32 devid;
    u32 cmd;
    void *in;
    void *out;
};

enum devdrv_container_cmd {
    /* user for no plusgin container */
    DEVDRV_CONTAINER_GET_DAVINCI_DEVLIST, /* get davinci devlist from item */
    DEVDRV_CONTAINER_GET_BARE_PID,
    DEVDRV_CONTAINER_GET_BARE_TGID,

    DEVDRV_CONTAINER_MAX_CMD,
};

struct devdrv_container_para {
    struct devdrv_dfx_para para;
    u32 admin;
};

#define DEVDRV_MINI_TOTAL_TFLOP 16
#define DEVDRV_MINI_FP16_UNIT 1
#define DEVDRV_MINI_INT8_UNIT 2

struct devdrv_container_alloc_para {
    u32 num;
    u32 npu_id[DEVDRV_MAX_DAVINCI_NUM];
};

struct devdrv_container_tflop_config {
    u32 tflop_mode;
    u32 total_tflop;
    u32 alloc_unit;
    u32 tflop_num;
};

enum devdrv_run_mode {
    DEVDRV_NORMAL_MODE = 0,
    DEVDRV_CONTAINER_MODE,
    DEVDRV_MAX_RUN_MODE,
};

enum devdrv_container_tflop_mode {
    DEVDRV_FP16 = 0,
    DEVDRV_INT8,
    DEVDRV_MAX_TFLOP_MODE,
};

enum devdrv_flash_config_cmd {
    DEVDRV_FLASH_CONFIG_READ_CMD = 0,
    DEVDRV_FLASH_CONFIG_WRITE_CMD,
    DEVDRV_FLASH_CONFIG_CLEAR_CMD,
    DEVDRV_FLASH_CONFIG_MAX_CMD,
};

struct devdrv_flash_config_ioctl_para {
    u32 dev_id;
    int cmd;
    int cfg_index;
    char name[DEVDRV_USER_CONFIG_NAME_MAX];
    u32 buf_size;
    void *buf;
};

struct devdrv_time_sync {
    long interval_seconds;
};

#define DEVDRV_MAX_LIB_LENGTH 128
#define DEVDRV_SHA256_SIZE 32

struct devdrv_load_kernel {
    unsigned int devid;
    unsigned int share;
    char libname[DEVDRV_MAX_LIB_LENGTH];
    unsigned char sha256[DEVDRV_SHA256_SIZE];
    int pid;
    void *binary;
    unsigned int size;
};

struct devdrv_load_kernel_serve {
    struct devdrv_load_kernel load_kernel;
    u8 thread_should_exit;
    u32 save_state;  // succ: 0, fail: 1
};

/*
 * |___SQ___|____INFO_____|__DOORBELL___|___CQ____|___TaskPool___|
 */
#define DEVDRV_VM_TASKPOOL_REAL         (1 * 1024 * 1024)
#define DEVDRV_VM_TASKPOOL_OFFSET       (12 * 1024 * 1024)
#define DEVDRV_VM_BLOCK_OFFSET          (32 * 1024 * 1024)
#define DEVDRV_VM_DOORBELL_BLOCK_OFFSET (64 * 1024 * 1024)
#define DEVDRV_VM_CQ_QUEUE_SIZE         (64 * 1024 * 1024)
#define DEVDRV_VM_CQ_BLOCK_OFFSET       (96 * 1024 * 1024)
#define DEVDRV_VM_CQ_SLOT_SIZE          (128 * 1024)

#define DEVDRV_VM_MEM_START         (0xFFC0000000ULL)
#define DEVDRV_VM_MEM_SIZE          (DEVDRV_VM_BLOCK_OFFSET * 3 + DEVDRV_VM_CQ_QUEUE_SIZE)

/* ****custom ioctrl****** */
typedef enum {
    DEVDRV_IOC_VA_TO_PA,              // current only use in lite
    DEVDRV_IOC_GET_SVM_SSID,          // current only use in lite
    DEVDRV_IOC_GET_CHIP_INFO,         // current only use in lite
    DEVDRV_IOC_ALLOC_CONTIGUOUS_MEM,  // current only use in lite
    DEVDRV_IOC_FREE_CONTIGUOUS_MEM,   // current only use in lite
} devdrv_custom_ioc_t;

typedef struct {
    u32 version;
    u32 cmd;
    u32 result;
    u32 reserved;
    u64 arg;
} devdrv_custom_para_t;

#ifdef CFG_SOC_PLATFORM_CLOUD
#define MCU_RESP_LEN  28
struct devdrv_mcu_msg
{
	unsigned char dev_id;
    unsigned char rw_flag;
	unsigned char send_len;
	unsigned char *send_data;
	unsigned char resp_len;
	unsigned char resp_data[MCU_RESP_LEN];
};
#endif

#endif
