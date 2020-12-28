/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2012-2019. All rights reserved.
 * Description:
 * Author: huawei
 * Create: 2019-10-15
 *
 */

#ifndef __DEVDRV_ID_COMMON_H
#define __DEVDRV_ID_COMMON_H

#include <linux/list.h>
#include <linux/cdev.h>
#include <linux/mutex.h>
#include <linux/time.h>
#include <linux/types.h>
#include <linux/uio_driver.h>
#include <linux/dma-mapping.h>
#include <linux/of_device.h>
#include <linux/notifier.h>
#include <linux/sched.h>
#include <linux/rbtree.h>
#include <linux/cpumask.h>

#include <linux/device.h>
#include <linux/version.h>
#if defined(CFG_SOC_PLATFORM_MINI)
#include <linux/pm_wakeup.h>
#endif

#include "devdrv_mailbox.h"
#include "devdrv_functional_cqsq.h"
#include "devdrv_user_common.h"
#include "devdrv_platform_resource.h"

#include "drv_log.h"

#define module_devdrv "devdrv"

#define devdrv_drv_err(fmt, ...) drv_err(module_devdrv, "<%s:%d> " fmt, current->comm, current->tgid, ##__VA_ARGS__)
#define devdrv_drv_warn(fmt, ...) drv_warn(module_devdrv, "<%s:%d> " fmt, current->comm, current->tgid, ##__VA_ARGS__)
#define devdrv_drv_info(fmt, ...) drv_info(module_devdrv, "<%s:%d> " fmt, current->comm, current->tgid, ##__VA_ARGS__)
#define devdrv_drv_event(fmt, ...) drv_event(module_devdrv, "<%s:%d> " fmt, current->comm, current->tgid, ##__VA_ARGS__)
#define devdrv_drv_debug(fmt, ...)

#define DEVDRV_SQ_FLOOR 16
#define DEVDRV_CQSQ_INVALID_INDEX 0xFEFE
#define CQ_HEAD_UPDATE_FLAG 0x1
#define DEVDRV_REPORT_PHASE 0x8000

#define DEVDRV_INVALID_FD_OR_NUM (-1)
#define DEVDRV_TS_DOORBELL_STRIDE 4096 /* stride 4KB */

#define DEVDRV_TS_DOORBELL_SQ_NUM 512
#define DEVDRV_TS_DOORBELL_CQ_NUM 512

#define DEVDRV_MANAGER_DEVICE_ENV 0
#define DEVDRV_MANAGER_HOST_ENV 1

/* reserved memory for sq slot */
#ifdef CFG_SOC_PLATFORM_MINIV2
#define DEVDRV_RESERVE_MEM_BASE 0xBB00000
#define DEVDRV_RESERVE_MEM_SIZE (32 * 1024 * 1024) /* sqmem size is 32M per ts */
#else
#define DEVDRV_RESERVE_MEM_BASE 0x60000000
#define DEVDRV_RESERVE_MEM_SIZE (32 * 1024 * 1024)
#endif

#define CQ_RESERVE_MEM_BASE (0x2B981000 + 0x200000) /* reserve mem: start addr + offset */
#define CQ_RESERVE_MEM_SIZE (12 * 1024 * 32)

/* test, using spi or doorbel for mailbox */
#define DEVDRV_WAKELOCK_SIZE 56
#define DEVDRV_MANAGER_MSG_ID_NUM_MAX 32

#define WAIT_PROCESS_EXIT_TIME 5000

#ifndef STATIC
#ifdef DEVDRV_MANAGER_HOST_UT_TEST
#define STATIC
#else
#define STATIC static
#endif
#endif

#define CONST

/* include devmm_dev.h and remove below two definition later */
typedef int (*devmm_report_handle_t)(pid_t pid, unsigned int did, unsigned int sts);
#define DEVMM_SVM_NORMAL_EXITED_FLAG 0xEEEEEEEE
#define DEVMM_SVM_ABNORMAL_EXITED_FLAG 0
/* delete above definitions later */
#define GIC_MULTICHIP_ONLINE_STAT 1
#define GIC_MULTICHIP_OFFLINE_STAT 0

/* for acpu and tscpu power down synchronization on lite plat */
#define TS_PD_DONE_FLG 0x7B7B
#define ACPU_PD_TS_DONE_FLG 0x5D5D

