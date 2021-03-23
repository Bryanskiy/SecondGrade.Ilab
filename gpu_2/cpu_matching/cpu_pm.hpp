#pragma once

#include <string>
#include <vector>

/* pm - pattern matching */
namespace pm {

class cpu_pm_t final {
public:
    cpu_pm_t(const std::string& text, const std::vector<std::string>& patterns) : text_(text), patterns_(patterns) {}

    void set_text(std::string& text) {text_ = text;}
    void push_pattern(std::string& pattern) {patterns_.push_back(pattern);}

    std::vector<std::size_t> match() const;
private:
    std::string text_;
    std::vector<std::string> patterns_;
};

} /* namespace pm */