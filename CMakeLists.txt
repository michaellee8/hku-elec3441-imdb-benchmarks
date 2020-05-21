cmake_minimum_required(VERSION 3.16)
project(hku_elec3441_imdb_benchmarks C CXX)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

find_package(libmongoc-1.0 1.7 REQUIRED)
find_package(libbson-1.0 1.7 REQUIRED)
find_package(mongocxx 3.0 REQUIRED)

add_executable(hku_elec3441_imdb_benchmarks main.c redis/benchmark.c memcached/benchmark.c mongodb/benchmark.c mongodb/benchmark.cpp)

add_executable(ignite_benchmark ignite/benchmark.cpp)