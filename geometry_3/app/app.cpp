#include "app.hpp"

namespace trap {

 App::triangleHandler::triangleHandler(const lingeo::triangle_t<3>& triangle, const glm::vec3& posRotation, const glm::vec3& dirRotation, 
                                       const glm::vec3& color, float lifeTime, float angle, float speedRotation) : 
triangle_(triangle), posRotation_(posRotation), dirRotation_(dirRotation), color_(color), lifeTime_(lifeTime), angle_(angle), speedRotation_(speedRotation) {}

glm::mat4 App::triangleHandler::getModelMatrix() const {
    glm::mat4 translationMatrix = glm::translate(glm::mat4(1.f), posRotation_);
    glm::mat4 res = glm::rotate(translationMatrix, glm::radians(angle_), dirRotation_);
    return res;
}

void App::triangleHandler::update(const std::vector<glm::vec3>& wCoords, float time) {
    lifeTime_ -= time;

    if (lifeTime_ > 0.f) {
        angle_ += speedRotation_ * std::min(time, lifeTime_);
    }

    for(std::size_t p = 0; p < 3; ++p) {
        for(std::size_t c = 0; c < 3; ++c) {
            triangle_[p][c] = wCoords[p][c];
        }
    }

    setColor({0.0, 0.0, 1.0});
}

void App::pushTriangle(const lingeo::triangle_t<3>& triangle, glm::vec3 posRotation, glm::vec3 dirRotation, float lifeTime, float speedRotation) {
    triangleHandler obj(triangle, posRotation, dirRotation, {0.0, 0.0, 1.0}, lifeTime, 0, speedRotation);
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

        auto triangle = triangles_[i].getTriangle();
        auto posRotation = triangles_[i].getPosRotation();

        auto normal = lingeo::plane_t(triangle[0], triangle[1], triangle[2]).normal();
        glm::vec3 format_normat = {normal[0] - posRotation[0], 
                                   normal[1] - posRotation[1], 
                                   normal[2] - posRotation[2]};
                                   
        for(std::size_t j = 0, maxj = 3; j < maxj; ++j) {
            glm::vec3 pos = {triangle[j][0] - posRotation[0], 
                             triangle[j][1] - posRotation[1], 
                             triangle[j][2] - posRotation[2]};
            driver_.pushVertex(pos, {0.0, 1.0, 0.0}, format_normat, i);
        }

        driver_.pushModelMatrix(triangles_[i].getModelMatrix());
    }
    driver_.initVulkan();
}


void App::run() {
    init();

    driver_.drawFrame();
    driver_.drawFrame();
    driver_.drawFrame();
    driver_.drawFrame();
    driver_.drawFrame();

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
    driver_.setCameraMatrices(camera_.getViewMatrix(), camera_.getProjMatrix());
    for(std::size_t i = 0, maxi = triangles_.size(); i < maxi; ++i) {
        driver_.setModelMatrix(i, triangles_[i].getModelMatrix());
        driver_.setColor(i, triangles_[i].getColor());
    }
}

void App::updateModels(float time) {
    long double x_min, y_min, z_min;
    long double x_max, y_max, z_max;
    x_min = y_min = z_min = std::numeric_limits<long double>::max();
    x_max = y_max = z_max = std::numeric_limits<long double>::min();

    for(std::size_t i = 0, maxi = triangles_.size(); i < maxi; ++i) {
        auto newWorldCoordinates = driver_.getWorldCoordinates(i);
        triangles_[i].update(newWorldCoordinates, time);

        auto x_proj = triangles_[i].projection_i(0);
        x_min = std::min(x_proj.first, x_min); x_max = std::max(x_proj.second, x_max);

        auto y_proj = triangles_[i].projection_i(1);
        y_min = std::min(y_proj.first, y_min); y_max = std::max(y_proj.second, y_max);

        auto z_proj = triangles_[i].projection_i(2);
        y_min = std::min(y_proj.first, y_min); y_max = std::max(y_proj.second, y_max);
    }

    octt::space_t space{x_min, x_max, y_min, y_max, z_min, z_max};
    octt::octtree_t<triangleHandler> octree(space);

    for(std::size_t i = 0, maxi = triangles_.size(); i < maxi; ++i) {
        octree.insert(&(triangles_[i]));
    }

    octree.fill_intersection();

    // for(std::size_t i = 0, maxi = triangles_.size(); i < maxi; ++i) {
    //       for(std::size_t j = i + 1, maxj = maxi; j < maxj; ++j) {
    //           if(lingeo::intersection(triangles_[i].getTriangle(), triangles_[j].getTriangle())) {
    //               triangles_[i].setColor({1.0, 0.0, 0.0});
    //               triangles_[j].setColor({1.0, 0.0, 0.0});
    //           }
    //       }
    //  }
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