include_directories(${PROJECT_SOURCE_DIR}/src
    ${LCM_INCLUDE_DIRS})

add_executable(controllerPDOrient controllerPDOrient.cpp)

pods_use_pkg_config_packages(controllerPDOrient
	lcmtypes)


target_link_libraries(controllerPDOrient
    ${LCM_LDFLAGS})


pods_install_executables(controllerPDOrient)


