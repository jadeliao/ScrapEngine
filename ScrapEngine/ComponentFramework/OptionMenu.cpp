#define _CRTDBG_MAP_ALLOC
#include "OptionMenu.h"
#include "TransformComponent.h"
#include "OpenGLActor.h"
#include "VulkanActor.h"
#include "AudioManager.h"
#include "UboStruct.h"
#include "Scene0.h"
#include "UIManager.h"

OptionMenu::~OptionMenu(){}

void OptionMenu::Display(){
    ImGui::SetNextWindowBgAlpha(windowAlpha);
    ImGui::Begin("Option Menu", NULL, 
           ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse);

    ImGui::SetWindowFontScale(1.5f);
    //Calculate ui window size
    //It needs to be dependent to the scene window size, so that when resizing the scene window,
    //ui window will change accordingly
    float windowW = width * 0.4f;
    float windowH = height * 0.6f;
    ImGui::SetWindowSize(ImVec2(windowW, windowH));
    ImGui::SetWindowPos(ImVec2(width / 2.0f - windowW * 0.5f, height / 2.0f - windowH * 0.5f));

    //Set when 
    checkHovered();
    //std::cout << "OptionMenu" << windowHovered << "\n";

    //Calculate tab width
    int tabNum = 4;
    float tabWidth = ImGui::GetWindowSize().x / tabNum;
    ImGuiTabBarFlags tab_bar_flags = ImGuiTabBarFlags_None;
    if (ImGui::BeginTabBar("OptionTab", tab_bar_flags))
    {
        ImGui::SetNextItemWidth(tabWidth);
        const char* tabName = "Audio";
        float textWidth = ImGui::CalcTextSize(tabName).x;
        if (ImGui::BeginTabItem("Audio"))
        {
            showAudioControl();
            ImGui::EndTabItem();
        }
        ImGui::SetNextItemWidth(tabWidth);
        if (ImGui::BeginTabItem("Renderer")){


            showRendererAssets();
            ImGui::EndTabItem();
        }
        ImGui::SetNextItemWidth(tabWidth);
        if (ImGui::BeginTabItem("UI")) {

            ImGui::Text("UI Theme");
            //Change Theme
            static int choice = 0;
            if (ImGui::RadioButton("Default Theme", &choice, 0)) {
                UIManager::getInstance()->defaultTheme();
            }
            ImGui::SameLine();
            if (ImGui::RadioButton("White Theme", &choice, 1)) {
                UIManager::getInstance()->whiteTheme();
            }
            //Change alpha
            ImGui::Text("UI Window Transparency");
            ImGui::SliderFloat("##windowAlpha", &windowAlpha, 0.0f, 1.0f, "%.2f");            

            ImGui::EndTabItem();
        }
        ImGui::SetNextItemWidth(tabWidth);
        if (ImGui::BeginTabItem("Light")) {
            showLightControl();
            ImGui::EndTabItem();
        }
        ImGui::EndTabBar();
    }

    ImGui::End();

}

void OptionMenu::showObjectList() {


    //Set window to display and select objects
    ImGui::Begin("Object List", &showWindow, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoMove);

    float windowW = width * 0.2f;
    float windowH = height * 0.3f;
    ImGui::SetWindowSize(ImVec2(windowW, windowH));
    ImGui::SetWindowPos(ImVec2(10.0f, 10.0f));

    if (gameObjects.empty()) {
        selectedObject = nullptr;
    }

    const char* selectedName = "None";
    if (selectedObject) {
        selectedName = selectedObject->GetName();
    }

    ImGui::Text("Selected Object: %s", selectedName);
    //Set a header to display object list
    if (!gameObjects.empty()) {
        //Create a list to display objects in the list
        if (ImGui::BeginListBox("##Object List", ImVec2(-FLT_MIN, 5 * ImGui::GetTextLineHeightWithSpacing()))) {
            for (auto object_ : gameObjects) {
                const char* objectName = object_.first;
                const bool isSelected = (selectedName == objectName);
                //When the object is selceted, set the selected object to corresponded object
                if (ImGui::Selectable(objectName, isSelected)) {
                    selectedObject = object_.second;
                }

            }
            ImGui::EndListBox();
        }
    }
    if (ImGui::Button("Delete##2")) {
        //Remove object from renderer
        renderer->RemoveObject(selectedName);
    }

    ImGui::End();
}

void OptionMenu::selectObject(Ref<Actor> object_) {
    selectedObject = object_;
}

