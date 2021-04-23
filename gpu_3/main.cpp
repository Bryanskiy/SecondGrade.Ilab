#include <iostream>
#include <vector>

#include "dirichlet/dirichlet.hpp"
#include "cl_framework/cl_linalg.hpp"

int main() {
    float step; std::cin >> step;
    point_t upper_angle; std::cin >> upper_angle.x_ >> upper_angle.y_;
    point_t lower_angle; std::cin >> lower_angle.x_ >> lower_angle.y_;
    
    std::vector<float> board_values;
    for(;;) {
        float value; std::cin >> value;
        if(!std::cin) {break;}

        board_values.push_back(value);
    }

    auto&& ans = GPU_square_dirichlet_problem(step, upper_angle, lower_angle, board_values);
    std::cout << ans << std::endl;
}