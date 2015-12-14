include_directories(${PROJECT_SOURCE_DIR}/src
    ${LCM_INCLUDE_DIRS} ${PROJECT_SOURCE_DIR}/src/simulink )

add_subdirectory (${PROJECT_SOURCE_DIR}/src/simulink ) 

#set(SOURCES simulator.cpp simulink/SimulatorDrone.c simulink/rtGetInf.c simulink/rtGetNaN.c simulink/rt_nonfinite.c  )
#add_executable(simulator ${SOURCES})
#LINK_DIRECTORIES(${PROJECT_SOURCE_DIR}/src)

#message("-- Making executable for testing --")   

add_executable(simulator simulator.cpp simulink/rtGetInf.c simulink/rtGetNaN.c simulink/rt_nonfinite.c simulink/SimulatorDrone.c)

pods_use_pkg_config_packages(simulator
	lcmtypes)

#target_link_libraries(simulator ${PROJECT_LINK_LIBS}
#    ${LCM_LDFLAGS})

target_link_libraries(simulator ${LCM_LDFLAGS})



pods_install_executables(simulator)


