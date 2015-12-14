include_directories(${PROJECT_SOURCE_DIR}/src
    ${LCM_INCLUDE_DIRS})

add_executable(remoteController remoteController.cpp)

pods_use_pkg_config_packages(remoteController
	lcmtypes)


target_link_libraries(remoteController
    ${LCM_LDFLAGS})


pods_install_executables(remoteController)


