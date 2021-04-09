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

    chain.dump(std::cout);
}