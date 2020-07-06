/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2012-2019. All rights reserved.
 * Description: This file contains api definition for memory manager driver.
 * Author: huawei
 * Create: 2019-06-11
 */

#ifndef __DEVMM_PROC_INFO_H__
#define __DEVMM_PROC_INFO_H__
#include <linux/err.h>
#include <linux/errno.h>
#include <linux/list.h>
#include <linux/spinlock.h>
#include <linux/threads.h>
#include <linux/types.h>
#include <linux/cdev.h>
#include <linux/mm.h>
#include <linux/mmu_notifier.h>
#include <linux/version.h>
#include <linux/semaphore.h>
#include <linux/atomic.h>

#include <linux/sched.h>
#include <linux/jiffies.h>

#include "devmm_dev.h"
#include "devdrv_interface.h"
#include "drv_log.h"

#define module_devmm "devmm"
#define DEVMM_DEVICE_AUTHORITY 0440
#define DEVMM_GET_DEVICE_SEMA_TIME 500
#define DEVMM_MEM_HBM_ID_OFFSET 2
#define DEVMM_MEM_P2P_DDR_ID_OFFSET 2

#ifndef DEVMM_UT
u32 devmm_get_proc_printf_flag(void);
#define devmm_drv_err(fmt, ...) do { \
    if (devmm_get_proc_printf_flag() == DEVMM_TRUE) {                                                         \
        drv_err(module_devmm, "<%s:%d,%d> " fmt, current->comm, current->tgid, current->pid, ##__VA_ARGS__);  \
    } else {                                                                                                  \
        drv_warn(module_devmm, "<%s:%d,%d> " fmt, current->comm, current->tgid, current->pid, ##__VA_ARGS__); \
    }                                                                                                         \
} while (0)
#else
#define devmm_drv_err(fmt, ...) \
    drv_err(module_devmm, "<%s:%d,%d> " fmt, current->comm, current->tgid, current->pid, ##__VA_ARGS__)
#endif

#define devmm_drv_warn(fmt, ...) \
    drv_warn(module_devmm, "<%s:%d,%d> " fmt, current->comm, current->tgid, current->pid, ##__VA_ARGS__)
#define devmm_drv_info(fmt, ...) \
    drv_info(module_devmm, "<%s:%d,%d> " fmt, current->comm, current->tgid, current->pid, ##__VA_ARGS__)
#define devmm_drv_debug_arg(fmt, ...) \
    drv_debug(module_devmm, "<%s:%d,%d> " fmt, current->comm, current->tgid, current->pid, ##__VA_ARGS__)
#define devmm_drv_switch(fmt, ...) (void)0

#define devmm_drv_info_if(cond, fmt, ...) \
    if (cond)                             \
    devmm_drv_info(fmt, ##__VA_ARGS__)

#define devmm_drv_warn_if(cond, fmt, ...) \
    if (cond)                             \
    devmm_drv_warn(fmt, ##__VA_ARGS__)

#define devmm_drv_err_if(cond, fmt, ...) \
    if (cond)                            \
    devmm_drv_err(fmt, ##__VA_ARGS__)

#ifdef DEVMM_UT
#define STATIC
#else
#define STATIC static
#endif

#define DEVMM_FAULT_OK VM_FAULT_NOPAGE

/*
 * linux kernel < 3.11 not defined VM_FAULT_SIGSEGV,
 * euler LINUX_VERSION_CODE 3.10 defined VM_FAULT_SIGSEGV
 */
#ifdef VM_FAULT_SIGSEGV
#define DEVMM_FAULT_ERROR VM_FAULT_SIGSEGV
#else
#define DEVMM_FAULT_ERROR VM_FAULT_ERROR
#endif

#ifndef CHECK_RET
#define CHECK_RET(a, ret) \
    {                     \
        if (!(a)) {       \
            return (ret); \
        }                 \
    }
#endif

#ifndef CHECK_DO_SOMETHING
#define CHECK_DO_SOMETHING(a, something) \
    {                                    \
        if (!(a)) {                      \
            something;                   \
        }                                \
    }

#endif

#define devmm_pin_page(page) get_page(page)

#define DEVMM_QUERY_DEVICE_EXIT_DELAY 100 /* query device process exited  period  */
#define DEVMM_QUERY_DEVICE_EXIT_MAXTIME 6000 /* 10 times for ts max timeout DELAY*MAXTIME */

#define DEVMM_SETUP_PRE_PRINTF_NUM 200
#define DEVMM_SETUP_RETRY_NUM 1000
#define DEVMM_SETUP_MAX_RETRY_NUM 15000
#define DEVMM_SETUP_DELAYMS_PER_TURN 50 /* 100*500ms is 50s */
#define DEVMM_GET_IDLE_TIMEOUT  50 /* 1000*50ms is 50s */
#define DEVMM_SETUP_INVAL_PID (-1) /* to CREATE_WAIT */
#define DEVMM_SETUP_RECREATE_PID (-2) /* to RE CREATE */
#define DEVMM_SETUP_SYNC_PID (-3)     /* wait until device procs free idle proc-space */

#define DEVMM_PTE_CACHE_READ_COUNT_INDEX_NUM 16

#define DEVMM_SHOW_PROCESS_SET_DEX 10

#define PXD_JUDGE(pxd) (((pxd) == NULL) || pxd##_none(*(pxd##_t *)(pxd)) || pxd##_bad(*(pxd##_t *)(pxd)))

#define DEVMM_MAX_HOST_CHIP_NUM 64

enum {
    DEVMM_CHAN_MEMSET8 = 0,
    DEVMM_CHAN_MEMSET16,
    DEVMM_CHAN_MEMSET32,
    DEVMM_CHAN_MEMSET64,
    DEVMM_CHAN_MEMSET_MAX,
};

struct devmm_page_query_arg {
    u32 dev_id;
    u32 hostpid;
    unsigned long va;
    unsigned long size;
    unsigned long offset;
    u32 page_size;
};

struct devmm_pa_list_info {
    u32 pin_flg;
    u32 write;
    unsigned long *palist;
    struct page **pages;
    u32 *szlist;
};


struct devmm_p2p_page_query_arg {
    unsigned int dev_id;
    unsigned int hostpid;
    unsigned long va;
    unsigned long page_size;
};

struct devmm_svm_heap {
    u32 heap_type;
    u32 heap_idx;
    u32 chunk_page_size;
    unsigned long start;
    unsigned long end;

    /*
     * page_bitmap define see devmm_dev.h define
     */
    u32 *page_bitmap;
};

struct devmm_ttbrinfo {
    u64 ttbr;
    u32 ssid;
    u64 tcr;
    void *l2buffer;
};

struct devmm_deviceinfo {
    pid_t devpid;
    u32 devid;
    u32 cluster_id;
    struct devmm_ttbrinfo device_ttbrinfo;
};

struct devmm_setupdevice {
    u32 dev_setup_map;   /* use for user thread to set ai cpu para */
    u32 dev_setuped_map; /* use for user thread flaged ai cpu fun para seted */
    u32 dev_daemon_inited;
    struct semaphore setup_sema;
};

#define DEVMM_P2P_FAULT_PAGE_MAX_NUM 16

struct devmm_p2p_pg_info {
    u32 pg_num;
    unsigned long pa[DEVMM_P2P_FAULT_PAGE_MAX_NUM];
};

struct devmm_huge_pg_info {
    struct page *hpage;
};

struct devmm_fault_info {
    int fault_ret;
    int p2p_flag;
    unsigned long fault_addr;
    struct devmm_svm_heap *heap;
    struct devmm_huge_pg_info huge_page_info;
    struct devmm_p2p_pg_info p2p_page_info;
};

#define DEVMM_SVM_MAX_AICORE_NUM 32
struct devmm_fault_err {
    u64 fault_addr;
    u32 fault_cnt;
};

#define DEVMM_SVM_PRO_MAX_PRINT_NUM 100
#define DEVMM_SVM_PRO_ERR_PRINT_PERIOD msecs_to_jiffies(2000)

struct devmm_err_print_ctl {
    u32 cnt;
    unsigned long jiffies;
};

#define DEVMM_MAX_IPC_MEM_NUM 8192 /* 8k */
#define DEVMM_MAX_IPC_NODE_LIST_NUM 32
#define DEVMM_IPC_NODE_ADDR_SHIFT  21 /* 2M mask */

struct devmm_ipc_mem_node {
    void *ipc_mem_node;
    u8 idx;
    struct list_head list;
};

struct devmm_ipc_mem_node_head {
    struct list_head head[DEVMM_MAX_IPC_NODE_LIST_NUM];
    struct mutex node_mutex;
    u32 node_cnt;
};

struct devmm_svm_process {
    u32 inited;
    u32 normal_exited; /* recv close-msg from host,will set this flag */
    pid_t hostpid;
    pid_t devpid;
    /*
     * start_addr:mapped start addr of this process
     * brk_addr:used add,we used range start_addr~brk_addr
     * brk addr=start_addr+heap_size*heap_cnt
     * end_addr:mapped end addr of this process
     */
    unsigned long start_addr;
    unsigned long end_addr;

    atomic64_t alloc_page_cnt;
    atomic64_t free_page_cnt;
    atomic64_t alloc_hugepage_cnt;
    atomic64_t free_hugepage_cnt;

    struct devmm_ttbrinfo local_ttbrinfo;
    struct devmm_deviceinfo deviceinfo[DEVMM_MAX_DEVICE_NUM];
    struct devmm_setupdevice setup_dev;

    struct devmm_ipc_mem_node_head ipc_node;

    struct vm_area_struct *vma;
    struct mm_struct *mm;
    struct task_struct *tsk;
    struct devmm_err_print_ctl err_print;
    struct devmm_fault_err fault_err[DEVMM_SVM_MAX_AICORE_NUM];
    struct mmu_notifier notifier;
    struct semaphore fault_sem;
    struct semaphore huge_fault_sem;
    struct rw_semaphore bitmap_sem;
    struct semaphore ioctl_sem;
    int doc_idx;
    struct devmm_svm_heap heaps[DEVMM_MAX_HEAP_NUM];
    struct list_head dma_list;
};

struct devmm_svm_doc_pro {
    uintptr_t doc_mnt_namespace;
    u32 svm_doc_pro_cnt;
    u8 svm_doc_flag;
};

extern struct mmu_notifier_ops devmm_process_mmu_notifier;

struct devmm_polling_process {
    wait_queue_head_t inq;
};

struct devmm_l2buff_info {
    phys_addr_t l2buff;
    size_t l2size;
};

struct devmm_dma_block {
    unsigned long pa;
    unsigned int sz;
    struct page *page;
    dma_addr_t dma;
};

struct devmm_rc_info {
    u32 rc_reg[DEVMM_PTE_CACHE_READ_COUNT_INDEX_NUM];
};

struct devmm_nid_info {
    int nid_array[DEVMM_MEM_TYPE_MAX][DEVMM_MAX_AGENTMM_DEVICE_NUM];
    u32 p2p_nid_mark[DEVMM_MAX_AGENTMM_DEVICE_NUM];
    int nid_dev;
};

struct devmm_dev_memalloced {
    struct list_head alloced_list;
    phys_addr_t p;
};

struct devmm_dma_node {
    struct list_head dma_node_list;
    struct DMA_ADDR dma_addr;
};

struct devmm_device_info {
    atomic_t devicechipnum;
    u32 devicefirstchipid;
    u32 devicechipid[DEVMM_MAX_DEVICE_NUM];
};

struct devmm_svm_statistics {
    atomic64_t fault_cnt;
    atomic64_t p2p_fault_cnt;
    atomic64_t page_cnt;
    atomic64_t page_lens;
    atomic64_t send_msg_cnt;
    atomic64_t recv_msg_cnt;
    atomic64_t copy_dirs_cnt[DEVMM_COPY_LEN_CNT4];
    atomic64_t copy_lens_cnt[DEVMM_COPY_LEN_CNT32];
    atomic64_t vir_addr_cnt;
    atomic64_t vir_addr_lens;

    atomic64_t page_alloc_cnt;
    atomic64_t page_free_cnt;
    atomic64_t huge_page_alloc_cnt;
    atomic64_t huge_page_free_cnt;

    atomic64_t page_map_cnt;
    atomic64_t page_unmap_cnt;
    atomic64_t huge_page_map_cnt;
    atomic64_t huge_page_unmap_cnt;

    atomic64_t send_p2p_msg_cnt;
    atomic64_t recv_p2p_msg_cnt;

    atomic64_t p2p_page_map_cnt;
    atomic64_t p2p_page_unmap_cnt;
    atomic64_t p2p_huge_page_map_cnt;
    atomic64_t p2p_huge_page_unmap_cnt;

    atomic64_t l2_buf_map_cnt;
    atomic64_t l2_buf_unmap_cnt;
};

#define DEVMM_MAX_DATA_SIZE (32 * 1024)
struct devmm_share_memory_head {
    u64 pa;
    void *va;
    unsigned int total_num;
    unsigned int free_index;
    unsigned int max_offset;
};

struct devmm_report_abort {
    devmm_report_handle_t report_proc_abort;
    struct rw_semaphore reprot_sem; /* lock devmm_report_handle set and use */
};

struct devmm_svm_dev {
    struct cdev char_dev;
    dev_t dev_no;
    struct device *dev;
    /* this fun_pointer is used for reporting proc exit event to devdrv */
    struct devmm_report_abort report_abort;

    /* svm page size,means svm mgmt's page size. 64k */
    u32 svm_page_size;
    u32 svm_page_shift;
    u32 huge_page_size;  // device
    /* heap size,means a 'heap' size, 32G. */
    u64 heap_size;

    /* device page size,means device page size, 64K. */
    u32 host_page_size;
    u32 host_hpage_size;
    u32 device_page_size;
    u32 device_hpage_size;

    u32 host_page_shift;
    u32 host_hpage_shift;
    u32 device_page_shift;
    u32 device_hpage_shift;

    u32 host_page2device_hugepage_order;
    u32 host_page2device_page_order;

    /* used by device */
    struct devmm_l2buff_info local_l2buffinfo[DEVMM_MAX_AGENTMM_DEVICE_NUM];

    /* used by host */
    struct devmm_l2buff_info device_l2buffinfo[DEVMM_MAX_DEVICE_NUM];

    u32 order;
    int hostchipnum;

    struct devmm_device_info device_info;

    atomic_t setup_device_times[DEVMM_MAX_DEVICE_NUM];

    atomic_t ipcnode_sq;
    spinlock_t proc_lock;
    struct semaphore share_memory_sem;
    struct devmm_polling_process polling_process;

    int svm_pro_cnt;
    struct devmm_svm_doc_pro svm_doc[DEVMM_MAX_DOC_NUM];
    struct devmm_svm_process svm_pro[DEVMM_SVM_MAX_PROCESS_NUM];
    struct devmm_svm_statistics stat;

    /* used by host */
    atomic_t device_l2buff_load_program[DEVMM_MAX_DEVICE_NUM];

    struct devmm_rc_info rc_info;
    struct devmm_nid_info nid_info;
    struct devmm_share_memory_head pa_info[DEVMM_MAX_DEVICE_NUM];
};

struct devmm_copy_side {
    struct devmm_dma_block *blks;
    unsigned int num;
    unsigned long blk_page_size;
};

struct devmm_copy_res {
    struct devdrv_dma_prepare *dma_prepare;
    struct devmm_copy_side from;
    struct devmm_copy_side to;
    struct devdrv_dma_node *dma_node;
    unsigned int dma_node_num;
    int copy_direction;
    int pin_flg;
    int dev_id;
    int dst_dev_id;
    int vmalloc_flag;
};

struct devmm_memory_attributes {
    int is_local_host;
    int is_host_pin;
    int is_svm;
    int is_svm_huge;
    int is_svm_host;
    int is_svm_device;
    int is_svm_non_page;
    int is_locked_host;
    int is_locked_device;
    u32 heap_idx;
    u32 devid;
    u32 page_size;
};

struct devmm_mem_aligned {
    DVdeviceptr aligned_down_addr;
    u64 aligned_count;
};

struct page_map_info {
    int nid;
    int is_4G;
    u32 adjust_order;
    unsigned long va;
    u64 page_num;
    struct page **inpages;
};

extern struct devmm_svm_dev *devmm_svm;

#define devmm_svm_pageshift2pagesize(pgshift) ((u32)1ul << (pgshift))

#define devmm_svm_get_host_pgsf() (devmm_svm->host_page_shift)
#define devmm_svm_get_device_pgsf() (devmm_svm->device_page_shift)
#define devmm_svm_set_host_pgsf(pgshift) (devmm_svm->host_page_shift = (pgshift))
#define devmm_svm_set_host_hpgsf(pgshift) (devmm_svm->host_hpage_shift = (pgshift))

#define devmm_svm_set_device_pgsf(pgshift) (devmm_svm->device_page_shift = (pgshift))
#define devmm_svm_set_device_hpgsf(pgshift) (devmm_svm->device_hpage_shift = (pgshift))
#define devmm_svm_set_device_hugepg(pgsize) (devmm_svm->huge_page_size = (pgsize))

#define devmm_svm_stat_fault_inc() (atomic64_inc(&devmm_svm->stat.fault_cnt))
#define devmm_svm_stat_p2p_fault_inc() (atomic64_inc(&devmm_svm->stat.p2p_fault_cnt))

#define devmm_svm_stat_copy_inc(dir, len)               \
    (atomic64_inc(&devmm_svm->stat.copy_dirs_cnt[dir]), \
     atomic64_inc(&devmm_svm->stat.copy_lens_cnt[ilog2((unsigned int)(len))]))

#define devmm_svm_stat_send_inc() (atomic64_inc(&devmm_svm->stat.send_msg_cnt))
#define devmm_svm_stat_recv_inc() (atomic64_inc(&devmm_svm->stat.recv_msg_cnt))
#define devmm_svm_stat_page_inc(len) \
    (atomic64_add(len, &devmm_svm->stat.page_lens), atomic64_inc(&devmm_svm->stat.page_cnt))
#define devmm_svm_stat_page_dec(len) \
    (atomic64_sub(len, &devmm_svm->stat.page_lens), atomic64_dec(&devmm_svm->stat.page_cnt))

#define devmm_svm_stat_p2p_send_inc() (atomic64_inc(&devmm_svm->stat.send_p2p_msg_cnt))
#define devmm_svm_stat_p2p_recv_inc() (atomic64_inc(&devmm_svm->stat.recv_p2p_msg_cnt))

#define devmm_svm_stat_vir_page_inc(len) \
    (atomic64_add(len, &devmm_svm->stat.vir_addr_lens), atomic64_inc(&devmm_svm->stat.vir_addr_cnt))
#define devmm_svm_stat_vir_page_dec(len) \
    (atomic64_sub(len, &devmm_svm->stat.vir_addr_lens), atomic64_dec(&devmm_svm->stat.vir_addr_cnt))
#define devmm_get_current_pid() (current->tgid)

#define devmm_svm_stat_pg_alloc_inc() (atomic64_inc(&devmm_svm->stat.page_alloc_cnt))
#define devmm_svm_stat_pg_free_inc() (atomic64_inc(&devmm_svm->stat.page_free_cnt))
#define devmm_svm_stat_huge_alloc_inc() (atomic64_inc(&devmm_svm->stat.huge_page_alloc_cnt))
#define devmm_svm_stat_huge_free_inc() (atomic64_inc(&devmm_svm->stat.huge_page_free_cnt))
#define devmm_svm_stat_pg_map_inc() (atomic64_inc(&devmm_svm->stat.page_map_cnt))
#define devmm_svm_stat_pg_unmap_inc() (atomic64_inc(&devmm_svm->stat.page_unmap_cnt))
#define devmm_svm_stat_huge_map_inc() (atomic64_inc(&devmm_svm->stat.huge_page_map_cnt))
#define devmm_svm_stat_huge_unmap_inc() (atomic64_inc(&devmm_svm->stat.huge_page_unmap_cnt))

#define devmm_svm_stat_p2p_map_inc() (atomic64_inc(&devmm_svm->stat.p2p_page_map_cnt))
#define devmm_svm_stat_p2p_unmap_inc() (atomic64_inc(&devmm_svm->stat.p2p_page_unmap_cnt))
#define devmm_svm_stat_p2p_huge_map_inc() (atomic64_inc(&devmm_svm->stat.p2p_huge_page_map_cnt))
#define devmm_svm_stat_p2p_huge_unmap_inc() (atomic64_inc(&devmm_svm->stat.p2p_huge_page_unmap_cnt))

#define devmm_svm_stat_l2_map_inc() (atomic64_inc(&devmm_svm->stat.l2_buf_map_cnt))
#define devmm_svm_stat_l2_unmap_inc() (atomic64_inc(&devmm_svm->stat.l2_buf_unmap_cnt))

#define devmm_host_hugepage_fault_adjust_order() (devmm_svm->host_page2device_hugepage_order)

enum devmm_page_convert_type {
    DEVMM_PIN_PAGES = 0,
    DEVMM_UNPIN_PAGES = 1,
    DEVMM_DEVICE_PAGES = 2,
    DEVMM_USER_PIN_PAGES = 3,
};

typedef int (*devmm_va_to_pa_process)(const struct vm_area_struct *vma, unsigned long va, unsigned long *pa);
typedef int (*devmm_make_raw_dmanode_list_fun)(struct devmm_mem_copy_convrt_para arg,
                                               struct devmm_svm_process *svm_process,
                                               struct devmm_copy_res *buf,
                                               struct devmm_memory_attributes *src_attr,
                                               struct devmm_memory_attributes *dst_attr);

struct device *devmm_devid_to_device(u32 dev_id);
struct devmm_svm_process *devmm_get_svm_pro(pid_t pid);
struct devmm_svm_process *devmm_get_pro_by_pid(pid_t pid, u32 inited, enum devmm_endpoint_type endtype);
struct devmm_svm_process *devmm_get_svm_pro_by_dev_vma(struct vm_area_struct *vma);
struct devmm_svm_process *devmm_get_idle_process_set_init(pid_t host_pid, pid_t dev_pid, u32 inited);
struct devmm_svm_process *devmm_get_pre_initing_pro(pid_t dev_pid, pid_t set_dev_pid, u32 inited, u32 initing);
struct devmm_svm_process *devmm_get_initing_pro_by_pid(pid_t pid, enum devmm_endpoint_type endtype);
struct devmm_svm_heap *devmm_svm_get_heap(struct devmm_svm_process *svm_process, unsigned long va);
int devmm_va_to_pa(const struct vm_area_struct *vma, unsigned long va, unsigned long *pa);
int devmm_get_va_to_pa(const struct vm_area_struct *vma, u32 heap_type, u64 va, u64 *pa);
int devmm_unmap(struct devmm_svm_process *svm_process, unsigned long vaddr);
int devmm_unmap_pages(struct devmm_svm_process *svm_process, unsigned long vaddr, u32 adjust_order);
int devmm_va_is_not_svm_process_addr(const struct devmm_svm_process *svm_process, unsigned long va);
int devmm_insert_pages_to_vma(struct devmm_svm_process *svm_process, unsigned long va, unsigned int adjust_order,
                              struct page **inpages);
int devmm_insert_range(struct vm_area_struct *vma, int nid, unsigned long va, u64 page_cnt);
int devmm_insert_pages_range_to_vma(struct devmm_svm_process *svm_process, unsigned long va,
                                    u64 page_num, struct page **inpages);
int devmm_va_to_palist(const struct vm_area_struct *vma, unsigned long va, unsigned long sz, unsigned long *pa,
                       unsigned int *num);
struct page *devmm_insert(struct page_map_info *page_map_info, struct devmm_svm_process *svm_process);
void devmm_print_phy_meminfo(int nid);
int devmm_get_another_nid(unsigned int nid);
int devmm_get_alloc_mask(void);
int devmm_alloc_pages(int nid, int is_4G, u64 page_num, struct page **outpages, struct devmm_svm_process *svm_process);
void devmm_free_pages(u64 page_num, struct page **outpages, struct devmm_svm_process *svm_process);
int devmm_dev_is_self_system(unsigned int dev_id);
int devmm_insert_virt_to_vma(struct vm_area_struct *vma, unsigned long va, unsigned int adjust_order,
                             unsigned long *paddr);
void devmm_merg_pa(unsigned long *palist, u32 pa_idx, u32 pgsz, u32 *merg_szlist, u32 *merg_idx);
int devmm_txatu_target_to_base(u32 devid, phys_addr_t target_addr, phys_addr_t *base_addr);
void devmm_zap_vma_ptes(struct vm_area_struct *vma, unsigned long vaddr, unsigned long size);
int devmm_va_to_pmd(const struct vm_area_struct *vma, unsigned long va, int huge_flag, pmd_t **tem_pmd);
void devmm_release_svm_process(struct devmm_svm_process *);
pgprot_t devmm_make_pgprot(unsigned int flg);
struct devmm_svm_process *devmm_svm_mmap_init_struct(struct vm_operations_struct *ops_managed,
                                                     struct vm_area_struct *vma);
int devmm_unpin_page_and_get_count(struct page *tem_page);
void devmm_init_dev_private(struct devmm_svm_dev *dev, struct file_operations *svm_fops);
void devmm_uninit_dev_private(struct devmm_svm_dev *dev);
int devmm_dma_sync_link_copy(u32 dev_id, struct devdrv_dma_node *dma_node, u32 node_cnt);
void devmm_release_private_proc(struct devmm_svm_process *svm_proc);
void devmm_notifier_release_private(struct devmm_svm_process *svm_pro);
int devmm_svm_get_svm_proc_and_heap(u32 pid, u64 va, struct devmm_svm_process **svm_process,
    struct devmm_svm_heap **heap);
void devmm_svm_ioctl_lock(struct devmm_svm_process *svm_proc);
void devmm_svm_ioctl_unlock(struct devmm_svm_process *svm_proc);
void devmm_put_page(struct page *tem_page, struct devmm_svm_process *svm_process);
/* host */
int devmm_ioctl_setup_device(struct devmm_svm_process *svm_pro, struct devmm_ioctl_arg *arg);
u32 devmm_svm_va_to_devid(unsigned long va);
int devmm_svm_get_vaflgs_by_pid(pid_t pid, unsigned long va, unsigned int dev_id, u32 *flag);
u32 *devmm_get_page_bitmap_by_va(u64 va);
u32 *devmm_get_page_bitmap(struct devmm_svm_process *svm_process, u64 va);
u32 *devmm_get_page_bitmap_with_heapidx(struct devmm_svm_process *svm_process, u32 heap_idx, u64 va);
void devmm_svm_set_mapped_with_heap(pid_t host_pid, unsigned long va, size_t size, unsigned int devid,
                                    struct devmm_svm_heap *heap);
void devmm_svm_clear_mapped_with_heap(pid_t host_pid, unsigned long va, size_t size, unsigned int devid,
                                      struct devmm_svm_heap *heap);
int devmm_query_page_by_msg(struct devmm_page_query_arg query_arg, unsigned int msg_id,
    struct devmm_dma_block *blks, unsigned int *num);
int devmm_insert_host_range(struct devmm_svm_process *svm_process, unsigned long va, unsigned long sz,
    u32 adjust_order);
int devmm_chan_query_meminfo_h2d(void *msg, u32 *ack_len);
int devmm_page_fault_h2d_sync(unsigned int dev_id, struct page **pages, unsigned long va, u32 adjust_order,
                              const struct devmm_svm_heap *heap);
void devmm_print_pre_alloced_va(struct devmm_svm_process *svm_process, u64 va);
u32 devmm_get_adjust_order_by_heap(struct devmm_svm_heap *heap);
void devmm_lock_report_handle(void);
void devmm_unlock_report_handle(void);
void devmm_get_memory_attributes(struct devmm_svm_process *svm_process,
    DVdeviceptr addr, struct devmm_memory_attributes *attributes);
int devmm_check_status_va_info(struct devmm_svm_process *svm_process, u64 va, u64 count);
/* host end
 * device
 */
int devmm_unmap_page_range(struct devmm_svm_process *svm_process, unsigned long vaddr, u64 num);
int devmm_pa_is_p2p_addr(unsigned long pa);
void devmm_unpin_page(struct page *page, struct devmm_svm_process *svm_process);
/* extern end */
#endif /* __DEVMM_PROC_INFO_H__ */
