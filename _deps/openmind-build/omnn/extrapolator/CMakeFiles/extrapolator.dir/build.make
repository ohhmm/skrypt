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
include _deps/openmind-build/omnn/extrapolator/CMakeFiles/extrapolator.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include _deps/openmind-build/omnn/extrapolator/CMakeFiles/extrapolator.dir/compiler_depend.make

# Include the progress variables for this target.
include _deps/openmind-build/omnn/extrapolator/CMakeFiles/extrapolator.dir/progress.make

# Include the compile flags for this target's objects.
include _deps/openmind-build/omnn/extrapolator/CMakeFiles/extrapolator.dir/flags.make

_deps/openmind-build/omnn/extrapolator/CMakeFiles/extrapolator.dir/Extrapolator.cpp.o: _deps/openmind-build/omnn/extrapolator/CMakeFiles/extrapolator.dir/flags.make
_deps/openmind-build/omnn/extrapolator/CMakeFiles/extrapolator.dir/Extrapolator.cpp.o: _deps/openmind-src/omnn/extrapolator/Extrapolator.cpp
_deps/openmind-build/omnn/extrapolator/CMakeFiles/extrapolator.dir/Extrapolator.cpp.o: _deps/openmind-build/omnn/extrapolator/CMakeFiles/extrapolator.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/ubuntu/skrypt/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object _deps/openmind-build/omnn/extrapolator/CMakeFiles/extrapolator.dir/Extrapolator.cpp.o"
	cd /home/ubuntu/skrypt/_deps/openmind-build/omnn/extrapolator && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT _deps/openmind-build/omnn/extrapolator/CMakeFiles/extrapolator.dir/Extrapolator.cpp.o -MF CMakeFiles/extrapolator.dir/Extrapolator.cpp.o.d -o CMakeFiles/extrapolator.dir/Extrapolator.cpp.o -c /home/ubuntu/skrypt/_deps/openmind-src/omnn/extrapolator/Extrapolator.cpp

_deps/openmind-build/omnn/extrapolator/CMakeFiles/extrapolator.dir/Extrapolator.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/extrapolator.dir/Extrapolator.cpp.i"
	cd /home/ubuntu/skrypt/_deps/openmind-build/omnn/extrapolator && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/ubuntu/skrypt/_deps/openmind-src/omnn/extrapolator/Extrapolator.cpp > CMakeFiles/extrapolator.dir/Extrapolator.cpp.i

_deps/openmind-build/omnn/extrapolator/CMakeFiles/extrapolator.dir/Extrapolator.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/extrapolator.dir/Extrapolator.cpp.s"
	cd /home/ubuntu/skrypt/_deps/openmind-build/omnn/extrapolator && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/ubuntu/skrypt/_deps/openmind-src/omnn/extrapolator/Extrapolator.cpp -o CMakeFiles/extrapolator.dir/Extrapolator.cpp.s

# Object files for target extrapolator
extrapolator_OBJECTS = \
"CMakeFiles/extrapolator.dir/Extrapolator.cpp.o"

# External object files for target extrapolator
extrapolator_EXTERNAL_OBJECTS =

_deps/openmind-build/omnn/extrapolator/libextrapolator.a: _deps/openmind-build/omnn/extrapolator/CMakeFiles/extrapolator.dir/Extrapolator.cpp.o
_deps/openmind-build/omnn/extrapolator/libextrapolator.a: _deps/openmind-build/omnn/extrapolator/CMakeFiles/extrapolator.dir/build.make
_deps/openmind-build/omnn/extrapolator/libextrapolator.a: _deps/openmind-build/omnn/extrapolator/CMakeFiles/extrapolator.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/ubuntu/skrypt/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX static library libextrapolator.a"
	cd /home/ubuntu/skrypt/_deps/openmind-build/omnn/extrapolator && $(CMAKE_COMMAND) -P CMakeFiles/extrapolator.dir/cmake_clean_target.cmake
	cd /home/ubuntu/skrypt/_deps/openmind-build/omnn/extrapolator && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/extrapolator.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
_deps/openmind-build/omnn/extrapolator/CMakeFiles/extrapolator.dir/build: _deps/openmind-build/omnn/extrapolator/libextrapolator.a
.PHONY : _deps/openmind-build/omnn/extrapolator/CMakeFiles/extrapolator.dir/build

_deps/openmind-build/omnn/extrapolator/CMakeFiles/extrapolator.dir/clean:
	cd /home/ubuntu/skrypt/_deps/openmind-build/omnn/extrapolator && $(CMAKE_COMMAND) -P CMakeFiles/extrapolator.dir/cmake_clean.cmake
.PHONY : _deps/openmind-build/omnn/extrapolator/CMakeFiles/extrapolator.dir/clean

_deps/openmind-build/omnn/extrapolator/CMakeFiles/extrapolator.dir/depend:
	cd /home/ubuntu/skrypt && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/ubuntu/skrypt /home/ubuntu/skrypt/_deps/openmind-src/omnn/extrapolator /home/ubuntu/skrypt /home/ubuntu/skrypt/_deps/openmind-build/omnn/extrapolator /home/ubuntu/skrypt/_deps/openmind-build/omnn/extrapolator/CMakeFiles/extrapolator.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : _deps/openmind-build/omnn/extrapolator/CMakeFiles/extrapolator.dir/depend
