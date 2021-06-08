#include <hpx/hpx_init.hpp>
#include <hpx/hpx.hpp>
#include <hpx/include/datapar.hpp>
#include <hpx/parallel/util/loop.hpp>
#include <bits/stdc++.h>


template <typename ExPolicy, typename T>
auto test(ExPolicy&& policy, std::vector<T> &nums)
{
    auto t1 = std::chrono::high_resolution_clock::now();
        hpx::for_each(policy, nums.begin(), nums.end(), [](auto &i){
            i *= 64;
            i += 25;
            i -= 26;
            i *= i;
        });
    auto t2 = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> diff = t2 - t1;
    return diff.count();
}

template <typename T>
void test(std::string const& test_name, std::size_t n, std::size_t iterations)
{
    double avg_time = 0.0;
    std::vector<T> nums(n);
    for (auto &i : nums) i = rand() % 1024;

    for (std::size_t i = 0; i != iterations; ++i)
    {
        auto t1 = test(hpx::execution::seq, nums);
        auto t2 = test(hpx::execution::simd, nums);
        avg_time += (t1/t2);
    }
    avg_time /= (double)iterations;

    std::string result = std::string(test_name);
    std::string n_str = std::to_string(n);
    std::string iterations_str = std::to_string(iterations);
    std::string simd_len_str = std::to_string(std::experimental::native_simd<T>::size());
    std::string avg_time_str = std::to_string(avg_time);

    result.resize(30, ' ');
    n_str.resize(20, ' ');
    iterations_str.resize(20, ' ');
    simd_len_str.resize(20, ' ');
    avg_time_str.resize(20, ' ');
    result += n_str + iterations_str + simd_len_str + avg_time_str;
    std::cout << result << std::endl;
}

int hpx_main()
{
    std::string header;
    std::string t_name = "Test Name";t_name.resize(30, ' ');
    std::string arr_size = "Elements"; arr_size.resize(20, ' ');
    std::string iters = "Iterations"; iters.resize(20, ' ');
    std::string simd_len = "Simd Len"; simd_len.resize(20, ' ');
    std::string spd_up = "Avg speed up";

    header += t_name + arr_size + iters + simd_len + spd_up;
    std::cout << header << "\n";
    for (int j = 100; j <= 100; j += 50)
    {
        for (int i = 5; i <= 25; i += 5)
        {
            test<int>("hpx for_each datapar<int>", 1 << i, j);
        }
        std::cout << "\n";
        for (int i = 5; i <= 25; i += 5)
        {
            test<float>("hpx for_each datapar<float>", 1 << i, j);
        }
        std::cout << "\n";
        for (int i = 5; i <= 25; i += 5)
        {
            test<double>("hpx for_each datapar<double>", 1 << i, j);
        }
        std::cout << "\n";
    }
    return hpx::finalize();
}

int main(int argc, char *argv[]) {
  return hpx::init(argc, argv);
}