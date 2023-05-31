#include "Actor.h"
#include "Debug.h"
#include <SDL.h>
#include "TransformComponent.h"
#include "OpenGLMesh.h"
#include "OpenGLShader.h"
#include "OpenGLMaterial.h"

Actor::Actor(Component* parent_, const char* actorName_): Component(parent_, actorName_), hasLifeTime(false), lifeTime(0.0f), 
																initialAlpha(1.0f), alphaChange(0.1f),visibility(true) {}

Actor::Actor(Component* parent_):Component(parent_, ""), hasLifeTime(false), lifeTime(0.0f),
	initialAlpha(1.0f), alphaChange(0.1f), visibility(true) {}

//Deep Copy constructor
Actor::Actor(const Actor& actor_){
	std::vector<Ref<Component>> new_List = std::vector<Ref<Component>>();
	new_List = actor_.GetComponentsList();
	components = new_List;
	parent = actor_.GetParent();
	//alpha = actor_.GetAlpha();
	//alphaChange = actor_.GetAlphaChange();
	//flash = actor_.IsFlashing();
	//updateInterval = actor_.GetUpdateInterval();
	//elapsedTime = 0.0f;
	//currentFlash = flash;
	//actorType = actor_.GetActorType();
}

bool Actor::OnCreate() {
	if (isCreated) return isCreated;

	for (auto component : components) {
		if (component->OnCreate() == false) {
			std::cout << typeid(this).name() << "\n";
			Debug::Error("Loading assets for Actor/Components: ", __FILE__, __LINE__);
			isCreated = false;
		}
	}

	isCreated = true;
	return isCreated;
}

Actor::~Actor() {
	OnDestroy();
}

void Actor::OnDestroy() {
	//RemoveAllComponents();
	if (!isCreated) return;
	Debug::Info("Deleting assets for Actor:" + std::string(GetName()), __FILE__, __LINE__);
	isCreated = false;
}

void Actor::Update(const float deltaTime) {
	for (auto component : components) {
		component->Update(deltaTime);
	}

	GetModelMatrix();
}

void Actor::HandleEvents(const SDL_Event& sdlEvent){
}

void Actor::Render()const {

}

void Actor::RemoveAllComponents() {
	for (auto component : components) {
		component->OnDestroy();
	}
	components.clear();
}

void Actor::ListComponents() const {
	std::cout << typeid(*this).name() << " contains the following components:\n";
	for (auto component : components) {
		std::cout << typeid(*component).name() << std::endl;
	}
	std::cout << '\n';
}

void Actor::SetModelMatrix(Matrix4 modelMatrix_){
	modelMatrix.normalMatrix = MMath::transpose(MMath::inverse(modelMatrix_));
	modelMatrix.modelMatrix = modelMatrix_;
}

ModelMatrix Actor::GetModelMatrix() {
	Matrix4 modelMatrix_;
	Ref<TransformComponent> transformComponent = GetComponent<TransformComponent>();
	if (transformComponent) { /// There is no such component, use default view
		modelMatrix_ = transformComponent->GetTransformMatrix();
	}
	else {
		modelMatrix_.loadIdentity();
	}

	if (parent) {
		modelMatrix_ = dynamic_cast<Actor*>(parent)->GetModelMatrix().modelMatrix * modelMatrix_;
	}
	SetModelMatrix(modelMatrix_);
	return modelMatrix;
}