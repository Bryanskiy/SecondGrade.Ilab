#pragma once

#include <vector>
#include <string>
#include <fstream>
#include <numeric>
#include <iostream>


#include "../cl_general.hpp"
#include "../../support/timer.hpp"

namespace pm {

class gpu_kmp_t : public cl_iclass_t {
public:
    gpu_kmp_t() = default;
    gpu_kmp_t(const cl::Device& device , const std::string& text, const std::vector<std::string>& patterns) : 
              cl_iclass_t(device), text_(text), patterns_(patterns) {}

    std::vector<unsigned> match();

    std::size_t time() const {return time_;}
    std::size_t gpu_only_time() const {return gpu_only_time_;}

private:
    std::string text_;
    std::vector<std::string> patterns_;

    std::size_t time_;
    std::size_t gpu_only_time_ = 0;
    bool builded_ = false;
};

} /* namespace pm */