# superproject -> install, pack, config
# subproject -> build, test

cmake_minimum_required(VERSION 3.8)

# set compiler
if(LINUX)
    set( CMAKE_C_COMPILER "gcc" )
    set( CMAKE_CXX_COMPILER "g++" )
elseif(WIN32)
    set( CMAKE_C_COMPILER "C:/MinGW/bin/gcc.exe" )
    set( CMAKE_CXX_COMPILER "C:/MinGW/bin/g++.exe" )
endif()

# set build type
if(NOT BUILD STREQUAL "debug" AND NOT BUILD STREQUAL "release")
    message("Enter -DBUILD=<debug|release>")
    set(BUILD "debug")
endif()

# include package variables
include("${CMAKE_CURRENT_SOURCE_DIR}/package.cmake")

project(${KPS_PROJECT_NAME})

# find modules
file(GLOB MODULES "${CMAKE_CURRENT_SOURCE_DIR}/modules/*")

# add modules
foreach(MODULE_PATH ${MODULES})
    # get module name
    get_filename_component(MODULE_NAME ${MODULE_PATH} NAME)

    # message events
    message("Module: ${MODULE_NAME}")
    message("---------------------------------------------------------")

    # add module
    add_subdirectory(${MODULE_PATH})

    message("---------------------------------------------------------")
    message("                                                         ")
    message("                                                         ")
endforeach()

# include builder configs
include("${CMAKE_CURRENT_SOURCE_DIR}/config.cmake")