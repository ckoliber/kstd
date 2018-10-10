project(app)

if(ANDROID)
    set(LIBS "-lpthread")
elseif(WIN32)
    set(LIBS "")
else()
    set(LIBS "-pthread")
endif()

set(LINK "dynamic")

include("${CMAKE_CURRENT_SOURCE_DIR}/../../module.cmake")