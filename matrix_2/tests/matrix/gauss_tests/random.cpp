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
    std::uniform_int_distribution<> int_dis(10, 50);

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
        if(matrix::equal(m[current_m][current_n], 0.0)) {
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

void case_run(const char* file_name, const std::pair<matrix::matrix_t<long double>, matrix::matrix_t<long double>>& solution) {
    std::ifstream in(file_name, std::ios::in);
    if(!in.good()) {
        std::cerr << "Can't open test file: " << file_name << std::endl;
        return;
    }

    std::size_t m , n;
    in >> m >> n;
    matrix::matrix_t<long double> mtx(m, n);
    in >> mtx;

    matrix::matrix_t<long double> coefs(m, 1);
    in >> coefs;
    auto ret = matrix::solve_linear_system(mtx, coefs);

    if((solution.first != ret.first) && (solution.second != ret.second)) {
        std::cerr << file_name << " test failed " << std::endl;
        return;
    }

    std::cout << file_name << " test SUCCESS" << std::endl;
} 

void cases_runner() {
    {
         /* joint system, the solution is unique*/
         char name[] = "tests/matrix/gauss_tests/cases/case_0.txt";
         matrix::matrix_t<long double> partly_solution = {{2}, 
                                                          {-1}, 
                                                          {1} };
         matrix::matrix_t<long double> fundamental_matrix;
         case_run(name, {partly_solution, fundamental_matrix});
     }

    {
        /* joint, homogeneous system m * n */
        char name[] = "tests/matrix/gauss_tests/cases/case_1.txt";
        matrix::matrix_t<long double> partly_solution = {{0}, 
                                                         {0}, 
                                                         {0},
                                                         {0},
                                                         {0}};
        matrix::matrix_t<long double> fundamental_matrix = { {-0.5, 0 },
                                                             {-0.5, -1},
                                                             {0   , 0 },
                                                             {1   , 0 },
                                                             {0   , 1 } };
        case_run(name, {partly_solution, fundamental_matrix}); 
    }
}

int main() {
    for(std::size_t i = 0; i < 100; ++i) {
         matrix::matrix_t<long double> m = generate_random_matrix();
         bool flag = check_gauss(m);
         if(!flag) {
             std::cerr << "Gauss test failed for: " << std::endl;
             std::cerr << m << std::endl;
             return 1;
         }
     }

    std::cout << "Gauss test: SUCCESS" << std::endl;

    cases_runner();
}