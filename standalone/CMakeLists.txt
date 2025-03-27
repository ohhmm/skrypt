cmake_minimum_required(VERSION 3.10)
project(skrypt_standalone)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Define source files
set(SOURCES
    src/state_loading.cpp
)

# Define include directories
include_directories(include)

# Create library
add_library(skrypt_standalone STATIC ${SOURCES})

# Only build tests if this is the main project and not being included as a subdirectory
if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME AND NOT DEFINED SKRYPT_MAIN_PROJECT)
    # Create test executable
    enable_testing()
    add_executable(state_loading_test tests/state_loading_test.cpp)
    target_link_libraries(state_loading_test skrypt_standalone)
    add_test(NAME StateLoadingTest COMMAND state_loading_test)
endif()
