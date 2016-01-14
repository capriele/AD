cmake_minimum_required(VERSION 2.6.0)

# pull in the pods macros. See cmake/pods.cmake for documentation
set(POD_NAME pose)
include(cmake/pods.cmake)

find_package(PkgConfig REQUIRED)

# Add this to be able to debug:
#add_definitions(-g)

pkg_check_modules(LCM REQUIRED lcm)

#tell cmake to build these subdirectories
add_subdirectory(src)
