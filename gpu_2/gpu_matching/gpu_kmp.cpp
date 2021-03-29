#include "gpu_kmp.hpp"

namespace {

std::size_t calculate_thread_count(std::size_t text_size, std::size_t pattern_size) {
    std::size_t ans = 1;
    pattern_size *= 2;
    while(text_size > pattern_size) {
        ans *= 2;
        text_size /= 2;
    }

    return ans;
}

}

namespace pm {

void gpu_kmp_t::build_program(const std::vector<std::string>& kernels) {

    std::string program_string;

    for(auto& kernel_name : kernels) {
        std::ifstream program_sources(kernel_name);

        if(!program_sources.good()) {
            throw std::runtime_error("Can't open kernel");
        }

        program_string += std::string(std::istreambuf_iterator<char>(program_sources), (std::istreambuf_iterator<char>()));
    }
    
    cl::Program::Sources source(1, std::make_pair(program_string.c_str(), program_string.length() + 1));
    program_ = cl::Program(context_, source);

    try {
        program_.build("-O2");
    } catch (cl::Error& e) {
        std::cerr << program_.getBuildInfo<CL_PROGRAM_BUILD_LOG>(device_);
        throw e;
    } 
}                                     

void gpu_kmp_t::enqueue_kernel(cl::Kernel& kernel, cl::NDRange& offset, cl::NDRange& global_size, cl::NDRange& local_size) {
    queue_.enqueueNDRangeKernel(kernel, offset, global_size, local_size, NULL, NULL);
}

std::vector<std::size_t> gpu_kmp_t::match() {
    Timer_t timer;

    if(!builded_) {
        build_program({"../gpu_matching/kmp.cl"});
        builded_ = true;
    }

    std::vector<std::size_t> ans;
    ans.reserve(patterns_.size());

    cl::Buffer text_buffer(context_, CL_MEM_READ_ONLY, text_.size() * sizeof(text_[0]));
    queue_.enqueueWriteBuffer(text_buffer, CL_TRUE, 0, text_.size() * sizeof(text_[0]), text_.data());

    for(auto&& pattern : patterns_) {
        if(pattern.size() > text_.size()) {
            ans.push_back(0u);
            continue;
        }

        auto&& preffix = preffix_function(pattern);
        cl::Buffer preffix_buffer(context_, CL_MEM_READ_ONLY, preffix.size() * sizeof(preffix[0]));
        queue_.enqueueWriteBuffer(preffix_buffer, CL_TRUE, 0, preffix.size() * sizeof(preffix[0]), preffix.data());

        cl::Buffer pattern_buffer(context_, CL_MEM_READ_ONLY, pattern.size() * sizeof(pattern[0]));
        queue_.enqueueWriteBuffer(pattern_buffer, CL_TRUE, 0, pattern.size() * sizeof(pattern[0]), pattern.data());

        std::size_t thread_count = calculate_thread_count(text_.size(), pattern.size());
        std::vector<std::size_t> ans_vector(thread_count);
        cl::Buffer ans_buffer(context_, CL_MEM_READ_ONLY, ans_vector.size() * sizeof(ans_vector[0]));
        queue_.enqueueWriteBuffer(ans_buffer, CL_TRUE, 0, ans_vector.size() * sizeof(ans_vector[0]), ans_vector.data());

        cl::NDRange offset(0);
        cl::NDRange global_size(thread_count);
        cl::NDRange local_size(std::min(thread_count, device_.getInfo<CL_DEVICE_MAX_WORK_GROUP_SIZE>()));

        cl::Kernel kernel(program_, "kmp");
        kernel.setArg(0, text_buffer);
        kernel.setArg(1, text_.size());
        kernel.setArg(2, pattern_buffer);
        kernel.setArg(3, pattern.size());
        kernel.setArg(4, preffix_buffer);
        kernel.setArg(5, preffix.size());
        kernel.setArg(6, ans_buffer);

        cl::LocalSpaceArg local_pattern = cl::__local(pattern.size() * sizeof(char));
        kernel.setArg(7, local_pattern);

        cl::LocalSpaceArg local_preffix = cl::__local(preffix.size() * sizeof(preffix[0]));
        kernel.setArg(8, local_preffix);

        enqueue_kernel(kernel, offset, global_size, local_size);

        std::size_t* map_data = (std::size_t*)queue_.enqueueMapBuffer(ans_buffer, CL_TRUE, CL_MAP_READ, 0, ans_vector.size() * sizeof(ans_vector[0]));
        std::size_t ans_pattern = 0;
        for(std::size_t i = 0; i < thread_count; ++i) {
            ans_pattern += map_data[i];
        }

        ans.push_back(ans_pattern);
    }

    time_ = timer.get_time().count();

    return ans;
}

std::vector<std::size_t> preffix_function(const std::string& str) {
    std::vector<std::size_t> ret(str.size());

    for(std::size_t i = 1, maxi = ret.size(); i < maxi; ++i) {
        std::size_t j = ret[i - 1];
        while((j > 0) && (str[i] != str[j])) {
            j = ret[j - 1];
        }

        if(str[i] == str[j]) {
            ++j;
        }

        ret[i] = j;
    }

    return ret;
}

} /* namespace pm */