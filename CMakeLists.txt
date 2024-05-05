cmake_minimum_required (VERSION 3.8)

project(Skrypt)

# Select C++ active standard
enable_language(CXX)
set (CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

option(OPENMIND_DEBUG_CHECKS "Additional debug checks" OFF)
# Manually specify the OpenCL include directories and library for linking
include_directories(/usr/include)
link_directories(/usr/lib/x86_64-linux-gnu)
set(OpenCL_LIBRARY "/usr/lib/x86_64-linux-gnu/libOpenCL.so")
# Comment out the find_package command for OpenCL
# find_package(OpenCL REQUIRED PATHS "/usr/lib/x86_64-linux-gnu" NO_DEFAULT_PATH)
option(OPENMIND_USE_OPENCL "OpenCL GPU calculations" ON)
option(OPENMIND_MATH_USE_LEVELDB_CACHE "DB cache solutions" ON)
set(BOOST_ADDITIONAL_COMPONENTS ${BOOST_ADDITIONAL_COMPONENTS} dll;program_options)
list(REMOVE_DUPLICATES BOOST_ADDITIONAL_COMPONENTS)
set(BOOST_ADDITIONAL_COMPONENTS ${BOOST_ADDITIONAL_COMPONENTS} CACHE VERSION "Boost components" FORCE)
# Tell OpenMind to use any boost version installed in the system if any
if(Boost_FOUND)
    set(OPENMIND_REQUIRED_BOOST_VERSION ${Boost_VERSION_STRING} CACHE STRING "Boost library version to use" FORCE)
endif()

# Find some Git
find_package(Git QUIET)
if(NOT GIT_EXECUTABLE)
    find_program(GIT_EXECUTABLE git PATHS
        "$ENV{ProgramFiles}/Microsoft Visual Studio/*/*/Common7/IDE/CommonExtensions/Microsoft/TeamFoundation/Team Explorer/Git/cmd/"
        )
    find_package(Git)
endif()

find_package(Threads)

# Fetch some OpenMind
include(FetchContent)
FetchContent_Declare(openmind
    GIT_REPOSITORY https://github.com/ohhmm/openmind.git
    GIT_TAG main)
FetchContent_MakeAvailable(openmind)

if(BUILD_TESTS OR OPENMIND_BUILD_TESTS)
    enable_testing()
endif(BUILD_TESTS OR OPENMIND_BUILD_TESTS)

# OpenMind: traverse subprojects
fold(libskrypt) # parse libskrypt prior to skrypt

install(DIRECTORY modules DESTINATION modules)
