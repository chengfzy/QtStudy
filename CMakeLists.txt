cmake_minimum_required(VERSION 3.2)
project(QtStudy)
set(CMAKE_CXX_STANDARD 14)

# cmake modules
list(APPEND CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)
include(${PROJECT_SOURCE_DIR}/cmake/CMakeHelper.cmake)


###############################################################################
# Options
###############################################################################
# Qt Folder
if (UNIX)
    set(QtFolder "/opt;$ENV{HOME}/Programs;$ENV{HOME}/opt" CACHE STRING "Qt install folder")
elseif (WIN32)
    set(QtFolder "D:/Programs/Qt" CACHE STRING "Qt install folder")
endif ()
option(DebugTest "Debug test" OFF)

# set cmake path for windows
if (WIN32)
    set(CMAKE_PREFIX_PATH "${CMAKE_PREFIX_PATH};D:/usr/lib/cmake")
endif ()


###############################################################################
# Libraries
###############################################################################
# setting before find libraries
findaddqtpath(${QtFolder})

# find packages
find_package(Gflags REQUIRED)                                   # gflags
find_package(Glog REQUIRED)                                     # glog
find_package(fmt REQUIRED)                                      # fmt
find_package(Boost COMPONENTS filesystem date_time REQUIRED)    # boost
find_package(Eigen3)                                            # Eigen
find_package(Qt5 COMPONENTS Core Widgets Gui Network REQUIRED)  # Qt

# dependency include directories and dependency libraries
list(APPEND PRIVATE_DEPEND_INCLUDES
     ${GFLAGS_INCLUDE_DIRS}
     ${GLOG_INCLUDE_DIRS}
     ${Boost_INCLUDE_DIRS}
     ${EIGEN3_INCLUDE_DIRS}
     )
list(APPEND PRIVATE_DEPEND_LIBS
     ${GFLAGS_LIBRARIES}
     ${GLOG_LIBRARIES}
     ${Boost_LIBRARIES}
     fmt::fmt-header-only
     Qt5::Core Qt5::Widgets Qt5::Gui Qt5::Network
     )


###############################################################################
# Project Setting
###############################################################################
# default built type
if (NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE RELEASE)
endif ()
message(STATUS "Build Type: ${CMAKE_BUILD_TYPE}")

# build setting
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)
# Qt build setting
#set(CMAKE_INCLUDE_CURRENT_DIR ON)   # find includes in corresponding build directories
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
add_subdirectory(Eg04_TcpServer)
add_subdirectory(Eg05_ThreadedTcpServer)
add_subdirectory(Eg06_TcpAsyncClient)
add_subdirectory(Eg07_TcpSyncClient)
add_subdirectory(Eg08_UdpBroadcastSender)
add_subdirectory(Eg09_UdpBroadcastReceiver)
add_subdirectory(Eg10_UdpMulticastSender)
add_subdirectory(Eg11_UdpMulticastReceiver)
add_subdirectory(Eg12_UdpImageCapture)
add_subdirectory(Eg13_UdpImageProcessor)