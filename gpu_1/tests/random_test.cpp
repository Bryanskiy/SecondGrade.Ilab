#include "../bsort.h"

#include <random>
#include <chrono>
#include <algorithm>

std::random_device rd;
std::mt19937 gen(rd());

std::vector<int> generate_random_array(std::size_t n) {
    std::uniform_int_distribution<> dis(-100, 100);

    std::vector<int> ret(n);

    for(std::size_t i = 0; i < n; ++i) {
        ret[i] = dis(gen);
    }

    return ret;
}

void check_mono(std::vector<int>& data) {
    for(std::size_t i = 1; i < data.size(); ++i) {
        if(data[i] < data[i-1]) {
            std::cout << "TEST FAILED" << std::endl;
            return;
        }
    }

    std::cout << "TEST success" << std::endl;    
}

void run_test(std::size_t n) {
    bsort::bsort_t app;

    std::vector<int> arr_gpu = generate_random_array(n);
    std::vector<int> arr_classic  = arr_gpu;

    std::cout << "-----------------------------------------------------------" << std::endl;
    std::cout << "Number of elements:                    " << n << std::endl;

    Timer_t classic_timer;
    std::sort(arr_classic.begin(), arr_classic.end());
    std::cout << "Classic CPU:                           " << classic_timer.get_time().count() << " mcs" << std::endl;
 
    app.run(arr_gpu, bsort::direction_t::increasing);
    std::cout << "Bitonic CPU_full / CPU_power_2 / GPU:  " << app.get_full_time() << " / " << app.get_power_2_time() << " / " << app.get_gpu_time() << " mcs" << std::endl;

    check_mono(arr_gpu);
}

int main() {
    try {
        for(std::size_t i = 50000; i < 1000000; i += 20000) {
            run_test(i);
        }   
    } catch(cl::Error e) {
        std::cerr << e.what() << std::endl;
        std::cerr << cl_get_error_string(e.err()) << std::endl;
    }
}