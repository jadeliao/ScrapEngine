#ifndef VULKANRENDERER_H 
#define VULKANRENDERER_H

#include <SDL_vulkan.h>
#include <SDL_image.h>
#include <vulkan/vulkan.h>
#include <iostream>
#include <fstream>
#include <memory>
#include <stdexcept>
#include <algorithm>
#include <cstring>
#include <cstdlib>
#include <cstdint>
#include <optional>
#include <set>
#include <array>
#include <chrono>
#include <string>

#include "Vector.h"
#include "VMath.h"
#include "MMath.h"
#include "Hash.h"

using namespace MATH;

#include "Renderer.h"
#include "VulkanMaterial.h"
#include "VulkanMesh.h"
#include "VulkanShader.h"
#include "VulkanActor.h"

const std::vector<const char*> validationLayers = {
    "VK_LAYER_KHRONOS_validation"
};

const std::vector<const char*> deviceExtensions = {
    VK_KHR_SWAPCHAIN_EXTENSION_NAME
};

#ifdef NDEBUG /// only use validation layers if in debug mode
const bool enableValidationLayers = false;
#else
const bool enableValidationLayers = false;
#endif

struct QueueFamilyIndices {
    /// optional means that it contains no value until it is assigned
    std::optional<uint32_t> graphicsFamily;
    std::optional<uint32_t> presentFamily;

    bool isComplete() {
        return graphicsFamily.has_value() && presentFamily.has_value();
    }
};

struct SwapChainSupportDetails {
    VkSurfaceCapabilitiesKHR capabilities;
    std::vector<VkSurfaceFormatKHR> formats;
    std::vector<VkPresentModeKHR> presentModes;
};

namespace std {
    template<> struct hash<Vertex> {
        size_t operator()(Vertex const& vertex) const noexcept {
            size_t hash1 = hash<Vec3>()(vertex.pos);
            size_t hash2 = hash<Vec3>()(vertex.normal);
            size_t hash3 = hash<Vec2>()(vertex.texCoord);
            size_t result = ((hash1 ^ (hash2 << 1)) >> 1) ^ (hash3 << 1);
            return result;
        }
    };
}

struct BufferInfo {
    std::vector<VkBuffer> buffers;
    std::vector<VkDeviceMemory> buffersMemory;

};

class VulkanRenderer : public Renderer {
public:
    /// C11 precautions
    VulkanRenderer(const VulkanRenderer&) = delete;  /// Copy constructor
    VulkanRenderer(VulkanRenderer&&) = delete;       /// Move constructor
    VulkanRenderer& operator=(const VulkanRenderer&) = delete; /// Copy operator
    VulkanRenderer& operator=(VulkanRenderer&&) = delete;      /// Move operator

    ~VulkanRenderer();
    SDL_Window* BuildWindow(std::string name_, int width, int height);
    bool OnCreate();
    void OnDestroy();
    void Render();
    void SetCameraUBO(const Matrix4& projection, const Matrix4& view);
    void SetGLightsUBO(const GlobalLight& glights);
    void SetGeometryUBO(const GeometryInfo& geometryInfo_);
    void ResizeEvent();
    void InitImGui() override;
    bool AddActor(const char* actorName_, Ref<Actor> actor_) override;
    void LoadTexture(const char* textureName_, const char* imageName_) override;
    void LoadModel(const char* modelName_, const char* modelFile_) override;
    void LoadShader(const char* shaderName_, const char* vertFile_, const char* fragFile_, const char* geomFile_ = "") override;
    void DrawRay(Vec3 start, Vec3 end, float lifeTime = 1.0f);

    SDL_Window* GetWindow() {
        return window;
    }

    static Renderer* Initialize() {
        if (!instance_) {
            return new VulkanRenderer ();
        }
        return nullptr;
    }

private:
    VulkanRenderer();
    //Variables for UI
    VkDescriptorPool uiDescriptorPool;
    const size_t MAX_FRAMES_IN_FLIGHT = 2;
    SDL_Event sdlEvent;

    SDL_Window* window;
    VkInstance instance;
    VkDebugUtilsMessengerEXT debugMessenger;
    VkSurfaceKHR surface;
    VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;
    VkDevice device;
    VkRenderPass renderPass;
    VkImage depthImage;
    VkDeviceMemory depthImageMemory;
    VkImageView depthImageView;

    VkCommandPool commandPool;

    //DrawNormal UBO
    GeometryInfo geometryUbo;
    BufferInfo geometryBuffer;
    CameraMatrices cameraUbo;//object ubo that stores projection matrix, view matrix and stuff
    BufferInfo cameraBuffer;
    GlobalLight glightUbo;
    BufferInfo glightBuffer;
    //std::vector<VkBuffer> cameraBuffers;
    //std::vector<VkDeviceMemory> cameraBuffersMemory;
    //std::vector<VkBuffer> glightBuffers;
    //std::vector<VkDeviceMemory> glightBuffersMemory;

