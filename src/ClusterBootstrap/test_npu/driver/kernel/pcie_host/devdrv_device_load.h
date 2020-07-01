/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2012-2019. All rights reserved.
 * Description:
 * Author: huawei
 * Create: 2017/8/18
 */

#ifndef _DEVDRV_DEVICE_LOAD_H_
#define _DEVDRV_DEVICE_LOAD_H_

#include <linux/workqueue.h>

extern struct devdrv_black_callback g_black_box;
#define DEVDRV_HOST_FILE_PATH "/home/bios/"

#ifdef CFG_SOC_PLATFORM_MDC_V51
#define DEVDRV_DEVICE_FILE_DDR_IMAG "/home/bios/ascend610_ddr.image"
#define DEVDRV_DEVICE_FILE_LOWPWR_IMAG "/home/bios/ascend610_lowpwr.image"
#define DEVDRV_DEVICE_FILE_HSM_IMAG "/home/bios/ascend610_hsm.image"
#define DEVDRV_DEVICE_FILE_UEFI "/home/bios/ascend610.fd"
#define DEVDRV_DEVICE_FILE_DTB "/home/bios/ascend610_dt.img"
#define DEVDRV_DEVICE_FILE_IMAGE "/home/bios/ascend610.image"
#define DEVDRV_DEVICE_FILE_FS "/home/bios/ascend610.cpio.gz"
#define DEVDRV_DEVICE_FILE_TEE "/home/bios/ascend610_tee.bin"

#define DEVDRV_AUTO_FILE_DDR_IMAG "/driver/device/ascend610_ddr.image"
#define DEVDRV_AUTO_FILE_LOWPWR_IMAG "/driver/device/ascend610_lowpwr.image"
#define DEVDRV_AUTO_FILE_HSM_IMAG "/driver/device/ascend610_hsm.image"
#define DEVDRV_AUTO_FILE_UEFI "/driver/device/ascend610.fd"
#define DEVDRV_AUTO_FILE_DTB "/driver/device/ascend610_dt.img"
#define DEVDRV_AUTO_FILE_IMAGE "/driver/device/ascend610.image"
#define DEVDRV_AUTO_FILE_FS "/driver/device/ascend610.cpio.gz"
#define DEVDRV_AUTO_FILE_TEE "/driver/device/ascend610_tee.bin"

#else
#ifndef CFG_SOC_PLATFORM_CLOUD
#define DEVDRV_DEVICE_FILE_DTB "/home/bios/davinci_mini_dt.img"
#define DEVDRV_DEVICE_FILE_IMAGE "/home/bios/davinci_mini.image"
#define DEVDRV_DEVICE_FILE_FS "/home/bios/davinci_mini.cpio.gz"
#define DEVDRV_DEVICE_FILE_UEFI "/home/bios/davinci_mini.fd"
#define DEVDRV_DEVICE_FILE_LPM3 "/home/bios/davinci_mini_lpm3.img"
#define DEVDRV_DEVICE_FILE_TEE "/home/bios/davinci_mini_tee.bin"
#define DEVDRV_DEVICE_FILE_CRL ""

#define DEVDRV_AUTO_DEVICE_FILE_DTB "/driver/device/davinci_mini_dt.img"
#define DEVDRV_AUTO_DEVICE_FILE_IMAGE "/driver/device/davinci_mini.image"
#define DEVDRV_AUTO_DEVICE_FILE_FS "/driver/device/davinci_mini.cpio.gz"
#define DEVDRV_AUTO_DEVICE_FILE_UEFI "/driver/device/davinci_mini.fd"
#define DEVDRV_AUTO_DEVICE_FILE_LPM3 "/driver/device/davinci_mini_lpm3.img"
#define DEVDRV_AUTO_DEVICE_FILE_TEE "/driver/device/davinci_mini_tee.bin"
#define DEVDRV_AUTO_DEVICE_FILE_CRL ""
#else
#define DEVDRV_DEVICE_FILE_UEFI ""
#define DEVDRV_DEVICE_FILE_DTB ""
#define DEVDRV_DEVICE_FILE_IMAGE "/home/bios/ascend_910.image"
#define DEVDRV_DEVICE_FILE_FS "/home/bios/ascend_910.cpio.gz"
#define DEVDRV_DEVICE_FILE_LPM3 ""
#define DEVDRV_DEVICE_FILE_TEE "/home/bios/ascend_910_tee.bin"
#define DEVDRV_DEVICE_FILE_CRL "/home/bios/ascend_910.crl"

#define DEVDRV_AUTO_DEVICE_FILE_UEFI ""
#define DEVDRV_AUTO_DEVICE_FILE_DTB ""
#define DEVDRV_AUTO_DEVICE_FILE_IMAGE "/driver/device/ascend_910.image"
#define DEVDRV_AUTO_DEVICE_FILE_FS "/driver/device/ascend_910.cpio.gz"
#define DEVDRV_AUTO_DEVICE_FILE_LPM3 ""
#define DEVDRV_AUTO_DEVICE_FILE_TEE "/driver/device/ascend_910_tee.bin"
#define DEVDRV_AUTO_DEVICE_FILE_CRL "/driver/device/ascend_910.crl"
#endif

#endif

#ifdef CFG_SOC_PLATFORM_MDC_V51
#define DEVDRV_BLOCKS_NUM 9 /* num of DEVDRV_DEVICE_FILE_* */
#else
#define DEVDRV_BLOCKS_NUM 7
#endif

#define DEVDRV_BLOCKS_COUNT 2

