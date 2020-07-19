/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2012-2019. All rights reserved.
 * Description: Host Pci
 * Author: Huawei Technologies Co., Ltd.
 * Create: 2018/8/18
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

#include <asm/io.h>
#include <linux/delay.h>
#include <linux/msi.h>

#include <linux/time.h>
#include <linux/timer.h>
#include <linux/timex.h>
#include <linux/rtc.h>
#include <linux/version.h>
#include <linux/semaphore.h>

#if LINUX_VERSION_CODE >= KERNEL_VERSION(4, 1, 0)
#include <linux/irq.h>
#include <linux/errno.h>
#endif
#include <linux/aer.h>
#include <linux/gfp.h>

#include "devdrv_dma.h"
#include "devdrv_ctrl.h"
#include "devdrv_msg.h"
#include "devdrv_pci.h"
#include "devdrv_common_msg.h"
#include "devdrv_util.h"
#include "resource_comm_drv.h"

#ifdef DRV_UT
#define STATIC
#else
#define STATIC static
#endif
static const char g_devdrv_driver_name[] = "devdrv_device_driver";

static const struct pci_device_id g_devdrv_tbl[] = {{ PCI_VDEVICE(HUAWEI, HISI_EP_DEVICE_ID), 0 },
                                                    { PCI_VDEVICE(HUAWEI, 0xd801), 0 },
                                                    {}};
MODULE_DEVICE_TABLE(pci, g_devdrv_tbl);

int g_pci_manage_device_num = 0;
int g_pci_manage_slot_num = 0;

int g_host_type;
int g_device_os_load_irq;
int g_msg_irq_base;
int g_msg_irq_num;
int g_dma_irq_base;
int g_dma_irq_num;

/* 3559 add para */
char *g_type = "normal";
module_param(g_type, charp, S_IRUGO);
STATIC void devdrv_uinit_3559_interrupt(struct devdrv_pci_ctrl *pci_ctrl);

int devdrv_get_product_type(void)
{
    return g_host_type;
}
EXPORT_SYMBOL(devdrv_get_product_type);

#ifndef writeq
static inline void writeq(u64 value, volatile void *addr)
{
    *(volatile u64 *)addr = value;
}
#endif

#ifndef readq
static inline u64 readq(volatile unsigned char *addr)
{
    return readl(addr) + ((u64)readl(addr + DEVDRV_ADDR_ADD) << DEVDRV_ADDR_MOVE_32);
}
#endif

STATIC int devdrv_runtime_suspend(struct device *dev)
{
    struct pci_dev *pdev = NULL;
    int ret;

    devdrv_info("enter devdrv_runtime_suspend!\n");

    pdev = container_of(dev, struct pci_dev, dev);
    ret = pci_set_power_state(pdev, PCI_D3hot);
    if (ret) {
        devdrv_err("pci_set_power_state D3hot error! ret = %d\n", ret);
        return ret;
    }

    return 0;
}

STATIC int devdrv_runtime_resume(struct device *dev)
{
    struct pci_dev *pdev = NULL;
    int ret;

    devdrv_info("enter devdrv_runtime_resume!\n");

    pdev = container_of(dev, struct pci_dev, dev);
    ret = pci_set_power_state(pdev, PCI_D0);
    if (ret) {
        devdrv_err("pci_set_power_state D0 error! ret = %d\n", ret);
        return ret;
    }

    return 0;
}

static const struct dev_pm_ops g_devdrv_pm_ops = { SET_RUNTIME_PM_OPS(devdrv_runtime_suspend, devdrv_runtime_resume,
                                                                      NULL) };

int devdrv_check_dl_dlcmsm_state(void *drvdata)
{
    struct devdrv_pci_ctrl *pci_ctrl = (struct devdrv_pci_ctrl *)drvdata;

    if (pci_ctrl == NULL) {
        devdrv_err("drvdata is NULL\n");
        return -ETIMEDOUT;
    }
    /* get DL DFX_FSM_STATE addr */
    if (devdrv_check_dlcmsm(pci_ctrl->io_base)) {
        devdrv_err("devdrv_check_dlcmsm fail\n");
        return -ETIMEDOUT;
    }

    return 0;
}
EXPORT_SYMBOL(devdrv_check_dl_dlcmsm_state);

int devdrv_register_irq_func(void *drvdata, int vector_index, irqreturn_t (*callback_func)(int, void *), void *para,
                             const char *name)
{
    struct devdrv_pci_ctrl *pci_ctrl = (struct devdrv_pci_ctrl *)drvdata;
    int vector;
    int rtn = 0;

    if (pci_ctrl == NULL) {
        devdrv_err("drvdata is NULL\n");
        return -EINVAL;
    }

    if (vector_index < 0) {
        devdrv_err("vector_index %d less than 0!\n", vector_index);
        return -EINVAL;
    }

    if ((devdrv_get_product_type() == HOST_TYPE_ARM_3559) || (devdrv_get_product_type() == HOST_TYPE_ARM_3519)) {
        vector = vector_index;
        if (vector >= DEVDRV_MSI_MAX_VECTORS) {
            devdrv_err("invalid vector %d!\n", vector);
            return -EINVAL;
        }
        pci_ctrl->msi_info[vector].callback_func = callback_func;
        pci_ctrl->msi_info[vector].data = para;
    } else {
        if (vector_index >= pci_ctrl->msix_irq_num) {
            devdrv_err("invalid vector %d!\n", vector_index);
            return -EINVAL;
        }
        vector = pci_ctrl->msix_ctrl.entries[vector_index].vector;

        rtn = request_irq(vector, callback_func, 0, name, para);
    }

    return rtn;
}
EXPORT_SYMBOL(devdrv_register_irq_func);

int devdrv_unregister_irq_func(void *drvdata, int vector_index, void *para)
{
    if (drvdata != NULL) {
        struct devdrv_pci_ctrl *pci_ctrl = (struct devdrv_pci_ctrl *)drvdata;
        int vector;

        if (vector_index < 0) {
            devdrv_err("devid:%u, vector_index %d less than 0!\n", pci_ctrl->dev_id, vector_index);
            return -EINVAL;
        }

        if ((devdrv_get_product_type() == HOST_TYPE_ARM_3559) || (devdrv_get_product_type() == HOST_TYPE_ARM_3519)) {
            vector = vector_index;
            if (vector >= DEVDRV_MSI_MAX_VECTORS) {
                devdrv_err("devid:%u, invalid vector %d!\n", pci_ctrl->dev_id, vector);
                return -EINVAL;
            }
            pci_ctrl->msi_info[vector].callback_func = NULL;
        } else {
            if (vector_index >= pci_ctrl->msix_irq_num) {
                devdrv_err("devid:%u, invalid vector %d!\n", pci_ctrl->dev_id, vector_index);
                return -EINVAL;
            }
            vector = pci_ctrl->msix_ctrl.entries[vector_index].vector;
            (void)free_irq(vector, para);
        }
    }

    return 0;
}
EXPORT_SYMBOL(devdrv_unregister_irq_func);

