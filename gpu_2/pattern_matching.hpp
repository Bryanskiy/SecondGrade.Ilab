#pragma once

#include <vector>
#include <string>
#include <stdexcept>
#include <iostream>
#include <fstream>

#define __CL_ENABLE_EXCEPTIONS
#include <CL/cl.hpp>

namespace pm {

class pattern_matching_t {
public:
    pattern_matching_t() {}
    void push_pattern(const std::string& pattern) {patterns_.push_back(pattern);}

    void set_text(const std::string& text) {
        text_ = text;
        matches_ = std::vector<std::vector<std::size_t>>{}; /* reset */
    }

    std::vector<std::vector<std::size_t>> get_matches() const {return matches_;}

    virtual void match() = 0;
    virtual ~pattern_matching_t() {}
protected:
    std::string text_;
    std::vector<std::string> patterns_;
    /* table of mathes : matches[i][j] - j occurrence in i pattern */
    std::vector<std::vector<std::size_t>> matches_;
};

/* it can be upgraded with CPU thread control */
class pmCPU_t : public pattern_matching_t {
public:
    pmCPU_t() {}
    void match() override;
    ~pmCPU_t() override {}
};

class pmGPU_t : public pattern_matching_t {
public:
    pmGPU_t() {}
    void match() override = 0;
    ~pmGPU_t() override {}

protected:
    void choose_device();
    void build_program(const char* file_name);

protected:
    cl::Device         device_;
    cl::Context        context_;
    cl::Program        program_;
    cl::CommandQueue   queue_;
};

class gpuKMP_t final : public pmGPU_t {
public:
    gpuKMP_t();
    void match() override;
    ~gpuKMP_t() override {}
private:
    std::pair<std::string, std::vector<std::size_t>> prepare_pattern_data() const;    
};

} /* namespace pm */