#define DEVDRV_BLOCKS_NAME_SIZE 32
#define DEVDRV_BLOCKS_ADDR_PAIR_NUM 3000
#define DEVDRV_BLOCKS_STATIC_NUM 48
#define DEVDRV_BLOCKS_SIZE (1024ul * 1024)
#define DEVDRV_LOAD_FILE_MAX_SIZE (DEVDRV_BLOCKS_SIZE * DEVDRV_BLOCKS_ADDR_PAIR_NUM)
#define DEVDRV_BLOCKS_STATIC_SIZE (DEVDRV_BLOCKS_SIZE * DEVDRV_BLOCKS_STATIC_NUM)

#define DEVDRV_DMA_CACHE_NUM 128
#define DEVDRV_DMA_ALLOC_DEPTH 8

#define DEVDRV_LOAD_FILE_BEGIN 0x1111111111111111ul
#define DEVDRV_NORMAL_BOOT_MODE 0x2222222222222222ul
#define DEVDRV_ABNORMAL_BOOT_MODE 0xFFFFFFFFFFFFFFFFul
#define DEVDRV_SLOW_BOOT_MODE 0x3333333333333333ul
#define DEVDRV_LOAD_SUCCESS 0x55555555u
#define DEVDRV_LOAD_NOTICE 0x5555555555555555ul
#define DEVDRV_LOAD_FINISH 0
#define DEVDRV_SEND_FINISH 0x6666666666666666ul
#define DEVDRV_SEND_PATT_FINISH 0x7777777777777777ul
#define DEVDRV_RECV_FINISH 0x8888888888888888ul
#define DEVDRV_NO_FILE 0X9999999999999999ul
#define DEVDRV_TEE_CHECK_FAIL 0x5A5AA5A55A5AA5A0
#define DEVDRV_IMAGE_CHECK_FAIL 0x5A5AA5A55A5AA5A1
#define DEVDRV_FILESYSTEM_CHECK_FAIL 0x5A5AA5A55A5AA5A2

#define DEVDRV_BIOS_VERSION_SUPPORT_REG 0x3dc4c
#define DEVDRV_BIOS_VERSION_SUPPORT_FLAG 0x5aa5dcba
#define DEVDRV_BIOS_VERSION_SUPPORT_CLEAR 0x5aa5dc02
#define DEVDRV_BIOS_BOOTROM_VERSION_REG 0x3dc50
#define DEVDRV_BIOS_XLOADER_VERSION_REG 0x3DC60
#define DEVDRV_BIOS_NVE_VERSION_REG 0x3dc70

#define DEVDRV_LOAD_TIMEOUT 200000ul /* 2s */
#define DEVDRV_LOAD_DELAY 100        /* 100ms */
#define DEVDRV_LOAD_TIMES (DEVDRV_LOAD_TIMEOUT / DEVDRV_LOAD_DELAY)
#define DEVDRV_LOAD_ABORT 1
#define DEVDRV_ADDR_ALIGN 64
#define DEVDRV_GET_FLAG_COUNT 100
#define DEVDRV_DELAY_TIME 20
#define DEVDRV_WAIT_LOAD_FILE_TIME 600

#ifdef DRV_UT
#define DEVDRV_LOAD_FILE_WAIT_TIME (10 * 1000) /* ms */
#else
#define DEVDRV_LOAD_FILE_WAIT_TIME (600 * 1000) /* ms */
#endif
#define DEVDRV_LOAD_FILE_CHECK_TIME 20
#define DEVDRV_LOAD_FILE_CHECK_CNT (DEVDRV_LOAD_FILE_WAIT_TIME / DEVDRV_LOAD_FILE_CHECK_TIME)

#ifdef CFG_SOC_PLATFORM_ESL_FPGA
#define DEVDRV_TIMER_SCHEDULE_TIMES 1800 /* fpga:1800s */
#else
#define DEVDRV_TIMER_SCHEDULE_TIMES 300 /* asic:300s */
#endif
#define DEVDRV_TIMER_EXPIRES (1 * HZ)

#define DEVDRV_ALIGN(addr, size) (((addr) + ((size)-1)) & (~((typeof(addr))(size)-1)))

enum devdrv_load_wait_mode {
    DEVDRV_LOAD_WAIT_INTERVAL = 0x0,
    DEVDRV_LOAD_WAIT_FOREVER
};

struct devdrv_load_addr_pair {
    void *addr;
    dma_addr_t dma_addr;
    u64 size;      /* block size */
    u64 data_size; /* data length is this block */
};

struct devdrv_load_blocks {
    char name[DEVDRV_BLOCKS_NAME_SIZE];
    u64 blocks_num;
    u64 blocks_valid_num;
    struct devdrv_load_addr_pair blocks_addr[DEVDRV_BLOCKS_ADDR_PAIR_NUM];
};

struct devdrv_load_work {
    struct devdrv_pci_ctrl *ctrl;
    struct work_struct work;
};

struct devdrv_agent_load {
    struct device *dev;
    u32 dev_id;
    void __iomem *mem_sram_base;
    struct devdrv_load_blocks *blocks;

    struct timer_list load_timer; /* device os load time out timer */
    int timer_remain;             /* timer_remain <= 0 means time out */
    int timer_expires;

    struct devdrv_load_work load_work;
    atomic_t load_flag;

    int load_wait_mode;
    int load_abort;
};

int devdrv_load_device(struct devdrv_pci_ctrl *pci_ctrl);
void devdrv_load_exit(struct devdrv_pci_ctrl *pci_ctrl);
void devdrv_notify_blackbox_err(u32 devid, u32 code);
void devdrv_set_load_abort(struct devdrv_agent_load *agent_loader);
irqreturn_t devdrv_load_irq(int irq, void *data);

#endif
