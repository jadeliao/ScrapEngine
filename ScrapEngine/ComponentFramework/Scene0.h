#ifndef SCENE0_H
#define SCENE0_H
#include "Scene.h"
#include "Matrix.h"
#include "Actor.h"

using namespace MATH;

/// Forward declarations 
union SDL_Event;
class Scene0 : public Scene {
private:

	GeometryInfo geoInfo_;
	const char* selectedActor;

public:
	explicit Scene0();
	explicit Scene0(const char* sceneName_);
	virtual ~Scene0();

	virtual bool OnCreate() override;
	virtual void OnDestroy() override;
	virtual void Update(const float deltaTime) override;
	virtual void Render() const override;
	virtual void HandleEvents(const SDL_Event &sdlEvent) override;


};


#endif // SCENE0_H