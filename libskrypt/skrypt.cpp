
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
	return base::Add(std::move(v));
}

const omnn::math::Valuable::va_names_t& skrypt::Skrypt::Load(boost::filesystem::ifstream& in)
{
	std::string line;
	while (std::getline(in, line)) {
		std::cout << line << std::endl;
		if (boost::algorithm::contains(line, "?")) {
			IMPLEMENT
		}
		else {
			Add(line);
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
	boost::iostreams::filtering_istream in;
	in.push(stream);

}
//
//Skrypt::Skrypt(std::istream& stdios)
//{
//}
