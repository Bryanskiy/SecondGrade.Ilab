#pragma once 

#define __CL_ENABLE_EXCEPTIONS
#include "CL/cl.hpp"

#include "../support/support.hpp"

namespace gpula {

class cl_class_t {
public:
    cl_class_t() : device_(sup::choose_default_device()),  context_(cl::Context(device_)),
        queue_(cl::CommandQueue(context_, device_, CL_QUEUE_PROFILING_ENABLE)) 
        {}

    cl_class_t(cl::Device device) : 
        device_(device),  context_(cl::Context(device_)),
        queue_(cl::CommandQueue(context_, device_, CL_QUEUE_PROFILING_ENABLE)) 
        {}

protected:
    cl::Device device_;
    cl::Context context_;
    cl::CommandQueue queue_;
    cl::Program program_;
};

class cl_float_t : public cl_class_t {
public:

private:

};

const cl_float_t operator+(cl_float_t lhs, cl_float_t rhs);

} /* namespace gpula */