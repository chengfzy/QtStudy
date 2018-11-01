cmake_minimum_required(VERSION 3.2)
project(QtStudy)
set(CMAKE_CXX_STANDARD 14)

# cmake modules
list(APPEND CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)


# projects build setting
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)
# Qt build setting
set(CMAKE_INCLUDE_CURRENT_DIR ON)   # find includes in corresponding build directories
set(CMAKE_AUTOMOC ON)   # instruct cmake to run moc automacically when needed
set(CMAKE_AUTOUIC ON)   # create code from a list of Qt designer ui files
set(CMAKE_AUTORCC ON)   # create code from a resource files


# library: boost
option(BUILD_USE_SYSTEM_BOOST "Use system boost in /usr/lib" ON)
if (BUILD_USE_SYSTEM_BOOST)
    message(STATUS "USE system boost in /usr/lib/")
    # boost: bypass default path for search another boost in /usr/lib
    # set(BOOST_NO_SYSTEM_PATHS FALSE)
    set(BOOST_ROOT "/usr")
    set(BOOST_INCLUDEDIR "${BOOST_ROOT}/include")
    set(BOOST_LIBRARYDIR "${BOOST_ROOT}/lib/x86_64-linux-gnu")
    find_package(Boost REQUIRED COMPONENTS filesystem date_time)
else ()
    message(STATUS "Use Boost in /usr/local")
    find_package(Boost REQUIRED COMPONENTS filesystem date_time REQUIRED)
endif ()
if (NOT Boost_FOUND)
    message(FATAL_ERROR "Cannot Found Boost")
endif ()
message(STATUS "Boost headers: ${Boost_INCLUDE_DIR}")
message(STATUS "Boost libs: ${Boost_LIBRARIES}")

# library: Qt
#set(CMAKE_PREFIX_PATH "/home/jeffery/Programs/Qt5.10.1/5.10.1/gcc_64")
find_package(Qt5 COMPONENTS Core Widgets Gui REQUIRED)


# libraries: others
include_directories(${Boost_INCLUDE_DIRS})
set(THIRD_LIBS ${THIRD_LIBS}
        gflags glog
        ${Boost_LIBRARIES}
        Qt5::Core Qt5::Widgets Qt5::Gui)


# inclue common folder
include_directories(common)

# Example
add_subdirectory(Eg01_HelloWorld)
add_subdirectory(Eg02_OpenGLWindow)
add_subdirectory(Eg03_OpenGL3DRendering)