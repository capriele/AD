include_directories(${PROJECT_SOURCE_DIR}/src
    ${LCM_INCLUDE_DIRS})

add_executable(controllerPDPose controllerPDPose.cpp)

pods_use_pkg_config_packages(controllerPDPose
	lcmtypes)


target_link_libraries(controllerPDPose
    ${LCM_LDFLAGS})


pods_install_executables(controllerPDPose)