void devdrv_rc_irq_set_mask(void __iomem *io_base, u32 val)
{
    writel(val, io_base + DEVDRV_RC_MSI_MASK);
}

void devdrv_rc_rd_status(const void __iomem *io_base, u32 *val, u32 i)
{
    u32 sts;

    sts = readl(io_base + DEVDRV_RC_MSI_STATUS);
    *val = ((sts >> i) & 0x1);
}

void devdrv_rc_wr_status(void __iomem *io_base, u32 i)
{
    u32 val;

    val = 1 << i;
    writel(val, io_base + DEVDRV_RC_MSI_STATUS);
}

irqreturn_t devdrv_irq_main_func(int irq, void *data)
{
    struct devdrv_pci_ctrl *pci_ctrl = data;
    u32 int_status;
    int i;

    if ((devdrv_get_product_type() == HOST_TYPE_ARM_3519) || (devdrv_get_product_type() == HOST_TYPE_ARM_3559)) {
        devdrv_rc_irq_set_mask(pci_ctrl->rc_reg_base, 0xFFFFFFFF);
        for (i = 0; i < DEVDRV_MSI_MAX_VECTORS; i++) {
            devdrv_rc_rd_status(pci_ctrl->rc_reg_base, &int_status, i);
            if (int_status == 0)
                continue;

            if (pci_ctrl->msi_info[i].callback_func != NULL)
                (void)pci_ctrl->msi_info[i].callback_func(i, pci_ctrl->msi_info[i].data);
            devdrv_rc_wr_status(pci_ctrl->rc_reg_base, i);
        }
        devdrv_rc_irq_set_mask(pci_ctrl->rc_reg_base, 0x0);
    }

    return IRQ_HANDLED;
}

int devdrv_pci_irq_vector(struct pci_dev *dev, unsigned int nr)
{
    if (dev == NULL) {
        devdrv_err("drvdata is NULL\n");
        return -EINVAL;
    }
    return dev->irq + nr;
}
EXPORT_SYMBOL(devdrv_pci_irq_vector);

STATIC int devdrv_init_3559_alloc_irq(struct devdrv_pci_ctrl *pci_ctrl)
{
#if LINUX_VERSION_CODE >= KERNEL_VERSION(4, 1, 0)
    int irq, i;
    irq = devdrv_pci_irq_vector(pci_ctrl->pdev, 0);
    if (irq < 0) {
        devdrv_err("devid:%u, alloc irq interrupt failed ret = %d\n", pci_ctrl->dev_id, irq);
        return -ENODEV;
    }

    for (i = 0; i < DEVDRV_MSI_MAX_VECTORS; i++) {
        pci_ctrl->msi_info[i].data = NULL;
        pci_ctrl->msi_info[i].callback_func = NULL;
    }

    if (request_irq(irq, devdrv_irq_main_func, IRQF_SHARED, "devdrv_irq_main_func", pci_ctrl) != 0) {
        devdrv_err("devid:%u, request irq interrupt failed\n", pci_ctrl->dev_id);
        return -EIO;
    }

    g_device_os_load_irq = DEVDRV_LOAD_MSI_VECTOR_NUM;
    g_msg_irq_base = DEVDRV_MSG_MSI_VECTOR_BASE;
    g_msg_irq_num = DEVDRV_MSG_MSI_VECTOR_NUM;
    g_dma_irq_base = DEVDRV_DMA_MSI_VECTOR_BASE;
    g_dma_irq_num = DEVDRV_DMA_MSI_VECTOR_NUM;

    pci_ctrl->msix_irq_num = DEVDRV_MSI_MAX_VECTORS;

    return 0;
#else
    devdrv_err("devid:%u, request irq interrupt not supported\n", pci_ctrl->dev_id);

    return -ENODEV;
#endif
}

int devdrv_init_3559_interrupt(struct devdrv_pci_ctrl *pci_ctrl)
{
    void __iomem *msix_base = NULL;
    int i;
    int ret;

    /* ioremap product reg */
    if (devdrv_get_product_type() == HOST_TYPE_ARM_3559)
        pci_ctrl->rc_reg_base = ioremap(DEVDRV_RC_REG_BASE_3559, 0x1000);
    else
        pci_ctrl->rc_reg_base = ioremap(DEVDRV_RC_REG_BASE_3519, 0x1000);
    if (pci_ctrl->rc_reg_base == NULL) {
        devdrv_err("devid:%u, ioremap 3559 reg fail\n", pci_ctrl->dev_id);
        return -ENODEV;
    }
    pci_ctrl->msi_addr = dma_alloc_coherent(&pci_ctrl->pdev->dev, 0x10, &pci_ctrl->msi_dma_addr, GFP_KERNEL);

    if (pci_ctrl->msi_addr == NULL) {
        devdrv_err("dma alloc failed!\n");
        iounmap(pci_ctrl->rc_reg_base);
        pci_ctrl->rc_reg_base = NULL;
        return -ENODEV;
    }

    writel((u32)pci_ctrl->msi_dma_addr, pci_ctrl->rc_reg_base + DEVDRV_RC_MSI_ADDR);
    writel((u32)(((u64)pci_ctrl->msi_dma_addr) >> DEVDRV_ADDR_MOVE_32),
           pci_ctrl->rc_reg_base + DEVDRV_RC_MSI_UPPER_ADDR);
    writel(0xFFFFFFFF, pci_ctrl->rc_reg_base + DEVDRV_RC_MSI_EN);

    /* write msi-x table */
    msix_base = pci_ctrl->msi_base + DEVDRV_MINI_MSI_X_OFFSET;
    for (i = 0; i < DEVDRV_MSI_MAX_VECTORS; i++) {
        writel((u32)(pci_ctrl->msi_dma_addr & DEVDRV_MSIX_ADDR_BIT), (msix_base + (long)i * DEVDRV_MSIX_TABLE_SPAN));
        writel((u32)(((u64)pci_ctrl->msi_dma_addr) >> DEVDRV_ADDR_MOVE_32),
               (msix_base + (long)i * DEVDRV_MSIX_TABLE_SPAN + DEVDRV_MSIX_TABLE_ADDRH));
        writel(i, (msix_base + (long)i * DEVDRV_MSIX_TABLE_SPAN + DEVDRV_MSIX_TABLE_NUM));
        writel(0, (msix_base + (long)i * DEVDRV_MSIX_TABLE_SPAN + DEVDRV_MSIX_TABLE_MASK));
    }
    ret = devdrv_init_3559_alloc_irq(pci_ctrl);
    if (ret) {
        devdrv_err("devid %d devdrv_init_3559_alloc_irq fail\n", pci_ctrl->dev_id);
        devdrv_uinit_3559_interrupt(pci_ctrl);
    }

    return ret;
}

