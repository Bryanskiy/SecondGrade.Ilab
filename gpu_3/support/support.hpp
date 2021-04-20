#pragma once 

#include <vector>
#define __CL_ENABLE_EXCEPTIONS
#include <CL/cl.hpp>
#include <iostream>
#include <string>

namespace sup {

const char* cl_get_error_string(int error_code);
std::vector<std::pair<cl::Platform, cl::Device>> get_devices();
cl::Device choose_default_device(const std::vector<std::pair<cl::Platform, cl::Device>>& data);
cl::Device choose_default_device();

} /* namespace sup */
