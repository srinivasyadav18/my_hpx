cmake_minimum_required(VERSION 3.17)
project(examples VERSION 1.0.0)

# set (HPX "/root/kmit/srinivas/install/hpx/")
find_package(HPX REQUIRED)

message ("HPX DIR : " ${HPX_DIR})

add_compile_options(${OpenMP_CXX_FLAGS})
# "-mavx512f" "-fno-tree-vectorize"
#"-fno-tree-vectorize"

add_hpx_executable(exe
    ESSENTIAL
    SOURCES main.cpp
    COMPONENT_DEPENDENCIES iostreams
)

target_compile_options(exe PUBLIC "-march=native" "-g" "-fno-tree-vectorize")
target_link_libraries(exe PUBLIC HPX::hpx HPX::iostreams_component "-ltbb")