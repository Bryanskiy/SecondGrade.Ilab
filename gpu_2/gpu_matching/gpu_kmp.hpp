#pragma once

#include <vector>
#include <string>
#include "cl_core.hpp"

namespace pm {

class gpu_kmp_t {
public:
    gpu_kmp_t() = default;
    gpu_kmp_t(const cl::Device& device , const std::string& text, const std::vector<std::string>& patterns) : 
        core_(device), text_(text), patterns_(patterns) {}

    std::vector<std::size_t> match();
private:
    clcore_t core_;
    std::string text_;
    std::vector<std::string> patterns_;

    bool builded_ = false;
};

std::vector<std::size_t> preffix_function(const std::string& str);

} /* namespace pm */