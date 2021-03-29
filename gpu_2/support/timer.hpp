#pragma once

#include <chrono>

class Timer_t {
public:
    using clock_t = std::chrono::high_resolution_clock;
    using microseconds_t = std::chrono::microseconds;

    Timer_t() : start_(clock_t::now()) {}
    microseconds_t get_time() {
        return std::chrono::duration_cast<microseconds_t>(clock_t::now() - start_);
    }
    void reset() {
        start_ = clock_t::now();
    }
private:
    std::chrono::time_point<clock_t> start_;
};