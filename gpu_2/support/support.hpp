#pragma once 

#include <vector>
#include <CL/cl.hpp>
#include <iostream>
#include <string>

namespace sup {

const char* cl_get_error_string(int error_code);
std::vector<std::pair<cl::Platform, cl::Device>> get_devices();
std::string read_str(std::istream& input);
cl::Device choose_default_device(const std::vector<std::pair<cl::Platform, cl::Device>>& data);

} /* namespace sup */