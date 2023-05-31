#include "InputController.h"
#include <SDL.h>
#include <iostream>

InputController::InputController(){

}

InputController::~InputController(){

}

bool InputController::IsConnected(){

    //SDL_Event sdlEvent;
    //return SDL_PollEvent(&sdlEvent);
	return true;
}

const char* InputController::HandleEvents(){

	SDL_Event sdlEvent;
	//std::cout << SDL_PollEvent(&sdlEvent) << "\n";
	if (!SDL_PollEvent(&sdlEvent)) return "";

	if (sdlEvent.type == SDL_EventType::SDL_QUIT) {
		std::cout << "Quit\n";
		return "QUIT";
	}
	else if (sdlEvent.type == SDL_KEYDOWN) {
		switch (sdlEvent.key.keysym.scancode) {
		case SDL_SCANCODE_ESCAPE:
			std::cout << "escape\n";
			return "ESCAPE";
			//[[fallthrough]]; /// C17 Prevents switch/case fallthrough warnings
		case SDL_SCANCODE_Q:
			std::cout << "Q\n";
			return "Q";

		case SDL_SCANCODE_W:
			std::cout << "W\n";
			return "W";

		default:
			break;
		}
	}
}
