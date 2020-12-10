/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2012-2019. All rights reserved.
 * Description:
 * Author: huawei
 * Create: 2019/6/18
 *
 * This program is free software; you can redistribute it and /or modify it
 * under the terms of the GNU General Public License as published by the Free
 * Software Foundation; either version 2 of the License, or (at your option)
 * any later version
 */

#ifndef _HDCDRV_MAIN_H_
#define _HDCDRV_MAIN_H_

#include <linux/mutex.h>
#include <linux/spinlock.h>
#include <linux/cdev.h>
#include <linux/semaphore.h>
#include <linux/wait.h>
#include <linux/rbtree.h>
#include <linux/sched.h>
#include <linux/cred.h>
#include <linux/gfp.h>

#include "drv_log.h"
#include "devdrv_interface.h"
#include "hdcdrv_cmd.h"
#include "hdcdrv_epoll.h"
#include "hdcdrv_mem.h"

#ifdef HDC_ENV_DEVICE
#include <linux/securec.h>
#else
#include "securec.h"
#endif

#define module_hdcdrv "hdcdrv"

#define PCIE_DEV_NAME "/dev/hisi_hdc"

#define hdcdrv_err_limit(fmt, ...) do { \
    if (printk_ratelimit()) \
        drv_err(module_hdcdrv, "<%s:%d> " fmt, current->comm, current->tgid, ##__VA_ARGS__); \
} while (0);
#define hdcdrv_warn_limit(fmt, ...) do { \
    if (printk_ratelimit()) \
        drv_warn(module_hdcdrv, "<%s:%d> " fmt, current->comm, current->tgid, ##__VA_ARGS__); \
} while (0);
#define hdcdrv_info_limit(fmt, ...) do { \
    if (printk_ratelimit()) \
        drv_info(module_hdcdrv, "<%s:%d> " fmt, current->comm, current->tgid, ##__VA_ARGS__); \
} while (0);
#define hdcdrv_err(fmt, ...) do { \
    drv_err(module_hdcdrv, "<%s:%d> " fmt, current->comm, current->tgid, ##__VA_ARGS__); \
} while (0);
#define hdcdrv_warn(fmt, ...) do { \
    drv_warn(module_hdcdrv, "<%s:%d> " fmt, current->comm, current->tgid, ##__VA_ARGS__); \
} while (0);
#define hdcdrv_info(fmt, ...) do { \
    drv_info(module_hdcdrv, "<%s:%d> " fmt, current->comm, current->tgid, ##__VA_ARGS__); \
} while (0);
#define hdcdrv_critical_info(fmt, ...) do { \
    drv_err(module_hdcdrv, "<%s:%d> " fmt, current->comm, current->tgid, ##__VA_ARGS__); \
} while (0);

#ifdef HDC_MEM_RB_DFX
#define hdcdrv_dfx_node_insert(root, hash_va, flag) do { \
    hdcdrv_dfx_node_insert_fn(root, hash_va, flag); \
} while (0);
#define hdcdrv_dfx_node_upgrade(root, hash_va, state) do { \
    hdcdrv_dfx_node_upgrade_fn(root, hash_va, state); \
} while (0);
#define hdcdrv_dfx_node_free(root, flag) do { \
    hdcdrv_dfx_node_free_fn(root, flag); \
} while (0);
#define hdcdrv_dfx_node_info(root, hash_va) do { \
    hdcdrv_dfx_node_info_fn(root, hash_va); \
} while (0);
#define hdcdrv_dfx_node_uninit(root) do { \
    hdcdrv_dfx_node_uninit_fn(root); \
} while (0);
#define hdcdrv_dfx_node_init() do { \
    hdcdrv_dfx_node_init_fn(); \
} while (0);
#define hdcdrv_dfx_node_cancel_work(work) do { \
    cancel_delayed_work_sync(work); \
} while (0);

#else
#define hdcdrv_dfx_node_insert(root, hash_va, flag)
#define hdcdrv_dfx_node_upgrade(root, hash_va, state)
#define hdcdrv_dfx_node_free(root, flag)
#define hdcdrv_dfx_node_info(root, hash_va)
#define hdcdrv_dfx_node_uninit(root)
#define hdcdrv_dfx_node_init()
#define hdcdrv_dfx_node_cancel_work(work)
#endif

