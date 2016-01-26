cmake_minimum_required(VERSION 2.8.11)
project (teste)



find_path (OPENCV_PATH opencv  REQUIRED)
find_path (FLY_CAPTURE_PATH flycapture  REQUIRED)


find_LIBRARY (FLYCAPTURE_LIB flycapture /usr/local/lib REQUIRED)


add_executable(recorder recorder.cpp)

#target_link_libraries(camera opencv_core.so  opencv_highgui.so opencv_imgproc.so opencv_tegra.so opencv_calib3d.so opencv_features2d.so)
target_link_libraries(recorder opencv_core.so  opencv_highgui.so opencv_imgproc.so opencv_calib3d.so opencv_features2d.so)
target_link_libraries(recorder ${FLYCAPTURE_LIB})
