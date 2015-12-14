include_directories(${PROJECT_SOURCE_DIR}/src
    ${LCM_INCLUDE_DIRS})

add_executable(motorCommander motorCommander.cpp)

pods_use_pkg_config_packages(motorCommander
	lcmtypes)


target_link_libraries(motorCommander
    ${LCM_LDFLAGS})


pods_install_executables(motorCommander)


