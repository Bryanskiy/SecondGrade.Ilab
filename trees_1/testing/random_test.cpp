#include "../avl_tree.hpp"
#include <random>
#include <iostream>
#include <limits>
/* it makes some operations with tree in a loop 
   if invariant is broken tree will throw exeption   */
void height_invariant_test() {
    avl::tree_t<int> tree;

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(std::numeric_limits<int>::min(), std::numeric_limits<int>::max());
    try {

        for(std::size_t i = 0; i < 100000; ++i) {
            int tmp = dis(gen);
            tree.insert(tmp);
        }        

    } catch(std::runtime_error e) {
        std::cerr << e.what();
        return;
    }

    std::cout << "Height invariant test: SUCCESS" << std::endl;
}

int main() {
    height_invariant_test();
}