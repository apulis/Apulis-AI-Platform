/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2012-2019. All rights reserved.
 * Description:
 * Author: huawei
 * Create: 2017/8/18
 */

#ifndef _DEVDRV_PCI_H_
#define _DEVDRV_PCI_H_

#include <linux/interrupt.h>
#include <linux/module.h>
#include <linux/pci.h>
#include <linux/types.h>
#include <linux/workqueue.h>

#include "devdrv_device_load.h"
#include "nvme_drv.h"
#include "apb_drv.h"
#include "devdrv_atu.h"
#include "securec.h"

extern int devdrv_sysfs_init(struct devdrv_pci_ctrl *pci_ctrl);
extern void devdrv_sysfs_exit(struct devdrv_pci_ctrl *pci_ctrl);

#define PCI_VENDOR_ID_HUAWEI 0x19e5

/* rc reg offset , for 3559 and 3519 */
#define DEVDRV_RC_REG_BASE_3559 0x12200000
#define DEVDRV_RC_REG_BASE_3519 0xEFF0000

#define DEVDRV_RC_MSI_ADDR 0x820
#define DEVDRV_RC_MSI_UPPER_ADDR 0x824
#define DEVDRV_RC_MSI_EN 0x828
#define DEVDRV_RC_MSI_MASK 0x82C
#define DEVDRV_RC_MSI_STATUS 0x830
#define DEVDRV_MINI_MSI_X_OFFSET 0x10000
#define DEVDRV_SYSTEM_START_FAIL 0x68021000
#define DEVDRV_PCIE_AER_ERROR 0x64021001
#define DEVDRV_DMA_TEST_SIZE 0x100000

/* ************ interrupt defined for 3559 ************* */
#define DEVDRV_MSI_MAX_VECTORS 32

/* device os load notify use irq vector 0, later 0 alse use to admin msg chan */
#define DEVDRV_LOAD_MSI_VECTOR_NUM 0

/* irq used to msg trans, a msg chan need two vector. one for tx finish, the other for rx msg.
   msg chan 0 is used to admin(chan 0) role */
#define DEVDRV_MSG_MSI_VECTOR_BASE 0
#define DEVDRV_MSG_MSI_VECTOR_NUM 20

/* irq used to dma, a dma chan need 12 vector. one for cq, the other for err.
   host surport 6 dma chan witch is related to enum devdrv_dma_data_type */
#define DEVDRV_DMA_MSI_VECTOR_BASE 20
#define DEVDRV_DMA_MSI_VECTOR_NUM 12

/* devdrv_remove is called from prereset or module_exit */
#define DEVDRV_REMOVE_CALLED_BY_PRERESET 0
#define DEVDRV_REMOVE_CALLED_BY_MODULE_EXIT 1

/* get ram info from device */
#define DEVDRV_DEVICE_RAM_INFO_MAX_NUM 256
#define DEVDRV_DEVICE_RAM_INFO_DATA_SIZE 48 /* all 50 left */
#define DEVDRV_DEVICE_RAM_INFO_NUM_ONE_TIME 4

/* bbox resved dma mem,2MB per mini */
#define DEVDRV_BBOX_RESVED_MEM_ALLOC_PAGES_ORDER 9
#define DEVDRV_BBOX_RESVED_MEM_SIZE (2 * 1024 * 1024)

#define DEVDRV_BIOS_VERSION_ARR_LEN 4
#define DEVDRV_HOST_RECV_INTERRUPT_FLAG 1

#define DEVDRV_ADDR_ADD 4
#define DEVDRV_ADDR_MOVE_32 32
#define DEVDRV_MSIX_TABLE_SPAN 0x10
#define DEVDRV_MSIX_TABLE_ADDRH 0x4
#define DEVDRV_MSIX_TABLE_NUM 0x8
#define DEVDRV_MSIX_TABLE_MASK 0xC
#define DEVDRV_MSIX_ADDR_BIT 0xFFFFFFFF
#define DEVDRV_DMA_BIT_MASK_64 64
#define DEVDRV_DMA_BIT_MASK_32 32
#define DEVDRV_FREE_PAGE_PARA 9

struct devdrv_pgtab_info;

struct devdrv_msix_ctrl {
    struct msix_entry entries[DEVDRV_MSI_X_MAX_VECTORS];
};

struct devdrv_msi_info {
    void *data;
    irqreturn_t (*callback_func)(int, void *);
};

/* command to get ram info from device */
struct devdrv_device_mem_info {
    u64 start_addr;
    u64 offset;
    u32 mem_type; /* enum devdrv_device_mem_type */
    u32 node_id;
};

