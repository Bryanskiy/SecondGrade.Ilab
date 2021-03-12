#include "app.hpp"

namespace trap {

void App::pushTriangle(const lingeo::triangle_t<3>& triangle, glm::vec3 posRotation, glm::vec3 dirRotation, std::size_t lifeTime, float speedRotation) {
    triangleHandler obj;
    obj.triangle_ = triangle;
    obj.posRotation_ = posRotation;
    obj.dirRotation_ = dirRotation;
    obj.color_ = {0.0, 0.0, 1.0};
    obj.lifeTime_ = lifeTime;
    obj.angle_ = 0.0;
    obj.speedRotation_ = speedRotation;

    triangles_.push_back(obj);
}

void App::init() {

    /* init window */
    glfwInit();
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    window_ = glfwCreateWindow(1200 , 800, "Vulkan", nullptr, nullptr);
    glfwSetWindowUserPointer(window_, this);
    glfwSetFramebufferSizeCallback(window_, framebufferResizeCallback);
    glfwSetKeyCallback(window_, keyCallback);
    glfwSetMouseButtonCallback(window_, mouseButtonCallback);

    /* init vulkan */
    driver_.setWindow(window_);
    for(std::size_t i = 0, maxi = triangles_.size(); i < maxi; ++i) {
        auto normal = lingeo::plane_t(triangles_[i].triangle_[0], triangles_[i].triangle_[1], triangles_[i].triangle_[2]).normal();
        glm::vec3 format_normat = {normal[0], normal[1], normal[2]};
        for(std::size_t j = 0, maxj = 3; j < maxj; ++j) {
            glm::vec3 pos = {triangles_[i].triangle_[j][0], triangles_[i].triangle_[j][1], triangles_[i].triangle_[j][2]};
            driver_.pushVertex(pos, {0.0, 1.0, 0.0}, format_normat, i);
        }
    }
    driver_.initVulkan();
}


void App::run() {
    init();

    auto startTime = std::chrono::high_resolution_clock::now();    
    while (!glfwWindowShouldClose(window_)) {
        auto currentTime = std::chrono::high_resolution_clock::now();
		float time = std::chrono::duration<float, std::chrono::seconds::period>(currentTime - startTime).count();
		startTime = currentTime;
        glfwPollEvents();
        camera_.update(time, window_);
        driver_.updateCameraMatrices(camera_.getViewMatrix(), camera_.getProjMatrix());
        driver_.drawFrame();
    }

    driver_.wait();
}

/* static */ void App::framebufferResizeCallback(GLFWwindow* window, int width, int height) {
    auto app = static_cast<App*>(glfwGetWindowUserPointer(window));
    app->driver_.setResizeFlag();
    app->camera_.setAspect(width, height);
}

/* static */ void App::mouseButtonCallback(GLFWwindow* window, int button, int action, int mods) {
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
        int wHeight = 0, wWidth = 0;
		glfwGetWindowSize(window, &wWidth, &wHeight);
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
        glfwSetCursorPos(window, wWidth / 2.0, wHeight / 2.0);
    } else if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE) {
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    }

}

/* static */ void App::keyCallback(GLFWwindow* window, int key, int scancode, int action, int mode) {
	auto app = static_cast<App*>(glfwGetWindowUserPointer(window));

	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, GL_TRUE);
	}
}

} /* namespace trap */ 