#pragma once

#define __CL_ENABLE_EXCEPTIONS
#include "CL/cl.hpp"

#include <iostream>
#include <fstream>

namespace pm {

using machine_t = std::pair<cl::Platform, cl::Device>;

const char* cl_get_error_string(int error_code);
std::vector<machine_t> get_devices();
cl::Device choose_default_device(const std::vector<machine_t>& data);
cl::Device choose_default_device();

class cl_iclass_t {
public:
    cl_iclass_t();
    cl_iclass_t(cl::Device device);

    void build_program(const std::vector<std::string>& kernels);

    virtual ~cl_iclass_t() = 0;

protected:
    cl::Device device_;
    cl::Context context_;
    cl::CommandQueue queue_;
    cl::Program program_;
};

} /* namespace pm */