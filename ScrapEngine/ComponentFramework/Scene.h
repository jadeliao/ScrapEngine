#ifndef SCENE_H
#define SCENE_H

#include <unordered_map>
#include <queue>
#include <vector>
#include "Renderer.h"
#include "ProfilingTool.h"
#include "LightActor.h"
#include "CameraActor.h"

union SDL_Event;

class Scene {
public:
	explicit Scene():renderer(Renderer::getInstance()), sceneName("None"){
		//profilingResult = { std::make_shared<ProfilingTool>("Update"), std::make_shared<ProfilingTool>("Render") };
		//Setup profiling tools
		profilingList["Update"] = std::make_shared<ProfilingTool>("Update");
		profilingList["Render"] = std::make_shared<ProfilingTool>("Render");
	};
	explicit Scene(const char* sceneName_) :renderer(Renderer::getInstance()), sceneName(sceneName_) {
		//profilingResult = { std::make_shared<ProfilingTool>("Update"), std::make_shared<ProfilingTool>("Render") };
		//Setup profiling tools
		profilingList["Update"] = std::make_shared<ProfilingTool>("Update");
		profilingList["Render"] = std::make_shared<ProfilingTool>("Render");
	};
	virtual ~Scene() {}

	virtual bool OnCreate() = 0;
	virtual void OnDestroy() = 0;
	virtual void Update(const float deltaTime) = 0;
	virtual void Render() const  = 0 ;
	virtual void HandleEvents(const SDL_Event &sdlEvent) = 0;

	Ref<LightActor> GetLight() { return light; }
	Ref<CameraActor> GetCamera() { return camera; }

	//virtual ProfilingResult getProfiling() {
	//	ProfilingResult temp = profilingList.front();
	//	profilingList.pop();
	//	return temp; 
	//}
	virtual std::unordered_map<const char*, Ref<ProfilingTool>> getProfiling() {return profilingList;}
	virtual Ref<ProfilingTool> getTool(const char* toolName_) const{
		for (auto tool : profilingList) {
			if (strcmp(tool.first, toolName_) == 0) {
				return tool.second;
			}
		}
		return nullptr;
	}


	void AddActor(Ref<Actor> actor_) {
		//Add the object to list if it is added to renderer
		if (renderer->AddActor(actor_->GetName(), actor_)) {
			//gameObjects[actor_->GetName()] = actor_;
		}
		else {
			//Destroy the object when it is not valid
			std::string message = std::string(actor_->GetName()) + " destroying object pointer";
			Debug::FatalError(message, __FILE__, __LINE__);
		}
	}

protected:
	Renderer *renderer;
	Ref<CameraActor> camera;
	Ref<LightActor> light;
	const char* sceneName;
	std::unordered_map<const char*, Ref<ProfilingTool>> profilingList;
};
#endif