STATIC void devdrv_uinit_3559_interrupt(struct devdrv_pci_ctrl *pci_ctrl)
{
    dma_free_coherent(&pci_ctrl->pdev->dev, 0x10, pci_ctrl->msi_addr, pci_ctrl->msi_dma_addr);
    pci_ctrl->msi_addr = NULL;
    iounmap(pci_ctrl->rc_reg_base);
    pci_ctrl->rc_reg_base = NULL;
}

void devdrv_bind_irq(struct devdrv_pci_ctrl *pci_ctrl)
{
    int i;
    u32 cpu_id, irq;

    for (i = 0; i < pci_ctrl->msix_irq_num; i++) {
        cpu_id = cpumask_local_spread(i + pci_ctrl->dev_id, dev_to_node(&pci_ctrl->pdev->dev));
        irq = pci_ctrl->msix_ctrl.entries[i].vector;
        (void)irq_set_affinity_hint(irq, get_cpu_mask(cpu_id));
    }
}

void devdrv_unbind_irq(struct devdrv_pci_ctrl *pci_ctrl)
{
    int i;
    u32 irq;

    for (i = 0; i < pci_ctrl->msix_irq_num; i++) {
        irq = pci_ctrl->msix_ctrl.entries[i].vector;
        (void)irq_set_affinity_hint(irq, NULL);
    }
}

int devdrv_init_interrupt_normal(struct devdrv_pci_ctrl *pci_ctrl)
{
    u32 i;
    int vector_num;

    /* request msix interrupt */
    for (i = 0; i < DEVDRV_MSI_X_MAX_VECTORS; i++)
        pci_ctrl->msix_ctrl.entries[i].entry = i;

    vector_num = pci_enable_msix_range(pci_ctrl->pdev, pci_ctrl->msix_ctrl.entries, DEVDRV_MSI_X_MIN_VECTORS,
                                       DEVDRV_MSI_X_MAX_VECTORS);

    g_device_os_load_irq = DEVDRV_LOAD_MSI_X_VECTOR_NUM;
    g_msg_irq_base = DEVDRV_MSG_MSI_X_VECTOR_BASE;
    g_msg_irq_num = DEVDRV_MSG_MSI_X_VECTOR_NUM;
    g_dma_irq_base = DEVDRV_DMA_MSI_X_VECTOR_BASE;
    g_dma_irq_num = DEVDRV_DMA_MSI_X_VECTOR_NUM;

    pci_ctrl->msix_irq_num = vector_num;

    devdrv_info("devid:%u, vector_num %d\n", pci_ctrl->dev_id, vector_num);

    if ((vector_num >= DEVDRV_MSI_X_MIN_VECTORS) && ((vector_num <= DEVDRV_MSI_X_MAX_VECTORS))) {
        devdrv_bind_irq(pci_ctrl);
        return 0;
    }

    devdrv_err("devid:%u, vector_num %d error\n", pci_ctrl->dev_id, vector_num);

    return -EINVAL;
}

int devdrv_init_interrupt(struct devdrv_pci_ctrl *pci_ctrl)
{
    int ret;
    if ((devdrv_get_product_type() == HOST_TYPE_ARM_3559) || (devdrv_get_product_type() == HOST_TYPE_ARM_3519)) {
        ret = devdrv_init_3559_interrupt(pci_ctrl);
    } else {
        ret = devdrv_init_interrupt_normal(pci_ctrl);
    }
    return ret;
}

int devdrv_uninit_interrupt(struct devdrv_pci_ctrl *pci_ctrl)
{
    if ((devdrv_get_product_type() == HOST_TYPE_ARM_3559) || (devdrv_get_product_type() == HOST_TYPE_ARM_3519)) {
#if LINUX_VERSION_CODE >= KERNEL_VERSION(4, 6, 0)
        pci_free_irq_vectors(pci_ctrl->pdev);
#endif
        devdrv_uinit_3559_interrupt(pci_ctrl);
    } else {
        devdrv_unbind_irq(pci_ctrl);
        pci_disable_msix(pci_ctrl->pdev);
    }

    return 0;
}

struct devdrv_dma_dev *devdrv_host_dma_init(struct devdrv_pci_ctrl *pci_ctrl)
{
    struct devdrv_dma_func_para para_in = {0};
    struct devdrv_res_dma_chan_info rm_out = {0};
    /* DMA dev init */
    para_in.drvdata = (void *)pci_ctrl;
    para_in.dev = &pci_ctrl->pdev->dev;
    para_in.io_base = pci_ctrl->io_base + DEVDRV_IEP_DMA_OFFSET;
    para_in.dev_id = pci_ctrl->dev_id;
    devdrv_dma_res_chan(pci_ctrl->func_id, g_dma_irq_base, g_dma_irq_num, &rm_out);
    para_in.chan_num = rm_out.chan_num;
    para_in.chan_begin = rm_out.chan_begin;
    para_in.done_irq_base = rm_out.done_irq_base;
    para_in.err_irq_base = rm_out.err_irq_base;
    para_in.err_flag = rm_out.err_flag;

    return devdrv_dma_init(&para_in, DEVDRV_DMA_REMOTE_SIDE, pci_ctrl->func_id);
}

STATIC int devdrv_atu_rx_atu_proc(struct devdrv_pci_ctrl *pci_ctrl)
{
    int ret;

    ret = devdrv_get_rx_atu_info(pci_ctrl, PCI_BAR_MEM);
    if (ret) {
        devdrv_err("get mem rx atu fail, devid %d ret = %d\n", pci_ctrl->dev_id, ret);
        return ret;
    }
    ret = devdrv_get_rx_atu_info(pci_ctrl, PCI_BAR_IO);
    if (ret) {
        devdrv_err("get mem rx atu fail, devid %d ret = %d\n", pci_ctrl->dev_id, ret);
        return ret;
    }
    ret = devdrv_get_rx_atu_info(pci_ctrl, PCI_BAR_RSV_MEM);
    if (ret) {
        devdrv_err("get mem rx atu fail, devid %d ret = %d\n", pci_ctrl->dev_id, ret);
        return ret;
    }

    return ret;
}

