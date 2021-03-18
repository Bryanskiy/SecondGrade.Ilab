#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <fstream>
#include <vector>
#include <algorithm>
#include <cstring>
#include "../camera/camera.hpp"
#include "../Vkdriver/Vkdriver.hpp"
#include "../linalg/include/geometry.h"

/* trap - triangles app :) */
namespace trap {

class App {
public:
    void run();

    void pushTriangle(const lingeo::triangle_t<3>& triangle, glm::vec3 posRotation, 
                       glm::vec3 dirRotation, float lifeTime, float speedRotation);              
private:

    class triangleHandler {
        public:
            triangleHandler(const lingeo::triangle_t<3>& triangle, const glm::vec3& posRotation, const glm::vec3& dirRotation, 
                            const glm::vec3& color, float lifeTime, float angle, float speedRotation);

            glm::mat4 getModelMatrix() const;
            void update(const std::vector<glm::vec3>& wCoords, float time);
            void setColor(const glm::vec3& color) {color_ = color;}

            lingeo::triangle_t<3> getTriangle() const {return triangle_;}
            glm::vec3 getPosRotation() const {return posRotation_;}
            glm::vec3 getColor() const {return color_;}

            auto projection_i(std::size_t i) {return triangle_.projection_i(i);}

        private:
            lingeo::triangle_t<3> triangle_; /* world coords */

            glm::vec3 posRotation_;
            glm::vec3 dirRotation_;        
            glm::vec3 color_ = {1.0, 1.0, 1.0};

            float lifeTime_; /* in sec */
            float angle_;
            float speedRotation_;
    };

private:
    std::vector<triangleHandler>    triangles_;
    GLFWwindow*                     window_ = nullptr;
    Camera                          camera_;
    vkdriver::Vkdriver              driver_;

private:
    void updateDriver();
    void updateModels(float time);

    static void framebufferResizeCallback(GLFWwindow* window, int width, int height);
    static void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods);    
    static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mode);

    void init();
};

} /* namespace trap */ 