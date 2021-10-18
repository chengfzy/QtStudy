# add common module
set(PRJ_COMMON common)
add_subdirectory(common)
# project include and libs
list(APPEND DEPEND_INCLUDES
    ${CMAKE_CURRENT_SOURCE_DIR}/common/include
)
list(APPEND DEPEND_LIBS ${PRJ_COMMON})

# add other projects
add_subdirectory(basic)
add_subdirectory(quick)
add_subdirectory(gl)
add_subdirectory(web)