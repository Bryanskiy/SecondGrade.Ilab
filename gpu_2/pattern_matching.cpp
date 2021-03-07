#include "./pattern_matching.hpp"

namespace pm {
/* ----------------------------------------------------- 
                CPU
--------------------------------------------------------*/

void pmCPU_t::match() {
    matches_.resize(patterns_.size());
    for(std::size_t i = 0, maxi = patterns_.size(); i < maxi; ++i) {
        std::string::size_type pos = 0u;
        for(;;) {
            pos = text_.find(patterns_[i], pos);
            if(pos == std::string::npos) {
                break;
            }

            matches_[i].push_back(pos);
            pos += 1;
        }
    }
}

/* ----------------------------------------------------- 
                 GPU
--------------------------------------------------------*/

void pmGPU_t::choose_device() {
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

void pmGPU_t::build_program(const char* file_name) {
    std::ifstream program_sources(file_name);
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

/* ----------------------------------------------------- 
                 GPU KMP
--------------------------------------------------------*/

gpuKMP_t::gpuKMP_t() {
    choose_device();
    context_ = cl::Context(device_);
    build_program("KMP.cl");
    queue_ = cl::CommandQueue(context_, device_, CL_QUEUE_PROFILING_ENABLE);
}

void gpuKMP_t::match() {
    auto prepaired_patterns = prepare_pattern_data();

    std::vector<std::size_t> preffix_funcs(prepaired_patterns.first.size());

    //cl::Buffer text_buffer(context_, CL_MEM_READ_ONLY, text_.size() * sizeof(text_[0]));
    cl::Buffer patterns_buffer(context_, CL_MEM_READ_ONLY, prepaired_patterns.first.size() * sizeof(prepaired_patterns.first[0]));
    cl::Buffer indices_buffer(context_, CL_MEM_READ_ONLY, prepaired_patterns.second.size() * sizeof(prepaired_patterns.second[0]));
    cl::Buffer preffix_buffer(context_, CL_MEM_READ_WRITE, preffix_funcs.size() * sizeof(std::size_t));

    //queue_.enqueueWriteBuffer(text_buffer, CL_TRUE, 0, text_.size() * sizeof(text_[0]), text_.data());
    queue_.enqueueWriteBuffer(patterns_buffer, CL_TRUE, 0, prepaired_patterns.first.size() * sizeof(prepaired_patterns.first[0]), prepaired_patterns.first.data());
    queue_.enqueueWriteBuffer(indices_buffer, CL_TRUE, 0, prepaired_patterns.second.size() * sizeof(prepaired_patterns.second[0]), prepaired_patterns.second.data());
    queue_.enqueueWriteBuffer(preffix_buffer, CL_TRUE, 0,  preffix_funcs.size() * sizeof(std::size_t), preffix_funcs.data()); 

    cl::NDRange offset(0);
    cl::NDRange global_size(patterns_.size());
    cl::NDRange local_size(1);

    cl::Kernel kernel(program_, "preffix");
    //kernel.setArg(0, text_buffer);
    kernel.setArg(0, patterns_buffer);
    kernel.setArg(1, indices_buffer);
    kernel.setArg(2, preffix_buffer);

    cl::Event event;
    queue_.enqueueNDRangeKernel(kernel, offset, global_size, local_size, NULL, &event);
    event.wait();

    auto map_data = (std::size_t*)queue_.enqueueMapBuffer(preffix_buffer, CL_TRUE, CL_MAP_READ, 0, preffix_funcs.size() * sizeof(std::size_t));
    for (size_t i = 0; i < preffix_funcs.size(); i++) {
        preffix_funcs[i] = map_data[i];
    }    
}

std::pair<std::string, std::vector<std::size_t>> gpuKMP_t::prepare_pattern_data() const {
    std::string patterns;
    std::vector<std::size_t> indices;
    std::size_t idx = 0;
    for(auto& pattern : patterns_) {
        patterns += pattern;
        idx += pattern.size();
        indices.push_back(idx);
    }

    return {patterns, indices};
}

} /* namespace pm */