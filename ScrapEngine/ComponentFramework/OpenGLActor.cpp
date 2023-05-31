#define _CRTDBG_MAP_ALLOC
#include "OpenGLActor.h"
#include "OpenGLMaterial.h"
#include "OpenGLMesh.h"
#include "OpenGLShader.h"
#include "TransformComponent.h"
#include "Debug.h"

OpenGLActor::OpenGLActor(Component* parent_, const char* actorName_): Actor(parent_, actorName_) {

}

OpenGLActor::~OpenGLActor(){
	OnDestroy();
}

bool OpenGLActor::OnCreate(){
	if (isCreated) return isCreated;
	isCreated = true;
	//Check for mesh, meterial, shader, and transform
	Ref<OpenGLMesh> mesh_ = GetComponent<OpenGLMesh>();
	Ref<OpenGLShader> shader_ = GetComponent<OpenGLShader>();
	Ref<OpenGLMaterial> material_ = GetComponent<OpenGLMaterial>();
	Ref<TransformComponent> transform_ = GetComponent<TransformComponent>();

	if (!mesh_ || !shader_ || !material_ || !transform_) {
		Debug::Error("Failed to Load Assets for OpenGLActor: " + std::string(GetName()), __FILE__, __LINE__);
		isCreated = false;
	}
	setAlpha(1.0f);
	return isCreated;
}

void OpenGLActor::OnDestroy(){
	if (!isCreated) return;
	Debug::Info("Deleting assets for OpenGLActor: ", __FILE__, __LINE__);
	for (auto component : components) {
		component->OnDestroy();
	}
	isCreated = false;
}


void OpenGLActor::Update(const float deltaTime_)
{
}

void OpenGLActor::Render() const{

}

void OpenGLActor::HandleEvents(const SDL_Event& sdlEvent)
{
}

void OpenGLActor::ChangeTexture(Ref<MaterialComponent> texture_){
	if (texture_) {
		RemoveComponent<MaterialComponent>();
		AddComponent<MaterialComponent>(texture_);
		Debug::Info((std::string)GetName() + "'s texture changed to " + (std::string)texture_->GetName(), __FILE__, __LINE__);
	}

}

void OpenGLActor::ChangeShader(Ref<ShaderComponent> shader_){
	if (shader_) {
		RemoveComponent<ShaderComponent>();
		AddComponent<ShaderComponent>(shader_);
		//Clean up material binding
		Debug::Info((std::string)GetName() + "'s shader changed to " + (std::string)shader_->GetName(), __FILE__, __LINE__);
	}
}