void devdrv_load_half_probe(struct devdrv_pci_ctrl *pci_ctrl)
{
    int ret;

    devdrv_info("dev id %d half probe start\n", pci_ctrl->dev_id);

    ret = devdrv_msg_init(pci_ctrl, g_msg_irq_base, g_msg_irq_num);
    if (ret != 0) {
        devdrv_err("dev id %d msg dev init failed ret = %d\n", pci_ctrl->dev_id, ret);
        return;
    }

    ret = devdrv_get_general_interrupt_db_info(pci_ctrl->msg_dev, &pci_ctrl->db_info.db_start,
                                               &pci_ctrl->db_info.db_num);
    if (ret) {
        devdrv_err("dev id %d get general interrupt fail ret = %d\n", pci_ctrl->dev_id, ret);
        goto msg_exit;
    }

    /*  */
    pci_ctrl->dma_dev = devdrv_host_dma_init(pci_ctrl);
    if (pci_ctrl->dma_dev == NULL) {
        devdrv_err("dev id %d dma dev init\n", pci_ctrl->dev_id);
        goto msg_exit;
    }
    pci_ctrl->dma_dev->dev_id = pci_ctrl->dev_id;

    ret = devdrv_atu_rx_atu_proc(pci_ctrl);
    if (ret) {
        devdrv_err("dev id %d, rx atu proc fail ret = %d\n", pci_ctrl->dev_id, ret);
        goto dma_exit;
    }

    devdrv_register_half_devctrl(pci_ctrl);

    /* alloc common msg queue */
    ret = devdrv_alloc_common_msg_queue(pci_ctrl);
    if (ret) {
        devdrv_err("dev id %d, alloc_common_queue fail ret = %d\n", pci_ctrl->dev_id, ret);
        goto dma_exit;
    }

    ret = devdrv_instance_callback_after_probe(pci_ctrl->dev_id);
    if (ret) {
        devdrv_err("After probe instance callback failed, ret=%d\n", ret);
        goto free_comm_msg;
    }

    ret = devdrv_dev_online(pci_ctrl);
    if (ret) {
        devdrv_err("dev id %d, online init fail ret = %d\n", pci_ctrl->dev_id, ret);
        goto free_comm_msg;
    }

    devdrv_set_host_phy_mach_flag(pci_ctrl, DEVDRV_HOST_PHY_MACH_FLAG);

    devdrv_info("dev id %d (%02x:%02x.%d), half probe finish, probe success\n", pci_ctrl->dev_id,
                pci_ctrl->pdev->bus->number, PCI_SLOT(pci_ctrl->pdev->devfn), PCI_FUNC(pci_ctrl->pdev->devfn));

    pci_ctrl->load_half_probe = 1;

    devdrv_set_startup_status(pci_ctrl, DEVDRV_STARTUP_STATUS_FINISH);
    pci_ctrl->load_status_flag = DEVDRV_LOAD_HALF_PROBE_STATUS;

    return;

free_comm_msg:
    devdrv_free_common_msg_queue(pci_ctrl);

dma_exit:
    devdrv_dma_exit(pci_ctrl->dma_dev);
    pci_ctrl->dma_dev = NULL;

msg_exit:
    devdrv_msg_exit(pci_ctrl);

    return;
}

void devdrv_half_probe_irq_task(struct work_struct *p_work)
{
    struct devdrv_pci_ctrl *pci_ctrl = container_of(p_work, struct devdrv_pci_ctrl, half_probe_work);

    devdrv_load_half_probe(pci_ctrl);
}

void devdrv_notify_dev_init_status(struct devdrv_pci_ctrl *pci_ctrl)
{
    pci_ctrl->shr_para->host_dev_id = pci_ctrl->dev_id;
    pci_ctrl->shr_para->host_mem_bar_base = (u64)pci_ctrl->mem_phy_base;
    pci_ctrl->shr_para->host_io_bar_base = (u64)pci_ctrl->io_phy_base;
    pci_ctrl->shr_para->host_interrupt_flag = DEVDRV_HOST_RECV_INTERRUPT_FLAG;
    if (devdrv_get_chip_type() == HISI_MINIV2) {
        agentdrv_res_set_func_total(pci_ctrl->shr_para->dev_num);
    } else {
        agentdrv_res_set_func_total(1);
    }
}

irqreturn_t devdrv_half_probe_irq(int irq, void *data)
{
    struct devdrv_pci_ctrl *pci_ctrl = (struct devdrv_pci_ctrl *)data;

    pci_ctrl->load_status_flag = DEVDRV_LOAD_SUCCESS_STATUS;
    if (pci_ctrl->shr_para->host_interrupt_flag == 0) {
        schedule_work(&pci_ctrl->half_probe_work);
    }
    /* notice device addr info for addr trans */
    devdrv_notify_dev_init_status(pci_ctrl);
    return IRQ_HANDLED;
}

int devdrv_get_os_load_flag(const struct devdrv_pci_ctrl *pci_ctrl)
{
    int load_flag = 0;
    int chip_type = devdrv_get_chip_type();
    if (chip_type == HISI_MINI) {
        load_flag = 1;
        devdrv_info("host dev id %d, load_flag %d\r\n", pci_ctrl->dev_id, load_flag);
    } else if (chip_type == HISI_CLOUD)  {
        /* only chip id is 0 need load in 4p smp os */
        if (pci_ctrl->shr_para->chip_id == 0) {
            load_flag = 1;
        }

        devdrv_info("host dev id %d, device dev_id %d, node id %d, load_flag %d\r\n", pci_ctrl->dev_id,
                    pci_ctrl->shr_para->chip_id, pci_ctrl->shr_para->node_id, load_flag);
    } else if (chip_type == HISI_MINIV2) {
        /* force function 1 don't load os in 2p smp os. */
        if (pci_ctrl->func_id == PCIE_PF_0){
            load_flag = 1;
        }
        devdrv_info("chip type %d host dev id %u func id %u load_flag %d\n", chip_type, pci_ctrl->dev_id,
                    pci_ctrl->func_id, load_flag);
    }

    return load_flag;
}

int devdrv_prepare_half_probe(struct devdrv_pci_ctrl *pci_ctrl)
{
    int ret = 0;
    int load_flag = devdrv_get_os_load_flag(pci_ctrl);
    if (load_flag) {
        ret = devdrv_load_device(pci_ctrl);
        if (ret) {
            devdrv_err("devid:%u, device os load failed ret = %d\n", pci_ctrl->dev_id, ret);
            return ret;
        }
        devdrv_register_irq_func((void *)pci_ctrl, g_device_os_load_irq, devdrv_load_irq, pci_ctrl, "devdrv_load_irq");
    } else {
        devdrv_info("device dev %d do not need load os, wait device interrupt to start half probe\n",
                    pci_ctrl->shr_para->chip_id);
        INIT_WORK(&pci_ctrl->half_probe_work, devdrv_half_probe_irq_task);

        devdrv_register_irq_func((void *)pci_ctrl, g_device_os_load_irq, devdrv_half_probe_irq, pci_ctrl,
                                 "devdrv_half_probe_irq");
    }
    pci_ctrl->load_vector = g_device_os_load_irq;
    return ret;
}

