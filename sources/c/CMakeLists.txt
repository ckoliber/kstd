# superproject -> install, pack, config
# subproject -> build, test

# one sub project -> add_executable (app)
# other sub projects -> add_library (etc)

cmake_minimum_required(VERSION 3.8)

if(LINUX)
    set( CMAKE_C_COMPILER "gcc" )
    set( CMAKE_CXX_COMPILER "g++" )
elseif(WIN32)
    set( CMAKE_C_COMPILER "C:/Qt/Qt5.10.0/Tools/mingw530_32/bin/gcc.exe" )
    set( CMAKE_CXX_COMPILER "C:/Qt/Qt5.10.0/Tools/mingw530_32/bin/g++.exe" )
endif()

if(NOT BUILD STREQUAL "debug" AND NOT BUILD STREQUAL "release")
    message("Enter -DBUILD=<debug|release>")
    set(BUILD "debug")
endif()

include("${CMAKE_CURRENT_SOURCE_DIR}/package.cmake")

project(${KPS_PROJECT_NAME})

add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/modules/app")

add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/modules/algo")
add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/modules/dsa")
add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/modules/io")
add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/modules/ipc")
add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/modules/local")
add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/modules/memory")
add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/modules/processor")

include("${CMAKE_CURRENT_SOURCE_DIR}/config.cmake")