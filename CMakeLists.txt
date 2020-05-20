cmake_minimum_required(VERSION 3.16)
project(hku_elec3441_imdb_benchmarks C CXX)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

add_executable(hku_elec3441_imdb_benchmarks main.c redis/benchmark.c memcached/benchmark.c)

add_executable(ignite_benchmark ignite/benchmark.cpp)