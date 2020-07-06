/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2012-2019. All rights reserved.
 * Description:
 * Author: huawei
 * Create: 2019-10-15
 */
#ifndef __DSMI_COMMON_INTERFACE_H__
#define __DSMI_COMMON_INTERFACE_H__

#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

#define DMP_MSG_HEAD_LENGTH (12)
#define DMP_MAX_MSG_DATA_LEN (1024 - DMP_MSG_HEAD_LENGTH)

#define BIT_IF_ONE(number, n) ((number >> n) & (0x1))
#define MAX_FILE_LEN 256
#define MAX_LINE_LEN 255
#define MAX_LOCK_NAME 30
#define UPGRADE_DST_PATH "/mnt/"
#define DEFAULT_UPGRDE_DIR_MODE 0770
#define PCIE_EP_MODE 0X0
#define PCIE_RC_MODE 0X1

#define DAVINCHI_SYS_VERSION 0XFF
#define INVALID_DEVICE_ID 0XFF

#define UPGRADE_HOST_ERROR_CODE (-1)
#define OPEN_FILE_FAIL (UPGRADE_HOST_ERROR_CODE - 1)
#define PARAM_NULL (UPGRADE_HOST_ERROR_CODE - 2)
#define FSEEK_FAIL (UPGRADE_HOST_ERROR_CODE - 3)
#define MALLOC_FAIL (UPGRADE_HOST_ERROR_CODE - 4)
#define READ_FILE_FAIL (UPGRADE_HOST_ERROR_CODE - 5)
#define WRITE_FILE_FAIL (UPGRADE_HOST_ERROR_CODE - 6)
#define MKDIR_FAIL (UPGRADE_HOST_ERROR_CODE - 7)
#define FILE_PAHT_TOO_LONG (UPGRADE_HOST_ERROR_CODE - 8)
#define NOT_FIND_COMPONENT_FILE (UPGRADE_HOST_ERROR_CODE - 9)
#define DEVICE_NOT_SUPPORT_COMPONENT_TYPE (UPGRADE_HOST_ERROR_CODE - 10)
#define DEVICE_ID_NOT_EXIST (UPGRADE_HOST_ERROR_CODE - 11)
#define CREATE_LOCK_FAIL (UPGRADE_HOST_ERROR_CODE - 12)
#define DEVICE_NOW_NOT_SUPPROT_UPGRADE (UPGRADE_HOST_ERROR_CODE - 13)
#define PARSE_CFG_FILE_LINE_FAIL (UPGRADE_HOST_ERROR_CODE - 14)
#define GET_PCIE_MODE_FAIL (UPGRADE_HOST_ERROR_CODE - 15)
#define GET_DEVICE_COUNT_FAIL (UPGRADE_HOST_ERROR_CODE - 16)
#define DEVICE_ID_NOT_PHY_MACH (UPGRADE_HOST_ERROR_CODE - 17)
#define DEVICE_IS_UPGRADING_NOW_CAN_NOT_UPGRADE (UPGRADE_HOST_ERROR_CODE - 18)
#define DEVICE_RETURN_UPGRADE_STATUS_IS_NOT_SUPPORT (UPGRADE_HOST_ERROR_CODE - 19)

// 1980 dsmi return value
#define DM_DDMP_ERROR_CODE_EAGAIN -18     // same as EAGAIN
#define DM_DDMP_ERROR_CODE_PERM_DENIED -1 // same as EPERM
// all of follow must same as inc/base.h
#define DM_DDMP_ERROR_CODE_SUCCESS 0                      // success
#define DM_DDMP_ERROR_CODE_PARAMETER_ERROR 3              // param error
#define DM_DDMP_ERROR_CODE_INVALID_HANDLE_ERROR 4         // invalid fd handle
#define DM_DDMP_ERROR_CODE_TIME_OUT 16                    // wait time out
#define DM_DDMP_ERROR_CODE_IOCRL_ERROR 17                 // ioctl error
#define DM_DDMP_ERROR_CODE_INVALID_DEVICE_ERROR 20        // invalid device
#define DM_DDMP_ERROR_CODE_SEND_ERROR 27                  // hdc or upd send error
#define DM_DDMP_ERROR_CODE_INTERNAL_ERROR 48              // internal error
#define DM_DDMP_ERROR_CODE_NOT_SUPPORT 49                 // dsmi command not support error
#define DM_DDMP_ERROR_CODE_MEMERY_OPRATOR_ERROR 50        // system memory function error
#define DM_DDMP_ERROR_CODE_PERIPHERAL_DEVICE_NOT_EXIST 51 // peripheral device not exist, BMC used

#define MAX_PATH_LEN_SIZE (1024)

