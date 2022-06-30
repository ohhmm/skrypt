
#include <skrypt.h>

#include <boost/algorithm/string.hpp>
#include <boost/filesystem/path.hpp>
#include <boost/filesystem/fstream.hpp>

#include <string>

using namespace skrypt;
using namespace omnn::math;

bool Skrypt::Add(std::string_view line) {
	Valuable v(line, varHost);
	v.CollectVaNames(vars);
	return base::Add(std::move(v));
}

Skrypt::Skrypt(const boost::filesystem::path& path)
{
	boost::filesystem::ifstream file(path);
	std::string line;
	while (std::getline(file, line)) {
		std::cout << line << std::endl;
		if (boost::algorithm::contains(line, "?")) {
			
		}
		else {
			Add(line);
		}
	}
}
