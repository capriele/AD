include_directories(${PROJECT_SOURCE_DIR}/src
    ${LCM_INCLUDE_DIRS})

add_executable(testsender testsender.cpp)

pods_use_pkg_config_packages(testsender
	lcmtypes)


target_link_libraries(testsender
    ${LCM_LDFLAGS})


pods_install_executables(testsender)


