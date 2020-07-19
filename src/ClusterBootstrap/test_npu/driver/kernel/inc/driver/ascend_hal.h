/**
 * @file ascend_hal.h
 *
 * Copyright (c) Huawei Technologies Co., Ltd. 2012-2019. All rights reserved.
 * Description:
 * Author: huawei
 * Create: 2020-01-21
 * @brief driver interface.
 * @version 1.0
 *
 */

#ifndef __ASCEND_HAL_H__
#define __ASCEND_HAL_H__

#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>
#include <time.h>
#include <sys/types.h>

#ifdef __cplusplus
extern "C" {
#endif

#ifndef int8_t
typedef signed char int8_t;
#endif

#ifndef int32_t
typedef signed int int32_t;
#endif

#ifndef uint32_t
typedef unsigned int uint32_t;
#endif

/**
 * @ingroup driver-stub
 * @brief API VERSION NUMBER combines major version and minor verssion,
 * @brief example : 1001  means version  1.001, major 1.0, minor 0.001
 */
#define __HAL_API_VERSION 1000

/**
 * @ingroup driver-stub
 * @brief driver error numbers.
 */
typedef enum tagDrvError {
    DRV_ERROR_NONE = 0,                /**< success */
    DRV_ERROR_NO_DEVICE = 1,           /**< no valid device */
    DRV_ERROR_INVALID_DEVICE = 2,      /**< invalid device */
    DRV_ERROR_INVALID_VALUE = 3,       /**< invalid value */
    DRV_ERROR_INVALID_HANDLE = 4,      /**< invalid handle */
    DRV_ERROR_INVALID_MALLOC_TYPE = 5, /**< invalid malloc type */
    DRV_ERROR_OUT_OF_MEMORY = 6,       /**< out of memory */
    DRV_ERROR_EVENTLIST_FULL = 7,      /**< driver event list full */
    DRV_ERROR_STREAMLIST_FULL = 8,     /**< driver stream list full */
    DRV_ERROR_QOS_QUEUE_FULL = 9,      /**< driver qos queue full*/
    DRV_ERROR_QOS_EMPTY = 10,          /**< driver qos queue empty*/
    DRV_ERROR_NONE_SUBMIT = 11,        /**< there is no submit command in driver qos queue*/
    DRV_ERROR_TASKCMD_QUEUE_FULL = 12, /**< task command queue full*/
    DRV_ERROR_REPORTQUEUE_FULL = 13,   /**< driver report queue full*/
    DRV_ERROR_TSREPORT_EMPTY = 14,     /**< ts report queue empty*/
    DRV_ERROR_OUT_OF_CMD_SLOT = 15,
    DRV_WAIT_TIMEOUT = 16,             /**< driver wait timeout*/
    DRV_ERROR_IOCRL_FAIL = 17,         /**< driver ioctl fail*/

    DRV_ERROR_SOCKET_CREATE = 18,      /**< driver create socket error*/
    DRV_ERROR_SOCKET_CONNECT = 19,     /**< driver connect socket error*/
    DRV_ERROR_SOCKET_BIND = 20,        /**< driver bind socket error*/
    DRV_ERROR_SOCKET_LISTEN = 21,      /**< driver listen socket error*/
    DRV_ERROR_SOCKET_ACCEPT = 22,      /**< driver accept socket error*/
    DRV_ERROR_CLIENT_BUSY = 23,        /**< driver client busy error*/
    DRV_ERROR_SOCKET_SET = 24,         /**< driver socker set error*/
    DRV_ERROR_SOCKET_CLOSE = 25,       /**< driver socker close error*/
    DRV_ERROR_RECV_MESG = 26,          /**< driver recv message error*/
    DRV_ERROR_SEND_MESG = 27,          /**< driver send message error*/
    DRV_ERROR_SERVER_BUSY = 28,
    DRV_ERROR_HDC_CONFIG_READ = 29,
    DRV_ERROR_HDC_UNINIT = 30,
    DRV_ERROR_HDC_SERVER_CREATE = 31,
    DRV_ERROR_HDC_SERVER_CLOSE = 32,
    DRV_ERROR_BUS_DOWN = 33,
    DRV_ERROR_DEVICE_NOT_READY = 34,
    DRV_ERROR_REMOTE_NOT_LISTEN = 35,
    DRV_ERROR_HDC_NO_BLOCK = 36,

    DRV_ERROR_OVER_LIMIT = 37,
    DRV_ERROR_FILE_OPS = 38,
    DRV_ERROR_MBIND_FAIL = 39,
    DRV_ERROR_MALLOC_FAIL = 40,

    DRV_ERROR_REGISTER_STATE_CALLBACK = 41,
    DRV_ERROR_CREATE_THREAD = 42,
    DRV_ERROR_DEV_PROCESS_HANG = 43,

    DRV_ERROR_REMOTE_NO_SESSION = 44,

    DRV_ERROR_HOT_RESET_IN_PROGRESS = 45,

    DRV_ERROR_PERIPHERAL_DEVICE_NOT_EXIST = 46,

    DRV_ERROR_DMP_PERM_DENY = 47,                       /**< dmp add error*/
    DRV_ERROR_DMP_INTERNAL = 48,
    DRV_ERROR_DMP_CMD_NOT_SUPPORT = 49,
    DRV_ERROR_DMP_MEMERY_OPERATOR = 50,
    DRV_PERIPHERAL_DEVICE_NOT_EXIST = 51,

    DRV_ERROR_HDC_DST_PATH_ILLEGAL = 52,                /**< send file dst path illegal*/
    DRV_ERROR_HDC_OPEN_FAILED = 53,                     /**< send file open failed */
    DRV_ERROR_HDC_NO_FREE_SPACE = 54,                   /**< send file no free space */
    DRV_ERROR_HDC_LOCAL_ABNORMAL_FILE = 55,             /**< send file local file abnormal*/
    DRV_ERROR_HDC_DST_PATH_PERMISSION_DENIED = 56,      /**< send file dst path Permission denied*/
    DRV_ERROR_HDC_DST_NO_SUCH_FILE = 57,                /**< pull file no such file or directory*/

    DRV_ERROR_RESERVED,
} drvError_t;
/**
 * @ingroup driver-stub
 * @brief driver unified error numbers.
 * @brief new code must return error numbers based on unified error numbers.
 */
#define HAL_ERROR_CODE_BASE  0x90020000

/**
 * @ingroup driver-stub
 * @brief module definition of error numbers
 */
#define HAL_MODULE_TYPE_COMMON   0x00
#define HAL_MODULE_TYPE_PCIE   0x01
#define HAL_MODULE_TYPE_HDC   0x02
#define HAL_MODULE_TYPE_SVM   0x03
#define HAL_MODULE_TYPE_DVPP   0x04
#define HAL_MODULE_TYPE_DEV_DRIVER   0x05
#define HAL_MODULE_TYPE_LOG_PROF   0x06
#define HAL_MODULE_TYPE_DEV_MANAGER   0x07
#define HAL_MODULE_TYPE_SAFETY_ISLAND   0x08

/**
 * @ingroup driver-stub
 * @brief each error code use definition "HAI_ERROR_CODE(MODULE, ERROR_CODE)"
 * @brief which MODULE is the module and ERROR_CODE is the error number.
 */
#define HAI_ERROR_CODE(MODULE, ERROR_CODE) (HAL_ERROR_CODE_BASE + ERROR_CODE + (MODULE << 12))
#define HAI_ERROR_CODE_NO_MODULE(ERROR_CODE) (ERROR_CODE & 0x00000FFF)
/**
 * @ingroup driver-stub
 * @brief turn deviceID to nodeID
 */
#define DEVICE_TO_NODE(x) x
#define NODE_TO_DEVICE(x) x

/**
 * @ingroup driver-stub
 * @brief memory type
 */
typedef enum tagDrvMemType {
    DRV_MEMORY_HBM, /**< HBM memory on device */
    DRV_MEMORY_DDR, /**< DDR memory on device */
} drvMemType_t;

/**
 * @ingroup driver-stub
 * @brief memcpy kind.
 */
typedef enum tagDrvMemcpyKind {
    DRV_MEMCPY_HOST_TO_HOST,     /**< host to host */
    DRV_MEMCPY_HOST_TO_DEVICE,   /**< host to device */
    DRV_MEMCPY_DEVICE_TO_HOST,   /**< device to host */
    DRV_MEMCPY_DEVICE_TO_DEVICE, /**< device to device */
} drvMemcpyKind_t;

/**
 * @ingroup driver-stub
 * @brief Async memcpy parameter.
 */
typedef struct tagDrvDmaAddr {
    void *dst;   /**< destination address */
    void *src;   /**< source address */
    int32_t len; /**< the number of byte to copy */
    int8_t flag; /**< mulitycopy flag */
} drvDmaAddr_t;

/**
 * @ingroup driver-stub
 * @brief interrupt number that task scheduler set to driver.
 */
typedef enum tagDrvInterruptNum {
    DRV_INTERRUPT_QOS_READY = 0, /**< QoS queue almost empty*/
    DRV_INTERRUPT_REPORT_READY,  /**< Return queue almost full*/
    DRV_INTERRUPT_RESERVED,
} drvInterruptNum_t;

typedef void *drvCommand_t;

#ifdef __linux
#define DLLEXPORT
#else
#define DLLEXPORT _declspec(dllexport)
#endif

#define PROCESS_SIGN_LENGTH  49
#define PROCESS_RESV_LENGTH  4

typedef enum tagDrvStatus {
    DRV_STATUS_INITING = 0x0,
    DRV_STATUS_WORK,
    DRV_STATUS_EXCEPTION,
    DRV_STATUS_SLEEP,
    DRV_STATUS_RESERVED,
} drvStatus_t;

typedef struct tagDrvSpec {
    unsigned int aiCoreNum;
    unsigned int aiCpuNum;
    unsigned int aiCoreInuse;
    unsigned int aiCoreErrorMap;
    unsigned int aiCpuInuse;
    unsigned int aiCpuErrorMap;
} drvSpec_t;

typedef struct tagDrvCpuInfo {
    unsigned int ccpu_num;
    unsigned int ccpu_os_sched;
    unsigned int dcpu_num;
    unsigned int dcpu_os_sched;
    unsigned int aicpu_num;
    unsigned int aicpu_os_sched;
    unsigned int tscpu_num;
    unsigned int tscpu_os_sched;
} drvCpuInfo_t;

struct devdrv_device_info {
    uint8_t env_type; /**< 0, FPGA  1, EMU 2, ESL*/
    unsigned int ctrl_cpu_ip;
    unsigned int ctrl_cpu_id;
    unsigned int ctrl_cpu_core_num;
    unsigned int ctrl_cpu_endian_little;
    unsigned int ts_cpu_core_num;
    unsigned int ai_cpu_core_num;
    unsigned int ai_core_num;
    unsigned int ai_cpu_core_id;
    unsigned int ai_core_id;
    unsigned int aicpu_occupy_bitmap;
    unsigned int hardware_version;
    unsigned int ts_num;
};

struct process_sign {
    pid_t tgid;
    char sign[PROCESS_SIGN_LENGTH];
    char resv[PROCESS_RESV_LENGTH];
};

/**
 * @ingroup driver-stub
 * @brief  get device info when open device
 */
struct drvDevInfo {
    int fd;
};

typedef enum {
    CMD_TYPE_POWERON,
    CMD_TYPE_POWEROFF,
    CMD_TYPE_CM_ALLOC,
    CMD_TYPE_CM_FREE,
    CMD_TYPE_SC_FREE,
    CMD_TYPE_MAX,
} devdrv_cmd_type_t;

typedef struct tag_alloc_cm_para {
    void **ptr;
    uint64_t size;
} devdrv_alloc_cm_para_t;

typedef struct tag_free_cm_para {
    void *ptr;
} devdrv_free_cm_para_t;

#ifdef __linux
typedef enum {
    DRVDEV_CALL_BACK_SUCCESS = 0,
    DRVDEV_CALL_BACK_FAILED,
} devdrv_callback_state_t;

typedef enum {
    GO_TO_SO = 0,
    GO_TO_SUSPEND,
    GO_TO_S3,
    GO_TO_S4,
    GO_TO_D0,
    GO_TO_D3,
    GO_TO_DISABLE_DEV,
    GO_TO_ENABLE_DEV,
    GO_TO_STATE_MAX,
} devdrv_state_t;

typedef struct tag_state_info {
    devdrv_state_t state;
    uint32_t devId;
} devdrv_state_info_t;

struct drvStreamIdInfo {
    uint32_t tsId;
    int32_t streamId;
    uint32_t strategy;
};

struct drvModelIdInfo {
    uint32_t tsId;
    int32_t modelId;
};

struct drvEventIdInfo {
    uint32_t tsId;
    int32_t eventId;
};

struct drvCommandInfo {
    uint32_t tsId;
    int32_t streamId;
    int32_t cmdCount;
    int32_t reportCount;
};

struct drvReportGetInfo {
    uint32_t tsId;
    int32_t count;
    void *reportPtr;
};

struct drvCommandOccupyInfo {
    uint32_t tsId;
    int32_t streamId;
    int32_t cmdCount;
    void *cmdPtr;
};

struct drvReportInfo {
    uint32_t tsId;
    int32_t timeout;
};

struct drvReportReleaseInfo {
    uint32_t tsId;
    int32_t count;
};

struct drvTaskPoolIdInfo {
    uint32_t tsId;
    int32_t taskPoolId;
    void *taskPoolVaddr;
    uint32_t size;
};

struct drvNotifyInfo {
    uint32_t tsId;
    uint32_t notifyId;
    uint64_t devAddrOffset;
};

struct drvIpcNotifyInfo {
    uint32_t tsId;
    uint32_t devId;
    uint32_t notifyId;
};

struct drvTsExceptionInfo {
    uint32_t tsId;
    uint64_t exception_code;
};

typedef devdrv_callback_state_t (*drvDeviceStateNotify)(devdrv_state_info_t *state);
#endif
typedef int (*drvDeviceExceptionReporFunc)(uint32_t devId, uint32_t exceptionId, struct timespec timeStamp);
typedef int (*drvDeviceStartupNotify)(uint32_t num, uint32_t *devId);
/**
* @ingroup driver-stub
* @brief Black box status notification callback function registration Interface
* @attention null
* @param [in] drvDeviceStateNotify state_callback
* @return 0 success, others for fail
*/
#ifdef __linux
DLLEXPORT drvError_t drvDeviceStateNotifierRegister(drvDeviceStateNotify state_callback);
#endif
/**
* @ingroup driver-stub
* @brief Black box status Start up callback function registration
* @attention null
* @param [in] startup_callback  callback function poiniter
* @return  0 for success, others for fail
*/
DLLEXPORT drvError_t drvDeviceStartupRegister(drvDeviceStartupNotify startup_callback);

DLLEXPORT drvError_t halGetAPIVersion(int *halAPIVersion);
/**
* @ingroup driver-stub
* @brief get cpu information of device
* @attention null
* @param [in]  devId device id
* @param [out]  drvspec cpu information
* @return  0 for success, others for fail
*/
DLLEXPORT drvError_t drvGetDeviceSpec(uint32_t devId, drvSpec_t *drvspec);
/**
* @ingroup driver-stub
* @brief get device availability information
* @attention null
* @param [in] devId  device id
* @param [out] status  device status
* @return   0 for success, others for fail
*/
DLLEXPORT drvError_t drvDeviceStatus(uint32_t devId, drvStatus_t *status);
/**
* @ingroup driver-stub
* @brief open device
* @attention it will return error when reopen device
* @param [in] devId  device id
* @param [out] devInfo  device information
* @return   0 for success, others for fail
*/
DLLEXPORT drvError_t drvDeviceOpen(void **devInfo, uint32_t devId);
/**
* @ingroup driver-stub
* @brief get the availabel device number
* @attention null
* @param [out] count  device number
* @return   0 for success, others for fail
*/
DLLEXPORT drvError_t drvDeviceGetCount(int32_t *count);
/**
* @ingroup driver-stub
* @brief close device
* @attention it will return error when reclose device
* @param [in] devid  device id
* @return   0 for success, others for fail
*/
DLLEXPORT drvError_t drvDeviceClose(uint32_t devId);
/**
* @ingroup driver-stub
* @brief alloc stream id of device
* @attention null
* @param [in] devid  device id
* @param [in] info  info when alloc stream id includes ts id and strategy of CQ/SQ
* @param [out] info  stream id, which is in [0,1023]
* @return   0 for success, others for fail
*/
DLLEXPORT drvError_t drvStreamIdAlloc(uint32_t devId, struct drvStreamIdInfo* info);
/**
* @ingroup driver-stub
* @brief alloc model id
* @attention null
* @param [in] devId  device id
* @param [in] info  info when alloc model id includes ts id
* @param [out] info  model id, which is in [0,1023]
* @return  0 for success, others for fail
*/
DLLEXPORT drvError_t drvModelIdAlloc(int32_t devId, struct drvModelIdInfo* info);
/**
* @ingroup driver-stub
* @brief release stream id resource
* @attention it will return error if refree stream id
* @param [in] devId  device id
* @param [in] info stream free information includes ts id and stream id
* @return  0 for success, others for fail
*/
DLLEXPORT drvError_t drvStreamIdFree(uint32_t devId, struct drvStreamIdInfo* info);
/**
* @ingroup driver-stub
* @brief alloc event id
* @attention null
* @param [in] devId  device id
* @param [in] info alloc information includes ts id
* @param [out] info  event id which is in [0,1023]
* @return   0 for success, others for fail
*/
DLLEXPORT drvError_t drvEventIdAlloc(uint32_t devId, struct drvEventIdInfo* info);
/**
* @ingroup driver-stub
* @brief free event id
* @attention it will return error when refree event id
* @param [in] devId  device id
* @param [in] info free event id information includes ts id and event id
* @return   0 for success, others for fail
*/
DLLEXPORT drvError_t drvEventIdFree(uint32_t devId, struct drvEventIdInfo* info);
/**
* @ingroup driver-stub
* @brief free model id
* @attention it will return error when refree model id
* @param [in] devId  device id
* @param [in] info free model id information includes ts id and model id
* @return   0 for success, others for fail
*/
DLLEXPORT drvError_t drvModelIdFree(int32_t devId, struct drvModelIdInfo* info);
/**
* @ingroup driver-stub
* @brief apply for certain number of sq cmd slots for filling in commands
* @attention The number of allocations may be insufficient, then the currently available slots are allocated,
* the number of allocations will sent out via cmdCount and return DRV_ERROR_OUT_OF_CMD_SLOT errorcode.
* @param [in] devId device id
* @param [in] info  the parameter includes:
*             tsId: ts id, ascend310:0, ascend910 :0
*             streamId: stream id which is in [0,1023]
*             cmdCount: number of slot which will be alloced which is in [1,1023]
* @param [out] info  the parameter includes:
*             cmdCount sq cmd slot number alloced
* @return   0 for success, others for fail
*/
DLLEXPORT drvError_t drvCommandOccupy(uint32_t devId, struct drvCommandOccupyInfo* info);
/**
* @ingroup driver-stub
* @brief runtime send command interface
* @attention runtime should has completed filling the command before calling this interface,
* this interface used to update the pointer of SQ, driver can only judge the scope of cmdCount,
* it it impossible to determine if runtime has completed filling the right command.
* @param [in] devId device id
* @param [in] info command send information includes:
*             tsId: ts id,  ascend310:0, ascend910 :0
*             streamId: stream id which is in [0,1023]
*             cmdCount: slot id number, which is in [1,1023]
*             reportCount: report number  expected
* @return   0 for success, others for fail
*/
DLLEXPORT drvError_t drvCommandSend(uint32_t devId, struct drvCommandInfo* info);
/**
* @ingroup driver-stub
* @brief runtime wait report interface, irq mode, wait report interrupt of TS
* @attention The wait time is ms level, Less than 0 is an infinite wait
* @param [in] devId device id
* @param [in] info report info which includes:
*             tsId: ts id,  ascend310:0, ascend910 :0
*             timeout: wait timeout time
* @return   0 for success, others for fail
*/
DLLEXPORT drvError_t drvReportIrqWait(uint32_t devId, struct drvReportInfo* info);
/**
* @ingroup driver-stub
* @brief runtime wait report interface, poll mode, wait report interrupt of TS
* @attention null
* @param [in] devId device id
* @param [in] info report info which includes:
*              tsId: ts id,  ascend310:0, ascend910 :0
* @param [out] info report info which includes:
*              timeout: poll wait timeout
* @return  0 for success, others for fail
*/
DLLEXPORT drvError_t drvReportPollWait(uint32_t devId, struct drvReportInfo* info);
/**
* @ingroup driver-stub
* @brief runtime get report interface
* @attention null
* @param [in] devId device id
* @param [in] info report info which includes:
*              tsId: ts id,  ascend310:0, ascend910 :0
* @param [out] info report info which includes:
*              count:  report number received
*              reportPtr: the first address of the received report
* @return  0 for success, others for fail
*/
DLLEXPORT drvError_t drvReportGet(uint32_t devId, struct drvReportGetInfo* info);
/**
* @ingroup driver-stub
* @brief runtime release report interface
* @attention null
* @param [in] devId device id
* @param [in] info report info which includes:
*             tsId: ts id,  ascend310:0, ascend910 :0
*             count:  release report number in [1,1023]
* @return  0 for success, others for fail
*/
DLLEXPORT drvError_t drvReportRelease(uint32_t devId, struct drvReportReleaseInfo* info);
/**
* @ingroup driver-stub
* @brief Obtains the PCIe bus information of device
* @attention null
* @param [in] devId device id
* @param [out] *bus  Returns the PCIe bus number
* @param [out] *dev  Returns the PCIe device ID
* @param [out] *func Returns the PCIe function ID
* @return   0 for success, others for fail
*/
DLLEXPORT drvError_t drvDeviceGetPcieInfo(uint32_t devId, int32_t *bus, int32_t *dev, int32_t *func);
/**
* @ingroup driver-stub
* @brief Get the dma handling method of the device
* @attention The transport method based on the source and destination addresses should be implemented
* by the runtime layer. However, since the mini and cloud implementation methods are different,
* the runtime does not have a corresponding macro partition, so DRV sinks to the kernel state and adds
* the macro partition
* @param [in] *src  Memory address of source device
* @param [in] *dest Memory address of the destination device
* @param [out] *trans_type trans type which has two types:
*              DRV_SDMA = 0x0;  SDMA mode move
*              DRV_PCIE_DMA = 0x1;  PCIE mode move
* @return   0 for success, others for fail
*/
DLLEXPORT drvError_t drvDeviceGetTransWay(void *src, void *dest, uint8_t *trans_type);
/**
* @ingroup driver-stub
* @brief Get current platform information
* @attention null
* @param [out] *info  0 Means currently on the Device side, 1/Means currently on the host side
* @return   0 for success, others for fail
*/
DLLEXPORT drvError_t drvGetPlatformInfo(uint32_t *info);
/**
* @ingroup driver-stub
* @brief Get the current number of devices
* @attention null
* @param [out] *num_dev  Number of current devices
* @return   0 for success, others for fail
*/
DLLEXPORT drvError_t drvGetDevNum(uint32_t *num_dev);
/**
* @ingroup driver-stub
* @brief Convert device-side devId to host-side devId
* @attention null
* @param [in] localDevId  chip ID
* @param [out] *devId  host side devId
* @return   0 for success, others for fail
*/
DLLEXPORT drvError_t drvGetDevIDByLocalDevID(uint32_t localDevId, uint32_t *devId);
/**
* @ingroup driver-stub
* @brief The device side and the host side both obtain the host IDs of all the current devices.
* If called in a container, get the host IDs of all devices in the current container.
* @attention null
* @param [out] *devices   host ID
* @param [out] len  Array length
* @return   0 for success, others for fail
*/
DLLEXPORT drvError_t drvGetDevIDs(uint32_t *devices, uint32_t len);
/**
* @ingroup driver-stub
* @brief Get the chip IDs of all current devices
* @attention null
* @param [out] *devices  host ID
* @param [out] len  Array length
* @return   0 for success, others for fail
*/
DLLEXPORT drvError_t drvGetDeviceLocalIDs(uint32_t *devices, uint32_t len);
/**
* @ingroup driver-stub
* @brief Get device id via host device id , only called in device side.
* @attention null
* @param [out] *devices  host ID
* @param [out] len  Array length
* @return   0 for success, others for fail
*/
DLLEXPORT drvError_t drvGetLocalDevIDByHostDevID(uint32_t host_dev_id, uint32_t *local_dev_id);
/**
* @ingroup driver-stub
* @brief Get device information, number and bit order of ctrl_cpu and ai_cpu, etc
* @attention null
* @param [in] devId  Device ID
* @param [out] *info  See struct devdrv_device_info
* @return   0 for success, others for fail
*/
DLLEXPORT drvError_t drvGetDevInfo(uint32_t devId, struct devdrv_device_info *info);
/**
* @ingroup driver-stub
* @brief Used to define the unique interface of the product. The cmd command word remains unified, compatible,
* and functions are implemented independently
* @attention only support lite
* @param [in] devId  Device ID
* @param [in] cmd  cmd command word
* @param [in] *para
* @param [out] None, can be passed in para
* @return    0 for success, others for fail
*/
DLLEXPORT drvError_t drvCustomCall(uint32_t devId, uint32_t cmd, void *para);
/**
* @ingroup driver-stub
* @brief The black box daemon on the host side calls the interface registration exception reporting function
* @attention null
* @param [in] exception_callback_func  Exception reporting function
* @return   0 for success, others for fail
*/
DLLEXPORT drvError_t drvDeviceExceptionHookRegister(drvDeviceExceptionReporFunc exception_callback_func);
/**
* @ingroup driver-stub
* @brief The black box daemon on the host side calls this interface to export the exception log
* @attention null
* @param [in] devId  Device ID
* @param [in] bbox_addr_offset  The relative offset of the log in the physical address of the device
* @param [in] size  Log size
* @param [in] *buffer  Receive log buffer
* @return    0 for success, others for fail
*/
DLLEXPORT drvError_t drvDeviceMemoryDump(uint32_t devId, uint64_t bbox_addr_offset, uint32_t size, void *buffer);
/**
* @ingroup driver-stub
* @brief Get CPU information by devId
* @attention null
* @param [in] devId  Device ID
* @param [out] *cpuInfo  See drvCpuInfo_t
* @return   0 for success, others for fail
*/
DLLEXPORT drvError_t drvGetCpuInfo(uint32_t devId, drvCpuInfo_t *cpuInfo);
/**
* @ingroup driver-stub
* @brief Flash cache interface
* @attention
* 1.Virtual address is the virtual address of this process; 2.Note whether the length passed in meets the requirements
* @param [in] base  Virtual address base address
* @param [in] len  cache length
* @return   0 for success, others for fail
*/
DLLEXPORT void drvFlushCache(uint64_t base, uint32_t len);
/**
* @ingroup driver-stub
* @brief Get physical ID (phyId) using logical ID (devIndex)
* @attention null
* @param [in] devIndex  Logical ID
* @param [out] *phyId  Physical ID
* @return  0 for success, others for fail
*/
DLLEXPORT drvError_t drvDeviceGetPhyIdByIndex(uint32_t devIndex, uint32_t *phyId);
/**
* @ingroup driver-stub
* @brief Get logical ID (devIndex) using physical ID (phyId)
* @attention null
* @param [in] phyId   Physical ID
* @param [out] devIndex  Logical ID
* @return   0 for success, others for fail
*/
DLLEXPORT drvError_t drvDeviceGetIndexByPhyId(uint32_t phyId, uint32_t *devIndex);
/**
* @ingroup driver-stub
* @brief host process random flags get interface
* @attention null
* @param [out] *sign  host process random flag
* @return   0 for success, others for fail
*/
DLLEXPORT drvError_t drvGetProcessSign(struct process_sign *sign);
/**
* @ingroup driver-stub
* @brief get online device list interface
* @attention when device boot,device list only get once,otherwise will get no device.
* @param [in] *devBuf buffer for storing physical device id
* @param [in] bufCnt buffer count,max count for store device id
* @param [out] devCnt online device count.
* @return   0 for success, others for fail
*/
DLLEXPORT drvError_t halGetOnlineDevList(unsigned int *devBuf, unsigned int bufCnt, unsigned int *devCnt);
DLLEXPORT drvError_t halDevOnlinePollWait(void);
#ifdef __linux
/**
* @ingroup driver-stub
* @brief Get non-container internal pid number
* @attention null
* @return pid number (non-container pid)
*/
DLLEXPORT pid_t drvDeviceGetBarePid(void);
DLLEXPORT pid_t drvDeviceGetBareTgid(void);
#endif
/**
* @ingroup driver-stub
* @brief HP/DELL/LENOVO PC send I2C reset cmd to Device
* @attention only support HP/DELL/LENOVO PC + EVB VB
* @param [in] devId  : Device ID
* @return   0 for success, others for fail
*/
DLLEXPORT drvError_t drvResetDevice(uint32_t devId);
/**
* @ingroup driver-stub
* @brief map kernel space for ddrdump
* @attention null
* @param [in] devId  : Device ID
* @param [in] virAddr : user space addr
* @param [out] *size : kernel space size
* @return   0 for success, others for fail
*/
DLLEXPORT drvError_t drvDmaMmap(uint32_t devId, uint64_t virAddr, uint32_t *size);
/**
* @ingroup driver-stub
* @brief read value from sram addr
* @attention offset + len <= load sram len(0x40000)
* @param [in]  devId  : Device ID
* @param [in]  offset: sram offset
* @param [in]  len: length of read content
* @param [out] *value: read value
* @return   0 for success, others for fail
*/
DLLEXPORT drvError_t drvPcieSramRead(uint32_t devId, uint32_t offset, uint8_t *value, uint32_t len);
/**
* @ingroup driver-stub
* @brief write value to sram addr
* @attention null
* @param [in]  devId  : Device ID
* @param [in]  offset: sram offset
* @param [in]  len: length of write content
* @param [in]  *value: write value
* @return   0 for success, others for fail
*/
DLLEXPORT drvError_t drvPcieSramWrite(uint32_t devId, uint32_t offset, uint8_t *value, uint32_t len);
/**
* @ingroup driver-stub
* @brief read value from bbox ddr addr
* @attention offset + len <= bbox ddr len(0x200000)
* @param [in]  devId  : Device ID
* @param [in]  offset : bbox ddr offset
* @param [in]  len : length of read content
* @param [out] *value : read value
* @return   0 for success, others for fail
*/
DLLEXPORT drvError_t drvPcieDdrRead(uint32_t devId, uint32_t offset, uint8_t *value, uint32_t len);
/**
* @ingroup driver-stub
* @brief pcie pre-reset, release pcie related resources applied by each module on the host side
* @attention All functions of pcie are invalid after calling
* @param [in]  devId  : Device ID
* @return   0 for success, others for fail
*/
DLLEXPORT drvError_t drvPciePreReset(uint32_t devId);
/**
* @ingroup driver-stub
* @brief pcie rescan, re-apply the pcie related resources required by each module on the host side
* @attention All functions of pcie are invalid after calling
* @param [in]  devId  : Device ID
* @return   0 for success, others for fail
*/
DLLEXPORT drvError_t drvPcieRescan(uint32_t devId);
#define DRV_P2P_STATUS_ENABLE 1
#define DRV_P2P_STATUS_DISABLE 0
/**
* @ingroup driver-stub
* @brief p2p Enable interface
* @attention Both directions must be set to take effect, and support sdma and vnic interworking
* @param [in]  dev : Logical device id
* @param [in]  peer_dev : Physical device id
* @return   0 for success, others for fail
*/
DLLEXPORT drvError_t drvEnableP2P(uint32_t dev, uint32_t peer_dev);
/**
* @ingroup driver-stub
* @brief p2p Disable interface
* @attention Both directions must be set to take effect, and support sdma and vnic interworking
* @param [in]  dev : Logical device id
* @param [in]  peer_dev : Physical device id
* @return   0 for success, others for fail
*/
DLLEXPORT drvError_t drvDisableP2P(uint32_t dev, uint32_t peer_dev);
/**
* @ingroup driver-stub
* @brief p2p Status interface
* @attention Both directions must be set to take effect, and support sdma and vnic interworking
* @param [in]  dev : Logical device id
* @param [in]  peer_dev : Physical device id
* @param [out]  0 for disable, 1 for enable
* @return   0 for success, others for fail
*/
DLLEXPORT drvError_t drvGetP2PStatus(uint32_t dev, uint32_t peer_dev, uint32_t *status);
/**
* @ingroup driver-stub
* @brief p2p enable related interface
* @attention Both directions must be set to take effect, and support sdma and vnic interworking
* @param [in]  dev_id : Logical device id
* @param [in]  master_dev_id : master device id
* @return   0 for success, others for fail
*/
DLLEXPORT drvError_t drvGetMasterDeviceInTheSameOS(unsigned int dev_id, unsigned int *master_dev_id);
/**
* @ingroup driver-stub
* @brief get user config from config file
* @attention The location of the config file in the system : /usr/local/HiAI/driver/config/davinci_user_config
* @param [in]  *config_name: config name uint32_t
* @param [in]  config_name_len: config name len
* @param [in]  config_value_len: max config value len
* @param [out] *config_value: config value
* @return   0 for success, others for fail
*/
DLLEXPORT drvError_t drvGetUserConfig(const char *config_name, uint32_t config_name_len, char *config_value,
                                      uint32_t config_value_len);
/**
* @ingroup driver-stub
* @brief host get device boot status
* @attention null
* @param [in]  device_id : device ID
* @param [out] boot_status : See dsmi_boot_status definition
* @return   0 for success, others for fail
*/
DLLEXPORT drvError_t drvGetDeviceBootStatus(int device_id, uint32_t *boot_status);
#ifdef CFG_SOC_PLATFORM_CLOUD
/**
* @ingroup driver-stub
* @brief read data from IMU bbox reserved addr
* @attention offset + len <= IMU bbox len(0x800000)
* @param [in]  devId : Device ID
* @param [in]  offset : bbox ddr offset
* @param [in]  len : length of read content
* @param [out] *value : read value
* @return   0 for success, others for fail
*/
DLLEXPORT drvError_t drvPcieIMUDDRRead(uint32_t devId, uint32_t offset, uint8_t *value, uint32_t len);
#endif
/**
* @ingroup driver-stub
* @brief Apply notify id
* @attention null
* @param [in] devId  Device number
* @param [in] *info  See struct drvNotifyInfo which includes:
*              tsId: ts id,  ascend310:0, ascend910 :0
* @param [out] *info: *notifyId : Returns notify id, the range of values for notify id [0,1023]
* @return   0 for success, others for fail
*/
DLLEXPORT drvError_t drvNotifyIdAlloc(uint32_t devId, struct drvNotifyInfo *info);
/**
* @ingroup driver-stub
* @brief free notify id
* @attention Repeated release of the notification id will return an error
* @param [in] devId  Device number
* @param [in] *info  See struct drvNotifyInfo which includes:
*              tsId: ts id,  ascend310:0, ascend910 :0
*              notifyId: the range of values for notify id [0,1023]
* @return   0 for success, others for fail
*/
DLLEXPORT drvError_t drvNotifyIdFree(uint32_t devId, struct drvNotifyInfo *info);
/**
* @ingroup driver-stub
* @brief Get offset address of notify id
* @attention null
* @param [in]  devId  Device number
* @param [in] *info  See struct drvNotifyInfo which includes:
*               tsId: ts id,  ascend310:0, ascend910 :0
*              notifyId, the range of values for notify id [0,1023]
* @param [out] *info: devAddrOffset:  Physical address offset
* @return    0 for success, others for fail
*/
DLLEXPORT drvError_t drvNotifyIdAddrOffset(uint32_t devId, struct drvNotifyInfo *info);
/**
* @ingroup driver-stub
* @brief drvCreateIpcNotify
* @attention null
* @param [in] *name  Ipc notify name to be created
* @param [in] len  name lenth
* @param [in] *info  See struct drvIpcNotifyInfo
*              tsId: ts id,  ascend310:0, ascend910 :0
*              devId: device id
* @return   0 for success, others for fail
*/
DLLEXPORT drvError_t drvCreateIpcNotify(struct drvIpcNotifyInfo *info, char *name, unsigned int len);
/**
* @ingroup driver-stub
* @brief Destroy ipc notify
* @attention null
* @param [in] *name  Ipc notify name to be destroyed
* @param [in] *info  See struct drvIpcNotifyInfo
*             tsId: ts id,  ascend310:0, ascend910 :0
*             devId: device id
* @return   0 for success, others for fail
*/
DLLEXPORT drvError_t drvDestroyIpcNotify(const char *name, struct drvIpcNotifyInfo *info);
/**
* @ingroup driver-stub
* @brief Open ipc notify
* @attention null
* @param [in] *name  Ipc notify name to open
* @param [in] *info  See struct drvIpcNotifyInfo
*             tsId: ts id,  ascend310:0, ascend910 :0
*             devId: device id
* @param [out] *info  *notifyId  Return opened notification id
* @return   0 for success, others for fail
*/
DLLEXPORT drvError_t drvOpenIpcNotify(const char *name, struct drvIpcNotifyInfo *info);
/**
* @ingroup driver-stub
* @brief Close ipc notify
* @attention null
* @param [in] *name  Ipc notify name to close
* @param [in] *info  See struct drvIpcNotifyInfo
*              tsId: ts id,  ascend310:0, ascend910 :0
*              devId: device id
*              notifyId : notification id
* @return   0 for success, others for fail
*/
DLLEXPORT drvError_t drvCloseIpcNotify(const char *name, struct drvIpcNotifyInfo *info);
/**
* @ingroup driver-stub
* @brief Set the notification pid whitelist
* @attention null
* @param [in] *name  Ipc notify name to be set
* @param [in] pid[]  Array of whitelisted processes
* @param [in] num  number of whitelisted processes
* @return   0 for success, others for fail
*/
DLLEXPORT drvError_t drvSetIpcNotifyPid(const char *name, pid_t pid[], int num);

/**
* @ingroup driver-stub
* @brief Start host and mini time zone synchronization thread
* @attention Can only be called once by the daemon on the host, not multiple times
* @return   0 for success, others for fail
*/
drvError_t drvStartTimeSyncServeHost(void);
/**
* @ingroup driver-stub
* @brief Assign taskPoolId via devId
* @param [in]  devId  Device ID
* @param [in]  *info  See struct drvTaskPoolIdInfo
*              tsId: ts id,  ascend310:0, ascend910 :0
*              *taskPoolVaddr: not used, suggest to NULL
*              size : not used, suggest to NULL
* @param [out] *info, taskPoolId: task poll id
* @return   0 for success, others for fail
*/
drvError_t drvTaskPoolIdAlloc(uint32_t devId, struct drvTaskPoolIdInfo* info);
/**
* @ingroup driver-stub
* @brief Free task poll id
* @param [in]  devId  Device ID
* @param [in]  *info  See struct drvTaskPoolIdInfo
*              tsId: ts id,  ascend310:0, ascend910 :0
*              *taskPoolVaddr: not used, suggest to NULL
*              size : not used, suggest to NULL
*              taskPoolId: task poll id
* @return   0 for success, others for fail
*/
drvError_t drvTaskPoolIdFree(uint32_t devId, struct drvTaskPoolIdInfo* info);
/**
* @ingroup driver-stub
* @brief Get taskPoolVaddr address by devId
* @param [in]  devId  Device ID
* @param [in]  *info See struct drvTaskPoolIdInfo
*              tsId: ts id,  ascend310:0, ascend910 :0
*              *taskPoolVaddr: not used, suggest to NULL
*              size : not used, suggest to NULL
*              taskPoolId: task poll id
* @param [out] *info, *taskPoolVaddr: task poll base addr
* @return   0 for success, others for fail
*/
drvError_t drvTaskPoolGetBaseVaddr(uint32_t devId, struct drvTaskPoolIdInfo* info);
/**
* @ingroup driver-stub
* @brief cqsq channel positioning information
* @attention null
* @param [in] devId  Device ID
* @return   0 for success, others for fail
*/
void devdrv_report_error_dump(uint32_t devId);

/**
* @ingroup driver-stub
* @brief Judge whether it is cloud pcie card
* @attention null
* @param [in] devId  Device ID
* @return   true or false
*/
bool devdrv_is_cloud_pcie_card(unsigned int dev_id);

/**
* @ingroup driver-stub
* @brief send IPC msg to safetyIsland
* @attention null
* @param [in]   devId  Device ID
*               msg : message contents
                msgSize : message size
* @return   0 for success, others for fail
*/
drvError_t halSafeIslandTimeSyncMsgSend(uint32_t devId, void *msg, size_t msgSize);


struct halSqCqChannelAllocInfo {
    uint32_t tsId;
    uint32_t sqeSize;
    uint32_t cqeSize;
    uint32_t sqeDepth;
    uint32_t cqeDepth;
    uint32_t sqId;
    uint32_t cqId;
    uint32_t grpId;
    uint32_t res[8];
};

struct halSqCqChannelFreeInfo {
    uint32_t tsId;
    uint32_t sqId;
    uint32_t cqId;
};

struct halSqCommandOccupyInfo {
    uint32_t tsId;
    uint32_t sqId;
    int32_t cmdCount;
    volatile void *cmdPtr;
    uint32_t res[8];
};

struct halSqCommandInfo {
    uint32_t tsId;
    uint32_t sqId;
    int32_t cmdCount;
    int32_t reportCount;
};

struct halReportInfo {
    uint32_t grpId;
    uint32_t tsId;
    int32_t timeout;
    uint64_t *cqIdBitmap;
    uint32_t cqIdBitmapSize;
    uint32_t res[8];
};

struct halReportGetInfo {
    uint32_t tsId;
    uint32_t cqId;
    int32_t count;
    void *reportPtr;
};
struct halReportReleaseInfo {
    uint32_t tsId;
    uint32_t cqId;
    int32_t count;
};

drvError_t halSqCqAllocate(uint32_t devId, struct halSqCqChannelAllocInfo *info);
drvError_t halSqCqFree(uint32_t devId, struct halSqCqChannelFreeInfo *info);
drvError_t halSqCommandOccupy(uint32_t devId, struct halSqCommandOccupyInfo *info);
drvError_t halSqCommandSend(uint32_t devId, struct halSqCommandInfo *info);
drvError_t halCqReportIrqWait(uint32_t devId, struct halReportInfo *info);
drvError_t halCqReportGet(uint32_t devId, struct halReportGetInfo *info);
drvError_t halCqReportRelease(uint32_t devId, struct halReportReleaseInfo *info);
#define DV_OFFLINE
#define DV_ONLINE
#define DV_OFF_ONLINE
#define DV_LITE

typedef unsigned long long UINT64;
typedef unsigned int UINT32;
typedef unsigned short UINT16;
typedef unsigned char UINT8;

#define DV_ADVISE_HBM 0x0000
#define DV_ADVISE_DDR 0x0001
#define DV_ADVISE_EXEC 0x0002
#define DV_ADVISE_HUGEPAGE 0x0004
#define DV_ADVISE_POPULATE 0x0008
#define DV_ADVISE_P2P_HBM 0x0010
#define DV_ADVISE_P2P_DDR 0x0020
#define DV_ADVISE_4G_DDR 0x0040
#define DV_ADVISE_LOCK_DEV 0x0080

#define ADVISE_TYPE (1UL)       /**< 0: DDR memory 1: HBM memory */
#define ADVISE_EXE (1UL << 1)   /**< setting executable permissions */
#define ADVISE_THP (1UL << 2)   /**< using huge page memory */
#define ADVISE_PLE (1UL << 3)   /**< memory prefetching */
#define ADVISE_PIN (1UL << 4)   /**< pin ddr memory */
#define ADVISE_UNPIN (1UL << 5) /**< unpin ddr memory */

typedef UINT32 DVmem_advise;
typedef UINT32 DVdevice;
typedef UINT64 DVdeviceptr;
typedef drvError_t DVresult;

struct DMA_OFFSET_ADDR {
    UINT64 offset;
    UINT32 devid;
};

struct DMA_PHY_ADDR {
    void *src;   /**< src addr(physical addr) */
    void *dst;   /**< dst addr(physical addr) */
    UINT32 len;  /**< length */
    UINT8 flag;  /**< Flag=0 Non-chain, SRC and DST are physical addresses, can be directly DMA copy operations*/
                 /**< Flag=1 chain, SRC is the address of the dma list and can be used for direct dma copy operations*/
    void *priv;
};

struct DMA_ADDR {
    union {
        struct DMA_PHY_ADDR phyAddr;
        struct DMA_OFFSET_ADDR offsetAddr;
    };
    UINT32 fixed_size; /**< The actual conversion size, due to hardware limitations, is 64M at most */
    UINT32 virt_id; /**< store logic id for destroy addr */
};

#define DV_MEM_LOCAL_HOST 0x0001
#define DV_MEM_MANAGE_LOCKED_DEVICE 0x0002
#define DV_MEM_MANAGE_LOCKED_HOST 0x0004
#define DV_MEM_MANAGE 0x0008

struct DVattribute {
    /**< DV_MEM_LOCAL_HOST : local host memory. alloced by malloc or drvMemAllocHost */
    /**< DV_MEM_MANAGE_LOCKED_DEVICE : manage memory & locked device */
    /**< DV_MEM_MANAGE_LOCKED_HOST : manage memory & locked host */
    /**< DV_MEM_MANAGE : manage memory & no locked */
    int isManaged;
    /**< isDevMem : 1-mapped device,real ddr in device;0-no ddr in device*/
    /**< isHostMem : 1-mapped host,real ddr in host;0-no ddr in host*/
    int isDevMem;
    int isHostMem;
    UINT32 devId;
    UINT32 pageSize;
};

#define DV_LOCK_HOST 0x0001
#define DV_LOCK_DEVICE 0x0002
#define DV_UNLOCK 0
/**
* @ingroup driver-stub
* @brief Set memory allocation strategy for memory range segments
* @attention
* 1. Ensure that the device id corresponding to the execution CPU where the thread calling the interface is consistent
* with the dev_id in the parameter (ids all start from 0), that is, the interface only supports setting the allocation
* strategy of the memory range segment on the device where the current execution thread is located;
* 2. Currently only offline scenarios are supported;
* @param [in] devPtr  Unsigned long, Start address of memory range segment
* @param [in] len  Unsigned long, the size of the memory range segment
* @param [in] type  Signed shaping, the type of memory range segment, currently only supports three: 0 (Local DDR),
* 1 (Local HBM), 2 (Cross HBM)
* @param [in] dev_id  device id
* @return DRV_ERROR_INVALID_VALUE : parameter error, unsupported type or board node number error
* @return DRV_ERROR_INVALID_DEVICE : device id error
* @return DRV_ERROR_MBIND_FAIL : internel memory bind fail
* @return DRV_ERROR_NONE : success
 */
DV_OFFLINE DVresult drvMbindHbm(DVdeviceptr devPtr, size_t len, unsigned int type, uint32_t dev_id);
/**
* @ingroup driver-stub
* @brief Get the current large page memory information on the device: total size, remaining size
* @attention null
* @param [in] device  Unsigned shaping, device id, this value is not used in offline scenarios
* @param [out] free  Remaining free large page memory size
* @param [out] total  Total large page memory size
* @return DRV_ERROR_INVALID_HANDLE : parameter error, addr is zero
* @return DRV_ERROR_FILE_OPS : internel error, open file fail
* @return DRV_ERROR_IOCRL_FAIL : internel error, ioctl fail
* @return DRV_ERROR_NONE : success
*/
DV_OFF_ONLINE DVresult drvGetMemInfo(DVdevice device, size_t *free, size_t *total);
/**
* @ingroup driver-stub
* @brief Load a certain length of data to the specified position of L2BUF
* @attention: offline mode:
* 1. User guarantees *vPtr points to the correct L2BUFF mapped virtual space starting address
* 2. The current interface only takes effect the first time it is successfully invoked throughout the OS life cycle
* @param [in] deviceId  Unsigned shaping, device id, this value is not used in offline scenarios
* @param [in] program  void pointer, a program to be loaded
* @param [in] offset  Offset value of the position to be loaded from the L2BUF starting address, in bytes
* @param [in] ByteCount  The length of the program to be loaded
* @param [out] vPtr  The start address of L2BUF is used as the input. After the load is completed, the address of
* the start position of the load is used as the output
* @return DRV_ERROR_INVALID_VALUE : Parameter error, null pointer, offset exceeds l2buf size,
* copy data exceeds l2buf range, etc
* @return DRV_ERROR_FILE_OPS : Internal error, file operation failed;
* @return DRV_ERROR_IOCRL_FAIL : Internal error, IOCTL operation failed;
* @return DRV_ERROR_INVALID_HANDLE : Internal error, loading program error
* @return DRV_ERROR_NONE : success
*/
DV_OFF_ONLINE DVresult drvLoadProgram(DVdevice deviceId, void *program, unsigned int offset,
    size_t ByteCount, void **vPtr);
/**
* @ingroup driver-stub
* @brief Request SVM memory resources for dvpp:
* 1. The returned address is 2M SIZE_ALIGN
* 2. The returned address-(address + bytesize) The space of the range segment is readable, writable, and executable
* 3. Only supports 4G
* @attention The requested memory cannot be accessed concurrently at Host and Device
* @param [in] bytesize  Applied space size, unit byte
* @param [in] device  Device specified by the requested space
* @param [in] advise  Memory type of the requested space, see the macro definition ADVISE_xxx for details
* @param [in] *pp    Unsigned 64-bit data pointer, output 64-bit address of requested space
* @return DRV_ERROR_INVALID_VALUE : Parameter error, null pointer, size is 0
* @return DRV_ERROR_OUT_OF_MEMORY : Not enough storage
* @return DRV_ERROR_INVALID_HANDLE: Internal setting space permission error
* @return DRV_ERROR_NONE : success
*/
DVresult drvMemAllocForDvppManaged(DVdeviceptr *pp, size_t bytesize, DVdevice device, DVmem_advise advise);
/**
* @ingroup driver-stub
* @brief Free up memory resources
* @attention This interface should be used in conjunction with drvMemAllocForDvppManaged
* @param [in] p  Unsigned 64-bit integer, 64-bit address of the requested space
* @return DRV_ERROR_INVALID_HANDLE : Parameter error, address is 0
* @return DRV_ERROR_NONE : success
*/
DVresult drvMemFreeDvppManaged(DVdeviceptr p);
/**
* @ingroup driver-stub
* @brief Apply for SVM memory resources
* 1. The returned address is SIZE_ALIGN
* 2. The returned address-(address + bytesize) The space of the range segment is readable, writable, and executable
* @attention The requested memory cannot be accessed concurrently at Host and Device
* @param [in] bytesize  Applied space size, unit byte
* @param [out] addr  Unsigned 64-bit data pointer, output 64-bit address of requested space
* @return DRV_ERROR_INVALID_VALUE : Parameter error, null pointer, size is 0
* @return DRV_ERROR_OUT_OF_MEMORY : Not enough storage
* @return DRV_ERROR_INVALID_HANDLE: Internal setting space permission error
* @return DRV_ERROR_NONE : success
*/
DV_OFF_ONLINE DVresult drvMemAllocManaged(DVdeviceptr *addr, size_t bytesize);
/**
* @ingroup driver-stub
* @brief Free up memory resources
* @attention This interface should be used in conjunction with drvMemAllocManaged
* @param [in] addr addr  Unsigned 64-bit integer, 64-bit address of the requested space
* @return DRV_ERROR_INVALID_HANDLE : Internal setting space permission error
* @return DRV_ERROR_NONE : success
*/
DV_OFF_ONLINE DVresult drvMemFreeManaged(DVdeviceptr addr);
/**
* @ingroup driver-stub
* @brief Specifying memory usage
* @attention
* 1. First apply for svm memory, then use this interface to specify attributes, and then access memory data
* 2. The memory management module is not responsible for the length check of ByteCount. Users need to ensure
* that the length is legal
* 3. The devPtr of Advise must be the memory directly applied by calling interface drvMemAllocManaged or
* interface drvMemAllocHugepageManaged. There must be no offset.
* @param [in] devPtr  Unsigned 64-bit integer, the device memory address must be the shared memory requested
* @param [in] count  memory size
* @param [in] advise  Use method, see the macro definition ADVISE_xxx for details
* @param [in] device  Device id
* @return DRV_ERROR_INVALID_HANDLE :  Internal operation error
* @return DRV_ERROR_INVALID_VALUE :  advise invalid
* @return DRV_ERROR_NONE : success
*/
DV_OFF_ONLINE DVresult drvMemAdvise(DVdeviceptr devPtr, size_t count, DVmem_advise advise, DVdevice device);
/**
* @ingroup driver-stub
* @brief Get the corresponding physical address based on the entered virtual address
* @attention
* 1. After applying for memory, you need to call the advise interface to allocate physical memory, and then
* call this interface. That is, the user should ensure that the page table has been established in the space where
* the virtual address is located to ensure that the corresponding physical address is correctly obtained
* @param [in] vptr  Unsigned 64-bit integer, the device memory address must be the shared memory requested
* @param [out] *pptr Unsigned 64-bit integer. The corresponding physical address is returned. The value is valid
* when the return is successful
* @return DRV_ERROR_INVALID_HANDLE : parameter error, pointer is empty, addr is zero.
* @return DRV_ERROR_FILE_OPS : internel error, file operation failed.
* @return DRV_ERROR_IOCRL_FAIL : Internal error, IOCTL operation failed
* @return DRV_ERROR_NONE : success
*/
DV_OFF_ONLINE DVresult drvMemAddressTranslate(DVdeviceptr vptr, UINT64 *pptr);
/**
* @ingroup driver-stub
* @brief Get the TTBR and substreamid of the current process
* @attention Can be called multiple times, it is recommended that Runtime be called once; the result record can be
* saved and can be used next time in this process
* @param [in] device  Unsigned shaping, device id, this value is not used in offline scenarios
* @param [out] *SSID  Returns the SubStreamid of the current process
* @return DRV_ERROR_INVALID_VALUE : Parameter error, pointer is empty
* @return DRV_ERROR_FILE_OPS : Internal error, file operation failed
* @return DRV_ERROR_IOCRL_FAIL : Internal error, IOCTL operation failed
* @return DRV_ERROR_NONE : success
*/
DV_OFF_ONLINE DVresult drvMemSmmuQuery(DVdevice device, UINT32 *SSID);
/**
* @ingroup driver-stub
* @brief Map the L2buff to the process address space, establish page table, and obtain the starting virtual address
* of the current process L2buff and the corresponding PTE
* @attention 1. It can only be called once during initialization, and a page will be created internally, and multiple
* calls are prohibited; it is released when the process exits.
* @param [in] device  Unsigned shaping, device id, this value is not used in offline scenarios
* @param [out] l2buff  Double pointer, returns a pointer to the starting virtual address of the L2buff
* @param [out] pte  Returns the page table entry corresponding to the starting address space
* @return DRV_ERROR_INVALID_HANDLE :  Parameter error, pointer is empty
* @return DRV_ERROR_FILE_OPS :  Internal error, file operation failed
* @return DRV_ERROR_IOCRL_FAIL :  Internal error, IOCTL operation failed
* @return DRV_ERROR_NONE : success
*/
DV_OFF_ONLINE DVresult drvMemAllocL2buffAddr(DVdevice device, void **l2buff, UINT64 *pte);
/**
* @ingroup driver-stub
* @brief Release L2buff address space, should be used in conjunction with drvMemAllocL2buffAddr
* @attention null
* @param [in] device  Unsigned shaping, device id, this value is not used in offline scenarios
* @param [in] l2buff  Pointer to the starting virtual address space of L2buff
* @return DRV_ERROR_INVALID_HANDLE : Parameter error, pointer is empty
* @return DRV_ERROR_NONE : success
*/
DV_OFF_ONLINE DVresult drvMemReleaseL2buffAddr(DVdevice device, void *l2buff);
/**
* @ingroup driver-stub
* @brief Apply for virtual memory space
* The application for virtual memory space has the following characteristics:
* 1. The returned address is SIZE_ALIGN
* 2. The returned address-(address + bytesize) The space of the range segment is readable, writable, and executable
* 3. Currently, we only apply for virtual address space, and do not set up a page table; Make sure to use large page
* memory when you build a page table later
* @attention
*  1. Ensure that there are enough large pages configured in the environment, otherwise the application will fail
*   Configuration method:
*   Overall configuration of large pages: echo num > /sys/kernel/mm/hugepages/hugepages-2048kB/nr_hugepages
*   Configure the number of large pages on a node:
*   echo num > /sys/devices/system/node/nodex/hugepages/hugepages-2048kB/nr_hugepages
*   Where num refers to the number of large pages to be configured, and nodex represents the nodex
* online:
*  1. The requested memory cannot be accessed concurrently at Host and Device.
*  2. The page table on the Host side is still managed by small page table, and the page table on the Device side is
* managed by the beat page table.
* @param [in] bytesize  The amount of memory space requested
* @param [out] pp  Unsigned 64-bit data pointer, returns the first address of the requested virtual memory space
* @return DRV_ERROR_INVALID_HANDLE : Parameter error, pointer is empty
* @return DRV_ERROR_FILE_OPS : Internal error, file operation failed
* @return DRV_ERROR_NONE : success
*/
DV_OFF_ONLINE DVresult drvMemAllocHugePageManaged(DVdeviceptr *pp, size_t bytesize);
/**
* @ingroup driver-stub
* @brief Free large page memory space. This interface should be used in conjunction with the
* drvMemAllocHugePageManaged interface.
* @attention null
* @param [in] addr  Symbol 64-bit integer, the address of the large page memory space to be released
* @return DRV_ERROR_INVALID_VALUE : Parameter error, address is 0 or inconsistent with application return
* @return DRV_ERROR_NONE : success
*/
DV_OFF_ONLINE DVresult drvMemFreeHugePageManaged(DVdeviceptr addr);
DV_OFF_ONLINE DVresult drvMemAllocPhy32PageManaged(DVdevice device, DVdeviceptr *pp, size_t bytesize);
DV_OFF_ONLINE DVresult drvMemFreePhy32PageManaged(DVdeviceptr addr);
/**
* @ingroup driver-stub
* @brief Set the initial memory value according to 8bits (device physical address, unified virtual address
* are supported)
* @attention
*  1. Make sure that the destination buffer can store at least num characters.
*  2. The interface supports processing data larger than 2G
* online:
*  1. The memory to be initialized needs to be on the Host or both on the Device side
*  2. The memory management module is not responsible for the length check of ByteCount. Users need to ensure
*  that the length is legal.
* @param [in] dst  Unsigned 64-bit integer, memory address to be initialized
* @param [in] destMax  The maximum number of valid initial memory values that can be set
* @param [in] value  8-bit unsigned, initial value set
* @param [in] num  Set the initial length of the memory space in bytes
* @return DRV_ERROR_NONE : success
* @return DRV_ERROR_INVALID_VALUE : The destination address is 0 and the number of values is 0
* @return DRV_ERROR_INVALID_HANDLE : Internal error, setting failed
*/
DV_OFF_ONLINE DVresult drvMemsetD8(DVdeviceptr dst, size_t destMax, UINT8 value, size_t num);
/**
* @ingroup driver-stub
* @brief Copy the data in the source buffer to the destination buffer synchronously
* @attention
* 1. The destination buffer must have enough space to store the contents of the source buffer to be copied.
* 2. (offline) This interface cannot process data larger than 2G
* @param [in] dst  Unsigned 64-bit integer, memory address to be initialized
* @param [in] destMax  The maximum number of valid initial memory values that can be set
* @param [in] value  16-bit unsigned, initial value set
* @param [in] num  Set the number of memory space initial values
* @return DRV_ERROR_NONE : success
* @return DRV_ERROR_INVALID_HANDLE : Internal error, copy failed
*/
DV_OFF_ONLINE DVresult drvMemcpy(DVdeviceptr dst, size_t destMax, DVdeviceptr src, size_t ByteCount);
/**
* @ingroup driver-stub
* @brief
* 1. (online) Apply for host-side pin memory and initialize it to 0
* 2. (offline) directly call malloc to apply for host-side memory
* @attention
* 1. (online) The applied memory has been allocated physical pages and has been pinned.
* The physical pages will not be migrated or swapped out.
* 2. (online) Application memory cannot be used on the device, it needs to be copied to the device explicitly.
* @param [in] pp  double pointer, returns a pointer to the requested memory space
* @param [in] bytesize  Unsigned 64-bit integer, the size of the requested space
* @return DRV_ERROR_NONE : success
* @return DRV_ERROR_MALLOC_FAIL :  Memory application failed
*/
DV_OFF_ONLINE DVresult drvMemAllocHost(void **pp, size_t bytesize);
/**
* @ingroup driver-stub
* @brief Release host-side memory resources
* @attention null
* @param [in] p  Pointer to the memory space to be released
* @return DRV_ERROR_NONE : success
* @return DRV_ERROR_INVALID_VALUE : Pointer is empty
*/
DV_OFF_ONLINE DVresult drvMemFreeHost(void *p);

DV_ONLINE DVresult halMemAllocDevice(DVdeviceptr *pp, size_t bytesize, DVmem_advise advise, DVdevice device);
DV_ONLINE DVresult halMemFreeDevice(DVdeviceptr p);
/**
* @ingroup driver-stub
* @brief Converts the address to the physical address for DMA copy, including H2D, D2H, and D2D asynchronous
* copy interfaces.
* @attention The memory management module does not verify the length of ByteCount. You need to ensure that
* the length is valid
* @param [in] pSrc Source address (virtual address)
* @param [in] pDst Destination address (virtual address)
* @param [in] len length
* @param [out] *dmaAddr see struct DMA_ADDR.
* 1. Flag= 0: non-chain, SRC and DST are physical addresses, can directly conduct DMA copy operation
* 2. Flag= 1: chain, SRC is the address of dma chain list, can directly conduct dma copy operation;
* @return DRV_ERROR_NONE : success
* @return DV_ERROR_XXX : convert fail
*/
DV_ONLINE DVresult drvMemConvertAddr(DVdeviceptr pSrc, DVdeviceptr pDst, UINT32 len, struct DMA_ADDR *dmaAddr);
/**
* @ingroup driver-stub
* @brief Releases the physical address information of the DMA copy
* @attention Available online, not offline. This interface is used with drvMemConvertAddr.
* @param [in] struct DMA_ADDR *ptr : Information to be released
* @return DRV_ERROR_NONE : success
* @return DV_ERROR_XXX : destyoy fail
*/
DV_ONLINE DVresult drvMemDestroyAddr(struct DMA_ADDR *ptr);
/**
* @ingroup driver-stub
* @brief Prefetch data to the memory of the specified device (uniformly shared virtual address)
* @attention Available online, not offline.
* First apply for svm memory, then fill the data, and then prefetch to the target device side.
* The output buffer scenario uses advice to allocate physical memory to the device side.
* If the host does not create a page table, this interface fails.
* The memory management module is not responsible for the length check of ByteCount,
* users need to ensure that the length is legal.
* @param [in] devPtr Memory to prefetch
* @param [in] len Prefetch size
* @param [in] device Destination device for prefetching data
* @return DRV_ERROR_NONE : success
* @return DV_ERROR_XXX : prefetch fail
*/
DV_ONLINE DVresult drvMemPrefetchToDevice(DVdeviceptr devPtr, size_t len, DVdevice device);
/**
* @ingroup driver-stub
* @brief Create a share corresponding to vptr based on name
* @attention Available online, not offline.
* vptr must be device memory, and must be directly allocated for calling the memory management interface, without offset
* @param [in] vptr Virtual memory to be shared
* @param [in] byte_count User-defined length to be shared
* @param [in] name_len The maximum length of the name array
* @param [out] name  Name used for sharing between processes
* @return DRV_ERROR_NONE : success
* @return DV_ERROR_XXX : create mem handle fail
*/
DV_ONLINE DVresult drvIpcCreateMemHandle(DVdeviceptr vptr, size_t byte_count, char *name, unsigned int name_len);
/**
* @ingroup driver-stub
* @brief Destroy shared memory created by drvIpcCreateMemHandle
* @attention Available online, not offline.
* @param [in] *name Name used for sharing between processes
* @return DRV_ERROR_NONE : success
* @return DV_ERROR_XXX : destroy mem handle fail
*/
DV_ONLINE DVresult drvIpcDestroyMemHandle(const char *name);
/**
* @ingroup driver-stub
* @brief Configure the whitelist of nodes with ipc mem shared memory
* @attention Available online, not offline.
* @param [in] *name Name used for sharing between processes
* @param [in] pid host pid whitelist array
* @param [in] num number of pid arrays
* @return DRV_ERROR_NONE : success
* @return DV_ERROR_XXX : set fail
*/
DV_ONLINE DVresult drvIpcSetPidMemHandle(const char *name, int pid[], int num);
/**
* @ingroup driver-stub
* @brief Open the shared memory corresponding to name, vptr returns the virtual address that can access shared memory
* @attention Available online, not offline.
* @param [in] *name Name used for sharing between processes
* @param [out] *vptr Virtual address with access to shared memory
* @return DRV_ERROR_NONE : success
* @return DV_ERROR_XXX : open fail
*/
DV_ONLINE DVresult drvIpcOpenMemHandle(const char *name, DVdeviceptr *vptr);
/**
* @ingroup driver-stub
* @brief Close the shared memory corresponding to name
* @attention Available online, not offline.
* @param [in] vptr The virtual address that drvIpcOpenMemHandle can access to shared memory
* @return DRV_ERROR_NONE : success
* @return DV_ERROR_XXX : close fail
*/
DV_ONLINE DVresult drvIpcCloseMemHandle(DVdeviceptr vptr);
/**
* @ingroup driver-stub
* @brief Get the properties of the virtual memory, if it is device memory, get the deviceID at the same time
* @attention Available online, not offline.
* @param [in] vptr  Virtual address to be queried
* @param [out] *attr  vptr property information corresponding to the page
* @return DRV_ERROR_NONE : success
* @return DV_ERROR_XXX : get fail
*/
DV_ONLINE DVresult drvMemGetAttribute(DVdeviceptr vptr, struct DVattribute *attr);
/**
* @ingroup driver-stub
* @brief Lock the specified va on the host or device side
* @attention Available online, not offline.
* @param [in] devPtr Address to be locked
* @param [in] lockType Lock to host or device
* @param [in] device The specified device id when locking to the device side
* @return DRV_ERROR_NONE : success
* @return DV_ERROR_XXX : lock fail
*/
DV_ONLINE DVresult drvMemLock(DVdeviceptr devPtr, unsigned int lockType, DVdevice device);
/**
* @ingroup driver-stub
* @brief unLock the specified va on the host or device side
* @attention Available online, not offline.
* @param [in] devPtr Address to be unlocked
* @return DRV_ERROR_NONE : success
* @return DV_ERROR_XXX : unlock fail
*/
DV_ONLINE DVresult drvMemUnLock(DVdeviceptr devPtr);
/**
* @ingroup driver-stub
* @brief Initialize Device Memory
* @attention Must have a paired hostpid
* @param [in] hostpid Paired host side pid
* @return DRV_ERROR_NONE : success
* @return DV_ERROR_XXX : init fail
*/
DV_ONLINE DVresult drvMemInitSvmDevice(int hostpid);
/**
* @ingroup driver-stub
* @brief Device mounts memory daemon background thread
* @attention Called by matrix after device os starts
* @return DRV_ERROR_NONE : success
* @return DV_ERROR_XXX : setup fail
*/
DV_ONLINE int devmm_daemon_setup(void);
/**
* @ingroup driver-stub
* @brief Open the memory management module interface and initialize related information
* @attention null
* @param [in] devid  Device id
* @param [in] devfd  Device file handle
* @return DRV_ERROR_NONE : success
* @return DV_ERROR_XXX : open fail
*/
DV_ONLINE int drvMemDeviceOpen(unsigned int devid, int devfd);
/**
* @ingroup driver-stub
* @brief Close the memory management module interface
* @attention Used with drvMemDeviceOpen.
* @param [in] devid  Device id
* @return DRV_ERROR_NONE  success
* @return DV_ERROR_XXX  close fail
*/
DV_ONLINE int drvMemDeviceClose(unsigned int devid);
/**
* @ingroup driver-stub
* @brief Applying for the Memory with the Execute Permission
* @attention Currently, this interface can be used only for the memory applied by the ion
* @param [in] bytesize Requested byte size
* @param [out] **pp  Level-2 pointer that stores the address of the allocated memory pointer
* @return DRV_ERROR_NONE  success
* @return DV_ERROR_XXX  alloc fail
*/
DV_ONLINE DVresult drvMemAllocProgram(void **pp, size_t bytesize);
/**
* @ingroup driver-stub
* @brief This command is used to set the read count value of the address l2 cache.
* @attention null
* @param [in] addr  Invokes drvMemAllocManaged and creates the address of the page table on the Advise of Device.
* @param [in] size  Space Size
* @param [in] rc Value of read count
* @return DRV_ERROR_NONE : success
* @return DV_ERROR_XXX : set fail
*/
DV_OFF_ONLINE DVresult drvMemSetRc(DVdeviceptr addr, UINT64 size, UINT32 rc);

typedef drvError_t hdcError_t;
typedef void *HDC_CLIENT;
typedef void *HDC_SESSION;
typedef void *HDC_SERVER;
#ifdef __linux
typedef void *HDC_EPOLL;

#define HDC_EPOLL_CTL_ADD 0
#define HDC_EPOLL_CTL_DEL 1

#define HDC_EPOLL_CONN_IN (0x1 << 0)
#define HDC_EPOLL_DATA_IN (0x1 << 1)
#define HDC_EPOLL_FAST_DATA_IN (0x1 << 2)
#define HDC_EPOLL_SESSION_CLOSE (0x1 << 3)

struct drvHdcEvent {
    unsigned int events;
    uintptr_t data;
};

#define HDC_SESSION_RUN_ENV_UNKNOW 0
#define HDC_SESSION_RUN_ENV_HOST 1
#define HDC_SESSION_RUN_ENV_CONTAINER 2
#endif

/**< The HDC interface is dead and blocked by default. Set HDC_FLAG_NOWAIT to be non-blocked */
/**< Set HDC_FLAG_WAIT_TIMEOUT to timeout after blocking for a period of time. HDC_FLAG_WAIT_TIMEOUT */
/**< takes precedence over HDC_FLAG_NOWAIT */
#define HDC_FLAG_NOWAIT (0x1 << 0)        /**< Occupy bit0 */
#define HDC_FLAG_WAIT_TIMEOUT (0x1 << 1)  /**< Occupy bit1 */
#define HDC_FLAG_MAP_VA32BIT (0x1 << 1)   /**< Use low 32bit memory */
#define HDC_FLAG_MAP_HUGE (0x1 << 2)      /**< Using large pages */

#ifdef __linux
#define DLLEXPORT
#else
#define DLLEXPORT _declspec(dllexport)
#endif
enum drvHdcServiceType {
    HDC_SERVICE_TYPE_DMP = 0,
    HDC_SERVICE_TYPE_MATRIX = 1,
    HDC_SERVICE_TYPE_IDE1 = 2,
    HDC_SERVICE_TYPE_FILE_TRANS = 3,
    HDC_SERVICE_TYPE_IDE2 = 4,
    HDC_SERVICE_TYPE_LOG = 5,
    HDC_SERVICE_TYPE_RDMA = 6,
    HDC_SERVICE_TYPE_BBOX = 7,
    HDC_SERVICE_TYPE_FRAMEWORK = 8,
    HDC_SERVICE_TYPE_TSD = 9,
    HDC_SERVICE_TYPE_TDT = 10,
    HDC_SERVICE_TYPE_DEBUG = 11,
    HDC_SERVICE_TYPE_USER1 = 12,
    HDC_SERVICE_TYPE_USER2 = 13,
    HDC_SERVICE_TYPE_USER3 = 14, /**< used by test */
    HDC_SERVICE_TYPE_USER4 = 15,
    HDC_SERVICE_TYPE_MAX
};

enum drvHdcChanType {
    HDC_CHAN_TYPE_SOCKET = 0,
    HDC_CHAN_TYPE_PCIE,
    HDC_CHAN_TYPE_MAX
};

enum drvHdcMemType {
    HDC_MEM_TYPE_TX_DATA = 0,
    HDC_MEM_TYPE_TX_CTRL = 1,
    HDC_MEM_TYPE_RX_DATA = 2,
    HDC_MEM_TYPE_RX_CTRL = 3,
    HDC_MEM_TYPE_DVPP = 4,
    HDC_MEM_TYPE_MAX
};

#define HDC_SESSION_MEM_MAX_NUM 100

struct drvHdcFastSendMsg {
    unsigned long long srcDataAddr;
    unsigned long long dstDataAddr;
    unsigned long long srcCtrlAddr;
    unsigned long long dstCtrlAddr;
    unsigned int dataLen;
    unsigned int ctrlLen;
};

struct drvHdcFastRecvMsg {
    unsigned long long dataAddr;
    unsigned long long ctrlAddr;
    unsigned int dataLen;
    unsigned int ctrlLen;
};

struct drvHdcCapacity {
    enum drvHdcChanType chanType;
    unsigned int maxSegment;
};

struct drvHdcMsgBuf {
    char *pBuf;
    int len;
};

struct drvHdcMsg {
    int count;
    struct drvHdcMsgBuf bufList[0];
};

struct drvHdcProgInfo {
    char name[256];
    int progress;
    long long int send_bytes;
    long long int rate;
    int remain_time;
};

/**
* @ingroup driver-stub
* @brief Before calling the sending interface of HDC, you need to know the size of the sending packet through
* this interface.
* @attention null
* @param [out] *capacity : get the packet size and channel type currently supported by HDC
* @return   0 for success, others for fail
*/
DLLEXPORT hdcError_t drvHdcGetCapacity(struct drvHdcCapacity *capacity);
/**
* @ingroup driver-stub
* @brief Create HDC Client and initialize it based on the passed node and device ID information
* @attention null
* @param [in]  maxSessionNum : The maximum number of sessions currently required by Clinet
* @param [in]  flag : Reserved parameters, currently fixed 0
* @param [out] HDC_CLIENT *client : Created a good HDC Client pointer
* @return   DRV_ERROR_NONE, DRV_ERROR_INVALID_VALUE
*/
DLLEXPORT hdcError_t drvHdcClientCreate(HDC_CLIENT *client, int maxSessionNum, int serviceType, int flag);
/**
* @ingroup driver-stub
* @brief Release HDC Client
* @attention null
* @param [in]  HDC_CLIENT client : HDC Client to be released
* @return   DRV_ERROR_NONE, DRV_ERROR_INVALID_VALUE
*/
DLLEXPORT hdcError_t drvHdcClientDestroy(HDC_CLIENT client);
/**
* @ingroup driver-stub
* @brief Create HDC Session for Host and Device communication
* @attention null
* @param [in]  peer_node : The node number of the node where the Device is located. Currently only 1 node is supported.
* Remote nodes are not supported. You need to pass a fixed value of 0
* @param [in]  peer_devid : Device's uniform ID in the host (number in each node)
* @param [in]  HDC_CLIENT client : HDC Client handle corresponding to the newly created Session
* @param [out] HDC_SESSION *session : Created session
* @return   DRV_ERROR_NONE, DRV_ERROR_INVALID_VALUE
*/
DLLEXPORT hdcError_t drvHdcSessionConnect(int peer_node, int peer_devid, HDC_CLIENT client, HDC_SESSION *session);
/**
* @ingroup driver-stub
* @brief Create and initialize HDC Server
* @attention null
* @param [in]  devid : only support [0, 64)
* @param [in]  serviceType : select server type
* @param [out] HDC_SERVER *server : Created HDC server
* @return   DRV_ERROR_NONE, DRV_ERROR_INVALID_VALUE
*/
DLLEXPORT hdcError_t drvHdcServerCreate(int devid, int serviceType, HDC_SERVER *pServer);
/**
* @ingroup driver-stub
* @brief Release HDC Server
* @attention null
* @param [in]  HDC_SERVER server : HDC server to be released
* @return   DRV_ERROR_NONE, DRV_ERROR_INVALID_VALUE
*/
DLLEXPORT hdcError_t drvHdcServerDestroy(HDC_SERVER server);
/**
* @ingroup driver-stub
* @brief Open HDC Session for communication between Host and Device
* @attention null
* @param [in]  HDC_SERVER server     : HDC server to which the newly created session belongs
* @param [out] HDC_SESSION *session  : Created session
* @return   DRV_ERROR_NONE, DRV_ERROR_INVALID_VALUE
*/
DLLEXPORT hdcError_t drvHdcSessionAccept(HDC_SERVER server, HDC_SESSION *session);
/**
* @ingroup driver-stub
* @brief Close HDC Session for communication between Host and Device
* @attention null
* @param [in]  HDC_SESSION session : Specify in which session to receive data
* @return   DRV_ERROR_NONE, DRV_ERROR_INVALID_VALUE
*/
DLLEXPORT hdcError_t drvHdcSessionClose(HDC_SESSION session);
/**
* @ingroup driver-stub
* @brief Apply for MSG descriptor for sending and receiving
* @attention The user applies for a message descriptor before sending and receiving data, and then releases the
* message descriptor after using it.
* @param [in]  HDC_SESSION session : Specify in which session to receive data
* @param [in]  count : Number of buffers in the message descriptor. Currently only one is supported
* @param [out] struct drvHdcMsg *ppMsg : Message descriptor pointer, used to store the send and receive buffer
* address and length
* @return   DRV_ERROR_NONE, DRV_ERROR_INVALID_VALUE
*/
DLLEXPORT hdcError_t drvHdcAllocMsg(HDC_SESSION session, struct drvHdcMsg **ppMsg, int count);
/**
* @ingroup driver-stub
* @brief Release MSG descriptor for sending and receiving
* @attention The user applies for a message descriptor before sending and receiving data, and then releases
* the message descriptor after using it.
* @param [in]  struct drvHdcMsg *pMsg   :  Pointer to message descriptor to be released
* @return   DRV_ERROR_NONE, DRV_ERROR_INVALID_VALUE
*/
DLLEXPORT hdcError_t drvHdcFreeMsg(struct drvHdcMsg *msg);
/**
* @ingroup driver-stub
* @brief Reuse MSG descriptor
* @attention This interface will clear the Buffer pointer in the message descriptor. For offline scenarios, Reuse
* will release the original Buffer. For online scenarios, Reuse will not release the original Buffer (the upper
* layer calls the device memory management interface on the Host to release it).
* @param [in]  struct drvHdcMsg *pMsg : The pointer of message need to Reuse
* @return   DRV_ERROR_NONE, DRV_ERROR_INVALID_VALUE
*/
DLLEXPORT hdcError_t drvHdcReuseMsg(struct drvHdcMsg *msg);
/**
* @ingroup driver-stub
* @brief Add the receiving and sending buffer to the MSG descriptor
* @attention The user applies for a message descriptor before sending and receiving data, and then releases the
* message descriptor after using it.
* @param [in]  struct drvHdcMsg *pMsg : The pointer of the message need to be operated
* @param [in]  char *pBuf : Buffer pointer to be added
* @param [in]  int len : The effective data length of the buffer to be added
* @return   DRV_ERROR_NONE, DRV_ERROR_INVALID_VALUE
*/
DLLEXPORT hdcError_t drvHdcAddMsgBuffer(struct drvHdcMsg *msg, char *pBuf, int len);
/**
* @ingroup driver-stub
* @brief Add MSG descriptor to send buffer
* @attention null
* @param [in]  struct drvHdcMsg *pMsg : Pointer to the message descriptor to be operated on
* @param [in]  int index              : Need to get the first few buffers, currently only supports one, fixed to 0
* @param [out] char **ppBuf           : Obtained Buffer pointer
* @param [out] int *pLen              : Buffer effective data length obtained
* @return   DRV_ERROR_NONE, DRV_ERROR_INVALID_VALUE
*/
DLLEXPORT hdcError_t drvHdcGetMsgBuffer(struct drvHdcMsg *msg, int index, char **pBuf, int *pLen);
/**
* @ingroup driver-stub
* @brief Receive data based on HDC Session
* @attention For the online scenario, the interface will parse the message sent by the peer, obtain the data buffer
* address and length, save it in the message descriptor, and return it to the upper layer. For the offline scenario,
* the interface applies for a new buffer to store the received data, saves the requested buffer and the length of the
* received data in the message descriptor, and returns it to the upper layer.
* @param [in]  HDC_SESSION session   : Specify in which session to receive data
* @param [in]  int bufLen            : The length of each receive buffer in bytes (only meaningful when offline)
* @param [in]  int flag              : Fixed 0
* @param [out] struct drvHdcMsg *msg : Descriptor pointer for receiving messages
* @param [out] int *recvBufCount      : The number of buffers that actually received the data
* @return   DRV_ERROR_NONE, DRV_ERROR_INVALID_VALUE
*/
DLLEXPORT hdcError_t drvHdcRecv(HDC_SESSION session, struct drvHdcMsg *msg, int bufLen, int flag, int *recvBufCount);
/**
* @ingroup driver-stub
* @brief Block waiting for the peer to send data and receive the length of the sent packet
* @attention null
* @param [in]  HDC_SESSION session : session
* @param [in]  int *msgLen         : Data length
* @param [in]  int flag            : Flag, 0 dead wait, HDC_FLAG_NOWAIT non-blocking, HDC_FLAG_WAIT_TIMEOUT
* blocking timeout
* @return   DRV_ERROR_NONE, DRV_ERROR_INVALID_VALUE
*/
DLLEXPORT hdcError_t drvHdcRecvPeek(HDC_SESSION session, int *msgLen, int flag);
/**
* @ingroup driver-stub
* @brief Receive data over normal channel, Save the received data to the upper layer buffer pBuf
* @attention null
* @param [in]  HDC_SESSION session : session
* @param [in]  char *pBuf     : Receive data buf
* @param [in]  int bufLen     : Received data buf length
* @param [out] int *msgLen    : Received data buf length
* @return   DRV_ERROR_NONE, DRV_ERROR_INVALID_VALUE
*/
DLLEXPORT hdcError_t drvHdcRecvBuf(HDC_SESSION session, char *pBuf, int bufLen, int *msgLen);
/**
* @ingroup driver-stub
* @brief Send data based on HDC Session
* @attention For the online scenario, this interface encapsulates the buffer address and length passed down
* from the upper layer into a message and sends it to the peer. For the offline scenario, the interface applies
* for a new buffer to store the received data, saves the requested buffer and the length of the received data in
* the message descriptor, and returns it to the upper layer.
* @param [in]  HDC_SESSION session    : Specify in which session to send data
* @param [in]  struct drvHdcMsg *msg : Descriptor pointer for sending messages. The maximum sending length
* must be obtained through the drvHdcGetCapacity function
* @param [in]  int flag               : Reserved parameter, currently fixed 0
* @param [in]  unsigned int timeout   : Allow time for send timeout determined by user mode
* @return   DRV_ERROR_NONE, DRV_ERROR_INVALID_VALUE
*/
DLLEXPORT hdcError_t drvHdcSend(HDC_SESSION session, struct drvHdcMsg *msg, int flag);
/**
* @ingroup driver-stub
* @brief Send data based on HDC Session
* @attention Like drvHdcSend function, there is an additional parameter timeout to set the session timeout
* @param [in]  HDC_SESSION session    : Specify in which session to send data
* @param [in]  struct drvHdcMsg *msg : Descriptor pointer for sending messages. The maximum sending length must
* be obtained through the drvHdcGetCapacity function
* @param [in]  int flag               : Reserved parameter, currently fixed 0
* @param [in]  unsigned int timeout   : Allow time for send timeout determined by user mode
* @return   DRV_ERROR_NONE, DRV_ERROR_INVALID_VALUE
*/
DLLEXPORT hdcError_t drvHdcSendEx(HDC_SESSION session, struct drvHdcMsg *msg, int flag, unsigned int timeout);
/**
* @ingroup driver-stub
* @brief Set session and process affinity
* @attention If the interface is not called after the session is created, after the process is abnormal, if the
* upper-layer application does not perform exception handling, HDC cannot detect and release the corresponding
* session resources.
* @param [in]  HDC_SESSION session    :    Specified session
* @return   DRV_ERROR_NONE, DRV_ERROR_INVALID_VALUE
*/
DLLEXPORT hdcError_t drvHdcSetSessionReference(HDC_SESSION session);
/**
* @ingroup driver-stub
* @brief Get the base trusted path sent to the specified node device, get trusted path, used to combine dst_path
* parameters of drvHdcSendFile
* @attention host call is valid, used to obtain the basic trusted path sent to the device side using the drvHdcSendFile
* interface
* @param [in]  int peer_node         	:	Node number of the node where the Device is located
* @param [in]  int peer_devid         :	Device's unified ID in the host
* @param [in]  unsigned int path_len	:	base_path space size
* @param [out] char *base_path		:	Obtained trusted path
* @return   DRV_ERROR_NONE, DRV_ERROR_INVALID_VALUE
*/
DLLEXPORT hdcError_t drvHdcGetTrustedBasePath(int peer_node, int peer_devid, char *base_path, unsigned int path_len);
/**
* @ingroup driver-stub
* @brief Send file to the specified path on the specified device
* @attention null
* @param [in]  int peer_node        :	Node number of the node where the Device is located
* @param [in]  int peer_devid       :	Device's unified ID in the host
* @param [in]  const char *file		:	Specify the file name of the sent file
* @param [in]  const char *dst_path	:	Specifies the path to send the file to the receiver. If the path is directory,
* the file name remains unchanged after it is sent to the peer; otherwise, the file name is changed to the part of the
* path after the file is sent to the receiver.
* @param [out] void (*progress_notifier)(struct drvHdcProgInfo *) :	  Specify the user's callback handler function;
* when progress of the file transfer increases by at least one percent,file transfer protocol will call this interface.
* @return   DRV_ERROR_NONE, DRV_ERROR_INVALID_VALUE
*/
DLLEXPORT hdcError_t drvHdcSendFile(int peer_node, int peer_devid, const char *file, const char *dst_path,
                                    void (*progress_notifier)(struct drvHdcProgInfo *));
/**
* @ingroup driver-stub
* @brief Request to release memory
* @attention Call the kernel function to apply for physical memory. If the continuous physical memory is insufficient,
* it will fail. If it is specified in the 4G range used by dvpp, the application may fail.
* @param [in]  enum drvHdcMemType mem_type  : Memory type, default is 0
* @param [in]  void * addr : Specify application start address, default is NULL
* @param [in]  unsigned int len : length
* @param [in]  unsigned int align  : The address returned by the application is aligned by align. Currently,
* only 4k is a common multiple
* @param [in]  unsigned int flag : Memory application flag. Low 4G / Large Page / normal, only valid on the devic side
* @param [in]  int devid : Device id
* @return   DRV_ERROR_NONE, DRV_ERROR_INVALID_VALUE
*/
DLLEXPORT void *drvHdcMallocEx(enum drvHdcMemType mem_type, void *addr, unsigned int align, unsigned int len, int devid,
                               unsigned int flag);
/**
* @ingroup driver-stub
* @brief Release memory
* @attention null
* @param [in]  enum drvHdcMemType mem_type  : Memory type
* @param [in]  void *buf : Applied memory address
* @return   DRV_ERROR_NONE, DRV_ERROR_INVALID_VALUE
*/
DLLEXPORT hdcError_t drvHdcFreeEx(enum drvHdcMemType mem_type, void *buf);
/**
* @ingroup driver-stub
* @brief Map DMA address
* @attention null
* @param [in]  enum drvHdcMemType mem_type   : Memory type
* @param [in]  void *buf : Applied memory address
* @param [in]  int devid : Device id
* @return   DRV_ERROR_NONE, DRV_ERROR_INVALID_VALUE
*/
DLLEXPORT hdcError_t drvHdcDmaMap(enum drvHdcMemType mem_type, void *buf, int devid);
/**
* @ingroup driver-stub
* @brief unMap DMA address
* @attention null
* @param [in]  enum drvHdcMemType mem_type   : Memory type
* @param [in]  void *buf : Applied memory address
* @return   DRV_ERROR_NONE, DRV_ERROR_INVALID_VALUE
*/
DLLEXPORT hdcError_t drvHdcDmaUnMap(enum drvHdcMemType mem_type, void *buf);
/**
* @ingroup driver-stub
* @brief ReMap DMA address
* @attention null
* @param [in]  enum drvHdcMemType mem_type   : Memory type
* @param [in]  void *buf : Applied memory address
* @param [in]  int devid : Device id
* @return   DRV_ERROR_NONE, DRV_ERROR_INVALID_VALUE
*/
DLLEXPORT hdcError_t drvHdcDmaReMap(enum drvHdcMemType mem_type, void *buf, int devid);
/**
* @ingroup driver-stub
* @brief Session copy-free fast sending interface, applications need to apply for memory through hdc in advance
* @attention After send function returns,src address cannot be reused directly. It must wait for peer to receive it.
* @param [in]  HDC_SESSION session    : Specify in which session
* @param [in]  msg : Send and receive information
* @param [in]  int flag : Fill in 0 default blocking, HDC_FLAG_NOWAIT set non-blocking
* @return   DRV_ERROR_NONE, DRV_ERROR_INVALID_VALUE
*/
DLLEXPORT hdcError_t drvHdcFastSend(HDC_SESSION session, struct drvHdcFastSendMsg msg, int flag);
/**
* @ingroup driver-stub
* @brief Session copy-free fast sending interface, applications need to apply for memory through hdc in advance,
* As with the drvHdcFastSend function, there is an extra timeout parameter to set the session timeout time.
* @attention After the send function returns, the src address cannot be reused directly. It must wait for the
* peer to receive it
* @param [in]  HDC_SESSION session    : Specify in which session
* @param [in]  msg : Send and receive information
* @param [in]  int flag : Fill in 0 default blocking, HDC_FLAG_NOWAIT set non-blocking
* @param [in]  unsigned int timeout   : Allow user-defined send timeout
* @return   DRV_ERROR_NONE, DRV_ERROR_INVALID_VALUE
*/
DLLEXPORT hdcError_t drvHdcFastSendEx(HDC_SESSION session, struct drvHdcFastSendMsg msg, int flag,
                                      unsigned int timeout);
/**
* @ingroup driver-stub
* @brief Session copy-free fast sending interface, applications need to apply for memory through hdc in advance
* @attention Need to apply for memory through hdc in advance. And after the send function returns, the src address
* cannot be reused directly. It must wait for the peer to receive it.
* @param [in]  HDC_SESSION session    : Specify in which session
* @param [in]  msg : Send and receive information
* @param [in]  int flag : Fill in 0 default blocking, HDC_FLAG_NOWAIT set non-blocking
* @return   DRV_ERROR_NONE, DRV_ERROR_INVALID_VALUE
*/
DLLEXPORT hdcError_t drvHdcFastRecv(HDC_SESSION session, struct drvHdcFastRecvMsg *msg, int flag);

/* hdc epoll func */
/**
* @ingroup driver-stub
* @brief HDC epoll create interface
* @attention null
* @param [in]  int size    : Specify the number of file handles to listen on
* @param [out]  HDC_EPOLL *epoll : Returns the supervised epoll handle
* @return   DRV_ERROR_NONE, DRV_ERROR_INVALID_VALUE
*/
DLLEXPORT drvError_t drvHdcEpollCreate(int size, HDC_EPOLL *epoll);
/**
* @ingroup driver-stub
* @brief close HDC epoll interface
* @attention null
* @param [out]  HDC_EPOLL *epoll : Returns the supervised epoll handle
* @return   DRV_ERROR_NONE, DRV_ERROR_INVALID_VALUE
*/
DLLEXPORT drvError_t drvHdcEpollClose(HDC_EPOLL epoll);
/**
* @ingroup driver-stub
* @brief HDC epoll control interface
* @attention null
* @param [in]  HDC_EPOLL *epoll : Specify the created epoll handle
* @param [in]  int op : Listen event operation type
* @param [in]  void *target : Specify to add / remove resource topics
* @param [in]  struct drvHdcEvent *event : Used with target, HDC_EPOLL_CONN_IN Used with HDC_SERVER to monitor whether
* there is a new connection; HDC_EPOLL_DATA_IN Cooperate with HDC_SESSION to monitor data entry of common channels
; HDC_EPOLL_FAST_DATA_IN Cooperate with HDC_SESSION to monitor fast channel data entry
* @return   DRV_ERROR_NONE, DRV_ERROR_INVALID_VALUE
*/
DLLEXPORT drvError_t drvHdcEpollCtl(HDC_EPOLL epoll, int op, void *target, struct drvHdcEvent *event);
/**
* @ingroup driver-stub
* @brief close HDC epoll interface
* @attention null
* @param [in]  HDC_EPOLL *epoll : Specify the created epoll handle
* @param [in]  struct drvHdcEvent *events : Returns the triggered event
* @param [in]  int maxevents : Specify the maximum number of events returned
* @param [in]  int timeout : Set timeout
* @param [in]  int *eventnum : Returns the number of valid events
* @return   DRV_ERROR_NONE, DRV_ERROR_INVALID_VALUE
*/
DLLEXPORT drvError_t drvHdcEpollWait(HDC_EPOLL epoll, struct drvHdcEvent *events, int maxevents, int timeout,
                                     int *eventnum);
/**
* @ingroup driver-stub
* @brief Get the environmental information of HOST
* @attention null
* @param [in]  HDC_SESSION session : Specify the query session
* @param [out]  int *runEnv : Returns host environment information
* @return   DRV_ERROR_NONE, DRV_ERROR_INVALID_VALUE
*/
DLLEXPORT drvError_t drvHdcGetSessionRunEnv(HDC_SESSION session, int *runEnv);
/**
* @ingroup driver-stub
* @brief close HDC epoll interface
* @attention null
* @param [int]  HDC_SERVER server : Specifying the queried service
* @param [int]  int *devid : Return the device ID
* @return   DRV_ERROR_NONE, DRV_ERROR_INVALID_VALUE
*/
DLLEXPORT hdcError_t drvHdcGetServerDevId(HDC_SERVER server, int *devid);
/**
* @ingroup driver-stub
* @brief close HDC epoll interface
* @attention null
* @param [int]  HDC_SESSION session : Specify the query session
* @param [int]  int *devid : Return the device ID
* @return   DRV_ERROR_NONE, DRV_ERROR_INVALID_VALUE
*/
DLLEXPORT hdcError_t drvHdcGetSessionDevId(HDC_SESSION session, int *devid);
/**
* @ingroup driver-stub
* @brief Set the timeout waiting time for ordinary sending and receiving and fast sending and receiving of the session,
* the unit is ms
* @attention There is no special need to set the timeout time, the default is 3s, Used with send and recv whose input
* parameters are HDC_FLAG_WAIT_TIMEOUT, called before send and recv
* @param [in]  HDC_SESSION session : Specify in which session
* @param [in]  send_timeout : The timeout time for ordinary sending. The default value is 3000ms. If it is 0, the
* timeout time is the value set last time
* @param [in]  recv_timeout : The timeout time for ordinary reception. The default value is 3000ms. If it is 0, the
* timeout time is the value set last time
* @param [in]  fast_send_timeout : Timeout for fast sending. The default value is 3000ms. If it is 0, the timeout time
* is the last value set
* @param [in]  fast_recv_timeout : The timeout time for fast receiving. The default value is 3000ms. If it is 0, the
* timeout time is the value set last time
* @return   DRV_ERROR_NONE, DRV_ERROR_INVALID_VALUE
*/
DLLEXPORT drvError_t drvHdcSetSessionTimeOut(HDC_SESSION session, unsigned int send_timeout,
    unsigned int recv_timeout, unsigned int fast_send_timeout, unsigned int fast_recv_timeout);
/**
* @ingroup driver-stub
* @brief Get the uid and euid values of the session initiated by the client and whether they have root privileges,
* Get the uid, euid, and root permissions of the process in which the session is located
* @attention null
* @param [in]  HDC_SESSION session : Specify in which session
* @param [out] unsigned int *uid   : client session creator uid
* @param [out] unsigned int *euid  : client session creator euid
* @param [out] int *root_privilege : Whether the client session creator has root privileges
* @return   DRV_ERROR_NONE, DRV_ERROR_INVALID_VALUE
*/
DLLEXPORT hdcError_t drvHdcGetSessionUid(HDC_SESSION session, unsigned int *uid, unsigned int *euid,
    int *root_privilege);
int dsmi_cmd_get_network_device_info(int device_id, const char *inbuf, unsigned int size_in, char *outbuf,
                                     unsigned int *size_out);
enum log_error_code {
    LOG_OK = 0,
    LOG_ERROR = -1,
    LOG_NOT_READY = -2,
};

#define LOG_CHANNEL_TYPE_AICPU (10)
#define LOG_DEVICE_ID_MAX (64)
#define LOG_CHANNEL_NUM_MAX (64)
#define LOG_SLOG_BUF_MAX_SIZE (2 * 1024 * 1024)
#define LOG_DRIVER_NAME "log_drv"
enum log_channel_type {
    LOG_CHANNEL_TYPE_TS = 0,
    LOG_CHANNEL_TYPE_TS_DUMP = 1,
    LOG_CHANNEL_TYPE_LPM3 = 30,
    LOG_CHANNEL_TYPE_IMP = 31,
    LOG_CHANNEL_TYPE_IMU = 32,

    LOG_CHANNEL_TYPE_ISP = 33,

    LOG_CHANNEL_TYPE_SIS = 37,
    LOG_CHANNEL_TYPE_HSM = 38,
    LOG_CHANNEL_TYPE_MAX
};
int log_write_slog(int device_id, const char *in_buf, unsigned int buf_size, int *write_size, unsigned int timeout);
int log_set_level(int device_id, int channel_type, unsigned int log_level);
int log_get_channel_type(int device_id, int *channel_type_set, int *channel_type_num, int set_size);
int log_get_device_id(int *device_id_set, int *device_id_num, int set_size);
int log_read(int device_id, char *buf, unsigned int *size, int timeout);

typedef void *PPC_CLIENT;
typedef void *PPC_SESSION;
typedef void *PPC_SERVER;

struct drvPpcMsgBuf {
    char *pBuf;
    int len;
};

struct drvPpcMsg {
    int count;
    struct drvPpcMsgBuf bufList[1];  /**< 1 just erase pclint warning. here should be 0 */
};

struct ppcMsgHead {
    bool freeBuf;
    struct drvPpcMsg msg;
};
drvError_t drvPpcInit(void);
drvError_t drvPpcUninit(void);
drvError_t drvPpcClientCreate(PPC_CLIENT *client, int maxSessionNum, int flag);
drvError_t drvPpcClientDestroy(PPC_CLIENT client);
drvError_t drvPpcSessionConnect(int server_pid, PPC_SESSION *session);
drvError_t drvPpcSessionDestroy(PPC_SESSION session);
drvError_t drvPpcServerCreate(int server_pid, PPC_SERVER *server);
drvError_t drvPpcServerDestroy(PPC_SERVER server);
drvError_t drvPpcSessionAccept(PPC_SERVER server, PPC_SESSION *session);
drvError_t drvPpcSessionClose(PPC_SESSION session);
drvError_t drvPpcAllocMsg(PPC_SESSION session, struct drvPpcMsg **ppMsg, int count);
drvError_t drvPpcFreeMsg(struct drvPpcMsg *msg);
drvError_t drvPpcReuseMsg(struct drvPpcMsg *msg);
drvError_t drvPpcAddMsgBuffer(struct drvPpcMsg *msg, char *pBuf, int len);
drvError_t drvPpcGetMsgBuffer(struct drvPpcMsg *msg, int index, char **pBuf, int *pLen);
drvError_t drvPpcRecv(PPC_SESSION session, struct drvPpcMsg *msg, int bufLen, int flag, int *recvBufCount);
drvError_t drvPpcSend(PPC_SESSION session, struct drvPpcMsg *msg, int flag);

/**< profile drv user */
#define PROF_DRIVER_NAME "prof_drv"
#define PROF_OK (0)
#define PROF_ERROR (-1)
#define PROF_TIMEOUT (-2)
#define PROF_STARTED_ALREADY (-3)
#define PROF_STOPPED_ALREADY (-4)
#define CHANNEL_NUM 160
#define CHANNEL_HBM (1)
#define CHANNEL_BUS (2)
#define CHANNEL_PCIE (3)
#define CHANNEL_NIC (4)
#define CHANNEL_DMA (5)
#define CHANNEL_DVPP (6)
#define CHANNEL_DDR (7)
#define CHANNEL_LLC (8)
#define CHANNEL_HCCS (9)
#define CHANNEL_TSCPU (10)
#define CHANNEL_AICPU0 (11) /* aicpu: 11---42, reserved */
#define CHANNEL_AICORE (43)
#define CHANNEL_AIV (85)
#define CHANNEL_HWTS_LOG1 (48)   // add for ts1 as hwts channel
#define CHANNEL_HWTS_CNT CHANNEL_AICORE
#define CHANNEL_TSFW (44)   // add for ts0 as tsfw channel
#define CHANNEL_HWTS_LOG (45)   // add for ts0 as hwts channel
#define CHANNEL_KEY_POINT (46)
#define CHANNEL_TSFW_L2 (47)   // add for cloud
#define CHANNEL_TSFW1 (49)   // add for ts1 as tsfw channel
#define CHANNEL_TSCPU_MAX (128)
#define CHANNEL_ROCE (129)
#define CHANNEL_IDS_MAX CHANNEL_NUM

#define PROF_NON_REAL 0
#define PROF_REAL 1
#define DEV_NUM 64
#define PROF_SQ_BUF_LEN 128
#define PROF_SQ_DATA_LEN (PROF_SQ_BUF_LEN - 32)

#define PROF_FILE_NAME_MAX 128
#define PROF_USER_DATA_LEN 128
#define PROF_BUFFER_LEN (10 * 1024)
#define PROF_TS_BUFFER_LEN (1024 * 1024)
#define PROF_TS_BUFFER_LEN_LITE (256 * 1024)

/**< period time / ms  */
#define PROF_PERIOD_MIN 10    /**< 10ms  */
#define PROF_PERIOD_MAX 10000 /**< 10s  */

#ifndef dma_addr_t
typedef unsigned long long dma_addr_t;
#endif

/**< this struct = the one in "prof_drv_dev.h" */
typedef struct prof_poll_info {
    unsigned int device_id;
    unsigned int channel_id;
} prof_poll_info_t;

/**< keep the same with driver */
#if defined(CONFIG_LITE_SEPARATE)
typedef struct prof_ioctl_para {
    unsigned int device_id;
    unsigned int channel_id;
    unsigned int cmd;
    unsigned int channel_type;  /**< for ts and other device */
    unsigned int buf_len;       /**< buffer size */
    unsigned int sample_period; /**< Sampling period */
    unsigned int real_time;     /**< real mode */
    unsigned int ts_data_size;  /**< ts configure data's size */
    int ret_val;
    int timeout;
    int poll_number; /**< channel number */

    char prof_file[PROF_FILE_NAME_MAX]; /**< file path */
    char ts_data[PROF_SQ_BUF_LEN];      /**< ts data's pointer */

    void *out_buf; /**< save return info */
} prof_ioctl_para_t;
#else
typedef struct prof_ioctl_para {
    unsigned int device_id;
    unsigned int channel_id;
    unsigned int cmd;
    unsigned int channel_type;   /**< for ts and other device */
    unsigned int buf_len;        /**< buffer size */
    unsigned int sample_period;  /**< Sampling period */
    unsigned int real_time;      /**< real mode */
    unsigned int ts_data_size;   /**< ts configure data's size */
    unsigned int user_data_size; /**< user data's size */
    int ret_val;
    int timeout;
    int poll_number; /**< channel number */

    char prof_file[PROF_FILE_NAME_MAX]; /**< file path */
    char ts_data[PROF_SQ_BUF_LEN];      /**< ts data's pointer */
    char user_data[PROF_USER_DATA_LEN]; /**< user data for peripheral device */

    void *out_buf; /**< save return info */
} prof_ioctl_para_t;
#endif

enum prof_cmd_type {
    PROF_GET_PLATFORM = 201,
    PROF_GET_DEVNUM,
    PROF_GET_DEVIDS,  // for reserve
    PROF_DEV_START,
    PROF_TS_START,
    PROF_STOP,
    PROF_READ,
    PROF_POLL,
    PROF_GET_TSNUM,
    PROF_CMD_MAX
};

enum prof_channel_type {
    PROF_TS_TYPE,
    PROF_PERIPHERAL_TYPE,
    PROF_CHANNEL_TYPE_MAX
};
/**
* @ingroup driver-stub
* @brief Trigger peripheral devices to start preparing to sample Profile information
* @attention null
* @param [in] device_id    device ID
* @param [in] channel_id   Channel ID(1--CHANNEL_HCCS or (CHANNEL_TSCPU_MAX + 1)--(CHANNEL_NUM - 1))
* @param [in] sample_period   Set the background sampling profile period
* @param [in] real_time   Real-time mode or non-real-time mode
* @param [in] file_path   Path to save the file
* @param [in] user_data   user_data for agent device
* @param [in] data_size   user_data's size
* @return   0 for success, others for fail
*/
int prof_peripheral_start(unsigned int device_id, unsigned int channel_id, unsigned int sample_period,
    unsigned int real_time, const char *file_path, const void *user_data, unsigned int data_size);
/**
* @ingroup driver-stub
* @brief Trigger ts to start preparing for sampling profile information
* @attention null
* @param [in] device_id   device ID
* @param [in] channel_id  Channel ID(CHANNEL_TSCPU--(CHANNEL_TSCPU_MAX - 1))
* @param [in] real_time  Real-time mode or non-real-time mode
* @param [in] *file_path  path to save the file
* @param [in] *ts_cpu_data  TS related data buffer
* @param [in] data_size  ts related data length
* @return  0 for success, others for fail
*/
int prof_tscpu_start(unsigned int device_id, unsigned int channel_id, unsigned int real_time, const char *file_path,
    const void *ts_cpu_data, unsigned int data_size);
/**
* @ingroup driver-stub
* @brief Trigger Prof sample end
* @attention nul
* @param [in] dev_id  Device ID
* @param [in] channel_id  channel ID(1--(CHANNEL_NUM - 1))
* @return   0 for success, others for fail
*/
int prof_stop(unsigned int device_id, unsigned int channel_id);
/**
* @ingroup driver-stub
* @brief Read and collect profile information
* @attention null
* @param [in] device_id  Device ID
* @param [in] channel_id  channel ID(1--(CHANNEL_NUM - 1))
* @param [in] *out_buf  Store read profile information
* @param [in] buf_size  Store the length of the profile to be read
* @return   0   success
* @return positive number for readable buffer length
* @return  -1 for fail
*/
int prof_channel_read(unsigned int device_id, unsigned int channel_id, char *out_buf, unsigned int buf_size);
/**
* @ingroup driver-stub
* @brief Querying valid channel information
* @attention null
* @param [in] *out_buf  User mode pointer
* @param [in] num  Number of channels to monitor
* @param [in] timeout  Timeout in seconds
* @return 0  No channels available
* @return positive number for channels Number
* @return -1 for fail
*/
int prof_channel_poll(struct prof_poll_info *out_buf, int num, int timeout);
int prof_get_tsnum(unsigned int *tsnum);

/**
 * @ingroup driver-stub
 * @brief ZIP MACRO
 */
#define HZIP_LEVEL_DEFAULT          0
#define HZIP_VERSION                "1.0.1"
#define HZIP_METHOD_DEFAULT         0
#define HZIP_WINDOWBITS_GZIP        16
#define HZIP_MEM_LEVEL_DEFAULT      0
#define HZIP_STRATEGY_DEFAULT       0
#define HZIP_FLUSH_TYPE_SYNC_FLUSH  0
#define HZIP_FLUSH_TYPE_FINISH      1
#define HZIP_OK                     0
#define HZIP_STREAM_END             1

/**
 * @ingroup driver-stub
 * @brief zip stream param
 */
struct zip_stream {
    void            *next_in;   /**< next input byte */
    unsigned long   avail_in;   /**< number of bytes available at next_in */
    unsigned long   total_in;   /**< total nb of input bytes read so far */
    void            *next_out;  /**< next output byte should be put there */
    unsigned long   avail_out;  /**< remaining free space at next_out */
    unsigned long   total_out;  /**< total nb of bytes output so far */
    char            *msg;       /**< last error message, NULL if no error */
    void            *workspace; /**< memory allocated for this stream */
    int             data_type;  /**< the data type: ascii or binary */
    unsigned long   adler;      /**< adler32 value of the uncompressed data */
    void            *reserved;  /**< reserved for future use */
};

/**
 * @ingroup driver-stub
 * @brief zlib deflate init
 * @attention null
 * @param [out] zstrm   zip stream
 * @param [in] level    HZIP_LEVEL_DEFAULT
 * @param [in] version  HZIP_VERSION
 * @param [in] stream_size  size of zstrm
 * @return   HZIP_OK   success
 * @return   other  fail
 */
int hw_deflateInit_(struct zip_stream *zstrm, int level, const char *version, int stream_size);

/**
 * @ingroup driver-stub
 * @brief gzip deflate init
 * @attention null
 * @param [out] zstrm  zip stream
 * @param [in] level   HZIP_LEVEL_DEFAULT
 * @param [in] method  HZIP_METHOD_DEFAULT
 * @param [in] windowBits  HZIP_WINDOWBITS_GZIP
 * @param [in] memLevel HZIP_MEM_LEVEL_DEFAULT
 * @param [in] strategy HZIP_STRATEGY_DEFAULT
 * @param [in] version  HZIP_VERSION
 * @param [in] stream_size  size of zstrm
 * @return   HZIP_OK   success
 * @return   other  fail
 */
int hw_deflateInit2_(struct zip_stream *zstrm, int level, int method, int windowBits,
                     int memLevel, int strategy, const char *version, int stream_size);

/**
 * @ingroup driver-stub
 * @brief deflat data
 * @attention null
 * @param [in] zstrm  zip stream
 * @param [in] flush  HZIP_FLUSH_TYPE_SYNC_FLUSH/HZIP_FLUSH_TYPE_FINISH
 * @return   HZIP_OK   success
 * @return   HZIP_STREAM_END   stream end
 * @return   other  fail
 */
int hw_deflate(struct zip_stream *zstrm, int flush);

/**
 * @ingroup driver-stub
 * @brief deflate end
 * @attention null
 * @param [in] zstrm  zip stream
 * @return   HZIP_OK   sucess
 * @return   other  fail
 */
int hw_deflateEnd(struct zip_stream *zstrm);

/**
 * @ingroup driver-stub
 * @brief zlib deflate init
 * @attention null
 * @param [out] zstrm  zip stream
 * @param [in] version  HZIP_VERSION
 * @param [in] stream_size  size of zstrm
 * @return   HZIP_OK   success
 * @return   other  fail
 */
int hw_inflateInit_(struct zip_stream *zstrm, const char *version, int stream_size);

/**
 * @ingroup driver-stub
 * @brief gzip inflate init
 * @attention null
 * @param [out] zstrm  zip stream
 * @param [in] windowBits  HZIP_WINDOWBITS_GZIP
 * @param [in] version  HZIP_VERSION
 * @param [in] stream_size  size of zstrm
 * @return   HZIP_OK   success
 * @return   other  fail
 */
int hw_inflateInit2_(struct zip_stream *zstrm, int windowBits, const char *version, int stream_size);

/**
 * @ingroup driver-stub
 * @brief inflate data
 * @attention null
 * @param [in] zstrm  zip stream
 * @param [in] flush  HZIP_FLUSH_TYPE_SYNC_FLUSH/HZIP_FLUSH_TYPE_FINISH
 * @return   HZIP_OK   success
 * @return   HZIP_STREAM_END   stream end
 * @return   other  fail
 */
int hw_inflate(struct zip_stream *zstrm, int flush);

/**
 * @ingroup driver-stub
 * @brief inflate end
 * @attention null
 * @param [in] zstrm  zip stream
 * @return   HZIP_OK   sucess
 * @return   other  fail
 */
int hw_inflateEnd(struct zip_stream *zstrm);

#ifdef __cplusplus
}
#endif
#endif

