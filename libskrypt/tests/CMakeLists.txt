# Rename the test target to avoid CTest naming conflicts
file(GLOB TEST_SOURCES "*.cpp")

# Set common test properties and dependencies
set(TEST_LINK_LIBRARIES
    libskrypt
    Boost::unit_test_framework
    Boost::program_options
    Boost::filesystem
    Boost::serialization
)

foreach(test_source ${TEST_SOURCES})
    get_filename_component(test_name ${test_source} NAME_WE)
    add_executable(${test_name} ${test_source})
    target_link_libraries(${test_name} PRIVATE ${TEST_LINK_LIBRARIES})
    set_target_properties(${test_name} PROPERTIES
        CXX_STANDARD 20
        CXX_STANDARD_REQUIRED ON
    )
endforeach()
