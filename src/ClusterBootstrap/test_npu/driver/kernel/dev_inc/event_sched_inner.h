/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2020-2020. All rights reserved.
 * Description:
 * Author: huawei
 * Create: 2020-01-02
 */
#ifndef __EVENT_SCHED_INNNER_H
#define __EVENT_SCHED_INNNER_H

struct sched_published_event_info {
    int pid;
    unsigned int gid;
    unsigned int event_id;
    unsigned int subevent_id;
    unsigned int msg_len;
    char *msg;
    unsigned long long publish_timestamp;
};

struct sched_published_event_func {
    int (*event_ack_func)(unsigned int subevent_id, const char *msg, unsigned int msg_len);
    void (*event_finish_func)(unsigned int subevent_id, const char *msg, unsigned int msg_len);
};

struct sched_published_event {
    struct sched_published_event_info event_info;
    struct sched_published_event_func event_func;
};

struct sched_subscribed_event {
    int pid;
    int host_pid;
    unsigned int gid;
    unsigned int event_id;
    unsigned int subevent_id;
    unsigned int msg_len; /* input msg buff size */
    char *msg;
    unsigned long long publish_timestamp;
    unsigned long long subscribe_timestamp;
};

int sched_submit_event(unsigned int chip_id, struct sched_published_event *event);
int sched_query_process_by_host_pid(unsigned int host_pid, unsigned int *chip_id, int *pid);

#endif