#define HDCDRV_STR_NAME(n)      (#n)
#define HDCDRV_STR_NAME_LEN     32

#define HDCDRV_VALID 1
#define HDCDRV_INVALID 0

#define HDCDRV_SESSION_FD_INVALID -1

#define HDCDRV_RUNNING_ENV_ARM_3559 0
#define HDCDRV_RUNNING_ENV_X86_NORMAL 1
#define HDCDRV_RUNNING_ENV_DAVICI 2

#define HDCDRV_MODE_KERNEL 0
#define HDCDRV_MODE_USER 1

#define HDCDRV_SERVICE_HIGH_LEVEL 0
#define HDCDRV_SERVICE_LOW_LEVEL 1

#define HDCDRV_SERVICE_SHORT_CONN	0
#define HDCDRV_SERVICE_LONG_CONN	1

#define HDCDRV_DESC_QUEUE_DEPTH	        16

#define HDCDRV_NON_TRANS_MSG_S_DESC_SIZE 0x200000 /* 2M */
#define HDCDRV_NON_TRANS_MSG_C_DESC_SIZE 0x400 /* 1k */

#define HDCDRV_RX_BUDGET 16
#define HDCDRV_TX_BUDGET 16

#define HDCDRV_SESSION_RX_LIST_MAX_PKT 8

#define HDCDRV_SESSION_STATUS_IDLE 0
#define HDCDRV_SESSION_STATUS_CONN 1
#define HDCDRV_SESSION_STATUS_REMOTE_CLOSED 2
#define HDCDRV_SESSION_STATUS_CLOSING 3
#define HDCDRV_SESSION_STATUS_MAX 4


#define HDCDRV_MSG_TX_STATUS_FULL 1
#define HDCDRV_MSG_TX_STATUS_EMPTY 0

#define HDCDRV_MSG_CHAN_TYPE_NORMAL 0
#define HDCDRV_MSG_CHAN_TYPE_FAST 1

/* msg chan cnt for 3559 host */
#define HDCDRV_NORMAL_MSG_CHAN_CNT 3
#define HDCDRV_PACKET_SEGMENT (32 * 1024)
#define HDCDRV_INVALID_PACKET_SEGMENT (-1)
/* ctrl msy type */
#define HDCDRV_CTRL_MSG_TYPE_CONNECT 1
#define HDCDRV_CTRL_MSG_TYPE_CONNECT_REPLY 2
#define HDCDRV_CTRL_MSG_TYPE_CLOSE 3
#define HDCDRV_CTRL_MSG_TYPE_SYNC 4
#define HDCDRV_CTRL_MSG_TYPE_RESET 5
#define HDCDRV_CTRL_MSG_TYPE_CHAN_SET 6
#define HDCDRV_CTRL_MSG_TYPE_SYNC_MEM_INFO 7

#define HDCDRV_RX_DEFAULT_TIMEOUT (3 * HZ)
#define HDCDRV_TX_DEFAULT_TIMEOUT (3 * HZ)
#define HDCDRV_INVALID_VALUE -1

#define HDCDRV_SMALL_PACKET_SEGMENT (4 * 1024)
#define HDCDRV_HUGE_PACKET_SEGMENT (512 * 1024)
#define HDCDRV_SMALL_PACKET_NUM 1024 /* power of 2 */
#define HDCDRV_HUGE_PACKET_NUM 64    /* power of 2 */

#define HDCDRV_BUF_MAX_CNT 16

#define HDCDRV_SET_PID_TIMEOUT 10
#define HDCDRV_KERNEL_DEFAULT_PID -2
#define HDCDRV_KERNEL_UNKNOW_PID -3

#define HDCDRV_DELAY_REMOTE_CLOSE_BIT 0
#define HDCDRV_DELAY_UNKNOWN_SESSION_BIT 1

#define HDCDRV_DEBUG_MODE_TIMEOUT (600 * HZ)

#define HDCDRV_ADD_FLAG 0
#define HDCDRV_DEL_FLAG 1
#define HDCDRV_FALSE_FLAG 0
#define HDCDRV_TRUE_FLAG 1

#define HDCDRV_RBTREE_PID 16
#define HDCDRV_RBTREE_PID_MASK 0xFFFFFFFFFFFF0000ull
#define HDCDRV_RBTREE_ADDR_MASK 0x0000FFFFFFFFFFFFull

