include_directories(${PROJECT_SOURCE_DIR}/src
    ${LCM_INCLUDE_DIRS})

add_executable(stateEstimatorOrientV1 stateEstimatorOrientV1.cpp)

pods_use_pkg_config_packages(stateEstimatorOrientV1
	lcmtypes)


target_link_libraries(stateEstimatorOrientV1
    ${LCM_LDFLAGS})


pods_install_executables(stateEstimatorOrientV1)


