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
include _deps/openmind-build/omnn/math/test/CMakeFiles/logic.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include _deps/openmind-build/omnn/math/test/CMakeFiles/logic.dir/compiler_depend.make

# Include the progress variables for this target.
include _deps/openmind-build/omnn/math/test/CMakeFiles/logic.dir/progress.make

# Include the compile flags for this target's objects.
include _deps/openmind-build/omnn/math/test/CMakeFiles/logic.dir/flags.make

_deps/openmind-build/omnn/math/test/CMakeFiles/logic.dir/logic.cpp.o: _deps/openmind-build/omnn/math/test/CMakeFiles/logic.dir/flags.make
_deps/openmind-build/omnn/math/test/CMakeFiles/logic.dir/logic.cpp.o: _deps/openmind-src/omnn/math/test/logic.cpp
_deps/openmind-build/omnn/math/test/CMakeFiles/logic.dir/logic.cpp.o: _deps/openmind-build/omnn/math/test/CMakeFiles/logic.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/ubuntu/skrypt/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object _deps/openmind-build/omnn/math/test/CMakeFiles/logic.dir/logic.cpp.o"
	cd /home/ubuntu/skrypt/_deps/openmind-build/omnn/math/test && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT _deps/openmind-build/omnn/math/test/CMakeFiles/logic.dir/logic.cpp.o -MF CMakeFiles/logic.dir/logic.cpp.o.d -o CMakeFiles/logic.dir/logic.cpp.o -c /home/ubuntu/skrypt/_deps/openmind-src/omnn/math/test/logic.cpp

_deps/openmind-build/omnn/math/test/CMakeFiles/logic.dir/logic.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/logic.dir/logic.cpp.i"
	cd /home/ubuntu/skrypt/_deps/openmind-build/omnn/math/test && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/ubuntu/skrypt/_deps/openmind-src/omnn/math/test/logic.cpp > CMakeFiles/logic.dir/logic.cpp.i

_deps/openmind-build/omnn/math/test/CMakeFiles/logic.dir/logic.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/logic.dir/logic.cpp.s"
	cd /home/ubuntu/skrypt/_deps/openmind-build/omnn/math/test && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/ubuntu/skrypt/_deps/openmind-src/omnn/math/test/logic.cpp -o CMakeFiles/logic.dir/logic.cpp.s

# Object files for target logic
logic_OBJECTS = \
"CMakeFiles/logic.dir/logic.cpp.o"

# External object files for target logic
logic_EXTERNAL_OBJECTS =

bin/logic: _deps/openmind-build/omnn/math/test/CMakeFiles/logic.dir/logic.cpp.o
bin/logic: _deps/openmind-build/omnn/math/test/CMakeFiles/logic.dir/build.make
bin/logic: /usr/lib/x86_64-linux-gnu/libboost_serialization.a
bin/logic: /usr/lib/x86_64-linux-gnu/libboost_system.a
bin/logic: /usr/lib/x86_64-linux-gnu/libboost_filesystem.a
bin/logic: /usr/lib/x86_64-linux-gnu/libboost_thread.a
bin/logic: /usr/lib/x86_64-linux-gnu/libboost_test_exec_monitor.a
bin/logic: /usr/lib/x86_64-linux-gnu/libboost_unit_test_framework.a
bin/logic: /usr/lib/x86_64-linux-gnu/libboost_serialization.a
bin/logic: /usr/lib/x86_64-linux-gnu/libboost_system.a
bin/logic: /usr/lib/x86_64-linux-gnu/libboost_filesystem.a
bin/logic: /usr/lib/x86_64-linux-gnu/libboost_thread.a
bin/logic: /usr/lib/x86_64-linux-gnu/libboost_test_exec_monitor.a
bin/logic: /usr/lib/x86_64-linux-gnu/libboost_unit_test_framework.a
bin/logic: /usr/lib/x86_64-linux-gnu/libboost_serialization.a
bin/logic: /usr/lib/x86_64-linux-gnu/libboost_system.a
bin/logic: /usr/lib/x86_64-linux-gnu/libboost_filesystem.a
bin/logic: /usr/lib/x86_64-linux-gnu/libboost_thread.a
bin/logic: /usr/lib/x86_64-linux-gnu/libboost_test_exec_monitor.a
bin/logic: /usr/lib/x86_64-linux-gnu/libboost_unit_test_framework.a
bin/logic: /usr/lib/x86_64-linux-gnu/libboost_serialization.a
bin/logic: /usr/lib/x86_64-linux-gnu/libboost_system.a
bin/logic: /usr/lib/x86_64-linux-gnu/libboost_filesystem.a
bin/logic: /usr/lib/x86_64-linux-gnu/libboost_thread.a
bin/logic: /usr/lib/x86_64-linux-gnu/libboost_test_exec_monitor.a
bin/logic: /usr/lib/x86_64-linux-gnu/libboost_unit_test_framework.a
bin/logic: _deps/openmind-build/omnn/math/libmath.a
bin/logic: _deps/openmind-build/omnn/storage/libstorage.a
bin/logic: _deps/openmind-build/omnn/rt/librt.a
bin/logic: /usr/lib/x86_64-linux-gnu/libboost_serialization.a
bin/logic: /usr/lib/x86_64-linux-gnu/libboost_system.a
bin/logic: /usr/lib/x86_64-linux-gnu/libboost_filesystem.a
bin/logic: /usr/lib/x86_64-linux-gnu/libboost_thread.a
bin/logic: /usr/lib/x86_64-linux-gnu/libboost_atomic.a
bin/logic: /usr/lib/x86_64-linux-gnu/libboost_test_exec_monitor.a
bin/logic: /usr/lib/x86_64-linux-gnu/libboost_unit_test_framework.a
bin/logic: /usr/lib/x86_64-linux-gnu/libtbb.so.12.5
bin/logic: /usr/lib/x86_64-linux-gnu/libboost_program_options.a
bin/logic: /usr/lib/x86_64-linux-gnu/libleveldb.so
bin/logic: _deps/openmind-build/omnn/math/test/CMakeFiles/logic.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/ubuntu/skrypt/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable ../../../../../bin/logic"
	cd /home/ubuntu/skrypt/_deps/openmind-build/omnn/math/test && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/logic.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
_deps/openmind-build/omnn/math/test/CMakeFiles/logic.dir/build: bin/logic
.PHONY : _deps/openmind-build/omnn/math/test/CMakeFiles/logic.dir/build

_deps/openmind-build/omnn/math/test/CMakeFiles/logic.dir/clean:
	cd /home/ubuntu/skrypt/_deps/openmind-build/omnn/math/test && $(CMAKE_COMMAND) -P CMakeFiles/logic.dir/cmake_clean.cmake
.PHONY : _deps/openmind-build/omnn/math/test/CMakeFiles/logic.dir/clean

_deps/openmind-build/omnn/math/test/CMakeFiles/logic.dir/depend:
	cd /home/ubuntu/skrypt && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/ubuntu/skrypt /home/ubuntu/skrypt/_deps/openmind-src/omnn/math/test /home/ubuntu/skrypt /home/ubuntu/skrypt/_deps/openmind-build/omnn/math/test /home/ubuntu/skrypt/_deps/openmind-build/omnn/math/test/CMakeFiles/logic.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : _deps/openmind-build/omnn/math/test/CMakeFiles/logic.dir/depend
