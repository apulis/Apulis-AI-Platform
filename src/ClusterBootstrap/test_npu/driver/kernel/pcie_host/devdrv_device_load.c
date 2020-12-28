/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2012-2019. All rights reserved.
 * Description:
 * Author: huawei
 * Create: 2017/8/18
 */

#ifdef CONFIG_GENERIC_BUG
#undef CONFIG_GENERIC_BUG
#endif
#ifdef CONFIG_BUG
#undef CONFIG_BUG
#endif

#ifdef CONFIG_DEBUG_BUGVERBOSE
#undef CONFIG_DEBUG_BUGVERBOSE
#endif

#include <linux/delay.h>
#include <linux/msi.h>

#include <linux/time.h>
#include <linux/timer.h>
#include <linux/timex.h>
#include <linux/rtc.h>
#include <linux/version.h>
#include <asm/uaccess.h>
#include <linux/fs.h>

#include "devdrv_device_load.h"
#include "devdrv_ctrl.h"
#include "devdrv_pci.h"
#include "devdrv_dma.h"
#include "devdrv_util.h"

#ifdef DRV_UT
#define STATIC
#else
#define STATIC static
#endif

#define DEVDRV_MAX_FILE_SIZE (1024 * 100)
#define DEVDRV_STR_MAX_LEN 100
#define DEVDRV_CONFIG_OK 0
#define DEVDRV_CONFIG_FAIL 1
#define DEVDRV_CONFIG_NO_MATCH 1

int g_file_check_count = 0;
char g_line_buf[DEVDRV_STR_MAX_LEN] = {0};
char g_devdrv_sdk_path[DEVDRV_STR_MAX_LEN] = {0};
char *g_davinci_config_file = "/lib/davinci.conf";

#ifdef CFG_SOC_PLATFORM_MDC_V51
char g_file_name[DEVDRV_BLOCKS_NUM][DEVDRV_STR_MAX_LEN] = {
    {DEVDRV_DEVICE_FILE_DDR_IMAG}, {DEVDRV_DEVICE_FILE_LOWPWR_IMAG}, {DEVDRV_DEVICE_FILE_HSM_IMAG},
    {DEVDRV_DEVICE_FILE_UEFI},     {DEVDRV_DEVICE_FILE_DTB},         {DEVDRV_DEVICE_FILE_IMAGE},
    {DEVDRV_DEVICE_FILE_FS},       {DEVDRV_DEVICE_FILE_TEE}
};
char *g_file_auto_name[DEVDRV_BLOCKS_NUM] = {
    DEVDRV_AUTO_FILE_DDR_IMAG, DEVDRV_AUTO_FILE_LOWPWR_IMAG, DEVDRV_AUTO_FILE_HSM_IMAG, DEVDRV_AUTO_FILE_UEFI,
    DEVDRV_AUTO_FILE_DTB,      DEVDRV_AUTO_FILE_IMAGE,       DEVDRV_AUTO_FILE_FS,       DEVDRV_AUTO_FILE_TEE
};

#else
char g_file_name[DEVDRV_BLOCKS_NUM][DEVDRV_STR_MAX_LEN] = {
    {DEVDRV_DEVICE_FILE_UEFI}, {DEVDRV_DEVICE_FILE_DTB},  {DEVDRV_DEVICE_FILE_IMAGE},
    {DEVDRV_DEVICE_FILE_FS},   {DEVDRV_DEVICE_FILE_LPM3}, {DEVDRV_DEVICE_FILE_TEE},
    {DEVDRV_DEVICE_FILE_CRL}
};
char *g_file_auto_name[DEVDRV_BLOCKS_NUM] = {
    DEVDRV_AUTO_DEVICE_FILE_UEFI, DEVDRV_AUTO_DEVICE_FILE_DTB,  DEVDRV_AUTO_DEVICE_FILE_IMAGE,
    DEVDRV_AUTO_DEVICE_FILE_FS,   DEVDRV_AUTO_DEVICE_FILE_LPM3, DEVDRV_AUTO_DEVICE_FILE_TEE,
    DEVDRV_AUTO_DEVICE_FILE_CRL
};
#endif

void devdrv_set_device_boot_status(struct devdrv_pci_ctrl *pci_ctrl, u32 status);

#ifndef writeq
static inline void writeq(u64 value, volatile void *addr)
{
    *(volatile u64 *)addr = value;
}
#endif

#ifndef readq
static inline u64 readq(void __iomem *addr)
{
    return readl(addr) + ((u64)readl(addr + 4) << 32);
}
#endif

#if LINUX_VERSION_CODE < KERNEL_VERSION(4, 14, 0)
STATIC ssize_t devdrv_load_file_read(struct file *file, loff_t *pos, char *addr, size_t count)
{
    char __user *buf = (char __user *)addr;
    mm_segment_t old_fs;
    ssize_t len;

    old_fs = get_fs();
    set_fs(get_ds()); /*lint !e501 */ /* kernel source */
    len = vfs_read(file, buf, count, pos);
    set_fs(old_fs);

    return len;
}
#endif

