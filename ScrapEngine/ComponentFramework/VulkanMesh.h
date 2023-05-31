#ifndef VULKANMESH_H
#define VULKANMESH_H

#include <SDL.h>
#include <SDL_vulkan.h>
#include <SDL_image.h>
#include <vulkan/vulkan.h>
#include <vector>
#include <array>

#include "Vector.h"
#include "VMath.h"
#include "MMath.h"
#include "Hash.h"
#include "MeshComponent.h"

using namespace MATH;

struct Vertex {
    Vec3 pos;
    Vec2 texCoord;
    Vec3 normal;

    static VkVertexInputBindingDescription getBindingDescription() {
        VkVertexInputBindingDescription bindingDescription{};
        bindingDescription.binding = 0;
        bindingDescription.stride = sizeof(Vertex);
        bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
        return bindingDescription;
    }

    static std::array<VkVertexInputAttributeDescription, 3> getAttributeDescriptions() {
        std::array<VkVertexInputAttributeDescription, 3> attributeDescriptions{};

        attributeDescriptions[0].binding = 0;
        attributeDescriptions[0].location = 0;
        attributeDescriptions[0].format = VK_FORMAT_R32G32B32_SFLOAT;
        attributeDescriptions[0].offset = offsetof(Vertex, pos);

        attributeDescriptions[1].binding = 0;
        attributeDescriptions[1].location = 1;
        attributeDescriptions[1].format = VK_FORMAT_R32G32B32_SFLOAT;
        attributeDescriptions[1].offset = offsetof(Vertex, normal);

        attributeDescriptions[2].binding = 0;
        attributeDescriptions[2].location = 2;
        attributeDescriptions[2].format = VK_FORMAT_R32G32_SFLOAT;
        attributeDescriptions[2].offset = offsetof(Vertex, texCoord);

        return attributeDescriptions;
    }

    bool operator == (const Vertex& other) const {
        return pos == other.pos && normal == other.normal && texCoord == other.texCoord;
    }

}; /// End of struct Vertex

class VulkanMesh : public MeshComponent {
private:
    //std::pair<VkBuffer, VkDeviceMemory>* vertex;
    //std::pair<VkBuffer, VkDeviceMemory>* index;
    //std::vector<Vertex> vertices;
    //std::vector<uint32_t> indices;

public:
    VulkanMesh(Component* parent_, const char* componentName_, const char* filename_);
    virtual ~VulkanMesh();
    virtual bool OnCreate() override;
    virtual void OnDestroy() override;
    virtual void Update(const float deltaTime_) override;
    virtual void Render()const override;

    Ref<std::pair<VkBuffer, VkDeviceMemory>> vertex;
    Ref<std::pair<VkBuffer, VkDeviceMemory>> index;
    std::vector<Vertex> vertices;
    std::vector<uint32_t> indices;

 /*   std::pair<VkBuffer, VkDeviceMemory>* getVertex() { return vertex; }
    std::pair<VkBuffer, VkDeviceMemory>* getIndex() { return index; }
    std::vector<Vertex> getVertices() { return vertices; }
    std::vector<uint32_t> getIndices() { return indices; }*/

    //void setVertex(std::pair<VkBuffer, VkDeviceMemory>* vertex_) { vertex = vertex_; }
    //void setIndex(std::pair<VkBuffer, VkDeviceMemory>* index_) { index = index_; }
    //void setVertices(std::vector<Vertex> vertices_) { vertices = vertices_; }
    //void setIndices(std::vector<uint32_t> indices_) { indices = indices_; }

    //void addVertices(Vertex vertex_) { vertices.push_back(vertex_); }
    //void addIndices(uint32_t index_) { indices.push_back(index_); }



    bool isValid() {
        return (vertex) && (index) &&
                (!vertices.empty()) && (!indices.empty());
    }
};

#endif

