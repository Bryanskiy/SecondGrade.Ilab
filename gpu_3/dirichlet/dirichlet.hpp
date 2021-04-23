#pragma once

#include <algorithm>

#include "../support/matrix/matrix.hpp"
#include "../cl_framework/cl_linalg.hpp"

struct point_t {
    float x_;
    float y_;
};

matrix::matrix_t<float> CPU_square_dirichlet_problem(float step, point_t upper_angle, point_t lower_angle, std::vector<float> border_values);
matrix::matrix_t<float> GPU_square_dirichlet_problem(float step, point_t upper_angle, point_t lower_angle, std::vector<float> border_values);