#pragma once 

#include <vector>
#include <CL/cl.hpp>

namespace clsup {

const char* cl_get_error_string(int error_code);
std::vector<std::pair<cl::Platform, cl::Device>> get_devices();

} /* namespace clsup */