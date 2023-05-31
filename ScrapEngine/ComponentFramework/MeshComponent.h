#ifndef MESHCOMPONENT_H
#define MESHCOMPONENT_H

#include "Component.h"

class MeshComponent : public Component {
	MeshComponent(const MeshComponent&) = delete;
	MeshComponent(MeshComponent&&) = delete;
	MeshComponent& operator = (const MeshComponent&) = delete;
	MeshComponent& operator = (MeshComponent&&) = delete;

protected:
	const char* filename;

public:
	
	MeshComponent(Component* parent_, const char* componentName_, const char* filename_) : Component(parent_, componentName_), filename(filename_){}
	virtual ~MeshComponent() = default;
	virtual bool OnCreate() = 0;
	virtual void OnDestroy() = 0;
	virtual void Update(const float deltaTime_) = 0;
	virtual void Render()const = 0;
	virtual const char* getFile() { return filename; }
	virtual void setFile(const char* fileName_) { filename = fileName_; }

};

#endif