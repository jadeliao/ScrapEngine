#ifndef RENDERER_H
#define RENDERER_H

#include "Debug.h"
#include <SDL.h>
#include <string>
#include <vector>
#include <unordered_map>
#include <memory>
#include "Actor.h"
#include "UboStruct.h"

class UIManager;
class MaterialComponent;
class MeshComponent;
class ShaderComponent;

struct GeometryInfo {
	float length;
	alignas(16) Vec4 colour;
};

struct RendererAssets {
	std::unordered_map<const char*, Ref<MeshComponent>> modelList;
	std::unordered_map<const char*, Ref<MaterialComponent>> textureList;
	std::unordered_map<const char*, Ref<ShaderComponent>> pipelineList;
};

enum class RendererType {
	NONE,
	OPENGL,
	VULKAN,
	DIRECTX11,
	DIRECTX12
};

class Renderer {

protected:
	static Renderer* instance_;
	RendererType rendererType;
	Ref<RendererAssets> assetPool;
	std::unordered_map<const char*, Ref<Actor>> actorList;
	//UIManager* uiManager;
	uint32_t width;
	uint32_t height;
	bool framebufferResized = false;
	//virtual Renderer* initInstance() = 0;
	Renderer() : rendererType(RendererType::NONE), width(0), height(0) {
		assetPool = std::make_shared<RendererAssets>();
	}

public:

	template<class T>
	static Renderer* getInstance() {
		if (!instance_) {
			instance_ = T::Initialize();
		}
		return instance_;
	}

	static Renderer* getInstance() {
		return instance_;
	}

	virtual ~Renderer() = default;
	virtual SDL_Window* BuildWindow(std::string name_, int width_, int height_) = 0;
	virtual bool OnCreate() = 0;
	virtual void OnDestroy() = 0;
	virtual void Render() = 0;
	RendererType getRendererType() { return rendererType; }
	void setRendererType(RendererType rendererType_) { rendererType = rendererType_; }
	virtual void InitImGui() = 0;
	virtual bool AddActor(const char* actorName_, Ref<Actor> actor_) = 0;
	virtual void LoadTexture(const char* textureName_, const char* imageName_) = 0;
	virtual void LoadModel(const char* modelName_, const char* modelFile_) = 0;
	virtual void LoadShader(const char* shaderName_, const char*, const char*, const char* gsFile_ = "") = 0;
	virtual void SetCameraUBO(const Matrix4& projection, const Matrix4& view) = 0;
	virtual void SetGLightsUBO(const GlobalLight& glights) = 0;
	virtual void SetGeometryUBO(const GeometryInfo& geometryInfo_) = 0;
	virtual void DrawRay(Vec3 start, Vec3 end, float lifeTime = 1.0f) = 0;

	uint32_t getWidth() { return width; }
	uint32_t getHeight() { return height; }

	virtual void RemoveActor(const char* actorName_) {
		for (auto actor_ : actorList) {
			if (strcmp(actor_.first, actorName_) == 0) {
				actor_.second->OnDestroy();
				actorList.erase(actorName_);
				break;
			}
		}
	}

	virtual Ref<MaterialComponent> GetTexture(const char* name_) {
		for (auto texture_ : assetPool->textureList) {
			if (strcmp(texture_.first, name_) == 0) {
				return texture_.second;
			}
		}
		//Debug message when fail
		std::string message = std::string(name_) + " -> Texture not found in AssetPool";
		Debug::FatalError(message, __FILE__, __LINE__);
		return nullptr;


	}

	virtual Ref<MeshComponent> GetModel(const char* name_) {

		for (auto model_ : assetPool->modelList) {
			if (strcmp(model_.first, name_) == 0) {
				return model_.second;
			}
		}
		//Debug message when fail
		std::string message = std::string(name_) + " -> Model not found in AssetPool";
		Debug::FatalError(message, __FILE__, __LINE__);
		return nullptr;
	}

	virtual Ref<ShaderComponent> GetShader(const char* name_) {

		for (auto shader_ : assetPool->pipelineList) {
			if (strcmp(shader_.first, name_) == 0) {
				return shader_.second;
			}
		}
		//Debug message when fail
		std::string message = std::string(name_) + " -> Pipeline not found in AssetPool";
		Debug::FatalError(message, __FILE__, __LINE__);
		return nullptr;
	}

	virtual std::vector<const char*> GetTextureNames() {
		std::vector<const char*> textureNames;

		for (auto texture_ : assetPool->textureList) {
			textureNames.push_back(texture_.first);
		}

		return textureNames;
	}
	virtual std::vector<const char*> GetModelNames() {
		std::vector<const char*> modelNames;

		for (auto model_ : assetPool->modelList) {
			modelNames.push_back(model_.first);
		}

		return modelNames;
	}
	virtual std::vector<const char*> GetShaderNames() {
		std::vector<const char*> shaderNames;

		for (auto shader_ : assetPool->pipelineList) {
			shaderNames.push_back(shader_.first);
		}

		return shaderNames;
	}


	bool ContainActor(const char* actorName_) {

		for (auto actorInList_ : actorList) {
			if (strcmp(actorInList_.first, actorName_) == 0) {
				std::string message = "Actor Exist in List: " + std::string(actorName_);
				Debug::FatalError(message, __FILE__, __LINE__);
				return true;
			}
		}
		return false;
	}

	Ref<Actor> GetActor(const char* actorName_) const {

		for (auto actorInList_ : actorList) {
			if (strcmp(actorInList_.first, actorName_) == 0) {
				return actorInList_.second;
			}
		}
		return nullptr;
	}

	void RemoveObject(const char* actorName_) {
		Ref<Actor> object_ = GetActor(actorName_);
		object_->OnDestroy();
		object_ = nullptr;
		///This removes the component from the vector list
		actorList.erase(actorName_);
	}

	std::unordered_map<const char*, Ref<Actor>> GetObjectList() const { return actorList; }

	void RemoveAllObjects() {
		for (auto object : actorList) {
			object.second->OnDestroy();
		}
		actorList.clear();
	}
};


#endif