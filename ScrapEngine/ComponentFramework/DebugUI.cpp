#include "DebugUI.h"
#include "Scene.h"
#include "Debug.h"

DebugUI::DebugUI() {

}

DebugUI::~DebugUI() {

}

void DebugUI::Display() {

    ImGui::SetNextWindowBgAlpha(windowAlpha);
    //Calculate ui window size
    //It needs to be dependent to the scene window size, so that when resizing the scene window,
    //ui window will change accordingly
    //Set window to display frame
    ImGui::Begin("Frame", &showWindow, ImGuiWindowFlags_NoTitleBar |
        ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoBackground);
    ImGui::SetWindowPos(ImVec2(width - ImGui::GetWindowWidth(), height - 35.0f));
    ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);

    ImGui::End();

    showProfiling();
    showDebugMsg();

}

void DebugUI::Update(Scene* scene_) {
	currentResult = scene_->getProfiling();
    width = renderer->getWidth();
    height = renderer->getHeight();
}

void DebugUI::showProfiling() {

    ImGui::SetNextWindowBgAlpha(windowAlpha);
    ImGui::Begin("Profiling", NULL, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoBackground);
    float windowW = width * 0.14f;
    float windowH = height * 0.3f;
    ImGui::SetWindowSize(ImVec2(windowW, windowH));
    ImGui::SetWindowPos(ImVec2(0.0f, 30.0f));
    for (auto tool : currentResult) {
        std::string toolText = std::string(tool.second->getName()) + " %.3fms";
        float duration = tool.second->getDuration();
        ImGui::Text(toolText.c_str(), duration);
    }
    ImGui::End();

}

void DebugUI::showDebugMsg() {

    ImGui::SetNextWindowBgAlpha(windowAlpha);
    ImGui::Begin("Debug Message", NULL, ImGuiWindowFlags_NoResize |
                                        ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_AlwaysHorizontalScrollbar);
    checkHovered();
    float windowW = width * 0.3f;
    float windowH = height * 0.2f;
    ImGui::SetWindowSize(ImVec2(windowW, windowH));
    ImGui::SetWindowPos(ImVec2(width - ImGui::GetWindowWidth(), 30.0f));
    ImGui::SetWindowFontScale(1.2f);

    for (auto debug : Debug::GetDebugList()) {
        ImGui::Text(debug.c_str());
    }
    ImGui::End();

}
