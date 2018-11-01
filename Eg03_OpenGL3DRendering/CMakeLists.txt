project(Eg03_OpenGL3DRendering)

# include directories
include_directories(./include)

# header and source files
file(GLOB_RECURSE FILE_INCLUDES "include/*.*")
file(GLOB_RECURSE FILE_SRCS "src/*.*")
file(GLOB_RECURSE FILE_RESS "res/*.qrc")

message(STATUS "Resources:"  ${FILE_RESS})

# build the library
add_executable(${PROJECT_NAME} ${FILE_INCLUDES} ${FILE_SRCS} ${FILE_RESS})
target_link_libraries(${PROJECT_NAME} ${THIRD_LIBS})