#if LINUX_VERSION_CODE < KERNEL_VERSION(4, 11, 0)
int devdrv_configure_extended_tags(struct pci_dev *dev)
{
    u32 cap;
    u16 ctl;
    int ret;

    ret = pcie_capability_read_dword(dev, PCI_EXP_DEVCAP, &cap);
    if (ret) {
        return 0;
    }

    if (!(cap & PCI_EXP_DEVCAP_EXT_TAG)) {
        return 0;
    }

    ret = pcie_capability_read_word(dev, PCI_EXP_DEVCTL, &ctl);
    if (ret) {
        return 0;
    }

    if (!(ctl & PCI_EXP_DEVCTL_EXT_TAG)) {
        devdrv_info("enabling Extended Tags\n");
        pcie_capability_set_word(dev, PCI_EXP_DEVCTL, PCI_EXP_DEVCTL_EXT_TAG);
    }

    return 0;
}
#endif

int devdrv_cfg_pdev(struct pci_dev *pdev)
{
    int ret;

#if LINUX_VERSION_CODE < KERNEL_VERSION(4, 11, 0)
#ifdef CFG_SOC_PLATFORM_CLOUD
    struct pci_dev *pdev_t = NULL;
#endif
#endif

    ret = pci_enable_device_mem(pdev);
    if (ret != 0) {
        devdrv_err("pci_enable_device_mem fail ret = %d\n", ret);
        return ret;
    }

    /* DMA 64 bit Mask has a bug, will be support in the future */
    /*lint -e598 -e648 */
    if (dma_set_mask_and_coherent(&pdev->dev, DMA_BIT_MASK(DEVDRV_DMA_BIT_MASK_64))) {
        /*lint +e598 +e648 */
        devdrv_info("dma_set_mask 64 bit fail\n");
        ret = dma_set_mask_and_coherent(&pdev->dev, DMA_BIT_MASK(DEVDRV_DMA_BIT_MASK_32));
        if (ret != 0) {
            devdrv_err("dma_set_mask 32 bit fail ret = %d\n", ret);
            goto disable_device;
        }
    }

    ret = pci_request_regions(pdev, "devdrv");
    if (ret != 0) {
        devdrv_err("pci_request_regions fail ret = %d\n", ret);
        goto disable_device;
    }

    pci_set_master(pdev);

#ifndef CFG_SOC_PLATFORM_CLOUD
    ret = pci_enable_pcie_error_reporting(pdev);
    if (ret) {
        devdrv_info("no support pcie error reporting: %d\n", ret);
    }
#endif

        /* pci_configure_device will config this after 4.11.0 */
#if LINUX_VERSION_CODE < KERNEL_VERSION(4, 11, 0)
    (void)devdrv_configure_extended_tags(pdev);

#ifdef CFG_SOC_PLATFORM_CLOUD
    /* network pf0 */
    pdev_t = devdrv_get_device_pf(pdev, NETWORK_PF_0);
    if (pdev_t)
        (void)devdrv_configure_extended_tags(pdev_t);

    /* network pf1 */
    pdev_t = devdrv_get_device_pf(pdev, NETWORK_PF_1);
    if (pdev_t)
        (void)devdrv_configure_extended_tags(pdev_t);
#endif

#endif

    return 0;

disable_device:
    pci_disable_device(pdev);

    return ret;
}
void devdrv_uncfg_pdev(struct pci_dev *pdev)
{
    pci_disable_pcie_error_reporting(pdev);
    pci_clear_master(pdev);
    pci_release_regions(pdev);
    pci_disable_device(pdev);
}
void devdrv_alloc_reservd_mem_for_bbox(struct devdrv_pci_ctrl *pci_ctrl)
{
    struct pci_dev *pdev = NULL;

    pdev = pci_ctrl->pdev;
    if (pci_ctrl->bbox_resv_dmaAddr == 0) {
        /* order=9, 2^9 * 4KB = 2MB; alloc_pages support max 4MB */
        pci_ctrl->bbox_resv_dmaPages = alloc_pages(GFP_KERNEL | __GFP_NORETRY | __GFP_ZERO,
                                                   DEVDRV_BBOX_RESVED_MEM_ALLOC_PAGES_ORDER);
        if (pci_ctrl->bbox_resv_dmaPages == NULL) {
            devdrv_err("devid:%u, alloc_pages fail\n", pci_ctrl->dev_id);
            return;
        }

        pci_ctrl->bbox_resv_size = DEVDRV_BBOX_RESVED_MEM_SIZE;
        pci_ctrl->bbox_resv_dmaAddr = dma_map_page(&pdev->dev, pci_ctrl->bbox_resv_dmaPages, 0,
                                                   pci_ctrl->bbox_resv_size, DMA_BIDIRECTIONAL);
        if (dma_mapping_error(&pdev->dev, pci_ctrl->bbox_resv_dmaAddr)) {
            __free_pages(pci_ctrl->bbox_resv_dmaPages, DEVDRV_FREE_PAGE_PARA);
            pci_ctrl->bbox_resv_dmaPages = NULL;
            devdrv_err("devid:%u, dma_mapping_error\n", pci_ctrl->dev_id);
            return;
        }
    }

    devdrv_info("devid:%u, alloc mem for bbox, page:%pK, size:0x%x\n", pci_ctrl->dev_id,
                pci_ctrl->bbox_resv_dmaPages, pci_ctrl->bbox_resv_size);

    writeq(pci_ctrl->bbox_resv_dmaAddr, (pci_ctrl->io_base + DEVDRV_IO_LOAD_SRAM_OFFSET + 0x800));  //lint !e144
    writel(pci_ctrl->bbox_resv_size, (pci_ctrl->io_base + DEVDRV_IO_LOAD_SRAM_OFFSET + 0x808));
    devdrv_info("devid:%u, alloc mem for bbox, page:%pK, size:0x%x\n", pci_ctrl->dev_id,
                pci_ctrl->bbox_resv_dmaPages,
                readl((volatile unsigned char *)pci_ctrl->io_base + DEVDRV_IO_LOAD_SRAM_OFFSET + 0x808));
}
int devdrv_get_bbox_reservd_mem(unsigned int devid, unsigned long long *dma_addr, struct page **dma_pages,
                                unsigned int *len)
{
    struct devdrv_pci_ctrl *pci_ctrl = NULL;
    struct devdrv_ctrl *ctrl = NULL;

    if (devid >= MAX_DEV_CNT) {
        devdrv_err("invalid dev id %d, id overflow!\n", devid);
        return -EINVAL;
    }
    if ((dma_addr == NULL) || (dma_pages == NULL) || (len == NULL)) {
        devdrv_err("devid %d dma_addr/dma_pages/len is NULL\n", devid);
        return -EINVAL;
    }
    ctrl = devdrv_get_top_half_devctrl_by_id(devid);
    if ((ctrl == NULL) || (ctrl->priv == NULL)) {
        devdrv_err("devid %d parameter is error\n", devid);
        return -EINVAL;
    }
    pci_ctrl = (struct devdrv_pci_ctrl *)ctrl->priv;

    *dma_addr = pci_ctrl->bbox_resv_dmaAddr;
    *dma_pages = pci_ctrl->bbox_resv_dmaPages;
    *len = pci_ctrl->bbox_resv_size;

    return 0;
}
EXPORT_SYMBOL(devdrv_get_bbox_reservd_mem);

