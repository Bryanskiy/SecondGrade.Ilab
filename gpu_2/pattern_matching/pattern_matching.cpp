#include "./pattern_matching.hpp"

namespace pm {
/* ----------------------------------------------------- 
                CPU
--------------------------------------------------------*/

void cpuPM_t::match() {
    Timer_t timer;
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

    time_ = timer.get_time().count();
}

/* ----------------------------------------------------- 
                 GPU
--------------------------------------------------------*/

void gpuPM_t::choose_device() {
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

void gpuPM_t::build_program(const std::vector<std::string>& kernels) {

    std::string program_string;

    for(std::size_t i = 0; i < kernels.size(); ++i) { 
        std::ifstream program_sources(kernels[i]);
        std::string tmp(std::istreambuf_iterator<char>(program_sources), (std::istreambuf_iterator<char>()));
        program_string += tmp;
    }    
    cl::Program::Sources source(1, std::make_pair(program_string.c_str(), program_string.length() + 1));
    program_ = cl::Program(context_, source);

    try {
        program_.build();
    } catch (cl::Error e) {
        std::cerr << program_.getBuildInfo<CL_PROGRAM_BUILD_LOG>(device_);
        throw e;
    }    
}

void gpuPM_t::enqueue_kernel(cl::Kernel& kernel, cl::NDRange& offset, cl::NDRange& global_size, cl::NDRange& local_size) {
    cl::Event event;
    queue_.enqueueNDRangeKernel(kernel, offset, global_size, local_size, NULL, &event);

    event.wait();

    std::size_t start = event.getProfilingInfo<CL_PROFILING_COMMAND_START>();
    std::size_t end = event.getProfilingInfo<CL_PROFILING_COMMAND_END>();
    gpu_time_ += (end - start) / 1000;
}

/* ----------------------------------------------------- 
                 GPU KMP
--------------------------------------------------------*/

gpuKMP_t::gpuKMP_t() {
    choose_device();
    context_ = cl::Context(device_);
    queue_ = cl::CommandQueue(context_, device_, CL_QUEUE_PROFILING_ENABLE);
}

void gpuKMP_t::match() {
    switch (realization_) {
    case PARALLEL_PATTERN: 
        parallel_pattern_match();
        break;

    case TEXT_DIVISION:
        text_divison_match();
        break;

    default:
        assert(0);
    }
}

void gpuKMP_t::parallel_pattern_match() {
    build_program({"./kernels/preffix_funcs1.cl",
                   "./kernels/kmp1.cl"});

    Timer_t timer;
    // CALCULATE PREFFIX FUNCS ON GPU
    //---------------------------------------------------------------
    auto prepaired_patterns = concatinate_patterns();

    std::vector<std::size_t> preffix_funcs(prepaired_patterns.first.size());

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

    cl::Kernel preffix_kernel(program_, "preffix");
    //kernel.setArg(0, text_buffer);
    preffix_kernel.setArg(0, patterns_buffer);
    preffix_kernel.setArg(1, indices_buffer);
    preffix_kernel.setArg(2, preffix_buffer);

    enqueue_kernel(preffix_kernel, offset, global_size, local_size);
    // RUN MATCHING
    //---------------------------------------------------------------
    cl::Buffer text_buffer(context_, CL_MEM_READ_ONLY, text_.size() * sizeof(text_[0]));
    std::vector<std::size_t> ans_vector(text_.size() * patterns_.size());
    cl::Buffer ans_buffer(context_, CL_MEM_WRITE_ONLY, ans_vector.size() * sizeof(ans_vector[0]));

    queue_.enqueueWriteBuffer(text_buffer, CL_TRUE, 0, text_.size() * sizeof(text_[0]), text_.data());
    queue_.enqueueWriteBuffer(ans_buffer, CL_TRUE, 0, ans_vector.size() * sizeof(ans_vector[0]), ans_vector.data());

    cl::Kernel kmp_kernel(program_, "kmp1");
    kmp_kernel.setArg(0, text_buffer);
    kmp_kernel.setArg(1, text_.size());
    kmp_kernel.setArg(2, patterns_buffer);
    kmp_kernel.setArg(3, indices_buffer);
    kmp_kernel.setArg(4, preffix_buffer);
    kmp_kernel.setArg(5, ans_buffer);

    enqueue_kernel(kmp_kernel, offset, global_size, local_size);

    // WRITE ANSWER
    //--------------------------------------------------------------
    auto map_data = (std::size_t*)queue_.enqueueMapBuffer(ans_buffer, CL_TRUE, CL_MAP_READ, 0, ans_vector.size() * sizeof(ans_vector[0]));

    /* free */
    queue_.enqueueUnmapMemObject(preffix_buffer, preffix_funcs.data());
    queue_.enqueueUnmapMemObject(patterns_buffer, prepaired_patterns.first.data());
    queue_.enqueueUnmapMemObject(indices_buffer, prepaired_patterns.second.data());
    queue_.enqueueUnmapMemObject(text_buffer, text_.data());
    queue_.enqueueUnmapMemObject(ans_buffer, ans_vector.data());

    matches_.resize(patterns_.size());
    for(std::size_t i = 0, maxi = patterns_.size(); i < maxi; ++i) {
        std::size_t j = 0;
        for(;;) {
            std::size_t tmp = map_data[text_.size() * i + j];

            if(tmp == 0) {
                break;
            }

            matches_[i].push_back(tmp - 1);
            ++j;
        }
    }

    time_ = timer.get_time().count();
}

std::pair<std::string, std::vector<std::size_t>> gpuKMP_t::concatinate_patterns() const {
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

void gpuKMP_t::text_divison_match() {
    build_program({"./kernels/preffix_funcs1.cl",
                   "./kernels/kmp1.cl"});
}

} /* namespace pm */