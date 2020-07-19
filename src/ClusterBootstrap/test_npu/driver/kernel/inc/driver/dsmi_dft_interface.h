/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2012-2019. All rights reserved.
 * Description: This file contains dsmi_dft_interface Info.
 * Author: huawei
 * Create: 2017-10-15
 */

#ifndef __DSMI_DFT_INTERFACE_H__
#define __DSMI_DFT_INTERFACE_H__

#include "dsmi_common_interface.h"

int dsmi_dft_start_peripheral_test(int device_index, const char *test_lib, int test_item, int *estimate_time);
int dsmi_dft_get_soc_test_result(int device_index, int *schedule, int *result_list, int *count);
int dsmi_dft_start_soc_test(int device_index, int test_item, int *estimate_time);
int dsmi_dft_clear_log(int device_id);
int dsmi_dft_get_peripheral_test_result(int device_index, int *schedule, int *result_list, int *count);
int dsmi_dft_set_aging_flag(int device_index, int state);
int dsmi_dft_set_aging_time(int device_index, int time);
int dsmi_dft_get_aging_flag(int device_index, int *state);
int dsmi_dft_get_aging_time(int device_index, int *time);
int dsmi_dft_get_aging_result(int device_index, int *result);

int dsmi_dft_set_dft_flag(int device_index, int enable_flag);
int dsmi_dft_get_dft_flag(int device_index, int *enable_flag);

#define DFT_TEST_ALL_MODULE 0XFFFFFFFFFFFFFFFF
#define DFT_TEST_ITEM_MAX_NUM 64

#pragma pack(1)

struct stress_test_start_info {
    unsigned long long module_id_map;  // 每个模块对应1位

    unsigned long long test_time_second;  // 测试时间，单位为秒
};

#pragma pack()

// 每个模块测试状态 大于0失败 且是错误码
typedef enum {
    MODULE_TEST_SUCCESS = 0,  // 测试成功
    MODULE_TEST_ING = -1,     // 正在测试
} MODULE_TEST_STATUS;

// 总的测试结果
typedef enum {
    DFT_GLOBAL_TEST_SUCCESS = 0,  // 测试成功
    DFT_GLOBAL_TEST_ING = -1,     // 测试中
    DFT_GLOBAL_TEST_FAIL = -2     // 测试失败
} DFT_TEST_GLOBAL_STATUS;

#pragma pack(1)

struct stress_test_result {
    int status;                              // 总测试状态
    int result_list[DFT_TEST_ITEM_MAX_NUM];  // 每个测试项的状态
    int support_num;                         // 支持的测试模块数量
};

#pragma pack()

struct stress_test_proc_info {
    unsigned char para[0];  //   当前保留，扩展用
};

#define DFT_PMU_TYPE_MAX (2)
#define DFT_PMU_TYPE_ADD (1)
#define DFT_PMU_DIE_OFFSET (16)

// 启动测试
int dft_stress_test_start(int device_id, const struct stress_test_start_info *test_info);
// 停止测试
int dft_stress_test_stop(int device_id);
// 获取测试结果
int dft_stress_get_result(int device_id, struct stress_test_result *test_result);
// DFT firmware load interface
int dsmi_dft_fw_load(int device_index, int fw_type, const char *fw_name);
int dsmi_write_efuse(int device_id, unsigned char obj, unsigned int dest_bit, unsigned int dest_size,
                     const unsigned int *src);
int dsmi_burn_efuse(int device_id, unsigned char obj);
int dsmi_efuse_flash_power(int device_id, unsigned char onoff);
int dsmi_efuse_data_check(int device_id, unsigned char obj, unsigned int dest_bit, unsigned int dest_size,
                          const unsigned int *input, unsigned int *out_flag);
int dsmi_dft_get_pmu_die(int device_id, unsigned int pmu_type, struct dsmi_soc_die_stru *pdevice_die);
int dsmi_get_pmu_voltage(int device_id, unsigned char pmu_type, unsigned char channel, unsigned int *volt_mv);
int dsmi_dft_set_elable(int device_id, int item_type, const char *elable_data, int len);
int dsmi_dft_clear_elable(int device_id, int item_type);
int dsmi_debug_send_data(int deviceid, int trg_type, const char *inbuf, int size_in, char *outbuf, int *size_out);
int dsmi_get_cntpct(int deviceid, struct dsmi_cntpct_stru *cntpct_data);

/*
函数原型	dsmi_upgrade_start_spec(int device_id, DSMI_COMPONENT_TYPE  component_type, char *file_name)
函数功能	升级设备固件，仅支持BOOTROM
输入说明	int device_id：设备id
			DSMI_COMPONENT_TYPE  component_type：固件类型
			char *file_name：配置文件路径
输出说明	无
返回值说明	0   成功
			非0 失败
使用说明	支持升级一个设备的一个固件，或者一个设备的所有可升级的固件 (第二个参数设置为0xFFFFFFFF)
			不支持升级所有设备，由上层封装接口实现
注意事项	无
使用场景	内部接口，仅提供给工具组调用，其他领域及产品禁止使用
*/
int dsmi_upgrade_start_spec(int device_id, DSMI_COMPONENT_TYPE component_type, const char *file_name);

/*
函数原型	dsmi_upgrade_get_component_static_version_spec(int device_id, 
            DSMI_COMPONENT_TYPE component_type, unsigned char* version_str, int *len)
函数功能	获取固件版本号，仅支持BOOTROM
输入说明	int device_id：设备id
			DSMI_COMPONENT_TYPE  component_type：固件类型
			unsigned char* version_str：用户申请的空间 存放返回的固件版本号
			int *len 用户申请的空间 用于存放版本号有效字符长度
输出说明	无
返回值说明	0   成功
			非0 失败
使用说明	无
注意事项	第三个参数版本号地址 是由用户申请，本模块只对其进行非空校验，大小由用户保证
使用场景	内部接口，仅提供给工具组调用，其他领域及产品禁止使用
*/
int dsmi_upgrade_get_component_static_version_spec(int device_id, DSMI_COMPONENT_TYPE component_type,
                                                   unsigned char *version_str, int *len);

/*
函数原型	dsmi_get_component_list_spec(int device_id, unsigned int *component_num, DSMI_COMPONENT_TYPE* component_table)
函数功能	获取可以升级的固件列表，仅支持BOOTROM
输入说明	int device_id：设备id
			unsigned int *component_num 用户申请的空间，用于存放返回的固件个数
			DSMI_COMPONENT_TYPE* component_table 用户申请的空间 用于存放返回的固件列表
输出说明	无
返回值说明	0   成功
			非0 失败
使用说明	无
注意事项    无
使用场景	内部接口，仅提供给工具组调用，其他领域及产品禁止使用
*/
int dsmi_get_component_list_spec(int device_id, unsigned int *component_num, DSMI_COMPONENT_TYPE *component_table);

#endif
