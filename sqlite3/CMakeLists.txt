cmake_minimum_required(VERSION 3.0)
project(hku_elec3441_imdb_benchmarks C CXX)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)


add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/SQLiteCpp)

include_directories(
        ${CMAKE_CURRENT_LIST_DIR}/SQLiteCpp/include
)

add_executable(
        benchmark
        benchmark.cpp
)

target_link_libraries(benchmark
        SQLiteCpp
        sqlite3
        pthread
        dl
        )