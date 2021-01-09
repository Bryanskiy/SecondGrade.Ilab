#pragma once

#include <iostream>
#include <fstream>
#include <limits>
#include <chrono>
#include <algorithm>
#include <cmath>

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
    void run(std::vector<int>& data, direction_t direction);

    std::size_t get_full_time() const;
    std::size_t get_gpu_time() const;
    std::size_t get_power_2_time() const;

private:

    cl::Device         device_;
    cl::Context        context_;
    cl::Program        program_;
    cl::CommandQueue   queue_;
    std::size_t        full_time_ = 0;
    std::size_t        gpu_time_ = 0;
    std::size_t        power_2_time_ = 0; 

    /* chose first device with avaible compiler */
    void choose_device();

    /* compile kernels */
    void build_program();

    /* sort elements */
    void sort(std::vector<int>& data, direction_t direction);

    /* sort power of 2 elements using bitonic sort */
    void sort_power_two(std::vector<int>& data, direction_t direction);

    std::size_t choose_work_group_size(std::size_t elements_number) const;

    void enqueue_kernel(cl::Kernel& kernel, cl::NDRange& offset, cl::NDRange& global_size, cl::NDRange& local_size);
};

}

class Timer_t {
public:
    using clock_t = std::chrono::high_resolution_clock;
    using microseconds_t = std::chrono::microseconds;

    Timer_t() : start_(clock_t::now()) {}
    microseconds_t get_time() {
        return std::chrono::duration_cast<microseconds_t>(clock_t::now() - start_);
    }
    void reset() {
        start_ = clock_t::now();
    }
private:
    std::chrono::time_point<clock_t> start_;
};

const char* cl_get_error_string(int error_code);