void __iomem *devdrv_map_msg_queue_addr(struct pci_dev *pdev)
{
    void __iomem *addr = NULL;
    unsigned long port, size;

    if (devdrv_get_chip_type() == HISI_MINI) {
        port = pci_resource_start(pdev, PCI_BAR_MEM) + DEVDRV_RESERVE_MEM_MSG_OFFSET;
        size = DEVDRV_RESERVE_MEM_MSG_SIZE;
    } else {
        port = pci_resource_start(pdev, PCI_BAR_RSV_MEM) + DEVDRV_RESERVE_MEM_MSG_OFFSET;
        size = DEVDRV_RESERVE_MEM_MSG_SIZE;
    }
    addr = ioremap(port, size);

    return addr;
}

int devdrv_probe(struct pci_dev *pdev, const struct pci_device_id *data)
{
    struct devdrv_pci_ctrl *pci_ctrl = NULL;
    int ret;
    u8 bus_num = pdev->bus->number;
    u8 device_num = PCI_SLOT(pdev->devfn);
    u8 func_num = PCI_FUNC(pdev->devfn);
    int chip_type = devdrv_get_chip_type();

    devdrv_info("probe driver IN. bdf:%02x:%02x.%d\n", bus_num, device_num, func_num);

    ret = devdrv_cfg_pdev(pdev);
    if (ret) {
        devdrv_err("devdrv_cfg_pdev fail ret = %d\n", ret);
        return ret;
    }

    pci_ctrl = kzalloc(sizeof(struct devdrv_pci_ctrl), GFP_KERNEL);
    if (pci_ctrl == NULL) {
        devdrv_err("pci_ctrl kzalloc fail\n");
        ret = -ENOMEM;
        goto uncfg_pdev;
    }
    pci_ctrl->dev_id = (u32)-1;
    pci_ctrl->load_half_probe = 0;
    pci_ctrl->load_vector = -1;

    devdrv_set_startup_status(pci_ctrl, DEVDRV_STARTUP_STATUS_INIT);

    pci_set_drvdata(pdev, pci_ctrl);
    pci_ctrl->pdev = pdev;
    pci_ctrl->func_id = 0;
    if (chip_type == HISI_MINIV2) {
        pci_ctrl->func_id = (u32)func_num;
    }

    pci_ctrl->mem_phy_base = (phys_addr_t)pci_resource_start(pci_ctrl->pdev, PCI_BAR_MEM);
    pci_ctrl->mem_phy_size = (u64)pci_resource_len(pci_ctrl->pdev, PCI_BAR_MEM);

    pci_ctrl->mem_base = devdrv_map_msg_queue_addr(pdev);
    if (pci_ctrl->mem_base == NULL) {
        devdrv_err("ioremap msg queue mem fail\n");
        ret = -ENODEV;
        goto kfree_pci_ctrl;
    }

    /* resource map bar01 for doorbells, depth and base address of SQ and CQ */
    pci_ctrl->rsv_mem_phy_base = (phys_addr_t)pci_resource_start(pdev, PCI_BAR_RSV_MEM);
    pci_ctrl->rsv_mem_phy_size = (u64)pci_resource_len(pci_ctrl->pdev, PCI_BAR_RSV_MEM);
    pci_ctrl->msi_base = ioremap(pci_resource_start(pdev, PCI_BAR_RSV_MEM) + DEVDRV_RESERVE_MEM_DB_BASE,
                                 AGENTDRV_DB_IOMAP_SIZE);
    if (pci_ctrl->msi_base == NULL) {
        devdrv_err("ioremap db mem fail\n");
        ret = -ENODEV;
        goto iounmap_mem;
    }

    pci_ctrl->io_phy_base = (phys_addr_t)pci_resource_start(pdev, PCI_BAR_IO);
    pci_ctrl->io_phy_size = (u64)pci_resource_len(pdev, PCI_BAR_IO);
    pci_ctrl->io_base = ioremap(pci_resource_start(pdev, PCI_BAR_IO), pci_resource_len(pdev, PCI_BAR_IO));
    if (pci_ctrl->io_base == NULL) {
        devdrv_err("ioremap io mem fail\n");
        ret = -ENODEV;
        goto iounmap_msi;
    }

    if ((chip_type == HISI_CLOUD) || (chip_type == HISI_MINIV2)) {
        pci_ctrl->shr_para = pci_ctrl->io_base + DEVDRV_IO_LOAD_SRAM_OFFSET + DEVDRV_SHR_PARA_ADDR_OFFSET;
    } else if (chip_type == HISI_MINI) {
        pci_ctrl->shr_para = pci_ctrl->mem_base + DEVDRV_SHR_PARA_ADDR_OFFSET;
        pci_ctrl->shr_para->hot_reset_pcie_flag = 0;
    }
    /* init pci ctrl and alloc devid */
    ret = devdrv_register_pci_devctrl(pci_ctrl);
    if (ret) {
        devdrv_err("alloc devid failed ret %d\n", ret);
        goto iounmap_io;
    }
    devdrv_probe_wait(pci_ctrl->dev_id);
    drvdrv_dev_startup_record(pci_ctrl->dev_id);
    drvdrv_dev_startup_report(pci_ctrl->dev_id);

    ret = devdrv_init_interrupt(pci_ctrl);
    if (ret) {
        devdrv_err("devid:%u, init interrupt failed. ret %d\n", pci_ctrl->dev_id, ret);
        goto iounmap_io;
    }
    pci_ctrl->load_status_flag = DEVDRV_LOAD_INIT_STATUS;
    ret = devdrv_prepare_half_probe(pci_ctrl);
    if (ret) {
        goto release_interrupt;
    }

    devdrv_set_devctrl_startup_flag(pci_ctrl->dev_id, DEVDRV_DEV_STARTUP_TOP_HALF_OK);

    devdrv_info("[sdk_init_time_host] probe ends, dev_id:%u "
                "waiting for device os to start after the second half of initialization\n", pci_ctrl->dev_id);

    /* alloc mem buffer for ddr dump */
    devdrv_alloc_reservd_mem_for_bbox(pci_ctrl);
    devdrv_set_device_status(pci_ctrl, DEVDRV_DEVICE_ALIVE);
    pci_ctrl->module_exit_flag = DEVDRV_REMOVE_CALLED_BY_PRERESET;

    ret = devdrv_sysfs_init(pci_ctrl);
    if (ret)
        devdrv_err("sysfs init failed ret = %d\n", ret);

    devdrv_info("probe driver OUT. dev_id:%u, bdf:%02x:%02x.%d\n", pci_ctrl->dev_id, bus_num, device_num, func_num);

    /* other process in low-half part (devdrv_load_half_probe) after device os loaded */
    return 0;

release_interrupt:
    (void)devdrv_uninit_interrupt(pci_ctrl);

iounmap_io:
    iounmap(pci_ctrl->io_base);
    pci_ctrl->io_base = NULL;

iounmap_msi:
    iounmap(pci_ctrl->msi_base);
    pci_ctrl->msi_base = NULL;

iounmap_mem:
    iounmap(pci_ctrl->mem_base);
    pci_ctrl->mem_base = NULL;

kfree_pci_ctrl:
    kfree(pci_ctrl);
    pci_ctrl = NULL;

uncfg_pdev:
    devdrv_uncfg_pdev(pdev);

    return ret;
}

