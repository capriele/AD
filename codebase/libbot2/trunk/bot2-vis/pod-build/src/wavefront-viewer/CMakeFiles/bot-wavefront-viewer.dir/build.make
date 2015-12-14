# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 2.8

#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:

# Remove some rules from gmake that .SUFFIXES does not remove.
SUFFIXES =

.SUFFIXES: .hpux_make_needs_suffix_list

# Suppress display of executed commands.
$(VERBOSE).SILENT:

# A target that is always out of date.
cmake_force:
.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /usr/bin/cmake

# The command to remove a file.
RM = /usr/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/ra/workspace/code/codebase/libbot2/trunk/bot2-vis

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/ra/workspace/code/codebase/libbot2/trunk/bot2-vis/pod-build

# Include any dependencies generated for this target.
include src/wavefront-viewer/CMakeFiles/bot-wavefront-viewer.dir/depend.make

# Include the progress variables for this target.
include src/wavefront-viewer/CMakeFiles/bot-wavefront-viewer.dir/progress.make

# Include the compile flags for this target's objects.
include src/wavefront-viewer/CMakeFiles/bot-wavefront-viewer.dir/flags.make

src/wavefront-viewer/CMakeFiles/bot-wavefront-viewer.dir/main.c.o: src/wavefront-viewer/CMakeFiles/bot-wavefront-viewer.dir/flags.make
src/wavefront-viewer/CMakeFiles/bot-wavefront-viewer.dir/main.c.o: ../src/wavefront-viewer/main.c
	$(CMAKE_COMMAND) -E cmake_progress_report /home/ra/workspace/code/codebase/libbot2/trunk/bot2-vis/pod-build/CMakeFiles $(CMAKE_PROGRESS_1)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building C object src/wavefront-viewer/CMakeFiles/bot-wavefront-viewer.dir/main.c.o"
	cd /home/ra/workspace/code/codebase/libbot2/trunk/bot2-vis/pod-build/src/wavefront-viewer && /usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -o CMakeFiles/bot-wavefront-viewer.dir/main.c.o   -c /home/ra/workspace/code/codebase/libbot2/trunk/bot2-vis/src/wavefront-viewer/main.c

src/wavefront-viewer/CMakeFiles/bot-wavefront-viewer.dir/main.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/bot-wavefront-viewer.dir/main.c.i"
	cd /home/ra/workspace/code/codebase/libbot2/trunk/bot2-vis/pod-build/src/wavefront-viewer && /usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -E /home/ra/workspace/code/codebase/libbot2/trunk/bot2-vis/src/wavefront-viewer/main.c > CMakeFiles/bot-wavefront-viewer.dir/main.c.i

src/wavefront-viewer/CMakeFiles/bot-wavefront-viewer.dir/main.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/bot-wavefront-viewer.dir/main.c.s"
	cd /home/ra/workspace/code/codebase/libbot2/trunk/bot2-vis/pod-build/src/wavefront-viewer && /usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -S /home/ra/workspace/code/codebase/libbot2/trunk/bot2-vis/src/wavefront-viewer/main.c -o CMakeFiles/bot-wavefront-viewer.dir/main.c.s

src/wavefront-viewer/CMakeFiles/bot-wavefront-viewer.dir/main.c.o.requires:
.PHONY : src/wavefront-viewer/CMakeFiles/bot-wavefront-viewer.dir/main.c.o.requires

src/wavefront-viewer/CMakeFiles/bot-wavefront-viewer.dir/main.c.o.provides: src/wavefront-viewer/CMakeFiles/bot-wavefront-viewer.dir/main.c.o.requires
	$(MAKE) -f src/wavefront-viewer/CMakeFiles/bot-wavefront-viewer.dir/build.make src/wavefront-viewer/CMakeFiles/bot-wavefront-viewer.dir/main.c.o.provides.build
.PHONY : src/wavefront-viewer/CMakeFiles/bot-wavefront-viewer.dir/main.c.o.provides

src/wavefront-viewer/CMakeFiles/bot-wavefront-viewer.dir/main.c.o.provides.build: src/wavefront-viewer/CMakeFiles/bot-wavefront-viewer.dir/main.c.o

src/wavefront-viewer/CMakeFiles/bot-wavefront-viewer.dir/renderer_wavefront.c.o: src/wavefront-viewer/CMakeFiles/bot-wavefront-viewer.dir/flags.make
src/wavefront-viewer/CMakeFiles/bot-wavefront-viewer.dir/renderer_wavefront.c.o: ../src/wavefront-viewer/renderer_wavefront.c
	$(CMAKE_COMMAND) -E cmake_progress_report /home/ra/workspace/code/codebase/libbot2/trunk/bot2-vis/pod-build/CMakeFiles $(CMAKE_PROGRESS_2)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building C object src/wavefront-viewer/CMakeFiles/bot-wavefront-viewer.dir/renderer_wavefront.c.o"
	cd /home/ra/workspace/code/codebase/libbot2/trunk/bot2-vis/pod-build/src/wavefront-viewer && /usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -o CMakeFiles/bot-wavefront-viewer.dir/renderer_wavefront.c.o   -c /home/ra/workspace/code/codebase/libbot2/trunk/bot2-vis/src/wavefront-viewer/renderer_wavefront.c

