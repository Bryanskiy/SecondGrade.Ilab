#include "dirichlet.hpp"

namespace {

matrix::matrix_t<float> create_system(std::size_t matrix_size) {

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

    return system;
}

std::vector<float> create_right(std::size_t matrix_size, const std::vector<float>& border_values) {
    std::size_t system_size = std::pow(matrix_size, 2);

    std::vector<float> right(system_size);
    for(std::size_t i = 0, maxi = matrix_size; i < maxi; ++i) {
        right[i] += border_values[i + matrix_size * 3 + 4];
        right[matrix_size * (i + 1) - 1] += border_values[i + 1];
        right[matrix_size * matrix_size - i - 1] += border_values[matrix_size + i + 2];
        right[i * matrix_size] += border_values[matrix_size * 3 + 2 - i];
    }

    return right;
}

matrix::matrix_t<float> write_answer(std::size_t grid_size, const std::vector<float>& right, const std::vector<float>& border_values) {
    std::size_t matrix_size = grid_size - 2;
    matrix::matrix_t<float> ret(grid_size, grid_size);
    int k = 0;
    for(int j = 1; j < grid_size - 1; ++j) {
        for(int i = matrix_size; i > 0; --i) {
            ret[i][j] = -right[k];
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

} /* namespace */

matrix::matrix_t<float> CPU_square_dirichlet_problem(float step, point_t upper_angle, point_t lower_angle, const std::vector<float>& border_values) {
    float x_len = std::abs(lower_angle.x_ - upper_angle.x_);
    float y_len = std::abs(lower_angle.y_ - upper_angle.y_);
    if(std::abs(x_len - y_len) > clf::accuracy) {
        throw std::runtime_error("invalid borders: area must be a square");
    }

    std::size_t grid_size = static_cast<float>(x_len) / step + 1;
    std::size_t matrix_size = grid_size - 2;
    std::size_t system_size = std::pow(matrix_size, 2);

    matrix::matrix_t<float> system = create_system(matrix_size);
    auto&& right = create_right(matrix_size, border_values);

    auto&& ans = matrix::solve_linear_system(system, right);
    if(ans.second.get_elements_number()) {
        throw std::runtime_error("CPU_square_dirichlet_problem: can't solve linear system");
    }

    return write_answer(grid_size, ans.first, border_values);
}

matrix::matrix_t<float> GPU_square_dirichlet_problem(float step, point_t upper_angle, point_t lower_angle, const std::vector<float>& border_values) {
    float x_len = std::abs(lower_angle.x_ - upper_angle.x_);
    float y_len = std::abs(lower_angle.y_ - upper_angle.y_);
    if(std::abs(x_len - y_len) > clf::accuracy) {
        throw std::runtime_error("invalid borders: area must be a square");
    }

    std::size_t grid_size = static_cast<float>(x_len) / step + 1;
    std::size_t matrix_size = grid_size - 2;
    std::size_t system_size = std::pow(matrix_size, 2);

    matrix::matrix_t<float> system = create_system(matrix_size);
    clf::cl_bandet_sparce_fmatrix_t A(system);
    clf::cl_fvector_t b(create_right(matrix_size, border_values));

    clf::cl_fvector_t x(system_size);
    clf::cl_fvector_t r = b - A * x;
    clf::cl_fvector_t p = r;

    while(r.scalar_mult(r) > clf::accuracy) {
        clf::cl_fvector_t tmp = A * p;

        float alpha = r.scalar_mult(r) / tmp.scalar_mult(p);
        x += (p * alpha);
        clf::cl_fvector_t next_r = r - alpha * tmp;
        float betta = next_r.scalar_mult(next_r) / r.scalar_mult(r);
        p = next_r + betta * p;
        r = next_r;
    }

    return write_answer(grid_size, x.unpack(), border_values);
}