#pragma once
#include "Component.h"
#include <unordered_map>

class ShaderComponent: public Component {
	ShaderComponent(const ShaderComponent&) = delete;
	ShaderComponent(ShaderComponent&&) = delete;
	ShaderComponent& operator = (const ShaderComponent&) = delete;
	ShaderComponent& operator = (ShaderComponent&&) = delete;

protected:
	const char* vsFilename;
	const char* fsFilename;
	const char* gsFilename;

public:
	ShaderComponent(Component* parent_, const char* componentName_, const char* vsFilename_, const char* fsFilename_):
		Component(parent_, componentName_), vsFilename(vsFilename_), fsFilename(fsFilename_), gsFilename("") {}
	ShaderComponent(Component* parent_, const char* componentName_, const char* vsFilename_, const char* fsFilename_, const char* gsFilename_):
		Component(parent_, componentName_), vsFilename(vsFilename_), fsFilename(fsFilename_), gsFilename(gsFilename_) {}
	virtual ~ShaderComponent() {
		//delete vsFilename;
		//delete fsFilename;
		//delete gsFilename;
	}
	virtual bool OnCreate() = 0;
	virtual void OnDestroy() = 0;
	virtual void Update(const float deltaTime_) = 0;
	virtual void Render()const = 0;
	virtual const char* getVsFile() { return vsFilename; }
	virtual const char* getFsFile() { return fsFilename; }
	virtual const char* getGsFile() { return gsFilename; }
};


