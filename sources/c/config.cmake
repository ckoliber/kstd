########################################################
# Start Install Configures
########################################################

# Install config
# install(
#     TARGETS ${KPS_PROJECT_NAME}
#     RUNTIME DESTINATION bin
#     INCLUDES DESTINATION include
#     LIBRARY DESTINATION lib
#     ARCHIVE DESTINATION lib
# )

########################################################
# End Install Configures
########################################################
#-------------------------------------------------------
#-------------------------------------------------------
#-------------------------------------------------------
########################################################
# Start Package Configures
########################################################

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

########################################################
# End Package Configures
########################################################
#-------------------------------------------------------
#-------------------------------------------------------
#-------------------------------------------------------
########################################################
# Start Target Configures
########################################################

# MakeFile Targets config
add_custom_target(build make -j4)
add_custom_target(start ./modules/app/app)
add_custom_target(daemon nohup ./modules/app/app > ../app.log &)
add_custom_target(cpack cpack)
add_custom_target(ctest ctest --verbose)

########################################################
# End Target Configures
########################################################

message("Enter command: make <build|start|daemon|cpack|ctest|install>")