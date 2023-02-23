

#include "skrypt.h"

#include <omnn/math/Variable.h>
using namespace omnn::math;

#include <boost/serialization/vector.hpp>
#include <boost/program_options.hpp>
#include <boost/multiprecision/cpp_int.hpp>
#include <boost/lambda2.hpp>
#include <boost/filesystem/path.hpp>
#include <boost/archive/binary_oarchive.hpp>

#include <iostream>


namespace {
    boost::program_options::options_description Options("Options");
    boost::filesystem::path filepath;
    auto& desc = Options.add_options()
		("help", "produce help message")
		("file", boost::program_options::value(&filepath), "Load task description")
		;
    boost::program_options::variables_map vm;
}
int main(int argc, const char* const argv[])
{
    boost::program_options::basic_command_line_parser<char> parser(argc, argv);

	std::cout << "SKRYPT RULEZ:\n"
		"Accepts three forms of input :\n"
		"- expressions with variables(automatically equal to zero)\n"
		"- single variable name(to show its value if known)\n"
		"- expression with question mark to check if its true\n";
    if (argc > 1)
    {
        skrypt::Skrypt s;
        s.Echo(true);
		s.MakeTotalEqu(true);
        s.Load(argv[1]);
    }
    else {
        skrypt::Skrypt s(std::cin);
    }
    return 0;
}

