#include <glew.h>
#include "OpenGLShader.h"
#include "Debug.h"
#include <fstream>
#include <string.h>

OpenGLShader::~OpenGLShader()
{
}

bool OpenGLShader::OnCreate() {
	if (isCreated) return isCreated;
	isCreated = true;
	return isCreated;
}

void OpenGLShader::OnDestroy() {
	isCreated = false;
}

void OpenGLShader::Update(const float deltaTime_){

}

void OpenGLShader::Render() const
{
}

GLuint OpenGLShader::GetUniformID(std::string name) {
	auto id = uniformMap.find(name);
#ifdef _DEBUG
	if (id == uniformMap.end()) {
		///Debug::Error(name + " :No such uniform name", __FILE__, __LINE__);
		return -1;
	}
#endif 
	return id->second;
}

