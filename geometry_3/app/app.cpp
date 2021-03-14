#include "app.hpp"

namespace trap {

glm::mat4 App::triangleHandler::getModelMatrix() const {
    glm::mat4 translationMatrix = glm::translate(glm::mat4(1.f), posRotation_);
    glm::mat4 res = glm::rotate(translationMatrix, glm::radians(angle_), dirRotation_);
    return res;
}

void App::pushTriangle(const lingeo::triangle_t<3>& triangle, glm::vec3 posRotation, glm::vec3 dirRotation, float lifeTime, float speedRotation) {
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
        glm::vec3 format_normat = {normal[0] - triangles_[i].posRotation_[0], 
                                   normal[1] - triangles_[i].posRotation_[1], 
                                   normal[2] - triangles_[i].posRotation_[2]};
                                   
        for(std::size_t j = 0, maxj = 3; j < maxj; ++j) {
            glm::vec3 pos = {triangles_[i].triangle_[j][0] - triangles_[i].posRotation_[0], 
                             triangles_[i].triangle_[j][1] - triangles_[i].posRotation_[1], 
                             triangles_[i].triangle_[j][2] - triangles_[i].posRotation_[2]};
            driver_.pushVertex(pos, {0.0, 1.0, 0.0}, format_normat, i);
        }

        driver_.pushModelInfo(triangles_[i].getModelMatrix());
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
        updateModels(time);
        updateDriver();

        driver_.drawFrame();
    }

    driver_.wait();
}

void App::updateDriver() {
    driver_.updateCameraMatrices(camera_.getViewMatrix(), camera_.getProjMatrix());
    for(std::size_t i = 0, maxi = triangles_.size(); i < maxi; ++i) {
        driver_.setModelData(i, triangles_[i].getModelMatrix());
        driver_.setColor(i, triangles_[i].color_);
    }
}

void App::updateModels(float time) {
    long double x_min, y_min, z_min;
    long double x_max, y_max, z_max;
    x_min = y_min = z_min = std::numeric_limits<long double>::max();
    x_max = y_max = z_max = std::numeric_limits<long double>::min();

    for(std::size_t i = 0, maxi = triangles_.size(); i < maxi; ++i) {
        {
            triangles_[i].lifeTime_ -= time;

            if (triangles_[i].lifeTime_ > 0.f) {
                triangles_[i].angle_ += triangles_[i].speedRotation_ * std::min(time, triangles_[i].lifeTime_);
            }
        }

        auto newWorldCoordinates = driver_.getWorldCoordinates(i);
        for(std::size_t p = 0; p < 3; ++p) {
            for(std::size_t c = 0; c < 3; ++c) {
                triangles_[i].triangle_[p][c] = newWorldCoordinates[p][c];
            }

            if(newWorldCoordinates[p][0] < x_min) {x_min = newWorldCoordinates[p][0];} 
            else if(newWorldCoordinates[p][0] > x_max) {x_max = newWorldCoordinates[p][0];}

            if(newWorldCoordinates[p][1] < y_min) {y_min = newWorldCoordinates[p][1];} 
            else if(newWorldCoordinates[p][1] > y_max) {y_max = newWorldCoordinates[p][1];}

            if(newWorldCoordinates[p][2] < z_min) {z_min = newWorldCoordinates[p][2];} 
            else if(newWorldCoordinates[p][2] > z_max) {z_max = newWorldCoordinates[p][1];}
        }
    }

    octt::space_t space{x_min, x_max, y_min, y_max, z_min, z_max};
    octt::octtree_t<lingeo::triangle_t<3>> octree(space);

    for(std::size_t i = 0, maxi = triangles_.size(); i < maxi; ++i) {
        octree.insert(i, triangles_[i].triangle_);
    }

    auto indices = octree.get_intersections();

    std::size_t k = 0;
    for(std::size_t i = 0, maxi = triangles_.size(); i < maxi; ++i) {
         if((k < indices.size()) && (i == indices[k])) {
             triangles_[i].color_ = {1.0, 0.0, 0.0};
             ++k;
         } else {
             triangles_[i].color_ = {0.0, 0.0, 1.0};
        }
    }
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