typedef struct dm_flash_info_stru {
    unsigned long flash_id;         /* combined device & manufacturer code */
    unsigned short device_id;       /* device id    */
    unsigned short vendor;          /* the primary vendor id  */
    unsigned int state;             /* flash health */
    unsigned long size;             /* total size in bytes  */
    unsigned int sector_count;      /* number of erase units  */
    unsigned short manufacturer_id; /* manufacturer id   */
} DM_FLASH_INFO_STRU, dm_flash_info_stru;

typedef struct tag_pcie_idinfo {
    unsigned int deviceid;
    unsigned int venderid;
    unsigned int subvenderid;
    unsigned int subdeviceid;
    unsigned int bdf_deviceid;
    unsigned int bdf_busid;
    unsigned int bdf_funcid;
} TAG_PCIE_IDINFO, tag_pcie_idinfo;

typedef struct tag_ecc_stat {
    unsigned int error_count;
} TAG_ECC_STAT, tag_ecc_stat;

typedef struct dsmi_upgrade_control {
    unsigned char control_cmd;
    unsigned char component_type;
    unsigned char file_name[MAX_FILE_LEN];
} DSMI_UPGRADE_CONTROL;

typedef enum dsmi_upgrade_device_state {
    UPGRADE_IDLE_STATE = 0,
    IS_UPGRADING = 1,
    UPGRADE_NOT_SUPPORT = 2,
    UPGRADE_UPGRADE_FAIL = 3,
    UPGRADE_STATE_NONE = 4,
    UPGRADE_WAITTING_RESTART = 5,
    UPGRADE_WAITTING_SYNC = 6,
    UPGRADE_SYNCHRONIZING = 7
} DSMI_UPGRADE_DEVICE_STATE;

typedef enum {
    DSMI_DEVICE_TYPE_DDR,
    DSMI_DEVICE_TYPE_SRAM,
    DSMI_DEVICE_TYPE_HBM,
    DSMI_DEVICE_TYPE_NPU,
    DSMI_DEVICE_TYPE_NONE = 0xff
} DSMI_DEVICE_TYPE;

typedef enum dsmi_boot_status {
    DSMI_BOOT_STATUS_UNINIT = 0, /* uninit status */
    DSMI_BOOT_STATUS_BIOS,       /* status of starting BIOS */
    DSMI_BOOT_STATUS_OS,         /* status of starting OS */
    DSMI_BOOT_STATUS_FINISH      /* finish boot start */
} DSMI_BOOT_STATUS;

typedef enum rdfx_detect_result {
    RDFX_DETECT_OK = 0,
    RDFX_DETECT_SOCK_FAIL = 1,
    RDFX_DETECT_RECV_TIMEOUT = 2,
    RDFX_DETECT_UNREACH = 3,
    RDFX_DETECT_TIME_EXCEEDED = 4,
    RDFX_DETECT_FAULT = 5,
    RDFX_DETECT_INIT = 6,
    RDFX_DETECT_MAX
} DSMI_NET_HEALTH_STATUS;

#define UTLRATE_TYPE_DDR (1)
#define UTLRATE_TYPE_AICORE (2)
#define UTLRATE_TYPE_AICPU (3)
#define UTLRATE_TYPE_CTRLCPU (4)
#define UTLRATE_TYPE_DDR_BANDWIDTH (5)
#define UTLRATE_TYPE_HBM (6)

#define TAISHAN_CORE_NUM 16
typedef struct dsmi_aicpu_info_stru {
    unsigned int maxFreq;
    unsigned int curFreq;
    unsigned int aicpuNum;
    unsigned int utilRate[TAISHAN_CORE_NUM];
} DSMI_AICPU_INFO;

typedef enum { 
    ECC_CONFIG_ITEM = 0X0, 
    P2P_CONFIG_ITEM = 0X1, 
    DFT_CONFIG_ITEM = 0X2 
} CONFIG_ITEM;

