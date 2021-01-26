#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <stdexcept>
#include <vector>
#include <cstring>
#include <iostream>
#include <optional>

namespace vkdriver {

class Vkdriver {

private:
    GLFWwindow*               window = nullptr;
    VkInstance                instance;
    VkDebugUtilsMessengerEXT  debugMessenger;
    VkPhysicalDevice          physicalDevice = VK_NULL_HANDLE;

    void initWindow();
    void initVulkan();
    void createInstance();
    void setupDebugMessenger();
    void pickPhysicalDevice();

    void mainLoop();
    void cleanup();

public:

    ~Vkdriver();
    void run();
};

}