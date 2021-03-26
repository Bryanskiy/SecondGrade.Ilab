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
    void enqueue_kernel(cl::Kernel& kernel, cl::NDRange& offset, cl::NDRange& global_size, cl::NDRange& local_size);

    cl::Device get_device() const {return device_;}
    cl::Context get_context() const {return context_;}
    cl::CommandQueue get_queue() const {return queue_;}
    cl::Program get_program() const {return program_;}
    std::size_t get_time() const {return time_;}

private:
    cl::Device device_;
    cl::Context context_;
    cl::CommandQueue queue_;
    cl::Program program_;
    std::size_t time_ = 0;
};

} /* namepsace pm */  

const char* cl_get_error_string(int error_code);