/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2012-2019. All rights reserved.
 * Description:
 * Author: huawei
 * Create:  2020-1-19
 */

#include "devdrv_pcie.h"

#ifndef CAP_SYS_ADMIN
#define CAP_SYS_ADMIN 21
#endif

STATIC int devdrv_manager_check_permission(void)
{
    u32 root;

    root = (u32)(current->cred->euid.val);
    if ((devdrv_manager_container_is_in_container() == 1) || (root != 0)) {
        devdrv_drv_err("now is in container or not root, just return.\n");
        return -EACCES;
    }

    if (!capable(CAP_SYS_ADMIN)) {
        devdrv_drv_err("permission deny, just return.\n");
        return -EACCES;
    }

    return 0;
}

int devdrv_manager_pcie_pre_reset(struct file *filep, unsigned int cmd, unsigned long arg)
{
#ifdef CFG_SOC_PLATFORM_MINI
    int ret;
    struct devdrv_pcie_pre_reset para;

    ret = devdrv_manager_check_permission();
    if (ret) {
        devdrv_drv_err("devdrv_manager_check_permission failed, ret(%d).\n", ret);
        return ret;
    }

    ret = copy_from_user_safe(&para, (void *)((uintptr_t)arg), sizeof(struct devdrv_pcie_pre_reset));
    if (ret) {
        devdrv_drv_err("copy from user failed, ret(%d).\n", ret);
        return -EINVAL;
    }
    ret = devdrv_pcie_prereset(para.dev_id);

    return ret;
#else
    return 0;
#endif
}

int devdrv_manager_pcie_rescan(struct file *filep, unsigned int cmd, unsigned long arg)
{
#ifdef CFG_SOC_PLATFORM_MINI
    struct devdrv_pcie_rescan para;
    int ret;

    ret = devdrv_manager_check_permission();
    if (ret) {
        devdrv_drv_err("devdrv_manager_check_permission failed, ret(%d).\n", ret);
        return ret;
    }

    ret = copy_from_user_safe(&para, (void *)((uintptr_t)arg), sizeof(struct devdrv_pcie_rescan));
    if (ret) {
        devdrv_drv_err("copy from user failed, ret(%d).\n", ret);
        return -EINVAL;
    }
    ret = devdrv_pcie_reinit(para.dev_id);

    return ret;
#else
    return 0;
#endif
}

int devdrv_manager_pcie_hot_reset(struct file *filep, unsigned int cmd, unsigned long arg)
{
    struct devdrv_pcie_hot_reset para;
    int ret;
    int i;

    ret = devdrv_manager_check_permission();
    if (ret) {
        devdrv_drv_err("devdrv_manager_check_permission failed, ret(%d).\n", ret);
        return ret;
    }

    ret = copy_from_user_safe(&para, (void *)((uintptr_t)arg), sizeof(struct devdrv_pcie_hot_reset));
    if (ret) {
        devdrv_drv_err("copy from user failed, ret(%d).\n", ret);
        return -EINVAL;
    }

    if (para.dev_id < DEVDRV_MAX_DAVINCI_NUM) {
        set_time_update_flag(para.dev_id);
    } else {
        for (i = 0; i < DEVDRV_MAX_DAVINCI_NUM; i++) {
            set_time_update_flag(i);
        }
    }

    ret = devdrv_hot_reset_device(para.dev_id);

    return ret;
}

// UEFI SRAM FLAG
#define DEVDRV_SRAM_BBOX_UEFI_DUMP_OFFSET 0x0800
#ifndef CFG_SOC_PLATFORM_CLOUD
#define DEVDRV_SRAM_BBOX_UEFI_DUMP_LEN 20
#else
#define DEVDRV_SRAM_BBOX_UEFI_DUMP_LEN 28
#endif

