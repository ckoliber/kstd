# variables:
#   LIBS: "..."
#   LINK: "dynamic|static"
# auto:
#   [APP]:  src/app.(c|cpp|cxx) (if not exists project is lib type)
#   SOURCE: src/x.(c|cpp|cxx)
#   HEADER: include/x.(h|hpp|hxx)
#   LIB:    lib/x.(so|a)
#   TEST:   test/x.test.(c|cpp|cxx)

# detect mode type
if(MODE STREQUAL "BUILD")
    message("Start building module")

    project(${MODULE_NAME})

    # message line
    message("                                                         ")

    # detect type of module: <executable|library>
    if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/src/app.c" OR EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/src/app.cpp" OR EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/src/app.cxx")
        message("Type: app")
        set(MODULE_TYPE "app")
    else()
        message("Type: lib")
        set(MODULE_TYPE "lib")
    endif()

    # detect type of link: <dynamic|static>
    if(NOT DEFINED LINK)
        message("Link: dynamic")
        set(LINK "dynamic")
    elseif(LINK STREQUAL "dynamic")
        message("Link: dynamic")
    elseif(LINK STREQUAL "static")
        message("Link: static")
    endif()

    # message line
    message("                                                         ")

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

    # target sources (.c, .cpp, .cxx)
    file(GLOB_RECURSE MODULE_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/src/*.c" "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp" "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cxx")
    target_sources(${MODULE_NAME} PUBLIC ${MODULE_SOURCES})
    
    # message sources
    foreach(MODULE_SOURCE ${MODULE_SOURCES})
        message("Source: ${MODULE_SOURCE}") 
    endforeach()

    # message line
    message("                                                         ")

    # target headers (.h, .hpp, .hxx)
    set(MODULE_HEADERS "${CMAKE_CURRENT_SOURCE_DIR}/include")
    target_include_directories(${MODULE_NAME} PUBLIC ${MODULE_HEADERS})

    # target libs (.so, .a)
    file(GLOB_RECURSE MODULE_LIBS "${CMAKE_CURRENT_SOURCE_DIR}/lib/*.so*" "${CMAKE_CURRENT_SOURCE_DIR}/lib/*.a")
    target_link_libraries(${MODULE_NAME} PUBLIC ${MODULE_LIBS})

    # message header
    foreach(MODULE_LIB ${MODULE_LIBS})
       message("Lib: ${MODULE_LIB}") 
    endforeach()

    # message line
    message("                                                         ")

    # target compiler (-O3, ...)
    set(OPTIONS -Wall -O0 -g)
    target_compile_options(${MODULE_NAME} PUBLIC ${OPTIONS})

    # enable ctest
    enable_testing()

    # remove app main file
    if(MODULE_TYPE STREQUAL "app")
        list(REMOVE_ITEM MODULE_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/src/app.c")
        list(REMOVE_ITEM MODULE_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/src/app.cpp")
        list(REMOVE_ITEM MODULE_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/src/app.cxx")
    endif()

    # target tests (.test.c, .test.cpp, .test.cxx)
    file(GLOB_RECURSE MODULE_TESTS "${CMAKE_CURRENT_SOURCE_DIR}/test/*.test.c" "${CMAKE_CURRENT_SOURCE_DIR}/test/*.test.cpp" "${CMAKE_CURRENT_SOURCE_DIR}/test/*.test.cxx")

    # message test
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
        target_sources(${TEST_NAME} PUBLIC ${MODULE_SOURCES} ${TEST_SOURCE})

        # test headers
        target_include_directories(${TEST_NAME} PUBLIC ${MODULE_HEADERS})

        # test libs
        target_link_libraries(${TEST_NAME} PUBLIC ${MODULE_LIBS})

        # add test
        add_test(NAME ${TEST_NAME} COMMAND ${TEST_NAME})
    endforeach()
elseif(MODE STREQUAL "LINK")
    message("Start linking module")

    message("Libs: ${LIBS}")

    # link ${LIBS} to project, tests
    target_link_libraries(${MODULE_NAME} PUBLIC ${LIBS})
endif()