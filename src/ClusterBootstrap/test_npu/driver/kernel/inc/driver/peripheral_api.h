/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2012-2019. All rights reserved.
 * Description:
 * Author: huawei 
 * Create: 2019-10-15
 */
#ifndef PERIPHERAL_API_H
#define PERIPHERAL_API_H
#include <stdint.h>

// 最大的摄像头数量.
#define HIAI_MAX_CAMERA_COUNT (25)

// 最大的MIC数量.
#define HIAI_MAX_MIC_COUNT (25)

// 摄像头支持的分辨率组合的最大数量.
#define HIAI_MAX_CAMERARESOLUTION_COUNT (25)

enum LIBMEDIA_STATUS {
    LIBMEDIA_STATUS_FAILED = 0,
    LIBMEDIA_STATUS_OK,
};

int MediaLibInit();

// 查询芯片是否可以连通.
// chipName  芯片名称.
// 存在返回1，否则返回0
// 示例:
// IsChipAlive "3559" //< 检测3559芯片是否在位.
int IsChipAlive(char *chipName);

// 查询摄像头通道
// 返回所有的摄像头ID信息。
// cameraIds 用于保存camera id。
// count  cameraIds 的长度。最多HIAI_MAX_CAMERA_COUNT个.
// 返回摄像头通道数量，摄像头id存入cameraIds中.
uint32_t QueryCameraIds(int *cameraIds, uint32_t *count);

// 2. 查询摄像头状态.
enum CameraStatus {
    CAMERA_STATUS_OPEN = 1,    // 摄像头处于打开状态
    CAMERA_STATUS_CLOSED = 2,  // 摄像头处于关闭状态
    CAMERA_NOT_EXISTS = 3,     // 该摄像头不存在
    CAMERA_STATUS_UNKOWN = 4,  // 摄像头状态未知
};

enum CameraStatus QueryCameraStatus(int cameraId);

// 打开摄像头
// 打开成功返回1，否则返回0.
int OpenCamera(int cameraId);

// 设置摄像头参数
enum CameraProperties {
    CAMERA_PROP_RESOLUTION = 1,    // 【Read/Write】分辨率  数据类型 CameraResolution* 长度为1
    CAMERA_PROP_FPS = 2,           // 【Read/Write】帧率, 数据类型为uint32_t
    CAMERA_PROP_IMAGE_FORMAT = 3,  // 【Read/Write】帧图片的格式.  数据类型为CameraImageFormat
    // 【Read】用于获取摄像头支持的所有的分辨率列表.数据类型为CameraResolution*, 数组长度为HIAI_MAX_CAMERARESOLUTION_COUNT
    CAMERA_PROP_SUPPORTED_RESOLUTION = 4,
    CAMERA_PROP_CAP_MODE = 5,    // 【Read/Write】帧数据获取的方式，主动或者被动.数据类型为CameraCapMode
    CAMERA_PROP_BRIGHTNESS = 6,  // 【Read/Write】亮度，数据类型为uint32_t
    CAMERA_PROP_CONTRAST = 7,    // 【Read/Write】对比度，数据类型为uint32_t
    CAMERA_PROP_SATURATION = 8,  // 【Read/Write】饱和度，数据类型为uint32_t
    CAMERA_PROP_HUE = 9,         // 【Read/Write】色调，数据类型为uint32_t
    CAMERA_PROP_GAIN = 10,       // 【Read/Write】增益，数据类型为uint32_t
    CAMERA_PROP_EXPOSURE = 11,   // 【Read/Write】曝光，数据类型为uint32_t
};

// 摄像头图片格式
enum CameraImageFormat {
    CAMERA_IMAGE_YUV420_SP = 1,
};

// 摄像头数据获取模式
enum CameraCapMode {
    CAMERA_CAP_ACTIVE = 1,   // 主动模式.
    CAMERA_CAP_PASSIVE = 2,  // 被动模式.
};

// 分辨率.
struct CameraResolution {
    int width;
    int height;
};

// 设置摄像头参数.
// 设置成功返回1，否则返回0
int SetCameraProperty(int cameraId, enum CameraProperties prop, const void *pInValue);

// 获取摄像头参数.
// 
// 获取成功返回1，否则返回0
int GetCameraProperty(int cameraId, enum CameraProperties prop, void *pValue);

// 获取摄像头数据.
// 执行成功返回1，否则返回0
typedef int (*CAP_CAMERA_CALLBACK)(const void *pdata, int size, void *param);

// 被动采样Camera
// 执行成功返回1， 否则返回0.
int CapCamera(int cameraId, CAP_CAMERA_CALLBACK, void *param);

