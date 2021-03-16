#include <hpx/hpx.hpp>
#include <hpx/hpx_init.hpp>

#include <iostream>
#include <vector>

static constexpr hpx::execution::dataseq_policy dataseq;
int hpx_main()
{
    std::cout << "HPX VC test\n";

    std::vector<int> nums(1<<30, 1);
    for (size_t i = 0; i < nums.size(); i++)
        nums[i] = i;

    // Sequential
    std::uint64_t t1 = hpx::chrono::high_resolution_clock::now();
    hpx::for_each(nums.begin(), nums.end(), [](auto &i){
        i += 5;
    });
    std::uint64_t elapsed1 = hpx::chrono::high_resolution_clock::now() - t1;
    
    // datapar_seq (only vectorisation)
    std::uint64_t t2 = hpx::chrono::high_resolution_clock::now();
    hpx::for_each(dataseq, nums.begin(), nums.end(), [](auto &i){
        i += 5;
    });
    std::uint64_t elapsed2 = hpx::chrono::high_resolution_clock::now() - t2;

    // OpemMP SIMD
    std::uint64_t t3 = hpx::chrono::high_resolution_clock::now();
    #pragma omp simd
    for (int i = 0; i < nums.size(); i++)
    {
        nums[i] += 5;
    }
    std::uint64_t elapsed3 = hpx::chrono::high_resolution_clock::now() - t3;

    std::cout << "Elapsed 1 (sequential) : " << elapsed1 / 1e9 << std::endl;
    std::cout << "Elapsed 2 (datapar_seq) : " << elapsed2 / 1e9 << std::endl;
    std::cout << "Elapsed 3 (openMP SIMD): " << elapsed3 / 1e9 << std::endl;

    return hpx::finalize();
}

int main(int argc, char* argv[])
{
    return hpx::init(argc, argv);
}

/*
OUTPUT:
------
HPX VC test
tcmalloc: large alloc 4294967296 bytes == 0x55c99b6d4000 @ 
Elapsed 1 (sequential) : 16.5986
Elapsed 2 (datapar_seq) : 12.812
Elapsed 3 (openMP SIMD): 3.53648
*/