// BIOS SRAM log (include POINT FLAG)
#define DEVDRV_SRAM_BBOX_BIOS_OFFSET 0x3DC00UL
#define DEVDRV_SRAM_BBOX_BIOS_LEN 0x400UL
/* sram in bar2 */
int drv_pcie_sram_read(struct file *filep, unsigned int cmd, unsigned long arg)
{
    struct devdrv_pcie_sram_read_para pcie_sram_read_para = {0};
    u32 phys_id;
    void __iomem *mem_sram_base = NULL;
    u64 sram_phy_addr = 0;
    size_t sram_size = 0;
    int ret;
    u32 i = 0;

    /* bbox is not support container */
    if (devdrv_manager_container_is_in_container()) {
        devdrv_drv_err("bbox sram read interface is not support container\n");
        return -EPERM;
    }
    ret = copy_from_user_safe(&pcie_sram_read_para, (void *)((uintptr_t)arg),
                              sizeof(struct devdrv_pcie_sram_read_para));
    if (ret) {
        devdrv_drv_err("copy pcie sram from user failed, ret(%d).\n", ret);
        return -EINVAL;
    }
    phys_id = pcie_sram_read_para.devId;

    /* only can read within region for bbox */
    if (!(((pcie_sram_read_para.offset >= DEVDRV_SRAM_BBOX_UEFI_DUMP_OFFSET) &&
           (((u64)pcie_sram_read_para.offset + (u64)pcie_sram_read_para.len) <=
            DEVDRV_SRAM_BBOX_UEFI_DUMP_OFFSET + DEVDRV_SRAM_BBOX_UEFI_DUMP_LEN)) ||
          ((pcie_sram_read_para.offset >= DEVDRV_SRAM_BBOX_BIOS_OFFSET) &&
           (((u64)pcie_sram_read_para.offset + (u64)pcie_sram_read_para.len) <=
            DEVDRV_SRAM_BBOX_BIOS_OFFSET + DEVDRV_SRAM_BBOX_BIOS_LEN)))) {
        devdrv_drv_err("cannot access sram, dev_id:%u, offset:%u, len:%u.\n", phys_id, pcie_sram_read_para.offset,
                       pcie_sram_read_para.len);
        return -EINVAL;
    }

    ret = devdrv_get_addr_info(phys_id, DEVDRV_ADDR_LOAD_RAM, 0, &sram_phy_addr, &sram_size);
    if (ret || (!sram_phy_addr) || (!sram_size)) {
        devdrv_drv_err("get sram addr by dev_id(%u) failed, ret(%d).\n", phys_id, ret);
        return EINVAL;
    }
    if ((pcie_sram_read_para.len == 0) || (pcie_sram_read_para.len > sizeof(pcie_sram_read_para.value))) {
        devdrv_drv_err("value len:%d error!offset:%d, sram_size:%d, dev_id:%u\n", pcie_sram_read_para.len,
                       pcie_sram_read_para.offset, (int)sram_size, phys_id);
        return -EINVAL;
    }
    mem_sram_base = ioremap(sram_phy_addr, sram_size);
    if (mem_sram_base == NULL) {
        devdrv_drv_err("sram ioremap failed. dev_id(%u)\n", phys_id);
        return -EINVAL;
    }
    for (i = 0; i < pcie_sram_read_para.len; i++)
        pcie_sram_read_para.value[i] = *((u8 *)mem_sram_base + pcie_sram_read_para.offset + i);
    iounmap(mem_sram_base);
    mem_sram_base = NULL;

    ret = copy_to_user_safe((void *)(uintptr_t)arg, &pcie_sram_read_para, sizeof(struct devdrv_pcie_sram_read_para));

    return ret;
}

