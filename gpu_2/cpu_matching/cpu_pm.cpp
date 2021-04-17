#include "cpu_pm.hpp"

namespace pm {

std::vector<std::size_t> match(std::string& text, std::string& pattern) {
    std::vector<std::size_t> ret;

    if((pattern.size() > text.size()) || (pattern.size() == 0)) { 
            return ret;
    }

    std::string::size_type pos = 0u;
    for(;;) {
        pos = text.find(pattern, pos);
        if(pos == std::string::npos) {
            break;
        }

        ret.push_back(pos);
        pos += 1;
    }

    return ret;
}

std::vector<std::vector<std::size_t>> cpu_pm_t::match(std::string& text) {
    Timer_t timer;
    std::vector<std::vector<std::size_t>> res;
    res.reserve(patterns_.size());

    for(auto&& pattern : patterns_) {
        res.emplace_back(pm::match(text, pattern));
    }

    time_ = timer.get_time().count();
    return res;
} 

} /* namespace pm */