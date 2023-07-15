
#include "OpenGLMaterial.h"
#include <glew.h>

OpenGLMaterial::~OpenGLMaterial() {
}

bool OpenGLMaterial::OnCreate(){
	if (isCreated) return isCreated;
	isCreated = true;
	return isCreated;
}

void OpenGLMaterial::OnDestroy(){
	isCreated = false;
}

void OpenGLMaterial::Update(const float deltaTime_){
}

void OpenGLMaterial::Render() const
{

}
