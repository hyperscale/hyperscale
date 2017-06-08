project (libeinfo C)
set (CMAKE_MACOSX_RPATH 1)

set (LIBEINFO_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include)
set (LIBEINFO_INCLUDE_DIR ${LIBEINFO_INCLUDE_DIR} PARENT_SCOPE)

if (TARGET libeinfo)
    message ("libeinfo is already defined")
else ()
    include_directories (${LIBEINFO_INCLUDE_DIR})
    set (SRC_FILES ./src/libeinfo.c)
    add_library (libeinfo STATIC ${SRC_FILES})

    list (APPEND HYPERSCALE_LIBS "libeinfo")

    target_include_directories (libeinfo PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
endif()
