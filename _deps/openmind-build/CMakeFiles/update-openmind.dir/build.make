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

# Utility rule file for update-openmind.

# Include any custom commands dependencies for this target.
include _deps/openmind-build/CMakeFiles/update-openmind.dir/compiler_depend.make

# Include the progress variables for this target.
include _deps/openmind-build/CMakeFiles/update-openmind.dir/progress.make

_deps/openmind-build/CMakeFiles/update-openmind:
	cd /home/ubuntu/skrypt/_deps/openmind-src && /usr/bin/git pull --rebase --autostash || /usr/bin/git pull --rebase --autostash origin HEAD
	cd /home/ubuntu/skrypt/_deps/openmind-src && /usr/bin/git pull --rebase --autostash origin HEAD
	cd /home/ubuntu/skrypt/_deps/openmind-src && /usr/bin/git pull --rebase --autostash https://github.com/ohhmm/openmind HEAD
	cd /home/ubuntu/skrypt/_deps/openmind-src && /usr/bin/git fetch --all

update-openmind: _deps/openmind-build/CMakeFiles/update-openmind
update-openmind: _deps/openmind-build/CMakeFiles/update-openmind.dir/build.make
.PHONY : update-openmind

# Rule to build all files generated by this target.
_deps/openmind-build/CMakeFiles/update-openmind.dir/build: update-openmind
.PHONY : _deps/openmind-build/CMakeFiles/update-openmind.dir/build

_deps/openmind-build/CMakeFiles/update-openmind.dir/clean:
	cd /home/ubuntu/skrypt/_deps/openmind-build && $(CMAKE_COMMAND) -P CMakeFiles/update-openmind.dir/cmake_clean.cmake
.PHONY : _deps/openmind-build/CMakeFiles/update-openmind.dir/clean

_deps/openmind-build/CMakeFiles/update-openmind.dir/depend:
	cd /home/ubuntu/skrypt && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/ubuntu/skrypt /home/ubuntu/skrypt/_deps/openmind-src /home/ubuntu/skrypt /home/ubuntu/skrypt/_deps/openmind-build /home/ubuntu/skrypt/_deps/openmind-build/CMakeFiles/update-openmind.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : _deps/openmind-build/CMakeFiles/update-openmind.dir/depend
