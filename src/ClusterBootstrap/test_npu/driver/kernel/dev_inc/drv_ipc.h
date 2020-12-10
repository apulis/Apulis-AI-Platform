/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2012-2019. All rights reserved.
 * Description: This file contains drv_ipc Info.
 * Author: huawei
 * Create: 2017-10-15
 */
/* drv_ipc.h */

#ifndef __DRV_IPC_H__
#define __DRV_IPC_H__

#include <linux/types.h>

/* ipc mailbox data register is 32 bits.
 */
typedef u32 mbox_msg_t;
typedef u32 mbox_msg_len_t;
typedef mbox_msg_t rproc_msg_t;
typedef mbox_msg_len_t rproc_msg_len_t;

#define DRV_IPC_RPROC_LEN   2


#if defined(CFG_SOC_PLATFORM_CLOUD)
typedef enum {
    HISI_RPROC_RX_IMU_MBX0,
    HISI_RPROC_RX_IMU_MBX1,
    HISI_RPROC_RX_TS_MBX2,
    HISI_RPROC_TX_TS_MBX17,
    HISI_RPROC_TX_IMU_MBX24,
    HISI_RPROC_TX_IMU_MBX25,
    HISI_RPROC_RX_IMU_MBX3,
    HISI_RPROC_MAX
} rproc_id_t;


#elif defined(CFG_SOC_PLATFORM_MDC_V51) || defined(CFG_SOC_PLATFORM_MINIV2)
enum {
    HISI_IPC_ID_SAFETY_ISLAND = 0,
    HISI_IPC_ID_LOWER_POWER,
    HISI_IPC_ID_TSC,
    HISI_IPC_ID_TSV,
    CLUSTER0_ISP_IPC_ID,
    CLUSTER1_ISP_IPC_ID,
    CLUSTER2_ISP_IPC_ID,
    CLUSTER3_ISP_IPC_ID,
    HISI_IPC_ID_TAISHAN,
    HISI_IPC_ID_MAX,
};