typedef enum dsmi_component_type {
    DSMI_COMPONENT_TYPE_NVE,
    DSMI_COMPONENT_TYPE_XLOADER,
    DSMI_COMPONENT_TYPE_M3FW,
    DSMI_COMPONENT_TYPE_UEFI,
    DSMI_COMPONENT_TYPE_TEE,
    DSMI_COMPONENT_TYPE_KERNEL,
    DSMI_COMPONENT_TYPE_DTB,
    DSMI_COMPONENT_TYPE_ROOTFS,
    DSMI_COMPONENT_TYPE_IMU,
    DSMI_COMPONENT_TYPE_IMP,
    DSMI_COMPONENT_TYPE_AICPU,
    DSMI_COMPONENT_TYPE_HBOOT1_A,
    DSMI_COMPONENT_TYPE_HBOOT1_B,
    DSMI_COMPONENT_TYPE_HBOOT2,
    DSMI_COMPONENT_TYPE_DDR,
    DSMI_COMPONENT_TYPE_LP,
    DSMI_COMPONENT_TYPE_HSM,
    DSMI_COMPONENT_TYPE_SAFETY_ISLAND,
    DSMI_COMPONENT_TYPE_RAWDATA,
    DSMI_COMPONENT_TYPE_SYSDRV,
    DSMI_COMPONENT_TYPE_ADSAPP,
    DSMI_COMPONENT_TYPE_COMISOLATOR,
    DSMI_COMPONENT_TYPE_CLUSTER,
    DSMI_COMPONENT_TYPE_CUSTOMIZED,
    DSMI_COMPONENT_TYPE_MAX,
    UPGRADE_AND_RESET_ALL_COMPONENT = 0xFFFFFFF7,
    UPGRADE_ALL_IMAGE_COMPONENT = 0xFFFFFFFD,
    UPGRADE_ALL_FIRMWARE_COMPONENT = 0xFFFFFFFE,
    UPGRADE_ALL_COMPONENT = 0xFFFFFFFF
} DSMI_COMPONENT_TYPE;

#define MAX_COMPONENT_NUM (32)

#define UPGRADE_LOCK "dsmi_upgrade_dev_lock_"

typedef struct cfg_file_des {
    unsigned char component_type;
    char src_component_path[MAX_FILE_LEN];
    char dst_compoent_path[MAX_FILE_LEN];
} CFG_FILE_DES;

typedef enum { DSMI_REVOCATION_TYPE_SOC = 0, DSMI_REVOCATION_TYPE_MAX } DSMI_REVOCATION_TYPE;

#define DSMI_SOC_DIE_LEN 5
struct dsmi_soc_die_stru {
    unsigned int soc_die[DSMI_SOC_DIE_LEN]; // 5 soc_die arrary size
};
struct dsmi_power_info_stru {
    unsigned short power;
};
struct dsmi_memory_info_stru {
    unsigned long memory_size;
    unsigned int freq;
    unsigned int utiliza;
};

struct dsmi_hbm_info_stru {
    unsigned long memory_size;  /* HBM total size, KB */
    unsigned int freq;          /* HBM freq, MHZ */
    unsigned long memory_usage; /* HBM memory_usage, KB */
    int temp;                   /* HBM temperature */
    unsigned int bandwith_util_rate;
};

typedef struct dsmi_aicore_info_stru {
    unsigned int freq;    /* normal freq */
    unsigned int curfreq; /* current freq */
} DSMI_AICORE_FRE_INFO;

struct dsmi_ecc_info_stru {
    int enable_flag;
    unsigned int single_bit_error_count;
    unsigned int double_bit_error_count;
};

#define MAX_CHIP_NAME 32
#define MAX_DEVICE_COUNT 64

struct dsmi_chip_info_stru {
    unsigned char chip_type[MAX_CHIP_NAME];
    unsigned char chip_name[MAX_CHIP_NAME];
    unsigned char chip_ver[MAX_CHIP_NAME];
};

#define DSMI_VNIC_PORT 0
#define DSMI_ROCE_PORT 1

enum ip_addr_type {
    /* * IPv4 */
    IPADDR_TYPE_V4 = 0U,
    /* * IPv6 */
    IPADDR_TYPE_V6 = 1U,
    IPADDR_TYPE_ANY = 2U
};

#define DSMI_ARRAY_IPV4_NUM 4
#define DSMI_ARRAY_IPV6_NUM 16

typedef struct ip_addr {
    union {
        unsigned char ip6[DSMI_ARRAY_IPV6_NUM];
        unsigned char ip4[DSMI_ARRAY_IPV4_NUM];
    } u_addr;
    enum ip_addr_type ip_type;
} ip_addr_t;


#define COMPUTING_POWER_PMU_NUM 4

struct dsmi_cntpct_stru {
    unsigned long long state;
    unsigned long long timestamp1;
    unsigned long long timestamp2;
    unsigned long long event_count[COMPUTING_POWER_PMU_NUM];
    unsigned int system_frequency;
};

typedef enum dsmi_channel_index { DEVICE = 0, HOST = 1, MCU = 2 } DMSI_CHANNEL_INDEX;

struct dmp_req_message_stru {
    unsigned char lun;
    unsigned char arg;
    unsigned short opcode;
    unsigned int offset;
    unsigned int length;
    unsigned char data[DMP_MAX_MSG_DATA_LEN];
};

#define DSMI_RSP_MSG_DATA_LEN 1012
struct dmp_rsp_message_stru {
    unsigned short errorcode;
    unsigned short opcode;
    unsigned int total_length;
    unsigned int length;
    unsigned char data[DSMI_RSP_MSG_DATA_LEN]; // 1012 rsp data size
};

