# find Qt folder and add it to cmake search path, cannot use function() due to CMAKE_PREFIX_PATH will be a local variable
# find Qt folder and add it to cmake search path, cannot use function() due to CMAKE_PREFIX_PATH will be a local variable
macro(FindAddQtPath QtPaths)
    # check windows arch and VC version
    if(WIN32)
        CheckWinCompiler()
        # VC suffix
        if(${VCVersion} STREQUAL "VC2015")
            set(_QtVcSuffix "msvc2015")
        elseif(${VCVersion} STREQUAL "VC2017")
            set(_QtVcSuffix "msvc2017")
        elseif(${VCVersion} STREQUAL "VC2019")
            set(_QtVcSuffix "msvc2017")
        endif()
        # arch suffix
        if(${VCArch} STREQUAL "x64")
            set(_QtArchSuffix "_64")
        elseif(${VCArch} STREQUAL "x86")
            set(_QtArchSuffix "")
        endif()                
    endif()

    # check input candidate folder and add it to cmake search path
    foreach(r ${ARGV})
        if(${r} MATCHES "cmake")
            message(STATUS "Add Qt path: ${r}")
            list(APPEND CMAKE_PREFIX_PATH "${CMAKE_PREFIX_PATH};${r}")
        else()
            # find Qt folder
            set(_QtRootFolder "${r}/Qt")
            file(GLOB FOLDER RELATIVE ${_QtRootFolder} ${_QtRootFolder}/*)
            foreach(f ${FOLDER})
                if(IS_DIRECTORY ${_QtRootFolder}/${f} AND ${f} MATCHES "[0-9.]+")
                    # parse Qt version and set Qt path
                    string(SUBSTRING ${f} 0 -1 _QtVersion)
                    if(UNIX)
                        set(_QtCMakePath "${_QtRootFolder}/${f}/gcc_64/lib/cmake")
                    elseif(WIN32)
                        set(_QtCMakePath "${_QtRootFolder}/${f}/${_QtVcSuffix}${_QtArchSuffix}")
                    endif()
                    # check path exist and add to CMake path
                    if(IS_DIRECTORY ${_QtCMakePath})
                        message(STATUS "Found Qt ${_QtVersion} in ${_QtRootFolder}/${f}")
                        message(STATUS "Add Qt path: ${_QtCMakePath}")
                        list(APPEND CMAKE_PREFIX_PATH "${CMAKE_PREFIX_PATH};${_QtCMakePath}")
                        break()
                    endif()
                endif()
            endforeach()
        endif()
    endforeach()
endmacro()
