#include "cpu_pm.hpp"

namespace pm {

std::vector<std::size_t> cpu_pm_t::match() {
    Timer_t timer;
    std::vector<std::size_t> res;
    res.reserve(patterns_.size());

    for(auto&& pattern : patterns_) {
        if((pattern.size() > text_.size()) || (pattern.size() == 0)) { 
            res.emplace_back(0u);
            continue;
        }

        std::size_t tmp = 0;
        std::string::size_type pos = 0u;
        for(;;) {
            pos = text_.find(pattern, pos);
            if(pos == std::string::npos) {
                break;
            }

            tmp += 1;
            pos += 1;
        }

        res.emplace_back(tmp);
    }

    time_ = timer.get_time().count();
    return res;
} 

} /* namespace pm */