STATIC void devdrv_load_free_one(struct device *dev, struct devdrv_load_addr_pair *addr_pair, int num)
{
    int i;

    for (i = 0; i < num; i++) {
        dma_free_coherent(dev, addr_pair[i].size, addr_pair[i].addr, addr_pair[i].dma_addr);
        addr_pair[i].addr = NULL;
    }

    return;
}

STATIC int devdrv_load_dma_alloc(struct device *dev, struct devdrv_load_addr_pair *addr_pair, size_t size, int depth)
{
    struct devdrv_load_addr_pair *p_addr = addr_pair;
    size_t part1_size;
    size_t part2_size;
    int block_num;
    int num;

    depth--;
    p_addr->addr = dma_alloc_coherent(dev, size, &p_addr->dma_addr, GFP_KERNEL);
    if (p_addr->addr == NULL) {
        if (depth <= 0) {
            devdrv_err("%s, dma_alloc_coherent fail (%llu)\n", dev_driver_string(dev), (u64)size);
            goto direct_out;
        }
        /* size should be align with cacheline,
         * otherwise bios will be wrong when data copy */
        part1_size = size >> 1;
        part1_size = DEVDRV_ALIGN(part1_size, DEVDRV_ADDR_ALIGN);
        if (part1_size >= size) {
            devdrv_err("%s, dma memory not enough\n", dev_driver_string(dev));
            goto direct_out;
        }
        num = devdrv_load_dma_alloc(dev, p_addr, part1_size, depth);
        if (num < 0) {
            goto direct_out;
        }
        block_num = num;
        p_addr += num;

        part2_size = size - part1_size;
        num = devdrv_load_dma_alloc(dev, p_addr, part2_size, depth);
        if (num < 0) {
            goto free_out;
        }
        block_num += num;
    } else {
        p_addr->size = (u64)size;
        block_num = 1;
    }

    return block_num;
free_out:
    devdrv_load_free_one(dev, addr_pair, block_num);
direct_out:
    return -ENOMEM;
}

STATIC int devdrv_load_contiguous_alloc(struct device *dev, struct devdrv_load_addr_pair *load_addr, int len,
                                        size_t size)
{
    struct devdrv_load_addr_pair *addr_pair = NULL;
    size_t remain_size;
    size_t alloc_size;
    int pairs_num;
    int num;
    int i;

    addr_pair = kzalloc(sizeof(*addr_pair) * DEVDRV_DMA_CACHE_NUM, GFP_KERNEL);
    if (addr_pair == NULL) {
        devdrv_err("addr_pair kzalloc fail\n");
        goto direct_out;
    }

    remain_size = size;
    pairs_num = 0;

    while (remain_size > 0) {
        alloc_size = (remain_size > DEVDRV_BLOCKS_SIZE) ? DEVDRV_BLOCKS_SIZE : remain_size;
        remain_size -= alloc_size;

        num = devdrv_load_dma_alloc(dev, addr_pair, alloc_size, DEVDRV_DMA_ALLOC_DEPTH);
        if (num <= 0) {
            devdrv_err("devdrv_load_dma_alloc fail.\n");
            goto dma_alloc_err;
        }

        if (pairs_num + num >= len) {
            devdrv_err("dma buffer is not enough.\n");
            goto need_more_buf_err;
        }
        for (i = 0; i < num; i++, pairs_num++) {
            load_addr[pairs_num].addr = addr_pair[i].addr;
            load_addr[pairs_num].dma_addr = addr_pair[i].dma_addr;
            load_addr[pairs_num].size = addr_pair[i].size;
        }
    }

    kfree(addr_pair);
    addr_pair = NULL;

    return pairs_num;
need_more_buf_err:
    devdrv_load_free_one(dev, addr_pair, num);
dma_alloc_err:
    devdrv_load_free_one(dev, load_addr, pairs_num);
    kfree(addr_pair);
    addr_pair = NULL;
direct_out:
    return -ENOMEM;
}

loff_t devdrv_get_i_size_read(struct file *p_file)
{
    return i_size_read(file_inode(p_file));
}

/*
 * read & write register with little endian.
 */
STATIC void devdrv_load_notice(struct devdrv_agent_load *loader, struct devdrv_load_blocks *blocks, u64 flag)
{
    u64 *sram_complet_addr = NULL;
    u64 *sram_reg = NULL;
    u64 j;

    sram_complet_addr = loader->mem_sram_base;

    /* skip the complet flag */
    sram_reg = sram_complet_addr + 1;
    if (blocks->blocks_num == 0) {
        devdrv_err("devid %u blocks num is zero\n", loader->dev_id);
        return;
    }

    writeq(blocks->blocks_valid_num, sram_reg++);  //lint !e144
    for (j = 0; j < blocks->blocks_valid_num; j++) {
        writeq(blocks->blocks_addr[j].dma_addr,
               sram_reg++);  //lint !e144
        writeq(blocks->blocks_addr[j].data_size,
               sram_reg++);  //lint !e144
    }

    wmb();

    /* notice agent to cpoy */
    writeq(flag, sram_complet_addr);  //lint !e144

    return;
}

