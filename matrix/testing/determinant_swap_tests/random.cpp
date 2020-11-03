#include "../../matrix.h"
#include <random>
#include <iostream>

matrix::matrix_t<double> generate_random_square_matrix(std::size_t size) {
    matrix::matrix_t<double> ret(size, size);

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(-5, 5);

    for(std::size_t i = 0; i < size; ++i) {
        for(std::size_t j = 0; j < size; ++j) {
            ret[i][j] = dis(gen);
        }
    }

    return ret;
}

void test1() {
    std::size_t size = 10;
    matrix::matrix_t<double> m = generate_random_square_matrix(size);
    double det = matrix::det(m);

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, size - 1);

    for(std::size_t i = 0; i < 100; ++i) {
        int lhs = dis(rd);
        int rhs = dis(rd);
        m.swap_rows(lhs, rhs);

        double new_det = matrix::det(m);
        if(lhs != rhs) {
            det *= -1;
        }

        if(std::abs(new_det - det) >= 1e-5) {
            std::cout << "Test failed" << std::endl;
            return;
        }
    }

    std::cout << "Test succeed" << std::endl;
}

int main() {
    test1();
}