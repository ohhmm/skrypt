#define BOOST_TEST_MODULE skryptFunction test
#include <boost/test/unit_test.hpp>
#include "skrypt.h"

using namespace ::skrypt;
using namespace std::string_literals;
using namespace std::string_view_literals;

BOOST_AUTO_TEST_CASE(FunctionDefinitionTest) {
    Skrypt skrypt;
    
    skrypt.Add("f(x) = x^2"sv);
    
    auto varhost = skrypt.GetVarHost();
    auto& f = varhost->Host("f"s);
    
    auto& solutions = skrypt.Known(f);
    BOOST_TEST(!solutions.empty());
    
    skrypt.Add("y = f(2)"sv);
    auto& y = varhost->Host("y"s);
    auto& ySolutions = skrypt.Known(y);
    BOOST_TEST(ySolutions.size() == 1);
    for (auto& solution : ySolutions) {
        BOOST_TEST(solution == 4);
    }
    
    skrypt.Add("z = 3"sv);
    skrypt.Add("w = f(z)"sv);
    auto& w = varhost->Host("w"s);
    auto& wSolutions = skrypt.Known(w);
    BOOST_TEST(wSolutions.size() == 1);
    for (auto& solution : wSolutions) {
        BOOST_TEST(solution == 9);
    }
}

BOOST_AUTO_TEST_CASE(MultipleArgumentFunctionTest) {
    Skrypt skrypt;
    
    skrypt.Add("g(x, y) = x*y + x^2"sv);
    
    skrypt.Add("result = g(2, 3)"sv);
    auto varhost = skrypt.GetVarHost();
    auto& result = varhost->Host("result"s);
    auto& resultSolutions = skrypt.Known(result);
    BOOST_TEST(resultSolutions.size() == 1);
    for (auto& solution : resultSolutions) {
        BOOST_TEST(solution == 10); // 2*3 + 2^2 = 6 + 4 = 10
    }
}

BOOST_AUTO_TEST_CASE(ComposedFunctionTest) {
    Skrypt skrypt;
    
    skrypt.Add("f(x) = x^2"sv);
    skrypt.Add("g(x) = 2*x + 1"sv);
    
    skrypt.Add("h = f(g(3))"sv);
    auto varhost = skrypt.GetVarHost();
    auto& h = varhost->Host("h"s);
    auto& hSolutions = skrypt.Known(h);
    BOOST_TEST(hSolutions.size() == 1);
    for (auto& solution : hSolutions) {
        BOOST_TEST(solution == 49); // f(g(3)) = f(2*3+1) = f(7) = 7^2 = 49
    }
}
