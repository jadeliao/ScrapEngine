#ifndef VULKANACTOR_H
#define VULKANACTOR_H

#include "Actor.h"
#include <SDL.h>
#include <SDL_vulkan.h>
#include <SDL_image.h>
#include <vulkan/vulkan.h>
#include <unordered_map>
#include "VulkanMaterial.h"
#include "VulkanShader.h"
#include "VulkanMesh.h"
#include "TransformComponent.h"

struct MaterialBinding {

	std::vector<VkDescriptorSet> descriptorSets;
	VkDescriptorPool descriptorPool;
	Ref<VulkanMaterial> material;
	Ref<VulkanShader> shader;

	bool isValid() {
		return (descriptorPool != 0) &&
				(!descriptorSets.empty());
	}
};

class VulkanActor : public Actor {

private:
	Ref<MaterialBinding> materialBinding;
	const char* currentTexture;
	bool materialBinded;

public:

	VulkanActor(Component* parent_, const char* actorName_);
	//VulkanActor(const VulkanActor& actor_);
	~VulkanActor() override;
	bool OnCreate() override;
	void OnDestroy() override;
	void Update(const float deltaTime_) override;
	void Render() const;
	virtual void HandleEvents(const SDL_Event& sdlEvent);

	void ChangeTexture(Ref<MaterialComponent> texture_) override;
	void ChangeShader(Ref<ShaderComponent> shader_) override;

	Ref<MaterialBinding> getBinding();
	bool isBinded() { return materialBinded; }
	void setBinded(bool binded_) { materialBinded = binded_; }
};


#endif