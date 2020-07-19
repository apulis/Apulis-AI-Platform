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

#ifndef _HDCDRV_CMD_H_
#define _HDCDRV_CMD_H_

#define HDCDRV_CHAR_DRIVER_NAME "hisi_hdc"

#define HDCDRV_SERVICE_TYPE_DMP 0
#define HDCDRV_SERVICE_TYPE_MATRIX 1
#define HDCDRV_SERVICE_TYPE_IDE1 2
#define HDCDRV_SERVICE_TYPE_FILE_TRANS 3
#define HDCDRV_SERVICE_TYPE_IDE2 4
#define HDCDRV_SERVICE_TYPE_LOG 5
#define HDCDRV_SERVICE_TYPE_RDMA 6
#define HDCDRV_SERVICE_TYPE_BBOX 7
/* the follow used in cloud */
#define HDCDRV_SERVICE_TYPE_FRAMEWORK 8
#define HDCDRV_SERVICE_TYPE_TSD 9
#define HDCDRV_SERVICE_TYPE_TDT 10
#define HDCDRV_SERVICE_TYPE_DEBUG 11
#define HDCDRV_SERVICE_TYPE_USER1 12
#define HDCDRV_SERVICE_TYPE_USER2 13
#define HDCDRV_SERVICE_TYPE_USER3 14 // use for test
#define HDCDRV_SERVICE_TYPE_USER4 15
#define HDCDRV_SURPORT_MAX_SERVICE 16

#ifdef HDC_ENV_DEVICE
#ifdef CFG_SOC_PLATFORM_CLOUD
#define HDCDRV_SURPORT_MAX_DEV 4
#else
#ifdef CFG_SOC_PLATFORM_MDC_V51
#define HDCDRV_SURPORT_MAX_DEV 2
#else
#define HDCDRV_SURPORT_MAX_DEV 1
#endif
#endif

#define HDCDRV_SURPORT_MAX_SESSION (64 * HDCDRV_SURPORT_MAX_DEV)
#define HDCDRV_SURPORT_MAX_LONG_SESSION      (56 * HDCDRV_SURPORT_MAX_DEV)
#else
#define HDCDRV_SURPORT_MAX_DEV 64
#define HDCDRV_SURPORT_MAX_SESSION 2048
#define HDCDRV_SURPORT_MAX_LONG_SESSION      1792
#endif

#define HDCDRV_SURPORT_MAX_SHORT_SESSION      (HDCDRV_SURPORT_MAX_SESSION - HDCDRV_SURPORT_MAX_LONG_SESSION)

#define HDCDRV_SURPORT_MAX_DEV_MSG_CHAN 64
#ifdef CFG_SOC_PLATFORM_CLOUD
#define HDCDRV_SURPORT_MAX_DEV_NORMAL_MSG_CHAN HDCDRV_SURPORT_MAX_SERVICE
#else
#define HDCDRV_SURPORT_MAX_DEV_NORMAL_MSG_CHAN 8
#endif

#define HDCDRV_SID_LEN 32

