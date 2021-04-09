cmake_minimum_required(VERSION 3.10)
project(matrix VERSION 1.0.0)

add_library(
    matrix
    matrix_buffer.hpp
    matrix.hpp
    matrix_chain.cpp
    matrix_chain.hpp
)