#include "InputManager.h"
#include "InputController.h"
#include "XBoxController.h"
#include <thread>

InputManager* InputManager::instance_(nullptr);

InputManager::InputManager(){
}



InputManager::~InputManager(){
	for (InputController* input : inputList) {
		if (input) delete input;
	}
}

InputManager* InputManager::getInstance() {
	if (!instance_) {
		instance_ = new InputManager();
	}

	return instance_;
}


bool InputManager::OnCreate() {
	XBoxController* XBoxInput = new XBoxController(0);
	AddInput(XBoxInput);

	return true;
}

void InputManager::AddInput(InputController* input_){
	inputList.push_back(input_);

}

void InputManager::Update(Scene* scene_) {
	for (InputController* input : inputList) {
		input->Update(scene_);
	}
}


void InputManager::Run(){

	if (inputList.empty()) return;

	//while (true) {
		for (InputController* input: inputList) {
			//std::this_thread::sleep_for(std::chrono::milliseconds(50));
			if (input->IsConnected()) {
				//std::thread inputThread(&InputController::HandleEvents, input);
				//inputThread.detach();
				input->HandleEvents();
			}
		}
	//}

}
