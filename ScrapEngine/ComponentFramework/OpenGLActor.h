#ifndef OPENGLACTOR_H
#define OPENGLACTOR_H
#include "Actor.h"
class OpenGLActor : public Actor{

private:

public:
	OpenGLActor(Component* parent_, const char* actorName_);
	~OpenGLActor();
	bool OnCreate() override;
	void OnDestroy() override;
	void Update(const float deltaTime_)override;
	void Render() const override;
	void HandleEvents(const SDL_Event& sdlEvent) override;
	void ChangeTexture(Ref<MaterialComponent> texture_) override;
	void ChangeShader(Ref<ShaderComponent> shader_) override;

};

#endif
