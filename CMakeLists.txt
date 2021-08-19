cmake_minimum_required(VERSION 3.20)
project(contaTres C)

set(CMAKE_C_STANDARD 99)

add_executable(count_threes_sequential count_threes_sequential.c)
add_executable(count_threes_parallel count_threes_parallel.c)
