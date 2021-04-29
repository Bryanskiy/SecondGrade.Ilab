#include <iostream>
#include <vector>

#include "dirichlet/dirichlet.hpp"
#include "cl_framework/cl_linalg.hpp"

int main() {
    float step; std::cin >> step;
    dirichlet_solver_t::point_t upper_angle; std::cin >> upper_angle.x >> upper_angle.y;
    dirichlet_solver_t::point_t lower_angle; std::cin >> lower_angle.x >> lower_angle.y;
    
    float x_len = std::abs(lower_angle.x - upper_angle.x);
    float y_len = std::abs(lower_angle.y - upper_angle.y);

    if(std::abs(x_len - y_len) > clf::accuracy) {
        std::cerr << "border isn't a square";
        return 1;
    }

    std::size_t values_count = 4 * x_len / step;

    std::vector<float> board_values;
    board_values.reserve(values_count);

    for(std::size_t i = 0; i < values_count; ++i) {
        float value; std::cin >> value;
        board_values.push_back(value);
    }

    dirichlet_solver_t solver;
    solver.init(upper_angle, lower_angle, board_values);
    auto&& ans = solver.solve();
    std::cout << ans;
}