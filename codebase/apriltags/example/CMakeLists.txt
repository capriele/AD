cmake_minimum_required (VERSION 2.6)

link_libraries(apriltags)
include_directories(/usr/include/flycapture)


add_executable(apriltags_demo apriltags_demo.cpp Serial.cpp)
pods_install_executables(apriltags_demo)

add_executable(imu imu.cpp Serial.cpp)
pods_install_executables(imu)

add_executable(april_locator april_locator.cpp)
pods_install_executables(april_locator)

find_path(FLY_CAPTURE_PATH flycapture REQUIRED)
find_LIBRARY (FLYCAPTURE_LIB flycapture /usr/include/ REQUIRED)
target_link_libraries(april_locator ${FLYCAPTURE_LIB})
