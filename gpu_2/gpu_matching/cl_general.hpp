#pragma once

#define __CL_ENABLE_EXCEPTIONS
#include "CL/cl.hpp"

#include <iostream>
#include <fstream>

namespace pm {

const char* cl_get_error_string(int error_code);
std::vector<std::pair<cl::Platform, cl::Device>> get_devices();
std::string read_str(std::istream& input);
cl::Device choose_default_device(const std::vector<std::pair<cl::Platform, cl::Device>>& data);
cl::Device choose_default_device();
void dump_devices(const std::vector<std::pair<cl::Platform, cl::Device>>& info, std::ostream& stream);

class cl_iclass_t {
public:
    cl_iclass_t() : device_(choose_default_device()),  context_(cl::Context(device_)),
        queue_(cl::CommandQueue(context_, device_, CL_QUEUE_PROFILING_ENABLE))
        {}

    cl_iclass_t(cl::Device device) :
        device_(device),  context_(cl::Context(device_)),
        queue_(cl::CommandQueue(context_, device_, CL_QUEUE_PROFILING_ENABLE))
        {}

    void build_program(const std::vector<std::string>& kernels);    

protected:
    cl::Device device_;
    cl::Context context_;
    cl::CommandQueue queue_;
    cl::Program program_;
};

} /* namespace pm */