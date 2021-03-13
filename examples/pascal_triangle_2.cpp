#include <hpx/hpx.hpp>
#include <hpx/hpx_init.hpp>

#include <fstream>
#include <iostream>
#include <omp.h>
#include <vector>

struct pascal_triangle
{
    pascal_triangle(std::uint64_t n)
      : n(n)
    {
        pt.resize(n);
        for (auto& row : pt)
            row.resize(n);
    }

    void do_work()
    {
        using hpx::dataflow;

        auto Op = [](int l, int r){return l + r};
        for (std::size_t i = 0; i != n; ++i)
        {
            for (std::size_t j = 0; j <= i; ++j)
            {
                pt[i][j] = dataflow(
                    hpx::launch::async, Op, 
                    pt[i-1][j-1], pt[i-1][j+1]
                )
            }
        }

    }

    void save()
    {
        std::fstream fout;
        fout.open("res.txt", std::fstream::out);
        fout << "Pascal Triangle of size : " << n << '\n';
        for (const auto& row : pt)
        {
            for (const auto& ele : row)
                fout << ele << "\t";
            fout << '\n';
        }
        fout.close();
    }

private:
    size_t n;
    std::vector<std::vector<std::uint64_t>> pt;
};

int hpx_main(hpx::program_options::variables_map& vm)
{
    std::uint64_t n = vm["n"].as<std::uint64_t>();

    pascal_triangle pt(n);
    std::uint64_t t = hpx::chrono::high_resolution_clock::now();

    pt.do_work();

    std::uint64_t elapsed = hpx::chrono::high_resolution_clock::now() - t;

    std::uint64_t const os_thread_count = hpx::get_os_thread_count();
    std::cout << "os_thread_count : " << os_thread_count
              << " elapsed : " << elapsed / 1e9 << " n : " << n << std::endl;

    return hpx::finalize();
}

int main(int argc, char* argv[])
{
    namespace po = hpx::program_options;

    po::options_description desc_commandline;
    desc_commandline.add_options()("n",
        po::value<std::uint64_t>()->default_value(25), "number of rows in pt");

    hpx::init_params init_args;
    init_args.desc_cmdline = desc_commandline;

    return hpx::init(argc, argv, init_args);
}
