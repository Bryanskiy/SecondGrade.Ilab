#pragma once

#include <CL/cl.hpp>
#include <boost/program_options.hpp>
#include <iostream>

namespace pm {

class clcore_t final {
public:
    static std::vector<std::pair<cl::Platform, cl::Device>> get_devices();

    clcore_t(const cl::Device& device) : device_(device) {}
private:
    cl::Device device_;
};

} /* namepsace pm */  