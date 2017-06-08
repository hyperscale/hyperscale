#set(Boost_USE_STATIC_LIBS   ON)
#find_package (Boost 1.54.0 COMPONENTS filesystem REQUIRED)
#include_directories (${Boost_INCLUDE_DIRS})

include_directories (${LIBEINFO_INCLUDE_DIR})

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

include_directories (${CLANG_INCLUDE})
include_directories (${CLANG_INCLUDE_DIRS})

include_directories (${LIBEINFO_INCLUDE_DIR})

file (
    GLOB_RECURSE
    SRC_FILES
    "*.cpp"
)

#set (LIB_LIBS "${LLVM_LIBS} ${HYPERSCALE_LINK_FLAGS} ${CLANG_LIBS} ${Boost_LIBRARIES} curses libeinfo")
set (LIB_LIBS "${LLVM_LIBS} ${HYPERSCALE_LINK_FLAGS} ${CLANG_LIBS} libeinfo")

add_library (HyperscaleShared SHARED ${SRC_FILES})
set_target_properties (HyperscaleShared PROPERTIES
    XCODE_ATTRIBUTE_DEBUG_INFORMATION_FORMAT "dwarf-with-dsym"
    OUTPUT_NAME hyperscale
    COMPILE_FLAGS "${LLVM_CXX_FLAGS} -I."
    LINK_FLAGS "${HYPERSCALE_LINK_FLAGS} ${LLVM_LINK_FLAGS} -rdynamic -lcurses"
)
target_link_libraries (HyperscaleShared ${LIB_LIBS})

add_library (HyperscaleStatic STATIC ${SRC_FILES})
set_target_properties (HyperscaleStatic PROPERTIES
    XCODE_ATTRIBUTE_DEBUG_INFORMATION_FORMAT "dwarf-with-dsym"
    OUTPUT_NAME hyperscale
    COMPILE_FLAGS "${LLVM_CXX_FLAGS} -I."
    LINK_FLAGS "${HYPERSCALE_LINK_FLAGS} ${LLVM_LINK_FLAGS} -rdynamic -lcurses"
)
target_link_libraries (HyperscaleStatic ${LIB_LIBS})

target_include_directories (HyperscaleStatic PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
