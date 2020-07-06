/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2012-2019. All rights reserved.
 * Description: This file contains api definition for memory manager driver.
 * Author: huawei
 * Create: 2017-10-15
 */

#ifndef __MM_API_DEV_H
#define __MM_API_DEV_H

#include <stdbool.h>
#include <linux/ioctl.h>

#ifndef __KERNEL__
#include "ascend_hal.h"
#else
#include <asm/processor.h>
#include <linux/atomic.h>
#include <linux/version.h>

typedef unsigned long long UINT64;
typedef unsigned int UINT32;
typedef unsigned short UINT16;
typedef unsigned char UINT8;

#define DV_ADVISE_HBM 0x0000
#define DV_ADVISE_DDR 0x0001
#define DV_ADVISE_EXEC 0x0002
#define DV_ADVISE_HUGEPAGE 0x0004
#define DV_ADVISE_POPULATE 0x0008
#define DV_ADVISE_P2P_HBM 0x0010
#define DV_ADVISE_P2P_DDR 0x0020
#define DV_ADVISE_4G_DDR 0x0040

typedef UINT32 DVmem_advise;
typedef UINT32 DVdevice;
typedef UINT64 DVdeviceptr;

struct DMA_OFFSET_ADDR {
    UINT64 offset;
    UINT32 devid;
};

struct DMA_PHY_ADDR {
    void *src;  /* 源地址(物理地址) */
    void *dst;  /* 目的地址(物理地址) */
    UINT32 len; /* 长度 */
    UINT8 flag; /* Flag= 0：非链式，src和dst为物理地址，可以直接进行DMA拷贝操作；
                   Flag=1：链式，src为dma链表的地址，可以直接进行DMA拷贝操作； */
    void *priv;
};

struct DMA_ADDR {
    union {
        struct DMA_PHY_ADDR phyAddr;
        struct DMA_OFFSET_ADDR offsetAddr;
    };
    UINT32 fixed_size; /* 实际转化大小，由于硬件限制，每次最大为64M */
    UINT32 virt_id; /* 存储逻辑id，为释放地址使用 */
};

#define DV_LOCK_HOST 0x0001
#define DV_LOCK_DEVICE 0x0002
#define DV_UNLOCK 0

#endif

#define DEVMM_MAX_DEVICE_NUM 64
#define DEVMM_MAX_AGENTMM_DEVICE_NUM 4
#define DEVMM_MAX_AGENTMM_CLUSTER_NUM DEVMM_MAX_DEVICE_NUM
#define DEVMM_MAX_DOC_NUM 9
#define DEVMM_MAX_DOC_PROCESS_NUM 32
#define DEVMM_SVM_MAX_PROCESS_NUM (DEVMM_MAX_DOC_PROCESS_NUM * DEVMM_MAX_DOC_NUM)

#define DEVMM_MAX_BLOCKS_NUM 100
#define DEVMM_MAX_DUMP_MEM_NUM (128)
#define DEVMM_CENTOR_MNS_DATA (-1)

#define DEVMM_SVM_DEV_NAME "devmm_svm"
#define DEVMM_SVM_DEV_PATH "/dev/"
#define DEVMM_SVM_MEM_START 0x100000000000ULL

#define DEVMM_HEAP_RANGE 35
#define DEVMM_DEV_MAPPED_RANGE 37
#define DEVMM_POOL_VM_RANGE (1UL << DEVMM_HEAP_RANGE) /* 32G */
#define DEVMM_HEAP_SIZE DEVMM_POOL_VM_RANGE
#define DEVMM_MAPPEDSZ_PER_DEV (1UL << DEVMM_DEV_MAPPED_RANGE)                 /* 128G */
#define DEVMM_MAX_MAPPED_RANGE (DEVMM_MAPPEDSZ_PER_DEV * DEVMM_MAX_DEVICE_NUM) /* 8T */
#define DEVMM_MAX_HEAP_NUM (DEVMM_MAX_MAPPED_RANGE >> DEVMM_HEAP_RANGE)
#define DEVMM_SVM_MEM_SIZE DEVMM_MAX_MAPPED_RANGE

#define DEVMM_POOL_DESTROY_THREADHOLD 16
#define DEVMM_POOL_PINNED_MEM_DESTROY_THREDHOLD 4

#define DEVMM_DMA_SMALL_PACKET_SIZE       1024
#define DEVMM_DMA_QUERY_WAIT_PACKET_SIZE       (1024 * 256)

#define DEVMM_MAX_ORDER 24UL  /* 32G */

#define DEVMM_ISOLATION_PAGE_NUM 1
#define DEVMM_ALLOC_ISOLATION_PAGES_NUM (DEVMM_ISOLATION_PAGE_NUM * 2)

#define DEVMM_INVALID_ADDR 0UL
#define DEVMM_INVALID_ADDR2 1UL /* for ioctl err */

#define DEVMM_TRUE 1UL
#define DEVMM_FALSE 0UL

#define DEVMM_CONVERT_MAX_SIZE (64 * (1ul << 20))

/*
 * device:
 * setup device:
 * UNINITED->h2d set POLLING->polled wateup thread set POLLED->forked thread set dev pid & PRE_INITING
 * ->mmap set INITING->ioctl set INITED
 * user drvMemInitSvmDevice:
 * UNINITED->ioctl set pid & PRE_INITING->mmap set INITING->ioctl set INITED
 *
 * host:
 * UNINITED->mmap set pid& INITING->ioctl set INITED
 */
