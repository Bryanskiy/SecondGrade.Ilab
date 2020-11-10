#include "../../matrix.h"
#include <random>
#include <iostream>
#include <cmath>
#include <fstream>

std::random_device rd;
std::mt19937 gen(rd());

matrix::matrix_t<double> generate_random_square_matrix(std::size_t size) {
#ifdef DEBUG_
    debug::print_block_separator();
    debug::log_file << "start generate random square matrix" << std::endl;
#endif
#ifdef DEBUG_
    debug::increment_indent();
#endif
    matrix::matrix_t<double> ret(size, size);
#ifdef DEBUG_
    debug::decrement_indent();
#endif

    std::uniform_real_distribution<> dis(-5, 5);

    for(std::size_t i = 0; i < size; ++i) {
        for(std::size_t j = 0; j < size; ++j) {
            ret[i][j] = dis(gen);
        }
    }
#ifdef DEBUG_
    debug::log_file << "generated matrix: " << std::endl;
    debug::log_file << ret;
    debug::log_file << "end generate random square matrix" << std::endl;
    debug::print_block_separator();
#endif
    return ret;
}

/* this func can change determinant sign */
template<typename T>
void swap_rows_and_cols(matrix::matrix_t<T>& m) {
    std::size_t max_n = m.get_col_number();
    std::size_t max_m = m.get_row_number();

    std::uniform_int_distribution<> dis_cols(0, max_n - 1);
    std::uniform_int_distribution<> dis_rows(0, max_m - 1);

    for(std::size_t i = 0; i < 100; ++i) {
        std::size_t row_1 = dis_rows(gen);
        std::size_t row_2 = dis_rows(gen);

        std::size_t col_1 = dis_cols(gen);
        std::size_t col_2 = dis_cols(gen);

        m.swap_rows(row_1, row_2);
        m.swap_cols(col_1, col_2);
    }
}

/* generate matrix with det == +-1 */
matrix::matrix_t<double> generate_orthogonal_matrix(std::size_t size) {
#ifdef DEBUG_
    debug::print_block_separator();
    debug::log_file << "start generate orthogonal matrix" << std::endl;
#endif
#ifdef DEBUG_
    debug::increment_indent();
#endif
    matrix::matrix_t<double> ret(size, size, 0.0);
#ifdef DEBUG_
    debug::decrement_indent();
#endif
    std::uniform_real_distribution<> dis(0, 3.14/2);

    for(std::size_t i = 0; i < size - 1; i += 2) {
        double angle = dis(gen);
        double sin = std::sin(angle);
        double cos = std::cos(angle);

        ret[i][i] = cos;
        ret[i + 1][i + 1] = cos;
        ret[i][i + 1] = -sin;
        ret[i + 1][i] = sin;
    }

    if((size % 2) == 1) {
        ret[size-1][size-1] = 1;
    }
#ifdef DEBUG_
    debug::log_file << "generated matrix: " << std::endl;
    debug::log_file << ret;
    debug::log_file << "end generate orthogonal matrix" << std::endl;
    debug::print_block_separator();
#endif
    return ret;
}

void test_random_1(std::size_t matrix_size) {
    for(std::size_t i = 0; i < 100; ++i) {
        matrix::matrix_t<double> m = generate_random_square_matrix(matrix_size);
        matrix::matrix_t<double> orthogonal = generate_orthogonal_matrix(matrix_size);

        if(std::abs(std::abs(orthogonal.det()) - 1) > matrix::tolerance) {
            std::cerr << "Incorrect orthogonal matrix" << std::endl;
            return;
        }

        double first_det = std::abs(m.det());

        matrix::matrix_t<double> mult = matrix::multiplication(m, orthogonal);
        swap_rows_and_cols(mult);
        double second_det = std::abs(mult.det());

        if(std::abs(first_det - second_det) > matrix::tolerance) {
            std::cerr << "test_random_1 for size: " << matrix_size << " failed" << std::endl;
            return;
        }

    }

    std::cout << "test_random_1 for size: " << matrix_size << " success" << std::endl;
}

template<typename T>
void test_random_2(char* name, T ans) {
    std::ifstream in(name, std::ios::in);
    if(!in.good()) {
        std::cerr << "Can't open test file: " << name << std::endl;
        return;
    }

    std::size_t matrix_size; in >> matrix_size;
    matrix::matrix_t<double> m(matrix_size, matrix_size); in >> m;
    double actual_det = std::abs(m.det());

    if(std::abs(actual_det - ans) > matrix::tolerance) {
        std::cerr << "test_random_2 failed for matrix: " << std::endl;
        std::cerr << m;
        return;
    }

    for(std::size_t i = 0; i < 10; ++i) {
        matrix::matrix_t<double> orthogonal = generate_orthogonal_matrix(m.get_row_number());

        if(std::abs(std::abs(orthogonal.det()) - 1) > matrix::tolerance) {
            std::cerr << "Incorrect orthogonal matrix" << std::endl;
            return;
        }

        matrix::matrix_t<double> mult = matrix::multiplication(m, orthogonal);
        swap_rows_and_cols(mult);
        double new_det = std::abs(mult.det());

        if(std::abs(actual_det - new_det) > matrix::tolerance) {
            std::cerr << "test_random_2 failed for matrix: " << std::endl;
            std::cerr << m;
            return;
        }
    }
    in.close();
    std::cout << name << " test success" << std::endl;
}

void test_random_2_runner() {
    {
        char name[] = "testing/determinant_tests/cases/case_0.txt";
        test_random_2(name, 42);
    }

    {
        char name[] = "testing/determinant_tests/cases/case_1.txt";
        test_random_2(name, 126);
    }

    {
        char name[] = "testing/determinant_tests/cases/case_2.txt";
        test_random_2(name, 42);
    }

    {
        char name[] = "testing/determinant_tests/cases/case_3.txt";
        test_random_2(name, 1);
    }

    {
        char name[] = "testing/determinant_tests/cases/case_4.txt";
        test_random_2(name, 42);
    }
}

int main() {
    test_random_1(7); /* odd size */
    test_random_1(8); /* even size */
    test_random_2_runner();
}