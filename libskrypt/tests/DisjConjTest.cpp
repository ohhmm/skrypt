#define BOOST_TEST_MODULE conjunctiondisjunction test
#include <boost/test/unit_test.hpp>
#include "skrypt.h"

using namespace ::skrypt;
using namespace std::string_literals;
using namespace std::string_view_literals;

BOOST_AUTO_TEST_CASE(ConjunctionDisjunctionTest){
    Skrypt skrypt;
    skrypt.Load(TEST_SRC_DIR "ConDisj.skrypt");

    // get the variable
    auto variableName = "x"s;
    auto varhost = skrypt.GetVarHost();
    auto& pendingTheVariable = varhost->Host(variableName);

    // check that the variable is already solved
    auto pendingVariableSolutions = skrypt.Known(pendingTheVariable);
    BOOST_TEST(pendingVariableSolutions.size() == 1);
    for (auto& solution : pendingVariableSolutions) {
        BOOST_TEST(solution == 10);
    }
}
