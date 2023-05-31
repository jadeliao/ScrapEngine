#ifndef INPUTMANAGER_H
#define INPUTMANAGER_H

#include <vector>
#include <iostream>

class InputController;
class Scene;

class InputManager{

private:
	std::vector<InputController*> inputList;
	static InputManager* instance_;
	InputManager();
public:
	static InputManager* getInstance();
	~InputManager();
	bool OnCreate();
	void AddInput(InputController* input_);
	void Update(Scene* scene_);
	void Run();

};
#endif
