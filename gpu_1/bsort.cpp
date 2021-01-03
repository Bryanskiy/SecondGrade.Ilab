#include "bsort.h"

namespace bsort {

void bsort_t::run(int* data, std::size_t elements_count, direction_t direction) {

    /* some opencl setup */
    chose_device();
    context_ = cl::Context(device_);
    build_program();
    queue_ = cl::CommandQueue(context_, device_);

    sort(data, elements_count, direction);
}

void bsort_t::chose_device() {
    std::vector<cl::Platform> platforms;
    cl::Platform::get(&platforms);

    for(const auto& platform : platforms) {
        std::vector<cl::Device> platform_devices;
        platform.getDevices(CL_DEVICE_TYPE_ALL, &platform_devices);

        for(const auto& device : platform_devices) {
            if(device.getInfo<CL_DEVICE_COMPILER_AVAILABLE>()) {
                device_ = device;
                return;
            }
        }
    }

    throw std::runtime_error("Can't chose device");
}

void bsort_t::build_program() {
    std::ifstream program_sources("bsort.cl");
    std::string program_string(std::istreambuf_iterator<char>(program_sources), (std::istreambuf_iterator<char>()));
    cl::Program::Sources source(1, std::make_pair(program_string.c_str(), program_string.length() + 1));
    program_ = cl::Program(context_, source);

    try {
        program_.build();
    } catch (cl::Error e) {
        std::cerr << program_.getBuildInfo<CL_PROGRAM_BUILD_LOG>(device_);
        throw e;
    }    
}

void bsort_t::sort(int* data, std::size_t num_elements, direction_t direction) {
    std::size_t power_two_greater = 1;

    while(power_two_greater <= num_elements) {
        power_two_greater *= 2;
    }

    if(num_elements == power_two_greater) {
        sort_power_two(data, power_two_greater, direction);
        return;
    }

    int* tmp = new int[power_two_greater];

    for(std::size_t i = 0; i < num_elements; ++i) {
        tmp[i] = data[i];
    }

    for(std::size_t i = num_elements; i < power_two_greater; ++i) {
        tmp[i] = std::numeric_limits<int>::max();
    }

    sort_power_two(tmp, power_two_greater, direction);

    for(std::size_t i = 0; i < num_elements; ++i) {
        data[i] = tmp[i];
    }

    delete[] tmp;
}

void bsort_t::sort_power_two(int* data, std::size_t num_elements, direction_t direction) {
    std::size_t num_stages = 0;

    std::size_t tmp = 1;
    while (tmp < num_elements) {
        ++num_stages;
        tmp *= 2;
    }

    cl::Buffer buffer(context_, CL_MEM_READ_WRITE | CL_MEM_USE_HOST_PTR, num_elements, data);

    for(std::size_t stage = 0; stage < num_stages; ++stage) {
        for(std::size_t stage_pass = 0; stage_pass < stage + 1; ++stage_pass) {
            cl::Kernel kernel(program_, "bitonic_sort");
            kernel.setArg(0, buffer);
            kernel.setArg(1, (int)stage);
            kernel.setArg(2, (int)stage_pass);
            kernel.setArg(3, (int)direction);
            

            cl::NDRange offset(0);
            cl::NDRange global_size(num_elements / 2);
            cl::NDRange local_size(1);

            cl::Event event;
            queue_.enqueueNDRangeKernel(kernel, offset, global_size, local_size, NULL, &event);

            event.wait();
        }
    }
}

}