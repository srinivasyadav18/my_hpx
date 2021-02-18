#include <hpx/hpx_init.hpp>
#include <hpx/iostream.hpp>
#include <hpx/hpx.hpp>
#include <vector>
#include <cstddef>
#include <cstdint>

double k = 0.5;
double dt = 1.;
double dx = 1.;

struct stepper{
    typedef double partition; // floating pt numbers
    typedef std::vector<partition> space; // arr of partitions is space

    // heat eqn
    static double heat(double left, double middle, double right){
        return middle + (k*dt / (dx*dx)) * (left - 2*middle + right);
    }

    space do_work(std::size_t nx, std::size_t nt){
        std::vector<space> U(2);
        for (space& s : U)
            s.resize(nx);

        for (std::size_t i = 0; i != nx; ++i)
            U[0][i] = double(i);

        for (std::size_t t = 0; t != nt; ++t){
            space const& curr = U[t % 2];
            space& next = U[(t+1) % 2];

            next[0] = heat(curr[nx-1], curr[0], curr[1]);

            for (std::size_t i = 1; i != nx-1; ++i){
                next[i] = heat(curr[i-1], curr[i], curr[i+1]);
            }

            next[nx-1] = heat(curr[nx-2], curr[nx-1], curr[0]);
        }

        return U[nt % 2];
    }
};

int hpx_main(int, char**)
{
    hpx::cout << "1d stencil plain serial code\n" << hpx::flush;

    stepper step;

    std::uint64_t t = hpx::chrono::high_resolution_clock::now();

    stepper::space solution = step.do_work(100000, 45000);
    
    std::uint64_t elapsed = hpx::chrono::high_resolution_clock::now() - t;

    hpx::cout << "Elapsed : " << elapsed << "\n" << hpx::flush;
    return hpx::finalize();
}

int main(int argc, char* argv[])
{
    return hpx::init(argc, argv);
}