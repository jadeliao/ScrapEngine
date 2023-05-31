#include "Camera.h"


Camera::Camera() {
	projection.loadIdentity();
	view.loadIdentity();
}

Camera::~Camera() {
	
}

bool Camera::OnCreate() {
	view = MMath::translate(0.0f, 0.0f, -5.0f) * MMath::rotate(0.0f, Vec3(0.0f, 1.0f, 0.0f));
	return true;
}

void Camera::Perspective(const float fovy, const float aspectRatio, const float near, const float far) {
	projection = MMath::perspective(fovy, aspectRatio, near,far);
}

void Camera::LookAt(const Vec3& eye, const Vec3& at, const Vec3& up) {
	view = MMath::lookAt(eye, at, up);
}

void Camera::rotate(float angle_, Vec3 axis_) {
	view = MMath::rotate(angle_, axis_) * view;
}

void Camera::move(float x_, float y_, float z_) {
	view = MMath::translate(x_, y_, z_) * view;
}

void Camera::Update(const float deltaTime) {
	
}

