pods_install_pkg_config_file(detectorVIS
    CFLAGS
    LIBS 
    REQUIRES ${REQUIRED_PACKAGES}
    VERSION 0.0.1)

include_directories(${PROJECT_SOURCE_DIR}/src
    ${LCM_INCLUDE_DIRS}
    # ${OpenCV_INCLUDE_DIRS})
    )

# find_path (OPENCV_PATH opencv REQUIRED)

add_executable(detectorVIS detectorVIS.cpp)

pods_use_pkg_config_packages(detectorVIS detectorVIS 
	lcmtypes)

target_link_libraries(detectorVIS
    ${LCM_LDFLAGS}
    apriltags)

# target_link_libraries(detectorVIS opencv_core.so opencv_highgui.so opencv_imgproc.so opencv_calib3d.so opencv_features2d.so)

pods_install_executables(detectorVIS)
