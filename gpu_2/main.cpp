#include <iostream>
#include <boost/program_options.hpp>

#include "cpu_matching/cpu_pm.hpp"
#include "gpu_matching/gpu_kmp.hpp"
#include "support/support.hpp"

std::vector<unsigned> main_process(cl::Device& device, std::istream& in, std::ostream& out) {
    std::string text = sup::read_str(std::cin);
    std::vector<std::string> patterns;
    std::size_t patterns_count; std::cin >> patterns_count;
    patterns.reserve(patterns_count);

    for(std::size_t i = 0; i < patterns_count; ++i) {
        std::string tmp = sup::read_str(std::cin);
        patterns.push_back(tmp);
    }

    pm::gpu_kmp_t pm(device, text, patterns);
    return pm.match();
}

int main(int argc, char** argv) {
/* -------------PROCESS MAIN ARGS ------------------------- */
    std::vector<std::pair<cl::Platform, cl::Device>> info;

    try {

        boost::program_options::options_description desc("Options");
        desc.add_options()
            ("help", "get app info")
            ("devices", "get avaible devices")
            ("set", boost::program_options::value<int>(), "set device");

        boost::program_options::variables_map vm;        
        boost::program_options::store(boost::program_options::parse_command_line(argc, argv, desc), vm);
        boost::program_options::notify(vm);

        if (vm.count("help")) {
            std::cout << "This is program to find patterns in a text" << std::endl;
            std::cout << desc;
            return 0;
        }

        info = sup::get_devices();

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
        cl::Device device;
        if(vm.count("set")) {
            id = vm["set"].as<int>();

            if(id >= info.size()) {
                std::cerr << "invalid id" << std::endl;
                return 0;
            }

            device = info[id].second;
        } else {
            device = sup::choose_default_device(info);   
        }

#ifdef LOG
    sup::dump_devices(info, sup::log.log_file);
    sup::log.separate();
    sup::log.log_file << "Chosen device: " << device.getInfo<CL_DEVICE_NAME>() << std::endl;
#endif 

/* --------------MAIN PROGRAM ----------------------------- */

    
        auto&& res = main_process(device, std::cin, std::cout);

        for(std::size_t i = 0, maxi = res.size(); i < maxi; ++i) {
            std::cout << i + 1 << " " << res[i] << std::endl;
        }

    } catch(cl::Error& err) {
        std::cerr << err.what() << std::endl; 
        std::cerr << sup::cl_get_error_string(err.err()) << std::endl;
    }
    
    catch(std::exception& err) {
        std::cerr << err.what() << std::endl; 
    }
}