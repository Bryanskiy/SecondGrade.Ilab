#include "../bsort.h"

#include <random>

std::random_device rd;
std::mt19937 gen(rd());

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

    std::cout << "TEST SUCCEED" << std::endl;    
}

void run_test(std::size_t n) {
    bsort::bsort_t app;
    int* arr = generate_random_array(n);
    app.run(arr, n, bsort::direction_t::increasing);
    check_mono(arr, n);
    delete[] arr;
}

int main() {
    run_test(101);
    run_test(1003);
    run_test(100007);
    run_test(1000011);
}