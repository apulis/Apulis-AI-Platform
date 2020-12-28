/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2012-2019. All rights reserved.
 * Description: Base Msg Definition
 * Author: Huawei Technologies Co., Ltd.
 * Create: 2018/8/18
 */

#ifndef __DEVDRV_ADMIN_MSG_H_
#define __DEVDRV_ADMIN_MSG_H_

#include "nvme_drv.h"
#include "devdrv_interface.h"

#define DEVDRV_P2P_SURPORT_MAX_DEVICE 8

enum msg_queue_type {
    TRANSPARENT_MSG_QUEUE = 0,
    NON_TRANSPARENT_MSG_QUEUE,
    MSG_QUEUE_TYPE_MAX
};

#define DEVDRV_PCIE_RESERVE_MEM_SIZE (9 * 1024 * 1024)
#define DEVDRV_MSG_QUEUE_MEM_BASE 0x20000 /* 128KB */
#define DEVDRV_MSG_QUEUE_MEM_SIZE (DEVDRV_RESERVE_MEM_MSG_SIZE - DEVDRV_MSG_QUEUE_MEM_BASE) /* 8MB - 128KB */
#define DEVDRV_MSG_QUEUE_MEM_ALIGN 0x400 /* 1KB */


#ifdef CFG_SOC_PLATFORM_ESL_FPGA
#define DEVDRV_MSG_TIMEOUT 100000000       /* 100s for fpga  */
#define DEVDRV_MSG_D2H_TIMEOUT 100000000   /* 100s */
#define DEVDRV_MSG_TIMEOUT_LOG 10000000    /* 10s */
#define DEVDRV_MSG_IRQ_TIMEOUT 3000000     /* 3s */
#define DEVDRV_MSG_IRQ_TIMEOUT_LOG 1000000 /* 1s */
#define DEVDRV_MSG_WAIT_MIN_TIME 200       /* 200us */
#define DEVDRV_MSG_WAIT_MAX_TIME 400       /* 400us */
#else
#define DEVDRV_MSG_TIMEOUT 5000000        /* 5s */
#define DEVDRV_MSG_D2H_TIMEOUT 30000000   /* 30s */
#define DEVDRV_MSG_TIMEOUT_LOG 1000000    /* 1s */
#define DEVDRV_MSG_IRQ_TIMEOUT 1000000    /* 1s */
#define DEVDRV_MSG_IRQ_TIMEOUT_LOG 100000 /* 100ms */
#define DEVDRV_MSG_WAIT_MIN_TIME 1       /* 1us */
#define DEVDRV_MSG_WAIT_MAX_TIME 2       /* 2us */
#endif
#define DEVDRV_MSG_WAIT_DMA_FINISH_TIMEOUT 100 /* 100ms */
#define DEVDRV_MSG_TIME_VOERFLOW 5000          /* 5s */

#ifdef CFG_SOC_PLATFORM_MDC_V51
#ifdef CFG_SOC_PLATFORM_ESL_FPGA
#define DEVDRV_SHR_PARA_ADDR 0xC6F1E400
#else
#define DEVDRV_SHR_PARA_ADDR 0xC6F00400
#endif
#else
#define DEVDRV_SHR_PARA_ADDR 0xA0D22400
#endif
#define DEVDRV_SHR_PARA_ADDR_OFFSET 0x400
#define DEVDRV_SHR_PARA_ADDR_SIZE 0x400

#define DEVDRV_P2P_MSG_SIZE 0x400 /* msg 1KB */
#define DEVDRV_P2P_SEND_MSG_ADDR_OFFSET 0
#define DEVDRV_P2P_SEND_MSG_ADDR_SIZE (DEVDRV_P2P_MSG_SIZE * DEVDRV_P2P_SURPORT_MAX_DEVICE)
#define DEVDRV_P2P_RECV_MSG_ADDR_OFFSET (DEVDRV_P2P_SEND_MSG_ADDR_OFFSET + DEVDRV_P2P_SEND_MSG_ADDR_SIZE)
#define DEVDRV_P2P_RECV_MSG_ADDR_SIZE (DEVDRV_P2P_MSG_SIZE * DEVDRV_P2P_SURPORT_MAX_DEVICE)
#define DEVDRV_P2P_MSG_ADDR_OFFSET (DEVDRV_SHR_PARA_ADDR_OFFSET + DEVDRV_SHR_PARA_ADDR_SIZE)
#define DEVDRV_P2P_MSG_ADDR_TOTAL_SIZE (DEVDRV_P2P_SEND_MSG_ADDR_SIZE + DEVDRV_P2P_RECV_MSG_ADDR_SIZE)

/* cmd opreation code, first 8 bits is mudule name, later 8 bits is op type */
enum devdrv_admin_msg_opcode {
    /* msg chan */
    DEVDRV_CREATE_MSG_QUEUE,
    DEVDRV_FREE_MSG_QUEUE,
    DEVDRV_NOTIFY_DMA_ERR_IRQ,
    DEVDRV_GET_GELNERAL_INTERRUPT_DB_INFO,
    DEVDRV_NOTIFY_DEV_ONLINE,
    DEVDRV_CFG_P2P_MSG_CHAN,
    DEVDRV_CFG_TX_ATU,
    DEVDRV_GET_RX_ATU,
    DEVDRV_ADMIN_MSG_MAX
};

