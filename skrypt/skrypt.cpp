

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
#include <vector>


namespace {
	bool IsInteractiveMode = true;
    boost::program_options::options_description Options("Options");
    std::vector<boost::filesystem::path> filepath;
    auto& desc = Options.add_options()
		("help", "produce help message")
		("file", boost::program_options::value(&filepath), "Load task description")
		("i,interactive", boost::program_options::value(&IsInteractiveMode)->default_value(true), "Continue interactive mode after loading scripts")
		;
    boost::program_options::variables_map vm;
}
int main(int argc, const char* const argv[])
{
	boost::program_options::store(boost::program_options::parse_command_line(argc, argv, Options), vm);
	boost::program_options::notify(vm);
	if (vm.count("help")) {
		std::cout << "Usage: options_description [options]\n" << Options;
		exit(0);
	}

	std::cout << "SKRYPT RULEZ:\n"
		"Accepts three forms of input :\n"
		"- equations with any variables are automatically equaled to zero\n"
		"- single variable name to show its value if known\n"
		"- expression with question mark to check if its true\n";
    if (argc > 1)
    {
        skrypt::Skrypt s;
        s.Echo(true);
		s.MakesTotalEqu(true);
		for(auto& file : filepath)
			s.Load(file);
		if(IsInteractiveMode)
			s.Load(std::cin);
    }
    else {
        skrypt::Skrypt s(std::cin);
    }
    return 0;
}

