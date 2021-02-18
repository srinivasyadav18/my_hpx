cmake_minimum_required(VERSION 3.12)
project(examples VERSION 1.0.0)

#Find HPX, set HPX_DIR env variable or Pass -DHPX_DIR during cmake
find_package(HPX)

#list of programs or targets to built
set(programs 1d_stencil 1d_stencil_omp)

#Iterate through each program and add dependencies
foreach (program ${programs})
    add_hpx_executable(${program}
        ESSENTIAL
        SOURCES ${program}.cpp
        COMPONENT_DEPENDENCIES iostreams
    )
    target_link_libraries(${program} PUBLIC HPX::hpx)
    message(built " " ${program}.cpp)
endforeach(program)

#Add open-mp flags to omp examples
set_target_properties(
    1d_stencil_omp PROPERTIES COMPILE_FLAGS "${OpenMP_CXX_FLAGS}"
)
