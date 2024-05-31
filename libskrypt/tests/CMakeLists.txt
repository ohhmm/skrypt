cmake_minimum_required(VERSION 3.8)

# Define a macro for simplifying adding tests
macro(package_add_test TESTNAME)
    # Create an executable in which the tests will be stored
    add_executable(${TESTNAME} ${ARGN})
    # Link the necessary libraries here, for example Boost libraries
    target_link_libraries(${TESTNAME} Boost::filesystem Boost::system Boost::serialization Boost::thread)
    # Add the test so that CTest knows about it
    add_test(NAME ${TESTNAME} COMMAND ${TESTNAME})
endmacro()

# Find Boost libraries
find_package(Boost 1.74.0 REQUIRED COMPONENTS filesystem system serialization thread)

# Now simply link against the necessary libraries as needed. Eg
package_add_test(TestIssue10 TestIssue10.cpp)
package_add_test(DisjConjTest DisjConjTest.cpp)
package_add_test(Farm Farm.cpp)
package_add_test(TestExpPrecedence TestExpPrecedence.cpp)
package_add_test(Comparison Comparison.cpp)
package_add_test(Chemistry Chemistry.cpp)
package_add_test(Modules Modules.cpp)
# The fastest_get test is already defined in the openmind-src submodule, so it's not added here again
# package_add_test(fastest_get ${CMAKE_CURRENT_SOURCE_DIR}/../_deps/openmind-src/omnn/rt/tests/fastest_get.cpp)
# ... Add other tests here
