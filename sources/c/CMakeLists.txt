cmake_minimum_required(VERSION 3.8)

# include package variables
include("${CMAKE_CURRENT_SOURCE_DIR}/package.cmake")
project(${KPS_PROJECT_NAME})



# configure compiler
macro(CONFIG_COMPILER)
    message("=================================================")
    message("Configure compiler")
    message("-------------------------------------------------")

    # compiler config
    if(LINUX)
        set( CMAKE_C_COMPILER "gcc" )
        set( CMAKE_CXX_COMPILER "g++" )
        message("Linux C compiler: ${CMAKE_C_COMPILER}")
        message("Linux CXX compiler: ${CMAKE_CXX_COMPILER}")
    elseif(UNIX)
        set( CMAKE_C_COMPILER "gcc" )
        set( CMAKE_CXX_COMPILER "g++" )
        message("Unix C compiler: ${CMAKE_C_COMPILER}")
        message("Unix CXX compiler: ${CMAKE_CXX_COMPILER}")
    elseif(APPLE)
        set( CMAKE_C_COMPILER "gcc" )
        set( CMAKE_CXX_COMPILER "g++" )
        message("Apple C compiler: ${CMAKE_C_COMPILER}")
        message("Apple CXX compiler: ${CMAKE_CXX_COMPILER}")
    elseif(ANDROID)
        set( CMAKE_C_COMPILER "gcc" )
        set( CMAKE_CXX_COMPILER "g++" )
        message("Android C compiler: ${CMAKE_C_COMPILER}")
        message("Android CXX compiler: ${CMAKE_CXX_COMPILER}")
    elseif(WIN32)
        set( CMAKE_C_COMPILER "C:/MinGW/bin/gcc.exe" )
        set( CMAKE_CXX_COMPILER "C:/MinGW/bin/g++.exe" )
        message("Win32 C compiler: ${CMAKE_C_COMPILER}")
        message("Win32 CXX compiler: ${CMAKE_CXX_COMPILER}")
    endif()

    message("=================================================")
    message("                                                 ")
endmacro()

# configure build type
macro(CONFIG_BUILD)
    message("=================================================")
    message("Configure build")
    message("-------------------------------------------------")

    # check set BUILD variable <debug|release_dynamic|release_static>
    if(NOT BUILD STREQUAL "debug" AND NOT BUILD STREQUAL "release_dynamic" AND NOT BUILD STREQUAL "release_static")
        message("Enter -DBUILD=<debug|release_dynamic|release_static>")
        set(BUILD "debug")
    endif()
    message("Build type: ${BUILD}")

    # project release create
    if(BUILD STREQUAL "release_dynamic" OR BUILD STREQUAL "release_static")
        set(TYPE "release")
        include("${CMAKE_CURRENT_SOURCE_DIR}/module.cmake")
    endif()

    message("=================================================")
    message("                                                 ")
endmacro()



# build modules
macro(BUILD_MODULES)
    message("=================================================")
    message("Building modules")
    message("-------------------------------------------------")

    # find modules
    file(GLOB MODULES "${CMAKE_CURRENT_SOURCE_DIR}/modules/*")

    # build modules
    foreach(MODULE_PATH ${MODULES})
        # get module name
        get_filename_component(MODULE_NAME ${MODULE_PATH} NAME)

        # check module is valid
        if(EXISTS "${MODULE_PATH}/module.cmake")
            # message events
            message("Module: ${MODULE_NAME}")
            message("---------------------------------------------------------")

            # build module
            set(TYPE "build")
            include("${MODULE_PATH}/module.cmake")
            include("${CMAKE_CURRENT_SOURCE_DIR}/module.cmake")

            message("---------------------------------------------------------")
        endif()
    endforeach()

    message("=================================================")
    message("                                                 ")
endmacro()

# link modules (circular dependency problem solving)
macro(LINK_MODULES)
    message("=================================================")
    message("Linking modules")
    message("-------------------------------------------------")

    # find modules
    file(GLOB MODULES "${CMAKE_CURRENT_SOURCE_DIR}/modules/*")

    # link modules
    foreach(MODULE_PATH ${MODULES})
        # get module name
        get_filename_component(MODULE_NAME ${MODULE_PATH} NAME)

        # check module is valid
        if(EXISTS "${MODULE_PATH}/module.cmake")
            # message events
            message("Module: ${MODULE_NAME}")
            message("---------------------------------------------------------")

            # link module
            set(TYPE "link")
            include("${MODULE_PATH}/module.cmake")
            include("${CMAKE_CURRENT_SOURCE_DIR}/module.cmake")

            message("---------------------------------------------------------")
        endif()
    endforeach()

    message("=================================================")
    message("                                                 ")
endmacro()



# set install
macro(SET_INSTALL)
    message("=================================================")
    message("Setting Install")
    message("-------------------------------------------------")
    
    # Install config
    # install(
    #     TARGETS ${KPS_PROJECT_NAME}
    #     RUNTIME DESTINATION bin
    #     INCLUDES DESTINATION include
    #     LIBRARY DESTINATION lib
    #     ARCHIVE DESTINATION lib
    # )

    message("=================================================")
    message("                                                 ")
endmacro()

# set ctest
macro(SET_CTEST)
    message("=================================================")
    message("Setting CTest")
    message("-------------------------------------------------")

    # CTest config
    enable_testing()
    include(CTest)

    message("=================================================")
    message("                                                 ") 
endmacro()

# set cpack
macro(SET_CPACK)
    message("=================================================")
    message("Setting CPack")
    message("-------------------------------------------------")

    # CPack config
    include(InstallRequiredSystemLibraries)
    set(CPACK_PACKAGE_NAME ${KPS_PROJECT_NAME})
    set(CPACK_PACKAGE_DESCRIPTION ${KPS_PROJECT_DESCRIPTION})
    set(CPACK_PACKAGE_VENDOR ${KPS_PROJECT_AUTHOR})
    set(CPACK_PACKAGE_CONTACT ${KPS_PROJECT_REPOSITORY})
    set(CPACK_PACKAGE_VERSION_MAJOR ${KPS_PROJECT_VERSION_MAJOR})
    set(CPACK_PACKAGE_VERSION_MINOR ${KPS_PROJECT_VERSION_MINOR})
    set(CPACK_PACKAGE_VERSION_PATCH ${KPS_PROJECT_VERSION_PATCH})
    # set(CPACK_PACKAGE_ICON "")
    set(CPACK_PACKAGE_FILE_NAME "${KPS_PROJECT_NAME}-${KPS_PROJECT_VERSION_MAJOR}.${KPS_PROJECT_VERSION_MINOR}.${KPS_PROJECT_VERSION_PATCH}")
    include(CPack)

    message("=================================================")
    message("                                                 ") 
endmacro()



# start cmake tasks
CONFIG_COMPILER()
CONFIG_BUILD()

BUILD_MODULES()
LINK_MODULES()

SET_INSTALL()
SET_CTEST()
SET_CPACK()