#define DEVMM_SVM_UNINITED_FLAG 0
#define DEVMM_SVM_POLLING_FLAG 1
#define DEVMM_SVM_POLLED_FLAG 2
#define DEVMM_SVM_PRE_INITING_FLAG 3
#define DEVMM_SVM_INITING_FLAG 4
#define DEVMM_SVM_INITED_FLAG 0xFFAACCEE
#define DEVMM_SVM_UNMAP_FLAG 0xAADDCC

#define DEVMM_SVM_INVALID_PID (-1)
#define DEVMM_SVM_INVALID_INDEX (-1)
#define DEVMM_SVM_DOC_VALID_FLAG 1

enum devmm_endpoint_type {
    DEVMM_END_HOST = 0x0,
    DEVMM_END_DEVICE,
    DEVMM_END_NUM,
};

enum devmm_mem_type {
    DEVMM_HBM_MEM = 0x0,
    DEVMM_DDR_MEM,
    DEVMM_P2P_HBM_MEM,
    DEVMM_P2P_DDR_MEM,
    DEVMM_MEM_TYPE_MAX,
};

enum devmm_copy_direction {
    DEVMM_COPY_HOST_TO_HOST,
    DEVMM_COPY_HOST_TO_DEVICE,
    DEVMM_COPY_DEVICE_TO_HOST,
    DEVMM_COPY_DEVICE_TO_DEVICE,
    DEVMM_COPY_INVILED_DIRECTION
};

#define DEVMM_SYNC_COPY_NUM (1024ul)

#define DEVMM_HEAP_ENABLE (0ul)
#define DEVMM_HEAP_DISABLE (1ul)

#define DEVMM_HEAP_PINNED_HOST (0xEFEF0001UL)
#define DEVMM_HEAP_HUGE_PAGE (0xEFEF0002UL)
#define DEVMM_HEAP_CHUNK_PAGE (0xEFEF0003UL)   // page_size is exchanged svm page_size
#define DEVMM_HEAP_CHUNK_PAGE4 (0xEFEF0004UL)  // page_size is exchanged svm page_size
#define DEVMM_HEAP_CHUNK_PAGE16 (0xEFEF0005UL)
#define DEVMM_HEAP_CHUNK_PAGE32 (0xEFEF0006UL)
#define DEVMM_HEAP_CHUNK_PAGE64 (0xEFEF0007UL)
#define DEVMM_HEAP_CHUNK_PAGE128 (0xEFEF0008UL)
#define DEVMM_HEAP_CHUNK_PAGE256 (0xEFEF0009UL)
#define DEVMM_HEAP_IDLE (0xEFEFABCDUL)

#define DEVMM_COPY_LEN_CNT0 (0)
#define DEVMM_COPY_LEN_CNT1 (1)
#define DEVMM_COPY_LEN_CNT2 (2)
#define DEVMM_COPY_LEN_CNT3 (3)
#define DEVMM_COPY_LEN_CNT4 (4)
#define DEVMM_COPY_LEN_CNT32 (32)

/* process normal exit and annormal exit flags */
#define DEVMM_SVM_NORMAL_EXITED_FLAG (0xEEEEEEEE)
#define DEVMM_SVM_ABNORMAL_EXITED_FLAG (0)

#define DEVMM_SVM_MAGIC 'M'

struct devmm_mem_alloc_host_para {
    DVdeviceptr p;
    size_t size;
};

struct devmm_mem_free_host_para {
    DVdeviceptr p;
};

struct devmm_mem_alloc_para {
    DVdeviceptr p;
    size_t size;
};

struct devmm_mem_copy_convrt_para {
    DVdeviceptr dst;
    DVdeviceptr src;
    size_t count;
};

struct devmm_mem_copy_para {
    DVdeviceptr dst;
    DVdeviceptr src;
    size_t ByteCount;
    enum devmm_copy_direction direction;
};

struct devmm_mem_convrt_addr_para {
    DVdeviceptr pSrc;
    DVdeviceptr pDst;
    UINT32 len;
    enum devmm_copy_direction direction;
    struct DMA_ADDR dmaAddr;
    UINT32 virt_id;
};

struct devmm_mem_desty_addr_para {
    struct DMA_ADDR dmaAddr;
};

struct devmm_mem_advise_para {
    DVdeviceptr devPtr;
    size_t count;
    DVmem_advise advise;
};

struct devmm_mem_prefetch_para {
    DVdeviceptr devPtr;
    size_t count;
};

struct devmm_mem_memset_para {
    DVdeviceptr dst;
    UINT64 value;
    size_t N;
    UINT32 hostmapped;
};

struct devmm_mem_translate_para {
    DVdeviceptr vptr;
    UINT64 pptr;
    UINT32 addr_in_device;
};

struct devmm_mem_smmu_query_para {
    UINT64 ttbr;
    UINT32 ssid;
    UINT64 tcr;
};

#define DEVMM_L2BUFF_PTE_NUM 16

struct devmm_mem_l2buff_para {
    DVdeviceptr p;
    UINT32 l2buffer_size;
    UINT64 pte[DEVMM_L2BUFF_PTE_NUM];
};

struct devmm_mem_setup_device_para {
    UINT32 aicpu_flag;
};

struct devmm_mem_l2buff_destroy_para {
    DVdeviceptr p;
};

#define DEVMM_MAX_NAME_SIZE 65

struct devmm_mem_ipc_create_para {
    char name[DEVMM_MAX_NAME_SIZE];
    UINT32 name_len;
    DVdeviceptr vptr;
    size_t len;
};

