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

}

namespace pm {

std::vector<unsigned> gpu_kmp_t::match() {
    Timer_t timer;

    if(!builded_) {
        build_program({"../gpu_matching/kmp/kmp.cl"});
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
        if((pattern.size() > text_.size()) || (pattern.size() == 0)) { 
            ++i;           
            continue;
        }

        auto&& preffix = preffix_function(pattern);
        cl::Buffer preffix_buffer(context_, CL_MEM_READ_ONLY, preffix.size() * sizeof(preffix[0]));
        queue_.enqueueWriteBuffer(preffix_buffer, CL_TRUE, 0, preffix.size() * sizeof(preffix[0]), preffix.data());

        cl::Buffer pattern_buffer(context_, CL_MEM_READ_ONLY, pattern.size() * sizeof(pattern[0]));
        queue_.enqueueWriteBuffer(pattern_buffer, CL_TRUE, 0, pattern.size() * sizeof(pattern[0]), pattern.data());

        std::size_t thread_count = calculate_thread_count(text_.size(), pattern.size());

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