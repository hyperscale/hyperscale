
file (
    GLOB_RECURSE
    SRC_FILES
    "*.cpp"
)

# Compile the compiler.
add_executable (
    hyperscale
    ${SRC_FILES}
)

include_directories(${CMAKE_LIBRARY_OUTPUT_DIRECTORY})

#set_target_properties (hyperscale PROPERTIES COMPILE_FLAGS "${LLVM_CXX_FLAGS} -I.")
#set_target_properties (hyperscale PROPERTIES LINK_FLAGS "${HYPERSCALE_LINK_FLAGS} ${LLVM_LINK_FLAGS} -rdynamic")
#target_link_libraries (hyperscale ${LLVM_LIBS} ${HYPERSCALE_LINK_FLAGS} ${HYPERSCALE_LIBS} ${CLANG_LIBS} ${Boost_LIBRARIES} HyperscaleStatic)
target_link_libraries (hyperscale HyperscaleShared)

install (TARGETS hyperscale DESTINATION bin)