void OptionMenu::showRendererAssets() {

    const char* renderTypeName = "";
    switch (renderer->getRendererType()) {
    case RendererType::VULKAN:
        renderTypeName = "Vulkan";
        break;
    case RendererType::OPENGL:
        renderTypeName = "OpenGL";
        break;
    }

    //Show remderer type
    ImGui::Text("Render Type: %s", renderTypeName);
    ImGui::SameLine(ImGui::GetWindowSize().x * 0.6f);
    if (ImGui::Checkbox("Object List", &showObjects)) {
    }

    //Get object name
    ImGui::Separator();
    ImGui::Spacing();
    static char actorNameInput[20];
    ImGui::InputTextWithHint("##ActorName", "Actor Name", actorNameInput, IM_ARRAYSIZE(actorNameInput));

    //Display model list
    ImGui::Spacing();
    ImGui::Separator();
    ImGui::Spacing();
    ImGui::Text("Select Model\n");
    if (!modelList.empty()) {
        //Find the index
        if (ImGui::BeginCombo("##Select Model", selectedModel)) {
            for (auto model_ : modelList) {
                const bool isSelected = (selectedModel == model_);
                if (ImGui::Selectable(model_, isSelected)) {
                    selectedModel = model_;
                }
            }
            ImGui::EndCombo();
        }
        ImGui::SameLine();
        if (ImGui::Button("Nothing")) {
        }
    }
    //Display texture list
    ImGui::Spacing();
    ImGui::Separator();
    ImGui::Spacing();
    ImGui::Text("Select Texture\n");
    if (!textureList.empty()) {
        if (ImGui::BeginCombo("##TextureList", selectedTexture)) {
            for (auto texture_ : textureList) {
                const bool isSelected = (selectedTexture == texture_);
                if (ImGui::Selectable(texture_, isSelected)) {
                    selectedTexture = texture_;
                }
            }
            ImGui::EndCombo();
        }
    }
    ImGui::SameLine();
    if (ImGui::Button("Change##1")) {
        selectedObject->ChangeTexture(renderer->GetTexture(selectedTexture));
    }

    //Display Shader
    ImGui::Spacing();
    ImGui::Separator();
    ImGui::Spacing();
    ImGui::Text("Select Shader");
    if (!shaderList.empty()) {
        if (ImGui::BeginCombo("##ShaderList", selectedShader)) {
            for (auto shader_ : shaderList) {
                const bool isSelected = (selectedShader == shader_);
                if (ImGui::Selectable(shader_, isSelected)) {
                    selectedShader = shader_;
                }
            }
            ImGui::EndCombo();
        }
    }
    ImGui::SameLine();
    if (ImGui::Button("Change##2")) {
        selectedObject->ChangeShader(renderer->GetShader(selectedShader));
    }

    //Input Transform
    ImGui::Spacing();
    ImGui::Separator();
    ImGui::Spacing();
    ImGui::Text("Tranform");

    static float position[3] = { 0.0f, 0.0f, 0.0f };
    ImGui::InputFloat3("Position", position);
    ImGui::Spacing();

    static float rotationAxis[3] = { 0.0f , 0.0f, 0.0f };
    ImGui::InputFloat3("Axis", rotationAxis);
    static float rotation = 0.0f;
    ImGui::InputFloat("Rotation", &rotation);
    ImGui::Spacing();

    static float scale[3] = { 0.0f , 0.0f, 0.0f };
    ImGui::InputFloat3("Scale", scale);

    ImGui::Spacing();
    ImGui::Separator();
    if (ImGui::Button("Add Actor")) {

        //Create a new actor and add to renderer
        //Get transform information
        Vec3 pos_(position[0], position[1], position[2]);
        Vec3 rotationAxis_(rotationAxis[0], rotationAxis[1], rotationAxis[2]);
        Quaternion orientation_;
        //Check valid orientation
        if (VMath::mag(rotationAxis_) < VERY_SMALL) {
            orientation_ = Quaternion();
        }
        else {
            orientation_ = QMath::angleAxisRotation(rotation, rotationAxis_);
        }
        //Check valid scale
        Vec3 scale_(scale[0], scale[1], scale[2]);
        if (scale_.x < VERY_SMALL) scale_.x = 1.0f;
        if (scale_.y < VERY_SMALL) scale_.y = 1.0f;
        if (scale_.z < VERY_SMALL) scale_.z = 1.0f;
        if (VMath::mag(scale_) < VERY_SMALL) {
            scale_ = Vec3(1.0f, 1.0f, 1.0f);
        }

        //Copy actor name into another char*
        char* actorName_ = (char*)malloc(20);
        strcpy_s(actorName_, sizeof(actorNameInput) - 1, actorNameInput);
        actorName_[sizeof(actorNameInput) - 1] = '\0';
        if (!addActor(actorName_, pos_, orientation_, scale_)) {
            Debug::Info("Fail to Add Actor to Scene", __FILE__, __LINE__);
        }
        else {
            Debug::Info("Actor Added to Scene " + std::string(actorName_), __FILE__, __LINE__);
        }
    }


    if (showObjects) {
        showObjectList();
    }

}

