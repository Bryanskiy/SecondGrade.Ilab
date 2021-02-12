/* ---------------------------------------------------------------------------
This test check only gauss algorithm invariant:
    1) after straight pass matrix transports to step matrix
    2) after reverse pass all elements in the same col before step equal to 0 
-----------------------------------------------------------------------------*/

#include "../../../matrix/matrix.hpp"
#include <random>
#include <iostream>
#include <cmath>
#include <fstream>

std::random_device rd;
std::mt19937 gen(rd());

matrix::matrix_t<long double> generate_random_matrix() {
    std::uniform_real_distribution<> double_dis(-5, 5);
    std::uniform_int_distribution<> int_dis(20, 100);

    std::size_t m = int_dis(gen);
    std::size_t n = int_dis(gen);

    matrix::matrix_t <long double> ret(m, n);

    for(std::size_t i = 0; i < m; ++i) {
        for(std::size_t j = 0; j < n; ++j) {
            ret[i][j] = double_dis(gen);
        }
    }

    return ret;
}

bool check_gauss(matrix::matrix_t<long double>& m) {
    matrix::matrix_t<long double> copy(m);

    matrix::gauss_straight(m);
    matrix::gauss_reverse(m);

    std::size_t current_m = 0;
    std::size_t current_n = 0;
    std::size_t max_m = m.get_rows_number();
    std::size_t max_n = m.get_cols_number();

    while((current_m < max_m) && (current_n < max_n)) {
        while(matrix::equal(m[current_m][current_n], 0.0)) {
            ++current_n;
            continue;
        }

        for(std::size_t i = 0; i < max_m; ++i) {
            if(i == current_m) {
                continue;
            }

            if(!matrix::equal(m[i][current_n], 0.0)) {
                return false;
            }
        }

        ++current_n;
        ++current_m;
    }

    return true;
}

int main() {
    for(std::size_t i = 0; i < 1000; ++i) {
        matrix::matrix_t<long double> m = generate_random_matrix();
        bool flag = check_gauss(m);
        if(!flag) {
            std::cerr << "Gauss test failed for: " << std::endl;
            std::cerr << m << std::endl;
            return 1;
        }
    }

    std::cout << "Gauss test: SUCCESS" << std::endl;
}