#ifndef SCENE1_H
#define SCENE1_H

#include "Scene.h"
#include "Matrix.h"
#include "CameraActor.h"
#include "Actor.h"
#include "UboStruct.h"

union SDL_Event;

class Scene1 : public Scene {
private:
	Ref<CameraActor> camera;
	GlobalLight glight;
	const char* playerName;

public:
	explicit Scene1();
	virtual ~Scene1();

	virtual bool OnCreate() override;
	virtual void OnDestroy() override;
	virtual void HandleEvents(const SDL_Event &sdlEvent) override;
	virtual void Update(const float deltaTime) override;
	virtual void Render() const override;
};


#endif // SCENE1_H