#ifndef XBOXCONTROLLER_H
#define XBOXCONTROLLER_H

#include "InputController.h"

#define WIN32_LEAN_AND_MEAN //leave out microsoft foundation classes
#include <Windows.h>
#include <XInput.h>
#include <cstdint>

enum XBoxKey {
	A = 0,
	Y,
	B,
	X,

};


class XBoxController:public InputController {

private:
	uint16_t controllerID;
	XINPUT_STATE controllerState{};

public:
	XBoxController(uint16_t playerID);
	bool IsConnected() override;
	void HandleEvents() override;
	void Update(Scene* scene_);
	XINPUT_STATE GetState();
	void Vibrate(uint16_t leftValue, uint16_t rightValue);

};

#endif

