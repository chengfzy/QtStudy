cmake_minimum_required(VERSION 3.12)
project(QtStudy)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# cmake modules
list(APPEND CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)
include(${PROJECT_SOURCE_DIR}/cmake/CMakeHelper.cmake)


###############################################################################
# Options
###############################################################################
# Qt folder
if(UNIX)
    option(UseSystemQt "Use Qt in system" ON)
    if(NOT ${UseSystemQt})
        set(QtFolder "/opt;$ENV{HOME}/Programs;$ENV{HOME}/opt" CACHE PATH "Qt install folder")
    else()
        set(QtFolder "/usr/lib/x86_64-linux-gnu/cmake" CACHE PATH "Qt system folder")
    endif()
elseif(WIN32)
    set(QtFolder "D:/Programs/Qt" CACHE PATH "Qt install folder")
endif()

# set cmake path for windows
if(WIN32)
    set(CMAKE_PREFIX_PATH "${CMAKE_PREFIX_PATH};D:/usr/lib/cmake")
endif()


###############################################################################
# Libraries
###############################################################################
# setting before find libraries
FindAddQtPath(${QtFolder})

# find packages
find_package(Gflags REQUIRED)                                   # gflags
find_package(Glog REQUIRED)                                     # glog
find_package(fmt REQUIRED)                                      # fmt

# dependency include directories and dependency libraries
list(APPEND DEPEND_INCLUDES
    ${GFLAGS_INCLUDE_DIRS}
    ${GLOG_INCLUDE_DIRS}
)
list(APPEND DEPEND_LIBS
    ${GFLAGS_LIBRARIES}
    ${GLOG_LIBRARIES}
    fmt::fmt-header-only
)

# Qt
find_package(QT NAMES Qt6 Qt5 COMPONENTS Core)
message(STATUS "Qt Version: ${QT_VERSION}")
if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Core Widgets Gui Qml Quick OpenGL OpenGLWidgets Network Charts
        PrintSupport
        REQUIRED
    )
    list(APPEND DEPEND_LIBS Qt::Core Qt::Widgets Qt::Qml Qt::Quick Qt::OpenGL Qt::OpenGLWidgets Qt::Network Qt::Charts
        Qt::PrintSupport
    )
else()
    find_package(Qt5 COMPONENTS Core Widgets Gui Qml Quick OpenGL Network Charts PrintSupport REQUIRED)
    list(APPEND DEPEND_LIBS Qt::Core Qt::Widgets Qt::Gui Qt::Qml Qt::Quick Qt::OpenGL Qt::Network Qt::Charts
        Qt::PrintSupport
    )
endif()

###############################################################################
# Project Setting
###############################################################################
# default built type
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE RELEASE)
endif()
message(STATUS "Build Type: ${CMAKE_BUILD_TYPE}")

# build setting
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)
# Qt build setting
set(CMAKE_AUTOMOC ON)   # instruct cmake to run moc automacically when needed
set(CMAKE_AUTOUIC ON)   # create code from a list of Qt designer ui files
set(CMAKE_AUTORCC ON)   # create code from a resource files


###############################################################################
# Project
###############################################################################
# third party
list(APPEND DEPEND_INCLUDES
    ${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/QCustomPlot/include
)

add_subdirectory(3rdparty)

# add modules
add_subdirectory(modules)