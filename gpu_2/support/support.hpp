#pragma once 

#include <vector>
#include <CL/cl.hpp>
#include <iostream>
#include <fstream>
#include <string>

namespace sup {

const char* cl_get_error_string(int error_code);
std::vector<std::pair<cl::Platform, cl::Device>> get_devices();
std::string read_str(std::istream& input);
cl::Device choose_default_device(const std::vector<std::pair<cl::Platform, cl::Device>>& data);
void dump_devices(const std::vector<std::pair<cl::Platform, cl::Device>>& info, std::ostream& stream);

#ifdef LOG
struct log_t {
public:
    log_t(const std::string& name) : log_file(name) {}
    log_t(const char* name) : log_file(name) {}

    void separate() {log_file << separator << std::endl;}

    std::ofstream log_file;
    const std::string separator = "--------------------------------------------------------------";
    std::string indent = "";
};

extern log_t log;
#endif

} /* namespace sup */