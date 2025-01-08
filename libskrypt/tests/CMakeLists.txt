
# Configure test dependencies
set(TEST_DEPS libskrypt)
set(BOOST_TEST_LINK_LIBS Boost::unit_test_framework)

# Configure tests with dependencies
test("${TEST_DEPS}")
