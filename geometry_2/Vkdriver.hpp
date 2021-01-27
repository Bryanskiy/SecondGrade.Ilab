#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <stdexcept>
#include <vector>
#include <cstring>
#include <iostream>
#include <fstream>
#include <optional>
#include <set>
#include <string>

namespace vkdriver {

class Vkdriver {

private:

    const uint32_t WIDTH = 800;
    const uint32_t HEIGHT = 600;

    const std::vector<const char*> validationLayers = {
        "VK_LAYER_KHRONOS_validation"
    };

    const std::vector<const char*> deviceExtensions = {
        VK_KHR_SWAPCHAIN_EXTENSION_NAME
    };

#ifdef NDEBUG
const bool enableValidationLayers = false;
#else
const bool enableValidationLayers = true;
#endif

    struct QueueFamilyIndices {
        std::optional<uint32_t> family;
        bool isComplete() {return family.has_value();}
    };
    
    struct SwapChainSupportDetails {
        VkSurfaceCapabilitiesKHR capabilities;
        std::vector<VkSurfaceFormatKHR> formats;
        std::vector<VkPresentModeKHR> presentModes;
    };

    GLFWwindow*               window = nullptr;
    VkInstance                instance;
    VkDebugUtilsMessengerEXT  debugMessenger;
    VkPhysicalDevice          physicalDevice = VK_NULL_HANDLE;
    VkDevice                  device;
    VkQueue                   queue;
    VkSurfaceKHR              surface;
    VkSwapchainKHR            swapChain;
    std::vector<VkImage>      swapChainImages;
    VkFormat                  swapChainImageFormat;
    VkExtent2D                swapChainExtent;
    std::vector<VkImageView>  swapChainImageViews;
    VkPipelineLayout          pipelineLayout;

    void                      createGraphicsPipeline();
    void                      initWindow();
    void                      initVulkan();
    void                      createInstance();
    void                      setupDebugMessenger();
    void                      createSurface();
    void                      pickPhysicalDevice();
    void                      createLogicalDevice();
    void                      createSwapChain();
    void                      createImageViews();
    void                      mainLoop();
    void                      cleanup();

    bool                      checkValidationLayerSupport();
    bool                      isDeviceSuitable(VkPhysicalDevice device);
    std::vector<const char*>  getRequiredExtensions();
    QueueFamilyIndices        findQueueFamilies(VkPhysicalDevice device);
    bool                      checkDeviceExtensionSupport(VkPhysicalDevice device);
    SwapChainSupportDetails   querySwapChainSupport(VkPhysicalDevice device);
    VkSurfaceFormatKHR        chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);
    VkPresentModeKHR          chooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes);
    VkExtent2D                chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities);
    VkShaderModule            createShaderModule(const std::vector<char>& code);

public:

    ~Vkdriver();
    void run();
};

}