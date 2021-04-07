#include "gpu_kmp.hpp"

namespace {

unsigned calculate_thread_count(unsigned text_size, unsigned pattern_size) {
    unsigned tmp = text_size / (4 * pattern_size) + 1;
    unsigned ans = 1;
    while(ans < tmp) {ans *= 2;}
    return ans;
}

std::vector<unsigned> preffix_function(const std::string& str) {
    std::vector<unsigned> ret(str.size());

    for(unsigned i = 1, maxi = ret.size(); i < maxi; ++i) {
        unsigned j = ret[i - 1];
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

const std::string wired_kernels = "\
__kernel void kmp(__global char* text, ulong text_size, __global char* pattern, ulong pattern_size,\ 
                  __global uint* preffix, ulong preffix_size, __global uint* answer)\
{\
    uint id = get_global_id(0);\
    uint thread_count = get_global_size(0);\
    uint step = text_size / thread_count;\
    if((text_size % thread_count) != 0) {\
        step += 1;\
    }\
\
    uint left_border  = step * id;\
    uint right_border = min(left_border + step - 1 + pattern_size, text_size);\
\
    uint positions_number = 0;\
    uint i = left_border;\
    uint j = 0;\
\
    while(i < right_border) {\
        if(pattern[j] == text[i]) {\
            ++i; ++j;\
        }\   
\           
        if(j == pattern_size) {\
            ++positions_number;\
            j = preffix[j - 1];\
        } else if((i < right_border) && pattern[j] != text[i]) {\
            if (j > 0) {\
                j = preffix[j - 1];\
            }\     
            else {\
                i += 1;\
            }\    
        }\
    }\
    answer[id] += positions_number;\
}";

}

namespace pm {

void gpu_kmp_t::build_program(const std::vector<std::string>& kernels) {

#ifdef LOG
    sup::log.separate();
    sup::log.log_file << "Start matching for text with len = " << text_.size() << std::endl;
#endif


    std::string program_string;

    bool use_extern_kernel = true;
    for(auto& kernel_name : kernels) {
        std::ifstream program_sources(kernel_name);

        if(!program_sources.good()) {
            use_extern_kernel = false;
            break;
        }

        auto&& lhs = std::istreambuf_iterator<char>{program_sources};
        auto&& rhs = std::istreambuf_iterator<char>{};
        program_string += std::string(lhs, rhs);
    }

    cl::Program::Sources source;
    if(use_extern_kernel) {
        source = cl::Program::Sources(1, std::make_pair(program_string.c_str(), program_string.length() + 1));
    } else {
        source = cl::Program::Sources(1, std::make_pair(wired_kernels.c_str(), wired_kernels.length() + 1));
    }    

    program_ = cl::Program(context_, source);

    try {
        program_.build("-O2");
    } catch (cl::Error& e) {
        std::cerr << program_.getBuildInfo<CL_PROGRAM_BUILD_LOG>(device_);
        throw e;
    } 
}

std::vector<unsigned> gpu_kmp_t::match() {
    Timer_t timer;

    if(!builded_) {
        build_program({"../gpu_matching/kmp.cl"});
        builded_ = true;
    }

    std::vector<unsigned> ans;
    ans.reserve(patterns_.size());

    cl::Buffer text_buffer(context_, CL_MEM_READ_ONLY, text_.size() * sizeof(text_[0]));
    queue_.enqueueWriteBuffer(text_buffer, CL_TRUE, 0, text_.size() * sizeof(text_[0]), text_.data());

    std::vector<cl::Event> events(patterns_.size());
    std::vector<std::vector<unsigned>> answers(patterns_.size());
    std::vector<cl::Buffer> ans_buffers(patterns_.size());

    unsigned i = 0;
    for(auto&& pattern : patterns_) {
#ifdef LOG
    sup::log.log_file << "Start processing pattern with len = " << pattern.size() << std::endl;
#endif        
        if((pattern.size() > text_.size()) || (pattern.size() == 0)) {
#ifdef LOG
    sup::log.log_file << "Skip this pattern" << std::endl;
#endif             
            continue;
        }

        auto&& preffix = preffix_function(pattern);
        cl::Buffer preffix_buffer(context_, CL_MEM_READ_ONLY, preffix.size() * sizeof(preffix[0]));
        queue_.enqueueWriteBuffer(preffix_buffer, CL_TRUE, 0, preffix.size() * sizeof(preffix[0]), preffix.data());

        cl::Buffer pattern_buffer(context_, CL_MEM_READ_ONLY, pattern.size() * sizeof(pattern[0]));
        queue_.enqueueWriteBuffer(pattern_buffer, CL_TRUE, 0, pattern.size() * sizeof(pattern[0]), pattern.data());

        std::size_t thread_count = calculate_thread_count(text_.size(), pattern.size());
#ifdef LOG
    sup::log.log_file << "Calculated thread count: " << thread_count << std::endl;
#endif

        answers[i].resize(thread_count);
        ans_buffers[i] = cl::Buffer(context_, CL_MEM_READ_WRITE, answers[i].size() * sizeof(answers[i][0]));
        queue_.enqueueWriteBuffer(ans_buffers[i], CL_TRUE, 0, answers[i].size() * sizeof(answers[i][0]), answers[i].data());

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
        kernel.setArg(6, ans_buffers[i]);

        queue_.enqueueNDRangeKernel(kernel, offset, global_size, local_size, NULL, &events[i]);
        ++i;
    }

    for(unsigned i = 0, maxi = patterns_.size(); i < maxi; ++i) {
        if((patterns_[i].size() > text_.size()) || (patterns_[i].size() == 0)) {
            ans.push_back(0);
            continue;
        }

        events[i].wait();

        std::size_t start = events[i].getProfilingInfo<CL_PROFILING_COMMAND_START>();
        std::size_t end = events[i].getProfilingInfo<CL_PROFILING_COMMAND_END>();
        gpu_only_time_ += (end - start) / 1000;

        queue_.enqueueReadBuffer(ans_buffers[i], CL_TRUE, 0, answers[i].size() * sizeof(answers[i][0]), answers[i].data());

        unsigned ans_pattern = std::accumulate(answers[i].begin(), answers[i].end(), 0);
        ans.push_back(ans_pattern);
    }

    time_ = timer.get_time().count();

    return ans;
}

} /* namespace pm */