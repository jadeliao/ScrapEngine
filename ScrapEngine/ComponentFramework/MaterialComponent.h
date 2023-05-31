#ifndef MATERIALCOMPONENE_H
#define MATERIALCOMPONENE_H

#include "Component.h"

//Texture
class MaterialComponent: public Component {
	protected:
		const char* filename;

	public:
		MaterialComponent() :Component(), filename(nullptr) {}
		MaterialComponent(Component* parent_, const char* componentName_, const char* filename_):Component(parent_, componentName_), filename(filename_) {}
		virtual ~MaterialComponent() = default;	
		virtual bool OnCreate() = 0;
		virtual void OnDestroy() = 0;
		virtual void Update(const float deltaTime_) = 0;
		virtual void Render()const = 0;
		virtual const char* getFile() { return filename; }
		virtual void setFile(const char* fileName_) { filename = fileName_; }
};

#endif