#define DEVDRV_MAX_DB_NUM 256
struct devdrv_db_info {
    u32 db_start;
    u32 db_num;
    u32 status[DEVDRV_MAX_DB_NUM];
};

struct devdrv_dma_test {
    u32 flag;
    struct timer_list test_timer; /* device os load time out timer */
    int timer_remain;             /* timer_remain <= 0 means time out */
    int timer_expires;
};

struct devdrv_bios_version {
    u32 bootrom[DEVDRV_BIOS_VERSION_ARR_LEN];
    u32 xloader[DEVDRV_BIOS_VERSION_ARR_LEN];
    u32 nve;
    bool valid;
};

#define DEVDRV_STARTUP_STATUS_INIT 0
#define DEVDRV_STARTUP_STATUS_TIMEOUT 1
#define DEVDRV_STARTUP_STATUS_FINISH 2

#define DEVDRV_MODULE_INIT_TIMEOUT 300000  /* 300s */
#define DEVDRV_MODULE_FINISH_TIMEOUT 60000 /* 60s */

#define DEVDRV_LOAD_INIT_STATUS 1
#define DEVDRV_LOAD_SUCCESS_STATUS 2
#define DEVDRV_LOAD_HALF_PROBE_STATUS 3
#define DEVDRV_LOAD_HALF_WAIT_COUNT 300

struct devdrv_startup_status {
    int status;
    u32 module_bit_map;
    unsigned long timestamp;
};

struct devdrv_bar_dma_info {
    u64 io_dma_addr;
    u64 io_phy_size;
    u64 mem_dma_addr;
    u64 mem_phy_size;
    u64 rsv_mem_dma_addr;
    u64 rsv_mem_phy_size;
};

#define DEVDRV_VDAVINCI_VIRTUAL_RESERVE_BYTES 40

struct devdrv_priv {
    struct device *dev;
    void *dvt;
    void *ops;
    char reserve[DEVDRV_VDAVINCI_VIRTUAL_RESERVE_BYTES];
};

struct devdrv_pci_ctrl {
    struct devdrv_priv vdavinci_priv;
    struct devdrv_agent_load *agent_loader;
    struct devdrv_msg_dev *msg_dev;
    struct devdrv_dma_dev *dma_dev;
    struct devdrv_load_work load_work;
    struct devdrv_dma_test *dma_test;
    struct pci_dev *pdev;
    struct devdrv_pgtab_info *pgtab_info;
    struct devdrv_shr_para __iomem *shr_para;
    void __iomem *io_base;
    void __iomem *msi_base;
    void __iomem *mem_base;
    void __iomem *rc_reg_base;
    phys_addr_t io_phy_base;
    u64 io_phy_size;
    phys_addr_t mem_phy_base;
    u64 mem_phy_size;
    phys_addr_t rsv_mem_phy_base;
    u64 rsv_mem_phy_size;
    u64 host_mem_dma_addr;
    u64 host_mem_phy_size;
    struct devdrv_bar_dma_info target_bar[DEVDRV_P2P_SURPORT_MAX_DEVICE];
    phys_addr_t mem_base_paddr;
    struct devdrv_db_info db_info;
    u32 dev_id; /* return from devdrv_ctrl layer */
    u32 func_id;
    u32 load_half_probe;
    int load_vector;
    struct work_struct half_probe_work;
    /* device boot status */
    u32 device_boot_status;
    /* devdrv_remove is called by prereset or module_exit */
    u32 module_exit_flag;
    /* load status flag */
    u32 load_status_flag;
    /*
    device is alive or dead
    if dead, no need to send admin msg for save time
    */
    u32 device_status;

    struct devdrv_startup_status startup_status;

    struct devdrv_bios_version device_bios_version;

    /* defined for normal host */
    int msix_irq_num;
    struct devdrv_msix_ctrl msix_ctrl;

    /* defined for 3559 */
    void *msi_addr;
    dma_addr_t msi_dma_addr;
    struct devdrv_msi_info msi_info[DEVDRV_MSI_MAX_VECTORS];
    struct devdrv_iob_atu mem_rx_atu[DEVDRV_MAX_RX_ATU_NUM];

    /* resved dma mem for bbox to ddr dump */
    u32 bbox_resv_size;
    u64 bbox_resv_dmaAddr;
    struct page *bbox_resv_dmaPages;
};

int devdrv_pci_irq_vector(struct pci_dev *dev, unsigned int nr);
void devdrv_load_half_probe(struct devdrv_pci_ctrl *pci_ctrl);
void devdrv_init_dev_num(void);
int devdrv_get_dev_num(void);
void devdrv_notify_dev_init_status(struct devdrv_pci_ctrl *pci_ctrl);

#endif
