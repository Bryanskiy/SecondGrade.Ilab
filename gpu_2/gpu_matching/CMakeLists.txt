cmake_minimum_required(VERSION 3.10)
project(gpu_matching VERSION 1.0.0)

add_library(
    gpu_matching

    cl_general.cpp
    cl_general.hpp
    
    filter.hpp
    filter.cpp
)