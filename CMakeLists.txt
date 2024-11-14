cmake_minimum_required (VERSION 3.8)

project(SkryptProject)

# Add custom include directory before system includes
include_directories(BEFORE ${CMAKE_SOURCE_DIR}/include)

# Select C++ active standard
enable_language(CXX)
set (CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

option(OPENMIND_DEBUG_CHECKS "Additional debug checks" OFF)
find_package(OpenCL)
option(OPENMIND_USE_OPENCL "OpenCL GPU calculations" ${OpenCL_FOUND})
option(OPENMIND_MATH_USE_LEVELDB_CACHE "DB cache solutions" ON)
option(BUILD_TESTS "Build tests" ON) # Explicitly set BUILD_TESTS option

# Set Boost components and linking preferences
set(Boost_USE_STATIC_LIBS ON) # Force static linking
set(Boost_USE_MULTITHREADED ON)
set(BOOST_ADDITIONAL_COMPONENTS ${BOOST_ADDITIONAL_COMPONENTS} program_options filesystem serialization unit_test_framework CACHE STRING "Boost components" FORCE)

# Find Boost and ensure test components are included
find_package(Boost REQUIRED COMPONENTS ${BOOST_ADDITIONAL_COMPONENTS})
set(BOOST_ADDITIONAL_COMPONENTS ${BOOST_ADDITIONAL_COMPONENTS} program_options)
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

# Fetch LevelDB first to ensure it's available for OpenMind
include(FetchContent)

# Set LevelDB options to disable benchmarks and tests
set(LEVELDB_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(LEVELDB_BUILD_BENCHMARKS OFF CACHE BOOL "" FORCE)
set(LEVELDB_INSTALL OFF CACHE BOOL "" FORCE)
set(LEVELDB_CXX_STANDARD 17 CACHE STRING "" FORCE)  # Set C++17 for LevelDB

FetchContent_Declare(
    leveldb
    GIT_REPOSITORY https://github.com/google/leveldb.git
    GIT_TAG 1.23
)
FetchContent_GetProperties(leveldb)
if(NOT leveldb_POPULATED)
    FetchContent_Populate(leveldb)
    # Apply patch to fix memory order syntax
    execute_process(
        COMMAND git apply ${CMAKE_SOURCE_DIR}/patches/leveldb-memory-order.patch
        WORKING_DIRECTORY ${leveldb_SOURCE_DIR}
    )
    add_subdirectory(${leveldb_SOURCE_DIR} ${leveldb_BINARY_DIR} EXCLUDE_FROM_ALL)
endif()

# Set up LevelDB paths and variables for OpenMind
get_target_property(LEVELDB_INCLUDE_DIR leveldb INTERFACE_INCLUDE_DIRECTORIES)
set(LEVELDB_ROOT ${leveldb_BINARY_DIR})
include_directories(${LEVELDB_INCLUDE_DIR} ${leveldb_SOURCE_DIR}/include)
link_directories(${leveldb_BINARY_DIR})

# Set OpenMind C++ standard before fetching
set(CMAKE_CXX_STANDARD_BEFORE_FETCH ${CMAKE_CXX_STANDARD})
set(CMAKE_CXX_STANDARD 17)  # Temporarily set C++17 for OpenMind

# Fetch OpenMind
FetchContent_Declare(openmind
    GIT_REPOSITORY https://github.com/ohhmm/openmind.git
    GIT_TAG main)
FetchContent_GetProperties(openmind)
if(NOT openmind_POPULATED)
    FetchContent_Populate(openmind)
    # Apply patches in order: C++17 standard, uBLAS allocator fix, integer sqrt fix, matrix fix, and polyfit compatibility
    execute_process(
        COMMAND git apply ${CMAKE_SOURCE_DIR}/patches/openmind-cpp17.patch
        WORKING_DIRECTORY ${openmind_SOURCE_DIR}
    )
    execute_process(
        COMMAND git apply ${CMAKE_SOURCE_DIR}/patches/openmind-ublas-allocator.patch
        WORKING_DIRECTORY ${openmind_SOURCE_DIR}
    )
    execute_process(
        COMMAND git apply ${CMAKE_SOURCE_DIR}/patches/openmind-integer-sqrt.patch
        WORKING_DIRECTORY ${openmind_SOURCE_DIR}
    )
    execute_process(
        COMMAND git apply ${CMAKE_SOURCE_DIR}/patches/boost-ublas-matrix.patch
        WORKING_DIRECTORY ${openmind_SOURCE_DIR}
    )
    execute_process(
        COMMAND git apply ${CMAKE_SOURCE_DIR}/patches/openmind-polyfit-compat.patch
        WORKING_DIRECTORY ${openmind_SOURCE_DIR}
    )
    add_subdirectory(${openmind_SOURCE_DIR} ${openmind_BINARY_DIR})
endif()

# Restore original C++ standard
set(CMAKE_CXX_STANDARD ${CMAKE_CXX_STANDARD_BEFORE_FETCH})

if(BUILD_TESTS OR OPENMIND_BUILD_TESTS)
    enable_testing()
    # Link against Boost test libraries
    set(BOOST_TEST_LINK_LIBS Boost::unit_test_framework)
    add_executable(skrypt_tests libskrypt/tests/skrypt_test.cpp) # Renamed test executable
    target_link_libraries(skrypt_tests ${BOOST_TEST_LINK_LIBS})
endif(BUILD_TESTS OR OPENMIND_BUILD_TESTS)

# OpenMind: traverse subprojects
add_subdirectory(libskrypt) # Use add_subdirectory instead of fold


install(DIRECTORY modules DESTINATION modules)