struct devmm_mem_ipc_set_pid_para {
    char name[DEVMM_MAX_NAME_SIZE];
    int set_pid[DEVMM_MAX_DOC_PROCESS_NUM];
    UINT32 num;
};

struct devmm_mem_ipc_destroy_para {
    char name[DEVMM_MAX_NAME_SIZE];
};

struct devmm_mem_query_size_para {
    char name[DEVMM_MAX_NAME_SIZE];
    int is_huge;
    size_t len;
};

struct devmm_mem_ipc_open_para {
    char name[DEVMM_MAX_NAME_SIZE];
    DVdeviceptr vptr;
};
struct devmm_mem_ipc_close_para {
    DVdeviceptr vptr;
};

struct devmm_init_process_para {
    UINT32 svm_page_size;
    UINT32 local_page_size;
    UINT32 huge_page_size;
    UINT32 l2buffsize;
};

struct devmm_update_heap_para {
    UINT32 op;
    UINT32 heap_type;
    UINT32 page_size;
    UINT32 heap_idx;
};

#define DEVMM_DEFAULT_SECTION_ORDER (7)

struct devmm_free_pages_para {
    unsigned long va;
};

struct devmm_dev_init_process_para {
    int hostpid;
    UINT64 ttbr;
    UINT32 ssid;
    UINT64 tcr;
    int devpid;
    int devnum;
    int dev_setup_map;
};

struct devmm_dev_set_process_para {
    int hostpid;
};

#define DEVMM_STATUS_VA_LIMIT 64

struct devmm_status_va_info_para {
    UINT64 va;
    UINT32 page_status;
};

struct devmm_status_va_check_para {
    UINT64 va;
    UINT64 count;
    int msg_id;
    UINT32 devid;
    UINT32 heap_idx;
};

struct devmm_query_device_mem_usedinfo {
    unsigned long total_num;
    unsigned long free_num;
    unsigned long shared_num;
    unsigned long huge_total_num;
    unsigned long huge_free_num;
    unsigned long huge_rsvd_num;
};

struct devmm_lock_cmd {
    DVdeviceptr devPtr;
    UINT32 lockType;
};

struct devmm_private_data {
    void *process;
};

#define DEVMM_POLLING_CMD_CREATE 0xEFEF0001
#define DEVMM_POLLING_CMD_CREATE_WAIT 0xEFEF0002
#define DEVMM_POLLING_CMD_DESTROY 0xEFEF0003
#define DEVMM_POLLING_CMD_INVALID 0xEFEFEFEF

#define DEVMM_POLLING_CMD_BUFSIZE 512

struct devmm_polling_cmd {
    pid_t hostpid;
    pid_t devpid;
    DVdevice devid;
    UINT32 devnum;
    UINT32 cmd;
};
struct devmm_program_load_cmd {
    DVdevice devid;
    int is_loaded;
};

struct devmm_set_read_count {
    DVdeviceptr addr;
    unsigned long size;
    unsigned int rc;
};

struct devmm_setup_info {
    pid_t hostpid;
    UINT32 dev_setmap;
};

struct devmm_dbg_info {
    pid_t hostpid;
    pid_t user_devpid;
    UINT32 user_dbg_state;
};

struct devmm_ioctl_head {
    UINT32 devid;
};

struct devmm_ioctl_arg {
    struct devmm_ioctl_head head;
    union {
        struct devmm_mem_alloc_host_para alloc_para;
        struct devmm_mem_free_host_para free_para;
        struct devmm_mem_alloc_para alloc_svm_para;
        struct devmm_mem_copy_para copy_para;
        struct devmm_mem_convrt_addr_para convrt_para;
        struct devmm_mem_desty_addr_para desty_para;
        struct devmm_mem_advise_para advise_para;
        struct devmm_mem_advise_para prefetch_para;
        struct devmm_mem_memset_para memset_para;
        struct devmm_mem_translate_para translate_para;
        struct devmm_mem_smmu_query_para smmu_para;
        struct devmm_mem_l2buff_para l2buff_para;
        struct devmm_mem_l2buff_destroy_para l2buff_destroy_para;
        struct devmm_mem_ipc_open_para ipc_open_para;
        struct devmm_mem_ipc_close_para ipc_close_para;
        struct devmm_mem_setup_device_para setup_device_para;

        struct devmm_mem_ipc_create_para ipc_create_para;
        struct devmm_mem_ipc_set_pid_para ipc_set_pid_para;
        struct devmm_mem_ipc_destroy_para ipc_destroy_para;
        struct devmm_mem_query_size_para query_size_para;

        struct devmm_init_process_para init_process_para;
        struct devmm_update_heap_para update_heap_para;
        struct devmm_free_pages_para free_pages_para;
        struct devmm_dev_init_process_para dev_init_process_para;
        struct devmm_dev_set_process_para dev_set_process_para;

        struct devmm_status_va_info_para status_va_info_para;
        struct devmm_status_va_check_para status_va_check_para;
        struct devmm_query_device_mem_usedinfo query_device_mem_usedinfo_para;

        struct devmm_lock_cmd lock_cmd_para;

        struct devmm_program_load_cmd program_load_cmd;
        struct devmm_setup_info setup_info;
        struct devmm_set_read_count set_read_count;
        struct devmm_dbg_info dbg_info;
    } data;
};

