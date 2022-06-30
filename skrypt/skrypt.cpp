

#include "skrypt.h"

#include <omnn/math/Variable.h>
using namespace omnn::math;

#include <boost/archive/binary_oarchive.hpp>
#include <boost/lambda2.hpp>
#include <boost/multiprecision/cpp_int.hpp>
#include <boost/program_options.hpp>
#include <boost/serialization/vector.hpp>
#include <boost/filesystem/path.hpp>

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

    std::cout << argc << std::endl;
    if (argc > 1)
    {
        skrypt::Skrypt skrypt(argv[1]);
    }
    return 0;
}

