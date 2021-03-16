cmake_minimum_required(VERSION 3.17)
project(examples VERSION 1.0.0)

#Find HPX, set HPX_DIR env variable or Pass -DHPX_DIR during cmake
find_package(HPX REQUIRED)
find_package(OpenMP REQUIRED)

#list of programs or targets to built
set(programs 
    test_vc
    )

#Iterate through each program and add dependencies
foreach (program ${programs})
    add_hpx_executable(${program}
        ESSENTIAL
        SOURCES ${program}.cpp
        COMPONENT_DEPENDENCIES iostreams
    )
    target_link_libraries(${program} PUBLIC HPX::hpx HPX::iostreams_component)
    message(built " " ${program}.cpp)
endforeach(program)

# Add open-mp flags to omp examples
set_target_properties(
    test_vc PROPERTIES COMPILE_FLAGS "${OpenMP_CXX_FLAGS}"
)

set_target_properties(
    test_vc PROPERTIES LINK_FLAGS "${OpenMP_CXX_FLAGS}"
)