STATIC void devdrv_load_half_free(struct devdrv_pci_ctrl *pci_ctrl)
{
    devdrv_slave_dev_delete(pci_ctrl->dev_id);

    devdrv_free_common_msg_queue(pci_ctrl);

    devdrv_dma_exit(pci_ctrl->dma_dev);
    pci_ctrl->dma_dev = NULL;

    devdrv_msg_exit(pci_ctrl);
}

STATIC void devdrv_load_probe_free(struct devdrv_pci_ctrl *pci_ctrl)
{
    struct pci_dev *pdev = pci_ctrl->pdev;

    if (pci_ctrl->bbox_resv_dmaPages != NULL) {
        dma_unmap_page(&pdev->dev, (dma_addr_t)pci_ctrl->bbox_resv_dmaAddr, pci_ctrl->bbox_resv_size,
                       DMA_BIDIRECTIONAL);
        __free_pages(pci_ctrl->bbox_resv_dmaPages, DEVDRV_BBOX_RESVED_MEM_ALLOC_PAGES_ORDER);
        pci_ctrl->bbox_resv_dmaPages = NULL;
        pci_ctrl->bbox_resv_dmaAddr = 0;
        pci_ctrl->bbox_resv_size = 0;
    }
    devdrv_set_devctrl_startup_flag(pci_ctrl->dev_id, DEVDRV_DEV_STARTUP_UNPROBED);

    devdrv_load_exit(pci_ctrl);

    (void)devdrv_uninit_interrupt(pci_ctrl);

    iounmap(pci_ctrl->io_base);
    pci_ctrl->io_base = NULL;
    iounmap(pci_ctrl->msi_base);
    pci_ctrl->msi_base = NULL;

    devdrv_set_host_phy_mach_flag(pci_ctrl, 0x0);
    iounmap(pci_ctrl->mem_base);
    pci_ctrl->mem_base = NULL;
    pci_ctrl->shr_para = NULL;
    pci_release_regions(pdev);
    pci_disable_device(pdev);
    kfree(pci_ctrl);
    pci_ctrl = NULL;

    return;
}

void devdrv_remove(struct pci_dev *pdev)
{
    struct devdrv_pci_ctrl *pci_ctrl = (struct devdrv_pci_ctrl *)pci_get_drvdata(pdev);
    struct devdrv_agent_load *agent_loader = NULL;
    u8 bus_num = pdev->bus->number;
    u8 device_num = PCI_SLOT(pdev->devfn);
    u8 func_num = PCI_FUNC(pdev->devfn);
    u32 dev_id = pci_ctrl->dev_id;
    int count = 0;

    agent_loader = pci_ctrl->agent_loader;
    devdrv_info(" remove driver IN. dev_id:%u, bdf:%02x:%02x.%d\n", dev_id, bus_num, device_num, func_num);

    devdrv_set_device_status(pci_ctrl, DEVDRV_DEVICE_REMOVE);
    /* unregister device ready interrupt */
    if (pci_ctrl->load_vector >= 0) {
        devdrv_unregister_irq_func((void *)pci_ctrl, pci_ctrl->load_vector, pci_ctrl);
        pci_ctrl->load_vector = -1;
    }
    msleep(10);
    /* wait half probe status */
    if (pci_ctrl->load_status_flag == DEVDRV_LOAD_SUCCESS_STATUS) {
        while (pci_ctrl->load_status_flag != DEVDRV_LOAD_HALF_PROBE_STATUS) {
            count++;
            if (count == DEVDRV_LOAD_HALF_WAIT_COUNT) {
                break;
            }
            msleep(10);
        }
    }
    devdrv_set_startup_status(pci_ctrl, DEVDRV_STARTUP_STATUS_INIT);
    devdrv_set_device_boot_status(pci_ctrl, DSMI_BOOT_STATUS_UNINIT);
    /* cancel work, when driver remove, should cancel device load work as early as possible */
    if ((agent_loader != NULL) && (pci_ctrl->load_work.work.func != NULL)) {
        devdrv_set_load_abort(agent_loader);
        cancel_work_sync(&pci_ctrl->load_work.work);
    }
    if ((agent_loader != NULL) && (pci_ctrl->agent_loader->load_work.work.func != NULL)) {
        cancel_work_sync(&pci_ctrl->agent_loader->load_work.work);
    }
    devdrv_dev_offline(pci_ctrl);
    devdrv_set_dma_status(pci_ctrl->dma_dev, DEVDRV_DMA_DEAD);
    /* report remove to dev manager */
    drvdrv_dev_state_notifier(pci_ctrl);
    if (pci_ctrl->module_exit_flag != DEVDRV_REMOVE_CALLED_BY_MODULE_EXIT) {
        devdrv_set_device_status(pci_ctrl, DEVDRV_DEVICE_DEAD);
    }

    if (pci_ctrl->load_half_probe) {
        devdrv_load_half_free(pci_ctrl);
    }

    devdrv_sysfs_exit(pci_ctrl);
    devdrv_load_probe_free(pci_ctrl);

    devdrv_info(" remove driver OUT. dev_id:%u, bdf:%02x:%02x.%d\n", dev_id, bus_num, device_num, func_num);

    return;
}

void devdrv_shutdown(struct pci_dev *pdev)
{
    if (devdrv_get_product_type() == HOST_TYPE_NORMAL) {
        devdrv_info("shutdown, pci remove driver.\n");
        devdrv_remove(pdev);
    }
    return;
}

void devdrv_dfx_dma_report_to_bbox(struct devdrv_dma_channel *dma_chan, u32 queue_init_sts)
{
    return;
}

static u64 g_aer_fatal_cnt = 0;
static u64 g_aer_nonfatal_cnt = 0;

