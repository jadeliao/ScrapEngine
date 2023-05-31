#ifndef COMPONENT_H
#define COMPONENT_H
#include <memory>

template<typename T>
using Ref = std::shared_ptr<T>;

union SDL_Event;

class Component {
	public:
		Component():parent(nullptr), isCreated(false), componentName("") {}
		Component(Component* parent_, const char* componentName_):parent(parent_), isCreated(false), componentName(componentName_) {}
		virtual ~Component() = default;
		virtual bool OnCreate() = 0;
		virtual void OnDestroy() = 0;
		virtual void Update(const float deltaTime_) = 0;
		virtual void Render()const = 0;
		virtual const char* GetName() { return componentName; }
		void SetName(const char* componentName_) { componentName = componentName_; }
		void SetParent(Component* parent_) { parent = parent_; }

	protected:
		const char* componentName;
		Component* parent;
		bool isCreated;
		Component* GetParent() const { return parent; }
};

#endif