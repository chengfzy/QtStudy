project(Eg01_HelloWorld)

add_executable(${PROJECT_NAME} ./main.cpp)
target_include_directories(${PROJECT_NAME}
        PRIVATE
        ${PRIVATE_DEPEND_INCLUDES})
target_link_libraries(${PROJECT_NAME}
        PRIVATE
        ${PUBLIC_DEPEND_LIBS}
        ${PRIVATE_DEPEND_LIBS})