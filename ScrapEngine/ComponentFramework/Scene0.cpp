#include <glew.h>
#include <iostream>
#include <math.h>
#include "Debug.h"
#include "Scene0.h"
#include "MMath.h"
#include "VulkanRenderer.h"
#include "OpenGLRenderer.h"
#include "OpenGLActor.h"
#include "Camera.h"
#include "TransformComponent.h"
#include "AudioManager.h"
#include "UboStruct.h"
#include "AssetManager.h"
#include "ProfilingTool.h"

Scene0::Scene0(): Scene(){
	camera = std::make_shared<CameraActor>(nullptr);
	light = std::make_shared<LightActor>(nullptr, "Light");
	Debug::Info("Created Scene0: ", __FILE__, __LINE__);
}

Scene0::Scene0(const char* sceneName_): Scene(sceneName_) {
	camera = std::make_shared<CameraActor>(nullptr);
	light = std::make_shared<LightActor>(nullptr, "Light");
	Debug::Info("Created Scene0: ", __FILE__, __LINE__);
}

Scene0::~Scene0() {

}

bool Scene0::OnCreate() {

	//Load assets of the scene through asset manager
	AssetManager::getInstance()->LoadScene(sceneName);

	//float aspectRatio;
	int width, height;

	//Geometry info
	geoInfo_ = { 0.01f, Vec4(0.0f, 0.0f, 1.0f, 1.0f)};

	selectedActor = "Cube";

	//Add camera
	camera->AddComponent<TransformComponent>(nullptr, Vec3(0.0f, 0.0f, -5.0f), Quaternion());
	camera->OnCreate();
	
	//Set lights
	GlobalLight gLight = {};
	gLight.position[0] = Vec4(8.0f, 0.0f, -5.0f, 1.0f);
	gLight.diffuse[0] = Vec4(0.0f, 1.0f, 0.0f, 1.0f);
	gLight.position[1] = Vec4(-8.0f, 0.0f, -5.0f, 1.0f);
	gLight.diffuse[1] = Vec4(0.0f, 0.0f, 0.0f, 1.0f);
	gLight.position[2] = Vec4(0.0f, 8.0f, -5.0f, 1.0f);
	gLight.diffuse[2] = Vec4(0.0f, 0.0f, 0.0f, 1.0f);

	light->SetLight(gLight);

	//Set life time for one of the actor and test
	//renderer->GetActor("Mario")->setLifeTime(true, 2.0f);

	//Set alpha changes for actor
	renderer->GetActor("Mario")->setAlphaChange(0.05f);

	//Visibility test for actors
	//renderer->GetActor("Cube")->setVisibility(false);
	
	//switch (renderer->getRendererType()){
	//case RendererType::VULKAN:
	//	SDL_GetWindowSize(dynamic_cast<VulkanRenderer*>(renderer)->GetWindow(), &width, &height);

	//	break;

	//case RendererType::OPENGL:

	//	break;
	//}
	return true;
}

