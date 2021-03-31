#pragma once

#include <vector>
#include <string>
#include <fstream>
#include <iostream>


#define __CL_ENABLE_EXCEPTIONS
#include "CL/cl.hpp"
#include "../support/timer.hpp"

namespace pm {

class gpu_kmp_t {
public:
    gpu_kmp_t() = default;
    gpu_kmp_t(const cl::Device& device , const std::string& text, const std::vector<std::string>& patterns) : 
              device_(device),  context_(cl::Context(device_)),
              queue_(cl::CommandQueue(context_, device_, CL_QUEUE_PROFILING_ENABLE)), 
              text_(text), patterns_(patterns) {}

    std::vector<std::size_t> match();

    std::size_t time() const {return time_;}
    std::size_t gpu_only_time() const {return gpu_only_time_;}

private:
    void build_program(const std::vector<std::string>& kernels);

private:
    cl::Device device_;
    cl::Context context_;
    cl::CommandQueue queue_;
    cl::Program program_;

    std::string text_;
    std::vector<std::string> patterns_;

    std::size_t time_;
    std::size_t gpu_only_time_ = 0;
    bool builded_ = false;
};

std::vector<std::size_t> preffix_function(const std::string& str);

} /* namespace pm */