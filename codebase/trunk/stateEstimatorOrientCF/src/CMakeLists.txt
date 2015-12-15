include_directories(${PROJECT_SOURCE_DIR}/src
    ${LCM_INCLUDE_DIRS})

add_executable(stateEstimatorOrientCF stateEstimatorOrientCF.cpp)

pods_use_pkg_config_packages(stateEstimatorOrientCF
	lcmtypes)


target_link_libraries(stateEstimatorOrientCF
    ${LCM_LDFLAGS})


pods_install_executables(stateEstimatorOrientCF)


