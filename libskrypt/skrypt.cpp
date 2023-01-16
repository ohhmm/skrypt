
#include <skrypt.h>

#include <boost/algorithm/string.hpp>
#include <boost/filesystem.hpp>
#include <boost/filesystem/fstream.hpp>
#include <boost/filesystem/path.hpp>
#include <boost/iostreams/device/mapped_file.hpp>
#include <boost/iostreams/filtering_stream.hpp>

#include <iostream>
#include <fstream>
#include <string>


using namespace skrypt;
using namespace omnn::math;


bool Skrypt::Add(std::string_view line) {
	Valuable v(line, varHost);
	v.CollectVaNames(vars);
	auto ok = v.IsVa();
	if (ok) {
		auto solutions = Solve(v.as<Variable>());
		for (auto& solution : solutions) {
			std::cout << ' ' << solution;
		}
		std::cout << std::endl;
	}
	else {
		std::cout << v << std::endl;
		ok = base::Add(std::move(v));
	}
	return ok;
}

const omnn::math::Valuable::va_names_t& skrypt::Skrypt::Load(std::istream& in)
{
	std::string line;
	while (std::getline(in, line)) {
		if (!line.empty()) {
			if (boost::algorithm::contains(line, "?")) {
				IMPLEMENT
			}
			else {
				Add(line);
			}
		}
	}
	return vars;
}

const omnn::math::Valuable::va_names_t& skrypt::Skrypt::Load(const boost::filesystem::path & path)
{
	boost::filesystem::ifstream stream(path);
	return Load(stream);
}

Skrypt::Skrypt(const boost::filesystem::path & path)
{
	Load(path);
}

Skrypt::Skrypt(std::istream & stream)
{
	MakeTotalEqu(true);
	Load(stream);
}
