project(Eg01_HelloWorld)

## header and source files
#file(GLOB_RECURSE FILE_INCLUDES "include/*.*")
#file(GLOB_RECURSE FILE_SRCS "src/*.*")
#
## build the library
#add_executable(${PROJECT_NAME} ${FILE_INCLUDES} ${FILE_SRCS})
#target_link_libraries(${PROJECT_NAME} PUBLIC ${PRJ_LIB})
#add_dependencies(${PROJECT_NAME} ${PRJ_LIB})


# build the library
add_executable(${PROJECT_NAME} main.cpp)
target_link_libraries(${PROJECT_NAME} ${THIRD_LIBS})