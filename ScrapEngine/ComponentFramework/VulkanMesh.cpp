#include "VulkanMesh.h"

VulkanMesh::VulkanMesh(Component* parent_, const char* componentName_, const char* filename_): MeshComponent(parent_, componentName_, filename_), index(nullptr), vertex(nullptr) {
}

VulkanMesh::~VulkanMesh(){
	OnDestroy();
}

bool VulkanMesh::OnCreate(){
	if (isCreated) return isCreated;
	vertex = std::make_shared<std::pair<VkBuffer, VkDeviceMemory>>();
	index = std::make_shared<std::pair<VkBuffer, VkDeviceMemory>>();
	isCreated = true;
	return isCreated;
}

void VulkanMesh::OnDestroy(){
	if (!isCreated) return;

	isCreated = false;
}

void VulkanMesh::Update(const float deltaTime_){
}

void VulkanMesh::Render() const{
}