#ifdef CFG_SOC_PLATFORM_CLOUD
#define LOAD_DEVICE_TIME 500
#else
#ifdef CFG_SOC_PLATFORM_MINIV2
#define LOAD_DEVICE_TIME 800
#else
#define LOAD_DEVICE_TIME 360
#endif
#endif

extern const struct file_operations devdrv_fops;

enum devdrv_power_mode {
    DEVDRV_LOW_POWER = 0x0,
    DEVDRV_NORMAL_POWER,
    DEVDRV_MAX_MODE,
};

enum devdrv_device_status {
    DRV_STATUS_INITING = 0x0,
    DRV_STATUS_WORK,
    DRV_STATUS_EXCEPTION,
    DRV_STATUS_RESERVED,
};

enum hccl_trans_way {
    DRV_SDMA = 0x0,
    DRV_PCIE_DMA,
};

/* for get report phase byte */
struct devdrv_report {
    u32 a;
    u32 b;
    u32 c;
};

struct devdrv_parameter {
    struct list_head list;
    pid_t pid;
    u32 cq_slot_size;

    u16 disable_wakelock;
};

#define P2P_ADDR_INFO_SIZE 48
struct dev_online_notify {
    phys_addr_t ts_db_addr;
    u32 ts_db_size;
    phys_addr_t ts_sram_addr;
    u32 ts_sram_size;
    phys_addr_t hwts_addr;
    u32 hwts_size;
};
struct host_cfg_notify {
    phys_addr_t host_mem_addr;
    u32 host_mem_size;
};
struct p2p_addr_info {
    struct devdrv_mailbox_message_header header;
    u8 remote_devid;
    u8 status;
    u8 notify_type;
    u8 local_devid;
    u32 reserved2;
    union notify_info {
        u8 buf[P2P_ADDR_INFO_SIZE];
        struct dev_online_notify dev_online_notify_data;
        struct host_cfg_notify host_cfg_notify_data;
    } data;
};

struct devdrv_cm_info {
    dma_addr_t dma_handle;
    void *cpu_addr;
    u64 size;
    u32 valid;
};

struct devdrv_cm_msg {
    u32 share_num;
    u64 size;
};

#define MAX_MEM_INFO_NUM 4
#define MEM_INFO_VALID 0xa5a5a5a5
#define MEM_TOTAL_SIZE 0x800000

struct devdrv_contiguous_mem {
    struct devdrv_cm_info mem_info[MAX_MEM_INFO_NUM];
    u64 total_size;
};

struct devdrv_cce_ops {
    struct cdev devdrv_cdev;
    struct device *cce_dev;
};

/* memory types managed by devdrv */
enum {
    DEVDRV_SQ_MEM = 0,
    DEVDRV_INFO_MEM,
    DEVDRV_DOORBELL_MEM,
    DEVDRV_MAX_MEM
};

struct devdrv_mem_info {
    phys_addr_t phy_addr;
    phys_addr_t bar_addr;
    phys_addr_t virt_addr;
    size_t size;
};

struct devdrv_int_context {
    int first_cq_index;
    struct devdrv_cce_ctrl *cce_ctrl;
    struct tasklet_struct find_cq_task;
};

struct devdrv_cce_ctrl {
    /* device id assigned by local device driver */
    u32 devid;
    u32 tsid;

    u32 stream_num;
    u32 event_num;
    u32 sq_num;
    u32 cq_num;
    u32 model_id_num;
    u32 notify_id_num;
    u32 task_id_num;

    struct list_head stream_available_list;
    struct list_head event_available_list;
    struct list_head model_available_list;
    struct list_head notify_available_list;
    struct list_head task_available_list;

    struct list_head sq_available_list;
    struct list_head cq_available_list;

    struct devdrv_mailbox mailbox;

    struct devdrv_int_context int_context[DEVDRV_CQ_IRQ_NUM];

    struct devdrv_info *dev_info;
    struct uio_info *devdrv_uio_info;

    struct devdrv_functional_cqsq functional_cqsq;

    void *event_addr;
    void *sq_sub_addr;
    void *cq_sub_addr;
    void *stream_sub_addr;
    void *model_addr;
    void *notify_addr;
    void *task_addr;
    spinlock_t spinlock;
    spinlock_t event_spinlock;
    spinlock_t model_spinlock;
    spinlock_t notify_spinlock;
    spinlock_t task_spinlock;

