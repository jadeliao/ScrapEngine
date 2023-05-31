#include "VulkanActor.h"
#include "Debug.h"

VulkanActor::VulkanActor(Component* parent_, const char* actorName_) : Actor(parent_, actorName_), currentTexture("None"), materialBinded(false) {

}

VulkanActor::~VulkanActor(){
	OnDestroy();
}

bool VulkanActor::OnCreate(){
	if (isCreated) return isCreated;
	materialBinding = std::make_shared<MaterialBinding>();
	isCreated = true;
	//Check for mesh, meterial, shader, and transform
	Ref<MeshComponent> mesh_ = GetComponent<MeshComponent>();
	Ref<ShaderComponent> shader_ = GetComponent<ShaderComponent>();
	Ref<MaterialComponent> material_ = GetComponent<MaterialComponent>();
	Ref<TransformComponent> transform_ = GetComponent<TransformComponent>();
	setAlpha(initialAlpha);

	if (!mesh_ || !shader_ || !material_ || !transform_) {
		Debug::Error("Failed to Load Assets for VulkanActor:\n		" + std::string(GetName()), __FILE__, __LINE__);
		isCreated = false;
	}
	else {
		Debug::Info("Succeed to Load Assets for VulkanActor:\n		" + std::string(GetName()), __FILE__, __LINE__);
		isCreated = true;
	}

	return isCreated;
}

void VulkanActor::OnDestroy(){
	if (!isCreated) return;
	//if (materialBinding) delete materialBinding;
	//for (auto material_ : materials) {
	//	if (material_.second) delete material_.second;
	//}
	//Debug::Info("Deleting materials for VulkanActor: ", __FILE__, __LINE__);
	//RemoveAllComponents();
	Debug::Info("Deleting assets for VulkanActor: ", __FILE__, __LINE__);

	isCreated = false;
}

void VulkanActor::Update(const float deltaTime_){
}

void VulkanActor::Render() const{
}

void VulkanActor::HandleEvents(const SDL_Event& sdlEvent){

}

void VulkanActor::ChangeTexture(Ref<MaterialComponent> texture_) {

	if (texture_) {
		RemoveComponent<MaterialComponent>();
		AddComponent<MaterialComponent>(texture_);
		//Clean up material binding
		materialBinding->material = 0;
		materialBinded = false;
		Debug::Info((std::string)GetName() + "'s texture changed to " + (std::string)texture_->GetName(), __FILE__, __LINE__);
	}

}

void VulkanActor::ChangeShader(Ref<ShaderComponent> shader_) {

	if (shader_) {
		RemoveComponent<ShaderComponent>();
		AddComponent<ShaderComponent>(shader_);
		//Clean up material binding
		materialBinding->shader = 0;
		materialBinded = false;
		Debug::Info((std::string)GetName() + "'s shader changed to " + (std::string)shader_->GetName(), __FILE__, __LINE__);
	}

}

Ref<MaterialBinding> VulkanActor::getBinding(){
	return materialBinding;
}