bool OptionMenu::addActor(const char* actorName_, Vec3 pos_, Quaternion orientation_, Vec3 scale_){

    //Get information for actor
    Ref<MeshComponent> mesh_;
    Ref<ShaderComponent> shader_;
    Ref<MaterialComponent> material_;
    Ref<TransformComponent> transform_;
    mesh_ = renderer->GetModel(selectedModel);
    shader_ = renderer->GetShader(selectedShader);
    material_ = renderer->GetTexture(selectedTexture);
    if (!mesh_ || !shader_ || !material_) return false;

    //Check if actor is in actorlist
    if (renderer->ContainActor(actorName_)) return false;

    //Create actor 
    Ref<Actor> actor_;
    transform_ = std::make_shared<TransformComponent>(actor_.get(), pos_, orientation_, scale_);
    switch (renderer->getRendererType()) {
    case RendererType::VULKAN:
        actor_ = std::make_shared<VulkanActor>(nullptr, actorName_);
        break;
    case RendererType::OPENGL:
        actor_ = std::make_shared<OpenGLActor>(nullptr, actorName_);
        break;
    }

    actor_->AddComponent(mesh_);
    actor_->AddComponent(shader_);
    actor_->AddComponent(material_);
    actor_->AddComponent(transform_);

    if (actor_->OnCreate()) {
        renderer->AddActor(actor_->GetName(), actor_);
        return true;
    }

    return false;
}

void OptionMenu::showAudioControl(){
    float itemWidth = ImGui::GetWindowSize().x * 0.9f;

    ImGui::Text("Master Volume");
    ImGui::SetNextItemWidth(itemWidth);
    int masterVol_ = AudioManager::getInstance()->getMasterVolume() * 100;
    if (ImGui::SliderInt("##masterVol", &masterVol_, 0, 100)) {
        AudioManager::getInstance()->setMasterVolume(masterVol_ * 0.01f);
    }
    ImGui::Text("BGM Volume");
    ImGui::SetNextItemWidth(itemWidth);
    int bgmVol_ = AudioManager::getInstance()->getBGMVolume() * 100;
    if (ImGui::SliderInt("##bgmVol", &bgmVol_, 0, 100)) {
        AudioManager::getInstance()->setBGMVolume(bgmVol_ * 0.01f);
    }
    ImGui::Text("SFX Volume");
    ImGui::SetNextItemWidth(itemWidth);
    int sfxVol_ = AudioManager::getInstance()->getSFXVolume() * 100;
    if (ImGui::SliderInt("##sfxVol", &sfxVol_, 0, 100)) {
        AudioManager::getInstance()->setSFXVolume(sfxVol_ * 0.01f);
    }

}

void OptionMenu::showLightControl(){

    GlobalLight gLight = currentScene->GetLight()->GetLight();
    float itemWidth = ImGui::GetWindowSize().x * 0.9f;
    float sameLineOffset = ImGui::GetWindowSize().x * 0.8f;
    static int lightIndex = 0;
    //For Light 1
    ImGui::Text("Light 1");
    ImGui::SameLine(sameLineOffset);
    showColourPicker(0);
    ImGui::SetNextItemWidth(itemWidth);
    lightPosSlider(0);
    //For Light 2
    ImGui::Text("Light 2");
    ImGui::SameLine(sameLineOffset);
    showColourPicker(1);
    ImGui::SetNextItemWidth(itemWidth);
    lightPosSlider(1);
    //For light 3
    ImGui::Text("Light 3");
    ImGui::SameLine(sameLineOffset);
    showColourPicker(2);
    ImGui::SetNextItemWidth(itemWidth);
    lightPosSlider(2);
}

void OptionMenu::showColourPicker(int lightIndex_){
    //Get the light colour corresponded to the index
    GlobalLight gLight = currentScene->GetLight()->GetLight();
    std::string str = "##diffuse" + std::to_string(lightIndex_ + 1);
    float windowW = width * 0.2f;
    float windowH = height * 0.1f;
    Vec4 diffuse = gLight.diffuse[lightIndex_];
    ImVec4 lightColour(diffuse.x, diffuse.y, diffuse.z, diffuse.w);
    if (ImGui::ColorEdit4(str.c_str(), &lightColour.x, ImGuiColorEditFlags_NoInputs)) {
        //Set the values when there is change in colour picker
        gLight.diffuse[lightIndex_] = Vec4(lightColour.x, lightColour.y, lightColour.z, lightColour.w);
        currentScene->GetLight()->SetLight(gLight);    
    }
}

void OptionMenu::lightPosSlider(int lightIndex_){
    //Get the light position corresponded to the index
    GlobalLight gLight = currentScene->GetLight()->GetLight();
    std::string str = "##lightPos" + std::to_string(lightIndex_ + 1);
    Vec3 pos = gLight.position[lightIndex_];
    float lightPos[3] = { pos.x, pos.y, pos.z };
    if (ImGui::SliderFloat3(str.c_str(), lightPos, -20.0f, 20.0f)) {
        //Set the data if there's chagnes made in sliders
        Vec3 newPos(lightPos[0], lightPos[1], lightPos[2]);
        gLight.position[lightIndex_] = newPos;
        currentScene->GetLight()->SetLight(gLight);
    }
}

void OptionMenu::Update(Scene* scene_){
    gameObjects = renderer->GetObjectList();
    textureList = renderer->GetTextureNames();
    modelList = renderer->GetModelNames();
    shaderList = renderer->GetShaderNames();
    currentScene = dynamic_cast<Scene0*>(scene_);
    width = renderer->getWidth();
    height = renderer->getHeight();

    if (!selectedObject && !gameObjects.empty()) {
        selectedObject = gameObjects.begin()->second;
    }
}
