
#include "Valuable.h"
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


void Skrypt::SetVarhost(decltype(varHost) host)
{
	auto allocatedVarsCount = varHost->Stored();
	if(allocatedVarsCount != 0) {
		LOG_AND_IMPLEMENT("VarHost has " << allocatedVarsCount << " vars allocated. Consider moving thouse to the new host. The base system needs the support.")
	}else{
		varHost = host;
	}
}

bool Skrypt::Add(std::string_view line) {
	Valuable v(line, varHost);
	v.CollectVaNames(vars);
    bool ok = {};
    try
    {
        if (v.IsVa()) {
            auto solutions = Solve(v.as<Variable>());
            for (auto& solution : solutions) {
                std::cout << ' ' << solution;
            }
            std::cout << std::endl;
            ok = true;
        }
        else {
            std::cout << v << std::endl;
            ok = base::Add(std::move(v));
        }
    }
    catch(...)
    {
        if (!ok) {
            std::cout << "Please, consider improving the system to make this request" << std::endl;
        }
    }
	return ok;
}

namespace {
	std::string Questionless(std::string s) {
		s.erase(std::remove(s.begin(), s.end(), '?'), s.end());
		return s;
	}
}
const omnn::math::Valuable::va_names_t& skrypt::Skrypt::Load(std::istream& in)
{
	std::string line;
	while (std::getline(in, line)) {
		if (!line.empty()) {
#ifdef SKRYPT_EQUAL_SIGN
			if (boost::algorithm::contains(line, "=")) {
				if (boost::algorithm::contains(line, "==")) {
					boost::replace_first(_2, "==", "-");
				}
				else {
					boost::replace_first(_2, "=", "-");
				}
				if (boost::algorithm::contains(line, "=")) {
					LOG_AND_IMPLEMENT("More than one '=' sign");
				}
			}
#endif
			if (boost::algorithm::contains(line, "?")) {
				auto yes = false;
				auto& total = Total();
				auto questionless = Questionless(line);
				Valuable v(questionless, varHost);
				auto lineVars = v.Vars();
				if (total.IsSum()) {
					auto rest = total / v;
					std::cout << "Total: " << total << std::endl
						<< total << " / " << v << ": " << rest << std::endl;
					auto& totalSum = total.as<Sum>();
					if (lineVars.size() == 1) {
						std::vector<Valuable> coefficients;
						auto& va = *lineVars.begin();
						auto totalGrade = totalSum.FillPolyCoeff(coefficients, va);
						coefficients.clear();
						if (v.IsSum()) {
							auto& lineSum = v.as<Sum>();
							auto lineGrade = lineSum.FillPolyCoeff(coefficients, va);
							if (rest.IsSum()) {
								auto restGrade = rest.as<Sum>().FillPolyCoeff(coefficients, va);
								yes = totalGrade == restGrade + lineGrade;
							}
						}
						else if (v.IsVa()) {
							auto solutions = Solve(v.as<Variable>());
							if (solutions.size() == 1) {
								yes = solutions.cbegin()->operator==(0);
							}
						}
					}
					else {
						IMPLEMENT
					}
				}
				else {
					IMPLEMENT
				}
				
				std::cout << '\n'
					<< v << " ?\n"
					<< (yes ? "YES\n" : "IDK\n")
					<< std::endl;
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
	std::cout << "Loading " << path << '\n' << std::endl;
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
