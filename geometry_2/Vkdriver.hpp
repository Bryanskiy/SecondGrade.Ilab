#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <array>
#include <stdexcept>
#include <vector>
#include <cstring>
#include <iostream>
#include <fstream>
#include <optional>
#include <set>
#include <string>
#include <chrono>
#include "camera.hpp"

namespace vkdriver {

struct Vertex {
    glm::vec3 pos;
    glm::vec3 color;
    glm::vec3 normal;
 
    static VkVertexInputBindingDescription getBindingDescription() {
        VkVertexInputBindingDescription bindingDescription{};
        bindingDescription.binding = 0;
        bindingDescription.stride = sizeof(Vertex);
        bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

        return bindingDescription;
    }

    static std::array<VkVertexInputAttributeDescription, 3> getAttributeDescriptions() {
        std::array<VkVertexInputAttributeDescription, 3> attributeDescriptions{};

		attributeDescriptions[0].binding = 0;
		attributeDescriptions[0].location = 0;
		attributeDescriptions[0].format = VK_FORMAT_R32G32B32_SFLOAT;
		attributeDescriptions[0].offset = offsetof(Vertex, pos);

		attributeDescriptions[1].binding = 0;
		attributeDescriptions[1].location = 1;
		attributeDescriptions[1].format = VK_FORMAT_R32G32B32_SFLOAT;
		attributeDescriptions[1].offset = offsetof(Vertex, color);

        attributeDescriptions[2].binding = 0;
		attributeDescriptions[2].location = 2;
		attributeDescriptions[2].format = VK_FORMAT_R32G32B32_SFLOAT;
		attributeDescriptions[2].offset = offsetof(Vertex, normal);

        return attributeDescriptions;
    }
};

struct UniformBufferObject {
    glm::mat4 model;
    glm::mat4 view;
    glm::mat4 proj;
};

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

    GLFWwindow*                     window = nullptr;
    VkInstance                      instance;
    VkDebugUtilsMessengerEXT        debugMessenger;
    VkPhysicalDevice                physicalDevice = VK_NULL_HANDLE;
    VkDevice                        device;
    VkQueue                         queue;
    VkSurfaceKHR                    surface;
    VkSwapchainKHR                  swapChain;
    std::vector<VkImage>            swapChainImages;
    VkFormat                        swapChainImageFormat;
    VkExtent2D                      swapChainExtent;
    std::vector<VkImageView>        swapChainImageViews;
    VkRenderPass                    renderPass;
    VkPipelineLayout                pipelineLayout;
    VkPipeline                      graphicsPipeline;
    std::vector<VkFramebuffer>      swapChainFramebuffers;
    VkCommandPool                   commandPool;
    std::vector<VkCommandBuffer>    commandBuffers;
    std::vector<VkSemaphore>        imageAvailableSemaphores;
    std::vector<VkSemaphore>        renderFinishedSemaphores;
    std::vector<VkFence>            inFlightFences;
    std::vector<VkFence>            imagesInFlight;
    size_t                          currentFrame = 0;
    const int                       MAX_FRAMES_IN_FLIGHT = 2;
    bool                            framebufferResized = false;
    VkBuffer                        vertexBuffer;
    VkDeviceMemory                  vertexBufferMemory;
    VkDescriptorSetLayout           descriptorSetLayout;
    std::vector<VkBuffer>           uniformBuffers;
    std::vector<VkDeviceMemory>     uniformBuffersMemory;
    VkDescriptorPool                descriptorPool;
    std::vector<VkDescriptorSet>    descriptorSets;
    VkImage                         depthImage;
    VkDeviceMemory                  depthImageMemory;
    VkImageView                     depthImageView;
    std::vector<Vertex>             vertices;
    Camera                          camera;

    void                            createDepthResources();
    void                            createDescriptorSetLayout();
    void                            createVertexBuffer();
    void                            cleanupSwapChain();
    void                            createSyncObjects();
    void                            createCommandBuffers();
    void                            createCommandPool();
    void                            createFramebuffers();
    void                            createRenderPass();
    void                            createGraphicsPipeline();
    void                            initWindow();
    void                            initVulkan();
    void                            createInstance();
    void                            setupDebugMessenger();
    void                            createSurface();
    void                            pickPhysicalDevice();
    void                            createLogicalDevice();
    void                            createSwapChain();
    void                            createImageViews();
    void                            mainLoop();
    void                            cleanup();
    void                            recreateSwapChain();
    void                            drawFrame();
    void                            createUniformBuffers();
    void                            createDescriptorPool();
    void                            createDescriptorSets();
    void                            updateUniformBuffer(uint32_t currentImage);
    void                            createBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer& buffer, VkDeviceMemory& bufferMemory);
    void                            copyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size);
    uint32_t                        findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties);
    bool                            checkValidationLayerSupport();
    bool                            isDeviceSuitable(VkPhysicalDevice device);
    std::vector<const char*>        getRequiredExtensions();
    QueueFamilyIndices              findQueueFamilies(VkPhysicalDevice device);
    bool                            checkDeviceExtensionSupport(VkPhysicalDevice device);
    SwapChainSupportDetails         querySwapChainSupport(VkPhysicalDevice device);
    VkSurfaceFormatKHR              chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);
    VkPresentModeKHR                chooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes);
    VkExtent2D                      chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities);
    VkShaderModule                  createShaderModule(const std::vector<char>& code);
    VkFormat                        findSupportedFormat(const std::vector<VkFormat>& candidates, VkImageTiling tiling, VkFormatFeatureFlags features);
    VkFormat                        findDepthFormat();
    void                            createImage(uint32_t width, uint32_t height, VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage, VkMemoryPropertyFlags properties, VkImage& image, VkDeviceMemory& imageMemory);
    VkImageView                     createImageView(VkImage image, VkFormat format, VkImageAspectFlags aspectFlags);

    static void                     keyCallback(GLFWwindow* window, int key, int scancode, int action, int mode);
    static void                     framebufferResizeCallback(GLFWwindow* window, int width, int height);
    static void                     mouseButtonCallback(GLFWwindow* window, int button, int action, int mods);

public:

    ~Vkdriver();
    void run(const std::vector<Vertex>& vertex_array);
};

}