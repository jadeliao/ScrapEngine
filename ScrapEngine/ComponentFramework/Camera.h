#pragma once
#include "MMath.h"
using namespace MATH;
class Camera {
private:
	Matrix4 projection;
	Matrix4 view;
	Matrix4 rotation;
	Matrix4 translate;
public:
	Camera();
	~Camera();
	bool OnCreate();
	void Perspective(const float fovy, const float aspectRatio, const float near, const float far);
	void LookAt(const Vec3& eye, const Vec3& at, const Vec3& up);
	void rotate(float angle_, Vec3 axis_);
	void move(float x_, float y_, float z_);
	void Update(const float deltaTime);

	inline Matrix4 GetProjectionMatrix() {
		return projection;
	}
	inline Matrix4 GetViewMatrix() {
		return view;
	}


};