struct dmp_message_stru {
    union {
        struct dmp_req_message_stru req;
        struct dmp_rsp_message_stru rsp;
    } data;
};

struct passthru_message_stru {
    unsigned int src_len;
    unsigned int rw_flag; /* 0 read ,1 write */
    struct dmp_message_stru src_message;
    struct dmp_message_stru dest_message;
};

struct dsmi_board_info_stru {
    unsigned int board_id;
    unsigned int pcb_id;
    unsigned int bom_id;
    unsigned int slot_id;
};

typedef struct dsmi_llc_perf_stru {
    unsigned int wr_hit_rate;
    unsigned int rd_hit_rate;
    unsigned int throughput;
} DSMI_LLC_PERF_INFO;

#define SENSOR_DATA_MAX_LEN 16

#define DSMI_TAG_SENSOR_TEMP_LEN 2
#define DSMI_TAG_SENSOR_NTC_TEMP_LEN 4
typedef union tag_sensor_info {
    unsigned char uchar;
    unsigned short ushort;
    unsigned int uint;
    signed int iint;
    signed char temp[DSMI_TAG_SENSOR_TEMP_LEN];   // 2 temp size
    signed int ntc_tmp[DSMI_TAG_SENSOR_NTC_TEMP_LEN]; // 4 ntc_tmp size
    unsigned int data[SENSOR_DATA_MAX_LEN];
} TAG_SENSOR_INFO;

#ifndef MAX_MATRIX_PROC_NUM
#define MAX_MATRIX_PROC_NUM 256
#endif

typedef struct {
    unsigned int pid;
    unsigned int mem_rate;
    unsigned int cpu_rate;
} DSMI_MATRIX_PORC_INFO_S;

struct dsmi_matrix_proc_info_get_stru {
    DSMI_MATRIX_PORC_INFO_S proc_info[MAX_MATRIX_PROC_NUM];
    int output_num;
};

int dsmi_dft_get_elable(int device_id, int item_type, char *elable_data, int *len);

/**
* @ingroup driver-stub
* @brief start upgrade 
* @attention Support to upgrade one firmware of a device, or all upgradeable firmware of a device (the second 
            parameter is set to 0xFFFFFFFF), Does not support upgrading all devices, implemented by upper 
            layer encapsulation interface
* @param [in] device_id  The device id
* @param [in] component_type firmware type
* @param [in] file_name  the path of firmware 
* @return  0 for success, others for fail
*/
int dsmi_upgrade_start(int device_id, DSMI_COMPONENT_TYPE component_type, const char *file_name);

/**
* @ingroup driver-stub
* @brief get upgrade state 
* @attention NULL
* @param [in] device_id  The device id
* @param [out] schedule  Upgrade progress
* @param [out] upgrade_status  Upgrade state
* @return  0 for success, others for fail
*/
int dsmi_upgrade_get_state(int device_id, unsigned char *schedule, unsigned char *upgrade_status);

/**
* @ingroup driver-stub
* @brief get the version of firmware
* @attention The address of the third parameter version number is applied by the user, 
             the module only performs non-null check on it, and the size is guaranteed by the user
* @param [in] device_id  The device id
* @param [out] schedule  Upgrade progress
* @param [out] version_str  The space requested by the user stores the returned firmware version number
* @param [out] version_len  the length of version_str
* @param [out] ret_len  The space requested by the user is used to store the effective character length 
               of the version number
* @return  0 for success, others for fail
*/
int dsmi_upgrade_get_component_static_version(int device_id, DSMI_COMPONENT_TYPE component_type,
    unsigned char *version_str, unsigned int version_len, unsigned int *ret_len);

/**
* @ingroup driver-stub
* @brief Get the system version number 
* @attention The address of the second parameter version number is applied by the user, 
             the module only performs non-null check on it, and the size is guaranteed by the user
* @param [in] device_id  The device id
* @param [out] version_str  User-applied space stores system version number
* @param [out] version_len  length of paramer version_str
* @param [out] ret_len  The space requested by the user is used to store the effective 
               length of the returned system version number
* @return  0 for success, others for fail
*/
int dsmi_get_version(int device_id, char *version_str, unsigned int version_len, unsigned int *ret_len);

/**
* @ingroup driver-stub
* @brief get upgrade state 
* @attention Get the number of firmware that can be support
* @param [in] device_id  The device id
* @param [out] component_count  The space requested by the user for storing the number of firmware returned
* @return  0 for success, others for fail
*/
int dsmi_get_component_count(int device_id, unsigned int *component_count);


