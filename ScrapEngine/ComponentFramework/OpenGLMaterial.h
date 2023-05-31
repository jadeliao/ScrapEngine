#ifndef OPENGLMATERIAL_H
#define OPENGLMATERIAL_H

#include "MaterialComponent.h"

typedef unsigned int GLuint;

class OpenGLMaterial : public MaterialComponent{

private:
	GLuint textureID;
public:
	OpenGLMaterial(Component* parent_, const char* componentName_, const char* filename_) :MaterialComponent(parent_, componentName_, filename_), textureID(0) {}
	~OpenGLMaterial();
	bool OnCreate();
	void OnDestroy();
	void Update(const float deltaTime_);
	void Render()const;
	void setTextureID(GLuint id_) { textureID = id_; }
	GLuint* getTextureID() { return &textureID; }
};


#endif

