/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2017-2020. All rights reserved.
 * Description:
 * Author: huawei
 * Create: 2020-01-02
 */

#ifndef EVENT_SCHED_API_H
#define EVENT_SCHED_API_H

#ifdef __cplusplus
extern "C" {
#endif

#define SCHED_SUCCESS 0
#define SCHED_INNER_ERR -1
#define SCHED_PARA_ERR -2
#define SCHED_OUT_OF_MEM -3
#define SCHED_UNINIT -4
#define SCHED_NO_PROCESS -5
#define SCHED_PROCESS_EXIT -6
#define SCHED_NO_SUBSCRIBE_THREAD -7
#define SCHED_NON_SCHED_GRP_MUL_THREAD -8
#define SCHED_GRP_INVALID -9
#define SCHED_PUBLISH_QUE_FULL -10
#define SCHED_NO_GRP -11
#define SCHED_GRP_EXIT -12
#define SCHED_THREAD_EXCEEDS_SPEC -13
#define SCHED_RUN_IN_ILLEGAL_CPU -14
#define SCHED_WAIT_TIMEOUT -15
#define SCHED_WAIT_FAILED -16
#define SCHED_WAIT_INTERRUPT -17
#define SCHED_THREAD_NOT_RUNNIG -18
#define SCHED_PROCESS_NOT_MATCH -19
#define SCHED_EVENT_NOT_MATCH -20
#define SCHED_PROCESS_REPEAT_ADD -21
#define SCHED_GRP_NON_SCHED -22
#define SCHED_NO_EVENT -23
#define SCHED_COPY_USER -24


#define SCHED_MAX_GRP_NUM 32
#define SCHED_MAX_THREAD_NUM_IN_GRP 16

typedef enum group_type {
    GRP_TYPE_BIND_DP_CPU = 1, /* 绑定到数据面cpu */
    GRP_TYPE_BIND_CP_CPU      /* 绑定到控制面cpu */
} GROUP_TYPE;

typedef enum event_id {
    EVENT_RANDOM_KERNEL,      /* 随机算子事件 */
    EVENT_DVPP_MSG,           /* DVPP  提交的算子事件 */
    EVENT_FR_MSG,             /* 特征检索 提交的算子事件 */
    EVENT_TS_HWTS_KERNEL,     /* ts/hwts 提交的算子事件 */
    EVENT_AICPU_MSG,          /* aicpu 激活自己的stream事件 */
    EVENT_TS_CTRL_MSG,        /* ts 控制消息事件 */
    EVENT_QUEUE_ENQUEUE,      /* 队列入队事件 (消费者) */
    EVENT_QUEUE_FULL_TO_NOT_FULL,   /* 队列满到非满事件(生产者) */
    EVENT_QUEUE_EMPTY_TO_NOT_EMPTY,   /* 队列空到非空事件(消费者) */
    EVENT_TDT_ENQUEUE,        /* TDT的数据入队 */
    EVENT_TIMER,              /* ros timer */
    /* 在这之间添加新的event */
    EVENT_TEST,               /* 预留一个测试用 */
    EVENT_MAX_NUM             /* !!最大事件ID不超过64 */
} EVENT_ID;

typedef enum schedule_priority {
    PRIORITY_LEVEL0,        /* high */
    PRIORITY_LEVEL1,        /* mid */   /* default */
    PRIORITY_LEVEL2,        /* low */
    PRIORITY_MAX,
} SCHEDULE_PRIORITY;

struct event_info_common {
    EVENT_ID event_id;
    unsigned int subevent_id;
    int pid;
    int host_pid;
    unsigned int grp_id;
    unsigned long long submit_timestamp; /* event提交时的时间戳 */
    unsigned long long sched_timestamp; /* event调度时的时间戳 */
};

#define EVENT_MAX_MSG_LEN  128  /* 消息最大长度 */

struct event_info_priv {
    unsigned int msg_len;
    char msg[EVENT_MAX_MSG_LEN];
};

struct event_info {
    struct event_info_common comm;
    struct event_info_priv priv;
};

struct event_summary {
    int pid; /* dev侧目标pid */
    unsigned int grp_id;
    EVENT_ID event_id;
    unsigned int subevent_id;
    unsigned int msg_len;
    char *msg;
};

int eSchedAttachDevice(unsigned int devId);
int eSchedDettachDevice(unsigned int devId);
int eSchedCreateGrp(unsigned int devId, unsigned int grpId, GROUP_TYPE type);
int eSchedGrpSubscribeEvent(unsigned int devId, unsigned int grpId, unsigned long long eventBitmap);
int eSchedSubscribeEvent(unsigned int devId, unsigned int grpId, unsigned int threadId, unsigned long long eventBitmap);
int eSchedSetPidPriority(unsigned int devId, SCHEDULE_PRIORITY priority);
int eSchedSetEventPriority(unsigned int devId, EVENT_ID eventId, SCHEDULE_PRIORITY priority);
int eSchedSetWeight(unsigned int devId, unsigned int weight);

/* return value:
   not find process: SCHED_NO_PROCESS
   no thread subcribe event: SCHED_NO_SUBSCRIBE_THREAD
   too many event: SCHED_PUBLISH_QUE_FULL
*/
int eSchedSubmitEvent(unsigned int devId, struct event_summary *event);

/* return value:
   timeout : SCHED_WAIT_TIMEOUT
   dettach : SCHED_PROCESS_EXIT
*/
int eSchedWaitEvent(unsigned int devId, unsigned int grpId,
                    unsigned int threadId, int timeout, struct event_info *event);

int eSchedGetEvent(unsigned int devId, unsigned int grpId, unsigned int threadId,
                   EVENT_ID eventId, struct event_info *event);
int eSchedAckEvent(unsigned int devId, EVENT_ID eventId, unsigned int subeventId, char *msg, unsigned int msgLen);

int eSchedConfigHostPid(unsigned int devId, int hostPid);
int eSchedBindHostPid(pid_t hostPid, const char *sign, unsigned int len);

#ifdef __cplusplus
}
#endif

#endif
