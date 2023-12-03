
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

using namespace std::literals;


void Skrypt::SetVarhost(decltype(varHost) host)
{
	auto allocatedVarsCount = varHost->Stored();
	if(allocatedVarsCount != 0) {
		LOG_AND_IMPLEMENT("VarHost has " << allocatedVarsCount << " vars allocated. Consider moving thouse to the new host. The base system needs the support.")
	}else{
		varHost = host;
	}
}

void Skrypt::PrintVarKnowns(const Variable& v)
{
	std::cout << v << " =";
	auto solutions = Solve(v);
	for (auto& solution : solutions) {
		std::cout << ' ' << solution;
	}
	std::cout << std::endl;
}

bool Skrypt::Add(std::string_view line) {
	Valuable v(line, varHost);
	v.CollectVaNames(vars);
    bool ok = {};
    try
    {
        if (v.IsVa()) {
			PrintVarKnowns(v.as<Variable>());
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
            std::cout << "Please, consider additional defining to the system to make this request" << std::endl;
        }
    }
	return ok;
}

namespace {
	std::string Questionless(std::string s) {
		s.erase(std::remove(s.begin(), s.end(), '?'), s.end());
		return s;
	}
	std::string Questionless(std::string_view s) {
		return Questionless(std::string(s));
	}
	auto FindBracePos(std::string_view line) {
		return line.find_first_of("{}[]"sv);
	}
}
bool Skrypt::ParseNextLine(std::istream& in, std::string_view& line) {
	if (!line.empty()) {
		auto bracePos = FindBracePos(line);
		auto brace = bracePos == std::string_view::npos ? 0 : line[bracePos];
		line.remove_prefix(bracePos);
		switch (brace)
		{
		case '{': {
			if (disjunctionParseMode || Expressions().empty()) {
				Skrypt subsrypt;
				auto parsingIsOnTheGo = subsrypt.ParseNextLine(in, line);
				if (parsingIsOnTheGo)
					subsrypt.Load(in);
				if(!subsrypt.IsEmpty())
					Add(subsrypt.Total());
				return parsingIsOnTheGo;
			}
			else {
				LOG_AND_IMPLEMENT("No need for double conjunction");
			}
		}
		case '}': {
			if (disjunctionParseMode) {
				LOG_AND_IMPLEMENT("Closing disjunction with conjunction brace");
			}
			return {};
		}
		case '[': {
			LOG_AND_IMPLEMENT("Implement OR system");
		}
		case ']': {
			return {};
		}
		case 0: {
			break;
		}
		default: {
			IMPLEMENT;
		}
		}
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
			if (questionless.empty()) {
				for (auto& [name, var] : vars) {
					PrintVarKnowns(var);
				}
				return true;
			}
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
			else if (total == constants::zero) {

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
	return true;
}

const omnn::math::Valuable::va_names_t& Skrypt::Load(std::istream& in)
{
	std::string line;
	while (std::getline(in, line)) {
		std::string_view substr = line;
		if (ParseNextLine(in, substr)) {
			if (substr.size()) {
				LOG_AND_IMPLEMENT("Multiple braces per line:" << substr);
			}
		}
		else {
			break;
		}
	}
	return vars;
}

const omnn::math::Valuable::va_names_t& Skrypt::Load(const boost::filesystem::path & path)
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
	MakesTotalEqu(true);
	Load(stream);
}