/**
* @ingroup driver-stub
* @brief get upgrade state 
* @attention NULL
* @param [in] device_id  The device id
* @param [out] component_table  The space requested by the user is used to store the returned firmware list
* @param [out] component_count  The count of firmware
* @return  0 for success, others for fail
*/
int dsmi_get_component_list(int device_id, DSMI_COMPONENT_TYPE *component_table, unsigned int component_count);

/**
* @ingroup driver-stub
* @brief Get the number of devices
* @attention NULL
* @param [out] device_count  The space requested by the user is used to store the number of returned devices
* @return  0 for success, others for fail
*/
int dsmi_get_device_count(int *device_count);

/*
函数原型	dsmi_get_all_device_count(int *device_count)
函数功能	获取设备个数，该个数是指可以通过lspci命令获取的设备数，不仅仅是指设备
管理模块获取的设备数目。
输入说明	int *device_count 用户申请的空间 用于存放返回的设备个数
输出说明	无
返回值说明	0   成功
            非0 失败
使用说明	无
注意事项	无
使用场景	无

*/
int dsmi_get_all_device_count(int *all_device_count);

/**
* @ingroup driver-stub
* @brief Get the id of all devices
* @attention NULL
* @param [out] device_count The space requested by the user is used to store the id of all returned devices
* @param [out] count Number of equipment
* @return  0 for success, others for fail
*/
int dsmi_list_device(int device_id_list[], int count);

/**
* @ingroup driver-stub
* @brief Start the container service
* @attention Cannot be used simultaneously with the computing power distribution mode
* @return  0 for success, others for fail
*/
int dsmi_enable_container_service(void);

/**
* @ingroup driver-stub
* @brief Logical id to physical id
* @attention NULL
* @param [in] logicid logic id
* @param [out] phyid   physic id
* @return  0 for success, others for fail
*/
int dsmi_get_phyid_from_logicid(unsigned int logicid, unsigned int *phyid);

/**
* @ingroup driver-stub
* @brief physical id to Logical id
* @attention NULL
* @param [in] phyid   physical id
* @param [out] logicid logic id
* @return  0 for success, others for fail
*/
int dsmi_get_logicid_from_phyid(unsigned int phyid, unsigned int *logicid);

/**
* @ingroup driver-stub
* @brief Query the overall health status of the device, support AI Server
* @attention NULL
* @param [in] device_id  The device id
* @param [out] phealth  The pointer of the overall health status of the device only represents this component,
                        and does not include other components that have a logical relationship with this component.
* @return  0 for success, others for fail
*/
int dsmi_get_device_health(int device_id, unsigned int *phealth);

/**
* @ingroup driver-stub
* @brief Query device fault code
* @attention NULL
* @param [in] device_id  The device id
* @param [out] errorcount  Number of error codes
* @param [out] perrorcode  error codes
* @return  0 for success, others for fail
*/
int dsmi_get_device_errorcode(int device_id, int *errorcount, unsigned int *perrorcode);

/**
* @ingroup driver-stub
* @brief Query the temperature of the ICE SOC of Shengteng AI processor
* @attention NULL
* @param [in] device_id  The device id
* @param [out] ptemperature  The temperature of the HiSilicon SOC of the Shengteng AI processor: unit Celsius, 
                         the accuracy is 1 degree Celsius, and the decimal point is rounded. 16-bit signed type, 
                         little endian. The value returned by the device is the actual temperature.
* @return  0 for success, others for fail
*/
int dsmi_get_device_temperature(int device_id, int *ptemperature);

/**
* @ingroup driver-stub
* @brief Query device power consumption
* @attention NULL
* @param [in] device_id  The device id
* @param [out] schedule  Device power consumption: unit is W, accuracy is 0.1W. 16-bit unsigned short type, 
               little endian
* @return  0 for success, others for fail
*/
int dsmi_get_device_power_info(int device_id, struct dsmi_power_info_stru *pdevice_power_info);

/**
* @ingroup driver-stub
* @brief Query PCIe device information
* @attention NULL
* @param [in] device_id  The device id
* @param [out] pcie_idinfo  PCIe device information
* @return  0 for success, others for fail
*/
int dsmi_get_pcie_info(int device_id, struct tag_pcie_idinfo *pcie_idinfo);

/**
* @ingroup driver-stub
* @brief Query the voltage of Sheng AI SOC of ascend AI processor
* @attention NULL
* @param [in] device_id  The device id
* @param [out] pvoltage  The voltage of the HiSilicon SOC of the Shengteng AI processor: the unit is V, 
                         and the accuracy is 0.01V
* @return  0 for success, others for fail
*/
int dsmi_get_device_voltage(int device_id, unsigned int *pvoltage);

