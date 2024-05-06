cmake_minimum_required(VERSION 3.10)

# Define project name
project(SkryptTests)

# Include the libskrypt directory to access the library functions
include_directories(${CMAKE_SOURCE_DIR}/libskrypt)

# Specify the C++ standard
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Find the Boost library components
find_package(Boost 1.74.0 REQUIRED COMPONENTS filesystem system serialization)

# Message to confirm the tests configuration is being processed
message(STATUS "Configuring tests...")

# Add test executables for each test case
add_executable(ChemistryTest Chemistry.cpp)
add_executable(ComparisonTest Comparison.cpp)
add_executable(DisjConjTest DisjConjTest.cpp)
add_executable(TestExpPrecedence TestExpPrecedence.cpp)
add_executable(TestIssue10 TestIssue10.cpp)
add_executable(ModulesTest Modules.cpp)

# Link test executables with the libskrypt library and necessary Boost components
target_link_libraries(ChemistryTest libskrypt Boost::filesystem Boost::system Boost::serialization)
target_link_libraries(ComparisonTest libskrypt Boost::filesystem Boost::system Boost::serialization)
target_link_libraries(DisjConjTest libskrypt Boost::filesystem Boost::system Boost::serialization)
target_link_libraries(TestExpPrecedence libskrypt Boost::filesystem Boost::system Boost::serialization)
target_link_libraries(TestIssue10 libskrypt Boost::filesystem Boost::system Boost::serialization)
target_link_libraries(ModulesTest libskrypt Boost::filesystem Boost::system Boost::serialization)

# Register tests with CTest
enable_testing()
add_test(NAME Chemistry COMMAND ChemistryTest)
add_test(NAME Comparison COMMAND ComparisonTest)
add_test(NAME DisjConj COMMAND DisjConjTest)
add_test(NAME ExpPrecedence COMMAND TestExpPrecedence)
add_test(NAME Issue10 COMMAND TestIssue10)
add_test(NAME Modules COMMAND ModulesTest)

# Custom target to build all tests
add_custom_target(build_tests ALL
    DEPENDS ChemistryTest ComparisonTest DisjConjTest TestExpPrecedence TestIssue10 ModulesTest
)

# Add a message to confirm that this CMakeLists.txt is being processed
message(STATUS "libskrypt/tests CMakeLists.txt has been processed.")
