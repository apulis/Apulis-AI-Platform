set(src_list vsprintf_s.c 
             wmemmove_s.c
             strncat_s.c
             vsnprintf_s.c
             fwscanf_s.c
             scanf_s.c
             strcat_s.c
             sscanf_s.c
             secureprintoutput_w.c
             wmemcpy_s.c
             wcsncat_s.c
             secureprintoutput_a.c
             secureinput_w.c
             memcpy_s.c
             fscanf_s.c
             vswscanf_s.c
             secureinput_a.c
             sprintf_s.c
             memmove_s.c
             swscanf_s.c
             snprintf_s.c
             vscanf_s.c
             vswprintf_s.c
             wcscpy_s.c
             vfwscanf_s.c
             memset_s.c
             wscanf_s.c
             vwscanf_s.c
             strtok_s.c
             wcsncpy_s.c
             vfscanf_s.c
             vsscanf_s.c
             wcstok_s.c
             securecutil.c
             gets_s.c
             swprintf_s.c
             strcpy_s.c
             wcscat_s.c
             strncpy_s.c
)

add_self_target(
    HI_TYPE OBJECT
    HI_TARGET libc_sec_src_objects 
    HI_SOURCE ${src_list}
    HI_HEAD ${libc_sec_headers}
    HI_CFLAG ${self_options}
    HI_CXXFLAG ${self_options}
    HI_DEF_MACRO ${self_definitions}
    )