/**
* @ingroup driver-stub
* @brief Get the occupancy rate of the HiSilicon SOC of the Ascension AI processor
* @attention NULL
* @param [in] device_id  The device id
* @param [in] device_type  device_type
* @param [out] putilization_rate  Utilization rate of HiSilicon SOC of ascend AI processor, unit:%
* @return  0 for success, others for fail
*/
int dsmi_get_device_utilization_rate(int device_id, int device_type, unsigned int *putilization_rate);

/**
* @ingroup driver-stub
* @brief Get the frequency of the HiSilicon SOC of the Ascension AI processor
* @attention NULL
* @param [in] device_id  The device id
* @param [out] device_type  device_type
* @param [out] pfrequency  Frequency, unit MHZ
* @return  0 for success, others for fail
*/
int dsmi_get_device_frequency(int device_id, int device_type, unsigned int *pfrequency);

/**
* @ingroup driver-stub
* @brief Get the number of Flash
* @attention NULL
* @param [in] device_id  The device id
* @param [out] pflash_count Returns the number of Flash, currently fixed at 1
* @return  0 for success, others for fail
*/
int dsmi_get_device_flash_count(int device_id, unsigned int *pflash_count);

/**
* @ingroup driver-stub
* @brief Get flash device information
* @attention NULL
* @param [in] device_id  The device id
* @param [in] flash_index Flash index number. The value is fixed at 0.
* @param [out] pflash_info Returns Flash device information.
* @return  0 for success, others for fail
*/
int dsmi_get_device_flash_info(int device_id, unsigned int flash_index, dm_flash_info_stru *pflash_info);

/**
* @ingroup driver-stub
* @brief Get memory information
* @attention NULL
* @param [in] device_id  The device id
* @param [out] pdevice_memory_info  Return memory information
* @return  0 for success, others for fail
*/
int dsmi_get_memory_info(int device_id, struct dsmi_memory_info_stru *pdevice_memory_info);

/**
* @ingroup driver-stub
* @brief Get ECC information
* @attention NULL
* @param [in] device_id  The device id
* @param [in] device_type  device type
* @param [out] pdevice_ecc_info  return ECC information
* @return  0 for success, others for fail
*/
int dsmi_get_ecc_info(int device_id, int device_type, struct dsmi_ecc_info_stru *pdevice_ecc_info);
int dsmi_passthru_mcu(int device_id, struct passthru_message_stru *passthru_message);

/**
* @ingroup driver-stub
* @brief Query device fault description
* @attention NULL
* @param [in] device_id  The device id
* @param [in] schedule  Error code to query
* @param [out] perrorinfo Corresponding error character description
* @param [out] buffsize The buff size brought in is fixed at 48 bytes. If the set buff size is greater 
                        than 48 bytes, the default is 48 bytes
* @return  0 for success, others for fail
*/
int dsmi_query_errorstring(int device_id, unsigned int errorcode, unsigned char *perrorinfo, int buffsize);

/**
* @ingroup driver-stub
* @brief Get board information, including board_id, pcb_id, bom_id, slot_id version numbers of the board 
* @attention NULL
* @param [in] device_id  The device id
* @param [out] pboard_info  return board info
* @return  0 for success, others for fail
*/
int dsmi_get_board_info(int device_id, struct dsmi_board_info_stru *pboard_info);

/**
* @ingroup driver-stub
* @brief Get system time
* @attention NULL
* @param [in] device_id  The device id
* @param [out] ntime_stamp  the number of seconds from 00:00:00, January 1,1970.
* @return  0 for success, others for fail
*/
int dsmi_get_system_time(int device_id, unsigned int *ntime_stamp);
int dsmi_config_ecc_enable(int device_id, DSMI_DEVICE_TYPE device_type, int enable_flag);
int dsmi_config_p2p_enable(int device_id, int enable_flag);
int dsmi_get_ecc_enable(int device_id, DSMI_DEVICE_TYPE device_type, int *enable_flag);
int dsmi_get_p2p_enable(int device_id, int *enable_flag);

/**
* @ingroup driver-stub
* @brief Set the MAC address of the specified device
* @attention NULL
* @param [in] device_id  The device id
* @param [in] mac_id Specify MAC, value range: 0 ~ dsmi_get_mac_count interface output
* @param [in] pmac_addr Set a 6-byte MAC address.
* @param [in] mac_addr_len  MAC address length, fixed length 6, unit byte.
* @return  0 for success, others for fail
*/
int dsmi_set_mac_addr(int device_id, int mac_id, const char *pmac_addr, unsigned int mac_addr_len);

/**
* @ingroup driver-stub
* @brief Query the number of MAC addresses
* @attention NULL
* @param [in] device_id  The device id
* @param [out] count Query the MAC number, the value range: 0 ~ 4.
* @return  0 for success, others for fail
*/
int dsmi_get_mac_count(int device_id, int *count);

