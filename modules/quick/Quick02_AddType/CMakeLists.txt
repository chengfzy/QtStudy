project(Quick02_AddType)

file(GLOB_RECURSE FILE_INCLUDES ./include/*.*)
file(GLOB_RECURSE FILE_SRCS ./src/*.*)
file(GLOB_RECURSE FILE_RESS ./res/*.qrc)
add_executable(${PROJECT_NAME} ${FILE_INCLUDES} ${FILE_SRCS} ${FILE_RESS})
target_include_directories(${PROJECT_NAME}
    PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${DEPEND_INCLUDES}
)
# target_link_libraries(${PROJECT_NAME} ${DEPEND_LIBS})
qt_add_qml_module(${PROJECT_NAME}
    URI People
    VERSION 1.0
    QML_FILES res/Person.qml
    NO_RESOURCE_TARGET_PATH
)