// 主动采样Camera
// 从摄像头读取一帧图片内容.
// 执行成功返回1， 否则返回0.
int ReadFrameFromCamera(int cameralId, void *pdata, int *size);

// 关闭摄像头
// 打开成功返回1，否则返回0.
int CloseCamera(int cameraId);

// //////////////////////////////////////////////////
// 
//     MIC  Interace
// 
// /////////////////////////////////////////////////
// 摄像头数据获取模式
enum MICCapMode {
    MIC_CAP_ACTIVE = 1,   // 主动模式.
    MIC_CAP_PASSIVE = 2,  // 被动模式.
};

enum AudioSampleRate {
    MIC_AUDIO_SAMPLE_RATE_8000 = 8000,   /* 8K samplerate */
    MIC_AUDIO_SAMPLE_RATE_12000 = 12000, /* 12K samplerate */
    MIC_AUDIO_SAMPLE_RATE_11025 = 11025, /* 11.025K samplerate */
    MIC_AUDIO_SAMPLE_RATE_16000 = 16000, /* 16K samplerate */
    MIC_AUDIO_SAMPLE_RATE_22050 = 22050, /* 22.050K samplerate */
    MIC_AUDIO_SAMPLE_RATE_24000 = 24000, /* 24K samplerate */
    MIC_AUDIO_SAMPLE_RATE_32000 = 32000, /* 32K samplerate */
    MIC_AUDIO_SAMPLE_RATE_44100 = 44100, /* 44.1K samplerate */
    MIC_AUDIO_SAMPLE_RATE_48000 = 48000, /* 48K samplerate */
    MIC_AUDIO_SAMPLE_RATE_64000 = 64000, /* 64K samplerate */
    MIC_AUDIO_SAMPLE_RATE_96000 = 96000, /* 96K samplerate */
    MIC_AUDIO_SAMPLE_RATE_BUTT,
};

enum AudioSampleNumPerFrame {
    MIC_SAMPLE_NUM_80 = 80,
    MIC_SAMPLE_NUM_160 = 160,
    MIC_SAMPLE_NUM_240 = 240,
    MIC_SAMPLE_NUM_320 = 320,
    MIC_SAMPLE_NUM_480 = 480,
    MIC_SAMPLE_NUM_1024 = 1024,
    MIC_SAMPLE_NUM_2048 = 2048,
};

enum AudioBitWidth {
    MIC_AUDIO_BIT_WIDTH_16 = 1, /* 16bit width */
    MIC_AUDIO_BIT_WIDTH_24 = 2, /* 24bit width */
    MIC_AUDIO_BIT_WIDTH_BUTT,
};

enum AudioMode {
    MIC_AUDIO_SOUND_MODE_MONO = 0,   /* mono */
    MIC_AUDIO_SOUND_MODE_STEREO = 1, /* stereo */
    MIC_AUDIO_SOUND_MODE_BUTT
};

struct MICProperties {
    enum AudioSampleRate sample_rate;               // 【Read/Write】采样率, 数据类型为uint32_t
    enum MICCapMode cap_mode;                       // 【Read/Write】MIC捕获声音的模式
    enum AudioBitWidth bit_width;                   // 【Read/Write】每个样本的bit位宽
    enum AudioSampleNumPerFrame frame_sample_rate;  // 【Read/Write】每帧的样本数量
    enum AudioMode sound_mode;                      // 【Read/Write】单声道还是立体声.
};

enum MICStatus {
    MIC_STATUS_OPEN = 1,    // MIC处于打开状态
    MIC_STATUS_CLOSED = 2,  // MIC处于关闭状态
    MIC_NOT_EXISTS = 3,     // MIC不存在
    MIC_STATUS_UNKOWN = 4,  // MIC状态未知
};

enum MICStatus QueryMICStatus();

// 打开 MIC
int OpenMIC();

// 设置MIC 属性.
int SetMICProperty(struct MICProperties *propties);

// 获取MIC 属性.
int GetMICProperty(struct MICProperties *propties);

typedef int (*CAP_MIC_CALLBACK)(const void *pdata, int size, void *param);

// 被动采样MIC
int CapMIC(CAP_MIC_CALLBACK, void *param);

// 主动采样模式时，用户态程序调用该接口从MIC设备读取音频数据
// 如果读取数据时，MIC设备的数据还没有采样完整，则阻塞用户态应用程序直到一帧数据接收完整
// 约定：采用memory copy方式向用户传递数据
// pdata 用户态buffer，用户需要准备足够长的buffer来保存一帧音频数据的长度
// size 用户态buffer的大小,执行成功后，返回实际数据长度
// 执行成功返回1， 否则返回0.
int ReadMicSound(void *pdata, int *size);

// 关闭MIC
int CloseMIC();

#endif
