#include <gtest/gtest.h>

#define DEBUG_
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

void height_invariant_test() {
    avl::tree_t<int> tree;

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(std::numeric_limits<int>::min(), std::numeric_limits<int>::max());

    for(std::size_t i = 0; i < 10000; ++i) {
        int tmp = dis(gen);
        tree.insert(tmp);
    }        

    try {
        tree.check_height_invariant();
    } catch(std::runtime_error e) {
        std::cerr << e.what() << std::endl;
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

    {
        avl::tree_t<int> tree;
        for(std::size_t i = 0; i < 100; ++i) {
            tree.insert(i);
        }

        avl::tree_t<int>::iterator it_prev = tree.begin();
        avl::tree_t<int>::iterator it = ++(tree.begin());
        while(it != tree.end()) {
            ASSERT_TRUE(*it > *it_prev);
            ++it; ++it_prev;
        }
    }
}

std::vector<int> generate_uniform_distribution(std::size_t elements_number) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(std::numeric_limits<int>::min(), std::numeric_limits<int>::max());

    std::vector<int> ret(elements_number);
    for(auto& i : ret) {
        i = dis(gen);
    }

    return ret;
}

/* elements number must be pow(2, k) - 1 */
std::vector<int> generate_distribution_without_rotations(std::size_t elements_number) {
    std::vector<int> ret(elements_number);

    std::size_t max_h = std::log2(elements_number);
    for(std::size_t h = 0; h <= max_h; ++h) {
        for(std::size_t i = 0; i < std::pow(2, h); ++i) {
            int elem = elements_number / std::pow(2, h + 1) + i * (std::pow(2, max_h + 1 - h));
            ret.push_back(elem);
        }
    }

    return ret;
}

template<typename Func>
void time_test(Func func, std::size_t elements_count) {
    std::cout << "Number of elements: " << elements_count << std::endl;

    std::set<int> stdset;
    avl::tree_t<int> avlset;

    std::vector<int> data = func(elements_count);

    Timer_t avlset_timer;
    for(std::size_t i = 0; i < elements_count; ++i) { 
        avlset.insert(data[i]);
    }
    std::cout << "Avl set time  : " << avlset_timer.get_time().count() << " mcs" << std::endl;

    Timer_t stdset_timer;
    for(std::size_t i = 0; i < elements_count; ++i) { 
        stdset.insert(data[i]);
    }
    std::cout << "Std::set time : " << stdset_timer.get_time().count() << " mcs" << std::endl;
}

int main(int argc, char **argv) {
    std::cout << "------------------------------------------------------------------" << std::endl;
    height_invariant_test();


    std::size_t number_elements = std::pow(2, 15) - 1;
    std::cout << "------------------------------------------------------------------" << std::endl;
    std::cout << "Uniform int distribution time test: " << std::endl;
    time_test(generate_uniform_distribution, number_elements);



    std::cout << "------------------------------------------------------------------" << std::endl;
    std::cout << "Distribution without rotations time test: " << std::endl;
    time_test(generate_distribution_without_rotations, number_elements);



    std::cout << "------------------------------------------------------------------" << std::endl;
    std::cout << "Unit tests: " << std::endl;
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}