typedef enum {
    HISI_RPROC_SI_Q_RX_RPID0,
    HISI_RPROC_SI_Q_RX_RPID1,
    HISI_RPROC_SI_Q_RX_RPID2,
    HISI_RPROC_SI_Q_RX_RPID3,
    HISI_RPROC_SI_Q_TX_RPID14,
    HISI_RPROC_SI_Q_TX_RPID15,
    HISI_RPROC_SI_Q_TX_RPID16,
    HISI_RPROC_SI_Q_TX_RPID17,
    HISI_RPROC_LP_Q_RX_RPID0,
    HISI_RPROC_LP_Q_RX_RPID1,
    HISI_RPROC_LP_Q_RX_RPID2,
    HISI_RPROC_LP_Q_RX_RPID3,
    HISI_RPROC_LP_Q_TX_RPID4_ACPU0,
    HISI_RPROC_LP_Q_TX_RPID4_ACPU1,
    HISI_RPROC_LP_Q_TX_RPID4_ACPU2,
    HISI_RPROC_LP_Q_TX_RPID4_ACPU3,
    HISI_RPROC_TSC_RX_RPID4,
    HISI_RPROC_TSC_RX_RPID5,
    HISI_RPROC_TSC_RX_RPID6,
    HISI_RPROC_TSC_RX_RPID7,
    HISI_RPROC_TSC_TX_RPID0,
    HISI_RPROC_TSC_TX_RPID1,
    HISI_RPROC_TSC_TX_RPID2,
    HISI_RPROC_TSC_TX_RPID3,
    HISI_RPROC_TSV_RX_RPID4,
    HISI_RPROC_TSV_RX_RPID5,
    HISI_RPROC_TSV_RX_RPID6,
    HISI_RPROC_TSV_RX_RPID7,
    HISI_RPROC_TSV_TX_RPID0,
    HISI_RPROC_TSV_TX_RPID1,
    HISI_RPROC_TSV_TX_RPID2,
    HISI_RPROC_TSV_TX_RPID3,
    /* IPC_ID: 4 */
    HISI_RPROC_ISP0_IPC0_MBX0_RX_RPID0,
    HISI_RPROC_ISP0_IPC0_MBX1_RX_RPID0,
    HISI_RPROC_ISP0_IPC0_MBX2_RX_RPID0,
    HISI_RPROC_ISP0_IPC0_MBX6_TX_RPID1,
    HISI_RPROC_ISP0_IPC0_MBX7_TX_RPID2,
    HISI_RPROC_ISP0_IPC0_MBX8_TX_RPID3,
    /* IPC_ID: 5 */
    HISI_RPROC_ISP0_IPC1_MBX0_RX_RPID0,
    HISI_RPROC_ISP0_IPC1_MBX1_RX_RPID0,
    HISI_RPROC_ISP0_IPC1_MBX2_RX_RPID0,
    HISI_RPROC_ISP0_IPC1_MBX6_TX_RPID1,
    HISI_RPROC_ISP0_IPC1_MBX7_TX_RPID2,
    HISI_RPROC_ISP0_IPC1_MBX8_TX_RPID3,
    /* IPC_ID: 6 */
    HISI_RPROC_ISP1_IPC0_MBX0_RX_RPID0,
    HISI_RPROC_ISP1_IPC0_MBX1_RX_RPID0,
    HISI_RPROC_ISP1_IPC0_MBX2_RX_RPID0,
    HISI_RPROC_ISP1_IPC0_MBX6_TX_RPID1,
    HISI_RPROC_ISP1_IPC0_MBX7_TX_RPID2,
    HISI_RPROC_ISP1_IPC0_MBX8_TX_RPID3,
    /* IPC_ID: 7 */
    HISI_RPROC_ISP1_IPC1_MBX0_RX_RPID0,
    HISI_RPROC_ISP1_IPC1_MBX1_RX_RPID0,
    HISI_RPROC_ISP1_IPC1_MBX2_RX_RPID0,
    HISI_RPROC_ISP1_IPC1_MBX6_TX_RPID6,
    HISI_RPROC_ISP1_IPC1_MBX7_TX_RPID7,
    HISI_RPROC_ISP1_IPC1_MBX8_TX_RPID8,
    /* IPC_ID: 8 src_(tx/rx)_des */
    /* ACPU0 */
    HISI_RPROC_TAISHAN_MBX0_ACPU1_RX_ACPU0,
    HISI_RPROC_TAISHAN_MBX0_ACPU2_RX_ACPU0,
    HISI_RPROC_TAISHAN_MBX0_ACPU3_RX_ACPU0,
    HISI_RPROC_TAISHAN_MBX1_ACPU0_TX_ACPU1,
    HISI_RPROC_TAISHAN_MBX2_ACPU0_TX_ACPU2,
    HISI_RPROC_TAISHAN_MBX3_ACPU0_TX_ACPU3,
    /* ACPU1 */
    HISI_RPROC_TAISHAN_MBX1_ACPU0_RX_ACPU1,
    HISI_RPROC_TAISHAN_MBX1_ACPU2_RX_ACPU1,
    HISI_RPROC_TAISHAN_MBX1_ACPU3_RX_ACPU1,
    HISI_RPROC_TAISHAN_MBX0_ACPU1_TX_ACPU0,
    HISI_RPROC_TAISHAN_MBX2_ACPU1_TX_ACPU2,
    HISI_RPROC_TAISHAN_MBX3_ACPU1_TX_ACPU3,
    /* ACPU2 */
    HISI_RPROC_TAISHAN_MBX2_ACPU0_RX_ACPU2,
    HISI_RPROC_TAISHAN_MBX2_ACPU1_RX_ACPU2,
    HISI_RPROC_TAISHAN_MBX2_ACPU3_RX_ACPU2,
    HISI_RPROC_TAISHAN_MBX0_ACPU2_TX_ACPU0,
    HISI_RPROC_TAISHAN_MBX1_ACPU2_TX_ACPU1,
    HISI_RPROC_TAISHAN_MBX3_ACPU2_TX_ACPU3,
    /* ACPU3 */
    HISI_RPROC_TAISHAN_MBX3_ACPU0_RX_ACPU3,
    HISI_RPROC_TAISHAN_MBX3_ACPU1_RX_ACPU3,
    HISI_RPROC_TAISHAN_MBX3_ACPU2_RX_ACPU3,
    HISI_RPROC_TAISHAN_MBX0_ACPU3_TX_ACPU0,
    HISI_RPROC_TAISHAN_MBX1_ACPU3_TX_ACPU1,
    HISI_RPROC_TAISHAN_MBX2_ACPU3_TX_ACPU2,
    HISI_RPROC_MAX
} rproc_id_t;
#else
typedef enum {
    HISI_RPROC_TX_TS,
    HISI_RPROC_TX_LPM3,
    HISI_RPROC_RX_TS_MBX4,   /* ACPU0 */
    HISI_RPROC_RX_LPM3_MBX5, /* ACPU0 */

    HISI_RPROC_RX_TS_MBX6,   /* ACPU1 */
    HISI_RPROC_RX_LPM3_MBX7, /* ACPU1 */

    HISI_RPROC_RX_TS_MBX8,   /* ACPU2 */
    HISI_RPROC_RX_LPM3_MBX9, /* ACPU2 */

    HISI_RPROC_RX_TS_MBX10,   /* ACPU3 */
    HISI_RPROC_RX_LPM3_MBX11, /* ACPU3 */
    HISI_RPROC_MAX
} rproc_id_t;
#endif