int drv_pcie_sram_write(struct file *filep, unsigned int cmd, unsigned long arg)
{
    struct devdrv_pcie_sram_write_para pcie_sram_write_para = {0};
    u32 phys_id;
    void __iomem *mem_sram_base = NULL;
    u64 sram_phy_addr = 0;
    size_t sram_size = 0;
    int ret;
    u32 i = 0;

    /* bbox is not support container */
    if (devdrv_manager_container_is_in_container()) {
        devdrv_drv_err("bbox sram write interface is not support container\n");
        return -EPERM;
    }
    ret = copy_from_user_safe(&pcie_sram_write_para, (void *)((uintptr_t)arg),
                              sizeof(struct devdrv_pcie_sram_write_para));
    if (ret) {
        devdrv_drv_err("copy pcie sram from user failed, ret(%d).\n", ret);
        return -EINVAL;
    }
    phys_id = pcie_sram_write_para.devId;

    /* only can write within region for bbox */
    if (!((pcie_sram_write_para.offset >= DEVDRV_SRAM_BBOX_UEFI_DUMP_OFFSET) &&
          (((u64)pcie_sram_write_para.offset + (u64)pcie_sram_write_para.len) <=
           DEVDRV_SRAM_BBOX_UEFI_DUMP_OFFSET + DEVDRV_SRAM_BBOX_UEFI_DUMP_LEN))) {
        devdrv_drv_err("cannot access sram, dev_id:%u, offset:%u, len:%u.\n", phys_id, pcie_sram_write_para.offset,
                       pcie_sram_write_para.len);
        return -EINVAL;
    }

    ret = devdrv_get_addr_info(phys_id, DEVDRV_ADDR_LOAD_RAM, 0, &sram_phy_addr, &sram_size);
    if (ret || (!sram_phy_addr) || (!sram_size)) {
        devdrv_drv_err("get sram addr by dev_id(%u) failed, ret(%d).\n", phys_id, ret);
        return -EINVAL;
    }
    if ((pcie_sram_write_para.len == 0) || (pcie_sram_write_para.len > sizeof(pcie_sram_write_para.value))) {
        devdrv_drv_err("value len:%d error!offset:%d, sram_size:%d, dev_id:%u\n", pcie_sram_write_para.len,
                       pcie_sram_write_para.offset, (int)sram_size, phys_id);
        return -EINVAL;
    }
    mem_sram_base = ioremap(sram_phy_addr, sram_size);
    if (mem_sram_base == NULL) {
        devdrv_drv_err("sram ioremap failed. dev_id(%u)\n", phys_id);
        return -EINVAL;
    }
    for (i = 0; i < pcie_sram_write_para.len; i++)
        *((u8 *)mem_sram_base + pcie_sram_write_para.offset + i) = pcie_sram_write_para.value[i];
    iounmap(mem_sram_base);
    mem_sram_base = NULL;

    ret = copy_to_user_safe((void *)((uintptr_t)arg), &pcie_sram_write_para,
                            sizeof(struct devdrv_pcie_sram_write_para));

    return ret;
}

/* bbox ddr in bar4 */
int drv_pcie_bbox_ddr_read(struct file *filep, unsigned int cmd, unsigned long arg)
{
    struct devdrv_pcie_ddr_read_para pcie_ddr_read_para = {0};
    u32 phys_id;
    void __iomem *mem_ddr_base = NULL;
    u64 bbox_ddr_phy_addr = 0;
    size_t bbox_ddr_size = 0;
    int ret;
    u32 i = 0;

    /* bbox is not support container */
    if (devdrv_manager_container_is_in_container()) {
        devdrv_drv_err("bbox ddr read interface is not support container\n");
        return -EPERM;
    }
    ret = copy_from_user_safe(&pcie_ddr_read_para, (void *)((uintptr_t)arg), sizeof(struct devdrv_pcie_ddr_read_para));
    if (ret) {
        devdrv_drv_err("copy pcie ddr from user failed, ret(%d).\n", ret);
        return -EINVAL;
    }
    phys_id = pcie_ddr_read_para.devId;

    ret = devdrv_get_addr_info(phys_id, DEVDRV_ADDR_TEST_BASE, 0, &bbox_ddr_phy_addr, &bbox_ddr_size);
    if (ret || (!bbox_ddr_phy_addr) || (!bbox_ddr_size)) {
        devdrv_drv_err("get bbox ddr addr by dev_id(%u) failed, ret(%d).\n", phys_id, ret);
        return -EINVAL;
    }

    if ((pcie_ddr_read_para.len == 0) || (pcie_ddr_read_para.len > sizeof(pcie_ddr_read_para.value)) ||
        ((u64)pcie_ddr_read_para.offset + (u64)pcie_ddr_read_para.len > bbox_ddr_size)) {
        devdrv_drv_err("value len:%d error!offset:%d,bbox_ddr_size:%d,dev_id:%u\n", pcie_ddr_read_para.len,
                       pcie_ddr_read_para.offset, (int)bbox_ddr_size, phys_id);
        return -EINVAL;
    }
    mem_ddr_base = ioremap(bbox_ddr_phy_addr, bbox_ddr_size);
    if (mem_ddr_base == NULL) {
        devdrv_drv_err("bbox ddr ioremap failed. dev_id(%u)\n", phys_id);
        return -EINVAL;
    }
    for (i = 0; i < pcie_ddr_read_para.len; i++)
        pcie_ddr_read_para.value[i] = *((u8 *)mem_ddr_base + pcie_ddr_read_para.offset + i);
    iounmap(mem_ddr_base);
    mem_ddr_base = NULL;

    ret = copy_to_user_safe((void *)(uintptr_t)arg, &pcie_ddr_read_para, sizeof(struct devdrv_pcie_ddr_read_para));

    return ret;
}

