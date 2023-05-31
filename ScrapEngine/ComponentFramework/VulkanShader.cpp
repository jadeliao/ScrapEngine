#include "VulkanShader.h"


VulkanShader::VulkanShader(Component* parent_, const char* componentName_, const char* vsFilename_, const char* fsFilename_):
							ShaderComponent(parent_, componentName_, vsFilename_, fsFilename_), 
							descriptorSetLayout(0), graphicsPipeline(0),pipelineLayout(0) {

}

VulkanShader::VulkanShader(Component* parent_, const char* componentName_, const char* vsFilename_, const char* fsFilename_, const char* gsFilename_):
							ShaderComponent(parent_, componentName_, vsFilename_, fsFilename_, gsFilename_),
							descriptorSetLayout(0), graphicsPipeline(0), pipelineLayout(0) {
}


VulkanShader::~VulkanShader() {
	OnDestroy();
}

bool VulkanShader::OnCreate() {
	if (isCreated) return isCreated;

	return isCreated;
}

void VulkanShader::OnDestroy() {
	if (!isCreated) return;

	isCreated = false;
}

void VulkanShader::Update(const float deltaTime_) {

}

void VulkanShader::Render() const {

}