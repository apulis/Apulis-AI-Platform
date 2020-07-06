ifeq ($(filter kirin710 kirin970 kirin980, $(TARGET_DEVICE)),)
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

libc_common_includes := \
        $(LOCAL_PATH)/include \
        $(LOCAL_PATH)/src

include $(CLEAR_VARS)

LOCAL_MODULE := libc_sec

LOCAL_SRC_FILES := $(libc_common_src_files)

LOCAL_C_INCLUDES := $(libc_common_includes)

LOCAL_CFLAGS := -Wall -s -DNDEBUG -O1 -DSECUREC_SUPPORT_STRTOLD=1 -Werror

LOCAL_MULTILIB := both

LOCAL_PROPRIETARY_MODULE := true

include $(BUILD_SHARED_LIBRARY)


include $(CLEAR_VARS)

LOCAL_MODULE := libc_sec

LOCAL_SRC_FILES := $(libc_common_src_files)

LOCAL_C_INCLUDES := $(libc_common_includes)

LOCAL_CFLAGS := -Wall -s -DNDEBUG -O1 -DSECUREC_SUPPORT_STRTOLD=1 -Werror

LOCAL_MULTILIB := both

LOCAL_PROPRIETARY_MODULE := true

include $(BUILD_STATIC_LIBRARY)

#built libc_sec.a for host
include $(CLEAR_VARS)

LOCAL_MODULE := libc_sec

LOCAL_SRC_FILES := $(libc_common_src_files)

LOCAL_C_INCLUDES := $(libc_common_includes)

LOCAL_CFLAGS := -Wall -s -DNDEBUG -O1 -DSECUREC_SUPPORT_STRTOLD=1 -Werror
LOCAL_UNINSTALLABLE_MODULE := false

include $(BUILD_HOST_STATIC_LIBRARY)

#add for omg host excutable on lite

include $(CLEAR_VARS)

LOCAL_MODULE := libc_sec

LOCAL_SRC_FILES := $(libc_common_src_files)

LOCAL_C_INCLUDES := $(libc_common_includes)

LOCAL_CFLAGS := -Wall -s -DNDEBUG -O1 -DSECUREC_SUPPORT_STRTOLD=1 -Werror

LOCAL_MULTILIB := 64

LOCAL_PROPRIETARY_MODULE := true

include $(BUILD_HOST_SHARED_LIBRARY)

endif

#built libc_sec_cce.a for host with arm compiler
include $(CLEAR_VARS)

LOCAL_PREBUILT_LIBS := ../bin/misc/libc_sec_cce/libc_sec_cce.a

include $(BUILD_HOST_PREBUILT)


