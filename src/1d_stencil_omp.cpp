#include <hpx/hpx_init.hpp>
#include <hpx/iostream.hpp>
#include <hpx/hpx.hpp>

#include <chrono>
#include <vector>
#include <cstddef>
#include <cstdint>
#include <cstdlib>

inline std::uint64_t now()
{
    std::chrono::nanoseconds ns = 
        std::chrono::steady_clock::now().time_since_epoch();
    
    return static_cast<std::uint64_t> (ns.count());
}

double k = 0.5;
double dt = 1.0;
double dx = 1.;

struct stepper
{
    typedef double partition;

    typedef std::vector<partition> space;

    static double heat(double left, double middle, double right)
    {
        return middle + (k * dt/(dx*dx)) * (left - 2*middle + right);
    }

    space do_work(std::size_t nx, std::size_t nt)
    {
        std::vector<space> U(2);
        for (space &s : U)
            s.resize(nx);

        for (std::size_t i = 0; i != nx; ++i)
            U[0][i] = double(i);

        for (std::size_t t = 0; t != nt; ++t)
        {
            const space &current = U[t % 2];
            space &next = U[(t + 1) % 2];

            next[0] = heat(current[nx-1], current[0], current[1]);

            #pragma omp parallel for
            for (std::int64_t i = 1; i < std::int64_t(nx-1); ++i){
                next[i] = heat(current[i-1], current[i], current[i+1]);
            }

            next[nx-1] = heat(current[nx-1], current[nx-1], current[0]); 
        }

        return U[nt % 2];
    }
};

int hpx_main(int, char**)
{
    hpx::cout << "1d stencil omp parallel code\n" << hpx::flush;

    stepper step;

    std::uint64_t t = now();

    stepper::space solution = step.do_work(100000, 45000);
    
    std::uint64_t elapsed = now() - t;

    hpx::cout << "Elapsed : " << elapsed << "\n" << hpx::flush;
    return hpx::finalize();
}

int main(int argc, char* argv[])
{
    return hpx::init(argc, argv);
}