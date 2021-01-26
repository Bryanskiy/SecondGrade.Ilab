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

    struct QueueFamilyIndices {
        std::optional<uint32_t> family;
        bool isComplete() {return family.has_value();}
    };

    GLFWwindow*               window = nullptr;
    VkInstance                instance;
    VkDebugUtilsMessengerEXT  debugMessenger;
    VkPhysicalDevice          physicalDevice = VK_NULL_HANDLE;
    VkDevice                  device;
    VkQueue                   queue;
    VkSurfaceKHR              surface;

    void                      initWindow();
    void                      initVulkan();
    void                      createInstance();
    void                      setupDebugMessenger();
    void                      createSurface();
    void                      pickPhysicalDevice();
    void                      createLogicalDevice();
    void                      mainLoop();
    void                      cleanup();

    bool                      isDeviceSuitable(VkPhysicalDevice device);
    QueueFamilyIndices        findQueueFamilies(VkPhysicalDevice device);

public:

    ~Vkdriver();
    void run();
};

}