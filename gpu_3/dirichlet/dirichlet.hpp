#pragma once

#include <algorithm>

#include "../support/matrix/matrix.hpp"
#include "../cl_framework/cl_linalg.hpp"

class dirichlet_solver_t {
public:

    enum execution_mode {
        CPU,
        GPU
    };

    struct point_t {
        float x;
        float y;    
    };  

    void init(point_t upper_angle, point_t lower_angle, const std::vector<float>& border_values);
    matrix::matrix_t<float> solve(execution_mode mode = GPU);

private:

    matrix::matrix_t<float> solve_GPU();
    matrix::matrix_t<float> solve_CPU();

    matrix::matrix_t<float> create_system(std::size_t matrix_size);

    std::vector<float> create_right(std::size_t matrix_size); 
    matrix::matrix_t<float> write_answer(std::size_t matrix_size, const std::vector<float>& right);

private:

    point_t upper_angle_;
    point_t lower_angle_;
    std::vector<float> border_values_;
};