/* bbox imu ddr in bar0 ATU12 */
int drv_pcie_bbox_imu_ddr_read(struct file *filep, unsigned int cmd, unsigned long arg)
{
#ifdef CFG_SOC_PLATFORM_CLOUD
    struct devdrv_pcie_imu_ddr_read_para imu_ddr_read_para = {0};
    void __iomem *mem_ddr_base = NULL;
    u64 bbox_ddr_phy_addr = 0;
    size_t bbox_ddr_size = 0;
    int ret;
    u32 i;

    /* bbox is not support container */
    if (devdrv_manager_container_is_in_container()) {
        devdrv_drv_err("bbox imu ddr read interface is not support container\n");
        return -EPERM;
    }
    ret = copy_from_user_safe(&imu_ddr_read_para, (void *)((uintptr_t)arg),
                              sizeof(struct devdrv_pcie_imu_ddr_read_para));
    if (ret) {
        devdrv_drv_err("copy pcie ddr from user failed, ret(%d).\n", ret);
        return -EINVAL;
    }

    ret = devdrv_get_addr_info(imu_ddr_read_para.devId, DEVDRV_ADDR_IMU_LOG_BASE, 0, &bbox_ddr_phy_addr,
                               &bbox_ddr_size);
    if (ret || (!bbox_ddr_phy_addr) || (!bbox_ddr_size)) {
        devdrv_drv_err("get bbox ddr addr by dev_id(%u) failed, ret(%d).\n", imu_ddr_read_para.devId, ret);
        return -EINVAL;
    }

    if ((imu_ddr_read_para.len == 0) || (imu_ddr_read_para.len > sizeof(imu_ddr_read_para.value)) ||
        ((u64)imu_ddr_read_para.offset + (u64)imu_ddr_read_para.len > bbox_ddr_size)) {
        devdrv_drv_err("value len:%d error!offset:%d,bbox_ddr_size:%d, dev_id:%u\n", imu_ddr_read_para.len,
                       imu_ddr_read_para.offset, (int)bbox_ddr_size, imu_ddr_read_para.devId);
        return -EINVAL;
    }
    mem_ddr_base = ioremap(bbox_ddr_phy_addr, bbox_ddr_size);
    if (mem_ddr_base == NULL) {
        devdrv_drv_err("bbox ddr ioremap failed. dev_id(%u)\n", imu_ddr_read_para.devId);
        return -EINVAL;
    }
    for (i = 0; i < imu_ddr_read_para.len; i++)
        imu_ddr_read_para.value[i] = *((u8 *)mem_ddr_base + imu_ddr_read_para.offset + i);
    iounmap(mem_ddr_base);
    mem_ddr_base = NULL;

    ret = copy_to_user_safe((void *)(uintptr_t)arg, &imu_ddr_read_para, sizeof(struct devdrv_pcie_imu_ddr_read_para));

    return ret;
#else
    return 0;
#endif
}

int devdrv_manager_p2p_attr_op(struct file *filep, unsigned int cmd, unsigned long arg)
{
    struct devdrv_p2p_attr p2p_attr;
    int ret, pid;
    struct devdrv_manager_context *dev_manager_context = filep->private_data;
    u32 phys_id, peer_phys_id;

    pid = dev_manager_context->pid;

    ret = copy_from_user_safe(&p2p_attr, (void *)((uintptr_t)arg), sizeof(struct devdrv_p2p_attr));
    if (ret) {
        devdrv_drv_err("copy from user failed, ret(%d).\n", ret);
        return -EINVAL;
    }

    if (devdrv_manager_container_logical_id_to_physical_id(p2p_attr.dev_id, &phys_id) != 0) {
        devdrv_drv_err("can't transfor virt id %u \n", p2p_attr.dev_id);
        return -EFAULT;
    }

    peer_phys_id = p2p_attr.peer_dev_id;

    ret = -EINVAL;

    switch (p2p_attr.op) {
        case DEVDRV_P2P_ADD:
            ret = devdrv_enable_p2p(pid, phys_id, peer_phys_id);
            break;
        case DEVDRV_P2P_DEL:
            ret = devdrv_disable_p2p(pid, phys_id, peer_phys_id);
            break;
        case DEVDRV_P2P_QUERY:
            if (devdrv_is_p2p_enabled(phys_id, peer_phys_id)) {
                ret = 0;
            }
            break;
        default:
            break;
    }

    return ret;
}