#define HDCDRV_MEM_POOL_TYPE_TX 0
#define HDCDRV_MEM_POOL_TYPE_RX 1
#define HDCDRV_MEM_POOL_TYPE_NUM 2

#define HDCDRV_CLOSE_TYPE_NONE 0
#define HDCDRV_CLOSE_TYPE_USER 1
#define HDCDRV_CLOSE_TYPE_KERNEL 2
#define HDCDRV_CLOSE_TYPE_RELEASE 3
#define HDCDRV_CLOSE_TYPE_NOT_SET_OWNER 4
#define HDCDRV_CLOSE_TYPE_REMOTE_CLOSED_PRE 5
#define HDCDRV_CLOSE_TYPE_REMOTE_CLOSED_POST 6
#define HDCDRV_CLOSE_TYPE_MAX 7

#define HDCDRV_RX 0
#define HDCDRV_TX 1

#define HDCDRV_NODE_IDLE            0
#define HDCDRV_NODE_BUSY            1

#ifdef CFG_SOC_PLATFORM_ESL_FPGA
#define HDCDRV_NODE_BUSY_WARING     2000 /* 2s for fpga */
#define HDCDRV_NODE_BUSY_TIMEOUT    50000 /* 50s for fpga */

#define HDCDRV_NODE_WAIT_TIME_MIN   0
#define HDCDRV_NODE_WAIT_TIME_MAX   30000 /* 30s for fpga */

#define HDCDRV_SESSION_DEFAULT_TIMEOUT 120000 /* 120s for fpga */
#define HDCDRV_SESSION_REMOTE_CLOSED_TIMEOUT (10 * HZ) /* 10s for fpga */
#define HDCDRV_CONN_TIMEOUT (100 * HZ) /* 100s for fpga */
#define HDCDRV_SESSION_RECLAIM_TIMEOUT (150 * HZ) /* 150s for fpga */

#define HDCDRV_TASKLET_STATUS_CHECK_TIME 10 /* 10s for fpga */
#else
#define HDCDRV_NODE_BUSY_WARING     100 /* ms */
#define HDCDRV_NODE_BUSY_TIMEOUT    5000 /* ms */

#define HDCDRV_NODE_WAIT_TIME_MIN   0
#define HDCDRV_NODE_WAIT_TIME_MAX   3000 /* ms */

#define HDCDRV_SESSION_DEFAULT_TIMEOUT 3000 /* ms */
#define HDCDRV_SESSION_REMOTE_CLOSED_TIMEOUT (3 * HZ) /* 3s */
#define HDCDRV_CONN_TIMEOUT (30 * HZ) /* 30s */
#define HDCDRV_SESSION_RECLAIM_TIMEOUT (60 * HZ) /* 60s */

#define HDCDRV_TASKLET_STATUS_CHECK_TIME 3
#endif

#define HDCDRV_MEM_DFX_LOCAL 0
#define HDCDRV_MEM_DFX_REMOTE 1
#define HDCDRV_MEM_DFX_DEFAULT_TIME 1800000 /* 30 min */
#define HDCDRV_MEM_DFX_ALLOC_USED 1
#define HDCDRV_MEM_DFX_FREE_USER 2
#define HDCDRV_MEM_DFX_FREE_RELEASE 3
#define HDCDRV_MEM_DFX_REMOTE_FREE 4
#define HDCDRV_INIT_INSTANCE_TIMEOUT (4 * HZ)

#define HDCDRV_SHOW_PKT_LEN 128
#define HDCDRV_SHOW_PKT_RX_NUM 50

#define HDCDRV_SEND_ALLOC_MEM_RETRY_TIME 100

#define HDCDRV_DEV_MODE 0640

#define HDCDRV_RECYCLE_DELAY_TIME 10

#ifndef __GFP_ACCOUNT

#ifdef __GFP_KMEMCG
#define __GFP_ACCOUNT __GFP_KMEMCG /* for linux version 3.10 */
#endif

#ifdef __GFP_NOACCOUNT
#define __GFP_ACCOUNT 0 /* for linux version 4.1 */
#endif

#endif


