#ifndef VULKANOBJECT_H
#define VULKANOBJECT_H

#include <SDL.h>
#include <SDL_vulkan.h>
#include <SDL_image.h>
#include <vulkan/vulkan.h>
#include <vector>
#include <array>
#include <unordered_map>

#include "Vector.h"
#include "VMath.h"
#include "MMath.h"
#include "Hash.h"
#include "RenderObject.h"
#include "VulkanMaterial.h"
#include "VulkanMesh.h"
#include "VulkanShader.h"
#include "VulkanActor.h"

//struct ModelMatrixPushConst {
//    Matrix4 normalMatrix;
//    Matrix4 modelMatrix;
//};


//struct VulkanModel {
//
//    ~VulkanModel() {
//        if (vertex) delete vertex;
//        if (index) delete index;
//    }
//
//    bool isValid() {
//        return (vertex) && (index) &&
//               (!vertices.empty()) && (!indices.empty());
//    }
//};


//struct VulkanPipeline {
//    const char* pipelineName;
//    VkPipeline graphicsPipeline;
//    VkPipelineLayout pipelineLayout;
//    VkDescriptorSetLayout descriptorSetLayout;
//
//    bool isValid() {
//        return (graphicsPipeline != 0) &&
//               (pipelineLayout != 0) &&
//               (descriptorSetLayout != 0);
//    }
//};

struct VulkanDescriptor {
    std::vector<VkDescriptorSet> descriptorSets;
    VkDescriptorPool descriptorPool;

    bool isValid() {
        return (descriptorPool != 0) &&
               (!descriptorSets.empty());
    }
};

using TexturePair = std::pair<VulkanDescriptor, VulkanMaterial*>;
using namespace MATH;

class VulkanObject : public RenderObject {
private:

    VulkanMesh* model;
    VulkanShader* pipeline;
    ModelMatrix modelPushConst;
    //Store a map that has const char as key, and pair of descriptorsets and texture as element
    std::unordered_map<const char*, TexturePair*> textures;
    const char* currentTexture;

public:
    VulkanObject(const char* name_);
    VulkanObject(const char* name_, Matrix4 modelMatrix_);
    ~VulkanObject();
    bool OnCreate() override;
    void OnDestroy() override;
    void AddTexture(const char* textureName_, const char* imageName_ = "") override;
    void LoadModel(const char* modelName_, const char* modelFile_ = "") override;
    void LoadShader(const char* shaderName_, const char* veVulkartFile_ = "",
                    const char* fragFile_ = "") override;
    VulkanMesh* getModel() { return model; }
    void setModel(VulkanMesh* model_) { model = model_; }
    ModelMatrix getPushConst() { return modelPushConst; }
    TexturePair* getTexture();
    TexturePair* getTexture(const char* textureName_);
    void setTexture(const char* textureName_, VulkanMaterial** texture_);
    std::unordered_map<const char*, TexturePair*> getTextures() { return textures; }
    VulkanShader* getPipeline() { return pipeline; }
    void setPipeline(VulkanShader* pipeline_) { pipeline = pipeline_; }
    void SetTexture(const char* texture_) override;
    std::vector<const char*> getTextureNames();
    const char* getTextureName() { return currentTexture; }
    void NextTexture();
    void PrevTexture();

    void SetModelMatrix(const Matrix4& modelMatrix_) override;
    Matrix4 GetModelMatrix() override;
        
};


#endif 
