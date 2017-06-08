
file (
    GLOB_RECURSE
    HYPERSCALE_BIN_SRC_FILES
    "*.cpp"
)

include_directories (${LIBEINFO_INCLUDE_DIR})

# Compile the compiler.
add_executable (
    HyperscaleBin
    ${HYPERSCALE_BIN_SRC_FILES}
)

include_directories(${CMAKE_LIBRARY_OUTPUT_DIRECTORY})

set_target_properties (HyperscaleBin PROPERTIES
    COMPILE_FLAGS "${LLVM_CXX_FLAGS} -I."
    OUTPUT_NAME hyperscale
    LINK_FLAGS "${HYPERSCALE_LINK_FLAGS} ${LLVM_LINK_FLAGS} -rdynamic -lcurses"
)
#target_link_libraries (hyperscale ${LLVM_LIBS} ${HYPERSCALE_LINK_FLAGS} ${HYPERSCALE_LIBS} ${CLANG_LIBS} ${Boost_LIBRARIES} HyperscaleStatic)
target_link_libraries (HyperscaleBin ${LLVM_LIBS} ${HYPERSCALE_LINK_FLAGS} ${CLANG_LIBS} HyperscaleLib EInfoLib)

install (TARGETS HyperscaleBin DESTINATION bin)
