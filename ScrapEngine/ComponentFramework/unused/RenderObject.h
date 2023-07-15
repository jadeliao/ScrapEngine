#ifndef RENDEROBJECT_H
#define RENDEROBJECT_H

#include "MMath.h"

using namespace MATH;

class RenderObject {

protected:
	const char* name;

public:
	explicit RenderObject(const char* name_):name(name_){}
	virtual bool OnCreate() = 0;
	virtual void OnDestroy() = 0;
	virtual void AddTexture(const char* textureName_, const char* imageName_ = "") = 0;
	virtual void LoadModel(const char* modelName_, const char* modelFile_ = "") = 0;
	virtual void LoadShader(const char* shaderName_, const char* vertFile_ = "", 
													const char* fragFile_ = "") = 0;
	virtual void SetModelMatrix(const Matrix4& modelMatrix_) = 0;
	virtual Matrix4 GetModelMatrix() = 0;
	virtual void SetTexture(const char* texture_) = 0;
	virtual std::vector<const char*> getTextureNames() = 0;
	virtual const char* getTextureName() = 0;
	const char* getName() { return name; }

};



#endif