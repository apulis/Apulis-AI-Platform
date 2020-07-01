/*
 * hw_vdavinci.h
 *
 * Created on: xxx
 * Author: xxx
 */
#ifndef HW_VDAVINCI_H_
#define HW_VDAVINCI_H_

#include <linux/list.h>
#include <linux/scatterlist.h>
#include <linux/uuid.h>

#define RESERVE_SIZE 40

enum HW_VDAVINCI_TYPE {
    HW_VDAVINCI_TYPE_D0,
    HW_VDAVINCI_TYPE_D1,
    HW_VDAVINCI_TYPE_D2,
    HW_VDAVINCI_TYPE_D3,
    HW_VDAVINCI_TYPE_D4,
    HW_VDAVINCI_TYPE_MAX
};

struct vdavinci_priv_ops {
    int (*vdavinci_create)(struct device *dev, void *vdavinci, u32 fid, enum HW_VDAVINCI_TYPE type, uuid_le uuid);
    void (*vdavinci_destroy)(struct device *dev, u32 fid);
    void (*vdavinci_release)(struct device *dev, u32 fid);
    int (*vdavinci_reset)(struct device *dev, u32 fid);
    void (*vdavinci_notify)(struct device *dev, u32 fid, int db_index);
};

struct vdavinci_priv {
    struct device *dev;
    void *dvt;
    struct vdavinci_priv_ops *ops;
    char reserve[RESERVE_SIZE];
};

int hw_dvt_init(struct vdavinci_priv *dev_priv);
int hw_dvt_uninit(struct vdavinci_priv *dev_priv);

/* *
 * hw_dvt_hypervisor_inject_msix - inject a MSIX interrupt into vdavinci
 *
 * Returns:
 * Zero on success, negative error code if failed.
 */
int hw_dvt_hypervisor_inject_msix(void *vdavinci, u32 vector);

/* *
 * hw_dvt_hypervisor_read_gpa - copy data from GPA to host data buffer
 * @vdavinci: a vdavinci
 * @gpa: guest physical address
 * @buf: host data buffer
 * @len: data length
 *
 * Returns:
 * Zero on success, negative error code if failed.
 */
int hw_dvt_hypervisor_read_gpa(void *vdavinci, unsigned long gpa, void *buf, unsigned long len);

/* *
 * hw_dvt_hypervisor_write_gpa - copy data from host data buffer to GPA
 * @vdavinci: a vdavinci
 * @gpa: guest physical address
 * @buf: host data buffer
 * @len: data length
 *
 * Returns:
 * Zero on success, negative error code if failed.
 */
int hw_dvt_hypervisor_write_gpa(void *vdavinci, unsigned long gpa, void *buf, unsigned long len);

/* *
 * hw_dvt_hypervisor_gfn_to_mfn - translate a GFN to MFN
 * @vdavinci: a vdavinci
 * @gpfn: guest pfn
 *
 * Returns:
 * MFN on success, hw_dvt_INVALID_ADDR if failed.
 */
unsigned long hw_dvt_hypervisor_gfn_to_mfn(void *vdavinci, unsigned long gfn);

/* *
 * hw_dvt_hypervisor_dma_map_guest_page - setup dma map for guest page
 * @vdavinci: a vdavinci
 * @gfn: guest pfn
 * @size: page size
 * @dma_sgt: retrieve allocated dma addr list(sg_table)
 *
 * Returns:
 * 0 on success, negative error code if failed.
 */
int hw_dvt_hypervisor_dma_map_guest_page(void *vdavinci, unsigned long gfn, unsigned long size,
    struct sg_table **dma_sgt);

/* *
 * hw_dvt_hypervisor_dma_unmap_guest_page - cancel dma map for guest page
 * @vdavinci: a vdavinci
 * @dma_sgt: the dma addr list(sg_table)
 */
void hw_dvt_hypervisor_dma_unmap_guest_page(void *vdavinci, struct sg_table *dma_sgt);
/* *
 * hw_dvt_hypervisor_is_valid_gfn - check if a visible gfn
 * @vdavinci: a vdavinci
 * @gfn: guest PFN
 *
 * Returns:
 * true on valid gfn, false on not.
 */
bool hw_dvt_hypervisor_is_valid_gfn(void *vdavinci, unsigned long gfn);

/* *
 * hw_dvt_hypervisor_mmio_get - get the mmio address and size of specified bar
 * @dst: return the mmio address to host driver
 * @size: retrun the mmio size to host driver
 * @vdavinci: a vdavinci
 * @bar: specify the bar[2/4]
 *
 * Returns:
 * true on valid gfn, false on not.
 */

int hw_dvt_hypervisor_mmio_get(void **dst, int *size, void *vdavinci, int bar);

#endif /* HW_VDAVINCI_H_ */
