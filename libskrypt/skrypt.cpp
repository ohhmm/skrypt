
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
    BackgroudLoadingModules(v);
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
    auto questionless = Questionless(line);
    if (questionless.empty()) {
        for (auto& [name, var] : vars) {
            PrintVarKnowns(var);
        }
    }
	else {
		Valuable expression(questionless, varHost);
		auto lineVars = expression.Vars();
        Valuable::vars_cont_t knowns;
		for (auto& v : lineVars) {
            auto& known = Known(v);
			if (known.size() == 1) {
                knowns.emplace(v, *known.begin());
			}
		}

        if (expression.IsSimple() == constants::zero) {
            if (expression == constants::zero)
                is = Valuable::YesNoMaybe::Yes;
            else if (expression.Distinct().contains(constants::zero)) {
                std::cout << "potentially ";
                is = Valuable::YesNoMaybe::Yes;
            }
			else {
				is = Valuable::YesNoMaybe::No;
			}
        } else if (knowns.size()) {
			expression.eval(knowns);
            expression.optimize();
            if (expression.IsInt()) {
                if (expression == 0) {
                    is = Valuable::YesNoMaybe::Yes;
                } else {
                    is = Valuable::YesNoMaybe::No;
                }
            } else if (expression.IsSimple()) {
                if (expression.Distinct().contains(constants::zero)) {
                    std::cout << "not neccessarily, but ";
                    is = Valuable::YesNoMaybe::Yes;
                } else {
                    is = Valuable::YesNoMaybe::No;
                }
            } else {
                IMPLEMENT
            }
        } else try {
            const auto total = MakesTotalEqu() ? Total().Link() : CalculateTotalExpression();
            if (total.IsSum()) {
                auto rest = total / expression;
                std::cout << "Total: " << total << std::endl
                          << total << " / " << expression << ": " << rest << std::endl;
                auto& totalSum = total.as<Sum>();
                if (lineVars.size() == 1) {
                    std::vector<Valuable> coefficients;
                    auto& va = *lineVars.begin();
                    auto totalGrade = totalSum.FillPolyCoeff(coefficients, va);
                    coefficients.clear();
                    if (expression.IsSum()) {
                        auto& lineSum = expression.as<Sum>();
                        auto lineGrade = lineSum.FillPolyCoeff(coefficients, va);
                        if (rest.IsSum()) {
                            auto restGrade = rest.as<Sum>().FillPolyCoeff(coefficients, va);
                            if (totalGrade == restGrade + lineGrade)
                                is = Valuable::YesNoMaybe::Yes;
                        }
                    } else if (expression.IsVa()) {
                        auto solutions = Solve(expression.as<Variable>());
                        if (solutions.size() == 1) {
                            if (solutions.cbegin()->operator==(0))
                                is = Valuable::YesNoMaybe::Yes;
                        }
                    }
                } else {
                    IMPLEMENT
                }
            } else if (total == constants::zero) {
            } else if (total.IsInt()) {
                is = Valuable::YesNoMaybe::No;
            } else {
                IMPLEMENT
            }
		}
		catch (...) {
			is = Valuable::YesNoMaybe::Maybe;
		}

		std::cout << '\n' << expression << " ?\n";
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
	sourceFilePath = path;
	Load(path);
}

Skrypt::Skrypt(std::istream & stream)
{
	MakesTotalEqu(true);
	Load(stream);
}

boost::filesystem::path Skrypt::FindModulePath(std::string_view name) const {
	auto path = sourceFilePath.parent_path() / name;
    if (!path.has_extension())
        path.replace_extension(".skrypt");
    if (!boost::filesystem::exists(path)) {
        path = path.filename();
    }
    return path;
}

Skrypt::module_t Skrypt::GetLoadedModule(std::string_view fileName) const {
    module_t loaded;
	std::shared_lock lock(modulesMapMutex);
	auto it = modules.find(fileName);
	if (it != modules.end()) {
		loaded = it->second;
	}
	return loaded;
}

bool Skrypt::IsModuleLoading(std::string_view name) const {
	IMPLEMENT

}

Skrypt::module_t Skrypt::Module(std::string_view fileName) {
    auto loaded = GetLoadedModule(fileName);
	// TODO : check if the module is being loaded
	// if (IsModuleLoading(fileName)) { }
    if (!loaded) {
        auto skrypt = std::make_shared<Skrypt>();
        skrypt->Echo({}); // Silence!
		skrypt->Load(FindModulePath(fileName));
        std::unique_lock lock(modulesMapMutex);
        auto module = modules.emplace(fileName, std::move(skrypt));
        loaded = module.first->second;
    }
	return loaded;
}

Skrypt::loading_module_t Skrypt::StartLoadingModule(std::string_view name) {
    return std::async(std::launch::async, &Skrypt::Module, this, name);
}

Skrypt::loading_modules_t Skrypt::LoadModules(const ::omnn::math::Valuable& v) {
    loading_modules_t loadingModules;
    for (auto& [name, var] : v.VaNames()) {
        auto dot = name.find('.');
        if (dot != std::string::npos) {
            auto moduleName = name.substr(0, dot);
            auto loading = loadingModules.find(std::string(moduleName));
            if (loading == loadingModules.end()) {
                // not loading yet
                {
                    std::shared_lock lock(modulesMapMutex);
                    auto loaded = modules.find(moduleName);
                    if (loaded != modules.end()) {
                        // already loaded
                        std::promise<module_t> promise;
                        promise.set_value(loaded->second);
                        loadingModules.emplace(moduleName, promise.get_future());
                        continue;
                    }
                }
                loadingModules.emplace(moduleName, StartLoadingModule(moduleName));
            }
        }
    }
    return loadingModules;
}

Skrypt::loading_modules_future_t Skrypt::StartLoadingModules(const Valuable& v) {
    return std::async(std::launch::async, &Skrypt::LoadModules, this, v);
}

void Skrypt::BackgroudLoadingModules(const ::omnn::math::Valuable& v) {
    modulesLoadingQueue.AddTask(&Skrypt::LoadModules, this, v);
}

std::string_view Skrypt::GetVariableName(const ::omnn::math::Variable& v) const {
	auto host = v.getVaHost();
	auto name = host->GetName(v.getId());
	return name;
}

std::string_view Skrypt::GetModuleName(std::string_view name) const {
    auto dot = name.find('.');
    if (dot == std::string::npos) {
        name = {};
    } else {
        name.remove_suffix(name.size() - dot);
    }
    return name;
}

std::string_view Skrypt::GetModuleName(const ::omnn::math::Variable& v) const {
    auto name = GetVariableName(v);
    return GetModuleName(name);
}

const ::omnn::math::Valuable::solutions_t& Skrypt::Known(const ::omnn::math::Variable& v)
{
	auto known = base::Known(v);
    if (known.size() == 0) {
        auto name = GetVariableName(v);
		auto moduleName = GetModuleName(name);
		auto module = Module(GetModuleName(name));
        name.remove_prefix(
			moduleName.size()
			+ 1 // '.'
			);
		auto& moduleVarNames = module->GetVarNames();
        auto moduleVarIt = moduleVarNames.find(name);
		if (moduleVarIt != moduleVarNames.end()) {
			auto& moduleVar = moduleVarIt->second;
            known = module->Known(moduleVar);
		}
	}
    return known;
}