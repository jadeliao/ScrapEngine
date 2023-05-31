#ifndef USERINTERFACE_H
#define USERINTERFACE_H

#include "Debug.h"
#include <unordered_map>
#include "Vector.h"
#include "imgui.h"
#include "imgui_impl_sdl.h"
#include "imgui_impl_vulkan.h"
#include "Renderer.h"

class Renderer;
class Scene;

using namespace MATH;

class UserInterface {
protected:
	int width;
	int height;
	Renderer* renderer;
	bool resized = false;
	bool showWindow;
	float windowAlpha;
	const char* shortcut;
	const char* soundFile;
	bool windowHovered;

	void checkHovered() {
		windowHovered = ImGui::IsWindowHovered();
	}

public:
	UserInterface() : renderer(Renderer::getInstance()), showWindow(false), windowAlpha(1.0f), 
						height(0), width(0), shortcut(""), soundFile(""), windowHovered(false) {}
	virtual ~UserInterface() = default;
	virtual void Display() = 0;
	virtual void Update(Scene* scene_) = 0;
	void setSize(int width_, int height_) {
		width = width_;
		height = height_;
	}
	void ShowWindow(bool showWindow_) { showWindow = showWindow_; }
	bool isOpened() { return showWindow; }
	void setWindowAlpha(float alpha_) { windowAlpha = alpha_; }
	void setShortcut(const char* shortcut_) { shortcut = shortcut_; }
	const char* getShortcut() { return shortcut; }
	void setSoundFile(const char* soundFile_) { soundFile = soundFile_; }
	const char* getSoundFile() { return soundFile; }
	//void setRenderType(RendererType renderType_) { renderType = renderType_; }
	//ImVec2 UIWindowResize(ImVec2 uiSize_) {
	//	//Find initial ratio
	//	float xRatio = ((uiSize_.x) / initWidth );
	//	float yRatio = ((uiSize_.y) / initHeight);

	//	float x = (width * xRatio);
	//	float y = (height * yRatio);
	//	return ImVec2(x, y);
	//}
	void Resized() { resized = true; }
	bool isHovered() { return windowHovered; }
};

#endif