int devdrv_check_va_pte(struct mm_struct *mm, pmd_t *pmd, unsigned long long va, unsigned long long end)
{
    pte_t *pte = NULL;
    int pte_id = 0;

    pte = pte_offset_kernel(pmd, va);

    do {
        if (!pte_none(*pte)) {
            devdrv_drv_err("pte(%d) addr valid\n", pte_id);
            return -EINVAL;
        }
        pte_id++;
        pte++;
        va += PAGE_SIZE;
    } while (va != end);

    return 0;
}

int devdrv_check_va_pmd(struct mm_struct *mm, pud_t *pud, unsigned long long va, unsigned long long end)
{
    pmd_t *pmd = NULL;
    unsigned long long next;
    int ret;

    pmd = pmd_offset(pud, va);

    do {
        next = pmd_addr_end(va, end);

        if (!pmd_none(*pmd)) {
            ret = devdrv_check_va_pte(mm, pmd, va, next);
            if (ret != 0) {
                return ret;
            }
        }
        pmd++;
        va = next;
    } while (va != end);

    return 0;
}

#if LINUX_VERSION_CODE >= KERNEL_VERSION(4, 11, 0)
int devdrv_check_va_pud(struct mm_struct *mm, p4d_t *p4d, unsigned long long va, unsigned long long end)
#else
int devdrv_check_va_pud(struct mm_struct *mm, pgd_t *pgd, unsigned long long va, unsigned long long end)
#endif
{
    pud_t *pud = NULL;
    unsigned long long next;
    int ret;

#if LINUX_VERSION_CODE >= KERNEL_VERSION(4, 11, 0)
    pud = pud_offset(p4d, va);
#else
    pud = pud_offset(pgd, va);
#endif

    do {
        next = pud_addr_end(va, end);
        if (!pud_none(*pud)) {
            ret = devdrv_check_va_pmd(mm, pud, va, next);
            if (ret != 0) {
                return ret;
            }
        }
        pud++;
        va = next;
    } while (va != end);

    return 0;
}

int devdrv_check_va_p4d(struct mm_struct *mm, pgd_t *pgd, unsigned long long va, unsigned long long end)
{
#if LINUX_VERSION_CODE >= KERNEL_VERSION(4, 11, 0)
    p4d_t *p4d = NULL;
    unsigned long long next;
    int ret;

    p4d = p4d_offset(pgd, va);

    do {
        next = p4d_addr_end(va, end);

        if (!p4d_none(*p4d)) {
            ret = devdrv_check_va_pud(mm, p4d, va, next);
            if (ret != 0) {
                return ret;
            }
        }
        p4d++;
        va = next;
    } while (va != end);

    return 0;
#else
    return devdrv_check_va_pud(mm, pgd, va, end);
#endif
}

int devdrv_check_va(struct vm_area_struct *vma, unsigned long long va, unsigned int size)
{
    unsigned long long next;
    unsigned long long end = va + PAGE_ALIGN(size);
    struct mm_struct *mm = vma->vm_mm;
    pgd_t *pgd = NULL;
    int ret;

    if (vma->vm_flags & VM_HUGETLB) {
        devdrv_drv_err("va hute table\n");
        return -EINVAL;
    }

    if (va & (PAGE_SIZE - 1)) {
        devdrv_drv_err("va is invalid\n");
        return -EINVAL;
    }

    if ((va < vma->vm_start) || (va + size > vma->vm_end)) {
        devdrv_drv_err("va or size %x error.\n", size);
        return -EINVAL;
    }

    if (mm->pgd == NULL) {
        return 0;
    }

    pgd = pgd_offset(mm, va);
    do {
        next = pgd_addr_end(va, end);

        if (!pgd_none(*pgd)) {
            ret = devdrv_check_va_p4d(mm, pgd, va, next);
            if (ret != 0) {
                return ret;
            }
        }
        pgd++;
        va = next;
    } while (va != end);
    return 0;
}

