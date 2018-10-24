set(MODULES memory dsa local processor kstd)

if(WIN32 OR ANDROID OR APPLE)
    set(LIBS "")
else()
    set(LIBS "-lrt")
endif()
set(LINK "static")