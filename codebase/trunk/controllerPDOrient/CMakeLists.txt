cmake_minimum_required(VERSION 2.6.0)

if (CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
	find_file(_build_dir build PATHS ${PROJECT_SOURCE_DIR} ${PROJECT_SOURCE_DIR}/.. ${PROJECT_SOURCE_DIR}/../.. ${PROJECT_SOURCE_DIR}/../../.. ${PROJECT_SOURCE_DIR}/../../../..)
	if (_build_dir)
		set(CMAKE_INSTALL_PREFIX "${_build_dir}" CACHE STRING
		"install prefix" FORCE)
	else()
		execute_process(COMMAND ${CMAKE_COMMAND} -E make_directory ${PROJECT_SOURCE_DIR}/build)
		set(CMAKE_INSTALL_PREFIX ${PROJECT_SOURCE_DIR}/build
		CACHE STRING "install prefix" FORCE)
	endif()
endif()
message(STATUS CMAKE_INSTALL_PREFIX=${CMAKE_INSTALL_PREFIX})


# pull in the pods macros. See cmake/pods.cmake for documentation
set(POD_NAME controllerPDOrient)
include(cmake/pods.cmake)

find_package(PkgConfig REQUIRED)

# Add this to be able to debug:
#add_definitions(-g)
add_definitions(-std=c++11)

pkg_check_modules(LCM REQUIRED lcm)

#tell cmake to build these subdirectories
add_subdirectory(src)
