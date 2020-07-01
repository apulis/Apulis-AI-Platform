/*********************************************************************************
 * Copyright (c) Huawei Technologies Co., Ltd. 2017-2020. All rights reserved.
 * Description: Api interface of queue
 * Author: huawei
 * Create: 2020-01-16
 *********************************************************************************/

#ifndef QUEUE_API_H
#define QUEUE_API_H

#ifdef __cplusplus
extern "C" {
#endif

#define DRV_ERROR_QUEUE_NONE                  0
#define DRV_ERROR_QUEUE_INNER_ERROR          -1
#define DRV_ERROR_QUEUE_PARA_ERROR           -2
#define DRV_ERROR_QUEUE_OUT_OF_MEM           -3
#define DRV_ERROR_QUEUE_EMPTY                -4
#define DRV_ERROR_QUEUE_NOT_EMPTY            -5
#define DRV_ERROR_QUEUE_NOT_INIT             -6
#define DRV_ERROR_QUEUE_FULL                 -7
#define DRV_ERROR_QUEUE_OUT_OF_SIZE          -8
#define DRV_ERROR_QUEUE_REPEEATED_INIT       -9
#define DRV_ERROR_QUEUE_IOCTL_FAIL           -10
#define DRV_ERROR_QUEUE_NOT_CREATED          -11
#define DRV_ERROR_QUEUE_RE_SUBSCRIBED        -12
#define DRV_ERROR_QUEUE_NO_SUBSCIBED         -13

#define MAX_STR_LEN        128   /* 队列名称支持的最大长度 */
#define RESERVE_LEN 8

typedef enum queue_status {
    QUEUE_NORMAL = 0,      /* 队列正常 */
    QUEUE_EMPTY,           /* 队列空 */
    QUEUE_FULL,            /* 队列满 */
}QUEUE_STATUS;

typedef enum queue_work_mode {
    QUEUE_MODE_PUSH = 1, /* 按group聚合发送事件 */
    QUEUE_MODE_PULL,       /* 按单个queue聚合发送事件 */
}QUEUE_WORK_MODE;

typedef enum queue_type {
    QUEUE_TYPE_GROUP = 1, /* 按group聚合发送事件 */
    QUEUE_TYPE_SINGLE,       /* 按单个queue聚合发送事件 */
}QUEUE_TYPE;

typedef struct {
    unsigned long long enqueCnt; /* 入队统计 */
    unsigned long long dequeCnt; /* 出队统计 */
    unsigned long long enqueFailCnt; /* 入队失败统计 */
    unsigned long long dequeFailCnt; /* 出队失败统计 */
    unsigned long long enqueEventOk; /* 入队事件发布成功统计 */
    unsigned long long enqueEventFail; /* 入队事件发布失败统计 */
    unsigned long long f2nfEventOk; /* 满到非满事件发布成功统计 */
    unsigned long long f2nfEventFail; /* 满到非满事件发布失败统计 */
    struct timeval lastEnqueTime; /* 最近一次入队时间 */
    struct timeval lastDequeTime; /* 最近一次出队时间 */
    int reserve[RESERVE_LEN]; /* 预留字段 */
}QueueStat;

typedef struct {
    int id; /* 查询的队列id */
    char name[MAX_STR_LEN]; /* 队列名称 */
    int size; /* 队列实际大小 */
    int depth; /* 队列深度 */
    int status; /* 队列状态：满、正常、空 */
    int workMode; /* 队列工作模式，参见QUEUE_WORK_MODE 定义 */
    int type; /* 队列类型，参见QUEUE_TYPE定义 */
    int subGroupId; /* 订阅入队事件group id */
    int subPid; /* 订阅入队事件pid */
    int subF2NFGroupId; /* 订阅满到非满事件group id */
    int subF2NFPid; /* 订阅满到非满事件pid */
    void* headDataPtr; /* 数据头指针，如果队列空，该指针为空 */
    int reserve[RESERVE_LEN]; /* 预留字段 */
    QueueStat stat; /* 队列统计 */
}QueueInfo;


/***********************************************************************************************************
函数功能：初始化创建队列所需的共享内存，业务进程初始化时调用，每个进程支持初始化一次。共享内存申请在内核完成
入参说明：
devid: 设备id
zone：设备内分区信息
maxSize 最大支持的队列个数，根据用户配置文件获取，默认8k
输出参数：
queueManage：队列管理结构地址
************************************************************************************************************/
int BuffQueueInit(unsigned int devid, unsigned int zone, unsigned int maxSize);

/************************************************************************************
函数功能：创建队列
入参说明：
devid: 设备id，dc场景预留参数，mdc场景固定填0
zone: 分区id
name: 创建队列名称,不作为key，允许为空，允许不同队列名称相同；最大长度支持128字节；
depth: 待创建的队列深度
输出参数：
qid：队列创建成功返回的队列id
************************************************************************************/
int CreateBuffQueue(unsigned int devid, unsigned int zone, const char *name, int depth, unsigned int *qid);

/************************************************************************************
函数功能：销毁队列
入参说明：
devid：设备id，dc场景预留参数，mdc场景固定填0
qid: 待销毁的队列id
************************************************************************************/
int DestroyBuffQueue(unsigned int devId, unsigned int qid);

/***********************************************************************************
函数功能：buff队列入队
入参说明；
devid：设备id，dc场景预留参数，mdc场景固定填0
qid: 队列id
mbuf：待入队的mbuff指针，由上层调用者申请
************************************************************************************/
int EnBuffQueue(unsigned int devId, unsigned int qid, void *mbuf);

/***********************************************************************************
函数功能：buff队列入队
入参说明；
devid：设备id，dc场景预留参数，mdc场景固定填0
qid: 队列id
出参说明：
buff：出队返回的mbuff指针，由调用者释放
***********************************************************************************/
int DeBuffQueue(unsigned int devId, unsigned int qid, void **mbuf);

/**********************************************************************************
函数功能：队列消费者调用，订阅buff队列，有消息入队时，通知队列消费者出队
devid：设备id，dc场景预留参数，mdc场景固定填0
qid: 被订阅的队列id
groupid：进程的分组id
type:表示队列的绑定方式，支持group和single两种方式，参见QUEUE_TYPE定义
***********************************************************************************/
int SubscribeBuffQueue(unsigned int devid, unsigned int qid, unsigned int groupId, int type);

/***********************************************************************************
函数功能：取消订阅buff队列
devid：设备id，dc场景预留参数，mdc场景固定填0
qid: 被订阅的队列id
***********************************************************************************/
int UnsubscribeBuffQueue(unsigned int devid, unsigned int qid);

/***********************************************************************************************************
函数功能：队列生产者调用，订阅队列满到非满事件，用于队列满场景，通知队列生成者继续工作
devid：设备id，dc场景预留参数，mdc场景固定填0
qid: 被订阅的队列id
groupid：进程的分组id
***********************************************************************************************************/
int SubQueueF2NFEvent(unsigned int devid, unsigned int qid, unsigned int groupid);

/***********************************************************************************************************

函数功能：队列生产者调用，订阅队列满到非满事件，用于队列满场景，通知队列生成者继续工作
devid：设备id，dc场景预留参数，mdc场景固定填0
qid: 被订阅的队列id
groupid：进程的分组id
***********************************************************************************************************/
int UnsubQueueF2NFEvent(unsigned int devid, unsigned int qid);

/***********************************************************************************************************
函数功能：设置队列工作模式，
devid: 设备id
qid: 队列id
groupid：进程的分组id
mode：工作模式，支持设置pull、push 2种方式，不设置时，默认为push；设置push模式时，订阅者要订阅入队事件；
      设置为pull方式时，订阅者要订阅队列空到非空事件
************************************************************************************************************/
int SetQueueWorkMode(unsigned int devid, unsigned int qid, int mode);

/***********************************************************************************************************
函数功能：查询队列状态
devid: 设备id
qid: 队列id
status：返回值，队列的状态信息，存在三种状态：normal、full、empty，参见QUEUE_STATUS定义
************************************************************************************************************/
int GetQueueStatus(unsigned int devid, unsigned int qid, int *status);

/***********************************************************************************
函数功能：获取队列状态信息
入参说明：
devid：设备id，dc场景预留参数，mdc场景固定填0
qid: 队列id
出参说明：
queInfo: 队列信息
***********************************************************************************/
int QueryBuffQueueInfo(unsigned int devid, unsigned int qid, QueueInfo *queInfo);

/***********************************************************************************************************
函数功能：根据队列名称获取队列id，用于进程重启等场景，新拉起的进程根据名称，查找已经创建的队列
devid: 设备id
qid: 队列id
status：返回值，队列的状态信息，存在三种状态：normal、full、empty，参见QUEUE_STATUS定义
使用说明：如果有多个name相同的队列，返回找到的第一个name相同的qid
************************************************************************************************************/
int GetQidbyName(unsigned int devid, unsigned int zone, const char *name, unsigned int *qid);

/***********************************************************************************************************
函数功能：获取队列深度
devid: 设备id
qid: 队列id
depth：返回值，返回队列的深度信息
************************************************************************************************************/
int halGetQueueDepth(unsigned int devid, unsigned int qid, unsigned int *depth);

#ifdef __cplusplus
}
#endif

#endif
