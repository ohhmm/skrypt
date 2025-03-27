
#include "Valuable.h"
#include <skrypt.h>

#include <boost/algorithm/string.hpp>
#include <boost/filesystem.hpp>
#include <boost/filesystem/fstream.hpp>
#include <boost/filesystem/path.hpp>
#include <boost/iostreams/device/mapped_file.hpp>
#include <boost/iostreams/filtering_stream.hpp>
#include <boost/dll/runtime_symbol_info.hpp>
#include <boost/dll/shared_library.hpp>

#include <fstream>
#include <iostream>
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

bool Skrypt::ParseTotal(std::istream& in, std::string_view& line, std::function<void(Valuable&&)> result)
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

const Variable& Skrypt::MappedModuleVariable(const Variable& v, module_t module) {
    auto name = GetVariableName(v);
    auto moduleName = GetModuleName(name);
    if (!moduleName.empty()) {
        name.remove_prefix(moduleName.size() + 1 // '.'
        );
        if (!module) {
            module = Module(moduleName);
        } else if (IsModuleLoading(moduleName)) {
            if(module != WaitTillModuleLoadingComplete(moduleName))
                LOG_AND_IMPLEMENT("Module provided does not match");
        }
        auto& moduleVarNames = module->GetVarNames();
        auto moduleVarIt = moduleVarNames.find(name);
        if (moduleVarIt != moduleVarNames.end()) {
            return moduleVarIt->second;
        } else {
            std::shared_lock lock(modulesLoadingMutex);
            auto loading = modulesLoading.find(moduleName);
            auto isModuleLoading = loading != modulesLoading.end();
            if (isModuleLoading) {
                auto loadingModule = loading->second;
                if (module != loadingModule) {
                    LOG_AND_IMPLEMENT(
                        "Either variable of different module or resolve race condition in module loading");
                } else {
                    auto moduleVarIt = module->GetVarNames().find(name);
                    if (moduleVarIt != module->GetVarNames().end()) {
                        return moduleVarIt->second;
                    } else {
                        LOG_AND_IMPLEMENT("Variable not found in the module");
                    }
                }
            } else {
                LOG_AND_IMPLEMENT("Module not found");
            }
        }
    }
    return v; // this module variable
}

void Skrypt::PrintVarKnowns(const Variable& v)
{
    auto module = Module(v);
    auto mappedModuleVariable = MappedModuleVariable(v, module);
    if (mappedModuleVariable.getVaHost() != v.getVaHost() || mappedModuleVariable != v) {
        std::cout << GetModuleName(v) << '.';
        module->PrintVarKnowns(mappedModuleVariable);
        return;
    }
	std::cout << v << " =";
	auto solutions = Solve(v);
	for (auto& solution : solutions) {
		std::cout << ' ' << solution;
	}
	std::cout << std::endl;
}

void Skrypt::PrintAllKnowns()
{
    for (auto& [name, var] : vars) {
        PrintVarKnowns(var);
    }
}

void Skrypt::BindTargetStream(std::ostream& o) {
    outputs.emplace_back(&o, [](auto){});
}

void Skrypt::BindTargetStream(std::shared_ptr<std::ostream> o) {
    outputs.emplace_back(std::move(o));
}

void Skrypt::BindTargetStream(const boost::filesystem::path& filepath){
    BindTargetStream(std::make_shared<boost::filesystem::ofstream>(filepath));
}

bool Skrypt::Add(Valuable&& v) {
    BackgroundLoadingModules(v);
    v.CollectVaNames(vars);
    bool ok = {};
    try
    {
        if (v.IsVa()) {
            PrintVarKnowns(v.as<Variable>());
            ok = true;
        } else {
            if(echo)
                std::cout << v << std::endl;
            if (ok && outputs.size()) {
                for (auto& o : outputs) {
                    *o << v << std::endl;
                }
            }
            if (DisjunctionParseMode())
                ok = disjunction.Add(std::move(v)) != disjunction.end();
            else
                ok = base::Add(std::move(v));
        }
    } catch (...) {
        if (echo && !ok) {
            std::cout << "Please, consider additional defining to the system to make this request" << std::endl;
        }
    }
    return ok;
}

