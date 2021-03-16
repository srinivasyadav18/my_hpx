cmake_minimum_required(VERSION 3.17)
project(examples VERSION 1.0.0)

find_package(Vc REQUIRED)
find_package(OpenMP REQUIRED)
find_package(HPX REQUIRED)

message("VC FOUND : " ${Vc_FOUND})
message("Vc inc dir : " ${Vc_INCLUDE_DIR})
message("Vc Flags : " ${Vc_ALL_FLAGS})
message ("HPX DIR : " ${HPX_DIR})

include_directories(${Vc_INCLUDE_DIR})
set (CMAKE_CXX_STANDARD 17)

add_compile_options(${Vc_ALL_FLAGS} ${OpenMP_CXX_FLAGS})

add_hpx_executable(exe
    ESSENTIAL
    SOURCES test_vc.cpp
    COMPONENT_DEPENDENCIES iostreams
)
target_link_libraries(exe PUBLIC HPX::hpx HPX::iostreams_component Vc::Vc "-ltbb")