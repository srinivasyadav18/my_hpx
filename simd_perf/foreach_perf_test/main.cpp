#include <hpx/hpx.hpp>
#include <hpx/config.hpp>
#include <hpx/hpx_init.hpp>
#include <hpx/modules/testing.hpp>
#include <hpx/modules/timing.hpp>
#include <hpx/include/datapar.hpp>
#include <hpx/parallel/util/loop.hpp>

#include <cstddef>
#include <cstdint>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <type_traits>
#include <vector>
#include <algorithm>
#include <numeric>

using hpx::program_options::options_description;
using hpx::program_options::value;
using hpx::program_options::variables_map;

template <typename T>
void test(std::string const& test_name, std::size_t n, std::size_t iterations)
{
    std::vector<T> nums(n);
    for (auto &i : nums) i = rand() % 1024;

    hpx::util::perf_test_report("foreach datapar", test_name.c_str(), [&](){
        hpx::for_each(hpx::execution::seq, nums.begin(), nums.end(), [](auto &i){
            i *= 64;
            i += 25;
            i -= 26;
            i *= i;
        });
    });
}


int hpx_main(variables_map& vm)
{
    {
        bool test_all = (vm.count("test-all") > 0);
        const int repetitions = vm["repetitions"].as<int>();

        if (test_all)
        {
            
        }
    }

    return hpx::finalize();
}

///////////////////////////////////////////////////////////////////////////////
int main(int argc, char* argv[])
{
    options_description cmdline("usage: " HPX_APPLICATION_STRING " [options]");

    cmdline.add_options()("elements",
        value<std::uint64_t>()->default_value(100000),
        "number of elements in std::vector")

        ("delay-iterations", value<std::uint64_t>()->default_value(0),
         "number of iterations in the delay loop")

        ("test-all", "run all benchmarks")
        ("repetitions", value<int>()->default_value(1),
         "number of repetitions of the full benchmark")

        ("info", value<std::string>()->default_value("no-info"),
         "extra info for plot output (e.g. branch name)");

    hpx::init_params init_args;
    init_args.desc_cmdline = cmdline;

    return hpx::init(argc, argv, init_args);
}