void devdrv_get_data_size(struct devdrv_load_blocks *blocks, u64 file_size, u64 cnt)
{
    if (cnt >= DEVDRV_BLOCKS_ADDR_PAIR_NUM) {
        devdrv_err("invalid cnt:%lld", cnt);
        return;
    }
    if (file_size > blocks->blocks_addr[cnt].size) {
        blocks->blocks_addr[cnt].data_size = blocks->blocks_addr[cnt].size;
    } else
        blocks->blocks_addr[cnt].data_size = file_size;
    ;
}

STATIC u64 devdrv_get_wr_flag(u64 remain_size, u64 trans_size)
{
    u64 flag_w;

    if (remain_size == trans_size) {
        flag_w = DEVDRV_SEND_FINISH;
    } else {
        flag_w = DEVDRV_SEND_PATT_FINISH;
    }

    return flag_w;
}

#ifndef CFG_SOC_PLATFORM_CLOUD
STATIC void devdrv_get_bios_version(struct devdrv_pci_ctrl *pci_ctrl)
{
    int i;
    void __iomem *bootrom_reg = NULL;
    void __iomem *xloader_reg = NULL;
    void __iomem *nve_reg = NULL;
    void __iomem *version_support_reg = NULL;
    u32 version_support_val;

    bootrom_reg = pci_ctrl->io_base + DEVDRV_IO_LOAD_SRAM_OFFSET + DEVDRV_BIOS_BOOTROM_VERSION_REG;
    xloader_reg = pci_ctrl->io_base + DEVDRV_IO_LOAD_SRAM_OFFSET + DEVDRV_BIOS_XLOADER_VERSION_REG;
    nve_reg = pci_ctrl->io_base + DEVDRV_IO_LOAD_SRAM_OFFSET + DEVDRV_BIOS_NVE_VERSION_REG;
    version_support_reg = pci_ctrl->io_base + DEVDRV_IO_LOAD_SRAM_OFFSET + DEVDRV_BIOS_VERSION_SUPPORT_REG;

    /* read version support flag */
    version_support_val = readl(version_support_reg);
    if (version_support_val == DEVDRV_BIOS_VERSION_SUPPORT_FLAG) {
        pci_ctrl->device_bios_version.valid = true;
        devdrv_info("dev id %u bios support read version\n", pci_ctrl->dev_id);
        for (i = 0; i < DEVDRV_BIOS_VERSION_ARR_LEN; i++) {
            pci_ctrl->device_bios_version.bootrom[i] = readl(bootrom_reg + (long)i * sizeof(u32));
            pci_ctrl->device_bios_version.xloader[i] = readl(xloader_reg + (long)i * sizeof(u32));
            devdrv_info("dev id %u bootrom:0x%x\n", pci_ctrl->dev_id, pci_ctrl->device_bios_version.bootrom[i]);
            devdrv_info("dev id %u xloader:0x%x\n", pci_ctrl->dev_id, pci_ctrl->device_bios_version.xloader[i]);
        }
        pci_ctrl->device_bios_version.nve = readl(nve_reg);
        devdrv_info("dev id %u nve:0x%x\n", pci_ctrl->dev_id, pci_ctrl->device_bios_version.nve);

        /* clear version support flag */
        writel(DEVDRV_BIOS_VERSION_SUPPORT_CLEAR, version_support_reg);
    } else {
        devdrv_info("dev id %u bios not support read version!!\n", pci_ctrl->dev_id);
    }
}
#endif

STATIC int devdrv_get_boot_mode_flag(struct devdrv_agent_load *loader, u64 *addr)
{
    u64 flag_r = 0;
    int count = 0;

    while (1) {
        flag_r = readq(addr);
        if ((flag_r == DEVDRV_ABNORMAL_BOOT_MODE) || (flag_r == DEVDRV_NORMAL_BOOT_MODE) ||
            (flag_r == DEVDRV_SLOW_BOOT_MODE))
            break;
        count++;
        if (count % DEVDRV_GET_FLAG_COUNT == 0) {
            devdrv_info("devid %u wait boot mode from bios:0x%llx\n", loader->dev_id, flag_r);
        }
        if (count >= DEVDRV_GET_FLAG_COUNT) {
            break;
        }
        msleep(DEVDRV_DELAY_TIME);
    }
    devdrv_info("dev id %u get boot mode from bios:0x%llx\n", loader->dev_id, flag_r);
    if (flag_r == DEVDRV_SLOW_BOOT_MODE) {
        loader->load_wait_mode = DEVDRV_LOAD_WAIT_FOREVER;
    } else if (flag_r == DEVDRV_NORMAL_BOOT_MODE) {
        loader->load_wait_mode = DEVDRV_LOAD_WAIT_INTERVAL;
    } else {
        devdrv_err("dev id %u get boot mode from bios err:0x%llx\n", loader->dev_id, flag_r);
        return -EINVAL;
    }

    return 0;
}

STATIC void devdrv_check_load_file(u32 devid, u64 flag)
{
    if (flag == DEVDRV_RECV_FINISH) {
        devdrv_info("dev %u load file success\n", devid);
    } else if (flag == DEVDRV_TEE_CHECK_FAIL) {
        devdrv_err("dev %u load file check tee failed\n", devid);
    } else if (flag == DEVDRV_IMAGE_CHECK_FAIL) {
        devdrv_err("dev %u load file check image failed\n", devid);
    } else if (flag == DEVDRV_FILESYSTEM_CHECK_FAIL) {
        devdrv_err("dev %u load file check filesystem failed\n", devid);
    }
    return;
}