#define IPCDRV_RPROC_MSG_LENGTH (8)
#define IPCDRV_MSG_LENGTH (28)

struct ipcdrv_msg_header {
    u32 msg_type   : 1;
    u32 cmd_type   : 7;
    u32 sync_type  : 1;
    u32 reserved   : 1;
    u32 msg_length : 14;
    u32 msg_index : 8;
};

struct ipcdrv_message {
    struct ipcdrv_msg_header ipc_msg_header;
    u8 ipcdrv_payload[IPCDRV_MSG_LENGTH];
};

struct ipcdrv_msg {
    struct ipcdrv_message ipc_message;
    u8 channel_type;
    void (*eventCallbackfunc)(int result, void *args);
};

enum ipc_msg_type {
    MSGTYPE_DRIVER_SEND = 0,
    MSGTYPE_DRIVER_RECEIVE = 1,
};

enum ipc_msg_sync_type {
    IPCDRV_MSG_SYNC = 0,
    IPCDRV_MSG_ASYNC = 1,
};

#define COMPUTING_POWER_PMU_NUM 4

struct tag_computing_power_msg {
    u64 state;
    u64 timestamp1;
    u64 timestamp2;
    u64 event_count[COMPUTING_POWER_PMU_NUM];
    u32 system_frequency;
};

#if defined(CFG_SOC_PLATFORM_MDC_V51) || defined(CFG_SOC_PLATFORM_MINIV2)
typedef enum {
    CHIP_0_IPC_ID = 0,
    CHIP_1_IPC_ID,
    CHIP_MAX_IPC_ID,
} CHIP_IPC_ID;

#define RPROC_SYNC_SEND(ipc_id, rproc_id, msg, len, \
    ack_buffer, ack_buffer_len) \
    hisi_rproc_xfer_sync(ipc_id, rproc_id, msg, len, \
    ack_buffer, ack_buffer_len)
#define RPROC_ASYNC_SEND(ipc_id, rproc_id, msg, len) \
    hisi_rproc_xfer_async(ipc_id, rproc_id, msg, len)
#define RPROC_MONITOR_REGISTER(ipc_id, rproc_id, nb) \
    hisi_rproc_rx_register(ipc_id, rproc_id, nb)
#define RPROC_MONITOR_UNREGISTER(ipc_id, rproc_id, nb) \
    hisi_rproc_rx_unregister(ipc_id, rproc_id, nb)
#define RPROC_PUT(ipc_id, rproc_id) hisi_rproc_put(ipc_id, rproc_id)
#define RPROC_FLUSH_TX(ipc_id, rproc_id)    hisi_rproc_flush_tx(ipc_id, rproc_id)
#endif

int hisi_rproc_xfer_sync(int ipc_id, rproc_id_t rproc_id, rproc_msg_t *msg, rproc_msg_len_t len,
                    rproc_msg_t *ack_buffer, rproc_msg_len_t ack_buffer_len);
int hisi_rproc_xfer_async(int ipc_id, rproc_id_t rproc_id, rproc_msg_t *msg, rproc_msg_len_t len);

/* register & unregister function should be called in pair.
 * DO NOT register multiple times for same rproc_id and nb
 */
int hisi_rproc_rx_register(int ipc_id, rproc_id_t rproc_id, struct notifier_block *nb);
int hisi_rproc_rx_unregister(int ipc_id, rproc_id_t rproc_id, struct notifier_block *nb);
int hisi_rproc_flush_tx(int ipc_id, rproc_id_t rproc_id);
int hisi_mbx_int_start_id(int dev_id); /* used by dev_devmng */
#endif /* __DRV_IPC_H__ */
