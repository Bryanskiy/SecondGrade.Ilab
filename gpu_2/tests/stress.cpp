#include "../pattern_matching.hpp"
#include "generator.h"

int main() {
    pm::pmCPU_t cpu;
    pm::gpuKMP_t kmp;
    string_generator_t gen;

    std::size_t patterns_size;
    std::size_t left_border, right_border;
    std::size_t text_size;

    std::cout << "Please, enter patterns count: "; std::cin >> patterns_size;
    std::cout << "Please, enter left and right border for pattern size: "; std::cin >> left_border >> right_border;
    std::cout << "Please, enter text size: "; std::cin >> text_size;

    for(std::size_t i = 0; i < patterns_size; ++i) {
        std::string pattern = gen.generate_string(left_border, right_border);
        cpu.push_pattern(pattern);
        kmp.push_pattern(pattern);
    }

    std::string text = gen.generate_string(text_size);
    cpu.set_text(text);
    kmp.set_text(text);

    try {
        cpu.match();
        kmp.match();
    } catch (cl::Error& err) {
        std::cerr << err.what() << std::endl;
        return 1;
    }

    auto cpu_result = cpu.get_matches();
    auto kmp_result = kmp.get_matches();

    if(cpu_result.size() != kmp_result.size()) {
        std::cerr << "TEST: FAILED" << std::endl;
        return 1;
    }

    for(std::size_t i = 0, maxi = cpu_result.size(); i < maxi; ++i) {
        if(cpu_result[i].size() != kmp_result[i].size()) {
            std::cerr << "TEST: FAILED" << std::endl;
            return 1;
        }


        for(std::size_t j = 0, maxj = cpu_result[i].size(); j < maxj; ++j) {
            if(cpu_result[i][j] != kmp_result[i][j]) {
                std::cerr << "TEST: FAILED" << std::endl;
                return 1; 
            }
        } 
    }

    std::cout << "TEST: SUCCESS" << std::endl;
}

