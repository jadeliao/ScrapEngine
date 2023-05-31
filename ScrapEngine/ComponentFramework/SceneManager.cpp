#define _CRTDBG_MAP_ALLOC
#include <SDL.h>
#include "SceneManager.h"
#include "VulkanRenderer.h"
#include "OpenGLRenderer.h"
#include "UIManager.h"
#include "AssetManager.h"
#include "AudioManager.h"
#include "InputManager.h"
#include "Timer.h"
#include "Scene0.h"

Renderer* Renderer::instance_ = nullptr;

SceneManager::SceneManager(RendererType rendererType_):
	currentScene(nullptr), timer(nullptr),
	fps(60), isRunning(false), rendererType(rendererType_),
	renderer(nullptr), assetManager(nullptr), audioManager(nullptr), uiManager(nullptr), inputManager(nullptr) {

}

SceneManager::~SceneManager() {
	if (currentScene) {
		currentScene->OnDestroy();
		delete currentScene;
		currentScene = nullptr;
	}
	
	if (timer) {
		delete timer;
		timer = nullptr;
	}

	if (audioManager) {
		//audioManager->OnDestroy();
		delete audioManager;
		audioManager = nullptr;
	}

	if (uiManager) {
		delete uiManager;
		uiManager = nullptr;
	}

	if (assetManager) {
		assetManager->OnDestroy();
		delete assetManager;
		assetManager = nullptr;
	}

	if (inputManager) {
		delete inputManager;
		inputManager = nullptr;
	}

	if (renderer) {
		delete renderer;
		renderer = nullptr;
	}
	Debug::Info("Deleting the GameSceneManager", __FILE__, __LINE__);
}

bool SceneManager::Initialize(std::string name_, int width_, int height_) {

	switch(rendererType){
	case RendererType::OPENGL:
		renderer = renderer->getInstance<OpenGLRenderer>();
		renderer->setRendererType(RendererType::OPENGL);
		renderer->BuildWindow(name_, width_, height_);
		Debug::Info("OPENGL Renderer Created", __FILE__, __LINE__);
		break;

	case RendererType::VULKAN:
		renderer = renderer->getInstance<VulkanRenderer>();
		renderer->setRendererType(RendererType::VULKAN);
		renderer->BuildWindow(name_, width_, height_);
		Debug::Info("VULKAN Renderer Created", __FILE__, __LINE__);
		break;

	case RendererType::DIRECTX11:
	case RendererType::DIRECTX12:
		Debug::FatalError("Renderer not supported", __FILE__, __LINE__);
		return false;
		break;
	}

	//Create Renderer
	if (!renderer->OnCreate()) {
		return false;
	}

	//SetupAssetManager
	assetManager = AssetManager::getInstance();
	if (!assetManager->OnCreate()) {
		return false;
	}
	Debug::Info("AssetManager Created", __FILE__, __LINE__);
	assetManager->LoadAssets("RendererAssets");

	//Add assets to renderer, replace this with asset manager
	//renderer->LoadModel("MarioModel", "./meshes/Mario.obj");
	//renderer->LoadModel("SkullModel", "./meshes/Skull.obj");
	//renderer->LoadTexture("MarioMain", "./textures/mario_main.png");
	//renderer->LoadTexture("MarioFire", "./textures/mario_fire.png");
	//renderer->LoadTexture("MarioBlack", "./textures/mario_mime.png");
	//renderer->LoadTexture("SkullNormal", "./textures/skull_texture.png");
	//renderer->LoadTexture("SkullDark", "./textures/skull_dark.png");
	//renderer->LoadTexture("SkullRed", "./textures/skull_red.png");
	//renderer->LoadTexture("SkullBlue", "./textures/skull_blue.png");
	//renderer->LoadShader("PhongShader", "shaders/textureVert.glsl", "shaders/textureFrag.glsl");
	//renderer->LoadShader("PhongShader", "shaders/phong.vert.spv", "shaders/phong.frag.spv");
	//renderer->LoadShader("NormalShader", "shaders/drawNormals.vert.spv", "shaders/drawNormals.frag.spv", "shaders/drawNormals.geom.spv");
	//renderer->LoadShader("LineShader", "shaders/drawLine.vert.spv", "shaders/drawLine.frag.spv", "shaders/drawLine.geom.spv");

	//Setup UI Manager
	uiManager = UIManager::getInstance();
	uiManager->setRenderer(renderer);
	uiManager->setSize(width_, height_);
	if (!uiManager->OnCreate()) {
		return false;
		Debug::FatalError("UIManager Failed", __FILE__, __LINE__);
	}
	Debug::Info("UIManager Created", __FILE__, __LINE__);

	//Setup Audio Manager
	audioManager = AudioManager::getInstance();
	if (!audioManager->OnCreate()) {
		return false;
		Debug::FatalError("AudioManager Failed", __FILE__, __LINE__);
	}
	Debug::Info("AudioManager Created", __FILE__, __LINE__);

	//Setup Input Manager
	inputManager = InputManager::getInstance();
	if (!inputManager->OnCreate()) {
		return false;
		Debug::FatalError("InputManager Failed", __FILE__, __LINE__);
	}
	Debug::Info("InputManager Created", __FILE__, __LINE__);

	timer = new Timer();
	if (timer == nullptr) {
		Debug::FatalError("Failed to initialize Timer object", __FILE__, __LINE__);
		return false;
	}

	BuildScene(SCENE0);

	return true;
}


