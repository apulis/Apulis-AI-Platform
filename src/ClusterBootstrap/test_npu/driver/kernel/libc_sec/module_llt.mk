
#build libc_sec.a for llt_test
include $(CLEAR_VARS)

LOCAL_MODULE := libc_sec

LOCAL_SRC_FILES := $(libc_common_src_files)

LOCAL_C_INCLUDES := libc_sec/include libc_sec/src

ifeq ($(strip $(BITCODE)),true)
LOCAL_CFLAGS := -Wall -DNDEBUG -DSECUREC_SUPPORT_STRTOLD=1 -Werror
else
LOCAL_CFLAGS := -Wall -s -DNDEBUG -O1 -DSECUREC_SUPPORT_STRTOLD=1 -Werror
endif

include $(BUILD_LLT_STATIC_LIBRARY)

#build libc_sec.so for llt_test
include $(CLEAR_VARS)

LOCAL_MODULE := libc_sec

LOCAL_SRC_FILES := $(libc_common_src_files)

LOCAL_C_INCLUDES := libc_sec/include libc_sec/src

LOCAL_CFLAGS := -Wall -DNDEBUG -DSECUREC_SUPPORT_STRTOLD=1 -Werror

include $(BUILD_LLT_SHARED_LIBRARY)

include $(CLEAR_VARS)

#built libc_sec_cce.a for tvm x86 llt
include $(CLEAR_VARS)

LOCAL_MODULE := libc_sec_cce

LOCAL_SRC_FILES := $(libc_common_src_files)

LOCAL_C_INCLUDES := libc_sec/include libc_sec/src

ifeq ($(strip $(BITCODE)),true)
LOCAL_CFLAGS := -Wall -DNDEBUG -DSECUREC_SUPPORT_STRTOLD=1 -Werror
else
LOCAL_CFLAGS := -Wall -s -DNDEBUG -O1 -DSECUREC_SUPPORT_STRTOLD=1 -Werror
endif

LOCAL_USE_COMPILER := DEVICE_ARM64

include $(BUILD_LLT_STATIC_LIBRARY)
