cmake_minimum_required (VERSION 2.6)

project (Hyperscale)

if (${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
    set (CMAKE_CXX_COMPILER "clang++")
endif ()

set(CMAKE_MACOSX_RPATH 1)

# Set some nicer output dirs.
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/lib)


set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake_modules" ${CMAKE_MODULE_PATH})

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
         FORCE)
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

set (CXX_FLAGS_EXTRA "-std=c++11")

string (REGEX REPLACE "^ +" "" HYPERSCALE_LINK_FLAGS ${HYPERSCALE_LINK_FLAGS_})
set (CMAKE_CXX_FLAGS         "$ENV{CPPFLAGS} -Wall")
set (CMAKE_CXX_FLAGS_DEBUG   "-g -ggdb -O0 -fno-inline -Wextra -Wno-unused-parameter ${CXX_FLAGS_EXTRA}")
set (CMAKE_CXX_FLAGS_RELEASE "-O3 -fomit-frame-pointer ${CXX_FLAGS_EXTRA}")

#set (HYPERSCALE_FLAGS_DEBUG   "-O0")
#set (HYPERSCALE_FLAGS_RELEASE "-O3")

#if (IS_DEBUG)
#    set (HYPERSCALE_FLAGS ${HYPERSCALE_FLAGS_DEBUG})
#else ()
#    set (HYPERSCALE_FLAGS ${HYPERSCALE_FLAGS_RELEASE})
#endif ()

if (${CMAKE_SYSTEM_NAME} MATCHES "FreeBSD")
    include_directories ("${PROJECT_BINARY_DIR}" "/usr/local/include")
else ()
    include_directories ("${PROJECT_BINARY_DIR}")
endif ()

#GENERATOR: "KeywordLookup.h": keyword decision tree used by the lexer
#add_custom_command(
#    OUTPUT ${CMAKE_SOURCE_DIR}/src/hyperscale/parser/KeywordLookup.h
#    MAIN_DEPENDENCY ${CMAKE_CURRENT_SOURCE_DIR}/KeywordLookupGenerator.py
#    DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/src/hyperscale/parser/Keywords.table
#    COMMAND ${PYTHON_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/KeywordLookupGenerator.py ${CMAKE_CURRENT_SOURCE_DIR}/src/hyperscale/parser/Keywords.table > ${CMAKE_SOURCE_DIR}/src/hyperscale/parser/KeywordLookup.h
#    VERBATIM
#)
#set_source_files_properties(${CMAKE_SOURCE_DIR}/src/hyperscale/parser/Lexer.cpp PROPERTIES OBJECT_DEPENDS "${CMAKE_SOURCE_DIR}/src/hyperscale/parser/KeywordLookup.h")


find_package (Boost 1.54.0 COMPONENTS filesystem program_options REQUIRED)
include_directories (${Boost_INCLUDE_DIRS})

add_subdirectory (lib/libeinfo libeinfo)
include_directories (${LIBEINFO_INCLUDE_DIR})

#add_subdirectory (lib/WTF wtf)
#include_directories (${LIBWTF_INCLUDE_DIR})

find_package(LLVM REQUIRED COMPONENTS
    option
    analysis
    bitreader
    mcparser
    instrumentation
)

find_package(Clang REQUIRED COMPONENTS
    clang
    clangAnalysis
    clangAST
    clangASTMatchers
    clangBasic
    clangCodeGen
    clangDriver
    clangEdit
    clangFrontend
    clangFrontendTool
    clangLex
    clangParse
    clangSema
    clangSerialization
    clangTooling
)

include_directories(${CLANG_INCLUDE})
include_directories(${CLANG_INCLUDE_DIRS})

include_directories (${CMAKE_SOURCE_DIR}/include)

file (
    GLOB_RECURSE
    SRC_FILES
    src/*
)

# Compile the compiler.
add_executable (
    hyperscale
    ${SRC_FILES}
)
set_target_properties (hyperscale PROPERTIES COMPILE_FLAGS "${LLVM_CXX_FLAGS} -I.")
set_target_properties (hyperscale PROPERTIES LINK_FLAGS "${HYPERSCALE_LINK_FLAGS} ${LLVM_LINK_FLAGS} -rdynamic")
target_link_libraries (hyperscale ${LLVM_LIBS} ${HYPERSCALE_LINK_FLAGS} libeinfo curses ${CLANG_LIBS} ${Boost_LIBRARIES})
install (TARGETS hyperscale DESTINATION bin)
