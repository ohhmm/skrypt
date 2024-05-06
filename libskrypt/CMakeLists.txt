# Include the openmind headers and the local directory for skrypt.h
include_directories(
    ${CMAKE_SOURCE_DIR}/_deps/openmind-src/omnn
    ${CMAKE_SOURCE_DIR}/_deps/openmind-src/omnn/math
    ${CMAKE_SOURCE_DIR}/_deps/openmind-src/omnn/rt
    ${CMAKE_CURRENT_SOURCE_DIR}
)

add_library(skrypt skrypt.cpp)

# Add the tests subdirectory
add_subdirectory(tests)
