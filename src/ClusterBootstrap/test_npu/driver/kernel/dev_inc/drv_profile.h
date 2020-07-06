/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2012-2019. All rights reserved.
 * Description: This file contains drv_profile Info.
 * Author: huawei
 * Create: 2017-10-15
 */

#ifndef _DRV_PROFILE_H_
#define _DRV_PROFILE_H_

#define SAMPLE_MASK 0x01
#define SAMPLE_ONLY_DATA 0x0
#define SAMPLE_WITH_HEADER 0x1

#define SAMPLE_NAME_MAX 16

/* prof_sample_register : 采样注册函数
 * int (*sample_fun)(void *arg, void *buf, int len, int flag) : 注册的回调函数
 * 		     arg : 各模块自己的参数
 * 		     buf : 采用保存buffer
 * 		     len : 采用保存buffer长度
 *		     flag: 标识符
 *		     		flag.bit0 = 1 : 发送采样"header"+采样数据
 *		  		flag.bit0 = 0 : 只发送采样数据
 *		  return : >=0 : 写入buffer的数据长度, <0 : 异常
 * arg : 返回给回调函数的指针
 * channel : enum prof_cmd_type 类型
 * return : 0 : 注册成功, 非0 : 异常
 */
int prof_sample_register(int (*sample_fun)(void *, void *, int, int), void *arg, int channel);

/* prof_sample_unregister : 采样去注册函数
 * int (*sample_fun)(void *arg, void *buf, int len) : 注册的回调函数
 * channel : enum prof_cmd_type 类型
 * return : 0 : 去注册成功, 非0 : 异常
 */
int prof_sample_unregister(int (*sample_fun)(void *, void *, int, int), int channel);

#endif /* _DRV_PROFILE_H_ */