#define HDCDRV_OK 0
#define HDCDRV_ERR -1
#define HDCDRV_PARA_ERR -2
#define HDCDRV_COPY_FROM_USER_FAIL -3
#define HDCDRV_COPY_TO_USER_FAIL -4
#define HDCDRV_SERVICE_LISTENING -5
#define HDCDRV_SERVICE_NO_LISTENING -6
#define HDCDRV_SERVICE_ACCEPTING -7
#define HDCDRV_DMA_MEM_ALLOC_FAIL -8
#define HDCDRV_NO_SESSION -9
#define HDCDRV_SEND_CTRL_MSG_FAIL -10
#define HDCDRV_REMOTE_REFUSED_CONNECT -11
#define HDCDRV_CONNECT_TIMEOUT -12
#define HDCDRV_TX_QUE_FULL -13
#define HDCDRV_TX_LEN_ERR -14
#define HDCDRV_TX_REMOTE_CLOSE -15
#define HDCDRV_RX_BUF_SMALL -16
#define HDCDRV_DEVICE_NOT_READY -17
#define HDCDRV_DEVICE_RESET -18
#define HDCDRV_NOT_SURPORT -19
#define HDCDRV_REMOTE_SERVICE_NO_LISTENING -20
#define HDCDRV_NO_BLOCK -21
#define HDCDRV_SESSION_HAS_CLOSED -22
#define HDCDRV_MEM_NOT_MATCH -23
#define HDCDRV_CONV_FAILED -24
#define HDC_LOW_POWER_STATE -25
#define HDCDRV_NO_EPOLL_FD -26
#define HDCDRV_RX_TIMEOUT -27
#define HDCDRV_TX_TIMEOUT -28
#define HDCDRV_DMA_MEM_ISUSED -29
#define HDCDRV_SESSION_ID_MISS_MATCH -30
#define HDCDRV_MEM_ALLOC_FAIL -31
#define HDCDRV_SQ_DESC_NULL -32
#define HDCDRV_F_NODE_SEARCH_FAIL -33
#define HDCDRV_DMA_COPY_FAIL -34
#define HDCDRV_SAFE_MEM_OP_FAIL -35
#define HDCDRV_CHAR_DEV_CREAT_FAIL -36
#define HDCDRV_DMA_MPA_FAIL -37
#define HDCDRV_FIND_VMA_FAIL -38
#define HDCDRV_DMA_QUE_FULL -39

/* if add a new error code need to add the same str in g_errno_str */
#define HDCDRV_NOWAIT 0
#define HDCDRV_WAIT_ALWAYS 1
#define HDCDRV_WAIT_TIMEOUT 2

#define HDCDRV_CMD_MAGIC 'H'

#define HDCDRV_MODE_DEFAULT 0
#define HDCDRV_MODE_CONTAINER 1

#define HDCDRV_SESSION_RUN_ENV_UNKNOW 0
#define HDCDRV_SESSION_RUN_ENV_HOST 1
#define HDCDRV_SESSION_RUN_ENV_CONTAINER 2

#define HDCDRV_MEM_MAX_LEN (512 * 1024 * 1024)
#define HDCDRV_CTRL_MEM_MAX_LEN (256 * 1024)
#define HDCDRV_MEM_MIN_LEN (4 * 1024)
#define HDCDRV_MEM_CACHE_LIMIT (64 * 1024)
#define HDCDRV_MEM_MAX_NUM (HDCDRV_MEM_MAX_LEN / HDCDRV_MEM_MIN_LEN)
#define HDCDRV_MEM_MAX_LEN_BIT 22      /* 4M */
#define HDCDRV_MEM_1MB_LEN_BIT 20      /* 1M */
#define HDCDRV_MEM_512KB_LEN_BIT 19    /* 512k */
#define HDCDRV_MEM_MIN_LEN_BIT 18      /* 256k */
#define HDCDRV_MEM_64KB_LEN_BIT 16      /* 64kb */

#define HDCDRV_MEM_ORDER_1MB 8      /* 1M */


#define HDCDRV_MEM_MIN_PAGE_LEN_BIT PAGE_SHIFT
#define HDCDRV_MEM_MIN_HUGE_PAGE_LEN_BIT HPAGE_SHIFT

#define HDCDRV_MEM_ORDER_NUM 11 /* 4M order is 10 */
#define HDCDRV_MEM_SCORE_SCALE 100

#define HDCDRV_FAST_MEM_TYPE_TX_DATA 0
#define HDCDRV_FAST_MEM_TYPE_TX_CTRL 1
#define HDCDRV_FAST_MEM_TYPE_RX_DATA 2
#define HDCDRV_FAST_MEM_TYPE_RX_CTRL 3
#define HDCDRV_FAST_MEM_TYPE_DVPP 4
#define HDCDRV_FAST_MEM_TYPE_MAX 5
#define HDCDRV_PAGE_TYPE_HUGE 1


