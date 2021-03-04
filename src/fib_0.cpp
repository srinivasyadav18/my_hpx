#include <hpx/hpx_init.hpp>
#include <hpx/include/async.hpp>
#include <hpx/iostream.hpp>

std::uint64_t fib(std::uint64_t n) {
  if (n < 2)
    return n;

  hpx::future<std::uint64_t> n1 = hpx::async(fib, n - 1);
  hpx::future<std::uint64_t> n2 = hpx::async(fib, n - 2);
  return n1.get() + n2.get();
}

int hpx_main(hpx::program_options::variables_map &vm) {
  hpx::cout << "number of os threads lanuched : " << hpx::get_os_thread_count()
            << '\n' << hpx::flush;
  std::uint64_t n = vm["n-value"].as<std::uint64_t>();
  auto ans = fib(n);
  hpx::cout << "fib " << n << " : " << ans;
  return hpx::finalize();
}

int main(int argc, char *argv[]) {
  hpx::program_options::options_description cmds(
      "Uasge : " HPX_APPLICATION_STRING " [options]");

  cmds.add_options()(
      "n-value",
      hpx::program_options::value<std::uint64_t>()->default_value(10),
      "n value for the Fibonacci function");

  hpx::init_params init_args;
  init_args.desc_cmdline = cmds;
  return hpx::init(argc, argv, init_args);
}