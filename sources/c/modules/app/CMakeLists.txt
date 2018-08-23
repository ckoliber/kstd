project(app)
set(LINK_LIBS "-lpthread")

########################################################
# Don't change area !!!
########################################################

# type of module: <executable|library>
add_executable(${PROJECT_NAME} "")

# target sources (.c, .cpp, .cxx)
file(GLOB_RECURSE SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/src/*.c" "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp" "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cxx")
target_sources(${PROJECT_NAME} PUBLIC ${SOURCES})

# target headers (.h, .hpp, .hxx)
set(HEADERS "${CMAKE_CURRENT_SOURCE_DIR}/include")
target_include_directories(${PROJECT_NAME} PUBLIC ${HEADERS})

# target libs (.so, .a, LibraryName)
file(GLOB_RECURSE LIBS "${CMAKE_CURRENT_SOURCE_DIR}/lib/*.so*" "${CMAKE_CURRENT_SOURCE_DIR}/lib/*.a")
target_link_libraries(${PROJECT_NAME} PUBLIC ${LIBS} ${LINK_LIBS})

# target compiler (-O3, ...)
set(OPTIONS -Wall -O0 -g)
target_compile_options(${PROJECT_NAME} PUBLIC ${OPTIONS})

# enable ctest
enable_testing()
list(REMOVE_ITEM SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/src/app.c")
file(GLOB_RECURSE TESTS "${CMAKE_CURRENT_SOURCE_DIR}/test/*.test.c" "${CMAKE_CURRENT_SOURCE_DIR}/test/*.test.cpp" "${CMAKE_CURRENT_SOURCE_DIR}/test/*.test.cxx")

# iterate test's and add_executable, add_test
foreach(TEST ${TESTS})
    get_filename_component(TEST_NAME ${TEST} NAME)
    add_executable(${TEST_NAME} "")
    target_sources(${TEST_NAME} PUBLIC ${SOURCES} ${TEST})
    target_include_directories(${TEST_NAME} PUBLIC ${HEADERS})
    target_link_libraries(${TEST_NAME} PUBLIC ${LIBS} ${LINK_LIBS})
    add_test(NAME ${TEST_NAME} COMMAND ${TEST_NAME})
endforeach()

########################################################
# End of don't change area !!!
########################################################

