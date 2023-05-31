#ifndef SCENEMANAGER_H
#define SCENEMANAGER_H

#include "Debug.h"
#include <string>
#include "Renderer.h"

enum class RendererType;

class SceneManager  {
public:
	
	SceneManager(RendererType rendererType_);
	~SceneManager();
	void Run();
	bool Initialize(std::string name_, int width_, int height_);
	void GetEvents();
	
	
private:
	
	enum SCENE_NUMBER {
		SCENE0 = 0,
		SCENE1,
		SCENE2,
		SCENE3,
		SCENE4,
		SCENE5,
		SCENE6
	};

	enum class RendererType rendererType;
	class Scene* currentScene;
	class Timer* timer;
	class UIManager* uiManager;
	class AssetManager* assetManager;
	class AudioManager* audioManager;
	class InputManager* inputManager;

	Renderer* renderer;
	unsigned int fps;
	bool isRunning;
	void BuildScene(SCENE_NUMBER scene_);
	void RendererReset(RendererType rendererType_);
};


#endif // SCENEMANAGER_H