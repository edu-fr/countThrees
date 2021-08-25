cmake_minimum_required(VERSION 3.20)
project(contaTres C)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -lpthread -o3")

add_executable(count_threes_parallel_global_counter count_threes_parallel_global_counter.c)
add_executable(count_threes_sequential count_threes_sequential.c)
add_executable(count_threes_parallel count_threes_parallel.c)
