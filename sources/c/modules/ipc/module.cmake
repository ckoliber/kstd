set(MODULES memory dsa local processor kstd)
if(ANDROID)
    set(LIBS "")
elseif(WIN32)
    set(LIBS "")
else()
    set(LIBS "-lrt")
endif()
set(LINK "static")