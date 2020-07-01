LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_INSTALLED_WIN_FILES := libc_sec.dll libc_sec.lib
LOCAL_VCXPROJ := $(LOCAL_PATH)\libc_sec.vcxproj

include $(BUILD_HOST_WIN)