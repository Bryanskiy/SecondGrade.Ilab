#include <iostream>

#include "gpu_matching/cl_general.hpp"
#include "gpu_matching/filter.hpp"

std::string read_str(std::istream& input) {
    std::string ret;
    size_t size = 0; input >> size;
    if(!size) {return ret;}

    input.ignore(1);
    ret.resize(size);
    input.read(ret.data(), size);
    input.ignore(1);

    return ret;
}

int main() {
    try {

        std::string text = read_str(std::cin);
        std::vector<std::string> patterns;
        std::size_t patterns_count; std::cin >> patterns_count;
        patterns.reserve(patterns_count);

        for(std::size_t i = 0; i < patterns_count; ++i) {
            std::string tmp = read_str(std::cin);
            patterns.push_back(tmp);
        }

        pm::gpu_filter_t pm(patterns);
        auto&& ans = pm.match(text);
        for(std::size_t i = 0; i < ans.size(); ++i) {
            std::cout << i + 1 << " " << ans[i].size() << std::endl;
        }

        } catch(cl::Error& err) {
            std::cerr << err.what() << std::endl; 
            std::cerr << pm::cl_get_error_string(err.err()) << std::endl;
        }
    
        catch(std::runtime_error& err) {
            std::cerr << err.what() << std::endl; 
        }
}