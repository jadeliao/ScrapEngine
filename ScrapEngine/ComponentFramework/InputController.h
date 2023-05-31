#ifndef INPUTCONTROLLER_H
#define INPUTCONTROLLER_H

class Scene;

class InputController {
protected:
	Scene* scene;
public:
	InputController():scene(nullptr) {}
	virtual ~InputController() = default;
	virtual void Update(Scene* scene_) = 0;
	virtual bool IsConnected() = 0;
	virtual void HandleEvents() = 0;

};

#endif

