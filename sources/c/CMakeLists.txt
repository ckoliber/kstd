# superproject -> install, pack, config
# subproject -> build, test

# one sub project -> add_executable (app)
# other sub projects -> add_library (etc)

cmake_minimum_required(VERSION 3.8)

if(BUILD STREQUAL "debug" OR BUILD STREQUAL "release")
    include("${CMAKE_CURRENT_SOURCE_DIR}/package.cmake")

    project(${KPS_PROJECT_NAME})

    add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/modules/app")
    # add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/modules/ui")
    # add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/modules/core")
    # add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/modules/net")

    include("${CMAKE_CURRENT_SOURCE_DIR}/config.cmake")
else()
    message("Enter -DBUILD=<debug|release>")
endif()