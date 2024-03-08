#define BOOST_TEST_MODULE modules test
#include "skrypt.h"
#include <boost/test/unit_test.hpp>

using namespace ::skrypt;
using namespace std::string_literals;
using namespace std::string_view_literals;

BOOST_AUTO_TEST_CASE(LoadingModulesNoExceptionsTest) {
    Skrypt(TEST_SRC_DIR "Module.skrypt");
}

BOOST_AUTO_TEST_CASE(ModulePropertyTest) {
    Skrypt skrypt;
    skrypt.Load(TEST_SRC_DIR "Module.skrypt");

    // get the variable
    auto variableName = "x"s;
    auto varhost = skrypt.GetVarHost();
    auto& variable = varhost->Host(variableName);
    auto& moduleVariableX = varhost->Host("TestExpOrderWithBrackets.x"s);

    // check that the variable is already solved
    auto moduleVariableSolutions = skrypt.Known(moduleVariableX);
    skrypt.Known(variable);
    BOOST_TEST(moduleVariableSolutions.size() == 1);
    for (auto& solution : moduleVariableSolutions) {
        BOOST_TEST(solution == 1);
    }
}