enum hdcdrv_cmd_type {
    HDCDRV_CMD_GET_PEER_DEV_ID = 0x7,
    HDCDRV_CMD_CONFIG = 0x8,
    HDCDRV_CMD_SET_SERVICE_LEVEL = 0x9,
    HDCDRV_CMD_SERVER_CREATE = 0x10,
    HDCDRV_CMD_SERVER_DESTROY = 0x11,
    HDCDRV_CMD_ACCEPT = 0x12,
    HDCDRV_CMD_CONNECT = 0x13,
    HDCDRV_CMD_CLOSE = 0x14,
    HDCDRV_CMD_SEND = 0x15,
    HDCDRV_CMD_RECV_PEEK = 0x16,
    HDCDRV_CMD_RECV = 0x17,
    HDCDRV_CMD_SET_SESSION_OWNER = 0x18,
    HDCDRV_CMD_GET_STAT = 0x19,
    HDCDRV_CMD_GET_SESSION_RUN_ENV = 0x1a,
    HDCDRV_CMD_SET_SESSION_TIMEOUT = 0x1b,
    HDCDRV_CMD_GET_SESSION_UID = 0x1c,
    HDCDRV_CMD_GET_PAGE_SIZE = 0x1d,
    HDCDRV_CMD_ALLOC_MEM = 0x20,
    HDCDRV_CMD_FREE_MEM = 0x21,
    HDCDRV_CMD_FAST_SEND = 0x22,
    HDCDRV_CMD_FAST_RECV = 0x23,
    HDCDRV_CMD_DMA_MAP = 0x24,
    HDCDRV_CMD_DMA_UNMAP = 0x25,
    HDCDRV_CMD_DMA_REMAP = 0x26,
    HDCDRV_CMD_EPOLL_ALLOC_FD = 0x40,
    HDCDRV_CMD_EPOLL_FREE_FD = 0x41,
    HDCDRV_CMD_EPOLL_CTL = 0x42,
    HDCDRV_CMD_EPOLL_WAIT = 0x43,
    HDCDRV_CMD_MAX
};

struct hdcdrv_cmd_get_peer_dev_id {
    int ret;
    int dev_id;         /* input */
    int peer_dev_id;    /* output */
};

struct hdcdrv_cmd_config {
    int ret;
    int pid;     /* input */
    int segment; /* input,output */
};

struct hdcdrv_cmd_set_service_level {
    int ret;
    int service_type; /* input */
    int level;        /* input */
};

struct hdcdrv_cmd_server_create {
    int ret;
    int dev_id;       /* input */
    int service_type; /* input */
    int pid;          /* input */
};

struct hdcdrv_cmd_server_destroy {
    int ret;
    int dev_id;       /* input */
    int service_type; /* input */
    int pid;          /* input */
};

struct hdcdrv_cmd_accept {
    int ret;
    int dev_id;                      /* input */
    int service_type;                /* input */
    int session;                     /* output */
    char session_id[HDCDRV_SID_LEN]; /* output */
};

struct hdcdrv_cmd_conncet {
    int ret;
    int dev_id;                      /* input */
    int service_type;                /* input */
    int session;                     /* output */
    char session_id[HDCDRV_SID_LEN]; /* output */
};

struct hdcdrv_cmd_close {
    int ret;
    int session;                     /* input */
    char session_id[HDCDRV_SID_LEN]; /* input */
    int pid;                         /* input */
};

struct hdcdrv_cmd_send {
    int ret;
    int session;                     /* input */
    char session_id[HDCDRV_SID_LEN]; /* input */
    void *buf;                       /* input */
    int len;                         /* input */
    int wait_flag;                   /* input */
    unsigned int timeout;            /* input */
};

struct hdcdrv_cmd_fast_send {
    int ret;
    int session;                      /* input */
    char session_id[HDCDRV_SID_LEN];  /* input */
    int wait_flag;                    /* input */
    unsigned long long src_data_addr; /* input */
    unsigned long long dst_data_addr; /* input */
    unsigned long long src_ctrl_addr; /* input */
    unsigned long long dst_ctrl_addr; /* input */
    int data_len;                     /* input */
    int ctrl_len;                     /* input */
    unsigned int timeout;             /* input */
};

