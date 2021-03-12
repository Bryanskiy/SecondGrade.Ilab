#pragma once

#include <vector>
#include <string>
#include <stdexcept>
#include <iostream>
#include <fstream>
#include <cassert>

#include "../other/timer.hpp"

#define __CL_ENABLE_EXCEPTIONS
#include <CL/cl.hpp>


namespace pm {

class basePM_t {
public:
    basePM_t() {}
    void push_pattern(const std::string& pattern) {patterns_.push_back(pattern);}

    void set_text(const std::string& text) {
        text_ = text;
    }

    std::vector<std::vector<std::size_t>> get_matches() const {return matches_;}

    void reset() {
        patterns_ = std::vector<std::string>{};
        matches_ = std::vector<std::vector<std::size_t>>{};
        time_ = 0;
    }

    std::size_t get_time() const {return time_;}

    virtual void match() = 0;
    virtual ~basePM_t() {}
protected:
    std::string text_;
    std::vector<std::string> patterns_;
    /* table of mathes : matches[i][j] - j occurrence in i pattern */
    std::vector<std::vector<std::size_t>> matches_;
    std::size_t time_ = 0;
};

/* it can be upgraded with CPU thread control */
class cpuPM_t : public basePM_t {
public:
    cpuPM_t() {}
    void match() override;
    ~cpuPM_t() override {}
};

class gpuPM_t : public basePM_t {
public:
    gpuPM_t() {}
    void match() override = 0;
    ~gpuPM_t() override {}

    std::size_t get_gpu_time() const {return gpu_time_;}

protected:
    void choose_device();
    void build_program(const std::vector<std::string>& kernels);
    void enqueue_kernel(cl::Kernel& kernel, cl::NDRange& offset, cl::NDRange& global_size, cl::NDRange& local_size);

protected:
    cl::Device         device_;
    cl::Context        context_;
    cl::Program        program_;
    cl::CommandQueue   queue_;
    std::size_t gpu_time_ = 0;
};

class gpuKMP_t final : public gpuPM_t {
public:
    enum REALIZATION {
        PARALLEL_PATTERN,
        TEXT_DIVISION,
    };

    gpuKMP_t();
    void match() override;
    ~gpuKMP_t() override {}

    void set_realization(REALIZATION realization) {realization_ = realization;}
private:
    void parallel_pattern_match();
    void text_divison_match();

    std::pair<std::string, std::vector<std::size_t>> concatinate_patterns() const;
private:    
    REALIZATION realization_ = PARALLEL_PATTERN;
};

} /* namespace pm */