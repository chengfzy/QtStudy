# Advanced Docking System
# 4.0.3 from https://github.com/githubuser0xFFFF/Qt-Advanced-Docking-System#license-information
project(ads)

# depend includes and libs
if(UNIX AND NOT APPLE)
    list(APPEND DEPEND_INCLUDES ${Qt${QT_VERSION_MAJOR}Gui_PRIVATE_INCLUDE_DIRS})
    # list(APPEND DEPEND_LIBS xcb)
endif()

# project files
file(GLOB FILE_INCLUDES ${CMAKE_CURRENT_SOURCE_DIR}/include/*.*)
file(GLOB FILE_SRCS ${CMAKE_CURRENT_SOURCE_DIR}/src/*.*)
file(GLOB FILE_RESS ${CMAKE_CURRENT_SOURCE_DIR}/res/*.*)
if(UNIX AND NOT APPLE)
    file(GLOB FILE_INCLUDES_LINUX ${CMAKE_CURRENT_SOURCE_DIR}/include/linux/*.*)
    file(GLOB FILE_SRCS_LINUX ${CMAKE_CURRENT_SOURCE_DIR}/src/linux/*.*)
    list(APPEND FILE_INCLUDES ${FILE_INCLUDES_LINUX})
    list(APPEND FILE_SRCS ${FILE_SRCS_LINUX})
endif()

# build target
add_library(${PROJECT_NAME} SHARED ${FILE_INCLUDES} ${FILE_SRCS} ${FILE_RESS})
target_include_directories(${PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${DEPEND_INCLUDES}
)
target_link_libraries(${PROJECT_NAME} PRIVATE ${DEPEND_LIBS})

# target properties
set_target_properties(${PROJECT_NAME} PROPERTIES
    OUTPUT_NAME AdvancedDockingSystem
)

# install
install(TARGETS ${PROJECT_NAME}
    RUNTIME DESTINATION bin
    LIBRARY DESTINATION lib
)