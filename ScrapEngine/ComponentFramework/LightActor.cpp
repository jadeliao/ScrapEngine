#include "LightActor.h"

LightActor::LightActor(Component* parent_, LightStyle lightStyle_, Vec3 location_, Vec4  colour_,
	float intensity_, Vec3 fallOff_ ): Actor(parent_){
	lightStyle = lightStyle_;
	position = location_;
	colour = colour_;
	intensity = intensity_;
	fallOff = fallOff_;
}
LightActor::~LightActor() {
	OnDestroy();
}

bool LightActor::OnCreate(){
	if (isCreated) 	return isCreated; //or return true


	isCreated = true;
	return isCreated;
}

void LightActor::OnDestroy() {
	//glDeleteBuffers(1, &uboLightDataID);
	isCreated = false;
}

void LightActor::Render() const{
	//glBindBuffer(GL_UNIFORM_BUFFER, GetLightDataID());
}
