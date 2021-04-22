#pragma once 

#define __CL_ENABLE_EXCEPTIONS
#include "CL/cl.hpp"

#include <stdexcept>
#include <iostream>
#include <fstream>

namespace clf {

using machine_t = std::pair<cl::Platform, cl::Device>;
const char* cl_get_error_string(int error_code);
std::vector<machine_t> get_devices();
cl::Device choose_default_device(const std::vector<machine_t>& data);
cl::Device choose_default_device();

class cl_iclass_t {
public:

    cl_iclass_t();
    cl_iclass_t(cl::Device device);

    virtual ~cl_iclass_t() = 0;

protected:

    static cl::Device device_;
    static cl::Context context_;
    static cl::CommandQueue queue_;
    static cl::Program program_;

private:

    static cl::Program build_program(cl::Context context, cl::Device device);
    static void init(cl::Device device);
    static bool init_;
};

} /* namespace clf */