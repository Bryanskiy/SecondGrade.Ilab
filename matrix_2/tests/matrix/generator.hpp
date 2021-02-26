#pragma once 

#include <random>
#include "../../matrix/matrix.hpp"

class generator_t {
public: 

    generator_t() : rd_(), gen_(rd_()) {}

    matrix::matrix_t<double> generate_matrix(std::size_t m, std::size_t n, double left_border, double right_border) {
        std::uniform_real_distribution<> double_dis(-left_border, right_border);
        matrix::matrix_t <double> ret(m, n);
        for(std::size_t i = 0; i < m; ++i) {
            for(std::size_t j = 0; j < n; ++j) {
                ret[i][j] = double_dis(gen_);
            }
        }

        return ret;
    }

    void random_shuffle(matrix::matrix_t<double>& m, std::size_t seed) {
        std::size_t max_n = m.get_cols_number();
        std::size_t max_m = m.get_rows_number();

        std::uniform_int_distribution<> dis_cols(0, max_n - 1);
        std::uniform_int_distribution<> dis_rows(0, max_m - 1);

        for(std::size_t i = 0; i < seed; ++i) {
            std::size_t row_1 = dis_rows(gen_);
            std::size_t row_2 = dis_rows(gen_);

            std::size_t col_1 = dis_cols(gen_);
            std::size_t col_2 = dis_cols(gen_);

            m.swap_rows(row_1, row_2);
            m.swap_cols(col_1, col_2);
        }
    }

    matrix::matrix_t<double> generate_orthogonal_matrix(std::size_t size) {
        matrix::matrix_t<double> ret(size, size, 0.0);

        std::uniform_real_distribution<> dis(0.0, 3.14/2.0);

        for(std::size_t i = 0; i < size - 1; i += 2) {
            double angle = dis(gen_);
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

        return ret;
    }
private:
    std::random_device rd_;
    std::mt19937 gen_;
};