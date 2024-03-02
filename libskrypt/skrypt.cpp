
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

bool skrypt::Skrypt::ParseTotal(std::istream& in, std::string_view& line, std::function<void(::omnn::math::Valuable&&)> result)
{
	MakesTotalEqu(true);
	auto parsingIsOnTheGo = ParseNextLine(in, line);
	if (parsingIsOnTheGo)
		Load(in);
	if (DisjunctionParseMode())
		result(std::move(disjunction));
	else if (!Expressions().empty()) {
		auto& total = const_cast<::omnn::math::Valuable&>(Total());
		result(std::move(total));
	}
	return parsingIsOnTheGo;
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
			if(DisjunctionParseMode())
				ok = disjunction.Add(v) != disjunction.end();
			else
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

void Skrypt::ProcessQuestionLine(std::string_view& line)
{
    Valuable::YesNoMaybe is = Valuable::YesNoMaybe::Maybe;
	auto& total = Total();
	auto questionless = Questionless(line);
	if (questionless.empty()) {
		for (auto& [name, var] : vars) {
			PrintVarKnowns(var);
		}
	}
	else {
		Valuable v(questionless, varHost);
		auto lineVars = v.Vars();
        if (v.IsInt()) {
            if (v == 0) {
                is = Valuable::YesNoMaybe::Yes;
            } else {
                is = Valuable::YesNoMaybe::No;
            }
        } else if (total.IsSum()) {
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
                        if (totalGrade == restGrade + lineGrade)
							is = Valuable::YesNoMaybe::Yes;
					}
				}
				else if (v.IsVa()) {
					auto solutions = Solve(v.as<Variable>());
					if (solutions.size() == 1) {
						if(solutions.cbegin()->operator==(0))
							is = Valuable::YesNoMaybe::Yes;
					}
				}
			}
			else {
				IMPLEMENT
			}
		} else if (total == constants::zero) {
        } else if (total.IsInt()) {
            is = Valuable::YesNoMaybe::No;
        }
		else {
			IMPLEMENT
		}

		std::cout << '\n' << v << " ?\n";
        if (is == Valuable::YesNoMaybe::Yes)
            std::cout << "YES";
        else if (is == Valuable::YesNoMaybe::No)
            std::cout << "NO";
        else
            std::cout << "IDK";
        std::cout << std::endl;
	}
}

bool Skrypt::ParseNextLine(std::istream& in, std::string_view& line) {
	if (!line.empty()) {
		auto bracePos = FindBracePos(line);
		auto brace = bracePos == std::string_view::npos ? 0 : line[bracePos];
		line.remove_prefix(bracePos+1);
		switch (brace)
		{
		case '{': {
			if (disjunctionParseMode) {
				Skrypt subscript;
				subscript.SetVarhost(GetVarHost());
				subscript.Echo(echo);
				return subscript.ParseTotal(in, line,
					[this](::omnn::math::Valuable&& result) {
						Add(std::move(result));
					});
			}
			else if (Expressions().empty()) {
				if (line.empty())
					return true;
				else
					break;
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
			if (disjunctionParseMode) {
				LOG_AND_IMPLEMENT("No need for double disjunction");
			}
			Skrypt subscript;
			subscript.SetVarhost(GetVarHost());
			subscript.Echo(echo);
			subscript.DisjunctionParseMode(true);
			return subscript.ParseTotal(in, line,
				[this](::omnn::math::Valuable&& result) {
					Add(std::move(result));
				});
		}
		case ']': {
			if (DisjunctionParseMode()) {
				Add(std::move(disjunction));
			}else {
				LOG_AND_IMPLEMENT("Closing conjunction with disjunction brace");
			}
			return {};
		}
		case 0: {
			break;
		}
		default: {
			IMPLEMENT;
		}
		}

		if (boost::algorithm::contains(line, "?")) {
			ProcessQuestionLine(line);
		}
		else {
			Add(line);
		}
		line = {};
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