STATIC void devdrv_get_rd_flag(struct devdrv_agent_load *loader, u64 *addr)
{
    u64 flag_r = 0;
    int count = 0;
    int ret;
    int count_f = 0; /* Use for tee.bin get rdy.
When device boot, the rdy would change. If 0xA20000 is change from 66 77 to
others, then consider device has boot, so it's ready. */

    while (1) {
        if (loader->load_abort == DEVDRV_LOAD_ABORT) {
            devdrv_info("device id %d load abort\n", loader->dev_id);
            return;
        }
        if (atomic_read(&loader->load_flag) == DEVDRV_LOAD_SUCCESS) {
            devdrv_info("device id %d load success\n", loader->dev_id);
            return;
        }
        flag_r = readq(addr);
        ret = (flag_r == DEVDRV_RECV_FINISH) || (flag_r == DEVDRV_TEE_CHECK_FAIL) ||
              (flag_r == DEVDRV_IMAGE_CHECK_FAIL) || (flag_r == DEVDRV_FILESYSTEM_CHECK_FAIL);
        if (ret) {
            break;
        }

        /* Check flag change to others, after 2 second. */
        count_f++;
        if ((count_f >= DEVDRV_GET_FLAG_COUNT) &&
            ((flag_r != DEVDRV_SEND_FINISH) && (flag_r != DEVDRV_SEND_PATT_FINISH))) {
            devdrv_info("dev id %d flag %llx chg to others, after 2 second.\n", loader->dev_id, flag_r);
            break;
        }

        if (loader->load_wait_mode == DEVDRV_LOAD_WAIT_INTERVAL) {
            count++;
            if (count >= DEVDRV_LOAD_FILE_CHECK_CNT) {
                devdrv_err("dev id %d load timeout\n", loader->dev_id);
                devdrv_notify_blackbox_err(loader->dev_id, DEVDRV_SYSTEM_START_FAIL);
                break;
            }
        }

        msleep(DEVDRV_LOAD_FILE_CHECK_TIME);
    }
    devdrv_check_load_file(loader->dev_id, flag_r);
}

STATIC int devdrv_load_file_copy(struct devdrv_agent_load *loader, char *name, struct devdrv_load_blocks *blocks)
{
    u64 *sram_complet_addr = NULL;
    struct file *p_file = NULL;
    u64 translated_size, remain_size, valid_block_cnt, file_size;
    loff_t offset, size;
    ssize_t len;
    u64 flag_w;
    int ret;
    u64 i;
    static int count = DEVDRV_WAIT_LOAD_FILE_TIME;
    static int wait_file_cnt = 0;

    if (loader->load_abort == DEVDRV_LOAD_ABORT) {
        devdrv_info("device id %u load abord\n", loader->dev_id);
        return 0;
    }
    /* file operation */
    sram_complet_addr = loader->mem_sram_base;

RETRY:
    p_file = filp_open(name, O_RDONLY | O_LARGEFILE, 0);
    if (IS_ERR_OR_NULL(p_file)) {
        if (wait_file_cnt < count) {
            wait_file_cnt++;
            ssleep(1);
            goto RETRY;
        }

        ret = -EIO;
        devdrv_err("devid:%u, unable to open file: %s (%d)\n", loader->dev_id, name, ret);
        goto direct_out;
    }
    count = 0;
    size = devdrv_get_i_size_read(p_file);
    devdrv_info("dev id %u, i_size_read: %s %lld\n", loader->dev_id, name, size);
    if (size <= 0) {
        ret = -EIO;
        devdrv_err("devid:%u, i_size_read failed: %s %lld\n", loader->dev_id, name, size);
        goto close_out;
    }

    remain_size = size;

    /* copy file data to dma momery */
    offset = 0;
    while (remain_size > 0) {
        devdrv_get_rd_flag(loader, sram_complet_addr);
        if (remain_size >= DEVDRV_BLOCKS_STATIC_SIZE) {
            valid_block_cnt = blocks->blocks_num;
            blocks->blocks_valid_num = blocks->blocks_num;
            translated_size = DEVDRV_BLOCKS_STATIC_SIZE;
            for (i = 0; i < valid_block_cnt; i++) {
                blocks->blocks_addr[i].data_size = blocks->blocks_addr[i].size;
            }
        } else {
            translated_size = remain_size;
            file_size = remain_size;
            i = 0;
            while ((file_size > 0) && (i < DEVDRV_BLOCKS_ADDR_PAIR_NUM)) {
                devdrv_get_data_size(blocks, file_size, i);
                file_size = file_size - blocks->blocks_addr[i].data_size;
                i++;
            }
            valid_block_cnt = i;
            blocks->blocks_valid_num = valid_block_cnt;
        }
        for (i = 0; i < valid_block_cnt; i++) {
#if LINUX_VERSION_CODE >= KERNEL_VERSION(4, 14, 0)
            len = kernel_read(p_file, blocks->blocks_addr[i].addr, (size_t)blocks->blocks_addr[i].data_size, &offset);
#else
            len = devdrv_load_file_read(p_file, &offset, blocks->blocks_addr[i].addr, blocks->blocks_addr[i].data_size);
#endif
            if (len < 0) {
                ret = -EIO;
                devdrv_err("devid:%u, file read err %ld\n", loader->dev_id, (long)len);
                goto free_out;
            }

            if (len != blocks->blocks_addr[i].data_size) {
                ret = -EIO;
                devdrv_err("devid:%u, file read %ld/%lld\n", loader->dev_id, (long)len, blocks->blocks_addr[i].size);
                goto free_out;
            }
        }
        flag_w = devdrv_get_wr_flag(remain_size, translated_size);
        devdrv_load_notice(loader, blocks, flag_w);
        remain_size -= translated_size;
    }

    /* check */
    if (offset != size) {
        ret = -EIO;
        devdrv_err("devid:%u, file read err offset = %lld, size = %lld\n", loader->dev_id, offset, size);
        goto free_out;
    }

    filp_close(p_file, NULL);
    p_file = NULL;

    return 0;

free_out:
close_out:
    filp_close(p_file, NULL);
    p_file = NULL;
direct_out:
    return ret;
}

