#include "dirichlet.hpp"

matrix::matrix_t<float> dirichlet_solver_t::create_system(std::size_t matrix_size) {

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

            } else if(((i + matrix_size) == j) || ((j + matrix_size) == i)) {
                system[i][j] = 1.0;
            }
        }
    }

    return system;
}

std::vector<float> dirichlet_solver_t::create_right(std::size_t matrix_size) {
    std::size_t system_size = std::pow(matrix_size, 2);

    std::vector<float> right(system_size);
    for(std::size_t i = 0, maxi = matrix_size; i < maxi; ++i) {
        right[i] += border_values_[i + matrix_size * 3 + 4];
        right[matrix_size * (i + 1) - 1] += border_values_[i + 1];
        right[matrix_size * matrix_size - i - 1] += border_values_[matrix_size + i + 2];
        right[i * matrix_size] += border_values_[matrix_size * 3 + 2 - i];
    }

    return right;
}

matrix::matrix_t<float> dirichlet_solver_t::write_answer(std::size_t matrix_size, const std::vector<float>& right) {
    matrix::matrix_t<float> ret(matrix_size, matrix_size);
    int k = 0;
    for(int j = 0; j < matrix_size; ++j) {
        for(int i = matrix_size - 1; i >= 0; --i) {
            ret[i][j] = -right[k];
            ++k;
        }
    }

    return ret;
} 
void dirichlet_solver_t::init(point_t upper_angle, point_t lower_angle, const std::vector<float>& border_values) {
    upper_angle_ = upper_angle;
    lower_angle_ = lower_angle;
    border_values_ = border_values;
}

matrix::matrix_t<float> dirichlet_solver_t::solve(execution_mode mode /* = GPU */) {
    switch (mode) {
        case GPU:
            return solve_GPU();

        case CPU:
            return solve_CPU();
    }

    return {};
}

matrix::matrix_t<float> dirichlet_solver_t::solve_CPU() {
    float len = std::abs(lower_angle_.x - upper_angle_.x);
    float step = len / border_values_.size() * 4.0;
    std::size_t grid_size = len / step + 1;
    std::size_t matrix_size = grid_size - 2;
    std::size_t system_size = std::pow(matrix_size, 2);

    matrix::matrix_t<float> system = create_system(matrix_size);
    auto&& right = create_right(matrix_size);

    auto&& ans = matrix::solve_linear_system(system, right);
    if(ans.second.get_elements_number()) {
        throw std::runtime_error("CPU_square_dirichlet_problem: can't solve linear system");
    }

    return write_answer(matrix_size, ans.first);
}

matrix::matrix_t<float> dirichlet_solver_t::solve_GPU() {
    float len = std::abs(lower_angle_.x - upper_angle_.x);
    float step = len / border_values_.size() * 4.0;
    std::size_t grid_size = len / step + 1;
    std::size_t matrix_size = grid_size - 2;
    std::size_t system_size = std::pow(matrix_size, 2);

    matrix::matrix_t<float> system = create_system(matrix_size);
    clf::cl_bandet_sparce_fmatrix_t A(system);
    clf::cl_fvector_t b(create_right(matrix_size));

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

    return write_answer(matrix_size, x.unpack());
}