enum devmm_mem_cmd {
    DEVMM_SVM_ALLOC_HOST_CMD = 1,
    DEVMM_SVM_FREE_HOST_CMD = 2,
    DEVMM_SVM_ALLOC_CMD = 3,
    DEVMM_SVM_MEMCPY_CMD = 5,
    DEVMM_SVM_MEMCPY_H2D_CMD = 6,
    DEVMM_SVM_MEMCPY_D2H_CMD = 7,
    DEVMM_SVM_MEMCPY_PEER_CMD = 8,
    DEVMM_SVM_CONVERT_ADDR_CMD = 9,
    DEVMM_SVM_DESTROY_ADDR_CMD = 10,
    DEVMM_SVM_ADVISE_CMD = 11,
    DEVMM_SVM_PREFETCH_CMD = 12,
    DEVMM_SVM_MEMSET8_CMD = 13,
    DEVMM_SVM_MEMSET16_CMD = 14,
    DEVMM_SVM_MEMSET32_CMD = 15,
    DEVMM_SVM_MEMSET64_CMD = 16,
    DEVMM_SVM_TRANSLATE_CMD = 17,
    DEVMM_SVM_SMMU_QUERY_CMD = 18,
    DEVMM_SVM_L2BUFF_CMD = 19,
    DEVMM_SVM_L2BUFF_DESTROY_CMD = 20,
    DEVMM_SVM_IPC_MEM_OPEN_CMD = 21,
    DEVMM_SVM_IPC_MEM_CLOSE_CMD = 22,
    DEVMM_SVM_SETUP_DEVICE_CMD = 26,
    DEVMM_SVM_IPC_MEM_CREATE_CMD = 27,
    DEVMM_SVM_IPC_MEM_DESTROY_CMD = 28,
    DEVMM_SVM_IPC_MEM_QUERY_CMD = 29,
    DEVMM_SVM_QUERY_L2BUFFSIZE_CMD = 30,
    DEVMM_SVM_DEV_INIT_PROCESS_CMD = 31,
    DEVMM_SVM_IPC_MEM_SET_PID_CMD = 32,
    DEVMM_SVM_INIT_PROCESS_CMD = 33,
    DEVMM_SVM_UPDATE_HEAP_CMD = 34,
    DEVMM_SVM_FREE_PAGES_CMD = 35,
    DEVMM_SVM_LOCK_CMD = 36,
    DEVMM_SVM_SET_READ_COUNT_CMD = 37,
    DEVMM_SVM_GET_SETUP_DEVICE_INFO_CMD = 38,
    DEVMM_SVM_SET_SETUP_DEVICE_INFO_CMD = 39,
    DEVMM_SVM_QUERY_MEM_USEDINFO_CMD = 40,
    DEVMM_SVM_DBG_VA_STATUS_CMD = 41,
    DEVMM_SVM_ALLOC_MEM_FOR_DVPP = 42,

    DEVMM_SVM_CMD_USE_PRIVATE_MAX_CMD,   /* above this svm process must inited */

    DEVMM_SVM_GET_DEVPID_BY_HOSTPID_CMD,
    DEVMM_SVM_WAIT_DEVICE_PROCESS_CMD,
    DEVMM_SVM_GET_DBG_STATE_CMD,
    DEVMM_SVM_SET_DBG_STATE_CMD,
    DEVMM_SVM_DEV_SET_PROCESSID_CMD,
    DEVMM_SVM_PROGRAM_LOAD_CMD,

    DEVMM_SVM_CMD_MAX_CMD,     /* max cmd id */
};