bool Skrypt::Add(std::string_view line) {
	Valuable v(line, varHost);
    return Add(v);
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
    YesNoMaybe is = YesNoMaybe::Maybe;
    auto questionless = Questionless(line);
    if (questionless.empty()) {
        PrintAllKnowns();
    }
	else {
        Valuable expression(questionless, varHost);
        expression.SetView(Valuable::View::Equation);
        auto lineVars = expression.Vars();
        Valuable::vars_cont_t knowns;
        for (auto& v : lineVars) {
            auto& known = Known(v);
            if (known.size() == 1) {
                knowns.emplace(v, *known.begin());
            }
        }

        if (expression.IsSimple()) {
            if (expression.IsZero()) {
                is = YesNoMaybe::Yes;
            } else if (expression.Distinct().contains(constants::zero)) {
                std::cout << "potentially ";
                is = YesNoMaybe::Yes;
            }
			else {
                is = YesNoMaybe::No;
            }
        } else if (knowns.size()) {
            expression.eval(knowns);
            expression.optimize();
            if (expression.IsInt()) {
                if (expression.IsZero()) {
                    is = YesNoMaybe::Yes;
                } else {
                    is = YesNoMaybe::No;
                }
            } else if (expression.IsSimple()) {
                if (expression.Distinct().contains(constants::zero)) {
                    std::cout << "not neccessarily, but ";
                    is = YesNoMaybe::Yes;
                } else {
                    is = YesNoMaybe::No;
                }
            } else {
                LOG_AND_IMPLEMENT(expression);
            }
        } else
            try {
                const auto total = MakesTotalEqu() ? Total().Link() : CalculateTotalExpression();
                if (total.IsSum()) {
                    auto rest = total / expression;
                    std::cout << "Total: " << total << std::endl
                              << total << " / " << expression << ": " << rest << std::endl;
                    auto& totalSum = total.as<Sum>();
                    if (lineVars.size() == 1) {
                        auto& va = *lineVars.begin();
                        std::vector<Valuable> coefficients;
                        auto totalGrade = totalSum.FillPolynomialCoefficients(coefficients, va);
                        coefficients.clear();
                        if (expression.IsSum()) {
                            auto& lineSum = expression.as<Sum>();
                            auto lineGrade = lineSum.FillPolynomialCoefficients(coefficients, va);
                            if (rest.IsSum()) {
                                auto restGrade = rest.as<Sum>().FillPolynomialCoefficients(coefficients, va);
                                if (totalGrade == restGrade + lineGrade)
                                    is = YesNoMaybe::Yes;
                            }
                        } else if (expression.IsVa()) {
                            auto solutions = Solve(expression.as<Variable>());
                            if (solutions.size() == 1) {
                                if (solutions.cbegin()->IsZero())
                                    is = YesNoMaybe::Yes;
                            }
                        }
                    } else {
                        IMPLEMENT
                    }
                } else if (total.IsZero()) {
                } else if (total.IsInt()) {
                    is = YesNoMaybe::No;
                } else {
                    IMPLEMENT
                }
            } catch (...) {
                is = YesNoMaybe::Maybe;
            }

        std::cout << '\n' << expression << " ?\n";
        if (is == YesNoMaybe::Yes)
            std::cout << "YES";
        else if (is == YesNoMaybe::No)
            std::cout << "NO";
        else
            std::cout << "IDK";
        std::cout << std::endl;
    }
}

