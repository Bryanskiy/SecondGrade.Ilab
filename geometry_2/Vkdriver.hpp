#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <array>
#include <stdexcept>
#include <vector>
#include <cstring>
#include <iostream>
#include <fstream>
#include <optional>
#include <set>
#include <glm/gtc/matrix_transform.hpp>
#include <string>
#include <chrono>

namespace vkdriver {

struct Vertex {
    glm::vec2 pos;
    glm::vec3 color;

    static VkVertexInputBindingDescription getBindingDescription() {
        VkVertexInputBindingDescription bindingDescription{};
        bindingDescription.binding = 0;
        bindingDescription.stride = sizeof(Vertex);
        bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

        return bindingDescription;
    }

    static std::array<VkVertexInputAttributeDescription, 2> getAttributeDescriptions() {
        std::array<VkVertexInputAttributeDescription, 2> attributeDescriptions{};

        attributeDescriptions[0].binding = 0;
        attributeDescriptions[0].location = 0;
        attributeDescriptions[0].format = VK_FORMAT_R32G32_SFLOAT;
        attributeDescriptions[0].offset = offsetof(Vertex, pos);

        attributeDescriptions[1].binding = 0;
        attributeDescriptions[1].location = 1;
        attributeDescriptions[1].format = VK_FORMAT_R32G32B32_SFLOAT;
        attributeDescriptions[1].offset = offsetof(Vertex, color);

        return attributeDescriptions;
    }
};

struct UniformBufferObject {
    glm::mat4 model;
    glm::mat4 view;
    glm::mat4 proj;
};

const std::vector<Vertex> vertices = {
    {{0.0f, -0.5f}, {0.3f, 0.7f, 1.0f}},
    {{0.5f, 0.5f}, {0.0f, 1.0f, 0.0f}},
    {{-0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}},
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

    static void                     framebufferResizeCallback(GLFWwindow* window, int width, int height);

public:

    ~Vkdriver();
    void run();
};

}