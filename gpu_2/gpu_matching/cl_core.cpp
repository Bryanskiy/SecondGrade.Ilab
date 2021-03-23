#include "cl_core.hpp"

namespace pm {

/* static */ std::vector<std::pair<cl::Platform, cl::Device>> clcore_t::get_devices() {
    std::vector<cl::Platform> platforms;
    cl::Platform::get(&platforms);

    std::vector<std::pair<cl::Platform, cl::Device>> ret;

    for(auto&& platform : platforms) {
        std::vector<cl::Device> platform_devices;
        platform.getDevices(CL_DEVICE_TYPE_ALL, &platform_devices);
        
        for(auto&& device : platform_devices) {
            if( device.getInfo<CL_DEVICE_AVAILABLE>() &&
                device.getInfo<CL_DEVICE_COMPILER_AVAILABLE>()) 
            {
                ret.push_back({platform, device});
            }
        }
    }
    return ret;
}

} /* namespace pm */