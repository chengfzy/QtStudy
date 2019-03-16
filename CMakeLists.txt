cmake_minimum_required(VERSION 3.2)
project(QtStudy)
set(CMAKE_CXX_STANDARD 14)

# cmake modules
list(APPEND CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)


###############################################################################
# Libraries
###############################################################################
# find and add Qt path from Home directory
foreach (r $ENV{HOME}/Programs $ENV{HOME})
    file(GLOB FOLDER RELATIVE ${r} ${r}/*)
    #    message(STATUS "folders: ${FOLDER}")
    foreach (f ${FOLDER})
        if (IS_DIRECTORY ${r}/${f} AND ${f} MATCHES "Qt*")
            string(SUBSTRING ${f} 2 -1 QtVersion)
            message(STATUS "Found Qtfolder: ${r}/${f}, Version: ${QtVersion}")
            set(CMAKE_PREFIX_PATH "${r}/${f}/${QtVersion}/gcc_64/lib/cmake")
            break()
        endif ()
    endforeach ()
endforeach ()

# find packages
find_package(Gflags REQUIRED)                                   # gflags
find_package(Glog REQUIRED)                                     # glog
find_package(Boost COMPONENTS filesystem date_time REQUIRED)    # boost
find_package(Eigen3)                                            # Eigen
find_package(OpenCV 4)                                          # OpenCV
find_package(Qt5 COMPONENTS Core Widgets Gui REQUIRED)          # Qt

# dependency include directories and dependency libraries
list(APPEND PRIVATE_DEPEND_INCLUDES
        ${GFLAGS_INCLUDE_DIRS}
        ${GLOG_INCLUDE_DIRS}
        ${Boost_INCLUDE_DIRS}
        ${EIGEN3_INCLUDE_DIRS}
        ${OpenCV_INCLUDE_DIRS}
        )
list(APPEND PRIVATE_DEPEND_LIBS
        ${GFLAGS_LIBRARIES}
        ${GLOG_LIBRARIES}
        ${Boost_LIBRARIES}
        Qt5::Core Qt5::Widgets Qt5::Gui
        )


###############################################################################
# Project Setting
###############################################################################
# default built type
IF (NOT CMAKE_BUILD_TYPE)
    SET(CMAKE_BUILD_TYPE RELEASE)
ENDIF ()
message(STATUS "Build Type: ${CMAKE_BUILD_TYPE}")

# build setting
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)
# Qt build setting
set(CMAKE_INCLUDE_CURRENT_DIR ON)   # find includes in corresponding build directories
set(CMAKE_AUTOMOC ON)   # instruct cmake to run moc automacically when needed
set(CMAKE_AUTOUIC ON)   # create code from a list of Qt designer ui files
set(CMAKE_AUTORCC ON)   # create code from a resource files

# project name
set(PRJ_COMMON common)

# project include
list(APPEND PRIVATE_DEPEND_INCLUDES
        ${CMAKE_CURRENT_SOURCE_DIR}/common/include)

# add project directory
add_subdirectory(common)
add_subdirectory(Eg01_HelloWorld)
add_subdirectory(Eg02_OpenGLWindow)
add_subdirectory(Eg03_OpenGL3DRendering)