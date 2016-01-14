pods_install_pkg_config_file(viewer
    CFLAGS
    LIBS 
    REQUIRES ${REQUIRED_PACKAGES}
    VERSION 0.0.1)

include_directories(${PROJECT_SOURCE_DIR}/src
    ${GTK2_INCLUDE_DIRS}
    ${OPENGL_INCLUDE_DIR}
    ${GLUT_INCLUDE_DIR}
    ${LCM_INCLUDE_DIRS}
    ${BOT2_VIS_INCLUDE_DIRS})

add_executable(viewer viewer.cpp)

pods_use_pkg_config_packages(viewer viewer 
	lcmtypes
	bot2-core 
	bot2-vis
	renderers)

target_link_libraries(viewer
    ${GTK2_LDFLAGS}
    ${OPENGL_LIBRARIES}
    ${GLUT_LIBRARIES}
    ${LCM_LDFLAGS}
    ${BOT2_VIS_LDFLAGS})

pods_install_executables(viewer)