int drv_dma_mmap(struct file *filep, unsigned int cmd, unsigned long arg)
{
    struct devdrv_alloc_host_dma_addr_para alloc_host_dma_addr_para = {0};
    unsigned long long bbox_resv_dmaAddr = 0;
    u32 phys_id;
    struct page *bbox_resv_dmaPages = NULL;
    unsigned int bbox_resv_size = 0;
    struct vm_area_struct *vma = NULL;
    int ret;

    ret = copy_from_user_safe(&alloc_host_dma_addr_para, (void *)(uintptr_t)arg,
                              sizeof(struct devdrv_alloc_host_dma_addr_para));
    if (ret) {
        devdrv_drv_err("copy from user failed, ret(%d).\n", ret);
        return -EINVAL;
    }

    phys_id = alloc_host_dma_addr_para.devId;

    /* id conversion is available when devmng start up succ */
    if (devdrv_manager_container_is_in_container()) {
        if (devdrv_manager_container_logical_id_to_physical_id(alloc_host_dma_addr_para.devId, &phys_id) != 0) {
            devdrv_drv_err("can't transfor virt id %u \n", alloc_host_dma_addr_para.devId);
            return -EFAULT;
        }
    }

    down_read(&current->mm->mmap_sem);
    vma = find_vma(current->mm, alloc_host_dma_addr_para.virAddr);
    up_read(&current->mm->mmap_sem);
    if (vma == NULL) {
        devdrv_drv_err("find vma failed. dev_id(%u)\n", phys_id);
        return 0;
    }

    ret = devdrv_get_bbox_reservd_mem(phys_id, &bbox_resv_dmaAddr, &bbox_resv_dmaPages, &bbox_resv_size);
    if (ret) {
        devdrv_drv_err("devdrv_get_bbox_reservd_mem failed, ret(%d). dev_id(%u)\n", ret, phys_id);
        return -EINVAL;
    }

    if (bbox_resv_size > 0) {
        ret = devdrv_check_va(vma, alloc_host_dma_addr_para.virAddr, bbox_resv_size);
        if (ret) {
            devdrv_drv_err("dev_id(%u) va already map, ret(%d).\n", phys_id, ret);
            return -EINVAL;
        }
        ret = remap_pfn_range(vma, vma->vm_start, page_to_pfn(bbox_resv_dmaPages), bbox_resv_size, vma->vm_page_prot);
        if (ret) {
            devdrv_drv_err("remap_pfn_range failed, ret(%d). dev_id(%u)\n", ret, phys_id);
            return -EINVAL;
        }
    }

    devdrv_drv_info("use pcie reserved buffer,"
                    "page:%pK, size:0x%pK, devid:%u.\n",
                    bbox_resv_dmaPages, (void *)(uintptr_t)bbox_resv_size, phys_id);

    alloc_host_dma_addr_para.size = bbox_resv_size;
    alloc_host_dma_addr_para.phyAddr = 0;

    ret = copy_to_user_safe((void *)((uintptr_t)arg), &alloc_host_dma_addr_para,
                            sizeof(struct devdrv_alloc_host_dma_addr_para));

    return ret;
}

#define DEVDRV_MAX_FILE_SIZE (1024 * 100)
#define DEVDRV_STR_MAX_LEN 100
#define DEVDRV_CONFIG_OK 0
#define DEVDRV_CONFIG_FAIL 1
#define DEVDRV_CONFIG_NO_MATCH 1

STATIC loff_t devdrv_get_i_size_read(struct file *p_file)
{
    return i_size_read(file_inode(p_file));
}

STATIC char *devdrv_get_one_line(char *file_buf, u32 file_buf_len, char *line_buf_tmp, u32 buf_len)
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
        devdrv_drv_err("file buf out of bound, offset:%d, file_buf_len:%d\n", offset, file_buf_len);
        return NULL;
    }
    return file_buf;
}

STATIC void devdrv_get_val(const char *buf, u32 buf_len, char *env_val, u32 val_len)
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

STATIC u32 devdrv_get_env_value(char *buf, u32 buf_len, const char *env_name, char *env_val, u32 val_len)
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
        devdrv_drv_err("get value error\n");
        return DEVDRV_CONFIG_FAIL;
    }

    buf++;
    offset++;

    while ((*buf == ' ') || (*buf == '\t')) {
        buf++;
        offset++;
    }

    if (offset >= buf_len) {
        devdrv_drv_err("buf out of bound, offset:%d, buf_len:%d\n", offset, buf_len);
        return DEVDRV_CONFIG_FAIL;
    }
    devdrv_get_val(buf, buf_len - offset, env_val, val_len);
    return DEVDRV_CONFIG_OK;
}

