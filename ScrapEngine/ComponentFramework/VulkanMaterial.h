#ifndef VULKANMATERIAL_H
#define VULKANMATERIAL_H

#include "MaterialComponent.h"
#include <SDL.h>
#include <SDL_vulkan.h>
#include <SDL_image.h>
#include <vulkan/vulkan.h>

class VulkanMaterial : public MaterialComponent {

private:

public:
    VulkanMaterial(Component* parent_, const char* componentName_, const char* filename_);
    virtual ~VulkanMaterial();
    virtual bool OnCreate() override;
    virtual void OnDestroy() override;
    virtual void Update(const float deltaTime_) override;
    virtual void Render()const override;

    VkImageView textureImageView;
    Ref<VkSampler> textureSampler;
    Ref<VkImage> textureImage;
    Ref<VkDeviceMemory> textureMemory;

 /*   VkImageView* getImageView() const { return textureImageView; }
    VkSampler* getTextureSampler() const { return textureSampler; }
    VkImage* getImage() const { return textureImage; }
    VkDeviceMemory* getMemory() const { return textureMemory; }
    void setImage(VkImage* image_) { textureImage = image_; }
    void setImageView(VkImageView* imageView_) { textureImageView = imageView_; }
    void setTextureSampler(VkSampler* sampler_) { textureSampler = sampler_; }
    void setMemory(VkDeviceMemory* memory_) { textureMemory = memory_; }*/

    bool isValid() {
        return (textureImageView != 0) &&
            (textureSampler != 0) &&
            (textureImage != 0) &&
            (textureMemory != 0);
    }
};

#endif

