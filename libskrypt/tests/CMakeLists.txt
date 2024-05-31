include_directories(${CMAKE_SOURCE_DIR}/libskrypt)
include_directories(${CMAKE_SOURCE_DIR}/_deps/openmind-src)

add_executable(GeometryTest GeometryTest.cpp)
target_link_libraries(GeometryTest PRIVATE skrypt)
add_test(NAME GeometryTest COMMAND GeometryTest)