STATIC void devdrv_load_blocks_free(struct devdrv_agent_load *loader)
{
    struct devdrv_load_blocks *blocks = loader->blocks;
    struct device *dev = loader->dev;

    if (loader->blocks == NULL) {
        return;
    }

    if (blocks->blocks_num == 0) {
        return;
    }

    devdrv_load_free_one(dev, blocks->blocks_addr, blocks->blocks_num);

    blocks->blocks_num = 0;

    kfree(blocks);
    blocks = NULL;
    loader->blocks = NULL;

    return;
}

void devdrv_set_load_abort(struct devdrv_agent_load *agent_loader)
{
    agent_loader->load_abort = DEVDRV_LOAD_ABORT;
}

STATIC int devdrv_load_file_trans(struct devdrv_agent_load *loader)
{
    struct devdrv_load_blocks *blocks = NULL;
    struct device *dev = loader->dev;
    struct devdrv_pci_ctrl *pci_ctrl = NULL;
    u64 *sram_complet_addr = NULL;
    u64 i = 0;
    int ret;

    pci_ctrl = loader->load_work.ctrl;
    blocks = kzalloc(sizeof(*blocks), GFP_KERNEL);
    if (blocks == NULL) {
        devdrv_err("devid:%u, blocks kzalloc fail\n", loader->dev_id);
        return -ENOMEM;
    }

    ret = devdrv_load_contiguous_alloc(dev, blocks->blocks_addr, DEVDRV_BLOCKS_ADDR_PAIR_NUM,
                                       (size_t)DEVDRV_BLOCKS_STATIC_SIZE);
    if (ret <= 0) {
        devdrv_err("devid:%u, load file comm dma alloc fail ret %d\n", loader->dev_id, ret);
        kfree(blocks);
        blocks = NULL;
        return -ENOMEM;
    }
    loader->blocks = blocks;
    blocks->blocks_num = ret;
    sram_complet_addr = loader->mem_sram_base;
    devdrv_info("devid %d load file alloc %llu block memory\n", loader->dev_id, blocks->blocks_num);
    ret = devdrv_get_boot_mode_flag(loader, sram_complet_addr);
#ifndef CFG_SOC_PLATFORM_CLOUD
    if (ret != 0) {
        devdrv_err("devid:%u, get boot mode flag fail, stop probe!\n", loader->dev_id);
        devdrv_load_blocks_free(loader);
        return -ENOMEM;
    }
    if (devdrv_get_product_type() == HOST_TYPE_NORMAL) {
        devdrv_get_bios_version(pci_ctrl);
    }
#endif

    /* set device boot status:boot bios */
    devdrv_set_device_boot_status(pci_ctrl, DSMI_BOOT_STATUS_BIOS);
    writeq(DEVDRV_RECV_FINISH, sram_complet_addr);  //lint !e144
    for (i = 0; i < DEVDRV_BLOCKS_NUM; i++) {
        if (!strcmp(g_file_name[i], "")) {
            continue;
        }
        ret = devdrv_load_file_copy(loader, g_file_name[i], blocks);
        if ((ret < 0) && (i != (DEVDRV_BLOCKS_NUM - 1))) {
            devdrv_err("devid:%u, %s file copy err\n", loader->dev_id, g_file_name[i]);
            devdrv_load_blocks_free(loader);
            return ret;
        }
    }

    /* set device boot status:boot bios */
    devdrv_set_device_boot_status(pci_ctrl, DSMI_BOOT_STATUS_OS);

    return 0;
}

STATIC void devdrv_load_finish(struct work_struct *p_work)
{
    struct devdrv_load_work *load_work = container_of(p_work, struct devdrv_load_work, work);
    struct devdrv_pci_ctrl *pci_ctrl = load_work->ctrl;
    struct devdrv_agent_load *loader = pci_ctrl->agent_loader;

    if (loader->load_timer.function != NULL) {
        del_timer_sync(&loader->load_timer);
    } else {
        devdrv_warn("dev id %d, irq fb before timer build.\n", pci_ctrl->dev_id);
    }

    loader->timer_expires = 0;

    /* free memory used  load files */
    devdrv_load_blocks_free(loader);

    devdrv_load_half_probe(pci_ctrl);
}

