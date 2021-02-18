cmake_minimum_required(VERSION 3.12)
project(hello VERSION 1.0.0)

find_package(HPX)

set(programs 1d_stencil 1d_stencil_omp)

foreach (program ${programs})
    add_hpx_executable(${program}
        ESSENTIAL
        SOURCES ${program}.cpp
        COMPONENT_DEPENDENCIES iostreams
    )
    target_link_libraries(${program} PUBLIC HPX::hpx)
    message(built " " ${program}.cpp)
endforeach(program)

set_target_properties(
    1d_stencil_omp PROPERTIES COMPILE_FLAGS "${OpenMP_CXX_FLAGS}"
)