#define DEVDRV_MSG_CMD_BEGIN 1
#define DEVDRV_MSG_CMD_FINISH_SUCCESS 2
#define DEVDRV_MSG_CMD_FINISH_FAILED 3
#define DEVDRV_MSG_CMD_INVALID_PARA 4
#define DEVDRV_MSG_CMD_IRQ_BEGIN 0x69727173 /* irqs */

#define DEVDRV_OP_ADD 1
#define DEVDRV_OP_DEL 2

struct devdrv_create_queue_command {
    u32 msg_type;   /* enum devdrv_msg_client_type */
    u32 queue_type; /* enum msg_queue_type */
    u32 queue_id;
    u64 sq_dma_base_host;
    u64 cq_dma_base_host;
    u32 sq_desc_size;
    u32 cq_desc_size;
    u16 sq_depth;
    u16 cq_depth;
    u32 sq_slave_mem_offset;
    u32 cq_slave_mem_offset;
    s32 irq_tx_finish_notify;
    s32 irq_rx_msg_notify;
};

struct devdrv_free_queue_cmd {
    u32 queue_id;
};

struct devdrv_notify_dma_err_irq_cmd {
    u32 dma_chan_id;
    u32 err_irq;
};

struct devdrv_general_interrupt_db_info {
    u32 db_start;
    u32 db_num;
};

struct devdrv_notify_dev_online_cmd {
    u32 devid;
    u32 status;
};

struct devdrv_p2p_msg_chan_cfg_cmd {
    u32 op;
    u32 devid;
};

struct devdrv_tx_atu_cfg_cmd {
    u32 op;
    u32 devid;
    u32 atu_type;
    u64 phy_addr;
    u64 target_addr;
    u64 target_size;
};

struct devdrv_get_rx_atu_cmd {
    u32 devid;
    u32 bar_num;
};

/* DMA single node read and write command */
struct devdrv_dma_single_node_command {
    struct devdrv_dma_node dma_node;
};

/* DMA chained read and write command */
struct devdrv_dma_chain_command {
    u64 dma_node_base;
    u32 node_cnt;
};

struct devdrv_admin_msg_command {
    u32 opcode;
    u32 status;
    char data[0];
};

struct devdrv_admin_msg_reply {
    u32 len;  // contain 'len' own occupied space
    char data[0];
};

/* The command channel uses a memory synchronous call */
#define DEVDRV_ADMIN_MSG_QUEUE_DEPTH 1
#define DEVDRV_ADMIN_MSG_QUEUE_BD_SIZE 0x400

#define DEVDRV_ADMIN_MSG_HEAD_LEN sizeof(struct devdrv_admin_msg_command)
#define DEVDRV_ADMIN_MSG_DATA_LEN (DEVDRV_ADMIN_MSG_QUEUE_BD_SIZE - DEVDRV_ADMIN_MSG_HEAD_LEN)

struct devdrv_non_trans_msg_desc {
    u32 in_data_len;  /* input real length */
    u32 out_data_len; /* output max length */
    u32 real_out_len; /* output real length */
    u32 msg_type;     /* enum devdrv_common_msg_type */
    u64 seq_num;      /* msg sequence number */
    u32 status;       /* DEVDRV_MSG_CMD_* */
    char data[0];
};

#define DEVDRV_NON_TRANS_MSG_HEAD_LEN sizeof(struct devdrv_non_trans_msg_desc)

struct devdrv_shr_para {
    int load_flag;      /* D2H: device bios notice host to load device os via pcie. 0: no, 1 yes */
    int chip_id;        /* D2H: device bios notice host: cloud ai server, index in one node(4P 0-3); others 0 */
    int node_id;        /* D2H: device bios notice host: cloud ai server has total 8P, one node has 4p, which node */
    int slot_id;        /* D2H: device bios notice host: slot_id (0-7) */
    int board_type;     /* D2H: device bios notice host: cloud pcie card, ai server, evb */
    int chip_type;      /* D2H: mini cloud */
    int platform_type;  /* D2H: esl, emu, fpga, asic */
    int dev_num;        /* D2H: cloud ai server, total dev num 4; others 1  */
    int hot_reset_flag; /* CCPU set, bios read */
    int hot_reset_pcie_flag; /* make sure pcie report hotreset */
    int host_dev_id;         /* H2D: the dev id in host side */
    int host_interrupt_flag; /* H2D: host notice device has receive interrupt begin half probe. 0: no, 1 yes */
    u32 admin_msg_status;    /* host set begin, device irq set irq_begin, host continue */
    u64 host_mem_bar_base;   /* H2D: mem bar 4 base addr */
    u64 host_io_bar_base;    /* H2D: io bar 2 base addr */
    u64 tx_atu_base_addr1;   /* D2H: cloud only, high 256GB address space */
    u64 tx_atu_base_size1;   /* D2H: cloud only, high 256GB space size */
    u64 tx_atu_base_addr2;   /* D2H: cloud only, low 128MB address space */
    u64 tx_atu_base_size2;   /* D2H: cloud only, low 128MB space size */
    u64 p2p_msg_base_addr[DEVDRV_P2P_SURPORT_MAX_DEVICE]; /* H2D: cloud only, p2p msg base for dma */
    u64 p2p_db_base_addr[DEVDRV_P2P_SURPORT_MAX_DEVICE];  /* H2D: cloud only, p2p doorbell base for dma */
};

#endif
