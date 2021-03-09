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


    std::cout << "CPU:               " << cpu.get_time() << " mcs" << std::endl;
    std::cout << "GPU full/GPU only: " << kmp.get_time() << " " << kmp.get_gpu_time() << " mcs" << std::endl;
}