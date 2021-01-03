#pragma once

#include <iostream>
#include <fstream>
#include <limits>

#define __CL_ENABLE_EXCEPTIONS
#include <CL/cl.hpp>

namespace bsort {

enum class direction_t {
    descending = 0,
    increasing = 1,
};

class bsort_t {
public:

    /* sort integer array in specified direction */
    void run(int* data, std::size_t elements_count, direction_t direction);

private:

    cl::Device         device_;
    cl::Context        context_;
    cl::Program        program_;
    cl::CommandQueue   queue_;

    /* chose first device with avaible compiler */
    void chose_device();

    /* compile kernels */
    void build_program();

    /* sort elements */
    void sort(int* data, std::size_t elements_count, direction_t direction);

    /* sort power of 2 elements using bitonic sort */
    void sort_power_two(int* data, std::size_t elements_count, direction_t direction);
};

}