/* dma payload is 128 bytes, cq/sq must aligned with 16/32/64/128 */
/* sq cq description */
struct hdcdrv_sq_desc {
    int local_session;
    int remote_session;
    u64 src_data_addr;
    u64 dst_data_addr;
    u64 src_ctrl_addr;
    u64 dst_ctrl_addr;
    int data_len;
    int ctrl_len;
    u32 resv[2];
    u32 inner_checker;
    u32 valid;
} __attribute__((aligned(64)));

#define HDCDRV_SQ_DESC_SIZE sizeof(struct hdcdrv_sq_desc)

struct hdcdrv_cq_desc {
    u32 status;
    u32 sq_head;
    int session;
    u32 valid;
} __attribute__((aligned(16)));

#define HDCDRV_CQ_DESC_SIZE sizeof(struct hdcdrv_cq_desc)

/* ctrl msg */
struct hdcdrv_ctrl_msg_sync {
    int segment;
    int peer_dev_id;
};

struct hdcdrv_ctrl_msg_connect {
    int service_type;
    int client_session;
    int msg_chan_id;
    char session_id[HDCDRV_SID_LEN];
    int run_env;
    int euid;
    int uid;
    int root_privilege;
};

struct hdcdrv_ctrl_msg_connect_reply {
    int client_session;
    int server_session;
    char session_id[HDCDRV_SID_LEN];
    int run_env;
};

struct hdcdrv_ctrl_msg_close {
    int local_session;
    int remote_session;
    int session_close_state;
    char session_id[HDCDRV_SID_LEN];
};

struct hdcdrv_ctrl_msg_chan_set {
    int normal_chan_num;
};

struct hdcdrv_mem_f {
    void *buf;
    dma_addr_t addr;
    struct page *page;
    u32 len;
    u32 power;
    u32 type;
};
struct hdcdrv_fast_mem {
    int phy_addr_num;
    u32 alloc_len;
    int mem_type;
    u32 page_type;
    int dma_map;
    int devid;
    u32 free_flag;
    u64 user_va;
    u64 hash_va;
    struct hdcdrv_mem_f *mem;
};
struct hdcdrv_dma_mem {
    u64 addr;
    u32 len;
    u32 resv;
};

struct hdcdrv_ctrl_msg_sync_mem_info {
    int type;
    int error_code;
    int flag;
    int phy_addr_num;
    int mem_type;
    u32 alloc_len;
    u64 hash_va;
    long long pid;
    struct hdcdrv_dma_mem mem[0];
};

/* reserved 1k mem for non trans msg head and hdc ctrl msg head */
#define HDCDRV_MEM_MAX_PHY_NUM ((int)((HDCDRV_NON_TRANS_MSG_S_DESC_SIZE - 0x400) / sizeof(struct hdcdrv_dma_mem)))
#define HDCDRV_MAX_DMA_NODE HDCDRV_MEM_MAX_PHY_NUM

struct hdcdrv_ctrl_msg {
    int type;
    int error_code;
    union {
        struct hdcdrv_ctrl_msg_sync sync_msg;
        struct hdcdrv_ctrl_msg_connect connect_msg;
        struct hdcdrv_ctrl_msg_connect_reply connect_msg_reply;
        struct hdcdrv_ctrl_msg_close close_msg;
        struct hdcdrv_ctrl_msg_chan_set chan_set_msg;
    };
};

struct hdcdrv_buf_desc {
    void *buf;
    u64 addr;  // src_data_addr
    int len;   // data_len
    int ctrl_len;

    u64 dst_data_addr;
    u64 src_ctrl_addr;
    u64 dst_ctrl_addr;

    int status;
    int skip_flag;
    int local_session;
    int remote_session;

    u32 inner_checker;

    struct hdcdrv_fast_mem *src_data;
    struct hdcdrv_fast_mem *dst_data;
    struct hdcdrv_fast_mem *src_ctrl;
    struct hdcdrv_fast_mem *dst_ctrl;
};

struct hdcdrv_recv_buf_desc_list {
    struct hdcdrv_buf_desc buf_d;
    struct hdcdrv_recv_buf_desc_list *next;
};

struct hdcdrv_connect_list {
    int session_fd;
    struct hdcdrv_connect_list *next;
};

