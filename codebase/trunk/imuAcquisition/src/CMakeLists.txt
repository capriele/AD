include_directories(${PROJECT_SOURCE_DIR}/src
    ${LCM_INCLUDE_DIRS})

add_executable(imuAcquisition imuAcquisition.cpp)

pods_use_pkg_config_packages(imuAcquisition
	lcmtypes)


target_link_libraries(imuAcquisition
    ${LCM_LDFLAGS})


pods_install_executables(imuAcquisition)


