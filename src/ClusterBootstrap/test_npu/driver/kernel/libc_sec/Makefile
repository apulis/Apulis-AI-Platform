
ifeq ($(DAVINCI_HIAI_DKMS),y)

EXTRA_CFLAGS += -I$(HIAI_DKMS_DIR)/libc_sec/include -I$(HIAI_DKMS_DIR)/libc_sec/src

else

CUR_MAKEFILE_PATH := $(strip \
                $(eval LOCAL_MODULE_MAKEFILE := $$(lastword $$(MAKEFILE_LIST))) \
                $(patsubst %/,%, $(dir $(LOCAL_MODULE_MAKEFILE))) \
                )
EXTRA_CFLAGS += -I$(CUR_MAKEFILE_PATH)/../libc_sec/include -I$(CUR_MAKEFILE_PATH)/../libc_sec/src

endif


obj-m := drv_seclib_host.o
drv_seclib_host-objs := src/memcpy_s.o src/memmove_s.o src/memset_s.o  src/securecutil.o  src/strcat_s.o src/strcpy_s.o src/strncat_s.o src/strncpy_s.o src/sprintf_s.o src/vsprintf_s.o src/snprintf_s.o src/vsnprintf_s.o src/secureprintoutput_a.o src/sscanf_s.o src/vsscanf_s.o src/secureinput_a.o src/securecmodule.o


