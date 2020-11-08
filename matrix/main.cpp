#include <iostream>
#include "matrix.h"
#include <vector>

int main() {
    std::size_t size; std::cin >> size;
    matrix::matrix_t<double> matrix(size, size);
    std::cin >> matrix;
    std::cout << matrix.det() << std::endl;
}