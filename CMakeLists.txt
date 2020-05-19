cmake_minimum_required(VERSION 3.16)
project(hku_elec3441_imdb_benchmarks C)

set(CMAKE_C_STANDARD 11)

add_executable(hku_elec3441_imdb_benchmarks main.c redis/benchmark.c memcached/benchmark.c)