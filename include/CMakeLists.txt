add_subdirectory(hyperscale)


file (
    GLOB_RECURSE
    INCLUDE_FILES
    "*.hpp"
)


foreach (file ${INCLUDE_FILES} )
    get_filename_component (dir ${file} DIRECTORY)
    string (REGEX REPLACE "^${CMAKE_CURRENT_SOURCE_DIR}" "" dir ${dir})
    install (FILES ${file} DESTINATION include/${dir})
endforeach()
