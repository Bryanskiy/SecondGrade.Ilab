#include "../../src/include/geometry.h"
#include "../../src/include/octtree.h"

#include <vector>
#include <random>
#include <fstream>
#include <iostream>

bool algorithm_naive(const std::vector<ivkg::triangle_t<3>>& triangles, std::ostream& out) {
    std::vector<int> flags(triangles.size());

    for(std::size_t i = 0; i < triangles.size() - 1; ++i) {
        for(std::size_t j = i + 1; j < triangles.size(); ++j) {
            if(ivkg::intersection(triangles[i], triangles[j])) {
                flags[i] = true;
                flags[j] = false;
            }
        }
    }

    for(std::size_t i = 0; i < flags.size(); ++i) {
        if(flags[i] == true) {
            out << i << std::endl;
        }
    }
}

void algorithm_fast(const std::vector<ivkg::triangle_t<3>>& triangles, std::ostream& out) {
    octt::space_t space = octt::create(triangles);
    octt::octtree_t<ivkg::triangle_t<3>> octtree(space);

    for(std::size_t i = 0; i < triangles.size(); ++i) {
        octtree.insert(i, triangles[i]);
    }

    octtree.print_intersections(out);
}

ivkg::point_t<3> generate_random_point(long double a, long double b) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dis(a, b);

    long double x = dis(gen);
    long double y = dis(gen);
    long double z = dis(gen);

    return {x, y, z};
}



ivkg::triangle_t<3> generate_random_triangle(long double a, long double b) {
    ivkg::point_t<3> A1 = generate_random_point(a, b);
    ivkg::point_t<3> B1 = generate_random_point(a, b);
    ivkg::point_t<3> C1 = generate_random_point(a, b);

    ivkg::point_t<3> offset = generate_random_point(ivkg::sign(a) * 10 * std::abs(a), ivkg::sign(b) * 10 * std::abs(b));

    ivkg::point_t<3> A = ivkg::vector_t<3>{A1} + ivkg::vector_t<3>{offset};
    ivkg::point_t<3> B = ivkg::vector_t<3>{B1} + ivkg::vector_t<3>{offset};
    ivkg::point_t<3> C = ivkg::vector_t<3>{C1} + ivkg::vector_t<3>{offset};

    return {A, B, C};
}

std::vector<ivkg::triangle_t<3>> generate_random_statistic(std::size_t n, long double a, long double b) {
    std::vector<ivkg::triangle_t<3>> ret(n);
    for(std::size_t i = 0; i < n; ++i) {
        ret[i] = generate_random_triangle(a, b);
    }

    return ret;
}

int main() {
    std::vector<ivkg::triangle_t<3>> stat = generate_random_statistic(1000, -100, 100);

    std::ofstream naive("naive.txt", std::ios_base::out | std::ios_base::trunc);
    std::ofstream fast("fast.txt", std::ios_base::out | std::ios_base::trunc);

    algorithm_fast(stat, fast);
    algorithm_naive(stat, naive);

    naive.close();
    fast.close();

}