/**
* @ingroup driver-stub
* @brief Get the MAC address of the specified device
* @attention NULL
* @param [in] device_id  The device id
* @param [in] mac_id Specify MAC, value range: 0 ~ dsmi_get_mac_count interface output
* @param [out] pmac_addr return a 6-byte MAC address.
* @param [in] mac_addr_len  MAC address length, fixed length 6, unit byte.
* @return  0 for success, others for fail
*/
int dsmi_get_mac_addr(int device_id, int mac_id, char *pmac_addr, unsigned int mac_addr_len);

/**
* @ingroup driver-stub
* @brief Set the ip address and mask address.
* @attention NULL
* @param [in] device_id  The device id
* @param [in] port_type  Specify the network port type
* @param [in] port_id  Specify the network port number, reserved field
* @param [in] ip_address  ip address info wants to set
* @param [in] mask_address  mask address info wants to set
* @return  0 for success, others for fail
*/
int dsmi_set_device_ip_address(int device_id, int port_type, int port_id, ip_addr_t ip_address, ip_addr_t mask_address);

/**
* @ingroup driver-stub
* @brief get the ip address and mask address.
* @attention NULL
* @param [in] device_id  The device id
* @param [in] port_type  Specify the network port type
* @param [in] port_id  Specify the network port number, reserved field
* @param [out] ip_address  return ip address info
* @param [out] mask_address  return mask address info
* @return  0 for success, others for fail
*/
int dsmi_get_device_ip_address(int device_id, int port_type, int port_id, ip_addr_t *ip_address,
    ip_addr_t *mask_address);
int dsmi_get_fan_count(int device_id, int *count);
int dsmi_get_fan_speed(int device_id, int fan_id, int *speed);
int dsmi_set_fan_speed(int device_id, int speed);
int dsmi_pre_reset_soc(int device_id);
int dsmi_rescan_soc(int device_id);

/**
* @ingroup driver-stub
* @brief Reset the HiSonic SOC of the designated Ascent AI processor 
* @attention NULL
* @param [in] device_id  The device id
* @return  0 for success, others for fail
*/
int dsmi_hot_reset_soc(int device_id);

/**
* @ingroup driver-stub
* @brief Get the startup state of the HiSilicon SOC of the Ascend AI processor
* @attention NULL
* @param [in] device_id  The device id
* @param [out] boot_status The startup state of the HiSilicon SOC of the Ascend AI processor
* @return 0 for success, others for fail
*/
int dsmi_get_device_boot_status(int device_id, enum dsmi_boot_status *boot_status);

/**
* @ingroup driver-stub
* @brief Relevant information about the HiSilicon SOC of the AI ??processor, including chip_type, chip_name, 
         chip_ver version number
* @attention NULL
* @param [in] device_id  The device id
* @param [out] chip_info  Get the relevant information of ascend AI processor Hisilicon SOC
* @return  0 for success, others for fail
*/
int dsmi_get_chip_info(int device_id, struct dsmi_chip_info_stru *chip_info);

/**
* @ingroup driver-stub
* @brief Get SOC sensor information
* @attention NULL
* @param [in] device_id The device id
* @param [in] sensor_id Specify sensor index
* @param [out] Returns the value that needs to be obtained
* @return  0 for success, others for fail
*/
int dsmi_get_soc_sensor_info(int device_id, int sensor_id, TAG_SENSOR_INFO *tsensor_info);

/**
* @ingroup driver-stub
* @brief set the gateway address.
* @attention NULL
* @param [in] device_id  The device id
* @param [in] port_type  Specify the network port type
* @param [in] port_id  Specify the network port number, reserved field
* @param [out] gtw_address  the gateway address info wants to set.
* @return  0 for success, others for fail
*/
int dsmi_set_gateway_addr(int device_id, int port_type, int port_id, ip_addr_t gtw_address);

/**
* @ingroup driver-stub
* @brief Query the gateway address.
* @attention NULL
* @param [in] device_id  The device id
* @param [in] port_type  Specify the network port type
* @param [in] port_id  Specify the network port number, reserved field
* @param [out] gtw_address  return gateway address info
* @return  0 for success, others for fail
*/
int dsmi_get_gateway_addr(int device_id, int port_type, int port_id, ip_addr_t *gtw_address);
int dsmi_get_mini2mcu_heartbeat_status(int device_id, unsigned char *status, unsigned int *disconn_cnt);
int dsmi_get_matrix_proc_info(int device_id, struct dsmi_matrix_proc_info_get_stru *matrix_proc_info);
int dsmi_send_sign_to_matrix_proc(int device_id, int matrix_pid, int *result);