struct hdcdrv_cmd_recv_peek {
    int ret;
    int session;                     /* input */
    char session_id[HDCDRV_SID_LEN]; /* input */
    int len;                         /* output, if remote session close, this is 0 */
    int wait_flag;                   /* input */
};

struct hdcdrv_cmd_recv {
    int ret;
    int session;                     /* input */
    char session_id[HDCDRV_SID_LEN]; /* input */
    void *buf;                       /* output */
    int len;                         /* input */
    int out_len;                     /* output */
};

struct hdcdrv_cmd_fast_recv {
    int ret;
    int session;                     /* input */
    char session_id[HDCDRV_SID_LEN]; /* input */
    int wait_flag;                   /* input */
    unsigned long long data_addr;    /* output */
    unsigned long long ctrl_addr;    /* output */
    int data_len;                    /* output */
    int ctrl_len;                    /* output */
};

struct hdcdrv_cmd_set_session_owner {
    int ret;
    int session;                     /* input */
    char session_id[HDCDRV_SID_LEN]; /* input */
    int owner_pid;                   /* input */
};

struct hdcdrv_dbg_stats {
    unsigned long long hdcdrv_msg_chan_send1;
    unsigned long long hdcdrv_rx_msg_notify1;
    unsigned long long hdcdrv_rx_msg_notify_task_check1;
    unsigned long long hdcdrv_rx_msg_notify_task_check2;
    unsigned long long hdcdrv_rx_msg_notify_task1;
    unsigned long long hdcdrv_rx_msg_notify_task2;
    unsigned long long hdcdrv_rx_msg_notify_task3;
    unsigned long long hdcdrv_rx_msg_notify_task4;
    unsigned long long hdcdrv_rx_msg_notify_task5;
    unsigned long long hdcdrv_rx_msg_notify_task6;
    unsigned long long hdcdrv_rx_msg_notify_task7;
    unsigned long long hdcdrv_rx_msg_notify_task8;
    unsigned long long hdcdrv_normal_dma_copy1;
    unsigned long long hdcdrv_rx_msg_callback1;
    unsigned long long hdcdrv_rx_msg_callback2;
    unsigned long long hdcdrv_rx_msg_callback3;
    unsigned long long hdcdrv_rx_msg_task_check1;
    unsigned long long hdcdrv_rx_msg_task_check2;
    unsigned long long hdcdrv_msg_chan_recv_task1;
    unsigned long long hdcdrv_msg_chan_recv_task2;
    unsigned long long hdcdrv_msg_chan_recv_task3;
    unsigned long long hdcdrv_msg_chan_recv_task4;
    unsigned long long hdcdrv_msg_chan_recv_task5;
    unsigned long long hdcdrv_msg_chan_recv_task6;
    unsigned long long hdcdrv_msg_chan_recv_task7;
    unsigned long long hdcdrv_tx_finish_notify1;
    unsigned long long hdcdrv_tx_finish_task_check1;
    unsigned long long hdcdrv_tx_finish_task_check2;
    unsigned long long hdcdrv_tx_finish_notify_task1;
    unsigned long long hdcdrv_tx_finish_notify_task2;
    unsigned long long hdcdrv_tx_finish_notify_task3;
    unsigned long long hdcdrv_tx_finish_notify_task4;
    unsigned long long hdcdrv_tx_finish_notify_task5;
    unsigned long long hdcdrv_tx_finish_notify_task6;
    unsigned long long hdcdrv_tx_finish_notify_task7;
    unsigned long long hdcdrv_mem_avail1;
    unsigned long long hdcdrv_recv_data_times;
};
struct hdcdrv_stats {
    unsigned long long tx;
    unsigned long long tx_bytes;
    unsigned long long rx;
    unsigned long long rx_bytes;
    unsigned long long tx_finish;
    unsigned long long tx_full;
    unsigned long long tx_fail;
    unsigned long long rx_fail;
    unsigned long long rx_full;
    unsigned long long rx_total;
};

