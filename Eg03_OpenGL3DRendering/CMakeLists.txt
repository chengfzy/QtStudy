project(Eg03_OpenGL3DRendering)

# should add include for automoc
file(GLOB_RECURSE FILE_INCLUDES ./include/*.*)
file(GLOB_RECURSE FILE_SRCS ./src/*.*)
file(GLOB_RECURSE FILE_RESS ./res/*.qrc)
add_executable(${PROJECT_NAME} ${FILE_INCLUDES} ${FILE_RESS})
target_include_directories(${PROJECT_NAME}
    PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${DEPEND_INCLUDES}
)
target_link_libraries(${PROJECT_NAME} ${DEPEND_LIBS})