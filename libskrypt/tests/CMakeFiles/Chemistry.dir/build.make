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
CMAKE_SOURCE_DIR = /home/ubuntu/skrypt

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/ubuntu/skrypt

# Include any dependencies generated for this target.
include libskrypt/tests/CMakeFiles/Chemistry.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include libskrypt/tests/CMakeFiles/Chemistry.dir/compiler_depend.make

# Include the progress variables for this target.
include libskrypt/tests/CMakeFiles/Chemistry.dir/progress.make

# Include the compile flags for this target's objects.
include libskrypt/tests/CMakeFiles/Chemistry.dir/flags.make

libskrypt/tests/CMakeFiles/Chemistry.dir/Chemistry.cpp.o: libskrypt/tests/CMakeFiles/Chemistry.dir/flags.make
libskrypt/tests/CMakeFiles/Chemistry.dir/Chemistry.cpp.o: libskrypt/tests/Chemistry.cpp
libskrypt/tests/CMakeFiles/Chemistry.dir/Chemistry.cpp.o: libskrypt/tests/CMakeFiles/Chemistry.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/ubuntu/skrypt/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object libskrypt/tests/CMakeFiles/Chemistry.dir/Chemistry.cpp.o"
	cd /home/ubuntu/skrypt/libskrypt/tests && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT libskrypt/tests/CMakeFiles/Chemistry.dir/Chemistry.cpp.o -MF CMakeFiles/Chemistry.dir/Chemistry.cpp.o.d -o CMakeFiles/Chemistry.dir/Chemistry.cpp.o -c /home/ubuntu/skrypt/libskrypt/tests/Chemistry.cpp

libskrypt/tests/CMakeFiles/Chemistry.dir/Chemistry.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/Chemistry.dir/Chemistry.cpp.i"
	cd /home/ubuntu/skrypt/libskrypt/tests && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/ubuntu/skrypt/libskrypt/tests/Chemistry.cpp > CMakeFiles/Chemistry.dir/Chemistry.cpp.i

libskrypt/tests/CMakeFiles/Chemistry.dir/Chemistry.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/Chemistry.dir/Chemistry.cpp.s"
	cd /home/ubuntu/skrypt/libskrypt/tests && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/ubuntu/skrypt/libskrypt/tests/Chemistry.cpp -o CMakeFiles/Chemistry.dir/Chemistry.cpp.s

# Object files for target Chemistry
Chemistry_OBJECTS = \
"CMakeFiles/Chemistry.dir/Chemistry.cpp.o"

# External object files for target Chemistry
Chemistry_EXTERNAL_OBJECTS =

bin/Chemistry: libskrypt/tests/CMakeFiles/Chemistry.dir/Chemistry.cpp.o
bin/Chemistry: libskrypt/tests/CMakeFiles/Chemistry.dir/build.make
bin/Chemistry: libskrypt/liblibskrypt.a
bin/Chemistry: _deps/openmind-build/omnn/math/libmath.a
bin/Chemistry: _deps/openmind-build/omnn/storage/libstorage.a
bin/Chemistry: _deps/openmind-build/omnn/rt/librt.a
bin/Chemistry: /usr/lib/x86_64-linux-gnu/libboost_test_exec_monitor.a
bin/Chemistry: /usr/lib/x86_64-linux-gnu/libboost_unit_test_framework.a
bin/Chemistry: /usr/lib/x86_64-linux-gnu/libboost_serialization.a
bin/Chemistry: /usr/lib/x86_64-linux-gnu/libboost_system.a
bin/Chemistry: /usr/lib/x86_64-linux-gnu/libboost_filesystem.a
bin/Chemistry: /usr/lib/x86_64-linux-gnu/libboost_thread.a
bin/Chemistry: /usr/lib/x86_64-linux-gnu/libboost_atomic.a
bin/Chemistry: /usr/lib/x86_64-linux-gnu/libboost_program_options.a
bin/Chemistry: /usr/lib/x86_64-linux-gnu/libtbb.so.12.5
bin/Chemistry: /usr/lib/x86_64-linux-gnu/libleveldb.so
bin/Chemistry: libskrypt/tests/CMakeFiles/Chemistry.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/ubuntu/skrypt/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable ../../bin/Chemistry"
	cd /home/ubuntu/skrypt/libskrypt/tests && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/Chemistry.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
libskrypt/tests/CMakeFiles/Chemistry.dir/build: bin/Chemistry
.PHONY : libskrypt/tests/CMakeFiles/Chemistry.dir/build

libskrypt/tests/CMakeFiles/Chemistry.dir/clean:
	cd /home/ubuntu/skrypt/libskrypt/tests && $(CMAKE_COMMAND) -P CMakeFiles/Chemistry.dir/cmake_clean.cmake
.PHONY : libskrypt/tests/CMakeFiles/Chemistry.dir/clean

libskrypt/tests/CMakeFiles/Chemistry.dir/depend:
	cd /home/ubuntu/skrypt && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/ubuntu/skrypt /home/ubuntu/skrypt/libskrypt/tests /home/ubuntu/skrypt /home/ubuntu/skrypt/libskrypt/tests /home/ubuntu/skrypt/libskrypt/tests/CMakeFiles/Chemistry.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : libskrypt/tests/CMakeFiles/Chemistry.dir/depend