bool Skrypt::ParseNextLine(std::istream& in, std::string_view& line)
{
    auto comment = line.find_first_of('#');
    if (comment != std::string_view::npos)
        line.remove_suffix(comment);
    if (!line.empty()) {
        auto bracePos = FindBracePos(line);
        auto brace = bracePos == std::string_view::npos ? 0 : line[bracePos];
        line.remove_prefix(bracePos + 1);
        switch (brace) {
        case '{': {
            if (disjunctionParseMode) {
                Skrypt subscript;
                subscript.SetVarhost(GetVarHost());
                subscript.Echo(echo);
                return subscript.ParseTotal(in, line,
                                            [this](::omnn::math::Valuable&& result) { Add(std::move(result)); });
            } else if (Expressions().empty()) {
                if (line.empty())
                    return true;
                else
                    break;
            } else {
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
            return subscript.ParseTotal(in, line, [this](::omnn::math::Valuable&& result) { Add(std::move(result)); });
        }
        case ']': {
            if (DisjunctionParseMode()) {
                Add(std::move(disjunction));
            } else {
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
        } else {
            Valuable expression;
            try {
                expression = Valuable(line, varHost);
            } catch (std::runtime_error error) {
                std::cout << "Expression parsing error: " << error.what() << std::endl;
            } catch (...) {
                std::cout << "Expression parsing error." << std::endl;
            }
            if (expression.IsSimple()) {
                std::cout << expression << std::endl;
            } else {
                Add(std::move(expression));
            }
        }
        line = {};
    }
    return true;
}

const Valuable::va_names_t& Skrypt::Load(std::istream& in)
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

const Valuable::va_names_t& Skrypt::Load(const boost::filesystem::path & path)
{
	std::cout << "Loading " << path << '\n' << std::endl;
    auto filepath = boost::filesystem::exists(path)
        ? path
        : FindModulePath({path.string()});
    boost::filesystem::ifstream stream(filepath);
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
    auto base = boost::dll::program_location();
    if (!boost::filesystem::exists(path)) {
        path = base.parent_path() / name;
        if (!path.has_extension())
            path.replace_extension(".skrypt");
    }

    if (!boost::filesystem::exists(path)) {
        auto loc = boost::dll::this_line_location();
        if (loc != base) {
            path = loc.parent_path() / name;
            if (!path.has_extension())
                path.replace_extension(".skrypt");
        }
    }

    if (!boost::filesystem::exists(path)) {
        for (auto& searchPath : moduleFileSearchAdditionalPaths) {
			path = searchPath / name;
			if (!path.has_extension())
				path.replace_extension(".skrypt");
            if (boost::filesystem::exists(path)) {
				break;
			}
		}
    }

    if (!boost::filesystem::exists(path)) {
        std::cerr << "Module not found: " << path << std::endl;
    }
    return path;
}

Skrypt::module_t Skrypt::GetLoadedModule(std::string_view fileName) {
    module_t loaded;
    std::shared_lock lock(modulesMapMutex);
    auto it = modules.find(fileName);
    if (it != modules.end()) {
        loaded = it->second;
    }
    return loaded;
}

bool Skrypt::IsModuleLoading(std::string_view name) {
    std::shared_lock lock(modulesLoadingMutex);
    return modulesLoading.contains(name);
}

void Skrypt::WaitAllModulesLoadingComplete() {
    while (!modulesLoadingQueue.empty()) {
        modulesLoadingQueue.PeekNextResult();
    }
}

Skrypt::module_t Skrypt::Module(std::string_view name) {
    auto module = GetLoadedModule(name);
    if (!module) {
		auto isCurrentlyLoading = IsModuleLoading(name);
        if (isCurrentlyLoading) {
			std::unique_lock lock(modulesLoadingMutex);
			auto loading = modulesLoading.find(std::string(name));
			if (loading != modulesLoading.end()) {
				module = loading->second;
            } else {
				// probably loaded in the meantime
				module = GetLoadedModule(name);
            }
        }
        if (!module) {
			std::unique_lock lock(modulesLoadingMutex);
            module = std::make_shared<Skrypt>();
            auto insertedLoadingModules = modulesLoading.emplace(name, module);
			auto inserted = insertedLoadingModules.second;
            isCurrentlyLoading = !inserted;
            if (isCurrentlyLoading) {
				module = insertedLoadingModules.first->second;
			}
        }
        {
			std::unique_lock lock(modulesMapMutex);
            auto insertedToModules = modules.emplace(name, module);
            auto inserted = insertedToModules.second;
            if (!inserted) {
				module = insertedToModules.first->second;
			}
		}
        if (!isCurrentlyLoading) {
            module->Echo({});
            module->moduleFileSearchAdditionalPaths = moduleFileSearchAdditionalPaths;
            module->Load(FindModulePath(name));

			std::unique_lock lock(modulesLoadingMutex);
            auto loading = modulesLoading.find(name);
            if (loading != modulesLoading.end()) {
				modulesLoading.erase(loading);
            } else {
                std::cerr << "Module " << name << " was not found in the loading map" << std::endl; // race condition
            }
        } else {
            WaitTillModuleLoadingComplete(name);
        }
    }
    return module;
}

Skrypt::loading_module_t Skrypt::StartLoadingModule(std::string_view moduleName) {
    std::cout << "Module " << moduleName << " loading started" << std::endl;
    return std::async(std::launch::async, [this, moduleName]() {
        auto module = Module(moduleName);
        std::cout << "Module " << moduleName << " loaded:" << std::endl;
        module->PrintAllKnowns();

        // Import known variables from the module TODO: through Binder
        const auto& moduleVarNames = module->GetVarNames();
        for (auto& [name, var] : moduleVarNames) {
            auto& knowns = module->Known(var);
            auto knownSolutionsNumber = knowns.size();
            if (knownSolutionsNumber) {
                std::string varName(moduleName);
                varName += '.';
                varName += name;
                auto& v = varHost->Host(varName);
                if (knownSolutionsNumber == 1)
                    Add(v, *knowns.begin());
                else {
                    auto& solutions = Yarns(v)[{}];
                    for (auto& solution : knowns) {
                        solutions.insert(solution);
					}
                }
            }
        }
        return module;
    });
}

Skrypt::loading_modules_t Skrypt::LoadModules(const ::omnn::math::Valuable& v) {
    loading_modules_t loadingModules;
    for (auto& [name, var] : v.VaNames()) {
        auto dot = name.find('.');
        if (dot != std::string::npos) {
            auto moduleName = name.substr(0, dot);
            auto loading = loadingModules.find(std::string(moduleName));
            if (loading == loadingModules.end()) { // not loading here yet
                auto loaded = GetLoadedModule(moduleName);
                if (loaded)
                {
                    std::promise<module_t> promise;
                    promise.set_value(loaded);
                    loadingModules.emplace(moduleName, promise.get_future());
                } else {
                    loadingModules.emplace(moduleName, StartLoadingModule(moduleName));
                }
            }
        }
    }
    return loadingModules;
}

Skrypt::loading_modules_future_t Skrypt::StartLoadingModules(const Valuable& v) {
    return std::async(std::launch::async, &Skrypt::LoadModules, this, v);
}

void Skrypt::BackgroundLoadingModules(const ::omnn::math::Valuable& v) {
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

Skrypt::module_t Skrypt::Module(const ::omnn::math::Variable& v) {
    auto name = GetVariableName(v);
    auto moduleName = GetModuleName(name);
    if (moduleName.empty()) {
        return {};
    } else
        return Module(moduleName);
}

Skrypt::module_t Skrypt::WaitTillModuleLoadingComplete(std::string_view name) {
    bool isModuleLoading = IsModuleLoading(name);
    if (isModuleLoading) {
        do {
            if (isModuleLoading) {
                auto loaded = modulesLoadingQueue.PeekNextResult();
                auto it = loaded.find(name);
                if (it != loaded.end()) {
                    return it->second.get();
                }
                isModuleLoading = IsModuleLoading(name);
                if (isModuleLoading) {
                    std::this_thread::yield();
                } else {
                    break;
                }
            }
        } while (isModuleLoading);
    }
    return Module(name);
}

const Valuable::solutions_t& Skrypt::Known(const Variable& v)
{
	auto known = std::cref(base::Known(v));
    if (known.get().size() == 0) {
        auto module = Module(v);
        auto mappedModuleVariable = MappedModuleVariable(v, module);
        if (mappedModuleVariable.getVaHost() != v.getVaHost() || mappedModuleVariable != v) {
            known = std::cref(module->Known(mappedModuleVariable));
        }
        if (known.get().size() == 0) {
            auto name = GetVariableName(v);
            auto moduleName = GetModuleName(name);
            if (!moduleName.empty()) {
                auto module = Module(moduleName);
                name.remove_prefix(moduleName.size() + 1 // '.'
                );
                auto& moduleVarNames = module->GetVarNames();
                auto moduleVarIt = moduleVarNames.find(name);
                if (moduleVarIt == moduleVarNames.end()) {
                    WaitTillModuleLoadingComplete(moduleName);
                    moduleVarIt = moduleVarNames.find(name);
                }
                if (moduleVarIt != moduleVarNames.end()) {
                    auto& moduleVar = moduleVarIt->second;
                    known = std::cref(module->Known(moduleVar));
                } else {
                    auto solution = module->Solve(moduleVarIt->second);
                    if (solution.size()) {
                        known = std::cref(module->Known(moduleVarIt->second));
                        if (known.get().size() == 0) {
                            IMPLEMENT
                        }
                    }
                }
            } else {
                WaitAllModulesLoadingComplete();
                known = std::cref(base::Known(v));
                if (known.get().size() == 0) {
                    Solve(v);
					known = std::cref(base::Known(v));
				}
			}
		}
	}
    return known;
}

// TODO : delete this in favour of moving non-copiable modules to shared modules loader
Skrypt::Skrypt(const skrypt::Skrypt& that)
    : base(that)
    , varHost(that.varHost)
    , vars(that.vars)
    , echo(that.echo)
    , disjunctionParseMode(that.disjunctionParseMode)
    , modules(that.modules)
    , modulesLoading(that.modulesLoading)
    , sourceFilePath(that.sourceFilePath)
    , moduleFileSearchAdditionalPaths(that.moduleFileSearchAdditionalPaths)
{
}

const Valuable::va_names_t& Skrypt::LoadFreesweeperState(const std::string& state_text, 
    std::shared_ptr<class HierarchicalKnowledgeMatrix> matrix)
{
    // Create a matrix if none was provided
    if (!matrix) {
        // Parse the state_text to determine dimensions
        size_t rows = 0, cols = 0;
        std::istringstream iss(state_text);
        std::string line;
        while (std::getline(iss, line)) {
            rows++;
            if (cols == 0) {
                cols = line.length();
            }
        }
        
        matrix = std::make_shared<HierarchicalKnowledgeMatrix>(rows, cols, 0.5);
    }
    
    // Parse the Freesweeper state and populate the matrix
    std::istringstream iss(state_text);
    std::string line;
    size_t row = 0;
    
    while (std::getline(iss, line) && row < matrix->GetDimensions().first) {
        for (size_t col = 0; col < line.length() && col < matrix->GetDimensions().second; col++) {
            char cell = line[col];
            
            if (cell >= '1' && cell <= '8') {
                // Number cells indicate adjacent mines
                int adjacent_mines = cell - '0';
                
                // Create variables for this cell
                std::string var_name = "cell_" + std::to_string(row) + "_" + std::to_string(col);
                auto& var = varHost->Host(var_name);
                vars[var_name] = var;
                
                // Update matrix with adjacent mine count
                matrix->SetValue(row, col, 0.0); // Not a mine
                
                // Create a deeper matrix for adjacent mines info
                auto deeper = matrix->AddDeeperMatrix(row, col);
                deeper->SetValue(0, 0, adjacent_mines / 8.0); // Store adjacent count as probability
            } else if (cell == '*') {
                // Mine cell
                std::string var_name = "cell_" + std::to_string(row) + "_" + std::to_string(col);
                auto& var = varHost->Host(var_name);
                vars[var_name] = var;
                
                matrix->SetValue(row, col, 1.0); // 100% chance of mine
            } else if (cell == '.') {
                // Unknown cell
                std::string var_name = "cell_" + std::to_string(row) + "_" + std::to_string(col);
                auto& var = varHost->Host(var_name);
                vars[var_name] = var;
                
                // Default probability (0.5) remains
            } else if (cell == ' ' || cell == '0') {
                // Empty cell
                std::string var_name = "cell_" + std::to_string(row) + "_" + std::to_string(col);
                auto& var = varHost->Host(var_name);
                vars[var_name] = var;
                
                matrix->SetValue(row, col, 0.0); // 0% chance of mine
            }
        }
        row++;
    }
    
    // Add the matrix to the system
    Add(Valuable("freesweeper_state_loaded = 1"));
    
    return vars;
}

const Valuable::va_names_t& Skrypt::LoadSudokuState(const std::string& state_text,
    std::shared_ptr<class HierarchicalKnowledgeMatrix> matrix)
{
    // Create a matrix if none was provided
    if (!matrix) {
        // Sudoku is typically 9x9
        matrix = std::make_shared<HierarchicalKnowledgeMatrix>(9, 9, 0.0);
    }
    
    // Parse the Sudoku state and populate the matrix
    std::istringstream iss(state_text);
    std::string line;
    size_t row = 0;
    
    while (std::getline(iss, line) && row < matrix->GetDimensions().first) {
        // Skip empty lines
        if (line.empty()) {
            continue;
        }
        
        size_t col = 0;
        for (char cell : line) {
            if (col >= matrix->GetDimensions().second) {
                break;
            }
            
            // Skip non-digit and non-dot characters
            if (!isdigit(cell) && cell != '.') {
                continue;
            }
            
            // Create variable for this cell
            std::string var_name = "sudoku_" + std::to_string(row) + "_" + std::to_string(col);
            auto& var = varHost->Host(var_name);
            vars[var_name] = var;
            
            if (cell == '.') {
                // Empty cell - add a deeper matrix with probabilities for each possible value
                auto deeper = matrix->AddDeeperMatrix(row, col);
                
                // Initially, all values 1-9 are equally possible
                for (int val = 1; val <= 9; val++) {
                    deeper->SetValue(0, val, 1.0/9.0);
                }
            } else if (isdigit(cell) && cell != '0') {
                // Known value
                int value = cell - '0';
                
                // Set the value in the matrix
                matrix->SetValue(row, col, value);
                
                // Add the known value to the system
                Add(var, Valuable(value));
            }
            
            col++;
        }
        row++;
    }
    
    // Add the matrix to the system
    Add(Valuable("sudoku_state_loaded = 1"));
    
    return vars;
}

const Valuable::va_names_t& Skrypt::LoadCppCode(const std::string& code,
    std::shared_ptr<class HierarchicalKnowledgeMatrix> matrix)
{
    // Create a matrix if none was provided
    // For C++ code, we'll use a matrix where:
    // - Rows represent lines of code
    // - Columns represent different properties (e.g., complexity, nesting level, etc.)
    if (!matrix) {
        // Count lines in the code
        size_t line_count = 1; // Start with 1 for the last line without newline
        for (char c : code) {
            if (c == '\n') {
                line_count++;
            }
        }
        
        // Use 10 columns for different code properties
        matrix = std::make_shared<HierarchicalKnowledgeMatrix>(line_count, 10, 0.0);
    }
    
    // Parse the C++ code and populate the matrix
    std::istringstream iss(code);
    std::string line;
    size_t row = 0;
    size_t nesting_level = 0;
    
    while (std::getline(iss, line) && row < matrix->GetDimensions().first) {
        // Create variable for this line
        std::string var_name = "cpp_line_" + std::to_string(row);
        auto& var = varHost->Host(var_name);
        vars[var_name] = var;
        
        // Track nesting level
        for (char c : line) {
            if (c == '{') nesting_level++;
            if (c == '}') nesting_level = (nesting_level > 0) ? nesting_level - 1 : 0;
        }
        
        // Column 0: Line length (normalized to 0-1 range, assuming max length of 200)
        double line_length = std::min(1.0, line.length() / 200.0);
        matrix->SetValue(row, 0, line_length);
        
        // Column 1: Nesting level (normalized to 0-1 range, assuming max nesting of 10)
        double norm_nesting = std::min(1.0, nesting_level / 10.0);
        matrix->SetValue(row, 1, norm_nesting);
        
        // Column 2: Contains function call (simple heuristic)
        bool has_function_call = line.find('(') != std::string::npos && 
                                line.find(')') != std::string::npos;
        matrix->SetValue(row, 2, has_function_call ? 1.0 : 0.0);
        
        // Column 3: Contains conditional
        bool has_conditional = line.find("if") != std::string::npos || 
                              line.find("else") != std::string::npos ||
                              line.find("switch") != std::string::npos;
        matrix->SetValue(row, 3, has_conditional ? 1.0 : 0.0);
        
        // Column 4: Contains loop
        bool has_loop = line.find("for") != std::string::npos || 
                       line.find("while") != std::string::npos ||
                       line.find("do") != std::string::npos;
        matrix->SetValue(row, 4, has_loop ? 1.0 : 0.0);
        
        // Add deeper matrix for more detailed analysis
        auto deeper = matrix->AddDeeperMatrix(row, 5);
        
        // Store the actual line text as a series of character codes
        for (size_t i = 0; i < line.length() && i < 100; i++) {
            // Normalize ASCII value to 0-1 range
            double char_val = static_cast<double>(line[i]) / 255.0;
            deeper->SetValue(0, i, char_val);
        }
        
        row++;
    }
    
    // Add the matrix to the system
    Add(Valuable("cpp_code_loaded = 1"));
    
    return vars;
}

const Valuable::va_names_t& Skrypt::LoadCallstack(const std::string& callstack,
    std::shared_ptr<class HierarchicalKnowledgeMatrix> matrix)
{
    // Create a matrix if none was provided
    // For callstack, we'll use a matrix where:
    // - Rows represent stack frames
    // - Columns represent different properties of each frame
    if (!matrix) {
        // Count lines in the callstack to estimate number of frames
        size_t frame_count = 1; // Start with 1 for the last line without newline
        for (char c : callstack) {
            if (c == '\n') {
                frame_count++;
            }
        }
        
        // Use 5 columns for different frame properties
        matrix = std::make_shared<HierarchicalKnowledgeMatrix>(frame_count, 5, 0.0);
    }
    
    // Parse the callstack and populate the matrix
    std::istringstream iss(callstack);
    std::string line;
    size_t row = 0;
    
    while (std::getline(iss, line) && row < matrix->GetDimensions().first) {
        // Skip empty lines
        if (line.empty()) {
            continue;
        }
        
        // Create variable for this frame
        std::string var_name = "frame_" + std::to_string(row);
        auto& var = varHost->Host(var_name);
        vars[var_name] = var;
        
        // Column 0: Frame has function name
        bool has_function = line.find('(') != std::string::npos;
        matrix->SetValue(row, 0, has_function ? 1.0 : 0.0);
        
        // Column 1: Frame has line number
        bool has_line_number = false;
        for (size_t i = 0; i < line.length(); i++) {
            if (isdigit(line[i]) && i > 0 && line[i-1] == ':') {
                has_line_number = true;
                break;
            }
        }
        matrix->SetValue(row, 1, has_line_number ? 1.0 : 0.0);
        
        // Column 2: Frame is from system library (simple heuristic)
        bool is_system_lib = line.find("/lib/") != std::string::npos || 
                            line.find("/usr/lib/") != std::string::npos;
        matrix->SetValue(row, 2, is_system_lib ? 1.0 : 0.0);
        
        // Add deeper matrix for more detailed analysis
        auto deeper = matrix->AddDeeperMatrix(row, 3);
        
        // Extract function name if available
        size_t func_start = line.find_last_of('/');
        if (func_start == std::string::npos) {
            func_start = 0;
        } else {
            func_start++;
        }
        
        size_t func_end = line.find('(', func_start);
        if (func_end == std::string::npos) {
            func_end = line.length();
        }
        
        std::string func_name = line.substr(func_start, func_end - func_start);
        
        // Store function name characters
        for (size_t i = 0; i < func_name.length() && i < 50; i++) {
            // Normalize ASCII value to 0-1 range
            double char_val = static_cast<double>(func_name[i]) / 255.0;
            deeper->SetValue(0, i, char_val);
        }
        
        row++;
    }
    
    // Add the matrix to the system
    Add(Valuable("callstack_loaded = 1"));
    
    return vars;
}

const Valuable::va_names_t& Skrypt::LoadCoredump(const boost::filesystem::path& coredump_path,
    std::shared_ptr<class HierarchicalKnowledgeMatrix> matrix)
{
    // Create a matrix if none was provided
    // For coredump, we'll use a matrix where:
    // - Rows represent different sections of the coredump
    // - Columns represent different properties of each section
    if (!matrix) {
        // Use a fixed size for coredump analysis
        // 20 sections with 10 properties each
        matrix = std::make_shared<HierarchicalKnowledgeMatrix>(20, 10, 0.0);
    }
    
    // Check if the coredump file exists
    if (!boost::filesystem::exists(coredump_path)) {
        std::cerr << "Coredump file not found: " << coredump_path << std::endl;
        
        // Add error indicator to the system
        Add(Valuable("coredump_load_error = 1"));
        
        return vars;
    }
    
    // Use boost::iostreams to read the beginning of the coredump file
    // This is a simplified approach - real coredump analysis would require more sophisticated parsing
    boost::iostreams::mapped_file_source file;
    try {
        file.open(coredump_path);
    } catch (const std::exception& e) {
        std::cerr << "Error opening coredump file: " << e.what() << std::endl;
        
        // Add error indicator to the system
        Add(Valuable("coredump_load_error = 1"));
        
        return vars;
    }
    
    // Create variables for coredump metadata
    auto& var_path = varHost->Host("coredump_path");
    vars["coredump_path"] = var_path;
    Add(var_path, Valuable(coredump_path.string()));
    
    auto& var_size = varHost->Host("coredump_size");
    vars["coredump_size"] = var_size;
    Add(var_size, Valuable(file.size()));
    
    // Analyze the first few bytes to determine if it's a valid ELF file (simplified)
    if (file.size() >= 4 && file.data()[0] == 0x7F && file.data()[1] == 'E' && 
        file.data()[2] == 'L' && file.data()[3] == 'F') {
        
        // It's an ELF file, likely a valid coredump
        auto& var_valid = varHost->Host("coredump_valid");
        vars["coredump_valid"] = var_valid;
        Add(var_valid, Valuable(1));
        
        // Set basic properties in the matrix
        matrix->SetValue(0, 0, 1.0); // Valid ELF file
        
        // Determine ELF class (32 or 64 bit)
        if (file.size() >= 5) {
            bool is_64bit = (file.data()[4] == 2);
            matrix->SetValue(0, 1, is_64bit ? 1.0 : 0.0);
            
            auto& var_64bit = varHost->Host("coredump_64bit");
            vars["coredump_64bit"] = var_64bit;
            Add(var_64bit, Valuable(is_64bit ? 1 : 0));
        }
        
        // Create a deeper matrix for the first section to store the ELF header bytes
        auto deeper = matrix->AddDeeperMatrix(0, 2);
        
        // Store the first 64 bytes of the file
        for (size_t i = 0; i < 64 && i < file.size(); i++) {
            // Normalize byte value to 0-1 range
            double byte_val = static_cast<double>(static_cast<unsigned char>(file.data()[i])) / 255.0;
            deeper->SetValue(0, i, byte_val);
        }
    } else {
        // Not a valid ELF file
        auto& var_valid = varHost->Host("coredump_valid");
        vars["coredump_valid"] = var_valid;
        Add(var_valid, Valuable(0));
        
        matrix->SetValue(0, 0, 0.0); // Not a valid ELF file
    }
    
    // Close the file
    file.close();
    
    // Add the matrix to the system
    Add(Valuable("coredump_loaded = 1"));
    
    return vars;
}