STATIC pci_ers_result_t devdrv_error_detected(struct pci_dev *pdev, enum pci_channel_state state)
{
    struct devdrv_pci_ctrl *pci_ctrl = (struct devdrv_pci_ctrl *)pci_get_drvdata(pdev);

    devdrv_warn("devid:%u, error detected: state:%d\n", pci_ctrl->dev_id, (int)state);
    devdrv_notify_blackbox_err(pci_ctrl->dev_id, DEVDRV_PCIE_AER_ERROR);
    switch (state) {
        /* I/O channel is in normal state */
        case pci_channel_io_normal:
            devdrv_err("devid:%u, aer_nonfatal_cnt = %llu\n", pci_ctrl->dev_id, ++g_aer_nonfatal_cnt);
            return PCI_ERS_RESULT_CAN_RECOVER;

        /* I/O to channel is blocked */
        case pci_channel_io_frozen:
            devdrv_err("devid:%u, aer_fatal_cnt = %llu\n", pci_ctrl->dev_id, ++g_aer_fatal_cnt);

            if (pci_is_enabled(pdev)) {
                pci_disable_pcie_error_reporting(pdev);
                pci_clear_master(pdev);
                pci_release_regions(pdev);
                pci_disable_device(pdev);
            }
            return PCI_ERS_RESULT_NEED_RESET;

        /* PCI card is dead */
        case pci_channel_io_perm_failure:
            return PCI_ERS_RESULT_DISCONNECT;
    }

    /* Request a slot reset. */
    return PCI_ERS_RESULT_NEED_RESET;
}

STATIC pci_ers_result_t devdrv_slot_reset(struct pci_dev *pdev)
{
    u8 byte = 0;
    int ret;

    devdrv_info("enter slot reset\n");

    if (pci_enable_device(pdev)) {
        devdrv_err("pci_enable_device fail\n");
        return PCI_ERS_RESULT_DISCONNECT;
    }

    pci_read_config_byte(pdev, 0x8, &byte);
    if (byte == 0xff) {
        devdrv_err("slot_reset failed ... got another PCI error !\n");
        return PCI_ERS_RESULT_DISCONNECT;
    }

    if (pci_request_regions(pdev, "devdrv")) {
        devdrv_err("pci_request_regions fail\n");
        return PCI_ERS_RESULT_DISCONNECT;
    }

    /*lint -e598 -e648 */
    if (dma_set_mask_and_coherent(&pdev->dev, DMA_BIT_MASK(DEVDRV_DMA_BIT_MASK_64))) {
        /*lint +e598 +e648 */
        devdrv_err("dma_set_mask 64 bit fail\n");
        ret = dma_set_mask_and_coherent(&pdev->dev, DMA_BIT_MASK(DEVDRV_DMA_BIT_MASK_32));
        if (ret) {
            devdrv_err("dma_set_mask fail ret = %d\n", ret);
            return PCI_ERS_RESULT_DISCONNECT;
        }
    }

    pci_set_master(pdev);

    devdrv_err("the resume of top business(vnic/hdc/devmm) still need to do\n");

    return PCI_ERS_RESULT_RECOVERED;
}

STATIC void devdrv_error_resume(struct pci_dev *pdev)
{
    devdrv_info("enter error resume\n");
    pci_cleanup_aer_uncorrect_error_status(pdev);
    pci_enable_pcie_error_reporting(pdev);
}

static const struct pci_error_handlers g_devdrv_err_handler = {
    .error_detected = devdrv_error_detected,
    .slot_reset = devdrv_slot_reset,
    .resume = devdrv_error_resume,
};

static struct pci_driver g_devdrv_driver_ver = {
    .name = g_devdrv_driver_name,
    .id_table = g_devdrv_tbl,
    .probe = devdrv_probe,
    .remove = devdrv_remove,
    .driver = {
        .name = "devdrv_device_driver",
        .pm = &g_devdrv_pm_ops,
    },
    .err_handler = &g_devdrv_err_handler,
    .shutdown = devdrv_shutdown,
};

void devdrv_init_dev_num(void)
{
    struct pci_dev *pdev = NULL;
    int dev_num = 0;
    int id_num = (int)(sizeof(g_devdrv_tbl) / sizeof(struct pci_device_id));
    int i;

    for (i = 0; i < id_num; i++) {
        pdev = NULL;
        dev_num = -1;

        do {
            pdev = pci_get_device(g_devdrv_tbl[i].vendor, g_devdrv_tbl[i].device, pdev);
            dev_num++;
        } while (pdev != NULL);

        devdrv_info("pci device %x:%x findout dev num %d\n", g_devdrv_tbl[i].vendor, g_devdrv_tbl[i].device, dev_num);

        if (dev_num > 0) {
            break;
        }
    }

    devdrv_info("findout total dev num %d\n", dev_num);

    g_pci_manage_device_num = dev_num;
}

int devdrv_get_dev_num(void)
{
    return g_pci_manage_device_num;
}
EXPORT_SYMBOL(devdrv_get_dev_num);

STATIC int __init devdrv_init_module(void)
{
    int ret;

    devdrv_info("insmod host(type %s) driver for %s\n", g_type, g_devdrv_driver_name);

    if (!strcmp(g_type, "3559")) {
        g_host_type = HOST_TYPE_ARM_3559;
    } else if (!strcmp(g_type, "3519")) {
        g_host_type = HOST_TYPE_ARM_3519;
    } else {
        g_host_type = HOST_TYPE_NORMAL;
    }

    devdrv_init_dev_num();

    (void)devdrv_ctrl_init();
    /*lint -e64 */
    ret = pci_register_driver(&g_devdrv_driver_ver);
    if (ret) {
        devdrv_err("insmod devdrv driver fail ret = %d\n", ret);
        return ret;
    }
    /*lint +e64 */
    devdrv_info("dev num %d\r\n", devdrv_get_dev_num());

    return 0;
}
module_init(devdrv_init_module);

STATIC void __exit devdrv_exit_module(void)
{
    struct devdrv_pci_ctrl *pci_ctrl = NULL;
    struct devdrv_ctrl *ctrl = NULL;
    u32 i;

    for (i = 0; i < MAX_DEV_CNT; i++) {
        ctrl = devdrv_get_top_half_devctrl_by_id(i);
        if ((ctrl == NULL) || (ctrl->priv == NULL)) {
            continue;
        }
        pci_ctrl = (struct devdrv_pci_ctrl *)ctrl->priv;
        pci_ctrl->module_exit_flag = DEVDRV_REMOVE_CALLED_BY_MODULE_EXIT;
    }

    pci_unregister_driver(&g_devdrv_driver_ver);
    return;
}
module_exit(devdrv_exit_module);

MODULE_AUTHOR("Huawei Tech. Co., Ltd.");
MODULE_DESCRIPTION("devdrv host pcie driver");
MODULE_LICENSE("GPL");
