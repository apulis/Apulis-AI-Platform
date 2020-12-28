LOCAL_PATH := $(call my-dir)

libc_common_src_files := \
        src/vsprintf_s.c \
        src/wmemmove_s.c\
        src/strncat_s.c\
        src/vsnprintf_s.c\
        src/fwscanf_s.c\
        src/scanf_s.c\
        src/strcat_s.c\
        src/sscanf_s.c\
        src/secureprintoutput_w.c\
        src/wmemcpy_s.c\
        src/wcsncat_s.c\
        src/secureprintoutput_a.c\
        src/secureinput_w.c\
        src/memcpy_s.c\
        src/fscanf_s.c\
        src/vswscanf_s.c\
        src/secureinput_a.c\
        src/sprintf_s.c\
        src/memmove_s.c\
        src/swscanf_s.c\
        src/snprintf_s.c\
        src/vscanf_s.c\
        src/vswprintf_s.c\
        src/wcscpy_s.c\
        src/vfwscanf_s.c\
        src/memset_s.c\
        src/wscanf_s.c\
        src/vwscanf_s.c\
        src/strtok_s.c\
        src/wcsncpy_s.c\
        src/vfscanf_s.c\
        src/vsscanf_s.c\
        src/wcstok_s.c\
        src/securecutil.c\
        src/gets_s.c\
        src/swprintf_s.c\
        src/strcpy_s.c\
        src/wcscat_s.c\
        src/strncpy_s.c

#built libc_sec.so for host
include $(CLEAR_VARS)

LOCAL_MODULE := libc_sec

LOCAL_SRC_FILES := $(libc_common_src_files)

LOCAL_C_INCLUDES := libc_sec/include libc_sec/src

LOCAL_CFLAGS := -Wall -s -DNDEBUG -O1 -DSECUREC_SUPPORT_STRTOLD=1 -Werror

include $(BUILD_HOST_SHARED_LIBRARY)

#built libc_sec.so for device
include $(CLEAR_VARS)

LOCAL_MODULE := libc_sec

LOCAL_SRC_FILES := $(libc_common_src_files)

LOCAL_C_INCLUDES := libc_sec/include libc_sec/src
ifeq ($(device_os),android)
LOCAL_CFLAGS := -Wall -DNDEBUG -O1 -DSECUREC_SUPPORT_STRTOLD=1 -Werror
else
LOCAL_CFLAGS := -Wall -s -DNDEBUG -O1 -DSECUREC_SUPPORT_STRTOLD=1 -Werror
endif

include $(BUILD_SHARED_LIBRARY)


#built libc_sec.a for device
include $(CLEAR_VARS)

LOCAL_MODULE := libc_sec

LOCAL_SRC_FILES := $(libc_common_src_files)

LOCAL_C_INCLUDES := libc_sec/include libc_sec/src
ifeq ($(device_os),android)
LOCAL_CFLAGS := -Wall -DNDEBUG -O1 -DSECUREC_SUPPORT_STRTOLD=1 -Werror
else
LOCAL_CFLAGS := -Wall -s -DNDEBUG -O1 -DSECUREC_SUPPORT_STRTOLD=1 -Werror
endif
LOCAL_CFLAGS += -fstack-protector-all -fPIC -D_FORTIFY_SOURCE=2 -O2
LOCAL_UNINSTALLABLE_MODULE := false

include $(BUILD_STATIC_LIBRARY)

#built libc_sec.a for host
include $(CLEAR_VARS)

LOCAL_MODULE := libc_sec

LOCAL_SRC_FILES := $(libc_common_src_files)

LOCAL_C_INCLUDES := libc_sec/include libc_sec/src

LOCAL_CFLAGS := -Wall -s -DNDEBUG -O1 -DSECUREC_SUPPORT_STRTOLD=1 -Werror
LOCAL_UNINSTALLABLE_MODULE := false

include $(BUILD_HOST_STATIC_LIBRARY)

#built libc_sec_cce.a for host with arm compiler
include $(CLEAR_VARS)

LOCAL_MODULE := libc_sec_cce

LOCAL_SRC_FILES := $(libc_common_src_files)

LOCAL_C_INCLUDES := libc_sec/include libc_sec/src

LOCAL_CFLAGS := -Wall -s -DNDEBUG -O1 -DSECUREC_SUPPORT_STRTOLD=1 -Werror

ifeq ($(strip $(HOST_COMPILE_ARCH)),x86_64)
LOCAL_TOOLCHAIN_PREFIX := $(CCACHE) bin/toolchain/x86/ubuntu/hcc_libs/hcc_x86_ubuntu_16_04_adk/bin/aarch64-linux-gnu-
endif

include $(BUILD_HOST_STATIC_LIBRARY)

#built libc_sec_x86.so for x86
include $(CLEAR_VARS)

LOCAL_MODULE := libc_sec_x86

LOCAL_CC := gcc
LOCAL_CXX := g++

LOCAL_SRC_FILES := $(libc_common_src_files)

LOCAL_C_INCLUDES := libc_sec/include libc_sec/src

LOCAL_CFLAGS := -Wall -s -DNDEBUG -O1 -DSECUREC_SUPPORT_STRTOLD=1 -Werror

include $(BUILD_HOST_SHARED_LIBRARY)

#build libc_sec_drv host
include $(CLEAR_VARS)

LOCAL_MODULE := drv_seclib_host

LOCAL_KO_SRC_FOLDER := $(LOCAL_PATH)
LOCAL_INSTALLED_KO_FILES := drv_seclib_host.ko

include $(BUILD_HOST_KO)

#built libc_sec for llt test
include $(LOCAL_PATH)/module_llt.mk