    struct workqueue_struct *wq;              /* for recycle work */
    char wakelock_name[DEVDRV_WAKELOCK_SIZE]; /* for power manager: wakelock */
    struct mutex wake_num_mutex;
    struct mutex recycle_mutex;
    u32 wakelock_num;

    /* manage memory & registers shared with user space */
    struct devdrv_mem_info mem_info[DEVDRV_MAX_MEM];

    atomic_t power_mode; /* for power manager: mode flag */

    atomic_t recycle_work_num;

    struct mutex cq_mutex_t;
    struct mutex sq_mutex_t;
    struct mutex stream_mutex_t;
    struct mutex event_mutex_t;
    struct mutex model_mutex_t;
    struct mutex notify_mutex_t;
    struct mutex task_mutex_t;

    struct devdrv_contiguous_mem cm;

    struct mutex mm_mutex_t;
};

struct devdrv_ts_context {
    u32 tsid;

    u32 stream_num;
    u32 event_num;
    u32 sq_num;
    u32 cq_num;
    u32 model_id_num;
    u32 notify_id_num;
    u32 task_id_num;

    struct list_head stream_list;
    struct list_head event_list;
    struct list_head model_list;
    struct list_head notify_list;
    struct list_head task_list;
    struct list_head sq_list;
    struct list_head cq_list;

    u32 send_count;
    u64 receive_count;

    atomic_t mailbox_message_count;
    spinlock_t mailbox_wait_spinlock;
    struct semaphore mailbox_wait;
    struct list_head message_list_header;

    int cq_tail_updated;
    wait_queue_head_t report_wait;

    struct work_struct recycle_work;
    struct devdrv_cce_context *cce_context;

    u32 last_ts_status;
};

struct devdrv_cce_context {
    pid_t pid;
    u32 devid;

    /* ipc receive process will check this and find cce_context */
    int ipc_port;

    struct list_head cce_ctrl_list;
    struct devdrv_cce_ctrl *cce_ctrl[DEVDRV_MAX_TS_NUM];
    struct devdrv_ts_context ts_context[DEVDRV_MAX_TS_NUM];

    u32 should_stop_thread;

    struct list_head ipc_msg_send_head;
    struct list_head ipc_msg_return_head;
    wait_queue_head_t ipc_wait;

    struct mutex stream_mutex;

    u32 process_status;
    struct rb_node rb_node;

    struct vm_area_struct *vma[DEVDRV_MAX_TS_NUM];

    u32 recycle_status;
};

struct devdrv_taskpool_id_info {
    int id;
    u32 devid;
    struct list_head list;
    void *cce_context;
};

struct devdrv_model_id_info {
    int id;
    u32 devid;
    struct list_head list;
    void *cce_context;
};

struct devdrv_notify_id_info {
    int id;
    u32 devid;
    struct list_head list;
    void *cce_context;

    spinlock_t spinlock;
    atomic_t ref;
};

struct devdrv_event_info {
    int id;
    u32 devid;

    /* ref used for ipc event manage */
    atomic_t ref;

    struct list_head list;

    /* list for ipc event */
    struct list_head ipc_wait_head;
    spinlock_t spinlock;

    void *cce_context;
};

struct devdrv_heart_beat {
    struct list_head queue;
    spinlock_t spinlock;
    struct timer_list timer;
    u32 sq;
    u32 cq;
    volatile u32 cmd_inc_counter; /* increment counter for sendind heart beat cmd */
    struct work_struct work;
    const void *exception_info;
    volatile u32 stop;     /* use in host manager heart beat to device,
                        * avoid access null ptr to heart beat node
                        * when heart beat is stop */
    volatile u32 too_much; /* flag if too much heart beat waiting in queue to be sent */
    volatile u32 broken;
    volatile u32 working;
};

#define DEVDRV_M3_HEART_BEAT_MAX_BEAT 10

struct devdrv_m3_heart_beat {
    struct timer_list timer;
    struct notifier_block nb;
    u16 queue[DEVDRV_M3_HEART_BEAT_MAX_BEAT];
    u8 queue_ptr;
    volatile u8 inc_counter;
    volatile u8 stop;
    volatile u8 work;
    spinlock_t spinlock;
    volatile u32 broken;
};

#define DEVDRV_IMU_HEART_BEAT_MAX_BEAT 10

struct devdrv_imu_heart_beat {
    struct timer_list timer;
    struct notifier_block nb;
    u32 queue[DEVDRV_IMU_HEART_BEAT_MAX_BEAT];
    u8 queue_ptr;
    volatile u8 inc_counter;
    volatile u8 stop;
    volatile u8 work;
    spinlock_t spinlock;
    volatile u32 broken;
    unsigned long last_timestamp;
};

