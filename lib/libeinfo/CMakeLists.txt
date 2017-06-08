set (LIBEINFO_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include)
set (LIBEINFO_INCLUDE_DIR ${LIBEINFO_INCLUDE_DIR} PARENT_SCOPE)

include_directories (${LIBEINFO_INCLUDE_DIR})

set (LIBEINFO_SRC_FILES ./src/libeinfo.c)

add_library (EInfoLib ${LIBEINFO_SRC_FILES})

set_target_properties (EInfoLib PROPERTIES
    XCODE_ATTRIBUTE_DEBUG_INFORMATION_FORMAT "dwarf-with-dsym"
    OUTPUT_NAME einfo
)

target_include_directories (EInfoLib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