struct hdcdrv_service {
    int listen_status;
    u64 accept_session_num;
    u64 connect_session_num_total;
    u64 connect_session_num;
    u64 close_session_num;
    long long listen_pid;
    wait_queue_head_t wq_conn_avail;
    struct hdcdrv_connect_list *conn_list_head;
    struct hdcdrv_stats stat;
    struct hdcdrv_ctx *ctx;
    struct hdcdrv_epoll_fd *epfd;
    struct mutex mutex;
};

struct hdcdrv_msg_chan_tasklet_status {
    unsigned long long schedule_in;
    unsigned long long schedule_in_last;
    int no_schedule_cnt;
};

struct hdcdrv_msg_chan {
    int chan_id;
    u32 dev_id;
    int type;
    void *chan;
    int rx_trigger_flag;
    int submit_dma_head;
    int dma_head;
    int rx_head;
    int session_cnt;
    int sq_head;
    int dma_need_submit_flag;
    enum devdrv_dma_data_type data_type;
    struct hdcdrv_stats stat;
    struct hdcdrv_dbg_stats dbg_stat;
    struct tasklet_struct tx_finish_task;
    struct tasklet_struct rx_notify_task;
    struct tasklet_struct rx_task;
    struct hdcdrv_msg_chan_tasklet_status tx_finish_task_status;
    struct hdcdrv_msg_chan_tasklet_status rx_notify_task_status;
    struct hdcdrv_msg_chan_tasklet_status rx_task_status;
    struct hdcdrv_buf_desc tx[HDCDRV_DESC_QUEUE_DEPTH];
    struct hdcdrv_buf_desc rx[HDCDRV_DESC_QUEUE_DEPTH];
    wait_queue_head_t send_wait;
    struct list_head wait_mem_list;
    struct device *dev;
    struct devdrv_dma_node node[HDCDRV_MAX_DMA_NODE];
    struct mutex mutex;
};

struct hdcdrv_dev {
    u32 valid;
    u32 dev_id;
    u32 peer_dev_id;
    int normal_chan_num;
    int msg_chan_cnt;
    struct hdcdrv_service service[HDCDRV_SURPORT_MAX_SERVICE];
    struct hdcdrv_msg_chan *msg_chan[HDCDRV_SURPORT_MAX_DEV_MSG_CHAN];
    void *ctrl_msg_chan;
    void *sync_mem_buf;
    struct mutex sync_mem_mutex;
    struct rb_root remote_rb_mems;
    struct rb_root remote_mem_rb_mems;
    struct delayed_work init;
    struct device *dev;
    struct mutex mutex;
    struct hdcdrv_mem_pool huge_mem_pool[HDCDRV_MEM_POOL_TYPE_NUM];
    struct hdcdrv_mem_pool small_mem_pool[HDCDRV_MEM_POOL_TYPE_NUM];
    struct semaphore hdc_instance_sem;
};

struct hdcdrv_fast_rx {
    u64 data_addr;
    u64 ctrl_addr;
    int data_len;
    int ctrl_len;
};

struct hdcdrv_session_fast_rx {
    int head;
    int tail;
    struct hdcdrv_fast_rx rx_list[HDCDRV_BUF_MAX_CNT];
};

struct hdcdrv_session_normal_rx {
    int head;
    int tail;
    struct hdcdrv_buf_desc rx_list[HDCDRV_SESSION_RX_LIST_MAX_PKT];
};

struct hdcdrv_timeout_jif {
    unsigned long long send_timeout;
    unsigned long long recv_timeout;
    unsigned long long fast_send_timeout;
    unsigned long long fast_recv_timeout;
};


struct hdcdrv_session {
    atomic_t status;
    char session_id[HDCDRV_SID_LEN];
    u32 inner_checker;
    int local_session_fd;
    int remote_session_fd;
    int dev_id;
    int service_type;
    u32 chan_id;
    u32 fast_chan_id;
    u32 delay_work_flag;
    int run_env;
    long long pid;
    int remote_close_state;
    int local_close_state;
    int euid;
    int uid;
    int root_privilege;
    u32 container_id;
    struct hdcdrv_timeout_jif timeout_jiffies;
    struct hdcdrv_msg_chan *msg_chan;
    struct hdcdrv_msg_chan *fast_msg_chan;
    struct hdcdrv_service *service;
    struct hdcdrv_ctx *ctx;
    struct hdcdrv_epoll_fd *epfd;
    struct hdcdrv_stats stat;
    wait_queue_head_t wq_conn;
    wait_queue_head_t wq_rx;
    struct delayed_work remote_close;
    struct delayed_work close_unknow_session;
    spinlock_t lock;
    struct hdcdrv_session_normal_rx normal_rx;
    struct hdcdrv_session_fast_rx fast_rx;
};