#define DEVMM_SVM_ALLOC_HOST _IOW(DEVMM_SVM_MAGIC, DEVMM_SVM_ALLOC_HOST_CMD, struct devmm_ioctl_arg)
#define DEVMM_SVM_FREE_HOST _IOW(DEVMM_SVM_MAGIC, DEVMM_SVM_FREE_HOST_CMD, struct devmm_ioctl_arg)
#define DEVMM_SVM_ALLOC _IOW(DEVMM_SVM_MAGIC, DEVMM_SVM_ALLOC_CMD, struct devmm_ioctl_arg)
#define DEVMM_SVM_MEMCPY _IOWR(DEVMM_SVM_MAGIC, DEVMM_SVM_MEMCPY_CMD, struct devmm_ioctl_arg)
#define DEVMM_SVM_MEMCPY_H2D _IOWR(DEVMM_SVM_MAGIC, DEVMM_SVM_MEMCPY_H2D_CMD, struct devmm_ioctl_arg)
#define DEVMM_SVM_MEMCPY_D2H _IOWR(DEVMM_SVM_MAGIC, DEVMM_SVM_MEMCPY_D2H_CMD, struct devmm_ioctl_arg)
#define DEVMM_SVM_MEMCPY_PEER _IOWR(DEVMM_SVM_MAGIC, DEVMM_SVM_MEMCPY_PEER_CMD, struct devmm_ioctl_arg)
#define DEVMM_SVM_CONVERT_ADDR _IOWR(DEVMM_SVM_MAGIC, DEVMM_SVM_CONVERT_ADDR_CMD, struct devmm_ioctl_arg)
#define DEVMM_SVM_DESTROY_ADDR _IOW(DEVMM_SVM_MAGIC, DEVMM_SVM_DESTROY_ADDR_CMD, struct devmm_ioctl_arg)
#define DEVMM_SVM_ADVISE _IOW(DEVMM_SVM_MAGIC, DEVMM_SVM_ADVISE_CMD, struct devmm_ioctl_arg)
#define DEVMM_SVM_PREFETCH _IOW(DEVMM_SVM_MAGIC, DEVMM_SVM_PREFETCH_CMD, struct devmm_ioctl_arg)
#define DEVMM_SVM_MEMSET8 _IOWR(DEVMM_SVM_MAGIC, DEVMM_SVM_MEMSET8_CMD, struct devmm_ioctl_arg)
#define DEVMM_SVM_MEMSET16 _IOWR(DEVMM_SVM_MAGIC, DEVMM_SVM_MEMSET16_CMD, struct devmm_ioctl_arg)
#define DEVMM_SVM_MEMSET32 _IOWR(DEVMM_SVM_MAGIC, DEVMM_SVM_MEMSET32_CMD, struct devmm_ioctl_arg)
#define DEVMM_SVM_MEMSET64 _IOWR(DEVMM_SVM_MAGIC, DEVMM_SVM_MEMSET64_CMD, struct devmm_ioctl_arg)
#define DEVMM_SVM_TRANSLATE _IOWR(DEVMM_SVM_MAGIC, DEVMM_SVM_TRANSLATE_CMD, struct devmm_ioctl_arg)
#define DEVMM_SVM_SMMU_QUERY _IOWR(DEVMM_SVM_MAGIC, DEVMM_SVM_SMMU_QUERY_CMD, struct devmm_ioctl_arg)
#define DEVMM_SVM_L2BUFF _IOWR(DEVMM_SVM_MAGIC, DEVMM_SVM_L2BUFF_CMD, struct devmm_ioctl_arg)
#define DEVMM_SVM_L2BUFF_DESTROY _IOW(DEVMM_SVM_MAGIC, DEVMM_SVM_L2BUFF_DESTROY_CMD, struct devmm_ioctl_arg)
#define DEVMM_SVM_IPC_MEM_OPEN _IOW(DEVMM_SVM_MAGIC, DEVMM_SVM_IPC_MEM_OPEN_CMD, struct devmm_ioctl_arg)
#define DEVMM_SVM_IPC_MEM_CLOSE _IOW(DEVMM_SVM_MAGIC, DEVMM_SVM_IPC_MEM_CLOSE_CMD, struct devmm_ioctl_arg)
#define DEVMM_SVM_SETUP_DEVICE _IOW(DEVMM_SVM_MAGIC, DEVMM_SVM_SETUP_DEVICE_CMD, struct devmm_ioctl_arg)
#define DEVMM_SVM_IPC_MEM_CREATE _IOWR(DEVMM_SVM_MAGIC, DEVMM_SVM_IPC_MEM_CREATE_CMD, struct devmm_ioctl_arg)
#define DEVMM_SVM_IPC_MEM_DESTROY _IOW(DEVMM_SVM_MAGIC, DEVMM_SVM_IPC_MEM_DESTROY_CMD, struct devmm_ioctl_arg)
#define DEVMM_SVM_IPC_MEM_QUERY _IOWR(DEVMM_SVM_MAGIC, DEVMM_SVM_IPC_MEM_QUERY_CMD, struct devmm_ioctl_arg)
#define DEVMM_SVM_QUERY_L2BUFFSIZE _IOWR(DEVMM_SVM_MAGIC, DEVMM_SVM_QUERY_L2BUFFSIZE_CMD, struct devmm_ioctl_arg)
#define DEVMM_SVM_DEV_INIT_PROCESS _IOWR(DEVMM_SVM_MAGIC, DEVMM_SVM_DEV_INIT_PROCESS_CMD, struct devmm_ioctl_arg)
#define DEVMM_SVM_IPC_MEM_SET_PID _IOW(DEVMM_SVM_MAGIC, DEVMM_SVM_IPC_MEM_SET_PID_CMD, struct devmm_ioctl_arg)
#define DEVMM_SVM_INIT_PROCESS _IOWR(DEVMM_SVM_MAGIC, DEVMM_SVM_INIT_PROCESS_CMD, struct devmm_ioctl_arg)
#define DEVMM_SVM_UPDATE_HEAP _IOW(DEVMM_SVM_MAGIC, DEVMM_SVM_UPDATE_HEAP_CMD, struct devmm_ioctl_arg)
#define DEVMM_SVM_FREE_PAGES _IOW(DEVMM_SVM_MAGIC, DEVMM_SVM_FREE_PAGES_CMD, struct devmm_ioctl_arg)
#define DEVMM_SVM_LOCK _IOW(DEVMM_SVM_MAGIC, DEVMM_SVM_LOCK_CMD, struct devmm_ioctl_arg)
#define DEVMM_SVM_SET_READ_COUNT _IOW(DEVMM_SVM_MAGIC, DEVMM_SVM_SET_READ_COUNT_CMD, struct devmm_ioctl_arg)
#define DEVMM_SVM_GET_SETUP_DEVICE_INFO _IOWR(DEVMM_SVM_MAGIC, DEVMM_SVM_GET_SETUP_DEVICE_INFO_CMD, struct devmm_ioctl_arg)
#define DEVMM_SVM_SET_SETUP_DEVICE_INFO _IOW(DEVMM_SVM_MAGIC, DEVMM_SVM_SET_SETUP_DEVICE_INFO_CMD, struct devmm_ioctl_arg)
#define DEVMM_SVM_QUERY_MEM_USEDINFO _IOWR(DEVMM_SVM_MAGIC, DEVMM_SVM_QUERY_MEM_USEDINFO_CMD, struct devmm_ioctl_arg)
#define DEVMM_SVM_DBG_VA_STATUS _IOWR(DEVMM_SVM_MAGIC, DEVMM_SVM_DBG_VA_STATUS_CMD, struct devmm_ioctl_arg)
/* above this svm process must inited */
#define DEVMM_SVM_GET_DEVPID_BY_HOSTPID _IOWR(DEVMM_SVM_MAGIC, DEVMM_SVM_GET_DEVPID_BY_HOSTPID_CMD, struct devmm_ioctl_arg)
#define DEVMM_SVM_WAIT_DEVICE_PROCESS _IOWR(DEVMM_SVM_MAGIC, DEVMM_SVM_WAIT_DEVICE_PROCESS_CMD, struct devmm_ioctl_arg)
#define DEVMM_SVM_GET_DBG_STATE _IOWR(DEVMM_SVM_MAGIC, DEVMM_SVM_GET_DBG_STATE_CMD, struct devmm_ioctl_arg)
#define DEVMM_SVM_SET_DBG_STATE _IOW(DEVMM_SVM_MAGIC, DEVMM_SVM_SET_DBG_STATE_CMD, struct devmm_ioctl_arg)
#define DEVMM_SVM_DEV_SET_PROCESSID _IOW(DEVMM_SVM_MAGIC, DEVMM_SVM_DEV_SET_PROCESSID_CMD, struct devmm_ioctl_arg)
#define DEVMM_SVM_PROGRAM_LOAD _IOWR(DEVMM_SVM_MAGIC, DEVMM_SVM_PROGRAM_LOAD_CMD, struct devmm_ioctl_arg)

