#include <gtest/gtest.h>
#include "../avl_tree.hpp"
#include <random>
#include <iostream>
#include <limits>
#include <chrono>
#include <cmath>

class Timer_t {
public:
    using clock_t = std::chrono::high_resolution_clock;
    using microseconds_t = std::chrono::microseconds;

    Timer_t() : start_(clock_t::now()) {}
    microseconds_t get_time() {
        return std::chrono::duration_cast<microseconds_t>(clock_t::now() - start_);
    }
    void reset() {
        start_ = clock_t::now();
    }
private:
    std::chrono::time_point<clock_t> start_;
};

/* it makes some operations with tree in a loop 
   if invariant is broken tree will throw exeption */
void height_invariant_test() {
    avl::tree_t<int> tree;

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(std::numeric_limits<int>::min(), std::numeric_limits<int>::max());
    try {

        for(std::size_t i = 0; i < 10000; ++i) {
            int tmp = dis(gen);
            tree.insert(tmp);
        }        

    } catch(std::runtime_error e) {
        std::cerr << e.what();
        return;
    }

    std::cout << "Height invariant test: SUCCESS" << std::endl;
}

TEST(Tree, LowerBound) {
    {
        avl::tree_t<int> tree;
        for(std::size_t i = 0; i < 100; ++i) {
            tree.insert(i);
        }

        for(std::size_t i = 0; i < 100; ++i) {
            int elem = *(tree.lower_bound(i));
            ASSERT_TRUE(elem == i);
        }

        ASSERT_TRUE(tree.lower_bound(101) == tree.end());
        ASSERT_TRUE(tree.lower_bound(-1) == tree.begin());
    }
}

TEST(Tree, UpperBound) {
    {
        avl::tree_t<int> tree;
        for(std::size_t i = 0; i < 100; ++i) {
            tree.insert(i);
        }

        for(std::size_t i = 0; i < 99; ++i) {
            int elem = *(tree.upper_bound(i));
            ASSERT_TRUE(elem == i + 1);
        }

        ASSERT_TRUE(tree.lower_bound(101) == tree.end());
        ASSERT_TRUE(tree.lower_bound(100) == tree.end());
        ASSERT_TRUE(tree.lower_bound(-1) == tree.begin());
    }
}

TEST(TreeIterator, IncrementOperator) {
    {
        avl::tree_t<int> tree;
        for(std::size_t i = 0; i < 100; ++i) {
            tree.insert(i);
        }

        avl::tree_t<int>::iterator it = tree.begin();
        for(std::size_t i = 0; it != tree.end(); ++i) {
            ASSERT_TRUE(*it == i);
            ++it;
        }
    }
}

void uniform_distribution_time_test() {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(std::numeric_limits<int>::min(), std::numeric_limits<int>::max());

    std::size_t iterations_count = std::pow(2, 14) - 1;
    std::cout << "Number of elements: " << iterations_count << std::endl;

    std::set<int> stdset;
    avl::tree_t<int> avlset;

    Timer_t stdset_timer;
    for(std::size_t i = 0; i < iterations_count; ++i) { 
        stdset.insert(dis(gen));
    }

    std::cout << "Std::set time : " << stdset_timer.get_time().count() << " mcs" << std::endl;

    Timer_t avlset_timer;
    for(std::size_t i = 0; i < iterations_count; ++i) {
        avlset.insert(dis(gen));
    }

    std::cout << "Avl tree time : " << avlset_timer.get_time().count() << " mcs" << std::endl;
}

void specific_distribution_for_avl_time_test() {
    std::size_t iterations_count = std::pow(2, 14) - 1;

    std::cout << "Number of elements: " << iterations_count << std::endl; 

    std::set<int> stdset;
    avl::tree_t<int> avlset;

    Timer_t stdset_timer;
    std::size_t max_h = std::log2(iterations_count);
    for(std::size_t h = 0; h <= max_h; ++h) {
        for(std::size_t i = 0; i < std::pow(2, h); ++i) {
            int elem = iterations_count / std::pow(2, h + 1) + i * (std::pow(2, max_h + 1 - h));
            stdset.insert(elem);
        }
    }
    std::cout << "Std::set time : " << stdset_timer.get_time().count() << " mcs" << std::endl;

    Timer_t avlset_timer;
    for(std::size_t h = 0; h <= max_h; ++h) {
        for(std::size_t i = 0; i < std::pow(2, h); ++i) {
            int elem = iterations_count / std::pow(2, h + 1) + i * (std::pow(2, max_h + 1 - h));
            avlset.insert(elem);
        }
    }
    std::cout << "Avl tree time : " << avlset_timer.get_time().count() << " mcs" << std::endl;

}

int main(int argc, char **argv) {
    std::cout << "------------------------------------------------------------------" << std::endl;
    height_invariant_test();



    std::cout << "------------------------------------------------------------------" << std::endl;
    std::cout << "Uniform int distribution time test: " << std::endl;
    uniform_distribution_time_test();



    std::cout << "------------------------------------------------------------------" << std::endl;
    std::cout << "Special distribution for avl tree time test: " << std::endl;
    specific_distribution_for_avl_time_test();



    std::cout << "------------------------------------------------------------------" << std::endl;
    std::cout << "Unit tests: " << std::endl;
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}