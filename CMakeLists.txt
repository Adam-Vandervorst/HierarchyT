cmake_minimum_required(VERSION 3.17)
project(HierarchyT)

set(CMAKE_CXX_STANDARD 20) # should get away with 17
set(GCC_COVERAGE_COMPILE_FLAGS "-O3") # ~10x speedup
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${GCC_COVERAGE_COMPILE_FLAGS}" )
add_executable(HierarchyT main.cpp src/Layer.h tests/grow.h tests/shrink.h tests/morph.h tests/compute.h tests/test_graphs.h tests/compute.cpp test_all.h tests/layer.h src/Layer.cpp tests/grow.cpp)