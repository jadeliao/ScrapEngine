#include <glew.h>
#include <SDL.h>
#include "CameraActor.h"
#include "MMath.h"
#include "QMath.h"
#include "TransformComponent.h"
#include "Debug.h"
//#include "UBO_Padding.h"
using namespace MATH;

CameraActor::CameraActor(Component* parent_): Actor(parent_) {
	projectionMatrix.loadIdentity();
	viewMatrix.loadIdentity();
}

CameraActor::~CameraActor() {
	OnDestroy();
}

bool CameraActor::OnCreate() {
	if (isCreated) 	return isCreated; //or return true

	//viewMatrix = MMath::rotate(0.0f, Vec3(0.0f, 1.0f, 0.0f)) * MMath::translate(0.0f, 0.0f, -5.0f);
	UpdateProjectionMatrix(45.0f, (16.0f / 9.0f), 0.5f, 100.0f); /// default projection
	UpdateViewMatrix();

	isCreated = true;
	return isCreated;
}

void CameraActor::OnDestroy(){
	isCreated = false;
}

void CameraActor::Render() const {

}

void CameraActor::Update(const float deltaTime){
	UpdateViewMatrix();
}

void CameraActor::HandleEvents(const SDL_Event & sdlEvent){
	switch (sdlEvent.type) {
	case SDL_KEYDOWN:
		if (sdlEvent.key.keysym.scancode == SDL_SCANCODE_LEFT) {
			//std::cout << "LEFT EVENT\n";
			Ref<TransformComponent> transform_ = GetComponent<TransformComponent>();
			transform_->SetTransform(
				transform_->GetPosition() + Vec3(0.1f, 0.0f, 0.0f), transform_->GetQuaternion());
			UpdateViewMatrix();
		}
		else if (sdlEvent.key.keysym.scancode == SDL_SCANCODE_RIGHT) {
			//std::cout << "RIGHT EVENT\n";
			Ref<TransformComponent> transform_ = GetComponent<TransformComponent>();
			transform_->SetTransform(
				transform_->GetPosition() + Vec3(-0.1f, 0.0f, 0.0f), transform_->GetQuaternion());
			UpdateViewMatrix();
		}
		else if (sdlEvent.key.keysym.scancode == SDL_SCANCODE_UP) {
			//std::cout << "UP EVENT\n";
			Ref<TransformComponent> transform_ = GetComponent<TransformComponent>();
			transform_->SetTransform(
				transform_->GetPosition() + Vec3(0.0f, 0.0f, 0.1f), transform_->GetQuaternion());
			UpdateViewMatrix();
		}
		else if (sdlEvent.key.keysym.scancode == SDL_SCANCODE_DOWN) {
			//std::cout << "DOWN EVENT\n";
			Ref<TransformComponent> transform_ = GetComponent<TransformComponent>();
			transform_->SetTransform(
				transform_->GetPosition() + Vec3(0.0f, 0.0f, -0.1f), transform_->GetQuaternion());
			UpdateViewMatrix();
		}
		else if (sdlEvent.key.keysym.scancode == SDL_SCANCODE_A) {
			//std::cout << "A EVENT\n";
			Ref<TransformComponent> transform_ = GetComponent<TransformComponent>();
			transform_->SetTransform(transform_->GetPosition(), QMath::angleAxisRotation(-2.0f, Vec3(0.0f, 1.0f, 0.0f)) * 
				transform_->GetQuaternion());
			UpdateViewMatrix();
		}
		else if (sdlEvent.key.keysym.scancode == SDL_SCANCODE_D) {
			//std::cout << "D EVENT\n";
			Ref<TransformComponent> transform_ = GetComponent<TransformComponent>();
			transform_->SetTransform(transform_->GetPosition(), QMath::angleAxisRotation(2.0f, Vec3(0.0f, 1.0f, 0.0f)) * 
				transform_->GetQuaternion());
			UpdateViewMatrix();
		}
		else if (sdlEvent.key.keysym.scancode == SDL_SCANCODE_W) {
			//std::cout << "W EVENT\n";
			Ref<TransformComponent> transform_ = GetComponent<TransformComponent>();
			transform_->SetTransform(transform_->GetPosition(), QMath::angleAxisRotation(-2.0f, Vec3(1.0f, 0.0f, 0.0f)) *
				transform_->GetQuaternion());
			UpdateViewMatrix();
		}
		else if (sdlEvent.key.keysym.scancode == SDL_SCANCODE_S) {
			//std::cout << "S EVENT\n";
			Ref<TransformComponent> transform_ = GetComponent<TransformComponent>();
			transform_->SetTransform(transform_->GetPosition(), QMath::angleAxisRotation(2.0f, Vec3(1.0f, 0.0f, 0.0f)) *
				transform_->GetQuaternion());
			UpdateViewMatrix();
		}
		break;

	case SDL_MOUSEWHEEL:
		if (sdlEvent.wheel.y > 0) {// scroll up
			Ref<TransformComponent> transform_ = GetComponent<TransformComponent>();
			transform_->SetTransform(
				transform_->GetPosition() + Vec3(0.0f, 0.0f, 0.1f), transform_->GetQuaternion());
			UpdateViewMatrix();
		}
		else if (sdlEvent.wheel.y < 0) { // scroll down
			Ref<TransformComponent> transform_ = GetComponent<TransformComponent>();
			transform_->SetTransform(
				transform_->GetPosition() + Vec3(0.0f, 0.0f, -0.1f), transform_->GetQuaternion());
			UpdateViewMatrix();
		}

	default:
		break;
	}
}

void CameraActor::UpdateViewMatrix() {
	Ref<TransformComponent> transformComponent = GetComponent<TransformComponent>();
	if (transformComponent.get() == nullptr) { /// There is no such component, use default view
		transformComponent = std::make_shared<TransformComponent>(nullptr, Vec3(0.0f, 0.0f, -5.0f), Quaternion());
		AddComponent(transformComponent);
		//viewMatrix = MMath::translate(0.0f, 0.0f, -5.0f) * MMath::rotate(0.0f, Vec3(0.0f, 1.0f, 0.0f));
		viewMatrix = MMath::lookAt(Vec3(0.0f, 0.0f, 5.0f), Vec3(0.0f, 0.0f, 0.0f), Vec3(0.0f, 1.0f, 0.0f));

	}
	else {
		Quaternion orientation = transformComponent->GetQuaternion();
		Vec3 position = transformComponent->GetPosition();
		viewMatrix = QMath::toMatrix4(orientation) * MMath::translate(position);
	}
	//Quaternion orientation = transformComponent->GetQuaternion();
	//Vec3 position = transformComponent->GetPosition();
	//viewMatrix = QMath::toMatrix4(orientation) * MMath::translate(position);


}

void CameraActor::UpdateProjectionMatrix(const float fovy, const float aspectRatio, const float near, const float far) {
	projectionMatrix = MMath::perspective(fovy, aspectRatio, near, far);

}