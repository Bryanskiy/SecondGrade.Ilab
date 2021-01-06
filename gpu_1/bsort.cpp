#include "bsort.h"

namespace bsort {

void bsort_t::run(std::vector<int>& data, direction_t direction) {

    /* some opencl setup */
    choose_device();
    context_ = cl::Context(device_);
    build_program();
    queue_ = cl::CommandQueue(context_, device_, CL_QUEUE_PROFILING_ENABLE);

    sort(data, direction);
}

void bsort_t::choose_device() {
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

void bsort_t::sort(std::vector<int>& data, direction_t direction) {

    Timer_t timer;
    std::size_t power_two_greater = 1;
    std::size_t num_elements = data.size();

    while(power_two_greater < num_elements) {
        power_two_greater *= 2;
    }

    if(num_elements == power_two_greater) {
        sort_power_two(data, direction);
        full_time_ = timer.get_time().count();
        return;
    }

    std::vector<int> tmp = data;
    tmp.resize(power_two_greater);

    for(std::size_t i = num_elements; i < power_two_greater; ++i) {
        tmp[i] = std::numeric_limits<int>::max();
    }

    sort_power_two(tmp, direction);

    for(std::size_t i = 0; i < num_elements; ++i) {
        data[i] = tmp[i];
    }

    full_time_ = timer.get_time().count();
}


void bsort_t::sort_power_two(std::vector<int>& data, direction_t direction) {
    Timer_t time;
    std::size_t num_stages = 0;
    std::size_t num_elements = data.size();

    std::size_t tmp = 1;
    while (tmp < num_elements) {
        ++num_stages;
        tmp *= 2;
    }

    cl::Buffer buffer(context_, CL_MEM_READ_WRITE | CL_MEM_USE_HOST_PTR, num_elements, data.data());

    cl::NDRange offset(0);
    cl::NDRange global_size(num_elements / 2);
    cl::NDRange local_size(choose_work_group_size(num_elements));

    for(std::size_t stage = 0; stage < num_stages; ++stage) {
        for(std::size_t stage_pass = 0; stage_pass < stage + 1; ++stage_pass) {
            cl::Kernel kernel(program_, "bitonic_sort");
            kernel.setArg(0, buffer);
            kernel.setArg(1, static_cast<unsigned>(stage));
            kernel.setArg(2, static_cast<unsigned>(stage_pass));
            kernel.setArg(3, static_cast<unsigned>(direction));

            cl::Event event;
            queue_.enqueueNDRangeKernel(kernel, offset, global_size, local_size, NULL, &event);
            
            event.wait();

            std::size_t start = event.getProfilingInfo<CL_PROFILING_COMMAND_START>();
            std::size_t end = event.getProfilingInfo<CL_PROFILING_COMMAND_END>();
            gpu_time_ += (end - start) / 1000;
        }
    }

    power_2_time_ = time.get_time().count();
}

std::size_t bsort_t::get_full_time() const {
    return full_time_;
}

std::size_t bsort_t::get_gpu_time() const {
    return gpu_time_;
}

std::size_t bsort_t::get_power_2_time() const {
    return power_2_time_;
}

std::size_t bsort_t::choose_work_group_size(std::size_t elements_number) const {
    std::size_t max = device_.getInfo<CL_DEVICE_MAX_WORK_GROUP_SIZE>();
    std::size_t ret = 1;
    std::size_t work_items_count = elements_number / 2;
    while((ret <= max) && (ret <= work_items_count)) {
        ret *= 2;
    }

    return ret / 2;
}

}

const char* cl_get_error_string(int error_code) {
    switch (error_code) {
        case 0: return "CL_SUCCESS";
        case -1: return "CL_DEVICE_NOT_FOUND";
        case -2: return "CL_DEVICE_NOT_AVAILABLE";
        case -3: return "CL_COMPILER_NOT_AVAILABLE";
        case -4: return "CL_MEM_OBJECT_ALLOCATION_FAILURE";
        case -5: return "CL_OUT_OF_RESOURCES";
        case -6: return "CL_OUT_OF_HOST_MEMORY";
        case -7: return "CL_PROFILING_INFO_NOT_AVAILABLE";
        case -8: return "CL_MEM_COPY_OVERLAP";
        case -9: return "CL_IMAGE_FORMAT_MISMATCH";
        case -10: return "CL_IMAGE_FORMAT_NOT_SUPPORTED";
        case -12: return "CL_MAP_FAILURE";
        case -13: return "CL_MISALIGNED_SUB_BUFFER_OFFSET";
        case -14: return "CL_EXEC_STATUS_ERROR_FOR_EVENTS_IN_WAIT_LIST";
        case -15: return "CL_COMPILE_PROGRAM_FAILURE";
        case -16: return "CL_LINKER_NOT_AVAILABLE";
        case -17: return "CL_LINK_PROGRAM_FAILURE";
        case -18: return "CL_DEVICE_PARTITION_FAILED";
        case -19: return "CL_KERNEL_ARG_INFO_NOT_AVAILABLE";
        case -30: return "CL_INVALID_VALUE";
        case -31: return "CL_INVALID_DEVICE_TYPE";
        case -32: return "CL_INVALID_PLATFORM";
        case -33: return "CL_INVALID_DEVICE";
        case -34: return "CL_INVALID_CONTEXT";
        case -35: return "CL_INVALID_QUEUE_PROPERTIES";
        case -36: return "CL_INVALID_COMMAND_QUEUE";
        case -37: return "CL_INVALID_HOST_PTR";
        case -38: return "CL_INVALID_MEM_OBJECT";
        case -39: return "CL_INVALID_IMAGE_FORMAT_DESCRIPTOR";
        case -40: return "CL_INVALID_IMAGE_SIZE";
        case -41: return "CL_INVALID_SAMPLER";
        case -42: return "CL_INVALID_BINARY";
        case -43: return "CL_INVALID_BUILD_OPTIONS";
        case -44: return "CL_INVALID_PROGRAM";
        case -45: return "CL_INVALID_PROGRAM_EXECUTABLE";
        case -46: return "CL_INVALID_KERNEL_NAME";
        case -47: return "CL_INVALID_KERNEL_DEFINITION";
        case -48: return "CL_INVALID_KERNEL";
        case -49: return "CL_INVALID_ARG_INDEX";
        case -50: return "CL_INVALID_ARG_VALUE";
        case -51: return "CL_INVALID_ARG_SIZE";
        case -52: return "CL_INVALID_KERNEL_ARGS";
        case -53: return "CL_INVALID_WORK_DIMENSION";
        case -54: return "CL_INVALID_WORK_GROUP_SIZE";
        case -55: return "CL_INVALID_WORK_ITEM_SIZE";
        case -56: return "CL_INVALID_GLOBAL_OFFSET";
        case -57: return "CL_INVALID_EVENT_WAIT_LIST";
        case -58: return "CL_INVALID_EVENT";
        case -59: return "CL_INVALID_OPERATION";
        case -60: return "CL_INVALID_GL_OBJECT";
        case -61: return "CL_INVALID_BUFFER_SIZE";
        case -62: return "CL_INVALID_MIP_LEVEL";
        case -63: return "CL_INVALID_GLOBAL_WORK_SIZE";
        case -64: return "CL_INVALID_PROPERTY";
        case -65: return "CL_INVALID_IMAGE_DESCRIPTOR";
        case -66: return "CL_INVALID_COMPILER_OPTIONS";
        case -67: return "CL_INVALID_LINKER_OPTIONS";
        case -68: return "CL_INVALID_DEVICE_PARTITION_COUNT";
        case -69: return "CL_INVALID_PIPE_SIZE";
        case -70: return "CL_INVALID_DEVICE_QUEUE";
        case -71: return "CL_INVALID_SPEC_ID";
        case -72: return "CL_MAX_SIZE_RESTRICTION_EXCEEDED";
        case -1002: return "CL_INVALID_D3D10_DEVICE_KHR";
        case -1003: return "CL_INVALID_D3D10_RESOURCE_KHR";
        case -1004: return "CL_D3D10_RESOURCE_ALREADY_ACQUIRED_KHR";
        case -1005: return "CL_D3D10_RESOURCE_NOT_ACQUIRED_KHR";
        case -1006: return "CL_INVALID_D3D11_DEVICE_KHR";
        case -1007: return "CL_INVALID_D3D11_RESOURCE_KHR";
        case -1008: return "CL_D3D11_RESOURCE_ALREADY_ACQUIRED_KHR";
        case -1009: return "CL_D3D11_RESOURCE_NOT_ACQUIRED_KHR";
        case -1010: return "CL_INVALID_DX9_MEDIA_ADAPTER_KHR";
        case -1011: return "CL_INVALID_DX9_MEDIA_SURFACE_KHR";
        case -1012: return "CL_DX9_MEDIA_SURFACE_ALREADY_ACQUIRED_KHR";
        case -1013: return "CL_DX9_MEDIA_SURFACE_NOT_ACQUIRED_KHR";
        case -1093: return "CL_INVALID_EGL_OBJECT_KHR";
        case -1092: return "CL_EGL_RESOURCE_NOT_ACQUIRED_KHR";
        case -1001: return "CL_PLATFORM_NOT_FOUND_KHR";
        case -1057: return "CL_DEVICE_PARTITION_FAILED_EXT";
        case -1058: return "CL_INVALID_PARTITION_COUNT_EXT";
        case -1059: return "CL_INVALID_PARTITION_NAME_EXT";
        case -1094: return "CL_INVALID_ACCELERATOR_INTEL";
        case -1095: return "CL_INVALID_ACCELERATOR_TYPE_INTEL";
        case -1096: return "CL_INVALID_ACCELERATOR_DESCRIPTOR_INTEL";
        case -1097: return "CL_ACCELERATOR_TYPE_NOT_SUPPORTED_INTEL";
        case -1000: return "CL_INVALID_GL_SHAREGROUP_REFERENCE_KHR";
        case -1098: return "CL_INVALID_VA_API_MEDIA_ADAPTER_INTEL";
        case -1099: return "CL_INVALID_VA_API_MEDIA_SURFACE_INTEL";
        case -1100: return "CL_VA_API_MEDIA_SURFACE_ALREADY_ACQUIRED_INTEL";
        case -1101: return "CL_VA_API_MEDIA_SURFACE_NOT_ACQUIRED_INTEL";
        default: return "CL_UNKNOWN_ERROR";
    }
}