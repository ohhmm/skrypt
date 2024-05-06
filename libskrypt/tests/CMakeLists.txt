cmake_minimum_required(VERSION 3.10)

# Define project name
project(SkryptTests)

# Include the libskrypt directory to access the library functions
include_directories(${CMAKE_SOURCE_DIR}/libskrypt)

# Include the openmind headers
include_directories(${CMAKE_SOURCE_DIR}/_deps/openmind-src)

# Specify the C++ standard
set(CMAKE_CXX_STANDARD 20)
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

# Define macros unconditionally for all test targets
target_compile_definitions(ChemistryTest PUBLIC
    APPLE_CONSTEXPR=constexpr
    NO_APPLE_CONSTEXPR=constexpr
    NO_CLANG_CONSTEXPR=constexpr
    MSVC_CONSTEXPR=constexpr
    NO_MSVC_CONSTEXPR=constexpr
    TEST_SRC_DIR="${CMAKE_SOURCE_DIR}/libskrypt/tests/"
)

target_compile_definitions(ComparisonTest PUBLIC
    APPLE_CONSTEXPR=constexpr
    NO_APPLE_CONSTEXPR=constexpr
    NO_CLANG_CONSTEXPR=constexpr
    MSVC_CONSTEXPR=constexpr
    NO_MSVC_CONSTEXPR=constexpr
    TEST_SRC_DIR="${CMAKE_SOURCE_DIR}/libskrypt/tests/"
)

target_compile_definitions(DisjConjTest PUBLIC
    APPLE_CONSTEXPR=constexpr
    NO_APPLE_CONSTEXPR=constexpr
    NO_CLANG_CONSTEXPR=constexpr
    MSVC_CONSTEXPR=constexpr
    NO_MSVC_CONSTEXPR=constexpr
    TEST_SRC_DIR="${CMAKE_SOURCE_DIR}/libskrypt/tests/"
)

target_compile_definitions(TestExpPrecedence PUBLIC
    APPLE_CONSTEXPR=constexpr
    NO_APPLE_CONSTEXPR=constexpr
    NO_CLANG_CONSTEXPR=constexpr
    MSVC_CONSTEXPR=constexpr
    NO_MSVC_CONSTEXPR=constexpr
    TEST_SRC_DIR="${CMAKE_SOURCE_DIR}/libskrypt/tests/"
)

target_compile_definitions(TestIssue10 PUBLIC
    APPLE_CONSTEXPR=constexpr
    NO_APPLE_CONSTEXPR=constexpr
    NO_CLANG_CONSTEXPR=constexpr
    MSVC_CONSTEXPR=constexpr
    NO_MSVC_CONSTEXPR=constexpr
    TEST_SRC_DIR="${CMAKE_SOURCE_DIR}/libskrypt/tests/"
)

target_compile_definitions(ModulesTest PUBLIC
    APPLE_CONSTEXPR=constexpr
    NO_APPLE_CONSTEXPR=constexpr
    NO_CLANG_CONSTEXPR=constexpr
    MSVC_CONSTEXPR=constexpr
    NO_MSVC_CONSTEXPR=constexpr
    TEST_SRC_DIR="${CMAKE_SOURCE_DIR}/libskrypt/tests/"
)

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

# Add a message to confirm that this CMakeLists.txt is being processed
message(STATUS "libskrypt/tests CMakeLists.txt has been processed.")
