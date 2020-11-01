#include "../../matrix.h"
#include <random>
#include <iostream>

matrix::matrix_t<int> generate_random_square_matrix(std::size_t size) {
    matrix::matrix_t<int> ret(size, size);

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(-100, 100);

    for(std::size_t i = 0; i < size; ++i) {
        for(std::size_t j = 0; j < size; ++j) {
            ret[i][j] = dis(gen);
        }
    }

    return ret;
}

void test1() {
    std::size_t size = 100;
    matrix::matrix_t<int> m = generate_random_square_matrix(size);
    int det = m.det();

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, size);

    for(std::size_t i = 0; i < 100; ++i) {
        int lhs = dis(rd);
        int rhs = dis(rd);

        m.swap_rows(lhs, rhs);
        int new_det = m.det();
        det *= -1;

        if(new_det != det) {
            std::cout << "Test failed" << std::endl;
            break;
        }
    }
}

int main() {
    test1();
}