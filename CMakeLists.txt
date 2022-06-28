# CMakeList.txt : Top-level CMake project file, do global configuration
# and include sub-projects here.
#
cmake_minimum_required (VERSION 3.8)

option(OPENMIND_DEBUG_CHECKS "Additional debug checks" OFF)
option(OPENMIND_USE_OPENCL "OpenCL GPU calculations" ON)
option(OPENMIND_MATH_USE_LEVELDB_CACHE "OpenCL GPU calculations" OFF)

# Find some Git
find_package(Git QUIET)
if(NOT GIT_EXECUTABLE)
    find_program(GIT_EXECUTABLE git PATHS
        "$ENV{ProgramFiles}/Microsoft Visual Studio/*/*/Common7/IDE/CommonExtensions/Microsoft/TeamFoundation/Team Explorer/Git/cmd/"
        )
    find_package(Git)
endif()

# Fetch some OpenMind
include(FetchContent)
FetchContent_Declare(openmind
    GIT_REPOSITORY https://github.com/ohhmm/openmind.git
    GIT_TAG generator)
FetchContent_MakeAvailable(openmind)

# Tell OpenMind to use any boost version installed in the system if any
find_package(Boost)
if(Boost_FOUND)
    set(OPENMIND_REQUIRED_BOOST_VERSION ${Boost_VERSION} CACHE VERSION "Boost library version to use" FORCE)
endif()

# Include sub-projects.
fold()
