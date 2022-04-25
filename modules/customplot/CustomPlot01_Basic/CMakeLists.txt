project(CustomPlot01_Basic)

# should add include for automoc
file(GLOB_RECURSE FILE_INCLUDES ./include/*.*)
file(GLOB_RECURSE FILE_SRCS ./src/*.*)
add_executable(${PROJECT_NAME} ${FILE_INCLUDES} ${FILE_SRCS})
target_include_directories(${PROJECT_NAME}
    PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${DEPEND_INCLUDES}
)
target_link_libraries(${PROJECT_NAME} ${DEPEND_LIBS})
add_dependencies(${PROJECT_NAME} QCustomPlot)