typedef unsigned long long phys_addr_t;
typedef int (*devmm_report_handle_t)(pid_t pid, UINT32 did, UINT32 sts);
void devmm_set_report_handle(devmm_report_handle_t p);
devmm_report_handle_t devmm_get_report_handle(void);

/*
 * one va page has a byte to mantain va status
 *    bit26~31 dev id
 *    bit25: isolation page flag
 *    bit24: p2p ddr
 *    bit21~23: reserve 5 bits
 *    bit20:bitmap_locked_flag
 *    bit15~bit19:order
 *    bit14: p2p hbm
 *    bit13: memory shared
 *    bit12: locked device
 *    bit11: locked host
 *    bit10: alloced
 *    bit9: advise:huge page
 *    bit8: advise:executabe
 *    bit7: advise:populate (no fault memory)
 *    bit6: advise:1-ddr,0-hbm(default)
 *    bit5: is first page of a va?
 *    bit4: host mapped
 *    bit3: dev mapped
 *    bit2: nosync flag for device faulting to host
 *    bit1: sync shared flag for device p2p
 *    bit0: ipc mem
 */
#define DEVMM_PAGE_IS_IPC_MEM_BIT 0
#define DEVMM_PAGE_SYNC_SHARE_NUM_BIT 1
#define DEVMM_PAGE_NOSYNC_BIT 2

#define DEVMM_PAGE_DEV_MAPPED_BIT 3
#define DEVMM_PAGE_HOST_MAPPED_BIT 4
#define DEVMM_PAGE_IS_FIRST_PAGE_BIT 5
#define DEVMM_PAGE_ADVISE_DDR_BIT 6
#define DEVMM_PAGE_ADVISE_POPULATE_BIT 7
#define DEVMM_PAGE_ADVISE_EXCUTABE_BIT 8
#define DEVMM_PAGE_ADVISE_HUGE_PAGE_BIT 9
#define DEVMM_PAGE_ALLOCED_BIT 10
#define DEVMM_PAGE_LOCKED_HOST_BIT 11
#define DEVMM_PAGE_LOCKED_DEVICE_BIT 12
#define DEVMM_PAGE_ADVISE_MEMORY_SHARED_BIT 13
#define DEVMM_PAGE_ADVISE_P2P_HBM_BIT 14
#define DEVMM_PAGE_ORDER_SHIT 15
#define DEVMM_PAGE_ORDER_WID 5
#define DEVMM_PAGE_BITMAP_LOCKED_BIT 20
#define DEVMM_PAGE_ADVISE_4G_BIT 23
#define DEVMM_PAGE_ADVISE_P2P_DDR_BIT 24
#define DEVMM_PAGE_IS_ISOLATION_BIT 25
#define DEVMM_PAGE_DEVID_SHIT 26
#define DEVMM_PAGE_DEVID_WID 6

