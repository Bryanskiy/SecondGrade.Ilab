#include <iostream>
#include <stdexcept>

#include "linalg/include/octtree.h"
#include "linalg/include/geometry.h"
#include "Vkdriver.hpp"

std::vector<vkdriver::Vertex> transformTriangles2Verteces(const std::vector<std::size_t>& intersection_indices, const std::vector<lingeo::triangle_t<3>>& triangles) {
    std::vector<vkdriver::Vertex> ret;
    std::size_t k = 0;
    std::size_t max_k = intersection_indices.size();
    for(std::size_t i = 0, max = triangles.size(); i < max; ++i) {
        glm::vec3 color;
        if((k < max_k) && (i == intersection_indices[k])) {
            color = {1.0, 0.0, 0.0};
            ++k;
        } else {
            color = {0.0, 0.0, 1.0}; 
        }

        lingeo::vector_t<3> n = lingeo::plane_t(triangles[i][0], triangles[i][1], triangles[i][2]).normal();
        glm::vec3 normal = {n[0], n[1], n[2]};
        normal = (glm::dot(normal, {0, 0, 1})) > 0 ? normal : -normal;

        for(std::size_t j = 0; j < 3; ++j) {
           vkdriver::Vertex vertex;
           vertex.pos = {triangles[i][j][0], triangles[i][j][1], triangles[i][j][2]};
           vertex.color = color;
           vertex.normal = normal;
           ret.push_back(vertex);
        }
    }

    return ret;
}

int main() {

    std::size_t N; std::cin >> N;
    std::vector<lingeo::triangle_t<3>> triangles(N);
    for(std::size_t i = 0; i < N; ++i) {
        std::cin >> triangles[i];
    }

    octt::space_t space = octt::create(triangles);
    octt::octtree_t<lingeo::triangle_t<3>> octtree(space);

    for(std::size_t i = 0; i < N; ++i) {
        octtree.insert(i, triangles[i]);
    }
    std::vector<std::size_t> intersection_indices = octtree.get_intersections();

    try {
        vkdriver::Vkdriver driver;
        driver.run(transformTriangles2Verteces(intersection_indices, triangles));
    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}

