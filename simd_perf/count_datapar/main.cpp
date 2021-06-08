#include <hpx/hpx_init.hpp>
#include <hpx/hpx.hpp>
#include <hpx/include/datapar.hpp>
#include <hpx/parallel/util/loop.hpp>
#include <bits/stdc++.h>

template <typename Iter>
bool is_aligned(Iter const& it)
{
    typedef typename std::iterator_traits<Iter>::value_type value_type;
    return (reinterpret_cast<std::uintptr_t>(std::addressof(*it)) &
                (std::experimental::memory_alignment_v<std::experimental::native_simd<value_type>> - 1)) == 0;
}

template <typename Iter, typename T>
std::size_t count_(Iter first, Iter last, T const val)
{
    std::size_t ret = 0;

    while (!is_aligned(first) && first != last)
    {
        if (*first++ == val)
            ++ret;
    }
    
    typedef typename std::iterator_traits<Iter>::value_type value_type;
    std::experimental::native_simd<value_type> tmp;
    std::size_t size = tmp.size();
    Iter const lastV = last - (size + 1);

    while (first < lastV)
    {
        tmp.copy_from(std::addressof(*first), std::experimental::vector_aligned);
        ret += std::experimental::popcount(tmp == val);
        std::advance(first, size);
    }

    while (first != last)
    {
        if (*first++ == val)
            ++ret;
    }
    return ret;
}

std::size_t v1, v2;

template <typename T>
auto test(hpx::execution::sequenced_policy, std::vector<T> &nums)
{
    auto t1 = std::chrono::high_resolution_clock::now();
        v1 = hpx::count(nums.begin(), nums.end(), static_cast<T> (42.0));
    auto t2 = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> diff = t2 - t1;
    return diff.count();
}

template <typename T>
auto test(hpx::execution::simd_policy, std::vector<T> &nums)
{
    auto t1 = std::chrono::high_resolution_clock::now();
        v2 = count_(nums.begin(), nums.end(), static_cast<T> (42.0));
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

    if (v1 != v2) std::cout << "\nFailed\n";

    std::string result = std::string(test_name);
    std::string n_str = std::to_string(n);
    std::string iterations_str = std::to_string(iterations);
    std::string avg_time_str = std::to_string(avg_time);

    result.resize(30, ' ');
    n_str.resize(20, ' ');
    iterations_str.resize(20, ' ');
    avg_time_str.resize(20, ' ');
    result += n_str + iterations_str + avg_time_str;
    std::cout << result << std::endl;
}

int hpx_main()
{
    std::string header;
    std::string t_name = "Test Name";t_name.resize(30, ' ');
    std::string arr_size = "Elements"; arr_size.resize(20, ' ');
    std::string iters = "Iterations"; iters.resize(20, ' ');
    std::string spd_up = "Avg speed up";

    header += t_name + arr_size + iters + spd_up;
    std::cout << header << "\n";
    for (int j = 100; j <= 100; j += 50)
    {
        for (int i = 5; i <= 25; i += 5)
        {
            test<int>("hpx count datapar<int>", 1 << i, j);
        }
        std::cout << "\n";
        for (int i = 5; i <= 25; i += 5)
        {
            test<float>("hpx count datapar<float>", 1 << i, j);
        }
        std::cout << "\n";
        for (int i = 5; i <= 25; i += 5)
        {
            test<double>("hpx count datapar<double>", 1 << i, j);
        }
        std::cout << "\n";
    }
    return hpx::finalize();
}

int main(int argc, char *argv[]) {
  return hpx::init(argc, argv);
}