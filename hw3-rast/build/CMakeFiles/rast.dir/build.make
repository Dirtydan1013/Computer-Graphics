# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.28

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:

#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:

# Disable VCS-based implicit rules.
% : %,v

# Disable VCS-based implicit rules.
% : RCS/%

# Disable VCS-based implicit rules.
% : RCS/%,v

# Disable VCS-based implicit rules.
% : SCCS/s.%

# Disable VCS-based implicit rules.
% : s.%

.SUFFIXES: .hpux_make_needs_suffix_list

# Command-line flag to silence nested $(MAKE).
$(VERBOSE)MAKESILENT = -s

#Suppress display of executed commands.
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
RM = /usr/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /mnt/d/程式儲存區/hw3-rast

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /mnt/d/程式儲存區/hw3-rast/build

# Include any dependencies generated for this target.
include CMakeFiles/rast.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/rast.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/rast.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/rast.dir/flags.make

CMakeFiles/rast.dir/hw-rast.cpp.o: CMakeFiles/rast.dir/flags.make
CMakeFiles/rast.dir/hw-rast.cpp.o: /mnt/d/程式儲存區/hw3-rast/hw-rast.cpp
CMakeFiles/rast.dir/hw-rast.cpp.o: CMakeFiles/rast.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/mnt/d/程式儲存區/hw3-rast/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/rast.dir/hw-rast.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/rast.dir/hw-rast.cpp.o -MF CMakeFiles/rast.dir/hw-rast.cpp.o.d -o CMakeFiles/rast.dir/hw-rast.cpp.o -c /mnt/d/程式儲存區/hw3-rast/hw-rast.cpp

CMakeFiles/rast.dir/hw-rast.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/rast.dir/hw-rast.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /mnt/d/程式儲存區/hw3-rast/hw-rast.cpp > CMakeFiles/rast.dir/hw-rast.cpp.i

CMakeFiles/rast.dir/hw-rast.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/rast.dir/hw-rast.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /mnt/d/程式儲存區/hw3-rast/hw-rast.cpp -o CMakeFiles/rast.dir/hw-rast.cpp.s

CMakeFiles/rast.dir/swgl-rast.cpp.o: CMakeFiles/rast.dir/flags.make
CMakeFiles/rast.dir/swgl-rast.cpp.o: /mnt/d/程式儲存區/hw3-rast/swgl-rast.cpp
CMakeFiles/rast.dir/swgl-rast.cpp.o: CMakeFiles/rast.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/mnt/d/程式儲存區/hw3-rast/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/rast.dir/swgl-rast.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/rast.dir/swgl-rast.cpp.o -MF CMakeFiles/rast.dir/swgl-rast.cpp.o.d -o CMakeFiles/rast.dir/swgl-rast.cpp.o -c /mnt/d/程式儲存區/hw3-rast/swgl-rast.cpp

CMakeFiles/rast.dir/swgl-rast.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/rast.dir/swgl-rast.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /mnt/d/程式儲存區/hw3-rast/swgl-rast.cpp > CMakeFiles/rast.dir/swgl-rast.cpp.i

CMakeFiles/rast.dir/swgl-rast.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/rast.dir/swgl-rast.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /mnt/d/程式儲存區/hw3-rast/swgl-rast.cpp -o CMakeFiles/rast.dir/swgl-rast.cpp.s

# Object files for target rast
rast_OBJECTS = \
"CMakeFiles/rast.dir/hw-rast.cpp.o" \
"CMakeFiles/rast.dir/swgl-rast.cpp.o"

# External object files for target rast
rast_EXTERNAL_OBJECTS =

rast: CMakeFiles/rast.dir/hw-rast.cpp.o
rast: CMakeFiles/rast.dir/swgl-rast.cpp.o
rast: CMakeFiles/rast.dir/build.make
rast: /usr/lib/x86_64-linux-gnu/libGL.so
rast: /usr/lib/x86_64-linux-gnu/libGLU.so
rast: CMakeFiles/rast.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --bold --progress-dir=/mnt/d/程式儲存區/hw3-rast/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Linking CXX executable rast"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/rast.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/rast.dir/build: rast
.PHONY : CMakeFiles/rast.dir/build

CMakeFiles/rast.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/rast.dir/cmake_clean.cmake
.PHONY : CMakeFiles/rast.dir/clean

CMakeFiles/rast.dir/depend:
	cd /mnt/d/程式儲存區/hw3-rast/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /mnt/d/程式儲存區/hw3-rast /mnt/d/程式儲存區/hw3-rast /mnt/d/程式儲存區/hw3-rast/build /mnt/d/程式儲存區/hw3-rast/build /mnt/d/程式儲存區/hw3-rast/build/CMakeFiles/rast.dir/DependInfo.cmake "--color=$(COLOR)"
.PHONY : CMakeFiles/rast.dir/depend

