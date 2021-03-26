#include "gpu_kmp.hpp"

namespace pm {

std::vector<std::size_t> gpu_kmp_t::match() {
    if(!builded_) {
        core_.build_program({"../gpu_matching/kmp.cl"});
        builded_ = true;
    }

    std::vector<std::size_t> ans;
    ans.reserve(patterns_.size());

    cl::Buffer text_buffer(core_.get_context(), CL_MEM_READ_ONLY, text_.size() * sizeof(text_[0]));
    auto queue = core_.get_queue();
    queue.enqueueWriteBuffer(text_buffer, CL_TRUE, 0, text_.size() * sizeof(text_[0]), text_.data());

    for(const auto& pattern : patterns_) {
        if(pattern.size() > text_.size()) {
            ans.push_back(0u);
        }

        auto preffix = preffix_function(pattern);
        cl::Buffer preffix_buffer(core_.get_context(), CL_MEM_READ_ONLY, preffix.size() * sizeof(preffix[0]));
        queue.enqueueWriteBuffer(preffix_buffer, CL_TRUE, 0, preffix.size() * sizeof(preffix[0]), preffix.data());

        cl::Buffer pattern_buffer(core_.get_context(), CL_MEM_READ_ONLY, pattern.size() * sizeof(pattern[0]));
        queue.enqueueWriteBuffer(pattern_buffer, CL_TRUE, 0, pattern.size() * sizeof(pattern[0]), pattern.data());

        std::size_t pattern_ans = 0;
        cl::Buffer ans_buffer(core_.get_context(), CL_MEM_READ_WRITE | CL_MEM_USE_HOST_PTR, 1, &pattern_ans);

        cl::NDRange offset(0);
        std::size_t thread_count = text_.size() / pattern.size() + 1;
        cl::NDRange global_size(thread_count);
        cl::NDRange local_size(1);

        cl::Kernel kernel(core_.get_program(), "kmp");
        kernel.setArg(0, text_buffer);
        kernel.setArg(1, text_.size());
        kernel.setArg(2, pattern_buffer);
        kernel.setArg(3, pattern.size());
        kernel.setArg(4, preffix_buffer);
        kernel.setArg(5, preffix.size());
        kernel.setArg(6, ans_buffer);

        cl::Event event;
        queue.enqueueNDRangeKernel(kernel, offset, global_size, local_size, NULL, &event);
        event.wait();

        ans.push_back(pattern_ans);
    }

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