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

# start module link tasks
MODULE_LIBS()