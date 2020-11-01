#include <iostream>
#include "matrix.h"

#include <vector>

int main() {
    std::vector<int> actual_v{3, 5, 7, 8, 6, 4, 2, 1, 9, 7, 3, 3};
    matrix::matrix_t<int> actual_m(3, 4, actual_v.begin(), actual_v.end());

    std::vector<int> expected_v{9, 7, 3, 3, 0, 24, 54, 63, 0, 0, -324, 162};
    matrix::matrix_t<int> expected_m(3, 4, expected_v.begin(), expected_v.end());

    actual_m.gauss_straight();


    //matrix::matrix_t<int> m;
    //std::cin >> m;
}