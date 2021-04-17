#pragma once

#include <string>
#include <vector>
#include "../support/timer.hpp"

/* pm - pattern matching */
namespace pm {

std::vector<std::size_t> match(std::string& text, std::string& pattern);

class cpu_pm_t final {
public:
    cpu_pm_t(const std::vector<std::string>& patterns) : patterns_(patterns) {}

    std::vector<std::vector<std::size_t>> match(std::string& text);
    std::size_t get_time() const {return time_;}
private:
    std::vector<std::string> patterns_;
    std::size_t time_;
};

} /* namespace pm */