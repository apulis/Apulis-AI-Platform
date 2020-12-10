/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2012-2019. All rights reserved.
 * Description:
 * Author: huawei
 * Create: 2020-01-15
 */
#ifndef _DRV_BUFF_API_H_
#define _DRV_BUFF_API_H_

#ifdef __cplusplus
extern "C" {
#endif

#define ZONE_ANY   (-1)
#define DEV_ANY    (-1)

#define UNI_ALIGN_MAX       4096 /* must be less zone align */
#define UNI_ALIGN_MIN       32

typedef struct mempool_t* poolHandle;
typedef struct Mbuf Mbuf;

typedef struct mp_attr {
    int devid;
    int zoneid;
    unsigned int blkSize;
    unsigned int blkNum;
    unsigned int align;          /* must be power of 2 */
    unsigned int phys_cont_flag; /* contiguous physical addr flag. 0--false, 1--true */
    unsigned int pool_id;
}mpAttr;

struct memzone_info {
    int devid;
    int zone;
    unsigned long long va;
    unsigned long long pa;
    unsigned long long size;
};

/* 接口支持内核态、用户态调用，内核态传入内核态虚拟地址，用户态传入用户态虚拟地址 */
int buff_get_phy_addr (void *buf, unsigned long long *phyAddr);
int buff_alloc(unsigned int size, void **buff, const char *file, unsigned int line);
int buff_alloc_by_pool(poolHandle pHandle, void **buff, const char *file, unsigned int line);
int buff_free(void *buff);
int mp_create(struct mp_attr *attr, struct mempool_t **mp, unsigned int private_id, const char *file,
              unsigned int line);
int mp_delete(struct mempool_t *mp);

#define BuffAlloc(size, buf)  buff_alloc(size, buf, __FILE__, __LINE__)

#define BuffAllocByPool(pHandle, buf)  buff_alloc_by_pool(pHandle, buf, __FILE__, __LINE__)

#define BuffCreatePool(attr, pHandle) mp_create(attr, pHandle, 1, __FILE__, __LINE__)


static inline int BuffFree(void *buf)
{
    return buff_free(buf);
}

static inline int BuffGetPhyAddr (void *buf, unsigned long long *phyAddr)
{
    return buff_get_phy_addr(buf, phyAddr);
}

static inline int BuffDeletePool(poolHandle pHandle)
{
    return mp_delete(pHandle);
}

int MbufAlloc(unsigned int size, Mbuf **mbuf);
int MbufAllocByPool(poolHandle pHandle, Mbuf **mbuf);
int MbufFree(Mbuf *mbuf);
int MbufGetDataPtr (Mbuf *mbuf, void **buf, unsigned int *size);
int MbufSetDateLen (Mbuf *mbuf, unsigned int len);
int MbufGetDataLen (Mbuf *mbuf, unsigned int *len);
int MbufCopyRef (Mbuf *mbuf, Mbuf **newMbuf);
int MbufCopy(Mbuf *mbuf, Mbuf **newMbuf);
int MbufGetPrivInfo (Mbuf *mbuf,  void **priv, unsigned int *size);
int MemzoneGet(struct memzone_info *info, unsigned int num);
int MemzoneInit(int devid, int zone, unsigned long long va, unsigned long long pa, unsigned long long size);
int BuffConfigGet(unsigned int *memzone_mx_num, unsigned int *queue_max_num);

// dfx接口
int halBuffRecycleByPid(int pid);

/* error code define here */
#define DRV_ERROR_BUFF_BASE             0x00000f00
#define DRV_ERROR_BUFF_CODE(code)       (DRV_ERROR_BUFF_BASE | (code))

#define DRV_ERROR_BUFF_NONE 0
#define DRV_ERROR_BUFF_INVALID          DRV_ERROR_BUFF_CODE(1)
#define DRV_ERROR_BUFF_EMPTY            DRV_ERROR_BUFF_CODE(2)
#define DRV_ERROR_BUFF_MP_NOT_FULL      DRV_ERROR_BUFF_CODE(3)
#define DRV_ERROR_BUFF_MP_FULL          DRV_ERROR_BUFF_CODE(4)
#define DRV_ERROR_BUFF_MP_STATUS        DRV_ERROR_BUFF_CODE(5)
#define DRV_ERROR_BUFF_FAIL             DRV_ERROR_BUFF_CODE(6)
#define DRV_ERROR_BUFF_ADDR             DRV_ERROR_BUFF_CODE(7)
#define DRV_ERROR_BUFF_INIT             DRV_ERROR_BUFF_CODE(8)
#define DRV_ERROR_BUFF_IOCTL            DRV_ERROR_BUFF_CODE(9)
#define DRV_ERROR_BUFF_CONT_ADDR        DRV_ERROR_BUFF_CODE(10)

/* error code define here */
#ifdef __cplusplus
}
#endif
#endif /* _DRV_BUFF_API_H_ */

