#ifndef OPTIONMENU_H
#define OPTIONMENU_H

#include "UserInterface.h"
#include <string>
#include "Vector.h"
#include "VMath.h"
#include "Quaternion.h"
#include "QMath.h"
#include "Actor.h"

using namespace MATH;

class Scene0;

class OptionMenu : public UserInterface{

private:

	Ref<Actor> selectedObject;
	std::unordered_map<const char*, Ref<Actor>> gameObjects;
	const char* selectedTexture;
	const char* selectedShader;
	const char* selectedModel;
	std::vector<const char*> textureList;
	std::vector<const char*> shaderList;
	std::vector<const char*> modelList;
	bool showObjects;
	bool colourPicker;
	Scene0* currentScene;
	void showObjectList();
	void showRendererAssets();
	bool addActor(const char* actorName_, Vec3 pos_, Quaternion orientation_, Vec3 scale_);
	void showAudioControl();
	void showLightControl();
	void showColourPicker(int lightIndex_);
	void lightPosSlider(int lightIndex_);

public:
	OptionMenu() : UserInterface(), showObjects(false), colourPicker(false), currentScene(nullptr) {}
	~OptionMenu();
	void Display();
	void Update(Scene* scene_);

	void selectObject(Ref<Actor>);

};

#endif
