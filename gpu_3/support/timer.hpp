#pragma once

#include <chrono>

template<typename Dim>
class Timer_t {
public:
    using clock_t = std::chrono::high_resolution_clock;
    using microseconds_t = std::chrono::microseconds;
    using milliseconds_t = std::chrono::milliseconds;

    Timer_t() : start_(clock_t::now()) {}
    std::size_t get_time() {
        return (std::chrono::duration_cast<Dim>(clock_t::now() - start_).count();
    }
    void reset() {
        start_ = clock_t::now();
    }
private:
    std::chrono::time_point<clock_t> start_;
};