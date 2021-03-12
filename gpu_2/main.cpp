#include "pattern_matching/pattern_matching.hpp"

#include <iostream>

std::string read_str(std::size_t character_count, std::istream& in) {
    std::string str; str.resize(character_count);
    for(std::size_t i = 0; i < character_count; ++i) {
        in >> str[i];
    }

    return str;
}

int main() {
    pm::gpuKMP_t matcher;

    std::size_t text_size; std::cin >> text_size;
    std::string text = read_str(text_size, std::cin);

    matcher.set_text(text);

    std::size_t patterns_count; std::cin >> patterns_count;

    for(std::size_t i = 0; i < patterns_count; ++i) {
        std::size_t tmp_size; std::cin >> tmp_size;

        std::string tmp = read_str(tmp_size, std::cin);

        matcher.push_pattern(tmp);
    }

    try {
        matcher.match();
    } catch (cl::Error& err) {
        std::cerr << err.what() << std::endl;
        return 0;
    }    
    auto ans = matcher.get_matches();
    for(std::size_t i = 0, maxi = ans.size(); i < maxi; ++i) {
        std::cout << i + 1 << " " << ans[i].size() << std::endl;
    }
}