    std::vector<VkCommandBuffer> commandBuffers;
    std::vector<VkSemaphore> imageAvailableSemaphores;
    std::vector<VkSemaphore> renderFinishedSemaphores;
    std::vector<VkFence> inFlightFences;
    std::vector<VkFence> imagesInFlight;
    size_t currentFrame = 0;

    std::string TEXTURE_PATH = "./textures/mario_main.png"; //mario_main.png has an incorrect size
    std::string MODEL_PATH = "./meshes/Mario.obj";

    bool hasStencilComponent(VkFormat format);

    /*Bind the material and pipeline within the actor 
      This is needed to make sure descriptor pool and sets are binded properly*/
    void bindTexture(Ref<VulkanActor> actor_);
    void initVulkan();
    void createInstance();
    void createSurface();
    void createLogicalDevice();
    void createSwapChain();
    void createImageViews();
    void recreateSwapChain();
    void updateCameraUniformBuffer(uint32_t currentImage);
    void updateGLightsUniformBuffer(uint32_t currentImage);
    void updateGeometryUniformBuffer(uint32_t currentImage);
    VkImageView createImageView(VkImage image, VkFormat format, VkImageAspectFlags aspectFlags);
    void createRenderPass();

    void createGraphicsPipeline(VulkanShader& vulkanPipeline_,
        const char* vertFile_, const char* fragFile_, const char* geomFile_="");

    void createFramebuffers();
    void createCommandPool();
    void createDepthResources();
    void createTextureImage(const char* fileName_, VkImage& textureImage_, VkDeviceMemory& textureMemory_);
    void createTextureImageView(VulkanMaterial& vulkanMaterial_);
    void createTextureSampler(VkSampler& textureSampler_);
    void createImage(uint32_t width, uint32_t height, VkFormat format, VkImageTiling tiling,
        VkImageUsageFlags usage, VkMemoryPropertyFlags properties, VkImage& image, VkDeviceMemory& imageMemory);
    void loadModel(VulkanMesh& vulkanMesh_);
    void createVertexBuffer(VulkanMesh& vulkanMesh_);
    /// A helper function for createVertexBuffer()
    uint32_t findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties);
    void createIndexBuffer(VulkanMesh& vulkanMesh_);
    void createUniformBuffers(VkDeviceSize bufferSize, std::vector<VkBuffer>& uniformBuffer,
                                std::vector<VkDeviceMemory>& uniformBufferMemory);
    void destroyUniformBuffer(std::vector<VkBuffer>& uniformBuffer,
        std::vector<VkDeviceMemory>& uniformBufferMemory);
 
    void createDescriptorSetLayout(VkDescriptorSetLayout& descriptorSetLayout_);
    void createDescriptorPool(VkDescriptorPool& descriptorPool_);
    void createDescriptorSets(VulkanMaterial& VulkanMaterial_, MaterialBinding& vulkanDescriptor_,
                                VkDescriptorSetLayout& descriptorSetLayout_);
    void createBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer& buffer, VkDeviceMemory& bufferMemory);
    void copyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size);
    void createCommandBuffers();
    void recordCommandBuffer();
    void createSyncObjects();
    void cleanup();
    void cleanupSwapChain();
    void transitionImageLayout(VkImage image, VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout);
    void copyBufferToImage(VkBuffer buffer, VkImage image, uint32_t width, uint32_t height);

    VkFormat findDepthFormat();
    VkFormat findSupportedFormat(const std::vector<VkFormat>& candidates, VkImageTiling tiling, VkFormatFeatureFlags features);

    VkResult CreateDebugUtilsMessengerEXT(VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDebugUtilsMessengerEXT* pDebugMessenger);
    void DestroyDebugUtilsMessengerEXT(VkInstance instance, VkDebugUtilsMessengerEXT debugMessenger, const VkAllocationCallbacks* pAllocator);
    void  populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo);
    void setupDebugMessenger();

    static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity, VkDebugUtilsMessageTypeFlagsEXT messageType, const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData, void* pUserData);

    void pickPhysicalDevice();
    bool isDeviceSuitable(VkPhysicalDevice device);
    bool checkDeviceExtensionSupport(VkPhysicalDevice device);
    std::vector<const char*> getRequiredExtensions();
    bool checkValidationLayerSupport();

    VkQueue graphicsQueue;
    VkQueue presentQueue;

    VkCommandBuffer beginSingleTimeCommands();
    void endSingleTimeCommands(VkCommandBuffer commandBuffer);

    QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device);

    VkSwapchainKHR swapChain;
    std::vector<VkImage> swapChainImages;
    VkFormat swapChainImageFormat;
    VkExtent2D swapChainExtent;
    std::vector<VkImageView> swapChainImageViews;
    std::vector<VkFramebuffer> swapChainFramebuffers;

    VkShaderModule createShaderModule(const std::vector<char>& code);
    VkSurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);
    VkPresentModeKHR chooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes);
    VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities);
    SwapChainSupportDetails querySwapChainSupport(VkPhysicalDevice device);

    static std::vector<char> readFile(const std::string& filename);

   
};
#endif 