struct hdcdrv_cmd_stat_session_brief {
    int active_num;
    int active_list[HDCDRV_SURPORT_MAX_SESSION];
    int remote_close_num;
    int remote_close_list[HDCDRV_SURPORT_MAX_SESSION];
    int accept_num;
    int connect_num;
    int close_num;
};

struct hdcdrv_timeout {
    unsigned int send_timeout;
    unsigned int recv_timeout;
    unsigned int fast_send_timeout;
    unsigned int fast_recv_timeout;
};


struct hdcdrv_mem_stat {
    int mem_nums[HDCDRV_FAST_MEM_TYPE_MAX];
    unsigned long long mem_size[HDCDRV_FAST_MEM_TYPE_MAX];
};

struct hdcdrv_cmd_stat_all {
    int dev_num;
    int dev_list[HDCDRV_SURPORT_MAX_DEV];
    struct hdcdrv_cmd_stat_session_brief s_brief;
};

struct hdcdrv_mem_info {
    unsigned int small_pool_size;
    unsigned int small_pool_remain_size;
    unsigned int huge_pool_size;
    unsigned int huge_pool_remain_size;
};
struct hdcdrv_cmd_stat_dev_service {
    struct hdcdrv_cmd_stat_session_brief s_brief;
    struct hdcdrv_stats stat;
    struct hdcdrv_mem_info tx_mem_info;
    struct hdcdrv_mem_info rx_mem_info;
};

struct hdcdrv_cmd_stat_chan {
    int w_sq_head;
    int w_sq_tail;
    int r_sq_head;
    int dma_head;
    int rx_head;
    int submit_dma_head;
    struct hdcdrv_stats stat;
    struct hdcdrv_dbg_stats dbg_stat;
};
struct hdcdrv_cmd_stat_session {
    int dev_id;
    int service_type;
    int status;
    int fast_chan_id;
    int chan_id;
    int local_session;
    int remote_session;
    int pkts_in_fast_list;
    int pkts_in_list;
    int remote_close_state;
    int local_close_state;
    struct hdcdrv_stats stat;
    struct hdcdrv_timeout timeout;
};

struct hdcdrv_cmd_get_stat {
    int ret;
    int dev_id;       /* input, -1 not care */
    int service_type; /* input, -1 not care */
    int session;      /* input, -1 not care */
    int chan_id;      /* input, -1 not care */
    void *outbuf;     /* output */
};

struct hdcdrv_cmd_get_uid_stat {
        int ret;
        int dev_id;             /* input, -1 not care */
        int session;            /* input, -1 not care */
        char session_id[HDCDRV_SID_LEN];          /* input */
        unsigned int euid;
        unsigned int uid;
        int root_privilege;
};

struct hdcdrv_cmd_alloc_mem {
    int ret;
    unsigned int type;      /* input */
    unsigned int len;       /* input */
    unsigned long long va;  /* input */
    unsigned int page_type; /* input */
    int devid;              /* input */
    int map;                /* input */
};

struct hdcdrv_cmd_free_mem {
    int ret;
    unsigned int type;          /* input */
    unsigned int len;           /* output */
    unsigned int page_type;     /* output */
    unsigned long long va;      /* input */
};

struct hdcdrv_cmd_dma_map {
    int ret;
    unsigned int type;     /* input */
    unsigned long long va; /* input */
    int devid;             /* input */
};

struct hdcdrv_cmd_dma_unmap {
    int ret;
    unsigned int type;     /* input */
    unsigned long long va; /* input */
};

struct hdcdrv_cmd_dma_remap {
    int ret;
    unsigned int type;     /* input */
    unsigned long long va; /* input */
    int devid;             /* input */
};

struct hdcdrv_cmd_result {
    int ret;
};

struct hdcdrv_cmd_epoll_alloc_fd {
    int size;               /* input */
    int epfd;               /* output */
    int magic_num;          /* output */
};

