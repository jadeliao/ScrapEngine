#ifndef UIMANAGER_H
#define UIMANAGER_H

#include <unordered_map>
#include "UserInterface.h"
//#include "Renderer.h"
#include "imgui.h"
#include "imgui_impl_sdl.h"
#include "imgui_impl_opengl3.h"
#include "imgui_impl_vulkan.h"

class Renderer;
class Scene;

class UIManager{

protected:
	static UIManager* instance_;
	UIManager();
	int width;
	int height;
	Renderer* renderer;
	std::unordered_map<const char*, UserInterface*> ui_list;
	const char* currentUI;
	ImGuiStyle* style;
	bool menuHovered;

public:
	~UIManager();
	static UIManager* getInstance();
	void Display();
	bool OnCreate();
	void OnDestroy();
	void Update(Scene* scene_);
	void setRenderer(Renderer* renderer_) { renderer = renderer_; }
	void setSize(int width_, int height_);
	void AddUserInterface(const char*, UserInterface*);
	void Resized() { ui_list[currentUI]->Resized(); }
	bool isValid();
	//UserInterface* getCurrentUI();
	void defaultTheme();
	void whiteTheme();
	//void greenTheme();
	bool menuOpened(const char* menuName_);
	void openMenu(const char* menuName_);
	bool isHovered();
};

#endif

