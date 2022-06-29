
#include <omnn/math/Variable.h>
using namespace omnn::math;

#include <boost/archive/binary_oarchive.hpp>
#include <boost/compute.hpp>
#include <boost/compute/core.hpp>
#include <boost/lambda2.hpp>
#include <boost/multiprecision/cpp_int.hpp>
#include <boost/program_options.hpp>
#include <boost/serialization/vector.hpp>
#include "gen.h"


namespace {
	DECL_VA(i);
	DECL_VA(n);

	auto ComputeUnitsWinner = []() -> boost::compute::device {
		auto devices = boost::compute::system::devices();
		if (devices.size() == 0) {
			std::cout << "Please, install an OpenCL driver." << std::endl;
			exit(-1);
		}

		auto cuwinner = boost::compute::system::default_device();
		for (auto& p : boost::compute::system::platforms()) {
			for (auto& d : p.devices()) {
				std::cout << d.name() << ':' << d.compute_units() << std::endl;
				if (d.compute_units() > cuwinner.compute_units())
					cuwinner = d;
			}
		}

		std::cout << "max_work_group_size: " << cuwinner.max_work_group_size() << std::endl;

		return cuwinner;
	}();
}

namespace gen {

	const Valuable::va_names_t& InitialVarNames() {
		Valuable::va_names_t Names = { {"i", i}, {"n", n} };
		return names;
	}

	Valuable BuildFormula(const std::string& s, const Valuable::va_names_t& varNames) {
		auto names = varNames;
		names["i"] = i;
		Valuable pattern(s, varNames);
		pattern.Eval(n, N);
		return pattern;
	}

	Valuable BuildFormula(const std::string& s, const Valuable::vars_cont_t& knownValues, const Valuable::va_names_t& varNames) {
		auto formula = BuildFormula(s, varNames);
		if (formula.eval(knownValues)) {
			//formula.optimize();
		}
		return formula;
	}

	Valuable BuildFormula(const std::string& s, const Valuable& nValue, const Valuable::va_names_t& varNames) {
		auto formula = BuildFormula(s, varNames);
		formula.Eval(n, nValue);
		return formula;
	}


	bool Generate(const Valuable& pattern, float* data, unsigned wgsz) {
		// build OpenCL kernel
		boost::compute::context context(ComputeUnitsWinner);
		boost::compute::command_queue queue(context, ComputeUnitsWinner);
		boost::compute::kernel k(boost::compute::program::build_with_source(openCLcode, context), "f");

		auto sz = wgsz * sizeof(cl_float);
		boost::compute::buffer ctx(context, sz);
		k.set_arg(0, ctx);

		// run the add kernel
		queue.enqueue_1d_range_kernel(k, 0, wgsz, 0);

		// transfer results to the host array 'c'
		std::vector<float_t> Y(wgsz);
		queue.enqueue_read_buffer(ctx, 0, sz, &Y[0]);
		queue.finish();
	}

}