struct hdcdrv_cmd_epoll_free_fd {
    int epfd;               /* input */
    int magic_num;          /* input */
};

struct hdcdrv_event {
    unsigned int events;
    int sub_data;       /* trans to user in epoll wait return */
    unsigned long long data; /* trans to user in epoll wait return */
};

/* same as HDC_EPOLL_OP_* */
#define HDCDRV_EPOLL_OP_ADD 0
#define HDCDRV_EPOLL_OP_DEL 1

/* same as HDC_EPOLL_* */
#define HDCDRV_EPOLL_CONN_IN (0x1 << 0)
#define HDCDRV_EPOLL_DATA_IN (0x1 << 1)
#define HDCDRV_EPOLL_FAST_DATA_IN (0x1 << 2)
#define HDCDRV_EPOLL_SESSION_CLOSE (0x1 << 3)

#define HDCDRV_EPOLL_CTL_PARA_NUM    4

struct hdcdrv_cmd_epoll_ctl {
    int epfd;                   /* input */
    int magic_num;              /* input */
    int op;                     /* input */
    int para1;                  /* input, service:dev_id, session:session_fd */
    char para2[HDCDRV_SID_LEN]; /* input, service:service_type, session:magic_num */
    struct hdcdrv_event event;  /* input */
};

struct hdcdrv_cmd_epoll_wait {
    int epfd;                   /* input */
    int magic_num;              /* input */
    int timeout;                /* input */
    int maxevents;              /* input */
    int ready_event;            /* output */
    struct hdcdrv_event *event;  /* output */
};

struct hdcdrv_cmd_get_session_run_env {
    int session;                     /* input */
    char session_id[HDCDRV_SID_LEN]; /* input */
    int run_env;                     /* output */
};

struct hdcdrv_cmd_set_session_timeout {
    int session;                     /* input */
    char session_id[HDCDRV_SID_LEN]; /* input */
    struct hdcdrv_timeout timeout;   /* input */
};

struct hdcdrv_cmd_get_page_size {
    unsigned int page_size;
    unsigned int hpage_size;
    unsigned int page_bit;
};

union hdcdrv_cmd {
    struct hdcdrv_cmd_result result;
    struct hdcdrv_cmd_get_peer_dev_id get_peer_dev_id;
    struct hdcdrv_cmd_config config;
    struct hdcdrv_cmd_set_service_level set_level;
    struct hdcdrv_cmd_server_create server_create;
    struct hdcdrv_cmd_server_destroy server_destroy;
    struct hdcdrv_cmd_accept accept;
    struct hdcdrv_cmd_conncet conncet;
    struct hdcdrv_cmd_close close;
    struct hdcdrv_cmd_send send;
    struct hdcdrv_cmd_recv_peek recv_peek;
    struct hdcdrv_cmd_recv recv;
    struct hdcdrv_cmd_set_session_owner set_owner;
    struct hdcdrv_cmd_get_session_run_env get_session_run_env;
    struct hdcdrv_cmd_set_session_timeout set_session_timeout;
    struct hdcdrv_cmd_get_stat get_stat;
    struct hdcdrv_cmd_get_uid_stat get_uid_stat;
    struct hdcdrv_cmd_alloc_mem alloc_mem;
    struct hdcdrv_cmd_free_mem free_mem;
    struct hdcdrv_cmd_fast_send fast_send;
    struct hdcdrv_cmd_fast_recv fast_recv;
    struct hdcdrv_cmd_dma_map dma_map;
    struct hdcdrv_cmd_dma_unmap dma_unmap;
    struct hdcdrv_cmd_dma_remap dma_remap;
    struct hdcdrv_cmd_epoll_alloc_fd epoll_alloc_fd;
    struct hdcdrv_cmd_epoll_free_fd epoll_free_fd;
    struct hdcdrv_cmd_epoll_ctl epoll_ctl;
    struct hdcdrv_cmd_epoll_wait epoll_wait;
    struct hdcdrv_cmd_get_page_size get_page_size;
};

#endif
