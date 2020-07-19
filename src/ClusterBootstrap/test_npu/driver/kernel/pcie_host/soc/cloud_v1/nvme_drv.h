/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2012-2019. All rights reserved.
 * Description:
 * Author: huawei
 * Create: 2017/8/18
 */

#ifndef _NVME_DRV_H_
#define _NVME_DRV_H_

#include "hipciec_nvme_global_reg_reg_offset.h"

#include "hipciec_nvme_pf_local_ctrl_reg_reg_offset.h"

/*
 * BAR       ATU       TYPE               SIZE     OFFSET        Target
 * BAR0(64M) ATU0      Doorbell           1M       0             0x800000
             ATU6      test mem           2M       0x200000      0x6FE00000
             ATU7      pcie msg queue     8M       0x800000      0x0
             ATU8      ts msg queue       32M      0x2000000     0x60000000
             ATU12     IMU BBOX LOG       8MB      0x1000000     0x5f800000
 * BAR2(16M) ATU1      APB                2M       0x0           0x148200000
 *           ATU2      IEP BAR            5M       0x500000      0x148800000
 *           ATU3      devdrv Doorbell    4M       0xA00000      0xaf400000
 *           ATU4      TS SRAM            128k     0xE00000      0xaf200000
 *           ATU5      IO SRAM            256k     0xE20000      0x12d80000
 *           x         hwts               64k      0xE60000      0xaf110000
 * BAR4      ATU9      HBM                1G       0             0x400000000
 */
#define AGENTDRV_CORE_NUM 1
#define AGENTDRV_PORT_NUM 0

#define HISI_EP_DEVICE_ID 0xa126
#define HISI_IEP_NVME_DEVICE_ID 0xa124

#define PCI_BAR_RSV_MEM 0
#define PCI_BAR_IO 2
#define PCI_BAR_MEM 4

#define DEVDRV_PF_NUM 2
#define DEVDRV_CORE_NUM 0x1
#define DEVDRV_PORT_NUM 0x0

#define DEVDRV_CHIP_ADDR_SPACE_SIZE 0x200000000000ULL

#define DEVDRV_IO_APB_OFFSET 0
#define DEVDRV_IO_APB_SIZE 0x200000
#define DEVDRV_IO_IEP_OFFSET 0x500000
#define DEVDRV_IO_IEP_SIZE 0x500000
#define DEVDRV_IO_TS_DB_OFFSET 0xA00000
#define DEVDRV_IO_TS_DB_SIZE 0x400000
#define DEVDRV_IO_TS_SRAM_OFFSET 0xE00000
#define DEVDRV_IO_TS_SRAM_SIZE 0x20000
#define DEVDRV_IO_LOAD_SRAM_OFFSET 0xE20000
#define DEVDRV_IO_LOAD_SRAM_SIZE 0x40000
#define DEVDRV_IO_HWTS_OFFSET 0xE60000
#define DEVDRV_IO_HWTS_SIZE 0x10000
#define DEVDRV_ADDR_IMU_LOG_OFFSET 0x1000000
#define DEVDRV_ADDR_IMU_LOG_SIZE 0x800000

#define DEVDRV_IEP_SDI0_OFFSET DEVDRV_IO_IEP_OFFSET
#define DEVDRV_IEP_SDI0_SIZE 0x200000
#define DEVDRV_IEP_DMA_OFFSET (DEVDRV_IEP_SDI0_OFFSET + DEVDRV_IEP_SDI0_SIZE * 2)
#define DEVDRV_IEP_DMA_SIZE 0x4000

/* mem base */
#define DEVDRV_RESERVE_MEM_MSG_OFFSET 0x800000
#define DEVDRV_RESERVE_MEM_MSG_SIZE 0x800000

#define DEVDRV_RESERVE_MEM_TEST_OFFSET 0x200000
#define DEVDRV_RESERVE_MEM_TEST_SIZE 0x200000

#define DEVDRV_RESERVE_MEM_TS_SQ_OFFSET 0x2000000
#define DEVDRV_RESERVE_MEM_TS_SQ_SIZE 0x2000000

/* not surport just for compile */
#define DEVDRV_MDC_RESERVE_MEM_OFFSET_64M 0
#define DEVDRV_MDC_RESERVE_MEM_OFFSET_128M 0
#define DEVDRV_MDC_RESERVE_MEM_OFFSET_4G 0
#define DEVDRV_MDC_RESERVE_MEM_OFFSET_8G 0
#define DEVDRV_MDC_RESERVE_MEM_OFFSET_16G 0
#define DEVDRV_MDC_RESERVE_MEM 0
#define DEVDRV_MDC_RESERVE_MEM_SIZE 0

#define AGENTDRV_DB_MEM_ADDRESS 0x800000
#define AGENTDRV_DB_MEM_SIZE 0x1000
#define AGENTDRV_DB_STRDE 8
#define AGENTDRV_DB_IOMAP_SIZE 0x20000  // include msi table

#define DEVDRV_RESERVE_MEM_DB_BASE 0x0 /*  doorbell base */
#define DEVDRV_MSG_CHAN_DB_OFFSET 0x4  /* doorbell offset */

#define DEVDRV_MAX_MSG_CHAN_CNT 64

/* ************ interrupt defined for normal host ************* */
#define DEVDRV_MSI_X_MAX_VECTORS 256
#define DEVDRV_MSI_X_MIN_VECTORS 128

/* device os load notify use irq vector 0, later 0 alse use to admin msg chan */
#define DEVDRV_LOAD_MSI_X_VECTOR_NUM 0

