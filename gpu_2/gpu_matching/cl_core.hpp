#pragma once

#define __CL_ENABLE_EXCEPTIONS
#include <CL/cl.hpp>
#include <boost/program_options.hpp>
#include <iostream>
#include <fstream>

namespace pm {

class clcore_t final {
public:
    static std::vector<std::pair<cl::Platform, cl::Device>> get_devices();

    clcore_t(const cl::Device& device);

    void build_program(const std::vector<std::string>& kernels);

    cl::Device get_device() const {return device_;}
    cl::Context get_context() const {return context_;}
    cl::CommandQueue get_queue() const {return queue_;}
    cl::Program get_program() const {return program_;}
    
private:
    cl::Device device_;
    cl::Context context_;
    cl::CommandQueue queue_;
    cl::Program program_;
};

} /* namepsace pm */  

const char* cl_get_error_string(int error_code);