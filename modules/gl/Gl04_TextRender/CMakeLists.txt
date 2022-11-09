find_package(Freetype QUIET)                     # freetype

if(${Freetype_FOUND})
    list(APPEND DEPEND_LIBS Freetype::Freetype)

    project(Gl04_TextRender)

    # project files
    file(GLOB_RECURSE FILE_INCLUDES ${CMAKE_CURRENT_SOURCE_DIR}/include/*.*)
    file(GLOB_RECURSE FILE_SRCS ${CMAKE_CURRENT_SOURCE_DIR}/src/*.*)
    file(GLOB_RECURSE FILE_RESS ${CMAKE_CURRENT_SOURCE_DIR}/res/*.*)

    add_executable(${PROJECT_NAME} ${FILE_INCLUDES} ${FILE_SRCS} ${FILE_RESS})
    target_include_directories(${PROJECT_NAME}
        PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${DEPEND_INCLUDES}
    )
    target_link_libraries(${PROJECT_NAME} ${DEPEND_LIBS})
endif()