#define DEVDRV_DEV_READY_EXIST 1
#define DEVDRV_DEV_READY_WORK 2

struct devdrv_manager_lpm3_func {
    u8 lpm3_heart_beat_en;
};

struct devdrv_manager_ts_func {
    u8 ts_heart_beat_en;
};

struct devdrv_manager_imu_func {
    u8 imu_heart_beat_en;
};

struct devdrv_device_manager_config {
    struct devdrv_manager_ts_func ts_func;
    struct devdrv_manager_lpm3_func lpm3_func;
    struct devdrv_manager_imu_func imu_func;
};

struct devdrv_host_manager_config {
    u8 heart_beat_en;
};

struct aicpu_dts_config {
    u32 flag;
    u32 fw_cpu_id_base;        /* firmware cpu start id */
    u32 fw_cpu_num;            /* firmware cpu num */
    u32 system_cnt_freq;       /* timer frequency */
    u32 ts_int_start_id;       /* interrupt start id, send int to ts when task finish runing or log */
    u32 ctrl_cpu_int_start_id; /* interrupt start id, send int to control cpu when page missing */
    u32 ipc_cpu_int_start_id;  /* ipc interrupt start id, send int to control cpu when page missing */
    u32 ipc_mbx_int_start_id;
};

struct firmware_info {
    /* 内存信息 */
    void *aicpu_fw_mem;
    u32 aicpu_fw_mem_size;
    void *alg_mem[AICPU_MAX_NUM];
    u32 alg_mem_size;

    /* 启动地址 */
    u64 aicpu_boot_addr; /* 此地址是对齐后的虚拟地址 */
    u64 ts_boot_addr;    /* 此地址是对齐后的物理地址 */
    u64 ts_boot_addr_virt;

    /* 黑匣子信息 */
    u64 ts_blackbox_base;
    u64 ts_blackbox_size;

    /* TS 信息 */
    u64 ts_start_log_base; /* TS log 物理地址 */
    u64 ts_start_log_size; /* TS log 大小 */

    u8 enable_bbox;
};

struct devdrv_container_info {
    u32 tflop_mode;
    u32 total_tflop;
    u32 alloc_unit;
    u32 tflop_num;
    struct mutex lock; /* used for container process */
    void *alloc_table;
};

struct devdrv_ts_mng {
    /* TS working Flag */
    volatile u32 *ts_work_status;
    volatile u32 ts_work_status_shadow;

    /* spinlock for read write ts status */
    spinlock_t ts_spinlock;
};

struct devdrv_info {
    u8 plat_type;
    u8 status;
    atomic_t occupy_ref;
    u32 env_type;
    u32 board_id;
    u32 slot_id;
    u32 dev_id; /* device id assigned by local device driver */

    /*
     * device id assigned by pcie driver
     * not used in device side
     */
    u32 pci_dev_id;

    struct semaphore sem;

    /* devdrv_drv_register is called successfully */
    struct mutex lock;
    volatile u32 driver_flag;
    struct device *dev;

    u32 ctrl_cpu_ip;
    u32 ctrl_cpu_id;
    u32 ctrl_cpu_core_num;
    u32 ctrl_cpu_endian_little;

    u32 ai_cpu_core_num;
    u32 ai_core_num;
    u32 ai_cpu_core_id;
    u32 ai_core_id;
    u32 aicpu_occupy_bitmap;
    u32 ai_subsys_ip_broken_map;
    u32 hardware_version;
    cpumask_t ccpumask;

    struct list_head cce_context_list;
    struct devdrv_hardware_inuse inuse;

    struct work_struct work;
    /* devdrv_manager_register is called successfully */
    volatile int dev_ready;
    spinlock_t spinlock;

    u32 ts_num;
    struct devdrv_ts_mng ts_mng[DEVDRV_MAX_TS_NUM];
    struct devdrv_cce_ctrl *cce_ctrl[DEVDRV_MAX_TS_NUM];
    struct devdrv_cce_ops cce_ops;

    /* independent ops and information for each device */
    struct devdrv_drv_ops *drv_ops;
    struct devdrv_platform_data *pdata;

    struct devdrv_heart_beat heart_beat;

    /*
     * HOST manager use this workqueue to
     * send heart beat msg to device.
     */
    struct workqueue_struct *heart_beat_wq;

