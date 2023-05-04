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
add_subdirectory(chart)
add_subdirectory(customplot)
add_subdirectory(ads)
add_subdirectory(gl)
add_subdirectory(web)
if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    add_subdirectory(quick)
endif()