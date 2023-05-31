#ifndef OPENSHADER_H
#define OPENSHADER_H

#include "ShaderComponent.h"
#include <unordered_map>
#include <string>

typedef unsigned int GLuint;

class OpenGLShader : public ShaderComponent{

private:
	unsigned int shaderID;
	unsigned int vertShaderID;
	unsigned int fragShaderID;
	std::unordered_map<std::string, GLuint > uniformMap;

public:
	OpenGLShader(Component* parent_, const char* componentName_, const char* vsFilename_, const char* fsFilename_) :
		ShaderComponent(parent_, componentName_, vsFilename_, fsFilename_, ""), shaderID(0), vertShaderID(0), fragShaderID(0) {}
	OpenGLShader(Component* parent_, const char* componentName_, const char* vsFilename_, const char* fsFilename_, const char* gsFilename_) :
		ShaderComponent(parent_, componentName_, vsFilename_, fsFilename_, gsFilename_), shaderID(0), vertShaderID(0), fragShaderID(0) {}
	virtual ~OpenGLShader();
	virtual bool OnCreate();
	virtual void OnDestroy();
	virtual void Update(const float deltaTime_);
	virtual void Render()const;
	inline GLuint GetProgram() const { return shaderID; }
	GLuint GetUniformID(std::string name);
	void AddUniformID(std::string name_, GLuint loc_) { uniformMap[name_] = loc_; }

	void setShaderID(unsigned int shaderID_) { shaderID = shaderID_; }
	void setVertShaderID(unsigned int vShaderID_) { vertShaderID = vShaderID_; }
	void setFragShaderID(unsigned int fShaderID_) { fragShaderID = fShaderID_; }

	unsigned int getShaderID() { return shaderID; }
	unsigned int getVertShaderID() { return vertShaderID; }
	unsigned int getFragShaderID() { return fragShaderID; }
};

#endif

