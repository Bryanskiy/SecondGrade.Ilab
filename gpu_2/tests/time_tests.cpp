#include <boost/program_options.hpp>

#include "../gpu_matching/filter.hpp"
#include "../gpu_matching/cl_general.hpp"
#include "../cpu_matching/cpu_pm.hpp"
#include "generator.h"

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
    std::vector<pm::machine_t> info;

    try {

        boost::program_options::options_description desc("Options");
        desc.add_options()
            ("help", "get options info")
            ("devices", "get avaible devices")
            ("usert", "set user tests moode")
            ("all", "run test for all devives")
            ("set", boost::program_options::value<int>(), "set device");

        boost::program_options::variables_map vm;        
        boost::program_options::store(boost::program_options::parse_command_line(argc, argv, desc), vm);
        boost::program_options::notify(vm);

        if (vm.count("help")) {
            std::cout << "Execute pattern matching time tests." << std::endl;
            std::cout << desc;
            return 0;
        }

        info = pm::get_devices();

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

        int id = 0;
        std::vector<cl::Device> devices;

        if(vm.count("set")) {
            id = vm["set"].as<int>();

            if(id >= info.size()) {
                std::cerr << "invalid id" << std::endl;
                return 0;
            }

            devices.push_back(info[id].second);
        } 

        if(vm.count("all")) {
            for(auto&& pair : info) {
                devices.push_back(pair.second);
            }
        } else {
            devices.push_back(pm::choose_default_device(info));          
        }

        std::vector<std::string> patterns;
        std::string text;

        if(!vm.count("usert")) {
            string_generator_t gen;
            std::size_t patterns_count;
            std::size_t left_border, right_border;
            std::size_t text_size;

            std::cout << "Please, enter patterns count: "; std::cin >> patterns_count;
            std::cout << "Please, enter left and right border for pattern size: "; std::cin >> left_border >> right_border;
            std::cout << "Please, enter text size: "; std::cin >> text_size;

            patterns.reserve(patterns_count);

            for(std::size_t i = 0; i < patterns_count; ++i) {
                std::string pattern = gen.generate_string(left_border, right_border);
                patterns.push_back(pattern);
            }

            text = gen.generate_string(text_size);
        } else {
            text = read_str(std::cin);

            std::size_t patterns_count; std::cin >> patterns_count;
            patterns.reserve(patterns_count);

            for(std::size_t i = 0; i < patterns_count; ++i) {
                std::string tmp = read_str(std::cin);
                patterns.push_back(tmp);
            }
        }

        for(auto&& device : devices) {
            std::cout << "-------------------------------------------------------" << std::endl;            
            std::cout << "Current device: " << device.getInfo<CL_DEVICE_NAME>() << std::endl; 

            pm::cpu_pm_t cpu(patterns);
            auto cpu_result = cpu.match(text);
            auto cpu_time = cpu.get_time();
            std::cout << "CPU:      " << cpu_time << " mcs" << std::endl;

            pm::gpu_filter_t filter(device, patterns);
            auto gpu_result = filter.match(text);
            auto gpu_only_time = filter.gpu_only_time();
            auto gpu_full_time = filter.time();
            std::cout << "GPU only: " << gpu_only_time << " mcs" << std::endl;
            std::cout << "GPU full: " << gpu_full_time << " mcs" << std::endl;

            /* it's a bad test, because for big random patterns avarage shot count == 0 */
            if(cpu_result != gpu_result) {
                std::string err_str = "STRESS TEST: FAILED (cpu and gpu algorithms have different results)";
                std::cerr << err_str << std::endl;
            }    
        }    

    } catch(cl::Error& ex) {
        std::cerr << "Error: "<<  ex.what() << std::endl;
        return 1;
    }
    
    
    catch(std::exception& ex) {
        std::cerr << "Error: " << ex.what() << std::endl;
        return 1;
    }    

}    