struct hdcdrv_mem_fd_list {
    struct hdcdrv_ctx *ctx;
    struct hdcdrv_fast_node *f_node;
    struct list_head list;
};

struct hdcdrv_ctx {
    int dev_id;
    int service_type;
    char session_id[HDCDRV_SID_LEN];
    int session_fd;
    long long pid;
    u32 count;

    struct file *file;
    struct hdcdrv_session *session;
    struct hdcdrv_service *service;
    struct hdcdrv_mem_fd_list mlist;

    struct hdcdrv_epoll_fd *epfd;

    spinlock_t mem_lock;
};

#define HDCDRV_KERNEL_WITHOUT_CTX (struct hdcdrv_ctx *)-4

struct hdcdrv_fast_node {
    struct rb_node node;
    u64 hash_va;
    long long pid;
    u32 stamp;
    u32 max_cost;
    atomic_t status;
    struct hdcdrv_mem_fd_list *mem_fd_node;
    struct hdcdrv_fast_mem fast_mem;  // must be last one
};

#ifdef HDC_MEM_RB_DFX
struct hdcdrv_mem_dfx_node {
    struct rb_node node;
    u64 hash_va;
    long long pid;
    u64 dfx_current_time;
    int mem_rbtree_state;
};
#endif

struct hdcdrv_debug {
    long long valid;
    long long pid;
};

struct hdcdrv_mem_list_entry {
    u32 devid;
    u64 hash_va;
    struct list_head list;
};

struct hdcdrv_mem_list {
    u32 list_len;
    struct hdcdrv_mem_list_entry list_entry;
    spinlock_t lock;
};

struct hdcdrv_mem_dfx_stat {
    u64 alloc_cnt;
    u64 alloc_size;
    u64 alloc_normal_size;
    u64 alloc_dma_size;
    u64 free_cnt;
    u64 free_size;
    spinlock_t lock;
};

struct hdcdrv_ctrl {
    struct cdev cdev;
    dev_t dev_no;
    struct class *cdev_class;
    struct delayed_work recycle;
    struct delayed_work recycle_mem;
    int segment;
    int cur_alloc_session;
    int cur_alloc_short_session;
    int service_level[HDCDRV_SURPORT_MAX_SERVICE];
    int service_conn_feature[HDCDRV_SURPORT_MAX_SERVICE];
    struct hdcdrv_dev devices[HDCDRV_SURPORT_MAX_DEV];
    struct hdcdrv_session sessions[HDCDRV_SURPORT_MAX_SESSION];
    struct rb_root rbtree;
    struct rb_root mem_rbtree;
    struct mutex mutex;
    struct hdcdrv_debug debug_state;
    struct hdcdrv_mem_dfx_stat  mem_dfx_stat;
    struct hdcdrv_epoll epolls;
#ifdef HDC_MEM_RB_DFX
    struct delayed_work mem_dfx_cycle;
    spinlock_t dfx_lock;
    u64 dfx_mem_max_count;
    u64 dfx_mem_current_count;
    u64 dfx_mem_remote_max_count;
    u64 dfx_mem_remote_current_count;
#endif
    spinlock_t lock;
};

extern struct hdcdrv_ctrl *hdc_ctrl;

extern int hdcdrv_get_service_level(int service_type);
extern long hdcdrv_session_alive_check(int session_fd, char *session_id);

extern void hdcdrv_mem_avail(int pool_type, struct list_head *target);
extern struct hdcdrv_fast_mem *hdcdrv_get_fast_mem(struct rb_root *root, int type, int len, u64 hash_va, int timeout);
extern long hdcdrv_fast_alloc_mem(struct hdcdrv_ctx *ctx, struct hdcdrv_cmd_alloc_mem *cmd);
extern long hdcdrv_fast_free_mem(struct hdcdrv_cmd_free_mem *cmd);
extern long hdcdrv_fast_dma_map(struct hdcdrv_cmd_dma_map *cmd);
extern long hdcdrv_fast_dma_unmap(struct hdcdrv_cmd_dma_unmap *cmd);
extern long hdcdrv_fast_dma_remap(struct hdcdrv_cmd_dma_remap *cmd);
extern long hdcdrv_dev_para_check(int dev_id, int service_type);
extern long long hdcdrv_get_pid(void);
extern void hdcdrv_fast_mem_uninit(struct rb_root *root, int reset);
extern void hdcdrv_fast_mem_free_abnormal(struct hdcdrv_fast_node *f_node);
extern struct hdcdrv_fast_node *hdcdrv_fast_node_search(struct rb_root *root,
                                                        u64 hash_va, u32 *status, int *node_stamp);
