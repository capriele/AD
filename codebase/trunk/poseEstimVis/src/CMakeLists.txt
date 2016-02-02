include_directories(${PROJECT_SOURCE_DIR}/src
    ${LCM_INCLUDE_DIRS})

add_executable(poseEstimVis poseEstimVis.cpp)

pods_use_pkg_config_packages(poseEstimVis
	lcmtypes)


target_link_libraries(poseEstimVis
    ${LCM_LDFLAGS})


pods_install_executables(poseEstimVis)


