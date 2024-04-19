cmake_minimum_required (VERSION 3.8)

# Select C++ active standard
enable_language(CXX)
set (CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

option(OPENMIND_DEBUG_CHECKS "Additional debug checks" OFF)
find_package(OpenCL)
option(OPENMIND_USE_OPENCL "OpenCL GPU calculations" ${OpenCL_FOUND})
option(OPENMIND_MATH_USE_LEVELDB_CACHE "DB cache solutions" ON)
set(BOOST_ADDITIONAL_COMPONENTS program_options;dll CACHE VERSION "Boost components" FORCE)
# Tell OpenMind to use any boost version installed in the system if any
if(Boost_FOUND)
    set(OPENMIND_REQUIRED_BOOST_VERSION ${Boost_VERSION_STRING} CACHE VERSION "Boost library version to use" FORCE)
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
