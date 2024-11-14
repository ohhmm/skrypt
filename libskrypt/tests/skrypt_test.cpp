#define BOOST_TEST_MODULE SkryptTests
#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_CASE(sanity_check)
{
    BOOST_CHECK(true);  // Basic test to verify framework
}

BOOST_AUTO_TEST_CASE(basic_math)
{
    BOOST_CHECK_EQUAL(2 + 2, 4);  // Simple math test
    BOOST_CHECK_GT(5, 3);         // Greater than comparison
}
