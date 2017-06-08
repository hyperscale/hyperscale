#set(Boost_USE_STATIC_LIBS   ON)
#find_package (Boost 1.54.0 COMPONENTS filesystem REQUIRED)
#include_directories (${Boost_INCLUDE_DIRS})


include_directories (${LIBEINFO_INCLUDE_DIR})

file (
    GLOB_RECURSE
    HYPERSCALE_LIB_SRC_FILES
    "*.cpp"
)

#set (LIB_LIBS "${LLVM_LIBS} ${HYPERSCALE_LINK_FLAGS} ${CLANG_LIBS} ${Boost_LIBRARIES} curses libeinfo")
set (LIB_LIBS "${LLVM_LIBS} ${HYPERSCALE_LINK_FLAGS} ${CLANG_LIBS}")

add_library (HyperscaleLib ${HYPERSCALE_LIB_SRC_FILES})
set_target_properties (HyperscaleLib PROPERTIES
    XCODE_ATTRIBUTE_DEBUG_INFORMATION_FORMAT "dwarf-with-dsym"
    OUTPUT_NAME hyperscale
    COMPILE_FLAGS "${LLVM_CXX_FLAGS} -I."
    LINK_FLAGS "${HYPERSCALE_LINK_FLAGS} ${LLVM_LINK_FLAGS} -rdynamic -lcurses -leinfo"
)
target_link_libraries (HyperscaleLib ${LIB_LIBS})


target_include_directories (HyperscaleLib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