struct hdcdrv_fast_node *hdcdrv_fast_node_search_timeout(struct rb_root *root, u64 hash_va, int timeout);
extern int hdcdrv_fast_node_insert(struct rb_root *root, struct hdcdrv_fast_node *fast_node);
extern void hdcdrv_fast_node_erase(spinlock_t *lock, struct rb_root *root, struct hdcdrv_fast_node *fast_node);
extern void hdcdrv_fast_node_free(struct hdcdrv_fast_node *fast_node);
extern long hdcdrv_bind_mem_ctx(struct hdcdrv_ctx *ctx, struct hdcdrv_fast_node *f_node);
extern void hdcdrv_unbind_mem_ctx(struct hdcdrv_fast_node *f_node);

extern int hdcdrv_get_running_env(void);
extern void hdcdrv_set_device_status(int devid, u32 valid);
extern void hdcdrv_set_device_para(u32 devid, int normal_chan_num);
extern long hdcdrv_session_para_check(int session_fd, char *session_id);
extern long hdcdrv_session_inner_check(int session_fd, u32 checker);
extern int hdcdrv_set_msg_chan_priv(void *msg_chan, void *priv);
extern struct hdcdrv_msg_chan *hdcdrv_get_msg_chan_priv(void *msg_chan);
extern struct hdcdrv_sq_desc *hdcdrv_get_w_sq_desc(void *msg_chan, u32 *tail);
extern void hdcdrv_set_w_sq_desc_head(void *msg_chan, u32 head);
extern void hdcdrv_copy_sq_desc_to_remote(struct hdcdrv_msg_chan *msg_dev, struct hdcdrv_sq_desc *sq_desc,
                                          enum devdrv_dma_data_type data_type);
extern bool hdcdrv_w_sq_full_check(void *msg_chan);
extern struct hdcdrv_sq_desc *hdcdrv_get_r_sq_desc(void *msg_chan, u32 *head);
extern void hdcdrv_move_r_sq_desc(void *msg_chan);
extern enum devdrv_dma_direction hdcdrv_get_dma_direction(void);
extern struct hdcdrv_cq_desc *hdcdrv_get_w_cq_desc(void *msg_chan);
extern void hdcdrv_copy_cq_desc_to_remote(struct hdcdrv_msg_chan *msg_dev, struct hdcdrv_cq_desc *cq_desc,
                                          enum devdrv_dma_data_type data_type);
extern struct hdcdrv_cq_desc *hdcdrv_get_r_cq_desc(void *msg_chan);
extern void hdcdrv_move_r_cq_desc(void *msg_chan);
extern void hdcdrv_fast_free_phy_mem(struct hdcdrv_fast_mem *f_mem);

extern void hdcdrv_rx_msg_notify(void *msg_chan);
extern void hdcdrv_tx_finish_notify(void *msg_chan);

extern long hdcdrv_ctrl_msg_send(u32 devid, void *data, u32 in_data_len, u32 out_data_len, u32 *real_out_len);
extern int hdcdrv_ctrl_msg_recv(u32 devid, void *data, u32 in_data_len, u32 out_data_len, u32 *real_out_len);
extern long hdcdrv_non_trans_ctrl_msg_send(u32 devid, void *data, u32 in_data_len, u32 out_data_len, u32 *real_out_len);

extern struct hdcdrv_dev *hdcdrv_add_dev(struct device *dev, u32 dev_id);
extern int hdcdrv_add_msg_chan_to_dev(u32 dev_id, void *chan);

extern int hdcdrv_add_ctrl_msg_chan_to_dev(u32 dev_id, void *chan);
extern void hdcdrv_free_msg_chan(struct hdcdrv_dev *hdc_dev);
extern void hdcdrv_set_segment(int segment);
extern void hdcdrv_set_peer_dev_id(int dev_id, int peer_dev_id);
extern int hdcdrv_get_peer_dev_id(int dev_id);

