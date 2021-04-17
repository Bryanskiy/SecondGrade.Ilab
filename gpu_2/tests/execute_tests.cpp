#include <boost/program_options.hpp>

#include "../gpu_matching/filter.hpp"
#include "../gpu_matching/cl_general.hpp"
#include "../cpu_matching/cpu_pm.hpp"
#include "generator.h"

void test1(cl::Device device) {
    std::cout << "----------------------------TEST1----------------------------------------" << std::endl;
    std::cout << "Current device: " << device.getInfo<CL_DEVICE_NAME>() << std::endl;     
    std::size_t iterations_count = 91234;

    std::string text;
    std::string pattern = "abssdlssdf";
    for(std::size_t i = 0; i < iterations_count; ++i) {
        text += pattern;
    }

    pm::cpu_pm_t cpu({pattern});
    pm::gpu_filter_t filter(device, {pattern});

    auto cpu_result = cpu.match(text);
    auto gpu_result = filter.match(text);

    bool correct = true;

    if(cpu_result[0].size() != iterations_count) {
        std::cout << "CPU algirithm failed - expected " << iterations_count << " but actual " << cpu_result[0].size() << std::endl;
        correct = false;
    }

    if(gpu_result[0].size() != iterations_count) {
         std::cout << "GPU algirithm failed - expected " << iterations_count << " but actual " << gpu_result[0].size() << std::endl;
        correct = false;
    }

    if(correct) {
        std::cout << "SUCCESS" << std::endl; 
    }
}

void test2(cl::Device device) {
    std::cout << "----------------------------TEST2----------------------------------------" << std::endl;
    std::cout << "Current device: " << device.getInfo<CL_DEVICE_NAME>() << std::endl; 
    std::size_t iterations_count = 16093;

    std::vector<std::string> patterns({"qwertyuiop", "iopasdfghj", "hjklzxcvm", "pasdfghjklzxcvm"});
    std::string text;
    for(std::size_t i = 0; i < iterations_count; ++i) {
        text += "qwertyuiopasdfghjklzxcvm";
    }

    pm::cpu_pm_t cpu(patterns);
    pm::gpu_filter_t filter(device, patterns);

    auto cpu_result = cpu.match(text);
    auto gpu_result = filter.match(text);

    bool correct = true;

    for(std::size_t i = 0; i < patterns.size(); ++i) {

        if(cpu_result[i].size() != iterations_count) {
            std::cout << "CPU algirithm failed on "<< i <<  " pattern - expected " << iterations_count << " but actual " << cpu_result[i].size() << std::endl;
            correct = false;
        }

        if(gpu_result[i].size() != iterations_count) {
            std::cout << "GPU algirithm failed on "<< i <<  " pattern - expected " << iterations_count << " but actual " << gpu_result[i].size() << std::endl;
            correct = false;
        }
    }

    if(correct) {
        std::cout << "SUCCESS" << std::endl; 
    }
}

void test3(cl::Device device) {
    std::cout << "----------------------------TEST2----------------------------------------" << std::endl;
    std::cout << "Current device: " << device.getInfo<CL_DEVICE_NAME>() << std::endl; 
    std::size_t iterations_count = 425;

    std::string pattern = "aabbcck";

    std::string text;

    std::vector<std::string> patterns;
    for(std::size_t i = 0; i < iterations_count; ++i) {
        patterns.push_back(pattern);
        text += pattern;
    }


    pm::cpu_pm_t cpu(patterns);
    pm::gpu_filter_t filter(device, patterns);

    auto cpu_result = cpu.match(text);
    auto gpu_result = filter.match(text);

    bool correct = true;
    if(cpu_result[0].size() != iterations_count) {
        std::cout << "CPU algirithm failed - expected " << iterations_count << " but actual " << cpu_result[0].size() << std::endl;
        correct = false;
    }

    if(gpu_result[0].size() != iterations_count) {
         std::cout << "GPU algirithm failed - expected " << iterations_count << " but actual " << gpu_result[0].size() << std::endl;
        correct = false;
    }

    if(correct) {
        std::cout << "SUCCESS" << std::endl; 
    }
}

int main() {
    try {

        std::vector<pm::machine_t> info = pm::get_devices();

        for(auto&& machine : info) {
            test1(machine.second);
            test2(machine.second);
            test3(machine.second);
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