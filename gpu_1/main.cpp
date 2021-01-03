#include "bsort.h"

int main() {
    /* init data */
    std::size_t N; std::cin >> N;
    int* data = new int[N];
    for(std::size_t i = 0; i < N; ++i) {
        std::cin >> data[i];
    }

    /* run sort */
    bsort::bsort_t app;
    try {
        app.run(data, N, bsort::direction_t::increasing);

        /* write ans */
        for(std::size_t i = 0; i < N; ++i) {
            std::cout << data[i] << ' ';
    }

    } catch(cl::Error e) {
        std::cerr << e.what() << " ERRROR CODE: " << e.err() << std::endl;
    } catch(std::runtime_error e) {
        std::cerr << e.what() << std::endl;
    }

    delete[] data;
}