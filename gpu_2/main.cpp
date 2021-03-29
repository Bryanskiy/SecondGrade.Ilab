#include <iostream>
#include <boost/program_options.hpp>

#include "cpu_matching/cpu_pm.hpp"
#include "gpu_matching/gpu_kmp.hpp"
#include "support/cl_support.hpp"

std::string read_str(std::istream& input) {
    std::string ret;
    size_t size = 0; input >> size;

    input.ignore(1);
    ret.resize(size);
    input.read(ret.data(), size);
    input.ignore(1);

    return ret;
}

int main(int argc, char** argv) {
/* -------------PROCESS MAIN ARGS ------------------------- */
    std::vector<std::pair<cl::Platform, cl::Device>> info;
    int id = 0;

    try {

        boost::program_options::options_description desc("Options");
        desc.add_options()
            ("help", "get options info")
            ("devices", "get avaible devices")
            ("set", boost::program_options::value<int>(), "set device");

        boost::program_options::variables_map vm;        
        boost::program_options::store(boost::program_options::parse_command_line(argc, argv, desc), vm);
        boost::program_options::notify(vm);

        if (vm.count("help")) {
            std::cout << desc;
            return 0;
        }

        info = clsup::get_devices();

        if(vm.count("devices")) {
            std::cout << "Available devices and platforms:" << std::endl;
            for (size_t i = 0; i < info.size(); ++i) {
                std::cout << "--------------------------------------------------------------" << std::endl;
                std::cout << "Id: " << i << std::endl;
                std::cout << "Platform:\t";
                std::cout << info[i].first.getInfo<CL_PLATFORM_NAME>() << std::endl;
                std::cout << "Device:\t\t";
                std::cout << info[i].second.getInfo<CL_DEVICE_NAME>() << std::endl;
            }

            return 0;
        }

        if(vm.count("set")) {
            id = vm["set"].as<int>();
        }

    } catch(std::exception& ex) {
        std::cerr << ex.what() << std::endl;
        return 1;
    }    

    cl::Device device = info[id].second;
/* --------------MAIN PROGRAM ----------------------------- */
    std::string text = read_str(std::cin);
    std::vector<std::string> patterns;
    std::size_t patterns_count; std::cin >> patterns_count;
    patterns.reserve(patterns_count);

    for(std::size_t i = 0; i < patterns_count; ++i) {
        std::string tmp = read_str(std::cin);
        patterns.push_back(tmp);
    }

    pm::gpu_kmp_t pm(device, text, patterns);
    try {
        auto&& res = pm.match();
        for(std::size_t i = 0, maxi = res.size(); i < maxi; ++i) {
            std::cout << i + 1 << " " << res[i] << std::endl;
        }
    } catch(cl::Error& err) {
        std::cerr << err.what() << std::endl; 
        std::cerr << clsup::cl_get_error_string(err.err()) << std::endl;
    }
    
    catch(std::exception& err) {
        std::cerr << err.what() << std::endl; 
    }
}