extern long hdcdrv_epoll_operation(struct hdcdrv_ctx *ctx, u32 drv_cmd, union hdcdrv_cmd *cmd_data,
                                   bool *copy_to_user_flag);

extern int hdcdrv_init(void);
extern void hdcdrv_uninit(void);

extern void hdcdrv_reset_dev(struct hdcdrv_dev *hdc_dev);
extern void hdcdrv_stop_work(struct hdcdrv_dev *hdc_dev);
extern void hdcdrv_remove_dev(struct hdcdrv_dev *hdc_dev);
extern void hdcdrv_del_dev(u32 dev_id);
extern int hdcdrv_get_session_run_env(void);
extern long hdcdrv_close(struct hdcdrv_cmd_close *cmd, int close_state);
extern int hdcdrv_send_mem_info(struct hdcdrv_fast_mem *mem, int devid, int flag);
extern int hdcdrv_set_mem_info(int devid, struct hdcdrv_ctrl_msg_sync_mem_info *msg);
extern int hdcdrv_mem_adapter(struct hdcdrv_fast_mem *src, struct hdcdrv_fast_mem *dst, struct devdrv_dma_node *node,
                              int *node_num, int len);
extern int hdcdrv_get_dma_node_info(struct hdcdrv_fast_mem *src, struct hdcdrv_fast_mem *dst,
                                    struct hdcdrv_msg_chan *msg_chan, int *node_idx, int len);
extern u64 hdcdrv_get_hash(u64 user_va, u64 pid);
extern u32 hdcdrv_get_mem_list_len(void);
extern bool hdcdrv_mem_is_notify(struct hdcdrv_fast_mem *f_mem);
extern bool hdcdrv_sid_check(char *dst_session_id, int dst_len, char *src_session_id, int src_len);
extern void hdcdrv_sid_gen(char *session_id, int len);
extern void hdcdrv_sid_copy(char *dst_session_id, int dst_len, char *src_session_id, int src_len);
extern void hdcdrv_node_status_busy(struct hdcdrv_fast_node *node);
extern void hdcdrv_node_status_idle(struct hdcdrv_fast_node *node);
extern void hdcdrv_node_status_idle_by_mem(struct hdcdrv_fast_mem *f_mem);
extern bool hdcdrv_node_is_timeout(int node_stamp);
extern bool hdcdrv_node_is_busy(struct hdcdrv_fast_node *node);

#ifdef HDC_MEM_RB_DFX
extern void hdcdrv_dfx_node_info_fn(struct rb_root *root, u64 hash_va);
extern int hdcdrv_dfx_node_insert_fn(struct rb_root *root, u64 hash_va, int flag);
extern void hdcdrv_dfx_node_free_fn(struct rb_root *root, int flag);
extern void hdcdrv_dfx_node_upgrade_fn(struct rb_root *root, u64 hash_va, int state);
extern void hdcdrv_dfx_node_uninit_fn(struct rb_root *root);
extern void hdcdrv_dfx_node_work_fn(struct work_struct *p_work);
extern void hdcdrv_dfx_node_init_fn(void);
#endif

#ifndef HDCDRV_ENV_DEVICE
extern int hdcdrv_container_vir_to_phs_devid(u32 virtual_devid, u32 *physical_devid);
extern int hdcdrv_check_in_container(void);
#endif
extern u32 hdcdrv_get_container_id(void);

static inline void hdcdrv_set_session_status(struct hdcdrv_session *session, int status)
{
    atomic_set(&session->status, status);
}

static inline int hdcdrv_get_session_status(struct hdcdrv_session *session)
{
    return atomic_read(&session->status);
}
long hdcdrv_kernel_server_create(int dev_id, int service_type);
long hdcdrv_kernel_server_destroy(int dev_id, int service_type);
long hdcdrv_kernel_accept(int dev_id, int service_type, int *session, char *session_id);
long hdcdrv_kernel_close(int session, char *session_id);
long hdcdrv_kernel_send_timeout(int session, char *session_id, void *buf, int len, int timeout);
int hdcdrv_get_segment(void);

#endif  // _DEVDRV_MAIN_H_
