# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.22

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
CMAKE_SOURCE_DIR = /home/ubuntu/repos/skrypt/standalone

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/ubuntu/repos/skrypt/standalone/build

# Include any dependencies generated for this target.
include CMakeFiles/state_loading_test.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/state_loading_test.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/state_loading_test.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/state_loading_test.dir/flags.make

CMakeFiles/state_loading_test.dir/tests/state_loading_test.cpp.o: CMakeFiles/state_loading_test.dir/flags.make
CMakeFiles/state_loading_test.dir/tests/state_loading_test.cpp.o: ../tests/state_loading_test.cpp
CMakeFiles/state_loading_test.dir/tests/state_loading_test.cpp.o: CMakeFiles/state_loading_test.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/ubuntu/repos/skrypt/standalone/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/state_loading_test.dir/tests/state_loading_test.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/state_loading_test.dir/tests/state_loading_test.cpp.o -MF CMakeFiles/state_loading_test.dir/tests/state_loading_test.cpp.o.d -o CMakeFiles/state_loading_test.dir/tests/state_loading_test.cpp.o -c /home/ubuntu/repos/skrypt/standalone/tests/state_loading_test.cpp

CMakeFiles/state_loading_test.dir/tests/state_loading_test.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/state_loading_test.dir/tests/state_loading_test.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/ubuntu/repos/skrypt/standalone/tests/state_loading_test.cpp > CMakeFiles/state_loading_test.dir/tests/state_loading_test.cpp.i

CMakeFiles/state_loading_test.dir/tests/state_loading_test.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/state_loading_test.dir/tests/state_loading_test.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/ubuntu/repos/skrypt/standalone/tests/state_loading_test.cpp -o CMakeFiles/state_loading_test.dir/tests/state_loading_test.cpp.s

# Object files for target state_loading_test
state_loading_test_OBJECTS = \
"CMakeFiles/state_loading_test.dir/tests/state_loading_test.cpp.o"

# External object files for target state_loading_test
state_loading_test_EXTERNAL_OBJECTS =

state_loading_test: CMakeFiles/state_loading_test.dir/tests/state_loading_test.cpp.o
state_loading_test: CMakeFiles/state_loading_test.dir/build.make
state_loading_test: libskrypt_standalone.a
state_loading_test: CMakeFiles/state_loading_test.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/ubuntu/repos/skrypt/standalone/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable state_loading_test"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/state_loading_test.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/state_loading_test.dir/build: state_loading_test
.PHONY : CMakeFiles/state_loading_test.dir/build

CMakeFiles/state_loading_test.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/state_loading_test.dir/cmake_clean.cmake
.PHONY : CMakeFiles/state_loading_test.dir/clean

CMakeFiles/state_loading_test.dir/depend:
	cd /home/ubuntu/repos/skrypt/standalone/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/ubuntu/repos/skrypt/standalone /home/ubuntu/repos/skrypt/standalone /home/ubuntu/repos/skrypt/standalone/build /home/ubuntu/repos/skrypt/standalone/build /home/ubuntu/repos/skrypt/standalone/build/CMakeFiles/state_loading_test.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/state_loading_test.dir/depend