    struct devdrv_m3_heart_beat m3_heart_beat;
    struct devdrv_imu_heart_beat imu_heart_beat;

    /* for nfe irq, devdrv_device_manager start a tasklet */
    struct tasklet_struct nfe_task;

    struct devdrv_host_manager_config host_config;
    struct devdrv_device_manager_config config;

    /* aicpu & ts upload related parameters */
    struct firmware_info fw_info;
    struct aicpu_dts_config dts_cfg;
    dma_addr_t aicpu_dma_handle;
    u32 firmware_hardware_version;

    u32 fw_wq_retry;
    struct delayed_work fw_load_wq;

    struct devdrv_container_info container;

    struct rb_root rb_root;
    struct semaphore no_trans_chan_wait_sema;
    unsigned int user_core_distribution;
};

struct devdrv_manager_msg_resource_id {
    /* how many ids request and returned */
    u8 id_type;
    u8 sync_type;
    u32 req_id_num;
    u32 ret_id_num;
    u16 id[DEVDRV_MANAGER_MSG_ID_NUM_MAX];
};

struct devdrv_drv_ops {
    void (*irq_trigger)(struct devdrv_info *dev_info, u32 tsid, int irq_num);
    int (*alloc_mem)(struct devdrv_info *dev_info, phys_addr_t *dev_phy_addr, phys_addr_t *host_phy_addr, size_t size);
    int (*free_mem)(struct devdrv_info *info, phys_addr_t phy_addr);
    void (*flush_cache)(u64 base, u32 len);
    u32 (*svm_va_to_devid)(unsigned long va);

    /* get stream, event, sq, cq ids from opposite side */
    int (*get_stream_sync)(struct devdrv_cce_ctrl *cce_ctrl);
    int (*get_event_sync)(struct devdrv_cce_ctrl *cce_ctrl);
    int (*get_sq_sync)(struct devdrv_info *dev_info);
    int (*get_cq_sync)(struct devdrv_info *dev_info);
    int (*get_model_sync)(struct devdrv_cce_ctrl *cce_ctrl);
    int (*get_notify_sync)(struct devdrv_cce_ctrl *cce_ctrl);
    int (*get_task_sync)(struct devdrv_cce_ctrl *cce_ctrl);

    int (*get_stream_async)(struct devdrv_info *dev_info);
    int (*get_event_async)(struct devdrv_info *dev_info);
    int (*get_sq_async)(struct devdrv_info *dev_info);
    int (*get_cq_async)(struct devdrv_info *dev_info);
    int (*get_task_async)(struct devdrv_info *dev_info);

    int (*add_stream_msg_chan)(struct devdrv_info *dev_info, u32 tsid,
                               struct devdrv_manager_msg_resource_id *dev_msg_resource_id);
    int (*add_event_msg_chan)(struct devdrv_info *dev_info, u32 tsid,
                              struct devdrv_manager_msg_resource_id *dev_msg_resource_id);
    int (*add_sq_msg_chan)(struct devdrv_info *dev_info, struct devdrv_manager_msg_resource_id *dev_msg_resource_id);
    int (*add_cq_msg_chan)(struct devdrv_info *dev_info, struct devdrv_manager_msg_resource_id *dev_msg_resource_id);
    int (*add_model_msg_chan)(struct devdrv_info *dev_info, u32 tsid,
                              struct devdrv_manager_msg_resource_id *dev_msg_resource_id);
    int (*add_notify_msg_chan)(struct devdrv_info *dev_info, u32 tsid,
                               struct devdrv_manager_msg_resource_id *dev_msg_resource_id);
    int (*add_task_msg_chan)(struct devdrv_info *dev_info, u32 tsid,
                             struct devdrv_manager_msg_resource_id *dev_msg_resource_id);

