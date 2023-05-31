#include "TextureUI.h"
#include "TransformComponent.h"
#include "Debug.h"
#include "VulkanActor.h"
#include "OpenGLActor.h"
#include "Renderer.h"

TextureUI::TextureUI(): UserInterface(), selectedObject(nullptr), selectedModel("None"), selectedShader("None"), selectedTexture("None") {
}

TextureUI::~TextureUI(){

}

void TextureUI::Display(){
    bool showWindow = true;

   //Set window to display frame
    ImGui::Begin("Frame", &showWindow, ImGuiWindowFlags_NoTitleBar |
        ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoBackground);
    ImGui::SetWindowPos(ImVec2(width - ImGui::GetWindowWidth(), height - 35.0f));
    ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);

    ImGui::End();

    ImGui::ShowDemoWindow();

    showObjectList();
    showRendererAssets();
}


void TextureUI::selectObject(Ref<Actor> object_){
    selectedObject = object_;
}

void TextureUI::showRendererAssets(){
    ImGui::SetNextWindowPos(ImVec2(10.0f, 270.0f), ImGuiCond_FirstUseEver);
    ImGui::SetNextWindowSize(ImVec2(300.0f, 350.0f), ImGuiCond_FirstUseEver);
    bool showWindow = true;
    //Set window to display and select objects
    ImGui::Begin("Assets", &showWindow);

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
        char *actorName_ = (char*)malloc(20);
        strcpy_s(actorName_, sizeof(actorNameInput)-1, actorNameInput);
        actorName_[sizeof(actorNameInput) - 1] = '\0';
        if (!addActor(actorName_, pos_, orientation_, scale_)) {
            Debug::Info("Fail to Add Actor to Scene", __FILE__, __LINE__);
        }
        else {
            Debug::Info("Actor Added to Scene " + std::string(actorName_), __FILE__, __LINE__);
        }
    }
    ImGui::End();
}

void TextureUI::showObjectList() {
    
    ImGui::SetNextWindowPos(ImVec2(10.0f, 10.0f), ImGuiCond_FirstUseEver);
    ImVec2 sizeInitial = ImVec2(300, 250);
    //ImGui::SetNextWindowSize(sizeInitial, ImGuiCond_FirstUseEver);
    ImGui::SetNextWindowSize(ImVec2(300.0f, 250.0f), ImGuiCond_FirstUseEver);

    bool showWindow = true;

    //Set window to display and select objects
    ImGui::Begin("Object List", &showWindow);
    //Set size of the ui window accordingly to the window
    //ImGui::SetWindowSize(sizeInitial, ImGuiCond_FirstUseEver);
    if (resized) {
 /*       ImVec2 currentSize = ImGui::GetWindowSize();
        std::cout << "Before resize " << currentSize.x << " " << currentSize.y << "\n";
        ImGui::SetWindowSize(UIWindowResize(currentSize));
        currentSize = UIWindowResize(currentSize);
        std::cout << "after resize " << currentSize.x << " " << currentSize.y << "\n";
 */       resized = false;
    }

    const char* selectedName = "None";
    if (selectedObject) {
        selectedName = selectedObject->GetName();
    }
    ImGui::Text("Selected Object: %s", selectedName);
    //Set a header to display object list
    if (!gameObjects.empty()) {
        if (ImGui::BeginListBox("##Object List", ImVec2(-FLT_MIN, 5 * ImGui::GetTextLineHeightWithSpacing()))) {
            for (auto object_ : gameObjects) {
                const char* objectName = object_.first;
                const bool isSelected = (selectedName == objectName);

                if (ImGui::Selectable(objectName, isSelected)) {
                    selectedObject = object_.second;
                }

            }
            ImGui::EndListBox();
        }
    }
    if (ImGui::Button("Delete##2")) {
        
        //Remove object from renderer
    }
    //Set a header to display texture list
    //if (ImGui::CollapsingHeader("Texture List")) {
    //    std::vector<const char*> textureNames;
    //    const char* selectedTexture = "No Object";
    //    if (selectedObject) {
    //        textureNames = selectedObject->getTextureNames();
    //        selectedTexture = selectedObject->getTextureName();
    //    }
    //    //Display the list box of the object textures
    //    if (ImGui::BeginListBox("##ObjectTextureList", ImVec2(-FLT_MIN, 5 * ImGui::GetTextLineHeightWithSpacing()))) {

    //        for (const char* textureName_ : textureNames) {
    //            const bool isSelected = (selectedTexture == textureName_);
    //            if (ImGui::Selectable(textureName_, isSelected)) {
    //                selectedObject->SetTexture(textureName_);
    //            }
    //        }
    //        ImGui::EndListBox();
    //    }
    //}

    ImGui::End();
}


bool TextureUI::addActor(const char* actorName_, Vec3 pos_, Quaternion orientation_, Vec3 scale_) {
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

void TextureUI::Update(Scene* scene_) {
    gameObjects = renderer->GetObjectList();
    textureList = renderer->GetTextureNames();
    modelList = renderer->GetModelNames();
    shaderList = renderer->GetShaderNames();
    width = renderer->getWidth();
    height = renderer->getHeight();

    if (!selectedObject && !gameObjects.empty()) {
        selectedObject = gameObjects.begin()->second;
    }
}