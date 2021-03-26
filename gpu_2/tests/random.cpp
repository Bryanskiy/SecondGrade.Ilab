#include "../gpu_matching/gpu_kmp.hpp"
#include "../cpu_matching/cpu_pm.hpp"
#include "generator.h"

int main(int argc, char** argv) {
/* -------------PROCESS MAIN ARGS ------------------------- */
    std::vector<std::pair<cl::Platform, cl::Device>> info;
    int id = 0;

    try {

        boost::program_options::options_description desc("Options");
        desc.add_options()
            ("help", "get options info")
            ("devices", "get avaible devices")
            ("stress", "add stress test")
            ("set", boost::program_options::value<int>(), "set device");

        boost::program_options::variables_map vm;        
        boost::program_options::store(boost::program_options::parse_command_line(argc, argv, desc), vm);
        boost::program_options::notify(vm);

        if (vm.count("help")) {
            std::cout << "Execute pattern matching random tests." << std::endl;
            std::cout << "Now, avaible only time test(default mode) and stress test, which execute algorithms on CPU and GPU and compare results." << std::endl;

            std::cout << desc;
            return 0;
        }

        info = pm::clcore_t::get_devices();

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

        cl::Device device = info[id].second;
        string_generator_t gen;

        std::size_t patterns_size;
        std::size_t left_border, right_border;
        std::size_t text_size;

        std::cout << "Please, enter patterns count: "; std::cin >> patterns_size;
        std::cout << "Please, enter left and right border for pattern size: "; std::cin >> left_border >> right_border;
        std::cout << "Please, enter text size: "; std::cin >> text_size;

        std::vector<std::string> patterns;

        for(std::size_t i = 0; i < patterns_size; ++i) {
            std::string pattern = gen.generate_string(left_border, right_border);
            patterns.push_back(pattern);
        }

        std::string text = gen.generate_string(text_size);
        pm::cpu_pm_t cpu(text, patterns);
        pm::gpu_kmp_t kmp(device, text, patterns);

        auto cpu_result = cpu.match();
        auto gpu_result = kmp.match();

        std::size_t cpu_time = cpu.get_time();
        std::size_t gpu_time = kmp.gpu_time();
        std::size_t gpu_full_time = kmp.time();

        std::cout << "CPU:      " << cpu_time << " mcs" << std::endl;
        std::cout << "GPU full: " << gpu_full_time << " mcs" << std::endl;
        std::cout << "GPU only: " << gpu_time << " mcs" << std::endl;

        if(vm.count("stress")) {
            if(cpu_result.size() != gpu_result.size()) {
                std::cerr << "TEST: FAILED" << std::endl;
                return 1;
            }

            for(std::size_t i = 0, maxi = cpu_result.size(); i < maxi; ++i) {
                if(cpu_result[i] != gpu_result[i]) {
                    std::cerr << "STRESS TEST: FAIL" << std::endl;
                    return 0;
                }
            }

            std::cout << "STRESS TEST: SUCCESS" << std::endl;
        }

    } catch(cl::Error& ex) {
        std::cerr << ex.what() << std::endl;
        return 1;
    }
    
    
    catch(std::exception& ex) {
        std::cerr << ex.what() << std::endl;
        return 1;
    }    

}    