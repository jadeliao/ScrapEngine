#include "VulkanMaterial.h"

VulkanMaterial::VulkanMaterial(Component* parent_, const char* componentName_, const char* filename_): MaterialComponent(parent_, componentName_, filename_),
				textureImage(0), textureImageView(0), textureSampler(0), textureMemory(0) {
}

VulkanMaterial::~VulkanMaterial(){
	OnDestroy();
}

bool VulkanMaterial::OnCreate(){
	if (isCreated) return isCreated;

	textureSampler = std::make_shared<VkSampler>();
	textureImage = std::make_shared<VkImage>();
	textureMemory = std::make_shared<VkDeviceMemory>();

	return isCreated;
}

void VulkanMaterial::OnDestroy(){
	if (!isCreated) return;

	//if (textureSampler) delete textureSampler;
	//if (textureImage) delete textureImage;
	//if (textureMemory) delete textureMemory;

	isCreated = false;
}

void VulkanMaterial::Update(const float deltaTime_)
{
}

void VulkanMaterial::Render() const
{
}
