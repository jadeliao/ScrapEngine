#include "XboxController.h"
#include <iostream>
#include <stdio.h>
#include <thread>
#include "Scene.h"
#include "TransformComponent.h"

XBoxController::XBoxController(uint16_t playerID): controllerID(playerID) {
	memset(&controllerState, 0, sizeof(XINPUT_STATE)); // sizeof(controllerState)
}


bool XBoxController::IsConnected() {
	uint16_t result = XInputGetState(controllerID , &controllerState);  //16 bits "DWORD" type
	return (result == ERROR_SUCCESS) ? true : false;

}

XINPUT_STATE XBoxController::GetState() {
	XInputGetState(controllerID, &controllerState);
	return controllerState;
}

void XBoxController::Vibrate(uint16_t leftValue, uint16_t rightValue) {
	XINPUT_VIBRATION vibration{};
	vibration.wLeftMotorSpeed = leftValue;
	vibration.wRightMotorSpeed = rightValue;
	XInputSetState(controllerID, &vibration);
}

void XBoxController::HandleEvents(){

	//std::this_thread::sleep_for(std::chrono::milliseconds(50));
	XINPUT_STATE xinput_state = GetState();
	uint16_t result = xinput_state.Gamepad.wButtons;

	switch (result) {
	case(XINPUT_GAMEPAD_A):
		std::cout << "GAMEPAD_A" << std::endl;
		//player0->Vibrate(XINPUT_GAMEPAD_A, XINPUT_GAMEPAD_A);
		break;

	case(XINPUT_GAMEPAD_B):
		std::cout << "GAMEPAD_A" << std::endl;
		//player0->Vibrate(XINPUT_GAMEPAD_B, XINPUT_GAMEPAD_B);
		break;

	case(XINPUT_GAMEPAD_X):
		std::cout << "GAMEPAD_X" << std::endl;
		//player0->Vibrate(XINPUT_GAMEPAD_X, XINPUT_GAMEPAD_X);
		break;

	case(XINPUT_GAMEPAD_Y):
		std::cout << "GAMEPAD_Y" << std::endl;
		//player0->Vibrate(XINPUT_GAMEPAD_Y, XINPUT_GAMEPAD_Y);
		break;

	case(XINPUT_GAMEPAD_LEFT_THUMB):
		std::cout << "LEFT_THUMB" << std::endl;
		break;

	case(XINPUT_GAMEPAD_RIGHT_THUMB):
		std::cout << "RIGHT_THUMB" << std::endl;
		break;

	case(XINPUT_GAMEPAD_LEFT_SHOULDER):
		std::cout << "LEFT_SHOULDER" << std::endl;
		break;

	case(XINPUT_GAMEPAD_RIGHT_SHOULDER):
		std::cout << "RIGHT_SHOULDER" << std::endl;

		break;

	case(XINPUT_GAMEPAD_DPAD_UP):
		std::cout << "DPAD_UP" << std::endl;

		break;

	case(XINPUT_GAMEPAD_DPAD_DOWN):
		std::cout << "DPAD_DOWN" << std::endl;

		break;

	case(XINPUT_GAMEPAD_DPAD_LEFT):
		std::cout << "DPAD_LEFT" << std::endl;

		break;

	case(XINPUT_GAMEPAD_DPAD_RIGHT):
		std::cout << "DPAD_RIGHT" << std::endl;

		break;

	case(XINPUT_GAMEPAD_START):
		std::cout << "START" << std::endl;
		//player0->Vibrate(0x0, 0xFFFF);

		break;

	case(XINPUT_GAMEPAD_BACK):
		std::cout << "BACK" << std::endl;
		//player0->Vibrate(0x0, 0x0);

		break;

	default:
		//std::cout << "NO INPUT" << std::endl;
		break;
	}

	//Trigger Input
	if (xinput_state.Gamepad.bLeftTrigger) {
		uint8_t leftTrigger = xinput_state.Gamepad.bLeftTrigger;
		printf("left trigger: %d 0x%X\n", leftTrigger, leftTrigger);
	}
	if (xinput_state.Gamepad.bRightTrigger) {
		uint8_t rightTrigger = xinput_state.Gamepad.bRightTrigger;
		printf("right trigger: %d 0x%X\n", rightTrigger, rightTrigger);
	}

	int16_t xInput = 0.0f;
	int16_t yInput = 0.0f;

	if (xinput_state.Gamepad.sThumbLX > 0 || xinput_state.Gamepad.sThumbLY > 0) {
		xInput = xinput_state.Gamepad.sThumbLX;
		yInput = xinput_state.Gamepad.sThumbLY;
		//std::cout << "Left Thumb" << std::endl;
		//(xInput > 0.0f) ? xAxis = 1.0f : xAxis = -1.0f;
		//(yInput > 0.0f) ? yAxis = 1.0f : yAxis = -1.0f;

		float mag = (abs(xInput) * abs(xInput) + abs(yInput) * abs(yInput)) / 32767;

		//Clip the mag 
		if (mag > 32767.0f) mag = 32767.0f;
		const float validRange = 200.0f;
		//If the thumb is being pushed 200 away from the origin, the input is being detected
		if (mag > validRange) {
			int16_t xAxis = xInput / mag;
			int16_t yAxis = yInput / mag;
			Ref<CameraActor> camera = scene->GetCamera();
			Ref<TransformComponent> cameraTransform = camera->GetComponent<TransformComponent>();

			Vec3 axisMove =Vec3(xAxis, yAxis, 0.0f);
			if (VMath::mag(axisMove) > VERY_SMALL) {
				cameraTransform->SetTransform(cameraTransform->GetPosition() + 0.1f * VMath::normalize(axisMove), cameraTransform->GetQuaternion());
				camera->UpdateViewMatrix();

			}
			printf("Left thumb x:%d y:%d\n", xAxis, yAxis);

		}
	}

	if (xinput_state.Gamepad.sThumbRX > 0 || xinput_state.Gamepad.sThumbRY > 0) {
		xInput = xinput_state.Gamepad.sThumbRX;
		yInput = xinput_state.Gamepad.sThumbRY;


		float mag = (xInput * xInput + yInput * yInput) / 32767;

		//Clip the mag 
		if (mag > 32767) mag = 32767.0f;
		const float validRange = 200.0f;
		//If the thumb is being pushed 200 away from the origin, the input is being detected
		if (mag > validRange) {
			int16_t xAxis = xInput / mag;
			int16_t yAxis = yInput / mag;
			Ref<CameraActor> camera = scene->GetCamera();
			Ref<TransformComponent> cameraTransform = camera->GetComponent<TransformComponent>();

			Vec3 axisMove = Vec3(0.0f, 0.0f, 0.0f);
			if (yAxis > VERY_SMALL) {
				axisMove = Vec3(0.0f, 0.0f, -1.0f);
			}
			else if (yAxis < VERY_SMALL) {
				axisMove = Vec3(0.0f, 0.0f, 1.0f);
			}
			cameraTransform->SetTransform(cameraTransform->GetPosition() + 0.1f * axisMove, cameraTransform->GetQuaternion());
			camera->UpdateViewMatrix();

			printf("Right thumb x:%d y:%d\n", xAxis, yAxis);


		}
	}


}

void XBoxController::Update(Scene* scene_) {
	scene = scene_;
}