
#include <omnn/math/Variable.h>
using namespace omnn::math;

#include <boost/compute.hpp>
#include <boost/compute/core.hpp>
#include <boost/lambda2.hpp>
#include <boost/multiprecision/cpp_int.hpp>
#include <boost/program_options.hpp>


uint64_t N = 0;
DECL_VA(i);
DECL_VA(n);
Valuable::va_names_t Names = {{"i", i}, {"n", n}};

auto BuildFormula() {
    std::cout << "For i=[0;" << N << ") enter pattern f(i):" << std::endl;
    std::string s;
    std::cin >> s;
//    s="i*i*i + -1*i +-1*i";
    Valuable pattern(s, Names);
// TODO:   std::cin >> pattern;
    
    std::cout << std::endl;
    return pattern;
}

auto ComputeUnitsWinner = []() -> boost::compute::device {
    auto devices = boost::compute::system::devices();
    if (devices.size() == 0) {
        std::cout << "Please, install an OpenCL driver." << std::endl;
        exit(-1);
    }

    auto cuwinner = boost::compute::system::default_device();
    for(auto& p: boost::compute::system::platforms()) {
        for(auto& d: p.devices()) {
            std::cout << d.name() << ':' << d.compute_units() << std::endl;
            if (d.compute_units() > cuwinner.compute_units())
                cuwinner = d;
        }
    }

    std::cout << "max_work_group_size: " << cuwinner.max_work_group_size() << std::endl;

    return cuwinner;
}();



int main()
{
    std::cout << "N=";
    std::cin >> N;
    std::cout << std::endl;

    auto y = BuildFormula();
    std::cout << "Y = " << y << std::endl;
    auto openCLcode = y.OpenCL();
    std::cout << "\nY code:\n" << openCLcode << std::endl;

    // build OpenCL kernel
    boost::compute::context context(ComputeUnitsWinner);
    boost::compute::command_queue queue(context, ComputeUnitsWinner);
    boost::compute::kernel k(boost::compute::program::build_with_source(openCLcode, context), "f");

    auto wgsz = N;
    auto sz = wgsz * sizeof(cl_float);
    boost::compute::buffer c(context, sz);
    k.set_arg(0, c);

    // run the add kernel
    queue.enqueue_1d_range_kernel(k, 0, wgsz, 0);

    // transfer results to the host array 'c'
    std::vector<float_t> Y(wgsz);
    queue.enqueue_read_buffer(c, 0, sz, &Y[0]);
    queue.finish();

    std::cout << "Result[" << Y.size() << "]:\n";
    for(auto& item: Y)
        std::cout << ' ' << item;
    std::cout << std::endl;

	return 0;
}
