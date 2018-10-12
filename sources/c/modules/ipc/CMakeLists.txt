set(LIBS "memory dsa local processor")
set(LINK "dynamic")

if(MODE STREQUAL "BUILD")
    include("${CMAKE_CURRENT_SOURCE_DIR}/../../module.cmake")
elseif(MODE STREQUAL "LINK")
    include("${CMAKE_CURRENT_SOURCE_DIR}/module.cmake")
endif()