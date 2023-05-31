#ifndef TEXTUREUI_H
#define TEXTUREUI_H

#include "UserInterface.h"
#include "Vector.h"
#include "VMath.h"
#include "Quaternion.h"
#include "QMath.h"
#include "Actor.h"

using namespace MATH;

class Scene;

class TextureUI : public UserInterface {
private:
	Ref<Actor> selectedObject;
	std::unordered_map<const char*, Ref<Actor>> gameObjects;
	const char* selectedTexture;
	const char* selectedShader;
	const char* selectedModel;
	std::vector<const char*> textureList;
	std::vector<const char*> shaderList;
	std::vector<const char*> modelList;

	void showObjectList();
	void showRendererAssets();
	bool addActor(const char* actorName_, Vec3 pos_, Quaternion orientation_, Vec3 scale_);

public:
	TextureUI();
	~TextureUI();
	void Display();
	void Update(Scene* scene_);
	void selectObject(Ref<Actor>);
};

#endif
