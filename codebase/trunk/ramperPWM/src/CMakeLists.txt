include_directories(${PROJECT_SOURCE_DIR}/src
    ${LCM_INCLUDE_DIRS})

add_executable(ramperPWM ramperPWM.cpp)

pods_use_pkg_config_packages(ramperPWM
	lcmtypes)


target_link_libraries(ramperPWM
    ${LCM_LDFLAGS})


pods_install_executables(ramperPWM)


