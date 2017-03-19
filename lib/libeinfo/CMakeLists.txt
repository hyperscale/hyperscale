cmake_minimum_required (VERSION 2.6)

set (LIBEINFO_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include)
set (LIBEINFO_INCLUDE_DIR ${LIBEINFO_INCLUDE_DIR} PARENT_SCOPE)

if (TARGET libeinfo)
    message ("libeinfo is already defined")
else ()
    include_directories (${LIBEINFO_INCLUDE_DIR})
    set (LIB_SRCS ./src/libeinfo.c)
    add_library (libeinfo STATIC ${LIB_SRCS})
endif()
