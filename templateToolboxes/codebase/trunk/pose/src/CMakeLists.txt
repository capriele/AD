include_directories(${PROJECT_SOURCE_DIR}/src
    ${LCM_INCLUDE_DIRS})

add_executable(pose pose.cpp)

pods_use_pkg_config_packages(pose
	lcmtypes)


target_link_libraries(pose
    ${LCM_LDFLAGS})


pods_install_executables(pose)