    int (*alloc_stream_msg_chan)(struct devdrv_info *dev_info, u32 tsid,
                                 struct devdrv_manager_msg_resource_id *dev_msg_resource_id);
    int (*alloc_event_msg_chan)(struct devdrv_info *dev_info, u32 tsid,
                                struct devdrv_manager_msg_resource_id *dev_msg_resource_id);
    int (*alloc_sq_msg_chan)(struct devdrv_info *dev_info, struct devdrv_manager_msg_resource_id *dev_msg_resource_id);
    int (*alloc_cq_msg_chan)(struct devdrv_info *dev_info, struct devdrv_manager_msg_resource_id *dev_msg_resource_id);
    int (*alloc_model_msg_chan)(struct devdrv_info *dev_info, u32 tsid,
                                struct devdrv_manager_msg_resource_id *dev_msg_resource_id);
    int (*alloc_notify_msg_chan)(struct devdrv_info *dev_info, u32 tsid,
                                 struct devdrv_manager_msg_resource_id *dev_msg_resource_id);
    int (*alloc_task_msg_chan)(struct devdrv_info *dev_info, u32 tsid,
                               struct devdrv_manager_msg_resource_id *dev_msg_resource_id);

    struct devdrv_event_info *(*get_event_id)(struct devdrv_cce_ctrl *cce_ctrl);

    /* send stream, event, sq, cq ids to opposite side */
    int (*send_stream)(struct devdrv_info *dev_info);
    int (*send_event)(struct devdrv_info *dev_info);
    int (*send_sq)(struct devdrv_info *dev_info);
    int (*send_cq)(struct devdrv_info *dev_info);

    /* functional sqcq */
    int (*create_functional_sq)(u32 devid, u32 tsid, u32 slot_len, u32 *sq_index, u64 *addr);
    int (*functional_set_sq_func)(u32 devid, u32 tsid, u32 sq_index, enum devdrv_cqsq_func function);
    int (*functional_send_sq)(u32 devid, u32 tsid, u32 sq_index, const u8 *buffer, u32 buf_len);
    void (*destroy_functional_sq)(u32 devid, u32 tsid, u32 sq_index);

    int (*create_functional_cq)(u32 devid, u32 tsid, u32 slot_len, u32 cq_type,
                                void (*callback)(const u8 *cq_slot, u8 *sq_slot), u32 *cq_index, u64 *addr);
    int (*functional_set_cq_func)(u32 devid, u32 tsid, u32 cq_index, enum devdrv_cqsq_func function);
    void (*destroy_functional_cq)(u32 devid, u32 tsid, u32 cq_index);

    /* mailbox */
    void (*mailbox_recycle)(struct devdrv_mailbox *mailbox);
    int (*mailbox_kernel_sync_no_feedback)(struct devdrv_mailbox *mailbox, const u8 *buf, u32 len, int *result);

    /* ipc api */
    int (*ipc_msg_send_sync)(struct devdrv_info *dev_info, unsigned long arg);
    int (*ipc_msg_recv_async)(struct devdrv_info *dev_info, unsigned long arg);
    int (*ipc_msg_send_async)(struct devdrv_info *dev_info, unsigned long arg);

    int (*ipc_msg_rx_register)(struct devdrv_info *dev_info, unsigned long arg);
    int (*ipc_msg_rx_unregister)(struct devdrv_info *dev_info, unsigned long arg);
    ssize_t (*ipc_msg_write)(struct file *filep, const char __user *buf, size_t size, loff_t *ppos);
    ssize_t (*ipc_msg_read)(struct file *filep, char __user *buf, size_t size, loff_t *ppos);
    void (*ipc_msg_stop_thread)(struct devdrv_cce_context *cce_context);

    int (*free_one_notify_id)(struct devdrv_cce_ctrl *cce_ctrl, struct devdrv_cce_context *cce_context, int id,
                              int inform_type);
    int (*notify_id_send_ts_msg)(struct devdrv_info *dev_info, u32 tsid, int notifyId);
    int (*get_dev_info)(struct devdrv_info *dev_info);
    int (*memcpy_to_device_sq)(struct devdrv_info *dev_info, u64 dst, u64 src, u32 buf_len);
};

struct devdrv_sq_sub_info {
    u32 index;
    struct uio_info *uio;
    struct list_head list;
    struct list_head list_stream;
    struct list_head list_cq;
    phys_addr_t bar_addr;
    phys_addr_t phy_addr;
};

struct devdrv_cq_sub_info {
    u32 index;
    struct uio_info *uio;
    struct list_head list;
    struct list_head list_sq;
    struct devdrv_cce_context *cce_context;
    spinlock_t spinlock; /*
                          * use for avoid the problem:
                          * tasklet(devdrv_find_cq_index) may access cq's uio mem,
                          * there is a delay time, between set cq's uio invalid and accessing cq's uio mem by tasklet.
                          */
    phys_addr_t virt_addr;
    phys_addr_t phy_addr;
    u32 size;
    struct device *dev;
};

