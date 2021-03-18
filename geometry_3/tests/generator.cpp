#include <iostream>
#include <random>
#include "../app/app.hpp"

lingeo::point_t<3> generate_random_point(double a, double b) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dis(a, b);

    long double x = dis(gen);
    long double y = dis(gen);
    long double z = dis(gen);

    return {x, y, z};
}

glm::vec3 generate_glm_random_vec(double a, double b) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dis(a, b);

    long double x = dis(gen);
    long double y = dis(gen);
    long double z = dis(gen);

    return {x, y, z};
}

lingeo::triangle_t<3> generate_random_triangle(std::size_t cube_size) {
    lingeo::point_t<3> A1 = generate_random_point(0, 5.0);
    lingeo::point_t<3> B1 = generate_random_point(0, 5.0);
    lingeo::point_t<3> C1 = generate_random_point(0, 5.0);

    lingeo::point_t<3> offset = generate_random_point(0, cube_size);

    lingeo::point_t<3> A = lingeo::vector_t<3>{A1} + lingeo::vector_t<3>{offset};
    lingeo::point_t<3> B = lingeo::vector_t<3>{B1} + lingeo::vector_t<3>{offset};
    lingeo::point_t<3> C = lingeo::vector_t<3>{C1} + lingeo::vector_t<3>{offset};

    return {A, B, C};
}

int main() {
    std::cout << "Please, enter triangles number: " << std::endl;
    std::size_t triangles_count; std::cin >> triangles_count;

    std::cout << "Triangles will be generated in cube a x a x a." << std::endl;
    std::cout << "Please, enter a: ";
    std::size_t cube_size; std::cin >> cube_size;

    trap::App app;
    for(std::size_t i = 0; i < triangles_count; ++i) {
        auto triangle = generate_random_triangle(cube_size);
        glm::vec3 posRotation = generate_glm_random_vec(0, cube_size);
        glm::vec3 dirRotation = generate_glm_random_vec(0, cube_size);

        app.pushTriangle(triangle, posRotation, dirRotation, std::numeric_limits<float>::max(), 1.0);
    }

    app.run();
}