void devdrv_notify_blackbox_err(u32 devid, u32 code)
{
    struct timespec stamp;

    stamp = current_kernel_time();

    if (g_black_box.callback) {
        g_black_box.callback(devid, code, stamp);
    }
}
#ifndef DRV_UT
#if LINUX_VERSION_CODE >= KERNEL_VERSION(4, 15, 0)
STATIC void devdrv_timer_task(struct timer_list *t)
{
    struct devdrv_agent_load *loader = from_timer(loader, t, load_timer);
#else
STATIC void devdrv_timer_task(unsigned long data)
{
    struct devdrv_pci_ctrl *pci_ctrl = (struct devdrv_pci_ctrl *)((uintptr_t)data);
    struct devdrv_agent_load *loader = pci_ctrl->agent_loader;
#endif
    u32 device_id = loader->dev_id;

    /* device os have being uped. */
    if (atomic_read(&loader->load_flag) == DEVDRV_LOAD_SUCCESS) {
        devdrv_info("devid:%u, device os load success\n", loader->dev_id);
        return;
    }

    /* when loader->timer_remain <= 0, means os load failed. */
    if (loader->timer_remain-- > 0) {
        loader->load_timer.expires = jiffies + loader->timer_expires;
        add_timer(&loader->load_timer);
    } else {
        devdrv_err("devid:%u, device %u os load failed\n", loader->dev_id, device_id);

#if LINUX_VERSION_CODE < KERNEL_VERSION(4, 15, 0)
        devdrv_set_startup_status(pci_ctrl, DEVDRV_STARTUP_STATUS_TIMEOUT);
#endif
        /* reset device */
        devdrv_notify_blackbox_err(device_id, DEVDRV_SYSTEM_START_FAIL);
    }

    return;
}
#endif
STATIC void devdrv_load_timer(struct devdrv_pci_ctrl *pci_ctrl)
{
    struct devdrv_agent_load *loader = pci_ctrl->agent_loader;

    loader->timer_remain = DEVDRV_TIMER_SCHEDULE_TIMES;
    loader->timer_expires = DEVDRV_TIMER_EXPIRES;
#ifndef DRV_UT
#if LINUX_VERSION_CODE >= KERNEL_VERSION(4, 15, 0)
    timer_setup(&loader->load_timer, devdrv_timer_task, 0);
#else
    setup_timer(&loader->load_timer, devdrv_timer_task, (uintptr_t)pci_ctrl);
#endif
#endif
    loader->load_timer.expires = jiffies + loader->timer_expires;

    add_timer(&loader->load_timer);
    devdrv_info("dev id %d, load timer add.\n", loader->dev_id);
    return;
}

irqreturn_t devdrv_load_irq(int irq, void *data)
{
    struct devdrv_pci_ctrl *pci_ctrl = (struct devdrv_pci_ctrl *)data;
    struct devdrv_agent_load *loader = pci_ctrl->agent_loader;

    pci_ctrl->load_status_flag = DEVDRV_LOAD_SUCCESS_STATUS;
    /* notice device addr info for addr trans */
    devdrv_notify_dev_init_status(pci_ctrl);
    /* set load success flag */
    if (atomic_read(&loader->load_flag) == DEVDRV_LOAD_SUCCESS) {
        return IRQ_HANDLED;
    } else {
        atomic_set(&loader->load_flag, DEVDRV_LOAD_SUCCESS);
    }

    /* start work queue */
    schedule_work(&loader->load_work.work);

    return IRQ_HANDLED;
}

char *devdrv_get_one_line(char *file_buf, u32 file_buf_len, char *line_buf_tmp, u32 buf_len)
{
    u32 i;
    u32 offset = 0;

    *line_buf_tmp = '\0';

    if (*file_buf == '\0') {
        return NULL;
    }
    while ((*file_buf == ' ') || (*file_buf == '\t')) {
        file_buf++;
        offset++;
    }

    for (i = 0; i < buf_len; i++) {
        if (*file_buf == '\n') {
            line_buf_tmp[i] = '\0';
            file_buf = file_buf + 1;
            offset += 1;
            goto EXIT;
        }
        if (*file_buf == '\0') {
            line_buf_tmp[i] = '\0';
            goto EXIT;
        }
        line_buf_tmp[i] = *file_buf++;
        offset++;
    }
    line_buf_tmp[buf_len - 1] = '\0';
    file_buf = file_buf + 1;
    offset += 1;

EXIT:
    if (offset >= file_buf_len) {
        devdrv_err("file buf out of bound, offset:%d, file_buf_len:%d\n", offset, file_buf_len);
        return NULL;
    }
    return file_buf;
}

void devdrv_get_val(const char *buf, u32 buf_len, char *env_val, u32 val_len)
{
    u32 i;

    *env_val = '\0';
    for (i = 0; (i < val_len) && (i < buf_len); i++) {
        if ((*buf == ' ') || (*buf == '\t') || (*buf == '\r') || (*buf == '\n') || (*buf == '\0')) {
            env_val[i] = '\0';
            break;
        }
        env_val[i] = *buf++;
    }
    env_val[val_len - 1] = '\0';
}

u32 devdrv_get_env_value(char *buf, u32 buf_len, const char *env_name, char *env_val, u32 val_len)
{
    u32 len;
    u32 offset = 0;

    if (*buf == '#') {
        return DEVDRV_CONFIG_FAIL;
    }

    len = (u32)strlen(env_name);
    if (strncmp(buf, env_name, len) != 0) {
        return DEVDRV_CONFIG_FAIL;
    }

    buf += len;
    offset += len;

    if ((*buf != ' ') && (*buf != '\t') && (*buf != '=')) {
        return DEVDRV_CONFIG_FAIL;
    }

    while ((*buf == ' ') || (*buf == '\t')) {
        buf++;
        offset++;
    }

    if (*buf != '=') {
        devdrv_err("get value error\n");
        return DEVDRV_CONFIG_FAIL;
    }

    buf++;
    offset++;

    while ((*buf == ' ') || (*buf == '\t')) {
        buf++;
        offset++;
    }

    if (offset >= buf_len) {
        devdrv_err("buf out of bound, offset:%d, buf_len:%d\n", offset, buf_len);
        return DEVDRV_CONFIG_FAIL;
    }
    devdrv_get_val(buf, buf_len - offset, env_val, val_len);
    return DEVDRV_CONFIG_OK;
}

u32 devdrv_get_env_value_form_file(char *file, const char *env_name, char *env_val, u32 env_val_len)
{
    struct file *fp = NULL;
    u32 ret;
    u32 filesize;
    u32 len;
    ssize_t len_integer;
    loff_t pos = 0;
    u32 match_flag = 0;
    char *buf = NULL;
    char *tmp_buf = NULL;
    char tmp_val[DEVDRV_STR_MAX_LEN];
    int ret_val;

    fp = filp_open(file, O_RDONLY, 0);
    if (IS_ERR(fp) || (fp == NULL)) {
        devdrv_info("open file(%s) fail\n", file);
        return DEVDRV_CONFIG_FAIL;
    }
    filesize = devdrv_get_i_size_read(fp);
    if (filesize >= DEVDRV_MAX_FILE_SIZE) {
        devdrv_err("file(%s) too large\n", file);
        filp_close(fp, NULL);
        fp = NULL;
        return DEVDRV_CONFIG_FAIL;
    }

    buf = (char *)kmalloc(filesize + 1, GFP_KERNEL);
    if (buf == NULL) {
        devdrv_err("malloc fail\n");
        filp_close(fp, NULL);
        fp = NULL;
        return DEVDRV_CONFIG_FAIL;
    }

    if (memset_s(buf, filesize + 1, 0, filesize + 1) != 0) {
        devdrv_err("memset_s failed\n");
        kfree(buf);
        filp_close(fp, NULL);
        fp = NULL;
        buf = NULL;
        return DEVDRV_CONFIG_FAIL;
    }
#if LINUX_VERSION_CODE >= KERNEL_VERSION(4, 14, 0)
    len_integer = kernel_read(fp, buf, (ssize_t)filesize, &pos);
#else
    len_integer = devdrv_load_file_read(fp, &pos, buf, filesize);
#endif
    if (len_integer <= 0) {
        devdrv_err("read file fail\n");
        kfree(buf);
        filp_close(fp, NULL);
        fp = NULL;
        buf = NULL;
        return DEVDRV_CONFIG_FAIL;
    }

    len = (u32)len_integer;
    buf[len] = '\0';
    tmp_buf = buf;
    while (1) {
        tmp_buf = devdrv_get_one_line(tmp_buf, len + 1, g_line_buf, sizeof(g_line_buf));
        if (tmp_buf == NULL) {
            break;
        }

        ret = devdrv_get_env_value(g_line_buf, DEVDRV_STR_MAX_LEN, env_name, tmp_val, sizeof(tmp_val));
        if (ret != 0) {
            continue;
        }
        len = (u32)strlen(tmp_val);
        if (env_val_len < (len + 1)) {
            kfree(buf);
            filp_close(fp, NULL);
            fp = NULL;
            buf = NULL;
            devdrv_err("env_val_len fail\n");
            return DEVDRV_CONFIG_FAIL;
        }
        ret_val = strcpy_s(env_val, env_val_len, tmp_val);
        if (ret_val != 0) {
            kfree(buf);
            filp_close(fp, NULL);
            fp = NULL;
            buf = NULL;
            devdrv_err("strcpy_s %d fail\n", ret_val);
            return DEVDRV_CONFIG_FAIL;
        }
        env_val[env_val_len - 1] = '\0';
        match_flag = 1;
        break;
    }

    kfree(buf);
    filp_close(fp, NULL);
    fp = NULL;
    buf = NULL;

    return (u32)((match_flag == 1) ? DEVDRV_CONFIG_OK : DEVDRV_CONFIG_NO_MATCH);
}

u32 devdrv_get_sdk_path(char *sdk_path, u32 path_buf_len)
{
    return devdrv_get_env_value_form_file(g_davinci_config_file, "DAVINCI_HOME_PATH", sdk_path, path_buf_len);
}

STATIC void devdrv_load_file(struct work_struct *p_work)
{
    struct devdrv_load_work *load_work = container_of(p_work, struct devdrv_load_work, work);
    struct devdrv_pci_ctrl *pci_ctrl = load_work->ctrl;
    struct devdrv_agent_load *loader = pci_ctrl->agent_loader;
    int ret;

    devdrv_info("enter trans file work queue. dev id %u\n", pci_ctrl->dev_id);
    ret = devdrv_load_file_trans(loader);
    if (ret) {
        kfree(loader);
        loader = NULL;
        pci_ctrl->agent_loader = NULL;
        devdrv_err("devid:%u, trans file to agent bios failed\n", pci_ctrl->dev_id);
        return;
    }

    devdrv_load_timer(pci_ctrl);
    devdrv_info("leave trans file work queue. dev id %u\n", pci_ctrl->dev_id);
    devdrv_get_rd_flag(loader, loader->mem_sram_base);
}

STATIC int devdrv_sdk_path_init(void)
{
    u32 ret;
    int i;

    ret = devdrv_get_sdk_path(g_devdrv_sdk_path, sizeof(g_devdrv_sdk_path));
    if (ret != 0) {
        devdrv_info("device os use original path\n");
        return 0;
    }
    for (i = 0; i < DEVDRV_BLOCKS_NUM; i++) {
        if (strcmp(g_file_name[i], "") == 0) {
            continue;
        }
        if ((memset_s(g_file_name[i], DEVDRV_STR_MAX_LEN, 0, DEVDRV_STR_MAX_LEN) != 0) ||
            (strcpy_s(g_file_name[i], DEVDRV_STR_MAX_LEN, g_devdrv_sdk_path) != 0)) {
            devdrv_err("memset_s or strcpy_s failed\n");
            return -EINVAL;
        }

        if (strlen(g_devdrv_sdk_path) + strlen(g_file_auto_name[i]) < DEVDRV_STR_MAX_LEN) {
            if (strcat_s(g_file_name[i], DEVDRV_STR_MAX_LEN, g_file_auto_name[i]) != 0) {
                devdrv_err("strcat_s failed\n");
                return -EINVAL;
            }
        } else {
            if (strncat_s(g_file_name[i], DEVDRV_STR_MAX_LEN, g_file_auto_name[i],
                          DEVDRV_STR_MAX_LEN - strlen(g_devdrv_sdk_path) - 1) != 0) {
                devdrv_err("g_strncat_s failed\n");
                return -EINVAL;
            }

            g_file_name[i][DEVDRV_STR_MAX_LEN - 1] = '\0';
        }
    }
    devdrv_info("device os use auto path\n");
    return 0;
}

int devdrv_load_device(struct devdrv_pci_ctrl *pci_ctrl)
{
    struct devdrv_agent_load *loader = NULL;
    struct device *dev = &pci_ctrl->pdev->dev;
    int ret;

    devdrv_info("device %d os load start\n", pci_ctrl->dev_id);

    if (g_file_check_count == 0) {
        g_file_check_count++;
        ret = devdrv_sdk_path_init();
        if (ret) {
            devdrv_err("devid %d devdrv_sdk_path_init fail\n", pci_ctrl->dev_id);
            return ret;
        }
    }

    loader = kzalloc(sizeof(*loader), GFP_KERNEL);
    if (loader == NULL) {
        devdrv_err("devid:%u, agent_loader kzalloc fail\n", pci_ctrl->dev_id);
        return -ENOMEM;
    }
    pci_ctrl->agent_loader = loader;

    loader->dev_id = pci_ctrl->dev_id;
    loader->mem_sram_base = pci_ctrl->io_base + DEVDRV_IO_LOAD_SRAM_OFFSET;
    atomic_set(&loader->load_flag, 0);
    loader->dev = dev;

    loader->load_work.ctrl = pci_ctrl;
    INIT_WORK(&loader->load_work.work, devdrv_load_finish);

    /* add file trans work queue */
    pci_ctrl->load_work.ctrl = pci_ctrl;
    INIT_WORK(&pci_ctrl->load_work.work, devdrv_load_file);

    /* start work queue */
    schedule_work(&pci_ctrl->load_work.work);
    devdrv_info("load write dma addr finish, wait device %u os start\n", pci_ctrl->dev_id);

    return 0;
}

void devdrv_load_exit(struct devdrv_pci_ctrl *pci_ctrl)
{
    struct devdrv_agent_load *loader = NULL;

    if (pci_ctrl->agent_loader == NULL) {
        return;
    }

    loader = pci_ctrl->agent_loader;
    devdrv_load_blocks_free(loader);

    if (loader->timer_expires) {
        del_timer_sync(&loader->load_timer);
        loader->timer_expires = 0;
    }

    kfree(loader);
    loader = NULL;
    devdrv_info("devid %u devdrv_load_exit finish\n", pci_ctrl->dev_id);
}
