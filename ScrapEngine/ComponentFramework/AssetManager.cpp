#include "AssetManager.h"
#include "Renderer.h"
#include <fstream>
#include "MeshComponent.h"
#include "ShaderComponent.h"
#include "MaterialComponent.h"
#include "TransformComponent.h"
#include "VulkanActor.h"
#include "OpenGLActor.h"
#include <mutex>

#define BUFFER_SIZE 128

AssetManager* AssetManager::instance_(nullptr);

AssetManager::AssetManager() {
	//ReadManiFest();
	//OnCreate();
	renderer = renderer->Renderer::getInstance();
}


AssetManager* AssetManager::getInstance(){
	if (!instance_) {
		instance_ = new AssetManager();
	}

	return instance_;
}


AssetManager::~AssetManager(){
	OnDestroy();
}


void AssetManager::LoadAssets(const char* fileName_) {
	XMLElement* fileData = ReadManiFest(fileName_);
	if (!fileData) {
		Debug::FatalError("Fail to Load Asset " + std::string(fileName_), __FILE__, __LINE__);
		return;
	}
	//Add Components
	XMLElement* componentData = fileData->FirstChildElement("Component");
	AddComponentData(componentData);
	Debug::Info("Succeed to Load Asset " + std::string(fileName_), __FILE__, __LINE__);

}

void AssetManager::LoadScene(const char* sceneName_) {

	//Read file
	char fileName[BUFFER_SIZE];
	strcpy_s(fileName, sceneName_);
	strcat_s(fileName, ".xml");
	sceneDoc.LoadFile(fileName);
	bool status = sceneDoc.Error();
	if (status) {
		std::cout << sceneDoc.ErrorIDToName(sceneDoc.ErrorID()) << std::endl;
	}
	//Loop through the document to read data
	XMLElement* rootData = sceneDoc.RootElement();
	XMLElement* fileData = rootData->FirstChildElement(sceneName_);

	if (!fileData) {
		Debug::Info("Fail to Load Scene " + std::string(sceneName_), __FILE__, __LINE__);
		return;
	}
	//Add light and camera reading in future (Maybe)
	//Add Actor to scene
	XMLElement* actorData = fileData->FirstChildElement("Actor");
	AddActorData(actorData);

	Debug::Info("Succeed to Load Scene " + std::string(sceneName_), __FILE__, __LINE__);

}

void AssetManager::OnDestroy() {
}

XMLElement* AssetManager::ReadManiFest(const char* fileName_) {
	//Read document with the given name
	char fileName[BUFFER_SIZE];
	strcpy_s(fileName, fileName_);
	strcat_s(fileName, ".xml");
	assetDoc.LoadFile(fileName);
	bool status = assetDoc.Error();
	if (status) {
		std::cout << assetDoc.ErrorIDToName(assetDoc.ErrorID()) << std::endl;
		return nullptr;
	}
	//Loop through the document to read data
	XMLElement* rootData = assetDoc.RootElement();
	XMLElement* fileData = rootData->FirstChildElement(fileName_);
	//While exists a component data
	//XMLElement* componentData = fileData->FirstChildElement("Component");
	//AddComponentData(componentData);
	//Loop through actor data to load
	//XMLElement* actorData = fileData->FirstChildElement("Actor");
	//AddActorData(actorData);

	return fileData;
}

void AssetManager::AddComponentData(XMLElement* componentData) {

	//Get First child element and check if it exists;
	XMLElement* componentFirstElement = componentData->FirstChildElement();
	while (componentFirstElement) {

		if (!componentFirstElement) {
			std::cerr << "No First Element \n";
		}
		else {
			//Get file name and add components		
			const char* componentType = componentFirstElement->Value();
			const char* componentName = componentFirstElement->FindAttribute("name")->Value();
			//Create component accordingly and add to asset manager
			if (strcmp(componentType, "Mesh") == 0) {
				const XMLAttribute* filenameAttribute = componentFirstElement->FindAttribute("filename");
				if (!filenameAttribute) {
					std::cerr << "No filename Attribute \n";
				}
				else {
					const char* filename = componentFirstElement->FindAttribute("filename")->Value();
					renderer->LoadModel(componentName, filename);
					//AddComponent<MeshComponent>(componentName, nullptr, filename);
				}
			}
			else if (strcmp(componentType, "Material") == 0) {
				const XMLAttribute* filenameAttribute = componentFirstElement->FindAttribute("filename");
				if (!componentFirstElement->FindAttribute("filename")) {
					std::cerr << "No filename Attribute \n";
				}
				else {
					const char* filename = componentFirstElement->FindAttribute("filename")->Value();
					renderer->LoadTexture(componentName, filename);
					//AddComponent<MaterialComponent>(componentName, nullptr, filename);
				}
			}
			else if (strcmp(componentType, "Shader") == 0) {
				const XMLAttribute* shaderTypeAttribute = componentFirstElement->FindAttribute("type");
				const XMLAttribute* vertFilenameAttribute = componentFirstElement->FindAttribute("vertFilename");
				const XMLAttribute* fragFilenameAttribute = componentFirstElement->FindAttribute("fragFilename");
				const XMLAttribute* geomFilenameAttribute = componentFirstElement->FindAttribute("geomFilename");


				if ((!shaderTypeAttribute) || (!vertFilenameAttribute) || (!fragFilenameAttribute)) {
					std::cerr << "Shader loading error \n";
				}
				else {
					//Get shader type
					const char* shaderType = shaderTypeAttribute->Value();

		
					const char* vertFile = vertFilenameAttribute->Value();
					const char* fragFile = fragFilenameAttribute->Value();
					//Get the file names
					const char* geomFile;
					if (geomFilenameAttribute) {
						geomFile = geomFilenameAttribute->Value();
					}
					else {
						geomFile = "";
					}
					//Add valid file extension for each renderer type
					//Only load the shader when the shader type matches with the renderer type
					switch (renderer->getRendererType()) {
					case RendererType::OPENGL:
						if (strcmp(shaderType, "OpenGL") == 0) {
							renderer->LoadShader(componentName, vertFile, fragFile);
						}

						break;
					case RendererType::VULKAN:

						if (strcmp(shaderType, "Vulkan") == 0) {
							renderer->LoadShader(componentName, vertFile, fragFile, geomFile);
							break;
						}

					}

				}
			}
		}
		componentFirstElement = componentFirstElement->NextSiblingElement();
	}
}

