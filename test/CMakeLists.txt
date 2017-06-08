
set (CMAKE_MEMORYCHECK_COMMAND valgrind)
set (CMAKE_MEMORYCHECK_COMMAND_OPTIONS "--error-exitcode=1 -v --track-origins=yes --leak-check=full --show-reachable=yes --trace-children=yes --child-silent-after-fork=yes -q")

find_file (HAVE_VALGRIND "valgrind")

file (
    GLOB_RECURSE
    TEST_FILES
    "*.cpp"
)

include_directories (${LIBEINFO_INCLUDE_DIR})

add_executable (
    hyperscale-test
    ${TEST_FILES}
)

set_target_properties (hyperscale-test PROPERTIES COMPILE_FLAGS "${LLVM_CXX_FLAGS} -I.")
set_target_properties (hyperscale-test PROPERTIES LINK_FLAGS "${HYPERSCALE_LINK_FLAGS} ${LLVM_LINK_FLAGS} -rdynamic")
target_link_libraries (hyperscale-test ${LLVM_LIBS} ${HYPERSCALE_LINK_FLAGS} HyperscaleLib EInfoLib curses ${CLANG_LIBS} ${Boost_LIBRARIES})

if (CMAKE_COMPILER_IS_GNUCXX)
    include (CodeCoverage)
    setup_target_for_coverage (hyperscale-coverage hyperscale-test coverage)
    target_link_libraries (hyperscale-test gcov)
endif ()


function (add_memcheck_test name binary)
    set (memcheck_command "${CMAKE_MEMORYCHECK_COMMAND} ${CMAKE_MEMORYCHECK_COMMAND_OPTIONS}")
    separate_arguments (memcheck_command)
    add_test (${name} ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${binary})
    if (HAVE_VALGRIND)
        add_test (memcheck_${name} ${memcheck_command} ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${binary})
    endif ()
endfunction (add_memcheck_test)

add_memcheck_test(Hyperscale hyperscale-test)
