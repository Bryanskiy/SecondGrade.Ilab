#pragma once

#include <string>
#include <vector>
#include "../support/timer.hpp"

/* pm - pattern matching */
namespace pm {

class cpu_pm_t final {
public:
    cpu_pm_t(const std::string& text, const std::vector<std::string>& patterns) : text_(text), patterns_(patterns) {}

    void set_text(std::string& text) {text_ = text;}
    void push_pattern(const std::string& pattern) {patterns_.push_back(pattern);}

    std::vector<std::size_t> match();
    std::size_t get_time() const {return time_;}
private:
    std::string text_;
    std::vector<std::string> patterns_;
    std::size_t time_;
};

} /* namespace pm */