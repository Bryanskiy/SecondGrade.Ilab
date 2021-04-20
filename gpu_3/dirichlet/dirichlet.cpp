#include "dirichlet.hpp"

matrix::matrix_t<float> CPU_square_dirichlet_problem(float step, point_t upper_angle, point_t lower_angle, std::vector<float> border_values) {
    float x_len = std::abs(lower_angle.x_ - upper_angle.x_);
    float y_len = std::abs(lower_angle.y_ - upper_angle.y_);
    if(x_len != y_len) {
        throw std::runtime_error("invalid borders: area must be a square");
    }

    std::size_t grid_size = static_cast<float>(x_len) / step + 1;
    std::size_t matrix_size = grid_size - 2;
    std::size_t system_size = std::pow(matrix_size, 2);
    matrix::matrix_t<float> system(system_size, system_size);

    for(std::size_t i = 0; i < system_size; ++i) {
        for(std::size_t j = 0; j < system_size; ++j) {
            if(i == j) {
                system[i][j] = -4.0;

                if(i == matrix_size - 1) {
                    continue;
                }

                if(i % (matrix_size) != matrix_size - 1) {
                    system[i + 1][j] = 1.0;
                    system[i][j + 1] = 1.0;
                }

            } else if((i + matrix_size) == j) {
                system[i][j] = 1.0;
            } else if((j + matrix_size) == i) {
                system[i][j] = 1.0;
            }
        }
    }

    matrix::matrix_t<float> right(system_size, 1);
    for(std::size_t i = 0, maxi = matrix_size; i < maxi; ++i) {
        right[i][0] += border_values[i + matrix_size * 4];
        right[matrix_size * (i + 1) - 1][0] += border_values[i + 1];
        right[matrix_size * 4 - i - 1][0] += border_values[matrix_size + i + 2];
        right[i * matrix_size][0] += border_values[matrix_size * 4 - 2 - i];
    }

    auto&& ans = matrix::solve_linear_system(system, right);
    if(ans.second.get_elements_number()) {
        throw std::runtime_error("CPU_square_dirichlet_problem: can't solve linear system");
    }

    matrix::matrix_t<float> ret(grid_size, grid_size);

    int k = 0;
    for(int j = 1; j < grid_size - 1; ++j) {
        for(int i = matrix_size; i > 0; --i) {
            ret[i][j] = -ans.first[k][0];
            ++k;
        }
    }

    for(std::size_t i = 0; i < grid_size - 1; ++i) {
        ret[0][i] = border_values[i];
        ret[grid_size - 1 - i][0] = border_values[i + (grid_size - 1) * 3];
        ret[grid_size - 1][grid_size - 1 - i] = border_values[i + (grid_size - 1) * 2];
        ret[i][grid_size - 1] = border_values[i + grid_size - 1];  
    }

    return ret;
}