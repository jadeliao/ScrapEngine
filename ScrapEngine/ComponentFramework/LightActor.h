#pragma once
#include <glew.h>
#include "Actor.h"
#include "Vector.h"
#include "UboStruct.h"

using namespace MATH;

enum class LightStyle {
	DirectionLight = 1,
	PointLight,
	SkyLight,
	SpotLight
};

class LightActor : public Actor {
private:
	LightStyle lightStyle;
	Vec3 position;
	Vec4 colour;
	float intensity;
	Vec3 fallOff;
	GlobalLight gLight;

public:
	LightActor(Component* parent_, const char* name_):Actor(parent_, name_),
		lightStyle(LightStyle::DirectionLight), position(Vec3(0.0f, 0.0f, 0.0f)), colour(Vec4(0.0f, 0.0f, 0.0f, 1.0f)),
		intensity(0.0f), fallOff(Vec3(0.0f, 0.0f, 0.0f)) {}
	LightActor(Component* parent_, LightStyle lightStyle_, Vec3 location_, Vec4  colour_,
		float intensity = 1.0f, Vec3 fallOff = Vec3(0.0f, 0.0f, 0.0f));
	~LightActor();
	bool OnCreate() override;
	void OnDestroy() override;
	void Render() const;
	Vec3 GetPosition() const { return position; }
	void SetLight(GlobalLight gLight_) { gLight = gLight_; }
	GlobalLight GetLight() { return gLight; }
};

