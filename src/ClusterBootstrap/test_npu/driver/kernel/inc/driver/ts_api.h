/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2017-2020. All rights reserved.
 * Description:
 * Author: huawei
 * Create: 2020-01-02
 */

#ifndef TS_API_H
#define TS_API_H

#ifdef __cplusplus
extern "C" {
#endif

#define TS_SUCCESS 0
#define TS_INNER_ERR -1
#define TS_PARA_ERR -2
#define TS_COPY_USER_ERR -3

int tsDevSendMsgAsync (unsigned int devId, unsigned int tsId, char *msg, unsigned int msgLen, unsigned int handleId);

#ifdef __cplusplus
}
#endif
#endif