/* MEROS FOR BITMAP* */
#define DEVMM_PAGE_DEV_MAPPED_MASK (1UL << DEVMM_PAGE_DEV_MAPPED_BIT)
#define DEVMM_PAGE_HOST_MAPPED_MASK (1UL << DEVMM_PAGE_HOST_MAPPED_BIT)
#define DEVMM_PAGE_IS_FIRST_PAGE_MASK (1UL << DEVMM_PAGE_IS_FIRST_PAGE_BIT)
#define DEVMM_PAGE_ADVISE_DDR_MASK (1UL << DEVMM_PAGE_ADVISE_DDR_BIT)
#define DEVMM_PAGE_ADVISE_POPULATE_MASK (1UL << DEVMM_PAGE_ADVISE_POPULATE_BIT)
#define DEVMM_PAGE_ADVISE_EXCUTABE_MASK (1UL << DEVMM_PAGE_ADVISE_EXCUTABE_BIT)
#define DEVMM_PAGE_ADVISE_HUGE_PAGE_MASK (1UL << DEVMM_PAGE_ADVISE_HUGE_PAGE_BIT)
#define DEVMM_PAGE_ORDER_MASK (((1UL << DEVMM_PAGE_ORDER_WID) - 1) << DEVMM_PAGE_ORDER_SHIT)
#define DEVMM_PAGE_ALLOCED_MASK (1UL << DEVMM_PAGE_ALLOCED_BIT)
#define DEVMM_PAGE_LOCKED_HOST_MASK (1UL << DEVMM_PAGE_LOCKED_HOST_BIT)
#define DEVMM_PAGE_LOCKED_DEVICE_MASK (1UL << DEVMM_PAGE_LOCKED_DEVICE_BIT)
#define DEVMM_PAGE_ADVISE_MEMORY_SHARED_MASK (1ULL << DEVMM_PAGE_ADVISE_MEMORY_SHARED_BIT)
#define DEVMM_PAGE_ADVISE_P2P_HBM_MASK (1UL << DEVMM_PAGE_ADVISE_P2P_HBM_BIT)
#define DEVMM_PAGE_ADVISE_P2P_DDR_MASK (1UL << DEVMM_PAGE_ADVISE_P2P_DDR_BIT)
#define DEVMM_PAGE_IS_ISOLATION_MASK (1UL << DEVMM_PAGE_IS_ISOLATION_BIT)
#define DEVMM_PAGE_DEVID_MASK (((1UL << DEVMM_PAGE_DEVID_WID) - 1) << DEVMM_PAGE_DEVID_SHIT)
/* MEROS FOR SYNC PROCESS* */
#define DEVMM_PAGE_SYNC_SHARE_NUM_FLG (1ULL << DEVMM_PAGE_SYNC_SHARE_NUM_BIT)
#define DEVMM_PAGE_NOSYNC_FLG (1ULL << DEVMM_PAGE_NOSYNC_BIT)
#define DEVMM_PAGE_IS_IPC_MEM (1ULL << DEVMM_PAGE_IS_IPC_MEM_BIT)
#define DEVMM_PAGE_4G_MASK (1ULL << DEVMM_PAGE_ADVISE_4G_BIT)

static inline void devmm_clear_bit(unsigned int nr, unsigned int *p)
{
#ifdef __KERNEL__
#if LINUX_VERSION_CODE >= KERNEL_VERSION(4, 12, 0)
    atomic_andnot((1UL << nr), (atomic_t *)p);
#endif
#endif
}

static inline int devmm_test_and_set_bit(unsigned int nr, unsigned int *p)
{
#ifdef __KERNEL__
#if LINUX_VERSION_CODE >= KERNEL_VERSION(4, 12, 0)
    unsigned int old;
    unsigned int mask = (1UL << nr);

    if (READ_ONCE(*p) & mask)
        return 1;

    old = atomic_fetch_or(mask, (atomic_t *)p);
    return !!(old & mask);
#else
    return 0;
#endif
#else
    return 0;
#endif
}

/*
 * bitmap加解锁操作：
 * devmm_page_bitmap_lock()/devmm_page_bitmap_unlock()必须配套使用
 */
static inline void devmm_page_bitmap_lock(UINT32 *bitmap)
{
    while (devmm_test_and_set_bit(DEVMM_PAGE_BITMAP_LOCKED_BIT, bitmap)) {
#ifdef __KERNEL__
        cpu_relax();
#endif
    }
}

static inline void devmm_page_bitmap_unlock(UINT32 *bitmap)
{
    devmm_clear_bit(DEVMM_PAGE_BITMAP_LOCKED_BIT, bitmap);
}

static inline void devmm_page_bitmap_set_value(UINT32 *bitmap, UINT32 shift, UINT32 wide, UINT32 value)
{
    UINT32 msk = ((1UL << wide) - 1);
    UINT32 val = (msk & value);

    devmm_page_bitmap_lock(bitmap);
    (*bitmap) &= (UINT32)(~(msk << shift));
    (*bitmap) |= (UINT32)(val << shift);
    devmm_page_bitmap_unlock(bitmap);
}

static inline UINT32 devmm_page_bitmap_get_value(UINT32 *bitmap, UINT32 shift, UINT32 wide)
{
    UINT32 msk = ((1UL << wide) - 1);
    UINT32 val;

    devmm_page_bitmap_lock(bitmap);
    val = (*bitmap) >> shift;
    devmm_page_bitmap_unlock(bitmap);
    return (val & msk);
}

static inline void devmm_page_clean_bitmap(UINT32 *bitmap)
{
    (*bitmap) = 0;
}

static inline UINT32 devmm_page_read_bitmap(UINT32 *bitmap)
{
    return (*bitmap);
}

static inline void devmm_page_bitmap_set_devid(UINT32 *bitmap, UINT32 devid)
{
    devmm_page_bitmap_set_value(bitmap, DEVMM_PAGE_DEVID_SHIT, DEVMM_PAGE_DEVID_WID, devid);
}

static inline UINT32 devmm_page_bitmap_get_devid(UINT32 *bitmap)
{
    return devmm_page_bitmap_get_value(bitmap, DEVMM_PAGE_DEVID_SHIT, DEVMM_PAGE_DEVID_WID);
}

static inline void devmm_page_bitmap_set_order(UINT32 *bitmap, UINT32 order)
{
    devmm_page_bitmap_set_value(bitmap, DEVMM_PAGE_ORDER_SHIT, DEVMM_PAGE_ORDER_WID, order);
}