/**
* @ingroup driver-stub
* @brief Query the frequency, capacity and utilization information of hbm
* @attention NULL
* @param [in] device_id  The device id
* @param [out] pdevice_hbm_info return hbm infomation
* @return  0 for success, others for fail
*/
int dsmi_get_hbm_info(int device_id, struct dsmi_hbm_info_stru *pdevice_hbm_info);

/**
* @ingroup driver-stub
* @brief Query the frequency and utilization information of aicore
* @attention NULL
* @param [in] device_id  The device id
* @param [out] pdevice_aicore_info  return aicore information
* @return  0 for success, others for fail
*/
int dsmi_get_aicore_info(int device_id, struct dsmi_aicore_info_stru *pdevice_aicore_info);

/**
* @ingroup driver-stub
* @brief Query the connectivity status of the RoCE network card's IP address
* @attention NULL
* @param [in] device_id The device id
* @param [out] presult return the result wants to query
* @return  0 for success, others for fail
*/
int dsmi_get_network_health(int device_id, DSMI_NET_HEALTH_STATUS *presult);

/**
* @ingroup driver-stub
* @brief Get the ID of the board
* @attention NULL
* @param [in] device_id The device id
* @param [out] board_id Board ID. In the AI ??Server scenario, the value is 0
* @return  0 for success, others for fail
*/
int dsmi_get_board_id(int device_id, unsigned int *board_id);

/**
* @ingroup driver-stub
* @brief Judge whether it is cloud pcie card
* @attention NULL
* @param [in] device_id The device id
* @param [out] NULL
* @return  true or false
*/
bool dsmi_is_cloud_pcie_card(int device_id);

/**
* @ingroup driver-stub
* @brief Query LLC performance parameters, including LLC read hit rate, write hit rate, and throughput
* @attention NULL
* @param [in] device_id  The device id
* @param [out] perf_para  LLC performance parameter information, including LLC read hit rate, 
                         write hit rate and throughput
* @return  0 for success, others for fail
*/
int dsmi_get_llc_perf_para(int device_id, DSMI_LLC_PERF_INFO *perf_para);

/**
* @ingroup driver-stub
* @brief Query the number, maximum operating frequency, current operating frequency and utilization rate of AICPU.
* @attention NULL
* @param [in] device_id  The device id
* @param [out] schedule  return the value wants to query
* @return  0 for success, others for fail
*/
int dsmi_get_aicpu_info(int device_id, struct dsmi_aicpu_info_stru *pdevice_aicpu_info);

/**
* @ingroup driver-stub
* @brief get user configuration
* @attention NULL
* @param [in] device_id  The device id
* @param [in] config_name Configuration item name, the maximum string length of the
                          configuration item name is 32
* @param [in] buf_size buf length, the maximum length is 1024 byte
* @param [out] buf  buf pointer to the content of the configuration item
* @return  0 for success, others for fail
*/
int dsmi_get_user_config(int device_id, const char *config_name, unsigned int buf_size, unsigned char *buf);

/**
* @ingroup driver-stub
* @brief set user configuration
* @attention NULL
* @param [in] device_id  The device id
* @param [in] config_name Configuration item name, the maximum string length of the
                          configuration item name is 32
* @param [in] buf_size buf length, the maximum length is 1024 byte
* @param [in] buf  buf pointer to the content of the configuration item
* @return  0 for success, others for fail
*/
int dsmi_set_user_config(int device_id, const char *config_name, unsigned int buf_size, unsigned char *buf);

/**
* @ingroup driver-stub
* @brief clear user configuration
* @attention NULL
* @param [in] device_id  The device id
* @param [in] config_name Configuration item name, the maximum string length of the
                          configuration item name is 32
* @return  0 for success, others for fail
*/
int dsmi_clear_user_config(int device_id, const char *config_name);

/**
* @ingroup driver-stub
* @brief Get the DIE ID of the specified device
* @attention NULL
* @param [in] device_id  The device id
* @param [out] schedule  return die id infomation
* @return  0 for success, others for fail
*/
int dsmi_get_device_die(int device_id, struct dsmi_soc_die_stru *pdevice_die);
int dsmi_get_device_ndie(int device_id, struct dsmi_soc_die_stru *pdevice_die);
/**
 * @ingroup driver-stub
 * @brief: revocation for different type of operation
 * @param [in] device_id device id
 * @param [in] revo_type revocation type,only support for DSMI_REVOCATION_TYPE_SOC.
 * @param [in] file_data revocation file data
 * @param [in] file_size file data size for revocation
 * @return  0 for success, others for fail
 */
int dsmi_set_sec_revocation(int device_id, DSMI_REVOCATION_TYPE revo_type, const unsigned char *file_data,
    unsigned int file_size);

#ifdef __cplusplus
}
#endif
#endif
