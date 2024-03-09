#pragma once
#include <omnn/rt/tasq.h>
#include <omnn/math/System.h>
#include <omnn/math/VarHost.h>

#include <boost/typeof/typeof.hpp>
#include <boost/filesystem.hpp>
#include <boost/filesystem/fstream.hpp>

#include <map>
#include <shared_mutex>
#include <string_view>


namespace skrypt {

class Skrypt
    : public omnn::math::System
{
    using base = omnn::math::System;

    ::omnn::math::VarHost::ptr varHost = ::omnn::math::VarHost::make<std::string>();

    ::omnn::math::Valuable::va_names_t vars;
    bool echo = {};
    bool disjunctionParseMode = {};

    using module_t = std::shared_ptr<Skrypt>;
    using modules_t = std::map<std::string_view, module_t>;
    using loading_module_t = std::future<module_t>;
    using loading_modules_t = std::map<std::string_view, loading_module_t>;
    using loading_modules_future_t = std::future<loading_modules_t>;

    modules_t modules;
    mutable std::shared_mutex modulesMapMutex;
    mutable std::shared_mutex modulesLoadingMutex;
    modules_t modulesLoading;
    ::omnn::rt::StoringTasksQueue<loading_modules_t> modulesLoadingQueue;
    ::boost::filesystem::path sourceFilePath;

protected:
    void SetVarhost(decltype(varHost));
    constexpr bool DisjunctionParseMode() const { return disjunctionParseMode; }
    void DisjunctionParseMode(bool isDisjunctionMode) { disjunctionParseMode = isDisjunctionMode; }
    ::omnn::math::Product disjunction;
    bool ParseTotal(std::istream&, std::string_view&, std::function<void(::omnn::math::Valuable&&)>);

public:
    using value_type = omnn::math::Valuable;

    Skrypt() {}
    Skrypt(const boost::filesystem::path&);
    Skrypt(std::istream&);

    auto GetVarHost() const { return varHost; }
    const auto& GetVarNames() const { return vars; }

    using base::Add;

    bool Add(std::string_view);
    bool ParseNextLine(std::istream&, std::string_view&);
    void PrintVarKnowns(const omnn::math::Variable&);
    void ProcessQuestionLine(std::string_view&);

    /// <summary>
    /// Loads .skrypt file and returns InitialVarNames for input
    /// </summary>
    /// <returns></returns>
    const omnn::math::Valuable::va_names_t& Load(std::istream&);

    /// <summary>
    /// Loads .skrypt file and returns InitialVarNames for input
    /// </summary>
    /// <returns></returns>
    const omnn::math::Valuable::va_names_t& Load(const boost::filesystem::path&);

    /// <summary>
    /// Loads or gets loaded .skrypt file and returns Skrypt object reference
    /// </summary>
    /// <returns>Skrypt&</returns>
    /// <param name="name">The module name</param>
    module_t Module(std::string_view name);
    module_t Module(const ::omnn::math::Variable&);
    module_t GetLoadedModule(std::string_view fileName) const;

    boost::filesystem::path FindModulePath(std::string_view name) const;
    bool IsModuleLoading(std::string_view name) const;
    loading_module_t StartLoadingModule(std::string_view name);
    loading_modules_t LoadModules(const ::omnn::math::Valuable& v);
    loading_modules_future_t StartLoadingModules(const ::omnn::math::Valuable& v);

    void BackgroudLoadingModules(const ::omnn::math::Valuable& v);
    std::string_view GetVariableName(const ::omnn::math::Variable&) const;
    std::string_view GetModuleName(std::string_view variableName) const;
    std::string_view GetModuleName(const ::omnn::math::Variable&) const;
    const solutions_t& Known(const ::omnn::math::Variable& v);
    const ::omnn::math::Variable& MappedModuleVariable(const ::omnn::math::Variable&, module_t module = {});

    void Echo(bool e) { echo = e; }
};

} // namespace skrypt
