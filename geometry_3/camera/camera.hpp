#pragma once

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#define GLFW_INCLUDE_VULKAN
#include <glm/glm.hpp>
#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>
#include "../linalg/include/octtree.h"

namespace trap {
class Camera {
public:
	Camera() {}

    glm::vec3 getPosition() const {return position;}
    glm::vec3 getDirection() const {return direction;}
    glm::vec3 getUpVector() const {return upVector;}
    float     getViewangle() const {return viewAngle;}
    float     getAspect() const {return aspect;}

    glm::mat4 getViewMatrix() const { return glm::lookAt(position, position + direction, upVector); } 
    glm::mat4 getProjMatrix() const { return glm::perspective(glm::radians(viewAngle), aspect, near, far);}

	void setAspect(float width, float height) {aspect = width / height;}

    void update(float time, GLFWwindow* window);

private:
    glm::vec3 position = {0, 0, 0};
    glm::vec3 direction = {1, 1, 1};

	glm::vec3 upVector = glm::vec3(0.f, 0.f, 1.f);
	float viewAngle = 45.f;
	float aspect = 4.f / 3.f;
	float near = 0.1f;
	float far = 1000.f;
    float speed = 5.f;
private:
    void moveInDirection(float distance) { position += distance * direction;}
    void moveUp(float distance) {position += distance * upVector;}
    void movePerpendicularDirection(float distance) {
        auto axis = glm::cross(direction, upVector);
		position += axis * distance;
    } 

    void turnInHorizontalPlane(float angle) {
		direction = glm::rotate(glm::mat4(1.f), angle, upVector) * glm::vec4(direction, 0.f);
	}

    void turnInVerticalPlane(float angle) {
        auto axis = glm::cross(direction, upVector);
		direction = glm::rotate(glm::mat4(1.f), angle, axis) * glm::vec4(direction, 0.f);
	}
};
} /* namespacec trap */ 