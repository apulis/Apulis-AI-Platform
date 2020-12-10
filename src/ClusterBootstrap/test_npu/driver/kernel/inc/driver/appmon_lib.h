/*******************************************************************************
 * Copyright (c) Huawei Technologies Co., Ltd. 2012-2019. All rights reserved.
 * Description:
 * Author: huawei 
 * Create: 2019-10-15
 *                     ALL RIGHTS RESERVED
 *******************************************************************************/

/* * @defgroup 应用程序心跳监控接口模块
 *  @brief   提供应用心跳监控接口
 *  @author
 *  @version 1.0
 *  @date    2019/06/14
 *
 *
 *  模块说明:
 *
 *  此模块描述了应用程序心跳监控接口.
 *
 *
 */

#ifndef _APPMON_LIB_H_
#define _APPMON_LIB_H_

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */
/**
 *  公共的监控进程与被监控进程通信使用的文件，如果创建私有监控进程，不能使用下面的宏定义
 *  APPMON_SERVER_PATH 用于进程间通信
 */
#define APPMON_SERVER_PATH "/var/driver/appmon.socket"

/**
 *  服务端信息结构体
 */
typedef struct server_info_s {
    /**
     *  内部维护信息。主要包括注册信息、进程间通信配置等。不对用户开放。
     */
    void *info;
} server_info_t;

/**
 *  客户端信息结构体
 */
typedef struct client_info_s {
    /**
     *  内部维护信息。主要包括注册信息、进程间通信配置等。不对用户开放。
     */
    void *info;
} client_info_t;

/* * 监控进程启动接口
 *
 *  启动监控进程，创建server，开始监听被监控进程连接并处理被监控进程发送的心跳请求
 *
 *  @param[in] serv_addr 通信句柄
 *  @param[out] srvr 初始化后的服务端
 *
 *  @return ::0  执行成功
 *  @return ::errno 执行失败,可用perror解析
 *
 *  @attention  无
 */
int appmon_start(server_info_t *srvr, const char *pid_file, const char *serv_addr);

/* * 监控进程退出接口
 *
 *  退出监控进程，关闭server，停止对各进程进行监控
 *
 *  @param[in] srvr 服务端地址
 *
 *  @return ::0  执行成功
 *  @return ::errno 执行失败,可用perror解析
 *
 *  @attention  无
 */
int appmon_stop(server_info_t *srvr);

/* * 应用程序客户端初始化
 *
 *  应用程序初始化，完成资源申请及进程间通信接口初始化
 *
 *  @param[in] serv_addr 服务端地址
 *  @param[out] clnt 初始化后的客户端
 *
 *  @return ::0  执行成功  < 0 启动失败  >  server已经存在
 *  @return ::errno 执行失败,可用perror解析
 *
 *  @attention  无
 */
int appmon_client_init(client_info_t *clnt, const char *serv_addr);

/* * 应用程序客户端退出
 *
 *  应用程序客户端退出，同时初始化时申请的资源被释放，app monitor不再对该app进行监控
 *
 *  @param[in] clnt 初始化过的客户端信息结构体指针
 *  @param[out] 无
 *
 *  @return :: 无
 *
 *  @attention  无
 */
void appmon_client_exit(client_info_t *clnt);

/* * 应用程序注册
 *
 *  应用程序注册，app monitor开始对该app进行监控
 *
 *  @param[in] clnt                 初始化过的客户端信息结构体指针
 *  @param[in] timeout              心跳超时时间，单位: 毫秒
 *  @param[in] timeout_action       心跳超时后，执行的脚本的绝对路径字符串\n
                                    脚本调用时会加上pid作为参数，如: timeout_action pid
 *  @param[out] 无
 *
 *  @return ::0  执行成功
 *  @return ::errno 执行失败,可用perror解析
 *
 *  @attention  脚本路径字符串长度不能超过255个字符
 */
int appmon_client_register(client_info_t *clnt, unsigned long timeout, const char *timeout_action);

/* * 应用程序注销
 *
 *  应用程序注销，app monitor不再对该app进行监控
 *
 *  @param[in] clnt     初始化过的客户端信息结构体指针
 *  @param[in] reason   注销原因字符串，用于日志记录，最多记录127个字符，可以为空
 *  @param[out] 无
 *
 *  @return ::0  执行成功
 *  @return ::errno 执行失败,可用perror解析
 *
 *  @attention  无
 */
int appmon_client_deregister(client_info_t *clnt, const char *reason);

/* * 应用程序客户端心跳发送函数
 *
 *  应用程序客户端心跳发送函数，用于维持与app monitor间的心跳
 *
 *  @param[in] clnt 初始化过的客户端信息结构体指针
 *  @param[out] 无
 *
 *  @return ::0  执行成功
 *  @return ::errno 执行失败,可用perror解析
 *
 *  @attention  无
 */
int appmon_client_heartbeat(client_info_t *clnt);

/* * 应用程序主动宣称死亡
 *
 *  应用程序主动宣称死亡，触发app monitor立即执行timeout_action脚本
 *
 *  @param[in] clnt     	初始化过的客户端信息结构体指针
 *  @param[in] last_words	临终遗言，用于日志记录，最多记录127个字符，可以为空
 *  @param[out] 无
 *
 *  @return ::0  执行成功
 *  @return ::errno 执行失败,可用perror解析
 *
 *  @attention  无
 */
int appmon_client_declare_death(client_info_t *clnt, const char *last_words);

#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif
