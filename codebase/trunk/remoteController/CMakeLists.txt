cmake_minimum_required(VERSION 2.6.0)

# pull in the pods macros. See cmake/pods.cmake for documentation
set(POD_NAME remoteController)
include(cmake/pods.cmake)

find_package(PkgConfig REQUIRED)

# Add this to be able to debug:
#add_definitions(-g)
add_definitions(-std=c++11)

pkg_check_modules(LCM REQUIRED lcm)

#tell cmake to build these subdirectories
add_subdirectory(src)
