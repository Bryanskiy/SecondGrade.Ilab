#include <iostream>

#include "matrix/matrix_chain.hpp"

int main() {
    matrix::Imatrix_chain_t chain;

    for(;;) {
        std::size_t lhs; std::cin >> lhs;
        std::size_t rhs; std::cin >> rhs;

        if(!std::cin) {break;}
        chain.push(lhs, rhs);
    }

    auto&& order = chain.get_optimal_order();
    for(std::size_t i = 0; i < order.size(); ++i) {
        std::cout << order[i] << " ";
    }
    std::cout << std::endl;
}