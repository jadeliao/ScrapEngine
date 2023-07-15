#ifndef OPENGLMESH_H
#define OPENGLMESH_H

#include "MeshComponent.h"
#include <glew.h>
#include <vector>
#include "Vector.h"

typedef unsigned int GLenum;
typedef unsigned int GLuint;

using namespace MATH;

class OpenGLMesh : public MeshComponent{
private:
	std::vector<Vec3> vertices;
	std::vector<Vec3> normals;
	std::vector<Vec2> uvCoords;
	size_t dataLength;
	GLenum drawmode;

	GLuint vao, vbo;

public:

	OpenGLMesh(Component* parent_, const char* componentName_, const char* filename_) :
		MeshComponent(parent_, componentName_, filename_), vao(0), vbo(0), dataLength(0) {}
	virtual ~OpenGLMesh();
	virtual bool OnCreate();
	virtual void OnDestroy();
	virtual void Update(const float deltaTime_);
	virtual void Render()const;

	void clear() {
		vertices.clear();
		normals.clear();
		uvCoords.clear();
	}

	void setDataLength(size_t length_) { dataLength = length_; }
	size_t getDataLength() { return dataLength; }

	void addVertex(Vec3 vertex_) { vertices.push_back(vertex_); }
	void addNormal(Vec3 vertex_) { normals.push_back(vertex_); }
	void addUvCoord(Vec2 vertex_) { uvCoords.push_back(vertex_); }
	std::vector<Vec3> getVertices() { return vertices; }
	std::vector<Vec3> getNormals() { return normals; }
	std::vector<Vec2> getUvCoords() { return uvCoords; }

	void setDrawMode(GLenum drawmode_) { drawmode = drawmode_; }
	void setVao(GLuint vao_) { vao = vao_; }
	void setVbo(GLuint vbo_) { vbo = vbo_; }
	GLenum* getDrawMode() { return &drawmode; }
	GLuint* getVao() { return &vao; }
	GLuint* getVbo() { return &vbo; }
};

#endif
