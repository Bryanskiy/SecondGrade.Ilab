cmake_minimum_required(VERSION 3.10)
project(cpu_matching VERSION 1.0.0)

add_library(
    cpu_matching
    cpu_pm.hpp
    cpu_pm.cpp
)