# variables:
#   MODULES:    "..."
#   LIBS:       "..."
#   LINK:       "dynamic|static"
# auto:
#   [APP]:      src/app.(c|cpp|cxx) (if not exists project is lib type)
#   SOURCE:     src/x.(c|cpp|cxx)
#   HEADER:     include/x.(h|hpp|hxx)
#   LIB:        lib/x.(so|a)
#   TEST:       test/x.test.(c|cpp|cxx)

# debug create module exec or lib
macro(MODULE_CREATE)
    # detect type of module: <executable|library>
    if(EXISTS "${MODULE_PATH}/src/app.c" OR EXISTS "${MODULE_PATH}/src/app.cpp" OR EXISTS "${MODULE_PATH}/src/app.cxx")
        set(MODULE_TYPE "app")
    else()
        set(MODULE_TYPE "lib")
    endif()
    message("Debug type: ${MODULE_TYPE}")

    # detect type of link: <dynamic|static>
    if(NOT LINK STREQUAL "dynamic" AND NOT LINK STREQUAL "static")
        set(LINK "dynamic")
    endif()
    message("Debug link: ${LINK}")

    # create module exec or lib
    if(MODULE_TYPE STREQUAL "app")
        # create exec
        add_executable(${MODULE_NAME} "")
    elseif(MODULE_TYPE STREQUAL "lib")
        # create lib based on link type
        if(LINK STREQUAL "dynamic")
            add_library(${MODULE_NAME} SHARED "")
        elseif(LINK STREQUAL "static")
            add_library(${MODULE_NAME} STATIC "")
        endif()
    endif()
endmacro()

# add module sources
macro(MODULE_SOURCES)
    message("                                                         ")
    
    # target sources (.c, .cpp, .cxx)
    file(GLOB_RECURSE MODULE_SOURCES "${MODULE_PATH}/src/*.c" "${MODULE_PATH}/src/*.cpp" "${MODULE_PATH}/src/*.cxx")
    target_sources(${MODULE_NAME} PRIVATE ${MODULE_SOURCES})
    
    # message sources
    foreach(MODULE_SOURCE ${MODULE_SOURCES})
        message("Source: ${MODULE_SOURCE}")
    endforeach()
endmacro()

# add module headers
macro(MODULE_HEADERS)
    message("                                                         ")

    # target headers (.h, .hpp, .hxx)
    set(MODULE_HEADERS "${MODULE_PATH}/include")
    target_include_directories(${MODULE_NAME} PUBLIC ${MODULE_HEADERS})

    # message headers
    foreach(MODULE_HEADER ${MODULE_HEADERS})
        message("Header: ${MODULE_HEADER}")
    endforeach()
endmacro()

# add module tests
macro(MODULE_TESTS)
    message("                                                         ")

    # target tests (.test.c, .test.cpp, .test.cxx)
    file(GLOB_RECURSE MODULE_TESTS "${MODULE_PATH}/test/*.test.c" "${MODULE_PATH}/test/*.test.cpp" "${MODULE_PATH}/test/*.test.cxx")

    # message tests
    foreach(MODULE_TEST ${MODULE_TESTS})
       message("Test: ${MODULE_TEST}") 
    endforeach()

    # iterate test's and add_executable, add_test
    foreach(TEST_SOURCE ${MODULE_TESTS})
        # get test name
        get_filename_component(TEST_NAME ${TEST_SOURCE} NAME)

        # test executable
        add_executable(${TEST_NAME} "")

        # test sources
        target_sources(${TEST_NAME} PRIVATE ${TEST_SOURCE})

        # test libs
        target_link_libraries(${TEST_NAME} PRIVATE ${MODULE_NAME})

        # add test
        add_test(NAME ${TEST_NAME} COMMAND ${TEST_NAME})
    endforeach()

endmacro()

# add module libs
macro(MODULE_LIBS)
    message("                                                         ")
    
    # target libs (.so, .a)
    file(GLOB_RECURSE MODULE_LIBS "${MODULE_PATH}/lib/*.so*" "${MODULE_PATH}/lib/*.a")
    target_link_libraries(${MODULE_NAME} PUBLIC ${MODULE_LIBS} ${LIBS} ${MODULES})

    # message libs
    foreach(MODULE_LIB ${MODULE_LIBS})
       message("Library: ${MODULE_LIB}") 
    endforeach()

    # message links
    foreach(MODULE_LINK ${LIBS})
       message("Link: ${MODULE_LINK}") 
    endforeach()

    # message modules
    foreach(MODULE_MODULE ${MODULES})
        message("Module: ${MODULE_MODULE}") 
    endforeach()
endmacro()

# add module compiler options
macro(MODULE_OPTIONS)
    message("                                                         ")

    # target compiler (-O3, ...)
    set(MODULE_OPTIONS -Wall -O0 -g -fPIC -Wall -Werror -pedantic -std=c11)
    target_compile_options(${MODULE_NAME} PRIVATE ${MODULE_OPTIONS})

    # message options
    foreach(MODULE_OPTION ${MODULE_OPTIONS})
       message("Option: ${MODULE_OPTION}") 
    endforeach()
endmacro()



# release create module exec or lib
macro(MODULE_RELEASE)
    set(MODULE_NAME ${KPS_PROJECT_NAME})

    # detect type of module: <executable|library>
    file(GLOB_RECURSE RELEASE_APP "${CMAKE_CURRENT_SOURCE_DIR}/modules/*/app.c" "${CMAKE_CURRENT_SOURCE_DIR}/modules/*/app.cpp" "${CMAKE_CURRENT_SOURCE_DIR}/modules/*/app.cxx")
    if(RELEASE_APP)
        set(MODULE_TYPE "app")
    else()
        set(MODULE_TYPE "lib")
    endif()
    message("Release type: ${MODULE_TYPE}")

    # detect type of link: <dynamic|static>
    if(BUILD STREQUAL "release_dynamic")
        set(LINK "dynamic")
    elseif(BUILD STREQUAL "release_static")
        set(LINK "static")
    endif()
    message("Release link: ${LINK}")

    # create module exec or lib
    if(MODULE_TYPE STREQUAL "app")
        # create exec
        add_executable(${MODULE_NAME} "")
    elseif(MODULE_TYPE STREQUAL "lib")
        # create lib based on link type
        if(LINK STREQUAL "dynamic")
            add_library(${MODULE_NAME} SHARED "")
        elseif(LINK STREQUAL "static")
            add_library(${MODULE_NAME} STATIC "")
        endif()
    endif()
endmacro()

# debug build module
macro(MODULE_BUILD)
    # if project is debug (compile all modules), else pack all sources and headers
    if(BUILD STREQUAL "debug")
        MODULE_CREATE()
        MODULE_SOURCES()
        MODULE_HEADERS()
        MODULE_TESTS()
        MODULE_OPTIONS()
    else()
        set(MODULE_NAME ${KPS_PROJECT_NAME})
        MODULE_SOURCES()
        MODULE_HEADERS()
        MODULE_OPTIONS()
    endif()
endmacro()

# debug and release link module
macro(MODULE_LINK)
    if(NOT BUILD STREQUAL "debug")
        set(MODULE_NAME ${KPS_PROJECT_NAME})
        set(MODULES )
    endif()
    MODULE_LIBS()
endmacro()



# start module build tasks
if(TYPE STREQUAL "release")
    MODULE_RELEASE()
elseif(TYPE STREQUAL "build")
    MODULE_BUILD()
elseif(TYPE STREQUAL "link")
    MODULE_LINK()
endif()