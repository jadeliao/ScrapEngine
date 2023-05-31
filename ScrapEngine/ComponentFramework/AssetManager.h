#ifndef ASSETMANAGER_H
#define ASSETMANAGER_H

#include "Debug.h"
#include <unordered_map>
#include <iostream>
#include <string>
#include "Actor.h"
#include "tinyxml2.h"

using namespace tinyxml2;

class Renderer;

class AssetManager{
private:
	static AssetManager* instance_;
	Renderer* renderer;
	XMLDocument assetDoc;
	XMLDocument sceneDoc;
	std::unordered_map<const char*, Ref<Actor>> actorList;
	std::unordered_map<const char*, Ref<Component>> componentCatalog;

	XMLElement* ReadManiFest(const char* fileName_);
	AssetManager();
	void AddComponentData(XMLElement* componentData);
	void AddActorData(XMLElement* actorData);

public:
	static AssetManager* getInstance();

	~AssetManager();
	bool OnCreate();
	void LoadAssets(const char* fileName_);
	void LoadScene(const char* sceneName_);
	void OnDestroy();
	std::unordered_map<const char*, Ref<Actor>> getActors() { return actorList; }

	void RemoveAllComponents();

	template<typename ActorTemplate>
	void AddActor(const char* name_, Ref<ActorTemplate> actor_) {
		if (std::dynamic_pointer_cast<Actor>(actor_)) {
			//Need to check memory for the string
			std::string* temp_name = new std::string();
			*temp_name = name_;
			actorList[temp_name->c_str()] = actor_;
		}
		else {
#ifdef _DEBUG
			std::cerr << "WARNING: Trying to add a component type to actor list - ignored\n";
#endif
		}
	}

	template<typename ComponentTemplate, typename ... Args>
	void AddComponent(const char* name_, Args&& ... args_) {
		Ref<ComponentTemplate> t = std::make_shared<ComponentTemplate>(std::forward<Args>(args_)...);
		componentCatalog[name_] = t;
	}

	template<typename ComponentTemplate, typename ... Args>
	void AddComponent(const char* name_, Ref<ComponentTemplate> component_) {
		componentCatalog[name_] = component_;
	}

	template<typename ComponentTemplate>
	Ref<ComponentTemplate> GetComponent(const char* name_) const {
		Ref<Component> id;
		//Find only compares the address, therefore need to use the actual value to compare
		for (std::pair<const char*, Ref<Component>> c : componentCatalog) {
			std::string key = c.first;
			std::string givenkey = name_;
			if (key == givenkey) {
				id = c.second;
			}
		}
#ifdef _DEBUG
		if (!id) {
			Debug::Error("Cannot find component", __FILE__, __LINE__);
			return Ref<ComponentTemplate>(nullptr);
		}
#endif
		return std::dynamic_pointer_cast<ComponentTemplate>(id);
	}

};

#endif