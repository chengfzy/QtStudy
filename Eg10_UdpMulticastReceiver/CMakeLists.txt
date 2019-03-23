project(Eg10_UdpMulticastReceiver)

# should add include for automoc
file(GLOB_RECURSE FILE_INCLUDES ./include/*.*)
file(GLOB_RECURSE FILE_SRCS ./src/*.*)
add_executable(${PROJECT_NAME} ${FILE_INCLUDES}  ${FILE_SRCS})
target_include_directories(${PROJECT_NAME}
        PRIVATE
        ${PRIVATE_DEPEND_INCLUDES}
        ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_link_libraries(${PROJECT_NAME}
        PRIVATE
        ${PUBLIC_DEPEND_LIBS}
        ${PRIVATE_DEPEND_LIBS})