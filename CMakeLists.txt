cmake_minimum_required(VERSION 2.8.0)
project(agile-drones-superbuild)

# PODs out-of-source build logic
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

include(ExternalProject)


# list *compilation* dependencies, in alphabetical order by target (note: dependencies must come first in my foreach above)
set(lcm_dependencies gtk)
set(libbot_dependencies lcm)

# download information, in alphabetical order
# set(cmake_GIT_REPOSITORY https://github.com/RobotLocomotion/cmake.git)
# set(cmake_GIT_TAG 2d8eebdaf3a7e7a7125c1190462bc02fb3a83824)
# set(cmake_GIT_CLONE_DIR "${PROJECT_SOURCE_DIR}/cmake")
# set(cmake_NO_BUILD TRUE)
# set(cmake_IS_PUBLIC TRUE)
set(gtk_GIT_REPOSITORY https://github.com/RobotLocomotion/gtk-pod.git)
set(gtk_GIT_TAG 78a110023b527e6ee372e4686118a8c7a6a51131)
set(gtk_IS_CMAKE_POD TRUE)
set(gtk_IS_PUBLIC TRUE)
set(lcm_GIT_REPOSITORY https://github.com/RobotLocomotion/lcm-pod.git)
set(lcm_GIT_TAG 23876258aaf1c29a0a29915aa82c7b785214a2bf) # note: cmake branch
set(lcm_IS_CMAKE_POD TRUE)
set(lcm_SOURCE_DIR ${PROJECT_SOURCE_DIR}/externals/lcm/lcm-1.0.0)
set(lcm_IS_PUBLIC TRUE)
set(libbot_GIT_REPOSITORY https://github.com/RobotLocomotion/libbot.git)
set(libbot_GIT_TAG 94fe5290329646959e1c27896d529abba81e6d93)
set(libbot_IS_PUBLIC TRUE)

find_program(MAKE_EXECUTABLE make)
if (NOT MAKE_EXECUTABLE)
	message(WARNING "couldn't find gnu make")
endif()
if (${CMAKE_GENERATOR} STREQUAL "Unix Makefiles")
	set(PODS_MAKE_COMMAND "$(MAKE)")   # so we can pass through commandline arguments.
else()
	set(PODS_MAKE_COMMAND ${MAKE_EXECUTABLE})
endif()

# process optional projects
# note: keep drake in this loop in case externals depend on drake (e.g. the director might in the near future)
set(EXTERNAL_PROJECTS gtk lcm libbot)

set(EXTERNAL_SOURCE_DIRS)
foreach (proj ${EXTERNAL_PROJECTS})
	set(deps)
	foreach(dep ${${proj}_dependencies})
		list(FIND EXTERNAL_PROJECTS ${dep} find_result)
		if (NOT find_result EQUAL -1)
			list(APPEND deps ${dep})
		endif()
	endforeach()

	if (NOT ${proj}_GIT_CLONE_DIR)
		set(${proj}_GIT_CLONE_DIR "${PROJECT_SOURCE_DIR}/externals/${proj}")
	endif()
	if (NOT ${proj}_SOURCE_DIR)
		set(${proj}_SOURCE_DIR ${${proj}_GIT_CLONE_DIR})
	endif()
	if (NOT DEFINED ${proj}_CONFIGURE_COMMAND AND ${proj}_IS_CMAKE_POD)
		set(${proj}_CONFIGURE_COMMAND ${CMAKE_COMMAND} -E make_directory ${${proj}_SOURCE_DIR}/pod-build
				COMMAND ${CMAKE_COMMAND} -E chdir ${${proj}_SOURCE_DIR}/pod-build ${CMAKE_COMMAND} -G${CMAKE_GENERATOR} -DCMAKE_INSTALL_PREFIX=${CMAKE_INSTALL_PREFIX} -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE} ..)
	endif()
	if (NOT DEFINED ${proj}_BUILD_COMMAND)
		if (${proj}_IS_CMAKE_POD)
			set(${proj}_BUILD_COMMAND ${CMAKE_COMMAND} -E chdir ${${proj}_SOURCE_DIR} ${CMAKE_COMMAND} --build pod-build --config $<CONFIGURATION> --target install)
		elseif(${proj}_NO_BUILD)
			set(${proj}_BUILD_COMMAND "")
		else()# then use the PODS gnu make system
			set(${proj}_BUILD_COMMAND ${PODS_MAKE_COMMAND} BUILD_PREFIX=${CMAKE_INSTALL_PREFIX} BUILD_TYPE=$<CONFIGURATION>)
		endif()
	endif()
	set(EXTERNAL_SOURCE_DIRS ${EXTERNAL_SOURCE_DIRS} ${${proj}_SOURCE_DIR} "\n")

	message(STATUS "Preparing to build ${proj} with dependencies: ${deps}")

	# separate download target so I can make the download-all custom command as recommended in:
	#   http://comments.gmane.org/gmane.comp.programming.tools.cmake.user/53002
	if (AUTO_UPDATE_EXTERNALS)
		ExternalProject_Add(download-${proj}
				DOWNLOAD_DIR ${${proj}_GIT_CLONE_DIR}
				SOURCE_DIR ${${proj}_GIT_CLONE_DIR}
				GIT_REPOSITORY ${${proj}_GIT_REPOSITORY}
				GIT_TAG ${${proj}_GIT_TAG}
				CONFIGURE_COMMAND ""
				BUILD_COMMAND ""
				INSTALL_COMMAND ""
				)
	else()
		ExternalProject_Add(download-${proj}
				DOWNLOAD_DIR ${${proj}_GIT_CLONE_DIR}
				SOURCE_DIR ${${proj}_GIT_CLONE_DIR}
				GIT_REPOSITORY ${${proj}_GIT_REPOSITORY}
				GIT_TAG ${${proj}_GIT_TAG}
				UPDATE_COMMAND ""
				CONFIGURE_COMMAND ""
				BUILD_COMMAND ""
				INSTALL_COMMAND ""
				)
	endif()

	# now the actual project
	ExternalProject_Add(${proj}
			SOURCE_DIR ${${proj}_SOURCE_DIR}
			#      BINARY_DIR ${${proj}_SOURCE_DIR}/pod-build
			DOWNLOAD_COMMAND ""
			UPDATE_COMMAND ""
			CONFIGURE_COMMAND "${${proj}_CONFIGURE_COMMAND}"
			BUILD_IN_SOURCE 1
			BUILD_COMMAND "${${proj}_BUILD_COMMAND}"
			INSTALL_COMMAND ""
			DEPENDS download-${proj} ${deps}
			${${proj}_ADDITIONAL_BUILD_ARGS}
			)
	#    message(STATUS "${proj}_BUILD_COMMAND: ${${proj}_BUILD_COMMAND}")

	# once we require cmake version >= 3.1, then we can zap this and just use the BUILD_ALWAYS flags above
	ExternalProject_Add_Step(${proj} forceconfigure
			COMMAND ${CMAKE_COMMAND} -E echo "Force configure of ${proj}"
			DEPENDEES update
			DEPENDERS configure
			ALWAYS 1)

endforeach()
file(WRITE ${CMAKE_CURRENT_BINARY_DIR}/drake_external_source_dirs.txt ${EXTERNAL_SOURCE_DIRS})

# todo: add a custom target for release_filelist

add_custom_target(download-all)
add_custom_target(clean-all)
add_custom_target(status)
set(PROJECT_LIST)
foreach (proj ${EXTERNAL_PROJECTS})
	add_dependencies(download-all download-${proj})
	ExternalProject_Get_Property(download-${proj} SOURCE_DIR)
	add_custom_target(status-${proj}
			COMMAND ${GIT_EXECUTABLE} status
			WORKING_DIRECTORY ${SOURCE_DIR})
	add_dependencies(status status-${proj})

	ExternalProject_Get_Property(${proj} SOURCE_DIR)
	if (NOT ${proj}_NO_BUILD)
		add_custom_target(clean-${proj}
				COMMAND ${PODS_MAKE_COMMAND} BUILD_PREFIX=${CMAKE_INSTALL_PREFIX} BUILD_TYPE=$<CONFIGURATION> clean
				WORKING_DIRECTORY ${SOURCE_DIR})
		add_dependencies(clean-all clean-${proj})
	endif()
	list(APPEND PROJECT_LIST ${SOURCE_DIR})
endforeach()
