#include "UIManager.h"
#include "Renderer.h"
#include "TextureUI.h"
#include "OptionMenu.h"
#include "AudioManager.h"
#include "DebugUI.h"

UIManager* UIManager::instance_(nullptr);

UIManager::UIManager():renderer(nullptr), width(0), height(0), currentUI("default"), style(nullptr) {

}

UIManager::~UIManager(){
	OnDestroy();
}

UIManager* UIManager::getInstance(){

	if (!instance_) {
		instance_ = new UIManager();
	}

	return instance_;
}

void UIManager::Display(){
	if (renderer) {
		switch (renderer->getRendererType()) {
		case RendererType::VULKAN:
			ImGui_ImplVulkan_NewFrame();
			ImGui_ImplSDL2_NewFrame();
			break;
		case RendererType::OPENGL:
			ImGui_ImplOpenGL3_NewFrame();
			ImGui_ImplSDL2_NewFrame();
			break;
		}
		ImGui::NewFrame();

		if (ImGui::BeginMainMenuBar()) {
			ImGui::SetWindowFontScale(1.2f);

			if (ImGui::BeginMenu("Menu")) {
				for (auto ui_ : ui_list) {
					bool isOpened_ = ui_.second->isOpened();
					if (ImGui::MenuItem(ui_.first, ui_.second->getShortcut(), isOpened_)) {
						openMenu(ui_.first);
					}
					if (isOpened_) {
						ui_.second->Display();
					}
				}
				ImGui::EndMenu();
			}


			if (ImGui::BeginMenu("Debug")) {
				if (ImGui::MenuItem("Clean##Debug")) { Debug::Clean(); }
				ImGui::EndMenu();
			}
			menuHovered = (ImGui::IsAnyItemHovered() || ImGui::IsWindowHovered());
			ImGui::EndMainMenuBar();
		}

		//Display current ui
		//if (!ui_list.empty()) {
		//	ui_list[currentUI]->Display();
		//}
		for (auto ui_ : ui_list) {
			if (ui_.second->isOpened()) {
				ui_.second->Display();
			}
		}

		ImGui::Render();
	}
}

bool UIManager::OnCreate(){

	renderer = Renderer::getInstance();
	if (!renderer) return false;
	//Initialize ImGui in renderer
	renderer->InitImGui();
	defaultTheme();	

	DebugUI* debugMenu = new DebugUI();
	debugMenu->setSoundFile("audio/smash.wav");
	AddUserInterface("DebugUI", debugMenu);

	OptionMenu* optionMenu = new OptionMenu();
	optionMenu->setShortcut("ESC");
	optionMenu->setSoundFile("audio/fastPunch.wav");
	AddUserInterface("OptionMenu", optionMenu);

	//currentUI = ui_list.begin()->first;
	//currentUI = "default";
	return true;
}

void UIManager::OnDestroy(){
	if (renderer) {

		switch (renderer->getRendererType()) {
		case RendererType::VULKAN:
			ImGui_ImplVulkan_Shutdown();
			ImGui_ImplSDL2_Shutdown();
			break;
		case RendererType::OPENGL:
			break;
		}
		ImGui::DestroyContext();
		//if (style) delete style;
	}

	for (auto ui_ : ui_list) {
		if (ui_.second) {
			delete ui_.second;
		}
	}

}

void UIManager::Update(Scene* scene_){
	for (auto ui_ : ui_list) {
		ui_.second->Update(scene_);
	}
}

void UIManager::setSize(int width_, int height_){
	width = width_;
	height = height_;
}

void UIManager::AddUserInterface(const char* uiName_, UserInterface* ui_){
	
	//Only add when there is no ui with the same name	
	if (ui_list.count(uiName_) <= 0) {
		ui_->setSize(width, height);
		ui_list[uiName_] = ui_;
	}

}

bool UIManager::isValid(){
	if (renderer) return true;

	return false;
}

