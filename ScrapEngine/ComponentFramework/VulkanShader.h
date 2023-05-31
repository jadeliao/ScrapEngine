#ifndef VULKANSHADER_H
#define VULKANSHADER_H


#include <SDL.h>
#include <SDL_vulkan.h>
#include <SDL_image.h>
#include <vulkan/vulkan.h>

#include "ShaderComponent.h"

class VulkanShader : public ShaderComponent{
private:

public:
    VkPipeline graphicsPipeline;
    VkPipelineLayout pipelineLayout;
    VkDescriptorSetLayout descriptorSetLayout;
    VulkanShader(Component* parent_, const char* componentName_, const char* vsFilename_, const char* fsFilename_);
    VulkanShader(Component* parent_, const char* componentName_, const char* vsFilename_, const char* fsFilename_, const char* gsFilename_);
    virtual ~VulkanShader();
    virtual bool OnCreate() override;
    virtual void OnDestroy() override;
    virtual void Update(const float deltaTime_) override;
    virtual void Render()const override;

    bool isValid() {
        return (graphicsPipeline != 0) &&
            (pipelineLayout != 0) &&
            (descriptorSetLayout != 0);
    }
};

#endif
