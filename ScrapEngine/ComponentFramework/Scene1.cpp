#include <iostream>
#include <glew.h>
#include "Scene1.h"
#include "Camera.h"
#include "MMath.h"
#include "OpenGLActor.h"
#include "TransformComponent.h"

Scene1::Scene1() : Scene(){
	
	camera = std::make_shared<CameraActor>(nullptr);

	Debug::Info("Created Scene1: ", __FILE__, __LINE__);

}

Scene1::~Scene1() {
	
}

bool Scene1::OnCreate() {

	camera->AddComponent<TransformComponent>(nullptr, Vec3(0.0f, 0.0f, -5.0f), Quaternion());
	camera->OnCreate();

	Ref<Actor> object_;
	playerName = "Mario";
	Ref<MeshComponent> mesh_;
	Ref<ShaderComponent> shader_;
	Ref<MaterialComponent> material_;
	Ref<TransformComponent> transform_;

	//Set lights
	glight.position[0] = Vec4(8.0f, 0.0f, -5.0f, 1.0f);
	glight.diffuse[0] = Vec4(0.0f, 1.0f, 0.0f, 1.0f);
	glight.position[1] = Vec4(-8.0f, 0.0f, -5.0f, 1.0f);
	glight.diffuse[1] = Vec4(0.5f, 0.0f, 0.5f, 1.0f);
	glight.position[2] = Vec4(0.0f, 8.0f, -5.0f, 1.0f);
	glight.diffuse[2] = Vec4(0.0f, 0.5f, 0.5f, 1.0f);

	switch (renderer->getRendererType()) {
	case RendererType::VULKAN:
		//SDL_GetWindowSize(dynamic_cast<VulkanRenderer*>(renderer)->GetWindow(), &width, &height);

		break;

	case RendererType::OPENGL:
		////Create Object
		object_ = std::make_shared<OpenGLActor>(nullptr, playerName);
		mesh_ = renderer->GetModel("MarioModel");
		shader_ = renderer->GetShader("PhongShader");
		material_ = renderer->GetTexture("MarioMain");
		transform_ = std::make_shared<TransformComponent>(object_, Vec3(0.0f, 0.0f, 0.0f), QMath::angleAxisRotation(180.0f, Vec3(0.0f, 1.0f, 0.0f)));
		//object_->AddComponent(mesh_);
		//object_->AddComponent(shader_);
		//object_->AddComponent(material_);
		object_->AddComponent(transform_);
		//object_->OnCreate();
		//AddActor(object_);

		////Create Object
		//object_ = std::make_shared<OpenGLActor>(nullptr, "Skull");
		//mesh_ = renderer->GetModel("SkullModel");
		//shader_ = renderer->GetShader("PhongShader");
		//material_ = renderer->GetTexture("SkullNormal");
		//transform_ = std::make_shared<TransformComponent>(object_, Vec3(3.0f, 0.0f, 0.0f), Quaternion(), Vec3(0.3f, 0.3f, 0.3f));
		//object_->AddComponent(mesh_);
		//object_->AddComponent(shader_);
		//object_->AddComponent(material_);
		//object_->AddComponent(transform_);
		//object_->OnCreate();
		//AddActor(object_);
		break;
	}

	return true;
}

void Scene1::OnDestroy() {}

void Scene1::HandleEvents(const SDL_Event &sdlEvent) {}

void Scene1::Update(const float deltaTime) {}

void Scene1::Render() const {
	renderer->Render();
	renderer->SetCameraUBO(camera->GetProjectionMatrix(), camera->GetViewMatrix());
	renderer->SetGLightsUBO(glight);
	switch (renderer->getRendererType()) {
	case RendererType::VULKAN:

		break;

	case RendererType::OPENGL:


		break;
	}

}