STATIC u32 devdrv_get_env_value_form_file(char *file, u32 file_len, const char *env_name, 
    char *env_val, u32 env_val_len)
{
    struct file *fp = NULL;
    u32 ret;
    u32 filesize;
    u32 len;
    int len_integer;
    loff_t pos = 0;
    u32 match_flag = 0;
    char *buf = NULL;
    char *tmp_buf = NULL;
    char tmp_val[DEVDRV_STR_MAX_LEN];
    int ret_val;
    char line_buf[DEVDRV_STR_MAX_LEN] = {0};

    if (file_len > DEVDRV_STR_MAX_LEN) {
        devdrv_drv_err("file len %d too large\n", file_len);
        return DEVDRV_CONFIG_FAIL;
    }

    fp = filp_open(file, O_RDONLY, 0);
    if (IS_ERR(fp) || (fp == NULL)) {
        devdrv_drv_err("open file(%s) fail\n", file);
        return DEVDRV_CONFIG_FAIL;
    }

    filesize = devdrv_get_i_size_read(fp);
    if (filesize >= DEVDRV_MAX_FILE_SIZE) {
        devdrv_drv_err("file(%s) too large\n", file);
        filp_close(fp, NULL);
        fp = NULL;
        return DEVDRV_CONFIG_FAIL;
    }

    buf = (char *)kmalloc(filesize + 1, GFP_KERNEL);
    if (buf == NULL) {
        devdrv_drv_err("malloc fail\n");
        filp_close(fp, NULL);
        fp = NULL;
        return DEVDRV_CONFIG_FAIL;
    }

    if (memset_s(buf, filesize + 1, 0, filesize + 1) != 0) {
        devdrv_drv_err("memset_s failed\n");
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
        devdrv_drv_err("read file fail\n");
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
        tmp_buf = devdrv_get_one_line(tmp_buf, len + 1, line_buf, sizeof(line_buf));
        if (tmp_buf == NULL) {
            break;
        }

        ret = devdrv_get_env_value(line_buf, DEVDRV_STR_MAX_LEN, env_name, tmp_val, sizeof(tmp_val));
        if (ret != 0) {
            continue;
        }
        len = (u32)strlen(tmp_val);
        if (env_val_len < (len + 1)) {
            kfree(buf);
            filp_close(fp, NULL);
            fp = NULL;
            buf = NULL;
            devdrv_drv_err("env_val_len fail\n");
            return DEVDRV_CONFIG_FAIL;
        }

        ret_val = strcpy_s(env_val, env_val_len, tmp_val);
        if (ret_val != 0) {
            kfree(buf);
            filp_close(fp, NULL);
            fp = NULL;
            buf = NULL;
            devdrv_drv_err("strcpy_s %d fail\n", ret_val);
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


STATIC u32 devdrv_get_user_config_bbox(const char *config_name, char *config_value, u32 config_value_len)
{
    u32 ret;
    char *user_file_path = "/driver/config/project_user_config";
    int ret_val;
    char devdrv_sdk_path[DEVDRV_STR_MAX_LEN] = {0};
    char davinci_config_file[DEVDRV_STR_MAX_LEN] = "/lib/davinci.conf";

    if ((config_name == NULL) || (config_value == NULL)) {
        devdrv_drv_err("config_name(%pK) or config_value(%pK) is NULL!\n", config_name, config_value);
        return DEVDRV_CONFIG_FAIL;
    }

    ret = devdrv_get_env_value_form_file(davinci_config_file, sizeof(davinci_config_file), "DAVINCI_HOME_PATH",
                                         devdrv_sdk_path, sizeof(devdrv_sdk_path));
    if (ret != 0) {
        devdrv_drv_err("get davinci_home_path failed, ret(%d)\n", ret);
        return ret;
    }

    ret_val = strcat_s(devdrv_sdk_path, DEVDRV_STR_MAX_LEN, user_file_path);
    if (ret_val != 0) {
        devdrv_drv_err("strcat_s %d fail\n", ret_val);
        return (u32)(-ret_val);
    }

    return devdrv_get_env_value_form_file(devdrv_sdk_path, sizeof(devdrv_sdk_path), config_name, config_value,
                                          config_value_len);
}

int drv_get_user_config(struct file *filep, unsigned int cmd, unsigned long arg)
{
    struct devdrv_get_user_config_para get_user_config_para = {{0}};
    int ret;

    ret = copy_from_user_safe(&get_user_config_para, (void *)(uintptr_t)arg,
                              sizeof(struct devdrv_get_user_config_para));
    if (ret) {
        devdrv_drv_err("copy from user failed, ret(%d).\n", ret);
        return -EINVAL;
    }
    get_user_config_para.config_name[DEVDRV_USER_CONFIG_NAME_LEN - 1] = '\0';
    get_user_config_para.config_value[DEVDRV_USER_CONFIG_VALUE_LEN - 1] = '\0';
    if ((strlen(get_user_config_para.config_name) != 0) &&
        (strlen(get_user_config_para.config_name) <= DEVDRV_USER_CONFIG_NAME_LEN) &&
        (get_user_config_para.config_value_len <= DEVDRV_USER_CONFIG_VALUE_LEN)) {
        ret = devdrv_get_user_config_bbox(get_user_config_para.config_name, get_user_config_para.config_value,
                                          get_user_config_para.config_value_len);
        if (ret) {
            /* no error log when not find */
            return -EINVAL;
        }
        ret = copy_to_user_safe((void *)(uintptr_t)arg, &get_user_config_para,
                                sizeof(struct devdrv_get_user_config_para));
    } else {
        ret = -1;
        devdrv_drv_err("config_name len is %lu, config value len %d.\n",
                       strlen(get_user_config_para.config_name), get_user_config_para.config_value_len);
    }

    return ret;
}

int drv_get_device_boot_status(struct file *filep, unsigned int cmd, unsigned long arg)
{
    struct devdrv_get_device_boot_status_para get_device_boot_status_para = {0};
    int ret;
    u32 phys_id;
    ret = copy_from_user_safe(&get_device_boot_status_para, (void *)((uintptr_t)arg),
                              sizeof(struct devdrv_get_device_boot_status_para));
    if (ret) {
        devdrv_drv_err("copy from user failed, ret(%d).\n", ret);
        return -EINVAL;
    }

    phys_id = get_device_boot_status_para.devId;

    /* id conversion is available when devmng start up succ */
    if (devdrv_manager_container_is_in_container()) {
        if (devdrv_manager_container_logical_id_to_physical_id(get_device_boot_status_para.devId, &phys_id) != 0) {
            devdrv_drv_err("can't transfor virt id %u \n", get_device_boot_status_para.devId);
            return -EFAULT;
        }
    }

    ret = devdrv_get_device_boot_status(phys_id, &get_device_boot_status_para.boot_status);
    if (ret) {
        devdrv_drv_err("cannot get device boot status, ret(%d), dev_id(%u).\n", ret, phys_id);
        return -EINVAL;
    }

    ret = copy_to_user_safe((void *)((uintptr_t)arg), &get_device_boot_status_para,
                            sizeof(struct devdrv_get_device_boot_status_para));

    return ret;
}

int devdrv_manager_get_host_phy_mach_flag(struct file *filep, unsigned int cmd, unsigned long arg)
{
    struct devdrv_get_host_phy_mach_flag_para para = {0};
    int ret;

    ret = copy_from_user_safe(&para, (void *)((uintptr_t)arg), sizeof(struct devdrv_get_host_phy_mach_flag_para));
    if (ret) {
        devdrv_drv_err("copy from user failed, ret(%d).\n", ret);
        return -EINVAL;
    }

    ret = devdrv_get_host_phy_mach_flag(para.devId, &para.host_flag);
    if (ret) {
        devdrv_drv_err("cannot get host flag, dev_id(%u).\n", para.devId);
        return -EINVAL;
    }

    ret = copy_to_user_safe((void *)((uintptr_t)arg), &para, sizeof(struct devdrv_get_host_phy_mach_flag_para));

    return ret;
}

int devdrv_manager_get_master_device_in_the_same_os(struct file *filep, unsigned int cmd, unsigned long arg)
{
    struct devdrv_get_device_os_para get_device_os_para = {0};
    int ret;

    ret = copy_from_user_safe(&get_device_os_para, (void *)((uintptr_t)arg),
                              sizeof(struct devdrv_get_device_os_para));
    if (ret) {
        devdrv_drv_err("copy from user failed, ret(%d).\n", ret);
        return -EINVAL;
    }

    ret = devdrv_get_master_devid_in_the_same_os(get_device_os_para.dev_id, &(get_device_os_para.master_dev_id));
    if (ret) {
        devdrv_drv_err("failed, ret(%d), dev_id(%u).\n", ret, get_device_os_para.dev_id);
        return ret;
    }

    ret = copy_to_user_safe((void *)((uintptr_t)arg), &get_device_os_para,
                            sizeof(struct devdrv_get_device_os_para));

    return ret;
}


