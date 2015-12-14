include_directories(${PROJECT_SOURCE_DIR}/src
    ${LCM_INCLUDE_DIRS})

add_executable(watchdog watchdog.cpp)

pods_use_pkg_config_packages(watchdog
	lcmtypes)


target_link_libraries(watchdog
    ${LCM_LDFLAGS})


pods_install_executables(watchdog)


