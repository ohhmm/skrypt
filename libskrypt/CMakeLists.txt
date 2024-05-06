# Include the openmind headers and the local directory for skrypt.h
include_directories(
    ${CMAKE_SOURCE_DIR}/_deps/openmind-src/omnn
    ${CMAKE_SOURCE_DIR}/_deps/openmind-src/omnn/math
    ${CMAKE_SOURCE_DIR}/_deps/openmind-src/omnn/rt
)

add_library(skrypt skrypt.cpp)

# Specify include directories for the skrypt target
target_include_directories(skrypt PUBLIC
    ${CMAKE_SOURCE_DIR}/_deps/openmind-src/omnn
    ${CMAKE_SOURCE_DIR}/_deps/openmind-src/omnn/math
    ${CMAKE_SOURCE_DIR}/_deps/openmind-src/omnn/rt
    ${CMAKE_CURRENT_SOURCE_DIR}
)

# Add the tests subdirectory
add_subdirectory(tests)
