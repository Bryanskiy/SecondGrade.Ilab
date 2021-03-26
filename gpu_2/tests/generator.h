#pragma once 

#include <random>
#include <string>

class string_generator_t {
public:
    string_generator_t() : rd_(), gen_(rd_()) {}

    std::string generate_string(std::size_t size) {
        std::uniform_int_distribution<> ch_dis('A', 'z');
        std::string str; str.reserve(size);

        for(std::size_t i = 0; i < size; ++i) {
            std::string::value_type ch = ch_dis(gen_);
            str.push_back(ch);
        }

        return str;
    }
    std::string generate_string(std::size_t left_border, std::size_t right_border) {
        std::uniform_int_distribution<> size_dis(left_border, right_border - 1);
        std::size_t size = size_dis(gen_);
        return generate_string(size);
    }
    
private:
    std::random_device rd_;
    std::mt19937 gen_;
};