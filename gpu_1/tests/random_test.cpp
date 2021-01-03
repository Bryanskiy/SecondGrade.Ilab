#include "../bsort.h"

#include <random>
#include <chrono>
#include <algorithm>

std::random_device rd;
std::mt19937 gen(rd());

class Timer_t {
public:
    using clock_t = std::chrono::high_resolution_clock;
    using milliseconds_t = std::chrono::milliseconds;

    Timer_t() : start_(clock_t::now()) {}
    milliseconds_t get_time() {
        return std::chrono::duration_cast<milliseconds_t>(clock_t::now() - start_);
    }
    void reset() {
        start_ = clock_t::now();
    }
private:
    std::chrono::time_point<clock_t> start_;
};

int* generate_random_array(std::size_t n) {
    std::uniform_int_distribution<> dis(std::numeric_limits<int>::min(), std::numeric_limits<int>::max());

    int* ret = new int[n];

    for(std::size_t i = 0; i < n; ++i) {
        ret[i] = dis(gen);
    }

    return ret;
}

void check_mono(int* data, std::size_t n) {
    for(std::size_t i = 1; i < n; ++i) {
        if(data[i] < data[i-1]) {
            std::cout << "TEST FAILED" << std::endl;
            return;
        }
    }

    std::cout << "TEST success" << std::endl;    
}

void run_test(std::size_t n) {
    bsort::bsort_t app;

    int* arr_gpu = generate_random_array(n);

    std::vector<int> arr_classic(n);
    for(std::size_t i = 0; i < n; ++i) {
        arr_classic[i] = arr_gpu[i];
    }

    std::cout << "------------------------------------------------" << std::endl;
    std::cout << "Number of elements: " << n << std::endl;

    Timer_t classic_timer;
    std::sort(arr_classic.begin(), arr_classic.end());
    std::cout << "Classic time: " << classic_timer.get_time().count() << std::endl;

    Timer_t gpu_timer;
    app.run(arr_gpu, n, bsort::direction_t::increasing);
    std::cout << "GPU time: " << gpu_timer.get_time().count() << std::endl;

    check_mono(arr_gpu, n);
    delete[] arr_gpu;
}

int main() {
    run_test(101);
    run_test(1003);
    run_test(100007);
    run_test(1000011);
    run_test(10000013);
}