cmake_minimum_required (VERSION 3.4)

project (Hyperscale CXX C)

if (${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
    set (CMAKE_CXX_COMPILER "clang++")
endif ()

set (CMAKE_MACOSX_RPATH 1)

# Make a job pool for things that can't yet be distributed
cmake_host_system_information (RESULT localhost_logical_cores QUERY NUMBER_OF_LOGICAL_CORES)
set_property (GLOBAL PROPERTY JOB_POOLS local_jobs=${localhost_logical_cores})
# Put linking in that category
set_property (GLOBAL PROPERTY JOB_POOL_LINK local_jobs)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

# Set some nicer output dirs.
set (CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/bin)
set (CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/lib)
set (CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/lib)

# Add path for custom CMake modules.
list (APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake/modules")
#set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake/modules" ${CMAKE_MODULE_PATH})

find_program (LLVM_CONFIG llvm-config DOC "path to llvm-config")
find_program (CLANG clang DOC "path to clang")

if (NOT LLVM_CONFIG)
    message (FATAL_ERROR "llvm-config executable not found. Re-run cmake with -DLLVM_CONFIG={path-to-llvm-config}.")
endif ()

execute_process (
    COMMAND ${LLVM_CONFIG} --version
    OUTPUT_VARIABLE D_LLVM_VERSION
    OUTPUT_STRIP_TRAILING_WHITESPACE
)
execute_process (
    COMMAND ${LLVM_CONFIG} --bindir
    OUTPUT_VARIABLE LLVM_BIN_DIR
    OUTPUT_STRIP_TRAILING_WHITESPACE
)
execute_process (
    COMMAND echo ${D_LLVM_VERSION}
    COMMAND sed -e "s/[^0-9].*//"
    OUTPUT_VARIABLE D_LLVM_VERSION_MAJOR
    OUTPUT_STRIP_TRAILING_WHITESPACE
)
execute_process (
    COMMAND echo ${D_LLVM_VERSION}
    COMMAND sed -e "s/[0-9]*\\.//"
    COMMAND sed -e "s/svn//"
    COMMAND sed -e "s/\\..*//"
    OUTPUT_VARIABLE D_LLVM_VERSION_MINOR
    OUTPUT_STRIP_TRAILING_WHITESPACE
)

set (HYPERSCALE_VERSION_MAJOR   0)
set (HYPERSCALE_VERSION_MINOR   1)
set (HYPERSCALE_VERSION_PATCH   0)
set (HYPERSCALE_INCLUDE_PATH    "${CMAKE_INSTALL_PREFIX}/include/hyperscale")
set (HYPERSCALE_LIBRARY_PATH    "${CMAKE_INSTALL_PREFIX}/lib/hyperscale")
set (HYPERSCALE_MODULE_PATH     "${CMAKE_INSTALL_PREFIX}/lib/hyperscale")
set (SYSTEM                     "${CMAKE_SYSTEM}")
set (SYSTEM_NAME                "${CMAKE_SYSTEM_NAME}")
set (SYSTEM_PROCESSOR           "${CMAKE_SYSTEM_PROCESSOR}")
set (SIZEOF_DATA_PTR            "${CMAKE_CXX_SIZEOF_DATA_PTR}")
set (D_LLVM_VERSION             "${D_LLVM_VERSION}")
set (D_LLVM_VERSION_MAJOR       "${D_LLVM_VERSION_MAJOR}")
set (D_LLVM_VERSION_MINOR       "${D_LLVM_VERSION_MINOR}")
set (LLVM_BIN_DIR               "${LLVM_BIN_DIR}")

string (TIMESTAMP HYPERSCALE_CURRENT_YEAR "%Y")

message (STATUS "LLVM version: ${D_LLVM_VERSION}")
message (STATUS "LLVM version major: ${D_LLVM_VERSION_MAJOR}")
message (STATUS "LLVM version minor: ${D_LLVM_VERSION_MINOR}")
message (STATUS "System name: ${CMAKE_SYSTEM_NAME}")
message (STATUS "System processor: ${CMAKE_SYSTEM_PROCESSOR}")
message (STATUS "System version: ${CMAKE_SYSTEM_VERSION}")

if (${D_LLVM_VERSION_MAJOR} LESS 3)
    message (FATAL_ERROR "LLVM >= 3.2 is required.")
elseif (${D_LLVM_VERSION_MAJOR} EQUAL 3)
    if (${D_LLVM_VERSION_MINOR} LESS 2)
        message (FATAL_ERROR "LLVM >= 3.2 is required.")
    #elseif (${D_LLVM_VERSION_MINOR} GREATER 5)
    #    message (FATAL_ERROR "LLVM >= 3.6 is not supported.")
    endif ()
endif ()


# Get LLVM configuration details.
execute_process (
    COMMAND ${LLVM_CONFIG} --libs
    OUTPUT_VARIABLE LLVM_LIBS
    OUTPUT_STRIP_TRAILING_WHITESPACE
)
execute_process (
    COMMAND ${LLVM_CONFIG} --ldflags
    OUTPUT_VARIABLE LLVM_LINK_FLAGS
    OUTPUT_STRIP_TRAILING_WHITESPACE
)
execute_process (
    COMMAND ${LLVM_CONFIG} --cppflags
    OUTPUT_VARIABLE LLVM_CXX_FLAGS
    OUTPUT_STRIP_TRAILING_WHITESPACE
)


# Populate the configuration header.
configure_file (
    "${CMAKE_SOURCE_DIR}/include/hyperscale/config.hpp.in"
    "${CMAKE_SOURCE_DIR}/include/hyperscale/config.hpp"
)

if (NOT CMAKE_BUILD_TYPE)
    set (CMAKE_BUILD_TYPE Release CACHE STRING
        "Choose the type of build. Options are 'Debug' and 'Release'."
        FORCE
    )
endif ()

string (TOUPPER ${CMAKE_BUILD_TYPE} CMAKE_BUILD_TYPE)
string (COMPARE EQUAL ${CMAKE_BUILD_TYPE} "DEBUG"   IS_DEBUG)
string (COMPARE EQUAL ${CMAKE_BUILD_TYPE} "RELEASE" IS_RELEASE)

if (NOT IS_DEBUG)
    if (NOT IS_RELEASE)
        message (FATAL_ERROR "Invalid CMAKE_BUILD_TYPE specified. Must be either 'Debug' or 'Release'.")
    endif ()
endif ()

message (STATUS "Build type: ${CMAKE_BUILD_TYPE}")

if (IS_DEBUG)
    string (REGEX REPLACE "-DNDEBUG" "" LLVM_CXX_FLAGS ${LLVM_CXX_FLAGS})
endif ()

# Set compile/link flags.
set (HYPERSCALE_LIBDIR_FLAGS_ "$ENV{LDFLAGS}")
set (HYPERSCALE_LIB_FLAGS_    "-lpthread")

set (HYPERSCALE_LIBS "")

if (${D_LLVM_VERSION_MINOR} GREATER 3)
    if (${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
        set (HYPERSCALE_LIB_FLAGS_ "${HYPERSCALE_LIB_FLAGS_} -lz -lncurses")
    else ()
        set (HYPERSCALE_LIB_FLAGS_ "${HYPERSCALE_LIB_FLAGS_} -lz -ltinfo")
    endif ()
endif()

if (${CMAKE_SYSTEM_NAME} MATCHES "FreeBSD")
    set (HYPERSCALE_LINK_FLAGS_ "${HYPERSCALE_LIBDIR_FLAGS_} -L/usr/local/lib ${HYPERSCALE_LIB_FLAGS_}")
else ()
    set (HYPERSCALE_LINK_FLAGS_ "${HYPERSCALE_LIBDIR_FLAGS_} ${HYPERSCALE_LIB_FLAGS_} -ldl")
endif ()

set (CXX_FLAGS_EXTRA "-std=c++14")

string (REGEX REPLACE "^ +" "" HYPERSCALE_LINK_FLAGS ${HYPERSCALE_LINK_FLAGS_})
set (CMAKE_CXX_FLAGS         "${CMAKE_CXX_FLAGS} -Wall -pedantic")
set (CMAKE_CXX_FLAGS_DEBUG   "-g -ggdb -O0 -fprofile-arcs -ftest-coverage -fno-inline -Wextra -Wno-unused-parameter -DDEBUG ${CXX_FLAGS_EXTRA}")
set (CMAKE_CXX_FLAGS_RELEASE "-O3 -fomit-frame-pointer ${CXX_FLAGS_EXTRA}")

set (BUILD_SHARED_LIBS ON)

if (${CMAKE_SYSTEM_NAME} MATCHES "FreeBSD")
    include_directories ("${PROJECT_BINARY_DIR}" "/usr/local/include")
else ()
    include_directories ("${PROJECT_BINARY_DIR}")
endif ()



include_directories (${CMAKE_SOURCE_DIR}/include)

add_subdirectory(bin)
add_subdirectory(include)
add_subdirectory(lib)
add_subdirectory(test)