void Scene0::HandleEvents(const SDL_Event &sdlEvent) {
	if (sdlEvent.type == SDL_WINDOWEVENT) {
		switch (sdlEvent.window.event) {
		case SDL_WINDOWEVENT_SIZE_CHANGED:
		std::cout << "Change size Event\n";
		dynamic_cast<VulkanRenderer*>(renderer)->ResizeEvent();
		//Update projection matrix for camera
		float aspectRatio = static_cast<float>(renderer->getWidth()) / static_cast<float>(renderer->getHeight());
		camera->UpdateProjectionMatrix(45.0f, aspectRatio, 0.5f, 100.0f); 
		break;

		}
	}
	else if (sdlEvent.type == SDL_KEYDOWN) {
		switch (sdlEvent.key.keysym.scancode) {
		case SDL_SCANCODE_C:
			break;
		case SDL_SCANCODE_V:
			break;
		case SDL_SCANCODE_B:
			break;
		}
	}
	else if (sdlEvent.type == SDL_MOUSEWHEEL) {

	}
	else if (sdlEvent.type == SDL_MOUSEBUTTONDOWN) {
		if (sdlEvent.button.button == SDL_BUTTON_LEFT) {
			Vec3 mouseCoords(static_cast<float>(sdlEvent.button.x), static_cast<float>(sdlEvent.button.y), -1.0f);
			Matrix4 ndc = MMath::viewportNDC(renderer->getWidth(), renderer->getHeight());
			Matrix4 projection = camera->GetProjectionMatrix();
			Matrix4 view = camera->GetViewMatrix();
			Matrix4 worldTransform =  MMath::inverse(ndc * projection * view);
			Vec3 worldDir = worldTransform * Vec3(mouseCoords.x, mouseCoords.y, 1.0f);
			worldDir = VMath::normalize(worldDir);
			Vec3 cameraPos = camera->GetComponent<TransformComponent>()->GetPosition();
			Vec3 worldStart(0.0f, 0.0f, 0.0f);

			//Find the angle between two vectors, worldDirection and the vector that the "camera is looking at" (-z axis)
			Vec3 worldOriginDir(0.0f, 0.0f, -1.0f);
			//Use the dot products and the magnitude to find the cosine
			float magOrigin = VMath::mag(worldOriginDir);
			float magDir = VMath::mag(worldDir);
			float cosine = VMath::dot(worldOriginDir, worldDir) / magOrigin * magDir;
			//Calculation the depth into z axis with following information
			int zRand = rand() % 10 + 0;
			float objectZ = -zRand; //the z position that I want the object to be
			float nearPlane = 0.5f;//near plane pos = 0.5f
			float cameraZ = abs(cameraPos.z);
			float depth = abs(objectZ) + abs(nearPlane) + cameraZ;
			//Calculate the length of the direction
			float hypotenuse = depth / cosine;
			float t = hypotenuse;
			Vec3 worldCoords = worldStart + t * worldDir;
			worldCoords.z = objectZ;

			//std::cout << "World Dir ";
			//worldDir.print();

			//std::cout << "World Coords ";
			//worldCoords.print();

			//worldCoords = Vec3(3.5f, 2.0f, 0.0f);
			//Vec3 localCoords = modelMatrix_ * worldCoords;
			Ref<Actor> actorInfo_ = renderer->GetActor(selectedActor);
			Ref<MeshComponent> mesh_ = actorInfo_->GetComponent<MeshComponent>();
			Ref<MaterialComponent> material_ = actorInfo_->GetComponent<MaterialComponent>();
			Ref<ShaderComponent> shader_ = actorInfo_->GetComponent<ShaderComponent>();
			Ref<TransformComponent> transformInfo_ = actorInfo_->GetComponent<TransformComponent>();
			Ref<TransformComponent> transform_ = std::make_shared<TransformComponent>(nullptr, worldCoords, Quaternion(), transformInfo_->GetScale());

			Ref<Actor> newActor;
			std::string startName = "newActor";
			std::string actorName = startName;
			int number = 0;
			char* actorNameC = new char[1024];
			actorName = startName + std::to_string(number);
			//Find a name that does not exist in the actor map
			while (renderer->ContainActor(actorName.c_str())) {
				number++;
				actorName = startName + std::to_string(number);
			}
			strcpy_s(actorNameC, 1024 - 1, actorName.c_str());
			actorNameC[1024 - 1] = '\0';
			switch (renderer->getRendererType()) {
			case RendererType::VULKAN:
				newActor = std::make_shared<VulkanActor>(nullptr, actorNameC);
				break;
			
			case RendererType::OPENGL:
				newActor = std::make_shared<OpenGLActor>(nullptr, actorNameC);
				//renderer->DrawRay(Vec3(worldStart.x, worldStart.y, 0.0f), worldCoords);
				break;
			}
			//Grab components
			newActor->AddComponent(mesh_);
			newActor->AddComponent(material_);
			newActor->AddComponent(shader_);
			newActor->AddComponent(transform_);
			float lifeTime = rand() % 10 + 2;
			newActor->setLifeTime(true, lifeTime);
			float alpha_ = 0.8f;
			newActor->setInitialAlpha(alpha_);
			newActor->OnCreate();

			renderer->AddActor(newActor->GetName(), newActor);

			//Clean up char memory
			delete[] actorNameC;
		}
		
	}
	camera->HandleEvents(sdlEvent);
}

void Scene0::Update(const float deltaTime) {
	Ref<ProfilingTool> update = getTool("Update");
	update->Start();

	static float elapsedTime = 0.0f;
	elapsedTime += deltaTime;

	switch (renderer->getRendererType()) {
	case RendererType::VULKAN:
		break;

	case RendererType::OPENGL:
		std::vector<Ref<LineData>> lineList_ = dynamic_cast<OpenGLRenderer*>(renderer)->getLineList();

		//This is used to update lines lifetime
		for (int i = 0; i < lineList_.size(); i++) {
			lineList_[i]->Update(deltaTime);
		}

		break;
	}

	//camera->Update(deltaTime);
	//Playing with geometry shader normal length
	//if (elapsedTime > 0.5f) {
	//	geoInfo_.length *= 1.5f;
	//	if (geoInfo_.length > 0.015f) geoInfo_.length = 0.002f;
	//	elapsedTime = 0.0f;
	//}

	//Phong shader texture alpha
	//if (elapsedTime > 0.5f) {
	//	geoInfo_.length -= 0.1f;
	//	if (geoInfo_.length < 0.0f) geoInfo_.length = 1.0f;
	//	elapsedTime = 0.0f;
	//	//std::cout << "Time" << elapsedTime << "\n";
	//	//std::cout << geoInfo_.length << "\n";
	//}

	for (auto actor_:renderer->GetObjectList()) {

		//Adjust alpha for the actor
		if (elapsedTime > 0.5f) {
			ModelMatrix actorModelMatrix = actor_.second->GetModelMatrix();
			float alpha = actorModelMatrix.alpha;
			alpha -= actor_.second->getAlphaChange();
			if (alpha < 0.0f) alpha = actor_.second->getInitialAlpha();
			actor_.second->setAlpha(alpha);
			//std::cout << "Time" << elapsedTime << "\n";
			//std::cout << actorModelMatrix.alpha << "\n";
		}

		//If the actor does not have a set life time, jump to next actor
		if (!actor_.second->HasLifeTime()) continue;

		//Remove the actor when its life time reaches 0
		float actor_lifetime = actor_.second->getLifeTime();
		if (actor_lifetime < 0.0f) {
			renderer->RemoveActor(actor_.first);
		}
		actor_lifetime-=deltaTime;
		actor_.second->setLifeTime(true, actor_lifetime);

	}
	if (elapsedTime > 0.5f) {
		elapsedTime = 0.0f;
	}
	update->Stop();
}

void Scene0::Render() const {

	getTool("Render")->Start();

	renderer->SetCameraUBO(camera->GetProjectionMatrix(), camera->GetViewMatrix());
	renderer->SetGLightsUBO(light->GetLight());
	renderer->SetGeometryUBO(geoInfo_);

	renderer->Render();

	getTool("Render")->Stop();

}


void Scene0::OnDestroy() {}
