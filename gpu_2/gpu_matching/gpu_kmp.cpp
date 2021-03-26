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

std::vector<std::size_t> gpu_kmp_t::match() {
    Timer_t timer;

    if(!builded_) {
        core_.build_program({"../gpu_matching/kmp.cl"});
        builded_ = true;
    }

    std::vector<std::size_t> ans;
    ans.reserve(patterns_.size());

    cl::Buffer text_buffer(core_.get_context(), CL_MEM_READ_ONLY, text_.size() * sizeof(text_[0]));
    auto queue = core_.get_queue();
    queue.enqueueWriteBuffer(text_buffer, CL_TRUE, 0, text_.size() * sizeof(text_[0]), text_.data());

    for(auto&& pattern : patterns_) {
        if(pattern.size() > text_.size()) {
            ans.push_back(0u);
            continue;
        }

        auto&& preffix = preffix_function(pattern);
        cl::Buffer preffix_buffer(core_.get_context(), CL_MEM_READ_ONLY, preffix.size() * sizeof(preffix[0]));
        queue.enqueueWriteBuffer(preffix_buffer, CL_TRUE, 0, preffix.size() * sizeof(preffix[0]), preffix.data());

        cl::Buffer pattern_buffer(core_.get_context(), CL_MEM_READ_ONLY, pattern.size() * sizeof(pattern[0]));
        queue.enqueueWriteBuffer(pattern_buffer, CL_TRUE, 0, pattern.size() * sizeof(pattern[0]), pattern.data());

        std::size_t thread_count = calculate_thread_count(text_.size(), pattern.size());
        std::vector<std::size_t> ans_vector(thread_count);
        cl::Buffer ans_buffer(core_.get_context(), CL_MEM_READ_ONLY, ans_vector.size() * sizeof(ans_vector[0]));
        queue.enqueueWriteBuffer(ans_buffer, CL_TRUE, 0, ans_vector.size() * sizeof(ans_vector[0]), ans_vector.data());

        cl::NDRange offset(0);
        cl::NDRange global_size(thread_count);
        cl::NDRange local_size(std::min(thread_count, core_.get_device().getInfo<CL_DEVICE_MAX_WORK_GROUP_SIZE>()));

        cl::Kernel kernel(core_.get_program(), "kmp");
        kernel.setArg(0, text_buffer);
        kernel.setArg(1, text_.size());
        kernel.setArg(2, pattern_buffer);
        kernel.setArg(3, pattern.size());
        kernel.setArg(4, preffix_buffer);
        kernel.setArg(5, preffix.size());
        kernel.setArg(6, ans_buffer);

        core_.enqueue_kernel(kernel, offset, global_size, local_size);

        std::size_t* map_data = (std::size_t*)queue.enqueueMapBuffer(ans_buffer, CL_TRUE, CL_MAP_READ, 0, ans_vector.size() * sizeof(ans_vector[0]));
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