# Install script for directory: /home/ra/workspace/code/codebase/libbot2/trunk/bot2-core/src/bot_core

# Set the install prefix
IF(NOT DEFINED CMAKE_INSTALL_PREFIX)
  SET(CMAKE_INSTALL_PREFIX "/home/ra/workspace/code/codebase/build")
ENDIF(NOT DEFINED CMAKE_INSTALL_PREFIX)
STRING(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
IF(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  IF(BUILD_TYPE)
    STRING(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  ELSE(BUILD_TYPE)
    SET(CMAKE_INSTALL_CONFIG_NAME "Release")
  ENDIF(BUILD_TYPE)
  MESSAGE(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
ENDIF(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)

# Set the component getting installed.
IF(NOT CMAKE_INSTALL_COMPONENT)
  IF(COMPONENT)
    MESSAGE(STATUS "Install component: \"${COMPONENT}\"")
    SET(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  ELSE(COMPONENT)
    SET(CMAKE_INSTALL_COMPONENT)
  ENDIF(COMPONENT)
ENDIF(NOT CMAKE_INSTALL_COMPONENT)

# Install shared libraries without execute permission?
IF(NOT DEFINED CMAKE_INSTALL_SO_NO_EXE)
  SET(CMAKE_INSTALL_SO_NO_EXE "1")
ENDIF(NOT DEFINED CMAKE_INSTALL_SO_NO_EXE)

IF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  FOREACH(file
      "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libbot2-core.so.1"
      "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libbot2-core.so"
      )
    IF(EXISTS "${file}" AND
       NOT IS_SYMLINK "${file}")
      FILE(RPATH_CHECK
           FILE "${file}"
           RPATH "/home/ra/workspace/code/codebase/build/lib")
    ENDIF()
  ENDFOREACH()
  FILE(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE SHARED_LIBRARY FILES
    "/home/ra/workspace/code/codebase/libbot2/trunk/bot2-core/pod-build/lib/libbot2-core.so.1"
    "/home/ra/workspace/code/codebase/libbot2/trunk/bot2-core/pod-build/lib/libbot2-core.so"
    )
  FOREACH(file
      "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libbot2-core.so.1"
      "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libbot2-core.so"
      )
    IF(EXISTS "${file}" AND
       NOT IS_SYMLINK "${file}")
      FILE(RPATH_CHANGE
           FILE "${file}"
           OLD_RPATH "/home/ra/workspace/code/codebase/libbot2/trunk/bot2-core/pod-build/lib:/home/ra/workspace/code/codebase/build/lib:"
           NEW_RPATH "/home/ra/workspace/code/codebase/build/lib")
      IF(CMAKE_INSTALL_DO_STRIP)
        EXECUTE_PROCESS(COMMAND "/usr/bin/strip" "${file}")
      ENDIF(CMAKE_INSTALL_DO_STRIP)
    ENDIF()
  ENDFOREACH()
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")

IF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  FILE(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/bot_core" TYPE FILE FILES
    "/home/ra/workspace/code/codebase/libbot2/trunk/bot2-core/src/bot_core/serial.h"
    "/home/ra/workspace/code/codebase/libbot2/trunk/bot2-core/src/bot_core/ringbuf.h"
    "/home/ra/workspace/code/codebase/libbot2/trunk/bot2-core/src/bot_core/ppm.h"
    "/home/ra/workspace/code/codebase/libbot2/trunk/bot2-core/src/bot_core/camtrans.h"
    "/home/ra/workspace/code/codebase/libbot2/trunk/bot2-core/src/bot_core/ssocket.h"
    "/home/ra/workspace/code/codebase/libbot2/trunk/bot2-core/src/bot_core/tictoc.h"
    "/home/ra/workspace/code/codebase/libbot2/trunk/bot2-core/src/bot_core/timespec.h"
    "/home/ra/workspace/code/codebase/libbot2/trunk/bot2-core/src/bot_core/circular.h"
    "/home/ra/workspace/code/codebase/libbot2/trunk/bot2-core/src/bot_core/fileutils.h"
    "/home/ra/workspace/code/codebase/libbot2/trunk/bot2-core/src/bot_core/trans.h"
    "/home/ra/workspace/code/codebase/libbot2/trunk/bot2-core/src/bot_core/lcm_util.h"
    "/home/ra/workspace/code/codebase/libbot2/trunk/bot2-core/src/bot_core/set.h"
    "/home/ra/workspace/code/codebase/libbot2/trunk/bot2-core/src/bot_core/rand_util.h"
    "/home/ra/workspace/code/codebase/libbot2/trunk/bot2-core/src/bot_core/ptr_circular.h"
    "/home/ra/workspace/code/codebase/libbot2/trunk/bot2-core/src/bot_core/glib_util.h"
    "/home/ra/workspace/code/codebase/libbot2/trunk/bot2-core/src/bot_core/timestamp.h"
    "/home/ra/workspace/code/codebase/libbot2/trunk/bot2-core/src/bot_core/fasttrig.h"
    "/home/ra/workspace/code/codebase/libbot2/trunk/bot2-core/src/bot_core/minheap.h"
    "/home/ra/workspace/code/codebase/libbot2/trunk/bot2-core/src/bot_core/ctrans.h"
    "/home/ra/workspace/code/codebase/libbot2/trunk/bot2-core/src/bot_core/small_linalg.h"
    "/home/ra/workspace/code/codebase/libbot2/trunk/bot2-core/src/bot_core/color_util.h"
    "/home/ra/workspace/code/codebase/libbot2/trunk/bot2-core/src/bot_core/signal_pipe.h"
    "/home/ra/workspace/code/codebase/libbot2/trunk/bot2-core/src/bot_core/math_util.h"
    "/home/ra/workspace/code/codebase/libbot2/trunk/bot2-core/src/bot_core/rotations.h"
    "/home/ra/workspace/code/codebase/libbot2/trunk/bot2-core/src/bot_core/bot_core.h"
    "/home/ra/workspace/code/codebase/libbot2/trunk/bot2-core/src/bot_core/gps_linearize.h"
    )
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")

IF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  FILE(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/pkgconfig" TYPE FILE FILES "/home/ra/workspace/code/codebase/libbot2/trunk/bot2-core/pod-build/lib/pkgconfig/bot2-core.pc")
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")