void UIManager::defaultTheme(){
	style = &ImGui::GetStyle();
	style->WindowPadding = ImVec2(15, 15);
	style->WindowRounding = 5.0f;
	style->FramePadding = ImVec2(5, 5);
	style->FrameRounding = 4.0f;
	style->PopupRounding = 4.0f;
	style->PopupBorderSize = 1.0f;
	style->ItemSpacing = ImVec2(12, 5);
	style->ItemInnerSpacing = ImVec2(8, 6);
	style->IndentSpacing = 25.0f;
	style->ScrollbarSize = 15.0f;
	style->ScrollbarRounding = 9.0f;
	style->ColumnsMinSpacing = 50.0f;
	style->GrabMinSize = 5.0f;
	style->GrabRounding = 3.0f;

	style->Colors[ImGuiCol_Text] = ImVec4(0.65f, 0.68f, 0.7f, 1.00f);
	style->Colors[ImGuiCol_WindowBg] = ImVec4(0.05f, 0.05f, 0.08f, 1.0f);
	style->Colors[ImGuiCol_Border] = ImVec4(0.3f, 0.3f, 0.3f, 0.8f);
	style->Colors[ImGuiCol_BorderShadow] = ImVec4(0.92f, 0.91f, 0.88f, 0.00f);
	style->Colors[ImGuiCol_TitleBg] = ImVec4(0.01f, 0.01f, 0.01f, 1.0f);
	style->Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.01f, 0.01f, 0.01f, 1.0f);
	style->Colors[ImGuiCol_TitleBgActive] = ImVec4(0.01f, 0.01f, 0.01f, 1.0f);
	style->Colors[ImGuiCol_FrameBg] = ImVec4(0.06f, 0.08f, 0.1f, 1.00f);
	style->Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.1f, 0.12f, 0.14f, 1.00f);
	style->Colors[ImGuiCol_FrameBgActive] = ImVec4(0.1f, 0.12f, 0.14f, 1.00f);
	style->Colors[ImGuiCol_Tab] = ImVec4(0.08f, 0.08f, 0.1f, 1.00f);
	style->Colors[ImGuiCol_TabHovered] = ImVec4(0.1f, 0.01f, 0.01f, 1.0f);
	style->Colors[ImGuiCol_TabActive] = ImVec4(0.1f, 0.01f, 0.01f, 1.0f);
	style->Colors[ImGuiCol_Header] = ImVec4(0.02f, 0.02f, 0.02f, 1.0f);
	style->Colors[ImGuiCol_HeaderHovered] = ImVec4(0.11f, 0.01f, 0.01f, 1.0f);
	style->Colors[ImGuiCol_HeaderActive] = ImVec4(0.11f, 0.01f, 0.01f, 1.0f);
	style->Colors[ImGuiCol_Button] = ImVec4(0.03f, 0.03f, 0.03f, 1.0f);
	style->Colors[ImGuiCol_ButtonHovered] = ImVec4(0.1f, 0.01f, 0.01f, 1.0f);
	style->Colors[ImGuiCol_Separator] = ImVec4(0.35f, 0.38f, 0.4f, 1.00f);
	style->Colors[ImGuiCol_SeparatorHovered] = ImVec4(0.35f, 0.38f, 0.4f, 1.00f);
	style->Colors[ImGuiCol_SeparatorActive] = ImVec4(0.35f, 0.38f, 0.4f, 1.00f);
	style->Colors[ImGuiCol_SliderGrab] = ImVec4(0.03f, 0.03f, 0.03f, 1.0f);
	style->Colors[ImGuiCol_SliderGrabActive] = ImVec4(0.11f, 0.01f, 0.01f, 1.0f);
	style->Colors[ImGuiCol_ScrollbarBg] = ImVec4(0.20f, 0.22f, 0.27f, 1.00f);
	style->Colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.03f, 0.03f, 0.03f, 1.0f);
	style->Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.08f, 0.1f, 0.12f, 1.00f);
	style->Colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.1f, 0.12f, 0.14f, 1.00f);
	style->Colors[ImGuiCol_MenuBarBg] = ImVec4(0.15f, 0.18f, 0.25f, 1.00f);
	style->Colors[ImGuiCol_PopupBg] = ImVec4(0.15f, 0.18f, 0.25f, 1.00f);
}

