set(libc_sec_headers
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)


set(self_options -s -O2 -Werror -fPIC)

set(self_definitions -DNDEBUG -DSECUREC_SUPPORT_STRTOLD=2 -D_FORTIFY_SOURCE=2)

add_self_target(HI_TYPE SHARED
                HI_TARGET libc_sec.so
                HI_OUTPATH ${TARGET_OUTPUT_PATH}
                HI_OBJECT libc_sec_src_objects
)

add_self_target(HI_TYPE STATIC
                HI_TARGET libc_sec.a
                HI_OUTPATH ${TARGET_OUTPUT_PATH}
                HI_OBJECT libc_sec_src_objects
)

add_subdirectory(src)