static inline UINT32 devmm_page_bitmap_get_order(UINT32 *bitmap)
{
    return devmm_page_bitmap_get_value(bitmap, DEVMM_PAGE_ORDER_SHIT, DEVMM_PAGE_ORDER_WID);
}

static inline void devmm_page_bitmap_set_flag(UINT32 *bitmap, UINT32 flag)
{
    devmm_page_bitmap_lock(bitmap);
    (*bitmap) |= flag;
    devmm_page_bitmap_unlock(bitmap);
}

static inline void devmm_page_bitmap_clear_flag(UINT32 *bitmap, UINT32 flag)
{
    devmm_page_bitmap_lock(bitmap);
    (*bitmap) &= (~flag);
    devmm_page_bitmap_unlock(bitmap);
}

static inline bool devmm_page_bitmap_is_dev_mapped(UINT32 *bitmap)
{
    return (bool)devmm_page_bitmap_get_value(bitmap, DEVMM_PAGE_DEV_MAPPED_BIT, 1);
}

static inline bool devmm_page_bitmap_is_host_mapped(UINT32 *bitmap)
{
    return (bool)devmm_page_bitmap_get_value(bitmap, DEVMM_PAGE_HOST_MAPPED_BIT, 1);
}

static inline bool devmm_page_bitmap_is_first_page(UINT32 *bitmap)
{
    return (bool)devmm_page_bitmap_get_value(bitmap, DEVMM_PAGE_IS_FIRST_PAGE_BIT, 1);
}

static inline bool devmm_page_bitmap_is_advise_ddr(UINT32 *bitmap)
{
    return (bool)devmm_page_bitmap_get_value(bitmap, DEVMM_PAGE_ADVISE_DDR_BIT, 1);
}

static inline bool devmm_page_bitmap_is_advise_populate(UINT32 *bitmap)
{
    return (bool)devmm_page_bitmap_get_value(bitmap, DEVMM_PAGE_ADVISE_POPULATE_BIT, 1);
}

static inline bool devmm_page_bitmap_is_advise_huge_page(UINT32 *bitmap)
{
    return (bool)devmm_page_bitmap_get_value(bitmap, DEVMM_PAGE_ADVISE_HUGE_PAGE_BIT, 1);
}

static inline bool devmm_page_bitmap_is_page_alloced(UINT32 *bitmap)
{
    return (bool)devmm_page_bitmap_get_value(bitmap, DEVMM_PAGE_ALLOCED_BIT, 1);
}

static inline bool devmm_page_bitmap_is_page_available(UINT32 *bitmap)
{
    bool ret = 0;

    devmm_page_bitmap_lock(bitmap);
    ret = (bool)(((UINT32)DEVMM_PAGE_ALLOCED_MASK & (*bitmap)) &&
                  (!((UINT32)DEVMM_PAGE_IS_ISOLATION_MASK & (*bitmap))));
    devmm_page_bitmap_unlock(bitmap);
    return ret;
}

static inline bool devmm_page_bitmap_is_locked_host(UINT32 *bitmap)
{
    return (bool)devmm_page_bitmap_get_value(bitmap, DEVMM_PAGE_LOCKED_HOST_BIT, 1);
}

static inline bool devmm_page_bitmap_is_locked_device(UINT32 *bitmap)
{
    return (bool)devmm_page_bitmap_get_value(bitmap, DEVMM_PAGE_LOCKED_DEVICE_BIT, 1);
}

static inline bool devmm_page_bitmap_advise_memory_shared(UINT32 *bitmap)
{
    return (bool)devmm_page_bitmap_get_value(bitmap, DEVMM_PAGE_ADVISE_MEMORY_SHARED_BIT, 1);
}

static inline bool devmm_page_bitmap_is_advise_p2p_hbm(UINT32 *bitmap)
{
    return (bool)devmm_page_bitmap_get_value(bitmap, DEVMM_PAGE_ADVISE_P2P_HBM_BIT, 1);
}

static inline bool devmm_page_bitmap_is_advise_p2p_ddr(UINT32 *bitmap)
{
    return (bool)devmm_page_bitmap_get_value(bitmap, DEVMM_PAGE_ADVISE_P2P_DDR_BIT, 1);
}

static inline bool devmm_page_bitmap_is_isolation_page(UINT32 *bitmap)
{
    return (bool)devmm_page_bitmap_get_value(bitmap, DEVMM_PAGE_IS_ISOLATION_BIT, 1);
}

static inline bool devmm_page_bitmap_is_share_num_flg(UINT32 *bitmap)
{
    return (bool)devmm_page_bitmap_get_value(bitmap, DEVMM_PAGE_SYNC_SHARE_NUM_BIT, 1);
}

static inline bool devmm_page_bitmap_is_nosync_flg(UINT32 *bitmap)
{
    return (bool)devmm_page_bitmap_get_value(bitmap, DEVMM_PAGE_NOSYNC_BIT, 1);
}

static inline bool devmm_page_bitmap_is_ipc_mem(UINT32 *bitmap)
{
    return (bool)devmm_page_bitmap_get_value(bitmap, DEVMM_PAGE_IS_IPC_MEM_BIT, 1);
}

static inline bool devmm_page_bitmap_is_advise_4g_ddr(UINT32 *bitmap)
{
    return (bool)devmm_page_bitmap_get_value(bitmap, DEVMM_PAGE_ADVISE_4G_BIT, 1);
}

#endif
