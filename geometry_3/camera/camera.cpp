#include "camera.hpp"

namespace trap {

void Camera::update(float time, GLFWwindow *window) {

    int wHeight = 0, wWidth = 0;
    glfwGetWindowSize(window, &wWidth, &wHeight);
    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
        double x = 0, y = 0;
        glfwGetCursorPos(window, &x, &y);
        const float dx = wWidth / 2.f - x;
        const float dy = wHeight / 2.f - y;

        turnInHorizontalPlane(glm::radians(90.f * dx / wWidth));
        turnInVerticalPlane(glm::radians(90.f * dy / wHeight));

        glfwSetCursorPos(window, wWidth / 2.0, wHeight / 2.0);
    }

    bool forward = glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS;
    bool back = glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS;
    if (forward && back) {
        forward = back = false;
    }

    bool left = glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS;
    bool right = glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS;
    if (left && right) {
        left = right = false;
    }

    bool down = glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS;
    bool up = glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS;
    if (down && up) {
        down = up = false;
    }

    if (forward || back) {
        moveInDirection(time * speed * ((forward) ? 1.f : -1.f));
    }
    if (left || right) {
        movePerpendicularDirection(time * speed * ((right) ? 1.f : -1.f));
    }

    if (down || up) {
        moveUp(time * speed * ((down) ? -1.f : 1.f));
    }
}

}/* namespace trap */