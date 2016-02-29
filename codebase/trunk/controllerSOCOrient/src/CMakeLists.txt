include_directories(${PROJECT_SOURCE_DIR}/src
    ${LCM_INCLUDE_DIRS})

add_executable(controllerSOCOrient controllerSOCOrient.cpp)

pods_use_pkg_config_packages(controllerSOCOrient
	lcmtypes)


target_link_libraries(controllerSOCOrient
    ${LCM_LDFLAGS} c3 /home/ra/tools/c3sc/lib/libc3sc.so)


pods_install_executables(controllerSOCOrient)


