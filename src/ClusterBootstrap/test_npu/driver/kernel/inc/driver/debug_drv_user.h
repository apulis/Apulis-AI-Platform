/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2012-2019. All rights reserved.
 * Description:
 * Author: huawei 
 * Create: 2019-10-15
 */
#ifndef __DEBUG_DRV_USER_H
#define __DEBUG_DRV_USER_H

#include "types.h"
#include "drv_mailbox.h"

typedef s32 (*callback_handle)(u16 devid, u32 event, u16 stream_id, u16 task_id);

/* ko's file description */
#define DEBUG_DRIVER_NAME "dbg_drv"

#define DEVICE_NUM_MAX (64)

#define DEBUG_OK (0)
#define DEBUG_ERROR (-1)
#define STATIC

enum ts_cmd_type {
    DEBUG_SUSPEND_DEVICE = 0x10,
    DEBUG_RESUME_DEVICE = 0x11,
    DEBUG_RESUME_PC = 0x12,
    DEBUG_SINGLE_STEP = 0x13,
    DEBUG_SET_BP = 0x14,
    DEBUG_UNSET_BP = 0x15,
    DEBUG_READ_TASK = 0x16,
    DEBUG_READ_CORE = 0x18,
    DEBUG_EXCEPTION_AICORES = 0X1B,
    DEBUG_EXCEPTION_REASON = 0X1C,
    DEBUG_READ_REG = 0x20,
    DEBUG_READ_LOCAL = 0x22,
    DEBUG_READ_SHARED = 0x23,
    DEBUG_READ_GLOBAL = 0x21,
    DEBUG_WRITE_REG = 0x30,
    DEBUG_WRITE_LOCAL = 0x32,
    DEBUG_WRITE_SHARED = 0x33,
    DEBUG_WRITE_GLOBAL = 0x31,
    DEBUG_SET_DEV_STATUS = 0x40, /* to debug status for HDC/TS */
    DEBUG_CALLBACK = 0x50,
    DEBUG_SET_TIME = 0x09,
    DEBUG_GET_INFO = 0x52,
    DEBUG_GET_DEVNUM = 0x53
};

enum ts_event_type {
    DEBUG_NOTIFY_INITIALIZED = 0x01,       /* debug proxy initial finished */
    DEBUG_NOTIFY_BREAKPOINT = 0x02,        /* debug proxy receive breakpoint */
    DEBUG_NOTIFY_TERMINATED = 0x03,        /* debug proxy finalize finished */
    DEBUG_NOTIFY_ELFLOADED = 0x04,         /* debug proxy receive elf load */
    DEBUG_NOTIFY_ATTACH_FINISH = 0x05,     /* debug proxy attach finish */
    DEBUG_NOTIFY_NEW_THREAD_CREATE = 0x06, /* debug proxy new thread create */
};

s32 suspend_device(u16 dev_id, u16 debug_id, u64 pid);
s32 resume_device(u16 dev_id, u16 debug_id, u64 pid);
s32 resume_until_pc(u16 dev_id, u16 debug_id, u64 pid, u64 virt_pc, u32 bitmap, u8 isHWBP);
s32 single_step(u16 dev_id, u16 debug_id, u8 core_id, u16 steps);
s32 set_breakpoint(u16 dev_id, u16 debug_id, u64 pid, u64 virt_addr, u8 isHWBP, u8 hw_bp_id);
s32 unset_breakpoint(u16 dev_id, u16 debug_id, u64 pid, u64 virt_addr, u8 isHWBP, u8 hw_bp_id);
s32 read_broken_tasks(u16 dev_id, u16 debug_id, struct broken_task *broken_tasks);
s32 read_broken_cores(u16 dev_id, u16 debug_id, u16 stream_id, u16 task_id, u32 *bitMap);
s32 read_register(u16 dev_id, u16 debug_id, u8 core_id, u8 type, u16 reg_id, u64 *val);
s32 read_local_memory(u16 dev_id, u16 debug_id, u8 core_id, u8 type, u16 addr, u8 *buf);
s32 read_shared_memory(u16 dev_id, u16 debug_id, u8 core_id, u64 addr, u8 *buf);
s32 read_global_memory(u16 dev_id, u16 debug_id, u8 core_id, u64 addr, u8 *buf);
s32 write_register(u16 dev_id, u16 debug_id, u8 core_id, u8 type, u16 reg_id, const u64 *val);
s32 write_local_memory(u16 dev_id, u16 debug_id, u8 core_id, u8 type, u16 addr, const u8 *buf);
s32 write_shared_memory(u16 dev_id, u16 debug_id, u8 core_id, u64 addr, const u8 *buf);
s32 write_global_memory(u16 dev_id, u16 debug_id, u8 core_id, u64 addr, const u8 *buf);
s32 set_callback_notification(callback_handle cb);
s32 get_device_info(u16 device_id, struct device_info *info);
s32 get_device_num(u32 *device_num);
s32 host_read_global_memory(u16 dev_id, u64 pid, u64 addr, u32 len, u8 *buf);
s32 host_write_global_memory(u16 dev_id, u64 pid, u64 addr, u32 len, u8 *buf);
s32 read_exception_aicores(u16 dev_id, u16 debug_id, u16 stream_id, u16 task_id, u32 *bitMap);
s32 read_exception_aicore_reason(u16 dev_id, u16 debug_id, u8 core_id, u64 *exception);
s32 set_debug_mode(u16 dev_id, u32 flag);

#endif