void AssetManager::AddActorData(XMLElement* actorData) {
	renderer = Renderer::getInstance();
	while (actorData) {
		//Get First child element and check if it exists;
		XMLElement* componentParentElement = actorData->FirstChildElement("Parent");
		XMLElement* componentMeshElement = actorData->FirstChildElement("Mesh");
		XMLElement* componentMaterialElement = actorData->FirstChildElement("Material");
		XMLElement* componentShaderElement = actorData->FirstChildElement("Shader");
		XMLElement* componentTransformElement = actorData->FirstChildElement("Transform");
		XMLElement* componentBodyElement = actorData->FirstChildElement("Body");
		XMLElement* componentAIElement = actorData->FirstChildElement("AI");
		if (!componentMeshElement || !componentMaterialElement || !componentShaderElement) {
			std::cerr << "Component Element not found\n";
		}
		else {

			Ref<Actor> newActor;
			//Get Actor name
			const char* actorName = actorData->FindAttribute("name")->Value();
			//Check actor data
			std::string parentName = componentParentElement->FindAttribute("componentName")->Value();
			
			switch (renderer->getRendererType()) {
			case RendererType::VULKAN:
				if (parentName == "None") {
					newActor = std::make_shared<VulkanActor>(nullptr, actorName);
				}
				else {
					newActor = std::make_shared<VulkanActor>(GetComponent<Actor>(parentName.c_str()).get(), actorName);
				}

				break;
			case RendererType::OPENGL:
				if (parentName == "None") {
					newActor = std::make_shared<OpenGLActor>(nullptr, actorName);
				}
				else {
					newActor = std::make_shared<OpenGLActor>(GetComponent<Actor>(parentName.c_str()).get(), actorName);
				}
				break;
			}

			//Get components to add to actor
			const char* meshName = componentMeshElement->FindAttribute("componentName")->Value();
			const char* materialName = componentMaterialElement->FindAttribute("componentName")->Value();
			const char* shaderName = componentShaderElement->FindAttribute("componentName")->Value();
			Ref<MeshComponent> actorMesh = renderer->GetModel(meshName);
			Ref<MaterialComponent> actorMaterial = renderer->GetTexture(materialName);
			Ref<ShaderComponent> actorShader = renderer->GetShader(shaderName);
			//Get data for transform
			Ref<TransformComponent> transform = std::make_shared<TransformComponent>(newActor.get());
			//Position data
			float posX = componentTransformElement->FloatAttribute("posX");
			float posY = componentTransformElement->FloatAttribute("posY");
			float posZ = componentTransformElement->FloatAttribute("posZ");
			//Orientation data
			float angle = componentTransformElement->FloatAttribute("angle");
			float axisX = componentTransformElement->FloatAttribute("axisX");
			float axisY = componentTransformElement->FloatAttribute("axisY");
			float axisZ = componentTransformElement->FloatAttribute("axisZ");
			Quaternion orientation = Quaternion();
			if (VMath::mag(Vec3(axisX, axisY, axisZ)) > VERY_SMALL) {
				orientation = QMath::angleAxisRotation(angle, Vec3(axisX, axisY, axisZ));
			}
			//Scale data
			float scaleX = componentTransformElement->FloatAttribute("scaleX");
			float scaleY = componentTransformElement->FloatAttribute("scaleY");
			float scaleZ = componentTransformElement->FloatAttribute("scaleZ");
			if (scaleX < VERY_SMALL) {
				scaleX = 0.0f;
			}
			if (scaleY < VERY_SMALL) {
				scaleY = 0.0f;
			}
			if (scaleZ < VERY_SMALL) {
				scaleZ = 0.0f;
			}
			// Set transform component
			transform->SetTransform(Vec3(posX, posY, posZ), orientation, Vec3(scaleX, scaleY, scaleZ));

			//Add all components to actor
			newActor->AddComponent<MeshComponent>(actorMesh);
			newActor->AddComponent<MaterialComponent>(actorMaterial);
			newActor->AddComponent<ShaderComponent>(actorShader);
			newActor->AddComponent<TransformComponent>(transform);
			newActor->OnCreate();
			renderer->AddActor(actorName, newActor);
		}
		actorData = actorData->NextSiblingElement("Actor");
	}
}

bool AssetManager::OnCreate(){
	for (std::pair<const char*, Ref<Component>> c : componentCatalog) {
		if (c.second->OnCreate() == false) {
			return false;
		}
	}
	return true;
}



void AssetManager::RemoveAllComponents(){

	componentCatalog.clear();
	actorList.clear();
}
