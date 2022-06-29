#include <omnn/math/Variable.h>

namespace gen {
	const omnn::math::Valuable::va_names_t& InitialVarNames();
	omnn::math::Valuable BuildFormula(const std::string& s, const omnn::math::Valuable::va_names_t& varNames = InitialVarNames());
	omnn::math::Valuable BuildFormula(const std::string& s, const omnn::math::Valuable& n, const omnn::math::Valuable::va_names_t& varNames = InitialVarNames());
	omnn::math::Valuable BuildFormula(const std::string& s, const omnn::math::Valuable::vars_cont_t& knownValues, const omnn::math::Valuable::va_names_t& varNames = InitialVarNames());

	bool Generate(const omnn::math::Valuable& pattern, float* data, unsigned n);
}