struct devdrv_stream_sub_info {
    int id;
    struct list_head list;
    struct list_head sq_list;
    struct devdrv_cce_context *cce_context;
};

struct devdrv_init_client {
    int (*register_client)(struct devdrv_info *dev_info);
    void (*unregister_client)(struct devdrv_info *dev_info);
    int (*manager_suspend)(struct devdrv_info *dev_info);
    int (*manager_resume)(struct devdrv_info *dev_info);
};

struct devdrv_client_info {
    u32 init;        /* upper layer driver is initialized or not */
    u32 client_init; /* devdrv_manager_register is called or not */
    void *priv;
    struct devdrv_init_client dev_client;
};

#define DEVDRV_SQ_CQ_MAP 0
#define DEVDRV_INFO_MAP 0
#define DEVDRV_DOORBELL_MAP 1

u32 *devdrv_doorbell_get_cq_head(struct devdrv_info *info, u32 tsid, u32 index);
struct devdrv_info *devdrv_get_devdrv_info(u32 devid);

#define devdrv_calc_sq_info(addr, index)                                                                            \
    ({                                                                                                              \
        struct devdrv_ts_sq_info *sq;                                                                               \
        sq = (struct devdrv_ts_sq_info *)((uintptr_t)((addr) +                                                        \
                                                      (long)(unsigned)sizeof(struct devdrv_ts_sq_info) * (index))); \
        sq;                                                                                                         \
    })

#define devdrv_calc_cq_info(addr, index)                                                                            \
    ({                                                                                                              \
        struct devdrv_ts_cq_info *cq;                                                                               \
        cq = (struct devdrv_ts_cq_info *)((uintptr_t)((addr) + DEVDRV_SQ_INFO_OCCUPY_SIZE +                           \
                                                      (long)(unsigned)sizeof(struct devdrv_ts_cq_info) * (index))); \
        cq;                                                                                                         \
    })

#define devdrv_calc_stream_info(addr, index)                                                                           \
    ({                                                                                                                 \
        struct devdrv_stream_info *stream_info;                                                                        \
        stream_info =                                                                                                  \
            (struct devdrv_stream_info *)((uintptr_t)((addr) + DEVDRV_SQ_INFO_OCCUPY_SIZE + \
                                                      DEVDRV_CQ_INFO_OCCUPY_SIZE + \
                                                      (long)(unsigned)sizeof(struct devdrv_stream_info) * (index)));   \
        stream_info;                                                                                                   \
    })

#define devdrv_report_get_phase(report) (((report)->c & DEVDRV_REPORT_PHASE) >> 15);

extern int memset_s(void *dest, size_t destMax, int c, size_t count);
extern int memcpy_s(void *dest, size_t destMax, const void *src, size_t count);
extern int strcpy_s(char *strDest, size_t destMax, const char *strSrc);
extern int strcat_s(char *strDest, size_t destMax, const char *strSrc);

int copy_from_user_safe(void *to, const void __user *from, unsigned long n);
int copy_to_user_safe(void __user *to, const void *from, unsigned long n);

#define DRV_PRINT_START(args...)    \
        devdrv_drv_debug("enter %s: %.4d.\n", __func__, __LINE__);

#define DRV_PRINT_END(args...)      \
        devdrv_drv_debug("exit %s: %.4d.\n", __func__, __LINE__);

#define DRV_CHECK_RET(expr, fmt, ...) do {      \
    if (expr) {                                 \
        devdrv_drv_err(fmt, ##__VA_ARGS__);     \
    }                                           \
} while (0)

#define DRV_CHECK_EXP_ACT(expr, action, fmt, ...) do {  \
    if (expr) {                                         \
        devdrv_drv_err(fmt, ##__VA_ARGS__);             \
        action;                                         \
    }                                                   \
} while (0)

#define DRV_CHECK_PTR(p, action, fmt, ...) do { \
    if ((p) == NULL) {                          \
        devdrv_drv_err(fmt, ##__VA_ARGS__);     \
        action;                                 \
    }                                           \
} while (0)

#define DRV_PRINT_INFO(fmt, ...) do {           \
    devdrv_drv_info(fmt, ##__VA_ARGS__);        \
} while (0)

#define DRV_PRINT_DBG(fmt, ...) do {            \
    devdrv_drv_debug(fmt, ##__VA_ARGS__);       \
} while (0)

#endif /* __DEVDRV_ID_COMMON_H */