src/wavefront-viewer/CMakeFiles/bot-wavefront-viewer.dir/renderer_wavefront.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/bot-wavefront-viewer.dir/renderer_wavefront.c.i"
	cd /home/ra/workspace/code/codebase/libbot2/trunk/bot2-vis/pod-build/src/wavefront-viewer && /usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -E /home/ra/workspace/code/codebase/libbot2/trunk/bot2-vis/src/wavefront-viewer/renderer_wavefront.c > CMakeFiles/bot-wavefront-viewer.dir/renderer_wavefront.c.i

src/wavefront-viewer/CMakeFiles/bot-wavefront-viewer.dir/renderer_wavefront.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/bot-wavefront-viewer.dir/renderer_wavefront.c.s"
	cd /home/ra/workspace/code/codebase/libbot2/trunk/bot2-vis/pod-build/src/wavefront-viewer && /usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -S /home/ra/workspace/code/codebase/libbot2/trunk/bot2-vis/src/wavefront-viewer/renderer_wavefront.c -o CMakeFiles/bot-wavefront-viewer.dir/renderer_wavefront.c.s

src/wavefront-viewer/CMakeFiles/bot-wavefront-viewer.dir/renderer_wavefront.c.o.requires:
.PHONY : src/wavefront-viewer/CMakeFiles/bot-wavefront-viewer.dir/renderer_wavefront.c.o.requires

src/wavefront-viewer/CMakeFiles/bot-wavefront-viewer.dir/renderer_wavefront.c.o.provides: src/wavefront-viewer/CMakeFiles/bot-wavefront-viewer.dir/renderer_wavefront.c.o.requires
	$(MAKE) -f src/wavefront-viewer/CMakeFiles/bot-wavefront-viewer.dir/build.make src/wavefront-viewer/CMakeFiles/bot-wavefront-viewer.dir/renderer_wavefront.c.o.provides.build
.PHONY : src/wavefront-viewer/CMakeFiles/bot-wavefront-viewer.dir/renderer_wavefront.c.o.provides

src/wavefront-viewer/CMakeFiles/bot-wavefront-viewer.dir/renderer_wavefront.c.o.provides.build: src/wavefront-viewer/CMakeFiles/bot-wavefront-viewer.dir/renderer_wavefront.c.o

# Object files for target bot-wavefront-viewer
bot__wavefront__viewer_OBJECTS = \
"CMakeFiles/bot-wavefront-viewer.dir/main.c.o" \
"CMakeFiles/bot-wavefront-viewer.dir/renderer_wavefront.c.o"

# External object files for target bot-wavefront-viewer
bot__wavefront__viewer_EXTERNAL_OBJECTS =

bin/bot-wavefront-viewer: src/wavefront-viewer/CMakeFiles/bot-wavefront-viewer.dir/main.c.o
bin/bot-wavefront-viewer: src/wavefront-viewer/CMakeFiles/bot-wavefront-viewer.dir/renderer_wavefront.c.o
bin/bot-wavefront-viewer: src/wavefront-viewer/CMakeFiles/bot-wavefront-viewer.dir/build.make
bin/bot-wavefront-viewer: src/wavefront-viewer/CMakeFiles/bot-wavefront-viewer.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --red --bold "Linking C executable ../../bin/bot-wavefront-viewer"
	cd /home/ra/workspace/code/codebase/libbot2/trunk/bot2-vis/pod-build/src/wavefront-viewer && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/bot-wavefront-viewer.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
src/wavefront-viewer/CMakeFiles/bot-wavefront-viewer.dir/build: bin/bot-wavefront-viewer
.PHONY : src/wavefront-viewer/CMakeFiles/bot-wavefront-viewer.dir/build

src/wavefront-viewer/CMakeFiles/bot-wavefront-viewer.dir/requires: src/wavefront-viewer/CMakeFiles/bot-wavefront-viewer.dir/main.c.o.requires
src/wavefront-viewer/CMakeFiles/bot-wavefront-viewer.dir/requires: src/wavefront-viewer/CMakeFiles/bot-wavefront-viewer.dir/renderer_wavefront.c.o.requires
.PHONY : src/wavefront-viewer/CMakeFiles/bot-wavefront-viewer.dir/requires

src/wavefront-viewer/CMakeFiles/bot-wavefront-viewer.dir/clean:
	cd /home/ra/workspace/code/codebase/libbot2/trunk/bot2-vis/pod-build/src/wavefront-viewer && $(CMAKE_COMMAND) -P CMakeFiles/bot-wavefront-viewer.dir/cmake_clean.cmake
.PHONY : src/wavefront-viewer/CMakeFiles/bot-wavefront-viewer.dir/clean

src/wavefront-viewer/CMakeFiles/bot-wavefront-viewer.dir/depend:
	cd /home/ra/workspace/code/codebase/libbot2/trunk/bot2-vis/pod-build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/ra/workspace/code/codebase/libbot2/trunk/bot2-vis /home/ra/workspace/code/codebase/libbot2/trunk/bot2-vis/src/wavefront-viewer /home/ra/workspace/code/codebase/libbot2/trunk/bot2-vis/pod-build /home/ra/workspace/code/codebase/libbot2/trunk/bot2-vis/pod-build/src/wavefront-viewer /home/ra/workspace/code/codebase/libbot2/trunk/bot2-vis/pod-build/src/wavefront-viewer/CMakeFiles/bot-wavefront-viewer.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : src/wavefront-viewer/CMakeFiles/bot-wavefront-viewer.dir/depend
