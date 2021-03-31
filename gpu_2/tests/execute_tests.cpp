#include <boost/program_options.hpp>

#include "../gpu_matching/gpu_kmp.hpp"
#include "../cpu_matching/cpu_pm.hpp"
#include "../support/support.hpp"

void test1(cl::Device device) {
    std::size_t iterations_count = 10000;

    std::string text;
    std::string pattern = "abs";
    for(std::size_t i = 0; i < iterations_count; ++i) {
        text += pattern;
    }

    pm::cpu_pm_t cpu(text, {pattern});
    pm::gpu_kmp_t kmp(device, text, {pattern});

    auto cpu_result = cpu.match();
    auto gpu_result = kmp.match();

    bool correct = true;

    if(cpu_result[0] != iterations_count) {
        std::cout << "test1(): CPU algirithm failed - expected " << iterations_count << " but actual " << cpu_result[0] << std::endl;
        correct = false;
    }

    if(gpu_result[0] != iterations_count) {
         std::cout << "test1(): GPU algirithm failed - expected " << iterations_count << " but actual " << gpu_result[0] << std::endl;
        correct = false;
    }

    if(correct) {
        std::cout << "test1(): SUCCESS" << std::endl; 
    }
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
            std::cout << "Execute pattern matching time tests." << std::endl;
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

        if(vm.count("set")) {
            id = vm["set"].as<int>();
        }

        cl::Device device = info[id].second;
        test1(device);

    } catch(cl::Error& ex) {
        std::cerr << "Error: "<<  ex.what() << std::endl;
        return 1;
    }   

    catch(std::exception& ex) {
        std::cerr << "Error: " << ex.what() << std::endl;
        return 1;
    }   
}