void UIManager::whiteTheme() {
	style = &ImGui::GetStyle();
	style->WindowPadding = ImVec2(15, 15);
	style->WindowRounding = 5.0f;
	style->FramePadding = ImVec2(5, 5);
	style->FrameRounding = 4.0f;
	style->PopupRounding = 4.0f;
	style->PopupBorderSize = 1.0f;
	style->ItemSpacing = ImVec2(12, 5);
	style->ItemInnerSpacing = ImVec2(8, 6);
	style->IndentSpacing = 25.0f;
	style->ScrollbarSize = 15.0f;
	style->ScrollbarRounding = 9.0f;
	style->ColumnsMinSpacing = 50.0f;
	style->GrabMinSize = 5.0f;
	style->GrabRounding = 3.0f;

	style->Colors[ImGuiCol_Text] = ImVec4(0.01f, 0.01f, 0.04f, 1.00f);
	style->Colors[ImGuiCol_WindowBg] = ImVec4(0.80f, 0.80f, 0.75f, 1.0f);
	style->Colors[ImGuiCol_Border] = ImVec4(0.6f, 0.6f, 0.6f, 0.8f);
	style->Colors[ImGuiCol_BorderShadow] = ImVec4(0.72f, 0.71f, 0.78f, 0.00f);
	style->Colors[ImGuiCol_TitleBg] = ImVec4(0.31f, 0.31f, 0.51f, 1.0f);
	style->Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.51f, 0.51f, 0.71f, 1.0f);
	style->Colors[ImGuiCol_TitleBgActive] = ImVec4(0.51f, 0.51f, 0.71f, 1.0f);
	style->Colors[ImGuiCol_FrameBg] = ImVec4(0.72f, 0.72f, 0.62f, 1.0f);
	style->Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.5f, 0.52f, 0.74f, 1.00f);
	style->Colors[ImGuiCol_FrameBgActive] = ImVec4(0.4f, 0.42f, 0.74f, 1.00f);
	style->Colors[ImGuiCol_Tab] = ImVec4(0.32f, 0.32f, 0.52f, 1.0f);
	style->Colors[ImGuiCol_TabHovered] = ImVec4(0.2f, 0.32f, 0.74f, 1.00f);
	style->Colors[ImGuiCol_TabActive] = ImVec4(0.2f, 0.32f, 0.74f, 1.00f);
	style->Colors[ImGuiCol_Header] = ImVec4(0.32f, 0.32f, 0.52f, 1.0f);
	style->Colors[ImGuiCol_HeaderHovered] = ImVec4(0.1f, 0.12f, 0.64f, 1.00f);
	style->Colors[ImGuiCol_HeaderActive] = ImVec4(0.1f, 0.12f, 0.64f, 1.00f);
	style->Colors[ImGuiCol_Button] = ImVec4(0.31f, 0.31f, 0.51f, 1.0f);
	style->Colors[ImGuiCol_ButtonHovered] = ImVec4(0.2f, 0.32f, 0.74f, 1.00f);
	style->Colors[ImGuiCol_Separator] = ImVec4(0.35f, 0.38f, 0.7f, 1.00f);
	style->Colors[ImGuiCol_SeparatorHovered] = ImVec4(0.35f, 0.38f, 0.4f, 1.00f);
	style->Colors[ImGuiCol_SeparatorActive] = ImVec4(0.35f, 0.38f, 0.4f, 1.00f);
	style->Colors[ImGuiCol_SliderGrab] = ImVec4(0.31f, 0.31f, 0.51f, 1.0f);
	style->Colors[ImGuiCol_SliderGrabActive] = ImVec4(0.2f, 0.32f, 0.74f, 1.00f);
	style->Colors[ImGuiCol_ScrollbarBg] = ImVec4(0.31f, 0.31f, 0.51f, 1.0f);
	style->Colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.12f, 0.12f, 0.32f, 1.0f);
	style->Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.2f, 0.32f, 0.74f, 1.00f);
	style->Colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.2f, 0.32f, 0.74f, 1.00f);
	style->Colors[ImGuiCol_PopupBg] = ImVec4(0.72f, 0.72f, 0.62f, 1.0f);
}


bool UIManager::menuOpened(const char* menuName_){
	for (auto ui_ : ui_list) {
		if (strcmp(ui_.first, "OptionMenu") == 0) {
			return ui_.second->isOpened();
		}
	}
	return false;
}

void UIManager::openMenu(const char* menuName_){
	for (auto ui_ : ui_list) {
		if (strcmp(ui_.first, menuName_) == 0) {
			ui_.second->ShowWindow(!ui_.second->isOpened());
			AudioManager* audioManager = AudioManager::getInstance();
			audioManager->PauseBGM(!audioManager->isBGMPaused());
			audioManager->PlaySoundEffects(ui_.second->getSoundFile());
		}
	}
}

bool UIManager::isHovered(){

	//If main menu bar is being hovered
	if (menuHovered) return true;

	//If any of the ui is being hovered
	for (auto ui_ : ui_list) {
		if (ui_.second->isHovered()) {
			return true;
		};
	}

	return false;
}