void SceneManager::Run() {
	timer->Start();
	isRunning = true;
	while (isRunning) {
		GetEvents();
		//inputManager->Update(currentScene);
		//inputManager->Run();
		timer->UpdateFrameTicks();
		currentScene->Update(timer->GetDeltaTime());
		uiManager->Update(currentScene);
		uiManager->Display();
		audioManager->Update();
		currentScene->Render();
		SDL_Delay(timer->GetSleepTime(fps));	
	}
}

void SceneManager::GetEvents() {
	SDL_Event sdlEvent;
	while (SDL_PollEvent(&sdlEvent)) {
		ImGui_ImplSDL2_ProcessEvent(&sdlEvent);
		if (sdlEvent.type == SDL_EventType::SDL_QUIT) {
			isRunning = false;
			return;
		}
		else if (sdlEvent.type == SDL_KEYDOWN) {

			switch (sdlEvent.key.keysym.scancode) {
			case SDL_SCANCODE_ESCAPE:
				uiManager->openMenu("OptionMenu");
				break;
			//case SDL_SCANCODE_Q:
			//	isRunning = false;
			//	return;

			case SDL_SCANCODE_F1:
				///BuildScene(SCENE1);
				break;

			case SDL_SCANCODE_F2:
				///BuildScene(SCENE2);
				break;

			case SDL_SCANCODE_F3:
				///BuildScene(SCENE3);
				break;

			case SDL_SCANCODE_F4:
				///BuildScene(SCENE4);
				break;

			case SDL_SCANCODE_F5:
				///BuildScene(SCENE5);
				break;

			case SDL_SCANCODE_F6:
				///BuildScene(SCENE6);
				break;
			case SDL_SCANCODE_F11:
				//Reset renderer type
				RendererReset(RendererType::OPENGL);
				break;
			case SDL_SCANCODE_F12:
				//Reset renderer type
				//RendererReset(RendererType::VULKAN);
				break;

			default:
				//BuildScene(SCENE0);
				break;
			}
		}
		if (currentScene == nullptr) {
			Debug::FatalError("Failed to initialize Scene", __FILE__, __LINE__);
			isRunning = false;
			return;
		}
		bool checkUIInput = uiManager->isHovered();
		//If the mouse cursor is above ui windows, don't perform any scene handle events
		if (checkUIInput) {
			return; 
		}
		currentScene->HandleEvents(sdlEvent);
	}
}

void SceneManager::BuildScene(SCENE_NUMBER scene) {
	bool status; 

	if (currentScene != nullptr) {
		delete currentScene;
		currentScene = nullptr;
	}

	switch (scene) {
	case SCENE0:  
		currentScene = new Scene0("Scene0");
		status = currentScene->OnCreate();
		break;

	case SCENE1:
		//currentScene = new Scene1();
		status = currentScene->OnCreate();
		break;

	case SCENE2:
		///currentScene = new Scene2();
		status = currentScene->OnCreate();
		break;

	case SCENE3:
		///currentScene = new Scene3();
		status = currentScene->OnCreate();
		break;

	case SCENE4:
		///currentScene = new Scene4();
		//status = currentScene->OnCreate();
		break;

	case SCENE5:
		///currentScene = new Scene5();
		status = currentScene->OnCreate();
		break;

	case SCENE6:
		///currentScene = new Scene6();
		status = currentScene->OnCreate();
		break;

	default:
		Debug::Error("Incorrect scene number assigned in the manager", __FILE__, __LINE__);
		currentScene = nullptr;
		break;
	}

}

//This is broken, don't use
void SceneManager::RendererReset(RendererType rendererType_){

	//Clean up scene manager
	//if (currentScene) {
	//	currentScene->OnDestroy();
	//	delete currentScene;
	//	currentScene = nullptr;
	//}

	//if (timer) {
	//	delete timer;
	//	timer = nullptr;
	//}
	//renderer->OnDestroy();
	//delete renderer;
	//renderer = nullptr;
	//Debug::Info("Deleting the GameSceneManager", __FILE__, __LINE__);

	//if (uiManager) {
	//	//uiManager->OnDestroy();
	//	delete uiManager;
	//}
	//if (assetManager) {
	//	assetManager->OnDestroy();
	//	delete assetManager;
	//}
	////Set the info for renderer
	//std::string name_ = { "Graphics Game Engine" };
	//int rWidth_ = 1280;
	//int rHeight_ = 720;
	//rendererType = rendererType_;
	//Initialize(name_, rWidth_, rHeight_);
}


