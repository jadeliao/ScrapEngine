#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(std140, binding = 1) uniform LightData{
    vec4 position[3];
    vec4 colour[3];
};

uniform sampler2D myTexture;

layout(location = 0) out vec4 fragColor;

layout(location = 0) in vec3 vertNormal;
layout(location = 1) in vec3 eyeDir; 
layout(location = 2) in vec2 texCoord;
layout(location = 3) in float textAlpha;
layout(location = 4) in vec3 lightDir[3];

void main() {
	fragColor = vec4(1.0, 0.0, 0.0, 1.0);

}