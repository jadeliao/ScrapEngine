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
	const int lightNum = 3;
	vec4 kd[lightNum] = colour;
	vec4 kt = texture(myTexture,texCoord);
	vec4 ks[lightNum];
	for (int i = 0; i < lightNum; i++){
		ks[i] = kd[i];
	}
	
	vec4 ka = 0.1 * kt;
	vec4 color = ka;

	float diff[lightNum];
	vec3 reflection[lightNum];
	float spec[lightNum];

	for (int i = 0; i < lightNum; i++){
		diff[i] = max(dot(vertNormal, lightDir[i]), 0.0);
		reflection[i] = normalize(reflect(-lightDir[i], vertNormal));
		spec[i] = max(dot(eyeDir, reflection[i]), 0.0);
		spec[i] = pow(spec[i],14.0);

		color += (diff[i] * ka) + (spec[i] * ks[i]);
	}
	fragColor = color * textAlpha;

}