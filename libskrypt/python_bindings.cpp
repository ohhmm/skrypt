#include <skrypt.h>
#include <boost/python.hpp>
#include <string>
#include <vector>

#ifdef OPENMIND_BUILD_PYTHON_BINDINGS

using namespace boost::python;
using namespace skrypt;

namespace {
    // Helper function to convert va_names_t to Python dictionary
    boost::python::dict va_names_to_dict(const omnn::math::Valuable::va_names_t& names) {
        boost::python::dict result;
        for (const auto& [name, var] : names) {
            result[name] = var;
        }
        return result;
    }
}

BOOST_PYTHON_MODULE(skrypt)
{
    // Expose Skrypt
    class_<Skrypt, bases<omnn::math::System>>("Skrypt", init<>())
        .def(init<const boost::filesystem::path&>())
        .def(init<std::istream&>())
        .def("Load", static_cast<const omnn::math::Valuable::va_names_t& (Skrypt::*)(std::istream&)>(&Skrypt::Load), 
             return_value_policy<copy_const_reference>())
        .def("Load", static_cast<const omnn::math::Valuable::va_names_t& (Skrypt::*)(const boost::filesystem::path&)>(&Skrypt::Load), 
             return_value_policy<copy_const_reference>())
        .def("LoadFreesweeperState", &Skrypt::LoadFreesweeperState, return_value_policy<copy_const_reference>())
        .def("LoadSudokuState", &Skrypt::LoadSudokuState, return_value_policy<copy_const_reference>())
        .def("LoadCppCode", &Skrypt::LoadCppCode, return_value_policy<copy_const_reference>())
        .def("LoadCallstack", &Skrypt::LoadCallstack, return_value_policy<copy_const_reference>())
        .def("LoadCoredump", &Skrypt::LoadCoredump, return_value_policy<copy_const_reference>())
        .def("GetVarHost", &Skrypt::GetVarHost)
        .def("GetVarNames", &Skrypt::GetVarNames, return_value_policy<copy_const_reference>())
        .def("Add", static_cast<bool (Skrypt::*)(std::string_view)>(&Skrypt::Add))
        .def("PrintVarKnowns", &Skrypt::PrintVarKnowns)
        .def("PrintAllKnowns", &Skrypt::PrintAllKnowns)
        .def("Module", static_cast<Skrypt::module_t (Skrypt::*)(std::string_view)>(&Skrypt::Module))
        .def("Module", static_cast<Skrypt::module_t (Skrypt::*)(const ::omnn::math::Variable&)>(&Skrypt::Module))
        .def("GetLoadedModule", &Skrypt::GetLoadedModule)
        .def("FindModulePath", &Skrypt::FindModulePath)
        .def("AddModuleSearchDirPath", &Skrypt::AddModuleSearchDirPath)
        .def("IsModuleLoading", &Skrypt::IsModuleLoading)
        .def("WaitTillModuleLoadingComplete", &Skrypt::WaitTillModuleLoadingComplete)
        .def("WaitAllModulesLoadingComplete", &Skrypt::WaitAllModulesLoadingComplete)
        .def("GetVariableName", &Skrypt::GetVariableName)
        .def("GetModuleName", static_cast<std::string_view (Skrypt::*)(std::string_view) const>(&Skrypt::GetModuleName))
        .def("GetModuleName", static_cast<std::string_view (Skrypt::*)(const ::omnn::math::Variable&) const>(&Skrypt::GetModuleName))
        .def("Known", &Skrypt::Known, return_value_policy<copy_const_reference>())
        .def("MappedModuleVariable", &Skrypt::MappedModuleVariable, return_value_policy<copy_const_reference>())
        .def("Echo", &Skrypt::Echo)
        ;
}

#endif // OPENMIND_BUILD_PYTHON_BINDINGS
