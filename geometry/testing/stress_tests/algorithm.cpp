#include "../../src/include/geometry.h"
#include "../../src/include/octtree.h"

#include <vector>
#include <random>
#include <fstream>
#include <iostream>
#include <algorithm>

bool algorithm_naive(const std::vector<lingeo::triangle_t<3>>& triangles, std::ostream& out) {
    std::vector<int> flags(triangles.size());

    for(std::size_t i = 0; i < triangles.size() - 1; ++i) {
        for(std::size_t j = i + 1; j < triangles.size(); ++j) {
            if(lingeo::intersection(triangles[i], triangles[j])) {
                flags[i] = true;
                flags[j] = true;
            }
        }
    }

    for(std::size_t i = 0; i < flags.size(); ++i) {
        if(flags[i] == true) {
            out << i << std::endl;
        }
    }
}

void algorithm_fast(const std::vector<lingeo::triangle_t<3>>& triangles, std::ostream& out) {
    octt::space_t space = octt::create(triangles);
    octt::octtree_t<lingeo::triangle_t<3>> octtree(space);

    for(std::size_t i = 0; i < triangles.size(); ++i) {
        octtree.insert(i, triangles[i]);
    }

    octtree.print_intersections(out);
}

lingeo::point_t<3> generate_random_point(int a, int b) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(a, b);

    long double x = dis(gen);
    long double y = dis(gen);
    long double z = dis(gen);

    return {x, y, z};
}



lingeo::triangle_t<3> generate_random_triangle(int a, int b) {
    lingeo::point_t<3> A1 = generate_random_point(a, b);
    lingeo::point_t<3> B1 = generate_random_point(a, b);
    lingeo::point_t<3> C1 = generate_random_point(a, b);

    lingeo::point_t<3> offset = generate_random_point(lingeo::sign(a) * 4 * std::abs(a),lingeo::sign(b) * 4 * std::abs(b));

    lingeo::point_t<3> A = lingeo::vector_t<3>{A1} + lingeo::vector_t<3>{offset};
    lingeo::point_t<3> B = lingeo::vector_t<3>{B1} + lingeo::vector_t<3>{offset};
    lingeo::point_t<3> C = lingeo::vector_t<3>{C1} + lingeo::vector_t<3>{offset};

    return {A, B, C};
}

std::vector<lingeo::triangle_t<3>> generate_random_statistic(std::size_t n, int a, int b) {
    std::vector<lingeo::triangle_t<3>> ret(n);
    for(std::size_t i = 0; i < n; ++i) {
        ret[i] = generate_random_triangle(a, b);
    }

    return ret;
}

int main() {
    for(;;) {
        std::vector<lingeo::triangle_t<3>> stat = generate_random_statistic(1000, -100, 100);
        {
            std::ofstream naive("naive.txt", std::ios_base::out | std::ios_base::trunc);
            std::ofstream fast("fast.txt", std::ios_base::out | std::ios_base::trunc);
            std::ofstream triangles("triangles.txt", std::ios_base::out | std::ios_base::trunc);

            algorithm_fast(stat, fast);
            algorithm_naive(stat, naive);

            for (std::size_t i = 0; i < stat.size(); ++i) {
                triangles << stat[i] << std::endl;
            }

            naive.close();
            fast.close();
            triangles.close();
        }
        {
            std::ifstream naive("naive.txt", std::ios_base::in | std::ios_base::trunc);
            std::ifstream fast("fast.txt", std::ios_base::in | std::ios_base::trunc);

            std::vector<std::size_t> naive_v;
            std::vector<std::size_t> fast_v;

            std::size_t tmp;
            while(naive >> tmp) {
                naive_v.push_back(tmp);
            }

            while(fast >> tmp) {
                fast_v.push_back(tmp);
            }

            std::sort(fast_v.begin(), fast_v.end());
            std::size_t max1 = fast_v.size();
            std::size_t max2 = naive_v.size();
            if(max1 != max2) {
                std::cout << "fail" << std::endl;
                break;
            }
            for(std::size_t i = 0; i < max1 && i < max2; ++i) {
                if(fast_v[i] != naive_v[i]) {
                    std::cout << "fail" << std::endl;
                    break;
                }
            }
        }
    }
}