/* irq used to msg trans, a msg chan need two vector. one for tx finish, the other for rx msg.
   msg chan 0 is used to admin(chan 0) role */
#define DEVDRV_MSG_MSI_X_VECTOR_BASE 0
#define DEVDRV_MSG_MSI_X_VECTOR_NUM 58

/* irq used to dma, a dma chan need 22 vector. one for cq, the other for err.
  host surport 11 dma chan witch is related to enum devdrv_dma_data_type */
#define DEVDRV_DMA_MSI_X_VECTOR_BASE 58
#define DEVDRV_DMA_MSI_X_VECTOR_NUM 22

/* irq used to devmm */
#define DEVDRV_DEVMM_MSI_X_VECTOR_BASE 80
#define DEVDRV_DEVMM_MSI_X_VECTOR_NUM 48

/* msg chan irq section2 */
#define DEVDRV_MSG_MSI_X_VECTOR_2_BASE 128

/* g_irq_reg_offset */
#define DEVDRV_IRQ_OFFSET_0 0
#define DEVDRV_IRQ_OFFSET_2 2
#define DEVDRV_IRQ_OFFSET_4 4
#define DEVDRV_IRQ_OFFSET_6 6
#define DEVDRV_IRQ_OFFSET_8 8
#define DEVDRV_IRQ_OFFSET_10 10
#define DEVDRV_IRQ_OFFSET_12 12
#define DEVDRV_IRQ_OFFSET_14 14

/* g_irq0_db */
#define DEVDRV_IRQ0_DB_0 0
#define DEVDRV_IRQ0_DB_8 8
#define DEVDRV_IRQ0_DB_16 16
#define DEVDRV_IRQ0_DB_24 24
#define DEVDRV_IRQ0_DB_32 32
#define DEVDRV_IRQ0_DB_40 40
#define DEVDRV_IRQ0_DB_48 48
#define DEVDRV_IRQ0_DB_56 56
#define DEVDRV_IRQ0_DB_64 64
#define DEVDRV_IRQ0_DB_72 72
#define DEVDRV_IRQ0_DB_80 80
#define DEVDRV_IRQ0_DB_88 88
#define DEVDRV_IRQ0_DB_96 96
#define DEVDRV_IRQ0_DB_104 104
#define DEVDRV_IRQ0_DB_112 112
#define DEVDRV_IRQ0_DB_120 120

/* SQ/CQ Doorbell Register Doorbell Interrupt to Local CPU
        Doorbell Number	        MSI Vector Number
        0,8,16,бн,120	        11
        1,9,17,бн,121	        12
        2,10,18,бн,122	        13
        3,11,19,бн,123	        14
        4,12,20,бн,124	        15
        5,13,21,бн,125	        16
        6,14,22,бн,126	        17
        7,15,23,бн,127	        18
        */
#define AGENTDRV_NVME_DB_IRQ_BASE 11
#define AGENTDRV_NVME_DB_IRQ_NUM 8
#define AGENTDRV_NVME_LOW_LEVEL_DB_IRQ_NUM 2
#define AGENTDRV_NVME_DB_IRQ_STRDE 8
#define AGENTDRV_QCNT_EACH_IRQ 16
#define AGENTDRV_TOTALE_DB_IRQ_NUM 128

#define AGENTDRV_MSG_CHAN_USED_DB_START 0
#define AGENTDRV_MSG_CHAN_USED_DB_END AGENTDRV_MAX_IO_MSG_CHAN

#define AGENTDRV_P2P_MSG_USED_DB_START 88
#define AGENTDRV_P2P_MSG_USED_DB_END 96
#define AGENTDRV_P2P_MSG_USED_DB_NUM (AGENTDRV_P2P_MSG_USED_DB_END - AGENTDRV_P2P_MSG_USED_DB_START)

#define AGENTDRV_GENERAL_INTR_USED_DB_START 96
#define AGENTDRV_GENERAL_INTR_USED_DB_END 127
#define AGENTDRV_GENERAL_INTR_USED_DB_NUM (AGENTDRV_GENERAL_INTR_USED_DB_END - AGENTDRV_GENERAL_INTR_USED_DB_START)

#define AGENTDRV_TEST_USED_DB_ID 127
#define AGENTDRV_TEST_USED_INTR_NUM 2

#define AGENTDRV_NVME_INT_REQ HiPCIECTRL40V200_HIPCIEC_NVME_GLOBAL_REG_INT_REQ_REG
#define AGENTDRV_NVME_SQ_DB_STS_BASE HiPCIECTRL40V200_HIPCIEC_NVME_GLOBAL_REG_SQDB_INT_STS0_REG
#define AGENTDRV_NVME_CQ_DB_STS_BASE HiPCIECTRL40V200_HIPCIEC_NVME_GLOBAL_REG_CQDB_INT_STS0_REG
#define AGENTDRV_NVME_SQ_DB_INT_MASK_BASE HiPCIECTRL40V200_HIPCIEC_NVME_GLOBAL_REG_SQDB_INT_MASK0_REG
#define AGENTDRV_NVME_CQ_DB_INT_MASK_BASE HiPCIECTRL40V200_HIPCIEC_NVME_GLOBAL_REG_CQDB_INT_MASK0_REG
#define AGENTDRV_NVME_INT_VECTOR_MASK_BASE HiPCIECTRL40V200_HIPCIEC_NVME_GLOBAL_REG_NVME_VECTOR_MASK_0_REG

#include "nvme_comm_drv.h"

#endif
