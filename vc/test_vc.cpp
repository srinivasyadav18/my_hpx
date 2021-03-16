#include <Vc/IO>
#include <Vc/Vc>

#include <hpx/hpx.hpp>
#include <hpx/hpx_init.hpp>

#include <omp.h>

#include <algorithm>
#include <chrono>
#include <execution>
#include <iostream>
#include <numeric>
#include <string>
#include <vector>

static constexpr hpx::execution::dataseq_policy dataseq;

bool debug = false;
template <typename T>
void print(const T& v)
{
    if (!debug)
        return;
    for (int i = 0; i < std::min(int(32), int(v.size())); i++)
        std::cout << v[i] << "\t";
    std::cout << "\n";
}

template <typename F>
void util(F& f, const std::string& context)
{
    auto start = std::chrono::high_resolution_clock::now();
    f();
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = end - start;
    auto cc(context);
    cc.resize(25, ' ');
    std::cout << "Time Elapsed [" << cc << "]\t:\t" << elapsed.count() << "s\n";
}

template <typename T, typename F>
void test(const T& nums, F& f, const std::string& context)
{
    print(nums);
    util(f, context);
    print(nums);
    if (debug)
        std::cout << "\n";
}

int hpx_main()
{
    static constexpr size_t sz = 1 << 30;
    std::vector<int> nums(sz);
    std::iota(nums.begin(), nums.end(), 0);

    auto normal_for_loop = [&]() {
        for (int i = 0; i < nums.size(); i++)
        {
            nums[i] += 5;
        }
    };

    auto std_for_each = [&]() {
        std::for_each(nums.begin(), nums.end(), [](int& i) { i += 5; });
    };

    auto vc_vectorised = [&]() {
        Vc::int_v gathered(Vc::IndexesFromZero);
        const int vc_len = (int) gathered.Size;
        if (debug)
            std::cout << "vector register len : " << gathered.Size << '\n';

        for (int i = 0; i < nums.size(); i += vc_len)
        {
            gathered.load(nums.data() + i);
            gathered += 5;
            gathered.store(nums.data() + i);
        }
    };

    auto omp_simd = [&]() {
#pragma omp simd
        for (size_t i = 0; i < nums.size(); i++)
        {
            nums[i] += 5;
        }
    };

    auto hpx_for_each_dataseq = [&]() {
        hpx::for_each(
            dataseq, nums.begin(), nums.end(), [](auto& i) { i += 5; });
    };

    auto hpx_for_each = [&]() {
        hpx::for_each(nums.begin(), nums.end(), [](auto& i) { i += 5; });
    };

    auto std_for_each_unseq = [&]() {
        std::for_each(std::execution::unseq, nums.begin(), nums.end(),
            [](auto& i) { i += 5; });
    };
    test(nums, normal_for_loop, "normal for loop");
    test(nums, vc_vectorised, "vc vectorised for loop");
    test(nums, std_for_each, "std::for_each");
    test(nums, std_for_each_unseq, "std::for_each unseq");
    test(nums, hpx_for_each, "hpx::for_each");
    test(nums, hpx_for_each_dataseq, "hpx::for_each dataseq");
    test(nums, omp_simd, "omp simd for loop");
    return hpx::finalize();
}

int main(int argc, char* argv[])
{
    return hpx::init(argc, argv);
}
