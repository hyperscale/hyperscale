
include(ExternalProject)

file (
    GLOB_RECURSE
    HYPERSCALE_BIN_SRC_FILES
    "*.cpp"
)

externalproject_add(
    HyperConsole
    GIT_REPOSITORY git@github.com:hyperlib/console.git
    PREFIX "${CMAKE_CURRENT_BINARY_DIR}/hyper/console"
    CMAKE_ARGS "-G${CMAKE_GENERATOR}" "-DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}"
)

include_directories (${LIBEINFO_INCLUDE_DIR})

# Compile the compiler.
add_executable (
    HyperscaleCmd
    ${HYPERSCALE_BIN_SRC_FILES}
)

add_dependencies(HyperscaleCmd HyperConsole)

include_directories(${CMAKE_LIBRARY_OUTPUT_DIRECTORY})

set_target_properties (HyperscaleCmd PROPERTIES
    COMPILE_FLAGS "${LLVM_CXX_FLAGS} -I."
    OUTPUT_NAME hyperscale
    LINK_FLAGS "${HYPERSCALE_LINK_FLAGS} ${LLVM_LINK_FLAGS} -rdynamic -lcurses"
)
#target_link_libraries (hyperscale ${LLVM_LIBS} ${HYPERSCALE_LINK_FLAGS} ${HYPERSCALE_LIBS} ${CLANG_LIBS} ${Boost_LIBRARIES} HyperscaleStatic)
target_link_libraries (HyperscaleCmd ${LLVM_LIBS} ${HYPERSCALE_LINK_FLAGS} ${CLANG_LIBS} HyperscaleLib EInfoLib)

install (TARGETS HyperscaleCmd DESTINATION bin)
