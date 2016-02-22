include_directories(${PROJECT_SOURCE_DIR}/src
    ${LCM_INCLUDE_DIRS})

add_executable(detectorVIS detectorVIS.cpp)

pods_use_pkg_config_packages(detectorVIS
	lcmtypes)


target_link_libraries(detectorVIS
    ${LCM_LDFLAGS})


pods_install_executables(detectorVIS)


