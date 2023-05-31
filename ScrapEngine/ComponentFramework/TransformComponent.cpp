#include <iostream>
#include "TransformComponent.h"
#include "QMath.h"
#include "MMath.h"
using namespace MATH;

TransformComponent::TransformComponent(Component* parent_):Component(parent_, "") {
	pos = Vec3(0.0f, 0.0f, 0.0f);
	orientation = Quaternion(1.0f, Vec3(0.0f, 0.0f, 0.0f));
	scale = Vec3(1.0f, 1.0f, 1.0f);
}

TransformComponent::TransformComponent(Component* parent_, Vec3 pos_, Quaternion orientation_, Vec3 scale_):
	Component{ parent_, ""}, pos{pos_}, orientation{orientation_}, scale{scale_} {
}

TransformComponent::~TransformComponent() {
	OnDestroy();
}

bool TransformComponent::OnCreate() {
	if (isCreated) return isCreated;
	isCreated = true;
	return isCreated;
}

void TransformComponent::OnDestroy() {
	isCreated = false;
}

void TransformComponent::Update(const float deltaTime) {
	//std::cout << "Hello from Update " << deltaTime << '\n';
}

void TransformComponent::Render()const {}

Matrix4 TransformComponent::GetTransformMatrix() const {
	return MMath::translate(pos) * MMath::scale(scale) * QMath::toMatrix4(orientation);
}