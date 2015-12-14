include_directories(${PROJECT_SOURCE_DIR}/src
    ${LCM_INCLUDE_DIRS})

add_executable(examplePod examplePod.cpp)

pods_use_pkg_config_packages(examplePod
	lcmtypes)


target_link_libraries(examplePod
    ${LCM_LDFLAGS})


pods_install_executables(examplePod)


