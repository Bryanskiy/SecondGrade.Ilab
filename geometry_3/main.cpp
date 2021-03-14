#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <iostream>
#include "app/app.hpp"


int main() {
    std::size_t triangle_count; 
    std::cin >> triangle_count;
    std::size_t time;
    std::cin >> time;

    trap::App app;
    for(std::size_t i = 0; i < triangle_count; ++i) {
        glm::vec3 rotationFst, rotationScd;
        std::size_t lifeTime;
        float speedRotation;

        lingeo::triangle_t<3> triangle;
        std::cin >> triangle;

        std::cin >> rotationFst.x >> rotationFst.y >> rotationFst.z;
        std::cin >> rotationScd.x >> rotationScd.y >> rotationScd.z;

        std::cin >> speedRotation;

        app.pushTriangle(triangle, rotationFst, rotationScd - rotationFst, time, speedRotation);
    }

    app.run();
} 